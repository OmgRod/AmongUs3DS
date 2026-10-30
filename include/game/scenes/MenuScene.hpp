#pragma once

#include <includes.hpp>

namespace game {

    class MenuSceneTop : public engine::Scene {
    private:
        // fields

    public:
        static std::shared_ptr<MenuSceneTop> create();
        void init();
        void update(float dt);
    };

    class MenuSceneBottom : public engine::Scene {
    private:
        // fields

    public:
        static std::shared_ptr<MenuSceneBottom> create();
        void init();
        void update(float dt);
    };

}