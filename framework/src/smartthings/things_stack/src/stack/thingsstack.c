//#******************************************************************
//#
//# Copyright 2015 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE // for the usleep
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "ocpayload.h"
#include "octypes.h"
#include "ocstack.h"
#include "thingsapi.h"
#include "oiceasysetup.h"
#include "oicdef.h"
#include "oiccommon.h"
#include "thingslogger.h"
#include "thingsmalloc.h"
#include "oicstringutil.h"

#include "oicresource.h"
#include "oicserverbuilder.h"
#include "oicsecuritymanager.h"
#include "oicreqhandler.h"

#include "thingsdatamanager.h"

#include "oicnetwork.h"
#include "easysetup_manager.h"
#include "cloud_manager.h"
#include "cloud_evtpend.h"

#include <wifi_manager/wifi_manager.h>

#include "oicthread.h"
#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif

#define TAG "[thingsstack]"

typedef void *(*pthread_FuncType) (void *);

volatile static int gQuitFlag = 0;
volatile static int isOICModuleInited = 0;

typedef struct SAbort_s {
    pthread_t* hThread;
    ESEnrolleeAbort level;
}SAbort_s;

typedef struct ResetArgs_s {
    OicResource* remoteOwner;
    ESEnrolleeReset resetType;
} ResetArgs;

OICAbortEasySetupFuncType gAbortEasySetup = NULL;

OICServerBuilder*           gServerBuilder = NULL;
OICRequestHandler*          gReqHandler = NULL;

OICResetConfirmFuncType     gConfirmResetStart = NULL;
OICSecOTMStateFuncType      gOTMEventHandler = NULL;

//OICGetDeviceStatusFuncType  gGetDeviceStatus = NULL;

//OICGetScheduleFuncType      gGetScheduleFunc = NULL;
//OICSetScheduleFuncType      gSetScheduleFunc = NULL;

static pthread_mutex_t gAppCBmutex   = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gNSmutex      = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gOICStopMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t gWifimutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gWificond;
static wifi_manager_scan_info_s *g_wifi_scan_info;
static int g_wifi_scan_count;

#define WIFI_SCAN_SIGNAL			\
do {										\
    pthread_mutex_lock(&gWifimutex);			\
    pthread_cond_signal(&gWificond);			\
    pthread_mutex_unlock(&gWifimutex);			\
} while(0)

#define WIFI_SCAN_WAIT						\
do {										\
    pthread_mutex_lock(&gWifimutex);			\
    pthread_cond_wait(&gWificond, &gWifimutex);	\
    pthread_mutex_unlock(&gWifimutex);			\
} while(0)

static bool bResetContinueFlag = false;
static bool bThreadOICReset = false;
static pthread_t hThreadOICReset = NULL;
static pthread_mutex_t mThreadOICReset = PTHREAD_MUTEX_INITIALIZER ;
static volatile RST_STATE mResetBitMask = RST_COMPLETE;

static void* tOICResetLoop(ResetArgs* Args);
static void* tOICAbortLoop(SAbort_s* contents);

typedef enum
{
    OIC_API_NONE = 0,
    OIC_API_OBSERVE_OK = 1,
    OIC_API_NOTI_OK = 2,
    OIC_API_PUSH_OK = 4
} OICNotiResult;

const char* OICGetSWVersion()
{
    return OIC_STACK_FIRMWARE_VERSION;
}

const char * OICGetDeviceIDString()
{
    return OCGetServerInstanceIDString();
}

int OICSetDeviceName(int devNum, char * name)
{
    int res = 0;
    if(!isOICModuleInited)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack not initialized");
    }
    else
    {
        if(devNum > 0)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Dev Num %d Not Supported Yet", devNum);
        }
        else
        {
            if(NULL != name && strlen(name) > 0)
            {
                OCStackResult result = OCSetPropertyValue(PAYLOAD_TYPE_DEVICE,
                                                          OC_RSRVD_DEVICE_NAME,
                                                          name);
                if( result == OC_STACK_OK )
                {
                    res = 1;
                }
                else
                {
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Set Device Name Failed : %d ", result);
                }
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid Device Name : %s ", name);
            }
        }
    }

    return res;
}

// This logic will be refactored later to reflact whole easy-setup states..
int OICGetEasySetupState(void)
{
    bool isOwned = false;
    // 1. check the owned state
    if( OC_STACK_OK != OCGetDeviceOwnedState(&isOwned) )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to get device owned state, Informing as UNOWNED~!!!!");
        isOwned = false;
    }

    // 2. check the resource publish state if necessary..

    // 3. anything else to check???

    return (isOwned == true);
}

int OICRegisterEasySetupStateFunc(OICGetEasySetupStateFuncType func)
{
    int res = 0;

    if( func != NULL )
    {
        ESRegisterNotifyEasySetupState((void*)func);
        res = 1;
    }

    return res;
}

void
_OCFAPI_WiFi_STA_Connected(void)
{
    THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
    wifi_manager_info_s wifi_info;
    wifi_manager_get_info(&wifi_info);

    char curDeviceIPAddress[18 + 1];
    wifi_net_ip4_addr_to_ip4_str(wifi_info.ip4_address, curDeviceIPAddress);

    OICWifiChangedCallFunc(1, wifi_info.ssid, curDeviceIPAddress);
}

