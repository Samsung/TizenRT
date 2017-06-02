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

#include "simulator_resource_model.h"
#include "simulator_resource_model_schema.h"
#include "simulator_exceptions.h"
#include "logger.h"

#include <boost/lexical_cast.hpp>
#include <cfloat>

#define TAG "SIM_RESOURCE_MODEL"

template <typename T>
struct TypeConverter {};

template <>
struct TypeConverter<int>
{
    constexpr static AttributeValueType type =
        AttributeValueType::INTEGER;
};

template <>
struct TypeConverter<double>
{
    constexpr static AttributeValueType type =
        AttributeValueType::DOUBLE;
};

template <>
struct TypeConverter<bool>
{
    constexpr static AttributeValueType type =
        AttributeValueType::BOOLEAN;
};

template <>
struct TypeConverter<std::string>
{
    constexpr static AttributeValueType type =
        AttributeValueType::STRING;
};

template <>
struct TypeConverter<SimulatorResourceModel>
{
    constexpr static AttributeValueType type =
        AttributeValueType::RESOURCE_MODEL;
};

template <typename T>
struct TypeDetails
{
    constexpr static AttributeValueType type =
        TypeConverter<T>::type;
    constexpr static AttributeValueType baseType =
        TypeConverter<T>::type;
    constexpr static int depth = 0;
};

template <typename T>
struct TypeDetails<std::vector<T>>
{
    constexpr static AttributeValueType type =
        AttributeValueType::VECTOR;
    constexpr static AttributeValueType baseType =
        TypeDetails<T>::baseType;
    constexpr static int depth = 1 + TypeDetails<T>::depth;
};

class AttributeTypeVisitor : public boost::static_visitor<>
{
    public:
        AttributeTypeVisitor() : m_type(AttributeValueType::UNKNOWN),
            m_baseType(AttributeValueType::UNKNOWN), m_depth(0) {}

        template <typename T>
        void operator ()(const T &)
        {
            m_type = TypeDetails<T>::type;
            m_baseType = TypeDetails<T>::baseType;
            m_depth = TypeDetails<T>::depth;
        }

        AttributeValueType m_type;
        AttributeValueType m_baseType;
        int m_depth;
};

class OCRepresentationBuilder
{
    public:
        class ValueConverter : public boost::static_visitor<>
        {
            public:
                ValueConverter(OC::OCRepresentation &rep, const std::string &name)
                    : m_rep(rep), m_name(name) {}

                template <typename T>
                void operator ()(const T &value)
                {
                    m_rep.setValue(m_name, value);
                }

                void operator ()(const SimulatorResourceModel &value)
                {
                    OC::OCRepresentation ocRep;
                    for (auto &element : value.getAttributeValues())
                    {
                        ValueConverter visitor(ocRep, element.first);
                        boost::apply_visitor(visitor, element.second);
                    }

                    m_rep.setValue(m_name, ocRep);
                }

                template <typename T>
                void operator ()(const std::vector<T> &values)
                {
                    m_rep.setValue(m_name, values);
                }

                void operator ()(const std::vector<SimulatorResourceModel> &values)
                {
                    std::vector<OC::OCRepresentation> ocRepArray(values.size());
                    for (size_t i = 0; i < values.size(); i++)
                    {
                        for (auto &element : values[i].getAttributeValues())
                        {
                            ValueConverter visitor(ocRepArray[i], element.first);
                            boost::apply_visitor(visitor, element.second);
                        }
                    }

                    m_rep.setValue(m_name, ocRepArray);
                }

                template <typename T>
                void operator ()(const std::vector<std::vector<T>> &values)
                {
                    m_rep.setValue(m_name, values);
                }

