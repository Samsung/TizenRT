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

#include <sstream>

#include "AraStorage/Cursor.h"
#include "AraStorage/DatabaseMacros.h"

#include <AVSCommon/Utils/Logger/Logger.h>

using namespace std;

static const std::string TAG("Cursor");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

Cursor::Cursor()
	: m_position(-1)
{
}

Cursor::~Cursor()
{
}

void Cursor::Initialize()
{
	m_position = -1;
	m_dataMap.clear();
	m_columnMap.clear();
}

void Cursor::SetColumn(const char *columnName)
{
	m_columnMap.push_back(string(columnName));
	m_dataMap.insert(pair<string, vector<string>>(string(columnName), vector<string>()));
}

void Cursor::SetData(const char *columnName, const char *value)
{
	auto it = m_dataMap.find(string(columnName));
	if (it != m_dataMap.end()) {
		it->second.push_back(string(value));
	}
}

int Cursor::GetCount()
{
	auto it = m_dataMap.begin();
	if (it != m_dataMap.end()) {
		return it->second.size();
	}

	return 0;
}

int Cursor::GetDouble(const char *columnName, double &value)
{
	return GetDouble(std::string(columnName), value);
}

int Cursor::GetDouble(const std::string &columnName, double &value)
{
	auto it = m_dataMap.find(columnName);
	if (it == m_dataMap.end()) {
		ACSDK_ERROR(LX("GetDoubleFailed").d("reason", "columnNotFound").d("columnName", columnName));
		return CURSOR_ERROR;
	}

	if (!IsValidPosition(m_position)) {
		ACSDK_ERROR(LX("GetDoubleFailed").d("reason", "cursorPositionOutOfRange").d("position", m_position));
		return CURSOR_ERROR;
	}

	value = std::stod(it->second[m_position]);
	return CURSOR_OK;
}

int Cursor::GetInt(const char *columnName, int &value)
{
	return GetInt(std::string(columnName), value);
}

int Cursor::GetInt(const std::string &columnName, int &value)
{
	auto it = m_dataMap.find(columnName);
	if (it == m_dataMap.end()) {
		ACSDK_ERROR(LX("GetIntFailed").d("reason", "columnNotFound").d("columnName", columnName));
		return CURSOR_ERROR;
	}

	if (!IsValidPosition(m_position)) {
		ACSDK_ERROR(LX("GetIntFailed").d("reason", "cursorPositionOutOfRange").d("position", m_position));
		return CURSOR_ERROR;
	}

	value = std::stoi(it->second[m_position]);
	return CURSOR_OK;
}

int Cursor::GetString(const char *columnName, string &value)
{
	return GetString(std::string(columnName), value);
}

int Cursor::GetString(const std::string &columnName, std::string &value)
{
	auto it = m_dataMap.find(columnName);
	if (it == m_dataMap.end()) {
		ACSDK_ERROR(LX("GetStringFailed").d("reason", "columnNotFound").d("columnName", columnName));
		return CURSOR_ERROR;
	}

	if (!IsValidPosition(m_position)) {
		ACSDK_ERROR(LX("GetStringFailed").d("reason", "cursorPositionOutOfRange").d("position", m_position));
		return CURSOR_ERROR;
	}

	value = it->second[m_position];
	return CURSOR_OK;
}

int Cursor::GetColumn(vector<string> &columnSet)
{
	if (m_columnMap.size() == 0) {
		return CURSOR_ERROR;
	}

	columnSet = m_columnMap;
	return CURSOR_OK;
}

bool Cursor::MoveToFirst()
{
	return MoveToPosition(0);
}

bool Cursor::MoveToLast()
{
	return MoveToPosition(GetCount() - 1);
}

bool Cursor::MoveToNext()
{
	return MoveToPosition(GetPosition() + 1);
}

bool Cursor::MoveToPrev()
{
	return MoveToPosition(GetPosition() - 1);
}

bool Cursor::MoveToPosition(int position)
{
	if (!IsValidPosition(position)) {
		m_position = -1;
		return false;
	}

	m_position = position;
	return true;
}

int Cursor::GetPosition()
{
	return m_position;
}

bool Cursor::IsValidPosition(int position)
{
	return (position >= 0 && position < GetCount());
}
