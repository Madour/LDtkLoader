// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cinttypes>
#include <string>
#include <array>

namespace ldtk {

    template <typename T>
    struct Point {
        Point() : x(0), y(0) {}
        Point(T x, T y) : x(x), y(y) {}
        T x;
        T y;
    };

    typedef Point<float> FloatPoint;
    typedef Point<int> IntPoint;
    typedef Point<unsigned int> UIntPoint;

    struct Color {
        Color();
        explicit Color(const std::string& hex);
        Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha=0xffu);

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 0xffu;
    };

    struct Vertex {
        FloatPoint pos = {0.f, 0.f};
        IntPoint tex = {0, 0};
        Color col = {255, 255, 255};
    };

    struct Tile {
        Tile() = default;
        int coordId=0;
        int tileId=0;
        IntPoint position;
        IntPoint world_position;
        IntPoint texture_position;
        bool flipX=false;
        bool flipY=false;
        mutable std::array<Vertex, 4> vertices;
        static const Tile None;
    private:
        Tile(int coord_id, int tile_id);
    };

    bool operator==(const Tile& l, const Tile& r);
    bool operator!=(const Tile& l, const Tile& r);

    enum class WorldLayout {
        Free,
        GridVania,
        LinearHorizontal,
        LinearVertical
    };

    enum class LayerType {
        IntGrid,
        Entities,
        Tiles,
        AutoLayer
    };

    enum class Dir {
        None,
        North,
        East,
        South,
        West
    };
}
