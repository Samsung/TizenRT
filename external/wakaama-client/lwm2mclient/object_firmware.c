/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Julien Vermillard - initial implementation
 *    Fabien Fleutot - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *
 *******************************************************************************/

/*
 * This object is single instance only, and provide firmware upgrade functionality.
 * Object ID is 5.
 */

/*
 * resources:
 * 0 package                   write
 * 1 package url               write
 * 2 update                    exec
 * 3 state                     read
 * 4 update supported objects  read/write
 * 5 update result             read
 */

#include "liblwm2m.h"
#include "lwm2mclient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---- private object "Firmware" specific defines ----
// Resource Id's:
#define RES_M_PACKAGE                   0
#define RES_M_PACKAGE_URI               1
#define RES_M_UPDATE                    2
#define RES_M_STATE                     3
#define RES_O_UPDATE_SUPPORTED_OBJECTS  4
#define RES_M_UPDATE_RESULT             5
#define RES_O_PKG_NAME                  6
#define RES_O_PKG_VERSION               7

#define LWM2M_FIRMWARE_PKG_URI_LEN      256

typedef struct
{
    uint8_t state;
    bool supported;
    uint8_t result;
    char pkg_name[LWM2M_MAX_STR_LEN];
    char pkg_version[LWM2M_MAX_STR_LEN];
    char pkg_uri[LWM2M_FIRMWARE_PKG_URI_LEN];
    lwm2m_exe_callback update_callback;
    void *update_callback_param;
    lwm2m_exe_callback notify_callback;
    void *notify_callback_param;
} firmware_data_t;

static void prv_notify_resource_changed(firmware_data_t *client, char *uri, lwm2m_data_t *data)
{
    if (client && client->notify_callback)
    {
        lwm2m_resource_t params;

        strncpy(params.uri, uri, LWM2M_MAX_URI_LEN);
        params.buffer = data->value.asBuffer.buffer;
        params.length = data->value.asBuffer.length;

        client->notify_callback(client->notify_callback_param, (void*)&params);
    }
}

