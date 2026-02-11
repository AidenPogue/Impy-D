//
// Created by aiden on 2025-12-10.
//
#include "Utils.hpp"

#include <assert.h>
#include <chrono>

#include "imgui.h"
#include "stb_image.h"


bool ImpyD::Utils::UploadTexture(const void* data, int width, int height, GLuint* out_texture)
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    *out_texture = image_texture;

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

std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>> ImpyD::Utils::ReceiveSongList(mpd_connection *connection)
{
    std::vector<std::unique_ptr<TitleFormatting::ITagged>> list;

    auto curSong = mpd_recv_song(connection);
    while (curSong != nullptr)
    {
        list.emplace_back(std::make_unique<MpdSongWrapper>(curSong));
        curSong = mpd_recv_song(connection);
    }

    return list;
}