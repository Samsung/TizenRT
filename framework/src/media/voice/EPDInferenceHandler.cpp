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

#include "aifw/aifw.h"
#include "aifw/AIModel.h"
#include "aifw/AIProcessHandler.h"
#include "EPDProcessHandler.h"
#include "EPDInferenceHandler.h"
#include <debug.h>

static uint16_t gModelCount = 1;

EPDInferenceHandler::EPDInferenceHandler(InferenceResultListener listener) :
	AIInferenceHandler(gModelCount, listener)
{
	medvdbg("EPDInferenceHandler constructor");
}

EPDInferenceHandler::~EPDInferenceHandler()
{
	medvdbg("EPDInferenceHandler destructor");
}

AIFW_RESULT EPDInferenceHandler::prepare(void)
{
	mEPDProcessHandler = std::make_shared<EPDProcessHandler>();
	if (!mEPDProcessHandler) {
		meddbg("EPD process handler memory allocation failed.");
		return AIFW_NO_MEM;
	}
	mEPDModel = std::make_shared<aifw::AIModel>(mEPDProcessHandler);
	if (!mEPDModel) {
		meddbg("EPD model memory allocation failed.");
		return AIFW_NO_MEM;
	}
	AIFW_RESULT result = mEPDModel->loadModel("/mnt/EPD.json");
	if (result != AIFW_OK) {
		meddbg("EPD Model load failed. error: %d", result);
		return result;
	}
	attachModel(mEPDModel);
	medvdbg("EPD model prepare done");
	return AIFW_OK;
}

/* onInferenceFinished will be called when inference finished properly */
AIFW_RESULT EPDInferenceHandler::onInferenceFinished(uint16_t idx, void *finalResult)
{
	AIFW_RESULT ret = AIFW_OK;
	float *result = (float *)finalResult;
	uint16_t postProcessResultCount = mEPDModel->getModelAttribute().postProcessResultCount;
	float *postProcessedData = new float[postProcessResultCount];
	if (!postProcessedData) {
		meddbg("post process data buffer memory allocation failed.");
		return AIFW_NO_MEM;
	}
	ret = mEPDModel->getResultData(postProcessedData, postProcessResultCount);
	if (ret != AIFW_OK) {
		meddbg("get result data of model failed. error: %d", ret);
		delete[] postProcessedData;
		postProcessedData = NULL;
		return ret;
	}
	result[0] = postProcessedData[0];
	result[1] = postProcessedData[1];
	delete[] postProcessedData;
	postProcessedData = NULL;
	medvdbg("EPD model ensembling done");
	return ret;
}

AIFW_RESULT EPDInferenceHandler::clearData(void)
{
	AIFW_RESULT res = mEPDModel->clearRawData();
	if (res!= AIFW_OK) {
		meddbg("clear raw data of EPD model failed. ret: %d", res);
		return res;
	}

	medvdbg("EPD model clear data done");
	return AIFW_OK;
}

AIModelAttribute EPDInferenceHandler::getModelAttribute(void)
{
	return mEPDModel->getModelAttribute();
}
