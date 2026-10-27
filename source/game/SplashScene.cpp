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
        m_innerslothLogo = engine::Sprite::create("romfs:/sprites/logoImage.png");
        m_innerslothLogo->setPosition(50.f, 50.f);
        m_innerslothLogo->setScale(0.5f);
        this->addChild(m_innerslothLogo);
    }

    void SplashSceneTop::update(float dt) {
        
    }

    std::shared_ptr<SplashSceneBottom> SplashSceneBottom::create() {
        auto scene = std::make_shared<SplashSceneBottom>();
        scene->init();
        return scene;
    }

    void SplashSceneBottom::init() {
        m_rodCredit = engine::Label::create("romfs:/fonts/arial.ttf", "Ported to 3DS\nby OmgRod");
        m_rodCredit->setAlignment(engine::Label::HAlign::Center);
        m_rodCredit->setPosition(150.f, 150.f);
        this->addChild(m_rodCredit);
    }

    void SplashSceneBottom::update(float dt) {
        
    }

}