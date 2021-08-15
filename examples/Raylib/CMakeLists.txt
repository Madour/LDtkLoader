cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-raylib)

find_package(raylib REQUIRED)

add_executable(LDtkRaylib main.cpp)
target_include_directories(LDtkRaylib PRIVATE ../../include ${raylib_INCLUDE_DIRS})
target_link_libraries(LDtkRaylib PRIVATE ${raylib_LIBRARIES} LDtkLoader)
set_target_properties(LDtkRaylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkRaylib RUNTIME_OUTPUT_DIRECTORY)
file(COPY assets DESTINATION ${BIN_DIR})
