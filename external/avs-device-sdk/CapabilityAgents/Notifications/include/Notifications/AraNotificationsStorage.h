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

#ifndef ALEXA_CLIENT_SDK_CAPABILITYAGENTS_NOTIFICATIONS_INCLUDE_NOTIFICATIONS_SQLITENOTIFICATIONSSTORAGE_H_
#define ALEXA_CLIENT_SDK_CAPABILITYAGENTS_NOTIFICATIONS_INCLUDE_NOTIFICATIONS_SQLITENOTIFICATIONSSTORAGE_H_

#include "Notifications/NotificationsStorageInterface.h"

#include <mutex>

#include <AVSCommon/Utils/Configuration/ConfigurationNode.h>
#include <AraStorage/Database.h>

namespace alexaClientSDK {
namespace capabilityAgents {
namespace notifications {

/**
 * An implementation that allows us to store NotificationIndicators using Arastorage.
 *
 */
class AraNotificationsStorage : public NotificationsStorageInterface {
public:
	/**
	 * Factory method for creating a storage object for Notifications based on an Arastorage database.
	 *
	 * @param configurationRoot The global config object.
	 * @return Pointer to the SQLiteMessagetStorge object, nullptr if there's an error creating it.
	 */
	static std::unique_ptr<AraNotificationsStorage> create(
		const avsCommon::utils::configuration::ConfigurationNode& configurationRoot);

	/**
	 * Constructor.
	 *
	 * @param dbFilePath The location of the Arastorage database file.
	 */
	AraNotificationsStorage(const std::string& databaseFilePath);

	~AraNotificationsStorage();

	bool createDatabase() override;

	bool open() override;

	void close() override;

	bool enqueue(const NotificationIndicator& notificationIndicator) override;

	bool dequeue() override;

	bool peek(NotificationIndicator* notificationIndicator) override;

	bool setIndicatorState(IndicatorState state) override;

	bool getIndicatorState(IndicatorState* state) override;

	bool checkForEmptyQueue(bool* empty) override;

	bool clearNotificationIndicators() override;

	bool getQueueSize(int* size) override;

private:
	/**
	 * Utility function to get the next record in the database. This method is not thread-safe.
	 * @param notificationIndicator A pointer to hold the next notificationIndicator in the database.
	 * @return Whether the getNext operation was successful.
	 */
	bool getNextNotificationIndicatorLocked(NotificationIndicator* notificationIndicator);
	bool popNotificationIndicatorLocked();

	/// A mutex to protect database access.
	std::mutex m_databaseMutex;

	/// The underlying database class.
	alexaClientSDK::storage::araStorage::Database m_database;

	std::vector<int> m_id;
};

}  // namespace notifications
}  // namespace capabilityAgents
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_CAPABILITYAGENTS_NOTIFICATIONS_INCLUDE_NOTIFICATIONS_SQLITENOTIFICATIONSSTORAGE_H_
