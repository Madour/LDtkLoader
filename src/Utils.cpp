// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Utils.hpp"


ldtk::LayerType ldtk::getLayerTypeFromString(const std::string& type_name) {
    if (type_name == "IntGrid")
        return LayerType::IntGrid;
    if (type_name == "Entities")
        return LayerType::Entities;
    if (type_name == "Tiles")
        return LayerType::Tiles;
    if (type_name == "AutoLayer")
        return LayerType::AutoLayer;
    return LayerType::Tiles;
}