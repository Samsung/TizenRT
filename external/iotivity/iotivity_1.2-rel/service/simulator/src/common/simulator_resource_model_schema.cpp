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

#include "simulator_resource_model_schema.h"
#include "simulator_exceptions.h"

template <typename TYPE>
static AttributeValueVariant buildArrayValue(int depth, AttributeValueVariant &childValue)
{
    if (1 == depth)
    {
        std::vector<TYPE> arrayValue {boost::get<TYPE>(childValue)};
        return arrayValue;
    }
    else if (2 == depth)
    {
        std::vector<std::vector<TYPE>> arrayValue {{boost::get<TYPE>(childValue)}};
        return arrayValue;
    }

    // This for handling array values of depth 3
    // NOTE: Caller of this method should validate and pass the depth information.
    // on invalid depth information this method might throw boost exception.
    std::vector<std::vector<std::vector<TYPE>>> arrayValue {{{boost::get<TYPE>(childValue)}}};
    return arrayValue;
}

template <typename T, typename=void>
struct UniquenessChecker
{
    static bool get(const std::vector<T> &/*value*/)
    {
        return true;
    }
};

template <typename T>
struct UniquenessChecker<T, typename std::enable_if<
    std::is_same<T, int>::value
    || std::is_same<T, double>::value
    || std::is_same<T, bool>::value
    || std::is_same<T, std::string>::value
    >::type >
{
    static bool get(const std::vector<T> &value)
    {
        std::vector<T> valueCopy = value;
        std::sort(valueCopy.begin(), valueCopy.end());
        if(valueCopy.end() != std::unique(valueCopy.begin(), valueCopy.end()))
            return false;
        return true;
    }
};

class IntegerValidator : public boost::static_visitor<bool>
{
    public:
        IntegerValidator(const std::shared_ptr<IntegerProperty> &property) :
            m_property(property) {}

        template <typename T>
        bool operator ()(const T &)
        {
            return false;
        }

        bool operator ()(const int &value)
        {
            return m_property->validate(value);
        }

    private:
        std::shared_ptr<IntegerProperty> m_property;
};

class DoubleValidator : public boost::static_visitor<bool>
{
    public:
        DoubleValidator(const std::shared_ptr<DoubleProperty> &property) :
            m_property(property) {}

        template <typename T>
        bool operator ()(const T &)
        {
            return false;
        }

        bool operator ()(const double &value)
        {
            return m_property->validate(value);
        }

    private:
        std::shared_ptr<DoubleProperty> m_property;
};

class StringValidator : public boost::static_visitor<bool>
{
    public:
        StringValidator(const std::shared_ptr<StringProperty> &property) :
            m_property(property) {}

        template <typename T>
        bool operator ()(const T &)
        {
            return false;
        }

        bool operator ()(const std::string &value)
        {
            return m_property->validate(value);
        }

    private:
        std::shared_ptr<StringProperty> m_property;
};

class ArrayValidator : public boost::static_visitor<bool>
{
    public:
        ArrayValidator(const std::shared_ptr<ArrayProperty> &property) :
            m_property(property) {}

        template <typename T>
        bool operator ()(const T &)
        {
            return false;
        }

        template <typename T>
        bool operator ()(const std::vector<T> &value)
        {
            // Validating length of array
            if (!checkLength(value))
            {
                return false;
            }

            // Validate the uniqueness of elements of array
            if (!checkUniqueness(value))
            {
                return false;
            }

            // Validating elements of array
            std::shared_ptr<AttributeProperty> elementProperty =
                m_property->getElementProperty();
            if (elementProperty)
            {
                for (size_t index = 0; index < value.size(); index++)
                {
                    AttributeValueVariant element = value[index];
                    if (!elementProperty->validate(element))
                        return false;
                }
            }

            return true;
        }

