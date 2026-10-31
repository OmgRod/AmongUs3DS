#pragma once

#include <includes.hpp>

namespace engine {

    class NfcManager {
    private:
        bool m_initialized = false;
        bool m_tagFound = false;

        NfcManager();

    public:
        NfcManager(const NfcManager&) = delete;
        NfcManager& operator=(const NfcManager&) = delete;

        static NfcManager& getInstance() {
            static NfcManager instance;
            return instance;
        }

        ~NfcManager();

        bool init();
        void shutdown();
        bool isInitialized() const { return m_initialized; }

        bool update();
        bool isTagFound() const { return m_tagFound; }
        bool readTagData(std::vector<uint8_t>& outData);
    };

}
