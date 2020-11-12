// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"

namespace ldtk {
    class Level;

    class Layer {
    public:
        enum class Type {
            IntGrid,
            Entities,
            Tiles,
            AutoLayer
        };

        friend Level;

        const Type type;
        const std::string name;
        const UIntPoint grid_size;
        const unsigned int cell_size;

        auto getOffset() const -> const IntPoint&;
        void setOffset(const IntPoint& offset);

        auto getOpacity() const -> float;
        void setOpacity(float opacity);

    private:
        explicit Layer(const nlohmann::json& j);

        IntPoint m_total_offset;
        float m_opacity;

        static Type getTypeFromString(const std::string& type_name);
    };

}
