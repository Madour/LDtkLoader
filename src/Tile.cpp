// Created by Modar Nasser on 22/02/2022.

#include "LDtkLoader/Tile.hpp"
#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/Level.hpp"

using namespace ldtk;

const Tile Tile::None{nullptr, {-1, -1}, -1, 0, 0};

Tile::Tile(const Layer* l, IntPoint pos, int tile_id, int flips, float a) :
layer(l),
coordId(l == nullptr ? -1 : l->getCoordIdAt(pos.x, pos.y)),
tileId(tile_id),
flipX(flips & 1),
flipY((flips>>1) & 1),
alpha(a)
{}

auto Tile::getPosition() const -> IntPoint {
    auto cell_size = layer->getCellSize();
    auto grid_pos = getGridPosition();
    auto offset = layer->getOffset();
    return {grid_pos.x * cell_size + offset.x, grid_pos.y * cell_size + offset.y};
}

auto Tile::getGridPosition() const -> IntPoint {
    const auto& grid_width = layer->getGridSize().x;
    auto y = coordId / grid_width;
    auto x = coordId - y * grid_width;
    return {x, y};
}

auto Tile::getWorldPosition() const -> IntPoint {
    auto pos = getPosition();
    return {pos.x + layer->level->position.x, pos.y + layer->level->position.y};
}

auto Tile::getTextureRect() const -> IntRect {
    const auto& tile_size = layer->getTileset().tile_size;
    auto tex_pos = layer->getTileset().getTileTexturePos(tileId);
    return {tex_pos.x, tex_pos.y, tile_size, tile_size};
}

auto Tile::getVertices() const -> std::array<Vertex, 4> {
    std::array<Vertex, 4> verts;
    auto tex_rect = getTextureRect();
    auto pos = getPosition();
    auto& cell_size = tex_rect.width;

    // set vertices coordinates
    verts[0].pos.x = static_cast<float>(pos.x);           verts[1].pos.x = static_cast<float>(pos.x+cell_size);
    verts[0].pos.y = static_cast<float>(pos.y);           verts[1].pos.y = static_cast<float>(pos.y);

    verts[3].pos.x = static_cast<float>(pos.x);           verts[2].pos.x = static_cast<float>(pos.x+cell_size);
    verts[3].pos.y = static_cast<float>(pos.y+cell_size); verts[2].pos.y = static_cast<float>(pos.y+cell_size);

    // set vertices texture coordinates
    std::array<IntPoint, 4> modif;
    std::array<IntPoint, 4> tex_coo = {{ {0, 0}, {cell_size, 0}, {cell_size, cell_size}, {0, cell_size} }};

    if (flipX) {
        modif[0].x =  cell_size; modif[1].x = -cell_size;
        modif[3].x =  cell_size; modif[2].x = -cell_size;
    }
    if (flipY) {
        modif[0].y =  cell_size; modif[1].y =  cell_size;
        modif[3].y = -cell_size; modif[2].y = -cell_size;
    }
    for (int i = 0; i < 4; ++i) {
        verts[i].tex.x = tex_rect.x + tex_coo[i].x + modif[i].x;
        verts[i].tex.y = tex_rect.y + tex_coo[i].y + modif[i].y;
    }

    return verts;
}


auto ldtk::operator==(const Tile& l, const Tile& r) -> bool {
    return (l.coordId == r.coordId) && (l.tileId == r.tileId);
}

auto ldtk::operator!=(const Tile& l, const Tile& r) -> bool {
    return !(ldtk::operator==(l, r));
}
