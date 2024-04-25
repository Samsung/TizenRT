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
#include <iostream>
#include <tensorflow/lite/c/common.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/micro_profiler.h>

#include "aifw/aifw_log.h"
#include "include/TFLM.h"

#ifndef CONFIG_TFLM_MEM_POOL_SIZE
#define AIFW_TFLM_POOL_SIZE 8192
#else
#define AIFW_TFLM_POOL_SIZE CONFIG_TFLM_MEM_POOL_SIZE
#endif

namespace aifw {

tflite::AllOpsResolver g_Resolver;
tflite::MicroProfiler g_Profiler;
TFLM::TFLM() :
	mModel(NULL), mBuf(NULL), mInterpreter(NULL), mErrorReporter(NULL),
#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	mInput(NULL), mOutput(NULL), mModelInputSize(0), mModelOutputSize(0)
#else
	mInputList(NULL), mOutputList(NULL), mInputSizeList(NULL), mOutputSizeList(NULL)
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
{
	this->mTensorArenaSize = AIFW_TFLM_POOL_SIZE;
	AIFW_LOGV("Tensor Arena size: %d", this->mTensorArenaSize);
	std::shared_ptr<uint8_t> tensorArena(new uint8_t[this->mTensorArenaSize], std::default_delete<uint8_t[]>());
	if (tensorArena.get() == NULL) {
		AIFW_LOGE("tensor arena memory allocation failed");
	}
	this->mTensorArena = tensorArena;
}

#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
void TFLM::clearMemory(void) {
	if (this->mInputList) {
		delete[] mInputList;
		mInputList = NULL;
	}
	if (this->mOutputList) {
		delete[] mOutputList;
		mOutputList = NULL;
	}
	if (this->mInputSizeList) {
		delete[] this->mInputSizeList;
		this->mInputSizeList = NULL;
	}
	if (this->mOutputSizeList) {
		delete[] this->mOutputSizeList;
		this->mOutputSizeList = NULL;
	}
}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */

TFLM::~TFLM()
{
	AIFW_LOGV(":DEINIT:");
	if (mBuf) {
		free(mBuf);
		mBuf = NULL;
	}
	mErrorReporter.reset();
	mInterpreter.reset();
#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	clearMemory();
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
}

#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
AIFW_RESULT TFLM::allocateMemory(void)
{
	uint16_t *input_dims_size;
	uint16_t *output_dims_size;
	this->mInputSetCount = this->mInterpreter->inputs_size();
	this->mInputList = new TfLiteTensor *[this->mInputSetCount];
	if (!this->mInputList) {
		AIFW_LOGE("Memory Allocation failed - model input buffer");
		goto mem_alloc_error;
	}
	this->mOutputSetCount = this->mInterpreter->outputs_size();
	this->mOutputList = new TfLiteTensor *[this->mOutputSetCount];
	if (!this->mOutputList) {
		AIFW_LOGE("Memory Allocation failed - model output buffer");
		goto mem_alloc_error;
	}
	input_dims_size = new uint16_t[this->mInputSetCount];
	if (!input_dims_size) {
		AIFW_LOGE("Internal Memory Allocation failed.");
		goto mem_alloc_error;
	}
	output_dims_size = new uint16_t[this->mOutputSetCount];
	if (!output_dims_size) {
		AIFW_LOGE("Internal Memory Allocation failed.");
		goto mem_alloc_error;
	}
	this->mInputSizeList = new uint16_t[this->mInputSetCount];
	if (!this->mInputSizeList) {
		AIFW_LOGE("Internal Memory Allocation failed.");
		goto mem_alloc_error;
	}
	this->mOutputSizeList = new uint16_t[this->mOutputSetCount];
	if (!this->mOutputSizeList) {
		AIFW_LOGE("Internal Memory Allocation failed.");
		goto mem_alloc_error;
	}
	for (uint16_t i = 0; i < this->mInputSetCount; i++) {
		this->mInputList[i] = this->mInterpreter->input(i);
		input_dims_size[i] = this->mInputList[i]->dims->size;
		this->mInputSizeList[i] = 1;
		AIFW_LOGV("inputset %d: ", i);
		for (uint16_t j = 0; j < input_dims_size[i]; j++) {
			AIFW_LOGV("InputDims[%d] value: %d", j, this->mInputList[i]->dims->data[j]);
			this->mInputSizeList[i] *= this->mInputList[i]->dims->data[j];
		}
		AIFW_LOGV("mInputSizeList[%d] =  %d\n", i, mInputSizeList[i]);
	}
	for (uint16_t i = 0; i < this->mOutputSetCount; i++) {
		this->mOutputList[i] = this->mInterpreter->output(i);
		output_dims_size[i] = this->mOutputList[i]->dims->size;
		this->mOutputSizeList[i] = 1;
		AIFW_LOGV("outputset %d: ", i);
		for (uint16_t j = 0; j < output_dims_size[i]; j++) {
			AIFW_LOGV("OutputDims[%d] value: %d", j, this->mOutputList[i]->dims->data[j]);
			this->mOutputSizeList[i] *= this->mOutputList[i]->dims->data[j];
		}
		AIFW_LOGV("mOutputSizeList[%d] =  %d\n", i, mOutputSizeList[i]);
	}
	return AIFW_OK;

mem_alloc_error:
	clearMemory();
	return AIFW_NO_MEM;
}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */

AIFW_RESULT TFLM::_loadModel(void)
{
	AIFW_RESULT res;
	mErrorReporter = std::make_shared<tflite::MicroErrorReporter>();
	this->mInterpreter = std::make_shared<tflite::MicroInterpreter>(
		this->mModel,
		g_Resolver,
		this->mTensorArena.get(),
		this->mTensorArenaSize,
		nullptr,
		&g_Profiler);

	TfLiteStatus allocate_status = this->mInterpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
		this->mErrorReporter->Report("AllocateTensors() failed");
		AIFW_LOGE("AllocateTensors() failed");
		return AIFW_ERROR;
	}
	AIFW_LOGV("AllocateTensors success.");
#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	this->mInput = this->mInterpreter->input(0);
	this->mOutput = this->mInterpreter->output(0);

