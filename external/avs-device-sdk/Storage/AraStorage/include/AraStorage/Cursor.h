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

#ifndef CURSOR_H_
#define CURSOR_H_

#include <string>
#include <vector>
#include <map>

#define CURSOR_OK 0
#define CURSOR_ERROR 1

/**
 * The Cursor class provides parsed data from databases on based arastorage.
 * The cursor works with position(row) and column name.
 */
class Cursor {
public:
	Cursor();
	virtual ~Cursor();

	/// Initialize cursor states.
	void Initialize();

	/// Set column key to get data from query statement. Application does not need to use it.
	void SetColumn(const char *columnName);

	/// Set data from query statement. Application does not need to use it.
	void SetData(const char *columnName, const char *value);

	/// Return row count.
	int GetCount();

	/// Get data value of string type.
	int GetString(const char *columnName, std::string &value);
	int GetString(const std::string &columnName, std::string &value);

	/// Get data value of integer type.
	int GetInt(const char *columnName, int &value);
	int GetInt(const std::string &columnName, int &value);

	/// Get data value of double type.
	int GetDouble(const char *columnName, double &value);
	int GetDouble(const std::string &columnName, double &value);

	/// Get column name;
	int GetColumn(std::vector<std::string> &columnSet);

	/// Move cursor position to first.
	bool MoveToFirst();

	/// Move cursor position to next.
	bool MoveToNext();

	/// Move cursor position to previous.
	bool MoveToPrev();

	/// Move cursor position to last.
	bool MoveToLast();

	/// Move cursor position to in-argument.
	bool MoveToPosition(int position);

	/// Return current position.
	int GetPosition();

private:
	/// Return true or false whether current position is valid or not.
	bool IsValidPosition(int position);

private:

	/// Result data of select query.
	std::map<std::string, std::vector<std::string>> m_dataMap;

	/// Column name of select query.
	std::vector<std::string> m_columnMap;

	/// Cursor position.
	int m_position;
};

#endif /* CURSOR_H_ */
