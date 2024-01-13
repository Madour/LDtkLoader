// Created by Modar Nasser on 22/02/2022.

#pragma once

#include <array>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class Layer;

    class Tile {
    public:
        const Layer* const layer;

        const int coordId = 0;
        const int tileId  = 0;

        const bool flipX = false;
        const bool flipY = false;

        const float alpha;

        auto getPosition() const -> IntPoint;
        auto getGridPosition() const -> IntPoint;
        auto getWorldPosition() const -> IntPoint;

        auto getTextureRect() const -> IntRect;

        auto getVertices() const -> std::array<Vertex, 4>;

        static const Tile None;

        Tile(const Layer* l, IntPoint pos, int tile_id, int flips, float a);
    };

    auto operator==(const Tile& l, const Tile& r) -> bool;
    auto operator!=(const Tile& l, const Tile& r) -> bool;

}
