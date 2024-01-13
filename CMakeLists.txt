cmake_minimum_required(VERSION 3.10)

project(LDtkLoader VERSION 1.5.2)

# get source files
file(GLOB_RECURSE Src src/*.cpp)
file(GLOB_RECURSE Inc include/LDtkLoader/*.hpp)

configure_file(${PROJECT_SOURCE_DIR}/src/Version.hpp.in ${CMAKE_BINARY_DIR}/include/LDtkLoader/generated/Version.hpp)

# create the static lib target
add_library(LDtkLoader STATIC ${Src} ${Inc})
if (MSVC)
    target_compile_definitions(LDtkLoader PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
# set include directory for build and install
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/>)
target_include_directories(LDtkLoader INTERFACE $<INSTALL_INTERFACE:include>)
# set lib target properties
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 11
    ARCHIVE_OUTPUT_DIRECTORY lib
    DEBUG_POSTFIX -d
)

# build options
option(LDTK_NO_THROW "Replace all throws by an error print and exit" OFF)
option(LDTK_BUILD_SFML_EXAMPLE   "Build an example using LDtkLoader with SFML." OFF)
option(LDTK_BUILD_SDL_EXAMPLE    "Build an example using LDtkLoader with SDL."  OFF)
option(LDTK_BUILD_RAYLIB_EXAMPLE "Build an example using LDtkLoader with raylib." OFF)
option(LDTK_BUILD_API_TEST       "Build an example that loads a file showcasing all features of LDtk."  OFF)

option(LDTK_FIELD_PUBLIC_OPTIONAL "Set to ON to enable the full optional interface on Field structures" OFF)
mark_as_advanced(LDTK_FIELD_PUBLIC_OPTIONAL)

if (LDTK_NO_THROW)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_NO_THROW JSON_NOEXCEPTION)
endif()

if (LDTK_BUILD_SFML_EXAMPLE)
    message(STATUS "LDtkSFML target available.")
    add_subdirectory(examples/SFML)
endif()

if (LDTK_BUILD_SDL_EXAMPLE)
    message(STATUS "LDtkSDL target available.")
    add_subdirectory(examples/SDL)
endif()

if (LDTK_BUILD_RAYLIB_EXAMPLE)
    message(STATUS "LDtkRaylib target available.")
    add_subdirectory(examples/Raylib)
endif()

if (LDTK_BUILD_API_TEST)
    message(STATUS "LDtkAPI_test target available.")
    add_subdirectory(examples/API_test)
endif()

if (LDTK_FIELD_PUBLIC_OPTIONAL)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_FIELD_PUBLIC_OPTIONAL)
endif()

# cmake install rules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# generate LDtkLoaderConfigVersion.cmake
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake"
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# generate LDtkLoaderConfig.cmake
configure_package_config_file(
    "cmake/LDtkLoaderConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader
)

# install license and readme to ${CMAKE_INSTALL_PREFIX}/share/doc/LDtkLoader
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
# install headers to ${CMAKE_INSTALL_PREFIX}/include
install(DIRECTORY include/LDtkLoader DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install static lib target to ${CMAKE_INSTALL_PREFIX}/lib
install(TARGETS LDtkLoader EXPORT LDtkLoaderTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
# install cmake package files to ${CMAKE_INSTALL_PREFIX}/lib/cmake/LDtkLoader
install(EXPORT LDtkLoaderTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
