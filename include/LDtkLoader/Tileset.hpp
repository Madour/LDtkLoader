// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>
#include <unordered_map>

#include "LDtkLoader/thirdparty/optional.hpp"
#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"

namespace ldtk {

    class World;

    struct Tileset {
        const std::string name;
        const int uid;
        const std::string path;
        const IntPoint texture_size;
        const int tile_size;
        const int spacing;
        const int padding;

        auto getTileTexturePos(int tile_id) const -> IntPoint;
        auto getTileData(int tile_id) const -> const std::string&;

        auto hasTags() const -> bool;
        auto getTagsEnum() const -> const Enum&;
        auto getTilesWithTag(const EnumValue& enumvalue) const -> const std::vector<int>&;

        Tileset(const nlohmann::json& j, World* w);

    private:
        const Enum* m_tags_enum;
        std::unordered_map<int, std::string> m_custom_data;
        std::unordered_map<std::string, std::vector<int>> m_tag_tiles_map;
    };

}
