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

#include "Bluetooth/AraBluetoothStorage.h"
#include "Bluetooth/BluetoothStorageInterface.h"
#include <AVSCommon/Utils/Logger/Logger.h>

/// String to identify log entries originating from this file.
static const std::string TAG{"AraBluetoothStorage"};

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

namespace alexaClientSDK {
namespace capabilityAgents {
namespace bluetooth {

using namespace alexaClientSDK::storage::araStorage;

/// Configuration root.
static const std::string BLUETOOTH_CONFIGURATION_ROOT_KEY = "bluetooth";

/// The node identifying the database.
static const std::string BLUETOOTH_DB_FILE_PATH_KEY = "databaseFilePath";

/// Table name.
static const std::string UUID_TABLE_NAME = "uuidMapping";

/// The UUID column.
static const std::string COLUMN_UUID = "uuid";

/// The MAC address column.
static const std::string COLUMN_MAC = "mac";

std::unique_ptr<BluetoothStorageInterface> AraBluetoothStorage::create(
	const avsCommon::utils::configuration::ConfigurationNode& configurationRoot) {
	ACSDK_DEBUG5(LX(__func__));
	auto bluetoothConfigurationRoot = configurationRoot[BLUETOOTH_CONFIGURATION_ROOT_KEY];
	if (!bluetoothConfigurationRoot) {
		ACSDK_ERROR(LX(__func__).d("reason", "loadConfigFailed").d("key", BLUETOOTH_CONFIGURATION_ROOT_KEY));
		return nullptr;
	}

	std::string filePath;
	if (!bluetoothConfigurationRoot.getString(BLUETOOTH_DB_FILE_PATH_KEY, &filePath) || filePath.empty()) {
		ACSDK_ERROR(LX(__func__).d("reason", "retrieveValueFailed").d("key", BLUETOOTH_DB_FILE_PATH_KEY));
		return nullptr;
	}

	return std::unique_ptr<AraBluetoothStorage>(new AraBluetoothStorage(filePath));
}

bool AraBluetoothStorage::createDatabase() {
	ACSDK_DEBUG5(LX(__func__));

	std::lock_guard<std::mutex> lock(m_databaseMutex);
	if (!m_db.initialize()) {
		ACSDK_ERROR(LX(__func__).d("reason", "initializeDBFailed"));
		return false;
	}

	// Create table
	CreateRequest request;
	request.SetTable(UUID_TABLE_NAME);
	request.SetPrimaryKey("uuid_hash", ATTR_TYPE_INT);
	request.SetCompositeKey(COLUMN_UUID, ATTR_TYPE_STR(255));
	request.SetColumnKey("mac_hash", ATTR_TYPE_INT);
	request.SetColumnKey(COLUMN_MAC, ATTR_TYPE_STR(255));
	int ret = m_db.CreateTable(request);
	if (ret != 0) {
		ACSDK_ERROR(LX("createDatabaseFailed").d("reason", "failed to create refreshToken table"));
		close();
		return false;
	}

	return true;
}

bool AraBluetoothStorage::open() {
	ACSDK_DEBUG5(LX(__func__));
	std::lock_guard<std::mutex> lock(m_databaseMutex);

	return m_db.open();
}

void AraBluetoothStorage::close() {
	ACSDK_DEBUG5(LX(__func__));
	std::lock_guard<std::mutex> lock(m_databaseMutex);

	return m_db.close();
}

bool AraBluetoothStorage::clear() {
	ACSDK_DEBUG5(LX(__func__));

	m_db.clearTable(UUID_TABLE_NAME);

	return true;
}

bool AraBluetoothStorage::getUuid(const std::string& mac, std::string* uuid) {
	ACSDK_DEBUG5(LX(__func__));
	if (!uuid) {
		ACSDK_ERROR(LX(__func__).d("reason", "nullUuid"));
		return false;
	}

	std::lock_guard<std::mutex> lock(m_databaseMutex);
	//return getAssociatedDataLocked(COLUMN_MAC, mac, COLUMN_UUID, uuid);

	SelectRequest request;
	request.SetTable(UUID_TABLE_NAME);
	request.SetWhere("mac_hash", (int)STR_HASH(mac));

	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getUuidFailed").d("reason", "selectFailed"));
		return false;
	}

