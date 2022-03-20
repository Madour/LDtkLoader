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
{
    for (const auto& fd : j["fieldDefs"]) {
        auto field_type = fd["__type"].get<std::string>();
        auto field_name = fd["identifier"].get<std::string>();

        // array fields
        if (field_type.find("Array") != std::string::npos) {
            if (field_type == "Array<Int>") {
                fields.emplace_back(FieldDef{FieldType::ArrayInt, field_name});
            }
            else if (field_type == "Array<Float>") {
                fields.emplace_back(FieldDef{FieldType::ArrayFloat, field_name});
            }
            else if (field_type == "Array<Bool>") {
                fields.emplace_back(FieldDef{FieldType::ArrayBool, field_name});
            }
            else if (field_type == "Array<String>" || field_type == "Array<Multilines>") {
                fields.emplace_back(FieldDef{FieldType::ArrayString, field_name});
            }
            else if (field_type == "Array<Color>") {
                fields.emplace_back(FieldDef{FieldType::ArrayColor, field_name});
            }
            else if (field_type == "Array<Point>") {
                fields.emplace_back(FieldDef{FieldType::ArrayPoint, field_name});
            }
            else if (field_type.find("LocalEnum") != std::string::npos) {
                fields.emplace_back(FieldDef{FieldType::ArrayEnum, field_name});
            }
            else if (field_type == "Array<FilePath>") {
                fields.emplace_back(FieldDef{FieldType::ArrayFilePath, field_name});
            }
            else if (field_type == "Array<EntityRef>") {
                fields.emplace_back(FieldDef{FieldType::ArrayEntityRef, field_name});
            }
        }
        else if (field_type == "Int") {
            fields.emplace_back(FieldDef{FieldType::Int, field_name});
        }
        else if (field_type == "Float") {
            fields.emplace_back(FieldDef{FieldType::Float, field_name});
        }
        else if (field_type == "Bool") {
            fields.emplace_back(FieldDef{FieldType::Bool, field_name});
        }
        else if (field_type == "String" || field_type == "Multilines") {
            fields.emplace_back(FieldDef{FieldType::String, field_name});
        }
        else if (field_type == "Color") {
            fields.emplace_back(FieldDef{FieldType::Color, field_name});
        }
        else if (field_type == "Point") {
            fields.emplace_back(FieldDef{FieldType::Point, field_name});
        }
        else if (field_type.find("LocalEnum") != std::string::npos) {
            fields.emplace_back(FieldDef{FieldType::Enum, field_name});
        }
        else if (field_type == "FilePath") {
            fields.emplace_back(FieldDef{FieldType::FilePath, field_name});
        }
        else if (field_type == "EntityRef") {
            fields.emplace_back(FieldDef{FieldType::EntityRef, field_name});
        }
    }
}
