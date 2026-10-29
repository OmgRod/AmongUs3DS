#include <includes.hpp>
#include <malloc.h>
#include <game/SplashScene.hpp>

int main(int argc, char **argv) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    romfsInit();

    void* socBuffer = memalign(0x1000, 0x100000); 
    Result socRes = socInit((u32*)socBuffer, 0x100000);
    if (R_FAILED(socRes)) {
        std::printf("Failed to initialize SOC service (0x%08lX). Networking disabled.\n", (unsigned long)socRes);
    }
    
    engine::AudioEngine::getInstance().init();

    C2D_Prepare();

    auto& director = engine::Director::getInstance();

    director.setClearColor(0, 0, 0, 255);

    auto topScene = game::SplashSceneTop::create();
    auto bottomScene = game::SplashSceneBottom::create();

    director.runWithScenes(topScene, bottomScene);

    director.mainLoop();

    engine::AudioEngine::getInstance().shutdown();

    socExit();
    free(socBuffer);

    C2D_Fini();
    C3D_Fini();
    romfsExit();
    gfxExit();
    
    return 0;
}