// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/TagsContainer.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;

    class EntityDef : public TagsContainer {
    public:
        const std::string name;
        const int uid;
        const IntPoint size;
        const Color color;
        const FloatPoint pivot;
        const Tileset* tileset;
        const IntRect texture_rect;

        EntityDef(const nlohmann::json& j, World* w);
    };

}
