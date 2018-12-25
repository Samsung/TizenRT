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

#include "AraStorage/Database.h"
#include "AraStorage/AqlGenerator.h"
#include "AraStorage/DatabaseParser.h"

#include <AVSCommon/Utils/Logger/Logger.h>
#include <stdlib.h>
#include <unistd.h>

namespace alexaClientSDK {
namespace storage {
namespace araStorage {

using namespace std;

static const std::string TAG("Database");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

bool Database::m_isInitialized = false;
bool Database::m_globalTableExist = false;

std::mutex Database::m_opLock;
static const std::string globalTable = "global_tab";

Database::Database(const std::string filePath)
	: m_filePath(filePath), m_state(0)
{
	// The underlying arastorage do not need a file path.
	// But we store the the path in a global table.
}

Database::~Database()
{
}

bool Database::initialize()
{
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!db_initialize()) {
		ACSDK_ERROR(LX(__FUNCTION__).m("db_initializeFailed"));
		return false;
	}

	// Check filepath exists
	{
		SelectRequest request;
		request.SetTable(globalTable);
		request.SetWhere("id", (int)STR_HASH(m_filePath));

		Cursor cursor;
		if (Select(request, cursor) != 0) {
			ACSDK_ERROR(LX("initializeFailed").d("reason", "selectFailed"));
			return false;
		}

		if (cursor.GetCount() > 0) {
			ACSDK_DEBUG9(LX(__FUNCTION__).m("DatabaseAlreadyExisted"));
			m_state = 1;
			return true;
		}
	}

	{   // Insert filepath
		InsertRequest upsert;
		upsert.SetTable(globalTable);
		upsert.SetData("id", (int)STR_HASH(m_filePath));
		upsert.SetData("filepath", m_filePath);
		if (Insert(upsert) != 0) {
			ACSDK_ERROR(LX("initializeFailed").d("reason", "InsertFailed"));
			return false;
		}
		ACSDK_DEBUG9(LX(__FUNCTION__).m("GlobalTableInserted"));
	}

	m_state = 1;
	return true;
}

bool Database::open()
{
	db_initialize();

	// Check filepath exists
	SelectRequest request;
	request.SetTable(globalTable);
	request.SetWhere("id", (int)STR_HASH(m_filePath));

	Cursor cursor;
	if (Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("openFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.GetCount() > 0) {
		ACSDK_DEBUG9(LX("openFailed").m("DatabaseFilepathFound"));
		m_state = 1;
		return true;
	}

	return false;
}

void Database::close()
{
	m_state = 0;
}

int Database::CreateTable(CreateRequest &request)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetCreateTableAql(request, aql);

	db_result_t ret = DBExecute((char*)aql.c_str());
	if (DB_OK != ret) {
		ACSDK_ERROR(LX("CreateTable").d("CreateTableFailed", (int)ret).d("AQL", aql));
		return -1;
	}

	int result = 0;
	vector<string> attr;
	aqlGenerator.GetCreateAttrAql(request, attr);
	for(vector<string>::iterator it = attr.begin(); it != attr.end(); it++) {
		db_result_t ret = DBExecute((char*)it->c_str());
		if (DB_OK != ret) {
			ACSDK_ERROR(LX("CreateTable").d("CreateAttrFailed", (int)ret).d("AQL", *it));
			result = -1;
		}
	}

	return result;
}

int Database::Delete(DeleteRequest &request)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetDeleteAql(request, aql);

	db_cursor_t* cursor = DBQuery((char*)aql.c_str());
	if (cursor == NULL) {
		ACSDK_DEBUG9(LX("Delete").d("DBQuery", "NoContent").d("AQL", aql));
		return 0;
	}

	return 0;
}

int Database::Select(SelectRequest &request, Cursor &cursor)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetSelectAql(request, aql);

	db_cursor_t* ret = DBQuery((char*)aql.c_str());
	if (ret == NULL) {
		ACSDK_ERROR(LX("SelectFailed").d("DBQuery", "Failed").d("AQL", aql));
		return -1;
	}

	DatabaseParser dbParser;
	dbParser.ParseDatabase(ret, cursor);

	ACSDK_DEBUG9(LX(__FUNCTION__).d("rows", cursor.GetCount()));
	return 0;
}

int Database::Insert(InsertRequest &request)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetInsertAql(request, aql);

	db_result_t ret = DBExecute((char*)aql.c_str());
	if (DB_OK != ret) {
		ACSDK_ERROR(LX("Insert").d("DBExecuteFailed", (int)ret).d("AQL", aql));
		return -1;
	}

	return 0;
}

int Database::DropTable(DropRequest &request)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetDropTableAql(request, aql);

	db_result_t ret = DBExecute((char*)aql.c_str());
	if (DB_OK != ret) {
		ACSDK_ERROR(LX("DropTable").d("DBExecuteFailed", (int)ret).d("AQL", aql));
		return -1;
	}

	return 0;
}

