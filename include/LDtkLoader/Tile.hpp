// Created by Modar Nasser on 22/02/2022.

#pragma once

#include <array>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {

    class Layer;

    class Tile {
    public:
        const Layer* layer = nullptr;

        const int coordId = 0;
        const int tileId  = 0;

        const bool flipX = false;
        const bool flipY = false;

        auto getPosition() const -> IntPoint;
        auto getGridPosition() const -> IntPoint;
        auto getWorldPosition() const -> IntPoint;

        auto getTextureRect() const -> IntRect;

        auto getVertices() const -> std::array<Vertex, 4>;

        static const Tile None;

        Tile(const Layer* l, IntPoint pos, int tile_id, int flips);
    };

    auto operator==(const Tile& l, const Tile& r) -> bool;
    auto operator!=(const Tile& l, const Tile& r) -> bool;

}
