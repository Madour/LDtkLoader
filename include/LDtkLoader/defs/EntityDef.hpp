// Created by Modar Nasser on 16/11/2020.

#pragma once

#include <array>
#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/containers/TagsContainer.hpp"
#include "LDtkLoader/defs/FieldDef.hpp"
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
        std::vector<FieldDef> fields;
        NineSliceBorders nine_slice_borders;

        EntityDef(const nlohmann::json& j, Project* p);
    };

}
