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

#ifndef _DIRECT_PAIRING_H_
#define _DIRECT_PAIRING_H_

#include <stdbool.h>
#include "ocstack.h"
#include "pmtypes.h"
#include "securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Discover direct-pairing devices in the same IP subnet. .
 *
 * @param[in] waittime  Timeout in seconds.
 *
 * @return OC_STACK_OK on success otherwise error.
 */
OCStackResult DPDeviceDiscovery(unsigned short waittime);

/**
 * Start direct-pairing processes.
 *
 * @param[in] ctx  user context passed back with resultCallback.
 * @param[in] peer  target device to establish direct-pairing.
 * @param[in] pmSel  selected pairing method.
 * @param[in] pinNumber  secret value for dtls connection.
 * @param[in] resultCallback  result event callback.
 *
 * @return OC_STACK_OK on success otherwise error.
 */
OCStackResult DPDirectPairing(void *ctx, OCDirectPairingDev_t* peer, OicSecPrm_t pmSel,
                                char *pinNumber, OCDirectPairingResultCB resultCallback);

/**
 * This function returns discovered devices list in direct-pairing discovery
 * Caller must NOT free returned constant pointer
 *
 * @return OCDirectPairingDev_t pointer on success otherwise NULL.
 */
const OCDirectPairingDev_t* DPGetDiscoveredDevices();

/**
 * This function returns paired devices list until now
 * Caller must NOT free returned constant pointer
 *
 * @return OCDirectPairingDev_t pointer on success otherwise NULL.
 */
const OCDirectPairingDev_t* DPGetPairedDevices();

/**
 * This function delete both discovered devices and paired devices list
 *
 */
void DPDeleteLists();

#ifdef __cplusplus
}
#endif
#endif //_DIRECT_PAIRING_H_

