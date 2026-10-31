#pragma once

#include <includes.hpp>

/**
 * @brief Main engine namespace.
 */
namespace engine {

    /**
     * @brief Singleton manager class handling 3DS Near Field Communication (NFC) tag detection and reading.
     */
    class NfcManager {
    private:
        bool m_initialized = false; ///< Initialization state flag.
        bool m_tagFound = false;    ///< Flag indicating whether an NFC tag is currently detected.

        /** @brief Private default constructor for singleton pattern. */
        NfcManager();

    public:
        /** @brief Deleted copy constructor. */
        NfcManager(const NfcManager&) = delete;
        
        /** @brief Deleted copy assignment operator. */
        NfcManager& operator=(const NfcManager&) = delete;

        /**
         * @brief Gets the singleton instance of the NfcManager.
         * @return Reference to the NfcManager instance.
         */
        static NfcManager& getInstance() {
            static NfcManager instance;
            return instance;
        }

        /** @brief Destructor shutting down NFC hardware resources. */
        ~NfcManager();

        /** @brief Initializes the NFC hardware subsystem. @return True on success, false otherwise. */
        bool init();
        
        /** @brief Shuts down the NFC subsystem. */
        void shutdown();
        
        /** @brief Checks if the NFC manager is initialized. @return True if initialized. */
        bool isInitialized() const { return m_initialized; }

        /** @brief Updates NFC state and checks for tag presence. @return True if tag state changed. */
        bool update();
        
        /** @brief Checks if an NFC tag is currently detected. @return True if tag found. */
        bool isTagFound() const { return m_tagFound; }

        /**
         * @brief Reads data payload from the detected NFC tag.
         * @param outData Reference to byte vector receiving tag data.
         * @return True on successful read, false otherwise.
         */
        bool readTagData(std::vector<uint8_t>& outData);
    };

}
