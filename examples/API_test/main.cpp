// Created by Modar Nasser on 28/01/2021.

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "LDtkLoader/Project.hpp"

auto getFileBytes(const std::string& filepath) -> std::vector<std::uint8_t> {
    std::basic_ifstream<std::uint8_t> file(filepath, std::ios::in | std::ios::binary);

    return std::vector<std::uint8_t>(
        std::istreambuf_iterator<std::uint8_t>(file),
        std::istreambuf_iterator<std::uint8_t>()
    );
}

auto loadFileStringFromBytes(const std::string& filepath) -> std::string {
    // emulate loading bytes from a virtual filesystem
    auto bytes = getFileBytes(filepath);

    return std::string(bytes.data(), bytes.data() + bytes.size());
}

int main() {
    try {
        // load from file
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features.ldtk");
        }

        // load from multiple files
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features_external.ldtk");
        }

        // load from file using custom file reader function
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features.ldtk", loadFileStringFromBytes);
        }

        // load from multiple files using custom file reader function
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromFile("all_features_external.ldtk", loadFileStringFromBytes);
        }

        // load from memory (vector)
        {
            ldtk::Project ldtk_project;
            ldtk_project.loadFromMemory(getFileBytes("all_features.ldtk"));
        }

        // load from memory (pointer and size)
        {
            auto file_bytes = getFileBytes("all_features.ldtk");
            ldtk::Project ldtk_project;
            ldtk_project.loadFromMemory(file_bytes.data(), static_cast<unsigned int>(file_bytes.size()));
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
