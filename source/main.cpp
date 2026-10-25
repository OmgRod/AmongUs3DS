#include <3ds.h>
#include <citro2d.h>
#include <vector>
#include <cmath>
#include <cstdio>

struct TouchPoint {
    u16 x;
    u16 y;
};

int main(int argc, char **argv) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    C2D_TextBuf c2dTextBuf = C2D_TextBufNew(4096);

    float playerX = 160.0f;
    float playerY = 120.0f;
    float speed = 150.0f;

    std::vector<TouchPoint> activeTouches;
    const char* lastButtonPressed = "None"; 

    u64 lastTick = osGetTime();
    int fps = 0;

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break; 

        if (kDown & KEY_A) lastButtonPressed = "A";
        else if (kDown & KEY_B) lastButtonPressed = "B";
        else if (kDown & KEY_X) lastButtonPressed = "X";
        else if (kDown & KEY_Y) lastButtonPressed = "Y";

        circlePosition circlePad;
        hidCircleRead(&circlePad);

        u64 currentTick = osGetTime();
        float dt = (currentTick - lastTick) / 1000.0f;
        if (dt <= 0.0f) dt = 0.016f;
        lastTick = currentTick;
        fps = (int)(1.0f / dt);

        float dx = circlePad.dx / 156.0f;
        float dy = -circlePad.dy / 156.0f;
        float magnitude = std::sqrt(dx*dx + dy*dy);
        
        if (magnitude > 0.15f) { 
            if (magnitude > 1.0f) {
                dx /= magnitude;
                dy /= magnitude;
            }
            playerX += dx * speed * dt;
            playerY += dy * speed * dt;
        }

        if (playerX < 20.0f) playerX = 20.0f;
        if (playerX > 300.0f) playerX = 300.0f;
        if (playerY < 20.0f) playerY = 20.0f;
        if (playerY > 210.0f) playerY = 210.0f;

        touchPosition touch;
        hidTouchRead(&touch);
        activeTouches.clear();
        if (kHeld & KEY_TOUCH) {
            activeTouches.push_back({touch.px, touch.py});
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(top, C2D_Color32(26, 26, 38, 255));
        C2D_SceneBegin(top);
        C2D_TextBufClear(c2dTextBuf);

        C2D_Text titleText, fpsText, btnText;
        C2D_TextParse(&titleText, c2dTextBuf, "Among Us 3DS Native Engine");
        C2D_TextOptimize(&titleText);
        C2D_DrawText(&titleText, C2D_WithColor, 10.0f, 10.0f, 0.5f, 0.6f, 0.6f, C2D_Color32(255, 255, 255, 255));

        char fpsBuf[32];
        snprintf(fpsBuf, sizeof(fpsBuf), "FPS: %d", fps);
        C2D_TextParse(&fpsText, c2dTextBuf, fpsBuf);
        C2D_TextOptimize(&fpsText);
        C2D_DrawText(&fpsText, C2D_WithColor, 320.0f, 10.0f, 0.5f, 0.5f, 0.5f, C2D_Color32(100, 255, 100, 255));

        char btnBuf[64];
        snprintf(btnBuf, sizeof(btnBuf), "Action: %s", lastButtonPressed);
        C2D_TextParse(&btnText, c2dTextBuf, btnBuf);
        C2D_TextOptimize(&btnText);
        C2D_DrawText(&btnText, C2D_WithColor, 10.0f, 40.0f, 0.5f, 0.5f, 0.5f, C2D_Color32(100, 204, 255, 255));

        C2D_TargetClear(bottom, C2D_Color32(40, 44, 52, 255));
        C2D_SceneBegin(bottom);

        u32 colorRed = C2D_Color32(197, 17, 17, 255);
        u32 colorDarkRed = C2D_Color32(122, 8, 56, 255);
        u32 colorVisor = C2D_Color32(147, 206, 222, 255);
        
        C2D_DrawRectangle(playerX - 14, playerY - 4, 0, 8, 20, colorDarkRed, colorDarkRed, colorDarkRed, colorDarkRed);
        C2D_DrawCircleSolid(playerX, playerY, 0.5f, 10.0f, colorRed); 
        C2D_DrawRectangle(playerX - 10, playerY, 0, 20, 16, colorRed, colorRed, colorRed, colorRed);
        C2D_DrawRectangle(playerX + 2, playerY - 2, 0, 12, 8, colorVisor, colorVisor, colorVisor, colorVisor);

        for (const auto& t : activeTouches) {
            C2D_DrawCircleSolid(t.x, t.y, 0.5f, 6.0f, C2D_Color32(255, 255, 255, 128));
        }

        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(c2dTextBuf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}