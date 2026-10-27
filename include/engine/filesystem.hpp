#pragma once

#include <includes.hpp>

namespace engine {
    class Filesystem {
    public:
        Filesystem(const Filesystem&) = delete;
        Filesystem& operator=(const Filesystem&) = delete;

        static Filesystem& getInstance() {
            static Filesystem obj;
            return obj;
        }

        std::string readFileRomfs(const char* path);
        std::vector<uint8_t> readFileBinaryRomfs(const char* path);

    private:
        Filesystem() {}
    };
}