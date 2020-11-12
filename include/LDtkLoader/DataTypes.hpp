// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cinttypes>
#include <string>

template <typename T>
struct Point {
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
    UIntPoint position;
    UIntPoint texture_position;
    bool flipX;
    bool flipY;
    unsigned int coordId;
    unsigned int tileId;
};

enum class LayerType {
    IntGrid,
    Entities,
    Tiles,
    AutoLayer
};