// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>
#include <vector>
#include <map>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/FieldsContainer.hpp"

namespace ldtk {

    class World;

    class Level : public FieldsContainer {
        friend World;

    public:
        struct BgImage {
            FilePath path;
            IntPoint pos;
            FloatPoint scale;
            IntRect crop;
        };

        Level(const Level&) = delete;
        Level(Level&&) = default;
        auto operator=(const Level&) -> Level& = delete;

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

        explicit Level(const nlohmann::json& j, World* w);

    private:
        std::vector<Layer> m_layers;
        std::experimental::optional<BgImage> m_bg_image;
        std::map<Dir, std::vector<int>> m_neighbours_id;
        std::map<Dir, std::vector<const Level*>> m_neighbours;
    };

}
