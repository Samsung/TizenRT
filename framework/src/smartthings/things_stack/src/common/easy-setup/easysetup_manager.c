#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE // for the usleep

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#ifdef OCF_RTOS
#include <errno.h>
#include <sys/select.h>
#endif

#include "oicnetwork.h"
#include "oicwaithandler.h"
#include "easysetup_manager.h"
#include "resourcehandler.h"
#include "cloud_manager.h"
#include "easysetup.h"
#include "thingslogger.h"
#include "thingsmalloc.h"

#include "oicthread.h"
#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif

#ifdef __SECURED__
#include "pinoxmcommon.h"
#include "oxmverifycommon.h"
#endif  // #ifdef __SECURED__

#define TAG "[ezsetup-mg]"
#define MAX_CON_ATTAMPT 20  // Connection Checking for 20 sec in total.
#define MAX_REFRESHCHECK_CNT    30   // 30 times

ESDevConfProvData *gDevConfProvData = NULL;
ESWiFiProvData *gWiFiProvData = NULL;
ESCloudProvData *gCloudProvData = NULL;

static OICServerBuilder*            gSERVERBuilder = NULL;
static OICUpdateDevProvDataFuncType gUpdateDevProvData = NULL;
static PINGeneratedFuncType         gPINGeneratedCB = NULL;
static PINCloseFuncType             gPINCloseCB = NULL;
static UserConfirmResultFuncType    gUserConfirmCB = NULL;

int ESMContinue = 0;
#ifdef OCF_RTOS
int CITokenExpireFds[2] = {-1, -1};
#endif
static pthread_t gthreadId_NetworkStatusCheck = 0;
static pthread_t gthreadId_CloudRefreshCheck = 0;

static const int iCommonSleepSec = CI_TOKEN_EXPIRECHECK_TIME;
static const int iFailSleepSec = 60;     // 60 sec
static pthread_mutex_t gESmutex = PTHREAD_MUTEX_INITIALIZER;

static bool isSetDeviceProperty = false;
static ESDeviceProperty deviceProperty;
static const char* defDeviceName = "Test Device";

void WiFiProvCbInApp(ESWiFiProvData *eventData);
void DevConfProvCbInApp(ESDevConfProvData *eventData);
void CloudDataProvCbInApp(ESCloudProvData *eventData);

typedef void (*CallbackToApp) (ESCloudProvData* );
CallbackToApp gCloudDataCbToApp = NULL;

ESProvisioningCallbacks gCallbacks = {
    .WiFiProvCb = &WiFiProvCbInApp,
    .DevConfProvCb = &DevConfProvCbInApp,
    .CloudDataProvCb = &CloudDataProvCbInApp
};

static void* WifiProvSetLoop(void *param);
static void* CloudRefreshCheckLoop(void* param);
static void* WifiProvTimeOutHandler(TimeOut_s* param);


#ifdef __SECURED__
void GeneratePinCB(char* pin, size_t pinSize)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(NULL == pin || pinSize <= 0)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "INVALID PIN");
        return;
    }

    THINGS_LOG_V(THINGS_INFO, TAG, "PIN CODE : %s", pin);

    // TODO: print out pin number using display panel - ysy
    if(gPINGeneratedCB)
    {
        gPINGeneratedCB(pin, pinSize);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "PIN GENERATED CB Not Registered~!!!");
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

void PinCloseCB(void)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    // TODO: print out pin number using display panel - ysy
    if(gPINCloseCB)
    {
        gPINCloseCB();
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "PIN GENERATED CB Not Registered~!!!");
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}


OCStackResult GetUserConfirmation(void * ctx)
{
    OCStackResult ret = OC_STACK_USER_DENIED_REQ;

    if(gUserConfirmCB)
    {
        int result = gUserConfirmCB();

        switch(result)
        {
            case 1:
            {
                THINGS_LOG_V(THINGS_INFO, TAG, "User Confirmed~!!");
                ret = OC_STACK_OK;
                break;
            }
            default:
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "User Denied~!! : [ %d ]", result);
                break;
            }
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "MUTUAL AUTHENTICATION CONFIRM CB Not Registered~!!!");
    }

    THINGS_LOG_V(THINGS_INFO, TAG, "User Confirm Result : [ %d ]", ret);

    return ret;
}

