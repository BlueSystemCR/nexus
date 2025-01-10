cmake_minimum_required(VERSION 3.16)

project(ReproductorMusica VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)

# Configurar TagLib para macOS con Homebrew
set(TAGLIB_ROOT "/opt/homebrew")
set(TAGLIB_LIBRARIES "${TAGLIB_ROOT}/lib/libtag.dylib")
set(TAGLIB_INCLUDE_DIRS "${TAGLIB_ROOT}/include/taglib")

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/metadatamanager.h
        src/metadatamanager.cpp
)

add_executable(ReproductorMusica
    ${PROJECT_SOURCES}
)

target_include_directories(ReproductorMusica PRIVATE ${TAGLIB_INCLUDE_DIRS})
target_link_libraries(ReproductorMusica PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    ${TAGLIB_LIBRARIES}
)
