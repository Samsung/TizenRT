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

#include <memory>
#include "aifw/aifw.h"
#include "aifw/aifw_log.h"
#include "aifw/aifw_utils.h"
#include "aifw/AIDataBuffer.h"
#include "SineWaveProcessHandler.h"

SineWaveProcessHandler::SineWaveProcessHandler() :
	mRawData(NULL), mInvokeOutput(NULL)
{
}

SineWaveProcessHandler::~SineWaveProcessHandler()
{
	if (mRawData) {
		delete[] mRawData;
		mRawData = NULL;
	}
	if (mInvokeOutput) {
		delete[] mInvokeOutput;
		mInvokeOutput = NULL;
	}
}

AIFW_RESULT SineWaveProcessHandler::parseData(void *data, uint16_t count, float *parsedData, AIModelAttribute *modelAttribute)
{
	if (!data) {
		AIFW_LOGE("raw data from source is NULL");
		return AIFW_INVALID_ARG;
	}
	if (!parsedData) {
		AIFW_LOGE("parsed data buffer argument is NULL");
		return AIFW_INVALID_ARG;
	}
	/* Here raw data values are parsed. Required raw data values can be selected or new data can be created using different raw data values. */
	for (uint16_t i = 0; i < modelAttribute->rawDataCount; i++) {
		parsedData[i] = ((float *)data)[i];
	}
	return AIFW_OK;
}

AIFW_RESULT SineWaveProcessHandler::preProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *invokeInput, AIModelAttribute *modelAttribute)
{
	if (!invokeInput) {
		AIFW_LOGE("Invalid argument - output data buffer");
		return AIFW_INVALID_ARG;
	}
	if (!mRawData) {
		mRawData = new float[modelAttribute->rawDataCount];
		if (!mRawData) {
			AIFW_LOGE("Memory Allocation failed - data read buffer");
			return AIFW_NO_MEM;
		}
	}
	/* Read latest row from AI data buffer. At this time the latest row includes only parsed raw data and does not include invoke result. */
	AIFW_RESULT res = buffer->readData(mRawData, 0, modelAttribute->rawDataCount, 0);
	if (res != AIFW_OK) {
		AIFW_LOGE("Reading Data from the buffer failed. ret: %d", res);
		return res;
	}
	/* Pre processing can be done at this point e.g. data can be normalized using mean and standard deviation values specific to model. Fnally pre-processed values, ready for inference is to be copied in invokeInput buffer */
	memcpy(invokeInput, mRawData, modelAttribute->invokeInputCount * sizeof(float));
	return AIFW_OK;
}

AIFW_RESULT SineWaveProcessHandler::postProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *resultData, AIModelAttribute *modelAttribute)
{
	if (!resultData) {
		AIFW_LOGE("Invalid argument - output buffer");
		return AIFW_INVALID_ARG;
	}
	if (!mInvokeOutput) {
		mInvokeOutput = new float[modelAttribute->invokeOutputCount];
		if (!mInvokeOutput) {
			AIFW_LOGE("Memory Allocation failed - data read buffer");
			return AIFW_NO_MEM;
		}
	}
	/* Read the latest raw from buffer. At this time latest row includes parsed raw data as well as invoke result. */
	AIFW_RESULT res = buffer->readData(mInvokeOutput, modelAttribute->rawDataCount, (modelAttribute->rawDataCount + modelAttribute->invokeOutputCount), 0);
	if (res != AIFW_OK) {
		AIFW_LOGE("Reading Data from the buffer failed.");
		return res;
	}
	/* Post processing of result can be done at this point. Finally copy postprocesses values in resultData buffer */
	memcpy(resultData, mInvokeOutput, modelAttribute->postProcessResultCount * sizeof(float));
	return AIFW_INFERENCE_FINISHED;
}

