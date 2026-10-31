#include <includes.hpp>
#include <malloc.h>
#include <game/scenes/SplashScene.hpp>

int main(int argc, char **argv) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    romfsInit();

    engine::NetworkManager::getInstance().init();
    engine::NfcManager::getInstance().init();
    engine::CameraManager::getInstance().init();
    engine::AudioEngine::getInstance().init();

    C2D_Prepare();

    auto& director = engine::Director::getInstance();

    director.setClearColor(0, 0, 0, 255);

    auto topScene = game::SplashSceneTop::create();
    auto bottomScene = game::SplashSceneBottom::create();

    director.runWithScenes(topScene, bottomScene);

    director.mainLoop();

    engine::AudioEngine::getInstance().shutdown();
    engine::CameraManager::getInstance().shutdown();
    engine::NfcManager::getInstance().shutdown();
    engine::NetworkManager::getInstance().shutdown();

    C2D_Fini();
    C3D_Fini();
    romfsExit();
    gfxExit();
    
    return 0;
}