
# Collect all source files in the App subdirectory
file(GLOB APP_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Create a static library for App
add_library(App STATIC ${APP_SRC})

# Add include directories
target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/App)

# Link AudioEngine if App depends on it
target_link_libraries(App PRIVATE AudioEngine)