                void operator ()(const std::vector<std::vector<SimulatorResourceModel>> &values)
                {
                    std::vector<std::vector<OC::OCRepresentation>> ocRepArray;
                    for (size_t i = 0; i < values.size(); i++)
                    {
                        for (size_t j = 0; j < values[i].size(); j++)
                        {
                            for (auto &element : values[i][j].getAttributeValues())
                            {
                                ValueConverter visitor(ocRepArray[i][j], element.first);
                                boost::apply_visitor(visitor, element.second);
                            }
                        }
                    }

                    m_rep.setValue(m_name, ocRepArray);
                }

                template <typename T>
                void operator ()(const std::vector<std::vector<std::vector<T>>> &values)
                {
                    m_rep.setValue(m_name, values);
                }

                void operator ()(const std::vector<std::vector<std::vector<SimulatorResourceModel>>> &values)
                {
                    std::vector<std::vector<std::vector<OC::OCRepresentation>>> ocRepArray;
                    for (size_t i = 0; i < values.size(); i++)
                    {
                        for (size_t j = 0; j < values[i].size(); j++)
                        {
                            for (size_t k = 0; j < values[i][j].size(); k++)
                            {
                                for (auto &element : values[i][j][k].getAttributeValues())
                                {
                                    ValueConverter visitor(ocRepArray[i][j][k], element.first);
                                    boost::apply_visitor(visitor, element.second);
                                }
                            }
                        }
                    }

                    m_rep.setValue(m_name, ocRepArray);
                }

            private:
                OC::OCRepresentation &m_rep;
                std::string m_name;
        };

        OC::OCRepresentation build(const SimulatorResourceModel &model)
        {
            OC::OCRepresentation ocRep;
            for (auto &element : model.getAttributeValues())
            {
                ValueConverter visitor(ocRep, element.first);
                boost::apply_visitor(visitor, element.second);
            }

            return std::move(ocRep);
        }
};

// TODO: Class is very heavy, revisit again to clean
class SimulatorResourceModelBuilder
{
    public:
        SimulatorResourceModel build(const OC::OCRepresentation &ocRep)
        {
            SimulatorResourceModel resModel;
            handleRepresentationType(resModel, ocRep);
            return std::move(resModel);
        }

    private:
        void handleRepresentationType(SimulatorResourceModel &resModel,
                                      const OC::OCRepresentation &ocRep)
        {
            for (auto &ocAttribute : ocRep)
            {
                if (OC::AttributeType::Integer == ocAttribute.type())
                {
                    resModel.add<int>(ocAttribute.attrname(), ocAttribute.getValue<int>());
                }
                else if (OC::AttributeType::Double == ocAttribute.type())
                {
                    resModel.add<double>(ocAttribute.attrname(), ocAttribute.getValue<double>());
                }
                else if (OC::AttributeType::Boolean == ocAttribute.type())
                {
                    resModel.add<bool>(ocAttribute.attrname(), ocAttribute.getValue<bool>());
                }
                else if (OC::AttributeType::String == ocAttribute.type())
                {
                    resModel.add<std::string>(ocAttribute.attrname(), ocAttribute.getValue<std::string>());
                }
                else if (OC::AttributeType::OCRepresentation == ocAttribute.type())
                {
                    SimulatorResourceModel subResModel;
                    OC::OCRepresentation ocSubRep = ocAttribute.getValue<OC::OCRepresentation>();
                    handleRepresentationType(subResModel, ocSubRep);
                    resModel.add<SimulatorResourceModel>(ocAttribute.attrname(), subResModel);
                }
                else if (OC::AttributeType::Vector == ocAttribute.type())
                {
                    handleVectorType(resModel, ocAttribute);
                }
            }
        }

        void handleVectorType(SimulatorResourceModel &resModel,
                              const OC::OCRepresentation::AttributeItem &ocAttribute)
        {
            if (1 == ocAttribute.depth())
            {
                handleVectorTypeDepth1(resModel, ocAttribute);
            }
            else if (2 == ocAttribute.depth())
            {
                handleVectorTypeDepth2(resModel, ocAttribute);
            }
            else if (3 == ocAttribute.depth())
            {
                handleVectorTypeDepth3(resModel, ocAttribute);
            }
        }

