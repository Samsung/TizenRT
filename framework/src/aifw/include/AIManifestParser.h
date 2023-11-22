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
 * @brief This class reads data from model manifest file.
 */

#pragma once

#include <memory>
#include <json/cJSON.h>
#include "aifw/aifw.h"

namespace aifw {

/**
 * @class AIManifestParser
 * @brief This class reads data from model manifest file.
 */
class AIManifestParser
{
public:
	/**
	 * @brief AIManifestParser destructor.
	 */
	~AIManifestParser();

	/**
	 * @brief Load the manifest information from a file.
	 * @param [in] path: Manifest file path.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT loadManifestFile(const char *path);

	/**
	 * @brief Fills data into modelAttribute object's member variables from manifest file.
	 * @param [out] modelAttribute: AIModelAttribute class object to save the manifest data extracted from manifest file.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT readData(AIModelAttribute *modelAttribute);

	/**
	 * @brief Get the model file name from manifest file.
	 * @return: File name string.
	 */
	const char *getModelFileName(void);

	/**
	 * @brief Get CRC value.
	 * @return: Returns integer CRC value.
	 */
	int getCRC(void);

	/**
	 * @brief Get version of model.
	 * @return: Version string.
	 */
	const char *getVersion(void);

private:
	std::shared_ptr<cJSON> mJSON;
};

} /* namespace aifw */

