/* ****************************************************************
 *
 * Copyright 2015 Intel Corporation All Rights Reserved.
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


#include "server.h"
#include "service.h"
#include "peripheral.h"

#include "logger.h"

#include <assert.h>


// Logging tag.
static char const TAG[] = "BLE_SERVER";

static CAResult_t CAGattServerSendResponseNotificationImpl(
    CAGattService * service,
    uint8_t const * data,
    size_t length)
{
    assert(service != NULL);

    GattCharacteristic1 * const characteristic =
        service->response_characteristic.characteristic;

    if (!gatt_characteristic1_get_notifying(characteristic))
    {
        OIC_LOG(WARNING,
                TAG,
                "Attempt to send response with notifications "
                "disabled.\n"
                "Client must enable notifications. "
                "No response was sent.");

        return CA_STATUS_FAILED;
    }

    GVariant * const value =
        g_variant_new_fixed_array(G_VARIANT_TYPE_BYTE,
                                  data,
                                  length,
                                  sizeof(data[0]));

    /*
      Send the response fragment by setting the "Value" property on
      the response characteristic, and emitting the
      org.freedesktop.Dbus.Properties.PropertiesChanged signal,
      accordingly.

      @todo Do we need to explicitly emit the GObject notify or
            org.freedesktop.Dbus.Properties.PropertiesChanged
            signal here?

      @todo It feels like this function should be part of the
            response characteristic implementation.

      @todo Do we need to decrease the ref count after setting the
            value, i.e. is ownership of the value transferred to the
            characteristic object?
    */
    gatt_characteristic1_set_value(characteristic, value);

    return CA_STATUS_OK;
}

typedef struct _CAGattServerResponseInfo
{
    uint8_t const * const data;
    size_t const length;
    CAResult_t result;

} CAGattServerResponseInfo;

static void CAGattServerSendResponseNotificationToService(gpointer data,
                                                          gpointer user_data)
{
    CAGattService * const service = data;
    CAGattServerResponseInfo * const info = user_data;

    CAResult_t const result =
        CAGattServerSendResponseNotificationImpl(service,
                                                 info->data,
                                                 info->length);

    if (result != CA_STATUS_OK)
    {
        // Propagate failure if any send operation fails.
        info->result = result;
    }
}

CAResult_t CAGattServerSendResponseNotification(
    char const * address,
    uint8_t const * data,
    size_t length)
{
    CAGattService * const s = CAGattServiceDecodeAddress(address);

    return CAGattServerSendResponseNotificationImpl(s, data, length);
}

CAResult_t CAGattServerSendResponseNotificationToAll(
    uint8_t const * data,
    size_t length)
{
    CAGattServerResponseInfo info =
    {
        .data   = data,
        .length = length,
        .result = CA_STATUS_OK
    };

    CAPeripheralForEachService(
        CAGattServerSendResponseNotificationToService,
        &info);

    return info.result;
}
