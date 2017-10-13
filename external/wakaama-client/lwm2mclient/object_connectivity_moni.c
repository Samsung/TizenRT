/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH Germany. 
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
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *    
 *******************************************************************************/

/*
 *  This Connectivity Monitoring object is optional and has a single instance
 * 
 *  Resources:
 *
 *          Name             | ID | Oper. | Inst. | Mand.|  Type   | Range | Units |
 *  Network Bearer           |  0 |  R    | Single|  Yes | Integer |       |       |
 *  Available Network Bearer |  1 |  R    | Multi |  Yes | Integer |       |       |
 *  Radio Signal Strength    |  2 |  R    | Single|  Yes | Integer |       | dBm   |
 *  Link Quality             |  3 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  IP Addresses             |  4 |  R    | Multi |  Yes | String  |       |       |
 *  Router IP Addresses      |  5 |  R    | Multi |  No  | String  |       |       |
 *  Link Utilization         |  6 |  R    | Single|  No  | Integer | 0-100 |   %   |
 *  APN                      |  7 |  R    | Multi |  No  | String  |       |       |
 *  Cell ID                  |  8 |  R    | Single|  No  | Integer |       |       |
 *  SMNC                     |  9 |  R    | Single|  No  | Integer | 0-999 |   %   |
 *  SMCC                     | 10 |  R    | Single|  No  | Integer | 0-999 |       |
 *
 */

#include "liblwm2m.h"
#include "lwm2mclient.h"
#include "internals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Resource Id's:
#define RES_M_NETWORK_BEARER            0
#define RES_M_AVL_NETWORK_BEARER        1
#define RES_M_RADIO_SIGNAL_STRENGTH     2
#define RES_O_LINK_QUALITY              3
#define RES_M_IP_ADDRESSES              4
#define RES_O_ROUTER_IP_ADDRESS         5
#define RES_O_LINK_UTILIZATION          6
#define RES_O_APN                       7
#define RES_O_CELL_ID                   8
#define RES_O_SMNC                      9
#define RES_O_SMCC                      10

#define IPV46_MAX_ADDR_LEN              45
#define TLV_IP_ADDRS_NUM                2
#define TLV_BEARERS_NUM                 1
#define TLV_APNS_NUM                    1

typedef struct
{
    char ipAddresses[TLV_IP_ADDRS_NUM][IPV46_MAX_ADDR_LEN];
    char routerIpAddresses[TLV_IP_ADDRS_NUM][IPV46_MAX_ADDR_LEN];
    long cellId;
    int signalStrength;
    int linkQuality;
    int linkUtilization;
    object_conn_monitoring_t *obj;
} conn_m_data_t;