#endif //#ifdef __SECURED__

/*******************************************************************
 *
 * DA API Function register
 *
 *******************************************************************/
void ESMRegisterUpdateDevProvDataFunc(OICUpdateDevProvDataFuncType func)
{
    gUpdateDevProvData = func;
}

int ESMSetDevicePropertyByApp(char* name, const WIFI_MODE* mode, int ea_mode, const WIFI_FREQ freq)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    int i=0;

    if( mode == NULL || ea_mode < 1 || WiFi_24G > freq || freq >= WiFi_FREQ_EOF)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Input Arguments.(mode=0x%X, ea_mode=%d, freq=%d)", mode, ea_mode, freq);
        return 0;
    }

    if( name == NULL )
        name = defDeviceName;

    if( ea_mode > NUM_WIFIMODE-1)
        ea_mode = NUM_WIFIMODE-1;

    DAStrncpy(deviceProperty.DevConf.deviceName, name, sizeof(deviceProperty.DevConf.deviceName));
    deviceProperty.WiFi.freq = freq;
    for(i=0; i < ea_mode; i++)
        deviceProperty.WiFi.mode[i] = mode[i];
    deviceProperty.WiFi.mode[ea_mode] = WiFi_EOF;
    isSetDeviceProperty = true;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "deviceName=%s, WiFi_frequence=%d, count of WiFi_mode=%d", deviceProperty.DevConf.deviceName, deviceProperty.WiFi.freq, ea_mode);
    for(i=0; i <= ea_mode; i++)
        THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi_mode[%d]=%d", i, deviceProperty.WiFi.mode[i]);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", isSetDeviceProperty);
    return 1;
}

ESMResult ESMInitEasySetup(int restartFlag, OICServerBuilder* serverBuilder)
{
    THINGS_LOG_D(THINGS_INFO, TAG, "Init EasySetup");

    if( isSetDeviceProperty == false )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error] Please Check whether you call \"OICSetDeviceProperty\" API.(We need value of WiFi-property.)");
        return ESM_ERROR;
    }

    if(restartFlag == 1 || gDevConfProvData || gWiFiProvData || gCloudProvData )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Restart EasySetup");
        ESMContinue = 0;
        if(gthreadId_CloudRefreshCheck)
        {
#ifdef OCF_RTOS
            if (CITokenExpireFds[1] != -1)
            {       
                ssize_t len = 0;
                do
                {
                    len = write(CITokenExpireFds[1], "w", 1);
                } while ((len == -1) && (errno == EINTR));
                if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
                {
                    THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "write failed: %s", strerror(errno));
                    return ESM_ERROR;
                }
            }
            pthread_join(gthreadId_CloudRefreshCheck, NULL);         
            close(CITokenExpireFds[0]);
            close(CITokenExpireFds[1]);
            CITokenExpireFds[0] = -1;
            CITokenExpireFds[1] = -1;
#else
            pthread_join(gthreadId_CloudRefreshCheck, NULL);
#endif
            gthreadId_CloudRefreshCheck = 0;
        }

        if( gthreadId_NetworkStatusCheck )
        {
            pthread_join(gthreadId_NetworkStatusCheck, NULL);
            gthreadId_NetworkStatusCheck = 0;
        }

        if(ESTerminateEnrollee() == ES_ERROR)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Init EasySetup Failed");
            return ESM_ERROR;
        }

    if(gDevConfProvData)
    {
        DAFree(gDevConfProvData);
        gDevConfProvData = NULL;
    }

    if(gWiFiProvData)
    {
        DAFree(gWiFiProvData);
        gWiFiProvData = NULL;
    }

    if(gCloudProvData)
    {
        DAFree(gCloudProvData);
        gCloudProvData = NULL;
    }
    }

    gDevConfProvData = (ESDevConfProvData *) DAMalloc(sizeof(ESDevConfProvData));
    gWiFiProvData = (ESWiFiProvData *) DAMalloc(sizeof(ESWiFiProvData));
    gCloudProvData = (ESCloudProvData *) DAMalloc(sizeof(ESCloudProvData));

    if( gDevConfProvData == NULL || gWiFiProvData == NULL || gCloudProvData == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Easy Setup Memory allocation is failed.");
        return ESM_ERROR;
    }

