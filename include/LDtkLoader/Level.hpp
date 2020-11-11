// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {


    class Level {
    public:
        Level();
        void loadFromFile(const std::string& filepath);

        auto getDefaultPivot() const -> Point<float>;

        auto getDefaultGridSize() const -> unsigned int;

        auto getBgColor() const -> const Color&;

    private:
        float m_default_pivot_x = 0.f;
        float m_default_pivot_y = 0.f;
        unsigned int m_default_grid_size = 0;
        Color m_background_color;

        std::vector<Tileset> m_tilesets;
    };

}




