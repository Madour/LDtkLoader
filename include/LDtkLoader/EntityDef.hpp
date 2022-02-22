// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;

    class EntityDef {
    public:
        const std::string name;
        const int uid;
        const IntPoint size;
        const Color color;
        const FloatPoint pivot;
        const Tileset* tileset;
        const IntRect texture_rect;
        const std::vector<std::string> tags;

        EntityDef(const nlohmann::json& j, World* w);
    };

}
