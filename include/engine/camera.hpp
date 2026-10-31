#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Selects which physical camera sensor to use on the hardware.
     */
    enum class CameraSelect {
        OuterLeft = 0, ///< Left exterior stereoscopic camera sensor.
        Inner = 1,     ///< Interior user-facing camera sensor.
        OuterRight = 2 ///< Right exterior stereoscopic camera sensor.
    };

    /**
     * @brief Specifies the pixel format of the captured camera frame data.
     */
    enum class CameraFormat {
        Rgb565 = 0, ///< 16-bit RGB format (5 bits red, 6 bits green, 5 bits blue).
        Yuyv = 1,   ///< YUYV YUV color space format.
        Bayer = 2   ///< Raw sensor Bayer filter mosaic format.
    };

    /**
     * @brief Defines standard preset dimensions for camera capture resolution.
     */
    enum class CameraSize {
        Vga = 0,    ///< VGA high resolution (640x480 pixels).
        Qvga = 1,   ///< QVGA standard resolution (320x240 pixels).
        Qqndv = 2   ///< QQNDV low resolution (160x120 pixels).
    };

    /**
     * @brief Singleton manager class handling hardware camera initialization, streaming, and frame retrieval.
     */
    class CameraManager {
    private:
        bool m_initialized = false; ///< Flag indicating whether the camera subsystem is initialized.
        bool m_capturing = false;   ///< Flag indicating whether a camera stream is currently active.

        /** @brief Private default constructor for singleton pattern. */
        CameraManager();

    public:
        /** @brief Deleted copy constructor. */
        CameraManager(const CameraManager&) = delete;
        
        /** @brief Deleted copy assignment operator. */
        CameraManager& operator=(const CameraManager&) = delete;

        /**
         * @brief Gets the singleton instance of the CameraManager.
         * @return Reference to the CameraManager instance.
         */
        static CameraManager& getInstance() {
            static CameraManager instance;
            return instance;
        }

        /** @brief Destructor shutting down camera streams and releasing hardware resources. */
        ~CameraManager();

        /** @brief Initializes the hardware camera subsystem. @return True on success, false otherwise. */
        bool init();
        
        /** @brief Shuts down the camera subsystem and terminates any active streams. */
        void shutdown();
        
        /** @brief Checks if the camera manager is initialized. @return True if initialized. */
        bool isInitialized() const { return m_initialized; }

        /**
         * @brief Starts a camera capture stream with specified settings.
         * @param camera Target camera sensor (default: Inner).
         * @param size Resolution preset size (default: Qvga).
         * @param format Pixel color format (default: Rgb565).
         * @return True if capture started successfully, false otherwise.
         */
        bool startCapture(CameraSelect camera = CameraSelect::Inner, CameraSize size = CameraSize::Qvga, CameraFormat format = CameraFormat::Rgb565);
        
        /** @brief Stops the currently active camera capture stream. */
        void stopCapture();
        
        /** @brief Checks if camera capture is currently active. @return True if capturing. */
        bool isCapturing() const { return m_capturing; }

        /**
     * @brief Retrieves the latest captured frame into a provided buffer.
     * @param buffer Pointer to destination byte buffer.
     * @param size Size of the destination buffer in bytes.
     * @return True if frame data was successfully copied, false otherwise.
     */
        bool getFrame(uint8_t* buffer, size_t size);
    };

}