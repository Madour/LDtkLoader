// Created by Modar Nasser on 25/02/2022.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"

#include "LDtkLoader/defs/EntityDef.hpp"
#include "LDtkLoader/defs/LayerDef.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/World.hpp"

namespace ldtk {
    class Project {
    public:
        Project() = default;
        Project(const Project&) = delete;
        Project(Project&&) = default;
        auto operator=(const Project&) -> Project& = delete;

        IID iid;

        void loadFromFile(const std::string& filepath);
        void loadFromFile(const std::string& filepath, const FileLoader& file_loader);

        void loadFromMemory(const std::vector<std::uint8_t>& bytes);
        void loadFromMemory(const unsigned char* data, size_t size);

        auto getFilePath() const -> const FilePath&;

        auto getDefaultPivot() const -> const FloatPoint&;

        auto getDefaultCellSize() const -> int;

        auto getBgColor() const -> const Color&;

        auto getLayerDef(int id) const -> const LayerDef&;
        auto getLayerDef(const std::string& name) const -> const LayerDef&;

        auto getEntityDef(int id) const -> const EntityDef&;
        auto getEntityDef(const std::string& name) const -> const EntityDef&;

        auto allTilesets() const -> const std::vector<Tileset>&;
        auto getTileset(int id) const -> const Tileset&;
        auto getTileset(const std::string& name) const -> const Tileset&;

        auto getEnum(int id) const -> const Enum&;
        auto getEnum(const std::string& name) const -> const Enum&;

        auto allWorlds() const -> const std::vector<World>&;
        auto getWorld() const -> const World&;
        auto getWorld(const std::string& name) const -> const World&;
        auto getWorld(const IID& iid) const -> const World&;

        auto allTocEntities() const -> const std::vector<EntityRef>&;
        auto getTocEntitiesByName(const std::string& name) const -> const std::vector<EntityRef>&;

    private:
        void load(const nlohmann::json& j, const FileLoader& file_loader, bool from_memory);

        FilePath m_file_path;
        FloatPoint m_default_pivot;
        int m_default_cell_size = 0;
        Color m_background_color;
        std::string m_json_version;

        std::vector<LayerDef> m_layers_defs;
        std::unordered_map<int, std::reference_wrapper<LayerDef>> m_layers_defs_by_uid;
        std::unordered_map<std::string, std::reference_wrapper<LayerDef>> m_layers_defs_by_name;

        std::vector<EntityDef> m_entities_defs;
        std::unordered_map<int, std::reference_wrapper<EntityDef>> m_entities_defs_by_uid;
        std::unordered_map<std::string, std::reference_wrapper<EntityDef>> m_entities_defs_by_name;

        std::vector<Tileset> m_tilesets;
        std::unordered_map<int, std::reference_wrapper<Tileset>> m_tilesets_by_uid;
        std::unordered_map<std::string, std::reference_wrapper<Tileset>> m_tilesets_by_name;

        std::vector<Enum> m_enums;
        std::unordered_map<int, std::reference_wrapper<Enum>> m_enums_by_uid;
        std::unordered_map<std::string, std::reference_wrapper<Enum>> m_enums_by_name;

        std::vector<World> m_worlds;

        std::vector<EntityRef> m_toc;
        mutable std::map<std::string, std::vector<EntityRef>> m_toc_map;
    };
}
