cmake_minimum_required(VERSION 3.10)

project(LDtkLoader)

# get source files
file(GLOB_RECURSE Src src/*.cpp)

# create the static lib target
add_library(LDtkLoader STATIC ${Src})
target_include_directories(LDtkLoader PRIVATE include)
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 11
    ARCHIVE_OUTPUT_DIRECTORY lib
)
if (LDTK_NO_THROW)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_NO_THROW)
endif()


# examples targets
option(BUILD_SFML_EXAMPLE "Build an example using LDtkLoader with SFML." OFF)
option(BUILD_SDL_EXAMPLE  "Build an example using LDtkLoader with SDL."  OFF)
option(BUILD_API_TEST     "Build an example that loads a file showcasing all features of LDtk."  OFF)

if (BUILD_SFML_EXAMPLE)
    message(STATUS "LDtkSFML target available.")
    add_subdirectory(examples/SFML)
endif()

if (BUILD_SDL_EXAMPLE)
    message(STATUS "LDtkSDL target available.")
    add_subdirectory(examples/SDL)
endif()

if (BUILD_API_TEST)
    message(STATUS "LDtkAPI_test target available.")
    add_subdirectory(examples/API_test)
endif()
