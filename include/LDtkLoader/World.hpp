// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json_fwd.hpp"

#include "LDtkLoader/defs/LayerDef.hpp"
#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Enum.hpp"

namespace ldtk {

    class Project;

    class World {
    public:
        World(const World&) = delete;
        World(World&&) = default;
        auto operator=(const World&) -> World& = delete;

        const IID iid;

        auto getName() const -> const std::string&;

        auto getDefaultPivot() const -> const FloatPoint&;

        auto getDefaultCellSize() const -> int;

        auto getBgColor() const -> const Color&;

        auto getLayout() const -> const WorldLayout&;

        auto getLayerDef(int id) const -> const LayerDef&;
        auto getLayerDef(const std::string& name) const -> const LayerDef&;

        auto getEntityDef(int id) const -> const EntityDef&;
        auto getEntityDef(const std::string& name) const -> const EntityDef&;

        auto allTilesets() const -> const std::vector<Tileset>&;
        auto getTileset(int id) const -> const Tileset&;
        auto getTileset(const std::string& name) const -> const Tileset&;

        auto getEnum(int id) const -> const Enum&;
        auto getEnum(const std::string& name) const -> const Enum&;

        auto allLevels() const -> const std::vector<Level>&;
        auto getLevel(int id) const -> const Level&;
        auto getLevel(const std::string& name) const -> const Level&;
        auto getLevel(const IID& iid) const -> const Level&;

        World(const nlohmann::json& j, Project* p, const FileLoader& file_loader, bool external_levels);

    private:
        const Project* const m_project;
        std::string m_name;

        WorldLayout m_layout = WorldLayout::Free;
        std::vector<Level> m_levels;
    };

}
