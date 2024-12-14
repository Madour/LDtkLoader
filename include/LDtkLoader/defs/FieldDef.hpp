// Created by Modar Nasser on 20/03/2022.

#pragma once

#include <map>
#include <string>
#include <vector>

#include "LDtkLoader/thirdparty/json_fwd.hpp"

#include "LDtkLoader/DataTypes.hpp"
#include "LDtkLoader/Enum.hpp"

namespace ldtk {

    class FieldDef
    {
        friend class EntityDef;

    public:
        FieldType type;
        std::string name;
    };

    template <typename T>
    struct Field;

    template <typename T>
    struct ArrayField;

    namespace detail {

        // clang-format off
        template<FieldType> struct field_type_from_enum_;
        template<> struct field_type_from_enum_<FieldType::Int>       { using type = Field<int>; };
        template<> struct field_type_from_enum_<FieldType::Float>     { using type = Field<float>; };
        template<> struct field_type_from_enum_<FieldType::Bool>      { using type = Field<bool>; };
        template<> struct field_type_from_enum_<FieldType::String>    { using type = Field<std::string>; };
        template<> struct field_type_from_enum_<FieldType::Color>     { using type = Field<Color>; };
        template<> struct field_type_from_enum_<FieldType::Point>     { using type = Field<IntPoint>; };
        template<> struct field_type_from_enum_<FieldType::Enum>      { using type = Field<EnumValue>; };
        template<> struct field_type_from_enum_<FieldType::FilePath>  { using type = Field<FilePath>;  };
        template<> struct field_type_from_enum_<FieldType::Tile>      { using type = Field<TileRect>; };
        template<> struct field_type_from_enum_<FieldType::EntityRef> { using type = Field<EntityRef>; };

        template<> struct field_type_from_enum_<FieldType::ArrayInt>      { using type = ArrayField<int>; };
        template<> struct field_type_from_enum_<FieldType::ArrayFloat>    { using type = ArrayField<float>; };
        template<> struct field_type_from_enum_<FieldType::ArrayBool>     { using type = ArrayField<bool>; };
        template<> struct field_type_from_enum_<FieldType::ArrayString>   { using type = ArrayField<std::string>; };
        template<> struct field_type_from_enum_<FieldType::ArrayColor>    { using type = ArrayField<Color>; };
        template<> struct field_type_from_enum_<FieldType::ArrayPoint>    { using type = ArrayField<IntPoint>; };
        template<> struct field_type_from_enum_<FieldType::ArrayEnum>     { using type = ArrayField<EnumValue>; };
        template<> struct field_type_from_enum_<FieldType::ArrayFilePath> { using type = ArrayField<FilePath>; };
        template<> struct field_type_from_enum_<FieldType::ArrayTile>     { using type = ArrayField<TileRect>; };
        template<> struct field_type_from_enum_<FieldType::ArrayEntityRef> { using type = ArrayField<EntityRef>; };
        // clang-format on

    } // namespace detail

    template <FieldType T>
    using getFieldType = typename detail::field_type_from_enum_<T>::type;

} // namespace ldtk
