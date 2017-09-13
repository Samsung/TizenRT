/*
 * cloud_ping.c
 *
 *  Created on: 2017. 1. 16.
 *      Author: eunseok
 */

#define OICPING_ENABLE      1

#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "ocpayload.h"
#include "octypes.h"
#include "escommon.h"
#include "thingsmalloc.h"
#include "thingslogger.h"
#include "oiclist.h"
#include "oicping.h"

#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif

#ifdef OICPING_ENABLE
#include "oickeepalive.h"
#endif
#include "resourcehandler.h"
#include "cloud_manager.h"

#include "oicthread.h"

#define TAG "[oicping]"

#define DEFAULT_CONTEXT_VALUE   0x99
#define PING_UNIT           60      // 60 seconds is minimum value of ping.
#define NET_TROUBLE_DELAY   5       // 5 seconds
#define PING_TIMEOUT        60      // unit : seconds
#define MAX_INTERVAL_EA     4       // Cloud Support 4 EA Interval class.
#define MAX_INTERVAL_VAL    60      // max Interval value 60 minute

#define KEEPALIVE_RESOURCE_URI "/oic/ping"

typedef void *(*pthread_FuncType) (void *);
typedef struct oicping_s
{
    pthread_mutex_t mutexState;             // mutex for State variable.
    PING_STATE bitMaskState;                // State of Ping

    pthread_mutex_t mutexThread;            // mutex for handleThread variable.
    pthread_t  handleThread;                // Ping-Thread Handler
    bool       continuThread;               // Continue Ping-Thread Flag.
    char*      addr;                        // IP address of remote
    uint16_t   port;                        // Port number for remote

    pthread_mutex_t mutexInt;               // mutex for interval variable.
    int64_t    interval[MAX_INTERVAL_EA];   // Interval EA  (unit : minute , max : 60)
    int        intseq;                      // sequence of interval
} oicping_s;

static List* list = NULL;
static const char INTERVAL_ARRAY[] = "inarray";
static const char INTERVAL[] = "in";
oicping_s* pPing = NULL;		//SKKIM for ping command

static void* tCIPingLoop(oicping_s* ping);
static OCStackApplicationResult handlerDiscoverPingResource(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse);
static OCStackApplicationResult handlerPingRequest(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse);
static OCStackApplicationResult handlerPingUpdateRequest(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse);

static oicping_s* createOICping_s(const char* remoteAddr, const uint16_t port);
static void terminateOICping_s(oicping_s* ping);
static int isStateKeyEqual(oicping_s* ping, const PING_STATE value);
static int isIPKeyEqual(oicping_s* ping, const char* targetIP);
static void checkPingThread(oicping_s* ping);
static void SetMask(oicping_s* ping, PING_STATE state);
static bool GetMask(oicping_s* ping, PING_STATE state);
static bool CASMask(oicping_s* ping, PING_STATE preState, bool preStateThen, PING_STATE postState);
static void UNSetMask(oicping_s* ping, PING_STATE state);
static int FindResourceOICPing(oicping_s* ping);
static int SendOICPingRequest(oicping_s* ping);
static bool SetInterval_SendUpdate(oicping_s* ping, int64_t* intervals, int intEA);
static bool SetInterval(oicping_s* ping, int64_t* intervals, int intEA, PING_STATE confirmState);
static bool SetDefInterval(oicping_s* ping);
static bool OICPingCreateThread(oicping_s* ping);
static bool OICPingDestroyThread(oicping_s* ping);
static void OCRepPayloadValuePrint( OCRepPayloadValue* value );
static char* MakeHostAddr(const char* ip, const uint16_t port);
static OCStackResult SendKeepAliveUpdateRequest(const char *host, int64_t* intervals, int intEA, OCCallbackData* cb );

#ifdef OICPING_ENABLE
bool OICPingInit(void)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;

    if( list == NULL )
    {
        if( (list = createList()) == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
            return res;
        }

        res = true;
    }
    else
        res = true;

	pPing = NULL;
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}

bool OICPingTerminate(void)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    oicping_s* ping = NULL;

    if( list )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "oicping_s terminate.");
        while( (ping = (oicping_s*)list->Pop(list)) != NULL )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate ping.(%s)", ping->addr);
            if( CASMask(ping, PING_ST_STARTTHREAD, true, PING_ST_INIT) == true)
            {
                OICPingDestroyThread(ping);
            }
            terminateOICping_s(ping);
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate ping-list.");
        terminateList(list);
        list = NULL;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return true;
}

