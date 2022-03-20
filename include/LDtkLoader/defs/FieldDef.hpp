// Created by Modar Nasser on 20/03/2022.

#pragma once

#include <map>
#include <string>

#include "LDtkLoader/thirdparty/json.hpp"
#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Tileset.hpp"

namespace ldtk {

    enum class FieldType {
        Int,
        Float,
        Bool,
        String,
        Color,
        Point,
        Enum,
        FilePath,
        EntityRef,
        ArrayInt,
        ArrayFloat,
        ArrayBool,
        ArrayString,
        ArrayColor,
        ArrayPoint,
        ArrayEnum,
        ArrayFilePath,
        ArrayEntityRef
    };

    class FieldDef {
        friend class EntityDef;
    public:
        FieldType type;
        std::string name;
    };

    template<FieldType> struct field_type_;
    template<> struct field_type_<FieldType::Int> { using type = int; };
    template<> struct field_type_<FieldType::ArrayInt> { using type = int; };
    template<> struct field_type_<FieldType::Float> { using type = float; };
    template<> struct field_type_<FieldType::ArrayFloat> { using type = float; };
    template<> struct field_type_<FieldType::Bool> { using type = bool; };
    template<> struct field_type_<FieldType::ArrayBool> { using type = bool; };
    template<> struct field_type_<FieldType::String> { using type = std::string; };
    template<> struct field_type_<FieldType::ArrayString> { using type = std::string; };
    template<> struct field_type_<FieldType::Color> { using type = Color; };
    template<> struct field_type_<FieldType::ArrayColor> { using type = Color; };
    template<> struct field_type_<FieldType::Point> { using type = IntPoint; };
    template<> struct field_type_<FieldType::ArrayPoint> { using type = IntPoint; };
    template<> struct field_type_<FieldType::Enum> { using type = EnumValue; };
    template<> struct field_type_<FieldType::ArrayEnum> { using type = EnumValue; };
    template<> struct field_type_<FieldType::FilePath> { using type = FilePath; };
    template<> struct field_type_<FieldType::ArrayFilePath> { using type = FilePath; };
    template<> struct field_type_<FieldType::EntityRef> { using type = EntityRef; };
    template<> struct field_type_<FieldType::ArrayEntityRef> { using type = EntityRef; };

    template<FieldType T>
    using getFieldType = typename field_type_<T>::type;

}
