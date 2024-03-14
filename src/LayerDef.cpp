// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/defs/LayerDef.hpp"

#include <iostream>

#include "LDtkLoader/Project.hpp"
#include "json.hpp"

using namespace ldtk;

LayerDef::LayerDef(const nlohmann::json& j, Project* p) :
type(getLayerTypeFromString(j["type"].get<std::string>())),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
cell_size(j["gridSize"].get<int>()),
opacity(j["displayOpacity"].get<float>()),
offset(j["pxOffsetX"].get<int>(), j["pxOffsetY"].get<int>()),
tile_pivot(j["tilePivotX"].get<float>(), j["tilePivotY"].get<float>()),
m_tileset(j["tilesetDefUid"].is_null() ? nullptr : &p->getTileset(j["tilesetDefUid"].get<int>()))
{
    if (type == LayerType::IntGrid) {
        for (const auto& int_grid_val : j["intGridValues"]) {
            auto val = int_grid_val["value"].get<int>();
            auto identifier = int_grid_val["identifier"].is_null() ? "" : int_grid_val["identifier"].get<std::string>();
            auto color = Color(int_grid_val["color"].get<std::string>());
            m_intgrid_values.emplace(val, IntGridValue{val, identifier, color});
        }
    }
}
