#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Singleton manager class handling socket initialization and network operations.
     */
    class NetworkManager {
    private:
        bool m_initialized = false; ///< Initialization state flag.
        void* m_socBuffer = nullptr;  ///< Socket allocation buffer for 3DS SOC service.

        /** @brief Private default constructor for singleton pattern. */
        NetworkManager();

    public:
        /** @brief Deleted copy constructor. */
        NetworkManager(const NetworkManager&) = delete;
        
        /** @brief Deleted copy assignment operator. */
        NetworkManager& operator=(const NetworkManager&) = delete;

        /**
         * @brief Gets the singleton instance of the NetworkManager.
         * @return Reference to the NetworkManager instance.
         */
        static NetworkManager& getInstance() {
            static NetworkManager instance;
            return instance;
        }

        /** @brief Destructor shutting down networking services. */
        ~NetworkManager();

        /** @brief Initializes the 3DS socket service. @return True on success, false otherwise. */
        bool init();
        
        /** @brief Shuts down networking services and frees socket buffers. */
        void shutdown();
        
        /** @brief Checks if the network manager is initialized. @return True if initialized. */
        bool isInitialized() const { return m_initialized; }

        /** @brief Creates a new UDP socket file descriptor. @return Socket descriptor ID, or -1 on failure. */
        int createUdpSocket() const;
        
        /** @brief Creates a new TCP socket file descriptor. @return Socket descriptor ID, or -1 on failure. */
        int createTcpSocket() const;
        
        /**
         * @brief Sets or clears non-blocking mode on a socket.
         * @param sock Socket file descriptor.
         * @param nonBlocking True for non-blocking mode, false for blocking.
         * @return True on success, false otherwise.
         */
        bool setNonBlocking(int sock, bool nonBlocking) const;
        
        /** @brief Closes a socket descriptor. @param sock Socket file descriptor. */
        void closeSocket(int sock) const;
    };

}
