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

Color::Color(int hex) :
Color((hex>>16)&0xff, (hex>>8)&0xff, hex&0xff)
{}

Color::Color(const std::string& hex) :
Color(std::stoul(hex.substr(1, 2), nullptr, 16),
      std::stoul(hex.substr(3, 2), nullptr, 16),
      std::stoul(hex.substr(5, 2), nullptr, 16))
{}

bool ldtk::operator==(const Color& lhs, const Color& rhs) {
    return (lhs.r == rhs.r && lhs.g == rhs.g &&
            lhs.b == rhs.b && lhs.a == rhs.a);

}

Tile::Tile(int coord_id, int tile_id) :
coordId(coord_id),
tileId(tile_id)
{}

const Tile Tile::None{-1, -1};

const IntGridValue IntGridValue::None{-1, "", Color()};

bool ldtk::operator==(const Tile& l, const Tile& r) {
    return (l.coordId == r.coordId) && (l.tileId == r.tileId);
}

bool ldtk::operator!=(const Tile& l, const Tile& r) {
    return !(ldtk::operator==(l, r));
}

ldtk::FilePath::FilePath(const std::string& str) {
    this->assign(str);
}

auto ldtk::FilePath::directory() const -> std::string {
    auto i = find_last_of("/\\");
    if (i == npos)
        return "";
    return substr(0, i+1);
}

auto ldtk::FilePath::filename() const -> std::string {
    auto i = find_last_of("/\\");
    if (i == npos)
        return *this;
    return substr(i+1);
}

auto ldtk::FilePath::extension() const -> std::string {
    auto f = filename();
    auto i = f.find_last_of('.');
    if (i == npos)
        return "";
    return f.substr(i+1);
}

auto operator<<(std::ostream& os, const ldtk::Color& col) -> std::ostream& {
    os << "rgb(" << (int)col.r << ", " << (int)col.g << ", " << (int)col.b << ")";
    return os;
}


auto operator<<(std::ostream& os, const ldtk::FilePath& fp) -> std::ostream& {
    os << fp.c_str();
    return os;
}
