// Created by Modar Nasser on 11/11/2020.

#include <fstream>

#include "LDtkLoader/World.hpp"
#include "LDtkLoader/Utils.hpp"

using namespace ldtk;

World::World() = default;

void World::loadFromFile(const std::string& filepath) {
    m_name = filepath;
    std::ifstream in(filepath);
    if (in.fail()) {
        ldtk_error("Failed to open file \"" + filepath + "\" : " + strerror(errno));
    }
    nlohmann::json j;
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
    m_enums_map.clear();
    m_enums_defs.clear();
    m_levels.clear();

    const auto& defs = j["defs"];

    // parsing enums first because tilesets need them for tags
    m_enums_map.reserve(defs["enums"].size());
    m_enums_defs.reserve(defs["enums"].size());
    for (const auto& en : defs["enums"]) {
        m_enums_defs.emplace_back(en, this);
        m_enums_map.emplace(en["identifier"].get<std::string>(), &m_enums_defs.back());
    }

    // parsing tilesets
    m_tilesets.reserve(defs["tilesets"].size());
    for (const auto& tileset : defs["tilesets"]) {
        m_tilesets.emplace_back(tileset, this);
    }

    // set enums tileset pointer
    for (auto& en : m_enums_defs) {
        if (en.m_tileset_id != -1) {
            en.m_tileset = &getTileset(en.m_tileset_id);
        }
    }

    // parsing layers defs
    m_layers_defs.reserve(defs["layers"].size());
    for (const auto& layer_def : defs["layers"]) {
        m_layers_defs.emplace_back(layer_def, this);
    }

    //parsing entities def
    m_entities_defs.reserve(defs["entities"].size());
    for (const auto& ent_def : defs["entities"]) {
        EntityDef new_entity_def{ent_def, this};
        m_entities_defs.emplace_back(ent_def, this);
    }

    // parsing levels
    m_levels.reserve(j["levels"].size());
    for (const auto& level : j["levels"]) {
        m_levels.emplace_back(level, this);
    }

    // fill levels neighbours
    for (auto& level : m_levels) {
        for (const auto& item : level.m_neighbours_id) {
            for (const auto& id : item.second)
                level.m_neighbours[item.first].push_back(&getLevel(id));
        }
        level.m_neighbours[Dir::None];
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
    for (const auto& en : m_enums_defs)
        if (en.uid == id)
            return en;
    ldtk_error("Enum ID \""+std::to_string(id)+"\" not found in World \""+m_name+"\".");
}

auto World::getEnum(const std::string& name) const -> const Enum& {
    if (m_enums_map.count(name) > 0)
        return *m_enums_map.at(name);
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
