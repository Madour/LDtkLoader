// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>
#include <unordered_map>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {
    class World;

    struct Tileset {
        friend World;

        const std::string name;
        const int uid;
        const std::string path;
        const IntPoint texture_size;
        const int tile_size;
        const int spacing;
        const int padding;

        auto getTileTexturePos(int tile_id) const -> IntPoint;
        auto getTileData(int tile_id) const -> const std::string&;

        explicit Tileset(const nlohmann::json& j);

    private:
        std::unordered_map<int, std::string> m_custom_data;
    };

}
