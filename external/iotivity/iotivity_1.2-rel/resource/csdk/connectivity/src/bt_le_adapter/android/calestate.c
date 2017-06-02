/******************************************************************
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

#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <unistd.h>

#include "calestate.h"
#include "caleinterface.h"
#include "caadapterutils.h"
#include "caleutils.h"

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "cathreadpool.h" /* for thread pool */
#include "octhread.h"
#include "uarraylist.h"

#define TAG PCF("OIC_CA_LE_STATE")

CAResult_t CALEUpdateDeviceState(const char* address,
                                 uint16_t state_type,
                                 uint16_t target_state,
                                 u_arraylist_t *deviceList,
                                 oc_mutex deviceListMutex)
{
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList is null");

    oc_mutex_lock(deviceListMutex);

    if (CALEIsDeviceInList(address, deviceList))
    {
        CALEState_t* curState = CALEGetStateInfo(address, deviceList);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            oc_mutex_unlock(deviceListMutex);
            return CA_STATUS_FAILED;
        }

        switch(state_type)
        {
            case CA_LE_CONNECTION_STATE:
                curState->connectedState = target_state;
                break;
            case CA_LE_SEND_STATE:
                curState->sendState = target_state;
                break;
            default:
                break;
        }
        OIC_LOG_V(INFO, TAG, "update state - addr: %s, conn: %d, send: %d, ACFlag: %d, mtu: %d",
                  curState->address, curState->connectedState, curState->sendState,
                  curState->autoConnectFlag, curState->mtuSize);
    }
    else /** state is added newly **/
    {
        if (strlen(address) > CA_MACADDR_SIZE)
        {
            OIC_LOG(ERROR, TAG, "address is not proper");
            oc_mutex_unlock(deviceListMutex);
            return CA_STATUS_INVALID_PARAM;
        }

        CALEState_t *newstate = (CALEState_t*) OICCalloc(1, sizeof(*newstate));
        if (!newstate)
        {
            OIC_LOG(ERROR, TAG, "out of memory");
            oc_mutex_unlock(deviceListMutex);
            return CA_MEMORY_ALLOC_FAILED;
        }

        OICStrcpy(newstate->address, sizeof(newstate->address), address);
        newstate->mtuSize = CA_DEFAULT_BLE_MTU_SIZE;
        switch(state_type)
        {
            case CA_LE_CONNECTION_STATE:
                newstate->connectedState = target_state;
                newstate->sendState = STATE_SEND_NONE;
                break;
            case CA_LE_SEND_STATE:
                newstate->connectedState = STATE_DISCONNECTED;
                newstate->sendState = target_state;
                break;
            default:
                break;
        }
        OIC_LOG_V(INFO, TAG, "add a new state to List - addr : %s, "
                  "conn : %d, send : %d, ACFlag : %d",
                  newstate->address, newstate->connectedState, newstate->sendState,
                  newstate->autoConnectFlag);
        u_arraylist_add(deviceList, newstate); // update new state
    }
    oc_mutex_unlock(deviceListMutex);

    return CA_STATUS_OK;
}

bool CALEIsDeviceInList(const char* remoteAddress,
                        u_arraylist_t *deviceList)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);
    VERIFY_NON_NULL_RET(deviceList, TAG, "deviceList is null", false);

    uint32_t length = u_arraylist_length(deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(deviceList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            return false;
        }

        if (!strcasecmp(remoteAddress, state->address))
        {
            return true;
        }
        else
        {
            continue;
        }
    }

    OIC_LOG(INFO, TAG, "there are no the device in list.");
    return false;
}

CAResult_t CALERemoveAllDeviceState(u_arraylist_t *deviceList,
                                    oc_mutex deviceListMutex)
{
    OIC_LOG(DEBUG, TAG, "CALERemoveAllDeviceState");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList is null");

    oc_mutex_lock(deviceListMutex);
    uint32_t length = u_arraylist_length(deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(deviceList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }
        OICFree(state);
    }

    oc_mutex_unlock(deviceListMutex);

    return CA_STATUS_OK;
}

CAResult_t CALEResetDeviceStateForAll(u_arraylist_t *deviceList,
                                      oc_mutex deviceListMutex)
{
    OIC_LOG(DEBUG, TAG, "CALEResetDeviceStateForAll");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList is null");

    oc_mutex_lock(deviceListMutex);
    size_t length = u_arraylist_length(deviceList);
    for (size_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(deviceList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "jarrayObj is null");
            continue;
        }

        // autoConnectFlag value will be not changed,
        // since it has reset only termination case.
        state->connectedState = STATE_DISCONNECTED;
        state->sendState = STATE_SEND_NONE;
    }
    oc_mutex_unlock(deviceListMutex);

    return CA_STATUS_OK;
}

