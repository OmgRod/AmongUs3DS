#pragma once

#include <citro2d.h>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <engine/filesystem.hpp>
#include <engine/math.hpp>

namespace engine {

    class Action;

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
        float m_anchorX = 0.0f;
        float m_anchorY = 0.0f;
        float m_zOrder = 0.0f;
        float m_contentWidth = 0.0f;
        float m_contentHeight = 0.0f;
        float m_opacity = 1.0f;
        bool m_visible = true;

        Node* m_parent = nullptr;
        std::vector<std::shared_ptr<Node>> m_children;
        std::vector<std::shared_ptr<Action>> m_actions;

    public:
        Node() = default;
        virtual ~Node() = default;

        static std::shared_ptr<Node> create();

        virtual void update(float dt);
        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation);
        virtual void visit(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation, float parentOpacity = 1.0f);

        void addChild(std::shared_ptr<Node> child);
        void removeChild(std::shared_ptr<Node> child);
        void removeFromParent();
        void sortAllChildren();

        void runAction(std::shared_ptr<Action> action);
        void stopAction(std::shared_ptr<Action> action);
        void stopAllActions();

        void setPosition(float x, float y) { m_x = x; m_y = y; }
        void setScale(float scale) { m_scaleX = scale; m_scaleY = scale; }
        void setScale(float x, float y) { m_scaleX = x; m_scaleY = y; }
        void setRotation(float angle) { m_rotation = angle; }
        void setAnchorPoint(float x, float y) { m_anchorX = x; m_anchorY = y; }
        void setZOrder(float z) { m_zOrder = z; sortAllChildren(); }
        void setVisible(bool visible) { m_visible = visible; }
        void toggleVisibility() { m_visible = !m_visible; }
        void setContentSize(float w, float h) { m_contentWidth = w; m_contentHeight = h; }
        
        void setOpacity(float opacity) { m_opacity = std::clamp(opacity, 0.0f, 1.0f); }
        float getOpacity() const { return m_opacity; }

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getScaleX() const { return m_scaleX; }
        float getScaleY() const { return m_scaleY; }
        float getRotation() const { return m_rotation; }
        bool isVisible() const { return m_visible; }
        Node* getParent() const { return m_parent; }
        void setParent(Node* parent) { m_parent = parent; }
        
        virtual float getWidth() const { return m_contentWidth; }
        virtual float getHeight() const { return m_contentHeight; }
    };

    class Sprite : public Node {
    private:
        C2D_Image m_image;
        C2D_ImageTint m_tint;
        bool m_hasTint = false;
        bool m_ownsTexture = false;
        std::vector<uint8_t> m_pixelData;

    public:
        Sprite();
        virtual ~Sprite();

        static std::shared_ptr<Sprite> create(const std::string& path);

        bool initWithFile(const std::string& path);
        void setTint(u32 color);
        void clearTint();

        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;
    };

    class DrawNode : public Node {
    public:
        enum ShapeType { Rectangle, Triangle, Quad };

    private:
        ShapeType m_shape;
        u32 m_color = C2D_Color32(255, 255, 255, 255);
        float m_x1 = 0, m_y1 = 0, m_x2 = 0, m_y2 = 0, m_x3 = 0, m_y3 = 0, m_x4 = 0, m_y4 = 0;

    public:
        DrawNode() = default;
        virtual ~DrawNode() = default;

        static std::shared_ptr<DrawNode> createRect(float w, float h, u32 color);
        static std::shared_ptr<DrawNode> createTriangle(float x1, float y1, float x2, float y2, float x3, float y3, u32 color);
        static std::shared_ptr<DrawNode> createQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, u32 color);

        void setColor(u32 color) { m_color = color; }
        u32 getColor() const { return m_color; }
        void setSize(float w, float h) { setContentSize(w, h); }

        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;
    };

    class Label : public Node {
    public:
        enum HAlign { Left, Center, Right };

    private:
        C2D_Font m_font;
        C2D_TextBuf m_textBuf;
        C2D_Text m_text;
        std::string m_string;
        u32 m_color = C2D_Color32(255, 255, 255, 255);
        float m_textScaleX = 0.5f;
        float m_textScaleY = 0.5f;
        float m_wrapWidth = 0.0f;
        HAlign m_alignment = HAlign::Left;
        bool m_fontLoaded = false;
        bool m_ownsFont = false;

    public:
        Label();
        virtual ~Label();

        static std::shared_ptr<Label> create(const std::string& fontPath, const std::string& text, float scale = 0.5f, u32 color = C2D_Color32(255, 255, 255, 255));
        
        void setString(const std::string& text);
        void setColor(u32 color) { m_color = color; }
        void setTextScale(float scale) { m_textScaleX = scale; m_textScaleY = scale; }
        void setWrapWidth(float width) { m_wrapWidth = width; parseText(); }
        void setAlignment(HAlign align) { m_alignment = align; }

        virtual void draw(float parentX, float parentY, float parentScaleX, float parentScaleY, float parentRotation) override;

    private:
        void parseText();
    };

    class Layout : public Node {
    public:
        enum Type { Vertical, Horizontal };

    private:
        Type m_type;
        float m_spacing = 10.0f;
        float m_padding = 10.0f;

    public:
        Layout(Type type) : m_type(type) {}
        virtual ~Layout() = default;

        static std::shared_ptr<Layout> createVertical(float spacing = 10.0f, float padding = 10.0f) {
            auto l = std::make_shared<Layout>(Vertical);
            l->m_spacing = spacing;
            l->m_padding = padding;
            return l;
        }

        static std::shared_ptr<Layout> createHorizontal(float spacing = 10.0f, float padding = 10.0f) {
            auto l = std::make_shared<Layout>(Horizontal);
            l->m_spacing = spacing;
            l->m_padding = padding;
            return l;
        }

        void setSpacing(float s) { m_spacing = s; updateLayout(); }
        void setPadding(float p) { m_padding = p; updateLayout(); }
        void updateLayout();

        void addChildAndLayout(std::shared_ptr<Node> child);
    };

    class Button : public Node {
    private:
        std::shared_ptr<DrawNode> m_bgRect;
        std::shared_ptr<Label> m_label;
        std::function<void()> m_onClick;
        u32 m_normalColor;
        u32 m_pressedColor;
        bool m_isPressed = false;

    public:
        Button(float w, float h, const std::string& title, u32 normalColor, u32 pressedColor, std::function<void()> onClick);
        virtual ~Button() = default;

        static std::shared_ptr<Button> create(float w, float h, const std::string& title, u32 normalColor, u32 pressedColor, std::function<void()> onClick);

        virtual void update(float dt) override;
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

        void replaceTopScene(std::shared_ptr<Scene> newScene);
        void replaceBottomScene(std::shared_ptr<Scene> newScene);
        
        std::shared_ptr<Scene> getTopScene() const { return m_topScene; }
        std::shared_ptr<Scene> getBottomScene() const { return m_bottomScene; }

        C2D_SpriteSheet loadSpriteSheet(const std::string& key, const std::string& path);
        void unloadSpriteSheet(const std::string& key);

        Size getTopScreenSize() const;
        Size getBottomScreenSize() const;

        void mainLoop();
    };

}