        void handleVectorTypeDepth1(SimulatorResourceModel &resModel,
                                    const OC::OCRepresentation::AttributeItem &ocAttribute)
        {
            if (OC::AttributeType::Integer == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<int>>());
            }
            else if (OC::AttributeType::Double == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<double>>());
            }
            else if (OC::AttributeType::Boolean == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<bool>>());
            }
            else if (OC::AttributeType::String == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<std::string>>());
            }
            else if (OC::AttributeType::OCRepresentation == ocAttribute.base_type())
            {
                std::vector<OC::OCRepresentation> ocSubRepArray =
                    ocAttribute.getValue<std::vector<OC::OCRepresentation>>();

                std::vector<SimulatorResourceModel> subResModelArray(ocSubRepArray.size());
                for  (size_t i = 0; i < ocSubRepArray.size(); i++)
                {
                    handleRepresentationType(subResModelArray[i], ocSubRepArray[i]);
                }

                resModel.add<std::vector<SimulatorResourceModel>>(ocAttribute.attrname(), subResModelArray);
            }
        }

        void handleVectorTypeDepth2(SimulatorResourceModel &resModel,
                                    const OC::OCRepresentation::AttributeItem &ocAttribute)
        {
            if (OC::AttributeType::Integer == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<std::vector<int>>>());
            }
            else if (OC::AttributeType::Double == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<std::vector<double>>>());
            }
            else if (OC::AttributeType::Boolean == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<std::vector<bool>>>());
            }
            else if (OC::AttributeType::String == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(), ocAttribute.getValue<std::vector<std::vector<std::string>>>());
            }
            else if (OC::AttributeType::OCRepresentation == ocAttribute.base_type())
            {
                std::vector<std::vector<OC::OCRepresentation>> ocSubRepArray =
                            ocAttribute.getValue<std::vector<std::vector<OC::OCRepresentation>>>();

                std::vector<std::vector<SimulatorResourceModel>> subResModelArray(ocSubRepArray.size());
                for  (size_t i = 0; i < ocSubRepArray.size(); i++)
                {
                    std::vector<SimulatorResourceModel> innerArray1(ocSubRepArray[i].size());
                    for  (size_t j = 0; j < ocSubRepArray[i].size(); j++)
                        handleRepresentationType(innerArray1[j], ocSubRepArray[i][j]);
                    subResModelArray[i] = innerArray1;
                }

                resModel.add<std::vector<std::vector<SimulatorResourceModel>>>(
                    ocAttribute.attrname(), subResModelArray);
            }
        }

        void handleVectorTypeDepth3(SimulatorResourceModel &resModel,
                                    const OC::OCRepresentation::AttributeItem &ocAttribute)
        {
            if (OC::AttributeType::Integer == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(),
                             ocAttribute.getValue<std::vector<std::vector<std::vector<int>>>>());
            }
            else if (OC::AttributeType::Double == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(),
                             ocAttribute.getValue<std::vector<std::vector<std::vector<double>>>>());
            }
            else if (OC::AttributeType::Boolean == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(),
                             ocAttribute.getValue<std::vector<std::vector<std::vector<bool>>>>());
            }
            else if (OC::AttributeType::String == ocAttribute.base_type())
            {
                resModel.add(ocAttribute.attrname(),
                             ocAttribute.getValue<std::vector<std::vector<std::vector<std::string>>>>());
            }
            else if (OC::AttributeType::OCRepresentation == ocAttribute.base_type())
            {
                std::vector<std::vector<std::vector<OC::OCRepresentation>>> ocSubRepArray =
                    ocAttribute.getValue<std::vector<std::vector<std::vector<OC::OCRepresentation>>>>();

                std::vector<std::vector<std::vector<SimulatorResourceModel>>> subResModelArray(
                    ocSubRepArray.size());
                for  (size_t i = 0; i < ocSubRepArray.size(); i++)
                {
                    std::vector<std::vector<SimulatorResourceModel>> innerArray1(ocSubRepArray[i].size());
                    for  (size_t j = 0; j < ocSubRepArray[i].size(); j++)
                    {
                        std::vector<SimulatorResourceModel> innerArray2(ocSubRepArray[i][j].size());
                        for  (size_t k = 0; k < ocSubRepArray[i][j].size(); k++)
                        {
                            handleRepresentationType(innerArray2[k], ocSubRepArray[i][j][k]);
                        }
                        innerArray1[j] = innerArray2;
                    }
                    subResModelArray[i] = innerArray1;
                }

                resModel.add<std::vector<std::vector<std::vector<SimulatorResourceModel>>>>(
                    ocAttribute.attrname(), subResModelArray);
            }
        }
};

