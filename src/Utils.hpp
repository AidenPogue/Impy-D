#ifndef IM_MPD_UTILS_H
#define IM_MPD_UTILS_H
#include <string>

#include "Mpd/MpdClientWrapper.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImpyD::Utils
{
    void FreeSongList(std::vector<mpd_song *> &songList);
    bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);
    void *CreateOrResizeBinaryBuffer(void *buffer, size_t &currentSize, size_t newSize);
    std::string SecondsToDurationString(float seconds);
    /**
     * Appends @p newTag to @p vec if @p vec does not contain it, otherwise does nothing.
     * @param vec Set to append to.
     * @param newTag New tag to append if not already in set.
     */
    void TagSetAppend(std::vector<mpd_tag_type> &vec, mpd_tag_type newTag);

    /**
     * Performs a union between 2 tag sets by repeatedly calling @ref TagSetAppend.
     * @param mainSet The set that will be modified.
     * @param toUnion The tags that will be unioned with @p mainSet.
     */
    void TagSetUnion(std::vector<mpd_tag_type> &mainSet, std::vector<mpd_tag_type> &toUnion);
}

#endif //IM_MPD_UTILS_H