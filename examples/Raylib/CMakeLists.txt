cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-raylib)

find_package(raylib REQUIRED)

if (MSVC)
    if(NOT DEFINED ${raylib_INCLUDE_DIRS})
        set(raylib_INCLUDE_DIRS ${RAYLIB_INCLUDE_DIRS})
    endif()

    if(NOT DEFINED ${raylib_LIBRARIES})
        set(raylib_LIBRARIES ${RAYLIB_LIBRARIES})
    endif()
endif()

add_executable(LDtkRaylib main.cpp)
target_link_libraries(LDtkRaylib PRIVATE LDtkLoader raylib)
set_target_properties(LDtkRaylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

add_custom_command(
    TARGET LDtkRaylib
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/ $<TARGET_FILE_DIR:LDtkRaylib>/assets/
)
