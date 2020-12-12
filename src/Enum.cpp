// Created by Modar Nasser on 13/11/2020.

#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

EnumValue EnumValue::None{"", -1, -1, -1};

bool ldtk::operator==(const EnumValue& l, const EnumValue& r) {
    return (l.id == r.id) && (l.type_id == r.type_id);
}
bool ldtk::operator!=(const EnumValue& l, const EnumValue& r) {
    return !(ldtk::operator==(l, r));
}

Enum::Enum(const nlohmann::json& j, const World* w) :
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<int>()),
m_tileset( j["iconTilesetUid"].is_null() ? nullptr : &w->getTileset(j["iconTilesetUid"].get<int>()) )
{
    int id = 0;
    for (const auto& value : j["values"]) {
        const auto& val_name = value["id"].get<std::string>();
        const auto& tile_id = value["tileId"].is_null() ? 0 : value["tileId"].get<int>();
        m_values.insert({val_name, {val_name, id++, tile_id, uid}});
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

auto Enum::getIconTexturePos(const std::string& val_name) const -> IntPoint {
    if (m_values.count(val_name) > 0)
        return getIconsTileset().getTileTexturePos(m_values.at(val_name).tile_id);
    ldtk_error("Enum \""+name+"\" does not have value \""+val_name+"\".");
}

auto Enum::getIconTexturePos(const EnumValue& val) const -> IntPoint {
    if (val.type_id == uid)
        return getIconsTileset().getTileTexturePos(val.tile_id);
    ldtk_error("Enum \""+name+"\" does not have value \""+val.name+"\".");
}
