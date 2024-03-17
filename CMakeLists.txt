cmake_minimum_required(VERSION 3.10)

project(LDtkLoader VERSION 1.5.3.1)
if (NOT PROJECT_VERSION_TWEAK)
    set(PROJECT_VERSION_TWEAK 0)
endif()

# build options
option(LDTK_NO_THROW             "Replace all throws by an error print and exit" OFF)
option(LDTK_BUILD_SFML_EXAMPLE   "Build an example using LDtkLoader with SFML." OFF)
option(LDTK_BUILD_SDL_EXAMPLE    "Build an example using LDtkLoader with SDL."  OFF)
option(LDTK_BUILD_RAYLIB_EXAMPLE "Build an example using LDtkLoader with raylib." OFF)
option(LDTK_BUILD_API_TEST       "Build an example that loads a file showcasing all features of LDtk."  OFF)

option(LDTK_FIELD_PUBLIC_OPTIONAL "Set to ON to enable the full optional interface on Field structures" OFF)
mark_as_advanced(LDTK_FIELD_PUBLIC_OPTIONAL)

# get source files
file(GLOB_RECURSE LDtkLoader_SRC src/*.cpp)
file(GLOB_RECURSE LDtkLoader_INC include/LDtkLoader/*.hpp)

configure_file(${PROJECT_SOURCE_DIR}/src/Version.hpp.in ${PROJECT_BINARY_DIR}/include/LDtkLoader/Version.hpp)

# create the static library target
add_library(LDtkLoader STATIC ${LDtkLoader_SRC} ${LDtkLoader_INC})
add_library(LDtkLoader::LDtkLoader ALIAS LDtkLoader)

# set compile options
if (MSVC)
    target_compile_definitions(LDtkLoader PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if (LDTK_NO_THROW)
    target_compile_definitions(LDtkLoader PUBLIC LDTK_NO_THROW JSON_NOEXCEPTION)
endif()
if (LDTK_FIELD_PUBLIC_OPTIONAL)
    target_compile_definitions(LDtkLoader PRIVATE LDTK_FIELD_PUBLIC_OPTIONAL)
endif()

# set include directory for build and install
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(LDtkLoader PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
target_include_directories(LDtkLoader INTERFACE $<INSTALL_INTERFACE:include>)

# set library target properties
set_target_properties(LDtkLoader PROPERTIES
    CXX_STANDARD 11
    ARCHIVE_OUTPUT_DIRECTORY lib
    DEBUG_POSTFIX -d
)

# add examples subdirectories whn requested
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

# cmake install rules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# generate LDtkLoaderConfigVersion.cmake
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake"
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# generate LDtkLoaderConfig.cmake
configure_package_config_file(
    "cmake/LDtkLoaderConfig.cmake.in" "${PROJECT_BINARY_DIR}/LDtkLoaderConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader
)

# create regular installation rules
# install license and readme to ${CMAKE_INSTALL_PREFIX}/share/doc/LDtkLoader
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
# install headers to ${CMAKE_INSTALL_PREFIX}/include
install(DIRECTORY include/LDtkLoader DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/LDtkLoader DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install the static library target to ${CMAKE_INSTALL_PREFIX}/lib
install(TARGETS LDtkLoader EXPORT LDtkLoaderTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT LDtkLoaderTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader NAMESPACE LDtkLoader::)
# install cmake package files to ${CMAKE_INSTALL_PREFIX}/lib/cmake/LDtkLoader
install(FILES ${PROJECT_BINARY_DIR}/LDtkLoaderConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)
install(FILES ${PROJECT_BINARY_DIR}/LDtkLoaderConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LDtkLoader)

# create build-tree export cmake module
export(TARGETS LDtkLoader FILE ${PROJECT_BINARY_DIR}/LDtkLoader.cmake NAMESPACE LDtkLoader::)
