#include <game/MenuScene.hpp>
#include <engine/actions.hpp>
#include <engine/input.hpp>

namespace game {

    std::shared_ptr<MenuSceneTop> MenuSceneTop::create() {
        auto scene = std::make_shared<MenuSceneTop>();
        scene->init();
        return scene;
    }

    void MenuSceneTop::init() {
        
    }

    void MenuSceneTop::update(float dt) {
        engine::Scene::update(dt);
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