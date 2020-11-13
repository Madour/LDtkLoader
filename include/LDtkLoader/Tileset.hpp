// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {
    class World;

    struct Tileset {
        friend World;

        const std::string name;
        const unsigned int uid;
        const std::string path;
        const UIntPoint image_size;
        const unsigned int tile_size;
        const unsigned int spacing;
        const unsigned int padding;

        auto getTileTexturePos(unsigned int tile_id) const -> UIntPoint;

    private:
        explicit Tileset(const nlohmann::json& j);
    };

}
