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
#include <pthread.h>
#include <signal.h>
#include "lw_aifw/lw_aifw.h"
#include "lw_aifw/AIDataSource.h"
#include "AIDataBuffer.h"
#include "AIModel.h"

namespace lw_aifw {

class AIServiceCore
{
private:
	uint32_t mTimerInterval;
	uint16_t mInferencePeriod;
	struct AIAppMetadata *mAppMetadata;
	std::shared_ptr<AIDataSource> mDataSource;
	std::shared_ptr<AIDataBuffer> mDataBuffer;
	std::shared_ptr<AIModel> m_AIModel[MAX_NUMBER_OF_MODEL];
	pthread_t mTimerThread;

	/**
	 * @brief Load model data which is come from app
	 * @param[in] uint16_t: App Metadata array pointer
	 * @return: LW_AIFW_RESULT enum object
	 */
	LW_AIFW_RESULT loadModel(struct AIAppMetadata *appMetadata);

public:
	/**
	 * @brief Construct the AIServiceCore class instance
	 * @details @b #include <AIServiceCore.h>
	 */
	AIServiceCore();

	/**
	 * @brief AIServiceCore destructor
	 * @details @b #include <AIServiceCore.h>
	 */
	~AIServiceCore();

	/**
	 * @brief Initialization
	 * @param[in] uint16_t: App Metadata array pointer
	 * @param[in] AIDataSource: framework collects data from this source
	 * @return: LW_AIFW_RESULT enum object
	 */
	LW_AIFW_RESULT init(struct AIAppMetadata *appMetadata, std::shared_ptr<lw_aifw::AIDataSource> dataSource);

	/**
	 * @brief Initialization for inference immediately
	 * @param[in] uint16_t: App Metadata array pointer
	 * @return: LW_AIFW_RESULT enum object
	 */
	LW_AIFW_RESULT init(struct AIAppMetadata *appMetadata);

	/**
	 * @brief Starts data collection as per interval in init and do inference on collected data
	 */
	LW_AIFW_RESULT startTimer(void);

	/**
	 * @brief Stops data collection and inference operation
	 */
	LW_AIFW_RESULT stopTimer(void);

	/**
	 * @brief gives total number of data sets in the streaming buffer.
	 */
	int getBufferDataSetCount();

	/**
	 * @brief Uses AIDataSource object to fetch data from source.
	 * @param[out] float: buffer get the data from source
	 * @param[in] uint_16: data count in a row
	 * @param[in] uint_16: Number of rows to get from source
	 */
	LW_AIFW_RESULT getSensorData(float *data, uint16_t dataPerSet, uint16_t setCount);

	/**
	 * @brief Uses AIDataSource object to fetch one data set at a time from source.
	 * @param[out] float: buffer get the data from source
	 * @param[in] uint_16: From what index relative to latest written dataset we have to fetch the new dataset.
	 */
	LW_AIFW_RESULT getSensorData(float *result, uint16_t index);

	/**
	 * @brief Shares inference result and data using Data source object created and set by application
	 * @param[in] data as void pointer
	 */
	LW_AIFW_RESULT shareData(void *data);

	/**
	 * @brief This function does inference based on input data and store inference results in output variable
	 * @param[in] uint_16: id of model in a model set using which inference to be done
	 * @param[in] void*: data to use for doing inference
	 * @param[out] void*: reference to inference result object
	 * @return: Result of function call as LW_AIFW_RESULT Object
	 */
	LW_AIFW_RESULT doInference(uint16_t modelId, void *input, void **output);

	/**
	 * @brief set data collection and inference interval for specified time
	 * @param[in] uint16_t: new inference interval value in milli sec
	 */
	LW_AIFW_RESULT setInterval(uint16_t interval);

	/**
	 * @brief return input and output count of model
	 * @param[out] int: number of input values for model
	 * @param[out] int: number of output values for model
	 * @param[in] uint_16: id of model in a model set using which inference to be done
	 */
	LW_AIFW_RESULT getModelDimensions(int *input, int *output, uint16_t modelId);

	/**
	 * @brief To get data from Data source and write in internal buffer
	 * @return: Result of function call as LW_AIFW_RESULT Object
	 */
	LW_AIFW_RESULT prepareData(void);

	/**
	 * @brief To call application data collected listener
	 * @param[in]: Result of function call as LW_AIFW_RESULT Object
	 */
	void executeDataReadyListener(LW_AIFW_RESULT result);

	void processAIData(LW_AIFW_RESULT result, float *data, uint16_t dataCount);

	/**
	 * @brief push Write Sensor data to Streaming buffer
	 * @param[in] int: Data to be written in streaming buffer
	 * @param[in] int: lenbgth of buffer
	 */
	void pushSensorData(float *data, uint16_t len);

	/**
	 * @brief return current time interval in milliseconds
	 */
	uint32_t getTimerInterval(void);

	static void onAIDataCollectedListener(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args);

	static void lw_aifw_timer_cb(int signo, siginfo_t *info, void *ucontext);
};

} // namespace lw_aifw
