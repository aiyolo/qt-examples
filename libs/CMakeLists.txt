project(libs)

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE UIFILES *.ui)
file(GLOB_RECURSE RCCFILES *.qrc)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${UIFILES} ${RCCFILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core
    opencv::opencv
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

