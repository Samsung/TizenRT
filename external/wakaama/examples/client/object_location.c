/*******************************************************************************
 * 
 * Copyright (c) 2014 Bosch Software Innovations GmbH, Germany.
 *
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
 ******************************************************************************/
/*! \file
  LWM2M object "Location" implementation

  \author Joerg Hubschneider
*/

/*
 *  Object     |      | Multiple  |     | Description                   |
 *  Name       |  ID  | Instances |Mand.|                               |
 *-------------+------+-----------+-----+-------------------------------+
 *  Location   |   6  |    No     |  No |  see TS E.7 page 101          |
 *
 *  Resources:
 *  Name        | ID  | Oper.|Instances|Mand.|  Type   | Range | Units | Description                                                                      |
 * -------------+-----+------+---------+-----+---------+-------+-------+----------------------------------------------------------------------------------+
 *  Latitude    |  0  |  R   | Single  | Yes | String  |       |  Deg  | The decimal notation of latitude  e.g. -  45.5723  [Worlds Geodetic System 1984].|
 *  Longitude   |  1  |  R   | Single  | Yes | String  |       |  Deg  | The decimal notation of longitude e.g. - 153.21760 [Worlds Geodetic System 1984].|
 *  Altidude    |  2  |  R   | Single  | No  | String  |       |   m   | The decimal notation of altidude in meters above sea level.                      |
 *  Uncertainty |  3  |  R   | Single  | No  | String  |       |   m   | The accuracy of the position in meters.                                          |
 *  Velocity    |  4  |  R   | Single  | No  | Opaque  |       |   *   | The velocity of the device as defined in 3GPP 23.032 GAD specification(*).       |
 *              |     |      |         |     |         |       |       | This set of values may not be available if the device is static.                 |
 *              |     |      |         |     |         |       |       | opaque: see OMA_TS 6.3.2                                                         |
 *  Timestamp   |  5  |  R   | Single  | Yes | Time    |       |   s   | The timestamp when the location measurement was performed.                       |
 */

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef LWM2M_CLIENT_MODE


// ---- private "object location" specific defines ----
// Resource Id's:
#define RES_M_LATITUDE     0
#define RES_M_LONGITUDE    1
#define RES_O_ALTITUDE     2
#define RES_O_UNCERTAINTY  3
#define RES_O_VELOCITY     4
#define RES_M_TIMESTAMP    5

//-----  3GPP TS 23.032 V11.0.0(2012-09) ---------
#define HORIZONTAL_VELOCITY                  0  // for Octet-1 upper half(..<<4)
#define HORIZONTAL_VELOCITY_VERTICAL         1  // set vertical direction bit!
#define HORIZONTAL_VELOCITY_WITH_UNCERTAINTY 2

#define VELOCITY_OCTETS                      5  // for HORITZOL_VELOCITY_WITH_UNCERTAINTY 
#define DEG_DECIMAL_PLACES                   6  // configuration: degree decimals implementation

typedef struct
{
    char     latitude   [5 + DEG_DECIMAL_PLACES]; //"359.12345" frag=5, 9+1=10! degrees +\0
    char     longitude  [5 + DEG_DECIMAL_PLACES];
    char     altitude   [5 + DEG_DECIMAL_PLACES];
    char     uncertainty[5 + DEG_DECIMAL_PLACES];
    uint8_t  velocity   [VELOCITY_OCTETS];        //3GPP notation 1st step: HORITZOL_VELOCITY_WITH_UNCERTAINTY
    unsigned long timestamp;
} location_data_t;

/**
implementation for all read-able resources
*/
static uint8_t prv_res2tlv(lwm2m_data_t* dataP,
                           location_data_t* locDataP)
{
    //-------------------------------------------------------------------- JH --
    uint8_t ret = COAP_205_CONTENT;  
    switch (dataP->id)     // location resourceId
    {
    case RES_M_LATITUDE:
        lwm2m_data_encode_string(locDataP->latitude, dataP);
        break;
    case RES_M_LONGITUDE:
        lwm2m_data_encode_string(locDataP->longitude, dataP);
        break;
    case RES_O_ALTITUDE:
        lwm2m_data_encode_string(locDataP->altitude, dataP);
        break;
    case RES_O_UNCERTAINTY:
        lwm2m_data_encode_string(locDataP->uncertainty, dataP);
        break;
    case RES_O_VELOCITY:
        lwm2m_data_encode_string(locDataP->velocity, dataP);
        break;
    case RES_M_TIMESTAMP:
        lwm2m_data_encode_int(locDataP->timestamp, dataP);
        break;
    default:
        ret = COAP_404_NOT_FOUND;
        break;
    }
  
    return ret;
}


/**
  * Implementation (callback-) function of reading object resources. For whole
  * object, single resources or a sequence of resources
  * see 3GPP TS 23.032 V11.0.0(2012-09) page 23,24.
  * implemented for: HORIZONTAL_VELOCITY_WITH_UNCERTAINT
  * @param objInstId    in,     instances ID of the location object to read
  * @param numDataP     in/out, pointer to the number of resource to read. 0 is the
  *                             exception for all readable resource of object instance
  * @param tlvArrayP    in/out, TLV data sequence with initialized resource ID to read
  * @param objectP      in,     private location data structure
  */
