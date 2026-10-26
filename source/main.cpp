#include <includes.hpp>

int main(int argc, char **argv) {
    // initialise stuff
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    romfsInit();
    ndspInit();
    C2D_Prepare();

    C2D_TextBuf c2dTextBuf = C2D_TextBufNew(4096);

    engine::AudioEngine::getInstance().playSFX("romfs:/audio/report_Bobdyfound.wav", true);

    // main loop
    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; 

        engine::AudioEngine::getInstance().update();

        // ...

        C3D_FrameEnd(0);
    }

    engine::AudioEngine::getInstance().shutdown();

    C2D_TextBufDelete(c2dTextBuf);
    C2D_Fini();
    C3D_Fini();
    ndspExit();
    romfsExit();
    gfxExit();
    return 0;
}