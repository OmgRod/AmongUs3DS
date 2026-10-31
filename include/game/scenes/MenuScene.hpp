#pragma once

#include <includes.hpp>
#include <game/objects/Starfield.hpp>

namespace game {

    struct FloatingCrew {
        std::shared_ptr<engine::Sprite> sprite;
        float x, y;
        float vx, vy;
        float rotation;
        float vRot;
    };

    class MenuSceneTop : public engine::Scene {
    private:
        std::shared_ptr<Starfield> m_starfield;
        std::vector<FloatingCrew> m_floatingCrew;

        void spawnCrewMember(bool outside);

    public:
        static std::shared_ptr<MenuSceneTop> create();
        void init();
        void update(float dt) override;
    };

    class MenuSceneBottom : public engine::Scene {
    private:
        std::shared_ptr<Starfield> m_starfield;

    public:
        static std::shared_ptr<MenuSceneBottom> create();
        void init();
        void update(float dt) override;
    };

}
