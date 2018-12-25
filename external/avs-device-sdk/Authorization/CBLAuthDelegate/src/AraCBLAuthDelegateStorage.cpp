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

#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include <AVSCommon/Utils/File/FileUtils.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/String/StringUtils.h>

#include "CBLAuthDelegate/AraCBLAuthDelegateStorage.h"

namespace alexaClientSDK {
namespace authorization {
namespace cblAuthDelegate {

using namespace avsCommon::utils::logger;
using namespace avsCommon::utils::string;
using namespace avsCommon::utils::file;
using namespace alexaClientSDK::storage::araStorage;

/// String to identify log entries originating from this file.
static const std::string TAG("AraCBLAuthDelegateStorage");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// Name of @c ConfigurationNode for CBLAuthDelegate
static const std::string CONFIG_KEY_CBL_AUTH_DELEGATE = "cblAuthDelegate";

/// Name of @c databaseFilePath value in in CBLAuthDelegate's @c ConfigurationNode.
static const std::string CONFIG_KEY_DB_FILE_PATH_KEY = "databaseFilePath";

/// The name of the refreshToken table.
#define REFRESH_TOKEN_TABLE_NAME "refreshToken"

/// The name of the index column
#define INDEX_COLUMN_NAME "ID"
/// The name of the key column
#define KEY_COLUMN_NAME "KEY"
/// The name of the refreshToken column.
#define REFRESH_TOKEN_COLUMN_NAME1 "VALUE1"
#define REFRESH_TOKEN_COLUMN_NAME2 "VALUE2"
#define REFRESH_TOKEN_COLUMN_NAME3 "VALUE3"

std::unique_ptr<AraCBLAuthDelegateStorage> AraCBLAuthDelegateStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot) {
	auto cblAuthDelegateConfigurationRoot = configurationRoot[CONFIG_KEY_CBL_AUTH_DELEGATE];
	if (!cblAuthDelegateConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed").d("reason", "missingConfigurationValue").d("key", CONFIG_KEY_CBL_AUTH_DELEGATE));
		return nullptr;
	}

	std::string databaseFilePath;
	if (!cblAuthDelegateConfigurationRoot.getString(CONFIG_KEY_DB_FILE_PATH_KEY, &databaseFilePath) ||
		databaseFilePath.empty()) {
		ACSDK_ERROR(LX("createFailed").d("reason", "missingConfigurationValue").d("key", CONFIG_KEY_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraCBLAuthDelegateStorage>(new AraCBLAuthDelegateStorage(databaseFilePath));
}

AraCBLAuthDelegateStorage::~AraCBLAuthDelegateStorage() {
	ACSDK_DEBUG5(LX("~AraCBLAuthDelegateStorage"));

	close();
}

bool AraCBLAuthDelegateStorage::createDatabase() {
	ACSDK_DEBUG5(LX("createDatabase"));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_database.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "DatabaseInitializeFailed"));
		return false;
	}

	// Create table
	CreateRequest request;
	request.SetTable(REFRESH_TOKEN_TABLE_NAME);
	request.SetPrimaryKey(INDEX_COLUMN_NAME, ATTR_TYPE_INT);
	request.SetCompositeKey(KEY_COLUMN_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(REFRESH_TOKEN_COLUMN_NAME1, ATTR_TYPE_STR(255));
	request.SetColumnKey(REFRESH_TOKEN_COLUMN_NAME2, ATTR_TYPE_STR(255));
	request.SetColumnKey(REFRESH_TOKEN_COLUMN_NAME3, ATTR_TYPE_STR(255));
	int ret = m_database.CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "failed to create refreshToken table"));
		close();
		return false;
	}

	return true;
}

bool AraCBLAuthDelegateStorage::open() {
	ACSDK_DEBUG5(LX("open"));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_database.open()) {
		ACSDK_DEBUG0(LX("openFailed").d("reason", "openSQLiteDatabaseFailed"));
		return false;
	}

	if (!m_database.tableExists(REFRESH_TOKEN_TABLE_NAME)) {
		ACSDK_ERROR(LX("openFailed").d("reason", "missingTable").d("name", REFRESH_TOKEN_TABLE_NAME));
		return false;
	}

	return true;
}

