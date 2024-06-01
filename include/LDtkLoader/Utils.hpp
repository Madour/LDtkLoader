// Created by Modar Nasser on 12/11/2020.

#pragma once

#include <iostream>
#include <string>

#include "LDtkLoader/DataTypes.hpp"

#ifdef LDTK_NO_THROW
    #include <exception>
    #define ldtk_error(msg)                       \
        do {                                      \
            ldtk::print_error(__FUNCTION__, msg); \
            exit(EXIT_FAILURE);                   \
        } while (0)
    #define ldtk_json_error(ex)                \
        do {                                   \
            ldtk::print_json_error(ex.what()); \
            exit(EXIT_FAILURE);                \
        } while (0)
    #define JSON_THROW_USER ldtk_json_error // override nlohmann::json throw macro
#else
    #define ldtk_error(msg)                                                                     \
        throw std::invalid_argument(                                                            \
            "LDtkLoader exception (in " + std::string(__FUNCTION__) + ") : " + std::string(msg) \
        )
#endif

namespace ldtk {

    template <typename T>
    using ref_wrapper = std::reference_wrapper<T>;

    auto getLayerTypeFromString(const std::string& type_name) -> LayerType;
    auto getWorldLayoutFromString(const std::string& layout_name) -> WorldLayout;
    auto getDirFromString(const std::string& dir_name) -> Dir;
    auto getFieldTypeFromString(const std::string& fieldtype_name) -> FieldType;

    void print_error(const std::string& fn, const std::string& msg);
    void print_json_error(const std::string& msg);

} // namespace ldtk
