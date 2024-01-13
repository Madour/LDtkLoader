// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>
#include <unordered_map>

#include "LDtkLoader/thirdparty/optional.hpp"
#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/containers/TagsContainer.hpp"

namespace ldtk {

    class Project;

    class Tileset : public TagsContainer {
    public:
        const std::string name;
        const int uid;
        const std::string path;
        const IntPoint texture_size;
        const int tile_size;
        const int spacing;
        const int padding;

        auto getTileTexturePos(int tile_id) const -> IntPoint;
        auto getTileCustomData(int tile_id) const -> const std::string&;

        auto hasTagsEnum() const -> bool;
        auto getTagsEnum() const -> const Enum&;
        auto getTilesWithTagEnum(const EnumValue& enumvalue) const -> const std::vector<int>&;

        Tileset(const nlohmann::json& j, Project* p);

    private:
        const Enum* const m_tags_enum;
        std::unordered_map<int, std::string> m_custom_data;
        std::unordered_map<std::string, std::vector<int>> m_tag_tiles_map;
    };

}