static uint8_t prv_set_value(lwm2m_data_t * dataP,
                             conn_m_data_t * connDataP)
{
    uint8_t result = COAP_400_BAD_REQUEST;
    object_conn_monitoring_t *obj = connDataP->obj;

    switch (dataP->id)
    {
    case RES_M_NETWORK_BEARER:
        lwm2m_data_encode_int(obj->network_bearer, dataP);
        result =  COAP_205_CONTENT;
        break;

    case RES_M_AVL_NETWORK_BEARER:
    {
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(TLV_BEARERS_NUM);
        subTlvP[0].id = 0;
        lwm2m_data_encode_int(obj->avl_network_bearer, subTlvP);
        lwm2m_data_encode_instances(subTlvP, TLV_BEARERS_NUM, dataP);
        result = COAP_205_CONTENT;
        break;
    }

    case RES_M_RADIO_SIGNAL_STRENGTH:
        lwm2m_data_encode_int(connDataP->signalStrength, dataP);
        result = COAP_205_CONTENT;
        break;

    case RES_O_LINK_QUALITY:
        lwm2m_data_encode_int(connDataP->linkQuality, dataP);
        result = COAP_205_CONTENT;
        break;

    case RES_M_IP_ADDRESSES:
    {
        int ri;
        lwm2m_data_t* subTlvP = lwm2m_data_new(TLV_IP_ADDRS_NUM);
        for (ri = 0; ri<TLV_IP_ADDRS_NUM; ri++)
        {
            subTlvP[ri].id = ri;
            lwm2m_data_encode_string(connDataP->ipAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, TLV_IP_ADDRS_NUM, dataP);
        result = COAP_205_CONTENT;
        break;
    }

    case RES_O_ROUTER_IP_ADDRESS:
    {
        int ri;
        lwm2m_data_t* subTlvP = lwm2m_data_new(TLV_IP_ADDRS_NUM);
        for (ri=0; ri<TLV_IP_ADDRS_NUM; ri++)
        {
            subTlvP[ri].id = ri;
            lwm2m_data_encode_string(connDataP->routerIpAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, TLV_IP_ADDRS_NUM, dataP);
        result = COAP_205_CONTENT;
        break;
    }

    case RES_O_LINK_UTILIZATION:
        lwm2m_data_encode_int(connDataP->linkUtilization, dataP);
        result = COAP_205_CONTENT;
        break;

    case RES_O_APN:
    {
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(TLV_APNS_NUM);
        subTlvP[0].id = 0;
        lwm2m_data_encode_string(obj->apn, subTlvP);
        lwm2m_data_encode_instances(subTlvP, TLV_APNS_NUM, dataP);
        result = COAP_205_CONTENT;
        break;
    }

    case RES_O_CELL_ID:
        lwm2m_data_encode_int(connDataP->cellId, dataP);
        result = COAP_205_CONTENT;
        break;

    case RES_O_SMNC:
        lwm2m_data_encode_int(obj->smnc, dataP);
        result = COAP_205_CONTENT;
        break;

    case RES_O_SMCC:
        lwm2m_data_encode_int(obj->smcc, dataP);
        result = COAP_205_CONTENT;
        break;

    default:
        result = COAP_404_NOT_FOUND;
        break;
    }

    return result;
}

static uint8_t prv_read(uint16_t instanceId,
                        int * numDataP,
                        lwm2m_data_t ** dataArrayP,
                        lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    // this is a single instance object
    if (instanceId != 0)
    {
        return COAP_404_NOT_FOUND ;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0)
    {
        uint16_t resList[] = {
                RES_M_NETWORK_BEARER,
                RES_M_AVL_NETWORK_BEARER,
                RES_M_RADIO_SIGNAL_STRENGTH,
                RES_O_LINK_QUALITY,
                RES_M_IP_ADDRESSES,
                RES_O_ROUTER_IP_ADDRESS,
                RES_O_LINK_UTILIZATION,
                RES_O_APN,
                RES_O_CELL_ID,
                RES_O_SMNC,
                RES_O_SMCC
        };
        int nbRes = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR ;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++)
        {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do
    {
        result = prv_set_value((*dataArrayP) + i, (conn_m_data_t*) (objectP->userData));
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT );

    return result;
}

lwm2m_object_t * get_object_conn_m(object_conn_monitoring_t *default_value)
{
    /*
     * The get_object_conn_m() function create the object itself and return a pointer to the structure that represent it.
     */
    lwm2m_object_t * connObj;

    connObj = (lwm2m_object_t *) lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != connObj)
    {
        memset(connObj, 0, sizeof(lwm2m_object_t));

        /*
         * It assigns his unique ID
         */
        connObj->objID = LWM2M_CONN_MONITOR_OBJECT_ID;
        
        /*
         * and its unique instance
         *
         */
        connObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != connObj->instanceList)
        {
            memset(connObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(connObj);
            return NULL;
        }
        
        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
         * know the resources of the object, only the server does.
         */
        connObj->readFunc = prv_read;
        connObj->executeFunc = NULL;
        connObj->userData = lwm2m_malloc(sizeof(conn_m_data_t));

        /*
         * Also some user data can be stored in the object with a private structure containing the needed variables
         */
        if (NULL != connObj->userData)
        {
            conn_m_data_t *conn_data = (conn_m_data_t*) connObj->userData;
            conn_data->obj = default_value;
            conn_data->cellId = default_value->cell_id;
            conn_data->signalStrength = default_value->radio_signal_strength;
            conn_data->linkQuality = default_value->link_quality;
            conn_data->linkUtilization = default_value->link_utilization;
            strncpy(conn_data->ipAddresses[0], default_value->ip_addr, IPV46_MAX_ADDR_LEN);
            strncpy(conn_data->ipAddresses[1], default_value->ip_addr2, IPV46_MAX_ADDR_LEN);
            strncpy(conn_data->routerIpAddresses[0], default_value->router_ip_addr, IPV46_MAX_ADDR_LEN);
            strncpy(conn_data->routerIpAddresses[1], default_value->router_ip_addr2, IPV46_MAX_ADDR_LEN);
        }
        else
        {
            lwm2m_free(connObj);
            connObj = NULL;
        }
    }
    return connObj;
}

void free_object_conn_m(lwm2m_object_t * objectP)
{
    lwm2m_free(objectP->userData);
    lwm2m_list_free(objectP->instanceList);
    lwm2m_free(objectP);
}

uint8_t connectivity_moni_change(lwm2m_data_t *dataArray, lwm2m_object_t *object)
{
    int64_t value;
    uint8_t result = COAP_400_BAD_REQUEST;
    conn_m_data_t *data = (conn_m_data_t*)object->userData;
    object_conn_monitoring_t *obj = data->obj;

    switch (dataArray->id)
    {
    case RES_M_NETWORK_BEARER:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            if ((0 <= value) && (50 >= value))
            {
                obj->network_bearer = (int)value;
                result = COAP_204_CHANGED;
            }
        }
        break;

    case RES_M_AVL_NETWORK_BEARER:
    {
        int num = 0, i = 0;
        lwm2m_data_t *parsed = NULL;

        num = tlv_parse(dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length, &parsed);
        if ((num > TLV_BEARERS_NUM) || (num == 0))
        {
            break;
        }

        for (i=0; i<num; i++)
        {
            if (1 == lwm2m_data_decode_int(parsed, &value))
            {
                data->obj->avl_network_bearer = (int)value;
                result = COAP_204_CHANGED;
            }
        }
        lwm2m_free(parsed);
        break;
    }
    case RES_M_RADIO_SIGNAL_STRENGTH:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->signalStrength = (int)value;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_LINK_QUALITY:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->linkQuality = (int)value;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_M_IP_ADDRESSES:
    {
        int num = 0, i = 0;
        lwm2m_data_t *parsed = NULL;

        num = tlv_parse(dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length, &parsed);
        if ((num > TLV_IP_ADDRS_NUM) || (num == 0))
        {
            break;
        }

        for (i=0; i<num; i++)
        {
            if (parsed[i].value.asBuffer.length > IPV46_MAX_ADDR_LEN)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "connectivity_moni_change: passed string is too big to be an IP address\r\n");
#endif
                continue;
            }

            memset(data->ipAddresses[i], 0, IPV46_MAX_ADDR_LEN);
            memcpy(data->ipAddresses[i], parsed[i].value.asBuffer.buffer, parsed[i].value.asBuffer.length);
            result = COAP_204_CHANGED;
        }
        lwm2m_free(parsed);
        break;
    }

    case RES_O_ROUTER_IP_ADDRESS:
    {
        int num = 0, i = 0;
        lwm2m_data_t *parsed = NULL;

        num = tlv_parse(dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length, &parsed);
        if ((num > TLV_IP_ADDRS_NUM) || (num == 0))
        {
            break;
        }

        for (i=0; i<num; i++)
        {
            if (parsed[i].value.asBuffer.length > IPV46_MAX_ADDR_LEN)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "connectivity_moni_change: passed string is too big to be an IP address\r\n");
#endif
                continue;
            }

            memset(data->routerIpAddresses[i], 0, IPV46_MAX_ADDR_LEN);
            memcpy(data->routerIpAddresses[i], parsed[i].value.asBuffer.buffer, parsed[i].value.asBuffer.length);
            lwm2m_free(parsed[i].value.asBuffer.buffer);
            result = COAP_204_CHANGED;
        }
        lwm2m_free(parsed);
        break;
    }

    case RES_O_LINK_UTILIZATION:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            if ((0 <= value) && (100 >= value))
            {
                data->linkUtilization = (int)value;
                result = COAP_204_CHANGED;
            }
        }
        break;

    case RES_O_APN:
    {
        int num = 0, i = 0;
        lwm2m_data_t *parsed = NULL;

        num = tlv_parse(dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length, &parsed);
        if ((num > TLV_APNS_NUM) || (num == 0))
        {
            break;
        }

        for (i=0; i<num; i++)
        {
            if (parsed[i].value.asBuffer.length > LWM2M_MAX_STR_LEN)
            {
#ifdef WITH_LOGS
                fprintf(stderr, "connectivity_moni_change: passed string is too big to be an IP address\r\n");
#endif
                continue;
            }

            memset(data->obj->apn, 0, LWM2M_MAX_STR_LEN);
            memcpy(data->obj->apn, parsed[i].value.asBuffer.buffer, parsed[i].value.asBuffer.length);
            data->obj->apn[parsed[i].value.asBuffer.length] = '\0';
            lwm2m_free(parsed[i].value.asBuffer.buffer);
            result = COAP_204_CHANGED;
        }
        lwm2m_free(parsed);
        break;
    }

    case RES_O_CELL_ID:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->cellId = (long)value;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_SMNC:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            obj->smnc = (int)value;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_SMCC:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            obj->smcc = (int)value;
            result = COAP_204_CHANGED;
        }
        break;

    default:
        result = COAP_405_METHOD_NOT_ALLOWED;
    }

    return result;
}

