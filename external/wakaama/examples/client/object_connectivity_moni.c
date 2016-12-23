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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "connectivity_interface.h"

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
#define RES_O_RFCHANNEL                 11
#define RES_O_TX_POWER                  12
#define RES_O_LISTEN_INTERVAL           13
#define RES_O_NOISE                     14
#define RES_O_RATE                      15
#define RES_O_LQI                       16
#define RES_O_WIFI_VERSION              17

#if 0
#define VALUE_NETWORK_BEARER_GSM    0   //GSM see 
#define VALUE_AVL_NETWORK_BEARER_1  0   //GSM
#define VALUE_AVL_NETWORK_BEARER_2  21  //WLAN
#define VALUE_AVL_NETWORK_BEARER_3  41  //Ethernet
#define VALUE_AVL_NETWORK_BEARER_4  42  //DSL
#define VALUE_AVL_NETWORK_BEARER_5  43  //PLC
#define VALUE_IP_ADDRESS_1              "192.168.178.101"
#define VALUE_IP_ADDRESS_2              "192.168.178.102"
#define VALUE_ROUTER_IP_ADDRESS_1       "192.168.178.001"
#define VALUE_ROUTER_IP_ADDRESS_2       "192.168.178.002"
#define VALUE_APN_1                     "web.vodafone.de"
#define VALUE_APN_2                     "cda.vodafone.de"
#define VALUE_CELL_ID                   69696969
#define VALUE_RADIO_SIGNAL_STRENGTH     80                  //dBm
#define VALUE_LINK_QUALITY              98     
#define VALUE_LINK_UTILIZATION          666
#define VALUE_SMNC                      33
#define VALUE_SMCC                      44
#endif



typedef struct
{
#if 0    
    char ipAddresses[2][16];        // limited to 2!
    char routerIpAddresses[2][16];  // limited to 2!
    long cellId;
    int signalStrength;
    int linkQuality;
    int linkUtilization;
    int rfChannel;
    uint8_t txPower;
    int listenInterval;
    
    int noise;
    int rate;
    int lqi;
    char version;
#endif
    char ipAddresses[2][16];        // limited to 2!
    int signalStrength;
    int rate;
    int nwbearer;
    int available_bearer;
} conn_m_data_t;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fetch_network_bearer
 *
 * Description:
 *   To get the underlined network bearer for lwm2m from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_network_bearer(int *value)
{
	(*value)=0;
}
#endif
/****************************************************************************
 * Name: fetch_avl_network_bearer
 *
 * Description:
 *   To get the available underlined network bearer for lwm2m from connectivity manager
 *
 * Returned Value:
 *  
 *
 ****************************************************************************/
#if 0
static void fetch_avl_network_bearer(int *value)
{
	(*value)=0;
}
#endif
/****************************************************************************
 * Name: fetch_rssi
 *
 * Description:
 *   Get signal strength value from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_rssi(int *value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_rssi(value);
    printf("address of rssi is %d\n", value);
#else
    (*value)=get_signal_strength();
#endif
}
#endif
/****************************************************************************
 * Name: fetch_channel
 *
 * Description:
 *   Get RF channel from driver
 *
 * Returned Value:
 *
 *
 ****************************************************************************/
#if 0
static void fetch_channel(int *value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_channel(WWD_STA_INTERFACE, value);
#else
    *value = 0;
#endif

}
#endif
/****************************************************************************
 * Name: fetch_lqi
 *
 * Description:
 *   Get link quality index value from connectivity manager
 *
 * Returned Value:
 * 
 *
 ****************************************************************************/
#if 0
 static void fetch_lqi(int *value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_rssi(value);
#else
    (*value)=get_signal_strength();
#endif
}
#endif

