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

#include "AraStorage/AqlGenerator.h"
#include "AraStorage/DatabaseMacros.h"
#include <AVSCommon/Utils/Logger/Logger.h>

using namespace std;

#define DB_CREATE "CREATE RELATION "
#define DB_CREATE_ATTR "CREATE ATTRIBUTE "
#define DB_DROP "REMOVE RELATION "
#define DB_INSERT "INSERT "
#define DB_SELECT "SELECT "
#define DB_DELETE "REMOVE FROM  "
#define DB_RM_INDEX "REMOVE INDEX "

static const std::string TAG("AqlGenerator");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

void AqlGenerator::GetSelectAql(SelectRequest &request, string &selectAql)
{
	string tableName;
	DMap whereMap;

	request.GetTable(tableName);
	request.GetWhereMap(whereMap);

	string aql(DB_SELECT);
	aql.append("ALL FROM "); //always select all column
	aql.append(tableName);

	if (whereMap.size() > 0) {
		aql.append(" WHERE ");

		for (auto ii = whereMap.begin(); ii != whereMap.end(); ++ii) {
			auto key = ii->first;
			aql.append(key);
			aql.append(" = ");

			auto value = ii->second;
			if (ATTR_IS_STRING(value)) {
				aql.append("'");
				aql.append(ATTR_TO_STRING(value));
				aql.append("'");
			} else {
				aql.append(ATTR_TO_STRING(value));
			}

			aql.append(" AND ");
		}

		int lastIdx = aql.rfind(" AND");
		aql = aql.substr(0, lastIdx);
	}
	aql.append(";");
	selectAql = aql;
	ACSDK_DEBUG9(LX("GetSelectAql").d("AQL", aql));
}

void AqlGenerator::GetCheckTableAql(const string &tableName, string& checkTableAql)
{
	string aql(DB_SELECT);
	aql.append("ALL FROM "); //always select all column
	aql.append(tableName);
	aql.append(";");

	checkTableAql = aql;
	ACSDK_DEBUG9(LX("GetCheckTableAql").d("AQL", aql));
}

void AqlGenerator::GetCreateTableAql(CreateRequest &request, string &createAql)
{
	string tableName;
	request.GetTable(tableName);

	string aql(DB_CREATE);
	aql.append(tableName);
	aql.append(";");

	createAql = aql;
	ACSDK_DEBUG9(LX("GetCreateTableAql").d("AQL", aql));
}

void AqlGenerator::GetCreateAttrAql(CreateRequest &request, vector<string> &createAttrAql)
{
	string tableName;
	string primaryKey;
	string primaryType;
	string compositeKey;
	string compositeType;
	vector<DPair> columnVector;

	request.GetTable(tableName);
	request.GetPrimaryKey(primaryKey, primaryType);
	request.GetCompositeKey(compositeKey, compositeType);
	request.GetColumnKey(columnVector);

	if (!primaryKey.empty() && !primaryType.empty()) {
		string aql(DB_CREATE_ATTR);
		aql.append(primaryKey);
		aql.append(" DOMAIN ");
		aql.append(primaryType);
		aql.append(" IN ");
		aql.append(tableName);
		aql.append(";");

		createAttrAql.push_back(aql);
		ACSDK_DEBUG9(LX("GetCreateAttrAql column 1").d("AQL", aql));
	}

	if (!compositeKey.empty() && !compositeType.empty()) {
		string aql(DB_CREATE_ATTR);
		aql.append(compositeKey);
		aql.append(" DOMAIN ");
		aql.append(compositeType);
		aql.append(" IN ");
		aql.append(tableName);
		aql.append(";");

		createAttrAql.push_back(aql);
		ACSDK_DEBUG9(LX("GetCreateAttrAql column 2").d("AQL", aql));
	}

	for (unsigned int i = 0; i < columnVector.size(); i++) {
		string aql(DB_CREATE_ATTR);
		aql.append(columnVector[i].first);
		aql.append(" DOMAIN ");
		aql.append(columnVector[i].second);
		aql.append(" IN ");
		aql.append(tableName);
		aql.append(";");

		createAttrAql.push_back(aql);
		ACSDK_DEBUG9(LX("GetCreateAttrAql column").d(" ", 3+i).d("AQL", aql));
	}
}

void AqlGenerator::GetRemoveIndexAql(InsertRequest &request, vector<string> &rmIndexAql)
{
	string tableName;
	DMap columnMap;
	request.GetTable(tableName);
	request.GetDataMap(columnMap);

	for (auto ii = columnMap.begin(); ii != columnMap.end(); ++ii) {
		auto key = ii->first;
		string aql(DB_RM_INDEX);
		aql.append(tableName);
		aql.append(".");
		aql.append(key);
		aql.append(";");

		rmIndexAql.push_back(aql);
		ACSDK_DEBUG9(LX("GetRemoveIndexAql").d("statement", aql));
	}
}

void AqlGenerator::GetDeleteAql(DeleteRequest &request, string &deleteAql)
{
	string tableName;
	DMap whereMap;

	request.GetTable(tableName);
	request.GetWhereMap(whereMap);

	string aql(DB_DELETE);
	aql.append(tableName);
	if (whereMap.size() > 0) {
		aql.append(" WHERE ");

		for (auto ii = whereMap.begin(); ii != whereMap.end(); ++ii) {
			auto key = ii->first;
			aql.append(key);
			aql.append(" = ");

			auto value = ii->second;
			if (ATTR_IS_STRING(value)) {
				aql.append("'");
				aql.append(ATTR_TO_STRING(value));
				aql.append("'");
			} else {
				aql.append(ATTR_TO_STRING(value));
			}

			aql.append(" AND ");
		}

		int lastIdx = aql.rfind(" AND");
		aql = aql.substr(0, lastIdx);
	}
	aql.append(";");

	deleteAql = aql;
	ACSDK_DEBUG9(LX("GetDeleteAql").d("AQL", aql));
}

void AqlGenerator::GetInsertAql(InsertRequest &request, string &insertAql)
{
	insertAql.clear();

	string tableName;
	DMap columnMap;
	request.GetTable(tableName);
	request.GetDataMap(columnMap);

	string aql(DB_INSERT);
	aql.append("(");

	for (auto ii = columnMap.begin(); ii != columnMap.end(); ++ii) {
		auto value = ii->second;
		if (ATTR_IS_STRING(value)) {
			aql.append("'");
			aql.append(ATTR_TO_STRING(value));
			aql.append("'");
		} else {
			aql.append(ATTR_TO_STRING(value));
		}
		aql.append(", ");
	}
	int lastIdx = aql.rfind(", ");
	aql = aql.substr(0, lastIdx);

	aql.append(") INTO ");
	aql.append(tableName);
	aql.append(";");
	insertAql = aql;
	ACSDK_DEBUG9(LX("GetInsertAql").d("AQL", aql));
}

void AqlGenerator::GetDropTableAql(DropRequest &request, string &dropAql)
{
	string tableName;
	request.GetTable(tableName);

	string aql(DB_DROP);
	aql.append(tableName);
	aql.append(";");

	dropAql = aql;
	ACSDK_DEBUG9(LX("GetDropTableAql").d("AQL", aql));
}
