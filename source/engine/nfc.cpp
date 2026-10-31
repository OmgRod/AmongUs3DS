#include <engine/nfc.hpp>

namespace engine {

    NfcManager::NfcManager() = default;

    NfcManager::~NfcManager() {
        shutdown();
    }

    bool NfcManager::init() {
        if (m_initialized) return true;
        Result res = nfcInit((NFC_OpType)0);
        if (R_FAILED(res)) {
            m_initialized = false;
            return false;
        }
        m_initialized = true;
        return true;
    }

    void NfcManager::shutdown() {
        if (!m_initialized) return;
        nfcExit();
        m_initialized = false;
        m_tagFound = false;
    }

    bool NfcManager::update() {
        if (!m_initialized) return false;
        return m_tagFound;
    }

    bool NfcManager::readTagData(std::vector<uint8_t>& outData) {
        if (!m_initialized || !m_tagFound) return false;
        return false;
    }

}
