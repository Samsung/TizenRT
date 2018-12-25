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

#include <AVSCommon/Utils/File/FileUtils.h>
#include <AVSCommon/Utils/Logger/Logger.h>

#include <Notifications/AraNotificationsStorage.h>

namespace alexaClientSDK {
namespace capabilityAgents {
namespace notifications {

using namespace avsCommon::utils::file;
using namespace avsCommon::utils::logger;
using namespace alexaClientSDK::storage::araStorage;

/// String to identify log entries originating from this file.
static const std::string TAG("AraNotificationsStorage");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The key in our config file to find the root of settings.
static const std::string NOTIFICATIONS_CONFIGURATION_ROOT_KEY = "notifications";

/// The key in our config file to find the database file path.
static const std::string NOTIFICATIONS_DB_FILE_PATH_KEY = "databaseFilePath";

static const std::string NOTIFICATION_INDICATOR_TABLE_NAME = "notificationIndicators";

static const std::string DATABASE_COLUMN_PERSIST_VISUAL_INDICATOR_NAME = "persistVisualIndicator";

static const std::string DATABASE_COLUMN_PLAY_AUDIO_INDICATOR_NAME = "playAudioIndicator";

static const std::string DATABASE_COLUMN_ASSET_ID_NAME = "assetId";

static const std::string DATABASE_COLUMN_ASSET_URL_NAME = "assetUrl";

/// The name of the table and the field that will hold the state of the indicator.
static const std::string INDICATOR_STATE_NAME = "indicatorState";

std::unique_ptr<AraNotificationsStorage> AraNotificationsStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot) {
	auto notificationConfigurationRoot = configurationRoot[NOTIFICATIONS_CONFIGURATION_ROOT_KEY];
	if (!notificationConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config for the Notification Storage database")
						.d("key", NOTIFICATIONS_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string notificationDatabaseFilePath;
	if (!notificationConfigurationRoot.getString(NOTIFICATIONS_DB_FILE_PATH_KEY, &notificationDatabaseFilePath) ||
		notificationDatabaseFilePath.empty()) {
		ACSDK_ERROR(
			LX("createFailed").d("reason", "Could not load config value").d("key", NOTIFICATIONS_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraNotificationsStorage>(new AraNotificationsStorage(notificationDatabaseFilePath));
}

AraNotificationsStorage::AraNotificationsStorage(const std::string& databaseFilePath) :
		m_database{databaseFilePath} {
}

bool AraNotificationsStorage::createDatabase() {
	if (!m_database.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "SQLiteCreateDatabaseFailed"));
		return false;
	}

	// Create NotificationIndicator table
	CreateRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);
	request.SetPrimaryKey("id", ATTR_TYPE_INT);
	request.SetCompositeKey(DATABASE_COLUMN_PERSIST_VISUAL_INDICATOR_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_PLAY_AUDIO_INDICATOR_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_ASSET_ID_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey(DATABASE_COLUMN_ASSET_URL_NAME, ATTR_TYPE_STR(255));
	int ret = m_database.CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "failed to create notification indicator table"));
		close();
		return false;
	}


	// Note: If the NotificationIndicator table creation succeeds and the IndicatorState table creation fails,
	// the database will be in an inconsistent state and will require deletion or another call to createDatabase().

	// Create IndicatorState table
	CreateRequest request2;
	request2.SetTable(INDICATOR_STATE_NAME);
	request2.SetPrimaryKey(INDICATOR_STATE_NAME, ATTR_TYPE_INT);
	ret = m_database.CreateTable(request2);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "failed to create indicator state table"));
		// Delete notification indicator table
		m_database.dropTable(NOTIFICATION_INDICATOR_TABLE_NAME);
		close();
		return false;
	}

	// The default IndicatorState should be OFF.
	if (!setIndicatorState(IndicatorState::OFF)) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "failed to set default indicator state"));
		close();
		return false;
	}

	return true;
}

