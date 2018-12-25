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

#include "Alerts/Storage/AraAlertStorage.h"
#include "Alerts/Alarm.h"
#include "Alerts/Reminder.h"
#include "Alerts/Timer.h"

#include <AVSCommon/Utils/File/FileUtils.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/String/StringUtils.h>

#include <fstream>
#include <set>

namespace alexaClientSDK {
namespace capabilityAgents {
namespace alerts {
namespace storage {

using namespace avsCommon::utils::file;
using namespace avsCommon::utils::logger;
using namespace avsCommon::utils::string;
using namespace alexaClientSDK::storage::araStorage;

/// String to identify log entries originating from this file.
static const std::string TAG("AraAlertStorage");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

/// The key in our config file to find the root of settings for this Capability Agent.
static const std::string ALERTS_CAPABILITY_AGENT_CONFIGURATION_ROOT_KEY = "alertsCapabilityAgent";

/// The key in our config file to find the database file path.
static const std::string ALERTS_CAPABILITY_AGENT_DB_FILE_PATH_KEY = "databaseFilePath";

/// A definition which we will store in the database to indicate Alarm type.
static const int ALERT_EVENT_TYPE_ALARM = 1;
/// A definition which we will store in the database to indicate Timer type.
static const int ALERT_EVENT_TYPE_TIMER = 2;
/// A definition which we will store in the database to indicate Reminder type.
static const int ALERT_EVENT_TYPE_REMINDER = 3;

/// This is the string value this code will expect form an Alert of Alarm type.
static const std::string ALERT_EVENT_TYPE_ALARM_STRING = "ALARM";
/// This is the string value this code will expect form an Alert of Timer type.
static const std::string ALERT_EVENT_TYPE_TIMER_STRING = "TIMER";
/// This is the string value this code will expect form an Alert of Reminder type.
static const std::string ALERT_EVENT_TYPE_REMINDER_STRING = "REMINDER";

/// A definition which we will store in the database to indicate an Alert's Unset state.
static const int ALERT_STATE_UNSET = 1;
/// A definition which we will store in the database to indicate an Alert's Set state.
static const int ALERT_STATE_SET = 2;
/// A definition which we will store in the database to indicate an Alert's Activating state.
static const int ALERT_STATE_ACTIVATING = 3;
/// A definition which we will store in the database to indicate an Alert's Active state.
static const int ALERT_STATE_ACTIVE = 4;
/// A definition which we will store in the database to indicate an Alert's Snoozing state.
static const int ALERT_STATE_SNOOZING = 5;
/// A definition which we will store in the database to indicate an Alert's Snoozed state.
static const int ALERT_STATE_SNOOZED = 6;
/// A definition which we will store in the database to indicate an Alert's Stopping state.
static const int ALERT_STATE_STOPPING = 7;
/// A definition which we will store in the database to indicate an Alert's Stopped state.
static const int ALERT_STATE_STOPPED = 8;
/// A definition which we will store in the database to indicate an Alert's Completed state.
static const int ALERT_STATE_COMPLETED = 9;
/// A definition which we will store in the database to indicate an Alert's Ready state.
static const int ALERT_STATE_READY = 10;

/// The name of the 'id' field we will use as the primary key in our tables.
static const std::string DATABASE_COLUMN_ID_NAME = "_id"; // int
static const std::string DATABASE_COLUMN_TOKEN_NAME = "_token"; // text
static const std::string DATABASE_COLUMN_TYPE_NAME = "_type"; // int
static const std::string DATABASE_COLUMN_STATE_NAME = "_state"; // int
static const std::string DATABASE_COLUMN_SCHD_TIME_UNIX_NAME = "unix"; // int
static const std::string DATABASE_COLUMN_SCHD_TIME_ISO_8601_NAME = "iso"; // text
static const std::string DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME = "cnt"; // int
static const std::string DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME = "ms"; // int
static const std::string DATABASE_COLUMN_BACKGROUND_ASSERT_NAME = "bg"; // text

static const std::string DATABASE_COLUMN_ALERT_ID_NAME = "alert_id"; // int
static const std::string DATABASE_COLUMN_AVS_ID_NAME = "avs_id"; // text
static const std::string DATABASE_COLUMN_URL_NAME = "url"; // text

static const std::string DATABASE_COLUMN_ALERT_PLAY_ORDER_POS_NAME = "pos"; // int
static const std::string DATABASE_COLUMN_ALERT_PLAY_ORDER_TOKEN_NAME = "order"; // text

/// A symbolic name for version one of our database.
static const int ALERTS_DATABASE_VERSION_ONE = 1;
/// A symbolic name for version two of our database.
static const int ALERTS_DATABASE_VERSION_TWO = 2;

/// The name of the alerts table.
static const std::string ALERTS_TABLE_NAME = "alerts";

/// The name of the alerts (v2) table.
static const std::string ALERTS_V2_TABLE_NAME = "alerts_v2";

/// The name of the alertAssets table.
static const std::string ALERT_ASSETS_TABLE_NAME = "alertAssets";

/// The name of the alertAssetPlayOrderItems table.
static const std::string ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME = "alertAssetPlayOrderItems";

struct AssetOrderItem {
	int index;
	std::string name;
};

struct AssetOrderItemCompare {
	bool operator()(const AssetOrderItem& lhs, const AssetOrderItem& rhs) const {
		return lhs.index < rhs.index;
	}
};

/**
 * Utility function to convert an alert type string into a value we can store in the database.
 *
 * @param alertType A string reflecting the type of an alert.
 * @param[out] dbType The destination of the converted type which may be stored in a database.
 * @return Whether the type was successfully converted.
 */
static bool alertTypeToDbField(const std::string& alertType, int* dbType) {
	if (ALERT_EVENT_TYPE_ALARM_STRING == alertType) {
		*dbType = ALERT_EVENT_TYPE_ALARM;
	} else if (ALERT_EVENT_TYPE_TIMER_STRING == alertType) {
		*dbType = ALERT_EVENT_TYPE_TIMER;
	} else if (ALERT_EVENT_TYPE_REMINDER_STRING == alertType) {
		*dbType = ALERT_EVENT_TYPE_REMINDER;
	} else {
		ACSDK_ERROR(
			LX("alertTypeToDbFieldFailed").m("Could not determine alert type.").d("alert type string", alertType));
		return false;
	}

	return true;
}

/**
 * Utility function to convert an alert state into a value we can store in the database.
 *
 * @param state The state of an alert.
 * @param[out] dbState The destination of the converted state which may be stored in a database.
 * @return Whether the state was successfully converted.
 */
static bool alertStateToDbField(Alert::State state, int* dbState) {
	switch (state) {
		case Alert::State::UNSET:
			*dbState = ALERT_STATE_UNSET;
			return true;
		case Alert::State::SET:
			*dbState = ALERT_STATE_SET;
			return true;
		case Alert::State::READY:
			*dbState = ALERT_STATE_READY;
			return true;
		case Alert::State::ACTIVATING:
			*dbState = ALERT_STATE_ACTIVATING;
			return true;
		case Alert::State::ACTIVE:
			*dbState = ALERT_STATE_ACTIVE;
			return true;
		case Alert::State::SNOOZING:
			*dbState = ALERT_STATE_SNOOZING;
			return true;
		case Alert::State::SNOOZED:
			*dbState = ALERT_STATE_SNOOZED;
			return true;
		case Alert::State::STOPPING:
			*dbState = ALERT_STATE_STOPPING;
			return true;
		case Alert::State::STOPPED:
			*dbState = ALERT_STATE_STOPPED;
			return true;
		case Alert::State::COMPLETED:
			*dbState = ALERT_STATE_COMPLETED;
			return true;
	}

	ACSDK_ERROR(LX("alertStateToDbFieldFailed")
					.m("Could not convert alert state.")
					.d("alert object state", static_cast<int>(state)));
	return false;
}

/**
 * Utility function to convert a database value for an alert state into its @c Alert equivalent.
 *
 * @param dbState The state of an alert, as stored in the database.
 * @param[out] dbState The destination of the converted state.
 * @return Whether the database value was successfully converted.
 */
static bool dbFieldToAlertState(int dbState, Alert::State* state) {
	switch (dbState) {
		case ALERT_STATE_UNSET:
			*state = Alert::State::UNSET;
			return true;
		case ALERT_STATE_SET:
			*state = Alert::State::SET;
			return true;
		case ALERT_STATE_READY:
			*state = Alert::State::READY;
			return true;
		case ALERT_STATE_ACTIVATING:
			*state = Alert::State::ACTIVATING;
			return true;
		case ALERT_STATE_ACTIVE:
			*state = Alert::State::ACTIVE;
			return true;
		case ALERT_STATE_SNOOZING:
			*state = Alert::State::SNOOZING;
			return true;
		case ALERT_STATE_SNOOZED:
			*state = Alert::State::SNOOZED;
			return true;
		case ALERT_STATE_STOPPING:
			*state = Alert::State::STOPPING;
			return true;
		case ALERT_STATE_STOPPED:
			*state = Alert::State::STOPPED;
			return true;
		case ALERT_STATE_COMPLETED:
			*state = Alert::State::COMPLETED;
			return true;
	}

	ACSDK_ERROR(LX("dbFieldToAlertStateFailed").m("Could not convert db value.").d("db value", dbState));
	return false;
}

std::unique_ptr<AraAlertStorage> AraAlertStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot,
	const std::shared_ptr<avsCommon::sdkInterfaces::audio::AlertsAudioFactoryInterface>& alertsAudioFactory) {
	auto alertsConfigurationRoot = configurationRoot[ALERTS_CAPABILITY_AGENT_CONFIGURATION_ROOT_KEY];
	if (!alertsConfigurationRoot) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config for the Alerts capability agent")
						.d("key", ALERTS_CAPABILITY_AGENT_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string alertDbFilePath;
	if (!alertsConfigurationRoot.getString(ALERTS_CAPABILITY_AGENT_DB_FILE_PATH_KEY, &alertDbFilePath) ||
		alertDbFilePath.empty()) {
		ACSDK_ERROR(LX("createFailed")
						.d("reason", "Could not load config value")
						.d("key", ALERTS_CAPABILITY_AGENT_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraAlertStorage>(new AraAlertStorage(alertDbFilePath, alertsAudioFactory));
}

AraAlertStorage::AraAlertStorage(
	const std::string& dbFilePath,
	const std::shared_ptr<avsCommon::sdkInterfaces::audio::AlertsAudioFactoryInterface>& alertsAudioFactory) :
		m_alertsAudioFactory{alertsAudioFactory},
		m_db{dbFilePath} {
}

AraAlertStorage::~AraAlertStorage() {
	close();
}

/**
 * Utility function to create the Alerts table within the database.
 *
 * @param db The Arastorage Database object.
 * @return Whether the table was successfully created.
 */
static bool createAlertsTable(Database* db) {
	if (!db) {
		ACSDK_ERROR(LX("createAlertsTableFailed").m("null db"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertsTable").m("start"));

	// Create table
	CreateRequest request;
	request.SetTable(ALERTS_V2_TABLE_NAME);
	request.SetPrimaryKey(DATABASE_COLUMN_ID_NAME, ATTR_TYPE_INT);
	request.SetCompositeKey(DATABASE_COLUMN_TOKEN_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey(DATABASE_COLUMN_TYPE_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_STATE_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_SCHD_TIME_UNIX_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_SCHD_TIME_ISO_8601_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey(DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_BACKGROUND_ASSERT_NAME, ATTR_TYPE_STR(255));
	int ret = db->CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createAlertsTableFailed").d("reason", "CreateTableFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertsTable").m("success"));
	return true;
}

/**
 * Utility function to create the AlertAssets table within the database.
 *
 * @param db The Database object.
 * @return Whether the table was successfully created.
 */
static bool createAlertAssetsTable(Database* db) {
	if (!db) {
		ACSDK_ERROR(LX("createAlertAssetsTableFailed").m("null db"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertAssetsTable").m("start"));

	// Create table
	CreateRequest request;
	request.SetTable(ALERT_ASSETS_TABLE_NAME);
	request.SetPrimaryKey(DATABASE_COLUMN_ID_NAME, ATTR_TYPE_INT);
	request.SetCompositeKey(DATABASE_COLUMN_ALERT_ID_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_AVS_ID_NAME, ATTR_TYPE_STR(255));
	request.SetColumnKey(DATABASE_COLUMN_URL_NAME, ATTR_TYPE_STR(255));
	int ret = db->CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createAlertAssetsTableFailed").d("reason", "CreateTableFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertAssetsTable").m("success"));
	return true;
}

/**
 * Utility function to create the AlertAssetPlayOrderItems table within the database.
 *
 * @param db The Database object.
 * @return Whether the table was successfully created.
 */
static bool createAlertAssetPlayOrderItemsTable(Database* db) {
	if (!db) {
		ACSDK_ERROR(LX("createAlertAssetPlayOrderItemsTableFailed").m("null db"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertAssetPlayOrderItemsTable").m("start"));

	// Create table
	CreateRequest request;
	request.SetTable(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME);
	request.SetPrimaryKey(DATABASE_COLUMN_ID_NAME, ATTR_TYPE_INT);
	request.SetCompositeKey(DATABASE_COLUMN_ALERT_ID_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_ALERT_PLAY_ORDER_POS_NAME, ATTR_TYPE_INT);
	request.SetColumnKey(DATABASE_COLUMN_ALERT_PLAY_ORDER_TOKEN_NAME, ATTR_TYPE_STR(255));
	int ret = db->CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createAlertAssetPlayOrderItemsTableFailed").d("reason", "CreateTableFailed"));
		return false;
	}

	ACSDK_DEBUG9(LX("createAlertAssetPlayOrderItemsTable").m("success"));
	return true;
}

bool AraAlertStorage::createDatabase() {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!m_db.initialize()) {
		ACSDK_ERROR(LX("createDatabaseFailed"));
		return false;
	}

	if (!createAlertsTable(&m_db)) {
		ACSDK_ERROR(LX("createDatabaseFailed").m("Alerts table could not be created."));
		close();
		return false;
	}

	if (!createAlertAssetsTable(&m_db)) {
		ACSDK_ERROR(LX("createDatabaseFailed").m("AlertAssets table could not be created."));
		close();
		return false;
	}

	if (!createAlertAssetPlayOrderItemsTable(&m_db)) {
		ACSDK_ERROR(LX("createDatabaseFailed").m("AlertAssetPlayOrderItems table could not be created."));
		close();
		return false;
	}

	return true;
}

bool AraAlertStorage::migrateAlertsDbFromV1ToV2() {
	ACSDK_DEBUG9(LX(__FUNCTION__));
	// The good case - the db file is already up to date.
	if (m_db.tableExists(ALERTS_V2_TABLE_NAME)) {
		return true;
	}

	if (!createAlertsTable(&m_db)) {
		ACSDK_ERROR(LX("migrateAlertsDbFromV1ToV2Failed").m("Alert table could not be created."));
		return false;
	}

	if (!m_db.tableExists(ALERT_ASSETS_TABLE_NAME)) {
		if (!createAlertAssetsTable(&m_db)) {
			ACSDK_ERROR(LX("migrateAlertsDbFromV1ToV2Failed").m("AlertAssets table could not be created."));
			return false;
		}
	}

	if (!m_db.tableExists(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME)) {
		if (!createAlertAssetPlayOrderItemsTable(&m_db)) {
			ACSDK_ERROR(
				LX("migrateAlertsDbFromV1ToV2Failed").m("AlertAssetPlayOrderItems table could not be created."));
			return false;
		}
	}

	return true;
}

bool AraAlertStorage::open() {
	ACSDK_DEBUG9(LX(__FUNCTION__));
	return m_db.open();
}

void AraAlertStorage::close() {
	ACSDK_DEBUG9(LX(__FUNCTION__));
	m_db.close();
}

bool AraAlertStorage::alertExists(const std::string& token) {
	ACSDK_DEBUG9(LX(__FUNCTION__).d(DATABASE_COLUMN_TOKEN_NAME, token));

	SelectRequest request;
	request.SetTable(ALERTS_V2_TABLE_NAME);
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("alertExistsFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			std::string _token;
			if (cursor.GetString(DATABASE_COLUMN_TOKEN_NAME, _token) != CURSOR_OK) {
				ACSDK_ERROR(LX("alertExistsFailed").d("reason", "GetLengthFailed"));
				//return false;
				continue;
			}

			if (_token.compare(token) == 0) {
				return true;
			}
		} while (cursor.MoveToNext());
	}

	ACSDK_DEBUG9(LX("alertExists").m("NotExist"));
	return false;
}

static bool storeAlertAssets(
	Database* db,
	int alertId,
	const std::unordered_map<std::string, Alert::Asset>& assets) {
	if (assets.empty()) {
		return true;
	}

	ACSDK_DEBUG9(LX(__FUNCTION__));

	int id = 0;
	if (!db->getTableMaxIntValue(ALERT_ASSETS_TABLE_NAME, DATABASE_COLUMN_ID_NAME, &id)) {
		ACSDK_ERROR(LX("storeAlertAssetsFailed").m("Cannot generate asset id."));
		return false;
	}
	id++;

	// go through each asset in the alert, and store in the database.
	for (auto& assetIter : assets) {
		auto& asset = assetIter.second;

		// Insert data
		InsertRequest request;
		request.SetTable(ALERT_ASSETS_TABLE_NAME);
		request.SetData(DATABASE_COLUMN_ID_NAME, id);
		request.SetData(DATABASE_COLUMN_ALERT_ID_NAME, alertId);
		request.SetData(DATABASE_COLUMN_AVS_ID_NAME, asset.id);
		request.SetData(DATABASE_COLUMN_URL_NAME, asset.url);
		if (db->Insert(request) != 0) {
			ACSDK_ERROR(LX("storeAlertAssetsFailed").d("reason", "InsertFailed"));
			return false;
		}

		id++;
	}

	return true;
}

static bool storeAlertAssetPlayOrderItems(
	Database* db,
	int alertId,
	const std::vector<std::string>& assetPlayOrderItems) {
	if (assetPlayOrderItems.empty()) {
		return true;
	}

	ACSDK_DEBUG9(LX(__FUNCTION__));

	int id = 0;
	if (!db->getTableMaxIntValue(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME, DATABASE_COLUMN_ID_NAME, &id)) {
		ACSDK_ERROR(LX("storeAlertAssetPlayOrderItemsFailed").m("Cannot generate asset id."));
		return false;
	}
	id++;

	// go through each assetPlayOrderItem in the alert, and store in the database.
	int itemIndex = 1;
	for (auto& assetId : assetPlayOrderItems) {
		// Insert data
		InsertRequest request;
		request.SetTable(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME);
		request.SetData(DATABASE_COLUMN_ID_NAME, id);
		request.SetData(DATABASE_COLUMN_ALERT_ID_NAME, alertId);
		request.SetData(DATABASE_COLUMN_ALERT_PLAY_ORDER_POS_NAME, itemIndex);
		request.SetData(DATABASE_COLUMN_ALERT_PLAY_ORDER_TOKEN_NAME, assetId);
		if (db->Insert(request) != 0) {
			ACSDK_ERROR(LX("storeAlertAssetPlayOrderItemsFailed").d("reason", "InsertFailed"));
			return false;
		}

		id++;
		itemIndex++;
	}

	return true;
}

bool AraAlertStorage::store(std::shared_ptr<Alert> alert) {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!alert) {
		ACSDK_ERROR(LX("storeFailed").m("Alert parameter is nullptr"));
		return false;
	}

	if (alertExists(alert->m_token)) {
		ACSDK_ERROR(LX("storeFailed").m("Alert already exists.").d(DATABASE_COLUMN_TOKEN_NAME, alert->m_token));
		return false;
	}

	int id = 0;
	if (!m_db.getTableMaxIntValue(ALERTS_V2_TABLE_NAME, DATABASE_COLUMN_ID_NAME, &id)) {
		ACSDK_ERROR(LX("storeFailed").m("Cannot generate alert id."));
		return false;
	}
	id++;

	int alertType = ALERT_EVENT_TYPE_ALARM;
	if (!alertTypeToDbField(alert->getTypeName(), &alertType)) {
		ACSDK_ERROR(LX("storeFailed").m("Could not convert type name to db field."));
		return false;
	}

	int alertState = ALERT_STATE_SET;
	if (!alertStateToDbField(alert->m_state, &alertState)) {
		ACSDK_ERROR(LX("storeFailed").m("Could not convert alert state to db field."));
		return false;
	}

	auto token = alert->m_token;
	auto iso8601 = alert->getScheduledTime_ISO_8601();
	auto assetId = alert->getBackgroundAssetId();

	// Insert data
	InsertRequest request;
	request.SetTable(ALERTS_V2_TABLE_NAME);
	request.SetData(DATABASE_COLUMN_ID_NAME, id);
	request.SetData(DATABASE_COLUMN_TOKEN_NAME, token);
	request.SetData(DATABASE_COLUMN_TYPE_NAME, alertType);
	request.SetData(DATABASE_COLUMN_STATE_NAME, alertState);
	request.SetData(DATABASE_COLUMN_SCHD_TIME_UNIX_NAME, (int)alert->getScheduledTime_Unix());
	request.SetData(DATABASE_COLUMN_SCHD_TIME_ISO_8601_NAME, iso8601);
	request.SetData(DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME, alert->getLoopCount());
	request.SetData(DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME, (int)alert->getLoopPause().count());
	request.SetData(DATABASE_COLUMN_BACKGROUND_ASSERT_NAME, assetId);
	if (m_db.Insert(request) != 0) {
		ACSDK_ERROR(LX("storeFailed").d("reason", "InsertFailed"));
		return false;
	}

	// capture the generated database id in the alert object.
	alert->m_dbId = id;

	if (!storeAlertAssets(&m_db, id, alert->m_assetConfiguration.assets)) {
		ACSDK_ERROR(LX("storeFailed").m("Could not store alertAssets."));
		return false;
	}

	if (!storeAlertAssetPlayOrderItems(&m_db, id, alert->m_assetConfiguration.assetPlayOrderItems)) {
		ACSDK_ERROR(LX("storeFailed").m("Could not store alertAssetPlayOrderItems."));
		return false;
	}

	return true;
}

static bool loadAlertAssets(Database* db, std::map<int, std::vector<Alert::Asset>>* alertAssetsMap) {
	ACSDK_DEBUG9(LX(__FUNCTION__));
	Cursor cursor;
	SelectRequest request;
	request.SetTable(ALERT_ASSETS_TABLE_NAME);
	if (db->Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("loadAlertAssetsFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			int alertId;
			std::string avsId;
			std::string url;

			if ((cursor.GetInt(DATABASE_COLUMN_ALERT_ID_NAME, alertId) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetsFailed").d("reason", "alertIdFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_AVS_ID_NAME, avsId) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetsFailed").d("reason", "avsIdFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_URL_NAME, url) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetsFailed").d("reason", "urlFailed"));
				break;
			}

			ACSDK_DEBUG9(LX("loadAlertAssets").d(DATABASE_COLUMN_ALERT_ID_NAME, alertId)
											  .d(DATABASE_COLUMN_AVS_ID_NAME, avsId)
											  .d(DATABASE_COLUMN_URL_NAME, url));
			(*alertAssetsMap)[alertId].push_back(Alert::Asset(avsId, url));
		} while (cursor.MoveToNext());
	}

	return true;
}

static bool loadAlertAssetPlayOrderItems(
	Database* db,
	std::map<int, std::set<AssetOrderItem, AssetOrderItemCompare>>* alertAssetOrderItemsMap) {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	Cursor cursor;
	SelectRequest request;
	request.SetTable(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME);
	if (db->Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("loadAlertAssetPlayOrderItemsFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			int alertId = 0;
			int playOrderPosition = 0;
			std::string playOrderToken;

			if ((cursor.GetInt(DATABASE_COLUMN_ALERT_ID_NAME, alertId) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetPlayOrderItemsFailed").d("reason", "alertIdFailed"));
				break;
			}

			if ((cursor.GetInt(DATABASE_COLUMN_ALERT_PLAY_ORDER_POS_NAME, playOrderPosition) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetPlayOrderItemsFailed").d("reason", "asset_play_order_positionFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_ALERT_PLAY_ORDER_TOKEN_NAME, playOrderToken) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadAlertAssetPlayOrderItemsFailed").d("reason", "asset_play_order_tokenFailed"));
				break;
			}

			//ACSDK_DEBUG9(LX("loadAlertAssetPlayOrderItems").d(DATABASE_COLUMN_ALERT_ID_NAME, alertId)
			//                                             .d(DATABASE_COLUMN_ALERT_PLAY_ORDER_POS_NAME, playOrderPosition)
			//                                             .d(DATABASE_COLUMN_ALERT_PLAY_ORDER_TOKEN_NAME, playOrderToken));
			(*alertAssetOrderItemsMap)[alertId].insert(AssetOrderItem{playOrderPosition, playOrderToken});
		} while (cursor.MoveToNext());
	}

	return true;
}

bool AraAlertStorage::loadHelper(int dbVersion, std::vector<std::shared_ptr<Alert>>* alertContainer) {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!alertContainer) {
		ACSDK_ERROR(LX("loadHelperFailed").m("Alert container parameter is nullptr."));
		return false;
	}

	if (dbVersion != ALERTS_DATABASE_VERSION_ONE && dbVersion != ALERTS_DATABASE_VERSION_TWO) {
		ACSDK_ERROR(LX("loadHelperFailed").d("Invalid version", dbVersion));
		return false;
	}

	std::string alertsTableName = ALERTS_TABLE_NAME;
	if (ALERTS_DATABASE_VERSION_TWO == dbVersion) {
		alertsTableName = ALERTS_V2_TABLE_NAME;
	}

	SelectRequest request;
	request.SetTable(alertsTableName);
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("loadHelperFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			// local values which we will use to capture what we read from the database.
			int id = 0;
			std::string token;
			int type = 0;
			int state = 0;
			std::string scheduledTime_ISO_8601;
			int loopCount = 0;
			int loopPauseInMilliseconds = 0;
			std::string backgroundAssetId;

			if ((cursor.GetInt(DATABASE_COLUMN_ID_NAME, id) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "idFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_TOKEN_NAME, token) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "tokenFailed"));
				break;
			}

			if ((cursor.GetInt(DATABASE_COLUMN_TYPE_NAME, type) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "typeFailed"));
				break;
			}

			if ((cursor.GetInt(DATABASE_COLUMN_STATE_NAME, state) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "stateFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_SCHD_TIME_ISO_8601_NAME, scheduledTime_ISO_8601) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "scheduled_time_iso_8601Failed"));
				break;
			}

			if ((cursor.GetInt(DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME, loopCount) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "asset_loop_countFailed"));
				break;
			}

			if ((cursor.GetInt(DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME, loopPauseInMilliseconds) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "asset_loop_pause_millisecondsFailed"));
				break;
			}

			if ((cursor.GetString(DATABASE_COLUMN_BACKGROUND_ASSERT_NAME, backgroundAssetId) != CURSOR_OK)) {
				ACSDK_ERROR(LX("loadHelperFailed").d("reason", "background_assetFailed"));
				break;
			}

			std::shared_ptr<Alert> alert;
			if (ALERT_EVENT_TYPE_ALARM == type) {
				alert = std::make_shared<Alarm>(m_alertsAudioFactory->alarmDefault(), m_alertsAudioFactory->alarmShort());
			} else if (ALERT_EVENT_TYPE_TIMER == type) {
				alert = std::make_shared<Timer>(m_alertsAudioFactory->timerDefault(), m_alertsAudioFactory->timerShort());
			} else if (ALERT_EVENT_TYPE_REMINDER == type) {
				alert = std::make_shared<Reminder>(
					m_alertsAudioFactory->reminderDefault(), m_alertsAudioFactory->reminderShort());
			} else {
				ACSDK_ERROR(
					LX("loadHelperFailed").m("Could not instantiate an alert object.").d("type read from database", type));
				return false;
			}

			alert->m_dbId = id;
			alert->m_token = token;
			alert->setTime_ISO_8601(scheduledTime_ISO_8601);
			alert->setLoopCount(loopCount);
			alert->setLoopPause(std::chrono::milliseconds{loopPauseInMilliseconds});
			alert->setBackgroundAssetId(backgroundAssetId);

			if (!dbFieldToAlertState(state, &(alert->m_state))) {
				ACSDK_ERROR(LX("loadHelperFailed").m("Could not convert alert state."));
				return false;
			}

			alertContainer->push_back(alert);
		} while (cursor.MoveToNext());
	}

	std::map<int, std::vector<Alert::Asset>> alertAssetsMap;
	if (!loadAlertAssets(&m_db, &alertAssetsMap)) {
		ACSDK_ERROR(LX("loadHelperFailed").m("Could not load alert assets."));
		return false;
	}

	std::map<int, std::set<AssetOrderItem, AssetOrderItemCompare>> alertAssetOrderItemsMap;
	if (!loadAlertAssetPlayOrderItems(&m_db, &alertAssetOrderItemsMap)) {
		ACSDK_ERROR(LX("loadHelperFailed").m("Could not load alert asset play order items."));
		return false;
	}

	for (auto alert : *alertContainer) {
		if (alertAssetsMap.find(alert->m_dbId) != alertAssetsMap.end()) {
			for (auto& mapEntry : alertAssetsMap[alert->m_dbId]) {
				alert->m_assetConfiguration.assets[mapEntry.id] = mapEntry;
			}
		}
		if (alertAssetOrderItemsMap.find(alert->m_dbId) != alertAssetOrderItemsMap.end()) {
			for (auto& mapEntry : alertAssetOrderItemsMap[alert->m_dbId]) {
				alert->m_assetConfiguration.assetPlayOrderItems.push_back(mapEntry.name);
			}
		}
	}

	return true;
}

bool AraAlertStorage::load(std::vector<std::shared_ptr<Alert>>* alertContainer) {
	ACSDK_DEBUG9(LX(__FUNCTION__));
	return loadHelper(ALERTS_DATABASE_VERSION_TWO, alertContainer);
}

bool AraAlertStorage::modify(std::shared_ptr<Alert> alert) {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!alert) {
		ACSDK_ERROR(LX("modifyFailed").m("Alert parameter is nullptr."));
		return false;
	}

	if (!alertExists(alert->m_token)) {
		ACSDK_ERROR(LX("modifyFailed").m("Cannot modify alert.").d(DATABASE_COLUMN_TOKEN_NAME, alert->m_token));
		return false;
	}

	int alertState = ALERT_STATE_SET;
	if (!alertStateToDbField(alert->m_state, &alertState)) {
		ACSDK_ERROR(LX("modifyFailed").m("Cannot convert state."));
		return false;
	}

	// Select data from database
	SelectRequest request;
	request.SetTable(ALERTS_V2_TABLE_NAME);
	request.SetWhere(DATABASE_COLUMN_ID_NAME, alert->m_dbId);

	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.GetCount() != 1) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "selectResultError").d("cursorCount", cursor.GetCount()));
		return false;
	}

	cursor.MoveToFirst();

	// local values which we will use to capture what we read from the database.
	//int id = 0; // stoi
	std::string token;
	int type = 0; // stoi
	//int state = 0; // stoi
	//int scheduledTime_unix; // stoi
	//std::string scheduledTime_ISO_8601;
	int loopCount = 0; // stoi
	int loopPauseInMilliseconds = 0; // stoi
	std::string backgroundAssetId;

	if (cursor.GetString(DATABASE_COLUMN_TOKEN_NAME, token) != CURSOR_OK) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "tokenFailed"));
		return false;
	}

	if (cursor.GetInt(DATABASE_COLUMN_TYPE_NAME, type) != CURSOR_OK) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "typeFailed"));
		return false;
	}

