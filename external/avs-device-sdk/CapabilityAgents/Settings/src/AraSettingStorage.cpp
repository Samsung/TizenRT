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

#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/String/StringUtils.h>

#include "Settings/AraSettingStorage.h"
#include <assert.h>

namespace alexaClientSDK {
namespace capabilityAgents {
namespace settings {

using namespace avsCommon::utils::logger;
using namespace avsCommon::utils::string;
using namespace alexaClientSDK::storage::araStorage;

/// String to identify log entries originating from this file.
static const std::string TAG("AraSettingStorage");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The key in our config file to find the root of settings.
static const std::string SETTINGS_CONFIGURATION_ROOT_KEY = "settings";
/// The key in our config file to find the database file path.
static const std::string SETTINGS_DB_FILE_PATH_KEY = "databaseFilePath";

/// The name of the settings table.
static const std::string SETTINGS_TABLE_NAME = "settings";
/// The setting id.
static const std::string SETTING_ID = "id";
/// The setting key.
static const std::string SETTING_KEY = "key";
/// The setting value.
static const std::string SETTING_VALUE = "value";
std::unique_ptr<AraSettingStorage> AraSettingStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot) {
	auto settingsConfigurationRoot = configurationRoot[SETTINGS_CONFIGURATION_ROOT_KEY];
	if (!settingsConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config for the Settings capability agent")
						.d("key", SETTINGS_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string settingDbFilePath;
	if (!settingsConfigurationRoot.getString(SETTINGS_DB_FILE_PATH_KEY, &settingDbFilePath) ||
		settingDbFilePath.empty()) {
		ACSDK_ERROR(LX("createFailed").d("reason", "Could not load config value").d("key", SETTINGS_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraSettingStorage>(new AraSettingStorage(settingDbFilePath));
}

AraSettingStorage::AraSettingStorage(const std::string& databaseFilePath) : m_database{databaseFilePath} {
}

bool AraSettingStorage::createDatabase() {
	if (!m_database.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "SQLiteCreateDatabaseFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("createDatabase").m("start"));

	// Create table
	CreateRequest request;
	request.SetTable(SETTINGS_TABLE_NAME);
	request.SetPrimaryKey(SETTING_ID, ATTR_TYPE_INT);
	request.SetCompositeKey(SETTING_KEY, ATTR_TYPE_STR(255));
	request.SetColumnKey(SETTING_VALUE, ATTR_TYPE_STR(255));
	int ret = m_database.CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "CreateTableFailed"));
		close();
		return false;
	}

	ACSDK_DEBUG9(LX("createDatabase").m("success"));
	return true;
}

bool AraSettingStorage::open() {
	return m_database.open();
}

void AraSettingStorage::close() {
	m_database.close();
}

bool AraSettingStorage::settingExists(const std::string& key) {
	SelectRequest request;
	request.SetTable(SETTINGS_TABLE_NAME);
	request.SetWhere(SETTING_ID, (int)STR_HASH(key));
	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("settingExistsFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			// local values which we will use to capture what we read from the database.
			std::string _key;
			if ((cursor.GetString(SETTING_KEY, _key) != CURSOR_OK)) {
				ACSDK_ERROR(LX("settingExistsFailed").d("reason", "settingKeyFailed"));
				continue;
			}

			if (_key.compare(key) != 0) {
				ACSDK_DEBUG9(LX("settingExists").m("same hash code but diffrenct string!!!!").d("_key", _key).d("key", key));
				continue;
			}

			assert(cursor.GetCount() == 1); // there should not be key confliction
			return true;
		} while (cursor.MoveToNext());
	}

	return false;
}

bool AraSettingStorage::store(const std::string& key, const std::string& value) {
	if (value.empty()) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "SettingValueisEmpty"));
		return false;
	}

	if (settingExists(key)) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "SettingAlreadyExists").d("key", key));
		return false;
	}

	// Insert data
	InsertRequest request;
	request.SetTable(SETTINGS_TABLE_NAME);
	request.SetData(SETTING_ID, (int)STR_HASH(key));
	request.SetData(SETTING_KEY, key);
	request.SetData(SETTING_VALUE, value);
	if (m_database.Insert(request) != 0) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "InsertFailed"));
		return false;
	}

	return true;
}

bool AraSettingStorage::load(std::unordered_map<std::string, std::string>* mapOfSettings) {
	SelectRequest request;
	request.SetTable(SETTINGS_TABLE_NAME);
	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("loadFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			// local values which we will use to capture what we read from the database.
			std::string key;
			std::string value;
			if ((cursor.GetString(SETTING_KEY, key) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadFailed").d("reason", "getKeyFailed"));
				break;
			}

			if ((cursor.GetString(SETTING_VALUE, value) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadFailed").d("reason", "getValueFailed"));
				break;
			}

			ACSDK_DEBUG9(LX("load").d(SETTING_KEY, key).d(SETTING_VALUE, value));

			mapOfSettings->insert(make_pair(key, value));
		} while (cursor.MoveToNext());
	}

	return true;
}

bool AraSettingStorage::modify(const std::string& key, const std::string& value) {
	if (value.empty()) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "SettingValueisEmpty"));
		return false;
	}

	if (!settingExists(key)) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "SettingDoesNotExistInDatabase").d("key", key));
		return false;
	}

	// Delete
	DeleteRequest delreq;
	delreq.SetTable(SETTINGS_TABLE_NAME);
	delreq.SetWhere(SETTING_ID, (int)STR_HASH(key));
	if (m_database.Delete(delreq) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "DeleteFailed"));
		return false;
	}

	// Insert
	InsertRequest insreq;
	insreq.SetTable(SETTINGS_TABLE_NAME);
	insreq.SetData(SETTING_ID, (int)STR_HASH(key));
	insreq.SetData(SETTING_KEY, key);
	insreq.SetData(SETTING_VALUE, value);
	if (m_database.Insert(insreq) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "InsertFailed"));
		return false;
	}

	return true;
}

bool AraSettingStorage::erase(const std::string& key) {
	if (key.empty()) {
		ACSDK_ERROR(LX("eraseFailed").d("reason", "SettingKeyEmpty"));
		return false;
	}

	if (!settingExists(key)) {
		ACSDK_ERROR(LX("eraseFailed").d("reason", "SettingDoesNotExistInDatabase").d("key", key));
		return false;
	}

	// Delete
	DeleteRequest delreq;
	delreq.SetTable(SETTINGS_TABLE_NAME);
	delreq.SetWhere(SETTING_ID, (int)STR_HASH(key));
	if (m_database.Delete(delreq) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

bool AraSettingStorage::clearDatabase() {
	if (!m_database.clearTable(SETTINGS_TABLE_NAME)) {
		ACSDK_ERROR(LX("clearDatabaseFailed").d("reason", "SqliteClearTableFailed"));
		return false;
	}
	return true;
}

AraSettingStorage::~AraSettingStorage() {
	close();
}
}  // namespace settings
}  // namespace capabilityAgents
}  // namespace alexaClientSDK