    private:
        template <typename T>
        bool checkLength(const std::vector<T> &value)
        {
            if (m_property->hasRange())
            {
                if (value.size() < m_property->getMinItems()
                    || (value.size() > m_property->getMaxItems() && !m_property->isVariable()))
                {
                    return false;
                }
            }

            return true;
        }

        template <typename T>
        bool checkUniqueness(const std::vector<T> &value)
        {
            if (m_property->isUnique())
            {
                return UniquenessChecker<T>::get(value);
            }

            return true;
        }

        std::shared_ptr<ArrayProperty> m_property;
};

class ModelValidator : public boost::static_visitor<bool>
{
    public:
        ModelValidator(const std::shared_ptr<ModelProperty> &property) :
            m_property(property) {}

        template <typename T>
        bool operator ()(const T &)
        {
            return false;
        }

        bool operator ()(const SimulatorResourceModel &value)
        {
            return m_property->validate(value);
        }

    private:
        std::shared_ptr<ModelProperty> m_property;
};

AttributeProperty::AttributeProperty(Type type) : m_type(type) {}

AttributeProperty::Type AttributeProperty::getType() const
{
    return m_type;
}

IntegerProperty::IntegerProperty(int defaultValue)
    : AttributeProperty(AttributeProperty::Type::INTEGER),
      m_defaultValue(defaultValue),
      m_min(0),
      m_max(0),
      m_hasRange(false) {}

std::shared_ptr<IntegerProperty> IntegerProperty::build(int defaultValue)
{
    return std::shared_ptr<IntegerProperty>(new IntegerProperty(defaultValue));
}

bool IntegerProperty::isInteger() const
{
    return true;
}

std::shared_ptr<IntegerProperty> IntegerProperty::asInteger()
{
    return shared_from_this();
}

bool IntegerProperty::hasRange() const
{
    return m_hasRange;
}

bool IntegerProperty::hasValues() const
{
    return m_values.size() > 0;
}

int IntegerProperty::getDefaultValue() const
{
    return m_defaultValue;
}

bool IntegerProperty::getRange(int &min, int &max) const
{
    if (!m_hasRange)
        return false;

    min = m_min;
    max = m_max;
    return true;
}

bool IntegerProperty::getValues(std::vector<int> &values) const
{
    if (!m_values.size())
        return false;

    values  = m_values;
    return true;
}

void IntegerProperty::setDefaultValue(int value)
{
    if(m_hasRange && !(value >= m_min && value <= m_max))
    {
        m_defaultValue = m_min;
    }
    else if(m_values.size() > 0
        && m_values.end() == std::find(m_values.begin(), m_values.end(), value))
    {
        m_defaultValue = m_values[0];
    }
    else
        m_defaultValue = value;
}

void IntegerProperty::setRange(int min, int max)
{
    m_min = min;
    m_max = max;
    m_hasRange = true;
    setDefaultValue(m_defaultValue);
}

void IntegerProperty::setValues(const std::vector<int> &values)
{
    m_values = values;
    setDefaultValue(m_defaultValue);
}

bool IntegerProperty::validate(const AttributeValueVariant &value)
{
    IntegerValidator integerValidator(shared_from_this());
    return boost::apply_visitor(integerValidator, value);
}

bool IntegerProperty::validate(const int &value)
{
    if (m_hasRange && (value < m_min || value > m_max))
    {
        return false;
    }
    else if (m_values.size() > 0)
    {
        if (m_values.end() == std::find(m_values.begin(), m_values.end(), value))
            return false;
    }

    return true;
}

AttributeValueVariant IntegerProperty::buildValue()
{
    return m_defaultValue;
}

DoubleProperty::DoubleProperty(double defaultValue)
    : AttributeProperty(AttributeProperty::Type::DOUBLE),
      m_defaultValue(defaultValue),
      m_min(0),
      m_max(0),
      m_hasRange(false) {}

std::shared_ptr<DoubleProperty> DoubleProperty::build(double defaultValue)
{
    return std::shared_ptr<DoubleProperty>(new DoubleProperty(defaultValue));
}

