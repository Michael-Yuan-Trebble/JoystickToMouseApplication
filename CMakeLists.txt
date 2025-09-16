cmake_minimum_required(VERSION 3.21)
project(JoystickMouse VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------
# Find Qt6 Widgets
# ------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable AUTOMOC, AUTOUIC, AUTORCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_ AUTORCC ON)

# ------------------------
# Add Exec
# ------------------------
add_executable(JoystickMouse
    JoystickMouse.cpp
 "Windows/MainWindow.cpp" 
 "Windows/MainWindow.h" "Windows/Navigator.cpp" "Windows/Navigator.h" "Windows/ActivateWindow.cpp" "Windows/ActivateWindow.h")

# ------------------------
# Link Qt Lib
# ------------------------
target_link_libraries(JoystickMouse PRIVATE Qt6::Widgets)

# ------------------------
# Include Dir
# ------------------------
target_include_directories(JoystickMouse PRIVATE
    ${CMAKE_SOURCE_DIR}/cmake/include
)

# ------------------------
# SDL3 Lib
# ------------------------
target_link_directories(JoystickMouse PRIVATE
    ${CMAKE_SOURCE_DIR}/cmake/lib/x64
)

target_link_libraries(JoystickMouse PRIVATE SDL3.lib)

# ------------------------
# Copy DLL
# ------------------------
if (WIN32)
    add_custom_command(TARGET JoystickMouse POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/cmake/lib/x64/SDL3.dll"
            $<TARGET_FILE_DIR:JoystickMouse>)
endif()