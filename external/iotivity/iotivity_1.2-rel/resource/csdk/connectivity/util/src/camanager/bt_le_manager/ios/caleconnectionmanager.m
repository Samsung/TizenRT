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

#include "logger.h"
#include "cautilinterface.h"
#include "camanagerleinterface.h"
#include "camanagerleutil.h"
#include "caleautoconnector.h"
#include "cacommon.h"
#include "cacommonutil.h"
#include "camanagerdevice.h"
#include "caleclient.h"
#include "caleutils.h"
#include "uarraylist.h"

#define TAG "OIC_CA_MANAGER_LE"

static const int SUPPORT_ADNROID_API_LEVEL = 18;
static const int AUTH_FAIL = 5;
static const int LINK_LOSS = 8;
static const int ACCEPT_TIMEOUT_EXCEPTION = 16;
static const int REMOTE_DISCONNECT = 19;
static const int LOCAL_DISCONNECT = 22;
static const int CONNECTION_FAILED_TO_BE_EASTABLISHED = 62;
static const int USER_REMOVED_BOND = 68;

static dispatch_queue_t g_caManagerAutoConnectQueue = NULL;
static NSMutableDictionary *g_candidateACList = NULL;

static void CAManagerAdapterStateChangeCB(CBCentralManager *central);
static void CAManagerLeGattConnectionStateChangeCB(CBPeripheral *peripheral,
                                       const char *remote_address, bool connected, NSError *error);
static void CAManagerLeGattServiceDiscoveredCB(CBPeripheral *peripheral, const char *remote_address,
                                               NSError *error);


CAResult_t CASetLEClientAutoConnectionDeviceInfo(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CASetClientAutoConnectionDeviceInfo");
    VERIFY_NON_NULL(address, TAG, "address");

    OIC_LOG_V(DEBUG, TAG, "set [%s] for Auto Connection", address);

    if (!CAManagerCheckBTAddress(address))
    {
        OIC_LOG(ERROR, TAG, "this address is not BT address string format");
        return CA_STATUS_FAILED;
    }

    // if there is target address in SharedPreference. it will be reseted.
    if (CAManagerIsConnectedDeviceAddress(address))
    {
        if (!CAManagerRemoveConnectedDeviceAddress(address))
        {
            OIC_LOG(ERROR, TAG, "Preference - remove has failed");
        }
        else
        {
            OIC_LOG(INFO, TAG, "Preference - remove success");
        }
    }

    // check candidate AC list
    if (g_candidateACList[@(address)] != nil) {
        OIC_LOG_V(DEBUG, TAG, "Candidate device : %s", address);

        if(!CAManagerIsConnectedDeviceAddress(address))
        {
            // if BLE address is matched each other
            // this address will be added into auto connection list.
            OIC_LOG(INFO, TAG, "AC list - address will be added into ACData list");
            CBPeripheral *peripheral = (CBPeripheral *)g_candidateACList[@(address)];

            CAManagerAddACData(address);

            if(!CAManagerSetValueConnectedDeviceAddress(peripheral, address))
            {
                OIC_LOG(ERROR, TAG, "AC list - address is un-registered");
                return CA_STATUS_FAILED;
            }

            [g_candidateACList removeObjectForKey:@(address)];

            CAManagerSetAutoConnectionFlag(address, false);

            // next connection will be requested as true flag
            // after first connection
            CALEClientSetAutoConnectFlag(address, true);
        }
    }

    else
    {
        // it will be added new target address.
        if (!CAManagerAddConnectedDeviceAddress(address))
        {
            OIC_LOG(ERROR, TAG, "Preference - putting has failed");
        }
        else
        {
            OIC_LOG(INFO, TAG, "Preference - putting success");
        }
    }

    return CA_STATUS_OK;
}