bool OICPingSetMask(const char* remoteAddr, uint16_t port, PING_STATE state)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    OicNode* node = NULL;
    oicping_s* ping = NULL;

    if( remoteAddr == NULL || strlen(remoteAddr) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(remoteAddr=%s, state=%d)", remoteAddr, state);
        return false;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return false;
    }

    node = list->FindByKey(list, (Key_Compare)isIPKeyEqual, remoteAddr);
    if( node == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s). So, Create Node.", remoteAddr);
        if( (ping = createOICping_s(remoteAddr, port)) == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
            return false;
        }
        list->Insert(list, (void*)ping);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remoteAddr);
        ping = node->item;
    }

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist oicping_s in remoteAddr_Node.(%s)", remoteAddr);
        return false;
    }

    if( port != 0 )
        ping->port = port;

    SetMask(ping, state);
    checkPingThread(ping);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return true;
}

int OICPingGetMask(const char* remoteAddr, PING_STATE state)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
    OicNode* node = NULL;
    oicping_s* ping = NULL;

    if( remoteAddr == NULL || strlen(remoteAddr) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(remoteAddr=%s, state=%d)", remoteAddr, state);
        return -1;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return -1;
    }

    node = list->FindByKey(list, (Key_Compare)isIPKeyEqual, remoteAddr);
    if( node == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s).", remoteAddr);
        return -1;
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remoteAddr);
        ping = node->item;
    }

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist oicping_s in remoteAddr_Node.(%s)", remoteAddr);
        return false;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return (int)GetMask(ping, state);
}

bool OICPingUNSetMask(const char* remoteAddr, PING_STATE state)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
    OicNode* node = NULL;
    oicping_s* ping = NULL;

    if( remoteAddr == NULL || strlen(remoteAddr) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(remoteAddr=%s, state=%d)", remoteAddr, state);
        return false;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return false;
    }

    node = list->FindByKey(list, (Key_Compare)isIPKeyEqual, remoteAddr);
    if( node == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s).", remoteAddr);
        return false;
//        if( (ping = createOICping_s(remoteAddr, port)) == NULL )
//        {
//            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
//            return false;
//        }
//        list->Insert(list, (void*)ping);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remoteAddr);
        ping = node->item;
    }

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist oicping_s in remoteAddr_Node.(%s)", remoteAddr);
        return false;
    }

//    if( port != 0 )
//        ping->port = port;

    UNSetMask(ping, state);

    checkPingThread(ping);
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return true;
}

bool OICPingSetCloudInterval( int64_t* intervals, int intEA )
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;
    OicNode* node = NULL;
    oicping_s* ping = NULL;

    if( intervals == NULL || intEA == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(intervals=0x%X, intEA=%d)", intervals, intEA);
        return false;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return false;
    }

    node = list->FindByKey(list, (Key_Compare)isStateKeyEqual, PING_ST_ISCLOUD);
    if( node == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for Cloud-Connection.");
        return false;
    }

    if( (ping = node->item) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist oicping_s in Cloud_Node.");
        return false;
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for Cloud-Connection(IP:Port=%s:%d).", ping->addr, ping->port);

    res = SetInterval_SendUpdate(ping, intervals, intEA);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}

bool OICPingSetInterval( const char* remoteAddr, int64_t* intervals, int intEA )
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;
    OicNode* node = NULL;
    oicping_s* ping = NULL;

    if( remoteAddr == NULL || intervals == NULL || intEA == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(remote=%s, intervals=0x%X, intEA=%d)", remoteAddr, intervals, intEA);
        return false;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return false;
    }

    node = list->FindByKey(list, (Key_Compare)isIPKeyEqual, remoteAddr);
    if( node == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s).", remoteAddr);
        return false;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remoteAddr);
    if( (ping = node->item) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist oicping_s in remoteAddr_Node.(%s)", remoteAddr);
        return false;
    }

    res = SetInterval_SendUpdate(ping, intervals, intEA);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}
#else
bool OICPingInit(void)
{
    return true;
}

bool OICPingTerminate(void)
{
    return true;
}

bool OICPingSetMask(const char* remoteAddr, uint16_t port, PING_STATE state)
{
    return true;
}

int OICPingGetMask(const char* remoteAddr, PING_STATE state)
{
    return 1;
}

bool OICPingUNSetMask(const char* remoteAddr, PING_STATE state)
{
    return true;
}

bool OICPingSetCloudInterval( int64_t* intervals, int intEA )
{
    return true;
}

bool OICPingSetInterval( const char* remoteAddr, int64_t* intervals, int intEA )
{
    return true;
}
#endif



/***************************************************************************************
 *
 * Interval Ping-Thread Definition Function.
 *
 ***************************************************************************************/
