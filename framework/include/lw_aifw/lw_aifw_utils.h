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
 * @ingroup lw_aifw
 * @{
 */

/**
 * @file lw_aifw/lw_aifw_utils.h
 * @brief Light Weight AI Framework utilities API
 */

#ifndef __LW_AIFW_UTILS_H__
#define __LW_AIFW_UTILS_H__

#include <stdint.h>
#include <math.h>
#include "lw_aifw/lw_aifw_result.h"

/**
 * @brief: Utility function to normaiza the data
 * @param [IN]: dataValues: Data to be normalized
 * @param [IN]: meanVals: Mean values to use in normalization
 * @param [IN]: stdValues: Standard deviation values to use in normalization
 * @param [IN]: countOfValues: number of values, it is assumed parameter 1, 2, 3 all have same number of values.
 */

void normalizeData(float dataValues[], double meanVals[], double stdValues[], uint16_t countOfValues);

/**
 * @brief: Utility function to get mse value from the predicted value
 * @param [IN]: realValues : Actual values
 * @param [IN]: predValues : Predicted values
 * @param [IN]: count : number of values
 * @param [IN]: result : Variable to be returned 
 */
LW_AIFW_RESULT getMSE(float *realValues, float *predValues, int count, float *result);

/**
 * @brief: Utility function to get rmse value from the predicted value
 * @param [IN]: realValues : Actual values
 * @param [IN]: predValues : Predicted values
 * @param [IN]: count : number of values
 * @param [IN]: result : Variable to be returned 
 */
LW_AIFW_RESULT getRMSE(float *realValues, float *predValues, int count, float *result);

#endif //__LW_AIFW_UTILS_H__

/** @} */ // end of lw_aifw group

