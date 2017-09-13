/*
 * oicnetwork.c
 *
 *  Created on: 2016. 10. 25.
 *      Author: eunseok
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thingslogger.h"
#include "thingsmalloc.h"
#include "thingsapi.h"
#include "oicnetwork.h"

#include "resourcehandler.h"
#include "cloud_manager.h"
#include "easysetup_manager.h"
#include "oicping.h"

#define TAG "[oicnetwork]"

static OICGetAPSearchListFuncType    gGetAPSearchList = NULL;
static OICSetAPConnectionFuncType    gSetAPConnection = NULL;

static int AppState = -1;
static char * AppAP_name = NULL;
static char * AppIP_addr = NULL;
volatile static bool isConnectedTargetAP = false;

#define IP_NULL_VAL     "0.0.0.0"


/*******************************************************************
 *
 * state : 0 --> disconnect
 *           1 --> connected
 *
 *******************************************************************/
static int CBskipChecking(int state, char* ap_name, char* ip_addr)
{
    int ret = 0;

    if(state != 0 && state != 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "invalid state value.(%d)", state);
        return 0;
    }

    if(AppState==-1)
    {
        AppState = 0;
        AppAP_name = (char*)DAMalloc(sizeof(char)*1);
        AppAP_name[0]=0;
        AppIP_addr = (char*)DAMalloc(sizeof(char)*1);
        AppIP_addr[0]=0;
    }

    // Process skip checking.
    if((state == 1 &&
       ((ap_name == NULL || strlen(ap_name)==0) ||
        (ip_addr == NULL || strlen(ip_addr)==0 || strcmp(ip_addr, IP_NULL_VAL)==0))) ||
       (state != 1 && state != 0) )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "invalid value.(state=%d, AP_SSID=%s, IP=%s)", state, ap_name, ip_addr);
        return 0;
    }

    if( AppState != state )
        ret = 1;
    else if ( state == 1 && (strcmp(ap_name, AppAP_name) !=0 || strcmp(ip_addr, AppIP_addr)!=0) )
        ret = 1;

    if( ret == 1 )
    {
        // Update contents
        AppState = state;
        if(ap_name)
        {
            DAFree(AppAP_name);
            AppAP_name = strdup(ap_name);
        }

        if(ip_addr)
        {
            DAFree(AppIP_addr);
            AppIP_addr = strdup(ip_addr);
        }
    }

    return ret;
}

void OICWifiStateChangedCB_init(void)   // when Soft-AP on, Not called Disconnected Call-Back Func. So, it's alternate.
{
    if(AppState==-1)
    {
        AppState = 0;
        AppAP_name = (char*)DAMalloc(sizeof(char)*1);
        AppAP_name[0]=0;
        AppIP_addr = (char*)DAMalloc(sizeof(char)*1);
        AppIP_addr[0]=0;
    }
    else
    {
        AppAP_name[0]=0;
        AppIP_addr[0]=0;
    }

    isConnectedTargetAP = false;
    CIRetryStop_byWiFiCB(false);
}

/*****************************************************************************
 *
 * It's called from DA Application when WiFi state changed.
 *
 * state : 1 -> Connected to WiFi AP.
 *         0 -> Disconnected from WiFi AP.
 * ap_name : AP SSID.
 * ip_addr : IP address.
 *
 *****************************************************************************/
int OICWifiStateChangedCB(int state, char* ap_name, char* ip_addr)
{
    bool Retry1Stop0 = false;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "state=%d, ap_name=%s, ip_name=%s", state, ap_name, ip_addr);

    if( OICIsNetAPIInitialize() == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "gGetSoftAPState is NULL.");
        return 0;
    }

    if(CBskipChecking(state, ap_name, ip_addr) != 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Call-back process skiped.");
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "AppState=%d, AppAP_name=%s, AppIP_addr=%s", AppState, AppAP_name, AppIP_addr);

    // Process.
    if(state == 1)
    {   // [Info] Always, when Soft-AP enabling, Call-Back State maintain disconnected. So, Soft-AP Check is not need.
        THINGS_LOG_D(THINGS_DEBUG, TAG, "AP CONNECTED.");
        isConnectedTargetAP = true;

        setSSIDinWiFiResource(AppAP_name);
        Retry1Stop0 = true;     // Re-Try with Last failed Access info.
        ESMWifiProvCheckCB(state, AppAP_name, AppIP_addr);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "AP DISCONNECTED.");
        isConnectedTargetAP = false;
        setSSIDinWiFiResource(NULL);
    }

    // Call Retry or forceStop Connection
    CIRetryStop_byWiFiCB(Retry1Stop0);

    return 1;
}

