#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Cardinal direction enumeration.
     */
    enum class Direction {
        Up,    ///< Up direction.
        Down,  ///< Down direction.
        Left,  ///< Left direction.
        Right  ///< Right direction.
    };

    /**
     * @brief Joystick or circle pad coordinate position structure.
     */
    struct JoyPosition {
        s16 dx; ///< Horizontal delta / axis value.
        s16 dy; ///< Vertical delta / axis value.
    };

    /**
     * @brief Singleton manager class handling 3DS hardware button input, circle pad, touch screen, and text entry.
     */
    class Input {
    private:
        touchPosition m_touch;       ///< Current touch screen coordinate position.
        circlePosition m_circlePad;  ///< Current circle pad axis position.
        circlePosition m_cStick;     ///< Current C-stick axis position (New 3DS).
        
        u32 m_kDown = 0;             ///< Keys pressed down in the current frame.
        u32 m_kHeld = 0;             ///< Keys currently held down.
        u32 m_kUp = 0;               ///< Keys released in the current frame.

        /** @brief Private default constructor for singleton pattern. */
        Input() = default;

    public:
        /** @brief Deleted copy constructor. */
        Input(const Input&) = delete;
        
        /** @brief Deleted copy assignment operator. */
        Input& operator=(const Input&) = delete;

        /**
         * @brief Gets the singleton instance of the Input manager.
         * @return Reference to the Input instance.
         */
        static Input& getInstance() {
            static Input instance;
            return instance;
        }

        /** @brief Polls hardware HID state and updates button and analog inputs for the current frame. */
        void poll() {
            hidScanInput();
            m_kDown = hidKeysDown();
            m_kHeld = hidKeysHeld();
            m_kUp = hidKeysUp();
            hidTouchRead(&m_touch);
            hidCircleRead(&m_circlePad);
            hidCstickRead(&m_cStick);
        }

        /** @brief Checks if specific key(s) were pressed down this frame. @param key Bitmask of keys. @return True if pressed. */
        bool isKeyDown(u32 key) const { return (m_kDown & key) != 0; }
        
        /** @brief Checks if specific key(s) are currently held down. @param key Bitmask of keys. @return True if held. */
        bool isKeyHeld(u32 key) const { return (m_kHeld & key) != 0; }
        
        /** @brief Checks if specific key(s) were released this frame. @param key Bitmask of keys. @return True if released. */
        bool isKeyUp(u32 key) const { return (m_kUp & key) != 0; }

        /** @brief Checks if the A button was pressed down. @return True if pressed. */
        bool isAKeyDown() const { return isKeyDown(KEY_A); }
        
        /** @brief Checks if the B button was pressed down. @return True if pressed. */
        bool isBKeyDown() const { return isKeyDown(KEY_B); }
        
        /** @brief Checks if the X button was pressed down. @return True if pressed. */
        bool isXKeyDown() const { return isKeyDown(KEY_X); }
        
        /** @brief Checks if the Y button was pressed down. @return True if pressed. */
        bool isYKeyDown() const { return isKeyDown(KEY_Y); }
        
        /** @brief Checks if the START button was pressed down. @return True if pressed. */
        bool isStartDown() const { return isKeyDown(KEY_START); }
        
        /** @brief Checks if the SELECT button was pressed down. @return True if pressed. */
        bool isSelectDown() const { return isKeyDown(KEY_SELECT); }
        
        /** @brief Checks if the L shoulder button was pressed down. @return True if pressed. */
        bool isLDown() const { return isKeyDown(KEY_L); }
        
        /** @brief Checks if the R shoulder button was pressed down. @return True if pressed. */
        bool isRDown() const { return isKeyDown(KEY_R); }
        
        /** @brief Checks if the ZL shoulder button was pressed down. @return True if pressed. */
        bool isZLDown() const { return isKeyDown(KEY_ZL); }
        
        /** @brief Checks if the ZR shoulder button was pressed down. @return True if pressed. */
        bool isZRDown() const { return isKeyDown(KEY_ZR); }

        /** @brief Checks if the D-Pad Up button was pressed down. @return True if pressed. */
        bool isDpadUp() const { return isKeyDown(KEY_DUP); }
        
        /** @brief Checks if the D-Pad Down button was pressed down. @return True if pressed. */
        bool isDpadDown() const { return isKeyDown(KEY_DDOWN); }
        
        /** @brief Checks if the D-Pad Left button was pressed down. @return True if pressed. */
        bool isDpadLeft() const { return isKeyDown(KEY_DLEFT); }
        
        /** @brief Checks if the D-Pad Right button was pressed down. @return True if pressed. */
        bool isDpadRight() const { return isKeyDown(KEY_DRIGHT); }

        /** @brief Gets current circle pad position coordinates. @return circlePosition struct. */
        circlePosition getCirclePad() const { return m_circlePad; }
        
        /** @brief Gets current C-stick position coordinates. @return circlePosition struct. */
        circlePosition getCStick() const { return m_cStick; }

        /** @brief Checks if the screen is currently being touched. @return True if touched. */
        bool isTouched() const { return isKeyDown(KEY_TOUCH); }
        
        /** @brief Gets current touch screen position coordinates. @return touchPosition struct. */
        touchPosition getTouch() const { return m_touch; }

        /**
         * @brief Opens the system software keyboard to prompt for text input.
         * @param title Prompt title string.
         * @param initialText Initial text string in input field.
         * @param maxLength Maximum character length allowed.
         * @return Entered text string.
         */
        std::string getTextInput(const std::string& title = "", const std::string& initialText = "", size_t maxLength = 256);
    };

}
