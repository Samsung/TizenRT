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

#include <stdio.h>
#include <memory>
#include "aifw/aifw.h"
#include "aifw/aifw_log.h"
#include "aifw/aifw_csv_reader.h"
#include "aifw_test_main.h"
#include "aifw/AIModelService.h"
#include "aifw/AIInferenceHandler.h"
#include "SineWaveInferenceHandler.h"

using namespace aifw;

/**
 * @brief ModelSetInfo keeps pointers to various model related objects and  variables.
 */
struct ModelSetInfo {
	uint32_t modelCode;
	std::shared_ptr<AIInferenceHandler> aiInferenceHandler;
	std::shared_ptr<AIModelService> aiModelService;
};

template <typename T>
struct arrayDeleter {
	void operator()(T const *p)
	{
		delete[] p;
	}
};

extern uint32_t gSineWaveCode;
static uint16_t gSensorValueCount;
static float *gSensorValues = NULL;
static void *gHandle = NULL;
static uint16_t gResultValueCount;
static float *gResultValues = NULL;
static void *gResultHandle = NULL;
static std::shared_ptr<ModelSetInfo> gModelSetList;
static uint16_t gModelSetListOffset = 0;
static uint16_t gMaxModelSetCount = 0;

static int16_t findModelSetInfoIndex(uint32_t modelCode)
{
	for (int i = 0; i < gModelSetListOffset; i++) {
		if (gModelSetList.get()[i].modelCode == modelCode)
			return i;
	}
	return -1;
}

/**
 * @brief This function initializes AI Helper module. It allocates memory and sets internal data structures for 
 *        AI helper to work properly. This function is pre-requiste before any application calls ai_helper_load_model API.
 * @param [in] maxModelSetCount: Maximum number of model sets to be loaded by all applications.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_init(uint16_t maxModelSetCount)
{
	gMaxModelSetCount = maxModelSetCount;
	std::shared_ptr<ModelSetInfo> modelSetList(new ModelSetInfo[gMaxModelSetCount], arrayDeleter<ModelSetInfo>());
	if (!modelSetList) {
		AIFW_LOGE("model set list Memory Allocation failed.");
		return -1;
	}
	gModelSetList = modelSetList;
	return 0;
}

/**
 * @brief This function deinitializes AI Helper & further no API of AI Helper should be called.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_deinit(void)
{
	for (int i = 0; i < gModelSetListOffset; i++) {
		gModelSetList.get()[i].aiInferenceHandler = NULL;
		gModelSetList.get()[i].aiModelService = NULL;
	}
	gModelSetList = NULL;
	gMaxModelSetCount = 0;
	gModelSetListOffset = 0;
	return 0;
}

/**
 * @brief This API loads all AI models in model set corresponding to model code. A service is created to perform operation on loaded model set.
 * It instantiates application components such as inference and process handlers. The loaded models are mapped with application inference handler.
 * @param [in] modelCode: modelCode for the modelSet.
 * @param [in] resultCallback: Function to receive inference result from AI Framework.
 * @param [in] collectRawDataCallback: This function is called by service for data collection and inference operation.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_load_model(uint32_t modelCode, InferenceResultListener resultCallback, CollectRawDataListener collectRawDataCallback)
{
	int index = findModelSetInfoIndex(modelCode);
	if (index != -1) {
		AIFW_LOGE("Model set with modelCode %d loaded already", modelCode);
		return 0;
	}
	gModelSetList.get()[gModelSetListOffset].aiInferenceHandler = std::make_shared<SineWaveInferenceHandler>(resultCallback);
	if (!gModelSetList.get()[gModelSetListOffset].aiInferenceHandler) {
		AIFW_LOGE("Model code not suppported or Memory allocation failed for aiInferenceHandler");
		return -1;
	}
	gModelSetList.get()[gModelSetListOffset].aiModelService = std::make_shared<AIModelService>(collectRawDataCallback, gModelSetList.get()[gModelSetListOffset].aiInferenceHandler);
	if (!gModelSetList.get()[gModelSetListOffset].aiModelService) {
		gModelSetList.get()[gModelSetListOffset].aiInferenceHandler = nullptr;
		AIFW_LOGE("Memory allocation failed for aiModelService");
		return -1;
	}
	AIFW_RESULT res = gModelSetList.get()[gModelSetListOffset].aiModelService->prepare();
	if (res != AIFW_OK) {
		gModelSetList.get()[gModelSetListOffset].aiInferenceHandler = nullptr;
		gModelSetList.get()[gModelSetListOffset].aiModelService = nullptr;
		AIFW_LOGE("AI model service prepare api failed");
		return -1;
	}
	gModelSetList.get()[gModelSetListOffset].modelCode = modelCode;
	gModelSetListOffset++;
	return 0;
}

/**
 * @brief Starts service for the corresponding modelSet. After this application will start receiving callback in Collect Raw Data listener.
 * @param [in] modelCode: modelCode for which service is to be started.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_start(uint32_t modelCode)
{
	int index = findModelSetInfoIndex(modelCode);
	if (index == -1) {
		AIFW_LOGE("model info not found for model code %d", modelCode);
		return -1;
	}
	AIFW_RESULT ret = gModelSetList.get()[index].aiModelService->start();
	return ret == AIFW_OK ? 0 : -1;
}

/**
 * @brief Stops the modelService for the corresponding modelSet. Appplication will stop receiving callback in Collect Raw Data listener.
 * @param [in] modelCode: modelCode for which service is to be stopped.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_stop(uint32_t modelCode)
{
	int index = findModelSetInfoIndex(modelCode);
	if (index == -1) {
		AIFW_LOGE("model info not found for model code %d", modelCode);
		return -1;
	}
	AIFW_RESULT ret = gModelSetList.get()[index].aiModelService->stop();
	return ret == AIFW_OK ? 0 : -1;
}

/**
 * @brief Helper function to push raw data to model service for pre-processing, inference and post processing.
 * @param [in] modelCode: modelCode for the modelSet.
 * @param [in] data: Incoming sensor data to be passed for processing.
 * @param [in] len: Length of incoming sensor data array.
 * @return: Returns integer result for FAIL(-1) or SUCCESS(0).
 */
