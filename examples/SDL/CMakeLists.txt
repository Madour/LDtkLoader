cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-SDL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})


# find SDL2 and SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


file(COPY tileset.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY level.ldtk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


add_executable(LDtkSDL main.cpp)
target_include_directories(LDtkSDL PRIVATE ../../include)
target_link_libraries(LDtkSDL PRIVATE LDtkLoader-s SDL2::SDL2 ${SDL2_IMAGE_LIBRARIES})
