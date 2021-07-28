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
    if (!j["__tile"].is_null()) {
        m_tileset = &w->getTileset(j["__tile"]["tilesetUid"].get<int>());
        m_src_rect.x = j["__tile"]["srcRect"][0].get<int>();
        m_src_rect.y = j["__tile"]["srcRect"][1].get<int>();
        m_src_rect.width = j["__tile"]["srcRect"][2].get<int>();
        m_src_rect.height = j["__tile"]["srcRect"][3].get<int>();
    }
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

auto Entity::hasTile() const -> bool {
    return m_tileset != nullptr;
}

auto Entity::getTileset() const -> const Tileset& {
    return *m_tileset;
}

auto Entity::getTextureRect() const -> const IntRect& {
    return m_src_rect;
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

auto Entity::hasTag(const std::string& tag) -> bool {
    auto pos = std::find(m_definition->tags.begin(), m_definition->tags.end(), tag);
    return pos != m_definition->tags.end();
}

auto Entity::getTags() -> const std::vector<std::string>& {
    return m_definition->tags;
}
