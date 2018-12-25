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

#ifndef ALEXA_CLIENT_SDK_STORAGE_SQLITESTORAGE_INCLUDE_SQLITESTORAGE_SQLITEMISCSTORAGE_H_
#define ALEXA_CLIENT_SDK_STORAGE_SQLITESTORAGE_INCLUDE_SQLITESTORAGE_SQLITEMISCSTORAGE_H_

#include <AVSCommon/SDKInterfaces/Storage/MiscStorageInterface.h>
#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include <AraStorage/Database.h>

namespace alexaClientSDK {
namespace storage {
namespace araStorage {

/**
 * A class that provides a Arastorage implementation of MiscStorage database.
 */
class AraMiscStorage : public avsCommon::sdkInterfaces::storage::MiscStorageInterface {
public:
	/**
	 * Factory method for creating a storage object for a Arastorage database.
	 * Note that the actual database will not be created by this function.
	 *
	 * @param configurationRoot The global config object.
	 * @return Pointer to the SQLiteAlertStorage object, nullptr if there's an error creating it.
	 */
	static std::unique_ptr<AraMiscStorage> create(
		const avsCommon::utils::configuration::ConfigurationNode& configurationRoot);

	/**
	 * Destructor
	 */
	~AraMiscStorage();

	/// @name MiscStorageInterface method overrides.
	/// @{
	bool createDatabase() override;

	bool open() override;

	void close() override;

	bool createTable(
		const std::string& componentName,
		const std::string& tableName,
		KeyType keyType,
		ValueType valueType) override;

	bool clearTable(const std::string& componentName, const std::string& tableName) override;

	bool deleteTable(const std::string& componentName, const std::string& tableName) override;

	bool get(const std::string& componentName, const std::string& tableName, const std::string& key, std::string* value)
		override;

	bool add(
		const std::string& componentName,
		const std::string& tableName,
		const std::string& key,
		const std::string& value) override;

	bool update(
		const std::string& componentName,
		const std::string& tableName,
		const std::string& key,
		const std::string& value) override;

	bool put(
		const std::string& componentName,
		const std::string& tableName,
		const std::string& key,
		const std::string& value) override;

	bool remove(const std::string& componentName, const std::string& tableName, const std::string& key) override;

	bool tableEntryExists(
		const std::string& componentName,
		const std::string& tableName,
		const std::string& key,
		bool* tableEntryExistsValue) override;

	bool tableExists(const std::string& componentName, const std::string& tableName, bool* tableExistsValue) override;

	bool load(
		const std::string& componentName,
		const std::string& tableName,
		std::unordered_map<std::string, std::string>* valueContainer) override;
	/// @}

private:
	/**
	 * Constructor.
	 *
	 * @param dbFilePath The location of the Arastorage database file.
	 */
	AraMiscStorage(const std::string& dbFilePath);

	/// The underlying database class.
	alexaClientSDK::storage::araStorage::Database m_db;
};

}  // namespace sqliteStorage
}  // namespace storage
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_STORAGE_SQLITESTORAGE_INCLUDE_SQLITESTORAGE_SQLITEMISCSTORAGE_H_
