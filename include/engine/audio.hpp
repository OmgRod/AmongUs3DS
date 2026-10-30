#pragma once

#include <includes.hpp>

namespace engine {

    using SoundId = uint32_t;
    constexpr SoundId INVALID_SOUND_ID = 0;

    enum class SoundFormat {
        Unknown,
        Wav
    };

    struct SoundParams {
        bool loop = false;
        float volume = 1.0f;    // 0.0f to 1.0f
        float pan = 0.0f;       // -1.0f (left) to 1.0f (right), 0.0f (center)
        float pitch = 1.0f;     // 1.0f = normal speed, 0.5f = half, 2.0f = double
        std::string tag;        // Custom identifier tag (e.g. "bgm", "sfx", "ambience")
    };

    struct Sound {
        SoundId id = INVALID_SOUND_ID;
        std::string path;
        std::string tag;
        SoundFormat format = SoundFormat::Unknown;
        u8* audioBuffer = nullptr;
        uint32_t dataSize = 0;
        int channel = -1;
        uint32_t baseSampleRate = 22050;
        float volume = 1.0f;
        float pan = 0.0f;
        float pitch = 1.0f;
        bool looping = false;
        bool paused = false;
        ndspWaveBuf waveBuf;
    };

    class AudioEngine {
    private:
        std::vector<std::unique_ptr<Sound>> m_playingSounds;
        bool m_initialized = false;
        SoundId m_nextSoundId = 1;
        float m_masterVolume = 1.0f;

        AudioEngine() = default;

        Sound* findSound(SoundId id);
        const Sound* findSound(SoundId id) const;
        void applyChannelMix(Sound& sound);
        void applyChannelRate(Sound& sound);

    public:
        ~AudioEngine();
        AudioEngine(const AudioEngine&) = delete;
        AudioEngine& operator=(const AudioEngine&) = delete;

        static AudioEngine& getInstance() {
            static AudioEngine instance;
            return instance;
        }

        bool init();
        void shutdown();
        void update();
        bool isInitialized() const { return m_initialized; }

        // Playback
        SoundId playSFX(const std::string& path, bool loop = false);
        SoundId playSFX(const std::string& path, const SoundParams& params);
        SoundId playSFX(const std::string& path, float volume, float pan = 0.0f, float pitch = 1.0f, bool loop = false, const std::string& tag = "");

        // Sound Control by SoundId
        bool stop(SoundId id);
        bool pause(SoundId id);
        bool resume(SoundId id);
        bool isPlaying(SoundId id) const;
        bool isPaused(SoundId id) const;

        bool setVolume(SoundId id, float volume);
        float getVolume(SoundId id) const;

        bool setPan(SoundId id, float pan);
        float getPan(SoundId id) const;

        bool setPitch(SoundId id, float pitchMultiplier);
        float getPitch(SoundId id) const;

        bool setLooping(SoundId id, bool loop);
        bool isLooping(SoundId id) const;

        const Sound* getSound(SoundId id) const { return findSound(id); }
        int getChannel(SoundId id) const;
        std::string getTag(SoundId id) const;
        std::string getPath(SoundId id) const;

        // Hardware Channel Control (0..23)
        bool stopChannel(int channel);
        bool pauseChannel(int channel);
        bool resumeChannel(int channel);
        bool setChannelVolume(int channel, float volume);
        bool setChannelPan(int channel, float pan);
        bool setChannelPitch(int channel, float pitchMultiplier);
        SoundId getSoundIdForChannel(int channel) const;

        // Batch / Tag / Path Control
        void stopByTag(const std::string& tag);
        void pauseByTag(const std::string& tag);
        void resumeByTag(const std::string& tag);
        void setVolumeByTag(const std::string& tag, float volume);

        void stopByPath(const std::string& path);
        void pauseByPath(const std::string& path);
        void resumeByPath(const std::string& path);

        void stopAll();
        void pauseAll();
        void resumeAll();

        // Master Volume
        void setMasterVolume(float volume);
        float getMasterVolume() const { return m_masterVolume; }

        // Queries
        size_t getPlayingSoundCount() const { return m_playingSounds.size(); }
        std::vector<SoundId> getSoundIdsByTag(const std::string& tag) const;
        std::vector<SoundId> getSoundIdsByPath(const std::string& path) const;
    };

}