	if (!cursor.MoveToFirst()) {
		ACSDK_ERROR(LX("getUuidFailed").d("reason", "notFound"));
		return false;
	}

	if ((cursor.GetString(COLUMN_UUID, *uuid) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getUuidFailed").d("reason", "getUuidFailed"));
		return false;
	}

	return true;
}

bool AraBluetoothStorage::getMac(const std::string& uuid, std::string* mac) {
	ACSDK_DEBUG5(LX(__func__));
	if (!mac) {
		ACSDK_ERROR(LX(__func__).d("reason", "nullMac"));
		return false;
	}

	std::lock_guard<std::mutex> lock(m_databaseMutex);
	//return getAssociatedDataLocked(COLUMN_UUID, uuid, COLUMN_MAC, mac);

	SelectRequest request;
	request.SetTable(UUID_TABLE_NAME);
	request.SetWhere("uuid_hash", (int)STR_HASH(uuid));

	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("getMacFailed").d("reason", "selectFailed"));
		return false;
	}

	if (!cursor.MoveToFirst()) {
		ACSDK_ERROR(LX("getMacFailed").d("reason", "notFound"));
		return false;
	}

	if ((cursor.GetString(COLUMN_MAC, *mac) != CURSOR_OK)) {
		ACSDK_ERROR(LX("getMacFailed").d("reason", "getMacFailed"));
		return false;
	}

	return true;
}

bool AraBluetoothStorage::getMappingsLocked(
	const std::string& keyPreference,
	std::unordered_map<std::string, std::string>* mappings) {
	ACSDK_DEBUG5(LX(__func__).d("keyPreference", keyPreference));

	if (!mappings) {
		ACSDK_ERROR(LX(__func__).d("reason", "nullMappings"));
		return false;
	}

	if (COLUMN_UUID != keyPreference && COLUMN_MAC != keyPreference) {
		ACSDK_ERROR(LX(__func__).d("reason", "invalidKeyPreference").d("keyPreference", keyPreference));
		return false;
	}

	SelectRequest request;
	request.SetTable(UUID_TABLE_NAME);
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX(__func__).d("reason", "database select failed"));
		return false;
	}

	if (cursor.MoveToFirst()) {
		do {
			std::string mac;
			std::string uuid;

			if ((cursor.GetString(COLUMN_MAC, mac) != CURSOR_OK)) {
				ACSDK_ERROR(LX("getMappingsLocked").d("reason", "getMacFailed"));
				continue;
			}

			if ((cursor.GetString(COLUMN_UUID, uuid) != CURSOR_OK)) {
				ACSDK_ERROR(LX("getMappingsLocked").d("reason", "getUuidFailed"));
				continue;
			}

			if (COLUMN_UUID == keyPreference) {
				mappings->insert({uuid, mac});
			} else if (COLUMN_MAC == keyPreference) {
				mappings->insert({mac, uuid});
			} else {
				ACSDK_ERROR(LX(__func__).d("reason", "unexpectedData").d("keyPreference", keyPreference));
			}
		} while (cursor.MoveToNext());
	}

	return true;
}

bool AraBluetoothStorage::getMacToUuid(std::unordered_map<std::string, std::string>* macToUuid) {
	ACSDK_DEBUG5(LX(__func__));
	std::lock_guard<std::mutex> lock(m_databaseMutex);
	return getMappingsLocked(COLUMN_MAC, macToUuid);
}

