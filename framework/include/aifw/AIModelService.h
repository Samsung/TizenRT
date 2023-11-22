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
 * @file aifw/AIModelService.h
 * @brief AIModelService class uses TizenRT software timer to invoke data request at a set interval.
 */

#pragma once

#include <memory>
#include <aifw/aifw_timer.h>
#include "aifw/aifw.h"
#include "aifw/AIInferenceHandler.h"

namespace aifw {

/**
 * @class AIModelService
 * @brief AIModelService class uses TizenRT software timer to invoke data request at a set interval.
 */
class AIModelService
{
public:
	/**
	 * @brief Constructs the AIModelService class instance.
	 * It initializes the data members of the class.
	 * @param [in] collectRawDataCallback: AI Framework calls this function when timer expires to collect the raw data and pass it for inference.
	 * @param [in] inferenceHandler: Pointer to class object derived from abstract class AIInferenceHandler.
	 */
	AIModelService(CollectRawDataListener collectRawDataCallback, std::shared_ptr<AIInferenceHandler> inferenceHandler);

	/**
	 * @brief AIModelService class destructor.
	 * It clears all the resources related to timer.
	 */
	~AIModelService();

	/**
	 * @brief Sets inference interval to specified time by changing time interval of the timer.
	 * After this, application will start receiving data collection callback as per new specified time interval.
	 * @param [in] interval: time interval to be set in milliseconds.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT setInterval(uint16_t interval);

	/**
	 * @brief mInterval > 0 : It starts the timer as per mInterval which is set in prepare API.
	 * 		  After this, application will start recieving data collection callback after time interval specified by mInterval.
	 * 		  mInterval = 0 : It just sets mServiceRunning to true, indicating service has started.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT start(void);

	/**
	 * @brief mInterval > 0 : It stops the timer.
	 * 		  After this, appplication will stop receiving callback in Collect Raw Data listener.
	 * 		  mInterval = 0 : It just sets mServiceRunning to false, indicating service has stopped.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT stop(void);

	/**
	 * @brief Pushes the incoming raw data to AIInferenceHandler for pre-processing, invoke, post processing and finally ensembling.
	 * @param [in] data: Incoming sensor data to be passed for inference.
	 * @param [in] count: Length of incoming sensor data array.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT pushData(void *data, uint16_t count);

	/**
	 * @brief It calls prepare function of AIInferenceHandler which attaches data processing logic(if required) in each model and loads the models.
	 * It then retrieves inference interval of model set from AIInferenceHandler.
	 * If inference interval > 0, it allocate memory to timer structure and create/initialize the timer. It does not start the timer.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT prepare(void);

	/**
	 * @brief Clears sensor data associated with all models in model set.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clearData(void);

	/**
	 * @brief Clears specific sensor data rows associated with all models in model set.
	 * @param [IN] offset: Offset of row to start deletion.
	 * @param [IN] count: Count of rows to delete.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clearData(uint16_t offset, uint16_t count);

	/**
	 * @brief Gives raw data collection callback.
	 * @return: Returns mCollectRawDataCallback.
	 */
	CollectRawDataListener getCollectRawDataCallback(void);

	/**
	 * @brief This function will be called by timer everytime it expires.
	 * Internally, it calls Collect Raw Data listener of application to collect raw data and pass it for inference.
	 * @param [in] args: AIModelService class object.
	 */
	static void timerTaskHandler(void *args);

private:
	/**
	 * @brief Destroys the timer created/initialized in prepare API and frees memory allocated to timer object.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT freeTimer(void);

	uint16_t mInterval;
	bool mServiceRunning;
	std::shared_ptr<AIInferenceHandler> mInferenceHandler;
	CollectRawDataListener mCollectRawDataCallback;
	aifw_timer *mTimer;
};

} /* namespace aifw */
