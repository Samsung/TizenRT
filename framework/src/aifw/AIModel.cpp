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
#include "include/TFLM.h"
#include "include/AIManifestParser.h"
#include "aifw/AIDataBuffer.h"
#include "aifw/AIProcessHandler.h"
#include "aifw/AIModel.h"

namespace aifw {

AIModel::AIModel(void) :
	mBuffer(nullptr), mInvokeInput(NULL), mInvokeOutput(NULL), mParsedData(NULL), mPostProcessedData(NULL), mDataProcessor(nullptr)
{
	memset(&mModelAttribute, '\0', sizeof(AIModelAttribute));
	mAIEngine = std::make_shared<TFLM>();
	if (!mAIEngine) {
		AIFW_LOGE("Model Engine memory allocation failed");
	}
}

AIModel::AIModel(std::shared_ptr<AIProcessHandler> dataProcessor) :
	mBuffer(nullptr), mInvokeInput(NULL), mInvokeOutput(NULL), mParsedData(NULL), mPostProcessedData(NULL), mDataProcessor(dataProcessor)
{
	memset(&mModelAttribute, '\0', sizeof(AIModelAttribute));
	mAIEngine = std::make_shared<TFLM>();
	if (!mAIEngine) {
		AIFW_LOGE("Model Engine memory allocation failed");
	}
}

void AIModel::clearModelAttribute(void)
{
	if (mModelAttribute.version) {
		delete[] mModelAttribute.version;
		mModelAttribute.version = NULL;
	}
	if (mModelAttribute.features) {
		delete[] mModelAttribute.features;
		mModelAttribute.features = NULL;
	}
	if (mModelAttribute.meanVals) {
		delete[] mModelAttribute.meanVals;
		mModelAttribute.meanVals = NULL;
	}
	if (mModelAttribute.stdVals) {
		delete[] mModelAttribute.stdVals;
		mModelAttribute.stdVals = NULL;
	}
}

AIModel::~AIModel()
{
	clearModelAttribute();
	if (mInvokeInput) {
		delete[] mInvokeInput;
		mInvokeInput = NULL;
	}

	if (mInvokeOutput) {
		delete[] mInvokeOutput;
		mInvokeOutput = NULL;
	}

	if (mParsedData) {
		delete[] mParsedData;
		mParsedData = NULL;
	}

	if (mPostProcessedData) {
		delete[] mPostProcessedData;
		mPostProcessedData = NULL;
	}
}

AIFW_RESULT AIModel::createDataBuffer(void)
{
	AIFW_RESULT res;
	mBuffer = std::make_shared<AIDataBuffer>();
	if (!mBuffer) {
		AIFW_LOGE("model data buffer Memory Allocation failed.");
		return AIFW_NO_MEM;
	}
	if (mDataProcessor) {
		res = mBuffer->init(mModelAttribute.maxRowsDataBuffer, (mModelAttribute.rawDataCount + mModelAttribute.invokeOutputCount));
	} else {
		res = mBuffer->init(mModelAttribute.maxRowsDataBuffer, (mModelAttribute.invokeInputCount + mModelAttribute.invokeOutputCount));
	}
	if (res != AIFW_OK) {
		AIFW_LOGE("model data buffer initialization failed.");
		return res;
	}
	AIFW_LOGV("Buffer creation and initialization done");
	return res;
}

AIFW_RESULT AIModel::allocateMemory(void)
{
	mInvokeOutput = new float[mModelAttribute.invokeOutputCount];
	if (!mInvokeOutput) {
		AIFW_LOGE("Memory Allocation failed - model output buffer");
		return AIFW_NO_MEM;
	}
	mInvokeInput = new float[mModelAttribute.invokeInputCount];
	if (!mInvokeInput) {
		AIFW_LOGE("Memory Allocation failed - model input buffer");
		return AIFW_NO_MEM;
	}

	if (mDataProcessor) {
		mParsedData = new float[mModelAttribute.rawDataCount];
		if (!mParsedData) {
			AIFW_LOGE("Memory Allocation failed of parsed data");
			return AIFW_NO_MEM;
		}
		mPostProcessedData = new float[mModelAttribute.postProcessResultCount];
		if (!mPostProcessedData) {
			AIFW_LOGE("Memory Allocation failed of result data");
			return AIFW_NO_MEM;
		}
	}
	return AIFW_OK;
}

AIFW_RESULT AIModel::fillModelAttribute(const AIModelAttribute &modelAttribute)
{
	mModelAttribute = {
		modelAttribute.crc32,
		NULL,
		{'\0'},
		modelAttribute.model,
		NULL,
		modelAttribute.featuresCount,
		modelAttribute.inferenceInterval,
		modelAttribute.modelCode,
		modelAttribute.maxRowsDataBuffer,
		modelAttribute.rawDataCount,
		modelAttribute.windowSize,
		modelAttribute.invokeInputCount,
		modelAttribute.invokeOutputCount,
		modelAttribute.postProcessResultCount,
		modelAttribute.inferenceResultCount,
		NULL,
		NULL
	};

	if (!modelAttribute.version) {
		AIFW_LOGE("AI model version is null");
		return AIFW_INVALID_ATTRIBUTE;
	}
	uint16_t len = strlen(modelAttribute.version);
	mModelAttribute.version = new char[len + 1];
	if (!mModelAttribute.version) {
		AIFW_LOGE("Failed to allocate memory for model attribute version");
		return AIFW_NO_MEM;
	}
	strncpy((char *)mModelAttribute.version, modelAttribute.version, len + 1);

	if (modelAttribute.features != NULL && modelAttribute.featuresCount != 0) {
		mModelAttribute.features = new uint16_t[modelAttribute.featuresCount];
		if (!mModelAttribute.features) {
			AIFW_LOGE("Memory allocation failed for features array");
			clearModelAttribute();
			return AIFW_NO_MEM;
		}
		memcpy(mModelAttribute.features, modelAttribute.features, modelAttribute.featuresCount * sizeof(uint16_t));
	}

	if (modelAttribute.meanVals != NULL) {
		mModelAttribute.meanVals = new float[modelAttribute.invokeInputCount];
		if (!mModelAttribute.meanVals) {
			AIFW_LOGE("Memory allocation failed for mean values array");
			clearModelAttribute();
			return AIFW_NO_MEM;
		}
		memcpy(mModelAttribute.meanVals, modelAttribute.meanVals, modelAttribute.invokeInputCount * sizeof(float));
	}

	if (modelAttribute.stdVals != NULL) {
		mModelAttribute.stdVals = new float[modelAttribute.invokeInputCount];
		if (!mModelAttribute.stdVals) {
			AIFW_LOGE("Memory allocation failed for std values array");
			clearModelAttribute();
			return AIFW_NO_MEM;
		}
		memcpy(mModelAttribute.stdVals, modelAttribute.stdVals, modelAttribute.invokeInputCount * sizeof(float));
	}

	return AIFW_OK;
}

AIFW_RESULT AIModel::loadModel(const char *scriptPath)
{
	AIFW_LOGV("Lets try loading file based model");
	if (!scriptPath) {
		AIFW_LOGE("Script path is null.");
		return AIFW_INVALID_ARG;
	}
	AIFW_RESULT res = setModelAttributes(scriptPath);
	if (res != AIFW_OK) {
		AIFW_LOGE("model attributes fetch failed. error: %d", res);
		return res;
	}
	AIFW_LOGV("json file parsed, filename: %s", scriptPath);
	res = allocateMemory();
	if (res != AIFW_OK) {
		AIFW_LOGE("Internal memory allocation failed, error: %d", res);
		return res;
	}
	const char *file = mModelAttribute.modelPath;
	if (strlen(file) > 0) {
		res = mAIEngine->loadModel(file);
		if (res != AIFW_OK) {
			AIFW_LOGE("Load model failed, model file: %s, error: %d", file, res);
			return res;
		}
		AIFW_LOGV("model load done, model file: %s", file);
		res = createDataBuffer();
		if (res != AIFW_OK) {
			AIFW_LOGE("data buffer creation failed, error: %d", res);
		}
		AIFW_LOGV("AI framework buffer initialized");
		return res;
	}
	AIFW_LOGE("model file path is empty");
	return AIFW_ERROR;
}

AIFW_RESULT AIModel::loadModel(const AIModelAttribute &modelAttribute)
{
	AIFW_LOGV("Lets try loading array based model");
	AIFW_RESULT res = AIFW_OK;
	res = fillModelAttribute(modelAttribute);
	if (res != AIFW_OK) {
		AIFW_LOGE("Filling model attribute failed. error: %d", res);
		return AIFW_ERROR;
	}
	if (!mModelAttribute.model) {
		AIFW_LOGE("Array model is NULL.");
		return AIFW_INVALID_ATTRIBUTE;
	}
	res = allocateMemory();
	if (res != AIFW_OK) {
		AIFW_LOGE("Internal memory allocation failed, error: %d", res);
		return res;
	}
	res = mAIEngine->loadModel(mModelAttribute.model);
	if (res != AIFW_OK) {
		AIFW_LOGE("Load model failed, error %d", res);
		return res;
	}
	AIFW_LOGV("Model load done");
	res = createDataBuffer();
	if (res != AIFW_OK) {
		AIFW_LOGE("data buffer creation failed, error %d", res);
	} else {
		AIFW_LOGV("AI framework buffer initialized");
	}
	return res;
}

AIFW_RESULT AIModel::invoke(void)
{
	AIFW_RESULT res;
	float *invokeResult = nullptr;
	memset(mInvokeInput, '\0', mModelAttribute.invokeInputCount * sizeof(float));
	memset(mInvokeOutput, '\0', mModelAttribute.invokeOutputCount * sizeof(float));
	if (mDataProcessor) {
		AIFW_LOGV("data processor is set");
		memset(mPostProcessedData, '\0', mModelAttribute.postProcessResultCount * sizeof(float));

		res = mDataProcessor->preProcessData(mBuffer, mInvokeInput, &mModelAttribute);
		if (res != AIFW_OK) {
			AIFW_LOGE("preProcessData failed, error: %d", res);
			return res;
		}
#ifdef CONFIG_AIFW_LOGD
		printf("invokeInput: ");
		for (uint16_t i = 0; i < mModelAttribute.invokeInputCount; i++) {
			printf("%f,", mInvokeInput[i]);
		}
		printf("\n");
#endif
		invokeResult = (float *)mAIEngine->invoke(mInvokeInput);
		if (!invokeResult) {
			AIFW_LOGE("Engine Invoke failed.");
			return AIFW_ERROR;
		}
		AIFW_LOGV("invoke completed fine");
		for (uint16_t i = 0; i < mModelAttribute.invokeOutputCount; i++) {
			mInvokeOutput[i] = invokeResult[i];
		}
#ifdef CONFIG_AIFW_LOGD
		printf("invoke Output: ");
		for (uint16_t i = 0; i < mModelAttribute.invokeOutputCount; i++) {
			printf("%f,", mInvokeOutput[i]);
		}
		printf("\n");
#endif
		res = mBuffer->writeData(mInvokeOutput, mModelAttribute.invokeOutputCount, mModelAttribute.rawDataCount);
		if (res != AIFW_OK) {
			AIFW_LOGE("model output data write to buffer failed, error: %d", res);
			return res;
		}
		res = mDataProcessor->postProcessData(mBuffer, mPostProcessedData, &mModelAttribute);
		if (res < AIFW_OK) {
			AIFW_LOGE("data post processing failed, error: %d", res);
		}
		AIFW_LOGV("pre-process, invoke and post-process completed OK");
		return res;
	} else {
		AIFW_LOGV("No data processor case");
		res = mBuffer->readData(mInvokeInput, 0, mModelAttribute.invokeInputCount, 0);
		if (res != AIFW_OK) {
			AIFW_LOGE("Reading Data from the buffer failed, error: %d", res);
			return res;
		}
#ifdef CONFIG_AIFW_LOGD
		printf("invokeInput: ");
		for (uint16_t i = 0; i < mModelAttribute.invokeInputCount; i++) {
			printf("%f,", mInvokeInput[i]);
		}
		printf("\n");
#endif
		invokeResult = (float *)mAIEngine->invoke(mInvokeInput);
		if (!invokeResult) {
			AIFW_LOGE("Engine Invoke failed.");
			return AIFW_ERROR;
		}
		AIFW_LOGV("invoke completed fine");
		for (uint16_t i = 0; i < mModelAttribute.invokeOutputCount; i++) {
			mInvokeOutput[i] = invokeResult[i];
		}
#ifdef CONFIG_AIFW_LOGD
		printf("invoke Output: ");
		for (uint16_t i = 0; i < mModelAttribute.invokeOutputCount; i++) {
			printf("%f,", mInvokeOutput[i]);
		}
		printf("\n");
#endif
		res = mBuffer->writeData(mInvokeOutput, mModelAttribute.invokeOutputCount, mModelAttribute.invokeInputCount);
		if (res != AIFW_OK) {
			AIFW_LOGE("Writing invoke result to the buffer failed, error: %d", res);
			return res;
		}
		AIFW_LOGV("read data, invoke and write data completed OK");
		return res;
	}
}

AIFW_RESULT AIModel::pushData(void *data, uint16_t count)
{
	if (!data) {
		AIFW_LOGE("raw data argument is null");
		return AIFW_INVALID_ARG;
	}
	AIFW_RESULT res;
	if (mDataProcessor) {
		memset(mParsedData, '\0', mModelAttribute.rawDataCount * sizeof(float));
		res = mDataProcessor->parseData(data, count, mParsedData, &mModelAttribute);

		bool proceeding = false;
		if (res < AIFW_OK) {
			AIFW_LOGE("Parsing raw data failed, error: %d", res);
			return res;
		}

		/* If parseData returned AIFW_INFERENCE_PROCEEDING, it means it will collect more data, invoke will be skipped */
		if (res == AIFW_INFERENCE_PROCEEDING) {
			AIFW_LOGI("Proceeding now!!");
			proceeding = true;
		}
		
		res = mBuffer->writeData(mParsedData, mModelAttribute.rawDataCount);
		if (res != AIFW_OK) {
			AIFW_LOGE("Writing Data to the buffer failed, error: %d", res);
			return res;
		}

		/* So we will skip invoke */
		if (proceeding) {
			return AIFW_INFERENCE_PROCEEDING;
		}
	} else {
		if (count > mModelAttribute.invokeInputCount) {
			AIFW_LOGE("size of raw data buffer is greater than required size (raw data count)");
			return AIFW_NOT_ENOUGH_SPACE;
		}
		res = mBuffer->writeData((float *)data, count);
		if (res != AIFW_OK) {
			AIFW_LOGE("Writing Data to the buffer failed, error: %d", res);
			return res;
		}
	}

	res = invoke();
	/* AIFW_OK or AIFW_INFERENCE_FINISHED or AIFW_INFERENCE_PROCEEDING is fine */
	if (res < AIFW_OK) {
		AIFW_LOGE("Invoke Failed, error: %d", res);
	}
	return res;
}

AIFW_RESULT AIModel::setModelAttributes(const char *path)
{
	if (!path) {
		AIFW_LOGE("json file path is null.");
		return AIFW_INVALID_ARG;
	}
	AIManifestParser manifestParser;
	AIFW_RESULT res;
	res = manifestParser.loadManifestFile(path);
	if (res != AIFW_OK) {
		AIFW_LOGE("loadManifestFile failed");
		return AIFW_ERROR;
	}
	res = manifestParser.readData(&mModelAttribute);
	if (res != AIFW_OK) {
		AIFW_LOGE("readData from manifest file failed");
		return AIFW_ERROR;
	}
	return AIFW_OK;
}

AIModelAttribute AIModel::getModelAttribute(void)
{
	return mModelAttribute;
}

AIFW_RESULT AIModel::getResultData(float *data, uint16_t count)
{
	if (count < mModelAttribute.postProcessResultCount) {
		AIFW_LOGE("Size of input buffer is less than required buffer size");
		return AIFW_NOT_ENOUGH_SPACE;
	}
	if (mDataProcessor) {
		memcpy(data, mPostProcessedData, mModelAttribute.postProcessResultCount * sizeof(float));
		return AIFW_OK;
	}
	memcpy(data, mInvokeOutput, mModelAttribute.postProcessResultCount * sizeof(float));
	return AIFW_OK;
}

AIFW_RESULT AIModel::getRawData(float *data, uint16_t count)
{
	if (count < mModelAttribute.rawDataCount) {
		AIFW_LOGE("Size of input buffer is less than required buffer size");
		return AIFW_NOT_ENOUGH_SPACE;
	}
	AIFW_RESULT res = mBuffer->readData(data, 0, mModelAttribute.rawDataCount, 0);
	if (res != AIFW_OK) {
		AIFW_LOGE("Reading Data from the mBuffer failed.");
	}
	return res;
}

AIFW_RESULT AIModel::clearRawData(void)
{
	if (mBuffer) {
		return mBuffer->clear();
	} else {
		AIFW_LOGE("Buffer clear operation cannot be done");
		return AIFW_ERROR;
	}
}

AIFW_RESULT AIModel::clearRawData(uint16_t offset, uint16_t count)
{
	if (mBuffer) {
		return mBuffer->clear(offset, count);
	} else {
		AIFW_LOGE("Buffer clear operation cannot be done");
		return AIFW_ERROR;
	}
}

} /* namespace aifw */

