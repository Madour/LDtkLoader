// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {
    class Level;

    class Tileset {
        friend Level;
    public:
        const std::string name;
        const unsigned int uid;
        const std::string path;
        const UIntPoint image_size;
        const unsigned int grid_size;
        const unsigned int spacing;
        const unsigned int padding;

    private:
        explicit Tileset(const nlohmann::json& j);
    };

}
