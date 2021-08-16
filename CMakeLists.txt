cmake_minimum_required(VERSION 3.10)

project(LDtkLoader VERSION 0.9.3)

# get source files
file(GLOB_RECURSE Src src/*.cpp)
file(GLOB_RECURSE Inc include/LDtkLoader/*.hpp)

# create the static lib target
add_library(LDtkLoader STATIC ${Src} ${Inc})
target_compile_definitions(LDtkLoader PRIVATE _CRT_SECURE_NO_WARNINGS)
# set include directory for build and install
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(LDtkLoader INTERFACE $<INSTALL_INTERFACE:include>)
# set lib target properties
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 11
    ARCHIVE_OUTPUT_DIRECTORY lib
    DEBUG_POSTFIX -d
)

# examples targets
option(LDTK_NO_THROW        "Replace all throws by an error print and exit" OFF)
option(BUILD_SFML_EXAMPLE   "Build an example using LDtkLoader with SFML." OFF)
option(BUILD_SDL_EXAMPLE    "Build an example using LDtkLoader with SDL."  OFF)
option(BUILD_RAYLIB_EXAMPLE "Build an example using LDtkLoader with raylib." OFF)
option(BUILD_API_TEST       "Build an example that loads a file showcasing all features of LDtk."  OFF)

if (LDTK_NO_THROW)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_NO_THROW)
endif()

if (BUILD_SFML_EXAMPLE)
    message(STATUS "LDtkSFML target available.")
    add_subdirectory(examples/SFML)
endif()

if (BUILD_SDL_EXAMPLE)
    message(STATUS "LDtkSDL target available.")
    add_subdirectory(examples/SDL)
endif()

if (BUILD_RAYLIB_EXAMPLE)
    message(STATUS "LDtkRaylib target available.")
    add_subdirectory(examples/Raylib)
endif()

if (BUILD_API_TEST)
    message(STATUS "LDtkAPI_test target available.")
    add_subdirectory(examples/API_test)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# generate LDtkLoaderConfig.cmake
configure_package_config_file(
    "cmake/LDtkLoaderConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader
)

# install license and readme to doc directory
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
# install headers to include directory
install(DIRECTORY include/LDtkLoader DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# cmake export targets
install(TARGETS LDtkLoader EXPORT LDtkLoaderTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
install(EXPORT LDtkLoaderTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
