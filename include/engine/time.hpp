#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Singleton time management class tracking frame delta time, elapsed time, and FPS.
     */
    class Time {
    private:
        uint64_t m_lastTick = 0;       ///< Hardware timer tick value from previous frame.
        float m_deltaTime = 0.0f;      ///< Time elapsed since the last frame in seconds.
        float m_elapsedTime = 0.0f;    ///< Total time elapsed since engine initialization in seconds.
        uint32_t m_frameCount = 0;     ///< Total frame counter.
        float m_fps = 0.0f;            ///< Current calculated frames per second.
        float m_fpsTimer = 0.0f;       ///< Internal accumulator timer for calculating FPS.

        /** @brief Private default constructor for singleton pattern. */
        Time();

    public:
        /** @brief Deleted copy constructor. */
        Time(const Time&) = delete;
        
        /** @brief Deleted copy assignment operator. */
        Time& operator=(const Time&) = delete;

        /**
         * @brief Gets the singleton instance of the Time manager.
         * @return Reference to the Time instance.
         */
        static Time& getInstance() {
            static Time instance;
            return instance;
        }

        /** @brief Updates delta time, elapsed time, and FPS counters each frame. */
        void update();

        /** @brief Gets the delta time between the current and previous frame. @return Delta time in seconds. */
        float getDeltaTime() const { return m_deltaTime; }
        
        /** @brief Gets the total time elapsed since startup. @return Elapsed time in seconds. */
        float getElapsedTime() const { return m_elapsedTime; }
        
        /** @brief Gets the current frames per second (FPS). @return FPS value. */
        float getFPS() const { return m_fps; }
    };

}
