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
#include <stdio.h>
#include "lw_aifw/lw_aifw_result.h"
#include "AICSVReader.h"

namespace lw_aifw {

	class AICSVArrayReader : public AICSVReader {
	private:
		char *m_Header;
		uint16_t m_SensorLength;
		float *m_DataValues;
		int m_SensorCount;
		uint16_t m_RowCount;
		uint16_t m_RowOffset;
		LW_AIFW_RESULT read(float *&values, uint16_t &numberofValues);
	public:
		AICSVArrayReader(float *datavalues, uint16_t sensorCount, uint16_t rowCount);
		~AICSVArrayReader();
		LW_AIFW_RESULT init(void);
		LW_AIFW_RESULT getSourceData(void);
		uint16_t getSensorCount(void) const;
	};

} /* lw_aifw */

