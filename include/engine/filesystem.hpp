#pragma once

#include <includes.hpp>

namespace engine {
    class Filesystem {
    public:
        Filesystem(const Filesystem&) = delete;
        Filesystem& operator=(const Filesystem&) = delete;

        static Filesystem& getInstance() {
            static Filesystem instance;
            return instance;
        }

        std::string resolvePath(const std::string& path) const {
            if (path.rfind("romfs:/", 0) == 0) {
                return "sdmc:/3ds/AmongUs3DS/assets/" + path.substr(7);
            }
            return path;
        }

        std::string readFileRomfs(const char* path);
        std::vector<uint8_t> readFileBinaryRomfs(const char* path);

    private:
        Filesystem() = default;
    };
}