bool AraNotificationsStorage::open() {
	if (!m_database.open()) {
		ACSDK_ERROR(LX("openFailed").d("reason", "openSQLiteDatabaseFailed"));
		return false;
	}

	if (!m_database.tableExists(NOTIFICATION_INDICATOR_TABLE_NAME)) {
		ACSDK_ERROR(
			LX("openFailed").d("reason", "table doesn't exist").d("TableName", NOTIFICATION_INDICATOR_TABLE_NAME));
		return false;
	}

	SelectRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);

	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("checkForEmptyQueueFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			// Load id to local vector when open database
			int id;
			if ((cursor.GetInt("id", id) != CURSOR_OK)) {
				ACSDK_ERROR(LX("openFailed").d("reason", "getIdFailed"));
				break;
			}

			m_id.push_back(id);
		} while (cursor.MoveToNext());
	}

	if (!m_database.tableExists(INDICATOR_STATE_NAME)) {
		ACSDK_ERROR(LX("openFailed").d("reason", "table doesn't exist").d("TableName", INDICATOR_STATE_NAME));
		return false;
	}

	return true;
}

void AraNotificationsStorage::close() {
	m_database.close();
}

bool AraNotificationsStorage::enqueue(const NotificationIndicator& notificationIndicator) {
	// Inserted rows representations of a NotificationIndicator:
	// | id | persistVisualIndicator | playAudioIndicator | assetId | assetUrl |

	// lock here to bind the id generation and the enqueue operations
	std::lock_guard<std::mutex> lock{m_databaseMutex};

	int id = 1;
	if (m_id.size() != 0) {
		id = m_id[m_id.size() - 1];
		id++;
	}

	// Insert data
	InsertRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);
	request.SetData("id", id);
	request.SetData(DATABASE_COLUMN_PERSIST_VISUAL_INDICATOR_NAME, (int)notificationIndicator.persistVisualIndicator);
	request.SetData(DATABASE_COLUMN_PLAY_AUDIO_INDICATOR_NAME, (int)notificationIndicator.playAudioIndicator);
	request.SetData(DATABASE_COLUMN_ASSET_ID_NAME, notificationIndicator.asset.assetId);
	request.SetData(DATABASE_COLUMN_ASSET_URL_NAME, notificationIndicator.asset.url);
	if (m_database.Insert(request) != 0) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "InsertFailed"));
		return false;
	}

	m_id.push_back(id);
	return true;
}

bool AraNotificationsStorage::dequeue() {
	std::lock_guard<std::mutex> lock{m_databaseMutex};

	// need to check if there are NotificationIndicators left to dequeue, but the indicator itself doesn't matter
	NotificationIndicator dummyIndicator;

	if (!getNextNotificationIndicatorLocked(&dummyIndicator)) {
		ACSDK_ERROR(LX("dequeueFailed").m("No records left in the database!"));
		return false;
	}

	if (!popNotificationIndicatorLocked()) {
		ACSDK_ERROR(LX("dequeueFailed").m("Could not pop notificationIndicator from table"));
		return false;
	}

	return true;
}

bool AraNotificationsStorage::peek(NotificationIndicator* notificationIndicator) {
	std::lock_guard<std::mutex> lock{m_databaseMutex};
	if (!getNextNotificationIndicatorLocked(notificationIndicator)) {
		ACSDK_ERROR(LX("peekFailed").m("Could not retrieve the next notificationIndicator from the database"));
		return false;
	}
	return true;
}

bool AraNotificationsStorage::setIndicatorState(IndicatorState state) {
	std::lock_guard<std::mutex> lock{m_databaseMutex};

	// first delete the old record, we only need to maintain one record of IndicatorState at a time.
	if (!m_database.clearTable(INDICATOR_STATE_NAME)) {
		ACSDK_ERROR(LX("setIndicatorStateFailed").d("reason", "clearTableFailed"));
		return false;
	}

	// we should only be storing one record in this table at any given time
	// Insert data
	InsertRequest request;
	request.SetTable(INDICATOR_STATE_NAME);
	request.SetData(INDICATOR_STATE_NAME, indicatorStateToInt(state));
	if (m_database.Insert(request) != 0) {
		ACSDK_ERROR(LX("setIndicatorStateFailed").d("reason", "InsertFailed"));
		return false;
	}

	return true;
}

bool AraNotificationsStorage::getIndicatorState(IndicatorState* state) {
	if (!state) {
		ACSDK_ERROR(LX("getIndicatorStateFailed").m("State parameter was nullptr"));
		return false;
	}

	SelectRequest request;
	request.SetTable(INDICATOR_STATE_NAME);
	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getIndicatorStateFailed").d("reason", "selectFailed"));
		return false;
	}

	if (!cursor.MoveToFirst()) {
		ACSDK_ERROR(LX("getIndicatorStateFailed").m("No record left in table"));
		return false;
	}

	int iValue = 0;
	if ((cursor.GetInt(INDICATOR_STATE_NAME, iValue) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getIndicatorStateFailed").m("GetIntFailed"));
		return false;
	}

	*state = avsCommon::avs::intToIndicatorState(iValue);

	if (IndicatorState::UNDEFINED == *state) {
		ACSDK_ERROR(LX("getIndicatorStateFailed").m("Unknown indicator state retrieved from table"));
		return false;
	}

	return true;
}

