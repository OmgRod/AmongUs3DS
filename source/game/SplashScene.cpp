#include <game/SplashScene.hpp>
#include <engine/actions.hpp>
#include <engine/input.hpp>

namespace game {

    std::shared_ptr<SplashSceneTop> SplashSceneTop::create() {
        auto scene = std::make_shared<SplashSceneTop>();
        scene->init();
        return scene;
    }

    void SplashSceneTop::init() {
        auto size = engine::Director::getInstance().getTopScreenSize();

        m_innerslothLogo = engine::Sprite::create("romfs:/sprites/logoImage.png");
        if (m_innerslothLogo) {
            m_innerslothLogo->setPosition(size.width / 2.0f, -100.0f);
            m_innerslothLogo->setAnchorPoint(0.5f, 0.5f);
            m_innerslothLogo->setScale(0.3f);
            this->addChild(m_innerslothLogo);

            m_innerslothLogo->runAction(engine::MoveTo::create(2.0f, size.width / 2.0f, size.height / 2.0f));
        }
    }

    void SplashSceneTop::update(float dt) {
        engine::Scene::update(dt);
    }

    std::shared_ptr<SplashSceneBottom> SplashSceneBottom::create() {
        auto scene = std::make_shared<SplashSceneBottom>();
        scene->init();
        return scene;
    }

    void SplashSceneBottom::init() {
        auto size = engine::Director::getInstance().getBottomScreenSize();

        m_rodCredit = engine::Label::create("romfs:/fonts/arial.ttf", "Ported to 3DS\nby OmgRod");
        m_rodCredit->setAlignment(engine::Label::HAlign::Center);
        m_rodCredit->setAnchorPoint(0.5f, 0.5f);
        m_rodCredit->setPosition(size.width / 2.0f, size.height / 2.0f);
        m_rodCredit->setTextScale(1.0f);
        this->addChild(m_rodCredit);
    }

    void SplashSceneBottom::update(float dt) {
        engine::Scene::update(dt);
    }

}