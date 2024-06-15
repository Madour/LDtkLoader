// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Layer.hpp"

#include <iostream>

#include "LDtkLoader/World.hpp"

#include "json.hpp"

using namespace ldtk;

Layer::Layer(const nlohmann::json& j, const World* w, const Level* l)
: level(l)
, iid(j.contains("iid") ? j["iid"].get<std::string>() : "")
, m_definition(&w->getLayerDef(j["layerDefUid"].get<int>()))
, m_override_tileset(
      j["overrideTilesetUid"].is_null() ? nullptr : &w->getTileset(j["overrideTilesetUid"].get<int>())
  )
, m_visible(j["visible"].get<bool>())
, m_total_offset(j["__pxTotalOffsetX"].get<int>(), j["__pxTotalOffsetY"].get<int>())
, m_opacity(j["__opacity"].get<float>())
, m_grid_size({j["__cWid"].get<int>(), j["__cHei"].get<int>()})
{
    std::string key = "gridTiles";
    if (getType() == LayerType::IntGrid || getType() == LayerType::AutoLayer) {
        key = "autoLayerTiles";
    }
    m_tiles.reserve(j[key].size());
    for (const auto& tile : j[key]) {
        const auto pos = IntPoint{tile["px"][0].get<int>(), tile["px"][1].get<int>()};
        const auto coord_id = key == "gridTiles" ? tile["d"][0].get<int>() : tile["d"][1].get<int>();
        const auto tile_id = tile["t"].get<int>();
        const auto flips = tile["f"].get<int>();
        const auto alpha = tile["a"].get<float>();
        m_tiles.emplace_back(this, pos, coord_id, tile_id, flips, alpha);

        auto& new_tile = m_tiles.back();
        m_tiles_map.emplace(new_tile.coordId, new_tile);

        if (getTileset().hasEnumTags()) {
            for (const EnumValue& enumtag : getTileset().getTileEnumTags(tile_id)) {
                m_tiles_by_enumtag[enumtag.name].emplace_back(new_tile);
            }
        }
    }

    int coord_id = 0;
    for (const auto& val : j["intGridCsv"]) {
        if (val.get<int>() != 0) {
            auto& intgridval = m_definition->m_intgrid_values.at(val.get<int>());
            m_intgrid.emplace(coord_id, intgridval);
            m_intgridpos_by_value[intgridval.value].emplace_back(getGridPositionFromCoordId(coord_id));
            m_intgridpos_by_name[intgridval.name].emplace_back(getGridPositionFromCoordId(coord_id));
        }
        coord_id++;
    }

    m_entities.reserve(j["entityInstances"].size());
    for (const auto& ent : j["entityInstances"]) {
        m_entities.emplace_back(ent, w, this);
        auto& new_ent = m_entities.back();
        m_entities_by_name[new_ent.getName()].emplace_back(new_ent);
        for (const auto& tag : new_ent.allTags()) {
            m_entities_by_tag[tag].emplace_back(new_ent);
        }
    }
}

auto Layer::getType() const -> const LayerType&
{
    return m_definition->type;
}

auto Layer::getName() const -> const std::string&
{
    return m_definition->name;
}

auto Layer::isVisible() const -> bool
{
    return m_visible;
}

auto Layer::getCellSize() const -> int
{
    return m_definition->cell_size;
}

auto Layer::getGridSize() const -> const IntPoint&
{
    return m_grid_size;
}

auto Layer::getOffset() const -> const IntPoint&
{
    return m_total_offset;
}

auto Layer::getOpacity() const -> float
{
    return m_opacity;
}

auto Layer::hasTileset() const -> bool
{
    return m_override_tileset != nullptr || m_definition->m_tileset != nullptr;
}

auto Layer::getTileset() const -> const Tileset&
{
    return m_override_tileset != nullptr ? *m_override_tileset : *m_definition->m_tileset;
}

auto Layer::allTiles() const -> const std::vector<Tile>&
{
    return m_tiles;
}

auto Layer::getTile(int grid_x, int grid_y) const -> const Tile&
{
    auto id = grid_x + m_grid_size.x * grid_y;
    if (m_tiles_map.count(id) > 0) {
        return m_tiles_map.at(id);
    }
    return Tile::None;
}

auto Layer::getTilesByEnumTag(const EnumValue& enumvalue) const -> const std::vector<ref_wrapper<Tile>>&
{
    const auto& enumtags_enum = getTileset().getEnumTagsEnum();
    if (enumvalue.type.uid != enumtags_enum.uid) {
        ldtk_error("Enum value \"" + enumvalue.name + "\" is not a value of Enum \"" + enumtags_enum.name
                   + "\".");
    }
    return m_tiles_by_enumtag.at(enumvalue.name);
}

auto Layer::getIntGridVal(int grid_x, int grid_y) const -> const IntGridValue&
{
    auto id = grid_x + m_grid_size.x * grid_y;
    if (m_intgrid.count(id) > 0) {
        return m_intgrid.at(id);
    }
    return IntGridValue::None;
}

auto Layer::getIntGridValPositions(int intgridval_value) const -> const std::vector<IntPoint>&
{
    if (m_intgridpos_by_value.find(intgridval_value) != m_intgridpos_by_value.end()) {
        return m_intgridpos_by_value.at(intgridval_value);
    }
    return m_intgridpos_by_value[intgridval_value];
}

auto Layer::getIntGridValPositions(const std::string& intgridval_name) const -> const std::vector<IntPoint>&
{
    if (m_intgridpos_by_name.find(intgridval_name) != m_intgridpos_by_name.end()) {
        return m_intgridpos_by_name.at(intgridval_name);
    }
    return m_intgridpos_by_name[intgridval_name];
}

auto Layer::allEntities() const -> const std::vector<Entity>&
{
    return m_entities;
}

auto Layer::hasEntity(const std::string& entity_name) const -> bool
{
    return m_entities_by_name.find(entity_name) != m_entities_by_name.end()
        && !m_entities_by_name.at(entity_name).empty();
}

auto Layer::getEntitiesByName(const std::string& name) const -> const std::vector<ref_wrapper<Entity>>&
{
    if (m_entities_by_name.find(name) != m_entities_by_name.end()) {
        return m_entities_by_name.at(name);
    }
    return m_entities_by_name[name];
}

auto Layer::getEntitiesByTag(const std::string& tag) const -> const std::vector<ref_wrapper<Entity>>&
{
    if (m_entities_by_tag.find(tag) != m_entities_by_tag.end()) {
        return m_entities_by_tag.at(tag);
    }
    return m_entities_by_tag[tag];
}

auto Layer::getEntity(const IID& entity_iid) const -> const Entity&
{
    for (const auto& entity : m_entities) {
        if (entity.iid == entity_iid) {
            return entity;
        }
    }
    ldtk_error("Entity with IID \"" + entity_iid.str() + "\" not found in Layer \"" + getName() + "\".");
}

auto Layer::getCoordIdAt(int grid_x, int grid_y) const -> int
{
    return grid_x + grid_y * m_grid_size.x;
}

auto Layer::getGridPositionFromCoordId(int coord_id) const -> IntPoint
{
    const auto& grid_width = getGridSize().x;
    auto y = coord_id / grid_width;
    auto x = coord_id - y * grid_width;
    return {x, y};
}
