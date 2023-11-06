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

#include "aifw/aifw_log.h"
#include "aifw/aifw_csv_reader.h"
#include "include/aifw_csv_reader_utils.h"

#define GET_MAX_CHARACTERS_PER_VALUE(maxChar, type)	\
	{						\
		switch (type) {				\
		case INT8:				\
			maxChar = INT_8;		\
			break;				\
		case UINT8:				\
			maxChar = UINT_8;		\
			break;				\
		case INT16:				\
			maxChar = INT_16;		\
			break;				\
		case INT32:				\
			maxChar = INT_32;		\
			break;				\
		case FLOAT32:				\
			maxChar = FLOAT_32;		\
			break;				\
		default:				\
			maxChar = NOT_INITIALIZED;	\
			break;				\
		}					\
	}

static void clearMemory(void **handle)
{
	CSVHandle *csvHandle = (CSVHandle *)(*handle);
	if (csvHandle->columnBuffer) {
		free(csvHandle->columnBuffer);
		csvHandle->columnBuffer = NULL;
	}
	if (csvHandle->lineBuffer) {
		free(csvHandle->lineBuffer);
		csvHandle->lineBuffer = NULL;
	}
	if (csvHandle) {
		free(csvHandle);
		csvHandle = NULL;
		*handle = NULL;
	}
}

static void fillValue(void *buffer, CSV_VALUE_DATA_TYPE_E type, uint16_t index, char *value)
{
	switch (type) {
	case INT8:
		*((int8_t *)buffer + index) = (int8_t)atoi(value);
		break;
	case UINT8:
		*((uint8_t *)buffer + index) = (uint8_t)atoi(value);
		break;
	case INT16:
		*((int16_t *)buffer + index) = (int16_t)atoi(value);
		break;
	case INT32:
		*((int32_t *)buffer + index) = atoi(value);
		break;
	case FLOAT32:
		*((float *)buffer + index) = atof(value);
		break;
	default:
		break;
	}
}

static char *skipSpace(char *ptr)
{
	while (*ptr && isspace((int)*ptr)) {
		ptr++;
	}
	return ptr;
}

static uint16_t calculateColumnCount(FILE *handle)
{
	int columnCount = 0;
	int character = fgetc(handle);
	if (character == EOF || character == '\n' || character == '\r' || character == '\0') {
		return columnCount;
	}
	while (character != EOF && character != '\n' && character != '\r' && character != '\0') {
		if (character == ',') {
			columnCount++;
		}
		character = fgetc(handle);
	}
	columnCount++;
	return columnCount;
}

AIFW_RESULT csvInit(void **csvHandle, const char *filename, CSV_VALUE_DATA_TYPE_E dataType, bool hasHeader)
{
	if (!csvHandle) {
		AIFW_LOGE("Double pointer to CSV Handle is NULL.");
		return AIFW_INVALID_ARG;		
	}
	if (!filename) {
		AIFW_LOGE("file name is null");
		return AIFW_INVALID_ARG;
	}
	if (dataType < INT8 || dataType > FLOAT32) {
		AIFW_LOGE("Input type is invalid");
		return AIFW_INVALID_ARG;
	}
	*csvHandle = malloc(sizeof(CSVHandle));
	if (!*csvHandle) {
		AIFW_LOGE("Memory allocation failed for csv handle");
		return AIFW_NO_MEM;
	}
	CSVHandle *handle = (CSVHandle *)(*csvHandle);
	handle->columnBuffer = NULL;
	handle->lineBuffer = NULL;
	handle->fileHandle = csvOpen(filename);
	if (!handle->fileHandle) {
		AIFW_LOGE("file open failed errno: %d", errno);
		clearMemory(csvHandle);
		return AIFW_ERROR_FILE_ACCESS;
	}
	handle->columnCount = calculateColumnCount(handle->fileHandle);
	if (handle->columnCount == 0) {
		AIFW_LOGE("Total column count in csv is 0.");
		csvDeinit(csvHandle);
		return AIFW_ERROR;
	}
	handle->columnDataType = dataType;
	GET_MAX_CHARACTERS_PER_VALUE(handle->maxCharPerColumn, dataType);
	handle->lineCounter = 0;
	AIFW_LOGV("Total columns count: %d", handle->columnCount);
	if (!hasHeader && fseek(handle->fileHandle, 0, SEEK_SET)) {
		AIFW_LOGE("Seeking file pointer to start of file failed.");
		csvDeinit(csvHandle);
		return AIFW_ERROR;
	}
	handle->columnBuffer = (char *)malloc(handle->maxCharPerColumn * sizeof(char));
	if (!handle->columnBuffer) {
		AIFW_LOGE("Memory allocation failed for column buffer");
		csvDeinit(csvHandle);
		return AIFW_NO_MEM;
	}
	handle->lineBuffer = (char *)malloc(handle->columnCount * handle->maxCharPerColumn * sizeof(char));
	if (!handle->lineBuffer) {
		AIFW_LOGE("Memory allocation failed for line buffer");
		csvDeinit(csvHandle);
		return AIFW_NO_MEM;
	}
	return AIFW_OK;
}

