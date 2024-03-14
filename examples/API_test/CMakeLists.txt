cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-API_test)

add_executable(LDtkAPI_test main.cpp)
target_link_libraries(LDtkAPI_test PRIVATE LDtkLoader)
set_target_properties(LDtkAPI_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

add_custom_command(
    TARGET LDtkAPI_test
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/ $<TARGET_FILE_DIR:LDtkAPI_test>/assets/
)