void
_OCFAPI_WiFi_STA_Disconnected(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
}

void
_OCFAPI_WiFi_SoftAP_STA_Join(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);

}

void
_OCFAPI_WiFi_SoftAP_STA_Leave(void)
{
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
}

void
_OCFAPI_WiFi_Scan_Done(wifi_manager_scan_info_s **scan_result, int res)
{
    THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
     */
    int scan_count = 0;
    wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
    g_wifi_scan_info = *scan_result;
    if (res == WIFI_SCAN_FAIL)
    {
        printf("WiFi scan failed\n");
        WIFI_SCAN_SIGNAL;
        return;
    }
	while (wifi_scan_iter != NULL) {
        scan_count++;
        //printf("WiFi AP SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d\n", wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi);
        wifi_scan_iter = wifi_scan_iter->next;
    }
    g_wifi_scan_count = scan_count;
    WIFI_SCAN_SIGNAL;
}

static const wifi_manager_cb_s wifi_callbacks = {
    _OCFAPI_WiFi_STA_Connected,
    _OCFAPI_WiFi_STA_Disconnected,
    _OCFAPI_WiFi_SoftAP_STA_Join,
    _OCFAPI_WiFi_SoftAP_STA_Leave,
    _OCFAPI_WiFi_Scan_Done,
};

typedef int8_t     INT8;

bool connectHomeAP(wifi_manager_ap_config_s* connect_config)
{
	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;

	int retry_count = 0;

	for( ; retry_count < 3; retry_count++)
	{
		result = wifi_manager_connect_ap(connect_config);

		if (result == WIFI_MANAGER_SUCCESS)
		{
			break;
		}
		else
		{
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi");
		}
	}

	if(retry_count == 3)
	{
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi 3 times");
        return false;
	}

	return true;
}


bool ocfHandleStopSoftAP(wifi_manager_ap_config_s* connect_config)
{
	wifi_manager_result_e result = WIFI_MANAGER_SUCCESS;
	wifi_manager_info_s info;
	wifi_manager_get_info(&info);

	if(info.mode == SOFTAP_MODE)
	{
		if(wifi_manager_set_mode(STA_MODE, NULL) != WIFI_MANAGER_SUCCESS)
		{
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to change to STA mode)");
			return false;
		}
		things_task_sleep(100);
	}

	//Stop_Easysetup_Timer(); //connect to ap, stop 5min timer.
	//Set_Oobe_State(0); //because connect to ap, set oobe flag zero(disable).

	int retry_count = 0;

	for( ; retry_count < 3; retry_count++)
	{
		result = wifi_manager_connect_ap(connect_config);

		if (result == WIFI_MANAGER_SUCCESS)
		{
			break;
		}
		else
		{
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi");
		}
	}

	if(retry_count == 3)
	{
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to connect WiFi 3 times");
        return false;
	}


	if(connect_config != NULL)
	{
        SaveAccesPointInfo(connect_config);
	}

	return true;
}

INT8
_OCFAPI_WiFiConnectionCB(AccessPointInfo1* pInfo, char* pCmdId)
{	
	THINGS_LOG_D(THINGS_INFO, TAG, "T%d --> %s", getpid(), __FUNCTION__);

	if(pInfo == NULL) {
        THINGS_LOG_D(THINGS_INFO, TAG, "Invalid params");
		return -1;
	}

	wifi_manager_ap_config_s connect_config;

	strncpy(connect_config.ssid, pInfo->ESSID, 32);
	connect_config.ssid_length = strlen(connect_config.ssid);
	strcpy(connect_config.passphrase, pInfo->SecurityKey);
	connect_config.passphrase_length = strlen(connect_config.passphrase);

    THINGS_LOG_D(THINGS_INFO, TAG, "[%s] ssid : %s", __FUNCTION__, connect_config.ssid);
    THINGS_LOG_D(THINGS_INFO, TAG, "[%s] passphrase : %s", __FUNCTION__, connect_config.passphrase);

	// set auth type
	if(strcmp(pInfo->AuthType, "WEP") == 0)
	{
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WEP_SHARED;
	}
	else if (strcmp(pInfo->AuthType, "WPA-PSK") == 0)
	{
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA_PSK;
	}
	else if (strcmp(pInfo->AuthType, "WPA2-PSK") == 0)
	{
		connect_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
	}

	// set encryption crypto type
	if(strcmp(pInfo->EncType, "WEP-64") == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
	}
	else if(strcmp(pInfo->EncType, "WEP-128") == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
	}
	else if(strcmp(pInfo->EncType, "TKIP") == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP;
	}
	else if(strcmp(pInfo->EncType, "AES") == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_AES;
	}
	else if(strcmp(pInfo->EncType, "TKIP_AES") == 0) {
		connect_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_TKIP_AND_AES;
	}

	int ret = ocfHandleStopSoftAP(&connect_config);
	if(ret != 1) {
		return -1;
	}

	return 1;
}

AccessPointInfo1* CreateAccessInfo()
{
    AccessPointInfo1* info = (AccessPointInfo1*)DAMalloc(sizeof(AccessPointInfo1));
    memset(info->ESSID,          0 , MAX_ESSID);
    memset(info->SecurityKey,  0 , MAX_SECUIRTYKEY);
    memset(info->AuthType,      0 , MAX_TYPE_AUTH);
    memset(info->EncType,       0 , MAX_TYPE_ENC);
    memset(info->Channel,       0 , MAX_CHANNEL);
    memset(info->SignalLevel,  0 , MAX_LEVEL_SIGNAL);
    memset(info->BSSID,          0 , MAX_BSSID);
    return info;
}

