cmake_minimum_required(VERSION 3.10)

project(LDtkLoader)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# temporary
file(GLOB_RECURSE Src src/*.cpp)

add_library(LDtkLoader STATIC ${Src})
target_include_directories(LDtkLoader PRIVATE include)
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 11
    ARCHIVE_OUTPUT_DIRECTORY lib
)

option(BUILD_SFML_EXAMPLE "Build an example using LDtkLoader with SFML." OFF)
option(BUILD_SDL_EXAMPLE  "Build an example using LDtkLoader with SDL."  OFF)

if (BUILD_SFML_EXAMPLE)
    message(STATUS "Will build LDtkLoader SFML example")
    add_subdirectory(examples/SFML)
endif()

if (BUILD_SDL_EXAMPLE)
    message(STATUS "Will build LDtkLoader SDL example")
    add_subdirectory(examples/SDL)
endif()

if (BUILD_SDL_EXAMPLE OR BUILD_SFML_EXAMPLE)
    message(STATUS "Will build LDtkLoader console example")
    add_subdirectory(examples/console)
endif()
