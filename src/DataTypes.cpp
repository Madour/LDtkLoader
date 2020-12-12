// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/DataTypes.hpp"

using namespace ldtk;

Color::Color() = default;

Color::Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) :
r(red&0xffu),
g(green&0xffu),
b(blue&0xffu),
a(alpha&0xffu)
{}

Color::Color(const std::string& hex) :
Color(std::stoul(hex.substr(1, 2), nullptr, 16),
      std::stoul(hex.substr(3, 2), nullptr, 16),
      std::stoul(hex.substr(5, 2), nullptr, 16))
{}

Tile::Tile(int coord_id, int tile_id) :
coordId(coord_id),
tileId(tile_id)
{}

const Tile Tile::None{-1, -1};

bool ldtk::operator==(const Tile& l, const Tile& r) {
    return (l.coordId == r.coordId) && (l.tileId == r.tileId);
}

bool ldtk::operator!=(const Tile& l, const Tile& r) {
    return !(ldtk::operator==(l, r));
}