/****************************************************************************
 * Name: fetch_tx_power
 *
 * Description:
 *   Get tx power
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_tx_power(uint8_t *value)
{ 
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_tx_power(value);
#else
    *value = 0;
#endif

}
#endif
/****************************************************************************
 * Name: fetch_listen_interval
 *
 * Description:
 *   Get tx power
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_listen_interval(wiced_listen_interval_t* value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_listen_interval(value);
#else
    *value = 0;
#endif

}
#endif
/****************************************************************************
 * Name: fetch_noise
 *
 * Description:
 *   Get noise
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_noise(int *value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_noise(value);
#else
    *value = 0;
#endif
}
#endif
/****************************************************************************
 * Name: fetch_rate
 *
 * Description:
 *   Get the transmision data rate for provided interface
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0
static void fetch_rate(int *value)
{
#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
    wwd_wifi_get_rate(WWD_STA_INTERFACE,value);
#else
    *value = 0;
#endif
}
#endif

/****************************************************************************
 * Name: fetch_wifi_version
 *
 * Description:
 *   Get the transmision data rate for provided interface
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
//static void fetch_wifi_version(char* value, uint8_t length)
//char fetch_wifi_version (char value, uint8_t length)
//{
//#if defined(CONFIG_WICED) && (CONFIG_ARCH_CHIP_BCM4390X == 1)
//    wwd_wifi_get_wifi_version(value, sizeof(value));
//    printf("wifi version called is %c\n", value);
//#else
//    *value = a;
//#endif
//    return value;
//}

/****************************************************************************
 * Name: fetch_ip_address
 *
 * Description:
 *   Get the assinged IPv4/IPv6 address from Connectivity Manager
 *
 * Returned Value:
 *   
 ****************************************************************************/
#if 0 
static void fetch_ip_address(char *value)
{
	get_ip_address(value);
}
#endif
/****************************************************************************
 * Name: fetch_router_ip_address
 *
 * Description:
 *   Get the Router's IPv4/IPv6 address from Connectivity Manager
 *
 * Returned Value:
 *  
 *
 ****************************************************************************/
#if 0
static void fetch_router_ip_address(char *value)
{
	get_router_ip_address(value);
}
#endif
/****************************************************************************
 * Name: fetch_link_utilization
 *
 * Description:
 *   Get the link utilization statistics from Connectivity Manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0 
static void fetch_link_utilization(int *value)
{
	(*value)=get_link_utilization();
}
#endif
/****************************************************************************
 * Name: fetch_APN
 *
 * Description:
 *   Get APN value from Connectivity Manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0 
static void fetch_APN(char *value)
{
	strcpy(value,"\0");
}
#endif
/****************************************************************************
 * Name: fetch_cell_id
 *
 * Description:
 *  Get cell id value from Connectivity Manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0 
static void fetch_cell_id(int *value)
{
	(*value)=0;
}
#endif
/****************************************************************************
 * Name: fetch_SMNC
 *
 * Description:
 *   
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0 
static void fetch_SMNC(int *value)
{
	(*value)=0;
}
#endif
/****************************************************************************
 * Name: fetch_SMCC
 *
 * Description:
 *  
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
#if 0 
static void fetch_SMCC(int *value)
{
	(*value)=0;
}
#endif
/****************************************************************************
 * Name: fetch_initial_values
 *
 * Description:
 *  Get initialization values from connectivity manager
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
static void fetch_initial_values(conn_m_data_t *myData)
{
#if 0    
            fetch_cell_id(&myData->cellId);
	   fetch_rssi(&myData->signalStrength);
	   fetch_lqi(&myData->linkQuality);
	   fetch_link_utilization(&myData->linkUtilization);
	   fetch_ip_address(*(myData->ipAddresses + 0));		
	   fetch_router_ip_address(*(myData->routerIpAddresses + 0));
           fetch_channel(&myData->rfChannel);
	   fetch_tx_power(&myData->txPower);
	   fetch_listen_interval(&myData->listenInterval);
           fetch_noise(&myData->noise);
	   fetch_rate(&myData->rate);
	   fetch_lqi(&myData->lqi);
#endif
    get_network_bearer(&myData->nwbearer);
    get_avl_network_bearer(&myData->available_bearer);
    get_signal_strength(&myData->signalStrength);
    get_bitrate(&myData->rate);
    get_ip_address(&myData->ipAddresses);
}

/****************************************************************************
 * Name: prv_set_value
 *
 * Description:
 *  
 *
 * Returned Value:
 *   Return the appropriate error code or success code value
 *
 ****************************************************************************/
 
