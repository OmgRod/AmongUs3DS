#include <engine/time.hpp>

namespace engine {

    Time::Time() {
        m_lastTick = svcGetSystemTick();
    }

    void Time::update() {
        uint64_t currentTick = svcGetSystemTick();
        uint64_t deltaTicks = currentTick - m_lastTick;
        m_lastTick = currentTick;

        m_deltaTime = static_cast<float>(deltaTicks) / 268111856.0f;
        if (m_deltaTime > 0.1f) m_deltaTime = 0.1f;

        m_elapsedTime += m_deltaTime;
        m_frameCount++;
        m_fpsTimer += m_deltaTime;

        if (m_fpsTimer >= 1.0f) {
            m_fps = static_cast<float>(m_frameCount) / m_fpsTimer;
            m_frameCount = 0;
            m_fpsTimer = 0.0f;
        }
    }

}