int
_OCFAPI_WiFiSearchCB(AccessPointInfo1*** pInfo, int* pCount)
{
    THINGS_LOG_V(THINGS_INFO, TAG, THINGS_FUNC_ENTRY);
    #if 0
    int ret = 0;
    int iter = 0;

    ret = pthread_mutex_init(&gWifimutex, NULL);
	if (ret != 0) {
		return -1;
	}
	ret = pthread_cond_init(&gWificond, NULL);
	if (ret != 0) {
		return -1;
    }

    wifi_manager_start_scan();
    WIFI_SCAN_WAIT;
    wifi_manager_scan_info_s *wifi_scan_iter = g_wifi_scan_info;

    if (pInfo)
    {
        (*pCount) = g_wifi_scan_count;
        *pInfo = (AccessPointInfo1**)malloc(sizeof(AccessPointInfo1*)*(*pCount));
        while (wifi_scan_iter != NULL)
        {
            (*pInfo)[iter] = CreateAccessInfo();
            strcpy((*pInfo)[iter]->ESSID, wifi_scan_iter->ssid);
            strcpy((*pInfo)[iter]->SecurityKey, "1130505*");
            strcpy((*pInfo)[iter]->AuthType, "WPA2-PSK");
            strcpy((*pInfo)[iter]->EncType, "AES");
            strcpy((*pInfo)[iter]->Channel, "1");
            strcpy((*pInfo)[iter]->SignalLevel, "-46");
            strcpy((*pInfo)[iter++]->BSSID, wifi_scan_iter->bssid);
            wifi_scan_iter = wifi_scan_iter->next;
        }
        ret = 1;
    }

	pthread_mutex_destroy(&gWifimutex);
	pthread_cond_destroy(&gWificond);
    THINGS_LOG_V(THINGS_INFO, TAG, THINGS_FUNC_EXIT);
    return ret;
    #else
    (*pCount) = 0;
    
    return 1;
    #endif
}

const WIFI_MODE WiFiMode[4] = {
    WiFi_11A,
    WiFi_11B,
    WiFi_11G,
    WiFi_11N,
};

#define WIFI_FREQUENCY WiFi_24G   

//jun
static void _OTMEventCb(const char *addr, uint16_t port, const char *uuid, int event)
{
	switch (event) {
	case OIC_OTM_READY:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_READY");
		break;
	case OIC_OTM_STARTED:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_STARTED");
		THINGS_LOG_D(THINGS_DEBUG, TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_DONE:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "OIC_OTM_DONE");
		THINGS_LOG_D(THINGS_DEBUG, TAG, "[%s:%d][%s]", addr, port, uuid);
		break;
	case OIC_OTM_ERROR:
		THINGS_LOG_D(THINGS_ERROR, TAG, "OIC_OTM_ERROR");
		break;
	default:
		THINGS_LOG_D(THINGS_ERROR, TAG, "Unknown");
		break;
	}
	if (gOTMEventHandler) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Call the registered funcation...");
		gOTMEventHandler(addr, port, uuid, event);
	}
}

//jun
int OICInitializeStack(const char* jsonPath, bool* easysetupCompleted)
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	if (isOICModuleInited) {
		THINGS_LOG(THINGS_ERROR, TAG, "Stack already initialized");
		return 0;
	}

	DALogInit();
	DALogSetVersion(OIC_STACK_FIRMWARE_VERSION);

	if (!DM_InitModule(jsonPath)) {
		THINGS_LOG(THINGS_ERROR, TAG, "DM_InitModule() failed");
		return 0;
	}
	isOICModuleInited = 1;

	/* If ocf_cloud.json exists and has mandatory elements for cloud, then it returns true. */
	*easysetupCompleted = DM_isThereOCFCloud();

	return 1;
}

int OICSendPushMessage(const char *push_uri, void *payload)
{
	if (payload == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "payload is NULL");
		return 0;
	}
	if (!PushNotificationToCloud(push_uri, (OCRepPayload *) payload)) {
		THINGS_LOG_D(THINGS_ERROR, TAG, "PushNotificationToCloud() failed");
		return 0;
	}
	return 1;
}

//jun
int OICDeinitializeStack()
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

	DM_TermiateModule();

	isOICModuleInited = 0;

	return 1;
}

//jun
int OICStartStack()
{
    OICRegisterSetAPConnectionFunc(_OCFAPI_WiFiConnectionCB);
    OICRegisterGetAPListFunc(_OCFAPI_WiFiSearchCB);

	if(wifi_manager_init(&wifi_callbacks) != WIFI_MANAGER_SUCCESS) {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize WiFi manager");
		return 0;
	}

	THINGS_LOG_D(THINGS_INFO, TAG, "THINGS_STACK version : %s", OIC_STACK_FIRMWARE_VERSION);
//jun
#if 0
    // 1. READ OCF_INFO File for stack intialization.
    if(!DM_InitModule(ResourcePath, InfoPath, CloudDBPath))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to parse OCF files");
        OICStop();
        return 0;
    }
