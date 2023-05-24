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
#include <iostream>
#include <tensorflow/lite/c/common.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/micro_profiler.h>

#include "lw_aifw/lw_aifw_log.h"
#include "include/AIModel.h"

namespace lw_aifw {
	tflite::AllOpsResolver g_Resolver;
	tflite::MicroProfiler g_Profiler;
	/*
	input: memorySize in Bytes
	*/
	AIModel::AIModel(size_t memorySize): m_Model(NULL), mBuf(NULL), m_Interpreter(NULL), m_ErrorReporter(NULL), m_Input(NULL), m_Output(NULL), m_InputField(0), m_OutputField(0)
	{
		this->m_TensorArenaSize = memorySize;
		std::shared_ptr<uint8_t> tensorArena(new uint8_t[this->m_TensorArenaSize], std::default_delete<uint8_t[]>());
		this->m_TensorArena = tensorArena;
	}

	AIModel::~AIModel()
	{
		if(mBuf) {
			free(mBuf);
			mBuf = NULL;
		}
		LW_AIFW_LOGV("Destructor");
	}

	LW_AIFW_RESULT AIModel::_loadModel(void)
	{
		m_ErrorReporter = std::make_shared<tflite::MicroErrorReporter>();
		this->m_Interpreter = std::make_shared<tflite::MicroInterpreter>(
			this->m_Model,
			g_Resolver,
			this->m_TensorArena.get(),
			this->m_TensorArenaSize,
			nullptr,
			&g_Profiler
		);

		TfLiteStatus allocate_status = this->m_Interpreter->AllocateTensors();
		if (allocate_status != kTfLiteOk) {
			this->m_ErrorReporter->Report("AllocateTensors() failed");
			LW_AIFW_LOGE("AllocateTensors() failed");
			return LW_AIFW_ERROR;
		}
		LW_AIFW_LOGV("AllocateTensors success.");
		this->m_Input = this->m_Interpreter->input(0);
		this->m_Output = this->m_Interpreter->output(0);

		int input_size = this->m_Input->dims->size;
		int out_size = this->m_Output->dims->size;
		LW_AIFW_LOGV("input_size =%d Member are:",input_size);
		this->m_InputField = 1;
		for(int i = 0; i < input_size; i++) {
			LW_AIFW_LOGV("%d",this->m_Input->dims->data[i]);
			this->m_InputField *= this->m_Input->dims->data[i];
		}

		LW_AIFW_LOGV("out_size: %d Member are:", out_size);
		this->m_OutputField = 1;
		for(uint8_t i=0;i<out_size;i++) {
			LW_AIFW_LOGV("%d ",this->m_Output->dims->data[i]);
			this->m_OutputField *= this->m_Output->dims->data[i];
		}
		LW_AIFW_LOGV("m_InputField = %d m_OutputField = %d", m_InputField, m_OutputField);
		LW_AIFW_LOGV("Model loaded successfully.");
		return LW_AIFW_OK;
	}

	LW_AIFW_RESULT AIModel::loadModel(const char *name)
	{
		LW_AIFW_LOGV("GetModel from File:%s",name);
		FILE *fp = fopen(name, "r");
		if(fp == NULL) {
			LW_AIFW_LOGE("File %s open operation failed errno : %d", name, errno);
			return LW_AIFW_ERROR_FILE_ACCESS;
		}
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (size < 0) {
			fclose(fp);
			LW_AIFW_LOGE("File %s size read as %d is invalid, errno %d", name, size, errno);
			return LW_AIFW_ERROR_FILE_ACCESS;
		}
		LW_AIFW_LOGV("Model File Size: %d", size);
		this->mBuf = (char *) malloc(size);
		if(!this->mBuf) {
			fclose(fp);
			LW_AIFW_LOGE("Memory not enough to allocate %d", size);
			return LW_AIFW_NO_MEM;
		}
		fread(this->mBuf, 1, size, fp);
		fclose(fp);

		LW_AIFW_LOGV("GetModel from Model file");
		this->m_Model = tflite::GetModel((const void *)this->mBuf);

		if (this->m_Model->version() != TFLITE_SCHEMA_VERSION) {
			LW_AIFW_LOGE("Error:Model Version Mismatch");
			return LW_AIFW_ERROR;
		} else {
			LW_AIFW_LOGV("Model Loaded from file %s", name);
		}
		_loadModel();
		return LW_AIFW_OK;
	}

