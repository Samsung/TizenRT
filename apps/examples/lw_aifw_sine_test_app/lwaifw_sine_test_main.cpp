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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdint.h>
#include "AIServiceCore.h"
#include "AICSVDataSource.h"
#include "lw_aifw/lw_aifw.h"
#include "lw_aifw/lw_aifw_log.h"
#include "lw_aifw/lw_aifw_utils.h"
#include "lw_aifw/lw_aifw_result.h"
#include "lwaifw_sine_test_model.h"
#include "lwaifw_sine_test_main.h"
#include "lwaifw_sine_test_input.h"

static uint16_t gNumberofModels = 1;

static struct TFModel gSineModelData = {
		.ModelType = ARRAY_MODEL,
		.Model = g_sine_model,
		.ModelCode = 0x00000000, /* We don't use it for now */
		.CountInputVals = 1
};

static std::shared_ptr<lw_aifw::AIServiceCore> gServiceCore;
static std::shared_ptr<lw_aifw::AIDataSource> gDataSource;

static uint16_t gDataValsPerSet;
static uint16_t gNumberOfSetToRead;
static void onDataReadyListener(LW_AIFW_RESULT result);

static struct AIAppMetadata gAppMetadata;

static struct model_input_output gSine;

LW_AIFW_RESULT lw_aifw_init(struct AIAppMetadata *appMetadata, DATA_SOURCE sourceType, const char *sourceName, float *dataValues, uint16_t dataValsPerSet, uint16_t setCount)
{
	LW_AIFW_RESULT ret = LW_AIFW_ERROR;
	LW_AIFW_LOGV("Model Type : %d, SourceType : %d", appMetadata->Models[0].ModelType, sourceType);

	switch (sourceType) {
	case _CSV_ARRAY:
		gDataSource = std::shared_ptr<lw_aifw::AICSVDataSource>(new lw_aifw::AICSVDataSource((float*)dataValues, dataValsPerSet, setCount));
		break;
	case _CSV_FILE:
		gDataSource = std::make_shared<lw_aifw::AICSVDataSource>(sourceName);
		break;
	default:
		LW_AIFW_LOGE("invalid.. Model Type : %d, SourceType : %d", appMetadata->Models[0].ModelType, sourceType);
		return ret;
	}

	gServiceCore = std::make_shared<lw_aifw::AIServiceCore>();
	ret = gServiceCore->init(appMetadata, gDataSource);
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("Failed to init ret : %d", ret);
	}
	return ret;
}

LW_AIFW_RESULT lw_aifw_deinit(void)
{
	gServiceCore.reset();
	gDataSource.reset();
	return LW_AIFW_OK;
}

LW_AIFW_RESULT lw_aifw_setInterval(uint16_t interval)
{
	return gServiceCore->setInterval(interval);
}

void lw_aifw_pushSensorData(float *data, uint16_t len) {
	gServiceCore->pushSensorData(data, len);
}

static void initAppMetadata(void)
{
	gAppMetadata.ModelCount = gNumberofModels;
	gAppMetadata.Models[0] = gSineModelData;
	gAppMetadata.DataReadyListener = onDataReadyListener;
}

static void onDataReadyListener(LW_AIFW_RESULT res)
{
	LW_AIFW_RESULT ret = res;
	if (ret == LW_AIFW_SOURCE_EOF) {
		LW_AIFW_LOGE("input data processing done");
		gServiceCore->stopTimer();
		return;
	}
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("Error = %d detected skip further processing", ret);
		return;
	}

	/* Reading from buffer */
	float *inputData = (float *)calloc(gDataValsPerSet * gNumberOfSetToRead, sizeof(float));
	if (!inputData) {
		LW_AIFW_LOGE("Memory allocation failed", ret);
		return;
	}
	ret = gServiceCore->getSensorData(inputData, gDataValsPerSet, gNumberOfSetToRead);
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("lw_aifw_getData data failed with error = %d so Skip inferencing",ret);
		return;
	}

	for (uint16_t i = 0; i < gAppMetadata.Models[0].CountInputVals; i++) {
		gSine.InputVals[i] = inputData[i];
		LW_AIFW_LOGE("index[%d], value %f", i, gSine.InputVals[i]);
	}
	free(inputData);

	void *inf_res1;
	ret = gServiceCore->doInference(0, (void*)gSine.InputVals, &inf_res1);
	if (ret != LW_AIFW_OK) {
		LW_AIFW_LOGE("inference error");
		return;
	}

	float *result = (float *)inf_res1;
	if (result == NULL) {
		LW_AIFW_LOGE("Error = Result variable value is NULL");
		return;
	}
	LW_AIFW_LOGE("Result val is %f", result[0]);
	return;
}

static void cleanup(void) {
	if (gSine.InputVals) {
		free(gSine.InputVals);
		gSine.InputVals = NULL;
	}
}

static int initializeModelInputOutput(void)
{
	gServiceCore->getModelDimensions(&gSine.InputCount, &gSine.OutputCount, 0);
	gSine.InputVals = (float *)calloc((gSine.InputCount + 1), sizeof(float));
	if (!gSine.InputVals) {
		LW_AIFW_LOGE("Sin input values creation failed");
		cleanup();
		return LW_AIFW_NO_MEM;
	}
	return 0;
}


static int lw_aifw_test_init(void)
{
#ifdef PRINT_COUNTER
	gIterationCount = 0;
#endif
	LW_AIFW_RESULT ret = LW_AIFW_ERROR;
	gDataValsPerSet = 1 + 1; /* 1 added for expected output to be read from CSV file */
	gNumberOfSetToRead = 1;
	initAppMetadata();
	
	ret = lw_aifw_init(&gAppMetadata, _CSV_ARRAY, NULL, (float *)gDataValues, NO_OF_SENSORS, NO_OF_ROWS_TO_READ);
	if (ret == LW_AIFW_OK) {
		initializeModelInputOutput();
		gServiceCore->startTimer();
	}
	return 0;
}

int lw_aifw_sine_test_app_main(int argc, char *argv[])
{
	printf("sine test app started\n");
	lw_aifw_test_init();
	return 0;
}

