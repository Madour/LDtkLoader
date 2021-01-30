// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Layer.hpp"

namespace ldtk {

    class World;

    class Level {
        friend World;

        struct BgImage {
            FilePath path;
            IntPoint pos;
            FloatPoint scale;
            IntRect crop;
        };

    public:
        Level(Level&& other) noexcept ;

        const World* world;

        const std::string name;
        const int uid;
        const IntPoint size;
        const IntPoint position;
        const Color bg_color;

        auto allLayers() const -> const std::vector<Layer>&;
        auto getLayer(const std::string& layer_name) const -> const Layer&;

        auto hasBgImage() const -> bool;
        auto getBgImage() const -> const BgImage&;

        auto getNeighbours(const Dir& direction) const -> const std::vector<const Level*>&;
        auto getNeighbourDirection(const Level& level) const -> Dir;

    private:
        explicit Level(const nlohmann::json& j, World* w);

        std::vector<Layer> m_layers;
        std::experimental::optional<BgImage> m_bg_image;
        std::unordered_map<Dir, std::vector<int>> m_neighbours_id;
        std::unordered_map<Dir, std::vector<const Level*>> m_neighbours;
    };

}
