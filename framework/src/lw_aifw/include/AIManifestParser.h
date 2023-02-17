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
 * @file AIManifestParser.h
 * @brief Manifest Information Class to read application specific data
 */

#ifdef CONFIG_LW_AIFW_READ_FROM_MANIFEST_FILE

#ifndef __AIMANIFEST_PARSER_H__
#define __AIMANIFEST_PARSER_H__

#include <json/cJSON.h>
#include "lw_aifw/lw_aifw_result.h"

namespace lw_aifw {

class AIManifestParser
{
public:
	/**
	 * @brief Load the manifest information from a file
	 * @details @b #include <AIManifestParser.h>
	 * @param[in] path Manifest file path
	 * @return Result of the loadFromFile operation
	 */
	LW_AIFW_RESULT loadFromFile(const char *path);

	/**
	 * @brief Get the version of the AI
	 * @details @b #include <AIManifestParser.h>
	 * @return Version as std::string type
	 */
	char *getVersion();

	/**
	 * @brief Get the path of the inference logic of the AI
	 * @details @b #include <AIManifestParser.h>
	 * @return Inference logic path
	 */
	char *getInferenceLogicPath();

	/**
	 * @brief Get the model count that the AI contains
	 * @details @b #include <AIManifestParser.h>
	 * @return Model count
	 */
	int getModelCount();

	/**
	 * @brief Get the path of the model of the given index
	 * @details @b #include <AIManifestParser.h>
	 * @return Model path
	 */
	char *getModelPath(int index);

	/**
	 * @brief Get the preference value array as the array of double values
	 * @details @b #include <AIManifestParser.h>
	 * The value should be convertible to the array of double values
     * @return LW_AIFW_RESULT
     */
	LW_AIFW_RESULT getPreferenceAsFloat64Array(const char *key, double *values, int &size);

	/**
	 * @brief Get the preference value as string type
	 * @details @b #include <AIManifestParser.h>
	 * The value should be convertible to the string value
	 * @return Preference value as string type
	 */
	char *getPreferenceAsString(const char *key);

	int getArraySize(const char *key);
private:
	cJSON *getPrefJsonObject(const char *key);

private:
	std::shared_ptr<cJSON> m_json;
};

} /* lw_aifw */
#endif /* __AIMANIFEST_PARSER_H__ */

#endif /* CONFIG_LW_AIFW_READ_FROM_MANIFEST_FILE */

