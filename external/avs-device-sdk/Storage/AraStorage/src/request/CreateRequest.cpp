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

#include "AraStorage/request/CreateRequest.h"
#include "AraStorage/DatabaseMacros.h"
#include <AVSCommon/Utils/Logger/Logger.h>

static const std::string TAG("CreateRequest");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

CreateRequest::CreateRequest(const std::string &tableName)
	: BaseTable(tableName)
{
}

void CreateRequest::SetPrimaryKey(const std::string &key, const std::string &type)
{
	m_primaryKey = key;
	m_primaryType = type;
}

void CreateRequest::SetCompositeKey(const std::string &key, const std::string &type)
{
	m_compositeKey = key;
	m_compositeType = type;
}

void CreateRequest::SetColumnKey(const std::string &key, const std::string &type)
{
	m_columnVector.push_back(DPair(key, type));
}

void CreateRequest::GetPrimaryKey(std::string &key, std::string &type)
{
	key = m_primaryKey;
	type = m_primaryType;
}

void CreateRequest::GetCompositeKey(std::string & key, std::string &type)
{
	key = m_compositeKey;
	type = m_compositeType;
}

void CreateRequest::GetColumnKey(std::vector<DPair> &columnKey)
{
	columnKey = m_columnVector;
}