static void* __attribute__((optimize("O0"))) tCIPingLoop(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int sleepMinute = 0;

    if( ping == NULL || ping->addr == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Invalid arguments.(pint=0x%X)", ping);
        return NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "IP=%s, handle=0x%X, State=0x%X", ping->addr, ping->handleThread, ping->bitMaskState);

    pthread_mutex_lock( &ping->mutexInt );
    for(int i = 0; i < MAX_INTERVAL_EA; i++)
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Interval[%d]=%lld", i, ping->interval[i]);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Intseq= %d", ping->intseq);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "ContinuThread= %d", ping->continuThread);

    if(ping->interval[0] == (int64_t)0 || ping->continuThread == true)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Invalid Interval-Time or Already Run-Thread for this ping.(interval[0]=%lld, continueFlag=%d)", ping->interval[0], ping->continuThread);
        pthread_mutex_unlock( &ping->mutexInt );
        return NULL;
    }
    pthread_mutex_unlock( &ping->mutexInt );

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Start FindResource for /oic/ping to Cloud(%s)", ping->addr);
    do
    {   // /oic/ping Resource Finding Start
        int sleepTime = 1;
        sleepTime = FindResourceOICPing(ping);
        sleep(sleepTime);
    }while(!ping->continuThread);

	pPing = ping;
    int sleepDelay = 10;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Start common-Ping request for /oic/ping to Cloud(%s)", ping->addr);
    while(ping->continuThread)
    {   // common-ping request Sending Start

        if( sleepMinute == 0 )
            sleepMinute = SendOICPingRequest(ping);

        if( sleepMinute )
        {
#ifdef OCF_RTOS
        	int sleepCount = PING_UNIT - NET_TROUBLE_DELAY - sleepDelay;
                sleepDelay = 0;
        	int cCount = 0;
        	while(1)
        	{
        		if(cCount > sleepCount){
        			break;
        		}

        		sleep(1);

        		if(ping->continuThread == false){
        			sleepMinute = 0;
        			break;
        		}
        		cCount++;
        	}
#else
            sleep(PING_UNIT - NET_TROUBLE_DELAY);
#endif

            sleepMinute--;
        }
        else
        {
            sleep(1);
        }

        if( GetMask(ping, PING_ST_INTUPDATED) == true )
        {
            sleepMinute = 0;
            UNSetMask(ping, PING_ST_INTUPDATED);
        }
    }

    UNSetMask(ping, PING_ST_STARTTHREAD|PING_ST_DISCOVERY|PING_ST_REQUEST|PING_ST_INTUPDATE|PING_ST_TIMEOUT);
    ping->continuThread = false;
    SetDefInterval(ping);
	pPing = NULL;
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return (void*)1;
}

void CommandPing() //SKKIM for ping command
{
	if(pPing != NULL)
		SendOICPingRequest(pPing);
}

static OCStackApplicationResult handlerDiscoverPingResource(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int64_t* interval = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
    oicping_s* ping = (oicping_s*)ctx;

    if (ctx == (void*) DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);

        //char* connectionType = getConnectivityType (clientResponse->connType);
        //THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovered on %s", connectionType.c_str());
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);

        if( strcmp(ping->addr, clientResponse->devAddr.addr) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, clientResponse->devAddr.addr);
            return OC_STACK_DELETE_TRANSACTION;
        }

        // Parsing payload.
        if (clientResponse->payload != NULL && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
        {
            OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
            while (val != NULL)
            {
                OCRepPayloadValuePrint(val);
                val = val->next;
            }

            OCRepPayloadGetIntArray(((const OCRepPayload *)clientResponse->payload), INTERVAL_ARRAY, &interval, dimensions);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "dimension=[%d, %d, %d]", dimensions[0], dimensions[1], dimensions[2]);
            for(int i=0; i < dimensions[0]; i++)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "interval[%d]=%d", i, interval[i]);
        }

        // Result processing.
        switch( clientResponse->result )
        {
        case OC_STACK_OK:
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovery Success.");
                if( interval == NULL || interval[0] == 0 || dimensions[0] == 0 )
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "Invalid received remote interval or dimensions.");
                return OC_STACK_DELETE_TRANSACTION;
            }

            if( SetInterval(ping, interval, dimensions[0], PING_ST_DISCOVERY) == true )
            {
                ping->continuThread = true;
            }
            break;
        case OC_STACK_TIMEOUT:
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Occurred Time-Out Event.");
            SetMask(ping, PING_ST_TIMEOUT);
            break;
        default :
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);
            break;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "handlerDiscoverPingResource received Null clientResponse");
    }
    /*! Added by DA for memory Leak fix
     */
    if(NULL != interval) DAFree(interval);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handlerPingRequest(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    oicping_s* ping = (oicping_s*)ctx;

    if (ctx == (void*) DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);

        //char* connectionType = getConnectivityType (clientResponse->connType);
        //THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovered on %s", connectionType.c_str());
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);

        if( strcmp(ping->addr, clientResponse->devAddr.addr) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, clientResponse->devAddr.addr);
            return OC_STACK_DELETE_TRANSACTION;
        }

        // Parsing payload.
        if (clientResponse->payload != NULL && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
        {
            OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
            while (val != NULL)
            {
                OCRepPayloadValuePrint(val);
                val = val->next;
            }
        }

        // Result processing.
        switch( clientResponse->result )
        {
        case OC_STACK_OK:
            THINGS_LOG_D(THINGS_INFO, TAG, "common-ping request Success.");
            if( GetMask(ping, PING_ST_REQUEST) == true && GetMask(ping, PING_ST_TIMEOUT) == false )
                UNSetMask(ping, PING_ST_REQUEST);
            break;
        case OC_STACK_TIMEOUT:
            THINGS_LOG_D(THINGS_WARNING, TAG, "Occurred Time-Out Event.");
            SetMask(ping, PING_ST_TIMEOUT);
            break;
        default :
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Support StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);
            break;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_WARNING, TAG, "handlerPingRequest received Null clientResponse");
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handlerPingUpdateRequest(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int64_t* interval = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
    oicping_s* ping = (oicping_s*)ctx;
    (void*)handle;

    if (ctx == (void*) DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);

        //char* connectionType = getConnectivityType (clientResponse->connType);
        //THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovered on %s", connectionType.c_str());
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);

        if( strcmp(ping->addr, clientResponse->devAddr.addr) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, clientResponse->devAddr.addr);
            return OC_STACK_DELETE_TRANSACTION;
        }

        // Parsing payload.
        if (clientResponse->payload != NULL && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
        {
            OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
            while (val != NULL)
            {
                OCRepPayloadValuePrint(val);
                val = val->next;
            }

            OCRepPayloadGetIntArray(((const OCRepPayload *)clientResponse->payload), INTERVAL_ARRAY, &interval, dimensions);
            THINGS_LOG_D(THINGS_DEBUG, TAG, "dimension=[%d, %d, %d]", dimensions[0], dimensions[1], dimensions[2]);
            for(int i=0; i < dimensions[0]; i++)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "interval[%d]=%d", i, interval[i]);
        }

        // Result processing.
        switch( clientResponse->result )
        {
        case OC_STACK_RESOURCE_CHANGED:
            THINGS_LOG_D(THINGS_DEBUG, TAG, "update-Ping request Success.");
            if( SetInterval(ping, interval, dimensions[0], PING_ST_INTUPDATE) == true )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Success Apply Interval.");
//                SetMask(ping, PING_ST_INTUPDATED);
            }
            break;
        case OC_STACK_TIMEOUT:
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Occurred Time-Out Event.");
            SetMask(ping, PING_ST_TIMEOUT);
            break;
        default :
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);
            break;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "handlerPingUpdateRequest received Null clientResponse");
    }
    /*! Added by DA for memory Leak fix
     */
    if(NULL != interval) OICFree(interval);
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return OC_STACK_DELETE_TRANSACTION;
}


