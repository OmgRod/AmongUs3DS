#include <engine/audio.hpp>
#include <engine/filesystem.hpp>
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace {
    struct WAVHeader {
        char chunkId[4];          // "RIFF"
        uint32_t chunkSize;       // File size - 8
        char format[4];           // "WAVE"
    };

    bool hasWavExtension(const std::string& path) {
        if (path.length() >= 4) {
            std::string ext = path.substr(path.length() - 4);
            return (ext == ".wav" || ext == ".WAV");
        }
        return false;
    }

    engine::SoundFormat getSoundFormatByExtension(std::string path) {
        engine::SoundFormat sf = engine::SoundFormat::Unknown;
        if (hasWavExtension(path)) {
            sf = engine::SoundFormat::Wav;
        }
        return sf;
    }
}

namespace engine {

AudioEngine::~AudioEngine() {
    shutdown();
}

void AudioEngine::shutdown() {
    for (auto& sound : m_playingSounds) {
        if (sound.channel >= 0) {
            ndspChnReset(sound.channel);
        }
        if (sound.audioBuffer) {
            linearFree(sound.audioBuffer);
        }
    }
    m_playingSounds.clear();
}

void AudioEngine::update() {
    for (auto it = m_playingSounds.begin(); it != m_playingSounds.end(); ) {
        if (it->channel >= 0 && !it->waveBuf.looping && it->waveBuf.status == NDSP_WBUF_DONE) {
            ndspChnReset(it->channel);
            if (it->audioBuffer) {
                linearFree(it->audioBuffer);
            }
            it = m_playingSounds.erase(it);
        } else {
            ++it;
        }
    }
}

void AudioEngine::playSFX(std::string path, bool loop) {
    update();

    SoundFormat format = getSoundFormatByExtension(path);
    if (format != SoundFormat::Wav) {
        std::printf("AudioEngine: Unsupported or unknown format for %s\n", path.c_str());
        return;
    }

    std::vector<uint8_t> fileData = Filesystem::getInstance().readFileBinaryRomfs(path.c_str());
    if (fileData.empty() || fileData.size() < sizeof(WAVHeader)) {
        std::printf("AudioEngine: Failed to read or invalid WAV file: %s\n", path.c_str());
        return;
    }

    WAVHeader header;
    std::memcpy(&header, fileData.data(), sizeof(WAVHeader));

    if (std::strncmp(header.chunkId, "RIFF", 4) != 0 || 
        std::strncmp(header.format, "WAVE", 4) != 0) {
        std::printf("AudioEngine: Invalid WAV file structure: %s\n", path.c_str());
        return;
    }

    uint32_t sampleRate = 22050;
    uint16_t numChannels = 2;
    uint16_t bitsPerSample = 16;
    size_t dataOffset = 0;
    uint32_t dataSize = 0;

    size_t cursor = 12;
    while (cursor < fileData.size() - 8) {
        char chunkId[4];
        uint32_t chunkSize;
        std::memcpy(chunkId, &fileData[cursor], 4);
        std::memcpy(&chunkSize, &fileData[cursor + 4], 4);

        if (std::strncmp(chunkId, "fmt ", 4) == 0) {
            std::memcpy(&numChannels, &fileData[cursor + 10], 2);
            std::memcpy(&sampleRate, &fileData[cursor + 12], 4);
            std::memcpy(&bitsPerSample, &fileData[cursor + 22], 2);
        } else if (std::strncmp(chunkId, "data", 4) == 0) {
            dataOffset = cursor + 8;
            dataSize = chunkSize;
            break;
        }
        cursor += 8 + chunkSize;
    }

    if (dataOffset == 0 || dataOffset + dataSize > fileData.size()) {
        std::printf("AudioEngine: Could not find valid 'data' chunk in WAV: %s\n", path.c_str());
        return;
    }

    u8* audioBuffer = (u8*)linearAlloc(dataSize);
    if (!audioBuffer) {
        std::printf("AudioEngine: Failed to allocate linear memory for audio.\n");
        return;
    }

    std::memcpy(audioBuffer, fileData.data() + dataOffset, dataSize);
    DSP_FlushDataCache(audioBuffer, dataSize);

    int channel = -1;
    for (int i = 0; i < 24; i++) {
        if (!ndspChnIsPlaying(i)) {
            channel = i;
            break;
        }
    }

    if (channel == -1) {
        std::printf("AudioEngine: No free NDSP channels available!\n");
        linearFree(audioBuffer);
        return;
    }

    ndspChnReset(channel);
    ndspChnSetInterp(channel, NDSP_INTERP_LINEAR);
    ndspChnSetRate(channel, (float)sampleRate);

    u16 ndspFormat = NDSP_FORMAT_MONO_PCM16;
    if (numChannels == 2) {
        ndspFormat = (bitsPerSample == 8) ? NDSP_FORMAT_STEREO_PCM8 : NDSP_FORMAT_STEREO_PCM16;
    } else {
        ndspFormat = (bitsPerSample == 8) ? NDSP_FORMAT_MONO_PCM8 : NDSP_FORMAT_MONO_PCM16;
    }
    
    ndspChnSetFormat(channel, ndspFormat);

    Sound sound;
    sound.path = path;
    sound.format = format;
    sound.audioBuffer = audioBuffer;
    sound.channel = channel;

    std::memset(&sound.waveBuf, 0, sizeof(ndspWaveBuf));
    sound.waveBuf.data_vaddr = audioBuffer;
    sound.waveBuf.nsamples = dataSize / (numChannels * (bitsPerSample / 8));
    sound.waveBuf.looping = loop;

    ndspChnWaveBufAdd(channel, &sound.waveBuf);

    this->m_playingSounds.push_back(sound);
    std::printf("Playing SFX (Looping: %s): %s\n", loop ? "true" : "false", path.c_str());
}

}