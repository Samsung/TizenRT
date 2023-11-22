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
 * @file AIProcessHandler.h
 * @brief Interface class for data processing operations.
 */

#pragma once

#include <memory>
#include "aifw/aifw.h"

namespace aifw {

class AIDataBuffer;

/**
 * @class AIProcessHandler
 * @brief Interface class for data processing operations.
 */
class AIProcessHandler
{
public:
	/**
	 * @brief: AIProcessHandler destructor.
	 */
	virtual ~AIProcessHandler()
	{
	}

	/**
	 * @brief Raw data values coming from data source present in param data are parsed in this function and filled in param parsedData.
	 * Required raw data values can be selected or new data can be created using different raw data values.
	 * @param [in] data: Input raw data. It is entire of sensor data set, developer should parse data from it.
	 * @param [in] count: Length of input raw data.
	 * @param [out] parsedData: Pointer of buffer to store parsed data. AIFW should write parsed data in AIDataBuffer which can be used later by other APIs of handler.
	 * @param [in] modelAttribute: Contains AIModelAttribute value of current AI Model.
	 * @return: AIFW_RESULT enum object. On success, AIFW_OK is returned.
	 * 			To skip inference, AIFW_INFERENCE_PROCEEDING is returned.
	 * 			On failure, a negative value is returned.
	 */
	virtual AIFW_RESULT parseData(void *data, uint16_t count, float *parsedData, AIModelAttribute *modelAttribute) = 0;

	/**
	 * @brief Performs preprocessing on data stored in AIDataBuffer before it is sent for invoke. Preprocessed data will not be updated on AIDataBuffer.
	 * @param [in] buffer: Pointer of AIDataBuffer. One row of AIDataBuffer includes parsed raw data and model invoke output. However, latest row only includes parsed raw data at this point of time.
	 * @param [out] invokeInput: Pointer of buffer to store preprocessed data. This buffer is later sent for invoke by AIFW.
	 * @param [in] modelAttribute: Contains AIModelAttribute value of current AI Model.
	 * @return: AIFW_RESULT enum object. On success, AIFW_OK is returned.
	 * 			On failure, a negative value is returned.
	 */
	virtual AIFW_RESULT preProcessData(std::shared_ptr<AIDataBuffer> buffer, float *invokeInput, AIModelAttribute *modelAttribute) = 0;

	/**
	 * @brief Performs postprocessing on data stored in AIDataBuffer after invoke. Postprocessed data will not be updated on AIDataBuffer.
	 * @param [in] buffer: Pointer of AIDataBuffer. At this point of time, latest row includes parsed raw data as well as invoke output.
	 * @param [out] resultData: Pointer of buffer to store postprocessed data. This will later be used in ensembling operation by inference handler.
	 * @param [in] modelAttribute: Contains AIModelAttribute value of current AI Model.
	 * @return: AIFW_RESULT enum object. On success, AIFW_OK is returned.
	 * 			On completion of modelSet inference cycle, AIFW_INFERENCE_FINISHED is returned.
	 * 			To skip ensembling, AIFW_INFERENCE_PROCEEDING is returned.
	 * 			On failure, a negative value is returned.
	 */
	virtual AIFW_RESULT postProcessData(std::shared_ptr<AIDataBuffer> buffer, float *resultData, AIModelAttribute *modelAttribute) = 0;
};

} /* namespace aifw */