/***************************************************************************************
 *
 * Interval Function Definition Collection.
 *
 ***************************************************************************************/
static oicping_s* createOICping_s(const char* remoteAddr, const uint16_t port)
{
    oicping_s* ping = NULL;

    if( remoteAddr == NULL || strlen(remoteAddr) == 0 )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "remote addr is NULL.");
        return NULL;
    }

    ping = (oicping_s*)DAMalloc(sizeof(oicping_s));
    if( ping == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
        return NULL;
    }

    ping->addr = strdup(remoteAddr);
    ping->port = port;
    ping->continuThread = false;
    pthread_mutex_init( &ping->mutexInt, NULL );
    pthread_mutex_init( &ping->mutexState, NULL );
    pthread_mutex_init( &ping->mutexThread, NULL );

    pthread_mutex_lock( &ping->mutexThread );
    ping->handleThread = 0;
    pthread_mutex_unlock( &ping->mutexThread );

    SetDefInterval(ping);

    SetMask(ping, PING_ST_INIT);
    return ping;
}

static void terminateOICping_s(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    if( ping == NULL )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "ping is NULL.");
        return ;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "ping thread destroy check.");
    if( GetMask(ping, PING_ST_TCPCONNECT) == false && GetMask(ping, PING_ST_STARTTHREAD) == true )
    {   // need to destroy ping-thread.
        OICPingDestroyThread(ping);
    }

    SetMask(ping, PING_ST_INIT);

    pthread_mutex_destroy(&ping->mutexInt);
    pthread_mutex_destroy(&ping->mutexState);
    pthread_mutex_destroy(&ping->mutexThread);

    if( ping->addr )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "ping->addr memory free.");
        DAFree( ping->addr );
        ping->addr = NULL;
    }

    DAFree(ping);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static int isStateKeyEqual(oicping_s* ping, const PING_STATE value)
{
    int res = 0;
    if( ping == NULL || ping->addr == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X, targetIP=%s)", ping);
        return res;
    }

    if( GetMask(ping, value) == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found ping-struct(%s) for State(%d)", ping->addr, value);
        res = 1;
    }

    return res;
}

static int isIPKeyEqual(oicping_s* ping, const char* targetIP)
{
    int res = 0;
    if( ping == NULL || ping->addr == NULL || targetIP == NULL || strlen(targetIP) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X, targetIP=%s)", ping, targetIP);
        return res;
    }

    if( strcmp( ping->addr, targetIP ) == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found ping-struct(%s) for targetIP(%s)", ping->addr, targetIP);
        res = 1;
    }

    return res;
}

