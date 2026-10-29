#include <game/SplashScene.hpp>
#include <game/MenuScene.hpp>
#include <game/objects/Starfield.hpp>
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

        m_starfield = game::Starfield::create(size.width, size.height, 60);

        m_innerslothLogo = engine::Sprite::create("romfs:/sprites/logoImage.png");
        if (m_innerslothLogo) {
            m_innerslothLogo->setPosition(size.width / 2.0f, -100.0f);
            m_innerslothLogo->setAnchorPoint(0.5f, 0.5f);
            m_innerslothLogo->setScale(0.3f);
            this->addChild(m_innerslothLogo);

            auto logoPtr = m_innerslothLogo;

            m_innerslothLogo->runAction(engine::Sequence::create(
                engine::PlaySound::create("romfs:/audio/panel_admin_cardmove1.wav"),
                engine::MoveTo::create(1.0f, size.width / 2.0f, size.height / 2.0f, engine::Ease::SineInOut),
                engine::DelayTime::create(0.2f),
                engine::PlaySound::create("romfs:/audio/task_Complete.wav"),
                engine::ScaleTo::create(0.25f, 0.325f, engine::Ease::SineIn),
                engine::ScaleTo::create(0.25f, 0.3f, engine::Ease::SineOut),
                engine::CallFunc::create([logoPtr]() {
                    logoPtr->runAction(engine::Sequence::create(
                        engine::FadeOut::create(0.5f),
                        engine::CallFunc::create([]() {
                            auto menuSceneTop = game::MenuScene::create();
                            auto menuSceneBottom = game::MenuScene::create();

                            engine::Director::getInstance().replaceTopScene(menuSceneTop);
                            engine::Director::getInstance().replaceBottomScene(menuSceneBottom);
                        }),
                        nullptr
                    ));
                }),
                nullptr
            ));
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