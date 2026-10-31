#include <game/scenes/MenuScene.hpp>
#include <game/objects/PlayerSprite.hpp>
#include <engine/actions.hpp>
#include <engine/input.hpp>
#include <algorithm>
#include <random>

namespace game {

    static float s_spawnTimer = 0.0f;

    std::shared_ptr<MenuSceneTop> MenuSceneTop::create() {
        auto scene = std::make_shared<MenuSceneTop>();
        scene->init();
        return scene;
    }

    void MenuSceneTop::init() {
        auto size = engine::Director::getInstance().getTopScreenSize();
        s_spawnTimer = 0.0f;

        auto& ae = engine::AudioEngine::getInstance();
        auto music = ae.playSFX("romfs:/audio/AmongUsTheme.wav", true);
        music.setTag("menu-music");

        m_starfield = Starfield::create(size.width, size.height, StarDirection::Right, 25.f);
        m_starfield->setZOrder(-100);
        this->addChild(m_starfield);

        for (int i = 0; i < 6; ++i) {
            spawnCrewMember(false);
        }

        auto logo = engine::Sprite::create("romfs:/sprites/bannerLogo_AmongUs.png");
        logo->setPosition(size.width / 2, size.height / 2);
        logo->setScale(0.55f);
        logo->setAnchorPoint(0.5f, 0.5f);
        logo->setZOrder(1000);
        this->addChild(logo);

        auto copyrightNotice = engine::Label::create("romfs:/fonts/arial.bcfnt", "This is an unofficial Nintendo 3DS port developed by OmgRod. Among Us is\nproperty of Innersloth LLC. OmgRod is not affiliated with or endorsed by\nInnersloth.");
        copyrightNotice->setTextScale(0.5f);
        copyrightNotice->setPosition(0.f, size.height - (copyrightNotice->getHeight() / 2) - 5.f);
        copyrightNotice->setZOrder(1000);
        this->addChild(copyrightNotice);
    }

    void MenuSceneTop::spawnCrewMember(bool outside) {
        auto size = engine::Director::getInstance().getTopScreenSize();
        int crewIndex = engine::Math::randomInt(1, 6);
        std::string path = "romfs:/sprites/mainscreenCrew" + std::to_string(crewIndex) + ".png";
        
        std::vector<int> usedColors;
        for (const auto& member : m_floatingCrew) {}

        int colorId = engine::Math::randomInt(0, 11);
        auto sprite = game::PlayerSprite::create(path, colorId);
        if (!sprite) return;

        float scale = engine::Math::randomFloat(0.15f, 0.30f);
        sprite->setScale(scale);
        sprite->setAnchorPoint(0.5f, 0.5f);

        int zOrderValue = static_cast<int>((scale - 0.15f) / 0.15f * 20.0f) - 10;
        sprite->setZOrder(zOrderValue);

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

        s_spawnTimer += dt;
        if (s_spawnTimer >= 3.0f) {
            s_spawnTimer = 0.0f;
            spawnCrewMember(true);
        }

        for (auto it = m_floatingCrew.begin(); it != m_floatingCrew.end(); ) {
            it->x += it->vx * dt;
            it->y += it->vy * dt;
            it->rotation += it->vRot * dt;

            it->sprite->setPosition(it->x, it->y);
            it->sprite->setRotation(it->rotation);

            bool outOfBounds = (it->x < -80.0f || it->x > size.width + 80.0f ||
                                it->y < -80.0f || it->y > size.height + 80.0f);

            if (outOfBounds) {
                it->sprite->removeFromParent();
                it = m_floatingCrew.erase(it);
            } else {
                ++it;
            }
        }

        if (m_floatingCrew.size() < 6) {
            spawnCrewMember(true);
        }
    }

    std::shared_ptr<MenuSceneBottom> MenuSceneBottom::create() {
        auto scene = std::make_shared<MenuSceneBottom>();
        scene->init();
        return scene;
    }

    void MenuSceneBottom::init() {
        
    }

    void MenuSceneBottom::update(float dt) {
        engine::Scene::update(dt);
    }

}