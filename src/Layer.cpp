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
        new_tile.position.x = tile["px"].get<std::vector<int>>()[0];
        new_tile.position.y = tile["px"].get<std::vector<int>>()[1];

        new_tile.world_position.x = static_cast<int>(new_tile.position.x) + l->position.x;
        new_tile.world_position.y = static_cast<int>(new_tile.position.y) + l->position.y;

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
                m_intgrid[coord_id] = &m_definition->m_intgrid_values[val.get<int>()-1];
            coord_id++;
        }
    }
    // LDtk pre v0.8
    else {
        for (const auto& val : j["intGrid"]) {
            m_intgrid[val["coordId"].get<int>()] = &m_definition->m_intgrid_values[val["v"].get<int>()];
        }
    }

    for (const auto& ent : j["entityInstances"]) {
        m_entities[ent["__identifier"]].emplace_back(ent, w);
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
    return m_definition->m_tileset != nullptr;
}

auto Layer::getTileset() const -> const Tileset& {
    return *m_definition->m_tileset;
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

auto Layer::hasEntity(const std::string& entity_name) const -> bool {
    return m_entities.count(entity_name) > 0;
}

auto Layer::getEntities(const std::string& entity_name) const -> const std::vector<Entity>& {
    if (m_entities.count(entity_name) > 0)
        return m_entities.at(entity_name);
    throw std::invalid_argument("Layer "+getName()+" does not have Entity named "+entity_name);
}

void Layer::updateTileVertices(const Tile& tile) const {
    updateTileVerticesPos(tile);
    updateTileVerticesTex(tile);
    updateTileVerticesCol(tile);
}

void Layer::updateTileVerticesPos(const Tile& tile) const {
    auto& verts = tile.vertices;
    auto& offset = m_total_offset;
    auto cell_size = getCellSize();
    verts[0].pos.x = static_cast<float>(tile.position.x+offset.x);           verts[0].pos.y = static_cast<float>(tile.position.y+offset.y);
    verts[1].pos.x = static_cast<float>(tile.position.x+cell_size+offset.x); verts[1].pos.y = static_cast<float>(tile.position.y+offset.y);
    verts[2].pos.x = static_cast<float>(tile.position.x+cell_size+offset.x); verts[2].pos.y = static_cast<float>(tile.position.y+cell_size+offset.y);
    verts[3].pos.x = static_cast<float>(tile.position.x+offset.x);           verts[3].pos.y = static_cast<float>(tile.position.y+cell_size+offset.y);
}

void Layer::updateTileVerticesTex(const Tile& tile) const {
    IntPoint modif[4];
    auto cell_size_i = static_cast<int>(getCellSize());
    if (tile.flipX) {
        modif[0].x =  cell_size_i; modif[1].x = -cell_size_i;
        modif[3].x =  cell_size_i; modif[2].x = -cell_size_i;
    }
    if (tile.flipY) {
        modif[0].y =  cell_size_i; modif[1].y =  cell_size_i;
        modif[3].y = -cell_size_i; modif[2].y = -cell_size_i;
    }
    UIntPoint tex_coo[4] = { {0, 0},  {16, 0},  {16, 16},  {0, 16}};
    for (int i = 0; i < 4; ++i) {
        tile.vertices[i].tex.x = tile.texture_position.x+tex_coo[i].x+modif[i].x;
        tile.vertices[i].tex.y = tile.texture_position.y+tex_coo[i].y+modif[i].y;
    }
}

void Layer::updateTileVerticesCol(const Tile& tile) const {
    for (int i = 0; i < 4; ++i) {
        tile.vertices[i].col.a = unsigned(m_opacity*255)&0xffu;
    }
}