//    ESDeviceProperty deviceProperty = {
//        {{WiFi_11G, WiFi_11N, WiFi_11AC, WiFi_EOF}, WiFi_5G}, {"Test Device"}
//    };

    if( serverBuilder != NULL )
        gSERVERBuilder = serverBuilder;

    if( gSERVERBuilder == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] gServerBuilder is NULL.");
        return ESM_ERROR;
    }

#ifdef __SECURED__
    bool gIsSecured = true;
#else
    bool gIsSecured = false;
#endif

    ESResourceMask resourcemMask = ES_WIFI_RESOURCE | ES_CLOUD_RESOURCE | ES_DEVCONF_RESOURCE;
    // Easy Setup Module Initiation

    if(ESInitEnrollee(gIsSecured, resourcemMask, gCallbacks) != ES_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "StartEasySetup and onboarding Fail!!");
        return ESM_ERROR;
    }

    ESSetState(ES_STATE_INIT);
    ESSetErrorCode(ES_ERRCODE_NO_ERROR);

    if(ESSetDeviceProperty(&deviceProperty) == ES_ERROR)
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "ESSetDeviceProperty funtion Failed");

#ifdef __SECURED__

    if(OC_STACK_OK != SetRandomPinPolicy(8, NUM_PIN))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Set to Non-Numerical PIN Generation~!!!!!");
    }
    else
    {
        THINGS_LOG(THINGS_INFO, TAG, "Set to Numerical PIN Generation~!!!!!");
    }

    SetGeneratePinCB(&GeneratePinCB);
    SetClosePinDisplayCB(&PinCloseCB);
    SetUserConfirmCB(NULL, GetUserConfirmation);
    SetVerifyOption(USER_CONFIRM);

#endif //#ifdef __SECURED__
    if(gthreadId_CloudRefreshCheck == 0)
	{
        int ret = pipe(CITokenExpireFds);
        if (-1 == ret)
	    {
	        close(CITokenExpireFds[0]);
	        close(CITokenExpireFds[1]);
	        CITokenExpireFds[0] = -1;
	        CITokenExpireFds[1] = -1;
	        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "pipe failed: %s", strerror(errno));
            return ESM_ERROR;
	    }
        ESMContinue = 1;
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Create CloudRefreshCheckLoop thread");
#ifdef OCF_RTOS
		pthread_create_rtos(&gthreadId_CloudRefreshCheck,NULL, CloudRefreshCheckLoop, (void *)&ESMContinue, THINGS_STACK_CLOUD_REFRESH_THREAD);
#else
		OICThread_create(&gthreadId_CloudRefreshCheck, NULL, CloudRefreshCheckLoop, (void *)&ESMContinue);
#endif
	}
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return ESM_OK;
}

ESMResult ESMTerminateEasySetup()
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate EasySetup");
    ESMContinue = 0;
    if(gthreadId_CloudRefreshCheck)
    {
#ifdef OCF_RTOS
        if (CITokenExpireFds[1] != -1)
        {  
            ssize_t len = 0;   
            do
            {
                len = write(CITokenExpireFds[1], "w", 1);
            } while ((len == -1) && (errno == EINTR));
            if ((len == -1) && (errno != EINTR) && (errno != EPIPE))
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "write failed: %s", strerror(errno));
                return ESM_ERROR;
            }
        }
        pthread_join(gthreadId_CloudRefreshCheck, NULL);
        close(CITokenExpireFds[0]);
        close(CITokenExpireFds[1]);
        CITokenExpireFds[0] = -1;
        CITokenExpireFds[1] = -1;
