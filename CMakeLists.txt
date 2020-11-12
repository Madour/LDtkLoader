cmake_minimum_required(VERSION 3.10)

project(LDtkLoader)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# temporary
file(GLOB_RECURSE Src src/*.cpp)

add_library(LDtkLoader-s STATIC ${Src})
target_include_directories(LDtkLoader-s PRIVATE include)

add_subdirectory(examples/console)

option(BUILD_SFML_EXAMPLE "Build an example using LDtkLoader with SFML." OFF)
option(BUILD_SDL_EXAMPLE "Build an example using LDtkLoader with SDL." OFF)

if (BUILD_SFML_EXAMPLE)
    message("\n-- [INFO] Will build SFML example")
    add_subdirectory(examples/SFML)
endif()

if (BUILD_SDL_EXAMPLE)
    message("\n-- [INFO] SDL example does not exist yet.")
endif()
