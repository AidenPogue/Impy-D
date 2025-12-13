//
// Created by aiden on 2025-12-10.
//
#include "Utils.hpp"

#include <assert.h>

#include "imgui.h"
#include "stb_image.h"

void ImMPD::Utils::FreeSongList(std::vector<mpd_song *> &songList)
{
    for (auto *song : songList)
    {
        if (song == nullptr)
        {
            continue;
        }
        mpd_song_free(song);
    }
    songList.clear();
}


// From https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users
// Simple helper function to load an image into a OpenGL texture with common settings
bool ImMPD::Utils::LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void *ImMPD::Utils::CreateOrResizeBinaryBuffer(void *buffer, size_t &currentSize, size_t newSize)
{
    if (buffer == nullptr)
    {
        buffer = IM_ALLOC(newSize);
    }
    else if (currentSize < newSize)
    {
        buffer = realloc(buffer, newSize);
    }
    assert(buffer != nullptr);
    currentSize = newSize;
    return buffer;
}

std::string ImMPD::Utils::MsToDurationString(unsigned milliseconds)
{
    unsigned ms = milliseconds;
}