	int input_dims_size = this->mInput->dims->size;
	int output_dims_size = this->mOutput->dims->size;
#if 0
	AIFW_LOGV("input_dims_size =%d Member are:", input_dims_size);
	for (int i = 0; i < input_dims_size; i++) {
		AIFW_LOGV("%d", this->mInput->dims->data[i]);
	}
#endif
	this->mModelInputSize = 1;
	for (int i = 0; i < input_dims_size; i++) {
		AIFW_LOGV("InputDims[%d] value: %d", i, this->mInput->dims->data[i]);
		this->mModelInputSize *= this->mInput->dims->data[i];
	}
#if 0
	AIFW_LOGV("output_dims_size: %d Member are:", output_dims_size);
	for (uint8_t i = 0; i < output_dims_size; i++) {
		AIFW_LOGV("%d ", this->mOutput->dims->data[i]);
	}
#endif
	this->mModelOutputSize = 1;
	for (uint8_t i = 0; i < output_dims_size; i++) {
		AIFW_LOGV("OutputDims[%d] value: %d", i, this->mOutput->dims->data[i]);
		this->mModelOutputSize *= this->mOutput->dims->data[i];
	}
	AIFW_LOGV("mModelInputSize = %d mModelOutputSize = %d", mModelInputSize, mModelOutputSize);
#else
	res = allocateMemory();
	if (res != AIFW_OK) {
		return res;
	}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
	AIFW_LOGV("Model loaded successfully.");
	return AIFW_OK;
}

AIFW_RESULT TFLM::loadModel(const char *file)
{
	AIFW_LOGV("GetModel from File:%s", file);
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
	AIFW_LOGV("Model File Size: %d", size);
	this->mBuf = (char *)malloc(size);
	if (!this->mBuf) {
		fclose(fp);
		AIFW_LOGE("Memory not enough to allocate %d", size);
		return AIFW_NO_MEM;
	}
	fread(this->mBuf, 1, size, fp);
	fclose(fp);

	AIFW_LOGV("GetModel from Model file");
	this->mModel = tflite::GetModel((const void *)this->mBuf);

	if (this->mModel->version() != TFLITE_SCHEMA_VERSION) {
		AIFW_LOGE("Error:Model Version Mismatch");
		return AIFW_ERROR;
	} else {
		AIFW_LOGV("Model Loaded from file %s", file);
	}
	return _loadModel();
}

AIFW_RESULT TFLM::loadModel(const unsigned char *model)
{
	this->mModel = tflite::GetModel(model);
	if (this->mModel->version() != TFLITE_SCHEMA_VERSION) {
		AIFW_LOGE("GetModel from array failed");
		return AIFW_ERROR;
	} else {
		AIFW_LOGV("Model Loaded from array");
	}
	return _loadModel();
}

#ifdef CONFIG_AIFW_MULTI_INOUT_SUPPORT
void TFLM::getModelDimensions(uint16_t *inputSetCount, uint16_t **inputSizeList, uint16_t *outputSetCount, uint16_t **outputSizeList)
{
	*inputSetCount = this->mInputSetCount;
	*inputSizeList = this->mInputSizeList;
	*outputSetCount = this->mOutputSetCount;
	*outputSizeList = this->mOutputSizeList;
}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */

#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
/* Run inference : with input data "features" and return output data ptr(Use output dimension to parse it) */
void *TFLM::invoke(void *inputData)
{
	float *value = (float *)(inputData);
	for (int i = 0; i < this->mModelInputSize; i++) {
		this->mInput->data.f[i] = value[i];
	}
	AIFW_START_TIMER
	TfLiteStatus invokeStatus = this->mInterpreter->Invoke();
	AIFW_END_TIMER
	if (invokeStatus != kTfLiteOk) {
		this->mErrorReporter->Report("Invoke failed");
		AIFW_LOGE("Invoke failed");
		return NULL;
	}
	return this->mOutput->data.data;
}
#else
/* Run inference : with input data "features", store output data in outputData parameter and return AIFW_OK on success */
AIFW_RESULT TFLM::invoke(void *inputData, void *outputData)
{
	float **value = (float **)(inputData);
	for (uint16_t i = 0; i < this->mInputSetCount; i++) {
		for (uint16_t j = 0; j < this->mInputSizeList[i]; j++) {
			this->mInputList[i]->data.f[j] = value[i][j];
		}
	}
	AIFW_START_TIMER
	TfLiteStatus invokeStatus = this->mInterpreter->Invoke();
	AIFW_END_TIMER
	if (invokeStatus != kTfLiteOk) {
		this->mErrorReporter->Report("Invoke failed");
		AIFW_LOGE("Invoke failed");
		return AIFW_ERROR;
	}
	float **outputRef = (float **)(outputData);
	for (uint16_t i = 0; i < this->mOutputSetCount; i++) {
		outputRef[i] = (float *)this->mOutputList[i]->data.data;
	}
	return AIFW_OK;
}
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
} /* namespace aifw */

