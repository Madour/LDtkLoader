// Created by Modar Nasser on 20/03/2022.

#pragma once

#include <map>
#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"
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

    template <typename T>
    struct Field;
    template <typename T>
    struct ArrayField;

    namespace {
        template<FieldType> struct field_type_from_enum_;
        template<> struct field_type_from_enum_<FieldType::Int>       { using type = Field<int>; };
        template<> struct field_type_from_enum_<FieldType::Float>     { using type = Field<float>; };
        template<> struct field_type_from_enum_<FieldType::Bool>      { using type = Field<bool>; };
        template<> struct field_type_from_enum_<FieldType::String>    { using type = Field<std::string>; };
        template<> struct field_type_from_enum_<FieldType::Color>     { using type = Field<Color>; };
        template<> struct field_type_from_enum_<FieldType::Point>     { using type = Field<IntPoint>; };
        template<> struct field_type_from_enum_<FieldType::Enum>      { using type = Field<EnumValue>; };
        template<> struct field_type_from_enum_<FieldType::FilePath>  { using type = Field<FilePath>;  };
        template<> struct field_type_from_enum_<FieldType::EntityRef> { using type = Field<EntityRef>; };

        template<> struct field_type_from_enum_<FieldType::ArrayInt>      { using type = ArrayField<int>; };
        template<> struct field_type_from_enum_<FieldType::ArrayFloat>    { using type = ArrayField<float>; };
        template<> struct field_type_from_enum_<FieldType::ArrayBool>     { using type = ArrayField<bool>; };
        template<> struct field_type_from_enum_<FieldType::ArrayString>   { using type = ArrayField<std::string>; };
        template<> struct field_type_from_enum_<FieldType::ArrayColor>    { using type = ArrayField<Color>; };
        template<> struct field_type_from_enum_<FieldType::ArrayPoint>    { using type = ArrayField<IntPoint>; };
        template<> struct field_type_from_enum_<FieldType::ArrayEnum>     { using type = ArrayField<EnumValue>; };
        template<> struct field_type_from_enum_<FieldType::ArrayFilePath> { using type = ArrayField<FilePath>; };
        template<> struct field_type_from_enum_<FieldType::ArrayEntityRef> { using type = ArrayField<EntityRef>; };
    }

    template<FieldType T>
    using getFieldType = typename field_type_from_enum_<T>::type;
}
