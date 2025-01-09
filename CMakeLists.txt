cmake_minimum_required(VERSION 3.16)

project(ReproductorMusica VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar Qt y sus componentes
find_package(Qt6 COMPONENTS 
    Core 
    Gui
    Widgets 
    Multimedia
    MultimediaWidgets
REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)

# Configurar la ruta de TagLib para macOS con Homebrew
set(TAGLIB_LIBRARY "/opt/homebrew/lib/libtag.dylib")
set(TAGLIB_INCLUDE_DIR "/opt/homebrew/include/taglib")

# Configurar rutas de plugins multimedia
set(QT_PLUGIN_PATH "/opt/homebrew/opt/qt6/plugins")
set(GST_PLUGIN_PATH "/opt/homebrew/lib/gstreamer-1.0")

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/audioconverter.cpp
    src/audioconverter.h
    src/playlist.cpp
    src/playlist.h
    src/threadpool.cpp
    src/threadpool.h
    src/asynctaskmanager.cpp
    src/asynctaskmanager.h
    src/metadatamanager.cpp
    src/metadatamanager.h
)

qt_standard_project_setup()

qt_add_executable(ReproductorMusica
    ${PROJECT_SOURCES}
)

target_link_libraries(ReproductorMusica PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${TAGLIB_LIBRARY}
)

target_include_directories(ReproductorMusica PRIVATE
    ${TAGLIB_INCLUDE_DIR}
    ${QT_PLUGIN_PATH}
    ${GST_PLUGIN_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(ReproductorMusica PRIVATE
    ${TAGLIB_CFLAGS_OTHER}
)
