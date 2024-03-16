// Created by Modar Nasser on 12/11/2020.

#include "LDtkLoader/Utils.hpp"


auto ldtk::getLayerTypeFromString(const std::string& type_name) -> ldtk::LayerType {
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

void ldtk::print_error(const std::string& fn, const std::string& msg) {
    std::cerr << "LDtkLoader Error (in " << fn << ") : " << msg << std::endl;
}

void ldtk::print_json_error(const std::string& msg) {
    std::cerr << "LDtkLoader Json Error : " << msg << std::endl;
}
