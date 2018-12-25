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

#ifndef CREATETABLE_H_
#define CREATETABLE_H_

#include <vector>
#include <string>

#include "AraStorage/DatabaseMacros.h"
#include "AraStorage/base/BaseTable.h"

/**
 * Create table request set class.
 * This class provide getter / setter for generating create table AQL query.
 */
class CreateRequest
	: public BaseTable
{
public:
	CreateRequest(const std::string &tableName);
	CreateRequest() = default;
	virtual ~CreateRequest() = default;

	/// Set primary key when table creation.
	void SetPrimaryKey(const std::string &key, const std::string &type);

	/// Set Composite key when table creation.
	void SetCompositeKey(const std::string &key, const std::string &type);

	/// Set column name when table creation.
	void SetColumnKey(const std::string &key, const std::string &type);

	/// Get primary key and type.
	void GetPrimaryKey(std::string &key, std::string &type);

	/// Get composite key and type.
	void GetCompositeKey(std::string &key, std::string &type);

	/// Get a column name map.
	void GetColumnKey(std::vector<DPair> &columnKey);

private:
	/// primary key.
	std::string m_primaryKey;

	/// primary key type.
	std::string m_primaryType;

	/// compoiste key.
	std::string m_compositeKey;

	/// composite key type.
	std::string m_compositeType;

	/// column vector.
	std::vector<DPair> m_columnVector;
};

#endif /* CREATETABLE_H_ */
