/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
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

/**
 * @file
 * This file contains the APIs for BT LE communications.
 */
#ifndef CA_LE_UTILS_H_
#define CA_LE_UTILS_H_

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#include "cacommon.h"
#include "cathreadpool.h"
#include "cagattservice.h"
#include "uarraylist.h"

#ifdef __cplusplus
extern "C"
{
#endif

static const char OIC_GATT_SERVICE_UUID[] = CA_GATT_SERVICE_UUID;
static const char OIC_GATT_CHARACTERISTIC_REQUEST_UUID[] = CA_GATT_REQUEST_CHRC_UUID;
static const char OIC_GATT_CHARACTERISTIC_RESPONSE_UUID[] = CA_GATT_RESPONSE_CHRC_UUID;
static const char OIC_GATT_CHARACTERISTIC_CONFIG_UUID[] = "00002902-0000-1000-8000-00805f9b34fb";

static const char CUSTOM_UUID[]   = "75004209-0000-0000-0000-000000000000";
static const char CUSTOM_UUID2[]  = "75004204-0000-0000-0000-000000000000";

static const int GATT_PROFILE = 7;
static const int GATT_SUCCESS = 0;

static const int BOND_BONDED = 12;
static const int BOND_BONDING = 11;
static const int BOND_NONE = 10;

static const int STATE_CONNECTED = 2;
static const int STATE_DISCONNECTED = 0;

typedef enum{
    CALE_STATE_OFF = 0,
    CALE_STATE_ON = 1,
    CALE_STATE_TURNING_OFF = 2
} CALE_STATE_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CA_LE_UTILS_H_ */
