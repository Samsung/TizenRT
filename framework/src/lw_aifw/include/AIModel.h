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

#ifndef __LWAIFW_AIMODEL_H__
#define __LWAIFW_AIMODEL_H__

#include <memory>
#include "lw_aifw/lw_aifw_result.h"

// Tensorflow structure declaration
struct TfLiteTensor;
namespace tflite {
struct Model;
class MicroInterpreter;
class ErrorReporter;
} // namespace tflite

namespace lw_aifw {
	class AIModel {
	public:
		AIModel();
		AIModel(size_t memorySize);
		~AIModel();
		LW_AIFW_RESULT loadModel(const char *name);
		LW_AIFW_RESULT loadModel(unsigned char *arg);
		void *invoke(void *inputData);
		/**
		 * @brief Get the result array size of the AI model
		 * @return result/output array size of the AI model
		 */
		uint8_t getOutArraySize();
		/**
		 * @brief Get the result array of the AI model
		 * @return result/output array of the AI model
		 */
		bool getOutputArr(uint8_t size, uint16_t *arr);
		/**
		 * @brief Get the Sample/Input array size of the AI model
		 * @return Sample/input array size of the AI model
		 */
		uint8_t getInArraySize();
		/**
		 * @brief Get the input array of the AI model
		 * @return Sample/input array of the AI model
		 */
		bool getInputArr(uint8_t size, uint16_t *arr);
		void getInOutDimensions(int *in, int *out);
	private:
		size_t m_TensorArenaSize;
		std::shared_ptr<uint8_t> m_TensorArena;
		const tflite::Model *m_Model;
		char * mBuf;
		std::shared_ptr<tflite::MicroInterpreter> m_Interpreter;
		std::shared_ptr<tflite::ErrorReporter> m_ErrorReporter;
		TfLiteTensor *m_Input;
		TfLiteTensor *m_Output;
		int m_InputField;
		int m_OutputField;
		LW_AIFW_RESULT _loadModel(void);
	};
} //lw_aifw

#endif // __LWAIFW_AIMODEL_H__

