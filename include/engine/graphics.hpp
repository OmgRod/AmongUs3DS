#pragma once

#include <includes.hpp>

namespace engine {

    enum class TargetScreen {
        Top,
        Bottom
    };

    class Node {
    protected:
        float m_x = 0.0f;
        float m_y = 0.0f;
        float m_scaleX = 1.0f;
        float m_scaleY = 1.0f;
        float m_rotation = 0.0f;
        float m_anchorX = 0.5f;
        float m_anchorY = 0.5f;
        float m_zOrder = 0.0f;
        bool m_visible = true;

        Node* m_parent = nullptr;
        std::vector<std::shared_ptr<Node>> m_children;

    public:
        Node() = default;
        virtual ~Node() = default;

        static std::shared_ptr<Node> create();

        virtual void update(float dt);
        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation);
        virtual void visit(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation);

        void addChild(std::shared_ptr<Node> child);
        void removeChild(std::shared_ptr<Node> child);
        void removeFromParent();
        void sortAllChildren();

        void setPosition(float x, float y) { m_x = x; m_y = y; }
        void setScale(float scale) { m_scaleX = scale; m_scaleY = scale; }
        void setScale(float x, float y) { m_scaleX = x; m_scaleY = y; }
        void setRotation(float angle) { m_rotation = angle; }
        void setAnchorPoint(float x, float y) { m_anchorX = x; m_anchorY = y; }
        void setZOrder(float z) { m_zOrder = z; sortAllChildren(); }
        void setVisible(bool visible) { m_visible = visible; }

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        Node* getParent() const { return m_parent; }
        void setParent(Node* parent) { m_parent = parent; }
    };

    class Sprite : public Node {
    private:
        C2D_Image m_image;
        C2D_ImageTint m_tint;
        bool m_hasTint = false;

    public:
        Sprite();
        virtual ~Sprite() = default;

        static std::shared_ptr<Sprite> create(C2D_SpriteSheet sheet, size_t imageIndex);

        bool initWithImage(C2D_SpriteSheet sheet, size_t imageIndex);
        void setTint(u32 color);
        void clearTint();

        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;
    };

    class Scene : public Node {
    public:
        static std::shared_ptr<Scene> create();
    };

    class Director {
    private:
        std::shared_ptr<Scene> m_topScene;
        std::shared_ptr<Scene> m_bottomScene;
        std::map<std::string, C2D_SpriteSheet> m_spriteSheets;
        u32 m_topClearColor;
        u32 m_bottomClearColor;

        Director();
        ~Director();

    public:
        Director(const Director&) = delete;
        Director& operator=(const Director&) = delete;

        static Director& getInstance() {
            static Director instance;
            return instance;
        }

        void setClearColor(u8 r, u8 g, u8 b, u8 a = 255);
        void setClearColorTop(u8 r, u8 g, u8 b, u8 a = 255);
        void setClearColorBottom(u8 r, u8 g, u8 b, u8 a = 255);

        void setTopScene(std::shared_ptr<Scene> scene);
        void setBottomScene(std::shared_ptr<Scene> scene);
        void runWithScenes(std::shared_ptr<Scene> topScene, std::shared_ptr<Scene> bottomScene);
        
        std::shared_ptr<Scene> getTopScene() const { return m_topScene; }
        std::shared_ptr<Scene> getBottomScene() const { return m_bottomScene; }

        C2D_SpriteSheet loadSpriteSheet(const std::string& key, const std::string& path);
        void unloadSpriteSheet(const std::string& key);

        void mainLoop();
    };

}