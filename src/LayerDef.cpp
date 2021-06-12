// Created by Modar Nasser on 12/11/2020.

#include <iostream>

#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

LayerDef::LayerDef(const nlohmann::json& j, World* w) :
type(getLayerTypeFromString(j["type"].get<std::string>())),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
cell_size(j["gridSize"].get<int>()),
opacity(j["displayOpacity"].get<float>()),
offset({j["pxOffsetX"].get<int>(), j["pxOffsetY"].get<int>()}),
tile_pivot({j["tilePivotX"].get<float>(), j["tilePivotY"].get<float>()})
{
    if ( !j["tilesetDefUid"].is_null() )
        m_tileset = &w->getTileset(j["tilesetDefUid"].get<int>());
    else if ( !j["autoTilesetDefUid"].is_null() )
        m_tileset = &w->getTileset(j["autoTilesetDefUid"].get<int>());

    int i = 0;
    for (const auto& val : j["intGridValues"]) {
        m_intgrid_values.push_back({
            i++,
            val["identifier"].is_null() ? "" : val["identifier"],
            Color(val["color"].get<std::string>())
        });
    }
}

