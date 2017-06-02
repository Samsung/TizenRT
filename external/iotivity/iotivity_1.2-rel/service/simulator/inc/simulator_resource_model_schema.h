/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef SIMULATOR_RESOURCE_MODEL_SCHEMA_H_
#define SIMULATOR_RESOURCE_MODEL_SCHEMA_H_

#include <unordered_map>
#include <memory>

#include "simulator_resource_model.h"

class IntegerProperty;
class DoubleProperty;
class BooleanProperty;
class StringProperty;
class ArrayProperty;
class ModelProperty;
class AttributeProperty
{
    public:
        enum class Type
        {
            INTEGER,
            DOUBLE,
            BOOLEAN,
            STRING,
            MODEL,
            ARRAY
        };

        explicit AttributeProperty(Type type);
        virtual ~AttributeProperty() {};

        Type getType() const;
        virtual bool isInteger() const { return false; }
        virtual bool isDouble() const { return false; }
        virtual bool isBoolean() const { return false; }
        virtual bool isString() const { return false; }
        virtual bool isArray() const { return false; }
        virtual bool isModel() const { return false; }
        virtual std::shared_ptr<IntegerProperty> asInteger() { return nullptr; }
        virtual std::shared_ptr<DoubleProperty> asDouble() { return nullptr; }
        virtual std::shared_ptr<BooleanProperty> asBoolean() { return nullptr; }
        virtual std::shared_ptr<StringProperty> asString() { return nullptr; }
        virtual std::shared_ptr<ArrayProperty> asArray() { return nullptr; }
        virtual std::shared_ptr<ModelProperty> asModel() { return nullptr; }

        virtual bool validate(const AttributeValueVariant &value) = 0;
        virtual AttributeValueVariant buildValue() = 0;

    private:
        Type m_type;
};

class IntegerProperty : public AttributeProperty,
    public std::enable_shared_from_this<IntegerProperty>
{
    public:
        static std::shared_ptr<IntegerProperty> build(int defaultValue = 0);

        bool isInteger() const;
        std::shared_ptr<IntegerProperty> asInteger();
        void setDefaultValue(int value);
        void setRange(int min, int max);
        void setValues(const std::vector<int> &values);
        bool hasRange() const;
        bool hasValues() const;
        int getDefaultValue() const;
        bool getRange(int &min, int &max) const;
        bool getValues(std::vector<int> &values) const;
        bool validate(const AttributeValueVariant &value);
        bool validate(const int &value);
        AttributeValueVariant buildValue();

    private:
        explicit IntegerProperty(int defaultValue);

        int m_defaultValue;
        int m_min;
        int m_max;
        std::vector<int> m_values;
        bool m_hasRange;
};

class DoubleProperty : public AttributeProperty,
    public std::enable_shared_from_this<DoubleProperty>
{
    public:
        static std::shared_ptr<DoubleProperty> build(double defaultValue = 0.0);

        bool isDouble() const;
        std::shared_ptr<DoubleProperty> asDouble();
        void setDefaultValue(double value);
        void setRange(double min, double max);
        void setValues(const std::vector<double> &values);
        bool hasRange() const;
        bool hasValues() const;
        double getDefaultValue() const;
        bool getRange(double &min, double &max) const;
        bool getValues(std::vector<double> &values) const;
        bool validate(const AttributeValueVariant &value);
        bool validate(const double &value);
        AttributeValueVariant buildValue();

    private:
        explicit DoubleProperty(double defaultValue);

        double m_defaultValue;
        double m_min;
        double m_max;
        std::vector<double> m_values;
        bool m_hasRange;
};

class BooleanProperty : public AttributeProperty,
    public std::enable_shared_from_this<BooleanProperty>
{
    public:
        static std::shared_ptr<BooleanProperty> build(bool defaultValue = true);

        bool isBoolean() const;
        std::shared_ptr<BooleanProperty> asBoolean();
        void setDefaultValue(bool value);
        bool getDefaultValue() const;
        bool validate(const AttributeValueVariant &value);
        AttributeValueVariant buildValue();

    private:
        explicit BooleanProperty(bool defaultValue);

        bool m_defaultValue;
};

class StringProperty : public AttributeProperty,
    public std::enable_shared_from_this<StringProperty>
{
    public:
        static std::shared_ptr<StringProperty> build(const std::string &defaultValue = "");

        bool isString() const;
        std::shared_ptr<StringProperty> asString();
        void setDefaultValue(const std::string &value);
        void setRange(size_t min, size_t max);
        void setValues(const std::vector<std::string> &values);
        bool hasRange() const;
        bool hasValues() const;
        std::string getDefaultValue() const;
        bool getRange(size_t &min, size_t &max) const;
        bool getValues(std::vector<std::string> &values) const;
        bool validate(const AttributeValueVariant &value);
        bool validate(const std::string &value);
        AttributeValueVariant buildValue();

    private:
        StringProperty(const std::string &defaultValue);

        std::string m_defaultValue;
        size_t m_min;
        size_t m_max;
        std::vector<std::string> m_values;
        bool m_hasRange;
};

class ArrayProperty : public AttributeProperty,
    public std::enable_shared_from_this<ArrayProperty>
{
    public:
        static std::shared_ptr<ArrayProperty> build();

        bool isArray() const;
        std::shared_ptr<ArrayProperty> asArray();
        void setRange(size_t minItems, size_t maxItems);
        void setVariable(bool state);
        void setUnique(bool state);
        bool setElementProperty(const std::shared_ptr<AttributeProperty> &property);
        bool hasRange() const;
        bool isVariable() const;
        bool isUnique() const;
        size_t getMinItems() const;
        size_t getMaxItems() const;
        std::shared_ptr<AttributeProperty> getElementProperty();
        bool validate(const AttributeValueVariant &value);
        AttributeValueVariant buildValue();

    private:
        ArrayProperty();
        int findDepth(std::shared_ptr<AttributeProperty> &elementProperty);

        size_t m_min;
        size_t m_max;
        bool m_isVariableSize;
        bool m_isUnique;
        std::shared_ptr<AttributeProperty> m_elementProperty;
        bool m_hasRange;
};

class ModelProperty : public AttributeProperty,
    public std::enable_shared_from_this<ModelProperty>
{
    public:
        static std::shared_ptr<ModelProperty> build();

        bool isModel() const;
        std::shared_ptr<ModelProperty> asModel();
        bool add(const std::string &name, const std::shared_ptr<AttributeProperty> &property,
                 bool required = false);
        std::shared_ptr<AttributeProperty> get(const std::string &name);
        std::unordered_map<std::string, std::shared_ptr<AttributeProperty>> getChildProperties();
        bool isRequired(const std::string &name);
        void remove(const std::string &name);
        void setRequired(const std::string &name);
        void unsetRequired(const std::string &name);
        SimulatorResourceModel buildResourceModel();
        bool validate(const AttributeValueVariant &value);
        bool validate(const SimulatorResourceModel &model);
        AttributeValueVariant buildValue();

    private:
        ModelProperty();

        std::unordered_map<std::string, bool> m_requiredAttributes;
        std::unordered_map<std::string, std::shared_ptr<AttributeProperty>> m_childProperties;
};

typedef ModelProperty SimulatorResourceModelSchema;

#endif
