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
 * @file aifw/aifw.h
 * @brief This file defines/declares structures & functions exposed by framework for application.
 */

#pragma once

#include "stdint.h"
#include "limits.h"
#include <errno.h>

#define AIFW_MAX_FILEPATH_LEN 32 + 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Common set of return values used in AI Framework functions.
 */
typedef enum _AIFW_RESULT {
	AIFW_OK = 0,			/* OK: Without any error  */
	AIFW_INFERENCE_FINISHED = 1,	/* Inference Finished */
	AIFW_INFERENCE_PROCEEDING = 2,	/* Return it if any of ai model should invoke many times for single inference or it should skip invoke until it collects raw data certain number of times */
	AIFW_ERROR = -1,		/* ERROR: All other types of error not specified by any following enum */
	AIFW_NO_MEM = -2,		/* Memory allocation (malloc/calloc) failed */
	AIFW_ERROR_FILE_ACCESS = -3,	/* File access error */
	AIFW_BUFFER_EMPTY = -4,	/* No data availbale in Read/write/other data buffer */
	AIFW_READ_ALREADY = -5,	/* all data already read from a read/write/other buffer */
	AIFW_NOT_ENOUGH_SPACE = -6,	/* read/write/other buffer has empty space less than required size */
	AIFW_INVALID_ARG = -7,	/* Invalid argument */
	AIFW_SERVICE_NOT_RUNNING = -8,	/* Data Service Not Running */
	AIFW_INFERENCE_ERROR = -9,	/* Inference Error */
	AIFW_INVALID_RAWDATA = -10,	/* Invalid Raw Data */
	AIFW_INVOKE_OUT_OF_BOUNDS = -11,	/* Invoke output is out of bound */
	AIFW_INVALID_ATTRIBUTE = -12,	/* Invalid argument in manifest file */
	AIFW_CSV_EMPTY_LINE = -13,	/* CSV has empty line or empty field */
	AIFW_SOURCE_EOF = -14,	/* End Of File or End of Source data */
} AIFW_RESULT;

/**
 * @brief: AI Framework calls this function to collect the raw data and pass it for inference.
 * This callback is called when timer expires. Time interval is set in 'inferenceInterval' field of AIModelAttribute structure.
 * It is expected that callee will not hold this thread.
*/
typedef void (*CollectRawDataListener)(void);

/**
 * @brief: Application will receive inference result in this function.
 * It is mandatory to be defined by each application.
 * An application can use this function to set inference result to device via SCube.
 * @param [in] res: Successful inference operation 'res' is set to AIFW_OK. Errors are set as per AIFW_RESULT enum values.
 * @param [in] values: inference result values after ensembling 
 * @param [in] count: count of values in inference result
*/
typedef void (*InferenceResultListener)(AIFW_RESULT res, void *values, uint16_t count);

/**
 * @brief This structure defines member fields to store properties of an AI Model.
 * crc32: CRC value of AI Model and Manifest file
 * version: AI Model version
 * modelPath: Path of file based AI Model
 * model: Array based AI Model
 * features: Features list to identify relevant raw data values for AI Model from data source
 * featuresCount: Number of elements in Features list
 * inferenceInterval: Interval at which data is sent to model for inference
 * modelCode: 32 bit value to identify model for OTN
 * maxRowsDataBuffer: Maximum number of rows to store in AI data buffer
 * rawDataCount: Count of parsed rawdata in one row of AI data buffer
 * windowSize: Number of  data set rows required for performing pre processing operation
 * invokeInputCount: Number of inputs to model
 * invokeOutputCount: Number of outpts from model
 * postProcessResultCount: Count of post processed result
 * inferenceResultCount: Number of primitive data values sent to application after inference of a modelset
 * MeanVals: List of mean values used in normalization
 * STDVals: List of standard deviation values used in normalization
 */
struct AIModelAttribute {
	uint32_t crc32;
	const char *version;
	char modelPath[AIFW_MAX_FILEPATH_LEN];
	const unsigned char *model;
	uint16_t *features;
	uint16_t featuresCount;
	uint32_t inferenceInterval;
	uint32_t modelCode;
	uint16_t maxRowsDataBuffer;
	uint16_t rawDataCount;
	uint16_t windowSize;
	uint16_t invokeInputCount;
	uint16_t invokeOutputCount;
	uint16_t postProcessResultCount;
	uint16_t inferenceResultCount;
	float *meanVals;
	float *stdVals;
};

#ifdef __cplusplus
}
#endif