static int ai_helper_push_data(uint32_t modelCode, void *data, uint16_t len)
{
	int index = findModelSetInfoIndex(modelCode);
	if (index == -1) {
		AIFW_LOGE("no model registered with modelcode %d", modelCode);
		return -1;
	}
	AIFW_RESULT ret = gModelSetList.get()[index].aiModelService->pushData(data, len);
	return ret == AIFW_OK ? 0 : -1;
}

static void *stopAndDeinitHelperModule(void *arg)
{
	AIFW_LOGV("Started thread");
	if (ai_helper_stop(gSineWaveCode) != AIFW_OK) {
		AIFW_LOGE("AI helper stop failed");
		return NULL;
	}
	ai_helper_deinit();
	AIFW_LOGV("AI helper deinit done.");
	return NULL;
}

/**
 * @brief: Deinitilizes AI Helper module and stops the service for the modelset.
 * Application calls this function if fetching raw data from data source fails.
*/
static void aifw_test_deinit(void)
{
	/* Deinitialize CSV data source for input raw data */
	if (gSensorValues) {
		free(gSensorValues);
		gSensorValues = NULL;
	}
	AIFW_RESULT retValue = csvDeinit(&gHandle);
	if (retValue != AIFW_OK) {
		AIFW_LOGE("Input CSV deinit failed with error: %d", retValue);
	}

	/* Deinitialize CSV data source for expected inference result data */
	if (gResultValues) {
		free(gResultValues);
		gResultValues = NULL;
	}
	retValue = csvDeinit(&gResultHandle);
	if (retValue != AIFW_OK) {
		AIFW_LOGE("Result CSV deinit failed with error: %d", retValue);
	}
	pthread_t thread;
	int result = pthread_create(&thread, NULL, stopAndDeinitHelperModule, NULL);
	if (result != 0) {
		AIFW_LOGE("ERROR Failed to start thread");
		return;
	}
}

