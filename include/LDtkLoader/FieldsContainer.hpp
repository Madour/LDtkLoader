// Created by Modar Nasser on 27/01/2021.

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "thirdparty/optional.hpp"
#include "DataTypes.hpp"
#include "Utils.hpp"

namespace ldtk {

    template<typename T>
    using optional = std::experimental::optional<T>;

    constexpr auto null = std::experimental::nullopt;

    struct IField {
        virtual ~IField() = default;
    };

    template <typename T>
    struct Field : IField, private optional<T> {

        using optional<T>::optional;
        using optional<T>::value;

        constexpr auto is_null() const -> bool {
            return !optional<T>::has_value();
        }

    };

    template <class T>
    constexpr bool operator==(const Field<T>& lhs, const T& rhs) {
        if (lhs.is_null()) return false;
        return (lhs.value() == rhs);
    }

    template <class T>
    constexpr bool operator==(const T& lhs, const Field<T>& rhs) {
        if (rhs.is_null()) return false;
        return (rhs.value() == lhs);
    }

    template <class T>
    constexpr bool operator!=(const Field<T>& lhs, const T& rhs) {
        return !(lhs == rhs);
    }

    template <class T>
    constexpr bool operator!=(const T& lhs, const Field<T>& rhs) {
        return !(lhs == rhs);
    }

    template <typename T>
    struct ArrayField : IField, std::vector<Field<T>> {
        ArrayField() = default;
        explicit ArrayField(const std::vector<Field<T>>& vals) : std::vector<Field<T>>(vals) {}
    };

    class FieldsContainer {

    public:
        template <typename T>
        void addField(const std::string& name, const T& field);

        template <typename T>
        void addField(const std::string& name, const Field<T>& field);

        template <typename T>
        auto getField(const std::string& name) const -> const Field<T>&;

        template <typename T>
        void addArrayField(const std::string& name, const std::vector<Field<T>>& field);

        template <typename T>
        void addArrayField(const std::string& name, const ArrayField<T>& field);

        template <typename T>
        auto getArrayField(const std::string& name) const -> const ArrayField<T>&;

    private:
        std::vector<std::shared_ptr<IField>> m_gc;
        std::unordered_map<std::string, IField*> m_fields;
        std::unordered_map<std::string, IField*> m_array_fields;

    };


    template <typename T>
    void FieldsContainer::addField(const std::string& name, const T& field) {
        auto* new_field = new Field<T>(field);
        m_fields[name] = new_field;
        m_gc.emplace_back(new_field);
    }

    template <typename T>
    void FieldsContainer::addField(const std::string& name, const Field<T>& field) {
        auto* new_field = new Field<T>(field);
        m_fields[name] = new_field;
        m_gc.emplace_back(new_field);
    }

    template <typename T>
    auto FieldsContainer::getField(const std::string& name) const -> const Field<T>& {
        if (m_fields.count(name) > 0) {
            const auto* field = m_fields.at(name);
            const auto* ret = dynamic_cast<const Field<T>*>(field);
            if (ret)
                return *ret;
            ldtk_error("Field \"" + name + "\" is not of type " + typeid(T).name() + ".");
        }
        ldtk_error("Field \"" + name + "\" does not exist.");
    }

    template <typename T>
    void FieldsContainer::addArrayField(const std::string& name, const std::vector<Field<T>>& field) {
        auto* new_field = new ArrayField<T>(field);
        m_array_fields[name] = new_field;
        m_gc.emplace_back(new_field);
    }

    template <typename T>
    void FieldsContainer::addArrayField(const std::string& name, const ArrayField<T>& field) {
        auto* new_field = new ArrayField<T>(field);
        m_array_fields[name] = new_field;
        m_gc.emplace_back(new_field);
    }

    template <typename T>
    auto FieldsContainer::getArrayField(const std::string& name) const -> const ArrayField<T>& {
        if (m_array_fields.count(name) > 0) {
            const auto* field = m_array_fields.at(name);
            const auto* ret = dynamic_cast<const ArrayField<T>*>(field);
            if (ret)
                return *ret;
            ldtk_error("ArrayField \"" + name + "\" is not of type " + typeid(T).name() + ".");
        }
        ldtk_error("ArrayField \"" + name + "\" does not exist.");
    }

}

template <typename T>
auto operator<<(std::ostream& os, ldtk::Field<T> field) -> std::ostream& {
    if (field.is_null())
        os << "null";
    else
        os << field.value();
    return os;
}
