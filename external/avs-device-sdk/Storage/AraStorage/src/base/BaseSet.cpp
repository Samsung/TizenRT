/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include "AraStorage/base/BaseSet.h"
#include "AraStorage/DatabaseMacros.h"

#include <AVSCommon/Utils/Logger/Logger.h>

static const std::string TAG("BaseSet");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

void BaseSet::SetData(const std::string &key, const std::string &value)
{
	std::string attr_value(ATTR_STRING_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(value);
	m_dataMap.insert(DPair(key, attr_value));
}

void BaseSet::SetData(const std::string &key, int value)
{
	std::string attr_value(ATTR_INT_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_dataMap.insert(DPair(key, attr_value));
}

void BaseSet::SetData(const std::string &key, long value)
{
	std::string attr_value(ATTR_LONG_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_dataMap.insert(DPair(key, attr_value));
}

void BaseSet::SetData(const std::string &key, double value)
{
	std::string attr_value(ATTR_DOUBLE_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_dataMap.insert(DPair(key, attr_value));
}

void BaseSet::GetDataMap(DMap &dataMap)
{
	dataMap = m_dataMap;
}

void BaseSet::SetWhere(const std::string &key, const std::string &value)
{
	std::string attr_value(ATTR_STRING_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(value);
	m_whereMap.insert(DPair(key, attr_value));
}

void BaseSet::SetWhere(const std::string &key, int value)
{
	std::string attr_value(ATTR_INT_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_whereMap.insert(DPair(key, attr_value));
}

void BaseSet::SetWhere(const std::string &key, long value)
{
	std::string attr_value(ATTR_LONG_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_whereMap.insert(DPair(key, attr_value));
}

void BaseSet::SetWhere(const std::string &key, double value)
{
	std::string attr_value(ATTR_DOUBLE_PREFIX);
	attr_value.append(ATTR_VALUE_PREFIX).append(std::to_string(value));
	m_whereMap.insert(DPair(key, attr_value));
}

void BaseSet::GetWhereMap(DMap &whereMap)
{
	whereMap = m_whereMap;
}
