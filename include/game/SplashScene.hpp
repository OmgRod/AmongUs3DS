#pragma once

#include <includes.hpp>

namespace game {

    class SplashSceneTop : public engine::Scene {
    private:
        std::shared_ptr<engine::Sprite> m_innerslothLogo;

    public:
        static std::shared_ptr<SplashSceneTop> create();
        void init();
        void update(float dt) override;
    };

    class SplashSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<engine::Label> m_rodCredit; // oh it's me

    public:
        static std::shared_ptr<SplashSceneBottom> create();
        void init();
        void update(float dt) override;
    };

}