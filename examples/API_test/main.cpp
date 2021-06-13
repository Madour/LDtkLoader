// Created by Modar Nasser on 28/01/2021.

#include <iostream>
#include "LDtkLoader/World.hpp"

int main() {
    ldtk::World world;
    try {
        world.loadFromFile("all_features.ldtk");
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // loads everything without errors
    std::cout << "Loaded API test file successfully !" << std::endl;

    return 0;
}
