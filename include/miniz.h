#ifndef MINIZ_H
#define MINIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

inline void create_directories(const char *path) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);
    size_t len = strlen(tmp);
    for (size_t i = 0; i < len; i++) {
        if (tmp[i] == '/' && i > 0) {
            tmp[i] = '\0';
            mkdir(tmp, 0777);
            tmp[i] = '/';
        }
    }
    mkdir(tmp, 0777);
}

inline bool extract_zip(const char *zip_path, const char *dest_dir) {
    FILE *f = fopen(zip_path, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    while (ftell(f) < file_size) {
        uint32_t sig = 0;
        if (fread(&sig, 1, 4, f) != 4) break;
        if (sig != 0x04034b50) break;

        uint16_t min_version = 0, flags = 0, method = 0, mod_time = 0, mod_date = 0;
        uint32_t crc32 = 0, comp_size = 0, uncomp_size = 0;
        uint16_t filename_len = 0, extra_len = 0;

        fread(&min_version, 1, 2, f);
        fread(&flags, 1, 2, f);
        fread(&method, 1, 2, f);
        fread(&mod_time, 1, 2, f);
        fread(&mod_date, 1, 2, f);
        fread(&crc32, 1, 4, f);
        fread(&comp_size, 1, 4, f);
        fread(&uncomp_size, 1, 4, f);
        fread(&filename_len, 1, 2, f);
        fread(&extra_len, 1, 2, f);

        char filename[512];
        if (filename_len >= sizeof(filename)) filename_len = sizeof(filename) - 1;
        fread(filename, 1, filename_len, f);
        filename[filename_len] = '\0';

        if (extra_len > 0) fseek(f, extra_len, SEEK_CUR);

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dest_dir, filename);

        bool is_dir = (filename[filename_len - 1] == '/' || filename[filename_len - 1] == '\\');
        if (is_dir) {
            create_directories(full_path);
        } else {
            char parent_dir[1024];
            snprintf(parent_dir, sizeof(parent_dir), "%s", full_path);
            char *last_slash = strrchr(parent_dir, '/');
            if (last_slash) {
                *last_slash = '\0';
                create_directories(parent_dir);
            }

            FILE *out = fopen(full_path, "wb");
            if (out) {
                if (method == 0) {
                    uint32_t remaining = comp_size;
                    char buf[4096];
                    while (remaining > 0) {
                        uint32_t chunk = remaining > sizeof(buf) ? sizeof(buf) : remaining;
                        size_t read = fread(buf, 1, chunk, f);
                        if (read == 0) break;
                        fwrite(buf, 1, read, out);
                        remaining -= read;
                    }
                } else {
                    fseek(f, comp_size, SEEK_CUR);
                }
                fclose(out);
            } else {
                fseek(f, comp_size, SEEK_CUR);
            }
        }
    }

    fclose(f);
    return true;
}

#endif // MINIZ_H