bool DoubleProperty::isDouble() const
{
    return true;
}

std::shared_ptr<DoubleProperty> DoubleProperty::asDouble()
{
    return shared_from_this();
}

bool DoubleProperty::hasRange() const
{
    return m_hasRange;
}

bool DoubleProperty::hasValues() const
{
    return m_values.size() > 0;
}

double DoubleProperty::getDefaultValue() const
{
    return m_defaultValue;
}

bool DoubleProperty::getRange(double &min, double &max) const
{
    if (!m_hasRange)
        return false;

    min = m_min;
    max = m_max;
    return true;
}

bool DoubleProperty::getValues(std::vector<double> &values) const
{
    if (!m_values.size())
        return false;

    values  = m_values;
    return true;
}

void DoubleProperty::setDefaultValue(double value)
{
    if(m_hasRange && !(value >= m_min && value <= m_max))
    {
        m_defaultValue = m_min;
    }
    else if(m_values.size() > 0
        && m_values.end() == std::find(m_values.begin(), m_values.end(), value))
    {
        m_defaultValue = m_values[0];
    }
    else
        m_defaultValue = value;
}

void DoubleProperty::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
    m_hasRange = true;
    setDefaultValue(m_defaultValue);
}

void DoubleProperty::setValues(const std::vector<double> &values)
{
    m_values = values;
    setDefaultValue(m_defaultValue);
}

bool DoubleProperty::validate(const AttributeValueVariant &value)
{
    DoubleValidator doubleValidator(shared_from_this());
    return boost::apply_visitor(doubleValidator, value);
}

bool DoubleProperty::validate(const double &value)
{
    if (m_hasRange && (value < m_min || value > m_max))
    {
        return false;
    }
    else if (m_values.size() > 0)
    {
        if (m_values.end() == std::find(m_values.begin(), m_values.end(), value))
            return false;
    }

    return true;
}

AttributeValueVariant DoubleProperty::buildValue()
{
    return m_defaultValue;
}

BooleanProperty::BooleanProperty(bool defaultValue)
    : AttributeProperty(AttributeProperty::Type::BOOLEAN),
      m_defaultValue(defaultValue) {}

std::shared_ptr<BooleanProperty> BooleanProperty::build(bool defaultValue)
{
    return std::shared_ptr<BooleanProperty>(new BooleanProperty(defaultValue));
}

bool BooleanProperty::isBoolean() const
{
    return true;
}

std::shared_ptr<BooleanProperty> BooleanProperty::asBoolean()
{
    return shared_from_this();
}

void BooleanProperty::setDefaultValue(bool value)
{
    m_defaultValue = value;
}

bool BooleanProperty::getDefaultValue() const
{
    return m_defaultValue;
}

bool BooleanProperty::validate(const AttributeValueVariant &)
{
    return true;
}

AttributeValueVariant BooleanProperty::buildValue()
{
    return m_defaultValue;
}

StringProperty::StringProperty(const std::string &defaultValue)
    : AttributeProperty(AttributeProperty::Type::STRING),
      m_defaultValue(defaultValue),
      m_min(0),
      m_max(0),
      m_hasRange(false) {}

std::shared_ptr<StringProperty> StringProperty::build(const std::string &defaultValue)
{
    return std::shared_ptr<StringProperty>(new StringProperty(defaultValue));
}

bool StringProperty::isString() const
{
    return true;
}

std::shared_ptr<StringProperty> StringProperty::asString()
{
    return shared_from_this();
}

bool StringProperty::hasRange() const
{
    return m_hasRange;
}

bool StringProperty::hasValues() const
{
    return m_values.size() > 0;
}

std::string StringProperty::getDefaultValue() const
{
    return m_defaultValue;
}

bool StringProperty::getRange(size_t &min, size_t &max) const
{
    if (!m_hasRange)
        return false;

    min = m_min;
    max = m_max;
    return true;
}

