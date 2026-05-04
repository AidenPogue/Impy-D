project(IM-MPD)

target_sources(im-mpd
        PUBLIC
        IClientAction.cpp
        NoParameterAction.cpp
        NoParameterPromiseAction.cpp
        ChangeVolumeAction.cpp
        GetCurrentSongAction.cpp
        ListAction.cpp
        PromiseAction.cpp
        SeekAction.cpp
        PlayIdAction.cpp
        FindAction.cpp
)