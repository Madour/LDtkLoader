// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/DataTypes.hpp"

#include <utility>

using namespace ldtk;

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

auto ldtk::operator==(const Color& lhs, const Color& rhs) -> bool {
    return (lhs.r == rhs.r && lhs.g == rhs.g &&
            lhs.b == rhs.b && lhs.a == rhs.a);

}

const IntGridValue IntGridValue::None{-1, "", Color()};

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

IID::IID(std::string  iid): m_iid(std::move(iid))
{}

auto IID::str() const -> const std::string& {
    return m_iid;
}

auto ldtk::operator==(const IID& lhs, const IID& rhs) -> bool {
    return lhs.str() == rhs.str();
}

EntityRef::EntityRef(IID ent, IID  layer, IID level, IID world) :
entity_iid(std::move(ent)),
layer_iid(std::move(layer)),
level_iid(std::move(level)),
world_iid(std::move(world))
{}

auto EntityRef::operator->() const -> const Entity* {
    return ref;
}

auto operator<<(std::ostream& os, const ldtk::Color& color) -> std::ostream& {
    os << "rgb(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ")";
    return os;
}


auto operator<<(std::ostream& os, const ldtk::FilePath& path) -> std::ostream& {
    os << path.c_str();
    return os;
}

auto operator<<(std::ostream& os, const ldtk::IID& iid) -> std::ostream& {
    os << iid.str();
    return os;
}
