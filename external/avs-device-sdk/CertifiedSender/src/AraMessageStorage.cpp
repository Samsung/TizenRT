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

#include "CertifiedSender/AraMessageStorage.h"

#include <AVSCommon/Utils/File/FileUtils.h>
#include <AVSCommon/Utils/Logger/Logger.h>

#include <fstream>

namespace alexaClientSDK {
namespace certifiedSender {

using namespace avsCommon::utils::file;
using namespace avsCommon::utils::logger;
using namespace alexaClientSDK::storage::araStorage;

/// String to identify log entries originating from this file.
static const std::string TAG("AraMessageStorage");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The key in our config file to find the root of settings for this Capability Agent.
static const std::string CERTIFIED_SENDER_CONFIGURATION_ROOT_KEY = "certifiedSender";
/// The key in our config file to find the database file path.
static const std::string CERTIFIED_SENDER_DB_FILE_PATH_KEY = "databaseFilePath";

/// The name of the alerts table.
static const std::string MESSAGES_TABLE_NAME = "messages";
/// The name of the 'id' field we will use as the primary key in our tables.
static const std::string DATABASE_COLUMN_ID_NAME = "id";
/// The name of the 'id' field we will use as the primary key in our tables.
static const std::string DATABASE_COLUMN_MESSAGE_TEXT_NAME = "message_text";

std::unique_ptr<AraMessageStorage> AraMessageStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot) {
	auto certifiedSenderConfigurationRoot = configurationRoot[CERTIFIED_SENDER_CONFIGURATION_ROOT_KEY];
	if (!certifiedSenderConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config for the Message Storage database")
						.d("key", CERTIFIED_SENDER_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string certifiedSenderDatabaseFilePath;
	if (!certifiedSenderConfigurationRoot.getString(
			CERTIFIED_SENDER_DB_FILE_PATH_KEY, &certifiedSenderDatabaseFilePath) ||
		certifiedSenderDatabaseFilePath.empty()) {
		ACSDK_ERROR(
			LX("createFailed").d("reason", "Could not load config value").d("key", CERTIFIED_SENDER_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraMessageStorage>(new AraMessageStorage(certifiedSenderDatabaseFilePath));
}

AraMessageStorage::AraMessageStorage(const std::string& certifiedSenderDatabaseFilePath) :
		m_database(certifiedSenderDatabaseFilePath) {
}

AraMessageStorage::~AraMessageStorage() {
	close();
}

bool AraMessageStorage::createDatabase() {
	if (!m_database.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("createDatabase").m("start"));

	// Create table
	CreateRequest request;
	request.SetTable(MESSAGES_TABLE_NAME);
	request.SetPrimaryKey(DATABASE_COLUMN_ID_NAME, ATTR_TYPE_INT);
	request.SetCompositeKey(DATABASE_COLUMN_MESSAGE_TEXT_NAME, ATTR_TYPE_STR(255));
	int ret = m_database.CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "CreateTableFailed"));
		close();
		return false;
	}

	ACSDK_DEBUG9(LX("createDatabase").m("success"));
	return true;
}

bool AraMessageStorage::open() {
	return m_database.open();
}

void AraMessageStorage::close() {
	m_database.close();
}

bool AraMessageStorage::store(const std::string& message, int* id) {
	if (!id) {
		ACSDK_ERROR(LX("storeFailed").m("id parameter was nullptr."));
		return false;
	}

	//std::string sqlString = std::string("INSERT INTO " + MESSAGES_TABLE_NAME + " (") + DATABASE_COLUMN_ID_NAME + ", " +
	//                        DATABASE_COLUMN_MESSAGE_TEXT_NAME + ") VALUES (" + "?, ?" + ");";

	int nextId = 0;
	if (!m_database.getTableMaxIntValue(MESSAGES_TABLE_NAME, DATABASE_COLUMN_ID_NAME, &nextId)) {
		ACSDK_ERROR(LX("storeFailed").m("Cannot generate message id."));
		return false;
	}
	nextId++;

	if (nextId <= 0) {
		ACSDK_ERROR(LX("storeFailed").m("Invalid computed row id.  Possible numerical overflow.").d("id", nextId));
		return false;
	}

	// Insert data
	InsertRequest request;
	request.SetTable(MESSAGES_TABLE_NAME);
	request.SetData(DATABASE_COLUMN_ID_NAME, nextId);
	request.SetData(DATABASE_COLUMN_MESSAGE_TEXT_NAME, message);
	if (m_database.Insert(request) != 0) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "InsertFailed"));
		return false;
	}

	*id = nextId;

	return true;
}

bool AraMessageStorage::load(std::queue<StoredMessage>* messageContainer) {
	if (!messageContainer) {
		ACSDK_ERROR(LX("loadFailed").m("Alert container parameter is nullptr."));
		return false;
	}

	SelectRequest request;
	request.SetTable(MESSAGES_TABLE_NAME);
	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("loadFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			// local values which we will use to capture what we read from the database.
			int id = 0;
			std::string message;
			if ((cursor.GetInt(DATABASE_COLUMN_ID_NAME, id) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadFailed").d("reason", "idFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_MESSAGE_TEXT_NAME, message) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadFailed").d("reason", "messageFailed"));
				break;
			}

			ACSDK_DEBUG9(LX("load").d("id", id).d("message", message));

			StoredMessage storedMessage(id, message);
			messageContainer->push(storedMessage);
		} while (cursor.MoveToNext());
	}

	return true;
}

bool AraMessageStorage::erase(int messageId) {
	// Delete
	DeleteRequest delreq;
	delreq.SetTable(MESSAGES_TABLE_NAME);
	delreq.SetWhere(DATABASE_COLUMN_ID_NAME, messageId);
	if (m_database.Delete(delreq) != 0) {
		ACSDK_ERROR(LX("eraseFailed").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

bool AraMessageStorage::clearDatabase() {
	if (!m_database.clearTable(MESSAGES_TABLE_NAME)) {
		ACSDK_ERROR(LX("clearDatabaseFailed").m("could not clear messages table."));
		return false;
	}

	return true;
}

}  // namespace certifiedSender
}  // namespace alexaClientSDK
