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
find_package(mdns)
find_package(qcustomplot)
find_package(nlohmann_json)

add_subdirectory(apps)
add_subdirectory(libs)