bool Database::getNumberTableRows(const std::string &tableName, int *numberRows)
{
	if (!numberRows) {
		ACSDK_ERROR(LX("getNumberTableRowsFailed").m("numberRows was nullptr."));
		return false;
	}

	Cursor cursor;
	SelectRequest request;
	request.SetTable(tableName);
	if (Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getNumberTableRowsFailed").d("reason", "selectFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("getNumberTableRows").d("count", cursor.GetCount()));
	*numberRows = cursor.GetCount();
	return true;
}

bool Database::getTableMaxIntValue(const std::string &tableName, const std::string & columnName, int *maxInt)
{
	if (!maxInt) {
		ACSDK_ERROR(LX("getMaxIntFailed").m("maxInt was nullptr."));
		return false;
	}

	Cursor cursor;
	SelectRequest request;
	request.SetTable(tableName);
	if (Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getMaxIntFailed").d("reason", "selectFailed"));
		return false;
	}

	*maxInt = 0;

	if (cursor.MoveToFirst()) {
		do {
			std::string value;
			if ((cursor.GetString(columnName, value) != CURSOR_OK)) {
				ACSDK_ERROR(LX("getMaxIntFailed").d("reason", "cursorGetStringFailed"));
				continue;
			}

			int int_value = std::stoi(value);
			if (*maxInt < int_value) {
				*maxInt = int_value;
			}
		} while (cursor.MoveToNext());
	}

	ACSDK_DEBUG9(LX("getTableMaxIntValue").d("maxInt", *maxInt));
	return true;
}

bool Database::tableExists(const std::string &tableName)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	std::unique_lock<std::mutex> lock(m_opLock);

	string aql;
	AqlGenerator aqlGenerator;
	aqlGenerator.GetCheckTableAql(tableName, aql);

	db_cursor_t *result = db_query((char *)aql.c_str());
	ACSDK_DEBUG9(LX(__FUNCTION__).d("result", (result != NULL)));
	return (result != NULL);
}

bool Database::clearTable(const std::string &tableName)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	DeleteRequest request;
	request.SetTable(tableName);
	if (Delete(request) != 0) {
		ACSDK_DEBUG9(LX("clearTable").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

bool Database::dropTable(const std::string &tableName)
{
	ACSDK_DEBUG9(LX(__FUNCTION__));
	DropRequest request;
	request.SetTable(tableName);
	return (DropTable(request) == 0);
}

bool Database::isDatabaseReady()
{
	return (m_isInitialized && (m_state != 0));
}

bool Database::db_initialize()
{
	{
		std::unique_lock<std::mutex> lock(m_opLock);
		if (!m_isInitialized) {
			ACSDK_DEBUG9(LX(__FUNCTION__).m("db_init to be called!"));
			db_result_t ret = db_init();
			if (DB_OK != ret) {
				ACSDK_ERROR(LX(__FUNCTION__).d("db_initFailed", (int)ret));
				return false;
			}
			m_isInitialized = true;
		}
	}

	// A global table to store database filepath.
	// If filepath exists in the table we think the database exists.
	// Otherwise, it does not exist, and need to initialize.
	if (!m_globalTableExist) {
		if (!tableExists(globalTable)) {
			ACSDK_DEBUG9(LX(__FUNCTION__).m("GlobalTableNotExist"));
			CreateRequest request;
			request.SetTable(globalTable);
			request.SetPrimaryKey("id", ATTR_TYPE_INT);
			request.SetCompositeKey("filepath", ATTR_TYPE_STR(255));
			if (CreateTable(request) != 0) {
				ACSDK_ERROR(LX(__FUNCTION__).m("CreateTableFailed"));
				return false;
			}
			ACSDK_DEBUG9(LX(__FUNCTION__).m("GlobalTableCreated"));
		}
		m_globalTableExist = true;
	}

	return true;
}

bool Database::IsRetryLimit(int &retryCount)
{
	if (++retryCount > DM_RETRY_LIMIT) {
		ACSDK_WARN(LX("IsRetryLimit").d("retryCount", retryCount).d("limit", DM_RETRY_LIMIT));
		return true;
	}

	ACSDK_DEBUG9(LX("IsRetryLimit").d("retryCount", retryCount).d("usleep", DM_RETRY_INTERVAL));
	usleep(DM_RETRY_INTERVAL);
	return false;
}

db_result_t Database::DBExecute(char *cmd)
{
	db_result_t result;
	int retryCount = 0;
	do {
		result = db_exec(cmd);
	} while (result != DB_OK && !IsRetryLimit(retryCount));

	return result;
}

db_cursor_t* Database::DBQuery(char *cmd)
{
	db_cursor_t* result;
	int retryCount = 0;
	do {
		result = db_query(cmd);
	} while (result == NULL && !IsRetryLimit(retryCount));

	return result;
}

}
}
}
