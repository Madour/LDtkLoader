// Created by Modar Nasser on 28/01/2021.

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>

#include "LDtkLoader/Project.hpp"

int main() {
    try {
        // load from file
        {
            ldtk::Project ldtk_project;

            ldtk_project.loadFromFile("all_features.ldtk");
        }

        // load from memory
        {
            ldtk::Project ldtk_project;

            std::basic_ifstream<std::uint8_t> file("all_features.ldtk", std::ios::in | std::ios::binary);
            const std::vector<std::uint8_t> bytes = std::vector<std::uint8_t>(
                std::istreambuf_iterator<std::uint8_t>(file),
                std::istreambuf_iterator<std::uint8_t>()
            );

            ldtk_project.loadFromMemory(bytes);
        }
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // loads everything without errors
    std::cout << "Loaded API test file successfully !" << std::endl;

    return 0;
}
