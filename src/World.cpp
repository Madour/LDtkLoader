// Created by Modar Nasser on 11/11/2020.

#include <fstream>

#include "LDtkLoader/World.hpp"
#include "LDtkLoader/Utils.hpp"

using namespace ldtk;
using json = nlohmann::json;

World::World() = default;

void World::loadFromFile(const std::string& filepath) {
    m_name = filepath;
    std::ifstream in(filepath);
    json j;
    in >> j;

    m_default_pivot_x = j["defaultPivotX"].get<float>();
    m_default_pivot_y = j["defaultPivotY"].get<float>();
    m_default_cell_size = j["defaultGridSize"].get<int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    auto layout = j["worldLayout"].get<std::string>();
    if (layout == "Free")
        m_layout = WorldLayout::Free;
    else if (layout == "GridVania")
        m_layout = WorldLayout::GridVania;
    else if (layout == "LinearHorizontal")
        m_layout = WorldLayout::LinearHorizontal;
    else if (layout == "LinearVertical")
        m_layout = WorldLayout::LinearVertical;

    // reset all containers
    m_layers_defs.clear();
    m_tilesets.clear();
    m_enums.clear();
    m_levels.clear();

    const auto& defs = j["defs"];

    // parsing tilesets
    for (const auto& tileset : defs["tilesets"]) {
        Tileset new_tileset{tileset};
        m_tilesets.push_back(std::move(new_tileset));
    }

    // parsing layers defs
    for (const auto& layer_def : defs["layers"]) {
        LayerDef new_layer_def{layer_def, this};
        m_layers_defs.push_back(std::move(new_layer_def));
    }

    // parsing enums
    for (const auto& en : defs["enums"]) {
        m_enums.insert({en["identifier"].get<std::string>(), std::move(Enum(en, this))});
    }

    //parsing entities def
    for (const auto& ent_def : defs["entities"]) {
        EntityDef new_entity_def{ent_def, this};
        m_entities_defs.push_back(std::move(new_entity_def));
    }

    // parsing levels
    for (const auto& level : j["levels"]) {
        Level new_level{level, this};
        m_levels.push_back(std::move(new_level));
    }
}

auto World::getName() const -> const std::string& {
    return m_name;
}

auto World::getDefaultPivot() const -> FloatPoint {
    return {m_default_pivot_x, m_default_pivot_y};
}

auto World::getDefaultCellSize() const -> int {
    return m_default_cell_size;
}

auto World::getBgColor() const -> const Color& {
    return m_background_color;
}

auto World::getLayout() const -> const WorldLayout& {
    return m_layout;
}

auto World::getLayerDef(int id) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.uid == id)
            return layer_def;
    ldtk_error("LayerDef ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getLayerDef(const std::string& name) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.name == name)
            return layer_def;
    ldtk_error("LayerDef name \""+name+"\" not found in World \""+m_name+"\".");
}

auto World::getEntityDef(int id) const -> const EntityDef& {
    for (const auto& entity_def : m_entities_defs)
        if (entity_def.uid == id)
            return entity_def;
    ldtk_error("EntityDef ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getEntityDef(const std::string& name) const -> const EntityDef& {
    for (const auto& entity_def : m_entities_defs)
        if (entity_def.name == name)
            return entity_def;
    ldtk_error("EntityDef name \""+name+"\" not found in World \""+m_name+"\".");
}

auto World::getTileset(int id) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.uid == id)
            return tileset;
    ldtk_error("Tileset ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getTileset(const std::string& name) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.name == name)
            return tileset;
    ldtk_error("Tileset name \""+name+"\" not found in World \""+m_name+"\".");
}

auto World::getEnum(int id) const -> const Enum& {
    for (const auto& item : m_enums)
        if (item.second.uid == id)
            return item.second;
    ldtk_error("Enum ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getEnum(const std::string& name) const -> const Enum& {
    if (m_enums.count(name) > 0)
        return m_enums.at(name);
    ldtk_error("Enum \""+name+"\" not found in World \""+m_name+"\".");
}

auto World::allLevels() const -> const std::vector<Level>& {
    return m_levels;
}

auto World::getLevel(int id) const -> const Level& {
    for (const auto& level : m_levels)
        if (level.uid == id)
            return level;
    ldtk_error("Level ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getLevel(const std::string& name) const -> const Level& {
    for (const auto& level : m_levels)
        if (level.name == name)
            return level;
    ldtk_error("Level name \""+name+"\" not found in World \""+m_name+"\".");
}
