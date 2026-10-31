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
        float volume = 1.0f;    // 0.0 – 1.0
        float pan = 0.0f;       // -1.0 (left) .. 1.0 (right)
        float pitch = 1.0f;     // 1.0 = normal
        std::string tag;        // e.g. "bgm", "sfx", "ambience"
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

    class AudioEngine;

    class SoundHandle {
    public:
        SoundHandle() = default;
        explicit SoundHandle(SoundId id) : m_id(id) {}

        SoundId id()    const { return m_id; }
        bool    valid() const { return m_id != INVALID_SOUND_ID; }

        SoundHandle& stop();
        SoundHandle& pause();
        SoundHandle& resume();

        SoundHandle& setVolume(float v);
        SoundHandle& setPan(float p);
        SoundHandle& setPitch(float p);
        SoundHandle& setLooping(bool l);
        SoundHandle& setTag(const std::string& tag);

        bool isPlaying() const;
        bool isPaused()  const;
        bool isLooping() const;

        float getVolume()  const;
        float getPan()     const;
        float getPitch()   const;
        int getChannel() const;
        std::string getTag() const;
        std::string getPath() const;

    private:
        SoundId m_id = INVALID_SOUND_ID;
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

        SoundHandle playSFX(const std::string& path, bool loop = false);
        SoundHandle playSFX(const std::string& path, const SoundParams& params);
        SoundHandle playSFX(const std::string& path, float volume, float pan = 0.0f, float pitch = 1.0f, bool loop = false, const std::string& tag = "");

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

        bool setTag(SoundId id, const std::string& tag);

        const Sound* getSound(SoundId id) const { return findSound(id); }
        int getChannel(SoundId id) const;
        std::string getTag(SoundId id) const;
        std::string getPath(SoundId id) const;

        bool stopChannel(int channel);
        bool pauseChannel(int channel);
        bool resumeChannel(int channel);
        bool setChannelVolume(int channel, float volume);
        bool setChannelPan(int channel, float pan);
        bool setChannelPitch(int channel, float pitchMultiplier);
        SoundId getSoundIdForChannel(int channel) const;

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

        void setMasterVolume(float volume);
        float getMasterVolume() const { return m_masterVolume; }

        size_t getPlayingSoundCount() const { return m_playingSounds.size(); }
        std::vector<SoundId> getSoundIdsByTag(const std::string& tag) const;
        std::vector<SoundId> getSoundIdsByPath(const std::string& path) const;
    };

    inline SoundHandle& SoundHandle::stop() { AudioEngine::getInstance().stop(m_id);   return *this; }
    inline SoundHandle& SoundHandle::pause() { AudioEngine::getInstance().pause(m_id);  return *this; }
    inline SoundHandle& SoundHandle::resume() { AudioEngine::getInstance().resume(m_id); return *this; }

    inline SoundHandle& SoundHandle::setVolume(float v) { AudioEngine::getInstance().setVolume(m_id, v);  return *this; }
    inline SoundHandle& SoundHandle::setPan(float p) { AudioEngine::getInstance().setPan(m_id, p);     return *this; }
    inline SoundHandle& SoundHandle::setPitch(float p) { AudioEngine::getInstance().setPitch(m_id, p);   return *this; }
    inline SoundHandle& SoundHandle::setLooping(bool l) { AudioEngine::getInstance().setLooping(m_id, l); return *this; }
    inline SoundHandle& SoundHandle::setTag(const std::string& tag) { AudioEngine::getInstance().setTag(m_id, tag);   return *this; }

    inline bool SoundHandle::isPlaying() const { return AudioEngine::getInstance().isPlaying(m_id); }
    inline bool SoundHandle::isPaused()  const { return AudioEngine::getInstance().isPaused(m_id);  }
    inline bool SoundHandle::isLooping() const { return AudioEngine::getInstance().isLooping(m_id); }
    inline float SoundHandle::getVolume() const { return AudioEngine::getInstance().getVolume(m_id); }
    inline float SoundHandle::getPan()    const { return AudioEngine::getInstance().getPan(m_id);    }
    inline float SoundHandle::getPitch()  const { return AudioEngine::getInstance().getPitch(m_id);  }
    inline int SoundHandle::getChannel() const { return AudioEngine::getInstance().getChannel(m_id); }
    inline std::string SoundHandle::getTag() const { return AudioEngine::getInstance().getTag(m_id);    }
    inline std::string SoundHandle::getPath() const { return AudioEngine::getInstance().getPath(m_id);   }

}
