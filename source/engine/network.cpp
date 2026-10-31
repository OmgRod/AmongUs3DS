#include <engine/network.hpp>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

namespace engine {

    NetworkManager::NetworkManager() = default;

    NetworkManager::~NetworkManager() {
        shutdown();
    }

    bool NetworkManager::init() {
        if (m_initialized) return true;

        m_socBuffer = memalign(0x1000, 0x100000);
        if (!m_socBuffer) return false;

        Result res = socInit(static_cast<u32*>(m_socBuffer), 0x100000);
        if (R_FAILED(res)) {
            free(m_socBuffer);
            m_socBuffer = nullptr;
            return false;
        }

        m_initialized = true;
        return true;
    }

    void NetworkManager::shutdown() {
        if (!m_initialized) return;

        socExit();
        if (m_socBuffer) {
            free(m_socBuffer);
            m_socBuffer = nullptr;
        }
        m_initialized = false;
    }

    int NetworkManager::createUdpSocket() const {
        return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    int NetworkManager::createTcpSocket() const {
        return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    bool NetworkManager::setNonBlocking(int sock, bool nonBlocking) const {
        if (sock < 0) return false;
        int flags = fcntl(sock, F_GETFL, 0);
        if (flags < 0) return false;
        if (nonBlocking) {
            flags |= O_NONBLOCK;
        } else {
            flags &= ~O_NONBLOCK;
        }
        return fcntl(sock, F_SETFL, flags) >= 0;
    }

    void NetworkManager::closeSocket(int sock) const {
        if (sock >= 0) {
            close(sock);
        }
    }

}
