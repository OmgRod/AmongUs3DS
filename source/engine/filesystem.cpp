#include <engine/filesystem.hpp>

namespace engine {

std::string Filesystem::readFileRomfs(const char* path) {
    std::string resolved = resolvePath(path);
    FILE* f = std::fopen(resolved.c_str(), "rb");
    if (!f) {
        std::printf("Filesystem: Failed to open file: %s (resolved: %s)\n", path, resolved.c_str());
        return "";
    }

    std::fseek(f, 0, SEEK_END);
    long length = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    if (length <= 0) {
        std::fclose(f);
        return "";
    }

    std::string content;
    content.resize(length);

    size_t readSize = std::fread(&content[0], 1, length, f);
    std::fclose(f);

    if (readSize != (size_t)length) {
        content.resize(readSize);
    }

    return content;
}

std::vector<uint8_t> Filesystem::readFileBinaryRomfs(const char* path) {
    std::string resolved = resolvePath(path);
    FILE* f = std::fopen(resolved.c_str(), "rb");
    if (!f) {
        std::printf("Filesystem: Failed to open binary file: %s (resolved: %s)\n", path, resolved.c_str());
        return {};
    }

    std::fseek(f, 0, SEEK_END);
    long length = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    if (length <= 0) {
        std::fclose(f);
        return {};
    }

    std::vector<uint8_t> buffer(length);
    size_t readSize = std::fread(buffer.data(), 1, length, f);
    std::fclose(f);

    if (readSize != (size_t)length) {
        buffer.resize(readSize);
    }

    return buffer;
}

}
