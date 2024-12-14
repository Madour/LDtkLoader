// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LDtkLoader/containers/TagsContainer.hpp"
#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/thirdparty/optional.hpp"

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Utils.hpp"

namespace ldtk {

    class Project;

    class Tileset : public TagsContainer
    {
    public:
        const std::string name;
        const int uid;
        const std::string path;
        const IntPoint texture_size;
        const int tile_size;
        const int spacing;
        const int padding;

        auto getTileIdAt(int pos_x, int pos_y) const -> int;

        auto getTileTexturePos(int tile_id) const -> IntPoint;
        auto getTileCustomData(int tile_id) const -> const std::string&;
        auto getTileEnumTags(int tile_id) const -> const std::vector<ref_wrapper<const EnumValue>>&;

        auto hasEnumTags() const -> bool;
        auto getEnumTagsEnum() const -> const Enum&;
        auto getTilesByEnumTag(const EnumValue& enumvalue) const -> const std::vector<int>&;

        Tileset(const nlohmann::json& j, Project* p);

    private:
        const Enum* const m_enumtags_enum;
        std::unordered_map<int, std::string> m_custom_data_map;
        std::unordered_map<int, std::vector<ref_wrapper<const EnumValue>>> m_enumtags_by_tile;
        std::unordered_map<std::string, std::vector<int>> m_tiles_by_enumtag;
    };

} // namespace ldtk
