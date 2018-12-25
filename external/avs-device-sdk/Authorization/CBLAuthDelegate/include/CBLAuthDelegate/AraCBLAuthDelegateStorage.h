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

#ifndef ALEXA_CLIENT_SDK_AUTHORIZATION_CBLAUTHDELEGATE_INCLUDE_CBLAUTHDELEGATE_ARACBLAUTHDELEGATESTORAGE_H_
#define ALEXA_CLIENT_SDK_AUTHORIZATION_CBLAUTHDELEGATE_INCLUDE_CBLAUTHDELEGATE_ARACBLAUTHDELEGATESTORAGE_H_

#include <mutex>
#include <string>

#include "AraStorage/Database.h"

#include "CBLAuthDelegate/CBLAuthDelegateStorageInterface.h"

namespace alexaClientSDK {
namespace authorization {
namespace cblAuthDelegate {

/**
 * An implementation of CBLAuthDelegateStorageInterface using Arastorage.
 */
class AraCBLAuthDelegateStorage : public CBLAuthDelegateStorageInterface {
public:
	/**
	 * Factory method for creating a storage object for CBLAuthDelegate based on an Arastorage database.
	 *
	 * @param configurationRoot The global config object.
	 * @return Pointer to the AraCBLAuthDelegateStorage object, nullptr if there's an error creating it.
	 */
	static std::unique_ptr<AraCBLAuthDelegateStorage> create(
		const avsCommon::utils::configuration::ConfigurationNode& configurationRoot);

	/**
	 * Destructor
	 */
	~AraCBLAuthDelegateStorage();

	/// @name CBLAuthDelegateStorageInterface method overrides.
	/// @{
	bool createDatabase() override;
	bool open() override;
	bool setRefreshToken(const std::string& refreshToken) override;
	bool clearRefreshToken() override;
	bool getRefreshToken(std::string* refreshToken) override;
	bool clear() override;
	/// @}

private:
	/**
	 * Constructor.
	 */
	AraCBLAuthDelegateStorage(const std::string& databaseFilePath);

	/**
	 * Close the database.
	 */
	void close();

	/// Mutex with which to serialize database operations.
	std::mutex m_mutex;
	/// Hold current token in memory, to avoid writing the same token to database twice.
	std::string m_refreshToken;
	/// The underlying database class.
	alexaClientSDK::storage::araStorage::Database m_database;
};

}  // namespace cblAuthDelegate
}  // namespace authorization
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_AUTHORIZATION_CBLAUTHDELEGATE_INCLUDE_CBLAUTHDELEGATE_ARACBLAUTHDELEGATESTORAGE_H_
