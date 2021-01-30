// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;
    class Layer;

    class LayerDef {
        friend World;
        friend Layer;

    public:
        const LayerType type;
        const std::string name;
        const int uid;
        const int cell_size;
        const float opacity;
        const IntPoint offset;
        const FloatPoint tile_pivot;

    private:
        LayerDef(const nlohmann::json& j, World* w);

        const Tileset* m_tileset = nullptr;
        std::vector<IntGridValue> m_intgrid_values;
    };

}
