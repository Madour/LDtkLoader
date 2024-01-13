// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <map>
#include <string>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class Project;

    class LayerDef {
        friend class Layer;

    public:
        const LayerType type;
        const std::string name;
        const int uid;
        const int cell_size;
        const float opacity;
        const IntPoint offset;
        const FloatPoint tile_pivot;

        LayerDef(const nlohmann::json& j, Project* p);

    private:
        const Tileset* const m_tileset;
        const Tileset* const m_auto_tileset;        // tileset for Auto layers based on a grid layer
        std::map<int, IntGridValue> m_intgrid_values;
    };

}