static uint8_t prv_set_value(lwm2m_data_t * dataP,
                             conn_m_data_t * connDataP)
{
    int value;
    uint8_t tx_val;
    char apn[20];	
    switch (dataP->id)
    {
    case RES_M_NETWORK_BEARER:
        get_network_bearer(&value);
        connDataP->nwbearer = value;
        printf("nwbearer is %d\n", value);
        lwm2m_data_encode_int(connDataP->nwbearer, dataP);
        return COAP_205_CONTENT;

    case RES_M_AVL_NETWORK_BEARER:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(riCnt);
        subTlvP[0].id    = 0;
        get_avl_network_bearer(&value);
        connDataP->available_bearer = value;
        lwm2m_data_encode_int(connDataP->available_bearer, subTlvP);
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT ;
    }

    case RES_M_RADIO_SIGNAL_STRENGTH: //s-int
        get_signal_strength(&value);
        connDataP->signalStrength =  value;
        lwm2m_data_encode_int(connDataP->signalStrength, dataP);
        return COAP_205_CONTENT;

    case RES_O_LINK_QUALITY: //s-int
        /* TODO */
#if 0        
	connDataP->linkQuality=value;
        lwm2m_data_encode_int(connDataP->linkQuality, dataP);
#endif
        return COAP_205_CONTENT ;

    case RES_M_IP_ADDRESSES:
    {
        int ri, riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t* subTlvP = lwm2m_data_new(riCnt);
        for (ri = 0; ri < riCnt; ri++)
        {
            subTlvP[ri].id = ri;
	   get_ip_address(*(connDataP->ipAddresses + ri));
            lwm2m_data_encode_string(connDataP->ipAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
    {
        /* TODO */
#if 0        
        int ri, riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t* subTlvP = lwm2m_data_new(riCnt);
        for (ri=0; ri<riCnt; ri++)
        {
            subTlvP[ri].id = ri;
            /* TODO */
            lwm2m_data_encode_string(connDataP->routerIpAddresses[ri], subTlvP + ri);
        }
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
#endif
        return COAP_205_CONTENT ;
    }
        break;

    case RES_O_LINK_UTILIZATION:
	/* TODO */
#if 0
        lwm2m_data_encode_int(connDataP->linkUtilization, dataP);
#endif
        return COAP_205_CONTENT;

    case RES_O_APN:
    {
        int riCnt = 1;   // reduced to 1 instance to fit in one block size
        lwm2m_data_t * subTlvP;
        subTlvP = lwm2m_data_new(riCnt);
        subTlvP[0].id     = 0;
	/* Not used */
        lwm2m_data_encode_string(apn, subTlvP);
        lwm2m_data_encode_instances(subTlvP, riCnt, dataP);
        return COAP_205_CONTENT;
    }
        break;

    case RES_O_CELL_ID:
        /* Not used */
        return COAP_205_CONTENT ;

    case RES_O_SMNC:
        /* Not used */
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT ;

    case RES_O_SMCC:
        /* Not used */
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT ;

    case RES_O_RFCHANNEL:
        /* TODO */
        printf("channel: %d\n", value);
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT;
    
    case RES_O_TX_POWER:
        /* TODO */
        return COAP_205_CONTENT;

   case RES_O_LISTEN_INTERVAL:
{
        /* TODO */
#if 0
	wiced_listen_interval_t interval_value;
        fetch_listen_interval(&interval_value);
#endif  
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT;
}
 
   case RES_O_NOISE:
        /* TODO */
        printf("Noise: %d dbm\n", value);
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT;

   case RES_O_RATE:
        get_bitrate(&value);
        printf("Rate: %d kbps\n ", value);
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT;

   case RES_O_LQI:
	/* TODO */
        if (value < -100)
            value = 0;
        else{
            if (value > -50)
               value = 100;
            else{
               value = 2*(value+100);              
            }
        }
        printf("LQI: %d (%)\n", value);
        lwm2m_data_encode_int(value, dataP);
        return COAP_205_CONTENT;

    default:
        return COAP_404_NOT_FOUND ;
    }
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
#if 0
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
                RES_O_SMCC,
                RES_O_RFCHANNEL
#endif
                RES_M_NETWORK_BEARER,
                RES_M_AVL_NETWORK_BEARER,
                RES_M_RADIO_SIGNAL_STRENGTH,
                RES_M_IP_ADDRESSES,
                RES_O_RATE
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

lwm2m_object_t * get_object_conn_m(void)
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
        conn_m_data_t *myData = (conn_m_data_t*) connObj->userData;
#if 0        
            myData->cellId          = VALUE_CELL_ID;
            myData->signalStrength  = VALUE_RADIO_SIGNAL_STRENGTH;
            myData->linkQuality     = VALUE_LINK_QUALITY;
            myData->linkUtilization = VALUE_LINK_UTILIZATION;
            strcpy(myData->ipAddresses[0],       VALUE_IP_ADDRESS_1);
            strcpy(myData->ipAddresses[1],       VALUE_IP_ADDRESS_2);
            strcpy(myData->routerIpAddresses[0], VALUE_ROUTER_IP_ADDRESS_1);
            strcpy(myData->routerIpAddresses[1], VALUE_ROUTER_IP_ADDRESS_2);
#else	   
	   fetch_initial_values(myData);
#endif
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

uint8_t connectivity_moni_change(lwm2m_data_t * dataArray,
                                 lwm2m_object_t * objectP)
{
    int64_t value;
    uint8_t result;
    conn_m_data_t * data;

    data = (conn_m_data_t*) (objectP->userData);

    switch (dataArray->id)
    {
    case RES_M_RADIO_SIGNAL_STRENGTH:
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->signalStrength = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
        break;

    case RES_O_LINK_QUALITY:
#if 0
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->linkQuality = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
#endif
        break;

    case RES_M_IP_ADDRESSES:
        if (sizeof(data->ipAddresses[0]) <= dataArray->value.asBuffer.length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->ipAddresses[0], 0, sizeof(data->ipAddresses[0]));
            memcpy(data->ipAddresses[0], dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length);
            data->ipAddresses[0][dataArray->value.asBuffer.length] = 0;
            result = COAP_204_CHANGED;
        }
        break;

    case RES_O_ROUTER_IP_ADDRESS:
#if 0
        if (sizeof(data->routerIpAddresses[0]) <= dataArray->value.asBuffer.length)
        {
            result = COAP_400_BAD_REQUEST;
        }
        else
        {
            memset(data->routerIpAddresses[0], 0, sizeof(data->routerIpAddresses[0]));
            memcpy(data->routerIpAddresses[0], dataArray->value.asBuffer.buffer, dataArray->value.asBuffer.length);
            data->routerIpAddresses[0][dataArray->value.asBuffer.length] = 0;
            result = COAP_204_CHANGED;
        }
#endif
        break;

    case RES_O_CELL_ID:
#if 0        
        if (1 == lwm2m_data_decode_int(dataArray, &value))
        {
            data->cellId = value;
            result = COAP_204_CHANGED;
        }
        else
        {
            result = COAP_400_BAD_REQUEST;
        }
#endif
        break;

    default:
        result = COAP_405_METHOD_NOT_ALLOWED;
    }

    return result;
}
