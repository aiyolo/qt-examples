project(showchart1)

file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UIFILES src/*.ui)
file(GLOB_RECURSE RCCFILES src/*.qrc)

add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${UIFILES} ${RCCFILES})
target_link_libraries(${PROJECT_NAME} qt::qt
    opencv::opencv
    libs
    qcustomplot::qcustomplot
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