AIFW_RESULT csvDeinit(void **handle)
{
	if (!handle) {
		AIFW_LOGE("Double pointer to CSV Handle is NULL.");
		return AIFW_INVALID_ARG;		
	}
	if (*handle) {
		CSVHandle *csvHandle = (CSVHandle *)(*handle);
		csvHandle->lineCounter = 0;
		AIFW_RESULT res = csvClose(csvHandle->fileHandle);
		clearMemory(handle);
		if (res != AIFW_OK) {
			AIFW_LOGE("CSV Close error. ret: %d", res);
			return res;
		}
	}
	return AIFW_OK;
}

AIFW_RESULT readCSVData(void *handle, void *buffer)
{
	if (!handle) {
		AIFW_LOGE("Pointer to CSV Handle is NULL");
		return AIFW_INVALID_ARG;		
	}
	if (!buffer) {
		AIFW_LOGE("Output values buffer is NULL");
		return AIFW_INVALID_ARG;		
	}
	CSVHandle *csvHandle = (CSVHandle *)handle;
	uint16_t linesize = csvHandle->columnCount * csvHandle->maxCharPerColumn * sizeof(char);

read_line_again:
	memset(csvHandle->columnBuffer, '\0', csvHandle->maxCharPerColumn * sizeof(char));
	memset(csvHandle->lineBuffer, '\0', linesize);
	AIFW_RESULT res = getCSVLine(csvHandle->fileHandle, csvHandle->lineBuffer, linesize);
	if (res != AIFW_OK) {
		AIFW_LOGE("Storing csv line data into a char array failed. ret: %d", res);
		return res;
	}
	csvHandle->lineCounter++;
	AIFW_LOGV("Lines read %d", csvHandle->lineCounter);

	/* First, let's check line is blank or not */
	char *ptr = csvHandle->lineBuffer;
	ptr = skipSpace(ptr);
	if (*ptr == '\r' || *ptr == '\n' || *ptr == '\0') {
		AIFW_LOGE("Line number %d is blank", csvHandle->lineCounter);
		goto read_line_again;
	}

	/* Check entire of contents in read line */
	uint16_t columnCounter = 0;
	while(columnCounter < csvHandle->columnCount) {
		/* Lets check if there is any invalid value in csv line */
		ptr = skipSpace(ptr);
		if (*ptr == ',' || *ptr == '\r' || *ptr == '\n' || *ptr == '\0') {
			AIFW_LOGE("invalid value at line number %d and column number %d", csvHandle->lineCounter, columnCounter + 1);
			goto read_line_again;
		}

		/* Copy data to buffer */
		uint16_t offset = 0;
		while(*ptr != ',' && *ptr != '\r' && *ptr != '\n' && *ptr != '\0') {
			csvHandle->columnBuffer[offset++] = *ptr;
			ptr++;
		}
		csvHandle->columnBuffer[offset] = '\0';
		fillValue(buffer, csvHandle->columnDataType, columnCounter, csvHandle->columnBuffer);
		columnCounter++;
		if (*ptr != ',') {
			break;
		}
		ptr++;
	}
	return AIFW_OK;
}

AIFW_RESULT getColumnCount(void *handle, uint16_t *columnCountPtr)
{
	if (!handle) {
		AIFW_LOGE("Pointer to CSV Handle is NULL");
		return AIFW_INVALID_ARG;		
	}
	if (!columnCountPtr) {
		AIFW_LOGE("Pointer to column count variable is NULL");
		return AIFW_INVALID_ARG;		
	}
	CSVHandle *csvHandle = (CSVHandle *)handle;
	*columnCountPtr = csvHandle->columnCount;
	return AIFW_OK;
}

