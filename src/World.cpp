// Created by Modar Nasser on 11/11/2020.

#include "LDtkLoader/World.hpp"

#include <fstream>
#include <istream>

#include "LDtkLoader/Project.hpp"
#include "LDtkLoader/Utils.hpp"

#include "json.hpp"

using namespace ldtk;

World::World(const nlohmann::json& j, Project* p, const FileLoader& file_loader, bool external_levels)
: iid(j.contains("iid") ? j["iid"].get<std::string>() : "")
, m_project(p)
, m_name(j.contains("identifier") ? j["identifier"].get<std::string>() : "")
, m_layout(getWorldLayoutFromString(j["worldLayout"].get<std::string>()))
{
    // parse levels
    m_levels.reserve(j["levels"].size());
    if (!external_levels) {
        for (const auto& level : j["levels"]) {
            m_levels.emplace_back(level, this);
        }
    }
    else {
        nlohmann::json external_level;
        for (const auto& level : j["levels"]) {
            // read then create the external levels
            const auto external_rel_path = level["externalRelPath"].get<std::string>();
            auto filepath = m_project->getFilePath().directory() + external_rel_path;
            if (file_loader != nullptr) {
                std::istream(file_loader(filepath).get()) >> external_level;
            }
            else {
                std::ifstream in(filepath);
                if (in.fail()) {
                    ldtk_error("Failed to open file \"" + external_rel_path + "\" : " + strerror(errno));
                }
                in >> external_level;
            }
            m_levels.emplace_back(external_level, this);
        }
    }

    // fill levels neighbours
    for (auto& level : m_levels) {
        level.m_neighbours.emplace(Dir::None, 0);
        for (const auto& item : level.m_neighbours_id) {
            for (const auto& id : item.second) {
                level.m_neighbours[item.first].push_back(&getLevel(id));
            }
        }
    }
}

auto World::getName() const -> const std::string&
{
    return m_name;
}

auto World::getDefaultPivot() const -> const FloatPoint&
{
    return m_project->getDefaultPivot();
}

auto World::getDefaultCellSize() const -> int
{
    return m_project->getDefaultCellSize();
}

auto World::getBgColor() const -> const Color&
{
    return m_project->getBgColor();
}

auto World::getLayout() const -> const WorldLayout&
{
    return m_layout;
}

auto World::getLayerDef(int id) const -> const LayerDef&
{
    return m_project->getLayerDef(id);
}

auto World::getLayerDef(const std::string& name) const -> const LayerDef&
{
    return m_project->getLayerDef(name);
}

auto World::getEntityDef(int id) const -> const EntityDef&
{
    return m_project->getEntityDef(id);
}

auto World::getEntityDef(const std::string& name) const -> const EntityDef&
{
    return m_project->getEntityDef(name);
}

auto World::allTilesets() const -> const std::vector<Tileset>&
{
    return m_project->allTilesets();
}

auto World::getTileset(int id) const -> const Tileset&
{
    return m_project->getTileset(id);
}

auto World::getTileset(const std::string& name) const -> const Tileset&
{
    return m_project->getTileset(name);
}

auto World::getEnum(int id) const -> const Enum&
{
    return m_project->getEnum(id);
}

auto World::getEnum(const std::string& name) const -> const Enum&
{
    return m_project->getEnum(name);
}

auto World::allLevels() const -> const std::vector<Level>&
{
    return m_levels;
}

auto World::getLevel(int id) const -> const Level&
{
    for (const auto& level : m_levels) {
        if (level.uid == id) {
            return level;
        }
    }
    ldtk_error("Level ID \"" + std::to_string(id) + "\" not found in World \"" + m_name + "\".");
}

auto World::getLevel(const std::string& name) const -> const Level&
{
    for (const auto& level : m_levels) {
        if (level.name == name) {
            return level;
        }
    }
    ldtk_error("Level name \"" + name + "\" not found in World \"" + m_name + "\".");
}

auto World::getLevel(const IID& level_iid) const -> const Level&
{
    for (const auto& level : m_levels) {
        if (level.iid == level_iid) {
            return level;
        }
    }
    ldtk_error("Level with IID \"" + level_iid.str() + "\" not found in World \"" + m_name + "\".");
}
