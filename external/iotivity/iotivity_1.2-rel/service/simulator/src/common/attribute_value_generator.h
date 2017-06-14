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

#ifndef SIMULATOR_ATTRIBUTE_VALUE_GENERATOR_H_
#define SIMULATOR_ATTRIBUTE_VALUE_GENERATOR_H_

#include "simulator_resource_model_schema.h"

class AttributeValueGen
{
    public:
        virtual bool hasNext() = 0;
        virtual AttributeValueVariant next() = 0;
        virtual AttributeValueVariant value() = 0;
        virtual void reset() = 0;
};

template <typename TYPE>
class RangeValueGen : public AttributeValueGen
{
    private:
        TYPE m_min;
        TYPE m_max;
        TYPE m_cur;

    public:
        RangeValueGen(TYPE min, TYPE max) : m_min(min), m_max(max), m_cur(min) {}

        bool hasNext()
        {
            return (m_cur <= m_max);
        }

        AttributeValueVariant next()
        {
            TYPE value = m_cur;
            m_cur++;
            return value;
        }

        AttributeValueVariant value()
        {
            return m_cur - 1;
        }

        void reset()
        {
            m_cur = m_min;
        }
};

template <typename TYPE>
class ValuesSetGen : public AttributeValueGen
{
    private:
        std::vector<TYPE> m_values;
        size_t m_index;

    public:
        ValuesSetGen(const std::vector<TYPE> &values) : m_values(values), m_index(0) {}

        bool hasNext()
        {
            return (m_index < m_values.size());
        }

        AttributeValueVariant next()
        {
            return m_values[m_index++];
        }

        AttributeValueVariant value()
        {
            return m_values[m_index - 1];
        }

        void reset()
        {
            m_index = 0;
        }
};

class AttributeValueGenFactory
{
    public:
        static std::unique_ptr<AttributeValueGen> create(
            const std::shared_ptr<AttributeProperty> &property);
};

#endif
