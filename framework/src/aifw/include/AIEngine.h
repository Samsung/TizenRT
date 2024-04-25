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

/**
 * @file AIEngine.h
 * @brief Interface class for AI operations.
 */

#pragma once

#include "aifw/aifw.h"
//#define AIFW_PRINT_INFERENCE_TIME /* Default disabled */
#ifdef AIFW_PRINT_INFERENCE_TIME
#include <time.h>
#define AIFW_START_TIMER					\
	struct timespec start;              			\
	struct timespec end;					\
	double diff_time = 0;                   		\
	double x_ns;                            		\
	double y_ns;                            		\
	clock_gettime(CLOCK_REALTIME, &start);

#define AIFW_END_TIMER										\
{										 		\
	clock_gettime(CLOCK_REALTIME, &end);          				 		\
	x_ns = (double)start.tv_sec * 1000000000 + (double)start.tv_nsec;		    	\
	y_ns = (double)end.tv_sec * 1000000000 + (double)end.tv_nsec;       			\
	diff_time += (double)y_ns - (double)x_ns;                           			\
	diff_time = diff_time/1000;                                         			\
	printf("latency = (%f) us", diff_time);							\
}
#else
#define AIFW_START_TIMER
#define AIFW_END_TIMER
#endif

namespace aifw {

/**
 * @class AIEngine
 * @brief Interface class for AI operations.
 */
class AIEngine
{
public:
	/**
	 * @brief AIEngine class destructor.
	 */
	virtual ~AIEngine()
	{
	}

	/**
	 * @brief Load the TFLITE model from a file.
	 * @param [in] file: Path of TFLITE model file.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT loadModel(const char *file) = 0;

	/**
	 * @brief Load array model.
	 * @param [in] model: Model in array form.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT loadModel(const unsigned char *model) = 0;

#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	/**
	 * @brief Run the inference with the given inputData.
	 * @param [in] inputData: Input data for model invoke.
	 * @return: Void * pointer to output result data.
	 */
	virtual void *invoke(void *inputData) = 0;
#else
	/**
	 * @brief Run the inference with the given inputData.
	 * @param [in] inputData: Input data for model invoke.
	 * @param [out] outputData: Pointer to Output Data for storing output of invoke.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT invoke(void *inputData, void *outputData) = 0;

	/**
	 * @brief Pass model dimensions to AIModel to allocate memory.
	 * @param [in] inputSetCount: Number of Input sets for model invoke.
	 * @param [in] mInputSizeList: Input size array for model invoke.
	 * @param [in] outputSetCount: Number of Output sets for model invoke.
	 * @param [in] mOutputSizeList: Output size array for model invoke.
	 * @return: Void
	 */
	virtual void getModelDimensions(uint16_t *inputSetCount, uint16_t **inputSizeList,  uint16_t *outputSetCount, uint16_t **outputSizeList) = 0;
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
};

} /* namespace aifw */

