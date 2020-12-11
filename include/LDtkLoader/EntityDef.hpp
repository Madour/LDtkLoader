// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;

    class EntityDef {
        friend World;
    public:
        const std::string name;
        int uid;
        const IntPoint size;
        const Color color;
        const FloatPoint pivot;

    private:
        EntityDef(const nlohmann::json& j, World* w);

        const Tileset* m_tileset;
        int m_tile_id;
    };

}