static void checkPingThread(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    if( ping == NULL || ping->addr == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
        return ;
    }

    if( list == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
        return ;
    }

    if( GetMask(ping, PING_ST_TCPCONNECT) == true && GetMask(ping, PING_ST_STARTTHREAD) == false )
    {   // need to create ping-thread.
        if( GetMask(ping, PING_ST_ISCLOUD) == false || (GetMask(ping, PING_ST_ISCLOUD) == true && GetMask(ping, PING_ST_SIGNIN) == true) )
        {   // create thread
            OICPingCreateThread(ping);
        }
    }

    if( GetMask(ping, PING_ST_TCPCONNECT) == false && GetMask(ping, PING_ST_STARTTHREAD) == true )
    {   // need to destroy ping-thread.
        OICPingDestroyThread(ping);
    }

    if( GetMask(ping, PING_ST_ISCLOUD|PING_ST_SIGNIN|PING_ST_TCPCONNECT) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not need oicping_s, So, Terminate oicping_s.(IP=%s)", ping->addr);
        if( (ping = list->EraseByKey(list, isIPKeyEqual, ping->addr)) != NULL )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Erase node-element Success in List.");
            terminateOICping_s(ping);
        }
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static void SetMask(oicping_s* ping, PING_STATE state)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
    if( ping == NULL )
        return ;

    pthread_mutex_lock( &ping->mutexState );
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(B) bitMaskState = 0x%X", ping->bitMaskState);
    if( state != PING_ST_INIT )
        ping->bitMaskState |= state;
    else
        ping->bitMaskState = state;
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(A) bitMaskState = 0x%X", ping->bitMaskState);
    pthread_mutex_unlock( &ping->mutexState );

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static bool GetMask(oicping_s* ping, PING_STATE state)
{
    bool res = true;

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
        return false;
    }

    pthread_mutex_lock( &ping->mutexState );
    res = (ping->bitMaskState & state) != 0 ? true : false;
    pthread_mutex_unlock( &ping->mutexState );

    return res;
}

static bool CASMask(oicping_s* ping, PING_STATE preState, bool preStateThen, PING_STATE postState)
{
    bool res = false;

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
        return false;
    }

    pthread_mutex_lock( &ping->mutexState );
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(B) bitMaskState = 0x%X", ping->bitMaskState);
    bool state = (ping->bitMaskState & preState) != 0 ? true : false;
    if( state == preStateThen )
    {
        if( postState != PING_ST_INIT )
            ping->bitMaskState |= postState;
        else
            ping->bitMaskState = postState;
        res = true;
    }
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(A) bitMaskState = 0x%X", ping->bitMaskState);
    pthread_mutex_unlock( &ping->mutexState );

    THINGS_LOG_D(THINGS_DEBUG,TAG, "result = %d", res);
    return res;
}

static void UNSetMask(oicping_s* ping, PING_STATE state)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
    if( ping == NULL )
        return ;

    pthread_mutex_lock( &ping->mutexState );
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(B) bitMaskState = 0x%X", ping->bitMaskState);
    if( state != PING_ST_INIT )
        ping->bitMaskState &= (~state);
    else
        ping->bitMaskState = state;
    THINGS_LOG_D(THINGS_DEBUG,TAG, "(A) bitMaskState = 0x%X", ping->bitMaskState);
    pthread_mutex_unlock( &ping->mutexState );

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

#ifdef OICPING_ENABLE
static int FindResourceOICPing(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int sleepTime=1;    // seconds
    OCDoHandle gReqHandle = NULL;
    OCCallbackData cb;
    char* hostAddr = NULL;

    cb.cb = handlerDiscoverPingResource;
    cb.cd = NULL;
    cb.context = (void*)ping;

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping struct is NULL.");
        return sleepTime;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "cb.context=0x%X", cb.context);

    if( (hostAddr = MakeHostAddr(ping->addr, ping->port)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.");
        return sleepTime;
    }

    while( CASMask(ping, PING_ST_REQUEST|PING_ST_INTUPDATE|PING_ST_TIMEOUT, false, PING_ST_DISCOVERY) == false )
        sleep(1); // sleep 1 second

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCFindKeepAliveResouce request to %s.", hostAddr);
    if( OCFindKeepAliveResource(&gReqHandle, hostAddr, &cb) == OC_STACK_OK )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Waiting for /oic/ping Cloud-response.(%s)", ping->addr);
        sleepTime = 5;
    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error]: Ping Resource Discovery-request is failed.");
        UNSetMask(ping, PING_ST_DISCOVERY);
    }
    /*! Added by DA for memory Leak fix
     */
    if(hostAddr) DAFree(hostAddr);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(sleepTime=%d)", sleepTime);
    return sleepTime;
}

