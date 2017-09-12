/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <jni.h>
#include "cacommon.h"
#include "logger.h"
#include "cacommonutil.h"
#include "camanagerleutil.h"
#include "uarraylist.h"
#include "octhread.h"
#include "camanagerdevice.h"
#include "oic_malloc.h"

#define TAG "OIC_CA_MANAGER_DEVICE"

static u_arraylist_t *g_deviceACDataList = NULL;
static oc_mutex g_deviceACDataListMutex = NULL;
static bool g_isBTRecovery = false;

jstring CAManagerGetLEAddressFromACData(JNIEnv *env, size_t idx)
{
    OIC_LOG_V(DEBUG, TAG, "CAManagerGetLEAddressFromACData (idx : %d)", idx);
    if (idx <= u_arraylist_length(g_deviceACDataList))
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(
                g_deviceACDataList, idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            return NULL;
        }

        const char* address = (*env)->GetStringUTFChars(env, curData->address, NULL);
        if (!address)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            return NULL;
        }
        OIC_LOG_V(INFO, TAG, "found target address : %s", address);
        (*env)->ReleaseStringUTFChars(env, curData->address, address);

        return curData->address;
    }
    OIC_LOG(DEBUG, TAG, "idx is greater than the length of ACDataList");
    return NULL;
}

void CAManagerCreateACDataList()
{
    OIC_LOG(DEBUG, TAG, "CAManagerCreateACDataList");

    oc_mutex_lock(g_deviceACDataListMutex);
    if (NULL == g_deviceACDataList)
    {
        OIC_LOG(DEBUG, TAG, "Create AC Data list");
        g_deviceACDataList = u_arraylist_create();
    }
    oc_mutex_unlock(g_deviceACDataListMutex);
}

