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
 * @file aifw_test/aifw_csv_reader_utils.h
 * @brief utils to read a CSV file. 
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "stdint.h"
#include "aifw/aifw.h"
#include "aifw/aifw_csv_reader.h"

/**
 * @struct _CSVHandle_s
 * @brief This structure defines necessary fields of a CSV required in read operation.
*/
typedef struct _CSVHandle_s {
    FILE *fileHandle;                           // File Handler of CSV
    char *columnBuffer;                         // Char buffer to store one CSV column value
    char *lineBuffer;                           // Char buffer to store one CSV line
    uint16_t columnCount;                       // Number of columns in one CSV line
    CSV_VALUE_DATA_TYPE_E columnDataType;       // Data type of column
    MAX_CHAR_COUNT_E maxCharPerColumn;          // Maximum number of characters in a column
    uint16_t lineCounter;                       // To count number of CSV lines read till now
} CSVHandle;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opens filename.csv.
 * @param [in] filename: Name of csv file to open.
 * @return: Returns FILE pointer for opened csv.
 */
FILE *csvOpen(const char *filename);

/**
 * @brief Closes the FILE stream corresponding to FILE* fp.
 * @param [in] fp: FILE pointer of the stream to be closed.
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT csvClose(FILE *fp);

/**
 * @brief Reads a line from the FILE stream corresponding to FILE* fp and stores it into the buffer pointed to by data.
 * It stops when either (size-1) characters are read, the newline character is read, or EOF is reached, whichever comes first.
 * @param [in] fp: FILE pointer of the stream.
 * @param [out] data: char array to store line data
 * @param [in] size: size of char array
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT getCSVLine(FILE *fp, char *data, uint16_t size);

/**
 * @brief Gets the next character from the FILE stream corresponding to FILE* fp till next ',' or '\0' or '\n' or '\r' or EOF and fills them into the buffer pointed to by columnBuffer.
 * Parse stream of characters stored in columnBuffer as per parameter 4 and fill in parameter 3
 * @param [in] fp: FILE pointer of the stream.
 * @param [out] columnBuffer: char array to store a csv line column.
 * @param [out] data: buffer to store parsed csv line column value.
 * @return: AIFW_RESULT enum object.
 */
AIFW_RESULT getValue(FILE *fp, char *columnBuffer, void *data, CSV_VALUE_DATA_TYPE_E datatype);

#ifdef __cplusplus
}
#endif

