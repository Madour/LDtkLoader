// Created by Modar Nasser on 22/11/2020.

#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/World.hpp"

using namespace ldtk;

Entity::Entity(const nlohmann::json& j, const World* w) :
m_definition( &w->getEntityDef(j["defUid"].get<int>()) ),
m_size( j["width"].get<int>(), j["height"].get<int>() ),
m_position( j["px"][0].get<int>(), j["px"][1].get<int>() ),
m_grid_pos( j["__grid"][0].get<int>(), j["__grid"][1].get<int>() )
{
    parseFields(j["fieldInstances"], w);
}

auto Entity::getName() const -> const std::string& {
    return m_definition->name;
}

auto Entity::getSize() const -> const IntPoint& {
    return m_size;
}

auto Entity::getPosition() const -> const IntPoint& {
    return m_position;
}

auto Entity::getGridPosition() const -> const IntPoint& {
    return m_grid_pos;
}

auto Entity::getColor() const -> const Color& {
    return m_definition->color;
}

auto Entity::getPivot() const -> const FloatPoint& {
    return m_definition->pivot;
}

auto Entity::hasIcon() const -> bool {
    return m_definition->tileset != nullptr;
}

auto Entity::getIconTileset() const -> const Tileset& {
    return *m_definition->tileset;
}

auto Entity::getIconTexturePos() const -> IntPoint {
    return m_definition->tileset->getTileTexturePos(m_definition->tile_id);
}
