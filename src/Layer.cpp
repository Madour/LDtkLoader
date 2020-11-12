// Created by Modar Nasser on 12/11/2020.

#include "Layer.hpp"
#include <cmath>

using namespace ldtk;

Layer::Layer(const nlohmann::json& j) :
type(getTypeFromString(j["__type"].get<std::string>())),
name(j["__identifier"].get<std::string>()),
grid_size({j["__cWid"].get<unsigned int>(), j["__cHei"].get<unsigned int>()}),
cell_size(j["grid_size"].get<unsigned int>()),
m_total_offset({j["__pxTotalOffsetX"].get<int>(), j["__pxTotalOffsetY"].get<int>()}),
m_opacity(j["__opacity"].get<float>())
{

}

auto Layer::getOffset() const -> const IntPoint& {
    return m_total_offset;
}

void Layer::setOffset(const IntPoint& offset) {
    m_total_offset = offset;
}

auto Layer::getOpacity() const -> float {
    return m_opacity;
}

void Layer::setOpacity(float opacity) {
    m_opacity = std::min(1.f, std::max(0.f, opacity));
}

Layer::Type Layer::getTypeFromString(const std::string& type_name) {
    if (type_name == "IntGrid")
        return Type::IntGrid;
    if (type_name == "Entities")
        return Type::Entities;
    if (type_name == "Tiles")
        return Type::Tiles;
    if (type_name == "AutoLayer")
        return Type::AutoLayer;
    return Type::Tiles;
}
