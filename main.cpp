// Created by Modar Nasser on 11/11/2020.

#include <iostream>
#include "LDtkLoader/World.hpp"

int main() {
    ldtk::World world;
    world.loadFromFile("level.ldtk");

    for (const auto& level : world.allLevels()) {
        std::cout << "Level : " << level.name << " " << level.uid << " " << level.size.x << "," << level.size.y << std::endl;
        for (const auto& layer : level.allLayers()) {
            std::cout << "  Layer : " << int(layer.type) << " " << layer.name << "\n";
            if (layer.hasTileset()) {
                std::cout << "    Tileset : " << layer.getTileset().name << " " << layer.getTileset().path << std::endl;
            }
            else std::cout << "    No Tileset\n";
        }
        std::cout << std::endl;
    }
    return 0;
}