
# Collect all source files in the AudioEngine subdirectory
file(GLOB AUDIOENGINE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Create a static library for AudioEngine
add_library(AudioEngine STATIC ${AUDIOENGINE_SRC})

# Add include directories
target_include_directories(AudioEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/AudioEngine)
