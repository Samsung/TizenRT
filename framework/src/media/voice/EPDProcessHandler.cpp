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

#include <memory>
#include "aifw/aifw.h"
#include "aifw/aifw_utils.h"
#include "aifw/AIDataBuffer.h"
#include "EPDProcessHandler.h"
#include <debug.h>

EPDProcessHandler::EPDProcessHandler()
{
	medvdbg("EPDProcessHandler cosntructor");
}

EPDProcessHandler::~EPDProcessHandler()
{
	medvdbg("EPDProcessHandler destructor");
}

AIFW_RESULT EPDProcessHandler::parseData(void *data, uint16_t count, float *parsedData, AIModelAttribute *modelAttribute)
{
	/** 
	 * Parse raw data values & store in parsedData buffer for writing in AI Framework buffer
	 * Finally, parsed data values are stored in parsedData buffer.
	 * e.g.
	 * Required raw data values can be selected using features key of json file
	 * New data values can be created using different raw data values.
	 */
	if (!data) {
		meddbg("raw data from source is NULL");
		return AIFW_INVALID_ARG;
	}
	if (!parsedData) {
		meddbg("parsed data buffer argument is NULL");
		return AIFW_INVALID_ARG;
	}
	/** 
	 * @todo Parse raw data values and fill values in parsedData buffer 
	 * Here, we have copied entire raw data into parsedData buffer.
	 */
	memcpy(parsedData, data, modelAttribute->rawDataCount * sizeof(float));
	/**
	 * We can also skip invoke until framework collects data as per windowSize key in json
	 * In order to skip inference return AIFW_INFERENCE_PROCEEDING from this function
	 */
	medvdbg("EPD model parse data complete OK");
	return AIFW_OK;
}

AIFW_RESULT EPDProcessHandler::preProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, uint16_t countInputSets, float **invokeInput, AIModelAttribute *modelAttribute)
{
	if (!buffer) {
		meddbg("Invalid argument - input data buffer");
		return AIFW_INVALID_ARG;
	}
	if (!invokeInput) {
		meddbg("Invalid argument - output data buffer");
		return AIFW_INVALID_ARG;
	}

	/* Preprocessing of raw data can be done at this point. Preprocess result save in invokeInput */
	/* Sample PreProcessing Logic : Filled Invokeinput with zeros and attached sample code for getting data from AIDataBuffer. */
	/* Sample Preprocessing Logic Start here. */
	/* Step 1 : Create memory to store one row of input data. */
	float *rawData = new float[modelAttribute->rawDataCount];
	if (!rawData) {
		meddbg("Memory Allocation failed - data read buffer");
		return AIFW_NO_MEM;
	}

	/* Step 2 : Read the parsed raw data from the latest row of buffer. At this time the latest row does not include invoke result. */
	AIFW_RESULT res = buffer->readData(rawData, 0, modelAttribute->rawDataCount, 0);
	if (res != AIFW_OK) {
		meddbg("Reading Data from the buffer failed.");
		delete[] rawData;
		return res;
	}

	/* Step 3 : Prepare InvokeInput for the model inference. */
	short *srawData = (short *)rawData;
	uint16_t inputSetSize[2] = {50, 100};
	memset(invokeInput[0], '\0', inputSetSize[0] * sizeof(float));
	memset(invokeInput[1], '\0', inputSetSize[1] * sizeof(float));

	/* Step 4 : Cleanup all local variables. */
	delete[] rawData;
	/* Sample Preprocessing Logic Ends here. */

	medvdbg("EPD model pre-process data complete OK");
	return AIFW_OK;
}

AIFW_RESULT EPDProcessHandler::postProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *resultData, AIModelAttribute *modelAttribute)
{
	if (!buffer) {
		meddbg("Invalid argument - input data buffer");
		return AIFW_INVALID_ARG;
	}
	if (!resultData) {
		meddbg("Invalid argument - output buffer");
		return AIFW_INVALID_ARG;
	}

	/* Post processing of result can be done at this point. Post Process result save in resultData */
	/* Sample PostProcessing Logic : Copy the invoke output. */
	/* Sample PostProcessing Logic Start here. */
	/* Step 1 : Create memory to store one row of data. */
	float *rawdata_invokeoutput = new float[modelAttribute->rawDataCount + modelAttribute->invokeOutputCount];
	if (!rawdata_invokeoutput) {
		meddbg("Memory Allocation failed - data read buffer");
		return AIFW_NO_MEM;
	}

	/* Step 2 : Read the latest raw from buffer. At this time latest row includes parsed raw data as well as invoke result. */
	AIFW_RESULT res = buffer->readData(rawdata_invokeoutput, 0);
	if (res != AIFW_OK) {
		meddbg("Reading Data from the buffer failed. error: %d", res);
		delete[] rawdata_invokeoutput;
		return res;
	}

	/* Step 3 : Add logic to copy the invoke output. */
	for (int i = 0; i < modelAttribute->postProcessResultCount; i++) {
		resultData[i] = rawdata_invokeoutput[modelAttribute->rawDataCount + i];
	}

	/* Step 4 : Cleanup all local variables. */
	delete[] rawdata_invokeoutput;
	/* Sample Postprocessing Logic Ends here. */

	medvdbg("EPD model post-process data complete OK");
	return res;
}
