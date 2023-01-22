// Created by Modar Nasser on 23/03/2022.

#pragma once

#include <vector>
#include <iostream>

#include "LDtkLoader/thirdparty/optional.hpp"

namespace ldtk {
    template<typename T>
    using optional = std::experimental::optional<T>;

    constexpr auto null = std::experimental::nullopt;

    struct IField {
        virtual ~IField() = default;
    };

    template <typename T>
    struct Field;

    template <typename T>
    struct ArrayField;

    template <typename T>
#if defined LDTK_FIELD_PUBLIC_OPTIONAL
    struct Field : IField, public optional<T> {
#else
    struct Field : IField, private optional<T> {
#endif
    private:
        static ArrayField<T> m_dummy;
    public:
        using value_type = T;

        using optional<T>::optional;
        using optional<T>::value;
        using optional<T>::value_or;

        constexpr auto is_null() const -> bool {
            return !optional<T>::has_value();
        }

        operator const ArrayField<T>&() const {
            throw std::runtime_error("Cannot convert ldtk::Field<T> to ldtk::ArrayField<T>");
            return m_dummy;
        }
    };

    template <typename T>
    ArrayField<T> Field<T>::m_dummy;

    template <class T>
    auto operator==(const Field<T>& lhs, const T& rhs) -> bool {
        if (lhs.is_null()) return false;
        return (lhs.value() == rhs);
    }

    template <class T>
    auto operator==(const T& lhs, const Field<T>& rhs) -> bool {
        if (rhs.is_null()) return false;
        return (rhs.value() == lhs);
    }

    template <class T>
    auto operator!=(const Field<T>& lhs, const T& rhs) -> bool {
        return !(lhs == rhs);
    }

    template <class T>
    auto operator!=(const T& lhs, const Field<T>& rhs) -> bool {
        return !(lhs == rhs);
    }

    template <typename T>
    struct ArrayField : IField, std::vector<Field<T>> {
        using value_type = T;
        ArrayField() = default;
        explicit ArrayField(const std::vector<Field<T>>& vals) : std::vector<Field<T>>(vals) {}

        operator const Field<T>&() const {
            throw std::runtime_error("Cannot convert ldtk::ArrayField<T> to ldtk::Field<T>");
            return this->at(0);
        }
    };
}

template <typename T>
auto operator<<(std::ostream& os, ldtk::Field<T> field) -> std::ostream& {
    if (field.is_null())
        os << "null";
    else
        os << field.value();
    return os;
}
