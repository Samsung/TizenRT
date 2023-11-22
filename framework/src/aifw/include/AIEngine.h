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

	/**
	 * @brief Run the inference with the given inputData.
	 * @param [in] inputData: Input data for model invoke.
	 * @return: Void * pointer to output result data.
	 */
	virtual void *invoke(void *inputData) = 0;
};

} /* namespace aifw */

