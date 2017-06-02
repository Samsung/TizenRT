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

#include "attribute_value_generator.h"

std::unique_ptr<AttributeValueGen> AttributeValueGenFactory::create(
    const std::shared_ptr<AttributeProperty> &property)
{
    if (!property)
        return nullptr;

    if (property->isInteger())
    {
        std::shared_ptr<IntegerProperty> intProperty = property->asInteger();
        if (intProperty->hasRange())
        {
            int min = 0;
            int max = 0;
            intProperty->getRange(min, max);
            return std::unique_ptr<AttributeValueGen>(
                       new RangeValueGen<int>(min, max));
        }
        else if (intProperty->hasValues())
        {
            std::vector<int> values;
            intProperty->getValues(values);
            return std::unique_ptr<AttributeValueGen>(
                       new ValuesSetGen<int>(values));
        }
    }
    else if (property->isDouble())
    {
        std::shared_ptr<DoubleProperty> doubleProperty = property->asDouble();
        if (doubleProperty->hasRange())
        {
            double min = 0.0;
            double max = 0.0;
            doubleProperty->getRange(min, max);
            return std::unique_ptr<AttributeValueGen>(
                       new RangeValueGen<double>(min, max));
        }
        else if (doubleProperty->hasValues())
        {
            std::vector<double> values;
            doubleProperty->getValues(values);
            return std::unique_ptr<AttributeValueGen>(
                       new ValuesSetGen<double>(values));
        }
    }
    else if (property->isBoolean())
    {
        return std::unique_ptr<AttributeValueGen>(new ValuesSetGen<bool>({true, false}));
    }
    else if (property->isString())
    {
        std::shared_ptr<StringProperty> stringProperty = property->asString();
        if (stringProperty->hasValues())
        {
            std::vector<std::string> values;
            stringProperty->getValues(values);
            return std::unique_ptr<AttributeValueGen>(
                       new ValuesSetGen<std::string>(values));
        }
    }

    return nullptr;
}
