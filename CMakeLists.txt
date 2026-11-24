cmake_minimum_required(VERSION 3.16)
project(outShooting CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDLTTF_VENDORED ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()
# Create your game executable target as usual
add_executable(outShooting WIN32 outShooting.cpp creature.cpp enemy.cpp bullet.cpp stage.cpp)

# Link to the actual SDL3 library.
target_link_libraries(outShooting PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)
target_link_options(outShooting PRIVATE "--preload-file"
"${CMAKE_SOURCE_DIR}/img@img")