class ToStringConverter
{
    public:
        class ValueVisitor : public boost::static_visitor<std::string>
        {
            public:

                template <typename T>
                std::string operator ()(const T &value)
                {
                    try
                    {
                        return boost::lexical_cast<std::string>(value);
                    }
                    catch (const boost::bad_lexical_cast &e)
                    {
                        return "CONVERSION_FAILED!";
                    }
                }

                std::string operator ()(const SimulatorResourceModel &value)
                {
                    std::ostringstream out;
                    out << "{ ";
                    for (auto &element : value.getAttributeValues())
                    {
                        out << "\"" << element.first << "\" : ";

                        ValueVisitor visitor;
                        out << boost::apply_visitor(visitor, element.second);

                        out << ", ";
                    }
                    out << "}";
                    return out.str();
                }

                template <typename T>
                std::string operator ()(const std::vector<T> &values)
                {
                    std::ostringstream out;
                    out << "[ ";

                    for (size_t i = 0; i < values.size(); i++)
                    {
                        out << operator ()(values[i]);
                        out << " ";
                    }

                    out << "]";
                    return out.str();
                }

                template <typename T>
                std::string operator ()(const std::vector<std::vector<T>> &values)
                {
                    std::ostringstream out;
                    out << "[ ";

                    for (size_t i = 0; i < values.size(); i++)
                    {
                        out << operator ()(values[i]);
                        out << " ";
                    }

                    out << "]";
                    return out.str();
                }

                template <typename T>
                std::string operator ()(const std::vector<std::vector<std::vector<T>>> &values)
                {
                    std::ostringstream out;
                    out << "[ ";

                    for (size_t i = 0; i < values.size(); i++)
                    {
                        out << operator ()(values[i]);
                        out << " ";
                    }

                    out << "]";
                    return out.str();
                }
        };

        std::string getStringRepresentation(const SimulatorResourceModel &resModel)
        {
            ValueVisitor visitor;
            AttributeValueVariant value = resModel;
            return boost::apply_visitor(visitor, value);
        }

        std::string getStringRepresentation(const AttributeValueVariant &value)
        {
            ValueVisitor visitor;
            return boost::apply_visitor(visitor, value);
        }
};

SimulatorResourceModel::TypeInfo::TypeInfo(
    AttributeValueType type, AttributeValueType baseType, int depth)
    :   m_type (type), m_baseType(baseType), m_depth(depth) {}

AttributeValueType SimulatorResourceModel::TypeInfo::type() const
{
    return m_type;
}

AttributeValueType SimulatorResourceModel::TypeInfo::baseType() const
{
    return m_baseType;
}

int SimulatorResourceModel::TypeInfo::depth() const
{
    return m_depth;
}

bool SimulatorResourceModel::TypeInfo::operator==(
    const SimulatorResourceModel::TypeInfo &rhs) const
{
    if (m_type == rhs.m_type && m_baseType == rhs.m_baseType
        && m_depth == rhs.m_depth)
        return true;
    return false;
}

SimulatorResourceAttribute::SimulatorResourceAttribute(const std::string &name)
    : m_name(name) {}

