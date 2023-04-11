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

#include "lw_aifw/lw_aifw_log.h"
#include "include/AICSVFileReader.h"

//#define CSVREADER_DEBUG 1
namespace lw_aifw {

AICSVFileReader::AICSVFileReader(const char* filename) : AICSVReader(), mFileName(filename), mLineNumber(0),  mTotalColumns(0), mFile(NULL), mPosition(0)
{
	LW_AIFW_LOGV("AICSVFileReader::AICSVFileReader");
}

AICSVFileReader::~AICSVFileReader()
{
	LW_AIFW_LOGV("AICSVFileReader::~AICSVFileReader");
}

LW_AIFW_RESULT AICSVFileReader::init(void)
{
	if (LW_AIFW_OK != readLine()) {
		LW_AIFW_LOGE("read header line failed");
		return LW_AIFW_ERROR;
	}
	char* ptr = skipSpace(mLine);
	if (*ptr == '\r' || *ptr == '\n' || *ptr == '\0') {
		LW_AIFW_LOGE("read header line failed");
		return LW_AIFW_ERROR;
	}
	mTotalColumns = parseCSVLine(ptr);
	if (mTotalColumns <= 0) {
		LW_AIFW_LOGE("error column count %d", mTotalColumns);
		return LW_AIFW_ERROR;
	}
	LW_AIFW_LOGV("columns count %d", mTotalColumns);
	return LW_AIFW_OK;
}

char *AICSVFileReader::skipSpace(char *ptr)
{
	while (*ptr && isspace((int)*ptr)) {
		ptr++;
	}
	return ptr;
}

char *AICSVFileReader::copyParam(char *src, char *dest)
{
	int i;
	/* Copy it into the parameter array */
	for (i = 0; i < MAX_PARAMSIZE; i++) {
		if (*src == ',' || *src == '\n' || *src == '\0' || *src == '\r') {
			dest[i] = '\0';
			if (*src == ',') {
				return ++src;
			} else {
				return NULL;
			}
		} else {
			dest[i] = *src++;
		}
	}
	dest[i] = '\0';
	return NULL;
}

char *AICSVFileReader::findParam(char *ptr)
{
	ptr = skipSpace(ptr);
	if (*ptr == '\r' || *ptr == '\n' || *ptr == '\0') {
		return NULL;
	}
	return ptr;
}

int AICSVFileReader::getTotalColumns(void)
{
	return mTotalColumns;
}

char *AICSVFileReader::getParam(unsigned int field)
{
	if (field >= MAX_FIELDS) {
		return NULL;
	}
	LW_AIFW_LOGV("get parm is , %s",mParams[field]);
	return mParams[field];
}

int AICSVFileReader::getCurrentLineNo(void)
{
	LW_AIFW_LOGV("line no is  %d",mLineNumber);
	return mLineNumber;
}

char *AICSVFileReader::getLine(void)
{
	LW_AIFW_LOGV("g_line is : %s",mLine);
	return mLine;
}

LW_AIFW_RESULT AICSVFileReader::readLine(void)
{
	mFile = fopen(mFileName, "r");
	if (mFile == NULL) {
		LW_AIFW_LOGE("File open operation failed errno : %d", errno);
		return LW_AIFW_ERROR_FILE_ACCESS;
	}
	LW_AIFW_RESULT ret = LW_AIFW_OK;
	fsetpos(mFile, &mPosition);
	mLine[LINESIZE] = '\0';
	if (!fgets(mLine, LINESIZE, mFile)) {
		if (feof(mFile)) {
			LW_AIFW_LOGE("reached eof");
			ret = LW_AIFW_SOURCE_EOF;
		} else {
			LW_AIFW_LOGE("File read operation failed errno : %d", errno);
			ret = LW_AIFW_ERROR_FILE_ACCESS;
		}
	} else {
		fgetpos(mFile, &mPosition);
		mLineNumber++;
#ifdef CSVREADER_DEBUG
		LW_AIFW_LOGV("Line: %s", mLine);
#endif
	}
	fclose(mFile);
	return ret;
}

int AICSVFileReader::parseCSVLine(char *ptr)
{
	if (!ptr) {
		LW_AIFW_LOGV("No line, skip parsing\n");
		return 0;
	}
	int nparms = 0;

	/* Copy each comma-separated value in an array */
	do {
		if (nparms >= MAX_FIELDS) {
			LW_AIFW_LOGE("nparms overflowed! nparms : %d max params %d", nparms, MAX_FIELDS);
			break;
		}
		ptr = copyParam(ptr, &mParams[nparms][0]);
		nparms++;
		if (ptr) {
#ifdef CSVREADER_DEBUG
			LW_AIFW_LOGV("%s",ptr);
#endif
			ptr = findParam(ptr);
		}
	} while (ptr);

#ifdef CSVREADER_DEBUG
	LW_AIFW_LOGV("Parameters: %d", nparms);
	for (int i = 0; i < nparms; i++) {
		LW_AIFW_LOGV("Param %d: %s", i + 1, mParams[i]);
	}
#endif
	return nparms;
}

LW_AIFW_RESULT AICSVFileReader::getSourceData(void)
{
	float data[8];
	int cnt_keys = 0;
	LW_AIFW_RESULT result = readLine();
	if (result == LW_AIFW_OK) {
		char* ptr = skipSpace(mLine);
		if (*ptr == '\r' || *ptr == '\n' || *ptr == '\0') {
			LW_AIFW_LOGE("after skip space empty line");
			result = LW_AIFW_ERROR;
		} else {
			cnt_keys = parseCSVLine(ptr);
			for(uint16_t j = 0; j < cnt_keys; j++) {
				char *str = getParam(j);
				data[j] = atof(str);
				LW_AIFW_LOGV("CSV data value %f", data[j]);
			}
		}
	} else if (result == LW_AIFW_SOURCE_EOF) {
		LW_AIFW_LOGE("Reached EoF");
	} else {
		LW_AIFW_LOGE("get source data returning error");
	}
	mRawDataCollectedListener(result, (float *)data, cnt_keys, mRawDataCollectedCBArgs);
	return LW_AIFW_OK;
}

} /* lw_aifw */