CAResult_t CALERemoveDeviceState(const char* remoteAddress,
                                 u_arraylist_t *deviceList)
{
    OIC_LOG(DEBUG, TAG, "CALERemoveDeviceState");
    VERIFY_NON_NULL(remoteAddress, TAG, "remoteAddress is null");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList is null");

    uint32_t length = u_arraylist_length(deviceList);
    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(deviceList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        if (!strcasecmp(state->address, remoteAddress))
        {
            OIC_LOG_V(DEBUG, TAG, "remove state : %s", state->address);

            CALEState_t* targetState  = (CALEState_t*)u_arraylist_remove(deviceList,
                                                                         index);
            if (NULL == targetState)
            {
                OIC_LOG(ERROR, TAG, "List removal failed.");
                return CA_STATUS_FAILED;
            }

            OICFree(targetState);
            return CA_STATUS_OK;
        }
    }
    return CA_STATUS_OK;
}

CALEState_t* CALEGetStateInfo(const char* remoteAddress,
                              u_arraylist_t *deviceList)
{
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", NULL);
    VERIFY_NON_NULL_RET(deviceList, TAG, "deviceList is null", NULL);

    uint32_t length = u_arraylist_length(deviceList);

    for (uint32_t index = 0; index < length; index++)
    {
        CALEState_t* state = (CALEState_t*) u_arraylist_get(deviceList, index);
        if (!state)
        {
            OIC_LOG(ERROR, TAG, "CALEState_t object is null");
            continue;
        }

        if (!strcasecmp(state->address, remoteAddress))
        {
            return state;
        }
        OIC_LOG_V(DEBUG, TAG, "state addr[%s, %d]", state->address, index);
    }

    OIC_LOG_V(DEBUG, TAG, "[%s] doesn't exist in deviceStateList", remoteAddress, length);
    return NULL;
}

bool CALEIsValidState(const char* remoteAddress,
                      uint16_t state_type,
                      uint16_t target_state,
                      u_arraylist_t *deviceList,
                      oc_mutex deviceListMutex)
{
    OIC_LOG_V(DEBUG, TAG, "CALEIsValidState : type[%d], target state[%d]",
              state_type, target_state);
    VERIFY_NON_NULL_RET(remoteAddress, TAG, "remoteAddress is null", false);
    VERIFY_NON_NULL_RET(deviceList, TAG, "deviceList is null", false);

    oc_mutex_lock(deviceListMutex);
    CALEState_t* state = CALEGetStateInfo(remoteAddress, deviceList);
    if (NULL == state)
    {
        OIC_LOG(DEBUG, TAG, "state is not updated yet");
        oc_mutex_unlock(deviceListMutex);
        return false;
    }

    uint16_t curValue = 0;
    switch(state_type)
    {
        case CA_LE_CONNECTION_STATE:
            curValue = state->connectedState;
            break;
        case CA_LE_SEND_STATE:
            curValue = state->sendState;
            break;
        default:
            break;
    }

    if (target_state == curValue)
    {
        oc_mutex_unlock(deviceListMutex);
        return true;
    }
    else
    {
        oc_mutex_unlock(deviceListMutex);
        return false;
    }

    oc_mutex_unlock(deviceListMutex);
    return false;
}


CAResult_t CALESetFlagToState(JNIEnv *env, jstring jni_address, jint state_idx, jboolean flag,
                              u_arraylist_t *deviceList, oc_mutex deviceListMutex)
{
    OIC_LOG(DEBUG, TAG, "IN - CALESetFlagToState");
    VERIFY_NON_NULL(env, TAG, "env");
    VERIFY_NON_NULL(jni_address, TAG, "jni_address");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList");

    oc_mutex_lock(deviceListMutex);

    char* address = (char*)(*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        CACheckJNIException(env);
        return CA_STATUS_FAILED;
    }

    if (CALEIsDeviceInList(address, deviceList))
    {
        CALEState_t* curState = CALEGetStateInfo(address, deviceList);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            (*env)->ReleaseStringUTFChars(env, jni_address, address);
            oc_mutex_unlock(deviceListMutex);
            return CA_STATUS_FAILED;
        }
        OIC_LOG_V(INFO, TAG, "%d flag is set : %d", state_idx, flag);

        switch(state_idx)
        {
            case CA_LE_AUTO_CONNECT_FLAG:
                curState->autoConnectFlag = flag;
                break;
            case CA_LE_DESCRIPTOR_FOUND:
                curState->isDescriptorFound = flag;
                break;
            default:
                break;
        }
    }

    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    oc_mutex_unlock(deviceListMutex);
    OIC_LOG(DEBUG, TAG, "OUT - CALESetFlagToState");
    return CA_STATUS_OK;
}

