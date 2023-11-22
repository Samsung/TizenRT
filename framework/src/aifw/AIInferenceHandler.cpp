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

#include "aifw/aifw.h"
#include "aifw/aifw_log.h"
#include "aifw/AIInferenceHandler.h"	

namespace aifw {

/**
 * @struct _arrayDeleter
 * @brief Custom deleter for shared pointer
 */
template <typename T>
struct _arrayDeleter {
	void operator()(T const *p)
	{
		delete[] p;
	}
};

AIInferenceHandler::AIInferenceHandler(uint16_t countOfModels, InferenceResultListener listener) :
	mMaxModelsCount(countOfModels), mModelIndex(0),
	mModels(new std::shared_ptr<AIModel>[mMaxModelsCount], _arrayDeleter<std::shared_ptr<AIModel>>()),
	mInferenceResultListener(listener)
{
}

void AIInferenceHandler::attachModel(std::shared_ptr<AIModel> model)
{
	mModels.get()[mModelIndex++] = model;
}

uint16_t AIInferenceHandler::getModelServiceInterval(void)
{
	return mModels.get()[0]->getModelAttribute().inferenceInterval;
}

AIFW_RESULT AIInferenceHandler::pushData(void *data, uint16_t count)
{
	if (!data) {
		AIFW_LOGE("raw data argument is null");
		return AIFW_INVALID_ARG;
	}
	AIFW_RESULT res;
	uint16_t idx;

	/* First create result buffer */
	uint16_t inferenceResultCount = mModels.get()[0]->getModelAttribute().inferenceResultCount;
	char *finalResult = new char[inferenceResultCount * sizeof(float)];
	if (!finalResult) {
		AIFW_LOGE("Memory Allocation failed for final result buffer");
		return AIFW_NO_MEM;
	}

	/* Now inference each of model */
	res = AIFW_OK;
	for (idx = 0; idx < mModelIndex; idx++) {
		res = mModels.get()[idx]->pushData(data, count);
		/** TODO if it failed during pushData, then Each of model will have different rawdata set.
		  * Should we remove data from buffer? or each model should have same step?
		  * parseData A model -> parseData B model -> preProcess A Model -> preProcess B Model ....
          */
		if (res < AIFW_OK) {
			/* Inference error in a model, complete inference cycle is terminated. */
			AIFW_LOGE("Push Data of model %d failed.", idx);
			break;
		}
		if (res == AIFW_INFERENCE_FINISHED) {
			AIFW_LOGV("model index %d inference finished", idx);
			break;
		}
	}

	/* We do nothing if Inference is in progress */
	if (res == AIFW_INFERENCE_PROCEEDING) {
		delete[] finalResult;
		finalResult = NULL;
		return AIFW_OK;
	}

	/* Proper result of inference should be handled by onInferenceFinished to get final result */
	if ((res == AIFW_OK) || (res == AIFW_INFERENCE_FINISHED)) {
		res = onInferenceFinished(idx, (void *)finalResult);
		if (res != AIFW_OK) {
			AIFW_LOGE("ensemble operation error: %d", res);
		}
	}

	/* Regardless type result, result will be shared through inferenceResultListener */
	mInferenceResultListener(res, (void *)finalResult, inferenceResultCount);
	if (finalResult) {
		delete[] finalResult;
		finalResult = NULL;
	}
	return AIFW_OK;
}

AIFW_RESULT AIInferenceHandler::clearData(void)
{
	AIFW_RESULT res = AIFW_OK;
	for (uint16_t idx = 0; idx < mModelIndex; idx++) {
		res = mModels.get()[idx]->clearRawData();
	}
	return res;
}

AIFW_RESULT AIInferenceHandler::clearData(uint16_t offset, uint16_t count)
{
	AIFW_RESULT res = AIFW_OK;
	for (uint16_t idx = 0; idx < mModelIndex; idx++) {
		res = mModels.get()[idx]->clearRawData(offset, count);
	}
	return res;
}

} /* namespace aifw */

