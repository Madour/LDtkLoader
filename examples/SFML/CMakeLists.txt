cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-SFML)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)


# find SFML
find_package(SFML REQUIRED COMPONENTS graphics)


file(COPY tileset.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY level.ldtk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(LDtkSFML main.cpp)
target_include_directories(LDtkSFML PRIVATE ../../include)
target_link_libraries(LDtkSFML PRIVATE LDtkLoader-s sfml-graphics)
