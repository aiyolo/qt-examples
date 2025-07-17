cmake_minimum_required(VERSION 3.15)
project(app)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5)
find_package(OpenCV)
find_package(fmt)

add_subdirectory(qtimageviewer)
add_subdirectory(qtoutlogger)
add_subdirectory(imagestorageservice)
add_subdirectory(colordetector)


