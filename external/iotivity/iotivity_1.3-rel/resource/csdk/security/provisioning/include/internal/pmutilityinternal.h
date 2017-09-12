/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _PM_UTILITY_INTERNAL_H_
#define _PM_UTILITY_INTERNAL_H_

#include "pmtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef MULTIPLE_OWNER
/**
 * Performs a deep copy of a single OicSecMom_t struct.
 *
 * @param[in] src    The source struct to be copied.
 *
 * @return   A pointer to the copy or NULL in case of an error.
 */
OicSecMom_t* CloneOicSecMom(const OicSecMom_t* src);

/**
 * Performs a deep copy of a single OicSecSubOwner_t struct.
 *
 * @param[in] src    The source struct to be copied.
 *
 * @return   A pointer to the copy or NULL in case of an error.
 */
OicSecSubOwner_t* CloneOicSecSubOwner(const OicSecSubOwner_t* src);
#endif //MULTIPLE_OWNER

/**
 * Performs a deep copy of a single OicSecDoxm_t struct.
 *
 * @param[in] src    The source struct to be copied.
 *
 * @return   A pointer to the copy or NULL in case of an error.
 */
OicSecDoxm_t* CloneOicSecDoxm(const OicSecDoxm_t* src);

/**
 * API to perform a deep copy of a list of OCProvisionDev_t. The result must be
 * freed using PMDeleteDeviceList.
 *
 * @param[in]   source  Source list to be cloned. Must not be null.
 * @return  Cloned source or NULL in case of an error.
 */
OCProvisionDev_t* PMCloneOCProvisionDevList(const OCProvisionDev_t* source);

#ifdef __cplusplus
}
#endif
#endif //_PM_UTILITY_INTERNAL_H_
