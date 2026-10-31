#pragma once

#include <includes.hpp>
#include <functional>

namespace engine {

    class WorkerThread {
    private:
        ::Thread m_handle = nullptr;
        bool m_running = false;

    public:
        WorkerThread();
        ~WorkerThread();

        bool start(std::function<void()> func, int stackSize = 0x4000, int priority = 0x3F);
        void join();
        bool isRunning() const { return m_running; }
    };

    class Mutex {
    private:
        LightLock m_lock;

    public:
        Mutex();
        ~Mutex() = default;

        void lock();
        void unlock();
        bool tryLock();
    };

}