jboolean CALEGetFlagFromState(JNIEnv *env, jstring jni_address, jint state_idx,
                              u_arraylist_t *deviceList, oc_mutex deviceListMutex)
{
    OIC_LOG(DEBUG, TAG, "IN - CALEGetFlagFromState");
    VERIFY_NON_NULL_RET(env, TAG, "env", false);
    VERIFY_NON_NULL_RET(jni_address, TAG, "jni_address", false);
    VERIFY_NON_NULL_RET(deviceList, TAG, "deviceList", false);

    oc_mutex_lock(deviceListMutex);

    char* address = (char*)(*env)->GetStringUTFChars(env, jni_address, NULL);
    if (!address)
    {
        OIC_LOG(ERROR, TAG, "address is not available");
        CACheckJNIException(env);
        oc_mutex_unlock(deviceListMutex);
        return JNI_FALSE;
    }

    CALEState_t* curState = CALEGetStateInfo(address, deviceList);
    (*env)->ReleaseStringUTFChars(env, jni_address, address);
    if(!curState)
    {
        OIC_LOG(INFO, TAG, "there is no information. auto connect flag is false");
        oc_mutex_unlock(deviceListMutex);
        return JNI_FALSE;
    }

    jboolean ret = JNI_FALSE;
    switch(state_idx)
    {
        case CA_LE_AUTO_CONNECT_FLAG:
            ret = curState->autoConnectFlag;
            break;
        case CA_LE_DESCRIPTOR_FOUND:
            ret = curState->isDescriptorFound;
            break;
        default:
            break;
    }
    oc_mutex_unlock(deviceListMutex);

    OIC_LOG_V(INFO, TAG, "%d flag is %d", state_idx, ret);
    OIC_LOG(DEBUG, TAG, "OUT - CALEGetFlagFromState");
    return ret;
}

CAResult_t CALESetMtuSize(const char* address, uint16_t mtuSize,
                          u_arraylist_t *deviceList, oc_mutex deviceListMutex)

{
    VERIFY_NON_NULL(address, TAG, "address is null");
    VERIFY_NON_NULL(deviceList, TAG, "deviceList is null");

    oc_mutex_lock(deviceListMutex);
    if (CALEIsDeviceInList(address, deviceList))
    {
        CALEState_t* curState = CALEGetStateInfo(address, deviceList);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            oc_mutex_unlock(deviceListMutex);
            return CA_STATUS_FAILED;
        }

        curState->mtuSize = mtuSize;
        OIC_LOG_V(INFO, TAG, "update state - addr: %s, mtu: %d",
                  curState->address, curState->mtuSize);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "there is no state info in the list");
    }
    oc_mutex_unlock(deviceListMutex);
    return CA_STATUS_OK;
}

uint16_t CALEGetMtuSize(const char* address, u_arraylist_t *deviceList, oc_mutex deviceListMutex)
{
    VERIFY_NON_NULL_RET(address, TAG, "address is null", CA_DEFAULT_BLE_MTU_SIZE);
    VERIFY_NON_NULL_RET(deviceList, TAG, "deviceList is null", CA_DEFAULT_BLE_MTU_SIZE);

    oc_mutex_lock(deviceListMutex);
    if (CALEIsDeviceInList(address, deviceList))
    {
        CALEState_t* curState = CALEGetStateInfo(address, deviceList);
        if(!curState)
        {
            OIC_LOG(ERROR, TAG, "curState is null");
            oc_mutex_unlock(deviceListMutex);
            return CA_DEFAULT_BLE_MTU_SIZE;
        }

        OIC_LOG_V(INFO, TAG, "state - addr: %s, mtu: %d",
                  curState->address, curState->mtuSize);
        oc_mutex_unlock(deviceListMutex);
        return curState->mtuSize;
    }

    oc_mutex_unlock(deviceListMutex);
    return CA_DEFAULT_BLE_MTU_SIZE;
}
