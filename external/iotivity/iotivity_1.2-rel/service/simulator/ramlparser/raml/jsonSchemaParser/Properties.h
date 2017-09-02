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
 * @file   Properties.h
 *
 * @brief   This file provides data Model for Json Schema Properties.
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>
#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <limits>
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include <memory>

namespace RAML
{
    class Properties;

    /** ValueVariant - Boost Variant to hold type of property*/
    typedef boost::variant <
    int,
    double,
    bool,
    std::string,
    Properties,
    std::vector<Properties>

    > ValueVariant;

    /** VariantType - enumeration for variant types*/
    enum class VariantType
    {
        UNKNOWN,
        INTEGER,
        DOUBLE,
        BOOLEAN,
        STRING,
        PROPERTY,
        ARRAY,
        OBJECT
    };

    /**
     * @class   ValueProperty
     * @brief   This class provides data Model for Json Schema Value Property.
     */
    class ValueProperty
    {
        public:
            /** Type - enumeration for ValueProperty types*/
            enum class Type
            {
                UNKNOWN,
                RANGE,
                VALUE_SET,
                PATTERN,
                FORMAT,
                ARRAY
            };

            /**
             * Constructor of ValueProperty.
             */
            ValueProperty();

            ValueProperty(const ValueProperty &) = default;

            ValueProperty &operator=(const ValueProperty &) = default;

            ValueProperty(ValueProperty &&) = default;

            ValueProperty &operator=(ValueProperty &&) = default;

            /**
             * explicit Constructor of ValueProperty for Type RANGE.
             *
             * @param min - minimum value of property.
             * @param max- maximum value of property.
             * @param multipleOf- multipleOf value of property.
             */
            explicit ValueProperty(double min, double max, int multipleOf);

            /**
             * explicit Constructor of ValueProperty for Type VALUE_SET.
             *
             * @param valueSet - allowed values in the Properties.
             */
            explicit ValueProperty(const std::vector<int> &valueSet);

            /**
             * explicit Constructor of ValueProperty for Type VALUE_SET.
             *
             * @param valueSet - allowed values in the Properties.
             */
            explicit ValueProperty(const std::vector<double> &valueSet);

            /**
             * explicit Constructor of ValueProperty for Type VALUE_SET.
             *
             * @param valueSet - allowed values in the Properties.
             */
            explicit ValueProperty(const std::vector<bool> &valueSet);

            /**
             * explicit Constructor of ValueProperty for Type VALUE_SET.
             *
             * @param valueSet - allowed values in the Properties.
             */
            explicit ValueProperty(const std::vector<std::string> &valueSet);

            /**
             * explicit Constructor of ValueProperty for Type VALUE_SET.
             *
             * @param valueSet - allowed values in the Properties.
             */
            explicit ValueProperty(const std::vector<ValueVariant> &valueSet);

            /**
             * explicit Constructor of ValueProperty for Type PATTERN or FORMAT.
             *
             * @param type - ValueProperty Type.
             * @param value - value for the pattern or format.
             */
            explicit ValueProperty(Type type, std::string value);

            /**
             * explicit Constructor of ValueProperty for Type ARRAY.
             *
             * @param type - ValueProperty Type.
             * @param minItems - minimum elements in the Array property.
             * @param maxItems - maximum elements in the Array property.
             * @param unique - unique elements in the Array property.
             * @param additionalItems - additional elements in the Array property.
             */
            explicit ValueProperty(Type type, int minItems, int maxItems, bool unique, bool additionalItems);

            /**
             * This method is for getting type of ValueProperty.
             *
             * @return Type of ValueProperty
             */
            Type type() const;

            /**
             * This method is for getting minimum value of ValueProperty.
             *
             * @return min value of ValueProperty
             */
            double min() const;

            /**
             * This method is for getting maximum value of ValueProperty.
             *
             * @return max value of ValueProperty
             */
            double max() const;

            /**
             * This method is for getting multipleOf value of ValueProperty.
             *
             * @return multipleOf value of ValueProperty
             */
            int multipleOf() const;

            /**
             * This method is for getting pattern value of ValueProperty.
             *
             * @return pattern value of ValueProperty
             */
            std::string pattern() const;

            /**
             * This method is for getting format value of ValueProperty.
             *
             * @return format value of ValueProperty
             */
            std::string format() const;

            /**
             * This method is for getting valueSet of ValueProperty.
             *
             * @return valueSet of ValueProperty
             */
            int valueSetSize() const;

            /**
             * This method is for getting valueSet of ValueProperty.
             *
             * @return valueSet of ValueProperty
             */
            std::vector<ValueVariant> valueSet() const;

            /**
             * This method is for getting valueArray of ValueProperty.
             *
             * @param minItems - reference to get minimum elements in the Array property.
             * @param maxItems - reference to get maximum elements in the Array property.
             * @param unique - reference to get unique elements in the Array property.
             * @param additionalItems - reference to get additional elements in the Array property.
             */
            void valueArray(int &minItems, int &maxItems, bool &unique, bool &additionalItems) const;