#endif

    if(DM_GetEasySetupConnectivityType() == es_conn_type_SoftAP)
    {
        if (DM_isThereOCFCloud() == false){
            wifi_manager_softap_config_s* ap_config = DM_GetSoftAPWifiConfig();
            if (wifi_manager_set_mode(SOFTAP_MODE, ap_config) != WIFI_MANAGER_SUCCESS)
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to set AP config");
                return 0;
            }

            OICSetDeviceProperty(ap_config->ssid, WiFiMode, 4 , WIFI_FREQUENCY);
        }else{ // Alread Owned.
            wifi_manager_ap_config_s* ap_config = DM_getHomeAPWifiConfig();
            connectHomeAP(ap_config);
            OICSetDeviceProperty(ap_config->ssid, WiFiMode, 4 , WIFI_FREQUENCY);
        }
    }
    else if (DM_GetEasySetupConnectivityType() == es_conn_type_BLE)
    {

    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "es_conn_type_Unknown");
        return 0;
    }

    // 2. Enable Security Features
#ifdef __SECURED__
   // int authType = DM_GetOCFAuthType();
  //  THINGS_LOG_D(THINGS_DEBUG, TAG, "Controller User/Device Auth Type : %d", authType);
    if(0 != SM_InitOICSecurity(AUTH_CERTIFICATE_CONFIRM, DM_GetSVRDBFilePath()))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize OICSecurity features");
        OICStop();
        return 0;
    }
    SM_SetOTMEventHandler(_OTMEventCb);
	
#endif
    gReqHandler = GetHandlerInstance();
    if (NULL == gReqHandler)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Request Handler");
        OICStop();
        return 0;
    }
    // [JAY] Will be refactored to received callbacks for GET/PUT/POST/DELETE/OBSERVE ..
    gReqHandler->InitModule();

    // 4. Initiate Server Builder
    gServerBuilder = GetBuilderInstance();
    if(NULL == gServerBuilder)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Resource Server");
        OICStop();
        return 0;
    }

    gServerBuilder->InitModule(gServerBuilder, gReqHandler->EntityHandler);

#ifdef __SECURED__
    //5. Generate MAC based device UUID
    if(0 != SM_GenerateMacBasedDeviceId(false))
    {
        THINGS_LOG(THINGS_WARNING, TAG, "Failed to generate MAC based deviceID");
        OICStop();
        return 0;
    }
#endif

    // 6. Register Device-ID & Resources
    if( DM_RegisterDeviceID() == false )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to register Device ID");
        OICStop();
        return 0;
    }

    if(!DM_RegisterResource(gServerBuilder))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to register Resource");
        OICStop();
        return 0;
    }

    // 7. Initiate Easy-Setup & Login to Cloud
    if(ESM_OK != ESMInitEasySetup(0, gServerBuilder))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Easy-Setup Module");
        OICStop();
        return 0;
    }
    // Register Callback
    ESMRegisterCloudCb(cloudDataCbESM);

    if( ESCloud_Init(gServerBuilder) == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize Cloud");
        OICStop();
        return 0;
    }

    // // 7. Initiate Notification
    // if(!InitNotification())
    // {
    //     THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to start Notification");
    //     OICStop();
    //     return 0;
    // }

    isOICModuleInited = 1;

    //CloudRetrySignIN(NULL);

    gQuitFlag = 0;

    THINGS_LOG_D(THINGS_INFO, TAG, "Stack Initialization Success");
   return 1;
}

int OICReset(void* remoteOwner, ESEnrolleeReset resetType)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int res = -1;

    if( resetType >= RST_ENUM_EOF || resetType < RST_NEED_CONFIRM)
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Not support ESEnrolleeReset value(%d). So, Set value with RST_NEED_CONFIRM", resetType);
        resetType = RST_NEED_CONFIRM;
    }

    if( gConfirmResetStart == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Reset-Start confirmation Function is not registered. It's mandatory");
        return res;
    }

    pthread_mutex_lock(&mThreadOICReset);
    if( bThreadOICReset == false )
    {
        bThreadOICReset = true;
        ResetArgs* args = (ResetArgs*)DAMalloc(sizeof(ResetArgs));
        if( args == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to allocate ResetArgs memory");
            bThreadOICReset = false;
            res = -1;
            goto GOTO_OUT;
        }
        args->remoteOwner = (OicResource*)remoteOwner;
        args->resetType = resetType;

        bResetContinueFlag = true;
#ifdef OCF_RTOS
    	  if( pthread_create_rtos(&hThreadOICReset, NULL, (pthread_FuncType)tOICResetLoop, args, THINGS_STACK_RESETLOOP_THREAD) != 0 )
#else
        if( OICThread_create(&hThreadOICReset, NULL, (pthread_FuncType)tOICResetLoop, args) != 0 )
#endif
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Failed to create thread");
            hThreadOICReset = 0;
            DAFree(args);
            args = NULL;
            bThreadOICReset = false;
            res = -1;
            goto GOTO_OUT;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "Reset Thread handler = 0x%X", hThreadOICReset);
        res = 1;
    }
    else
    {
        THINGS_LOG(THINGS_INFO, TAG, "Already run Reset-Process.");
        res = 0;
    }
   
GOTO_OUT:
    pthread_mutex_unlock(&mThreadOICReset);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return res;
}

