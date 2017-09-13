/**
 * \file    ssDevice.c
 * \brief    reading for device unique value
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ssDefines.h"
#include "ssDevice.h"

#include "thingslogger.h"

#define TAG "SECURE_STORAGE"

static GetDeviceUniqueID gGetDeviceUniqueID = NULL;
static char gDeviceUniqueID[SS_DEVICE_ID_MAX_SIZE];
static unsigned int gDeviceUniqueIDLen = 0;

void ssSetDeviceUniqueIDCallback(GetDeviceUniqueID getIdCB)
{
    gGetDeviceUniqueID = getIdCB;
}

/**
 * @brief    API to read device unique ID
 *
 * @param    pIdBuf            [out] Buffer for device unique ID
 * @param   nIdBufSize      [in] size of pIdBuf
 * @param    pIdLen            [out] Length of ID
 * @return    SS_SUCCESS        if no error is occured
 */
int ssGetDeviceID(unsigned char* pIdBuf, size_t nIdBufSize, unsigned int* pIdLen)
{
    int returnValue = SS_NULL_REFERENCE;

    if(NULL == pIdBuf || pIdLen == NULL || nIdBufSize == 0)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid parameter..");
        return SS_INVALID_ARGUMENT;
    }

    if(gGetDeviceUniqueID)
    {
        if(0 < gDeviceUniqueIDLen)
        {
            if(nIdBufSize >= gDeviceUniqueIDLen)
            {
                memcpy(pIdBuf, gDeviceUniqueID, gDeviceUniqueIDLen);
                *pIdLen = gDeviceUniqueIDLen;
                returnValue = SS_SUCCESS;
            }
            else
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "DeviceID buffer too small..");
                returnValue = SS_BUFFER_OVERRUN;
            }
        }
        else
        {
            returnValue = gGetDeviceUniqueID(pIdBuf, nIdBufSize, pIdLen);
            if(SS_SUCCESS == returnValue)
            {
                memset(gDeviceUniqueID, 0x00, SS_DEVICE_ID_MAX_SIZE);
                memcpy(gDeviceUniqueID, pIdBuf, *pIdLen);
                gDeviceUniqueIDLen = *pIdLen;
                THINGS_LOG(THINGS_INFO, TAG, "gGetDeviceUniqueID() was successfully done.");
            }
        }
    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Can not find callback to read device unique ID.");
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Please set the callback using ssSetDeviceUniqueIDCallback() API.");
        returnValue = SS_NULL_REFERENCE;
    }

    return returnValue;
}