#else
        pthread_join(gthreadId_CloudRefreshCheck, NULL);
#endif
        gthreadId_CloudRefreshCheck = 0;
    }
    if(gthreadId_NetworkStatusCheck)
    {
        pthread_join(gthreadId_NetworkStatusCheck, NULL);
        gthreadId_NetworkStatusCheck = 0;
    }

    if(ESTerminateEnrollee() != ES_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Terminate EasySetup Error");
        return ESM_ERROR;
    }

    if(gDevConfProvData)
    {
        DAFree(gDevConfProvData);
        gDevConfProvData = NULL;
    }

    if(gWiFiProvData)
    {
        DAFree(gWiFiProvData);
        gWiFiProvData = NULL;
    }

    if(gCloudProvData)
    {
        DAFree(gCloudProvData);
        gCloudProvData = NULL;
    }

    gSERVERBuilder = NULL;
    ESMContinue = 0;

    return ESM_MODULE_TERMINATED;
}

static int checkRefresh(int* hourCnt24)
{
    OCStackResult ret = OC_STACK_ERROR;
    int sleepSec = 0;

    if( *hourCnt24 >= CI_COMMON_REFRESH_TIME )
    {
        if( (ret = RefreshTokenIntoCloud()) != OC_STACK_OK )
            THINGS_LOG_D(THINGS_DEBUG, TAG, "RefreshTokenIntoCloud is failed in CloudRefreshCheckLoop");
        else
            *hourCnt24 = 0;
    }else
    {
       // Find Resource & if accesstoken is expireded then refresh process is called.
        if( (ret = FindCloudResource()) != OC_STACK_OK )
            THINGS_LOG_D(THINGS_DEBUG, TAG, "FindCloudResource is failed in CloudRefreshCheckLoop");
    }

    if( ret != OC_STACK_OK)
    {
        sleepSec = iFailSleepSec;
    }else
    {
       sleepSec = iCommonSleepSec;
       (*hourCnt24)++;
    }

    return sleepSec;
}

static void* CloudRefreshCheckLoop(void* param)
{
#ifdef OCF_RTOS
    THINGS_LOG_D(THINGS_DEBUG, TAG, "CloudRefreshCheckLoop Close Loop Enter.");
    int iFailedCnt = 0;
    int * Continue = (int*)param;
    int hourCnt24 = 0;
    int ret = -1;
    fd_set readFds;
    struct timeval timeout;

    timeout.tv_sec = iCommonSleepSec;
    timeout.tv_usec = 0;
    int maxFd = CITokenExpireFds[0];
    THINGS_LOG_D(THINGS_DEBUG, TAG, "device ID = %s", OCGetServerInstanceIDString());

    // Close Loop Thread.
    while(*Continue)
    {
        if( ESGetState() != ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD )
        {
            hourCnt24++;
            timeout.tv_sec = iCommonSleepSec;
            FD_ZERO(&readFds);
            FD_SET(CITokenExpireFds[0], &readFds);
            ret = select(maxFd + 1, &readFds, NULL, NULL, &timeout);
            if (ret == 0)
                continue;
            if (FD_ISSET(CITokenExpireFds[0], &readFds))
            {
                char buf[2] = {0};
                ssize_t len = read(CITokenExpireFds[0], buf, sizeof(buf));
                if (len > 0 && buf[0] == 'w')
                    break;
                continue;
            }
            continue;
        }
        if( (timeout.tv_sec = checkRefresh(&hourCnt24)) == iFailSleepSec )
            iFailedCnt++;
        else
            iFailedCnt = 0;

        if( iFailedCnt >= MAX_REFRESHCHECK_CNT )
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Refresh checking routin is Failed. Return cloud state to INIT.");
            iFailedCnt = 0;
            ESMGetNetworkStatus();  // State return
        }
        FD_ZERO(&readFds);
        FD_SET(CITokenExpireFds[0], &readFds);
        ret = select(maxFd + 1, &readFds, NULL, NULL, &timeout);
        if (ret == 0)
            continue;
        if (FD_ISSET(CITokenExpireFds[0], &readFds))
        {
            char buf[2] = {0};
            ssize_t len = read(CITokenExpireFds[0], buf, sizeof(buf));
            if (len > 0 && buf[0] == 'w')
                break;
        }
    }
    FD_CLR(CITokenExpireFds[0], &readFds);
    FD_CLR(CITokenExpireFds[1], &readFds);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "CloudRefreshCheckLoop Close Loop Exit.");
    return NULL;
