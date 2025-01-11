cmake_minimum_required(VERSION 3.16)

project(ReproductorMusica VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(TAGLIB REQUIRED taglib)

qt_standard_project_setup()

qt_add_executable(ReproductorMusica
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/metadatamanager.cpp
    src/metadatamanager.h
    src/gstreamerplayer.cpp
    src/gstreamerplayer.h
)

target_link_libraries(ReproductorMusica PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${GSTREAMER_LIBRARIES}
    ${GSTREAMER_AUDIO_LIBRARIES}
    ${TAGLIB_LIBRARIES}
)

target_include_directories(ReproductorMusica PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_AUDIO_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_compile_options(ReproductorMusica PRIVATE
    ${GSTREAMER_CFLAGS_OTHER}
    ${GSTREAMER_AUDIO_CFLAGS_OTHER}
    ${TAGLIB_CFLAGS_OTHER}
)

target_link_directories(ReproductorMusica PRIVATE
    ${GSTREAMER_LIBRARY_DIRS}
    ${GSTREAMER_AUDIO_LIBRARY_DIRS}
    ${TAGLIB_LIBRARY_DIRS}
)

set_target_properties(ReproductorMusica PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
