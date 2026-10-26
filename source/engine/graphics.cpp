#include <engine/graphics.hpp>

namespace engine {
    std::shared_ptr<Node> Node::create() {
        return std::make_shared<Node>();
    }

    void Node::addChild(std::shared_ptr<Node> child) {
        if (!child) return;
        child->setParent(this);
        m_children.push_back(child);
        sortAllChildren();
    }

    void Node::removeChild(std::shared_ptr<Node> child) {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end()) {
            (*it)->setParent(nullptr);
            m_children.erase(it);
        }
    }

    void Node::removeFromParent() {
        if (m_parent) {
            m_parent = nullptr;
        }
    }

    void Node::sortAllChildren() {
        std::stable_sort(m_children.begin(), m_children.end(), 
            [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
                return a->m_zOrder < b->m_zOrder;
            });
    }

    void Node::update(float dt) {
        for (auto& child : m_children) {
            if (child->m_visible) {
                child->update(dt);
            }
        }
    }

    void Node::draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) {}

    void Node::visit(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) {
        if (!m_visible) return;

        float absScaleX = m_scaleX * parentScaleX;
        float absScaleY = m_scaleY * parentScaleY;
        float absRot = m_rotation + parentRotation;

        float rad = parentRotation * (M_PI / 180.0f);
        float cosA = cos(rad);
        float sinA = sin(rad);
        
        float localX = m_x * parentScaleX;
        float localY = m_y * parentScaleY;

        float absX = parentX + (localX * cosA - localY * sinA);
        float absY = parentY + (localX * sinA + localY * cosA);

        draw(absX, absY, absScaleX, absScaleY, absRot);

        for (auto& child : m_children) {
            child->visit(absX, absY, absScaleX, absScaleY, absRot);
        }
    }

    Sprite::Sprite() {
        m_image.tex = nullptr;
        m_image.subtex = nullptr;
    }

    std::shared_ptr<Sprite> Sprite::create(C2D_SpriteSheet sheet, size_t imageIndex) {
        auto sprite = std::make_shared<Sprite>();
        if (sprite->initWithImage(sheet, imageIndex)) {
            return sprite;
        }
        return nullptr;
    }

    bool Sprite::initWithImage(C2D_SpriteSheet sheet, size_t imageIndex) {
        if (!sheet) return false;
        m_image = C2D_SpriteSheetGetImage(sheet, imageIndex);
        return m_image.tex != nullptr;
    }

    void Sprite::setTint(u32 color) {
        C2D_PlainImageTint(&m_tint, color, 1.0f);
        m_hasTint = true;
    }

    void Sprite::clearTint() {
        m_hasTint = false;
    }

    void Sprite::draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) {
        if (!m_image.tex) return;

        C2D_DrawParams params;
        params.pos.x = parentX;
        params.pos.y = parentY;
        params.pos.w = m_image.subtex->width * parentScaleX;
        params.pos.h = m_image.subtex->height * parentScaleY;
        params.center.x = m_anchorX;
        params.center.y = m_anchorY;
        params.angle = parentRotation * (M_PI / 180.0f);
        params.depth = m_zOrder;

        C2D_DrawImage(m_image, &params, m_hasTint ? &m_tint : nullptr);
    }

    std::shared_ptr<Scene> Scene::create() {
        return std::make_shared<Scene>();
    }

    Director::Director() {
        m_topClearColor = C2D_Color32(15, 15, 20, 255);
        m_bottomClearColor = C2D_Color32(30, 30, 35, 255);
    }

    Director::~Director() {
        for (auto& pair : m_spriteSheets) {
            C2D_SpriteSheetFree(pair.second);
        }
        m_spriteSheets.clear();
    }

    void Director::setClearColor(u8 r, u8 g, u8 b, u8 a) {
        m_topClearColor = C2D_Color32(r, g, b, a);
        m_bottomClearColor = C2D_Color32(r, g, b, a);
    }

    void Director::setClearColorTop(u8 r, u8 g, u8 b, u8 a) {
        m_topClearColor = C2D_Color32(r, g, b, a);
    }

    void Director::setClearColorBottom(u8 r, u8 g, u8 b, u8 a) {
        m_bottomClearColor = C2D_Color32(r, g, b, a);
    }

    void Director::setTopScene(std::shared_ptr<Scene> scene) {
        m_topScene = scene;
    }

    void Director::setBottomScene(std::shared_ptr<Scene> scene) {
        m_bottomScene = scene;
    }

    void Director::runWithScenes(std::shared_ptr<Scene> topScene, std::shared_ptr<Scene> bottomScene) {
        m_topScene = topScene;
        m_bottomScene = bottomScene;
    }

    C2D_SpriteSheet Director::loadSpriteSheet(const std::string& key, const std::string& path) {
        if (m_spriteSheets.find(key) != m_spriteSheets.end()) {
            return m_spriteSheets[key];
        }
        C2D_SpriteSheet sheet = C2D_SpriteSheetLoad(path.c_str());
        if (sheet) {
            m_spriteSheets[key] = sheet;
        }
        return sheet;
    }

    void Director::unloadSpriteSheet(const std::string& key) {
        auto it = m_spriteSheets.find(key);
        if (it != m_spriteSheets.end()) {
            C2D_SpriteSheetFree(it->second);
            m_spriteSheets.erase(it);
        }
    }

    void Director::mainLoop() {
        C3D_RenderTarget* topTarget = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
        C3D_RenderTarget* bottomTarget = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

        float dt = 1.0f / 60.0f;

        while (aptMainLoop()) {
            gspWaitForVBlank();
            hidScanInput();

            u32 kDown = hidKeysDown();
            if (kDown & KEY_START) break; 

            if (m_topScene) {
                m_topScene->update(dt);
            }
            if (m_bottomScene) {
                m_bottomScene->update(dt);
            }

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

            C2D_TargetClear(topTarget, m_topClearColor);
            C2D_SceneBegin(topTarget);
            if (m_topScene) {
                m_topScene->visit(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
            }

            C2D_TargetClear(bottomTarget, m_bottomClearColor);
            C2D_SceneBegin(bottomTarget);
            if (m_bottomScene) {
                m_bottomScene->visit(0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
            }

            C3D_FrameEnd(0);
        }
    }
}