static int SendOICPingRequest(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int sleepTime = 0;  // minute
    OCDoHandle gReqHandle = NULL;
    OCStackResult result=OC_STACK_ERROR;
    OCCallbackData cb;
    int64_t interval = 0;
    char* hostAddr = NULL;
    OCRepPayload* payload = NULL;

    cb.cb = handlerPingRequest;
    cb.cd = NULL;
    cb.context = (void*)ping;

    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping struct is NULL.");
        return sleepTime;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "cb.context=0x%X", cb.context);

    if( (hostAddr = MakeHostAddr(ping->addr, ping->port)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "hostAddr Memory Allocation is failed.");
        return sleepTime;
    }

    payload = OCRepPayloadCreate();
    if (!payload)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "payload Memory Allocation is failed.");
        /*! Added by DA for memory Leak fix
         */
        if(hostAddr) DAFree(hostAddr);
        return sleepTime;
    }

    while( CASMask(ping, PING_ST_DISCOVERY|PING_ST_INTUPDATE|PING_ST_TIMEOUT, false, PING_ST_REQUEST) == false )
        sleep(1); // sleep 1 second

    pthread_mutex_lock( &ping->mutexInt );
    interval = ping->interval[ping->intseq];
    // period send request to Cloud for interval hand-shaking /oic/ping resource.
    OCRepPayloadSetPropInt(payload, INTERVAL, interval);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "interval = %lld", interval);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCSendKeepAliveRequest request to %s.", hostAddr);
    if( (result= OCSendKeepAliveRequest(&gReqHandle, hostAddr, payload, &cb)) == OC_STACK_OK )
//    if( (result= SendKeepAliveRequest(hostAddr, interval, &cb)) == OC_STACK_OK )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Sending Success about common-Ping request.");
        do {
            ping->intseq = (ping->intseq+1) % MAX_INTERVAL_EA;
        }while(ping->interval[ping->intseq] == 0);

        sleepTime = interval;
    }
    UNSetMask(ping, PING_ST_INTUPDATED);
    pthread_mutex_unlock( &ping->mutexInt );

    if(result != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error]: Sending about common-Ping request is failed.");
        UNSetMask(ping, PING_ST_REQUEST);
        OCRepPayloadDestroy(payload);
    }
    /*! Added by DA for memory Leak fix
     */
    if(hostAddr) DAFree(hostAddr);
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return sleepTime;
}

static bool SetInterval_SendUpdate(oicping_s* ping, int64_t* intervals, int intEA)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

//    int i = 0;
    bool res = false;
    OCStackResult result=OC_STACK_ERROR;
    OCCallbackData cb;
    char* hostAddr = NULL;

    cb.cb = handlerPingUpdateRequest;
    cb.cd = NULL;
    cb.context = (void*)ping;

    if( ping == NULL || intervals == NULL || intEA == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(ping=%0x%X, intervals=0x%X, intEA=%d)", ping, intervals, intEA);
        return false;
    }

    if( GetMask(ping, PING_ST_ISCLOUD) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Interval Update & Sending update-Ping request is Support only for Cloud.");
        return false;
    }

    if( intEA > MAX_INTERVAL_EA )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "intEA is over %d. So, fix to %d", MAX_INTERVAL_EA, MAX_INTERVAL_EA);
        intEA = MAX_INTERVAL_EA;
    }

    if( GetMask(ping, PING_ST_STARTTHREAD) == false || ping->continuThread == false )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Ping-Thread is not ready.");
        return false;
    }

    if( (hostAddr = MakeHostAddr(ping->addr, ping->port)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.");
        return false;
    }

    while( CASMask(ping, PING_ST_DISCOVERY|PING_ST_REQUEST|PING_ST_TIMEOUT, false, PING_ST_INTUPDATE) == false )
        sleep(1); // sleep 1 second

//    pthread_mutex_lock( &ping->mutexInt );
    result = SendKeepAliveUpdateRequest(hostAddr, intervals, intEA, &cb);
//    if( (result = SendKeepAliveUpdateRequest(host, intervals, intEA, &cb)) == OC_STACK_OK )     // send interval-update request to Cloud. & set State.
//    {
//        memset(ping->interval, 0, sizeof(int64_t)*MAX_INTERVAL_EA);
//        for(i=0; i < intEA; i++)
//        {
//            ping->interval[i] = intervals[i];
//        }
//        ping->intseq = 0;
//        res = true;
//    }
//    pthread_mutex_unlock( &ping->mutexInt );

    if(result != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error]: Sending about update-Ping request is failed.");
        UNSetMask(ping, PING_ST_INTUPDATE);
    }
    else
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Success Sending update-Ping request.");
        res = true;
    }
    /*! Added by DA for memory Leak fix
     */
    if(hostAddr) DAFree(hostAddr);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}
#else
static int FindResourceOICPing(oicping_s* ping)
{
    return 1;
}

static int SendOICPingRequest(oicping_s* ping)
{
    return 1;
}

