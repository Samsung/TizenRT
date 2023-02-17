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
#include <stdint.h>
#include <memory>
#include "lw_aifw/lw_aifw_result.h"

namespace lw_aifw {

	typedef void (*aiDataCollectedListener)(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args);

	/**
	 * This is an abstract class. It defines APIs which are required to be implemented by any data source implementation.
	 */

	class AIDataSource {
	protected:
		aiDataCollectedListener mAIDataCollectedListener;
		/* ToDo: plan to use std::function & remove mAIDataCollectedCBArgs */
		void *mAIDataCollectedCBArgs;
	public:
		/**
		 * @brief Default constructor
		 */
		AIDataSource();

		/**
		 * @brief Default destructor
		 */
		virtual ~AIDataSource();

		/**
		 * @brief Data source supports asynchronous data collection. 
		 * This function set callback function to be called when the data is ready.
		 * @param [IN]: listener: Reference to listener of type aiDataCollectedListener
		 * @param [IN]: calbackFuncArgs: Listener can pass an argument to be passed back when callback function is called.
		 */
		virtual void setAIDataCollectedListener(aiDataCollectedListener listener, void *calbackFuncArgs);

		/**
		 * @brief Shares data from application to data source.
		 */
		virtual LW_AIFW_RESULT shareData(void* data) = 0;

		/**
		 * @brief Gets the data from data source, and calls registered callback function with data.
		 * This is asynchronous function call returns LW_AIFW_OK when request for data is registered by data source.
		 * Actual result information is passed as an argument to callback function.
		 */
		virtual LW_AIFW_RESULT getDataAsync(void) = 0;
	};

} /* lw_aifw */

