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
#include "include/AICSVArrayReader.h"

namespace lw_aifw {

	AICSVArrayReader::AICSVArrayReader(float *datavalues, uint16_t sensorCount, uint16_t rowCount) : AICSVReader(), m_Header(NULL), m_SensorLength(0), m_DataValues(datavalues), m_SensorCount(sensorCount), m_RowCount(rowCount), m_RowOffset(0)
	{
		LW_AIFW_LOGV("sensorCount %d rowCount %d", sensorCount, rowCount);
	}

	AICSVArrayReader::~AICSVArrayReader()
	{
		LW_AIFW_LOGV("Destructor.");
	}

	LW_AIFW_RESULT AICSVArrayReader::init(void){
		return LW_AIFW_OK;
	}

	LW_AIFW_RESULT AICSVArrayReader::read(float *&values, uint16_t &numberofValues)
	{
		LW_AIFW_LOGV("Row Offset %d", m_RowOffset);
		if (m_RowOffset >= m_RowCount) {
			LW_AIFW_LOGV("completed array read row offset %d", m_RowOffset);
			return LW_AIFW_SOURCE_EOF;
		}
		//values = m_DataValues + (m_RowOffset * (m_SensorCount + 2));
		values = m_DataValues + (m_RowOffset * (m_SensorCount));
		m_RowOffset++;
		numberofValues = m_SensorCount;
		return LW_AIFW_OK;
	}

	uint16_t AICSVArrayReader::getSensorCount (void) const
	{
		return m_SensorCount;
	}

	LW_AIFW_RESULT AICSVArrayReader::getSourceData(void)
	{
		float *values = NULL;
		float data[8];
		uint16_t numberofValues = 0;
		LW_AIFW_RESULT result = read(values, numberofValues);
		if (result == LW_AIFW_OK) {
			LW_AIFW_LOGV("%d values read", numberofValues);
			for (uint16_t i = 0; i < numberofValues; i++) {
				data[i] = *(values + i);
				LW_AIFW_LOGV("index: %d value: %f", i, data[i]);
			}
		} else {
			LW_AIFW_LOGE("Error happened ret: %d", result);
		}
		mRawDataCollectedListener(result, (float *)data, numberofValues, mRawDataCollectedCBArgs);
		return LW_AIFW_OK;
	}	
		
} /* lw_aifw */

