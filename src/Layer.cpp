// Created by Modar Nasser on 12/11/2020.

#include <iostream>

#include "LDtkLoader/Layer.hpp"
#include "LDtkLoader/Utils.hpp"

using namespace ldtk;

Layer::Layer(const nlohmann::json& j) :
type(getLayerTypeFromString(j["__type"].get<std::string>())),
name(j["__identifier"].get<std::string>()),
grid_size({j["__cWid"].get<unsigned int>(), j["__cHei"].get<unsigned int>()}),
cell_size(j["__gridSize"].get<unsigned int>()),
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

auto Layer::hasTileset() const -> bool {
    return m_tileset != nullptr;
}

auto Layer::getTileset() const -> const Tileset& {
    if (m_tileset == nullptr) {
        std::cerr << "ERROR in Layer::getTileset : Layer " << name << " doesn't have a tileset" << std::endl;
        exit(-1);
    }
    return *m_tileset;
}

void Layer::setLayerDef(const LayerDef& layer_def) {
    m_layer_def = &layer_def;
}

void Layer::setTileset(const Tileset& tileset) {
    m_tileset = &tileset;
}
