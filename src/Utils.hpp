//
// Created by aiden on 2025-12-10.
//

#ifndef IM_MPD_UTILS_H
#define IM_MPD_UTILS_H
#include "MpdClientWrapper.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImMPD::Utils
{
    void FreeSongList(std::vector<mpd_song *> &songList);
    bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);
    void *CreateOrResizeBinaryBuffer(void *buffer, size_t &currentSize, size_t newSize);
    std::string MsToDurationString(unsigned milliseconds);
}

#endif //IM_MPD_UTILS_H