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
 * @file aifw/aifw_utils.h
 * @brief AI Framework utilities API
 */

#pragma once

#include <stdint.h>
#include "aifw/aifw.h"

/**
 * @brief: Utility function to normalize the data
 * @param [in,out] dataValues: Data to be normalized
 * @param [in] meanVals: Mean values to use in normalization
 * @param [in] stdValues: Standard deviation values to use in normalization
 * @param [in] countOfValues: number of values. Parameter 1~3 all have same count of values.
 */
void normalizeData(float dataValues[], float meanVals[], float stdValues[], uint16_t countOfValues);

/**
 * @brief: Utility function to get mse value from the predicted value
 * @param [in] realValues: Actual values
 * @param [in] predValues: Predicted values
 * @param [in] count: number of values
 * @param [out] result: Variable to be returned 
 */
AIFW_RESULT getMSE(float *realValues, float *predValues, int count, float *result);

/**
 * @brief: Utility function to get rmse value from the predicted value
 * @param [in] realValues: Actual values
 * @param [in] predValues: Predicted values
 * @param [in] count: number of values
 * @param [out] result: Variable to be returned 
 */
AIFW_RESULT getRMSE(float *realValues, float *predValues, int count, float *result);

