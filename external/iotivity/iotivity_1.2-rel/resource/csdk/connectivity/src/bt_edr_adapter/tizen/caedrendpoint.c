/* ****************************************************************
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
 * This file provides the APIs to send data on established RFCOMM connections.
 */

#include "caedrendpoint.h"
#include "caadapterutils.h"
#include "caedrutils.h"
#include "logger.h"

CAResult_t CAEDRSendData(int serverFD, const char *addr, const void *data, uint32_t dataLength)
{
    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "IN");

    VERIFY_NON_NULL(data, EDR_ADAPTER_TAG, "Data is null");
    VERIFY_NON_NULL(addr, EDR_ADAPTER_TAG, "Addr is null");

    if (0 > serverFD)
    {
        OIC_LOG(ERROR, EDR_ADAPTER_TAG, "Invalid input: Negative socket id");
        return CA_STATUS_INVALID_PARAM;
    }

    int dataLen = bt_socket_send_data(serverFD, (const char *)data, dataLength);
    int errcode = get_last_result();

    if (TIZEN_ERROR_NONE == errcode)
    {
        CALogSendStateInfo(CA_ADAPTER_RFCOMM_BTEDR, addr, 0, dataLength, true, NULL);
    }
    else
    {
        if (dataLen == -1)
        {
            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "sending data failed!, soketid [%d] errmsg [%s]",
                      serverFD, get_error_message(errcode));
        }
        else if (dataLen == 0)
        {
            OIC_LOG_V(ERROR, EDR_ADAPTER_TAG, "soketid [%d] may be disconnected? errmsg [%s]",
                      serverFD, get_error_message(errcode));
        }

        CALogSendStateInfo(CA_ADAPTER_RFCOMM_BTEDR, addr, 0, dataLength,
                           false, get_error_message(errcode));

        return CA_SOCKET_OPERATION_FAILED;
    }

    OIC_LOG(DEBUG, EDR_ADAPTER_TAG, "OUT");
    return CA_STATUS_OK;
}

