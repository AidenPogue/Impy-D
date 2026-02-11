#ifndef IM_MPD_UTILS_H
#define IM_MPD_UTILS_H
#include <string>

#include <GL/gl.h>
#include "Mpd/MpdClientWrapper.hpp"
#include <future>

namespace ImpyD::Utils
{
    /**
     * Uploads a raw RGBA image to the gpu.
     * @param data
     * @param width
     * @param height
     * @param out_texture
     * @return
     */
    bool UploadTexture(const void* data, int width, int height, GLuint* out_texture);


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

    std::vector<std::unique_ptr<TitleFormatting::ITagged>> ReceiveSongList(mpd_connection *connection);

    /**
     * Checks if a future is ready without blocking.
     * @tparam T
     * @param future
     * @return Whether the future is ready.
     */
    template <class T>
    bool IsReady(const std::future<T> &future)
    {
        return future.valid() && future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }



}

#endif //IM_MPD_UTILS_H