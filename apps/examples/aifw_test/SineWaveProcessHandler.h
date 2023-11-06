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
#include "aifw/AIProcessHandler.h"

class SineWaveProcessHandler : public aifw::AIProcessHandler
{
public:
	SineWaveProcessHandler();
	~SineWaveProcessHandler();

	AIFW_RESULT parseData(void *data, uint16_t count, float *parsedData, AIModelAttribute *modelAttribute);
	AIFW_RESULT preProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *invokeInput, AIModelAttribute *modelAttribute);
	AIFW_RESULT postProcessData(std::shared_ptr<aifw::AIDataBuffer> buffer, float *resultData, AIModelAttribute *modelAttribute);

private:
	float *mRawData;
	float *mInvokeOutput;
};

