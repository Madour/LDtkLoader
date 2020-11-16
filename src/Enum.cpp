// Created by Modar Nasser on 13/11/2020.

#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Enum::Enum(const nlohmann::json& j, World* w) :
name(j["identifier"].get<std::string>()),
uid(j["uid"].get<unsigned int>()),
m_tileset( j["iconTilesetUid"].is_null() ? nullptr : &w->getTileset(j["iconTilesetUid"].get<unsigned int>()) )
{
    unsigned int id = 0;
    for (const auto& value : j["values"]) {
        const auto& val_name = value["id"].get<std::string>();
        const auto& tile_id = value["tileId"].is_null() ? 0u : value["tileId"].get<unsigned int>();
        m_values.insert({val_name, {val_name, id++, tile_id, uid}});
    }
}

auto Enum::operator[](const std::string& val_name) const -> const EnumValue& {
    if (m_values.count(val_name) > 0)
        return m_values.at(val_name);
    throw std::invalid_argument("Enum "+name+" does not have value "+val_name);
}

auto Enum::hasIcons() -> bool {
    return m_tileset != nullptr;
}

auto Enum::getIconsTileset() -> const Tileset& {
    if (m_tileset != nullptr)
        return *m_tileset;
    throw std::invalid_argument("Enum "+name+" values don't have icons.");
}

auto Enum::getIconTexturePos(const std::string& val_name) const -> UIntPoint {
    if (m_tileset == nullptr)
        throw std::invalid_argument("Enum "+name+" values don't have icons.");
    if (m_values.count(val_name) > 0)
        return m_tileset->getTileTexturePos(m_values.at(val_name).tile_id);
    throw std::invalid_argument("Enum "+name+" does not have value "+val_name);
}
