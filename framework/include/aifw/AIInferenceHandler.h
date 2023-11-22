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
 * @file AIInferenceHandler.h
 */

#pragma once

#include <stdlib.h>
#include "aifw/aifw.h"
#include "aifw/AIModel.h"

namespace aifw {

/**
 * @class AIInferenceHandler
 * @brief Inference handler base class
 */
class AIInferenceHandler
{
public:
	/**
	 * @brief Constructs the AIInferenceHandler class instance.
	 * It initializes data members of the class and allocates required memory to them for other APIs of AIInferenceHandler to work properly.
	 * @param [in] countOfModels: Number of models attached with this AIInferenceHandler instance.
	 * @param [in] listener: Application will receive final inference result in this callback function. This function will be called when inference cycle of model set completes.
	 */
	AIInferenceHandler(uint16_t countOfModels, InferenceResultListener listener);

	/**
	 * @brief AIInferenceHandler destructor
	 */
	virtual ~AIInferenceHandler()
	{
	}

	/**
	 * @brief Pushes incoming raw data to all attached models for pre-processing, invoke, post processing and finally ensembling.
	 * @param [in] data: Incoming sensor data to be passed for inference.
	 * @param [in] count: Length of incoming sensor data array.
	 * @return: AIFW_RESULT enum object.
	 */
	AIFW_RESULT pushData(void *data, uint16_t count);

	/**
	 * @brief Gives inference interval of model set.
	 * @return: Negative value indicates an error. Non negative value tells inference interval of attached models.
	 */
	uint16_t getModelServiceInterval(void);

	/**
	 * @brief It creates process handler object specific to model and pass them as an argument while creating AIModel object.
	 * Process handler does not need to be created if model does not require parsing, pre/post processing of data.
	 * Finally, model is loaded and attached to AIInferenceHandler.
	 * It is resposibility of model developer to perform this step for each model in model set in sequence of model execution. Inference operation on models will also be done be in same sequence.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT prepare(void) = 0;

	/**
	 * @brief Clears sensor data associated with all models in model set.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT clearData(void);

	/**
	 * @brief Clears specific sensor data rows associated with all models in model set.
	 * @param [IN] offset: Offset of row to start deletion.
	 * @param [IN] count: Count of rows to delete.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT clearData(uint16_t offset, uint16_t count);	

protected:
	/**
	 * @brief Performs operations on post processed(or invoke output) results of attached models in the model set.
	 * This function will be called always when inference cycle of a model set is finished successfully i.e. return value is AIFW_OK or AIFW_INFERENCE_FINISHED.
	 * @param [in] idx: Index of last model till which inference is performed.
	 * @param [out] finalResult: Final inference result of modelSet will be stored in buffer pointed by this param.
	 * @return: AIFW_RESULT enum object.
	 */
	virtual AIFW_RESULT onInferenceFinished(uint16_t idx, void *finalResult) = 0;

	/**
	 * @brief Attaches pointer to an AI Model object to AI Model pointer list of AIInferenceHandler. Each AI Model pointer points to a model attached to AIInferenceHandler.
	 * @param [in] model: Pointer to AI Model object to be attached.
	 * @return: AIFW_RESULT enum object.
	 */
	void attachModel(std::shared_ptr<AIModel> model);

private:
	uint16_t mMaxModelsCount;
	uint16_t mModelIndex;
	std::shared_ptr<std::shared_ptr<AIModel>> mModels;
	InferenceResultListener mInferenceResultListener;
};

} /* namespace aifw */