#else
    THINGS_LOG_D(THINGS_DEBUG, TAG, "CloudRefreshCheckLoop Close Loop Enter.");

    int iFailedCnt = 0;
    int sleepSec = iCommonSleepSec;
    int * Continue = (int*)param;
    int hourCnt24 = 0;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "device ID = %s", OCGetServerInstanceIDString());

    // Close Loop Thread.
    while(*Continue)
    {
        if( ESGetState() != ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD )
        {
            hourCnt24++;
            sleepSec = iCommonSleepSec;
            sleep(sleepSec);
            continue;
        }

        if( (sleepSec = checkRefresh(&hourCnt24)) == iFailSleepSec )
            iFailedCnt++;
        else
            iFailedCnt = 0;

        if( iFailedCnt >= MAX_REFRESHCHECK_CNT )
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Refresh checking routin is Failed. Return cloud state to INIT.");
            iFailedCnt = 0;
            ESMGetNetworkStatus();  // State return
        }

        sleep(sleepSec);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "CloudRefreshCheckLoop Close Loop Exit.");

    return NULL;
#endif
}

static int OICUpdateDevProvData( ESDevConfProvData * DevProvData )
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    if( DevProvData == NULL )
        return -1;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Country=%s, DateTime=%s, Language=%s", DevProvData->country, DevProvData->datetime, DevProvData->language);

    if( gUpdateDevProvData == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "DA API is not registered.(gUpdateDateTime)");
        return -1;
    }

    return gUpdateDevProvData( DevProvData );
}

static void* WifiProvSetLoop(void *param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Open Loop Enter.");

    static int timeoutHandleNum=100;
    TimeOut_s timeout;

    ESSetState(ES_STATE_CONNECTING_TO_ENROLLER);
    SetWiFiProvState(WIFI_READY);

    while(GetWiFiProvState() == WIFI_READY)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "waiting... WIFI_RESPONSE-Done for wifi provisioning.");
        usleep(50000);
    }

    if(GetWiFiProvState() != WIFI_RESPONSE)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "WiFi Response is failed.");
        SetWiFiProvState(WIFI_INIT);
        DAFree((AccessPointInfo1*)param);
        return 0;
    }

    // Notify DevConfProvData to THINGS_App.
    OICUpdateDevProvData(gDevConfProvData);

    usleep(50000);

    int res = OICSetAPConnection((AccessPointInfo1*)param );
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Set AP Connection Result : %d", res);

    if(res != 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Wifi Provisioning is Failed");
        ESSetState(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
        DAFree((AccessPointInfo1*)param);
        SetWiFiProvState(WIFI_INIT);
        return 0;
    }

    OICWifiStateChangedCB_init();
    SetWiFiProvState(WIFI_SET);
    DAFree((AccessPointInfo1*)param);

    if( AddRequestHandle((OCDoHandle)timeoutHandleNum) != NULL )
    {
        timeout.cur_num = 300;  // 5 min    because, DA-App wait AP-Connecting for 5 min.
        timeout.cur_counter = timeout.cur_num;
        CreateTimeOutProcess((OCDoHandle)timeoutHandleNum++, WifiProvTimeOutHandler, &timeout);
    }

    gthreadId_NetworkStatusCheck = 0;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Open Loop Exit.");

    return NULL;
}

