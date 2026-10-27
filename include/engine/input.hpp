#pragma once

#include <includes.hpp>

namespace engine {

    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    struct JoyPosition {
        s16 dx;
        s16 dy;
    };

    class Input {
    private:
        touchPosition m_touch;
        circlePosition m_circlePad;
        circlePosition m_cStick;
        
        u32 m_kDown = 0;
        u32 m_kHeld = 0;
        u32 m_kUp = 0;

        Input() = default;

    public:
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        static Input& getInstance() {
            static Input instance;
            return instance;
        }

        void poll() {
            hidScanInput();
            m_kDown = hidKeysDown();
            m_kHeld = hidKeysHeld();
            m_kUp = hidKeysUp();
            hidTouchRead(&m_touch);
            hidCircleRead(&m_circlePad);
            hidCstickRead(&m_cStick);
        }

        bool isKeyDown(u32 key) const { return (m_kDown & key) != 0; }
        bool isKeyHeld(u32 key) const { return (m_kHeld & key) != 0; }
        bool isKeyUp(u32 key) const { return (m_kUp & key) != 0; }

        bool isAKeyDown() const { return isKeyDown(KEY_A); }
        bool isBKeyDown() const { return isKeyDown(KEY_B); }
        bool isXKeyDown() const { return isKeyDown(KEY_X); }
        bool isYKeyDown() const { return isKeyDown(KEY_Y); }
        
        bool isStartDown() const { return isKeyDown(KEY_START); }
        bool isSelectDown() const { return isKeyDown(KEY_SELECT); }
        
        bool isLDown() const { return isKeyDown(KEY_L); }
        bool isRDown() const { return isKeyDown(KEY_R); }
        bool isZLDown() const { return isKeyDown(KEY_ZL); }
        bool isZRDown() const { return isKeyDown(KEY_ZR); }

        bool isDpadUp() const { return isKeyDown(KEY_DUP); }
        bool isDpadDown() const { return isKeyDown(KEY_DDOWN); }
        bool isDpadLeft() const { return isKeyDown(KEY_DLEFT); }
        bool isDpadRight() const { return isKeyDown(KEY_DRIGHT); }

        circlePosition getCirclePad() const { return m_circlePad; }
        circlePosition getCStick() const { return m_cStick; }

        bool isTouched() const { return isKeyDown(KEY_TOUCH); }
        touchPosition getTouch() const { return m_touch; }
    };

}