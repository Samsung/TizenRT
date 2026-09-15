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

#include <iostream>

#include "tinyara/config.h"
#include "aifw/aifw_log.h"
#include "include/ONERTM.h"
#include "OMInterpreter.h"

namespace aifw {

ONERTM::ONERTM() :
	mBuf(NULL), mInterpreter(NULL), mConfig(NULL),
#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	mModelInputSize(0), mModelOutputSize(0)
#else
	mInputSizeList(NULL), mOutputSizeList(NULL), mInputSetCount(0), mOutputSetCount(0)
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
{
}

ONERTM::~ONERTM()
{
	AIFW_LOGD(":DEINIT:");
	if (mBuf) {
		free(mBuf);
		mBuf = NULL;
	}
	mInterpreter.reset();
#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	if (this->mInputSizeList) {
		delete[] this->mInputSizeList;
		this->mInputSizeList = NULL;
	}
	if (this->mOutputSizeList) {
		delete[] this->mOutputSizeList;
		this->mOutputSizeList = NULL;
	}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
}

AIFW_RESULT ONERTM::resetInferenceState(void)
{
	this->mInterpreter.reset();
	this->mInterpreter = std::make_shared<onert_micro::OMInterpreter>();
	this->mConfig = std::make_shared<onert_micro::OMConfig>();
	return AIFW_OK;
}

AIFW_RESULT ONERTM::_loadModel(void)
{
	this->mInterpreter = std::make_shared<onert_micro::OMInterpreter>();
	this->mConfig = std::make_shared<onert_micro::OMConfig>();
	this->mInterpreter->importModel(const_cast<const char *>(this->mBuf), *this->mConfig);

#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	// TODO: support multiple input/outputs
	this->mModelInputSize = this->mInterpreter->getInputSizeAt(0);
	this->mModelOutputSize = this->mInterpreter->getOutputSizeAt(0);
#else
	this->mInputSetCount = mInterpreter->getNumberOfInputs();
	this->mOutputSetCount = mInterpreter->getNumberOfOutputs();
	this->mInputSizeList = new uint16_t[this->mInputSetCount];
	if (!this->mInputSizeList) {
		AIFW_LOGE("Internal memory allocation failed");
		return AIFW_NO_MEM;
	}
	this->mOutputSizeList = new uint16_t[this->mOutputSetCount];
	if (!this->mOutputSizeList) {
		AIFW_LOGE("Internal memory allocation failed");
		return AIFW_NO_MEM;
	}
	for (uint16_t i = 0; i < this->mInputSetCount; i++) {
		this->mInputSizeList[i] = this->mInterpreter->getInputSizeAt(i);
	}
	for (uint16_t i = 0; i < this->mOutputSetCount; i++) {
		this->mOutputSizeList[i] = this->mInterpreter->getOutputSizeAt(i);
	}
	AIFW_LOGD("number of inputs: %d \n", this->mInputSetCount);
	for (uint16_t i = 0; i < this->mInputSetCount; i++) {
		AIFW_LOGD("Model Input Size [%d] =%d\n", i, this->mInputSizeList[i]);
	}
	AIFW_LOGD("number of outputs: %d \n", this->mOutputSetCount);
	for (uint16_t i = 0; i < this->mOutputSetCount; i++) {
		AIFW_LOGD("Model Output Size [%d] = %d\n", i, this->mOutputSizeList[i]);
	}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */

	AIFW_LOGD("Interpreter initialization success.");

	return AIFW_OK;
}

AIFW_RESULT ONERTM::loadModel(const char *file)
{
	AIFW_LOGD("GetModel from File:%s", file);
	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		AIFW_LOGE("File %s open operation failed errno : %d", file, errno);
		return AIFW_ERROR_FILE_ACCESS;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (size < 0) {
		fclose(fp);
		AIFW_LOGE("File %s size read as %d is invalid, errno %d", file, size, errno);
		return AIFW_ERROR_FILE_ACCESS;
	}
	AIFW_LOGD("Model File Size: %d", size);
	this->mBuf = (char *)malloc(size);
	if (!this->mBuf) {
		fclose(fp);
		AIFW_LOGE("Memory not enough to allocate %d", size);
		return AIFW_NO_MEM;
	}
	fread(this->mBuf, 1, size, fp);
	fclose(fp);

	AIFW_LOGD("GetModel from Model file");

	AIFW_LOGD("Model Loaded from file %s", file);
	
	return _loadModel();
}

AIFW_RESULT ONERTM::loadModel(const unsigned char *model)
{
	this->mBuf = reinterpret_cast<char*>(const_cast<unsigned char *>(model));
	return _loadModel();
}

#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
void ONERTM::getModelDimensions(uint16_t *inputSetCount, uint16_t **inputSizeList, uint16_t *outputSetCount, uint16_t **outputSizeList)
{
	*inputSetCount = this->mInputSetCount;
	*inputSizeList = this->mInputSizeList;
	*outputSetCount = this->mOutputSetCount;
	*outputSizeList = this->mOutputSizeList;
	AIFW_LOGD("input set count : %d, output set count : %d", *inputSetCount, *outputSetCount);
}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */

#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
/* Run inference : with input data "features" and return output data ptr(Use output dimension to parse it) */
void *ONERTM::invoke(void *inputData)
{
	float *value = (float *)(inputData);
	this->mInterpreter->reset();
	this->mInterpreter->allocateInputs();
	auto *data = this->mInterpreter->getInputDataAt(0);
	for (uint32_t i = 0; i < this->mModelInputSize; ++i) {
		reinterpret_cast<float *>(data)[i] = value[i];
	}
	AIFW_START_TIMER
	this->mInterpreter->run(*this->mConfig);
	AIFW_END_TIMER

	return this->mInterpreter->getOutputDataAt(0);
}
#else
/* Run inference : with input data "features", store output data in outputData parameter and return AIFW_OK on success */
AIFW_RESULT ONERTM::invoke(void *inputData, void *outputData)
{
	
	this->mInterpreter->reset();
	this->mInterpreter->allocateInputs();
	float **value = (float **)(inputData);
	float **output = (float **)(outputData);
	for (uint16_t i = 0; i < this->mInputSetCount; i++) {
		auto *data = this->mInterpreter->getInputDataAt(i);
		for (uint32_t j = 0; j < this->mInputSizeList[i]; j++) {
			reinterpret_cast<float *>(data)[j] = value[i][j];
		}
	}

	AIFW_START_TIMER
	this->mInterpreter->run(*this->mConfig);
	AIFW_END_TIMER
	for (uint16_t i = 0; i < this->mOutputSetCount; i++) {
		output[i] = (float *)this->mInterpreter->getOutputDataAt(i);
	}
	return AIFW_OK;
}

#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
} /* namespace aifw */
