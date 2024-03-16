# LDtk Loader

[![LDtk Version](https://img.shields.io/badge/LDtk_version-1.5.3-default)](https://ldtk.io/json/)
[![Build](https://github.com/Madour/LDtkLoader/actions/workflows/cmake.yml/badge.svg)](https://github.com/Madour/LDtkLoader/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/Madour/LDtkLoader/actions/workflows/codeql.yml/badge.svg)](https://github.com/Madour/LDtkLoader/actions/workflows/codeql.yml)

**LDtkLoader** is a loader for **[LDtk](https://github.com/deepnight/ldtk)** levels and tile maps.

Use it to load and get data from LDtk projects in your game.

Supports the entire .ldtk format (including external levels).

For loading older versions, see the [Releases](https://github.com/Madour/LDtkLoader/releases) page.

### Sample code

A taste of simplicity :

```c++
// load the file
ldtk::Project ldtk_project;
ldtk_project.loadFromFile("my_project.ldtk");

// get a world
const auto& world = ldtk_project.getWorld("MyWorld");

// get a level
const auto& level1 = world.getLevel("Level1");

// get a layer
const auto& bg_layer = level1.getLayer("BgLayer");

// iterate on the tiles of the layer
for (const auto& tile : bg_layer.allTiles()) {
    // do something with the tile (storing, rendering ...)
}

// iterate on Enemy entities
for (const ldtk::Entity& enemy : level1.getLayer("Entities").getEntitiesByName("Enemy")) {

    // iterate over an array field of Enum values
    for (const auto& item : enemy.getArrayField<ldtk::EnumValue>("items")) {
        // test if field is null
        if (!item.is_null()) {
            // do something with item
            if (item == world.getEnum("Items")["Sword"]) {
                // the enemy has a Sword !
            }
        }
    }

    // get an Entity field
    int enemy_hp = enemy.getField<int>("HP").value();
}
```

### Build

Should work with any C++11 compiler.
You can select Debug or Release mode when building.

```shell
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE={Debug|Release} ..
cmake --build . --config {Debug|Release}
```

This will compile the static library named LDtkLoader in build/lib.

When built in Debug configuration, the library name has the suffix '-d'.

Additional CMake options :

 - `-DLDTK_NO_THROW`: to print message and exit when there is an error instead of throwing exceptions
 - `-DLDTK_BUILD_API_TEST`: to build the API test example, to make sure the library works as expected
 - `-DLDTK_BUILD_SFML_EXAMPLE=ON`: to build the SFML example (requires SFML installed)
 - `-DLDTK_BUILD_SDL_EXAMPLE=ON`: to build the SDL example (requires SDL2 and SDL2_image installed)
 - `-DLDTK_BUILD_RAYLIB_EXAMPLE=ON`: to build the raylib example (requires raylib installed)

Advanced CMake options (use if you know what you are doing):

 - `LDTK_FIELD_PUBLIC_OPTIONAL=ON`: to enable the full optional interface for the `ldtk::Field` structure.

### Install

In the build directory, run :

```shell
cmake [-DCMAKE_INSTALL_PREFIX=/install/path/LDtkLoader] ..
cmake --install . --config {Debug|Release}
```

This will copy the libraries, the headers and the cmake config files to the install path.

### How to use LDtkLoader in your project

After installation, to use LDtkLoader in a CMake project, use the `find_package` command:

```cmake
find_package(LDtkLoader 1.5)
```

The `find_package` command will automatically get the correct library (Release or Debug)
depending on your current CMake configuration.

> [!NOTE]
> If LDtkLoader was installed at a custom path and CMake is unable to find the package, pass the `LDtkLoader_ROOT`
> option to CMake, like this: `cmake -DLDtkLoader_ROOT=/install/path/LDtkLoader ..`.


Then, the library should be linked to the target:

```cmake
target_link_libraries(YourTarget PRIVATE LDtkLoader::LDtkLoader)
```

### Documentation

API documentation can be found on the [**wiki**](https://github.com/Madour/LDtkLoader/wiki).

### Demo

> [!WARNING]
> The demos below are probably using an older version of the library.

A demo using LDtkLoader with SFML can be found [**here**](https://github.com/Madour/LDtk-SFML-Game).

Here is a preview :

https://user-images.githubusercontent.com/11854124/122610077-57f48a00-d07f-11eb-98ac-278a3d3dbd01.mp4

--- 

A demo using LDtkLoader with Direct X11 can be found [**here**](https://github.com/dontpanic5/LDtk-D11/) (by @dontpanic5)

Here is a preview : 

https://user-images.githubusercontent.com/242570/129111903-88727f39-9e21-45cb-bc76-a5d7975a4a9c.mp4

### To Do
 - [ ] Helper classes/methods for SFML or SDL rendering

### License

LDtkLoader is licensed under the **zlib license**.

Bundled dependencies :
 - [json](https://github.com/nlohmann/json) - MIT license
 - [Optional](https://github.com/akrzemi1/Optional) - Boost Software Licence