CAResult_t CAUnsetLEClientAutoConnectionDeviceInfo(const char* address)
{
    OIC_LOG(DEBUG, TAG, "CAUnsetClientAutoConnectionDeviceInfo");
    VERIFY_NON_NULL(address, TAG, "address");

    OIC_LOG_V(DEBUG, TAG, "unset [%s] for Auto Connection", address);

    if (!CAManagerCheckBTAddress(address))
    {
        OIC_LOG(ERROR, TAG, "this address is not BT address string format");
        return CA_STATUS_FAILED;
    }

    // if there is target address in SharedPreference. it would be removed
    if (CAManagerIsConnectedDeviceAddress(address))
    {
        if (!CAManagerRemoveConnectedDeviceAddress(address))
        {
            OIC_LOG(ERROR, TAG, "Preference - remove has failed");
        }
        else
        {
            OIC_LOG(INFO, TAG, "Preference - remove success");
        }
    }

    // remove target device for auto connection
    CAResult_t ret = CAManagerRemoveData(address);
    if (CA_STATUS_OK != ret)
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveData has failed");
    }

    CALEClientSetAutoConnectFlag(address, false);

    return CA_STATUS_OK;
}

CAResult_t CAManagerLEClientInitialize()
{
    OIC_LOG(DEBUG, TAG, "CAManagerClientInitialize");

    CAManagerInitMutexVaraibles();
    CAManagerInitLEAutoConnection();

    CAManagerCreateACDataList();

    g_candidateACList = [[NSMutableDictionary alloc] init];

    if (!g_caManagerAutoConnectQueue) {
        g_caManagerAutoConnectQueue = dispatch_queue_create("com.caManager.AucoConnect",
                                                            DISPATCH_QUEUE_SERIAL);
    }

    //callback register to caleclient
    CALEClientSetCAManagerCallback(CAManagerAdapterStateChangeCB,
                                   CAManagerLeGattConnectionStateChangeCB,
                                   CAManagerLeGattServiceDiscoveredCB);

    // get last connected device list
    bool ret = false;
    ret = CAManagerGetConnectedDeviceAddress();
    if (!ret)
    {
        // create new set<String> object
        ret = CAManagerCreateSetString();
        if (ret)
        {
            OIC_LOG(DEBUG, TAG, "created new SetString");
        }
        else
        {
            OIC_LOG(ERROR, TAG, "CAManagerCreateSetString has failed");
            return CA_STATUS_FAILED;
        }
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "get previous Set<String> object");
    }

    return CA_STATUS_OK;
}

CAResult_t CAManagerLEClientTerminate()
{
    OIC_LOG(DEBUG, TAG, "CAManagerClientTerminate");

    // stop gatt connection
    CAResult_t res = CALEClientDisconnectAll();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CALEClientDisconnectAll has failed");
    }

    res = CAManagerRemoveAllData();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAManagerRemoveAllData has failed");
    }

    CAManagerTerminateLEAutoConnection();
    CAManagerTerminateMutexVariables();

    if (g_candidateACList)
    {
        [g_candidateACList removeAllObjects];
        [g_candidateACList release];
        g_candidateACList = nil;
    }

    res = CAManagerTerminateSetString();
    if (CA_STATUS_OK != res)
    {
        OIC_LOG(ERROR, TAG, "CAManagerTerminateSetString has failed");
    }

    return res;
}


//BLE CALLBACK


static void CAManagerAdapterStateChangeCB(CBCentralManager *central)
{
    switch(central.state){
        case CBCentralManagerStatePoweredOn:
        {
            OIC_LOG(DEBUG, TAG, "AdapterStateChangedCallback : state_on");

            // when BT state is on. recovery flag has to be reset.
            CAManagerSetBTRecovery(false);

            // find target device for autoconnect
            size_t length = CAManagerGetACDataLength();
            OIC_LOG_V(DEBUG, TAG, "target device : %d", length);
            for (size_t idx = 0; idx < length; idx++)
            {
                char *leAddress = CAManagerGetLEAddressFromACData(idx);
                if (leAddress)
                {
                    dispatch_async(g_caManagerAutoConnectQueue, ^{
                        CAResult_t res = CAManagerStartAutoConnection(leAddress);
                        if (CA_STATUS_OK != res)
                        {
                            OIC_LOG(ERROR, TAG, "CAManagerStartAutoConnection has failed");
                            return;
                        }
                    });
                }
            }
            break;
        }
        case CBCentralManagerStatePoweredOff:
        {
            OIC_LOG(DEBUG, TAG, "AdapterStateChangedCallback : state_off");

            // reset autoconnect flag for all target devices
            size_t length = CAManagerGetACDataLength();
            for (size_t idx = 0; idx < length; idx++)
            {
                char *address = CAManagerGetLEAddressFromACData(idx);
                if (address)
                {
                    CAManagerSetAutoConnectionFlag(address, false);
                    CAManagerSignalToRetryCond(address); //cancel if auto connecting is processing
                }
            }

            // check whether BT recovery is needed or not
            if (CAManagerIsRecoveryFlagSet())
            {
                CAManagerProcessRecovery(STATE_OFF);
            }
            break;
        }
        case CBCentralManagerStateUnsupported:
        case CBCentralManagerStateUnauthorized:
        case CBCentralManagerStateResetting:
        default:
            break;
    }
}

