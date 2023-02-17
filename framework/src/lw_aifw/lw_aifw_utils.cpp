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

#include "lw_aifw/lw_aifw_utils.h"
#include "lw_aifw/lw_aifw_log.h"

void normalizeData(float dataValues[], double meanVals[], double stdValues[], uint16_t countOfValues)
{
	for (uint16_t i = 0; i < countOfValues; i++) {
		dataValues[i] = dataValues[i] - meanVals[i];
		dataValues[i] = dataValues[i] / (stdValues[i] + 0.000001);
	}
}

LW_AIFW_RESULT getMSE(float *realValues, float *predValues, int count, float *result){
	if (realValues == NULL) {
		LW_AIFW_LOGE("[Error] The parameter realValues is NULL \n");
		return LW_AIFW_ERROR;
	}

	if (predValues == NULL) {
		LW_AIFW_LOGE("[Error] The parameter predValues is NULL \n");
		return LW_AIFW_ERROR;
	}

	if (count <= 0) {
		LW_AIFW_LOGE("[Error] count is %d, the count value must be greater than 0 \n", count);
		return LW_AIFW_ERROR;
	}

	float mse = 0.0;
	for (int i = 0; i < count; i++){
		mse += pow(realValues[i] - predValues[i], 2);
	}

	*result = (mse / count);
	return LW_AIFW_OK;
}

LW_AIFW_RESULT getRMSE(float *realValues, float *predValues, int count, float *result){
	float mseResult;
	if (getMSE(realValues, predValues, count, &mseResult) == LW_AIFW_ERROR) {
		return LW_AIFW_ERROR;
	}
	*result = sqrt(mseResult);

	return LW_AIFW_OK;
}
