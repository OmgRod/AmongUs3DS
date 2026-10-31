#pragma once

#include <includes.hpp>

namespace engine {

    class NetworkManager {
    private:
        bool m_initialized = false;
        void* m_socBuffer = nullptr;

        NetworkManager();

    public:
        NetworkManager(const NetworkManager&) = delete;
        NetworkManager& operator=(const NetworkManager&) = delete;

        static NetworkManager& getInstance() {
            static NetworkManager instance;
            return instance;
        }

        ~NetworkManager();

        bool init();
        void shutdown();
        bool isInitialized() const { return m_initialized; }

        int createUdpSocket() const;
        int createTcpSocket() const;
        bool setNonBlocking(int sock, bool nonBlocking) const;
        void closeSocket(int sock) const;
    };

}
