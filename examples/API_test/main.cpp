// Created by Modar Nasser on 28/01/2021.

#include <iostream>
#include "LDtkLoader/Project.hpp"

int main() {
    ldtk::Project ldtk_project;
    try {
        ldtk_project.loadFromFile("all_features.ldtk");
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // loads everything without errors
    std::cout << "Loaded API test file successfully !" << std::endl;

    return 0;
}
