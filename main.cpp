// Created by Modar Nasser on 11/11/2020.

#include <iostream>
#include "LDtkLoader/World.hpp"

int main() {
    ldtk::World world;
    world.loadFromFile("level.ldtk");

    for (const auto& level : world.allLevels()) {
        std::cout << "Level : " << level.name << " " << level.size.x << "px x" << level.size.y << "px" << std::endl;
        for (const auto& layer : level.allLayers()) {
            if (layer.hasTileset()) {
                std::cout << "Layer" << layer.name << " :" << std::endl;
                if (layer.allTiles().empty())
                    std::cout << "-> No tiles in this layer\n" ;
                else {
                    // print the layer
                    for (int y = 0; y < layer.grid_size.y; ++y) {
                        for (int x = 0; x < layer.grid_size.x; ++x) {
                            try {
                                const auto& tile = layer.getTile(x, y);
                                printf("%3d ", tile.tileId);
                            }
                            catch (const std::exception& e) {
                                std::cout << "    ";
                            }
                        }
                        std::cout << std::endl;
                    }
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}