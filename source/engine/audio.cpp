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
        char subchunk1Id[4];      // "fmt "
        uint32_t subchunk1Size;   // 16 for PCM
        uint16_t audioFormat;     // 1 (PCM)
        uint16_t numChannels;     // 1 = Mono, 2 = Stereo
        uint32_t sampleRate;      // e.g., 22050, 44100, 48000
        uint32_t byteRate;        // SampleRate * NumChannels * BitsPerSample / 8
        uint16_t blockAlign;      // NumChannels * BitsPerSample / 8
        uint16_t bitsPerSample;   // 8 or 16
        char subchunk2Id[4];      // "data"
        uint32_t subchunk2Size;   // Audio data size in bytes
    };

    engine::SoundFormat getSoundFormatByExtension(std::string path) {
        engine::SoundFormat sf = engine::SoundFormat::Unknown;
        if (path.ends_with(".wav")) {
            sf = engine::SoundFormat::Wav;
        }
        return sf;
    }
}

namespace engine {

AudioEngine::~AudioEngine() {
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

    size_t dataOffset = sizeof(WAVHeader);
    u32 dataSize = header.subchunk2Size;
    if (dataOffset + dataSize > fileData.size()) {
        dataSize = fileData.size() - dataOffset;
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
    ndspChnSetRate(channel, (float)header.sampleRate);

    u16 ndspFormat = NDSP_FORMAT_MONO_PCM16;
    if (header.numChannels == 2) {
        ndspFormat = (header.bitsPerSample == 8) ? NDSP_FORMAT_STEREO_PCM8 : NDSP_FORMAT_STEREO_PCM16;
    } else {
        ndspFormat = (header.bitsPerSample == 8) ? NDSP_FORMAT_MONO_PCM8 : NDSP_FORMAT_MONO_PCM16;
    }
    ndspChnSetFormat(channel, ndspFormat);

    Sound sound;
    sound.path = path;
    sound.format = format;
    sound.audioBuffer = audioBuffer;
    sound.channel = channel;

    std::memset(&sound.waveBuf, 0, sizeof(ndspWaveBuf));
    sound.waveBuf.data_vaddr = audioBuffer;
    sound.waveBuf.nsamples = dataSize / (header.numChannels * (header.bitsPerSample / 8));
    sound.waveBuf.looping = loop;
    sound.waveBuf.fmt_type = ndspFormat;

    ndspChnWaveBufAdd(channel, &sound.waveBuf);

    this->m_playingSounds.push_back(sound);
    std::printf("Playing SFX (Looping: %s): %s\n", loop ? "true" : "false", path.c_str());
}

}