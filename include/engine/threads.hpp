#pragma once

#include <includes.hpp>
#include <functional>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Wrapper class managing a background thread execution worker on 3DS.
     */
    class WorkerThread {
    private:
        ::Thread m_handle = nullptr; ///< 3DS thread handle.
        bool m_running = false;      ///< Flag indicating whether the thread is currently executing.

    public:
        /** @brief Constructs a worker thread instance. */
        WorkerThread();
        
        /** @brief Destructor ensuring thread joins/releases resources. */
        ~WorkerThread();

        /**
         * @brief Starts executing a function on a new background thread.
         * @param func Std::function callback to execute.
         * @param stackSize Stack size in bytes (default: 0x4000).
         * @param priority Thread priority level (default: 0x3F).
         * @return True if thread started successfully, false otherwise.
         */
        bool start(std::function<void()> func, int stackSize = 0x4000, int priority = 0x3F);
        
        /** @brief Blocks execution until the worker thread completes and joins. */
        void join();
        
        /** @brief Checks if the worker thread is currently running. @return True if running. */
        bool isRunning() const { return m_running; }
    };

    /**
     * @brief Synchronization primitive wrapping 3DS LightLock.
     */
    class Mutex {
    private:
        LightLock m_lock; ///< Underlying 3DS light lock structure.

    public:
        /** @brief Constructs and initializes a mutex lock. */
        Mutex();
        
        /** @brief Default destructor. */
        ~Mutex() = default;

        /** @brief Acquires exclusive ownership of the mutex, blocking until available. */
        void lock();
        
        /** @brief Releases exclusive ownership of the mutex. */
        void unlock();
        
        /**
         * @brief Attempts to acquire the mutex without blocking.
         * @return True if successfully acquired, false if already locked.
         */
        bool tryLock();
    };

}