	if (cursor.GetInt(DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME, loopCount) != CURSOR_OK) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "asset_loop_countFailed"));
		return false;
	}

	if (cursor.GetInt(DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME, loopPauseInMilliseconds) != CURSOR_OK) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "asset_loop_pause_millisecondsFailed"));
		return false;
	}

	if (cursor.GetString(DATABASE_COLUMN_BACKGROUND_ASSERT_NAME, backgroundAssetId) != CURSOR_OK) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "background_assetFailed"));
		return false;
	}

	// Delete record from database
	DeleteRequest delreq;
	delreq.SetTable(ALERTS_V2_TABLE_NAME);
	delreq.SetWhere(DATABASE_COLUMN_ID_NAME, alert->m_dbId);
	if (m_db.Delete(delreq) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "DeleteFailed"));
		return false;
	}

	// Insert back with update
	InsertRequest insreq;
	insreq.SetTable(ALERTS_V2_TABLE_NAME);
	insreq.SetData(DATABASE_COLUMN_ID_NAME, alert->m_dbId);
	insreq.SetData(DATABASE_COLUMN_TOKEN_NAME, token);
	insreq.SetData(DATABASE_COLUMN_TYPE_NAME, type);
	insreq.SetData(DATABASE_COLUMN_STATE_NAME, alertState); // update
	insreq.SetData(DATABASE_COLUMN_SCHD_TIME_UNIX_NAME, (int)alert->getScheduledTime_Unix()); // update
	insreq.SetData(DATABASE_COLUMN_SCHD_TIME_ISO_8601_NAME, alert->getScheduledTime_ISO_8601()); // update
	insreq.SetData(DATABASE_COLUMN_ASSERT_LOOP_COUNT_NAME, loopCount);
	insreq.SetData(DATABASE_COLUMN_ASSERT_LOOP_PAUSE_MS_NAME, loopPauseInMilliseconds);
	insreq.SetData(DATABASE_COLUMN_BACKGROUND_ASSERT_NAME, backgroundAssetId);
	if (m_db.Insert(insreq) != 0) {
		ACSDK_ERROR(LX("modifyFailed").d("reason", "InsertFailed"));
		return false;
	}

	return true;
}