static bool SetInterval_SendUpdate(oicping_s* ping, int64_t* intervals, int intEA)
{
    return true;
}
#endif

static bool SetInterval(oicping_s* ping, int64_t* intervals, int intEA, PING_STATE confirmState)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int i = 0;
    bool res = false;

    if( ping == NULL || intervals == NULL || intEA == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(ping=%0x%X, intervals=0x%X, intEA=%d)", ping, intervals, intEA);
        return false;
    }

    if( intEA > MAX_INTERVAL_EA )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "intEA is over %d. So, fix to %d", MAX_INTERVAL_EA, MAX_INTERVAL_EA);
        intEA = MAX_INTERVAL_EA;
    }

    if( confirmState == PING_ST_INIT)
        confirmState = PING_ALL_FLAG;

    pthread_mutex_lock( &ping->mutexInt );
    if( GetMask(ping, confirmState) == true && GetMask(ping, PING_ST_TIMEOUT) == false )
    {
        memset(ping->interval, 0, sizeof(int64_t)*MAX_INTERVAL_EA);
        for(i=0; i < intEA; i++)
        {
            ping->interval[i] = intervals[i];
        }
        ping->intseq = 0;
        res = true;

        if( confirmState != PING_ALL_FLAG )
            UNSetMask(ping, confirmState);

        if( confirmState == PING_ST_INTUPDATE )
            SetMask(ping, PING_ST_INTUPDATED);
    }
    pthread_mutex_unlock( &ping->mutexInt );

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}

static bool SetDefInterval(oicping_s* ping)
{
    if( ping == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
        return false;
    }

    pthread_mutex_lock( &ping->mutexInt );
    memset( ping->interval, 0, sizeof(int64_t)*MAX_INTERVAL_EA );
    ping->interval[0] = (int64_t)1;
    ping->interval[1] = (int64_t)2;
    ping->interval[2] = (int64_t)4;
    ping->interval[3] = (int64_t)8;
    ping->intseq = 0;
    pthread_mutex_unlock( &ping->mutexInt );

    return true;
}

static bool OICPingCreateThread(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;

    if( ping == NULL || ping->addr == NULL || ping->port == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
        if( ping )
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(addr=%s, port=%d)", ping->addr, ping->port);
        return res;
    }

    pthread_mutex_lock( &ping->mutexThread );
    if( ping->handleThread == 0 )
    {
#ifdef OCF_RTOS
        if( pthread_create_rtos(&ping->handleThread,NULL, (pthread_FuncType)tCIPingLoop, ping, THINGS_STACK_PING_THREAD) != 0 )
#else
        if( OICThread_create(&ping->handleThread, NULL, (pthread_FuncType)tCIPingLoop, ping) != 0 )
#endif
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create tCIPingLoop thread is failed.");
            ping->handleThread = 0;
               goto GOTO_OUT;
        }

        SetMask( ping, PING_ST_STARTTHREAD );
        res = true;
    }
GOTO_OUT:
    pthread_mutex_unlock( &ping->mutexThread );

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", res);
    return res;
}

static bool OICPingDestroyThread(oicping_s* ping)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;

    if( ping == NULL || ping->addr == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
        return res;
    }

    pthread_mutex_lock( &ping->mutexThread );
    if( ping->handleThread )
    {
#ifdef OCF_RTOS
        ping->continuThread= false;
        sleep(2); /* wait till thread exit*/
        pthread_detach(ping->handleThread);
#else /* there is problem in artik during thread_cancel hence avoiding it*/
        pthread_cancel(ping->handleThread);
        pthread_detach(ping->handleThread);
#endif
        ping->handleThread = 0;
        ping->continuThread= false;
        SetDefInterval(ping);

        UNSetMask( ping, PING_ST_STARTTHREAD|PING_ST_DISCOVERY|PING_ST_REQUEST|PING_ST_INTUPDATE|PING_ST_TIMEOUT );
        res = true;
    }
    pthread_mutex_unlock( &ping->mutexThread );

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", res);
    return res;
}

