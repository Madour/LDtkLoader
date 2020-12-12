// Created by Modar Nasser on 12/11/2020.

#include <iostream>

#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/World.hpp"
#include "LDtkLoader/Utils.hpp"

using namespace ldtk;

LayerDef::LayerDef(const nlohmann::json& j, World* w) :
type(getLayerTypeFromString(j["type"].get<std::string>())),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
cell_size(j["gridSize"].get<int>()),
opacity(j["displayOpacity"].get<float>()),
offset({j["pxOffsetX"].get<int>(), j["pxOffsetY"].get<int>()}),
pivot({j["tilePivotX"].get<float>(), j["tilePivotY"].get<float>()})
{
    if ( !j["tilesetDefUid"].is_null() )
        m_tileset = &w->getTileset(j["tilesetDefUid"].get<int>());
    if ( !j["autoTilesetDefUid"].is_null() )
        m_tileset = &w->getTileset(j["autoTilesetDefUid"].get<int>());
}

auto LayerDef::getTileset() const -> const Tileset& {
    if (m_tileset)
        return *m_tileset;
    ldtk_error("Layer \""+name+"\" doesn't have a tileset.");
}