int OICStop(void)
{
    pthread_mutex_lock(&gOICStopMutex);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    gQuitFlag = 1;
    isOICModuleInited = 0;

    pthread_mutex_lock(&mThreadOICReset);
    if( bThreadOICReset == true )
    {
        bResetContinueFlag = false;
        pthread_join(hThreadOICReset, NULL);
        pthread_detach(hThreadOICReset);
        hThreadOICReset = 0;
        bThreadOICReset = false;
    }
    pthread_mutex_unlock(&mThreadOICReset);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate Cloud Session Managing");
    ESCloud_Terminate();

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate EasySetup");
    ESMTerminateEasySetup();

    // // Notification Stop
    // if(!DeinitNotification())
    // {
    //     THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to stop Notification");
    // }

#ifdef __SECURED__
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate OIC Security Manager");
    SM_TerminateOICSecurity();
#endif

    if(gServerBuilder != NULL)
    {
        gServerBuilder->DeinitModule(gServerBuilder);
        ReleaseBuilderInstance(gServerBuilder);
        gServerBuilder = NULL;
    }
    if(gReqHandler != NULL)
    {
        gReqHandler->DeinitModule();
        ReleaseHandlerInstance(gReqHandler);
        gReqHandler = NULL;
    }
    //jun
    //DM_TermiateModule();

    // [Jay] Need to add memory release for the Queue..
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    pthread_mutex_unlock(&gOICStopMutex);
    return 1;
}

int OICRegisterConfirmResetStartFunc( OICResetConfirmFuncType func)
{
    if(func != NULL)
    {
        gConfirmResetStart = func;
        return 1;
    }
    else
    {
        return 0;
    }
}


int OICNotifyObservers(const char* uri)
{
    if (gReqHandler == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "gReqHandler is NULL");
		return 0;
	}
	return gReqHandler->NotifyOicObservers(uri);
}

int OICRegisterUpdateDevProvDataFunc(OICUpdateDevProvDataFuncType func)
{
    if(func != NULL)
    {
        ESMRegisterUpdateDevProvDataFunc(func);
        return 1;
    }
    else
    {
        return 0;
    }
}

int OICRegisterSetAPConnectionFunc(OICSetAPConnectionFuncType func)
{
    // [JAY] Stack (Easy-setup) can request device to access specific AP
    //       with this registered callback
    if(func != NULL)
    {
        OICRegisterSetAPConFunc(func);
        return 1;
    }
    else
    {
        return 0;
    }
}

int OICWifiChangedCallFunc(int state, char* ap_name, char* ip_addr)
{
    int ret = 0;

    if( isOICModuleInited == 0 )
        return ret;

    pthread_mutex_lock(&gAppCBmutex);
#ifdef OCF_RTOS
    pthread_mutex_lock(&mThreadOICReset);
    ret = OICWifiStateChangedCB( state, ap_name, ip_addr );
    pthread_mutex_unlock(&mThreadOICReset);
#else
    ret = OICWifiStateChangedCB( state, ap_name, ip_addr );
#endif
    pthread_mutex_unlock(&gAppCBmutex);

    return ret;
}

#if 0
int OICSendThirdPartyPush(OICNotiMessageInfo* msgInfo)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    OCRepPayload* payload = NULL;
    OCRepPayload* data = NULL;

    if(ESGetCloudLogINState())
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Pushing to cloud => /service/pushservice");

        payload = OCRepPayloadCreate();
        data = OCRepPayloadCreate();

        const char *peerID = OCGetServerInstanceIDString();

        if(NULL == peerID || NULL == msgInfo->uri
            || NULL == msgInfo->eventType || NULL == msgInfo->message)
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Push Data is NULL");
            OCRepPayloadDestroy(payload);
            OCRepPayloadDestroy(data);
            return 0;
        }

        OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, 0); // pushservice no need messageid it is just mandatory filed!
        OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, peerID);
        OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_SOURCE, msgInfo->uri);
        OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_TYPE, msgInfo->eventType);
        OCRepPayloadSetPropString(payload, KEY_PUSH_CODE, msgInfo->message);

        OCRepPayloadSetPropString(data, KEY_PUSH_DATA_ET, msgInfo->data_et);
        OCRepPayloadSetPropObject(payload, KEY_PUSH_DATA, data);

        THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", NS_ATTRIBUTE_PROVIDER_ID, peerID);
        THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", NS_ATTRIBUTE_SOURCE, msgInfo->uri);
        THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", NS_ATTRIBUTE_TYPE, msgInfo->eventType);
        THINGS_LOG_D(THINGS_INFO, TAG, "[%s] : [%s]", KEY_PUSH_CODE, msgInfo->message);
        THINGS_LOG_D(THINGS_INFO, TAG, "KEY_PUSH_DATA");
        THINGS_LOG_D(THINGS_INFO, TAG, "  [%s] : [%s]", KEY_PUSH_DATA_ET, msgInfo->data_et);

        if(!PushNotificationToCloud(URI_PUSHSERVICE, payload))
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "PushNotificationToCloud return Error");
            OCRepPayloadDestroy(data);
            return 0;
        }
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to send Push Noti. message to Cloud due to not connected");
        return 0;
    }

    OCRepPayloadDestroy(data);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return 1;
}
#endif


