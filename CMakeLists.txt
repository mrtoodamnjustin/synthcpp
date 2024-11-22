cmake_minimum_required(VERSION 3.5)

# Project name and version
project(synthcpp VERSION 0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Add subdirectories
add_subdirectory(src/AudioEngine)
add_subdirectory(src/App)

# Link the components into the final executable
add_executable(synthcpp src/main.cpp)

# Link libraries (e.g., linking AudioEngine and App to the main executable)
target_link_libraries(synthcpp PRIVATE Qt6::Core Qt6::Widgets AudioEngine App)
