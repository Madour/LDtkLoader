// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class World;

    class LayerDef {
        friend World;
    public:
        const LayerType type;
        const std::string name;
        const unsigned int uid;
        const unsigned int cell_size;
        const float opacity;
        const IntPoint offset;
        const FloatPoint pivot;
        const int tileset_id;
        const int autotileset_id;

    private:
        explicit LayerDef(const nlohmann::json& j);
    };

}