static uint8_t prv_location_read(uint16_t objInstId,
                                 int*  numDataP,
                                 lwm2m_data_t** tlvArrayP,
                                 lwm2m_object_t*  objectP)
{   
    //-------------------------------------------------------------------- JH --
    int     i;
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    location_data_t* locDataP = (location_data_t*)(objectP->userData);

    // defined as single instance object!
    if (objInstId != 0) return COAP_404_NOT_FOUND;

    if (*numDataP == 0)     // full object, readable resources!
    {
        uint16_t readResIds[] = {
                RES_M_LATITUDE,
                RES_M_LONGITUDE,
                RES_O_ALTITUDE,
                RES_O_UNCERTAINTY,
                RES_O_VELOCITY,
                RES_M_TIMESTAMP
        }; // readable resources!
        
        *numDataP  = sizeof(readResIds)/sizeof(uint16_t);
        *tlvArrayP = lwm2m_data_new(*numDataP);
        if (*tlvArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        
        // init readable resource id's
        for (i = 0 ; i < *numDataP ; i++)
        {
            (*tlvArrayP)[i].id = readResIds[i];
        }
    }
    
    for (i = 0 ; i < *numDataP ; i++)
    {
        result = prv_res2tlv ((*tlvArrayP)+i, locDataP);
        if (result!=COAP_205_CONTENT) break;
    }
    
    return result;
}

void display_location_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
    location_data_t * data = (location_data_t *)object->userData;
    fprintf(stdout, "  /%u: Location object:\r\n", object->objID);
    if (NULL != data)
    {
        fprintf(stdout, "    latitude: %s, longitude: %s, altitude: %s, uncertainty: %s, timestamp: %lu\r\n",
                data->latitude, data->longitude, data->altitude, data->uncertainty, data->timestamp);
    }
#endif
}

/**
  * Convenience function to set the velocity attributes.
  * see 3GPP TS 23.032 V11.0.0(2012-09) page 23,24.
  * implemented for: HORIZONTAL_VELOCITY_WITH_UNCERTAINTY
  * @param locationObj location object reference (to be casted!)
  * @param bearing          [Deg]  0 - 359    resolution: 1 degree
  * @param horizontalSpeed  [km/h] 1 - s^16-1 resolution: 1 km/h steps
  * @param speedUncertainty [km/h] 1-254      resolution: 1 km/h (255=undefined!)
  */
void location_setVelocity(lwm2m_object_t* locationObj,
                          uint16_t bearing,
                          uint16_t horizontalSpeed,
                          uint8_t speedUncertainty)
{
    //-------------------------------------------------------------------- JH --
    location_data_t* pData = locationObj->userData;
    pData->velocity[0] = HORIZONTAL_VELOCITY_WITH_UNCERTAINTY << 4;
    pData->velocity[0] = (bearing & 0x100) >> 8;
    pData->velocity[1] = (bearing & 0x0FF);
    pData->velocity[2] = horizontalSpeed >> 8;
    pData->velocity[3] = horizontalSpeed & 0xff;
    pData->velocity[4] = speedUncertainty;
}

/**
  * A convenience function to set the location coordinates with its timestamp.
  * @see testMe()
  * @param locationObj location object reference (to be casted!)
  * @param latitude  the second argument.
  * @param longitude the second argument.
  * @param altitude  the second argument.
  * @param timestamp the related timestamp. Seconds since 1970.
  */
void location_setLocationAtTime(lwm2m_object_t* locationObj,
                             float latitude,
                             float longitude,
                             float altitude,
                             uint64_t timestamp)
{
    //-------------------------------------------------------------------- JH --
    location_data_t* pData = locationObj->userData;

#if defined(ARDUINO)
    dtostrf (latitude,  -8, 6, pData->latitude);
    dtostrf (longitude, -8, 6, pData->longitude);
    dtostrf (altitude,  -8, 4, pData->altitude);
#else
    snprintf(pData->latitude, 5+DEG_DECIMAL_PLACES, "%-8.6f", (double)latitude);
    snprintf(pData->longitude,5+DEG_DECIMAL_PLACES, "%-8.6f", (double)longitude);
    snprintf(pData->altitude, 5+DEG_DECIMAL_PLACES, "%-8.4f", (double)altitude);
#endif

    pData->timestamp = timestamp;
}

/**
  * This function creates the LWM2M Location. 
  * @return gives back allocated LWM2M data object structure pointer. On error, 
  * NULL value is returned.
  */
lwm2m_object_t * get_object_location(void)
{
    //-------------------------------------------------------------------- JH --
    lwm2m_object_t * locationObj;

    locationObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));
    if (NULL != locationObj)
    {
        memset(locationObj, 0, sizeof(lwm2m_object_t));

        // It assigns its unique ID
        // The 6 is the standard ID for the optional object "Location".
        locationObj->objID = LWM2M_LOCATION_OBJECT_ID;
        
        // and its unique instance
        locationObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != locationObj->instanceList)
        {
            memset(locationObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else
        {
            lwm2m_free(locationObj);
            return NULL;
        }

        // And the private function that will access the object.
        // Those function will be called when a read query is made by the server.
        // In fact the library don't need to know the resources of the object, only the server does.
        //
        locationObj->readFunc    = prv_location_read;
        locationObj->userData    = lwm2m_malloc(sizeof(location_data_t));

        // initialize private data structure containing the needed variables
        if (NULL != locationObj->userData)
        {
            location_data_t* data = (location_data_t*)locationObj->userData;
            strcpy (data->latitude,     "27.986065");  // Mount Everest :)
            strcpy (data->longitude,    "86.922623");
            strcpy (data->altitude,     "8495.0000");
            strcpy (data->uncertainty,  "0.01");
            location_setVelocity(locationObj, 0, 0, 255); // 255: speedUncertainty not supported!
            data->timestamp   = time(NULL);
        }
        else
        {
            lwm2m_free(locationObj);
            locationObj = NULL;
        }
    }
    
    return locationObj;
}

void free_object_location(lwm2m_object_t * object)
{
    lwm2m_list_free(object->instanceList);
    lwm2m_free(object->userData);
    lwm2m_free(object);
}

#endif  //LWM2M_CLIENT_MODE
