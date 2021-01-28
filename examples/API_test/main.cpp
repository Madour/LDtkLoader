// Created by Modar Nasser on 28/01/2021.

#include <iostream>
#include "LDtkLoader/World.hpp"

int main() {
    ldtk::World world;
    world.loadFromFile("all_features.ldtk");

    // loads everything without errors
    std::cout << "Loaded API test file successfully !" << std::endl;

    return 0;
}
