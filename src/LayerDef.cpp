// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/Utils.hpp"

using namespace ldtk;

LayerDef::LayerDef(const nlohmann::json& j) :
type(getLayerTypeFromString(j["type"].get<std::string>())),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<unsigned int>()),
cell_size(j["gridSize"].get<unsigned int>()),
opacity(j["displayOpacity"].get<float>()),
offset({j["pxOffsetX"].get<int>(), j["pxOffsetY"].get<int>()}),
pivot({j["tilePivotX"].get<float>(), j["tilePivotY"].get<float>()}),
tileset_id(j["tilesetDefUid"].is_null() ? -1 : j["tilesetDefUid"].get<int>()),
autotileset_id(j["autoTilesetDefUid"].is_null() ? -1 : j["autoTilesetDefUid"].get<int>())
{}