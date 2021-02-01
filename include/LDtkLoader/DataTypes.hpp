// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cinttypes>
#include <string>
#include <array>
#include <iostream>

namespace ldtk {

    template <typename T>
    struct Point {
        Point() : x(0), y(0) {}
        Point(T x, T y) : x(x), y(y) {}
        T x;
        T y;
    };

    template <typename T>
    bool operator==(const Point<T>& lhs, const Point<T>& rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }

    typedef Point<float> FloatPoint;
    typedef Point<int> IntPoint;
    typedef Point<unsigned int> UIntPoint;

    template <typename T>
    struct Rect {
        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(T x, T y, T w, T h) : x(x), y(y), width(w), height(h) {}
        T x; T y;
        T width; T height;
    };

    template <typename T>
    bool operator==(const Rect<T>& lhs, const Rect<T>& rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height);
    }

    typedef Rect<float> FloatRect;
    typedef Rect<int> IntRect;

    struct Color {
        Color();
        explicit Color(const std::string& hex);
        Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha=0xffu);

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 0xffu;
    };

    bool operator==(const Color& lhs, const Color& rhs);

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

    struct IntGridValue {
        const int value;
        const std::string name;
        const Color color;
        static const IntGridValue None;
    };

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

    class FilePath : std::string {
    public:
        FilePath() = default;
        FilePath(const std::string& str);
        using std::string::basic_string;
        using std::string::c_str;

        std::string parent_path() const;
        std::string filename() const;
        std::string extension() const;
    };
}

template <typename T>
auto operator<<(std::ostream& os, const ldtk::Point<T>& pt) -> std::ostream& {
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}
auto operator<<(std::ostream& os, const ldtk::Color& col) -> std::ostream&;
auto operator<<(std::ostream& os, const ldtk::FilePath& fp) -> std::ostream&;
