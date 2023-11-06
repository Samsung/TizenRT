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
#include "include/aifw_csv_reader_utils.h"

#define nullptr ((void*)0)

static AIFW_RESULT readLine(FILE *fp, char *line, uint16_t linesize)
{
	if (!fgets(line, linesize, fp)) {
		if (feof(fp)) {
			AIFW_LOGE("reached eof");
			return AIFW_SOURCE_EOF;
		}
		AIFW_LOGE("File read operation failed errno : %d", errno);
		return AIFW_ERROR_FILE_ACCESS;
	}
	AIFW_LOGV("read line success %s length %d", line, strlen(line));
	return AIFW_OK;
}

FILE *csvOpen(const char *filename)
{
	if (!filename) {
		AIFW_LOGE("filename is null");
		return NULL;
	}
	return fopen(filename, "r");
}

AIFW_RESULT csvClose(FILE *fp)
{
	if (!fp) {
		AIFW_LOGE("file handle param is NULL");
		return AIFW_INVALID_ARG;
	}
	int ret = fclose(fp);
	if (ret != OK) {
		AIFW_LOGE("file close error errno : %d", errno);
		return AIFW_ERROR;
	}
	return AIFW_OK;
}

AIFW_RESULT getCSVLine(FILE *fp, char *data, uint16_t size)
{
	if (!fp) {
		AIFW_LOGE("file handle param is NULL");
		return AIFW_INVALID_ARG;
	}
	if (!data) {
		AIFW_LOGE("output buffer is null");
		return AIFW_INVALID_ARG;
	}
	if (size == 0) {
		AIFW_LOGE("output buffer size is zero");
		return AIFW_INVALID_ARG;
	}
	AIFW_RESULT result;
	result = readLine(fp, data, size);
	if (result != AIFW_OK) {
		AIFW_LOGE("File read operation failed. ret : %d", result);
	} else {
		AIFW_LOGV("File read operation OK");
	}
	return result;
}

AIFW_RESULT getValue(FILE *fp, char *columnBuffer, void *data, CSV_VALUE_DATA_TYPE_E datatype)
{
	if (!fp) {
		AIFW_LOGE("file handle param is NULL");
		return AIFW_INVALID_ARG;
	}
	if (!columnBuffer) {
		AIFW_LOGE("character column buffer is NULL");
		return AIFW_INVALID_ARG;
	}
	if (!data) {
		AIFW_LOGE("pointer to output value is NULL");
		return AIFW_INVALID_ARG;
	}
	if (datatype < INT8 || datatype > FLOAT32) {
		AIFW_LOGE("Input type is invalid");
		return AIFW_INVALID_ARG;
	}
	int character = getc(fp);
	if (character == EOF) {
		if (feof(fp)) {
			AIFW_LOGE("reached eof");
			return AIFW_SOURCE_EOF;
		}
		AIFW_LOGE("File read operation failed errno : %d", errno);
		return AIFW_ERROR_FILE_ACCESS;
	}
	uint16_t offset = 0;
	while (character != ',' && character != '\n' && character != '\r' && character != '\0') {
		if (character == EOF) {
			if (ferror(fp)) {
				AIFW_LOGE("File read operation failed errno : %d", errno);
				return AIFW_ERROR_FILE_ACCESS;
			}
			AIFW_LOGV("reached eof");
			break;
		}
		columnBuffer[offset++] = character;
		character = getc(fp);
	}
	columnBuffer[offset] = '\0';
	switch (datatype) {
	case INT8:
		*(int8_t *)data = (int8_t)atoi(columnBuffer);
		break;
	case UINT8:
		*(uint8_t *)data = (uint8_t)atoi(columnBuffer);
		break;
	case INT16:
		*(int16_t *)data = (int16_t)atoi(columnBuffer);
		break;
	case INT32:
		*(int32_t *)data = atoi(columnBuffer);
		break;
	case FLOAT32:
		*(float *)data = atof(columnBuffer);
		break;
	default:
		break;
	}
	AIFW_LOGV("Read Value success");
	return AIFW_OK;
}