int OICRegisterStopSoftAPFunc(OICStopSoftAPFuncType func)
{
    if( NULL != func )
    {
        return RegisterStopSoftAPCB(func);
    }
    else
    {
        return 0;
    }
}

int OICRegisterPINGeneratedFunc(OICPINGeneratedFuncType func)
{
    if( NULL != func )
    {
#ifdef __SECURED__
        return ESMRegisterPINGeneratedCB(func);
#else
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
        return 0;
#endif
    }
    else
    {
        return 0;
    }
}

int OICRegisterPINDisplayCloseFunc(OICPINDisplayCloseFuncType func)
{
    if( NULL != func )
    {
#ifdef __SECURED__
        return ESMRegisterPINCloseCB(func);
#else
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
        return 0;
#endif
    }
    else
    {
        return 0;
    }
}

int OICRegisterUserConfirmFunc(OICGetUserConfirmFuncType func)
{
    if( NULL != func )
    {
#ifdef __SECURED__
        return ESMRegisterUserConfirmCB(func);
#else
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
        return 0;
#endif
    }
    else
    {
        return 0;
    }
}

#ifndef OCF_RTOS
int OICRegisterBIXBYReqHandleFunc(OICBIXBYReqHandleFuncType func)
{
    if( NULL != func )
    {
        return RegisterBixbyHandleCB(func);
    }
    else
    {
        return 0;
    }
}
#endif

//jun
int OICRegisterHandleRequestFunc(OICHandleRequestFuncType getFunc, OICHandleRequestFuncType setFunc)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
	if ((NULL == getFunc) || (NULL == setFunc)) {
		THINGS_LOG(THINGS_ERROR, TAG, "Callback is NULL");
		return 0;
	}
	RegisterHandleRequestFunc(getFunc, setFunc);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
// Functions Definition for Security feature
//////////////////////////////////////////////////////////////////////////
//jun
int OICRegisterOTMEventHandler(OICSecOTMStateFuncType otmEventCallback)
{
	THINGS_LOG(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
#ifdef __SECURED__
	if (otmEventCallback == NULL) {
		THINGS_LOG(THINGS_ERROR, TAG, "otmEventCallback is NULL");
		return 0;
	}
	gOTMEventHandler = otmEventCallback;
#else
	THINGS_LOG(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
	return 0;
#endif
	return 1;
}
#if 0
int OICRegisterOTMEventHandler(OICSecOTMStateFuncType otmEventCallback)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "In OICRegisterOTMEventHandler");
#ifdef __SECURED__
    SM_SetOTMEventHandler(otmEventCallback);
#else
    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
    THINGS_LOG(THINGS_DEBUG, TAG, "Out OICRegisterOTMEventHandler");
}
#endif

int OICSaveACL(const char* uuid, const char* resourceUri, const char* resourceType, const char* interfaceName, uint16_t permission)
{
    int ret = -1;
    THINGS_LOG(THINGS_DEBUG, TAG, "In OICSaveACL");
#ifdef __SECURED__
    ret = SM_SaveAcl(uuid, resourceUri, resourceType, interfaceName, permission);
#else
    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
    THINGS_LOG(THINGS_DEBUG, TAG, "Out OICSaveACL");
    return ret;
}

int OICSavePreconfiguredPin(const char* pin)
{
    int ret = -1;
    THINGS_LOG(THINGS_DEBUG, TAG, "In OICSavePreconfiguredPin");
#ifdef __SECURED__
    ret = SM_SavePreconfiguredPin(pin);
#else
    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
    THINGS_LOG(THINGS_DEBUG, TAG, "Out OICSavePreconfiguredPin");
    return ret;
}

int OICSetMOTStatus(bool enable)
{
    int ret = -1;
    THINGS_LOG(THINGS_DEBUG, TAG, "In OICSetMOTStatus");
#ifdef __SECURED__
    ret = SM_SetMOTStatus(enable);
#else
    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Stack is in UNSECURED Mode");
#endif
    THINGS_LOG(THINGS_DEBUG, TAG, "Out OICSetMOTStatus");
    return ret;
}

//////////////////////////////////////////////////////////////////////////
// Functions Definition for Reset Handling.
//////////////////////////////////////////////////////////////////////////
static void OICSetResetMask(RST_STATE value)
{
    if( value != RST_COMPLETE )
        mResetBitMask |= value;
    else
        mResetBitMask = value;
}

bool OICGetResetMask(RST_STATE value)
{
    return (int)(mResetBitMask & value) != 0 ? true : false;
}

void OICControlQueueEmpty(void)
{
    if( OICGetResetMask(RST_CONTROL_MODULE_DISABLE) == true )
        OICSetResetMask(RST_CONTROL_QUEUE_EMPTY);
}

int OICReturnUserOpinion4Reset(int bResetStart) // bResetStart : User opinion.
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int result = 0;

    if( bResetStart == true && bResetContinueFlag == true )   // User allow Reset-Starting.
    {
        OICSetResetMask(RST_USER_CONFIRM_COMPLETED);
        result = 1;
    }
    else    // User not allow Reset.
    {
        bResetContinueFlag = false;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "User Opinion : %d", result);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return result;
}