/**
 * @brief: AI Framework calls this function to collect the raw data and pass it for inference.
 * This callback is called when timer expires. Time interval is set in 'inferenceInterval' field of json.
*/
static void sine_collectRawDataListener(void)
{
	memset(gSensorValues, '\0', gSensorValueCount * sizeof(float));
	AIFW_RESULT result = readCSVData(gHandle, gSensorValues);
	if (result == AIFW_OK) {
		int res = ai_helper_push_data(gSineWaveCode, (void *)gSensorValues, gSensorValueCount);
		if (res == 0) {
			AIFW_LOGV("push data operation OK");
		} else {
			AIFW_LOGE("push data operation failed. ret: %d", res);
		}
		return;
	}
	AIFW_LOGE("reading input CSV data return error result : %d", result);
	aifw_test_deinit();
}

/**
 * @brief: Application will receive inference result in this function.
 * It is mandatory to be defined by each application.
 * @param [in] res: Successful inference operation 'res' is set to AIFW_OK. Errors are set as per AIFW_RESULT enum values.
 * @param [in] values: inference result values after ensembling 
 * @param [in] count: count of values in inference result
*/
static void sine_inferenceResultListener(AIFW_RESULT res, void *values, uint16_t count)
{
	if (res != AIFW_OK) {
		AIFW_LOGE("Inference failed for this cycle, ret: %d", res);
		return;
	}
	float *predictedResult = (float *)values;
	/* Read expected inference result from result CSV to compare it from predicted inference result */
	memset(gResultValues, '\0', gResultValueCount * sizeof(float));
	AIFW_RESULT result = readCSVData(gResultHandle, gResultValues);
	if (result != AIFW_OK) {
		AIFW_LOGE("reading result CSV data failed, inference result values cannot be verfied for this inference cycle. ret: %d", result);
	}
	AIFW_LOGI("Expected value: %f, AIFW prediction result : %f", gResultValues[1], predictedResult[0]);
}

int aifw_test_main(int argc, char *argv[])
{
	/* Initialize CSV data source for input raw data */
	AIFW_RESULT res = csvInit(&gHandle, "/mnt/AI/SineWave_packet.csv", FLOAT32, false);
	if (res != AIFW_OK) {
		AIFW_LOGE("FILE NOT FOUND || ERROR OPENING CSV. ret: %d", res);
		return -1;
	}
	res = getColumnCount(gHandle, &gSensorValueCount);
	if (res != AIFW_OK) {
		AIFW_LOGE("Fetching sensor value count failed. ret: %d\n", res);
		goto cleanup;
	}
	gSensorValues = (float *)malloc(gSensorValueCount * sizeof(float));
	if (!gSensorValues) {
		AIFW_LOGE("Memory allocation failed for sensor values buffer");
		goto cleanup;
	}
	AIFW_LOGV("Raw input data csv initialization OK");

	/* Initialize CSV data source for expected inference result data */
	res = csvInit(&gResultHandle, "/mnt/AI/SineWave_resultPacket.csv", FLOAT32, false);
	if (res != AIFW_OK) {
		AIFW_LOGE("FILE NOT FOUND || ERROR OPENING CSV. ret: %d", res);
		goto cleanup;
	}
	res = getColumnCount(gResultHandle, &gResultValueCount);
	if (res != AIFW_OK) {
		AIFW_LOGE("Fetching result value count failed. ret: %d\n", res);
		goto cleanup;
	}
	gResultValues = (float *)malloc(gResultValueCount * sizeof(float));
	if (!gResultValues) {
		AIFW_LOGE("Memory allocation failed for result values buffer");
		goto cleanup;
	}
	AIFW_LOGV("Result data csv initialization OK");

	if (ai_helper_init(1) != 0) {
		AIFW_LOGE("AI helper init failed");
		goto cleanup;
	}
	if (ai_helper_load_model(gSineWaveCode, sine_inferenceResultListener, sine_collectRawDataListener) != 0) {
		AIFW_LOGE("Load model failed");
		goto cleanup;
	}
	if (ai_helper_start(gSineWaveCode) != 0) {
		AIFW_LOGE("AI helper start failed");
		goto cleanup;
	}
	return 0;

cleanup:
	aifw_test_deinit();
	return -1;
}

