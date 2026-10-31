#include <engine/threads.hpp>
#include <malloc.h>

namespace engine {

    struct ThreadContext {
        std::function<void()> func;
        bool* runningFlag;
    };

    static void threadEntry(void* arg) {
        auto* ctx = static_cast<ThreadContext*>(arg);
        if (ctx && ctx->func) {
            ctx->func();
        }
        if (ctx && ctx->runningFlag) {
            *ctx->runningFlag = false;
        }
        delete ctx;
    }

    WorkerThread::WorkerThread() = default;

    WorkerThread::~WorkerThread() {
        join();
    }

    bool WorkerThread::start(std::function<void()> func, int stackSize, int priority) {
        if (m_running) return false;

        auto* ctx = new ThreadContext{func, &m_running};
        m_running = true;

        s32 prio = 0;
        svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);

        m_handle = threadCreate(threadEntry, ctx, stackSize, prio - 1, -2, false);
        if (!m_handle) {
            m_running = false;
            delete ctx;
            return false;
        }

        return true;
    }

    void WorkerThread::join() {
        if (m_handle && m_running) {
            threadJoin(m_handle, U64_MAX);
            threadFree(m_handle);
            m_handle = nullptr;
            m_running = false;
        }
    }

    Mutex::Mutex() {
        LightLock_Init(&m_lock);
    }

    void Mutex::lock() {
        LightLock_Lock(&m_lock);
    }

    void Mutex::unlock() {
        LightLock_Unlock(&m_lock);
    }

    bool Mutex::tryLock() {
        return LightLock_TryLock(&m_lock) == 0;
    }

}
