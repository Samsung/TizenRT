/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @file   Properties.cpp
 *
 * @brief   This file provides data Model for Json Schema Properties.
 */

#include "Properties.h"
#include <boost/lexical_cast.hpp>
#include <boost/variant/static_visitor.hpp>
#include "RamlExceptions.h"

namespace RAML
{
    template <typename T>
    struct TypeConverter {};

    template <>
    struct TypeConverter<int>
    {
        constexpr static VariantType type =
            VariantType::INTEGER;
    };

    template <>
    struct TypeConverter<double>
    {
        constexpr static VariantType type =
            VariantType::DOUBLE;
    };

    template <>
    struct TypeConverter<bool>
    {
        constexpr static VariantType type =
            VariantType::BOOLEAN;
    };

    template <>
    struct TypeConverter<std::string>
    {
        constexpr static VariantType type =
            VariantType::STRING;
    };

    template <>
    struct TypeConverter<Properties>
    {
        constexpr static VariantType type =
            VariantType::PROPERTY;
    };

    template <typename T>
    struct TypeDetails
    {
        constexpr static VariantType type =
            TypeConverter<T>::type;
        constexpr static VariantType baseType =
            TypeConverter<T>::type;
        constexpr static int depth = 0;
    };

    template <typename T>
    struct TypeDetails<std::vector<T>>
    {
        constexpr static VariantType type =
            VariantType::ARRAY;
        constexpr static VariantType baseType =
            TypeDetails<T>::baseType;
        constexpr static int depth = 1 + TypeDetails<T>::depth;
    };

    class PropertyTypeVisitor : public boost::static_visitor<>
    {
        public:
            PropertyTypeVisitor() : m_type(VariantType::UNKNOWN),
                m_baseType(VariantType::UNKNOWN), m_depth(0) {}

            template <typename T>
            void operator ()(const T &)
            {
                m_type = TypeDetails<T>::type;
                m_baseType = TypeDetails<T>::baseType;
                m_depth = TypeDetails<T>::depth;
            }

            VariantType m_type;
            VariantType m_baseType;
            int m_depth;
    };

    Properties::TypeInfo Properties::getType() const
    {
        if (m_value)
        {
            RAML::PropertyTypeVisitor typeVisitor;
            boost::apply_visitor(typeVisitor, *(m_value.get()));
            Properties::TypeInfo typeInfo(typeVisitor.m_type, typeVisitor.m_baseType,
                                          typeVisitor.m_depth);
            return typeInfo;
        }
        else if (!m_typeString.empty()) //to read properties even if default value is not present
        {
            if (m_typeString == "string")
                return Properties::TypeInfo(VariantType::STRING, VariantType::STRING, 0);
            else if (m_typeString == "integer")
                return Properties::TypeInfo(VariantType::INTEGER, VariantType::INTEGER, 0);
            else if (m_typeString == "number")
                return Properties::TypeInfo(VariantType::DOUBLE, VariantType::DOUBLE, 0);
            else if (m_typeString == "boolean")
                return Properties::TypeInfo(VariantType::BOOLEAN, VariantType::BOOLEAN, 0);
        }
        return Properties::TypeInfo();
    }

    void Properties::setTypeString(const std::string &type)
    {
        m_typeString = type;
    }

    Properties::TypeInfo::TypeInfo(VariantType type,
                                   VariantType baseType, int depth)
        :   m_type (type), m_baseType(baseType), m_depth(depth) {}

    VariantType Properties::TypeInfo::type() const
    {
        return m_type;
    }

    VariantType Properties::TypeInfo::baseType() const
    {
        return m_baseType;
    }

    int Properties::TypeInfo::depth() const
    {
        return m_depth;
    }

    bool Properties::TypeInfo::operator==(
        const Properties::TypeInfo &rhs ) const
    {
        if (m_type == rhs.type() && m_baseType == rhs.baseType()
            && m_depth == rhs.depth())
            return true;
        return false;
    }

    bool Properties::TypeInfo::operator!=(
        const Properties::TypeInfo &rhs ) const
    {
        if (m_type != rhs.type() || m_baseType != rhs.baseType()
            || m_depth != rhs.depth())
            return true;
        return false;
    }

    ValueVariant Properties::getValue() const
    {
        if (!isDefaultValue())
            throw JsonException("Reading Empty Property Value");
        return *m_value;
    }

    std::string Properties::getName() const
    {
        return m_name;
    }

    void Properties::setName(const std::string &name)
    {
        m_name = name;
    }
    std::string Properties::getDescription() const
    {
        return m_description;
    }

