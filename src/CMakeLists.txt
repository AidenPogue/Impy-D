project(IM-MPD)

target_sources(im-mpd
    PUBLIC 
        main.cpp
        Mpd/MpdClientWrapper.cpp
        Utils.cpp
        stb_image.cpp
        MainWindow.cpp
        Config.cpp
        ConfigLoader.cpp
)

add_subdirectory(panels)
add_subdirectory(TitleFormatting)
add_subdirectory(PanelFactory)
add_subdirectory(Mpd)