        private:
            Type m_type;
            double m_min;
            double m_max;
            int m_multipleOf;
            std::vector<ValueVariant> m_valueSet;
            std::string m_pattern;
            std::string m_format;
            int m_minItems;
            int m_maxItems;
            bool m_unique;
            bool m_additionalItems;
    };

    /**
     * @class   Properties
     * @brief   This class provides data Model for Json Schema Properties.
     */
    class Properties
    {
        public:
            /**
             * @class   TypeInfo
             * @brief   This class provides type information of Json Properties.
             */
            class TypeInfo
            {
                public:
                    /**
                     * Constructor of TypeInfo.
                     *
                     * @param VariantType - type of property.
                     * @param VariantType - type of parent property.
                     * @param int - depth of property.
                     */
                    TypeInfo(VariantType type = VariantType::UNKNOWN,
                             VariantType baseType = VariantType::UNKNOWN,
                             int depth = 0);

                    TypeInfo(const TypeInfo &) = default;

                    TypeInfo &operator=(const TypeInfo &) = default;

                    TypeInfo(TypeInfo &&) = default;

                    TypeInfo &operator=(TypeInfo &&) = default;

                    /**
                     * This method is for getting type of properties.
                     *
                     * @return VariantType of Property
                     */
                    VariantType type() const;

                    /**
                     * This method is for getting base or parent type of properties.
                     *
                     * @return VariantType of parent Property
                     */
                    VariantType baseType() const;

                    /**
                     * This method is for getting depth of properties.
                     *
                     * @return depth as int
                     */
                    int depth() const;

                    /**
                     *  operator for TypeInfo to check equality.
                     *
                     * @param TypeInfo.
                     */
                    bool operator ==(const TypeInfo &) const;

                    /**
                     *  operator for TypeInfo to check inequality.
                     *
                     * @param TypeInfo.
                     */
                    bool operator !=(const TypeInfo &) const;

                private:
                    VariantType m_type;
                    VariantType m_baseType;
                    int m_depth;
            };

            /**
              * Constructor of Properties.
              *
              * @param name - Properties name as string.
              */
            Properties(const std::string &name) : m_name(name) {}

            Properties() = default;

            Properties(const Properties &) = default;

            Properties &operator=(const Properties &) = default;

            Properties(Properties &&) = default;

            Properties &operator=(Properties &&) = default;

            /**
             * This method is for getting TypeInfo of Properties.
             *
             * @return Properties TypeInfo
             */
            TypeInfo getType() const;

            /**
             * This method is for setting type of Properties.
             *
             * @param type -Propertie's Type
             */
            void setTypeString(const std::string &type);

            /**
             * This method is for getting Name from Properties.
             *
             * @return Properties name as string
             */
            std::string getName() const;

            /**
             * This method is for setting name to Properties
             *
             * @param name - Properties name as string.
             */
            void setName(const std::string &name);

            /**
             * This method is for getting Description from Properties.
             *
             * @return Description as string
             */
            std::string getDescription() const;

            /**
             * This method is for setting Description to Properties
             *
             * @param description - Description as string.
             */
            void setDescription(const std::string &description);

            /**
             * This method is for setting Value to Properties.
             *
             * @param value -  Value of Properties
             */
            template <typename T>
            void setValue(const T &value)
            {
                m_value = std::make_shared<ValueVariant>(value);
            }

            /**
             * This method is for getting Value from Properties.
             *
             * @return Properties Value
             */
            ValueVariant getValue() const;

            /**
             * This method is for getting Value from Properties.
             *
             * @return Properties Value
             */
            template <typename T>
            T getValue() const
            {
                return boost::get<T>(*(m_value.get()));
            }

            /**
             * This method is for checking if default Value exists in the Properties.
             *
             * @return true if present and false if not present
             */
            bool isDefaultValue() const { return ((m_value != nullptr) ? true : false); }

            /**
             * This method is for getting ValueProperty from Properties.
             *
             * @return vector of pointer to ValueProperty
             */
            std::vector<std::shared_ptr<ValueProperty> > const &getValueProperties() const;

            /**
             * This method is for setting ValueProperty to Properties
             *
             * @param value - pointer to ValueProperty
             */
            void setValueProperty(const std::shared_ptr<ValueProperty> &value);


            /**
             * This method is for getting RequiredValue from Properties.
             *
             * @return list of RequiredValue as string
             */
            std::vector<std::string> const &getRequiredValues() const;

            /**
             * This method is for setting RequiredValue to Properties
             *
             * @param reqValue - RequiredValue as string.
             */
            void setRequiredValue(const std::string &reqValue);

        private:
            TypeInfo m_type;
            std::string m_typeString;
            std::string m_name;
            std::string m_description;
            std::shared_ptr<ValueVariant> m_value;
            std::vector<std::shared_ptr<ValueProperty> > m_valueProperty;
            std::vector<std::string> m_required;
    };

    /** PropertiesPtr - shared Ptr to Properties.*/
    typedef std::shared_ptr<Properties> PropertiesPtr;

    /** ValuePropertyPtr - shared Ptr to ValueProperty.*/
    typedef std::shared_ptr<ValueProperty> ValuePropertyPtr;

}
#endif
