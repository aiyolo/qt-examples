project(qtimagebrowser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 使用GLOB收集源文件
file(GLOB SOURCES "src/*.cpp" "src/*.h")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接库
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    libs
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

