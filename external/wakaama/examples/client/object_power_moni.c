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

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Resource Id's:
#define RES_M_OBJ_ID            0
#define RES_M_OBJINST_ID        1
#define RES_O_STATE             2
#define RES_O_CURRENT           3
#define RES_O_TIME              4

struct power_m_data_s
{
    char state[10];
    int  current;
    int  time;
    int  W;    
};

typedef struct power_m_data_s power_m_data_t;

struct power_monitor_oi_s
{   //linked list:
    struct power_monitor_oi_s*   next;       // matches lwm2m_list_t::next
    uint16_t                	 objInstId;  // matches lwm2m_list_t::id
    // resources
    power_m_data_t       *power_monitor_resource;
};

typedef struct power_monitor_oi_s power_monitor_oi_t;

typedef enum
{
     STATE_NORMAL,
     STATE_IDLE,
     STATE_STANDBY,
     STATE_SLEEP,
} dm_lwm2m_power_state_e;


static const char *power_state_str[] = {"NORMAL", "IDLE", "STANDBY", "SLEEP"};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fetch_state
 *
 * Description:
 *   To get the state (idle, normal, sleep, standby) of a device
 *
 * Returned Value:
 *   
 *
****************************************************************************/
static void fetch_objID(int *value)
{
    (*value) = 8;
}
/****************************************************************************/
static void fetch_objInstId(int *value)
{
    (*value) = 0;
}

/*****************************************************************************/
static void fetch_state(int *value)
{
    (*value) = STATE_NORMAL;
}

/****************************************************************************/

static void fetch_time (int *value)
{
   (*value) = 1000;
}

/****************************************************************************/
static void fetch_current (int *value)
{
   (*value) = 1;
}
/****************************************************************************/
static void fetch_write (int *value)
{

   (*value) = 1;
}
/****************************************************************************/


static uint8_t prv_set_value(lwm2m_data_t * dataP, power_m_data_t *powerDataP)
{
   int value;
   
   switch (dataP->id)
   {

    case RES_O_STATE:
         fetch_state(&value);
         printf("state is %d\n", value); usleep(20000);
        printf("Device power state is %s\n", power_state_str[value]);
        strncpy(powerDataP->state, power_state_str[value], strlen(power_state_str[value]));
         lwm2m_data_encode_string(powerDataP->state, dataP);
         return COAP_205_CONTENT;

    case RES_O_CURRENT :
         fetch_current(&value);
         printf("Current consumption is %d\n", value);
         powerDataP->current = value;
         lwm2m_data_encode_int(powerDataP->current, dataP);
         return COAP_205_CONTENT;

   case RES_O_TIME :
         fetch_time(&value);
         printf("Time spent in the state is %d\n", value);
         powerDataP->time = value;
         lwm2m_data_encode_int(powerDataP->time, dataP);
         return COAP_205_CONTENT;
   } 
}



static uint8_t prv_write_resources(uint16_t instanceId, int numData,
               lwm2m_data_t* tlvArray, lwm2m_object_t* objectP, bool doCreate)
{
    int i;
    uint8_t result;
    int64_t value;

    power_monitor_oi_t* powerMonitorOiP = (power_monitor_oi_t *)
            lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == powerMonitorOiP)
        return COAP_404_NOT_FOUND ;

    i = 0;
    do
    {
      switch (tlvArray[i].id)
      {
        case RES_O_STATE:
          if (doCreate==false)
          {
            result = COAP_405_METHOD_NOT_ALLOWED;
          }
          else
          {
            if (1 != lwm2m_data_decode_int(&tlvArray[i], &value))
            {
              result = COAP_400_BAD_REQUEST;
            }
            else
            {
              // TODO
            }
          }
          break;
          
        case RES_O_CURRENT:
          if (doCreate==false)
          {
            result = COAP_405_METHOD_NOT_ALLOWED;
          }
          else
          {
            if (1 != lwm2m_data_decode_int(&tlvArray[i], &value))
            {
              result = COAP_400_BAD_REQUEST;
            }
            else
            {
              // TODO
            }
          }
          break;
          
        case RES_O_TIME:
          if (doCreate==false)
          {
            result = COAP_405_METHOD_NOT_ALLOWED;
          }
          else
          {
            if (1 != lwm2m_data_decode_int(&tlvArray[i], &value))
            {
              result = COAP_400_BAD_REQUEST;
            }
            else
            {
              // TODO
            }
          }
          break;
        default:
          return COAP_404_NOT_FOUND ;
      }
        i++;
    } while (i < numData && result == COAP_204_CHANGED );

    return result;
}

static uint8_t prv_write(uint16_t instanceId, int numData,
                         lwm2m_data_t* tlvArray, lwm2m_object_t* objectP)
{
    return prv_write_resources(instanceId, numData, tlvArray, objectP, false);
}

