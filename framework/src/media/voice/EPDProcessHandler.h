/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include <memory>
#include "aifw/aifw.h"
#include "aifw/AIProcessHandler.h"

/**
 * @brief This class implements operations related to inference. It prepares data in parseData function.
 * Pre-processes data in preProcessData
 * Performs post-processes operation in postProcessData.
 * Class declares 2 buffers to store data used in pre-process and post-process operations.
 * These all operations are specific to a model. 
*/
class EPDProcessHandler : public aifw::AIProcessHandler
{
public:
	/**
	 * @brief EPDProcessHandler constructor.
	*/
	EPDProcessHandler();

	/**
	 * @brief EPDProcessHandler destructor.
	*/
	~EPDProcessHandler();

	/**
	 *! @copydoc AIProcessHandler::parseData()
	*/
	AIFW_RESULT parseData(void *data, uint16_t count, float *parsedData, AIModelAttribute *modelAttribute);

	/**
	 *! @copydoc AIProcessHandler::preProcessData()
	*/
	AIFW_RESULT preProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, uint16_t countInputSets, float **invokeInput, AIModelAttribute *modelAttribute);

	/**
	 *! @copydoc AIProcessHandler::postProcessData()
	*/	
	AIFW_RESULT postProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *resultData, AIModelAttribute *modelAttribute);
};