bool StringProperty::getValues(std::vector<std::string> &values) const
{
    if (!m_values.size())
        return false;

    values  = m_values;
    return true;
}

void StringProperty::setDefaultValue(const std::string &value)
{
    if(m_values.size() > 0
        && m_values.end() == std::find(m_values.begin(), m_values.end(), value))
    {
        m_defaultValue = m_values[0];
    }
    else if(m_hasRange)
    {
        if(value.length() >= m_min && value.length() <= m_max)
            m_defaultValue = value;
        else
            m_defaultValue.clear();
    }
    else
        m_defaultValue = value;
}

void StringProperty::setRange(size_t min, size_t max)
{
    m_min = min;
    m_max = max;
    m_hasRange = true;
    setDefaultValue(m_defaultValue);
}

void StringProperty::setValues(const std::vector<std::string> &values)
{
    m_values = values;
    setDefaultValue(m_defaultValue);
}

bool StringProperty::validate(const AttributeValueVariant &value)
{
    StringValidator stringValidator(shared_from_this());
    return boost::apply_visitor(stringValidator, value);
}

bool StringProperty::validate(const std::string &value)
{
    size_t length = value.length();
    if (m_hasRange && (length < m_min || length > m_max))
    {
        return false;
    }
    else if (m_values.size() > 0)
    {
        if (m_values.end() == std::find(m_values.begin(), m_values.end(), value))
        {
            return false;
        }
    }

    return true;
}

AttributeValueVariant StringProperty::buildValue()
{
    return m_defaultValue;
}

ArrayProperty::ArrayProperty()
    :   AttributeProperty(AttributeProperty::Type::ARRAY),
        m_min(0),
        m_max(0),
        m_isVariableSize(false),
        m_isUnique(false),
        m_hasRange(false) {}

std::shared_ptr<ArrayProperty> ArrayProperty::build()
{
    return std::shared_ptr<ArrayProperty>(new ArrayProperty());
}

bool ArrayProperty::isArray() const
{
    return true;
}

std::shared_ptr<ArrayProperty> ArrayProperty::asArray()
{
    return shared_from_this();
}

void ArrayProperty::setRange(size_t minItems, size_t maxItems)
{
    m_min = minItems;
    m_max = maxItems;
    m_hasRange = true;
}

void ArrayProperty::setVariable(bool state)
{
    m_isVariableSize = state;
}

void ArrayProperty::setUnique(bool state)
{
    m_isUnique = state;
}

bool ArrayProperty::setElementProperty(const std::shared_ptr<AttributeProperty> &property)
{
    if (!property)
        return false;

    m_elementProperty = property;
    return true;
}

bool ArrayProperty::hasRange() const
{
    return  m_hasRange;
}

bool ArrayProperty::isVariable() const
{
    return m_isVariableSize;
}

bool ArrayProperty::isUnique() const
{
    return m_isUnique;
}

size_t ArrayProperty::getMinItems() const
{
    return m_min;
}

size_t ArrayProperty::getMaxItems() const
{
    return m_max;
}

std::shared_ptr<AttributeProperty> ArrayProperty::getElementProperty()
{
    return m_elementProperty;
}

bool ArrayProperty::validate(const AttributeValueVariant &value)
{
    ArrayValidator arrayValidator(shared_from_this());
    return boost::apply_visitor(arrayValidator, value);
}

