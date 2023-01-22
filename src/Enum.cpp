// Created by Modar Nasser on 13/11/2020.

#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/Utils.hpp"
#include "json.hpp"

using namespace ldtk;

EnumValue::EnumValue(std::string val_name, int val_id, int val_tile_id, const Color& val_color, const Enum& val_enum_type) :
name(std::move(val_name)),
color(val_color),
type(val_enum_type),
id(val_id),
tile_id(val_tile_id)
{}

auto EnumValue::hasIcon() const -> bool {
    return tile_id != -1;
}

auto EnumValue::getIconTileset() const -> const Tileset& {
    return type.getIconsTileset();
}

auto EnumValue::getIconTexturePos() const -> IntPoint {
    return getIconTileset().getTileTexturePos(tile_id);
}

auto ldtk::operator==(const EnumValue& l, const EnumValue& r) -> bool {
    return (l.id == r.id) && (l.type.uid == r.type.uid);
}
auto ldtk::operator!=(const EnumValue& l, const EnumValue& r) -> bool {
    return !(ldtk::operator==(l, r));
}

Enum::Enum(const nlohmann::json& j) :
TagsContainer(j.contains("tags") ? j["tags"] : nlohmann::json{}),
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
m_tileset_id(j["iconTilesetUid"].is_null() ? -1 : j["iconTilesetUid"].get<int>())
{
    int id = 0;
    for (const auto& value : j["values"]) {
        const auto& val_name = value["id"].get<std::string>();
        const auto& tile_id = value["tileId"].is_null() ? -1 : value["tileId"].get<int>();
        m_values.insert({val_name, {val_name, id++, tile_id, Color(value["color"].get<int>()), *this}});
    }
}

auto Enum::operator[](const std::string& val_name) const -> const EnumValue& {
    if (m_values.count(val_name) > 0)
        return m_values.at(val_name);
    ldtk_error("Enum \""+name+"\" does not have value \""+val_name+"\".");
}

auto Enum::hasIcons() const -> bool {
    return m_tileset != nullptr;
}

auto Enum::getIconsTileset() const -> const Tileset& {
    if (m_tileset != nullptr)
        return *m_tileset;
    ldtk_error("Enum \""+name+"\" values don't have icons.");
}

auto operator<<(std::ostream& os, const ldtk::EnumValue& enum_value) -> std::ostream& {
    os << enum_value.type.name << "." << enum_value.name;
    return os;
}