	LW_AIFW_RESULT AIModel::loadModel(unsigned char *name)
	{
		this->m_Model = tflite::GetModel(name);
		if (this->m_Model->version() != TFLITE_SCHEMA_VERSION) {
			LW_AIFW_LOGE("GetModel from array failed");
			return LW_AIFW_ERROR;
		}
		else{
			LW_AIFW_LOGV("Model Loaded from array");
		}
		_loadModel();
		return LW_AIFW_OK;
	}

	//Run inference : with input data "features" and return output data ptr(Use output dimension to parse it)
	void *AIModel::invoke(void *inputData) {
		int inputSize;
		int outSize;
		// Deep copy
		float* value = (float*)(inputData);
		for (int i = 0; i < this->m_InputField; i++) {
			this->m_Input->data.f[i] = value[i];
		}
		inputSize = this->getInArraySize();
		outSize = this->getOutArraySize();
		LW_AIFW_LOGV("input_size =%d Member are:", inputSize);

		for(uint8_t i = 0 ;i < inputSize; i++){
			LW_AIFW_LOGV("%d ", this->m_Input->dims->data[i]);
		}
		LW_AIFW_LOGV("out_size =%d Member are:", outSize);

		for(uint8_t i = 0; i < outSize; i++){
			LW_AIFW_LOGV("%d ", this->m_Output->dims->data[i]);
		}
		TfLiteStatus invokeStatus = this->m_Interpreter->Invoke();
		if (invokeStatus != kTfLiteOk) {
			this->m_ErrorReporter->Report("Invoke failed");
			LW_AIFW_LOGE("Invoke failed");
			return NULL;
		}
		return this->m_Output->data.data;
	}
	/*
		OUT : size of result/output array
	*/
	uint8_t AIModel::getOutArraySize()
	{
		uint8_t outArrSize = 0;
		if (this->m_Output->dims) {
			outArrSize = this->m_Output->dims->size;
		}
		else{
			LW_AIFW_LOGE("Output tensor dimension is NULL hence returning size zero");
		}
		return outArrSize;
	}

	/*
		IN: array size and calller allocated array to be filled
		OUT : 0 :FALSE on Fail, 1: TRUE on Success
	*/
	bool AIModel::getOutputArr(uint8_t size, uint16_t *arr) {
		bool ret = FALSE;
		if (this->m_Output->dims) {
			for (uint8_t i = 0; i < size; i++) {
				arr[i] = this->m_Output->dims->data[i];
			}
			ret = TRUE;
		}
		else{
			LW_AIFW_LOGE("Output tensor dimension is NULL hence returning FALSE");
		}
		return ret;
	}

/*
	IN: void.
	OUT : size of sample/input array
*/
	uint8_t AIModel::getInArraySize() {
		uint8_t inArrSize = 0;
		if (this->m_Input->dims){
			inArrSize = this->m_Input->dims->size;
		}
		else{
			LW_AIFW_LOGE("Input tensor dimension is NULL hence returning size zero");
		}
		return inArrSize;
	}
/*
	IN: array size and calller allocated array to be filled
	OUT : 0 :FALSE on Fail, 1: TRUE on Success
*/
	bool AIModel::getInputArr(uint8_t size, uint16_t *arr) {
		bool ret = FALSE;
		if (this->m_Input->dims) {
			for (uint8_t i = 0; i < size; i++) {
				arr[i] = this->m_Input->dims->data[i];
			}
			ret = TRUE;
		}
		else{
			LW_AIFW_LOGE("Input tensor dimension is NULL hence returning FALSE");
		}
		return ret;
	}

	void AIModel::getInOutDimensions(int *in, int *out) {
		*in = m_InputField;
		*out = m_OutputField;
	}

} //lw_aifw

