/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#pragma once

#include <stdlib.h>
#include "stdio.h"
#include "stdint.h"
#include "sys/types.h"
#include "aifw/aifw.h"

namespace aifw {

class AIModel;
class AIDataBufferNode;

/**
 * @class AIDataBuffer
 * @brief This class creates a doubly linked list and provides API to perform operations on that list.
 */
class AIDataBuffer
{
public:
	/**
	 * @brief Construct the AIDataBuffer class instance.
	 */
	AIDataBuffer();

	/**
	 * @brief AIDataBuffer destructor.
	 */
	~AIDataBuffer();

	/**
	 * @brief Read a row from data buffer.
	 * @param [out] buffer: Output buffer to copy data.
	 * @param [in] row: Index of row to read, 0 being latest row.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT readData(float *buffer, uint16_t row);

	/**
	 * @brief Read a row from column startCol to column endCol from data buffer.
	 * @param [out] buffer: Output buffer to copy data.
	 * @param [in] startCol: Column from where reading values from row will start.
	 * @param [in] endCol: Column upto which values will be read from row.
	 * @param [in] row: Index of row to read, 0 being latest row.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT readData(float *buffer, uint16_t startCol, uint16_t endCol, uint16_t row);

	/**
	 * @brief Gives number of filled rows in the streaming buffer.
	 * @return: Negative value indicates an error. Non negative value tells number of filled rows in buffer.
	 */
	uint16_t getRowCount();

	/**
	 * @brief Marks all nodes empty and sets number of filled nodes to 0 in AIDataBuffer
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clear(void);

	/**
	 * @brief Marks specific nodes empty, move them to the end of AIDataBuffer, and decrement number of filled nodes in AIDataBuffer
	 * @param [IN] offset: Offset of row to start clearing.
	 * @param [IN] count: Count of rows to clear.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clear(uint16_t offset, uint16_t count);

	friend class AIModel;
private:
	/**
	 * @brief Creates a doubly linked list with node count equal to row and node size equal to size.
	 * @param [in] row: Number of rows/nodes needed in streaming buffer.
	 * @param [in] size: Number of values in a single row/node.
	 * @return: AIFW_RESULT enum object. Before returning any error, it releases all the memory allocated.
	 */
	AIFW_RESULT init(uint16_t row, uint16_t size);

	/**
	 * @brief Modifies the streaming buffer.
	 * It compares row and size with previous set value of row and size and according to that it modifies the existing nodes or create more nodes or both.
	 * @param [in] row: Number of rows needed in the streaming buffer.
	 * @param [in] size: Number of values in a single row.
	 * @return: AIFW_RESULT enum object. In case of any error, previously allocated memory is not released.
	 */
	AIFW_RESULT reinit(uint16_t row, uint16_t size);

	/**
	 * @brief Deinitializes the streaming buffer.
	 * It clears the memory allocated to doubly linked list and resets class member variables.
	 */
	void deinit(void);

	/**
	 * @brief Writes a row into streaming buffer.
	 * Last node is detached and attached at the starting of the linked list. Values are then written in that node.
	 * @param [in] buffer: Input buffer from which data values are copied.
	 * @param [in] size: Number of values in input buffer.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT writeData(float *buffer, uint16_t size);

	/**
	 * @brief Writes values at 0th index row of streaming buffer starting from column specified by param offset.
	 * @param [in] buffer: Input buffer from which data values are copied.
	 * @param [in] size: Number of values in input buffer.
	 * @param [in] offset: Column from which writing values will start.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT writeData(float *buffer, uint16_t size, uint16_t offset);

	/**
	 * @brief Deletes a row data, marks that row empty and puts it at the end of the streaming buffer.
	 * @param [in] row: Index of row whose data needs to be deleted, 0 being latest row.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT deleteData(uint16_t row);

	/**
	 * @brief Clears the memory allocated to the list with head pointer equal to ptr.
	 * @param [in] ptr: Pointer to the head of list.
	 */
	void clearMemory(AIDataBufferNode *ptr);

	/**
	 * @brief Insert count number of nodes in a doubly linked list.
	 * @param [in] count: Number of nodes in the list.
	 * @param [in] size: Number of values in a single node.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT createList(uint16_t count, uint16_t size);

	AIDataBufferNode *mStart;
	AIDataBufferNode *mEnd;
	uint16_t mMaxRows;
	uint16_t mRowSize;
	uint16_t mRowCount;
	pthread_mutex_t mLock;
};

} // namespace aifw

