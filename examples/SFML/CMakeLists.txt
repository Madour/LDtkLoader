cmake_minimum_required(VERSION 3.10)
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

project(LDtkLoader-SFML)

# find SFML
find_package(SFML REQUIRED COMPONENTS graphics)

add_executable(LDtkSFML main.cpp)
target_include_directories(LDtkSFML PRIVATE ../../include)
target_link_libraries(LDtkSFML PRIVATE LDtkLoader sfml-graphics)
set_target_properties(LDtkSFML PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkSFML RUNTIME_OUTPUT_DIRECTORY)
file(COPY assets DESTINATION ${BIN_DIR})