static uint8_t prv_delete(uint16_t id, lwm2m_object_t * objectP)
{
    power_monitor_oi_t* targetP;

    objectP->instanceList = lwm2m_list_remove(objectP->instanceList, id,
                                              (lwm2m_list_t**)&targetP);
    if (NULL == targetP) return COAP_404_NOT_FOUND;

//     LWM2M_LIST_FREE(targetP->accCtrlValList);
    lwm2m_free(targetP);

    return COAP_202_DELETED;
}

static uint8_t prv_read(uint16_t instanceId,
                        int * numDataP,
                        lwm2m_data_t ** dataArrayP,
                        lwm2m_object_t * objectP)
{
    uint8_t result;
    int i;

    power_monitor_oi_t* powerMoniOiP =
        (power_monitor_oi_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    printf("instance Id is %d\n", instanceId);
    if (powerMoniOiP == NULL)
    {
        printf("No instance found\n");
        return COAP_404_NOT_FOUND ;
    }

    
    if (*numDataP == 0)
    {
       uint16_t resList[] = {

                RES_O_STATE,
                RES_O_CURRENT,
                RES_O_TIME
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
        result = prv_set_value((*dataArrayP) + i, powerMoniOiP->power_monitor_resource);    
        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);
    return result;
}

static uint8_t prv_create(uint16_t objInstId, int numData,
                          lwm2m_data_t * tlvArray, lwm2m_object_t * objectP)
{
    power_monitor_oi_t * targetP;
    uint8_t result;

    targetP = (power_monitor_oi_t *)lwm2m_malloc(sizeof(power_monitor_oi_t));
    if (NULL == targetP) return COAP_500_INTERNAL_SERVER_ERROR;
    memset(targetP, 0, sizeof(power_monitor_oi_t));

    targetP->objInstId    = objInstId;
    objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, targetP);

    result = prv_write_resources(objInstId, numData, tlvArray, objectP, true);

    if (result != COAP_204_CHANGED)
    {
        prv_delete(objInstId, objectP);
    }
    else
    {
        result = COAP_201_CREATED;
    }
    return result;  
}

lwm2m_object_t * get_power_monitor_object(void)
{
  lwm2m_object_t * powerObj;
  
  powerObj = (lwm2m_object_t *) malloc(sizeof(lwm2m_object_t));
  
  if (powerObj != NULL)
  {
    memset(powerObj, 0x00, sizeof(lwm2m_object_t));
    
    powerObj->objID = LWM2M_POWER_MONITOR_OBJECT_ID;
    
     powerObj->readFunc = prv_read;
    powerObj->writeFunc = prv_write;
    powerObj->createFunc = prv_create;
    powerObj->deleteFunc = prv_delete;
    powerObj->executeFunc = NULL;
  }
  
  return powerObj;
}


bool power_monitor_obj_add_inst (lwm2m_object_t* powerMonitorObjP, uint16_t instId,
                                  int domain_code)
{
    bool ret = false;

    if (NULL == powerMonitorObjP)
    {
        return ret;
    }
    else
    {
        // create an access control object instance
        power_monitor_oi_t* powerMonitorOiP;
        powerMonitorOiP = (power_monitor_oi_t *)lwm2m_malloc(sizeof(power_monitor_oi_t));
        if (NULL == powerMonitorOiP)
        {
            return ret;
        }
        else
        {
            memset(powerMonitorOiP, 0, sizeof(power_monitor_oi_t));
            // list: key
            powerMonitorOiP->objInstId    = instId;
            // object instance data:
            powerMonitorOiP->power_monitor_resource = (power_m_data_t *) 
                                                      lwm2m_malloc(sizeof(power_m_data_t));
            powerMonitorOiP->power_monitor_resource->W     = domain_code;
            
            powerMonitorObjP->instanceList =
                    LWM2M_LIST_ADD(powerMonitorObjP->instanceList, powerMonitorOiP);
            ret = true;
        }
    }
    return ret;
}
#if 0
bool power_monitor_oi_add_ac_val (lwm2m_object_t* powerMonitorObjP, uint16_t instId,
                             uint16_t pmResId, uint16_t pmValue)
{
    bool ret = false;

    power_monitor_oi_t* powerMonitorOiP = (power_monitor_oi_t *)
            lwm2m_list_find(powerMonitorObjP->instanceList, instId);
    if (NULL == powerMonitorOiP)
        return ret;

    return prv_add_ac_val (powerMonitorOiP, pmResId, pmValue);
}
#endif

void power_monitor_free_object(lwm2m_object_t * objectP)
{
    power_monitor_oi_t *powermOiT;
    power_monitor_oi_t *powermOiP = (power_monitor_oi_t*)objectP->instanceList;
    while (powermOiP != NULL)
    {
        // first free acl (multiple resource!):
        powermOiT = powermOiP;
        powermOiP = powermOiP->next;
        if (powermOiT->power_monitor_resource != NULL)
          lwm2m_free(powermOiT->power_monitor_resource);
          
        lwm2m_free(powermOiT);
    }
    lwm2m_free(objectP);
}
/****************************************************************************/



