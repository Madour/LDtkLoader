// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/Level.hpp"
#include <fstream>
#include <iostream>

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
}

auto Level::getDefaultPivot() const -> Point<float> {
    return {m_default_pivot_x, m_default_pivot_y};
}

auto Level::getDefaultGridSize() const -> unsigned int {
    return m_default_grid_size;
}

auto Level::getBgColor() const -> const Color& {
    return m_background_color;
}
