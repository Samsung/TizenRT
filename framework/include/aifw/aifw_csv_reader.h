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

/**
 * @file aifw_test/aifw_csv_reader.h
 * @brief APIs to read a csv file.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "aifw/aifw.h"

typedef enum _CSV_VALUE_DATA_TYPE_E {
	INT8,
	UINT8,
	INT16,
	INT32,
	FLOAT32,
	DATATYPE_NOT_INITIALIZED = -1
} CSV_VALUE_DATA_TYPE_E;

/**
 * Number of characters for minimum / maximum value.
 * Value defined here doesnot include null terminating character.
 */
typedef enum _MAX_CHAR_COUNT_E {
	INT_8 = 4,		/* -128 ~ 127 */
	UINT_8 = 3,		/* 0 ~ 256 */
	INT_16 = 6,		/* -32768 ~ 32767 */
	INT_32 = 11,		/* -2147483648 ~ 2147483647 */
	FLOAT_32 = 24,		/* -1.7976931348623158e+308 ~ */
	NOT_INITIALIZED = 0
} MAX_CHAR_COUNT_E;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the csv data source reader.
 * Opens the csv file with name filename and stores FILE pointer for opened CSV. Also it allocates memory to required buffers as per column count and its data type.
 * @param [in] filename: Name of csv file to open.
 * @param [in] dataType: Data type of column data values.
 * @param [in] hasHeader: false(header is not present in csv) or true(header is present in csv)
 * @param [out] csvHandle: Void double pointer to a structure containing necessary fields of a csv.
 *                         This contains data required for operation in CSV read. These are required by aifw_csv_reader and will be passed by application in further APIs
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT csvInit(void **csvHandle, const char *filename, CSV_VALUE_DATA_TYPE_E dataType, bool hasHeader);

/**
 * @brief De-init the csv data source reader.
 * Closes the csv file and clears the allocated memory.
 * @param [in] handle: Void pointer to csv handle structure.
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT csvDeinit(void **handle);

/**
 * @brief Reads a CSV line, parses it and fills data values in buffer pointed by "buffer".
 * @param [in] handle: Void pointer to csv handle structure.
 * @param [out] buffer: Buffer to hold the data values of csv.
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT readCSVData(void *handle, void *buffer);

/**
 * @brief Get total number of columns in CSV.
 * @param [in] handle: Void pointer to csv handle structure.
 * @param [out] columnCountPtr: Pointer to a variable storing column count.
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT getColumnCount(void *handle, uint16_t *columnCountPtr);

#ifdef __cplusplus
}
#endif

