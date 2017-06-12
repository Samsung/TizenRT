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

#include <string.h>

#include "cacommon.h"
#include "logger.h"
#include "cacommonutil.h"
#include "camanagerleutil.h"
#include "uarraylist.h"
#include "octhread.h"
#include "camanagerdevice.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define TAG "OIC_CA_MANAGER_DEVICE"

static u_arraylist_t *g_deviceACDataList = NULL;
static oc_mutex g_deviceACDataListMutex = NULL;
static bool g_isBTRecovery = false;

char* CAManagerGetLEAddressFromACData(size_t idx)
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetLEAddressFromACData");
    OIC_LOG_V(DEBUG, TAG, "idx : %d", idx);
    if (idx <= u_arraylist_length(g_deviceACDataList))
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(
                g_deviceACDataList, idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            return NULL;
        }

        const char* address = curData->address;
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            return NULL;
        }
        OIC_LOG_V(INFO, TAG, "found out target address : %s", address);

        return curData->address;
    }
    return NULL;
}

void CAManagerCreateACDataList()
{
    OIC_LOG(DEBUG, TAG, "CAManagerCreateACDataList");

    oc_mutex_lock(g_deviceACDataListMutex);
    // create new object array
    if (g_deviceACDataList == NULL)
    {
        OIC_LOG(DEBUG, TAG, "Create AC Data list");

        g_deviceACDataList = u_arraylist_create();
    }
    oc_mutex_unlock(g_deviceACDataListMutex);
}

CAResult_t CAManagerInitMutexVaraibles()
{
    if (NULL == g_deviceACDataListMutex)
    {
        g_deviceACDataListMutex = oc_mutex_new();
        if (NULL == g_deviceACDataListMutex)
        {
            OIC_LOG(ERROR, TAG, "oc_mutex_new has failed");
            return CA_STATUS_FAILED;
        }
    }
    return CA_STATUS_OK;
}

void CAManagerTerminateMutexVariables()
{
    if (g_deviceACDataListMutex)
    {
        oc_mutex_free(g_deviceACDataListMutex);
        g_deviceACDataListMutex = NULL;
    }
}

static CAManagerACData_t *CAManagerCreateACData(char *address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerCreateACData");
    VERIFY_NON_NULL_RET(address, TAG, "address", NULL);

    // create block data
    CAManagerACData_t *data = (CAManagerACData_t *) OICCalloc(1, sizeof(*data));
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "memory alloc has failed");
        return NULL;
    }

    OICStrcpy(data->address, strlen(address)+1, address);
    data->isAutoConnect = false;

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerCreateACData");
    return data;
}

bool CAManagerIsMatchedACData(const char* address)
{
    VERIFY_NON_NULL_RET(address, TAG, "address", false);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            return false;
        }

        const char* setAddress = curData->address;
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            return false;
        }

        if (!strcmp(setAddress, address))
        {
            return true;
        }
    }

    return false;
}

void CAManagerAddACData(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerAddACData");

    VERIFY_NON_NULL_VOID(address, TAG, "address");

    oc_mutex_lock(g_deviceACDataListMutex);

    if(!CAManagerIsMatchedACData(address))
    {
        OIC_LOG(DEBUG, TAG, "ACdata will be added");

        CAManagerACData_t *data = CAManagerCreateACData(address);
        u_arraylist_add(g_deviceACDataList, data);
    }
    oc_mutex_unlock(g_deviceACDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerAddACData");
}

CAResult_t CAManagerRemoveData(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerRemoveData");
    VERIFY_NON_NULL(address, TAG, "address");

    oc_mutex_lock(g_deviceACDataListMutex);

    OIC_LOG_V(DEBUG, TAG, "(%s) will be removed", address);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = curData->address;
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcmp(setAddress, address))
        {
            if (NULL == u_arraylist_remove(g_deviceACDataList, idx))
            {
                OIC_LOG(ERROR, TAG, "removal has failed.");
                oc_mutex_unlock(g_deviceACDataListMutex);
                return CA_STATUS_FAILED;
            }

            OICFree(curData);
            oc_mutex_unlock(g_deviceACDataListMutex);
            OIC_LOG(DEBUG, TAG, "remove done");
            return CA_STATUS_OK;
        }
    }

    oc_mutex_unlock(g_deviceACDataListMutex);
    OIC_LOG(DEBUG, TAG, "OUT-CAManagerRemoveData");
    return CA_STATUS_OK;
}

CAResult_t CAManagerRemoveAllData()
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerRemoveAllData");

    oc_mutex_lock(g_deviceACDataListMutex);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        if (NULL == u_arraylist_remove(g_deviceACDataList, idx))
        {
            OIC_LOG(ERROR, TAG, "removal has failed.");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        OICFree(curData);
    }
    oc_mutex_unlock(g_deviceACDataListMutex);
    OIC_LOG(DEBUG, TAG, "OUT-CAManagerRemoveAllData");
    return CA_STATUS_OK;
}

bool CAManagerGetAutoConnectionFlag(const char* address)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerGetAutoConnectionFlag");
    VERIFY_NON_NULL_RET(address, TAG, "address", NULL);

    oc_mutex_lock(g_deviceACDataListMutex);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = curData->address;
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcmp(setAddress, address))
        {
            oc_mutex_unlock(g_deviceACDataListMutex);
            OIC_LOG_V(DEBUG, TAG, "flag is %d", curData->isAutoConnect);
            return curData->isAutoConnect;
        }
    }
    oc_mutex_unlock(g_deviceACDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerGetAutoConnectionFlag");
    return false;
}

void CAManagerSetAutoConnectionFlag(const char* address, bool flag)
{
    OIC_LOG(DEBUG, TAG, "IN-CAManagerSetAutoConnectionFlag");
    VERIFY_NON_NULL_VOID(address, TAG, "address");

    oc_mutex_lock(g_deviceACDataListMutex);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return;
        }

        const char* setAddress = curData->address;
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            oc_mutex_unlock(g_deviceACDataListMutex);
            return;
        }

        if (!strcmp(setAddress, address))
        {
            OIC_LOG_V(DEBUG, TAG, "flag is set to %d", flag);
            curData->isAutoConnect = flag;
            oc_mutex_unlock(g_deviceACDataListMutex);
            return;
        }
    }
    oc_mutex_unlock(g_deviceACDataListMutex);

    OIC_LOG(DEBUG, TAG, "OUT-CAManagerSetAutoConnectionFlag");
}

size_t CAManagerGetACDataLength()
{
    return u_arraylist_length(g_deviceACDataList);
}

void CAManagerSetBTRecovery(bool flag)
{
    g_isBTRecovery = flag;
    OIC_LOG_V(DEBUG, TAG, "BT recovery flag : %d", g_isBTRecovery);
}

bool CAManagerIsRecoveryFlagSet()
{
    return g_isBTRecovery;
}
