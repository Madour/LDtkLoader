cmake_minimum_required(VERSION 3.10)

project(LDtkLoader)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

# temporary
file(GLOB_RECURSE Src src/*.cpp)
file(COPY level.ldtk DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


add_library(LDtkLoader-s STATIC ${Src})
target_include_directories(LDtkLoader-s PRIVATE include)


add_executable(demo main.cpp)
target_include_directories(demo PRIVATE include)
target_link_libraries(demo PRIVATE LDtkLoader-s)
