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
#pragma once

#include "aifw/aifw.h"
#include "aifw/AIModel.h"
#include "aifw/AIInferenceHandler.h"

class EPDInferenceHandler : public aifw::AIInferenceHandler
{
public:
    /**
	 * @brief EPDInferenceHandler constructor.
	 * @param [IN] listener: Callback for inference result.
	*/
	EPDInferenceHandler(InferenceResultListener listener);

    /**
	 * @brief EPDInferenceHandler destructor.
	*/
	~EPDInferenceHandler();

    /**
	 *! @copydoc AIInferenceHandler::onInferenceFinished()
	*/
	AIFW_RESULT onInferenceFinished(uint16_t idx, void *finalResult);

    /**
	 *! @copydoc AIInferenceHandler::prepare()
	*/
	AIFW_RESULT prepare(void);

	/**
	 *! @copydoc AIInferenceHandler::clearData()
	*/
	AIFW_RESULT clearData(void) override;

	AIModelAttribute getModelAttribute(void);

private:
    /**
	 * @brief mEPDModel is a list of objects. Each object maps to a model in model set defined in EPDInferenceHandler.cpp
	*/
	std::shared_ptr<aifw::AIModel> mEPDModel;
	std::shared_ptr<aifw::AIProcessHandler> mEPDProcessHandler;
};
