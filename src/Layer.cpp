// Created by Modar Nasser on 12/11/2020.

#include <iostream>

#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Layer::Layer(const nlohmann::json& j, const World* w, const Level* l) :
level(l),
m_definition(&w->getLayerDef(j["layerDefUid"].get<int>())),
m_visible(j["visible"].get<bool>()),
m_total_offset(j["__pxTotalOffsetX"].get<int>(), j["__pxTotalOffsetY"].get<int>()),
m_opacity(j["__opacity"].get<float>()),
m_grid_size({j["__cWid"].get<int>(), j["__cHei"].get<int>()})
{
    if ( !j["overrideTilesetUid"].is_null() ) {
        m_override_tileset = &w->getTileset(j["overrideTilesetUid"].get<int>());
    }

    std::string key = "gridTiles";
    int coordId_index = 0;
    if (getType() == LayerType::IntGrid || getType() == LayerType::AutoLayer) {
        key = "autoLayerTiles";
        coordId_index = 1;
    }
    m_tiles.reserve(j[key].size());
    for (const auto& tile : j[key]) {
        Tile new_tile;
        new_tile.coordId = tile["d"].get<std::vector<int>>()[coordId_index];
        new_tile.position.x = tile["px"].get<std::vector<int>>()[0] + m_total_offset.x;
        new_tile.position.y = tile["px"].get<std::vector<int>>()[1] + m_total_offset.y;

        new_tile.world_position.x = static_cast<int>(new_tile.position.x) + level->position.x;
        new_tile.world_position.y = static_cast<int>(new_tile.position.y) + level->position.y;

        new_tile.tileId = tile["t"].get<int>();
        new_tile.texture_position.x = tile["src"].get<std::vector<int>>()[0];
        new_tile.texture_position.y = tile["src"].get<std::vector<int>>()[1];

        auto flip = tile["f"].get<unsigned int>();
        new_tile.flipX = flip & 1u;
        new_tile.flipY = (flip>>1u) & 1u;

        updateTileVertices(new_tile);

        m_tiles.push_back(new_tile);
    }

    for (auto& tile : m_tiles)
        m_tiles_map[tile.coordId] = &tile;

    // LDtk v0.8+
    if (j.contains("intGridCsv")) {
        int coord_id = 0;
        for (const auto& val : j["intGridCsv"]) {
            if (val.get<int>() != 0)
                m_intgrid[coord_id] = &m_definition->m_intgrid_values.at(val.get<int>());
            coord_id++;
        }
    }
    // LDtk pre v0.8
    else {
        for (const auto& val : j["intGrid"]) {
            m_intgrid[val["coordId"].get<int>()] = &m_definition->m_intgrid_values.at(val["v"].get<int>());
        }
    }

    m_entities.reserve(j["entityInstances"].size());
    for (const auto& ent : j["entityInstances"]) {
        m_entities.emplace_back(ent, w);
        auto& new_ent = m_entities.back();
        m_entities_by_name[new_ent.getName()].emplace_back(new_ent);
        for (const auto& tag : new_ent.getTags()) {
            m_entities_by_tag[tag].emplace_back(new_ent);
        }
    }
}


auto Layer::getType() const -> const LayerType& {
    return m_definition->type;
}

auto Layer::getName() const -> const std::string& {
    return m_definition->name;
}

auto Layer::getCellSize() const -> int {
    return m_definition->cell_size;
}

auto Layer::getGridSize() const -> const IntPoint& {
    return m_grid_size;
}

auto Layer::getOffset() const -> const IntPoint& {
    return m_total_offset;
}

void Layer::setOffset(const IntPoint& offset) const {
    m_total_offset = offset;
    for (const auto& tile : m_tiles)
        updateTileVerticesPos(tile);
}

auto Layer::getOpacity() const -> float {
    return m_opacity;
}

void Layer::setOpacity(float opacity) const {
    m_opacity = std::min(1.f, std::max(0.f, opacity));
    for (const auto& tile : m_tiles)
        updateTileVerticesCol(tile);
}

