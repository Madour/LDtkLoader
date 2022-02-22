// Created by Modar Nasser on 16/11/2020.

#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

EntityDef::EntityDef(const nlohmann::json& j, World* w) :
TagsContainer(j["tags"]),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
size(j["width"].get<int>(), j["height"].get<int>()),
color(j["color"].get<std::string>()),
pivot(j["pivotX"].get<float>(), j["pivotY"].get<float>()),
tileset(j["tileRect"].is_null() ? nullptr : &w->getTileset(j["tileRect"]["tilesetUid"].get<int>())),
texture_rect(j["tileRect"].is_null() ? IntRect{} : IntRect{j["tileRect"]["x"], j["tileRect"]["y"], j["tileRect"]["w"], j["tileRect"]["h"]})
{}
