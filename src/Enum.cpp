// Created by Modar Nasser on 13/11/2020.

#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/Utils.hpp"
#include "json.hpp"

using namespace ldtk;

EnumValue::EnumValue(std::string val_name, int val_id, const IntRect& val_tile_rect, const Color& val_color, const Enum& val_enum_type) :
name(std::move(val_name)),
color(val_color),
type(val_enum_type),
id(val_id),
tile_rect(val_tile_rect)
{}

auto EnumValue::hasIcon() const -> bool {
    return type.hasIcons() && tile_rect != IntRect(-1, -1, -1, -1);
}

auto EnumValue::getIconTileset() const -> const Tileset& {
    return type.getIconsTileset();
}

auto EnumValue::getIconTextureRect() const -> const IntRect& {
    return tile_rect;
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
        const auto& j_tile_rect = value["tileRect"];
        const auto tile_rect = j_tile_rect.is_null() ? IntRect{-1, -1, -1, -1}
                                                     : IntRect{j_tile_rect["x"].get<int>(),
                                                               j_tile_rect["y"].get<int>(),
                                                               j_tile_rect["w"].get<int>(),
                                                               j_tile_rect["h"].get<int>()};
        const auto color = Color(value["color"].get<int>());
        m_values.insert({val_name, {val_name, id++, tile_rect, color, *this}});
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
