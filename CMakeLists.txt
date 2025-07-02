cmake_minimum_required(VERSION 3.15)
project(app CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UIFILES src/*.ui)
file(GLOB_RECURSE RCCFILES src/*.qrc)

add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${UIFILES} ${RCCFILES})

if(WIN32)
    if(DEFINED ENV{QT5_ROOT})
        set(CMAKE_PREFIX_PATH "${QT5_ROOT}")
    endif()

    find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core)

else()
    if(APPLE)
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/opt/homebrew/opt/qt/lib")
    endif()

    find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Gui Qt6::Core)
endif()

# Removed invalid target compile options