static void* WifiProvTimeOutHandler(TimeOut_s* param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi Setting of Provisioning data was Time-Out.");

    ESSetState(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
    ESSetErrorCode(ES_ERRCODE_NO_INTERNETCONNECTION);

    THINGS_LOG_ERROR(THINGS_ERROR, TAG, "ESSetState ES_STATE_CONNECTED_FAIL_TO_ENROLLER");

    if(ESM_OK != ESMInitEasySetup(1, NULL))
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Easy-Setup Module Initialization failed");
        OICStop();
        return NULL;
    }

    PROFILING_TIME("WiFi Provisioning End.");

    SetWiFiProvState(WIFI_INIT);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");

    return NULL;
}

int ESMWifiProvCheckCB(int enabled, char* ssid, char* addr)
{
    int ret = -1;

    if(GetWiFiProvState() != WIFI_SET)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "WiFi Provisioning State is Not WIFI_SET step.(%d)", GetWiFiProvState());
        return 0;
    }

    if( enabled != 1 || ssid == NULL || addr == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "enabled=%d, ssid=%s, addr=%s", enabled, ssid, addr);
        return 0;
    }

    pthread_mutex_lock(&gESmutex);
    if(strcmp(ssid, gWiFiProvData->ssid) == 0)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CONNECTED TO TARGET AP  %s ", ssid);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "GET IP ADDR  : %s ",addr);
        ret = 1;
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SSID is not Targeted One : (%s), (%s)", ssid, gWiFiProvData->ssid);
        ret = 0;
    }
    pthread_mutex_unlock(&gESmutex);


    if( ret == 1 )
    {
        ESSetState(ES_STATE_CONNECTED_TO_ENROLLER);
        SetWiFiProvState(WIFI_DONE);
        THINGS_LOG(THINGS_DEBUG, TAG, "ESSetState ES_STATE_CONNECTED_TO_ENROLLER");

        if( gSERVERBuilder->BroadcastPresence(gSERVERBuilder, 20) == 1)
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Broadcast Presence Failed.");
            ret = 0;
        }

        DelAllRequestHandle();  // clear time-out thread.
        PROFILING_TIME("WiFi Provisioning End.");

        SetWiFiProvState(WIFI_INIT);
    }

    return ret;
}

