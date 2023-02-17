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
#include "lw_aifw/lw_aifw_result.h"

namespace lw_aifw {
	typedef void (*rawDataCollectedListener)(LW_AIFW_RESULT result, float *data, uint16_t dataCount, void *args);
	
	class AICSVReader {
	protected:
		rawDataCollectedListener mRawDataCollectedListener;
		/* ToDo: plan to use std::function & remove mRawDataCollectedCBArgs */
		void *mRawDataCollectedCBArgs;
	public:
		AICSVReader();
		virtual ~AICSVReader();
		virtual void setRawDataCollectedListener(rawDataCollectedListener listener, void *calbackFuncArgs);
		virtual LW_AIFW_RESULT init(void) = 0;
		virtual LW_AIFW_RESULT getSourceData(void) = 0;
	};

} /* lw_aifw */
