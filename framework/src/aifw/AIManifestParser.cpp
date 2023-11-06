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

#include <stdlib.h>
#include "aifw/aifw_log.h"
#include "aifw/aifw.h"
#include "include/AIManifestParser.h"

#define MANIFEST_KEY_VERSION "version"
#define MANIFEST_KEY_MODEL_TYPE "modeltype"
#define MANIFEST_KEY_MODEL_NAME "modelname"
#define MANIFEST_KEY_MODEL_PATH "modelfile"
#define MANIFEST_KEY_INVOKE_INPUT_COUNT "invokeinputcount"
#define MANIFEST_KEY_INVOKE_OUTPUT_COUNT "invokeoutputcount"
#define MANIFEST_KEY_WINDOW_SIZE "windowsize"
#define MANIFEST_KEY_MAX_ROWS_DATA_BUFFER "maxrowsdatabuffer"
#define MANIFEST_KEY_FEATURES "features"
#define MANIFEST_KEY_RAW_DATA_COUNT "rawdatacount"
#define MANIFEST_KEY_POST_PROCESS_RESULT_COUNT "postprocessresultcount"
#define MANIFEST_KEY_INFERENCE_RESULT_COUNT "inferenceresultcount"
#define MANIFEST_KEY_CRC32 "crc32"
#define MANIFEST_KEY_PREPROCESSING "preprocessing"
#define MANIFEST_KEY_MEAN_VALS "mean"
#define MANIFEST_KEY_STD_VALS "std"
#define MANIFEST_KEY_MODELS "models"
#define MANIFEST_KEY_INFERENCE_INTERVAL "inferenceinterval"
#define MANIFEST_KEY_MODEL_CODE "modelcode"

#define NULL_STRING "(null)"