void WiFiProvCbInApp(ESWiFiProvData *eventData)
{
    THINGS_LOG_V(THINGS_DEBUG, TAG, "Enter");
    AccessPointInfo1* pInfo = NULL;

    if(eventData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ESWiFiProvData Data Is NULL");
        return;
    }

    if(OICIsNetAPIInitialize() != 1)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Network API Is not ready.");
        return ;
    }

    pInfo = (AccessPointInfo1*)DAMalloc(sizeof(AccessPointInfo1));
    if( pInfo == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "AccessPointInfo1 memory allocation is failed.");
        return;
    }

    pthread_mutex_lock(&gESmutex);
    memcpy(gWiFiProvData, eventData, sizeof(ESWiFiProvData));
    pthread_mutex_unlock(&gESmutex);

    // Attention : when 64bit linux Build, runtime-error occur. (memset () at ../sysdeps/x86_64/memset.S: Not Found File or Directory)
    // It's because of pInfo.(64 bit malloc internal code flow is not support that character array data-type of AccessPointInfo1)
    // TODO : Fix Bug for 64bit support.
    memset(pInfo->ESSID,0,MAX_ESSID);
    memset(pInfo->SecurityKey,0,MAX_SECUIRTYKEY);
    memset(pInfo->EncType,0,MAX_TYPE_ENC);
    memset(pInfo->AuthType,0,MAX_TYPE_AUTH);
    memset(pInfo->Channel,0,MAX_CHANNEL);
    memset(pInfo->BSSID,0,MAX_BSSID);
    memset(pInfo->SignalLevel,0,MAX_LEVEL_SIGNAL);

    if( strlen(eventData->ssid) > 0 )
    {
        DAStrncpy(pInfo->ESSID, eventData->ssid, strlen(eventData->ssid));
    }

    if( strlen(eventData->pwd) > 0 )
    {
        DAStrncpy(pInfo->SecurityKey, eventData->pwd, strlen(eventData->pwd));
    }

    if(eventData->authtype >= NONE_AUTH && eventData->authtype <= WPA2_PSK)
    {
        switch(eventData->authtype)
        {
            case WEP:
                DAStrncpy(pInfo->AuthType, "WEP", strlen("WEP"));
                break;
            case WPA_PSK:
                DAStrncpy(pInfo->AuthType, "WPA-PSK", strlen("WPA-PSK"));
                break;
            case WPA2_PSK:
                DAStrncpy(pInfo->AuthType, "WPA2-PSK", strlen("WPA2-PSK"));
                break;
            case NONE_AUTH:
            default:
                DAStrncpy(pInfo->AuthType, "NONE", strlen("NONE"));
                break;
        }
    }
    else
    {
        DAStrncpy(pInfo->AuthType, "NONE", strlen("NONE"));
    }

    if(eventData->enctype >= NONE_ENC && eventData->enctype <= TKIP_AES)
    {
        switch(eventData->enctype)
        {
            case WEP_64:
                DAStrncpy(pInfo->EncType, "WEP_64", strlen("WEP_64"));
                break;
            case WEP_128:
                DAStrncpy(pInfo->EncType, "WEP_128", strlen("WEP_128"));
                break;
            case TKIP:
                DAStrncpy(pInfo->EncType, "TKIP", strlen("TKIP"));
                break;
            case AES:
                DAStrncpy(pInfo->EncType, "AES", strlen("AES"));
                break;
            case TKIP_AES:
                DAStrncpy(pInfo->EncType, "TKIP_AES", strlen("TKIP_AES"));
                break;
            default:
                DAStrncpy(pInfo->EncType, "NONE", strlen("NONE"));
                break;
        }
    }
    else
    {
        DAStrncpy(pInfo->EncType, "NONE", strlen("NONE"));
    }

    if( eventData->discoveryChannel != -1)
        snprintf(pInfo->Channel, MAX_CHANNEL, "%d", eventData->discoveryChannel);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "SSID : %s", pInfo->ESSID);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "PW : %s", pInfo->SecurityKey);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Auth : %s", pInfo->AuthType);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enc : %s", pInfo->EncType);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Channel : %s", pInfo->Channel);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied ssid = %s", gWiFiProvData->ssid);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied pwd = %s", gWiFiProvData->pwd);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied enctype = %d", gWiFiProvData->enctype);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied discoveryChannel = %d", gWiFiProvData->discoveryChannel);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Copied authtype = %d", gWiFiProvData->authtype);

    // Connect to AP
    if(gthreadId_NetworkStatusCheck == 0)
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Create NetworkStatusCheck thread");
        DelAllRequestHandle();  // clear time-out thread.
        SetWiFiProvState(WIFI_INIT);

#ifdef OCF_RTOS
        pthread_create_rtos(&gthreadId_NetworkStatusCheck, NULL, WifiProvSetLoop, (void *)pInfo, THINGS_STACK_CLOUD_REFRESH_THREAD);
#else
         OICThread_create(&gthreadId_NetworkStatusCheck, NULL, WifiProvSetLoop, (void *)pInfo);
#endif
        SetWiFiProvState(WIFI_READY);
    }
    else
    {
        DAFree(pInfo);
    }

    THINGS_LOG_V(THINGS_DEBUG, TAG, "Exit");
}


void DevConfProvCbInApp(ESDevConfProvData *eventData)
{
    if(eventData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ESDevConfProvData Is NULL");
        return;
    }

    pthread_mutex_lock(&gESmutex);
    memcpy(gDevConfProvData, eventData, sizeof(ESDevConfProvData));
    pthread_mutex_unlock(&gESmutex);

    if( strlen(eventData->language) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Language : %s", eventData->language);
    }

    if( strlen(eventData->country) > 0 )
    {
        // check payload
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Country : %s", eventData->country);
    }

    if( strlen(eventData->datetime) > 0 )
    {
        // check payload
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Datetime : %s", eventData->datetime);
    }
}


