//
// Created by aiden on 2025-12-10.
//
#include "Utils.hpp"

#include <assert.h>
#include <chrono>

#include "imgui.h"
#include "stb_image.h"

// From https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users
// Simple helper function to load an image into a OpenGL texture with common settings
bool ImpyD::Utils::LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
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

std::string ImpyD::Utils::SecondsToDurationString(float seconds)
{
    //milliseconds = milliseconds % 1000;
    auto sec = (unsigned)seconds;
    auto minutes = sec / 60;
    auto hours = minutes / 60;
    return std::format("{}{:02}:{:02}", hours > 0 ? std::format("{:02}:", hours) : "", minutes % 60, sec % 60);
}

void ImpyD::Utils::TagSetAppend(std::vector<mpd_tag_type> &vec, mpd_tag_type newTag)
{
    for (const auto &tag : vec)
    {
        if (tag == newTag)
        {
            return;
        }
    }

    vec.emplace_back(newTag);
}

void ImpyD::Utils::TagSetUnion(std::vector<mpd_tag_type> &mainSet, std::vector<mpd_tag_type> &toUnion)
{
    for (const auto &toAdd : toUnion)
    {
        TagSetAppend(mainSet, toAdd);
    }
}
