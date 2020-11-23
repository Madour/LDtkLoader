cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-SFML)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# Specify SFML install location
if (WIN32)
    set(SFML_DIR C:/SFML/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()
# find SFML
find_package(SFML REQUIRED COMPONENTS graphics audio)

file(COPY tileset.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY level.ldtk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(LDtkSFML main.cpp)
target_include_directories(LDtkSFML PRIVATE ../../include)
target_link_libraries(LDtkSFML PRIVATE LDtkLoader-s sfml-graphics)
