// Created by Modar Nasser on 16/11/2020.

#include "LDtkLoader/defs/EntityDef.hpp"

#include "LDtkLoader/Project.hpp"

#include "json.hpp"

using namespace ldtk;

EntityDef::EntityDef(const nlohmann::json& j, Project* p)
: TagsContainer(j["tags"])
, name(j["identifier"].get<std::string>())
, uid(j["uid"].get<int>())
, size(j["width"].get<int>(), j["height"].get<int>())
, color(j["color"].get<std::string>())
, pivot(j["pivotX"].get<float>(), j["pivotY"].get<float>())
, tileset(j["tileRect"].is_null() ? nullptr : &p->getTileset(j["tileRect"]["tilesetUid"].get<int>()))
, texture_rect(
      j["tileRect"].is_null()
          ? IntRect{}
          : IntRect{j["tileRect"]["x"], j["tileRect"]["y"], j["tileRect"]["w"], j["tileRect"]["h"]}
  )
, nine_slice_borders({-1, -1, -1, -1})
{
    if (j.contains("nineSliceBorders") && !j["nineSliceBorders"].empty()) {
        auto borders = j["nineSliceBorders"].get<std::array<int, 4>>();
        nine_slice_borders.top = borders[0];
        nine_slice_borders.right = borders[1];
        nine_slice_borders.bottom = borders[2];
        nine_slice_borders.left = borders[3];
    }

    for (const auto& fd : j["fieldDefs"]) {
        auto field_type = fd["__type"].get<std::string>();
        auto field_name = fd["identifier"].get<std::string>();
        fields.emplace_back(FieldDef{getFieldTypeFromString(field_type), field_name});
    }
}