static void OCRepPayloadValuePrint( OCRepPayloadValue* value )
{
    switch(value->type)
    {
    case OCREP_PROP_INT:
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",value->name, value->i);
        break;
    case OCREP_PROP_DOUBLE:
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %f, double",value->name, value->d);
        break;
    case OCREP_PROP_STRING:
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",value->name, value->str);
        break;
    case OCREP_PROP_BOOL:
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %d, bool",value->name, value->b);
        break;
    case OCREP_PROP_BYTE_STRING:
        THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(binary) size : %d", value->name, value->ocByteStr.len);
        //OCPrintByteValue( value->ocByteStr.bytes, value->ocByteStr.len );
        break;
    case OCREP_PROP_OBJECT:
        // Note: Only prints the URI (if available), to print further, you'll
        // need to dig into the object better!
        THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(OCRep):%s", value->name, value->obj->uri);
        break;
    case OCREP_PROP_ARRAY:
        switch(value->arr.type)
        {
            case OCREP_PROP_INT:
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(int array):%zu x %zu x %zu",
                        value->name,
                        value->arr.dimensions[0], value->arr.dimensions[1],
                        value->arr.dimensions[2]);
                        for(size_t iter  = 0 ; iter < value->arr.dimensions[0] ; iter++)
                        {
                            THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(int):%d", value->arr.iArray[iter]);
                        }
                break;
            case OCREP_PROP_DOUBLE:
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(double array):%zu x %zu x %zu",
                        value->name,
                        value->arr.dimensions[0], value->arr.dimensions[1],
                        value->arr.dimensions[2]);
                        for(size_t iter  = 0 ; iter < value->arr.dimensions[0] ; iter++)
                        {
                            THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(double):%f", value->arr.dArray[iter]);
                        }
                break;
            case OCREP_PROP_BOOL:
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(bool array):%zu x %zu x %zu",
                        value->name,
                        value->arr.dimensions[0], value->arr.dimensions[1],
                        value->arr.dimensions[2]);
                        for(size_t iter  = 0 ; iter < value->arr.dimensions[0] ; iter++)
                        {
                            THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(bool):%s", value->arr.bArray[iter] ? "true" : "false");
                        }
                break;
            case OCREP_PROP_STRING:
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(string array):%zu x %zu x %zu",
                        value->name,
                        value->arr.dimensions[0], value->arr.dimensions[1],
                        value->arr.dimensions[2]);
                        for(size_t iter  = 0 ; iter < value->arr.dimensions[0] ; iter++)
                        {
                            THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(string):%s", value->arr.strArray[iter]);
                        }
                break;
            case OCREP_PROP_BYTE_STRING:
                THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(byte array):%zu x %zu x %zu",
                        value->name,
                        value->arr.dimensions[0], value->arr.dimensions[1],
                        value->arr.dimensions[2]);
                break;
            default:
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Key: %s <-- Unknown/unsupported array type!",
                        value->name);
                break;
        }
        break;
    default :
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", value->type);
        break;
    }
}

#include "cloud_manager.h"

static char* MakeHostAddr(const char* ip, const uint16_t port)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    char* host = NULL;
    int length = 0;

    THINGS_LOG_D(THINGS_DEBUG, TAG,"ip=%s, port=%d", ip, port);
    if( ip == NULL || strlen(ip) == 0 || port == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.");
        return NULL;
    }

    length = strlen(ip) + 7 + strlen(DEFAULT_COAP_TCP_HOST);
    if( (host=(char*)DAMalloc(sizeof(char)*length)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
        return NULL;
    }

    // Update Cloud Address.
    memset(host, 0, length);
    if( snprintf(host, length, "%s%s:%d", DEFAULT_COAP_TCP_HOST, ip, port) >= length )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] host buffer is overflowed.");
        DAFree(host);
        host = NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "host = %s", host);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return host;
}

static OCStackResult SendKeepAliveUpdateRequest(const char *host, int64_t* intervals, int intEA, OCCallbackData* cb )
{
    OCDoHandle gReqHandle = NULL;
    OCStackResult result = OC_STACK_ERROR;
    char targetUri[MAX_URI_LENGTH * 2] = { 0, };
    OCRepPayload *payload = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};

    if( host == NULL || strlen(host) == 0 || intervals == NULL || intEA == 0 )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid eventData.(host=%s, intervals=0x%X, intEA=%d)", host, intervals, intEA);
        result = OC_STACK_NO_MEMORY;
        goto no_memory;
    }

    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, KEEPALIVE_RESOURCE_URI);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "oicping Update Request to : %s", targetUri);

    payload = OCRepPayloadCreate();
    if (!payload)
    {
        result = OC_STACK_NO_MEMORY;
        goto no_memory;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "intEA=%d", intEA);
    for(int i=0; i < intEA; i++)
        THINGS_LOG_D(THINGS_DEBUG, TAG, "intervals[%d]=%d", i, intervals[i]);

    dimensions[0] = (size_t)intEA;
    OCRepPayloadSetIntArray(payload, INTERVAL_ARRAY, intervals, dimensions);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "key     : %s", INTERVAL_ARRAY);

//#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
//    CASelectCipherSuite(0x35, (1 << 4));
//#endif

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCSendKeepAliveRequest request to %s.", host);
    result = OCSendKeepAliveRequest(&gReqHandle, host, payload, cb);
//    result = OCDoResource(&gReqHandle, OC_REST_POST, targetUri,
//                          NULL, (OCPayload *) payload, CT_ADAPTER_TCP,
//                          OC_HIGH_QOS, cb, NULL, 0);

    if(result != OC_STACK_OK )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "oicping Update Request Sending is failed.(URI: %s)", targetUri);
        OCRepPayloadDestroy(payload);
    }

no_memory:
    return result;
}
