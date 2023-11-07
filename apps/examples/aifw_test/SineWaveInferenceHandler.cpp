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

#include "tinyara/config.h"
#include "aifw/aifw.h"
#include "aifw/aifw_log.h"
#include "aifw/AIModel.h"
#include "aifw/AIProcessHandler.h"
#include "SineWaveProcessHandler.h"
#include "SineWaveInferenceHandler.h"
#ifndef CONFIG_PRODUCT_AI_MODEL_FILE_SUPPORT
#include "SineWaveAIModel.h"
#endif

extern "C" {
uint32_t gSineWaveCode = 0x00000000;
}

#ifdef CONFIG_PRODUCT_AI_MODEL_FILE_SUPPORT
struct Model_Attributes {
	uint32_t modelCode;
	const char *scriptPath;
};
static const struct Model_Attributes gModelCodeMap[] = {
	{gSineWaveCode, "/mnt/AI/SineWaveAIModel.json"}};

static uint16_t gModelCount = sizeof(gModelCodeMap) / sizeof(struct Model_Attributes);
#else
static uint16_t gModelCount = 1;
#endif

SineWaveInferenceHandler::SineWaveInferenceHandler(InferenceResultListener listener) :
	AIInferenceHandler(gModelCount, listener), mSWModel(NULL), mPostProcessedData(NULL)
{
}

SineWaveInferenceHandler::~SineWaveInferenceHandler()
{
	if (mPostProcessedData) {
		delete[] mPostProcessedData;
		mPostProcessedData = NULL;
	}
}

AIFW_RESULT SineWaveInferenceHandler::prepare(void)
{
	std::shared_ptr<aifw::AIProcessHandler> sineWaveProcessHandler = std::make_shared<SineWaveProcessHandler>();
	if (!sineWaveProcessHandler) {
		AIFW_LOGE("Sine Wave process handler memory allocation failed.");
		return AIFW_NO_MEM;
	}
	mSWModel = std::make_shared<aifw::AIModel>(sineWaveProcessHandler);
	if (!mSWModel) {
		AIFW_LOGE("Temperature prediction model memory allocation failed.");
		return AIFW_NO_MEM;
	}
	AIFW_RESULT result;
#ifdef CONFIG_PRODUCT_AI_MODEL_FILE_SUPPORT
	result = mSWModel->loadModel(gModelCodeMap[0].scriptPath);
	if (result != AIFW_OK) {
		AIFW_LOGE("Sine wave load failed");
		return result;
	}
#else
	result = mSWModel->loadModel(gSineWaveModelAttribute);
	if (result != AIFW_OK) {
		AIFW_LOGE("Sine wave load failed. ret: %d", result);
		return result;
	}
#endif
	attachModel(mSWModel);
	return AIFW_OK;
}

AIFW_RESULT SineWaveInferenceHandler::onInferenceFinished(uint16_t idx, void *finalResult)
{
	AIFW_RESULT ret = AIFW_OK;
	float *result = (float *)finalResult;
	uint16_t postProcessResultCount = mSWModel->getModelAttribute().postProcessResultCount;
	if (!mPostProcessedData) {
		mPostProcessedData = new float[postProcessResultCount];
		if (!mPostProcessedData) {
			AIFW_LOGE("post process data buffer memory allocation failed.");
			return AIFW_NO_MEM;
		}
	}
	ret = mSWModel->getResultData(mPostProcessedData, postProcessResultCount);
	if (ret != AIFW_OK) {
		AIFW_LOGE("get result data of model failed");
		return ret;
	}
	result[0] = mPostProcessedData[0];
	if (result[0] < -1 || result[0] > 1) {
		AIFW_LOGE("Error !! invoke result is out of bounds : %f", result[0]);
		return AIFW_INVOKE_OUT_OF_BOUNDS;
	}
	return AIFW_OK;
}

