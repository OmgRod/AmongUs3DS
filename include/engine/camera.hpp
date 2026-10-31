#pragma once

#include <includes.hpp>

namespace engine {

    enum class CameraSelect {
        OuterLeft = 0,
        Inner = 1,
        OuterRight = 2
    };

    enum class CameraFormat {
        Rgb565 = 0,
        Yuyv = 1,
        Bayer = 2
    };

    enum class CameraSize {
        Vga = 0,    // 640x480
        Qvga = 1,   // 320x240
        Qqndv = 2   // 160x120
    };

    class CameraManager {
    private:
        bool m_initialized = false;
        bool m_capturing = false;

        CameraManager();

    public:
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;

        static CameraManager& getInstance() {
            static CameraManager instance;
            return instance;
        }

        ~CameraManager();

        bool init();
        void shutdown();
        bool isInitialized() const { return m_initialized; }

        bool startCapture(CameraSelect camera = CameraSelect::Inner, CameraSize size = CameraSize::Qvga, CameraFormat format = CameraFormat::Rgb565);
        void stopCapture();
        bool isCapturing() const { return m_capturing; }

        bool getFrame(uint8_t* buffer, size_t size);
    };

}
