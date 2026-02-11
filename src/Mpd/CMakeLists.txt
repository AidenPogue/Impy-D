project(IM-MPD)

target_sources(im-mpd
        PUBLIC
        MpdClientWrapper.cpp
        MpdSongWrapper.cpp
        ArbitraryTagged.cpp
        IFilterGenerator.cpp
        TagFilterGenerator.cpp
        UriFilterGenerator.cpp
        AlbumArtHelper.cpp
        IdleClientWrapper.cpp
        ConnectionManager.cpp
        AlbumArtClient.cpp
)

add_subdirectory(ClientActions)