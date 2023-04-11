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
#include "lw_aifw/lw_aifw_result.h"
#include "lw_aifw/AIDataSource.h"

namespace lw_aifw {

class AICSVReader;
class AICSVDataSource : public AIDataSource {
private:
	uint16_t m_SensorCount;
	std::shared_ptr<AICSVReader> mAICSVReader;
public:
	AICSVDataSource(const char *sourceName);
	AICSVDataSource(float *datavalues, uint16_t sensorCount, uint16_t rowCount);
	~AICSVDataSource();
	std::shared_ptr<AICSVReader> getCSVReader(void);
	LW_AIFW_RESULT shareData(void* data);
	LW_AIFW_RESULT getDataAsync(void);
	static void onRawDataCollectedListener(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args);
};

} /* lw_aifw */

