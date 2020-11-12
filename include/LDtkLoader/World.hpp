// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/Level.hpp"

namespace ldtk {


    class World {
    public:
        World();
        void loadFromFile(const std::string& filepath);

        auto getDefaultPivot() const -> Point<float>;

        auto getDefaultGridSize() const -> unsigned int;

        auto getBgColor() const -> const Color&;

        auto getLayerDef(unsigned int id) const -> const LayerDef&;
        auto getLayerDef(const std::string& name) const -> const LayerDef&;

        auto getTileset(unsigned int id) const -> const Tileset&;
        auto getTileset(const std::string& name) const -> const Tileset&;

        auto allLevels() const -> const std::vector<Level>&;

    private:
        std::string m_name;
        float m_default_pivot_x = 0.f;
        float m_default_pivot_y = 0.f;
        unsigned int m_default_cell_size = 0;
        Color m_background_color;

        std::vector<Tileset> m_tilesets;
        std::vector<LayerDef> m_layers_defs;

        std::vector<Level> m_levels;
    };

}