SimulatorResourceAttribute::SimulatorResourceAttribute(const std::string &name,
        const AttributeValueVariant &value) : m_name(name)
{
    m_value = std::make_shared<AttributeValueVariant>(value);
}

void SimulatorResourceAttribute::setName(const std::string &name)
{
    m_name = name;
}

std::string SimulatorResourceAttribute::getName() const
{
    return m_name;
}

const SimulatorResourceModel::TypeInfo SimulatorResourceAttribute::getType() const
{
    if (m_value)
    {
        AttributeTypeVisitor typeVisitor;
        boost::apply_visitor(typeVisitor, *(m_value.get()));
        return SimulatorResourceModel::TypeInfo(typeVisitor.m_type, typeVisitor.m_baseType,
                                                typeVisitor.m_depth);
    }

    return SimulatorResourceModel::TypeInfo();
}

void SimulatorResourceAttribute::setProperty(const std::shared_ptr<AttributeProperty> &property)
{
    m_property = property;
}

std::shared_ptr<AttributeProperty> SimulatorResourceAttribute::getProperty() const
{
    return m_property;
}

AttributeValueVariant SimulatorResourceAttribute::getValue() const
{
    return *m_value;
}

std::string SimulatorResourceAttribute::asString() const
{
    return ToStringConverter().getStringRepresentation(*m_value);
}

bool SimulatorResourceModel::remove(const std::string &name)
{
    if (m_attributes.end() == m_attributes.find(name))
    {
        return false;
    }

    m_attributes.erase(m_attributes.find(name));
    return true;
}

bool SimulatorResourceModel::contains(const std::string &name) const
{
    if (m_attributes.end() != m_attributes.find(name))
    {
        return true;
    }

    return false;
}

size_t SimulatorResourceModel::size() const
{
    return m_attributes.size();
}

SimulatorResourceModel::TypeInfo SimulatorResourceModel::getType(const std::string &name) const
{
    if (m_attributes.end() != m_attributes.find(name))
    {
        return getTypeInfo(m_attributes.find(name)->second);
    }

    return SimulatorResourceModel::TypeInfo();
}

std::map<std::string, AttributeValueVariant> SimulatorResourceModel::getAttributeValues() const
{
    return m_attributes;
}

AttributeValueVariant SimulatorResourceModel::getAttributeValue(const std::string &name) const
{
    auto ite = m_attributes.find(name);
    if (m_attributes.end() != ite)
    {
        return (*ite).second;
    }

    return AttributeValueVariant();
}

std::set<std::string> SimulatorResourceModel::getAttributeNameSet() const
{
    std::set<std::string> nameSet;
    for (auto &attributeEntry : m_attributes)
    {
        nameSet.insert(attributeEntry.first);
    }

    return nameSet;
}

SimulatorResourceModel::TypeInfo SimulatorResourceModel::getTypeInfo(const AttributeValueVariant
        &value) const
{
    AttributeTypeVisitor typeVisitor;
    boost::apply_visitor(typeVisitor, value);
    SimulatorResourceModel::TypeInfo typeInfo(typeVisitor.m_type, typeVisitor.m_baseType,
            typeVisitor.m_depth);
    return typeInfo;
}

bool SimulatorResourceModel::updateValue(const std::string &name,
        const AttributeValueVariant &value)
{
    if (name.empty())
    {
        return false;
    }

    if (m_attributes.end() == m_attributes.find(name))
    {
        return false;
    }

    if (!(getTypeInfo(m_attributes[name]) == getTypeInfo(value)))
    {
        return false;
    }

    m_attributes[name] = value;
    return true;
}

OC::OCRepresentation SimulatorResourceModel::asOCRepresentation() const
{
    return OCRepresentationBuilder().build(*this);
}

std::string SimulatorResourceModel::asString() const
{
    return ToStringConverter().getStringRepresentation(*this);
}

SimulatorResourceModel SimulatorResourceModel::build(const OC::OCRepresentation &ocRep)
{
    return SimulatorResourceModelBuilder().build(ocRep);
}