void CloudDataProvCbInApp(ESCloudProvData *eventData)
{
    if(eventData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ESCloudProvData Data Is NULL");
        return;
    }

    pthread_mutex_lock(&gESmutex);
    memcpy(gCloudProvData, eventData, sizeof(ESCloudProvData));
    pthread_mutex_unlock(&gESmutex);

    if(strlen(eventData->authCode) > 0 )
    {
         THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthCode : %s", gCloudProvData->authCode);
    }

    if(strlen(eventData->accesstoken) > 0 )
    {
         THINGS_LOG_D(THINGS_DEBUG, TAG, "Accesstoken : %s", gCloudProvData->accesstoken);
    }

    if(strlen(eventData->refreshtoken) > 0 )
    {
         THINGS_LOG_D(THINGS_DEBUG, TAG, "Refreshtoken : %s", gCloudProvData->refreshtoken);
    }

    if(strlen(eventData->uid) > 0 )
    {
         THINGS_LOG_D(THINGS_DEBUG, TAG, "Uid : %s", gCloudProvData->uid);
    }

    if(strlen(eventData->authProvider) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthProvider : %s", gCloudProvData->authProvider);
    }

    if(strlen(eventData->hostName) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CIHostName : %s", gCloudProvData->hostName);
    }

    if(strlen(eventData->ip) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CIip : %s", gCloudProvData->ip);
    }

    if(strlen(eventData->port) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CIPort : %s", gCloudProvData->port);
    }

    if(strlen(eventData->ciServer) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "CIServer : %s", gCloudProvData->ciServer);
    }

    if(strlen(eventData->serverID) > 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Server ID : %s", gCloudProvData->serverID);
    }

    gCloudDataCbToApp(gCloudProvData);
    // CB to thingsstack.c
    // [Jay] Need to check the running state of the thread...
}

bool ESMGetNetworkStatus(void)
{
    //THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    bool isOk = false;

    if( OICIsNetAPIInitialize() != 1 )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Network API is not initialized.");
        return false;
    }

    if( OICIsConnectedAP() == true)
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Connected to AP");
        ESSetState( ES_STATE_CONNECTED_TO_ENROLLER );

        if( gSERVERBuilder != NULL && gSERVERBuilder->BroadcastPresence != NULL )
        {
            if( gSERVERBuilder->BroadcastPresence(gSERVERBuilder, 20) == 1 )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Broadcast Presence Failed.");
                return isOk;
            }
        }
        else
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "ServerBuilder is not initialized.");
        }

        isOk = true;
    }
    else
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Disconnected to AP");
        ESSetState( ES_STATE_INIT );
    }

    //THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return isOk;
}

void* ESMRegisterCloudCb(void *func)
{
    gCloudDataCbToApp = func;
    return NULL;
}

ESMResult ESMSetWifiConnErr(void)
{
    ESSetState(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
    ESSetErrorCode(ES_ERRCODE_UNKNOWN);
    return ESM_OK;
}

int ESMRegisterPINGeneratedCB(PINGeneratedFuncType func)
{
    if(func)
    {
        gPINGeneratedCB = func;
        // SetGeneratePinCB(GeneratePinCB);
        return 1;
    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid PINGeneratedCB function");
        return 0;
    }
}

int ESMRegisterPINCloseCB(PINCloseFuncType func)
{
    if(func)
    {
        gPINCloseCB = func;
        // SetGeneratePinCB(GeneratePinCB);
        return 1;
    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid PINGeneratedCB function");
        return 0;
    }
}

int ESMRegisterUserConfirmCB(UserConfirmResultFuncType func)
{
    if(func)
    {
        gUserConfirmCB = func;

        // SetUserConfirmCB(NULL, GetUserConfirmation);
        return 1;
    }
    else
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Invalid UserConformCB function");
        return 0;
    }
}