static void* __attribute__((optimize("O0"))) tOICResetLoop(ResetArgs* Args)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter. Handler=0x%X", hThreadOICReset);

    int result = 0;
    OICResetResultFuncType THINGS_CBFunc4ResetResult = NULL;

    if( Args == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Args is NULL. It's mandatory.");
        goto GOTO_OUT;
    }

    if( gConfirmResetStart == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Reset-Start confirmation Function is not registered. It's mandatory.");
        goto GOTO_OUT;
    }

    OICSetResetMask(RST_COMPLETE);
    THINGS_LOG(THINGS_DEBUG, TAG, "Start Reset Processing.");

    // 0. Trigger User-Confirm GUI and Wait for getting User-Opinion with regard to Reset.
    THINGS_LOG(THINGS_DEBUG, TAG, "Trigger Reset-GUI.");
    if( gConfirmResetStart(&THINGS_CBFunc4ResetResult, Args->resetType) == 0 )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "User don't want to reset. So reset canceled.");
        goto GOTO_OUT;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Waiting User-Opinion for Reset-Start.");
    while( bResetContinueFlag == true && OICGetResetMask(RST_USER_CONFIRM_COMPLETED) == false)  // It's Blocking until receive user-opinion.
    {
        usleep(300000);     // 0.3 seconds wait.
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Break-Stop Check.");
    if( bResetContinueFlag == false )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Called Reset break-STOP.");
        goto GOTO_OUT;
    }

    // 1. Easy Setup : Provisioning Disable.
    // 2. Controlling Module : Disable pushing request to Queue.
    THINGS_LOG(THINGS_DEBUG, TAG, "Disable Modules.(EasySetup/Controlling Module)");
    OICSetResetMask(RST_EASY_SETUP_DISABLE | RST_CONTROL_MODULE_DISABLE);

    // remove this code because ARTIK051 remove MessageHandlingLoop
//    // 3. Waiting until processing-done already exist request in Queue.
//    THINGS_LOG(THINGS_DEBUG, TAG, "Waiting Queue-Empty of Controlling Module.");
//    while( bResetContinueFlag == true && OICGetResetMask(RST_CONTROL_QUEUE_EMPTY) == false)
//    {
//        usleep(300000);     // 0.3 seconds wait.
//    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Break-Stop Check.");
    if( bResetContinueFlag == false )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Called Reset break-STOP.");
        goto GOTO_OUT;
    }

    // 4. Notification Push Disable.
    THINGS_LOG(THINGS_DEBUG, TAG, "Disable Notification Module.");
    OICSetResetMask(RST_NOTI_MODULE_DISABLE);

   OCClearObserverlist();     // delete All Observer. (for remote Client)

    // 5. Cloud Manager : Terminate
    THINGS_LOG(THINGS_DEBUG, TAG, "Terminate Cloud Module.");
    ESCloud_Terminate();

    // 6. Easy Setup : Terminate
    THINGS_LOG(THINGS_DEBUG, TAG, "Terminate Easy-Setup Module.");
    if( ESMTerminateEasySetup() != ESM_MODULE_TERMINATED)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module termination failed");
        goto GOTO_OUT;
    }

    // 7. Delete Cloud data in "ocf_cloud.json"
    THINGS_LOG(THINGS_DEBUG, TAG, "Delete Cloud-Data in ocf_cloud.json file.");
    if( DM_DelOCFCloudData() == 0 )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "It's failed to delete information about Cloud-Data in \"ocf_infoX.json\" file.");
        goto GOTO_OUT;
    }

    OCClearCallBackList();    // delete All Client Call-Back List. (for SET Self-Request)

    // 8. Security Reset.
#ifdef __SECURED__
    if(0 != SM_ResetSvrDb())
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to initialize OICSecurity features.");
        goto GOTO_OUT;
    }

    if(0 != SM_GenerateMacBasedDeviceId(true))
    {
        THINGS_LOG(THINGS_WARNING, TAG, "Failed to generate MAC based deviceID.");
        goto GOTO_OUT;
    }
#else
    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "NOT SECURED MODE!!!!! ");
#endif

    // 9. Easy Setup : Initialization.
    THINGS_LOG(THINGS_DEBUG, TAG, "Initial Easy-Setup Module.");
    if(ESM_OK != ESMInitEasySetup(0, gServerBuilder))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module initialization failed");
        goto GOTO_OUT;
    }
    ESMRegisterCloudCb(cloudDataCbESM);

    // 10. Cloud Manager : Initialization.
    THINGS_LOG(THINGS_DEBUG, TAG, "Initial Cloud Module.");
    if( ESCloud_Init(gServerBuilder) == NULL )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "It's failed Cloud_Module initialization.");
        goto GOTO_OUT;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Reset Success.");
    result = 1;

GOTO_OUT:
    // 11. All Module Enable.
    OICSetResetMask(RST_COMPLETE);

		/*SVACE warning fix*/
    if(Args){
		  // 12. If reset-processing is triggered by remote-owner, then Notify result of Reset.
		  THINGS_LOG(THINGS_DEBUG, TAG, "Notify result of reset to remote-client.(mobile)");
		  NotifyResultOfReset(Args->remoteOwner, result);
    }

    // 13. If there is Reset Result Call-Back Function, then Notify result of Reset to DA-Application.
    if( THINGS_CBFunc4ResetResult )
    {   // Notify result of reset to DA.
        THINGS_LOG(THINGS_DEBUG, TAG, "Notify result of reset to DA.");
        THINGS_CBFunc4ResetResult(result);
    }

    if( Args )
    {
        DAFree( Args );
        Args = NULL;
    }
    pthread_mutex_lock(&mThreadOICReset);
    hThreadOICReset = 0;
    bThreadOICReset = false;
    pthread_mutex_unlock(&mThreadOICReset);
    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return NULL;
}

