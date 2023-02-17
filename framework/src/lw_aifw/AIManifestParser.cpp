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

#include "lw_aifw/lw_aifw_log.h"

#include "AIManifestParser.h"

#define MANIFEST_KEY_VERSION         "version"
#define MANIFEST_KEY_INFERENCE_LOGIC "inference_logic"
#define MANIFEST_KEY_MODELS          "models"
#define MANIFEST_KEY_PREFERENCES     "preferences"

#define NULL_STRING "(null)"

namespace lw_aifw {

	LW_AIFW_RESULT AIManifestParser::loadFromFile(const char *path)
	{
		FILE *f = fopen(path, "rb");
		if (!f) {
			LW_AIFW_LOGE("error: Cannot open file");
			return LW_AIFW_ERROR;
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

		this->m_json = json;
		if (!this->m_json.get()) {
			LW_AIFW_LOGE("error: Cannot parse manifest.json!");
			delete[] buffer;
			return LW_AIFW_ERROR;
		}
		delete[] buffer;
		return LW_AIFW_OK;
	}

	char *AIManifestParser::getVersion()
	{
		if (!this->m_json.get()) {
			return NULL_STRING;
		}

		cJSON *version = cJSON_GetObjectItem(this->m_json.get(), MANIFEST_KEY_VERSION);
		if (!version) {
			LW_AIFW_LOGE("error: There is no AI version in the manifest!");
			return NULL_STRING;
		}
		return version->valuestring;
	}

	char *AIManifestParser::getInferenceLogicPath()
	{
		if (!this->m_json.get()) {
			return NULL_STRING;
		}

		cJSON *inferenceLogic = cJSON_GetObjectItem(this->m_json.get(), MANIFEST_KEY_INFERENCE_LOGIC);
		if (!inferenceLogic) {
			LW_AIFW_LOGE("error: There is no inference logic in the manifest!");
			return NULL_STRING;
		}
		return inferenceLogic->valuestring;
	}

	int AIManifestParser::getModelCount()
	{
		if (!m_json.get()) {
			return 0;
		}

		cJSON *models = cJSON_GetObjectItem(this->m_json.get(), MANIFEST_KEY_MODELS);
		int count = cJSON_GetArraySize(models);
		LW_AIFW_LOGV("model count %d", count);
		return count;
	}

	char *AIManifestParser::getModelPath(int index)
	{
		if (!this->m_json.get()) {
			return NULL_STRING;
		}

		cJSON *models = cJSON_GetObjectItem(this->m_json.get(), MANIFEST_KEY_MODELS);

		if (index < 0 || index > cJSON_GetArraySize(models) - 1) {
			LW_AIFW_LOGE("error: Index out of bound!");
			return NULL_STRING;
		}
		char *model = cJSON_GetArrayItem(models, index)->valuestring;
		LW_AIFW_LOGV("model index %d val: %s", index, model);
		return model;
	}

	cJSON *AIManifestParser::getPrefJsonObject(const char *key)
	{
		if (!this->m_json.get()) {
			return nullptr;
		}

		cJSON *preferences = cJSON_GetObjectItem(this->m_json.get(), MANIFEST_KEY_PREFERENCES);

		if (!preferences) {
			LW_AIFW_LOGE("error: There is no preferences data in the manifest!");
			return nullptr;
		}

		cJSON *obj = cJSON_GetObjectItem(preferences, key);
		if (!obj) {
			LW_AIFW_LOGE("error: Cannot get the preference: %s!", key);
			return nullptr;
		}

		return obj;
	}

	char *AIManifestParser::getPreferenceAsString(const char *key)
	{
		cJSON *result = this->getPrefJsonObject(key);

		if (result) {
			return result->valuestring;
		}

		return NULL_STRING;
	}

	LW_AIFW_RESULT AIManifestParser::getPreferenceAsFloat64Array(const char *key, double *values, int &size)
	{
		if (!values) {
			LW_AIFW_LOGE("Output param is null");
			return LW_AIFW_ERROR;
		}

		cJSON *result = this->getPrefJsonObject(key);
		if(!result) {
			LW_AIFW_LOGE("Key not found");
			return LW_AIFW_ERROR;
		}

		int arrSize = cJSON_GetArraySize(result);
		if (size < arrSize) {
			LW_AIFW_LOGE("Output param size is small");
			return LW_AIFW_ERROR;
		}

		size = arrSize;
		for(int i = 0; i < arrSize; i++) {
			values[i] = cJSON_GetArrayItem(result, i)->valuedouble;
			LW_AIFW_LOGV("key: %s, index %d val: %lf", key, i, values[i]);
		}
		return LW_AIFW_OK;
	}

	int AIManifestParser::getArraySize(const char *key)
	{
		cJSON *result = this->getPrefJsonObject(key);
		if(!result) {
			LW_AIFW_LOGE("Key not found");
			return LW_AIFW_ERROR;
		}
		return(cJSON_GetArraySize(result));
	}

} //lw_aifw

