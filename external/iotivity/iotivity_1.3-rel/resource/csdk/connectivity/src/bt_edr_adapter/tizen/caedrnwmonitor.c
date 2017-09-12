/******************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
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
 *
 * This file provides the APIs for EDR Network Monitor.
 */

#include <string.h>
#include <bluetooth.h>

#include "caedrinterface.h"
#include "caedrdevicelist.h"
#include "caedrutils.h"
#include "caadapterutils.h"
#include "caqueueingthread.h"
#include "caremotehandler.h"

/**
 * @var g_edrNetworkChangeCallback
 * @brief Maintains the callback to be notified on local bluetooth adapter status change.
 */
static CAEDRNetworkStatusCallback g_edrNetworkChangeCallback = NULL;

/**
 * @fn CAEDRAdapterStateChangeCallback
 * @brief This callback is registered to receive bluetooth adapter state changes.
 */
static void CAEDRAdapterStateChangeCallback(int result, bt_adapter_state_e adapterState,
                                            void *userData);

CAResult_t CAEDRInitializeNetworkMonitor(const ca_thread_pool_t threadPool)
{
    (void)threadPool;
    return CA_STATUS_OK;
}

void CAEDRTerminateNetworkMonitor()
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    g_edrNetworkChangeCallback = NULL;

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}

CAResult_t CAEDRStartNetworkMonitor()
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Initialize Bluetooth service
    int ret = bt_initialize();
    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Bluetooth initialization failed!, error num [%x]",
                  ret);
        return CA_STATUS_FAILED;
    }

    ret = bt_adapter_set_state_changed_cb(CAEDRAdapterStateChangeCallback, NULL);
    if(BT_ERROR_NONE != ret)
    {
       OIC_LOG(ERROR, EDR_ADAPTER_TAG, "bt_adapter_set_state_changed_cb failed");
       return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRStopNetworkMonitor()
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");
    // Unset bluetooth adapter callbacks
    int ret = bt_adapter_unset_state_changed_cb();
    if(BT_ERROR_NONE != ret)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "bt_adapter_set_state_changed_cb failed");
        return CA_STATUS_FAILED;
    }

    ret = bt_deinitialize();
    if (BT_ERROR_NONE != ret)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "bt_deinitialize failed");
        return CA_STATUS_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

void CAEDRSetNetworkChangeCallback(
    CAEDRNetworkStatusCallback networkChangeCallback)
{
    g_edrNetworkChangeCallback = networkChangeCallback;
}

CAResult_t CAEDRGetInterfaceInformation(CAEndpoint_t **info)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    // Input validation
    VERIFY_NON_NULL(info, EDR_ADAPTER_TAG, "LocalConnectivity info is null");

    // Get the bluetooth adapter local address
    char *localAddress = NULL;
    int err = bt_adapter_get_address(&localAddress);
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG,
                  "Getting local adapter address failed!, error num [%x]",
                  err);
        return CA_STATUS_FAILED;
    }

    // Create network info
    *info = CACreateEndpointObject(CA_DEFAULT_FLAGS, CA_ADAPTER_RFCOMM_BTEDR, localAddress, 0);
    if (NULL == *info)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Failed to create LocalConnectivity instance!");

        OICFree(localAddress);
        return CA_MEMORY_ALLOC_FAILED;
    }

    OICFree(localAddress);

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

CAResult_t CAEDRGetAdapterEnableState(bool *state)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");
    VERIFY_NON_NULL(state, EDR_ADAPTER_TAG, "state holder is NULL!");

    bt_adapter_state_e adapterState;
    int err = bt_adapter_get_state(&adapterState);
    // Get Bluetooth adapter state
    if (BT_ERROR_NONE != err)
    {
        OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "Bluetooth get state failed!, error num [%x]",
                  err);

        return CA_STATUS_FAILED;
    }

    *state = false;
    if (BT_ADAPTER_ENABLED == adapterState)
    {
        *state = true;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

void CAEDRAdapterStateChangeCallback(int result, bt_adapter_state_e adapterState,
                                     void *userData)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");
    OC_UNUSED(result);
    OC_UNUSED(userData);

    if (BT_ADAPTER_ENABLED == adapterState)
    {
        // Notity to upper layer
        if (g_edrNetworkChangeCallback)
        {
            g_edrNetworkChangeCallback(CA_INTERFACE_UP);
        }
    }
    else if (BT_ADAPTER_DISABLED == adapterState)
    {
        // Notity to upper layer
        if (g_edrNetworkChangeCallback)
        {
            g_edrNetworkChangeCallback(CA_INTERFACE_DOWN);
        }
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
}
