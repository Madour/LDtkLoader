// Created by Modar Nasser on 25/02/2022.

#include "LDtkLoader/Project.hpp"

#include <fstream>

#include "LDtkLoader/World.hpp"

using namespace ldtk;

void Project::loadFromFile(const std::string& filepath) {
    m_file_path = filepath;
    std::ifstream in(filepath);
    if (in.fail()) {
        ldtk_error("Failed to open file \"" + filepath + "\" : " + strerror(errno));
    }
    nlohmann::json j;
    in >> j;

    m_default_pivot.x = j["defaultPivotX"].get<float>();
    m_default_pivot.y = j["defaultPivotY"].get<float>();
    m_default_cell_size = j["defaultGridSize"].get<int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    // reset all containers
    m_enums.clear();
    m_tilesets.clear();
    m_layers_defs.clear();
    m_entities_defs.clear();

    const auto& defs = j["defs"];

    // parse enums first because tilesets need them for tags
    m_enums.reserve(defs["enums"].size());
    for (const auto& en : defs["enums"]) {
        m_enums.emplace_back(en);
    }

    // parse tilesets
    m_tilesets.reserve(defs["tilesets"].size());
    for (const auto& tileset : defs["tilesets"]) {
        m_tilesets.emplace_back(tileset, this);
    }

    // set enums tileset pointer
    for (auto& en : m_enums) {
        if (en.m_tileset_id != -1) {
            en.m_tileset = &getTileset(en.m_tileset_id);
        }
    }

    // parse layers defs
    m_layers_defs.reserve(defs["layers"].size());
    for (const auto& layer_def : defs["layers"]) {
        m_layers_defs.emplace_back(layer_def, this);
    }

    //parse entities def
    m_entities_defs.reserve(defs["entities"].size());
    for (const auto& ent_def : defs["entities"]) {
        m_entities_defs.emplace_back(ent_def, this);
    }

    // parse worlds
    auto external_levels = j["externalLevels"].get<bool>();
    if (j["worlds"].empty()) {
        m_worlds.emplace_back(j, this, external_levels);
    } else {
        for (const auto& world : j["worlds"]) {
            m_worlds.emplace_back(world, this, external_levels);
        }
    }

    // TODO : resolve EntityRefs using iids
}

auto Project::getFilePath() const -> const FilePath& {
    return m_file_path;
}

auto Project::getDefaultPivot() const -> const FloatPoint& {
    return m_default_pivot;
}

auto Project::getDefaultCellSize() const -> int {
    return m_default_cell_size;
}

auto Project::getBgColor() const -> const Color& {
    return m_background_color;
}

auto Project::getLayerDef(int id) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.uid == id)
            return layer_def;
    ldtk_error("LayerDef ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getLayerDef(const std::string& name) const -> const LayerDef& {
    for (const auto& layer_def : m_layers_defs)
        if (layer_def.name == name)
            return layer_def;
    ldtk_error("LayerDef name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEntityDef(int id) const -> const EntityDef& {
    for (const auto& entity_def : m_entities_defs)
        if (entity_def.uid == id)
            return entity_def;
    ldtk_error("EntityDef ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEntityDef(const std::string& name) const -> const EntityDef& {
    for (const auto& entity_def : m_entities_defs)
        if (entity_def.name == name)
            return entity_def;
    ldtk_error("EntityDef name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::allTilesets() const -> const std::vector<Tileset>& {
    return m_tilesets;
}

auto Project::getTileset(int id) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.uid == id)
            return tileset;
    ldtk_error("Tileset ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getTileset(const std::string& name) const -> const Tileset& {
    for (const auto& tileset : m_tilesets)
        if (tileset.name == name)
            return tileset;
    ldtk_error("Tileset name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEnum(int id) const -> const Enum& {
    for (const auto& en : m_enums)
        if (en.uid == id)
            return en;
    ldtk_error("Enum ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEnum(const std::string& name) const -> const Enum& {
    for (const auto& en : m_enums)
        if (en.name == name)
            return en;
    ldtk_error("Enum \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::allWorlds() const -> const std::vector<World>& {
    return m_worlds;
}

auto Project::getWorld() const -> const World& {
    if (m_worlds.size() == 1) {
        return m_worlds[0];
    } else {
        ldtk_error("Your LDtk project has Multi-Worlds option enabled. "
                   "Please specify the name of the world you want to get.");
    }
}

auto Project::getWorld(const std::string& name) const -> const World& {
    for (const auto& world : m_worlds)
        if (world.getName() == name)
            return world;
    ldtk_error("World name \""+name+"\" not found in Project \""+getFilePath().c_str()+"\".");
}
