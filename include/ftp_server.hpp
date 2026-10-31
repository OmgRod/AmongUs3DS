#pragma once

#include <includes.hpp>
#include <string>
#include <thread>
#include <atomic>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

namespace engine {
    class FtpServer {
    private:
        int m_serverSocket = -1;
        int m_port = 5000;
        std::atomic<bool> m_running{false};
        std::atomic<bool> m_transferComplete{false};
        std::atomic<size_t> m_bytesReceived{0};
        std::atomic<size_t> m_totalBytes{1024 * 1024}; // default estimate
        Thread m_thread;
        std::string m_targetFilePath;

        static void threadEntry(void *arg);
        void run();

    public:
        FtpServer(int port, const std::string &targetFilePath);
        ~FtpServer();

        bool start();
        void stop();
        bool isTransferComplete() const { return m_transferComplete; }
        size_t getBytesReceived() const { return m_bytesReceived; }
        size_t getTotalBytes() const { return m_totalBytes; }
        float getProgress() const {
            if (m_totalBytes == 0) return 0.0f;
            float p = (float)m_bytesReceived / (float)m_totalBytes;
            return p > 1.0f ? 1.0f : p;
        }
    };
}
