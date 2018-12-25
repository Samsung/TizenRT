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

#ifndef BASESET_H_
#define BASESET_H_

#include <string>
#include "AraStorage/DatabaseMacros.h"

/**
 * Set clause helper(parent) class.
 */
class BaseSet
{
public:
	BaseSet() = default;
	virtual ~BaseSet() = default;

	/// Set key and text value data.
	void SetData(const std::string &key, const std::string &value);

	/// Set key and integer value data.
	void SetData(const std::string &key, int value);

	/// Set key and long value data.
	void SetData(const std::string &key, long value);

	/// Set key and double value data.
	void SetData(const std::string &key, double value);

	/// Get a map of set clause.
	void GetDataMap(DMap &dataMap);

	/// Set key and text value of where clause.
	void SetWhere(const std::string &key, const std::string &value);

	/// Set key and integer value of where clause.
	void SetWhere(const std::string &key, int value);

	/// Set key and long value of where clause.
	void SetWhere(const std::string &key, long value);

	/// Set key and double value of where clause.
	void SetWhere(const std::string &key, double value);

	/// Get a map of where clause.
	void GetWhereMap(DMap &whereMap);

private:
	/// The map of set clause.
	DMap m_dataMap;

	/// The map of where clause.
	DMap m_whereMap;
};

#endif /* BASEMAP_H_ */
