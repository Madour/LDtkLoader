cmake_minimum_required(VERSION 3.10)

project(LDtkLoader-API_test)

add_executable(LDtkAPI_test main.cpp)
target_include_directories(LDtkAPI_test PRIVATE ../../include)
target_link_libraries(LDtkAPI_test PRIVATE LDtkLoader)
set_target_properties(LDtkAPI_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)

get_target_property(BIN_DIR LDtkAPI_test RUNTIME_OUTPUT_DIRECTORY)
file(COPY all_features.ldtk DESTINATION ${BIN_DIR})
file(COPY all_features_external.ldtk DESTINATION ${BIN_DIR})
file(COPY all_features_external DESTINATION ${BIN_DIR})
file(COPY atlas DESTINATION ${BIN_DIR})
