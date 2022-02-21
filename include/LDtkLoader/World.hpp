// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/EntityDef.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Enum.hpp"

namespace ldtk {


    class World {
    public:
        World() = default;
        World(const World&) = delete;
        World(World&&) = default;
        auto operator=(const World&) -> World& = delete;

        void loadFromFile(const std::string& filepath);

        auto getFilePath() const -> const FilePath&;

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

    private:
        FilePath m_file_path;
        FloatPoint m_default_pivot;
        int m_default_cell_size = 0;
        Color m_background_color;

        WorldLayout m_layout = WorldLayout::Free;

        std::vector<Tileset> m_tilesets;
        std::vector<LayerDef> m_layers_defs;
        std::vector<EntityDef> m_entities_defs;
        std::vector<Enum> m_enums;

        std::vector<Level> m_levels;
    };

}