/**
* @brief : CB registration function for retrieving contents metadata for providing
* @param CIinfo : Cloud information for Cloud Connection.
* @param func   : Call-Back Function pointer is called when finish Cloud connection.
* @return : 1 (success), 0 (failed), -1 (Try Again, Now Reset Processing)
*/
int OICCloudSignUP(OICCloudInfo* CIinfo, OICCloudConResultFuncType func)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int res = 0;
    int CIport = 0;
    if( CIinfo == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "CIinfo is NULL.");
        return 0;
    }

    if( CIinfo->access_token == NULL || CIinfo->auth_provider == NULL || CIinfo->client_id == NULL || CIinfo->refresh_token == NULL || CIinfo->user_id == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid input argument.(access_token=0x%X, auth_provider=0x%X, client_id=0x%X, refresh_token=0x%X, user_id=0x%X)"
                                               , CIinfo->access_token, CIinfo->auth_provider, CIinfo->client_id, CIinfo->refresh_token, CIinfo->user_id);
        return 0;
    }

    if( CIinfo->domain == NULL && CIinfo->ip == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Your must insert domain or IP.");
        return 0;
    }

    if( bThreadOICReset != false )
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Now, Doing Reset Process. Please Check your system.");
        return -1;
    }

    // Disable Easy Setup Module.
    OICSetResetMask(RST_EASY_SETUP_DISABLE);

    CIStopCloudConnection( func );

    if( StartCloudProvisioning((void*)CIinfo) == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Success trigger Cloud Sign-UP.");
        res = 1;
    }

    // Enable Easy Setup Module.
    OICSetResetMask(RST_COMPLETE);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}

/**
* @brief : CB registration function for retrieving contents metadata for providing
* @param func   : Call-Back Function pointer is called when finish Cloud connection.
* @return : 1 (success), 0 (failed), -1 (Try Again, Now Reset Processing)
*/
int OICCloudSignIN( OICCloudConResultFuncType func )
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    int res = 0;

    if( ESGetCloudLogINState() == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Already Sign-IN Completed.");
        return 1;
    }

    if( bThreadOICReset != false )
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Now, Doing Reset Process. Please Check your system.");
        return -1;
    }

    // Disable Easy Setup Module.
    OICSetResetMask(RST_EASY_SETUP_DISABLE);

    CIStopCloudConnection( func );
    CICPDelPendedData();
    CICPDelIsThereCP();

    if( CloudRetrySignIN(NULL) == 0 )    // default. Cloud retry.
    {
        res = 1;
    }

    // Enable Easy Setup Module.
    OICSetResetMask(RST_COMPLETE);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return res;
}

/**
* @brief : CB registration function for retrieving contents metata for providing
*/
void OICCloudSignOUT( void )
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    CIStopCloudConnection(NULL);

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

int OICSetDeviceProperty(const char* name, const WIFI_MODE* mode, const int ea_mode, const WIFI_FREQ freq)
{
    return ESMSetDevicePropertyByApp(name, mode, ea_mode, freq);
}

void OICRegisterEasySetupAbortFunc( OICAbortEasySetupFuncType func )
{
    gAbortEasySetup = func;
}

OCEntityHandlerResult OICAbort(pthread_t* hThreadAbort, ESEnrolleeAbort level)
{
    SAbort_s* ARGs = NULL;
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if( hThreadAbort == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "hThreadAbort is NULL.");
        return ehResult;
    }

    if( (*hThreadAbort) == 0)
    {
        if( (ARGs = (SAbort_s*)DAMalloc(sizeof(SAbort_s))) == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.(for abort Thread)");
            return OC_EH_ERROR;
        }

        ARGs->hThread = hThreadAbort;
        ARGs->level = level;

        ehResult = OC_EH_OK;
#ifdef OCF_RTOS

        if( pthread_create_rtos(hThreadAbort, NULL, (pthread_FuncType)tOICAbortLoop, ARGs, THINGS_STACK_OICABORT_THREAD) != 0 )
#else
        if( OICThread_create(hThreadAbort, NULL, (pthread_FuncType)tOICAbortLoop, ARGs) != 0 )
#endif

        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.(for abort Thread)");
            *hThreadAbort = 0;
            ehResult = OC_EH_ERROR;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Already called Thread. So, OC_EH_NOT_ACCEPTABLE");
        ehResult = OC_EH_NOT_ACCEPTABLE;
    }

    return ehResult;
}

static void* __attribute__((optimize("O0"))) tOICAbortLoop(SAbort_s* contents)
{
    THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

    if( contents == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "contents is NULL.");
        goto GOTO_OUT;
    }

    if( gAbortEasySetup != NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Registered Abort Function. So, Call Function DA API.");
        gAbortEasySetup(contents->level);
    }

GOTO_OUT:
    if( contents )
    {
        *(contents->hThread) = 0;
        DAFree(contents);
        contents = NULL;
    }

    THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
    return NULL;
}