    void Properties::setDescription(const std::string &description)
    {
        m_description = description;
    }
    std::vector<std::shared_ptr<ValueProperty> > const &Properties::getValueProperties()
    const
    {
        return m_valueProperty;
    }
    void Properties::setValueProperty(const std::shared_ptr<ValueProperty> &value)
    {
        m_valueProperty.push_back(value);
    }
    std::vector<std::string> const &Properties::getRequiredValues() const
    {
        return m_required;
    }
    void Properties::setRequiredValue(const std::string &reqValue)
    {
        auto it = m_required.begin();
        for (; it != m_required.end(); ++it)
        {
            if (*it == reqValue)
                break;
        }
        if (m_required.end() == it)
        {
            m_required.push_back(reqValue);
        }
    }
    ValueProperty::ValueProperty()
        :   m_type(ValueProperty::Type::UNKNOWN),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false) {}

    ValueProperty::ValueProperty(double min, double max, int multipleOf)
        :   m_type(ValueProperty::Type::RANGE),
            m_min(min),
            m_max(max),
            m_multipleOf(multipleOf),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false) {}

    ValueProperty::ValueProperty(
        const std::vector<int> &valueSet)
        :   m_type(ValueProperty::Type::VALUE_SET),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_valueSet(valueSet.begin(), valueSet.end()),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)  {}

    ValueProperty::ValueProperty(
        const std::vector<double> &valueSet)
        :   m_type(ValueProperty::Type::VALUE_SET),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_valueSet(valueSet.begin(), valueSet.end()),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)  {}

    ValueProperty::ValueProperty(
        const std::vector<bool> &valueSet)
        :   m_type(ValueProperty::Type::VALUE_SET),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_valueSet(valueSet.begin(), valueSet.end()),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)  {}

    ValueProperty::ValueProperty(
        const std::vector<std::string> &valueSet)
        :   m_type(ValueProperty::Type::VALUE_SET),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_valueSet(valueSet.begin(), valueSet.end()),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)  {}

    ValueProperty::ValueProperty(
        const std::vector<ValueVariant> &valueSet)
        :   m_type(ValueProperty::Type::VALUE_SET),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_valueSet(valueSet.begin(), valueSet.end()),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)  {}

    ValueProperty::ValueProperty(Type type, std::string value)
        :   m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_minItems(INT_MIN),
            m_maxItems(INT_MAX),
            m_unique(false),
            m_additionalItems(false)
    {
        if (type == ValueProperty::Type::PATTERN)
        {
            m_type = ValueProperty::Type::PATTERN;
            m_pattern = value;
        }
        else if (type == ValueProperty::Type::FORMAT)
        {
            m_type = ValueProperty::Type::FORMAT;
            m_format = value;
        }
        else
        {
            m_type = ValueProperty::Type::UNKNOWN;
        }
    }

    ValueProperty::ValueProperty(Type type, int minItems, int maxItems, bool unique,
                                 bool additionalItems)
        :   m_type(ValueProperty::Type::UNKNOWN),
            m_min(INT_MIN),
            m_max(INT_MAX),
            m_multipleOf(INT_MAX),
            m_minItems(1),
            m_maxItems(20),
            m_unique(false),
            m_additionalItems(false)
    {
        if (type == ValueProperty::Type::ARRAY)
        {
            m_type = ValueProperty::Type::ARRAY;

            if (minItems > 0)
                m_minItems = minItems;

            if (maxItems != INT_MAX && maxItems > m_minItems)
                m_maxItems = maxItems;

            m_unique = unique;
            m_additionalItems = additionalItems;
        }
    }

    ValueProperty::Type ValueProperty::type() const
    {
        return m_type;
    }

    double ValueProperty::min() const
    {
        return m_min;
    }

    double ValueProperty::max() const
    {
        return m_max;
    }

    int ValueProperty::multipleOf() const
    {
        return m_multipleOf;
    }

    std::string ValueProperty::pattern() const
    {
        return m_pattern;
    }

    std::string ValueProperty::format() const
    {
        return m_format;
    }

    int ValueProperty::valueSetSize() const
    {
        return m_valueSet.size();
    }

    std::vector<ValueVariant> ValueProperty::valueSet() const
    {
        return m_valueSet;
    }

    void ValueProperty::valueArray(int &minItems, int &maxItems, bool &unique,
                                   bool &additionalItems) const
    {
        minItems = m_minItems;
        maxItems = m_maxItems;
        unique = m_unique;
        additionalItems = m_additionalItems;
    }


}