bool AraNotificationsStorage::checkForEmptyQueue(bool* empty) {
	if (!empty) {
		ACSDK_ERROR(LX("checkForEmptyQueueFailed").m("empty parameter was nullptr"));
		return false;
	}

	SelectRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);

	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("checkForEmptyQueueFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.GetCount() < 1) {
		*empty = true;
	} else {
		*empty = false;
	}

	return true;
}

bool AraNotificationsStorage::clearNotificationIndicators() {
	std::lock_guard<std::mutex> lock{m_databaseMutex};

	if (!m_database.clearTable(NOTIFICATION_INDICATOR_TABLE_NAME)) {
		ACSDK_ERROR(LX("clearNotificationIndicatorsFailed").d("reason", "clearTableFailed"));
		return false;
	}

	return true;
}

AraNotificationsStorage::~AraNotificationsStorage() {
	close();
}

bool AraNotificationsStorage::getNextNotificationIndicatorLocked(NotificationIndicator* notificationIndicator) {
	// the minimum id is the next NotificationIndicator in the queue
	int id = 1;
	if (m_id.size() != 0) { // first is the mininum
		id = m_id[0];
	}

	SelectRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);
	request.SetWhere("id", id);
	Cursor cursor;
	if (m_database.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("popNotificationIndicatorLockedFailed").d("reason", "DeleteFailed"));
		return false;
	}

	if (!cursor.MoveToFirst()) {
		ACSDK_ERROR(LX("getNextNotificationIndicatorLockedFailed").m("No records left in table"));
		return false;
	}

	int persistVisualIndicator = 0;
	int playAudioIndicator = 0;
	std::string assetId = "";
	std::string assetUrl = "";

	if ((cursor.GetInt(DATABASE_COLUMN_PERSIST_VISUAL_INDICATOR_NAME, persistVisualIndicator) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getNextNotificationIndicatorLockedFailed").d("reason", "GetIntFailed"));
		return false;
	}

	if ((cursor.GetInt(DATABASE_COLUMN_PLAY_AUDIO_INDICATOR_NAME, playAudioIndicator) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getNextNotificationIndicatorLockedFailed").d("reason", "GetIntFailed"));
		return false;
	}

	if ((cursor.GetString(DATABASE_COLUMN_ASSET_ID_NAME, assetId) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getNextNotificationIndicatorLockedFailed").d("reason", "getAssertIdFailed"));
		return false;
	}

	if ((cursor.GetString(DATABASE_COLUMN_ASSET_URL_NAME, assetUrl) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getNextNotificationIndicatorLockedFailed").d("reason", "getAssertUrlFailed"));
		return false;
	}

	// load up the out NotificationIndicator
	*notificationIndicator = NotificationIndicator((bool)persistVisualIndicator, (bool)playAudioIndicator, assetId, assetUrl);
	return true;
}

/**
 * A utility function to pop the next notificationIndicator from the database.
 *
 * @param database Pounter to the database.
 * @return Whether the delete operation was successful.
 * @note This function should only be called when holding m_databaseMutex.
 */
bool AraNotificationsStorage::popNotificationIndicatorLocked() {
	// the next notificationIndicator in the queue corresponds to the minimum id
	int id = 1;
	if (m_id.size() != 0) { // first is the mininum
		id = m_id[0];
	}

	DeleteRequest request;
	request.SetTable(NOTIFICATION_INDICATOR_TABLE_NAME);
	request.SetWhere("id", id);
	if (m_database.Delete(request) != 0) {
		ACSDK_ERROR(LX("popNotificationIndicatorLockedFailed").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

bool AraNotificationsStorage::getQueueSize(int* size) {
	if (!size) {
		ACSDK_ERROR(LX("getQueueSizeFailed").m("size parameter was nullptr"));
		return false;
	}

	if (!m_database.getNumberTableRows(NOTIFICATION_INDICATOR_TABLE_NAME, size)) {
		ACSDK_ERROR(LX("getQueueSizeFailed").m("Failed to count rows in table"));
		return false;
	}
	return true;
}

}  // namespace notifications
}  // namespace capabilityAgents
}  // namespace alexaClientSDK
