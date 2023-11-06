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

#include <math.h>
#include "aifw/aifw_utils.h"
#include "aifw/aifw_log.h"
#include "aifw/aifw.h"

void normalizeData(float dataValues[], float meanVals[], float stdValues[], uint16_t countOfValues)
{
	for (uint16_t i = 0; i < countOfValues; i++) {
		dataValues[i] = dataValues[i] - meanVals[i];
		dataValues[i] = dataValues[i] / (stdValues[i] + 0.000001);
	}
}

AIFW_RESULT getMSE(float *realValues, float *predValues, int count, float *result)
{
	if (realValues == NULL) {
		AIFW_LOGE("[Error] The parameter realValues is NULL \n");
		return AIFW_ERROR;
	}

	if (predValues == NULL) {
		AIFW_LOGE("[Error] The parameter predValues is NULL \n");
		return AIFW_ERROR;
	}

	if (count <= 0) {
		AIFW_LOGE("[Error] count is %d, the count value must be greater than 0 \n", count);
		return AIFW_ERROR;
	}

	float mse = 0.0;
	for (int i = 0; i < count; i++) {
		mse += pow(realValues[i] - predValues[i], 2);
	}
	*result = (mse / count);
	return AIFW_OK;
}

AIFW_RESULT getRMSE(float *realValues, float *predValues, int count, float *result)
{
	float mseResult;
	if (getMSE(realValues, predValues, count, &mseResult) == AIFW_ERROR) {
		return AIFW_ERROR;
	}
	*result = sqrt(mseResult);
	return AIFW_OK;
}

