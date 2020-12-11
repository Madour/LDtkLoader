// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>
#include <iostream>

#include "LDtkLoader/DataTypes.hpp"

#define ldtk_error(msg) ldtk::print_error(__FUNCTION__, msg);exit(-1)

namespace ldtk {
    LayerType getLayerTypeFromString(const std::string& type_name);

    inline void print_error(const std::string& fn, const std::string& msg) {
        std::cerr << "LDtkLoader Error (in " << fn << ") : " << msg << std::endl;
    }
}
