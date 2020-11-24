<h1> LDtk Loader <img src=https://img.shields.io/badge/LDtk_version-0.5.2-yellow></h1>


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

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

Additional CMake options you can pass :
 - `-DBUILD_SFML_EXAMPLE=ON` : to build the SFML example (requires SFML installed)
    - set `-DSFML_ROOT=/path/to/SFML/` if CMake is unable to find it
    - set `-DSFML_STATIC_LIBRARIES=TRUE` if you want to link the static libraries

 - `-DBUILD_SDL_EXAMPLE=ON` : to build the SDL example (requires SDL2 and SDL2_image installed) 

### Documentation

Documentation is under construction and can be found on the [**wiki**](https://github.com/Madour/LDtkLoader/wiki).

### Supported features

 - [x] IntGrid layers
 - [x] Tiles layers
 - [x] AutoLayer layers
 - [x] Enums
 - [x] Entities layers

Note that LDtkLoader loads only the data needed to render the levels. Things like rules,
header metadata etc are not loaded.

### To Do :
 - [ ] Extended IntGrid data (store values)
 - [ ] Improve error handling
 - [ ] Make API more consistent
 - [ ] Helper classes/methods for rendering
 - [ ] API documentation
