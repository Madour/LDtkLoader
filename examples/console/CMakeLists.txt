cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-Console)

add_executable(LDtkConsole main.cpp)
target_include_directories(LDtkConsole PRIVATE ../../include)
target_link_libraries(LDtkConsole PRIVATE LDtkLoader)
set_target_properties(LDtkConsole PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkConsole RUNTIME_OUTPUT_DIRECTORY)
file(COPY level.ldtk DESTINATION ${BIN_DIR})