namespace aifw {

AIManifestParser::~AIManifestParser()
{
	AIFW_LOGV(":DEINIT:");
	mJSON.reset();
}

AIFW_RESULT AIManifestParser::loadManifestFile(const char *path)
{
	FILE *f = fopen(path, "rb");
	if (!f) {
		AIFW_LOGE("error: Cannot open file at path: %s", path);
		return AIFW_ERROR;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = new char[fsize + 1];
	fread(buffer, fsize, 1, f);
	fclose(f);

	std::shared_ptr<cJSON> json(cJSON_Parse(buffer), [](void *p) {
		cJSON_Delete((cJSON *)p);
	});

	this->mJSON = json;
	if (!this->mJSON.get()) {
		AIFW_LOGE("error: Cannot parse manifest.json!");
		delete[] buffer;
		return AIFW_ERROR;
	}
	delete[] buffer;
	return AIFW_OK;
}

AIFW_RESULT AIManifestParser::readData(AIModelAttribute *modelAttribute)
{
	modelAttribute->version = NULL;
	modelAttribute->features = NULL;
	modelAttribute->meanVals = NULL;
	modelAttribute->stdVals = NULL;

	AIFW_RESULT ret = AIFW_OK;
	cJSON *version, *modelfile, *features, *maxrowsdatabuffer, *rawdatacount, *windowsize, *invokeinputcount, *invokeoutputcount, *postprocessresultcount, *inferenceresultcount, *crc, *preprocess, *meanVals, *stdVals, *inferenceinterval, *modelcode;
	uint16_t len;
	char *file;
	//	Get AI version
	version = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_VERSION);
	if (!version) {
		AIFW_LOGE("No AI version in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	if (!version->valuestring) {
		AIFW_LOGE(" AI version in the manifest is null");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	len = strlen(version->valuestring);
	modelAttribute->version = new char[len + 1];
	if (!modelAttribute->version) {
		AIFW_LOGE("Failed to allocate memory for model attribute version");
		ret = AIFW_NO_MEM;
		goto cleanup;
	}
	strncpy((char *)modelAttribute->version, version->valuestring, len + 1);

	//	Get ModelFile path
	modelfile = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_MODEL_PATH);
	if (!modelfile) {
		AIFW_LOGE("No Model file path in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	if (!modelfile->valuestring) {
		AIFW_LOGE(" Model file path in the manifest is null");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	file = modelfile->valuestring;
	strncpy(modelAttribute->modelPath, file, AIFW_MAX_FILEPATH_LEN);

	//	Get Features
	features = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_FEATURES);
	if (!features) {
		AIFW_LOGE("No features array in the manifest");
	} else {
		modelAttribute->featuresCount = cJSON_GetArraySize(features);
		if (modelAttribute->featuresCount > 0) {
			modelAttribute->features = new uint16_t[modelAttribute->featuresCount];
			if (!modelAttribute->features) {
				AIFW_LOGE("Failed to allocate memory for features");
				ret = AIFW_NO_MEM;
				goto cleanup;
			}
			for (int i = 0; i < modelAttribute->featuresCount; i++) {
				cJSON *featureval = cJSON_GetArrayItem(features, i);
				modelAttribute->features[i] = featureval->valueint;
			}
		} else {
			modelAttribute->features = NULL;
			AIFW_LOGV("features count is 0");
		}
	}

	// Get MaxRowsDataBuffer
	maxrowsdatabuffer = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_MAX_ROWS_DATA_BUFFER);
	if (!maxrowsdatabuffer) {
		AIFW_LOGE("No maxrowsdatabuffer in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->maxRowsDataBuffer = maxrowsdatabuffer->valueint;

	// Get RawDataCount
	rawdatacount = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_RAW_DATA_COUNT);
	if (!rawdatacount) {
		AIFW_LOGE("No rawdatacount in the manifest!");
	} else {
		modelAttribute->rawDataCount = rawdatacount->valueint;
	}

	//	Get WindowSize
	windowsize = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_WINDOW_SIZE);
	if (!windowsize) {
		AIFW_LOGE("No windowSize in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->windowSize = windowsize->valueint;

	//	Get invokeinputcount
	invokeinputcount = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_INVOKE_INPUT_COUNT);
	if (!invokeinputcount) {
		AIFW_LOGE("No invokeinputcount in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->invokeInputCount = invokeinputcount->valueint;

	//	Get invokeoutputcount
	invokeoutputcount = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_INVOKE_OUTPUT_COUNT);
	if (!invokeoutputcount) {
		AIFW_LOGE("No invokeoutputcount in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->invokeOutputCount = invokeoutputcount->valueint;

	//	Get postprocessresultcount
	postprocessresultcount = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_POST_PROCESS_RESULT_COUNT);
	if (!postprocessresultcount) {
		AIFW_LOGE("No postProcessResultCount in the manifest!");
	} else {
		modelAttribute->postProcessResultCount = postprocessresultcount->valueint;
	}

	//	Get inferenceresultcount
	inferenceresultcount = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_INFERENCE_RESULT_COUNT);
	if (!inferenceresultcount) {
		AIFW_LOGE("No inferenceResultCount in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->inferenceResultCount = inferenceresultcount->valueint;

	//	Get CRC
	crc = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_CRC32);
	if (!crc) {
		AIFW_LOGE("No CRC in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->crc32 = crc->valueint;

	// Get preprocess values
	preprocess = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_PREPROCESSING);
	if (!preprocess) {
		AIFW_LOGE("No mean and std Vals array in the manifest!");
	} else {
		//	Get MeanVals
		meanVals = cJSON_GetObjectItem(preprocess, MANIFEST_KEY_MEAN_VALS);
		if (!meanVals) {
			AIFW_LOGE("No meanVals array in the manifest!");
		} else {
			int meanValsCount = cJSON_GetArraySize(meanVals);
			if (meanValsCount > 0) {
				modelAttribute->meanVals = new float[meanValsCount];
				if (!modelAttribute->meanVals) {
					AIFW_LOGE("Failed to allocate memory for mean values");
					ret = AIFW_NO_MEM;
					goto cleanup;
				}
				for (int i = 0; i < meanValsCount; i++) {
					cJSON *tempmeanval = cJSON_GetArrayItem(meanVals, i);
					modelAttribute->meanVals[i] = tempmeanval->valuedouble;
				}
			} else {
				modelAttribute->meanVals = NULL;
				AIFW_LOGV("mean values count is 0");
			}
		}

		//	Get StdVals
		cJSON *stdVals = cJSON_GetObjectItem(preprocess, MANIFEST_KEY_STD_VALS);
		if (!stdVals) {
			AIFW_LOGE("No stdVals array in the manifest!");
		} else {
			int stdValsCount = cJSON_GetArraySize(stdVals);
			if (stdValsCount > 0) {
				modelAttribute->stdVals = new float[stdValsCount];
				if (!modelAttribute->stdVals) {
					AIFW_LOGE("Failed to allocate memory for std values");
					ret = AIFW_NO_MEM;
					goto cleanup;
				}
				for (int i = 0; i < stdValsCount; i++) {
					cJSON *tempstdval = cJSON_GetArrayItem(stdVals, i);
					modelAttribute->stdVals[i] = tempstdval->valuedouble;
				}
			} else {
				modelAttribute->stdVals = NULL;
				AIFW_LOGV("std values count is 0");
			}
		}
	}

	// get inference interval
	inferenceinterval = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_INFERENCE_INTERVAL);
	if (!inferenceinterval) {
		AIFW_LOGE("No interval in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->inferenceInterval = inferenceinterval->valueint;

	// get modelCode
	modelcode = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_MODEL_CODE);
	if (!modelcode) {
		AIFW_LOGE("No modelcode in the manifest!");
		ret = AIFW_INVALID_ATTRIBUTE;
		goto cleanup;
	}
	modelAttribute->modelCode = modelcode->valueint;

	return ret;

/* TODO Let's consider removing duplicated code here & AIModel */
cleanup : {
	AIFW_LOGV("Cleaning up the memory");
	if (modelAttribute->version) {
		delete[] modelAttribute->version;
		modelAttribute->version = NULL;
	}
	if (modelAttribute->features) {
		delete[] modelAttribute->features;
		modelAttribute->features = NULL;
	}
	if (modelAttribute->meanVals) {
		delete[] modelAttribute->meanVals;
		modelAttribute->meanVals = NULL;
	}
	if (modelAttribute->stdVals) {
		delete[] modelAttribute->stdVals;
		modelAttribute->stdVals = NULL;
	}
	return ret;
}
}

const char *AIManifestParser::getModelFileName(void)
{
	cJSON *modelfile = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_MODEL_PATH);
	if (!modelfile) {
		AIFW_LOGE("No Model file path in the manifest!");
		return NULL_STRING;
	} else {
		return modelfile->valuestring;
	}
}

int AIManifestParser::getCRC(void)
{
	int crc = -1;
	cJSON *ptr = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_CRC32);
	if (!ptr) {
		AIFW_LOGE("No CRC in the manifest!");
	} else {
		crc = ptr->valueint;
	}
	return crc;
}

const char *AIManifestParser::getVersion(void)
{
	cJSON *ptr = cJSON_GetObjectItem(this->mJSON.get(), MANIFEST_KEY_VERSION);
	if (!ptr) {
		AIFW_LOGE("No Num version in the manifest!");
		return NULL_STRING;
	}
	return ptr->valuestring;
}

} /* namespace aifw */

