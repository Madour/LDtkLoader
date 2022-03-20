// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/containers/TagsContainer.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class Project;

    class EntityDef : public TagsContainer {
    public:
        const std::string name;
        const int uid;
        const IntPoint size;
        const Color color;
        const FloatPoint pivot;
        const Tileset* tileset;
        const IntRect texture_rect;

        EntityDef(const nlohmann::json& j, Project* p);
    };

}
