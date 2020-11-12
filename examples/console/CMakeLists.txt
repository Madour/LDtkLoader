cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-Console)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

file(COPY level.ldtk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(LDtkConsole main.cpp)
target_include_directories(LDtkConsole PRIVATE ../../include)
target_link_libraries(LDtkConsole PRIVATE LDtkLoader-s)
