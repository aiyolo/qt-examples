project(qtoutlogger)

file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UIFILES src/*.ui)
file(GLOB_RECURSE RCCFILES src/*.qrc)

add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${UIFILES} ${RCCFILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core
    opencv::opencv
    fmt::fmt
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DQT_MESSAGELOGCONTEXT
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

