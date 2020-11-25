// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cinttypes>
#include <string>
#include <array>

namespace ldtk {

    template <typename T>
    struct Point {
        Point() {x = 0; y = 0;}
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
        Color(unsigned int red, unsigned int green, unsigned int bblue);

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;

    };

    struct Vertex {
        FloatPoint pos = {0.f, 0.f};
        UIntPoint tex = {0, 0};
        Color col = Color(255, 255, 255);
    };

    struct Tile {
        unsigned int coordId=0;
        unsigned int tileId=0;
        UIntPoint position;
        UIntPoint texture_position;
        bool flipX=false;
        bool flipY=false;
        mutable std::array<Vertex, 4> vertices;
    };

    enum class LayerType {
        IntGrid,
        Entities,
        Tiles,
        AutoLayer
    };
}
