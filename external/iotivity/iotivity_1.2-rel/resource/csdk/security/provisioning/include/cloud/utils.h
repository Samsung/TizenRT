/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/
#ifndef CLOUD_UTILS_H
#define CLOUD_UTILS_H

#include "occloudprovisioning.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern char *DEFAULT_QUERY;
extern char *DEFAULT_PREFIX;

typedef OCStackResult (*UserFunctionCB)(void *params, void **data, OCClientResponse *clientResponse);

/**
 * This function sets required CoAP prefix depending on
 * secure or non-secure connection
 *
 * @param[in] secure   secure flag
 */
void setCoapPrefix(bool secure);

/**
 * This hepler function fills OCCallbackData
 *
 * @param[in] cbData       cloud host and port
 * @param[in] ctx          external context
 * @param[in] cb           external callback
 * @param[in] fn           internal function
 * @param[in] params       params to internal function
 */
void fillCallbackData(OCCallbackData *cbData, void *ctx, OCCloudResponseCB cb,
                                              UserFunctionCB fn, void *params);

/**
 * Get the device id from db
 *
 * @return  device id
 */
char *getDeviceId();

/**
 * Clear previously filled array of strings
 *
 * @param[out] list           array of strings structure to clear
 */
void clearStringArray(stringArray_t *list);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CLOUD_UTILS_H
