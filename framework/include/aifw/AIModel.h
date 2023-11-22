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

#pragma once

#include <memory>
#include "aifw/aifw.h"

namespace aifw {

class AIEngine;
class AIDataBuffer;
class AIProcessHandler;

/**
 * @class AIModel
 * @brief Class for performing operations on an AI Model.
 */
class AIModel
{
public:
	/**
	 * @brief Constructs the AIModel class instance.
	 * It initializes data members of the class and constructs AI Engine object.
	 */
	AIModel(void);

	/**
	 * @brief (Parameterized) Constructs the AIModel class when data processor is provided.
	 * It initializes data members of the class and constructs AI Engine object.
	 * @param [in] dataProcessor: Pointer of AIProcessHandler to carry out raw data parsing, pre-/post-invoke processing of data.
	 */
	AIModel(std::shared_ptr<AIProcessHandler> dataProcessor);

	/**
	 * @brief AIModel class destructor.
	 * It frees memory allocated to mModelAttribute's member variables, and buffers which were required to store data at different stages of inference. 
	 */
	~AIModel();

	/**
	 * @brief It loads manifest information from file specified by scriptPath and fills it into mModelAttribute's member variables.
	 * It allocates necessary buffers, loads TFLITE model from file, and initializes AI data buffer using information parsed from manifest file.
	 * @param [in] scriptPath: Manifest file path.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT loadModel(const char *scriptPath);

	/**
	 * @brief It fills model attributes information into mModelAttribute's member variables from input param modelAttribute.
	 * It then allocates necessary buffers, loads array model, and initializes AI data buffer using model attributes information.
	 * @param [in] modelAttribute: AIModelAttribute structure variable containing model attributes.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT loadModel(const AIModelAttribute &modelAttribute);

	/**
	 * @brief Pushes incoming raw data for parsing, pre-processing, invoke, and post-processing.
	 * @param [in] data: Incoming sensor data to be passed for inference.
	 * @param [in] count: Length of incoming sensor data array.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT pushData(void *data, uint16_t count);

	/**
	 * @brief Gives AIModelAttribute structure variable containing model attributes.
	 * @return: Returns mModelAttribute.
	 */
	AIModelAttribute getModelAttribute(void);

	/**
	 * @brief Fills latest post processed data(or invoke output data in case of no data processor) into buffer pointed by data.
	 * @param [out] data: Pointer to buffer which will hold post processed(or invoke output) data.
	 * @param [in] count: Number of values that buffer can hold.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT getResultData(float *data, uint16_t count);

	/**
	 * @brief Reads latest raw data from AIDataBuffer and fills into buffer pointed by data.
	 * @param [out] data: Pointer to buffer which will hold raw data.
	 * @param [in] count: Number of values that buffer can hold.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT getRawData(float *data, uint16_t count);

	/**
	 * @brief Clears sensor data stored stored in model's buffer
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clearRawData(void);

	/**
	 * @brief Clears sensor data stored stored in model's buffer
	 * @param [IN] offset: Offset of row to start deletion.
	 * @param [IN] count: Count of rows to delete.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT clearRawData(uint16_t offset, uint16_t count);
private:
	/**
	 * @brief It constructs AIDataBuffer object and initializes it.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT createDataBuffer(void);

	/**
	 * @brief It carries out invoke and pre-/post-invoke operations on data.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT invoke(void);

	/**
	 * @brief It loads manifest information from file specified by scriptPath and fills it into mModelAttribute's member variables.
	 * @param [in] path: Manifest file path.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT setModelAttributes(const char *path);

	/**
	 * @brief It frees memory allocated to mModelAttribute's member variables.
	 */
	void clearModelAttribute(void);

	/**
	 * @brief It allocates buffers required to store data at different stages of inference.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT allocateMemory(void);

	/**
	 * @brief It fills model attributes information into mModelAttribute's member variables from input param modelAttribute.
	 * @param [in] modelAttribute: AIModelAttribute structure variable containing model attributes.
	 * @return: AIFW_RESULT enum object.
	 */	
	AIFW_RESULT fillModelAttribute(const AIModelAttribute &modelAttribute);

	AIModelAttribute mModelAttribute;
	std::shared_ptr<AIDataBuffer> mBuffer;
	std::shared_ptr<AIEngine> mAIEngine;
	float *mInvokeInput;
	float *mInvokeOutput;
	float *mParsedData;
	float *mPostProcessedData;
	std::shared_ptr<AIProcessHandler> mDataProcessor;
};

} /* namespace aifw */

