cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-SDL)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(LDtkSDL main.cpp)
target_include_directories(LDtkSDL PRIVATE ../../include)
target_link_libraries(LDtkSDL PRIVATE LDtkLoader SDL2::SDL2 ${SDL2_IMAGE_LIBRARIES})
set_target_properties(LDtkSDL PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkSDL RUNTIME_OUTPUT_DIRECTORY)
file(COPY assets DESTINATION ${BIN_DIR})
