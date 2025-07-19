project(qtimageprocessor)

set(HEADERS
    src/imageprocessor.h
    src/imageprocessorwindow.h
    src/algorithmparams.h
    src/imagealgorithm.h
    src/bluralgorithm.h
    src/edgedetectionalgorithm.h
    src/brightnesscontrastalgorithm.h
    src/grayscalealgorithm.h
    src/algorithmfactory.h
)

set(SOURCES
    src/main.cpp
    src/imageprocessor.cpp
    src/imageprocessorwindow.cpp
    src/algorithmparams.cpp
    src/imagealgorithm.cpp
    src/bluralgorithm.cpp
    src/edgedetectionalgorithm.cpp
    src/brightnesscontrastalgorithm.cpp
    src/grayscalealgorithm.cpp
    src/algorithmfactory.cpp
)

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
