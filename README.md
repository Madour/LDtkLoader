<h1> LDtk Loader <img src=https://img.shields.io/badge/LDtk_version-0.7.2-default></h1>


### Load LDtk levels in your C++ game

**LDtkLoader** is a loader for **[LDtk](https://github.com/deepnight/ldtk)** tile maps.

You can use it to load LDtk levels in your game and access any data you need via the API.

### Sample code

A taste of simplicity :

```c++
// load the file
ldtk::World world;
world.loadFromFile("my_levels.ldtk");

// get a level
const auto& level1 = world.getLevel("Level1");

// get a layer
const auto& bg_layer = level1.getLayer("BgLayer");

// iterate on the tiles of the layer
for (const auto& tile : bg_layer.allTiles()) {
    // do something with the tile (storing, rendering ...)
}

// iterate on Enemy entities
for (const auto& enemy : level1.getLayer("Entities").getEntities("Enemy")) {
    // access a field
    const auto& enemy_weapon = enemy.getField<ldtk::EnumValue>("ActiveWeapon");
    if (enemy_weapon == world.getEnum("Weapon")["Sword"]) {
        // the enemy has a Sword !
    }
}
```

### Build

Should work with any C++11 compiler. 

```shell
mkdir build && cd build
cmake ..
cmake --build . --config [Release|Debug]
```

Additional CMake options you can pass :
 - `-DLDTK_NO_THROW` : to print message and exit when there is an error instead of throwing exceptions

 - `-DBUILD_API_TEST` : to build the API test example, to make sure the library works as expected

 - `-DBUILD_SFML_EXAMPLE=ON` : to build the SFML example (requires SFML installed)
    - set `-DSFML_ROOT=/path/to/SFML/` if CMake is unable to find it
    - set `-DSFML_STATIC_LIBRARIES=TRUE` if you want to link the static libraries

 - `-DBUILD_SDL_EXAMPLE=ON` : to build the SDL example (requires SDL2 and SDL2_image installed)

### Install

In the build directory, run : 

```shell
cmake -DCMAKE_INSTALL_PREFIX=/install/path/LDtkLoader ..
cmake --install . --config [Release|Debug]
```

This will copy the libraries, the headers and the cmake config file to the given install path.

### How to use LDtkLoader in your project

After installation, to use the LDtkLoader library in your CMake project, you can just call :

```cmake
find_package(LDtkLoader)
```

The `find_package` command will automatically get the correct library (release or debug) depending on your current CMake configuration.

Then you only have to link the library to your target :

```cmake
target_link_libraries(YourTarget PRIVATE LDtkLoader)
```

Note : if you installed LDtkLoader at a custom path, you might need 
to pass a `-DLDtkLoader_ROOT=/install/path/LDtkLoader` option to cmake so it can find the package.

### Documentation

Documentation is under construction and can be found on the [**wiki**](https://github.com/Madour/LDtkLoader/wiki).

### To Do
 - [ ] Helper classes/methods for SFML or SDL rendering
 - [ ] API documentation

### License

LDtkLoader is licensed under the **zlib license**.

Bundled dependencies : 
 - [json](https://github.com/nlohmann/json) - MIT license
 - [Optional](https://github.com/akrzemi1/Optional) - Boost Software Licence
