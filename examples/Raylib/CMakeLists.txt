cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-Raylib)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

find_package(raylib REQUIRED)
add_executable(LDtkRaylib main.cpp)
target_include_directories(LDtkRaylib PRIVATE ../../include ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(LDtkRaylib PRIVATE ${RAYLIB_LIBRARIES} LDtkLoader)
set_target_properties(LDtkRaylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkRaylib RUNTIME_OUTPUT_DIRECTORY)
file(COPY assets DESTINATION ${BIN_DIR})
