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

#include <algorithm>
#include <cctype>
#include <assert.h>

#include <AraStorage/AraMiscStorage.h>
#include <AVSCommon/Utils/Logger/Logger.h>

namespace alexaClientSDK {
namespace storage {
namespace araStorage {

using namespace avsCommon::utils::configuration;
using namespace avsCommon::utils::logger;

/// String to identify log entries originating from this file.
static const std::string TAG("AraMiscStorage");

/// Names of the columns in the database
static const std::string KEY_COLUMN_NAME = "key";
static const std::string VALUE_COLUMN_NAME = "value";
static const std::string VALUE_COLUMN_NAME2 = "v2";
static const std::string VALUE_COLUMN_NAME3 = "v3";
static const std::string VALUE_COLUMN_NAME4 = "v4";

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The key in our config file to find the root of settings for this database.
static const std::string MISC_DATABASE_CONFIGURATION_ROOT_KEY = "miscDatabase";
/// The key in our config file to find the database file path.
static const std::string MISC_DATABASE_DB_FILE_PATH_KEY = "databaseFilePath";
/// Component and table name separator in DB table name.
static const std::string MISC_DATABASE_DB_COMPONENT_TABLE_NAMES_SEPARATOR = "_";

/// String key/value type
static const std::string STRING_KEY_VALUE_TYPE = "STRING";
/// Unknown key/value type
static const std::string UNKNOWN_KEY_VALUE_TYPE = "UNKNOWN";

/// DB type
static const std::string TEXT_DB_TYPE = "TEXT";

/// Boolean to check if table exists
static const bool CHECK_TABLE_EXISTS = true;
/// Boolean to check if table doesn't exist
static const bool CHECK_TABLE_NOT_EXISTS = false;

/**
 * Helper method that will check basic things about the DB.
 *
 * @param db The Database handle for MiscDB
 * @param componentName The component name.
 * @param tableName The table name to check.
 * @return an error message if the checks fail, else a blank string
 */
static std::string basicDBChecks(Database& db, const std::string& componentName, const std::string& tableName);

/**
 * Helper method that will check basic things about the DB.
 *
 * @param db The Database handle for MiscDB
 * @param componentName The component name.
 * @param tableName The table name to check.
 * @param tableShouldExist If true, checks if the table should exist. If false, it checks the opposite.
 * @return an error message if the checks fail, else a blank string
 */
static std::string basicDBChecks(
	Database& db,
	const std::string& componentName,
	const std::string& tableName,
	bool tableShouldExist);

/**
 * Helper method that will get the table name as it is in the DB.
 * @param componentName The component name.
 * @param tableName The table name to check
 *
 * @return the table name as it is in the DB, else a blank string
 */
static std::string getDBTableName(const std::string& componentName, const std::string& tableName);

std::unique_ptr<AraMiscStorage> AraMiscStorage::create(const ConfigurationNode& configurationRoot) {
	auto miscDatabaseConfigurationRoot = configurationRoot[MISC_DATABASE_CONFIGURATION_ROOT_KEY];
	if (!miscDatabaseConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config for misc database")
						.d("key", MISC_DATABASE_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string miscDbFilePath;
	if (!miscDatabaseConfigurationRoot.getString(MISC_DATABASE_DB_FILE_PATH_KEY, &miscDbFilePath) ||
		miscDbFilePath.empty()) {
		ACSDK_ERROR(
			LX("createFailed").d("reason", "Could not load config value").d("key", MISC_DATABASE_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraMiscStorage>(new AraMiscStorage(miscDbFilePath));
}

AraMiscStorage::AraMiscStorage(const std::string& dbFilePath) : m_db{dbFilePath} {
}

AraMiscStorage::~AraMiscStorage() {
	close();
}

bool AraMiscStorage::open() {
	if (!m_db.open()) {
		ACSDK_DEBUG0(LX("openDatabaseFailed"));
		return false;
	}

	return true;
}

void AraMiscStorage::close() {
	m_db.close();
}

bool AraMiscStorage::createDatabase() {
	if (!m_db.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed"));
		return false;
	}

	return true;
}

std::string getDBTableName(const std::string& componentName, const std::string& tableName) {
	if (componentName.empty() || tableName.empty()) {
		std::string emptyParam;
		if (componentName.empty() && tableName.empty()) {
			emptyParam = "Component and table";
		} else {
			if (componentName.empty()) {
				emptyParam = "Component";
			} else {
				emptyParam = "Table";
			}
		}
		ACSDK_ERROR(LX("getDBTableNameError").d("reason", emptyParam + " name can't be empty."));
		return "";
	}

	return (componentName + MISC_DATABASE_DB_COMPONENT_TABLE_NAMES_SEPARATOR + tableName);
}

std::string basicDBChecks(Database& db, const std::string& componentName, const std::string& tableName) {
	if (!db.isDatabaseReady()) {
		return "Database is not ready";
	}

	if (componentName.empty()) {
		return "Component name is empty";
	}

	if (tableName.empty()) {
		return "Table name is empty";
	}

	return "";
}

std::string basicDBChecks(
	Database& db,
	const std::string& componentName,
	const std::string& tableName,
	bool tableShouldExist) {
	const std::string errorReason = basicDBChecks(db, componentName, tableName);
	if (!errorReason.empty()) {
		return errorReason;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);
	bool tableExists = db.tableExists(dbTableName);
	if (tableShouldExist && !tableExists) {
		return "Table does not exist";
	}
	if (!tableShouldExist && tableExists) {
		return "Table already exists";
	}

	return "";
}

bool AraMiscStorage::createTable(
	const std::string& componentName,
	const std::string& tableName,
	KeyType keyType,
	ValueType valueType) {
	const std::string errorEvent = "createTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_NOT_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	if (keyType == KeyType::UNKNOWN_KEY) {
		ACSDK_ERROR(LX(errorEvent).m("Unknown key type"));
		return false;
	}

	if (valueType == ValueType::UNKNOWN_VALUE) {
		ACSDK_ERROR(LX(errorEvent).m("Unknown value type"));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	CreateRequest request;
	request.SetTable(dbTableName);
	request.SetPrimaryKey("key_hash", ATTR_TYPE_INT);
	request.SetCompositeKey(KEY_COLUMN_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey("value_len", ATTR_TYPE_INT);
	request.SetColumnKey(VALUE_COLUMN_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey(VALUE_COLUMN_NAME2, ATTR_TYPE_STR(255));
	request.SetColumnKey(VALUE_COLUMN_NAME3, ATTR_TYPE_STR(255));
	request.SetColumnKey(VALUE_COLUMN_NAME4, ATTR_TYPE_STR(255));
	if (m_db.CreateTable(request) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("Could not create table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::clearTable(const std::string& componentName, const std::string& tableName) {
	const std::string errorEvent = "clearTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	if (!m_db.clearTable(dbTableName)) {
		ACSDK_ERROR(LX(errorEvent).d("Could not clear table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::deleteTable(const std::string& componentName, const std::string& tableName) {
	const std::string errorEvent = "deleteTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	int numOfTableEntries = 0;
	if (!m_db.getNumberTableRows(dbTableName, &numOfTableEntries)) {
		ACSDK_ERROR(LX(errorEvent).m("Failed to count rows in table"));
		return false;
	}

	if (numOfTableEntries > 0) {
		ACSDK_ERROR(LX(errorEvent).m("Unable to delete table that is not empty"));
		return false;
	}

	if (!m_db.dropTable(dbTableName)) {
		ACSDK_ERROR(LX(errorEvent).d("Could not delete table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::get(
	const std::string& componentName,
	const std::string& tableName,
	const std::string& key,
	std::string* value) {
	const std::string errorEvent = "getFromTableFailed";

	if (!value) {
		ACSDK_ERROR(LX(errorEvent).m("Value is nullptr."));
		return false;
	}

	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);
	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	SelectRequest request;
	request.SetTable(dbTableName);
	request.SetWhere("key_hash", (int)STR_HASH(key));
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("Could not get value for " + key + " from table", dbTableName));
		return false;
	}


	if (cursor.MoveToFirst()) {
		if (cursor.GetString(VALUE_COLUMN_NAME, *value) != CURSOR_OK) {
			ACSDK_ERROR(LX(errorEvent).m("getValueStringFailed"));
			return false;
		}

		std::string substr2 = "";
		std::string substr3 = "";
		std::string substr4 = "";
		cursor.GetString(VALUE_COLUMN_NAME2, substr2);
		cursor.GetString(VALUE_COLUMN_NAME3, substr3);
		cursor.GetString(VALUE_COLUMN_NAME4, substr4);
		value->append(substr2).append(substr3).append(substr4);

		int vlen = 0;
		cursor.GetInt("value_len", vlen);
		if (vlen != (int)value->size()) {
			ACSDK_ERROR(LX(errorEvent).d("value_len", vlen).d("actual_len", value->size()).d("text", *value));
			return false;
		}
	} else {
		ACSDK_ERROR(LX(errorEvent).d("Could not get value for " + key + " from table", dbTableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::tableEntryExists(
	const std::string& componentName,
	const std::string& tableName,
	const std::string& key,
	bool* tableEntryExistsValue) {
	const std::string errorEvent = "tableEntryExistsFailed";

	if (!tableEntryExistsValue) {
		ACSDK_ERROR(LX(errorEvent).m("tableEntryExistsValue is nullptr."));
		return false;
	}

	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);
	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	std::string tableEntry;
	if (!get(componentName, tableName, key, &tableEntry)) {
		ACSDK_WARN(LX(errorEvent).m("Unable to get table entry"));
		//return false;
	}

	*tableEntryExistsValue = !tableEntry.empty();
	return true;
}

bool AraMiscStorage::tableExists(
	const std::string& componentName,
	const std::string& tableName,
	bool* tableExistsValue) {
	const std::string errorEvent = "tableExistsFailed";

	if (!tableExistsValue) {
		ACSDK_ERROR(LX(errorEvent).m("tableExistsValue is nullptr."));
		return false;
	}

	const std::string errorReason = basicDBChecks(m_db, componentName, tableName);
	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);
	*tableExistsValue = m_db.tableExists(dbTableName);
	return true;
}

bool AraMiscStorage::add(
	const std::string& componentName,
	const std::string& tableName,
	const std::string& key,
	const std::string& value) {
	const std::string errorEvent = "addToTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	bool tableEntryExistsValue;
	if (!tableEntryExists(componentName, tableName, key, &tableEntryExistsValue)) {
		ACSDK_ERROR(LX(errorEvent).d("Unable to get table entry information for " + key + " in table", tableName));
		return false;
	}

	if (tableEntryExistsValue) {
		ACSDK_ERROR(LX(errorEvent).d("An entry already exists for " + key + " in table", tableName));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	InsertRequest request;
	request.SetTable(dbTableName);
	request.SetData("key_hash", (int)STR_HASH(key));
	request.SetData(KEY_COLUMN_NAME, key);

	auto vlen = value.size();
	request.SetData("value_len", (int)vlen);
	request.SetData(VALUE_COLUMN_NAME, value.substr(0, 250));

	std::string substr2 = "";
	std::string substr3 = "";
	std::string substr4 = "";

	if (vlen > 250) {
		substr2 = value.substr(250, 250);
		if (vlen > 500) {
			substr3 = value.substr(500, 250);
			if (vlen > 750) {
				substr4 = value.substr(750, 250);
			}
			assert(vlen <= 1000);
		}
	}

	request.SetData(VALUE_COLUMN_NAME2, substr2);
	request.SetData(VALUE_COLUMN_NAME3, substr3);
	request.SetData(VALUE_COLUMN_NAME4, substr4);

	if (m_db.Insert(request) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("Could not add entry (" + key + ", " + value + ") to table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::update(
	const std::string& componentName,
	const std::string& tableName,
	const std::string& key,
	const std::string& value) {
	const std::string errorEvent = "updateTableEntryFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	bool tableEntryExistsValue;
	if (!tableEntryExists(componentName, tableName, key, &tableEntryExistsValue)) {
		ACSDK_ERROR(LX(errorEvent).d("Unable to get table entry information for " + key + " in table", tableName));
		return false;
	}
	if (!tableEntryExistsValue) {
		ACSDK_ERROR(LX(errorEvent).d("An entry does not exist for " + key + " in table", tableName));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	DeleteRequest delreq;
	delreq.SetTable(dbTableName);
	delreq.SetWhere("key_hash", (int)STR_HASH(key));
	if (m_db.Delete(delreq) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("reason", "DeleteFailed"));
		return false;
	}

	InsertRequest request;
	request.SetTable(dbTableName);
	request.SetData("key_hash", (int)STR_HASH(key));
	request.SetData(KEY_COLUMN_NAME, key);

	auto vlen = value.size();
	request.SetData("value_len", (int)vlen);
	request.SetData(VALUE_COLUMN_NAME, value.substr(0, 250));

	std::string substr2 = "";
	std::string substr3 = "";
	std::string substr4 = "";

	if (vlen > 250) {
		substr2 = value.substr(250, 250);
		if (vlen > 500) {
			substr3 = value.substr(500, 250);
			if (vlen > 750) {
				substr4 = value.substr(750, 250);
			}
			assert(vlen <= 1000);
		}
	}

	request.SetData(VALUE_COLUMN_NAME2, substr2);
	request.SetData(VALUE_COLUMN_NAME3, substr3);
	request.SetData(VALUE_COLUMN_NAME4, substr4);

	if (m_db.Insert(request) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("Could not update entry (" + key + ", " + value + ") to table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::put(
	const std::string& componentName,
	const std::string& tableName,
	const std::string& key,
	const std::string& value) {
	const std::string errorEvent = "putToTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	bool tableEntryExistsValue;
	if (!tableEntryExists(componentName, tableName, key, &tableEntryExistsValue)) {
		ACSDK_ERROR(LX(errorEvent).d("Unable to get table entry information for " + key + " in table", tableName));
		return false;
	}

	if (!tableEntryExistsValue) {
		return add(componentName, tableName, key, value);
	} else {
		return update(componentName, tableName, key, value);
	}
}

bool AraMiscStorage::remove(const std::string& componentName, const std::string& tableName, const std::string& key) {
	const std::string errorEvent = "removeTableEntryFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	bool tableEntryExistsValue;
	if (!tableEntryExists(componentName, tableName, key, &tableEntryExistsValue)) {
		ACSDK_ERROR(LX(errorEvent).d("Unable to get table entry information for " + key + " in table", tableName));
		return false;
	}
	if (!tableEntryExistsValue) {
		ACSDK_ERROR(LX(errorEvent).d("An entry does not exist for " + key + " in table", tableName));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	DeleteRequest request;
	request.SetTable(dbTableName);
	request.SetWhere("key_hash", (int)STR_HASH(key));
	if (m_db.Delete(request) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("Could not remove value for " + key + " from table", tableName));
		return false;
	}

	return true;
}

bool AraMiscStorage::load(
	const std::string& componentName,
	const std::string& tableName,
	std::unordered_map<std::string, std::string>* valueContainer) {
	const std::string errorEvent = "loadFromTableFailed";
	const std::string errorReason = basicDBChecks(m_db, componentName, tableName, CHECK_TABLE_EXISTS);

	if (!errorReason.empty()) {
		ACSDK_ERROR(LX(errorEvent).m(errorReason));
		return false;
	}

	if (!valueContainer) {
		ACSDK_ERROR(LX(errorEvent).m("Value container is nullptr."));
		return false;
	}

	std::string dbTableName = getDBTableName(componentName, tableName);

	SelectRequest request;
	request.SetTable(dbTableName);
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX(errorEvent).d("reason", "selectFailed"));
		return false;
	}


	if (cursor.MoveToFirst()) {
		do {
			std::string key, value;
			if (cursor.GetString(KEY_COLUMN_NAME, key) != CURSOR_OK) {
				ACSDK_ERROR(LX(errorEvent).d("reason", "getKeyFailed"));
				return false;
			}

			if (cursor.GetString(VALUE_COLUMN_NAME, value) != CURSOR_OK) {
				ACSDK_ERROR(LX(errorEvent).m("getValueFailed"));
				return false;
			}

			std::string substr2 = "";
			std::string substr3 = "";
			std::string substr4 = "";
			cursor.GetString(VALUE_COLUMN_NAME2, substr2);
			cursor.GetString(VALUE_COLUMN_NAME3, substr3);
			cursor.GetString(VALUE_COLUMN_NAME4, substr4);
			value.append(substr2).append(substr3).append(substr4);

			int vlen = 0;
			cursor.GetInt("value_len", vlen);
			if (vlen != (int)value.size()) {
				ACSDK_ERROR(LX(errorEvent).d("value_len", vlen).d("actual_len", value.size()).d("text", value));
				continue;
			}

			if (!(key.empty()) && !(value.empty())) {
				valueContainer->insert(std::make_pair(key, value));
			}
		} while (cursor.MoveToNext());
	}

	return true;
}

}  // namespace sqliteStorage
}  // namespace storage
}  // namespace alexaClientSDK
