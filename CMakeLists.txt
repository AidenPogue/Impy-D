cmake_minimum_required(VERSION 3.23)

project(IM-MPD)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(im-mpd)

target_include_directories(im-mpd PRIVATE include)

set_property(TARGET im-mpd PROPERTY CXX_STANDARD 20)

add_subdirectory(src)

set(ImGuiRoot "./imgui")
add_library(imgui)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(X_11 REQUIRED x11)
pkg_check_modules(FC REQUIRED fontconfig)

target_sources(imgui
  PRIVATE
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp

    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp

  PUBLIC
    FILE_SET imguiHeaders
    TYPE HEADERS
    BASE_DIRS
      imgui
    FILES
      imgui/imgui.h
      imgui/backends/imgui_impl_glfw.h
      imgui/backends/imgui_impl_opengl3.h
)

target_link_libraries(im-mpd
    PUBLIC
        imgui
        ${GLFW_LIBRARIES}
        ${X_11_LIBRARIES}
        ${FC_LIBRARIES}
        mpdclient
        GL
)

# target_include_directories(im-mpd PUBLIC ${GLFW_INCLUDE_DIRS})
target_compile_options(im-mpd PUBLIC ${GLFW_CFLAGS_OTHER} ${X_11_CFLAGS_OTHER} ${FC_CFLAGS_OTHER})