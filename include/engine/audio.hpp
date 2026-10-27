#pragma once

#include <includes.hpp>

namespace engine {

    enum class SoundFormat {
        Unknown,
        Wav
    };

    struct Sound {
        std::string path;
        SoundFormat format;
        u8* audioBuffer = nullptr;
        int channel = -1;
        ndspWaveBuf waveBuf;
    };

    class AudioEngine {
    private:
        std::vector<std::unique_ptr<Sound>> m_playingSounds;
        bool m_initialized = false;

        AudioEngine() = default;

    public:
        ~AudioEngine();
        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete;

        static AudioEngine& getInstance() {
            static AudioEngine instance;
            return instance;
        }

        bool init();
        void update();
        void playSFX(std::string path, bool loop = false);
        void shutdown();
        bool isInitialized() const { return m_initialized; }
    };

}