bool AraBluetoothStorage::getUuidToMac(std::unordered_map<std::string, std::string>* uuidToMac) {
	ACSDK_DEBUG5(LX(__func__));
	std::lock_guard<std::mutex> lock(m_databaseMutex);
	return getMappingsLocked(COLUMN_UUID, uuidToMac);
}

bool AraBluetoothStorage::getOrderedMac(bool ascending, std::list<std::string>* macs) {
	ACSDK_DEBUG5(LX(__func__));

	if (!macs) {
		ACSDK_ERROR(LX(__func__).d("reason", "nullMacs"));
		return false;
	}

	std::lock_guard<std::mutex> lock(m_databaseMutex);

	SelectRequest request;
	request.SetTable(UUID_TABLE_NAME);
	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX(__func__).d("reason", "database select failed"));
		return false;
	}

	if (ascending) {
		if (cursor.MoveToFirst()) {
			do {
				std::string mac;
				if ((cursor.GetString(COLUMN_MAC, mac) != CURSOR_OK)) {
					ACSDK_ERROR(LX("getMappingsLocked").d("reason", "getMacFailed"));
					continue;
				}

				if (mac.empty()) {
					continue;
				}

				macs->push_back(mac);
			} while (cursor.MoveToNext());
		}
	} else {
		if (cursor.MoveToLast()) {
			do {
				std::string mac;
				if ((cursor.GetString(COLUMN_MAC, mac) != CURSOR_OK)) {
					ACSDK_ERROR(LX("getMappingsLocked").d("reason", "getMacFailed"));
					continue;
				}

				if (mac.empty()) {
					continue;
				}

				macs->push_back(mac);
			} while (cursor.MoveToPrev());
		}
	}

	return true;
};

bool AraBluetoothStorage::insertByMac(const std::string& mac, const std::string& uuid, bool overwrite) {
	ACSDK_DEBUG5(LX(__func__));
	std::lock_guard<std::mutex> lock(m_databaseMutex);

	// Select data from database
	SelectRequest request;
	request.SetTable(UUID_TABLE_NAME);
	request.SetWhere("mac_hash", (int)STR_HASH(mac));

	Cursor cursor;
	if (m_db.Select(request, cursor) != 0) {
		ACSDK_ERROR(LX("insertByMacFailed").d("reason", "selectFailed"));
		return false;
	}

	if (cursor.GetCount() > 0) {
		if (overwrite) {
			remove(mac);
		} else {
			ACSDK_ERROR(LX("insertByMacFailed").d("reason", "macAlreadyExist"));
			return false;
		}
	}

	// Insert back with update
	InsertRequest insreq;
	insreq.SetTable(UUID_TABLE_NAME);
	insreq.SetData("uuid_hash", (int)STR_HASH(uuid));
	insreq.SetData(COLUMN_UUID, uuid);
	insreq.SetData("mac_hash", (int)STR_HASH(mac));
	insreq.SetData(COLUMN_MAC, mac);
	if (m_db.Insert(insreq) != 0) {
		ACSDK_ERROR(LX("insertByMacFailed").d("reason", "InsertFailed"));
		return false;
	}

	return true;
}

bool AraBluetoothStorage::remove(const std::string& mac) {
	ACSDK_DEBUG5(LX(__func__));
	const std::string sqlString = "DELETE FROM " + UUID_TABLE_NAME + " WHERE " + COLUMN_MAC + "=?;";

	std::lock_guard<std::mutex> lock(m_databaseMutex);

	DeleteRequest delreq;
	delreq.SetTable(UUID_TABLE_NAME);
	delreq.SetWhere("mac_hash", (int)STR_HASH(mac));
	if (m_db.Delete(delreq) != 0) {
		ACSDK_ERROR(LX("remove").d("reason", "DeleteFailed"));
		return false;
	}

	return true;
}

AraBluetoothStorage::AraBluetoothStorage(const std::string& filePath) : m_db{filePath} {
}

}  // namespace bluetooth
}  // namespace capabilityAgents
}  // namespace alexaClientSDK
