#pragma once

#include <includes.hpp>

namespace engine {
    enum SoundFormat {
        Unknown,
        Wav
    };

    struct Sound {
        std::string path;
        SoundFormat format;
        u8* audioBuffer = nullptr;
        ndspWaveBuf waveBuf;
        int channel = -1;
    };

    class AudioEngine {
    public:
        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete;
        
        static AudioEngine& getInstance() {
            static AudioEngine obj;
            return obj;
        }

        void playSFX(std::string path, bool loop = false);
        void update();

        void shutdown();

    private:
        AudioEngine() {}
        ~AudioEngine();

        std::vector<Sound> m_playingSounds;
    };
}