/**
 * A utility function to delete alert records from the database for a given alert id.
 * This function will clean up records in the alerts table.
 *
 * @param db The database object.
 * @param alertId The alert id of the alert to be deleted.
 * @return Whether the delete operation was successful.
 */
static bool eraseAlert(Database* db, int alertId) {
	ACSDK_DEBUG9(LX(__FUNCTION__).d("alertId", alertId));

	DeleteRequest request;
	request.SetTable(ALERTS_V2_TABLE_NAME);
	request.SetWhere(DATABASE_COLUMN_ID_NAME, alertId);
	if (db->Delete(request) != 0) {
		ACSDK_ERROR(LX("eraseAlert").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

/**
 * A utility function to delete alert records from the database for a given alert id.
 * This function will clean up records in the alertAssets table.
 *
 * @param db The database object.
 * @param alertId The alert id of the alert to be deleted.
 * @return Whether the delete operation was successful.
 */
static bool eraseAlertAssets(Database* db, int alertId) {
	ACSDK_DEBUG9(LX(__FUNCTION__).d("alertId", alertId));

	DeleteRequest request;
	request.SetTable(ALERT_ASSETS_TABLE_NAME);
	request.SetWhere(DATABASE_COLUMN_ALERT_ID_NAME, alertId);
	if (db->Delete(request) != 0) {
		ACSDK_ERROR(LX("eraseAlertAssets").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

/**
 * A utility function to delete alert records from the database for a given alert id.
 * This function will clean up records in the alertAssetPlayOrderItems table.
 *
 * @param db The database object.
 * @param alertId The alert id of the alert to be deleted.
 * @return Whether the delete operation was successful.
 */
static bool eraseAlertAssetPlayOrderItems(Database* db, int alertId) {
	ACSDK_DEBUG9(LX(__FUNCTION__).d("alertId", alertId));

	DeleteRequest request;
	request.SetTable(ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME);
	request.SetWhere(DATABASE_COLUMN_ALERT_ID_NAME, alertId);
	if (db->Delete(request) != 0) {
		ACSDK_ERROR(LX("eraseAlertAssets").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

/**
 * A utility function to delete an alert from the database for a given alert id.  This will clean up records in
 * all tables which are associated with the alert.
 *
 * @param db The DB object.
 * @param alertId The alert id of the alert to be deleted.
 * @return Whether the delete operation was successful.
 */
static bool eraseAlertByAlertId(Database* db, int alertId) {
	ACSDK_DEBUG9(LX(__FUNCTION__).d("alertId", alertId));

	if (!db) {
		ACSDK_ERROR(LX("eraseAlertByAlertIdFailed").m("db is nullptr."));
		return false;
	}

	if (!eraseAlert(db, alertId)) {
		ACSDK_ERROR(LX("eraseAlertByAlertIdFailed").m("Could not erase alert table items."));
		return false;
	}

	if (!eraseAlertAssets(db, alertId)) {
		ACSDK_ERROR(LX("eraseAlertByAlertIdFailed").m("Could not erase alertAsset table items."));
		return false;
	}

	if (!eraseAlertAssetPlayOrderItems(db, alertId)) {
		ACSDK_ERROR(LX("eraseAlertByAlertIdFailed").m("Could not erase alertAssetPlayOrderItems table items."));
		return false;
	}

	return true;
}

bool AraAlertStorage::erase(std::shared_ptr<Alert> alert) {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	if (!alert) {
		ACSDK_ERROR(LX("eraseFailed").m("Alert parameter is nullptr."));
		return false;
	}

	if (!alertExists(alert->m_token)) {
		ACSDK_ERROR(LX("eraseFailed").m("Cannot delete alert - not in database.").d(DATABASE_COLUMN_TOKEN_NAME, alert->m_token));
		return false;
	}

	return eraseAlertByAlertId(&m_db, alert->m_dbId);
}

bool AraAlertStorage::clearDatabase() {
	ACSDK_DEBUG9(LX(__FUNCTION__));

	const std::vector<std::string> tablesToClear = {
		ALERTS_V2_TABLE_NAME, ALERT_ASSETS_TABLE_NAME, ALERT_ASSET_PLAY_ORDER_ITEMS_TABLE_NAME};
	for (auto& tableName : tablesToClear) {
		if (!m_db.clearTable(tableName)) {
			ACSDK_ERROR(LX("clearDatabaseFailed").d("could not clear table", tableName));
			return false;
		}
	}

	return true;
}

/**
 * Utility diagnostic function to print a one-line summary of all alerts in the database.
 *
 * @param db The database object.
 */
static void printOneLineSummary(Database* db) {
	int numberAlerts = 0;

	if (!db->getNumberTableRows(ALERTS_V2_TABLE_NAME, &numberAlerts)) {
		ACSDK_ERROR(LX("printOneLineSummaryFailed").m("could not read number of alerts."));
		return;
	}

	ACSDK_INFO(LX("ONE-LINE-STAT: Number of alerts:" + std::to_string(numberAlerts)));
}

/**
 * Utility diagnostic function to print the details of all the alerts stored in the database.
 *
 * @param db The database object.
 * @param shouldPrintEverything If @c true, then all details of an alert will be printed.  If @c false, then
 * summary information will be printed instead.
 */
static void printAlertsSummary(
	Database* db,
	const std::vector<std::shared_ptr<Alert>>& alerts,
	bool shouldPrintEverything = false) {
	printOneLineSummary(db);

	for (auto& alert : alerts) {
		alert->printDiagnostic();
	}
}

void AraAlertStorage::printStats(StatLevel level) {
	std::vector<std::shared_ptr<Alert>> alerts;
	load(&alerts);
	switch (level) {
		case StatLevel::ONE_LINE:
			printOneLineSummary(&m_db);
			break;
		case StatLevel::ALERTS_SUMMARY:
			printAlertsSummary(&m_db, alerts, false);
			break;
		case StatLevel::EVERYTHING:
			printAlertsSummary(&m_db, alerts, true);
			break;
	}
}

}  // namespace storage
}  // namespace alerts
}  // namespace capabilityAgents
}  // namespace alexaClientSDK