auto Layer::hasTileset() const -> bool {
    return m_override_tileset != nullptr || m_definition->m_tileset != nullptr;
}

auto Layer::getTileset() const -> const Tileset& {
    return m_override_tileset == nullptr ? *m_definition->m_tileset : *m_override_tileset;
}

auto Layer::allTiles() const -> const std::vector<Tile>& {
    return m_tiles;
}

auto Layer::getTile(int grid_x, int grid_y) const -> const Tile& {
    auto id = grid_x + m_grid_size.x*grid_y;
    if (m_tiles_map.count(id) > 0)
        return *(m_tiles_map.at(id));
    return Tile::None;
}

auto Layer::getIntGridVal(int grid_x, int grid_y) const -> const IntGridValue& {
    auto id = grid_x + m_grid_size.x*grid_y;
    if (m_intgrid.count(id) > 0)
        return *(m_intgrid.at(id));
    return IntGridValue::None;
}

auto Layer::allEntities() const -> const std::vector<Entity>& {
    return m_entities;
}

auto Layer::hasEntity(const std::string& entity_name) const -> bool {
    return m_entities_by_name.count(entity_name) > 0 && !m_entities_by_name.at(entity_name).empty();
}

auto Layer::getEntitiesByName(const std::string& entity_name) const -> const std::vector<std::reference_wrapper<Entity>>& {
    if (m_entities_by_name.count(entity_name) > 0)
        return m_entities_by_name.at(entity_name);
    else
        return m_entities_by_name[entity_name];
}

auto Layer::getEntitiesByTag(const std::string& tag) const -> const std::vector<std::reference_wrapper<Entity>>& {
    if (m_entities_by_tag.count(tag) > 0)
        return m_entities_by_tag.at(tag);
    else
        return m_entities_by_tag[tag];
}

void Layer::updateTileVertices(const Tile& tile) const {
    updateTileVerticesPos(tile);
    updateTileVerticesTex(tile);
    updateTileVerticesCol(tile);
}

void Layer::updateTileVerticesPos(const Tile& tile) const {
    auto& verts = tile.vertices;
    auto cell_size = getCellSize();

    verts[0].pos.x = static_cast<float>(tile.position.x);           verts[1].pos.x = static_cast<float>(tile.position.x+cell_size);
    verts[0].pos.y = static_cast<float>(tile.position.y);           verts[1].pos.y = static_cast<float>(tile.position.y);

    verts[3].pos.x = static_cast<float>(tile.position.x);           verts[2].pos.x = static_cast<float>(tile.position.x+cell_size);
    verts[3].pos.y = static_cast<float>(tile.position.y+cell_size); verts[2].pos.y = static_cast<float>(tile.position.y+cell_size);
}

void Layer::updateTileVerticesTex(const Tile& tile) const {
    auto cell_size = getCellSize();
    IntPoint modif[4];
    IntPoint tex_coo[4] = {{0, 0}, {cell_size, 0}, {cell_size, cell_size}, {0, cell_size}};

    if (tile.flipX) {
        modif[0].x =  cell_size; modif[1].x = -cell_size;
        modif[3].x =  cell_size; modif[2].x = -cell_size;
    }
    if (tile.flipY) {
        modif[0].y =  cell_size; modif[1].y =  cell_size;
        modif[3].y = -cell_size; modif[2].y = -cell_size;
    }
    for (int i = 0; i < 4; ++i) {
        tile.vertices[i].tex.x = tile.texture_position.x + tex_coo[i].x + modif[i].x;
        tile.vertices[i].tex.y = tile.texture_position.y + tex_coo[i].y + modif[i].y;
    }
}

void Layer::updateTileVerticesCol(const Tile& tile) const {
    for (int i = 0; i < 4; ++i) {
        tile.vertices[i].col.a = unsigned(m_opacity*255)&0xffu;
    }
}
