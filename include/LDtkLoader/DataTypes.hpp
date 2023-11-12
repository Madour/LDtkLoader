// Created by Modar Nasser on 11/11/2020.

#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <streambuf>
#include <string>

namespace ldtk {

    template <typename T>
    struct Point {
        Point() : x(0), y(0) {}
        Point(T x, T y) : x(x), y(y) {}
        T x;
        T y;
    };

    template <typename T>
    auto operator==(const Point<T>& lhs, const Point<T>& rhs) -> bool {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }

    using FloatPoint = Point<float>;
    using IntPoint = Point<int>;
    using UIntPoint = Point<unsigned int>;

    template <typename T>
    struct Rect {
        Rect() : x(0), y(0), width(0), height(0) {}
        Rect(T x, T y, T w, T h) : x(x), y(y), width(w), height(h) {}
        Rect(const Point<T>& pos, const Point<T>& size) : x(pos.x), y(pos.y), width(size.x), height(size.y) {}
        T x; T y;
        T width; T height;
    };

    template <typename T>
    auto operator==(const Rect<T>& lhs, const Rect<T>& rhs) -> bool {
        return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height);
    }

    template <typename T>
    auto operator!=(const Rect<T>& lhs, const Rect<T>& rhs) -> bool {
        return !(lhs == rhs);
    }

    using FloatRect = Rect<float>;
    using IntRect = Rect<int>;

    struct NineSliceBorders {
        int top;
        int right;
        int bottom;
        int left;
    };

    struct Color {
        Color() = default;
        explicit Color(const std::string& hex);
        explicit Color(int hex);
        Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha=0xffu);

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 0xffu;
    };

    auto operator==(const Color& lhs, const Color& rhs) -> bool;

    struct Vertex {
        FloatPoint pos = {0.f, 0.f};
        IntPoint tex = {0, 0};
    };

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

        auto directory() const -> std::string;
        auto filename() const -> std::string;
        auto extension() const -> std::string;
    };

    using FileLoader = std::function<std::unique_ptr<std::streambuf> (const std::string &)>;

    struct IID {
        IID() = default;
        explicit IID(std::string iid);
        auto str() const -> const std::string&;
    private:
        std::string m_iid;
    };

    auto operator==(const IID& lhs, const IID& rhs) -> bool;

    class Entity;
    struct EntityRef {
        EntityRef(IID ent, IID  layer, IID level, IID world);
        IID entity_iid;
        IID layer_iid;
        IID level_iid;
        IID world_iid;
        auto operator->() const -> const Entity*;
    private:
        friend class Project;
        const Entity* ref = nullptr;
    };
}

template <typename T>
auto operator<<(std::ostream& os, const ldtk::Point<T>& point) -> std::ostream& {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}
template <typename T>
auto operator<<(std::ostream& os, const ldtk::Rect<T>& rect) -> std::ostream& {
    os << "(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height << ")";
    return os;
}
auto operator<<(std::ostream& os, const ldtk::Color& color) -> std::ostream&;
auto operator<<(std::ostream& os, const ldtk::FilePath& path) -> std::ostream&;
auto operator<<(std::ostream& os, const ldtk::IID& iid) -> std::ostream&;
