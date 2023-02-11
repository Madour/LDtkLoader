// Created by Modar Nasser on 25/02/2022.

#include "LDtkLoader/Project.hpp"

#include <fstream>
#include <istream>

#include "LDtkLoader/World.hpp"
#include "LDtkLoader/generated/Version.hpp"
#include "json.hpp"

using namespace ldtk;

namespace ldtk {
    std::vector<EntityRef*> temporary_entity_refs_array;
}

void Project::loadFromFile(const std::string& filepath) {
    m_file_path = filepath;

    std::ifstream in(filepath);
    if (in.fail()) {
        ldtk_error("Failed to open file \"" + filepath + "\" : " + strerror(errno));
    }
    nlohmann::json j;
    in >> j;

    load(j, nullptr, false);
}

void Project::loadFromFile(const std::string& filepath, const FileLoader& file_loader) {
    m_file_path = filepath;
    nlohmann::json j;
    std::istream(file_loader(filepath).get()) >> j;

    load(j, file_loader, false);
}

void Project::loadFromMemory(const std::vector<std::uint8_t>& bytes) {
    m_file_path = "<loaded_from_memory>";
    const nlohmann::json j = nlohmann::json::parse(bytes.data(), bytes.data() + bytes.size());

    load(j, nullptr, true);
}

void Project::loadFromMemory(unsigned char* data, unsigned int size) {
    m_file_path = "<loaded_from_memory>";
    const nlohmann::json j = nlohmann::json::parse(data, data + size);

    load(j, nullptr, true);
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

auto Project::getWorld(const IID& iid) const -> const World& {
    for (const auto& world : m_worlds)
        if (world.iid == iid)
            return world;
    ldtk_error("World with IID \""+iid.str()+"\" not found in Project \""+getFilePath().c_str()+"\".");
}

auto Project::allTocEntities() const -> const std::vector<EntityRef>& {
    return m_toc;
}

auto Project::getTocEntitiesByName(const std::string& name) const -> const std::vector<EntityRef>& {
    if (m_toc_map.count(name) > 0)
        return m_toc_map.at(name);
    else
        return m_toc_map[name];
}

void Project::load(const nlohmann::json& j, const FileLoader& file_loader, bool from_memory) {
    if (j.contains("iid")) {
        iid = IID(j["iid"]);
    }

    m_json_version = j["jsonVersion"].get<std::string>();

    // compare loaded json version with the library API version and issue a warning if mismatching
    {
        auto api_version = std::string(API_VERSION);
        auto json_major_minor = m_json_version.substr(0, m_json_version.find('.', 3));
        auto api_major_minor = api_version.substr(0, api_version.find('.', 3));

        if (json_major_minor != api_major_minor) {
            std::cout << "LDtkLoader Warning: Version mismatch. LDtkLoader v" << api_major_minor
                      << " may fail to load the project \"" << getFilePath().c_str()<< "\" created"
                      << " with LDtk v" << json_major_minor << ". Please make sure your software is"
                      << " up to date.";
        }
    }

    m_default_pivot.x = j["defaultPivotX"].get<float>();
    m_default_pivot.y = j["defaultPivotY"].get<float>();
    m_default_cell_size = j["defaultGridSize"].get<int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    // reset all containers
    m_enums.clear();
    m_tilesets.clear();
    m_layers_defs.clear();
    m_entities_defs.clear();
    m_worlds.clear();
    m_toc.clear();
    m_toc_map.clear();

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

    if (external_levels && from_memory) {
        ldtk_error("External levels are not supported when loading from memory.");
    }

    if (j["worlds"].empty()) {
        m_worlds.emplace_back(j, this, file_loader, external_levels);
    } else {
        for (const auto& world : j["worlds"]) {
            m_worlds.emplace_back(world, this, file_loader, external_levels);
        }
    }

    auto resolveEntityRef = [this](EntityRef& ref) {
        const auto& world = (m_worlds.size() == 1 ? getWorld() : getWorld(ref.world_iid));
        ref.ref = &world.getLevel(ref.level_iid)
                        .getLayer(ref.layer_iid)
                        .getEntity(ref.entity_iid);
    };

    // parse the table of content (v1.2.4)
    if (j.contains("toc")) {
        for (const auto& toc_entry: j["toc"]) {
            auto entity_name = toc_entry["identifier"].get<std::string>();
            for (const auto& ref: toc_entry["instances"]) {
                auto entity_ref = EntityRef(IID(ref["entityIid"]), IID(ref["layerIid"]),
                                            IID(ref["levelIid"]), IID(ref["worldIid"]));
                resolveEntityRef(entity_ref);

                m_toc.push_back(entity_ref);
                m_toc_map[entity_name].push_back(entity_ref);
            }
        }
    }

    // resolve all EntityRefs in the project
    for (auto* ref : temporary_entity_refs_array) {
        resolveEntityRef(*ref);
    }
    temporary_entity_refs_array.clear();
}
