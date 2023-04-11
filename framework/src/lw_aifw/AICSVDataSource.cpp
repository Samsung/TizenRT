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

#include "lw_aifw/AICSVDataSource.h"
#include "lw_aifw/lw_aifw_log.h"
#include "lw_aifw/AICSVFileReader.h"
#include "lw_aifw/AICSVArrayReader.h"

namespace lw_aifw {
	
	AICSVDataSource::AICSVDataSource(const char *sourceName) : AIDataSource(), m_SensorCount(0)
	{
		LW_AIFW_LOGV("CSV file: %s", sourceName);
		mAICSVReader = std::shared_ptr<AICSVReader>(new AICSVFileReader(sourceName));
		mAICSVReader->setRawDataCollectedListener(AICSVDataSource::onRawDataCollectedListener, (void*)this);
		LW_AIFW_RESULT res = mAICSVReader->init();
		if (LW_AIFW_OK != res) {
			LW_AIFW_LOGE("CSV reader init failed: error, %d", res);
		}
	}

	AICSVDataSource::AICSVDataSource(float *datavalues, uint16_t sensorCount, uint16_t rowCount) : AIDataSource(), m_SensorCount(sensorCount)
	{
		mAICSVReader = std::shared_ptr<AICSVReader>(new AICSVArrayReader(datavalues, sensorCount, rowCount));
		mAICSVReader->setRawDataCollectedListener(AICSVDataSource::onRawDataCollectedListener, (void*)this);
	}

	AICSVDataSource::~AICSVDataSource()
	{
		LW_AIFW_LOGV("Destructor");
	}

	LW_AIFW_RESULT AICSVDataSource::shareData(void* data)
	{
		return LW_AIFW_OK;
	}

	LW_AIFW_RESULT AICSVDataSource::getDataAsync()
	{
		return mAICSVReader->getSourceData();
	}

	void AICSVDataSource::onRawDataCollectedListener(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args)
	{
		((AICSVDataSource*)args)->mAIDataCollectedListener(result, (float *)data, dataCount, ((AICSVDataSource*)args)->mAIDataCollectedCBArgs);
	}

	std::shared_ptr<AICSVReader> AICSVDataSource::getCSVReader(void)
	{
		return mAICSVReader;
	}
	
} /* lw_aifw */

