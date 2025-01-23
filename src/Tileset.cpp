// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/Tileset.hpp"

#include "LDtkLoader/Project.hpp"

#include "json.hpp"

using namespace ldtk;

Tileset::Tileset(const nlohmann::json& j, Project* p)
: TagsContainer(j.contains("tags") ? j["tags"] : nlohmann::json{})
, name(j["identifier"].get<std::string>())
, uid(j["uid"].get<int>())
, path(j["relPath"].is_null() ? "" : j["relPath"].get<std::string>())
, texture_size({j["pxWid"].get<int>(), j["pxHei"].get<int>()})
, tile_size(j["tileGridSize"].get<int>())
, spacing(j["spacing"].get<int>())
, padding(j["padding"].get<int>())
, m_enumtags_enum(j["tagsSourceEnumUid"].is_null() ? nullptr : &p->getEnum(j["tagsSourceEnumUid"].get<int>()))
{
    // parse tiles custom data
    m_custom_data_map.reserve(j["customData"].size() + 1);
    m_custom_data_map.emplace(-1, "");
    for (const auto& data : j["customData"]) {
        m_custom_data_map.emplace(data["tileId"].get<int>(), data["data"].get<std::string>());
    }

    // parse tiles enum tags
    m_enumtags_by_tile.reserve(j["enumTags"].size() + 1);
    m_enumtags_by_tile.insert({-1, {}});
    if (m_enumtags_enum != nullptr) {
        for (const auto& tag : j["enumTags"]) {
            auto enumval_name = tag["enumValueId"].get<std::string>();
            const auto& tiles = tag["tileIds"];

            m_tiles_by_enumtag[enumval_name].reserve(tiles.size());
            for (const auto& tile_id : tiles) {
                m_tiles_by_enumtag[enumval_name].push_back(tile_id.get<int>());
                m_enumtags_by_tile[tile_id].emplace_back(getEnumTagsEnum()[enumval_name]);
            }
        }
    }
}

auto Tileset::getTileIdAt(int pos_x, int pos_y) const -> int
{
    const auto effective_tex_width = texture_size.x - padding * 2 + spacing;
    const auto effective_tile_size = tile_size + spacing;

    auto grid_width = effective_tex_width / effective_tile_size;
    grid_width += (effective_tex_width % effective_tile_size) > 0 ? 1 : 0;

    auto col = (pos_x - padding) / (tile_size + spacing);
    auto row = (pos_y - padding) / (tile_size + spacing);

    return col + row * grid_width;
}

auto Tileset::getTileTexturePos(int tile_id) const -> IntPoint
{
    const auto effective_tex_width = texture_size.x - padding * 2 + spacing;
    const auto effective_tile_size = tile_size + spacing;

    auto grid_width = effective_tex_width / effective_tile_size;
    grid_width += (effective_tex_width % effective_tile_size) > 0 ? 1 : 0;

    return {
        padding + (tile_id % grid_width) * (tile_size + spacing),
        padding + (tile_id / grid_width) * (tile_size + spacing)
    };
}

auto Tileset::getTileCustomData(int tile_id) const -> const std::string&
{
    if (m_custom_data_map.find(tile_id) != m_custom_data_map.end()) {
        return m_custom_data_map.at(tile_id);
    }
    return m_custom_data_map.at(-1);
}

auto Tileset::getTileEnumTags(int tile_id) const -> const std::vector<ref_wrapper<const EnumValue>>&
{
    if (m_enumtags_by_tile.find(tile_id) != m_enumtags_by_tile.end()) {
        return m_enumtags_by_tile.at(tile_id);
    }
    return m_enumtags_by_tile.at(-1);
}

auto Tileset::hasEnumTags() const -> bool
{
    return m_enumtags_enum != nullptr;
}

auto Tileset::getEnumTagsEnum() const -> const Enum&
{
    return *m_enumtags_enum;
}

auto Tileset::getTilesByEnumTag(const EnumValue& enumvalue) const -> const std::vector<int>&
{
    if (enumvalue.type.uid != m_enumtags_enum->uid) {
        ldtk_error("Enum value \"" + enumvalue.name + "\" is not a value of Enum \"" + m_enumtags_enum->name
                   + "\".");
    }

    return m_tiles_by_enumtag.at(enumvalue.name);
}
