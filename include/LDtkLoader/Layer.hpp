// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/defs/LayerDef.hpp"
#include "LDtkLoader/thirdparty/json_fwd.hpp"

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Entity.hpp"
#include "LDtkLoader/Tile.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class World;

    class Level;

    class Layer
    {
    public:
        ~Layer() = default;
        Layer(const Layer&) = delete;
        Layer(Layer&&) noexcept = default;
        auto operator=(const Layer&) -> Layer& = delete;
        auto operator=(Layer&&) -> Layer& = delete;

        const Level* const level;
        const IID iid;

        auto getType() const -> const LayerType&;
        auto getName() const -> const std::string&;

        auto isVisible() const -> bool;

        auto getCellSize() const -> int;
        auto getGridSize() const -> const IntPoint&;

        auto getOffset() const -> const IntPoint&;

        auto getOpacity() const -> float;

        auto hasTileset() const -> bool;
        auto getTileset() const -> const Tileset&;

        auto allTiles() const -> const std::vector<Tile>&;
        auto getTile(int grid_x, int grid_y) const -> const Tile&;
        auto getTilesByEnumTag(const EnumValue& enumvalue) const -> const std::vector<ref_wrapper<Tile>>&;

        auto getIntGridVal(int grid_x, int grid_y) const -> const IntGridValue&;
        auto getIntGridValPositions(int intgridval_value) const -> const std::vector<IntPoint>&;
        auto getIntGridValPositions(const std::string& intgridval_name) const -> const std::vector<IntPoint>&;

        auto hasEntity(const std::string& entity_name) const -> bool;
        auto allEntities() const -> const std::vector<Entity>&;
        auto getEntitiesByName(const std::string& name) const -> const std::vector<ref_wrapper<Entity>>&;
        auto getEntitiesByTag(const std::string& tag) const -> const std::vector<ref_wrapper<Entity>>&;
        auto getEntity(const IID& entity_iid) const -> const Entity&;

        auto getCoordIdAt(int grid_x, int grid_y) const -> int;
        auto getGridPositionFromCoordId(int coord_id) const -> IntPoint;

        Layer(const nlohmann::json& j, const World* w, const Level* l);

    private:
        const LayerDef* const m_definition;
        const Tileset* const m_override_tileset;

        bool m_visible;
        IntPoint m_total_offset;
        float m_opacity;
        const IntPoint m_grid_size;

        std::vector<Tile> m_tiles;
        std::map<int, ref_wrapper<Tile>> m_tiles_map;
        std::map<std::string, std::vector<ref_wrapper<Tile>>> m_tiles_by_enumtag;

        std::map<int, ref_wrapper<const IntGridValue>> m_intgrid;
        mutable std::map<int, std::vector<IntPoint>> m_intgridpos_by_value;
        mutable std::map<std::string, std::vector<IntPoint>> m_intgridpos_by_name;

        std::vector<Entity> m_entities;
        mutable std::unordered_map<std::string, std::vector<ref_wrapper<Entity>>> m_entities_by_name;
        mutable std::unordered_map<std::string, std::vector<ref_wrapper<Entity>>> m_entities_by_tag;
    };

} // namespace ldtk