static void CAManagerLeGattConnectionStateChangeCB(CBPeripheral *peripheral,
                                         const char *remote_address, bool connected, NSError *error)
{
    NSLog(@"peripheral : %@", peripheral);
    OIC_LOG_V(DEBUG, TAG, "CAManagerLeGattConnectionStateChangeCB-remote_Address(%s),connected(%s)",
              remote_address, connected ? "true":"false");
    NSLog(@"error : %@", error);

    VERIFY_NON_NULL_VOID(peripheral, TAG, "peripheral");
    VERIFY_NON_NULL_VOID(remote_address, TAG, "remote_address");


    if (connected) {
        OIC_LOG(DEBUG, TAG, "LE is connected");
    }
    else {
        OIC_LOG(DEBUG, TAG, "LE is disconnected");

        if (!CAManagerIsMatchedACData(remote_address))
        {
            OIC_LOG_V(DEBUG, TAG, "this[%s] is not target address for Auto Connection",
                      remote_address);
            return;
        }

        CAManagerSetAutoConnectionFlag(remote_address, false);

        dispatch_async(g_caManagerAutoConnectQueue, ^{
            CAResult_t res = CAManagerStartAutoConnection(remote_address);
            if (CA_STATUS_OK != res)
            {
                OIC_LOG(ERROR, TAG, "CAManagerStartAutoConnection has failed");
                return;
            }
        });
    }
}

static void CAManagerLeGattServiceDiscoveredCB(CBPeripheral *peripheral,
                                               const char *remote_address, NSError *error)
{
    OIC_LOG(DEBUG, TAG, "CAManagerLeGattServiceDiscoveredCB");

    if (error){
        OIC_LOG(ERROR, TAG, "error - CAManagerLeGattServiceDiscoveredCB");
        return;
    }

    VERIFY_NON_NULL_VOID(peripheral, TAG, "peripheral");
    VERIFY_NON_NULL_VOID(remote_address, TAG, "remote_address");

    OIC_LOG_V(DEBUG, TAG, "ServicesDiscovered device : %s", remote_address);

    if(CAManagerIsConnectedDeviceAddress(remote_address))
    {
        // if BLE address is matched each other
        // this address will be added into auto connection list.
        OIC_LOG(INFO, TAG, "AC list - address will be added into ACData list");
        CAManagerAddACData(remote_address);

        if(!CAManagerSetValueConnectedDeviceAddress(peripheral, remote_address))
        {
            OIC_LOG(ERROR, TAG, "AC list - address is un-registered");
            return;
        }

        CAManagerSetAutoConnectionFlag(remote_address, false);

        // next connection will be requested as true flag
        // after first connection
        CALEClientSetAutoConnectFlag(remote_address, true);
    }
    else
    {
        OIC_LOG(INFO, TAG, "AC list - device is not matched");

        //store candidate AC peripheral
        g_candidateACList[@(remote_address)] = peripheral;
    }

    CAManagerSignalToRetryCond(remote_address);

    OIC_LOG(INFO, TAG, "ServicesDiscovery is successful");
}

void CAManagerLEStopScan()
{
    OIC_LOG(DEBUG, TAG, "CAManagerLEStopScan");
    CALEClientStopScan();
}
