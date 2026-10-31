#include <game/objects/PlayerSprite.hpp>
#include <stb_image.h>
#include <cstdio>
#include <vector>

namespace game {

    std::shared_ptr<PlayerSprite> PlayerSprite::create(const std::string& path, int colorId) {
        auto sprite = std::make_shared<PlayerSprite>();
        if (sprite->initWithFileAndColor(path, colorId)) {
            return sprite;
        }
        return nullptr;
    }

    void PlayerSprite::applyPlayerColorMapping(unsigned char* imgData, int width, int height, int colorId) {
        const auto& layers = ColorPalette::getLayers(colorId);

        // Extract RGBA components from target palette layers
        float mR = (float)((layers.bodyMain >> 0) & 0xFF);
        float mG = (float)((layers.bodyMain >> 8) & 0xFF);
        float mB = (float)((layers.bodyMain >> 16) & 0xFF);

        float sR = (float)((layers.bodyShadow >> 0) & 0xFF);
        float sG = (float)((layers.bodyShadow >> 8) & 0xFF);
        float sB = (float)((layers.bodyShadow >> 16) & 0xFF);

        // Visor Base Color (#95c9dc -> R:149, G:201, B:220)
        const float vBaseR = 149.0f;
        const float vBaseG = 201.0f;
        const float vBaseB = 220.0f;

        int totalPixels = width * height;
        for (int i = 0; i < totalPixels; ++i) {
            int idx = i * 4;
            unsigned char r = imgData[idx + 0];
            unsigned char g = imgData[idx + 1];
            unsigned char b = imgData[idx + 2];
            unsigned char a = imgData[idx + 3];

            if (a == 0) continue; // Skip transparent pixels

            // Check for Green mask component (Visor)
            if (g > 100 && r < 50 && b < 50) {
                float intensity = (float)g / 255.0f;
                imgData[idx + 0] = (unsigned char)(vBaseR * intensity);
                imgData[idx + 1] = (unsigned char)(vBaseG * intensity);
                imgData[idx + 2] = (unsigned char)(vBaseB * intensity);
            }
            // Check for Red mask component (Main suit body)
            else if (r > 100 && g < 50 && b < 50) {
                float intensity = (float)r / 255.0f;
                imgData[idx + 0] = (unsigned char)(mR * intensity);
                imgData[idx + 1] = (unsigned char)(mG * intensity);
                imgData[idx + 2] = (unsigned char)(mB * intensity);
            }
            // Check for Blue mask component (Suit shadow)
            else if (b > 100 && r < 50 && g < 50) {
                float intensity = (float)b / 255.0f;
                imgData[idx + 0] = (unsigned char)(sR * intensity);
                imgData[idx + 1] = (unsigned char)(sG * intensity);
                imgData[idx + 2] = (unsigned char)(sB * intensity);
            }
        }
    }

    bool PlayerSprite::initWithFileAndColor(const std::string& path, int colorId) {
        m_colorId = colorId;

        std::vector<uint8_t> fileData = engine::Filesystem::getInstance().readFileBinaryRomfs(path.c_str());
        if (fileData.empty()) {
            FILE* f = std::fopen(path.c_str(), "rb");
            if (f) {
                std::fseek(f, 0, SEEK_END);
                long length = std::ftell(f);
                std::fseek(f, 0, SEEK_SET);
                if (length > 0) {
                    fileData.resize(length);
                    std::fread(fileData.data(), 1, length, f);
                }
                std::fclose(f);
            }
        }

        if (fileData.empty()) {
            std::printf("[PlayerSprite] Failed to read file: %s\n", path.c_str());
            return false;
        }

        int width, height, channels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* imgData = stbi_load_from_memory(
            fileData.data(), (int)fileData.size(), &width, &height, &channels, 4
        );

        if (!imgData) {
            std::printf("[PlayerSprite] Failed to decode image: %s\n", stbi_failure_reason());
            return false;
        }

        // Remap colors based on the chosen player color ID
        applyPlayerColorMapping(imgData, width, height, m_colorId);

        auto nextPow2 = [](int v) {
            v--;
            v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
            v++;
            return v < 8 ? 8 : v;
        };

        int texWidth = nextPow2(width);
        int texHeight = nextPow2(height);
        std::vector<uint8_t> finalData(texWidth * texHeight * 4, 0);

        auto getMortonOffset = [](int x, int y, int texW) -> int {
            int tileX = x / 8;
            int tileY = y / 8;
            int tilesPerRow = texW / 8;
            int tileIndex = (tileY * tilesPerRow) + tileX;

            int localX = x % 8;
            int localY = y % 8;

            int pixelInTile = (localX & 1)       | ((localY & 1) << 1) |
                              ((localX & 2) << 1) | ((localY & 2) << 2) |
                              ((localX & 4) << 2) | ((localY & 4) << 3);

            return ((tileIndex * 64) + pixelInTile) * 4;
        };

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int srcIdx = (y * width + x) * 4;
                int dstIdx = getMortonOffset(x, y, texWidth);

                finalData[dstIdx + 0] = imgData[srcIdx + 3];
                finalData[dstIdx + 1] = imgData[srcIdx + 2];
                finalData[dstIdx + 2] = imgData[srcIdx + 1];
                finalData[dstIdx + 3] = imgData[srcIdx + 0];
            }
        }

        C3D_Tex* tex = new C3D_Tex();
        if (!C3D_TexInit(tex, texWidth, texHeight, GPU_RGBA8)) {
            stbi_image_free(imgData);
            delete tex;
            return false;
        }

        C3D_TexUpload(tex, finalData.data());
        stbi_image_free(imgData);
        C3D_TexSetFilter(tex, GPU_LINEAR, GPU_LINEAR);

        // Assign to base class texture properties (ensure m_image, m_ownsTexture, etc. are protected in engine::Sprite)
        m_image.tex = tex;
        float right = (float)width / (float)texWidth;
        float bottom = 1.0f - ((float)height / (float)texHeight);
        m_image.subtex = new Tex3DS_SubTexture{(u16)width, (u16)height, 0.0f, 1.0f, right, bottom};
        m_ownsTexture = true;

        setContentSize((float)width, (float)height);

        return true;
    }

    void PlayerSprite::setPlayerColor(int colorId) {
        m_colorId = colorId;
    }

} // namespace game