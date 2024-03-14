cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-SFML)

# find SFML
find_package(SFML REQUIRED COMPONENTS graphics)

add_executable(LDtkSFML main.cpp)
target_link_libraries(LDtkSFML PRIVATE LDtkLoader sfml-graphics)
set_target_properties(LDtkSFML PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

add_custom_command(
    TARGET LDtkSFML
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/ $<TARGET_FILE_DIR:LDtkSFML>/assets/
)
