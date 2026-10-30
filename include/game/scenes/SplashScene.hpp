#pragma once

#include <includes.hpp>
#include <game/objects/Starfield.hpp>

namespace game {

    class SplashSceneTop : public engine::Scene {
    private:
        std::shared_ptr<engine::Sprite> m_innerslothLogo;
        std::shared_ptr<engine::DrawNode> m_fadeOut;
        std::shared_ptr<Starfield> m_starfield;

    public:
        static std::shared_ptr<SplashSceneTop> create();
        void init();
        void update(float dt);
    };

    class SplashSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<engine::Label> m_rodCredit; // oh it's me
        std::shared_ptr<engine::DrawNode> m_fadeOut;
        std::shared_ptr<Starfield> m_starfield;

    public:
        static std::shared_ptr<SplashSceneBottom> create();
        void init();
        void update(float dt);
        void startFadeOut(float duration);
    };

}