static uint8_t prv_firmware_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_data_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
    int i;
    uint8_t result;
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(5);
        if (*dataArrayP == NULL) {
            result = COAP_500_INTERNAL_SERVER_ERROR;
            goto exit;
        }
        *numDataP = 5;
        (*dataArrayP)[0].id = RES_M_STATE;
        (*dataArrayP)[1].id = RES_O_UPDATE_SUPPORTED_OBJECTS;
        (*dataArrayP)[2].id = RES_M_UPDATE_RESULT;
        (*dataArrayP)[3].id = RES_O_PKG_NAME;
        (*dataArrayP)[4].id = RES_O_PKG_VERSION;
    }

    i = 0;
    do
    {
        switch ((*dataArrayP)[i].id)
        {
        case RES_M_PACKAGE:
        case RES_M_PACKAGE_URI:
        case RES_M_UPDATE:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;

        case RES_M_STATE:
            lwm2m_data_encode_int(data->state, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            lwm2m_data_encode_bool(data->supported, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_M_UPDATE_RESULT:
            lwm2m_data_encode_int(data->result, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_O_PKG_NAME:
            lwm2m_data_encode_string(data->pkg_name, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        case RES_O_PKG_VERSION:
            lwm2m_data_encode_string(data->pkg_version, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;

        default:
            result = COAP_404_NOT_FOUND;
        }

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

exit:
    return result;
}

static uint8_t prv_firmware_write(uint16_t instanceId,
                                  int numData,
                                  lwm2m_data_t * dataArray,
                                  lwm2m_object_t * objectP)
{
    int i = 0;
    int ret = 0;
    uint8_t result = 0;
    firmware_data_t *data = (firmware_data_t*)(objectP->userData);

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    do
    {
        switch (dataArray[i].id)
        {
        case RES_M_PACKAGE:
            // inline firmware binary
            result = COAP_204_CHANGED;
            prv_notify_resource_changed(data, LWM2M_URI_FIRMWARE_PACKAGE, &dataArray[i]);
            break;

        case RES_M_PACKAGE_URI:
            strncpy(data->pkg_uri, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
            prv_notify_resource_changed(data, LWM2M_URI_FIRMWARE_PACKAGE_URI, &dataArray[i]);
            result = COAP_204_CHANGED;
            break;

        case RES_O_UPDATE_SUPPORTED_OBJECTS:
            ret = lwm2m_data_decode_bool(&dataArray[i], &data->supported);
            if (ret == 1)
            {
                prv_notify_resource_changed(data, LWM2M_URI_FIRMWARE_UPD_SUPP_OBJ, &dataArray[i]);
                result = COAP_204_CHANGED;
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
            break;

        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
        }

        i++;
    } while (i < numData && result == COAP_204_CHANGED);

    return result;
}

static uint8_t prv_firmware_execute(uint16_t instanceId,
                                    uint16_t resourceId,
                                    uint8_t * buffer,
                                    int length,
                                    lwm2m_object_t * objectP)
{
    firmware_data_t * data = (firmware_data_t*)(objectP->userData);

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND;
    }

    if (length != 0)
    {
        return COAP_400_BAD_REQUEST;
    }

    // for execute callback, resId is always set.
    switch (resourceId)
    {
    case RES_M_UPDATE:
        if (data->state == 2)
        {
#ifdef WITH_LOGS
            fprintf(stdout, "\n\t Firmware Update\r\n\n");
#endif
            if (data->update_callback)
                data->update_callback(data->update_callback_param, NULL);
            return COAP_204_CHANGED;
        }
        else
        {
            // firmware update already running
            return COAP_400_BAD_REQUEST;
        }
    default:
        return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_firmware_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    firmware_data_t * data = (firmware_data_t *)object->userData;
    fprintf(stdout, "  /%u: Firmware object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "\tstate: %u, supported: %s, result: %u\r\n\tname: %s\r\n\tversion: %s\r\n",
                data->state, data->supported?"true":"false", data->result,
                data->pkg_name, data->pkg_version);
    }
#endif
}

lwm2m_object_t * get_object_firmware(object_firmware_t *default_value)
{
    /*
     * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * firmwareObj;

    firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != firmwareObj)
    {
        memset(firmwareObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns its unique ID
         * The 5 is the standard ID for the optional object "Object firmware".
         */
        firmwareObj->objID = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;

        /*
         * and its unique instance
         *
         */
        firmwareObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != firmwareObj->instanceList)
        {
            memset(firmwareObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(firmwareObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        firmwareObj->readFunc    = prv_firmware_read;
        firmwareObj->writeFunc   = prv_firmware_write;
        firmwareObj->executeFunc = prv_firmware_execute;
        firmwareObj->userData    = lwm2m_malloc(sizeof(firmware_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != firmwareObj->userData)
        {
            ((firmware_data_t*)firmwareObj->userData)->state = 0;
            ((firmware_data_t*)firmwareObj->userData)->supported = default_value->supported;
            ((firmware_data_t*)firmwareObj->userData)->result = 0;
            strncpy(((firmware_data_t*)firmwareObj->userData)->pkg_name, default_value->pkg_name, LWM2M_MAX_STR_LEN);
            strncpy(((firmware_data_t*)firmwareObj->userData)->pkg_version, default_value->pkg_version, LWM2M_MAX_STR_LEN);
        }
        else
        {
            lwm2m_free(firmwareObj);
            firmwareObj = NULL;
        }
    }

    return firmwareObj;
}

void free_object_firmware(lwm2m_object_t * objectP)
{
    if (NULL != objectP->userData)
    {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }
    if (NULL != objectP->instanceList)
    {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }
    lwm2m_free(objectP);
}

uint8_t firmware_change_object(lwm2m_data_t *dataArray, lwm2m_object_t *object)
{
    uint8_t result;

    switch (dataArray->id)
    {
        case RES_M_STATE:
        {
            int64_t value;
            if (1 == lwm2m_data_decode_int(dataArray, &value))
            {
                if ((0 <= value) && (3 >= value))
                {
                    ((firmware_data_t*)(object->userData))->state = (uint8_t)value;
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
        }
        break;

        case RES_M_UPDATE_RESULT:
        {
            int64_t value;
            if (1 == lwm2m_data_decode_int(dataArray, &value))
            {
                if ((0 <= value) && (7 >= value))
                {
                    ((firmware_data_t*)(object->userData))->result = (uint8_t)value;
                    result = COAP_204_CHANGED;
                }
                else
                {
                    result = COAP_400_BAD_REQUEST;
                }
            }
            else
            {
                result = COAP_400_BAD_REQUEST;
            }
        }
        break;

        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
    }

    return result;
}

void prv_firmware_register_callback(lwm2m_object_t * objectP, enum lwm2m_execute_callback_type type,
        lwm2m_exe_callback callback, void *param)
{
    firmware_data_t * data = NULL;

    if (!objectP)
        return;

    data = (firmware_data_t*)(objectP->userData);

    switch(type)
    {
    case LWM2M_EXE_FIRMWARE_UPDATE:
        data->update_callback = callback;
        data->update_callback_param = param;
        break;
    case LWM2M_NOTIFY_RESOURCE_CHANGED:
        data->notify_callback = callback;
        data->notify_callback_param = param;
        break;
    default:
#ifdef WITH_LOGS
        fprintf(stderr, "prv_firmware_register_callback: unsupported callback\r\n");
#endif
        break;
    }
}
