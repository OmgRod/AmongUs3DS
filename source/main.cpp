#include <includes.hpp>

class GameplayScene : public engine::Scene {
public:
    static std::shared_ptr<GameplayScene> create() {
        auto scene = std::make_shared<GameplayScene>();
        scene->init();
        return scene;
    }

    void init() {
        engine::AudioEngine::getInstance().playSFX("romfs:/audio/report_Bobdyfound.wav", true);
    }

    void update(float dt) override {
        engine::Scene::update(dt);
        engine::AudioEngine::getInstance().update();
    }
};

class TabletUIScene : public engine::Scene {
public:
    static std::shared_ptr<TabletUIScene> create() {
        auto scene = std::make_shared<TabletUIScene>();
        scene->init();
        return scene;
    }

    void init() {
    }

    void update(float dt) override {
        engine::Scene::update(dt);

        touchPosition touch;
        hidTouchRead(&touch);
        if (hidKeysDown() & KEY_TOUCH) {
        }
    }
};

int main(int argc, char **argv) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    romfsInit();
    ndspInit();
    C2D_Prepare();

    auto& director = engine::Director::getInstance();

    director.setClearColorTop(10, 10, 15, 255);
    director.setClearColorBottom(40, 45, 55, 255);

    auto topScene = GameplayScene::create();
    auto bottomScene = TabletUIScene::create();

    director.runWithScenes(topScene, bottomScene);

    director.mainLoop();

    engine::AudioEngine::getInstance().shutdown();
    C2D_Fini();
    C3D_Fini();
    ndspExit();
    romfsExit();
    gfxExit();
    
    return 0;
}