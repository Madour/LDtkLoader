// Created by Modar Nasser on 22/02/2022.

#include "LDtkLoader/Tile.hpp"

#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/Level.hpp"

using namespace ldtk;

const Tile Tile::None{nullptr, {-1, -1}, -1, -1, 0, 0};

Tile::Tile(const Layer* l, IntPoint pos, int coord_id, int tile_id, int flips, float a)
: layer(l)
, coordId(coord_id)
, tileId(tile_id)
, flipX((flips & 1) != 0)
, flipY(((flips >> 1) & 1) != 0)
, alpha(a)
, m_position(pos)
{}

auto Tile::getPosition() const -> IntPoint
{
    auto offset = layer->getOffset();
    return {m_position.x + offset.x, m_position.y + offset.y};
}

auto Tile::getGridPosition() const -> IntPoint
{
    return layer->getGridPositionFromCoordId(coordId);
}

auto Tile::getWorldPosition() const -> IntPoint
{
    auto pos = getPosition();
    return {pos.x + layer->level->position.x, pos.y + layer->level->position.y};
}

auto Tile::getTextureRect() const -> IntRect
{
    const auto& tile_size = layer->getTileset().tile_size;
    auto tex_pos = layer->getTileset().getTileTexturePos(tileId);
    return {tex_pos.x, tex_pos.y, tile_size, tile_size};
}

auto Tile::getVertices() const -> std::array<Vertex, 4>
{
    const auto tex_rect = getTextureRect();
    const auto pos = getPosition();
    const auto& cell_size = tex_rect.width;

    const auto left = static_cast<float>(pos.x);
    const auto top = static_cast<float>(pos.y);
    const auto right = static_cast<float>(pos.x + cell_size);
    const auto bottom = static_cast<float>(pos.y + cell_size);

    // clang-format off
    // set vertices coordinates
    std::array<Vertex, 4> verts{};
    verts[0].pos.x = left;          verts[1].pos.x = right;
    verts[0].pos.y = top;           verts[1].pos.y = top;

    verts[3].pos.x = left;          verts[2].pos.x = right;
    verts[3].pos.y = bottom;        verts[2].pos.y = bottom;

    // compute vertices texture coordinates
    std::array<IntPoint, 4> tex_coo{};
    tex_coo[0].x = 0;               tex_coo[1].x = cell_size;
    tex_coo[0].y = 0;               tex_coo[1].y = 0;

    tex_coo[3].x = 0;               tex_coo[2].x = cell_size;
    tex_coo[3].y = cell_size;       tex_coo[2].y = cell_size;

    std::array<IntPoint, 4> modif{};
    if (flipX) {
        modif[0].x = cell_size;     modif[1].x = -cell_size;
        modif[3].x = cell_size;     modif[2].x = -cell_size;
    }
    if (flipY) {
        modif[0].y = cell_size;     modif[1].y = cell_size;
        modif[3].y = -cell_size;    modif[2].y = -cell_size;
    }

    // set vertices texture coordinates
    for (size_t i = 0; i < verts.size(); ++i) {
        verts[i].tex.x = tex_rect.x + tex_coo.at(i).x + modif.at(i).x;
        verts[i].tex.y = tex_rect.y + tex_coo.at(i).y + modif.at(i).y;
    }
    // clang-format on

    return verts;
}


auto ldtk::operator==(const Tile& l, const Tile& r) -> bool
{
    return (l.coordId == r.coordId) && (l.tileId == r.tileId);
}

auto ldtk::operator!=(const Tile& l, const Tile& r) -> bool
{
    return !(ldtk::operator==(l, r));
}
