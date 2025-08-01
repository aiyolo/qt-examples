# Database Admin Application
set(TARGET_NAME databaseadmin)

# Find required Qt packages
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Sql Concurrent)

# Collect source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/database/databasemanager.cpp
    src/models/tablemodel.cpp
    src/delegates/checkboxdelegate.cpp
    src/delegates/buttondelegate.cpp
)

set(HEADERS
    src/mainwindow.h
    src/database/databasemanager.h
    src/models/tablemodel.h
    src/delegates/checkboxdelegate.h
    src/delegates/buttondelegate.h
)

# Create executable
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${TARGET_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Sql
    Qt5::Concurrent
)

# Set include directories
target_include_directories(${TARGET_NAME} PRIVATE src)

# Install executable
install(TARGETS ${TARGET_NAME} DESTINATION bin)
