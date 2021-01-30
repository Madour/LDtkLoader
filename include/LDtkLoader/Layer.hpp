// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/Tileset.hpp"
#include "LDtkLoader/Entity.hpp"

namespace ldtk {

    class World;
    class Level;

    class Layer {
        friend Level;
    public:
        Layer(Layer&& other) noexcept;

        const Level* level = nullptr;

        auto getType() const -> const LayerType&;
        auto getName() const -> const std::string&;

        auto getCellSize() const -> int;
        auto getGridSize() const -> const IntPoint&;

        auto getOffset() const -> const IntPoint&;
        void setOffset(const IntPoint& offset) const;

        auto getOpacity() const -> float;
        void setOpacity(float opacity) const;

        auto hasTileset() const -> bool;
        auto getTileset() const -> const Tileset&;

        auto allTiles() const -> const std::vector<Tile>&;
        auto getTile(int grid_x, int grid_y) const -> const Tile&;

        auto getIntGridVal(int grid_x, int grid_y) const -> const IntGridValue&;

        auto hasEntity(const std::string& entity_name) const -> bool;
        auto getEntities(const std::string& entity_name) const -> const std::vector<Entity>&;

    private:
        Layer(const nlohmann::json& j, const World* w, const Level* l);

        void updateTileVertices(const Tile& tile) const;
        void updateTileVerticesPos(const Tile& tile) const;
        void updateTileVerticesTex(const Tile& tile) const;
        void updateTileVerticesCol(const Tile& tile) const;

        const LayerDef* m_definition = nullptr;

        mutable IntPoint m_total_offset;
        mutable float m_opacity;
        const IntPoint m_grid_size;

        std::vector<Tile> m_tiles;
        std::unordered_map<std::string, std::vector<Entity>> m_entities;
        std::map<int, const IntGridValue*> m_intgrid;
        std::map<int, Tile*> m_tiles_map;
    };

}