void CAManagerDestroyACDataList()
{
    OIC_LOG(DEBUG, TAG, "CAManagerDestroyACDataList");
    if (g_deviceACDataList)
    {
        OIC_LOG(DEBUG, TAG, "Destroy AC Data list");
        u_arraylist_free(&g_deviceACDataList);
        g_deviceACDataList = NULL;
    }
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

static CAManagerACData_t *CAManagerCreateACData(jstring jaddress)
{
    VERIFY_NON_NULL_RET(jaddress, TAG, "jaddress", NULL);

    // create AC data
    CAManagerACData_t *data = (CAManagerACData_t *) OICCalloc(1, sizeof(*data));
    if (!data)
    {
        OIC_LOG(ERROR, TAG, "memory alloc has failed");
        return NULL;
    }

    data->address = jaddress;
    data->isAutoConnecting = false;
    return data;
}

bool CAManagerIsInACDataList(JNIEnv *env, jstring jaddress)
{
    VERIFY_NON_NULL_RET(env, TAG, "env", NULL);
    VERIFY_NON_NULL_RET(jaddress, TAG, "jaddress", false);

    oc_mutex_lock(g_deviceACDataListMutex);

    const char* address = (*env)->GetStringUTFChars(env, jaddress, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        oc_mutex_unlock(g_deviceACDataListMutex);
        return false;
    }

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return false;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, curData->address, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return false;
        }

        if (!strcmp(setAddress, address))
        {
            (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return true;
        }
        (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in list", address);
    (*env)->ReleaseStringUTFChars(env, jaddress, address);
    oc_mutex_unlock(g_deviceACDataListMutex);

    return false;
}

void CAManagerAddACData(JNIEnv *env, jstring jaddress)
{
    VERIFY_NON_NULL_VOID(env, TAG, "env");
    VERIFY_NON_NULL_VOID(jaddress, TAG, "jaddress");

    if(!CAManagerIsInACDataList(env, jaddress))
    {
        OIC_LOG(DEBUG, TAG, "new ACdata will be added in List");
        // add CAManagerACData
        jobject gaddress = (*env)->NewGlobalRef(env, jaddress);

        CAManagerACData_t *data = CAManagerCreateACData(gaddress);

        oc_mutex_lock(g_deviceACDataListMutex);
        u_arraylist_add(g_deviceACDataList, data);
        oc_mutex_unlock(g_deviceACDataListMutex);
    }
    else
    {
        OIC_LOG(DEBUG, TAG, "the address is already in ACData list");
    }
}

CAResult_t CAManagerRemoveACData(JNIEnv *env, jstring jaddress)
{
    OIC_LOG(DEBUG, TAG, "CAManagerRemoveACData");
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(jaddress, TAG, "jaddress");

    oc_mutex_lock(g_deviceACDataListMutex);

    const char* address = (*env)->GetStringUTFChars(env, jaddress, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        oc_mutex_unlock(g_deviceACDataListMutex);
        return CA_STATUS_FAILED;
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] will be removed", address);

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, curData->address, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcmp(setAddress, address))
        {
            if (NULL == u_arraylist_remove(g_deviceACDataList, idx))
            {
                OIC_LOG(ERROR, TAG, "removal has failed.");
                (*env)->ReleaseStringUTFChars(env, jaddress, address);
                (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
                oc_mutex_unlock(g_deviceACDataListMutex);
                return CA_STATUS_FAILED;
            }

            (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);

            if (curData->address)
            {
                (*env)->DeleteGlobalRef(env, curData->address);
            }

            OICFree(curData);
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            OIC_LOG(DEBUG, TAG, "remove done");
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in list", address);
    (*env)->ReleaseStringUTFChars(env, jaddress, address);
    oc_mutex_unlock(g_deviceACDataListMutex);

    return CA_STATUS_OK;
}

CAResult_t CAManagerRemoveAllACData(JNIEnv *env)
{
    OIC_LOG(DEBUG, TAG, "IN - CAManagerRemoveAllACData");
    VERIFY_NON_NULL(env, TAG, "env");

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

        if (curData->address)
        {
            (*env)->DeleteGlobalRef(env, curData->address);
        }

        OICFree(curData);
    }
    oc_mutex_unlock(g_deviceACDataListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CAManagerRemoveAllACData");
    return CA_STATUS_OK;
}

CAResult_t CAManagerGetAutoConnectingFlag(JNIEnv *env, jstring jaddress, bool *flag)
{
    OIC_LOG(DEBUG, TAG, "CAManagerGetAutoConnectingFlag");
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(jaddress, TAG, "jaddress");

    oc_mutex_lock(g_deviceACDataListMutex);

    const char* address = (*env)->GetStringUTFChars(env, jaddress, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        oc_mutex_unlock(g_deviceACDataListMutex);
        return CA_STATUS_FAILED;
    }

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, curData->address, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "setAddress is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_FAILED;
        }

        if (!strcmp(setAddress, address))
        {
            OIC_LOG_V(DEBUG, TAG, "address : [%s], isAutoConnecting : %d", address,
                      curData->isAutoConnecting);
            *flag = curData->isAutoConnecting;
            (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return CA_STATUS_OK;
        }
        (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in list", address);
    (*env)->ReleaseStringUTFChars(env, jaddress, address);
    oc_mutex_unlock(g_deviceACDataListMutex);

    return CA_STATUS_FAILED;
}

bool CAManagerSetAutoConnectingFlag(JNIEnv *env, jstring jaddress, bool flag)
{
    OIC_LOG(DEBUG, TAG, "CAManagerSetAutoConnectingFlag");
    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(jaddress, TAG, "jaddress", false);

    oc_mutex_lock(g_deviceACDataListMutex);

    const char* address = (*env)->GetStringUTFChars(env, jaddress, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is null");
        oc_mutex_unlock(g_deviceACDataListMutex);
        return false;
    }

    size_t length = u_arraylist_length(g_deviceACDataList);
    for (size_t idx = 0; idx < length; idx++)
    {
        CAManagerACData_t *curData = (CAManagerACData_t *) u_arraylist_get(g_deviceACDataList,
                                                                           idx);
        if (!curData)
        {
            OIC_LOG(ERROR, TAG, "curData is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return false;
        }

        const char* setAddress = (*env)->GetStringUTFChars(env, curData->address, NULL);
        if (!setAddress)
        {
            OIC_LOG(ERROR, TAG, "address is null");
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return false;
        }

        if (!strcmp(setAddress, address))
        {
            OIC_LOG_V(DEBUG, TAG, "isAutoConnecting flag of [%s] is set to %d", address,
                      curData->isAutoConnecting);
            curData->isAutoConnecting = flag;
            (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
            (*env)->ReleaseStringUTFChars(env, jaddress, address);
            oc_mutex_unlock(g_deviceACDataListMutex);
            return true;
        }
        (*env)->ReleaseStringUTFChars(env, curData->address, setAddress);
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in list", address);
    (*env)->ReleaseStringUTFChars(env, jaddress, address);
    oc_mutex_unlock(g_deviceACDataListMutex);

    return false;
}

size_t CAManagerGetACDataLength()
{
    return u_arraylist_length(g_deviceACDataList);
}

void CAManagerSetBTRecovery(bool flag)
{
    g_isBTRecovery = flag;
    OIC_LOG_V(DEBUG, TAG, "BT recovery flag is set to %d", g_isBTRecovery);
}

bool CAManagerIsRecoveryFlagSet()
{
    return g_isBTRecovery;
}
