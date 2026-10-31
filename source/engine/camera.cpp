#include <engine/camera.hpp>
#include <3ds.h>

namespace engine {

    CameraManager::CameraManager() = default;

    CameraManager::~CameraManager() {
        shutdown();
    }

    bool CameraManager::init() {
        if (m_initialized) return true;
        Result res = camInit();
        if (R_FAILED(res)) {
            m_initialized = false;
            return false;
        }
        m_initialized = true;
        return true;
    }

    void CameraManager::shutdown() {
        if (!m_initialized) return;
        stopCapture();
        camExit();
        m_initialized = false;
    }

    bool CameraManager::startCapture(CameraSelect camera, CameraSize size, CameraFormat format) {
        if (!m_initialized) {
            if (!init()) return false;
        }
        if (m_capturing) return false;

        Result res = CAMU_StartCapture(1); // PORT_CAM1
        if (R_FAILED(res)) return false;

        m_capturing = true;
        return true;
    }

    void CameraManager::stopCapture() {
        if (!m_capturing) return;
        CAMU_StopCapture(1);
        m_capturing = false;
    }

    bool CameraManager::getFrame(uint8_t* buffer, size_t size) {
        if (!m_capturing || !buffer) return false;
        gspWaitForVBlank();
        return true;
    }

}
