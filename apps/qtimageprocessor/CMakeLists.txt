project(qtimageprocessor)

# Find required packages
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)

# Enable Qt5 automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Automatically find all source files
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UIFILES src/*.ui)
file(GLOB_RECURSE RCCFILES src/*.qrc)

add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${UIFILES} ${RCCFILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core
    opencv::opencv
    libs
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()
