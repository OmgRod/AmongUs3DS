#include <game/scenes/MenuScene.hpp>
#include <game/objects/PlayerSprite.hpp>
#include <game/objects/colors.hpp>
#include <engine/actions.hpp>
#include <engine/input.hpp>

namespace game {

    std::shared_ptr<MenuSceneTop> MenuSceneTop::create() {
        auto scene = std::make_shared<MenuSceneTop>();
        scene->init();
        return scene;
    }

    void MenuSceneTop::init() {
        auto size = engine::Director::getInstance().getTopScreenSize();

        auto& ae = engine::AudioEngine::getInstance();
        auto music = ae.playSFX("sdmc:/3ds/AmongUs3DS/assets/audio/AmongUsTheme.wav", true);
        music.setTag("menu-music");

        m_starfield = Starfield::create(size.width, size.height, StarDirection::Right, 25.f);
        this->addChild(m_starfield);

        m_floatingCrew.reserve(8);
        for (int i = 0; i < 6; ++i) {
            spawnCrewMember(false);
        }

        auto logo = engine::Sprite::create("sdmc:/3ds/AmongUs3DS/assets/sprites/bannerLogo_AmongUs.png");
        logo->setPosition(size.width / 2, size.height / 2);
        logo->setScale(0.55f);
        logo->setAnchorPoint(0.5f, 0.5f);
        this->addChild(logo);

        auto copyrightNotice = engine::Label::create("romfs:/fonts/arial.bcfnt", "This is an unofficial Nintendo 3DS port developed by OmgRod. Among Us is\nproperty of Innersloth LLC. OmgRod is not affiliated with or endorsed by\nInnersloth.");
        copyrightNotice->setTextScale(0.5f);
        copyrightNotice->setPosition(0.f, size.height - (copyrightNotice->getHeight() / 2) - 5.f);
        this->addChild(copyrightNotice);
    }

    void MenuSceneTop::spawnCrewMember(bool outside) {
        auto size = engine::Director::getInstance().getTopScreenSize();
        int crewIndex = engine::Math::randomInt(1, 6);
        int colorId = engine::Math::randomInt(0, 11);
        std::string path = "sdmc:/3ds/AmongUs3DS/assets/sprites/mainscreenCrew" + std::to_string(crewIndex) + ".png";
        
        auto sprite = game::PlayerSprite::create(path, colorId);
        if (!sprite) return;

        float scale = engine::Math::randomFloat(0.15f, 0.30f);
        sprite->setScale(scale);
        sprite->setAnchorPoint(0.5f, 0.5f);

        float x, y;
        if (outside) {
            int edge = engine::Math::randomInt(0, 3);
            switch (edge) {
                case 0: // Top
                    x = engine::Math::randomFloat(0, size.width);
                    y = -50.0f;
                    break;
                case 1: // Bottom
                    x = engine::Math::randomFloat(0, size.width);
                    y = size.height + 50.0f;
                    break;
                case 2: // Left
                    x = -50.0f;
                    y = engine::Math::randomFloat(0, size.height);
                    break;
                default: // Right
                    x = size.width + 50.0f;
                    y = engine::Math::randomFloat(0, size.height);
                    break;
            }
        } else {
            x = engine::Math::randomFloat(30, size.width - 30);
            y = engine::Math::randomFloat(30, size.height - 30);
        }

        float vx = engine::Math::randomFloat(-6.0f, 6.0f);
        float vy = engine::Math::randomFloat(-6.0f, 6.0f);
        if (std::abs(vx) < 1.0f) vx = (vx < 0 ? -2.0f : 2.0f);
        if (std::abs(vy) < 1.0f) vy = (vy < 0 ? -2.0f : 2.0f);

        float rotation = engine::Math::randomFloat(0.0f, 360.0f);
        float vRot = engine::Math::randomFloat(-12.0f, 12.0f);

        sprite->setPosition(x, y);
        sprite->setRotation(rotation);

        this->addChild(sprite);
        m_floatingCrew.push_back({sprite, x, y, vx, vy, rotation, vRot});
    }

    void MenuSceneTop::update(float dt) {
        engine::Scene::update(dt);

        auto size = engine::Director::getInstance().getTopScreenSize();

        for (auto& crew : m_floatingCrew) {
            crew.x += crew.vx * dt;
            crew.y += crew.vy * dt;
            crew.rotation += crew.vRot * dt;

            crew.sprite->setPosition(crew.x, crew.y);
            crew.sprite->setRotation(crew.rotation);

            bool outOfBounds = (crew.x < -70.0f || crew.x > size.width + 70.0f ||
                                crew.y < -70.0f || crew.y > size.height + 70.0f);

            if (outOfBounds) {
                int edge = engine::Math::randomInt(0, 3);
                switch (edge) {
                    case 0: // Top
                        crew.x = engine::Math::randomFloat(0, size.width);
                        crew.y = -50.0f;
                        break;
                    case 1: // Bottom
                        crew.x = engine::Math::randomFloat(0, size.width);
                        crew.y = size.height + 50.0f;
                        break;
                    case 2: // Left
                        crew.x = -50.0f;
                        crew.y = engine::Math::randomFloat(0, size.height);
                        break;
                    default: // Right
                        crew.x = size.width + 50.0f;
                        crew.y = engine::Math::randomFloat(0, size.height);
                        break;
                }
                crew.vx = engine::Math::randomFloat(-6.0f, 6.0f);
                crew.vy = engine::Math::randomFloat(-6.0f, 6.0f);
                if (std::abs(crew.vx) < 1.0f) crew.vx = (crew.vx < 0 ? -2.0f : 2.0f);
                if (std::abs(crew.vy) < 1.0f) crew.vy = (crew.vy < 0 ? -2.0f : 2.0f);
                crew.vRot = engine::Math::randomFloat(-12.0f, 12.0f);
            }
        }
    }

    std::shared_ptr<MenuSceneBottom> MenuSceneBottom::create() {
        auto scene = std::make_shared<MenuSceneBottom>();
        scene->init();
        return scene;
    }

    void MenuSceneBottom::init() {
        auto size = engine::Director::getInstance().getBottomScreenSize();

        m_starfield = Starfield::create(size.width, size.height, StarDirection::Right, 25.f);
        this->addChild(m_starfield);
    }

    void MenuSceneBottom::update(float dt) {
        engine::Scene::update(dt);
    }

}
