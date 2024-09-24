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

/**
 * @file onert-micro.h
 * @brief AIEngine implementation for onert-micro
 */

#ifndef __ONERT_MICRO_H__
#define __ONERT_MICRO_H__

#include "tinyara/config.h"
#include <memory>
#include "aifw/aifw.h"
#include "AIEngine.h"

// forward declaration
/**
 * @namespace luci_interpreter
 * @brief OneRTMicro namespace
 */
namespace luci_interpreter
{
	class Interpreter;
}
namespace aifw {

/**
 * @class ONERTM
 * @brief Class to perform AI operations using ONERT for Microcontroller
 */
class ONERTM : public AIEngine
{
public:
	ONERTM();
	~ONERTM();
	AIFW_RESULT loadModel(const char *file);
	AIFW_RESULT loadModel(const unsigned char *model);
#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	void *invoke(void *inputData);
#else
	AIFW_RESULT invoke(void *inputData, void *outputData);
	void getModelDimensions(uint16_t *inputSetCount, uint16_t **inputSizeList, uint16_t *outputSetCount, uint16_t **outputSizeList);
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
	AIFW_RESULT resetInferenceState(void);

private:
	AIFW_RESULT _loadModel(void);

	char *mBuf;
	std::shared_ptr<luci_interpreter::Interpreter> mInterpreter;
#ifndef CONFIG_AIFW_MULTI_INOUT_SUPPORT
	uint16_t mModelInputSize;
	uint16_t mModelOutputSize;
#else
	uint16_t *mInputSizeList;
	uint16_t *mOutputSizeList;
	uint16_t mInputSetCount;
	uint16_t mOutputSetCount;
#endif /* CONFIG_AIFW_MULTI_INOUT_SUPPORT */
};

} /* namespace aifw */

#endif /* __ONERT_MICRO_H__ */
