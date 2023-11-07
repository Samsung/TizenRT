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
	mModel(NULL), mBuf(NULL), mInterpreter(NULL), mErrorReporter(NULL), mInput(NULL), mOutput(NULL), mModelInputSize(0), mModelOutputSize(0)
{
	this->mTensorArenaSize = AIFW_TFLM_POOL_SIZE;
	AIFW_LOGV("Tensor Arena size: %d", this->mTensorArenaSize);
	std::shared_ptr<uint8_t> tensorArena(new uint8_t[this->mTensorArenaSize], std::default_delete<uint8_t[]>());
	if (tensorArena.get() == NULL) {
		AIFW_LOGE("tensor arena memory allocation failed");
	}
	this->mTensorArena = tensorArena;
}

TFLM::~TFLM()
{
	AIFW_LOGE(":DEINIT:");
	if (mBuf) {
		free(mBuf);
		mBuf = NULL;
	}
	mErrorReporter.reset();
	mInterpreter.reset();
}

AIFW_RESULT TFLM::_loadModel(void)
{
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
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (size < 0) {
		fclose(fp);
		AIFW_LOGE("File %s size read as %ld is invalid, errno %d", file, size, errno);
		return AIFW_ERROR_FILE_ACCESS;
	}
	AIFW_LOGV("Model File Size: %ld", size);
	this->mBuf = (char *)malloc(size);
	if (!this->mBuf) {
		fclose(fp);
		AIFW_LOGE("Memory not enough to allocate %ld", size);
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

/* Run inference : with input data "features" and return output data ptr(Use output dimension to parse it) */
void *TFLM::invoke(void *inputData)
{
	float *value = (float *)(inputData);
	for (int i = 0; i < this->mModelInputSize; i++) {
		this->mInput->data.f[i] = value[i];
	}
	TfLiteStatus invokeStatus = this->mInterpreter->Invoke();
	if (invokeStatus != kTfLiteOk) {
		this->mErrorReporter->Report("Invoke failed");
		AIFW_LOGE("Invoke failed");
		return NULL;
	}
	return this->mOutput->data.data;
}

} /* namespace aifw */