AttributeValueVariant ArrayProperty::buildValue()
{
    // Find the depth and element property
    std::shared_ptr<AttributeProperty> elementProperty;
    int depth = findDepth(elementProperty);

    if (depth < 1 || depth > 3)
    {
        throw SimulatorException(SIMULATOR_BAD_SCHEMA,
                                 "No support for array of depth more than 3!");
    }

    AttributeValueVariant elementValue  = elementProperty->buildValue();
    if (elementProperty->isInteger())
    {
        return buildArrayValue<int>(depth, elementValue);
    }
    else if (elementProperty->isDouble())
    {
        return buildArrayValue<double>(depth, elementValue);
    }
    else if (elementProperty->isBoolean())
    {
        return buildArrayValue<bool>(depth, elementValue);
    }
    else if (elementProperty->isString())
    {
        return buildArrayValue<std::string>(depth, elementValue);
    }
    else if (elementProperty->isModel())
    {
        return buildArrayValue<SimulatorResourceModel>(depth, elementValue);
    }

    throw SimulatorException(SIMULATOR_ERROR,
                             "Failed to build value from property of type array!");
}

int ArrayProperty::findDepth(std::shared_ptr<AttributeProperty> &elementProperty)
{
    if (!m_elementProperty)
    {
        throw SimulatorException(SIMULATOR_BAD_SCHEMA, "Invalid Array property!");
    }

    if (!m_elementProperty->isArray())
    {
        elementProperty = m_elementProperty;
        return 1;
    }

    return (1 + m_elementProperty->asArray()->findDepth(elementProperty));
}

ModelProperty::ModelProperty()
    : AttributeProperty(AttributeProperty::Type::MODEL) {}

std::shared_ptr<ModelProperty> ModelProperty::build()
{
    return std::shared_ptr<ModelProperty>(new ModelProperty());
}

bool ModelProperty::isModel() const
{
    return true;
}

std::shared_ptr<ModelProperty> ModelProperty::asModel()
{
    return shared_from_this();
}

bool ModelProperty::add(const std::string &name,
                        const std::shared_ptr<AttributeProperty> &property, bool required)
{
    if (name.empty() || !property)
        return false;

    m_childProperties[name] = property;
    m_requiredAttributes[name] = required;
    return true;
}

std::shared_ptr<AttributeProperty> ModelProperty::get(
    const std::string &name)
{
    if (m_childProperties.end() != m_childProperties.find(name))
        return m_childProperties[name];
    return nullptr;
}

std::unordered_map<std::string, std::shared_ptr<AttributeProperty> >
ModelProperty::getChildProperties()
{
    return m_childProperties;
}

bool ModelProperty::isRequired(const std::string &name)
{
    if (m_requiredAttributes.end() == m_requiredAttributes.find(name))
        return false;

    return m_requiredAttributes[name];
}

void ModelProperty::remove(const std::string &name)
{
    if (m_requiredAttributes.end() != m_requiredAttributes.find(name))
        m_requiredAttributes.erase(m_requiredAttributes.find(name));

    if (m_childProperties.end() != m_childProperties.find(name))
        m_childProperties.erase(m_childProperties.find(name));
}

void ModelProperty::setRequired(const std::string &name)
{
    if (m_requiredAttributes.end() != m_requiredAttributes.find(name))
        m_requiredAttributes[name] = true;
}

void ModelProperty::unsetRequired(const std::string &name)
{
    if (m_requiredAttributes.end() != m_requiredAttributes.find(name))
        m_requiredAttributes[name] = false;
}

bool ModelProperty::validate(const AttributeValueVariant &value)
{
    ModelValidator modelValidator(shared_from_this());
    return boost::apply_visitor(modelValidator, value);
}

bool ModelProperty::validate(const SimulatorResourceModel &model)
{
    for (auto &attributeEntry : model.getAttributeValues())
    {
        std::shared_ptr<AttributeProperty> childProperty = get(attributeEntry.first);
        if (childProperty)
        {
            if (!childProperty->validate(attributeEntry.second))
                return false;
        }
    }

    return true;
}

AttributeValueVariant ModelProperty::buildValue()
{
    return buildResourceModel();
}

SimulatorResourceModel ModelProperty::buildResourceModel()
{
    SimulatorResourceModel resourceModel;
    for (auto &propertyElement : m_childProperties)
    {
        resourceModel.add(propertyElement.first, (propertyElement.second)->buildValue());
    }

    return resourceModel;
}
