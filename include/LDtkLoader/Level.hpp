// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Layer.hpp"

namespace ldtk {

    class World;

    class Level {
        friend World;
    public:
        Level(Level&& other) noexcept ;

        const std::string name;
        const unsigned int uid;
        const UIntPoint size;

        auto allLayers() const -> const std::vector<Layer>&;
        auto getLayer(const std::string& layer_name) const -> const Layer&;

    private:
        explicit Level(const nlohmann::json& j, World* w);

        std::vector<Layer> m_layers;
    };

}
