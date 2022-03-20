// Created by Modar Nasser on 16/11/2020.

#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/Project.hpp"

using namespace ldtk;

EntityDef::EntityDef(const nlohmann::json& j, Project* p) :
TagsContainer(j["tags"]),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
size(j["width"].get<int>(), j["height"].get<int>()),
color(j["color"].get<std::string>()),
pivot(j["pivotX"].get<float>(), j["pivotY"].get<float>()),
tileset(j.contains("tileRect") ?
        (j["tileRect"].is_null() ? nullptr : &p->getTileset(j["tileRect"]["tilesetUid"].get<int>())) :
        (j["tilesetId"].is_null() ? nullptr : &p->getTileset(j["tilesetId"].get<int>())) // v0.9.3
),
texture_rect(j.contains("tileRect") ?
        (j["tileRect"].is_null() ?
            IntRect{} :
            IntRect{j["tileRect"]["x"], j["tileRect"]["y"], j["tileRect"]["w"], j["tileRect"]["h"]}) :
        (j["tileId"].is_null() ? // v0.9.3
            IntRect{} :
            IntRect{tileset->getTileTexturePos(j["tileId"].get<int>()), {tileset->tile_size, tileset->tile_size}})
)
{}
