// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/LayerDef.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    class Level;

    class Layer {
        friend Level;
    public:
        const LayerType type;
        const std::string name;
        const UIntPoint grid_size;
        const unsigned int cell_size;

        auto getOffset() const -> const IntPoint&;
        void setOffset(const IntPoint& offset);

        auto getOpacity() const -> float;
        void setOpacity(float opacity);

        auto hasTileset() const -> bool;
        auto getTileset() const -> const Tileset&;

    private:
        explicit Layer(const nlohmann::json& j);
        void setLayerDef(const LayerDef& layer_def);
        void setTileset(const Tileset& tileset);

        const LayerDef* m_layer_def = nullptr;
        const Tileset* m_tileset = nullptr;
        IntPoint m_total_offset;
        float m_opacity;
    };

}