bool AraCBLAuthDelegateStorage::setRefreshToken(const std::string& refreshToken) {
	ACSDK_DEBUG5(LX("setRefreshToken"));
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (refreshToken.empty()) {
			ACSDK_ERROR(LX("setRefreshTokenFailed").d("reason", "refreshTokenIsEmpty"));
			return false;
		}

		if (m_refreshToken.compare(refreshToken) == 0) {
			ACSDK_DEBUG9(LX("setRefreshToken same token"));
			return true;
		}

		// Delete data
		DeleteRequest request;
		request.SetTable(REFRESH_TOKEN_TABLE_NAME);
		request.SetWhere(INDEX_COLUMN_NAME, 1);
		if (m_database.Delete(request) != 0) {
			ACSDK_ERROR(LX("setRefreshTokenFailed").d("reason", "clearTableFailed"));
			//return false; // maybe not exist, it's ok
		}

		ACSDK_DEBUG9(LX("setRefreshToken").d("tokenLength", refreshToken.length()));

		// Insert data
		InsertRequest request2;
		request2.SetTable(REFRESH_TOKEN_TABLE_NAME);
		request2.SetData(INDEX_COLUMN_NAME, 1);
		request2.SetData(KEY_COLUMN_NAME, (int)refreshToken.length());
		request2.SetData(REFRESH_TOKEN_COLUMN_NAME1, refreshToken.substr(0, 250));
		request2.SetData(REFRESH_TOKEN_COLUMN_NAME2, refreshToken.substr(250, 250));
		request2.SetData(REFRESH_TOKEN_COLUMN_NAME3, refreshToken.substr(500));
		if (m_database.Insert(request2) != 0) {
			ACSDK_ERROR(LX("setRefreshTokenFailed").d("reason", "failed to insert refreshToken string"));
			return false;
		}
	}

	// Verification
	{
		std::string readToken;
		if (!getRefreshToken(&readToken)) {
			ACSDK_ERROR(LX("setRefreshTokenFailed").d("reason", "getRefreshTokenFailed"));
			return false;
		}

		if (readToken.compare(refreshToken) != 0) {
			ACSDK_ERROR(LX("setRefreshTokenFailed").d("reason", "compareRefreshTokenFailed"));
			return false;
		}
	}

	m_refreshToken = refreshToken;
	return true;
}

bool AraCBLAuthDelegateStorage::clearRefreshToken() {
	ACSDK_DEBUG5(LX("clearRefreshToken"));

	std::lock_guard<std::mutex> lock(m_mutex);

	m_refreshToken.erase();

	if (!m_database.clearTable(REFRESH_TOKEN_TABLE_NAME)) {
		ACSDK_ERROR(LX("clearRefreshTokenFailed").d("reason", "clearTableFailed"));
		return false;
	}

	return true;
}

bool AraCBLAuthDelegateStorage::getRefreshToken(std::string* refreshToken) {
	ACSDK_DEBUG5(LX("getRefreshToken"));

	std::lock_guard<std::mutex> lock(m_mutex);

	if (!refreshToken) {
		ACSDK_ERROR(LX("getRefreshTokenFailed").d("reason", "nullRefreshToken"));
		return false;
	}

	m_refreshToken.erase();

	Cursor cursor;
	SelectRequest request;
	request.SetTable(REFRESH_TOKEN_TABLE_NAME);
	request.SetWhere(INDEX_COLUMN_NAME, 1);
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getRefreshTokenFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.GetCount() != 1) {
		ACSDK_ERROR(LX("getRefreshTokenFailed").d("reason", "selectResultError").d("cursorCount", cursor.GetCount()));
		return false;
	}

	cursor.MoveToFirst();

	int tokenLen = 0;
	if ((cursor.GetInt(KEY_COLUMN_NAME, tokenLen) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getRefreshTokenFailed").d("reason", "GetLengthFailed"));
		return false;
	}

	std::string strVal1;
	std::string strVal2;
	std::string strVal3;
	if ((cursor.GetString(REFRESH_TOKEN_COLUMN_NAME1, strVal1) != CURSOR_OK) ||
		(cursor.GetString(REFRESH_TOKEN_COLUMN_NAME2, strVal2) != CURSOR_OK) ||
		(cursor.GetString(REFRESH_TOKEN_COLUMN_NAME3, strVal3) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getRefreshTokenFailed").d("reason", "GetTokenFailed"));
		return false;
	}

	*refreshToken = strVal1.append(strVal2).append(strVal3);
	ACSDK_DEBUG9(LX("getRefreshToken").d("tokenLen", tokenLen).d("tokenLength", refreshToken->length()));

	m_refreshToken = *refreshToken;
	return true;
}

bool AraCBLAuthDelegateStorage::clear() {
	ACSDK_DEBUG5(LX("clear"));

	return clearRefreshToken();
}

void AraCBLAuthDelegateStorage::close() {
	ACSDK_DEBUG5(LX("close"));

	std::lock_guard<std::mutex> lock(m_mutex);

	m_database.close();
}

AraCBLAuthDelegateStorage::AraCBLAuthDelegateStorage(const std::string& databaseFilePath)
	   : m_database{databaseFilePath}
{
}

}  // namespace cblAuthDelegate
}  // namespace authorization
}  // namespace alexaClientSDK
