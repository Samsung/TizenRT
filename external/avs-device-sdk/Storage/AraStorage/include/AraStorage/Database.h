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

#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#include <arastorage/arastorage.h>
#include <string>
#include <thread>

#include "AraStorage/DatabaseMacros.h"
#include "AraStorage/Cursor.h"
#include "AraStorage/request/GeneralRequest.h"
#include "AraStorage/request/CreateRequest.h"

namespace alexaClientSDK {
namespace storage {
namespace araStorage {

/**
 * A provider class to manage database operations
 */
class Database
{
public:
	Database(const std::string filePath);
	~Database();

	/// Common interfaces: initialize, open, close.
	bool initialize();
	bool open();
	void close();

	/// Create table.
	int CreateTable(CreateRequest &request);

	/// Insert.
	int Insert(InsertRequest &request);

	/// Select.
	int Select(SelectRequest &request, Cursor &cursor);

	/// Delete.
	int Delete(DeleteRequest &request);

	/// Drop table.
	int DropTable(DropRequest &request);

	/// Utils interfaces below.

    /**
     * Get number of rows in a specified table.
     *
     * @param tableName The name of the table specified.
     * @param numberRows The number of rows outputed.
     * @return true on success, false on failure.
     */
	bool getNumberTableRows(const std::string &tableName, int *numberRows);

    /**
     * Get the maximum in values in a specified column of a specified table.
     *
     * @param tableName The name of the table specified.
     * @param columnName The name of the column specified.
     * @param maxInt The maximum in the given column.
     * @return true on success, false on failure.
     */
	bool getTableMaxIntValue(const std::string &tableName, const std::string &columnName, int *maxInt);

    /**
     * Check to see if a specified table exists.
     *
     * @param tableName The name of the table to check.
     * @return true if the table exists, flase if it doesn't or there is an error.
     */
    bool tableExists(const std::string &tableName);

    /**
     * Remove all the rows from the specified table.
     *
     * @param tableName The name of the table to clear.
     * @return true if successful, false if there was an error.
     */
    bool clearTable(const std::string &tableName);

	/**
	 * Drops a table from the database.
	 *
	 * @param tableName The name of the table to be dropped.
     * @return true if successful, false if there was an error.
	 */
	bool dropTable(const std::string &tableName);

    bool isDatabaseReady();

private:
	/// Decide retry limit count.
	bool db_initialize();
	bool IsRetryLimit(int &retryCount);
	db_cursor_t *DBQuery(char *cmd);
	db_result_t DBExecute(char *cmd);

	std::string m_filePath;
	int m_state;
	static bool m_isInitialized;
	static bool m_globalTableExist;
	static std::mutex m_opLock;
};

}
}
}
#endif // _DB_MANAGER_H_

