// Created by Modar Nasser on 25/02/2022.

#include "LDtkLoader/Project.hpp"

#include <fstream>
#include <istream>

#include "LDtkLoader/Utils.hpp"
#include "LDtkLoader/Version.hpp"
#include "LDtkLoader/World.hpp"
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

    const nlohmann::json j = nlohmann::json::parse(in);
    load(j, nullptr, false);
}

void Project::loadFromFile(const std::string& filepath, const FileLoader& file_loader) {
    m_file_path = filepath;

    auto streambuf = file_loader(filepath);
    std::istream in(streambuf.get());
    if (in.fail()) {
        ldtk_error("Failed to open file \"" + filepath + "\" using custom file loader : " + strerror(errno));
    }

    const nlohmann::json j = nlohmann::json::parse(in);
    load(j, file_loader, false);
}

void Project::loadFromMemory(const std::vector<std::uint8_t>& bytes) {
    loadFromMemory(bytes.data(), bytes.size());
}

void Project::loadFromMemory(const unsigned char* data, size_t size) {
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
    if (m_layers_defs_by_uid.find(id) != m_layers_defs_by_uid.end())
        return m_layers_defs_by_uid.at(id);

    ldtk_error("LayerDef ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getLayerDef(const std::string& name) const -> const LayerDef& {
    if (m_layers_defs_by_name.find(name) != m_layers_defs_by_name.end())
        return m_layers_defs_by_name.at(name);

    ldtk_error("LayerDef name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEntityDef(int id) const -> const EntityDef& {
    if (m_entities_defs_by_uid.find(id) != m_entities_defs_by_uid.end())
        return m_entities_defs_by_uid.at(id);

    ldtk_error("EntityDef ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEntityDef(const std::string& name) const -> const EntityDef& {
    if (m_entities_defs_by_name.find(name) != m_entities_defs_by_name.end())
        return m_entities_defs_by_name.at(name);

    ldtk_error("EntityDef name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::allTilesets() const -> const std::vector<Tileset>& {
    return m_tilesets;
}

auto Project::getTileset(int id) const -> const Tileset& {
    if (m_tilesets_by_uid.find(id) != m_tilesets_by_uid.end())
        return m_tilesets_by_uid.at(id);

    ldtk_error("Tileset ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getTileset(const std::string& name) const -> const Tileset& {
    if (m_tilesets_by_name.find(name) != m_tilesets_by_name.end())
        return m_tilesets_by_name.at(name);

    ldtk_error("Tileset name \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEnum(int id) const -> const Enum& {
    if (m_enums_by_uid.find(id) != m_enums_by_uid.end())
        return m_enums_by_uid.at(id);

    ldtk_error("Enum ID \""+std::to_string(id)+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::getEnum(const std::string& name) const -> const Enum& {
    if (m_enums_by_name.find(name) != m_enums_by_name.end())
        return m_enums_by_name.at(name);

    ldtk_error("Enum \""+name+"\" not found in Project \""+m_file_path.filename()+"\".");
}

auto Project::allWorlds() const -> const std::vector<World>& {
    return m_worlds;
}

auto Project::getWorld() const -> const World& {
    if (m_worlds.size() == 1)
        return m_worlds[0];

    ldtk_error("Your LDtk project has Multi-Worlds option enabled. "
               "Please specify the name of the world you want to get.");
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
        auto api_version = std::string(API_VERSION_STRING);
        auto json_major_minor = m_json_version.substr(0, m_json_version.find('.', 3));
        auto api_major_minor = api_version.substr(0, api_version.find('.', 3));

        if (json_major_minor != api_major_minor) {
            std::cout << "LDtkLoader Warning: Version mismatch. LDtkLoader v" << API_VERSION_STRING
                      << " may fail to load the project \"" << getFilePath().c_str()<< "\" created"
                      << " with LDtk v" << json_major_minor << ". Please make sure your software is"
                      << " up to date." << std::endl;
        }
    }

    m_default_pivot.x = j["defaultPivotX"].get<float>();
    m_default_pivot.y = j["defaultPivotY"].get<float>();
    m_default_cell_size = j["defaultGridSize"].get<int>();

    m_background_color = Color(j["bgColor"].get<std::string>());

    // reset all containers
    m_layers_defs.clear();
    m_layers_defs_by_uid.clear();
    m_layers_defs_by_name.clear();
    m_entities_defs.clear();
    m_entities_defs_by_uid.clear();
    m_entities_defs_by_name.clear();
    m_tilesets.clear();
    m_tilesets_by_uid.clear();
    m_tilesets_by_name.clear();
    m_enums.clear();
    m_enums_by_uid.clear();
    m_enums_by_name.clear();
    m_worlds.clear();
    m_toc.clear();
    m_toc_map.clear();

    const auto& defs = j["defs"];

    // parse enums first because tilesets need them for tags
    m_enums.reserve(defs["enums"].size());
    for (const auto& en : defs["enums"]) {
        m_enums.emplace_back(en);
        auto& new_enum = m_enums.back();
        m_enums_by_uid.emplace(new_enum.uid, new_enum);
        m_enums_by_name.emplace(new_enum.name, new_enum);
    }

    // parse tilesets
    m_tilesets.reserve(defs["tilesets"].size());
    for (const auto& tileset : defs["tilesets"]) {
        m_tilesets.emplace_back(tileset, this);
        auto& new_tileset = m_tilesets.back();
        m_tilesets_by_uid.emplace(new_tileset.uid, new_tileset);
        m_tilesets_by_name.emplace(new_tileset.name, new_tileset);
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
        auto& new_def = m_layers_defs.back();
        m_layers_defs_by_uid.emplace(new_def.uid, new_def);
        m_layers_defs_by_name.emplace(new_def.name, new_def);
    }

    //parse entities def
    m_entities_defs.reserve(defs["entities"].size());
    for (const auto& ent_def : defs["entities"]) {
        m_entities_defs.emplace_back(ent_def, this);
        auto& new_def = m_entities_defs.back();
        m_entities_defs_by_uid.emplace(new_def.uid, new_def);
        m_entities_defs_by_name.emplace(new_def.name, new_def);
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
