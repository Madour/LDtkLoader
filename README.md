# LDtkLoader

### Load LDtk levels in your C++ game

**LDtkLoader** is a loader for [LDtk](https://github.com/deepnight/ldtk) tile maps.


### Sample code

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
```

### Build

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

Additional CMake options you can pass :
 - `-DBUILD_SFML_EXAMPLE=ON` : to build the SFML example (requires SFML installed)
 - `-DBUILD_SDL_EXAMPLE=ON` : to build the SDL example (comming soon™) 

### Features

 - [x] IntGrid layers
 - [x] Tiles layers
 - [x] AutoLayer layers
 - [ ] Entities layers
 - [ ] Enums

Note that LDtkLoader loads only the data needed to render the levels. Things like rules,
header metadata etc are not loaded.

### To Do 

- SDL example
- Helper classes for rendering