/*****************************************************************************
 *
 * It's called from Iotivity Framework when Adaptor state changed.
 *
 * adapter : Adapter Type.
 * enabled  : true -> connected.
 *            false-> disconnected.
 *
 *****************************************************************************/
void OICAdapterStateCB(CATransportAdapter_t adapter, bool enabled)
{
    (void)adapter;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "IN");

    if (enabled)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED but Not Guarantee to be Communication-Available");
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "AP DISCONNECTED.");
        isConnectedTargetAP = false;
        setSSIDinWiFiResource(NULL);
        CIRetryStop_byWiFiCB(false);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "OUT");
}

/*****************************************************************************
 *
 * It's called from Iotivity Framework when TCP session state changed.
 *
 * info : information about remote-peer.
 * connected  : true -> connected
 *              false-> disconnected
 *
 *****************************************************************************/
void OICTCPsessStateCB(const CAEndpoint_t * info, bool connected)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    if( info == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[IoTivity Error] invalid call-back parameter.(info is null.)");
        return ;
    }

    if( info->adapter != CA_ADAPTER_TCP )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Don't support adapter type.(%d)", info->adapter);
        return ;
    }

    if(strlen(info->addr) == 0 || info->port == 0)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[IoTivity Error] invalid call-back parameter.(addrIP=%s, port=%d)", info->addr, info->port);
        return ;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "IP = %s, port = %d", info->addr, info->port);

    if( connected == true)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED");
        OICPingSetMask(info->addr, info->port, PING_ST_TCPCONNECT);
        return ;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "DISCONNECTED");
    OICPingUNSetMask(info->addr, PING_ST_SIGNIN|PING_ST_TCPCONNECT);

    if( CIRetryStop_byTCPCB(info->addr, info->port) == -1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] System Error.");
        return ;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
}

int OICSetAPConnection( AccessPointInfo1* APinfo )
{
    if( OICIsNetAPIInitialize() == 0 || APinfo == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not yet Initialized or APinfo(0x%X) is NULL.", APinfo);
        return 0;
    }

    return gSetAPConnection( APinfo ,"0");
}

int OICGetAPList(AccessPointInfo1*** pInfo, int* pCount)
{
    if( OICIsNetAPIInitialize() == 0 || pInfo == NULL || pCount == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Can't Call GetAPSearchList(pInfo=0x%X, pCount=0x%X).", pInfo, pCount);
        return 0;
    }

    return gGetAPSearchList(pInfo, pCount);
}

/*******************************************************************
 *
 * DA API Function register
 *
 *******************************************************************/
void OICRegisterSetAPConFunc(OICSetAPConnectionFuncType func)
{
    gSetAPConnection = func;
}

void OICRegisterGetAPListFunc(OICGetAPSearchListFuncType func)
{
    gGetAPSearchList = func;
}

/*****************************************************************************
 *
 * Ready Check All of Network APIs.
 *
 * return : 0 --> Not Ready.
 *          1 --> Initialize Success.
 *         -1 --> Initialize Failed.
 *
 *****************************************************************************/
int OICIsNetAPIInitialize(void)
{
    if( gSetAPConnection == NULL || gGetAPSearchList == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Not Initialized Network API.");
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] gSetAPConnection = 0x%X, gGetAPSearchList = 0x%X", gSetAPConnection, gGetAPSearchList);
        return 0;
    }

    return 1;
}

bool OICIsConnectedAP(void)
{
    return isConnectedTargetAP;
}

/*
 * If Easy-Setup is Reseted, then need to recover about Current Connected-AP info.
 * This Function serve to recover Connected-AP info with Customized-method.
 */
void OICApplyCurrentAP(void)
{
    if( AppState == 1 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Connected AP.(AP_name=%s) Update AP info.", AppAP_name);
        setSSIDinWiFiResource(AppAP_name);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "DisConnected AP. So, Clear rested-AP info.");
        setSSIDinWiFiResource(NULL);
    }
}
