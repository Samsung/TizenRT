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

#ifndef ALEXA_CLIENT_SDK_CAPABILITYAGENTS_BLUETOOTH_INCLUDE_BLUETOOTH_ARABLUETOOTHSTORAGE_H_
#define ALEXA_CLIENT_SDK_CAPABILITYAGENTS_BLUETOOTH_INCLUDE_BLUETOOTH_ARABLUETOOTHSTORAGE_H_

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AraStorage/Database.h"

#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include "Bluetooth/BluetoothStorageInterface.h"

namespace alexaClientSDK {
namespace capabilityAgents {
namespace bluetooth {

/// A concrete implementation of @c BluetoothStorageInterface using Arastorage.
class AraBluetoothStorage : public BluetoothStorageInterface {
public:
	/**
	 * Create an instance of a AraBluetoothStorage object.
	 *
	 * @param configurationRoot A ConfigurationNode containing the location of the .db file.
	 * Should take the form:
	 * "bluetooth" : { "databaseFilePath" : "<filePath>" }
	 */
	static std::unique_ptr<BluetoothStorageInterface> create(
		const avsCommon::utils::configuration::ConfigurationNode& configurationRoot);

	/// @name BluetoothStorageInterface
	/// @{
	bool createDatabase() override;
	bool open() override;
	void close() override;
	bool clear() override;
	bool getUuid(const std::string& mac, std::string* uuid) override;
	bool getMac(const std::string& uuid, std::string* mac) override;
	bool getMacToUuid(std::unordered_map<std::string, std::string>* macToUuid) override;
	bool getUuidToMac(std::unordered_map<std::string, std::string>* uuidToMac) override;
	bool getOrderedMac(bool ascending, std::list<std::string>* macs) override;
	bool insertByMac(const std::string& mac, const std::string& uuid, bool overwrite = true) override;
	bool remove(const std::string& mac) override;
	/// @}

private:
	/**
	 * Constructor.
	 *
	 * @param filepath The filepath of the DB file.
	 */
	AraBluetoothStorage(const std::string& filepath);

	/**
	 * Utility that extracts from the database all uuid and mac pairs. The pairs are returned in a map,
	 * and the key is dependent on the @c keyPreference param.
	 * The lock must be obtained before calling this function.
	 *
	 * @param keyPreference A preference of whether the uuid or the mac should be the key.
	 * @param[out] mappings A mapping of uuid and mac keyed by keyPreference.
	 * @return A bool indicationg success.
	 */
	bool getMappingsLocked(const std::string& keyPreference, std::unordered_map<std::string, std::string>* mappings);

	/// A mutex to protect database access.
	std::mutex m_databaseMutex;

	/// The underlying Arastorage database.
	alexaClientSDK::storage::araStorage::Database m_db;
};

}  // namespace bluetooth
}  // namespace capabilityAgents
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_CAPABILITYAGENTS_BLUETOOTH_INCLUDE_BLUETOOTH_ARABLUETOOTHSTORAGE_H_
