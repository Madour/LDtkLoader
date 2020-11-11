// Created by Modar Nasser on 11/11/2020.

#include <fstream>
#include <iostream>

#include "LDtkLoader/Level.hpp"

using namespace ldtk;
using json = nlohmann::json;

Level::Level() = default;

void Level::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    json j;
    in >> j;

    m_default_pivot_x = j["defaultPivotX"].get<float>();
    m_default_pivot_y = j["defaultPivotY"].get<float>();
    m_default_grid_size = j["defaultGridSize"].get<unsigned int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    const auto& defs = j["defs"];

    // parsing tilesets
    for (const auto& tileset : defs["tilesets"]) {
        Tileset new_tileset{tileset};
        m_tilesets.push_back(new_tileset);
    }
}

auto Level::getDefaultPivot() const -> FloatPoint {
    return {m_default_pivot_x, m_default_pivot_y};
}

auto Level::getDefaultGridSize() const -> unsigned int {
    return m_default_grid_size;
}

auto Level::getBgColor() const -> const Color& {
    return m_background_color;
}
