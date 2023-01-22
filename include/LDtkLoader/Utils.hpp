// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <string>
#include <iostream>

#include "LDtkLoader/DataTypes.hpp"

#ifdef LDTK_NO_THROW
    #define ldtk_error(msg) do{ldtk::print_error(__FUNCTION__, msg);exit(EXIT_FAILURE);}while(0)
#else
    #define ldtk_error(msg) throw std::invalid_argument("LDtkLoader exception (in "+std::string(__FUNCTION__)+") : "+std::string(msg))
#endif

namespace ldtk {
    auto getLayerTypeFromString(const std::string& type_name) -> LayerType;

    inline void print_error(const std::string& fn, const std::string& msg) {
        std::cerr << "LDtkLoader Error (in " << fn << ") : " << msg << std::endl;
    }
}
