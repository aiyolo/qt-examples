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

find_package(Qt5)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core)

target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")


