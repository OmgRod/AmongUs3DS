#pragma once

#include <includes.hpp>

namespace engine {

    class Time {
    private:
        uint64_t m_lastTick = 0;
        float m_deltaTime = 0.0f;
        float m_elapsedTime = 0.0f;
        uint32_t m_frameCount = 0;
        float m_fps = 0.0f;
        float m_fpsTimer = 0.0f;

        Time();

    public:
        Time(const Time&) = delete;
        Time& operator=(const Time&) = delete;

        static Time& getInstance() {
            static Time instance;
            return instance;
        }

        void update();

        float getDeltaTime() const { return m_deltaTime; }
        float getElapsedTime() const { return m_elapsedTime; }
        float getFPS() const { return m_fps; }
    };

}
