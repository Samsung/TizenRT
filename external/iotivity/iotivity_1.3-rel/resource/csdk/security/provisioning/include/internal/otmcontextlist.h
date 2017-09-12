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

#ifndef _OTM_CONTEXT_LIST_H_
#define _OTM_CONTEXT_LIST_H_

#include "pmtypes.h"
#include "ocstack.h"
#include "octypes.h"
#include "experimental/securevirtualresourcetypes.h"
#include "ownershiptransfermanager.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct OTMContextItem {
    OTMContext_t* otmCtx;
    OCDevAddr endpoint;
    struct OTMContextItem* next;
}OTMContextItem_t;

/**
 * API to remove OTMContext from OTMContext list.
 *
 * @param[in] addr IP address of peer
 * @param[in] port secure port number of peer
 */
void RemoveOTMContext(const char* addr, uint16_t port);

/**
 * API to add OTMContext to OTMContext list.
 * If same OTMContext exist, will be ignored.
 *
 * @param[in] ctx new OTMContext
 * @param[in] addr IP address of peer
 * @param[in] port secure port number of peer
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult AddOTMContext(OTMContext_t* ctx, const char* addr, uint16_t port);

/**
 * API to read OTMContext from OTMContext list
 *
 * @param[in] addr IP address of peer
 * @param[in] port secure port number of peer
 *
 * @return OTMContext in case of context found, otherwise NULL.
 */
OTMContext_t* GetOTMContext(const char* addr, uint16_t port);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_OTM_CONTEXT_LIST_H_