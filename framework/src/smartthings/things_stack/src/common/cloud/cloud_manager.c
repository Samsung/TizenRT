#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE

#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "ocstack.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "cautilinterface.h"

#include "escommon.h"
#include "cacommon.h"
#include "oicstringutil.h"
#include "oicdef.h"
#include "thingslogger.h"
#include "thingsmalloc.h"
#include "thingsdatamanager.h"
#include "cloud_connector.h"
#include "cloud_manager.h"
#include "cloud_evtpend.h"
#include "oiceasysetup.h"
#include "easysetup.h"
#include "easysetup_manager.h"
#include "resourcehandler.h"
#include "oicnetwork.h"
#include "NSProviderInterface.h"
#include "oicping.h"
#include "oicthread.h"
#include "oicsecuritymanager.h"
#ifdef OCF_RTOS
#include "thingsRtosUtil.h"
#endif

#define DEFAULT_CI_HOST         "52.40.216.160:5683"        //!< Non Security Cloud IPv4:Port. Not used in common.
#define MAX_CI_ADDRESS          256
#define RESOURCE_DISCOVERY_QUERY "/oic/res"
#define RESOURCE_DISCOVERY_QUERY_DI "/oic/res?di="

#define ERR_UPPER_UNIT  10000       //!< Unit of Network-State for Cloud Error-Code
#define MAX_SIGNUP_SENDNUM  1       //!< Max re-transmission count of SendCnt_Sign_UP variable.
#define MAX_RETRY_RSCPUBLISH    10  //!< Max re-transmission count of ReTranslateRSCPublishCnt variable.

#define TAG "[cloudmnger]"

typedef void *(*pthread_FuncType) (void *);

/*! Cloud Session Management enum. */
typedef enum
{
    CISESS_NULL = -2,
    CISESS_APDISCON = -1,           //!< -1 : AP Disconnected.
    CISESS_STOP = 0,                //!<  0 : Cloud Connection Disconnected & AP Connected.
    CISESS_BUSY = 1,                //!<  1 : Cloud Connection Connecting & Connected. (Busy)
    CISESS_STOP_TRIGGER = 2,        //!<  2 : Triggered for Cloud Connection disconnect.
    CISESS_SIGNOUT = 3              //!<  3 : Sign-Out from Cloud.
} CISESS_LEVEL;

/*! Provisioned Data + Time-Out Data. */
typedef struct ESCloudEvent_TimeOut_s {
    TimeOut_s timeOut;              //!< Time-Out value(unit: seconds)
    ESCloudProvData event;          //!< Cloud Provisioned Data
} ESCloudEvent_TimeOut;

volatile static CISESS_LEVEL gQisCloudThreadRunning = CISESS_APDISCON;  //!< Variable for managing Cloud session.(No cached)

static int SendCnt_Sign_UP = 0;             //!< Counter for Sign-UP re-transmission process.
static int ReTranslateRSCPublishCnt = 0;    //!< Counter for Resource-Publish re-transmission process.

static pthread_mutex_t gESmutex = PTHREAD_MUTEX_INITIALIZER;        //!< Mutex variable for thread-safety of StartCIConnection().
static pthread_mutex_t gESTCPsessmutex = PTHREAD_MUTEX_INITIALIZER; //!< Mutex variable for thread-safety of CIRetryStop_byTCPCB().

static char gCloudIP[IP_PORT] = {0,};               //!< Current Connected Cloud Ipv4 address.
static char gCloudPort[IP_PORT] = {0,};             //!< Current Connected Cloud Port number.
static char gCloudAddress[MAX_CI_ADDRESS] = {0,};   //!< Current Connected Cloud Address.
static ESCloud_SignUp * SignedUp_data = NULL;       //!< Variable for Loading/Saving components to "ocf_infoX.json" file.

static OICServerBuilder* gServerBuilder = NULL;     //!< Server-Builder pointer to get Resource List.
static OicResource **gResourceLists = NULL;         //!< Resource List pointer to be publishing resources to Cloud.
static int gLen = 0;                                //!< EA of Resources in "gResourceLists" variable.

static OICCloudConResultFuncType THINGS_resCBfunction = NULL;  //!< call-back Function pointer : it's called when finish Cloud-Connection.


static OCStackResult PublishResourceMainDevIntoCloud(OicResource** list, int length, TimeOut_s* timeout);
static int CloudRetrySignUP(ESCloudProvData* EventData, TimeOut_s* TimeOut);
static char* MakeCloudAddress(char* ip, char* port, const char* ciAddr);
static void CloudRequestRetry_Trigger(TimeOut_s* TimeOut);
static ESCloudProvData* IfFailedThenRetry(OCDoHandle handle, OCStackResult result, int* nErr);
static void forceSessionStop(CISESS_LEVEL state);
static int ESCloudState_SetAndNotify(CloudStatus state, ESErrorCode ESerr, OCClientResponse * response, CIErrorCode * CIerr );
static int GetCloudCode(OCClientResponse* response, OCMethod method, CIErrorCode* err);
static CAResponseResult_t OCResultToNetworkResult(OCStackResult ocCode, OCMethod method);
static char* MakeIpPort(char* pIP, char* pPort);
static ESCloudEvent_TimeOut * cloneDataAddTimeOut(ESCloudProvData* CloudData, TimeOut_s* timeout);
static void CIfinishCloudCon( int result );
static void PublishDevProfileIntoCloud( TimeOut_s* TimeOut );

/**
 * @brief          Handle-Function for Re-transmission of Sign-UP request when occurred time-out of Cloud Sign-UP.
 * @param TimeOut  Variable to set time-out process of next-Sign-Up.
 * @return         always return 0
 */
static void* handleSignUPTimeOut(TimeOut_s* TimeOut)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Sign-UP is Time-Out.");

    ESCloudProvData * pendCloudData = NULL;

    if( TimeOut == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] TimeOut is NULL.");
        return 0;
    }

    SendCnt_Sign_UP++;

    if(SendCnt_Sign_UP <= MAX_SIGNUP_SENDNUM)
    {
        if( (pendCloudData = CICPGetPendedData()) != NULL )
        {
            TimeOut->cur_counter = TimeOut->cur_num;
            THINGS_LOG_V(THINGS_INFO, TAG, "Sign-UP Request Send is re-tryed(%d).", SendCnt_Sign_UP);
            if( CloudRetrySignUP(pendCloudData, TimeOut) == 0 )
            {
                SendCnt_Sign_UP = MAX_SIGNUP_SENDNUM+1;
            }
        }
        else
        {
            SendCnt_Sign_UP = MAX_SIGNUP_SENDNUM+1;
        }
    }

    if( SendCnt_Sign_UP > MAX_SIGNUP_SENDNUM )
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Sign-UP Re-Try Request Send is failed.");

        OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD|PING_ST_SIGNIN);

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL ) != 0 )
         {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
         }

        forceSessionStop(CISESS_NULL);
        SendCnt_Sign_UP = 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return 0;
}

/**
 * @brief          Handle-Function for Re-transmission of Sign-IN request when occurred time-out of Cloud Sign-IN.
 * @param TimeOut  Variable to set time-out process of next-Sign-IN.
 * @return         always return 0
 */
static void* handleSignINTimeOut(TimeOut_s* TimeOut)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Sign-IN is Time-Out.");

    if( TimeOut == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] TimeOut is NULL.");
        return 0;
    }

    int threshold = TimeOut->ori_num * 10;       // PDF threshold 0.2 --> 5

    if( TimeOut->cur_num < threshold &&
       (TimeOut->cur_num = (int)nextTimeOut( (long long)TimeOut->ori_num, (long long)TimeOut->cur_num )) != -1 )
    {
        if( TimeOut->cur_num > threshold )
        {
            TimeOut->cur_num = threshold;
        }
        TimeOut->cur_counter = TimeOut->cur_num;

        THINGS_LOG_V(THINGS_INFO, TAG, "Sign-IN Request Send is re-tryed. (PDF interval:%d)", TimeOut->cur_num);

        forceSessionStop(CISESS_SIGNOUT);
        CloudRequestRetry_Trigger(TimeOut);
    }
    else
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Sign-IN Re-Try Request Send is failed.");

        OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD|PING_ST_SIGNIN);

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL ) != 0 )
         {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
         }

        forceSessionStop(CISESS_NULL);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return 0;
}

/**
 * @brief          Handle-Function for Re-transmission of RSC-publish request when occurred time-out of Cloud RSC-publish.
 * @param TimeOut  Variable to set time-out process of next-RSC-publish.
 * @return         always return 0
 */
static void* handlePublishTimeOut(TimeOut_s* TimeOut)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Resource-Publish is Time-Out.");

    if( TimeOut == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] TimeOut is NULL.");
        return 0;
    }

    int threshold = TimeOut->ori_num * 5;       // PDF threshold 0.2 --> 5

    if( TimeOut->cur_num < threshold &&
       (TimeOut->cur_num = (int)nextTimeOut( (long long)TimeOut->ori_num, (long long)TimeOut->cur_num )) != -1 )
    {
        if( TimeOut->cur_num > threshold )
        {
            TimeOut->cur_num = threshold;
        }
        TimeOut->cur_counter = TimeOut->cur_num;

        THINGS_LOG_V(THINGS_INFO, TAG, "RSC_Publish Request Send is re-tryed. (PDF interval:%d)", TimeOut->cur_num);
        PublishResourceIntoCloud(RscPub_MainOnly, TimeOut);
    }
    else
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Resource-Publish Re-Try Request Sending is failed.");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL ) != 0 )
         {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
         }

        forceSessionStop(CISESS_NULL);
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return 0;
}

/**
 * @brief          Handle-Function for Re-transmission of RSC-publish request when occurred time-out of Cloud RSC-publish.
 * @param TimeOut  Variable to set time-out process of next-RSC-publish.
 * @return         always return 0
 */
static void* handleDevProfileTimeOut(TimeOut_s* TimeOut)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Device Profile Publish is Time-Out.");

    if( TimeOut == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] TimeOut is NULL.");
        return 0;
    }

    int threshold = TimeOut->ori_num * 5;       // PDF threshold 0.2 --> 5

    if( TimeOut->cur_num < threshold &&
       (TimeOut->cur_num = (int)nextTimeOut( (long long)TimeOut->ori_num, (long long)TimeOut->cur_num )) != -1 )
    {
        if( TimeOut->cur_num > threshold )
        {
            TimeOut->cur_num = threshold;
        }
        TimeOut->cur_counter = TimeOut->cur_num;

        THINGS_LOG_V(THINGS_INFO, TAG, "Device Profile Request Send is re-tryed. (PDF interval:%d)", TimeOut->cur_num);
        PublishDevProfileIntoCloud(TimeOut);
    }
    else
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "Device Profile publish Re-Try Request Sending is failed.");

//        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL ) != 0 )
//         {
//            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
//         }
//
//        forceSessionStop();
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return 0;
}

/**
 * @brief             Setting-Function for basic-element of ESCloud_SignUP. (address/port/domain/refreshtoken)
 * @param cloud_info  ESCloud_SignUP pointer. (Subject of this function)
 * @param cloud_addr  Current Connected Cloud Server Address.
 * @param provData    Current Used Cloud-Provisioning Data structure.
 * @return            Success : return 1 \n
 *                    Failed  : return 0
 */
static int SetDefCloudInfo( ESCloud_SignUp* cloud_info, const char* cloud_addr, const ESCloudProvData* provData)
{
    int result = 1;
    char* IpPort = NULL;
    char* Domain = NULL;
    char* tDomain = NULL;

    if( cloud_info == NULL || cloud_addr == NULL || cloud_addr[0] == 0 )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Parameter value is invalid.(cloud_info=0x%X, coud_addr=%s)", cloud_info, cloud_addr);
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "cloud_addr = %s", cloud_addr);

    if( cloud_info->address != NULL )
    {
        DAFree(cloud_info->address);
        cloud_info->address = NULL;
    }

    if( cloud_info->port != NULL )
    {
        DAFree(cloud_info->port);
        cloud_info->port = NULL;
    }

    if( cloud_info->domain != NULL )
    {
        DAFree(cloud_info->domain);
        cloud_info->domain = NULL;
    }

    IpPort = strstr(cloud_addr, "://");
    if( IpPort == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "cloud_addr is invalid.");
        return 0;
    }
    IpPort = IpPort+3;

    if( (cloud_info->address = (char*)DAMalloc(sizeof(char)*MAX_CI_ADDRESS)) == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cloud_info address memory allocation is Failed.");
        return 0;
    }

    DAStrncpy( cloud_info->address, IpPort, MAX_CI_ADDRESS);      // Ip:Port  save.

    // if exist Cloud Domain Name, then Save Name and Port.
    if( cloud_info->redirectURI != NULL && strlen(cloud_info->redirectURI) > 0 )
        Domain = strdup(cloud_info->redirectURI);
    else if( provData != NULL )
        Domain = strdup(provData->hostName);

    if(Domain != NULL && strlen(Domain) != 0)
    {
        tDomain = Domain;
        if( strstr(Domain, "://") != NULL )
            tDomain = strstr(Domain, "://")+3;

        if( strchr(tDomain, ':') != NULL )
        {
            IpPort = strchr(tDomain, ':');
            *IpPort = NULL;
            IpPort++;
        }
        else
        {
            if( (IpPort = strchr(IpPort, ':')) == NULL )
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not exit Port number.");
                result = 0;
                goto GOTO_OUT;
            }
            IpPort++;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "Domain = %s", tDomain);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Port   = %s", IpPort);

        int length_port = strlen(IpPort)+1;
        int length_domain = strlen(tDomain)+1;

        if( (cloud_info->port = (char*)DAMalloc(sizeof(char)*length_port)) == NULL )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cloud_info port memory allocation is Failed.");
            result = 0;
            goto GOTO_OUT;
        }

        if( (cloud_info->domain = (char*)DAMalloc(sizeof(char)*length_domain)) == NULL )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cloud_info domain-name memory allocation is Failed.");
            result = 0;
            goto GOTO_OUT;
        }

        DAStrncpy( cloud_info->port, IpPort, length_port);             // Cloud Port save.

        DAStrncpy( cloud_info->domain, tDomain, length_domain);    // Cloud domain name save.
    }

    if( cloud_info->refreshToken == NULL )
    {
        if( provData == NULL || provData->refreshtoken[0] == 0 )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not exist Refresh Token. please check your system.");
            result = 0;
            goto GOTO_OUT;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "refreshtoken = %s", provData->refreshtoken);

        int length_refresh = strlen(provData->refreshtoken)+1;

        if( (cloud_info->refreshToken = (char*)DAMalloc(sizeof(char)*length_refresh)) == NULL )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cloud_info port memory allocation is Failed.");
            result = 0;
            goto GOTO_OUT;
        }

        DAStrncpy( cloud_info->refreshToken, provData->refreshtoken, length_refresh);    // Cloud Refresh Token save.
    }

GOTO_OUT:
    if(Domain)
    {
        DAFree(Domain);
        Domain = NULL;
        tDomain = NULL;
    }
    IpPort = NULL;

    if( result == 0 )
    {
        if( cloud_info->address )
        {
            DAFree(cloud_info->address);
            cloud_info->address = NULL;
        }

        if( cloud_info->port )
        {
            DAFree(cloud_info->port);
            cloud_info->port = NULL;
        }

        if( cloud_info->domain )
        {
            DAFree(cloud_info->domain);
            cloud_info->domain = NULL;
        }
    }

    return result;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Sign-UP response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleRegisterCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    int res = OC_STACK_DELETE_TRANSACTION;
    int nErr = 0;
    ESCloudProvData* provedData = NULL;

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Register callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Register response received code: %s(%d)", getResult(clientResponse->result), clientResponse->result);
    if( (provedData = IfFailedThenRetry(handle, clientResponse->result, &nErr)) == NULL )
    {
        if( nErr == 1 || nErr == 4 )    // OC_STACK_COMM_ERROR or Handle is invalid.
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "OC_STACK_COMM_ERROR(1) or Handle InValid(4) : %d", nErr);
            return OC_STACK_DELETE_TRANSACTION;
        }

        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "System Error occurred.(provedData=0x%X, nErr=%d)", provedData, nErr);
        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL ) != 0 )
         {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
         }

        return OC_STACK_DELETE_TRANSACTION;
    }

    SendCnt_Sign_UP = 0;

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "PAYLOAD_TYPE_REPRESENTATION received");
        THINGS_LOG_D(THINGS_DEBUG, TAG, "You can login using received session variable after disconnected or reboot");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
         while (val != NULL)
         {
             if( val->type == OCREP_PROP_INT)
             {
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int ",val->name, val->i);
             }
             else if( val->type == OCREP_PROP_STRING)
             {
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
             }
             else
             {
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);
             }

             val = val->next;
         }


         if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
         {
             CIErrorCode CIerr = ERRCI_NO_ERROR;

             OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD|PING_ST_SIGNIN);

             if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, (CIErrorCode)NULL, clientResponse, &CIerr ) != 0 )
             {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
                res = OC_STACK_DELETE_TRANSACTION;
                goto GOTO_OUT;
             }

             switch(CIerr)
             {
             case ERRCI_NOT_ACCEPTABLE:
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "Cloud received duplicated-request in 500msec. retry-again");
                if(gQisCloudThreadRunning != CISESS_APDISCON)
                {
                    CloudRequestRetry_Trigger(NULL);
                }
                 break;
             case ERRCI_INTERNAL_SERVER_ERROR:
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                 ESMGetNetworkStatus();  // State return
                 break;
             case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
             case ERRCI_SAMSUNG_ACCOUNT_AUTHORIZATION_FAILED:
             case ERRCI_FORBIDDEN:
             case ERRCI_USER_NOT_FOUND:
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE] %d : fix the information and try again.", CIerr);
                 if( CloudRetrySignIN( NULL ) != 0 )
                 {
                     THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Cloud connection Re-try is failed.");
                 }
                 res = OC_STACK_DELETE_TRANSACTION;
                 break;
             case ERRCI_DEVICE_DUPLICATE_PARAM:
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE] %d : check your deviceID and try again.", CIerr);
                 ;  // TODO : need cloud definition.
                 break;
             default :
                 THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                 ESMGetNetworkStatus();  // State return
                 res = OC_STACK_DELETE_TRANSACTION;
                 break;
             }

             goto GOTO_OUT;
         }

        if ( ESCloud_SignUp_init( &SignedUp_data ) == false )
         {
            ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SignedUp_data-memory allocation is Failed, try again.");
            res = OC_STACK_DELETE_TRANSACTION;
            goto GOTO_OUT;
         }

         // [Kes] get Signed-data but mandatory key-value are follows. (accesstoken / refreshtoken / tokentype / expiresin / uid)
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_ACCESS, &SignedUp_data->accessToken);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_ACCESS_REFRESH, &SignedUp_data->refreshToken);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_TYPE, &SignedUp_data->tokenType);
        OCRepPayloadGetPropInt(((OCRepPayload *)clientResponse->payload), KEY_EXPIRE_TIME, &SignedUp_data->expireTime);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_ID_USER, &SignedUp_data->uid);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_SERVER_REDIRECT_URI, &SignedUp_data->redirectURI);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_CERTIFICATE_FILE, &SignedUp_data->certificate);
        OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_SERVER_ID, &SignedUp_data->sid);

#ifdef __SECURED__
        if( SM_SaveCloudAcl(SignedUp_data->sid) != 0 )  // OIC_SEC_OK(0)
        {
            ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "It's failed to save Cloud UUID to ACL list.");
            res = OC_STACK_DELETE_TRANSACTION;
            goto GOTO_OUT;
        }
#endif //__SECURED__

        if( SetDefCloudInfo(SignedUp_data, gCloudAddress, provedData) == 0 )
        {
            ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "SignedUp_data default setting is Failed.");
            res = OC_STACK_DELETE_TRANSACTION;
            goto GOTO_OUT;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "expiresin : %lld", SignedUp_data->expireTime);

        if(SignedUp_data->accessToken == NULL || strlen(SignedUp_data->accessToken) < 1)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Session Key is NULL");

            if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_INVALID_ACCESSTOKEN, NULL, NULL ) != 0 )
            {
               THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
               res =  OC_STACK_DELETE_TRANSACTION;
               goto GOTO_OUT;
            }
        }
        else
        {
            THINGS_LOG(THINGS_DEBUG, TAG, "Write Sign-Up data to file.");
            if( DM_UpdateOCFCloud(SignedUp_data) == 0 )
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Sign-UP data can not write to info file.");
                res = OC_STACK_DELETE_TRANSACTION;
                goto GOTO_OUT;
            }

            THINGS_LOG(THINGS_DEBUG, TAG, "Start Login Into Cloud");
            LogInOutToCloud(true, NULL);
            res = OC_STACK_DELETE_TRANSACTION;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Received Failed-Response of Sign-UP without payload from Cloud Server.");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL ) != 0 )
         {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
            res = OC_STACK_DELETE_TRANSACTION;
            goto GOTO_OUT;
         }
    }

GOTO_OUT:
    if(provedData)
    {
        DAFree(provedData);
        provedData = NULL;
    }

    return res;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Sign-IN response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1)  \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleLoginCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Login callback received");

    int nErr = 0;
    ESCloudProvData* provedData = NULL;
    OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Login callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Login response received code: %s(%d)", getResult(clientResponse->result), clientResponse->result);
    provedData = IfFailedThenRetry(handle, clientResponse->result, &nErr);

    if( provedData )
    {
        DAFree(provedData);
        provedData = NULL;
    }

    if( nErr >= 3 || nErr == 1 ) // mem alloc error, handle is invalid.
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Memory alloc error or InValid Resp CB.(%d)", nErr);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "PAYLOAD_TYPE_REPRESENTATION received");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;

            OICPingUNSetMask(gCloudIP, PING_ST_SIGNIN);

            if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, (CIErrorCode)NULL, clientResponse, &CIerr ) != 0 )
            {
               THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
               return OC_STACK_DELETE_TRANSACTION;
            }

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD);
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE] AccessToken \"%d\", refresh AccessToken and try again.", CIerr);
                RefreshTokenIntoCloud();   // refresh accesstoken.
                break;
            case ERRCI_FORBIDDEN:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
                CloudRetrySignIN(NULL);        // TODO : need re-code
                break;
            case ERRCI_DEVICE_NOT_FOUND:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Device Not Found, Check your device ID and User ID coupling.");
                OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD);
                ESMGetNetworkStatus();  // State return
                if( OICReset(NULL, RST_AUTO_RESET) == -1 )
                {
                    THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] OICReset is failed.");
                }
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD);
                ESMGetNetworkStatus();  // State return
                break;
            }

            return OC_STACK_DELETE_TRANSACTION;
        }

        if( ESCloudState_SetAndNotify( ES_STATE_REGISTERED_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
        {
           THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
           return OC_STACK_DELETE_TRANSACTION;
        }

        OICPingSetMask(gCloudIP, atoi(gCloudPort), PING_ST_SIGNIN);

        // [ysy] Plublish resources to cloud
        THINGS_LOG(THINGS_DEBUG, TAG,  "Start OCCloudPUblish");
        ReTranslateRSCPublishCnt = 0;
        PublishResourceIntoCloud(RscPub_MainOnly, NULL);
#ifdef ENABLE_PUSH_NOTY
        NSProviderEnableRemoteService(gCloudAddress);
#endif

        res = OC_STACK_DELETE_TRANSACTION;
    }
    else if(clientResponse->result != OC_STACK_RESOURCE_CHANGED)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Received Failed-Response of Sign-IN without payload from Cloud Server.");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL ) != 0 )
         {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
            return OC_STACK_DELETE_TRANSACTION;
         }
    }

    return res;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Sign-OUT response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleLogoutCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Logout callback received");

    OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Logout callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Logout response received code: %s(%d)", getResult(clientResponse->result), clientResponse->result);

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "PAYLOAD_TYPE_REPRESENTATION received");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }
    }

    if( gQisCloudThreadRunning != CISESS_BUSY && gQisCloudThreadRunning != CISESS_APDISCON )
    {
        ESEnrolleeState ESerr = ES_STATE_INIT;

        if( ESMGetNetworkStatus() == true )
            ESerr = ES_STATE_CONNECTED_TO_ENROLLER;

        if( ESCloudState_SetAndNotify( ESerr, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
        {
           THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
        }
    }

    res = OC_STACK_DELETE_TRANSACTION;

    return res;
}

static bool Check_COMM_ERROR_retrans(OCStackResult result)
{
#define MAX_COMM_ERROR_RETRANSMISSION       10
    bool ret = false;
    static int retransmission_count = 0;

    if(result == OC_STACK_COMM_ERROR)        // Don't Send Request Message.
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Request-Send is Failed.");
        if(gQisCloudThreadRunning != CISESS_APDISCON && retransmission_count < MAX_COMM_ERROR_RETRANSMISSION)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Re-transmission try.");
            retransmission_count++;
            ret = true;
        }
        else
        {
            retransmission_count = 0;
        }
    }
    else
    {
        retransmission_count = 0;
    }

    return ret;
#undef MAX_COMM_ERROR_RETRANSMISSION
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Refresh-accessToken response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleRefreshTokenCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Refresh Token callback received");

    OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Login/out callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Refresh Token response received code: %s(%d)", getResult(clientResponse->result), clientResponse->result);

    if( Check_COMM_ERROR_retrans(clientResponse->result) == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Re-Start Refresh-Token Process.");
        RefreshTokenIntoCloud();
    }

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "PAYLOAD_TYPE_REPRESENTATION received");


        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val != NULL)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;
            // Method don't care because issue is not occurred by it.
            if( GetCloudCode(clientResponse, OC_REST_POST, &CIerr) == 0 )
                CIerr = ERRCI_UNKNOWN;

            THINGS_LOG_D(THINGS_DEBUG, TAG, "Cloud ErrCode = %d", CIerr);
//            ESSetCloudErrorCode(CIerr);

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> RefreshToken is invalid, fix RefreshToken and try again.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> RefreshToken is mismatched with client ID, fix RefreshToken or Client ID.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> RefreshToken is expired, refresh RefreshToken and try again.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_FORBIDDEN:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
                ESMGetNetworkStatus();  // State return
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                ESMGetNetworkStatus();  // State return
                break;
            }

            return OC_STACK_DELETE_TRANSACTION;
        }

         OICFree(SignedUp_data->accessToken);
         OICFree(SignedUp_data->refreshToken);
         OICFree(SignedUp_data->tokenType);
         SignedUp_data->expireTime = 0;

            OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_ACCESS, &SignedUp_data->accessToken);
            OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_ACCESS_REFRESH, &SignedUp_data->refreshToken);
            OCRepPayloadGetPropString(((OCRepPayload *)clientResponse->payload), KEY_TOKEN_TYPE, &SignedUp_data->tokenType);
            OCRepPayloadGetPropInt(((OCRepPayload *)clientResponse->payload), KEY_EXPIRE_TIME, &SignedUp_data->expireTime);

            THINGS_LOG_D(THINGS_DEBUG, TAG, "expiresin : %lld", SignedUp_data->expireTime);

            if(SignedUp_data->accessToken == NULL || strlen(SignedUp_data->accessToken) < 1)
            {
                THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Session Key is NULL. Refresh Token Failed.");
            }
            else
            {
              THINGS_LOG(THINGS_DEBUG, TAG, "Write Sign-Up data to file.");
              if( DM_UpdateOCFCloud(SignedUp_data) == 1 )
                {
                  THINGS_LOG_D(THINGS_DEBUG, TAG, "Re-Try Sign-IN step.");
                  forceSessionStop(CISESS_SIGNOUT);
                  LogInOutToCloud(true, NULL);
              } else
                {
                  THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Refreshed Sign-UP data can not write to info file.");
                }
            }

            res = OC_STACK_DELETE_TRANSACTION;
    }

    return res;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Resource-Publish response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleMainDevPublishCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Publish callback received");

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Publish callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Publish resource response received, code: %s(%d)", getResult(clientResponse->result), clientResponse->result);
    if(CASRequestHandle(handle, NULL) == 1 )
    {
        DelAllRequestHandle();

        if( Check_COMM_ERROR_retrans(clientResponse->result) == true )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Re-Start PublishResourceIntoCloud");
                PublishResourceIntoCloud(RscPub_MainOnly, NULL);
            }

    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Response Handle(0x%X) is not exist in the Request Handle list.", handle);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (clientResponse->payload != NULL &&
            clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val != NULL)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;

            if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, (CIErrorCode)NULL, clientResponse, &CIerr ) != 0 )
            {
               THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
               PROFILING_TIME("Cloud Provisioning End.");
               return OC_STACK_DELETE_TRANSACTION;
            }

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", CIerr);
                RefreshTokenIntoCloud();   // refresh accesstoken.
                break;
            case ERRCI_FORBIDDEN:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
                break;
            case ERRCI_RDLINK_INVALID_PARAM:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> Resource List Cbor syntax is invalid. Re-Translation.");
                if( ReTranslateRSCPublishCnt < MAX_RETRY_RSCPUBLISH )
                {
                    PublishResourceIntoCloud(RscPub_MainOnly, NULL);
                    ReTranslateRSCPublishCnt++;
                } else
                {
                    THINGS_LOG_D(THINGS_INFO, TAG, "[INFO] Resource Publish is Re-translated about %d\n Please check Network Line.", ReTranslateRSCPublishCnt);
                    ReTranslateRSCPublishCnt = 0;
                    ESMGetNetworkStatus();  // State return
                }
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                break;
            }
        }

    }
    else if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Received Failed-Response of Resource-Publish without payload from Cloud Server.");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL ) != 0 )
         {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
            PROFILING_TIME("Cloud Provisioning End.");
            return OC_STACK_DELETE_TRANSACTION;
         }
    }

    if( clientResponse->result == OC_STACK_RESOURCE_CHANGED )
    {
        if( ESCloudState_SetAndNotify( ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
        {
           THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
           PROFILING_TIME("Cloud Provisioning End.");
           return OC_STACK_DELETE_TRANSACTION;
        }

        PublishResourceIntoCloud(RscPub_SubALL, NULL);
        PublishDevProfileIntoCloud(NULL);
    }

    PROFILING_TIME("Cloud Provisioning End.");
    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Resource-Publish response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleSubDevPublishCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Sub-Device resource Publish callback received");

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Publish callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Publish resource response received, code: %s(%d)", getResult(clientResponse->result), clientResponse->result);
    if( Check_COMM_ERROR_retrans(clientResponse->result) == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Resource Publish Request-Send is Failed.");
        PublishResourceIntoCloud(RscPub_SubALL, NULL);
    }

    if (clientResponse->payload != NULL &&
            clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val != NULL)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;

            if( GetCloudCode(clientResponse, OC_REST_POST, &CIerr) == 0 )
                CIerr = ERRCI_UNKNOWN;

//            ESSetCloudErrorCode(CIerr);

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", CIerr);
                RefreshTokenIntoCloud();   // refresh accesstoken.
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                break;
            }
        }

    }

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult handleDevProfileCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Device Profile Publish callback received");

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Publish callback received");
    }

    DelAllRequestHandle();

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Device Profile Publish response received, code: %s(%d)", getResult(clientResponse->result), clientResponse->result);
    if( Check_COMM_ERROR_retrans(clientResponse->result) == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Device Profile Publish Request-Send is Failed.");
        PublishDevProfileIntoCloud(NULL);
    }

    if (clientResponse->payload != NULL &&
            clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val != NULL)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;

            if( GetCloudCode(clientResponse, OC_REST_POST, &CIerr) == 0 )
                CIerr = ERRCI_UNKNOWN;

//            ESSetCloudErrorCode(CIerr);

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", CIerr);
                RefreshTokenIntoCloud();   // refresh accesstoken.
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                break;
            }
        }

    }

    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Find-Resource response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handleFindResourceCB(void* ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    if (ctx == (void*) DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Callback Context for DISCOVER query recvd successfully");
    }

    if (clientResponse)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", getResult(clientResponse->result), clientResponse->result);

        //char* connectionType = getConnectivityType (clientResponse->connType);
        //THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovered on %s", connectionType.c_str());
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);


        if (clientResponse->payload != NULL &&
                    clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
        {
            OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
            while (val != NULL)
            {
                if( val->type == OCREP_PROP_INT)
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
                else if( val->type == OCREP_PROP_STRING)
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
                else
                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

                val = val->next;
            }
        }

        if( clientResponse->result != OC_STACK_OK )
        {
            THINGS_LOG_D(THINGS_INFO, TAG, "Refresh Token process Start.");
            RefreshTokenIntoCloud();
        }

    }
    else
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "handleFindResourceCB received Null clientResponse");
    }


    return OC_STACK_DELETE_TRANSACTION;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Topic-Publish in Notification.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param clientResponse  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handlePublishTopicCB(void *ctx,
        OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Publish Topic callback received");

    OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

    if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid Publish Topic callback received");
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Publish Topic response received code: %s(%d)", getResult(clientResponse->result), clientResponse->result);

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "PAYLOAD_TYPE_REPRESENTATION received");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val)
        {
            if( val->type == OCREP_PROP_INT)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int",val->name, val->i);
            else if( val->type == OCREP_PROP_STRING)
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string",val->name, val->str);
            else
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", val->type);

            val = val->next;
        }

        if( clientResponse->result != OC_STACK_RESOURCE_CHANGED )
        {
            CIErrorCode CIerr = ERRCI_NO_ERROR;

            if(0 == GetCloudCode(clientResponse, OC_REST_POST, &CIerr))
            {
                CIerr = ERRCI_UNKNOWN;
            }

            switch(CIerr)
            {
            case ERRCI_INTERNAL_SERVER_ERROR:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
                ESMGetNetworkStatus();  // State return
                break;
            case ERRCI_TOKEN_VALIDATION_FAILED:
            case ERRCI_TOKEN_EXPIRED:
                THINGS_LOG_D(THINGS_DEBUG, TAG, "[ERRCODE] AccessToken \"%d\", refresh AccessToken and try again.", CIerr);
                RefreshTokenIntoCloud();   // refresh accesstoken.
                break;
            default :
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support This Cloud-Error-Code(%d) Exception", CIerr);
                //ESMGetNetworkStatus();  // Don't need to sign out on this session
                break;
            }
        }

        res = OC_STACK_DELETE_TRANSACTION;
    }

    return res;
}

/**
 * @brief                 Call function to send request for Publishing Resource to Cloud.
 * @param list            Resources-list pointer. (to be publishing)
 * @param length          EA of Resources in "list" pointer.
 * @param timeout         time-out value how long wait cloud-response when send request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
static OCStackResult PublishResourceMainDevIntoCloud(OicResource** list, int length, TimeOut_s* timeout)
{
    ESErrorCode ESerr = ES_ERRCODE_UNKNOWN;
    OCStackResult   res = OC_STACK_ERROR;

    if(list == NULL || length < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid Parameter Inserted");
        ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
        return res;
    }
    else
    {
        if( ESCloudState_SetAndNotify( ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
       {
           THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
           return res;
       }

        if( CIConnectionPreCheck(NULL, NULL) == 0 )
        {
            res = OCCloudRscPublish(gCloudAddress, list, length, handleMainDevPublishCB, handlePublishTimeOut, timeout);
        }
        else
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "AP is not connected to internet.");
            ESerr = ES_ERRCODE_NO_INTERNETCONNECTION;
        }

        if (res != OC_STACK_OK)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Unable to publish user resources to cloud : %d", res);

            if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ESerr, NULL, NULL ) != 0 )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
                return res;
            }

            return res;
        }
    }

    return res;
}

OCStackResult PublishResourceIntoCloud(Enum_RPTarget target, TimeOut_s* timeout)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    int subDevPubFail = -1;
    OCStackResult   res = OC_STACK_OK;

    if( target == RscPub_ALL || target == RscPub_MainOnly )
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Main-Device Resource Publish Start.");
        res = PublishResourceMainDevIntoCloud(gResourceLists, gLen, timeout);
        usleep(10000);  // 1 device resource-publish per 10 msec.
    }

    if (res == OC_STACK_OK && target != RscPub_MainOnly)
    {
        THINGS_LOG(THINGS_DEBUG, TAG, "Sub-Devices Resource Publish Start.");
        int deviceCnt = 0;
        char* deviceID = NULL;
        STDevice ** dList = NULL;

        if(1 == DM_GetDeviceInformation(&deviceCnt, &dList) && deviceCnt > 1 )
        {
            for(int index = 1 ; index < deviceCnt ; index++)
            {
                // Get device id of hosted ones..
                if( dList[index]->isPhysical == 1 )
                {
                    if( subDevPubFail == -1 )
                        subDevPubFail = 0;

                    deviceID = dList[index]->device_id;

                    THINGS_LOG_D(THINGS_DEBUG, TAG, "Sub-device ID=%s", deviceID);
                    if( deviceID == NULL || deviceID[0] == 0 )
                    {
                        THINGS_LOG_D(THINGS_DEBUG, TAG, "not exist device ID. So, skip publish resource.");
                        subDevPubFail = 1;
                        continue;
                    }

                    OCStackResult result = OCCloudRscPublishWithDeviceID(gCloudAddress,
                                                    deviceID,
                                                    dList[index]->pChildResources,
                                                    dList[index]->capaCnt,
                                                    handleSubDevPublishCB,
                                                    handlePublishTimeOut,
                                                    timeout);
                    if( result != OC_STACK_OK )
                        subDevPubFail = 1;

                    deviceID = NULL;
                    usleep(10000);  // 1 device resource-publish per 10 msec.
                }
            }
        }

        if( dList )
        {
            DAFree(dList);
            dList = NULL;
        }

        if( target == RscPub_SubALL || target == RscPub_NeedSubOnly )
        {
            switch( subDevPubFail )
            {
            case 1:     // occurred error
                THINGS_LOG_D(THINGS_INFO, TAG, "Sending-request is failed for Sub-device publish.");
                res = OC_STACK_ERROR;
                break;
            case -1:    // not exist sub-dev
                THINGS_LOG_D(THINGS_INFO, TAG, "Not exist sub-device publish.");
                res = OC_STACK_ERROR;
                break;
            case 0:
                THINGS_LOG_D(THINGS_INFO, TAG, "Sending-request is Success for Sub-device publish.");
                res = OC_STACK_OK;
                break;
            }
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return res;
}


/**
 * @brief                 Call function to send request for login/logout to Cloud.
 * @param value           true  : login \n
 *                        false : logout
 * @param timeout         time-out value how long wait cloud-response when send request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult LogInOutToCloud(bool value, TimeOut_s* timeout)
{
    int port = 0;
    char* CIip = NULL;
    char* CIDomain = NULL;
    ESErrorCode ESerr = ES_ERRCODE_UNKNOWN;
    OCStackResult   res = OC_STACK_ERROR;
    OCClientResponseHandler callback = NULL;
    CheckTimeOutCallFunc calltimeout = NULL;

    if(SignedUp_data == NULL || SignedUp_data->accessToken == NULL || strlen(SignedUp_data->accessToken) < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No Session Key Retrived from the Cloud ");
        return res;
    }

    if( SignedUp_data->domain != NULL && (SignedUp_data->port == NULL || SignedUp_data->port[0] == 0) )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "domain=%s but, not exist port number.(%s)", SignedUp_data->domain, SignedUp_data->port);
        return res;
    }

    if( value == true )
    {
        callback = handleLoginCB;
        calltimeout = handleSignINTimeOut;
        if( ESCloudState_SetAndNotify( ES_STATE_REGISTERING_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
            return res;
        }

        if( SignedUp_data->domain != NULL )
        {
            CIDomain = strdup(SignedUp_data->domain);
        }
        THINGS_LOG_D(THINGS_DEBUG, TAG, "*********** Start Sign-IN ********** CIDomain : %s", CIDomain);
    }
    else
    {
        callback = handleLogoutCB;
        calltimeout = NULL;
        gQisCloudThreadRunning = CISESS_SIGNOUT;
        if( ESCloudState_SetAndNotify( ES_STATE_LOGOUT_FROM_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
            return res;
        }
        THINGS_LOG_D(THINGS_DEBUG, TAG, "*********** Start Sign-OUT **********");
    }

    if( CIConnectionPreCheck(CIDomain, &CIip) == 0 )
    {
        if(CIip != NULL)
        {
            if( MakeCloudAddress(CIip, SignedUp_data->port,NULL) == NULL )
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Making CloudAddress is failed.");
                goto GOTO_OUT;
            }
            DAStrncpy(gCloudIP, CIip, IP_PORT);
            DAStrncpy(gCloudPort, SignedUp_data->port, IP_PORT);

        port = atoi(gCloudPort);
        if( gCloudIP[0] == NULL || gCloudPort[0] == NULL || 0 >= port || port > 65535 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud info is invalid.(gCloudIP=%s, gCloudPort=%s, port=%d)", gCloudIP, gCloudPort, port);
            goto GOTO_OUT;
        }
        }

        if(gCloudAddress[0] == 0)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "gCloudAddress is invalid.(0x%X)", gCloudAddress);
            goto GOTO_OUT;
        }

        THINGS_LOG_D(THINGS_INFO, TAG, "Cloud Addr : %s", gCloudAddress);
        THINGS_LOG_D(THINGS_INFO, TAG, "device ID : %s", OCGetServerInstanceIDString());
        THINGS_LOG_D(THINGS_INFO, TAG, "accessToken : %s", SignedUp_data->accessToken);
        THINGS_LOG_D(THINGS_INFO, TAG, "refresh token : %s", SignedUp_data->refreshToken);
        THINGS_LOG_D(THINGS_INFO, TAG, "user ID : %s", SignedUp_data->uid);
        THINGS_LOG_D(THINGS_INFO, TAG, "toke type : %s", SignedUp_data->tokenType);
        THINGS_LOG_D(THINGS_INFO, TAG, "expire Time : %lld", SignedUp_data->expireTime);
        THINGS_LOG_D(THINGS_INFO, TAG, "certificate : %s", SignedUp_data->certificate);
        THINGS_LOG_D(THINGS_INFO, TAG, "redirect URI : %s", SignedUp_data->redirectURI);
        THINGS_LOG_D(THINGS_INFO, TAG, "sid : %s", SignedUp_data->sid);

        if( value == true )
            OICPingSetMask(gCloudIP, port, PING_ST_ISCLOUD);
        else
            OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD|PING_ST_SIGNIN|PING_ST_TCPCONNECT);

        res = OCCloudSession(gCloudAddress,
                           SignedUp_data->uid,
                           OCGetServerInstanceIDString(),
                           SignedUp_data->accessToken,
                           value,
                           callback,
                           calltimeout,
                           timeout);

        THINGS_LOG_D(THINGS_DEBUG, TAG, "OCCloudLogInOut return : %d", res);
    }
    else
    {
        THINGS_LOG_V(THINGS_INFO, TAG, "AP is not connected to internet.");
        ESerr = ES_ERRCODE_NO_INTERNETCONNECTION;
    }

GOTO_OUT:
    if( CIip != NULL )
    {
        DAFree(CIip);
        CIip = NULL;
    }

    if( CIDomain != NULL )
    {
        DAFree(CIDomain);
        CIDomain = NULL;
    }

    if(res != OC_STACK_OK)
    {
        if( value == true )
        {
            OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD);

            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud Server LogIn Failed : %d", res);
            if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ESerr, NULL, NULL ) != 0 )
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
                return res;
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud Server LogOut Failed : %d", res);
            ESMGetNetworkStatus();
            return res;
        }
    }

    return res;
}

/**
 * @brief                 Call function to send request for Finding myself-resource to Cloud.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult FindCloudResource(void)
{
    OCStackResult ret = OC_STACK_ERROR;

    char* szQueryUri = NULL;
    int length = MAX_CI_ADDRESS;
    int lengthQuery = 0;

    length += strlen(RESOURCE_DISCOVERY_QUERY_DI);
    length += strlen(OCGetServerInstanceIDString());
    lengthQuery = sizeof(char)*(length+2);

    if( (szQueryUri = (char*)DAMalloc(lengthQuery)) == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "szQueryUri memory allocation is failed.");
        return OC_STACK_ERROR;
    }

    memset(szQueryUri, 0, lengthQuery);
    if( DAStrcat(szQueryUri, lengthQuery, gCloudAddress) == NULL )
    {
        ret = OC_STACK_NO_MEMORY;
    }
    if( DAStrcat(szQueryUri, lengthQuery, RESOURCE_DISCOVERY_QUERY_DI) == NULL )
    {
        ret = OC_STACK_NO_MEMORY;
    }
    if( DAStrcat(szQueryUri, lengthQuery, OCGetServerInstanceIDString()) == NULL )
    {
        ret = OC_STACK_NO_MEMORY;
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Find Resource to Cloud URI : %s", szQueryUri);

    OCCallbackData cbData;
    cbData.cb = handleFindResourceCB;
    cbData.context = (void*)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    if( ret != OC_STACK_NO_MEMORY )
    {
        ret = OCDoResource(NULL, OC_REST_DISCOVER, szQueryUri, NULL, 0, CT_DEFAULT,
                           OC_MEDIUM_QOS,
                           &cbData, NULL, 0);
    }

    if (ret != OC_STACK_OK)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Find Resource in Cloud is failed.");
    }

    DAFree(szQueryUri);

    return ret;
}

/**
 * @brief                 Call function to send request for Sign-UP to Cloud.
 * @details               It's Entry-Point to send Sign-UP request when receive Cloud-Provisioning Data.
 * @param eventData       Provisioned Cloud-Data for cloud connection.
 * @param timeout         time-out value how long wait cloud-response when send request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
static OCStackResult RegisterServerIntoCloud(ESCloudProvData* eventData, TimeOut_s* timeout)
{
    OCStackResult   res = OC_STACK_ERROR;

    if( eventData == NULL || (eventData->authCode[0] == 0 && eventData->accesstoken[0] == 0))
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid eventData.");
        return res;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "AuthCode     : %s", eventData->authCode);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Access Token : %s", eventData->accesstoken);

    if ( SignedUp_data != NULL )
    {
        ESCloud_SignUp_clear(SignedUp_data);
        SignedUp_data = NULL;
    }

    int port = atoi(gCloudPort);
    if( gCloudIP[0] == NULL || gCloudPort[0] == NULL || 0 >= port || port > 65535 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud info is invalid.(gCloudIP=%s, gCloudPort=%s, port=%d)", gCloudIP, gCloudPort, port);
        return res;
    }

    if ( ESCloudState_SetAndNotify( ES_STATE_REGISTERING_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL ) != 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
        return res;
    }

    OICPingSetMask(gCloudIP, (uint16_t)port, PING_ST_ISCLOUD);

    // [Jay] Get Session Key
    res = OCCloudSignup(gCloudAddress,
                        OCGetServerInstanceIDString(),
                        eventData,
                        handleRegisterCB,
                        handleSignUPTimeOut,
                        timeout);

    if(res != OC_STACK_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud Server Registration Failed : %d", res);
        OICPingUNSetMask(gCloudIP, PING_ST_ISCLOUD);
        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL ) != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
            return res;
        }
    }

    return res;
}

/**
 * @brief                 Call function to send request for Refreshing access-token from Cloud.
 * @details               It's Entry-Point to send "RefreshToken" request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult RefreshTokenIntoCloud(void)
{
    OCStackResult   res = OC_STACK_ERROR;
    if(SignedUp_data == NULL || SignedUp_data->accessToken == NULL || strlen(SignedUp_data->accessToken) < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No Session Key Retrived from the Cloud ");
    }
    else
    {
        res = OCCloudRefresh(gCloudAddress,
                SignedUp_data->uid,
                OCGetServerInstanceIDString(),
                SignedUp_data->refreshToken,
                handleRefreshTokenCB);

        THINGS_LOG_D(THINGS_DEBUG, TAG, "OCCloud Refresh Token return : %d", res);
    }

    if(res != OC_STACK_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Cloud accessToken refresh Failed : %d", res);
    }

    return res;
}

int PushNotificationToCloud(const char* uri, OCRepPayload* payload)
{
   THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

   int ret = 0;

   if( OICGetResetMask(RST_NOTI_MODULE_DISABLE) == true )
   {
       THINGS_LOG(THINGS_INFO, TAG, "Notification Module Disable.");
       ret = 0;
   }
   else if(NULL != payload)
   {
       if(OC_STACK_OK
            == OCCloudTopicPublishTopic(gCloudAddress,
                                        uri,
                                        payload,
                                        handlePublishTopicCB) )
       {
           ret = 1;
       }
   }

   THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

   return ret;
}

/**
 * @brief                 Making Cloud-Address format to be used for Cloud Connection.
 * @details               Cloud-Address format : coaps+tcp://XXX.XXX.XXX.XXX:XXXXX
 * @param ip              Cloud IPv4 address.
 * @param port            Cloud Port number.
 * @param ciAddr          Cloud address. (XXXXX/IP:Port format)
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
static char* MakeCloudAddress(char* ip, char* port, const char* ciAddr)
{
    char * delmem = NULL;
    char * ipport = (char*)ciAddr;
    THINGS_LOG_D(THINGS_DEBUG,TAG,"ip=%s, port=%s, ciAddr=%s", ip, port, ciAddr);

    if( ciAddr != NULL && strlen(ciAddr) > 0)               // ci address.
    {
        char * point = strstr(ciAddr, DEFAULT_COAP_TCP_HOST);
        if(point)
        {
            ipport = point+strlen(DEFAULT_COAP_TCP_HOST);
        }
    }
    else if( (ipport = MakeIpPort(ip, port)) != NULL )    // ip , port coupling.
    {
        delmem = ipport;
    }
    else                                                  // ext case.
    {
        ipport = DEFAULT_CI_HOST;
    }

    // Update Cloud Address.
    memset(gCloudAddress, 0, MAX_CI_ADDRESS);
    if( DAStrcat(gCloudAddress, MAX_CI_ADDRESS, DEFAULT_COAP_TCP_HOST) == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR,TAG,"DAStrcat() is failed.");
        if( delmem )  DAFree(delmem);
        return NULL;
    }

    if( DAStrcat(gCloudAddress, MAX_CI_ADDRESS, ipport) == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR,TAG,"DAStrcat() is failed.");
        if( delmem )  DAFree(delmem);
        return NULL;
    }

    if( delmem )
        DAFree(delmem);

    return gCloudAddress;
}

/**
 * @brief                 Branch-Function whether Sign-UP or Sign-IN step.
 * @details               It's decide how to send request base on argument-data. (Sign-UP request or Sign-IN request.)
 * @param cloud_adress    Cloud address. (XXXXXIPv4:Port format)
 * @param eventData       Received Cloud-Provisioning Data.
 * @param list            Resources List pointer. (for Cloud Publishing Resource.)
 * @param length          ES of Resources in "list".
 * @param timeout         Time-out value how long wait cloud-response when send request.
 * @return                Success      return  0 \n
 *                        Failed       return  1 \n
 *                        System Error return -1
 */
static int StartCIConnection(const char* cloud_adress, ESCloudProvData* eventData,
                                                       OicResource** list,
                                                       int length,
                                                       TimeOut_s* timeout)
{
    int ret = 0;
    OCStackResult result = OC_STACK_ERROR;

    gResourceLists = list;
    gLen = length;

    if( MakeCloudAddress(NULL,NULL,cloud_adress) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR,TAG,"Making Cloud Address is failed.");
        result = OC_STACK_NO_MEMORY;
    }

    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr Addr         : %s", gCloudAddress);
    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr AuthCode     : %s", eventData->authCode);
    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr Accesstoken  : %s", eventData->accesstoken);
    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr Uid          : %s", eventData->uid);
    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr AuthProvider : %s", eventData->authProvider);
    THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr clientID     : %s", eventData->clientID);

    if(timeout != NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr timeout_cnt     : %d", timeout->cur_counter);
        THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr timeout_cur_num : %d", timeout->cur_num);
        THINGS_LOG_D(THINGS_DEBUG,TAG,"CI Svr timeout_ori_num : %d", timeout->ori_num);
    }

    if( result != OC_STACK_NO_MEMORY )
    {
        if( eventData != NULL && (eventData->authCode[0] != 0 || eventData->accesstoken[0] != 0))
        {
            result = RegisterServerIntoCloud(eventData, timeout);
        }
        else
        {
            if( ESCloud_SignUp_init(&SignedUp_data) == true
                && (ret = DM_LoadLegacyCloudData(&SignedUp_data)) == 1 )
            {
                result = LogInOutToCloud(true, timeout);
            }
            else
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "Sign-UP data load failed.");
            }
        }
    }

    if(OC_STACK_OK != result )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR,TAG,"StartCIConnection() is failed.");
    }
    else
    {
        ret = 1;    // Success.
    }

    return ret;
}

/**
 * @brief                 Cloud Connection Entry-Point Function.
 * @details               Entry-Point for Cloud Connection at all-of-situation. (It support Multi-Thread safty.)
 * @param param           It contain follow Data Structure. \n
 *                        1. ESCloudProvData : Provisioned Cloud-Data. \n
 *                        2. TimeOut_s : Time-Out value for Cloud-response Time-Out processing.
 * @return                Always return NULL
 */
static void* CIConnectionInitLoop(ESCloudEvent_TimeOut *param)
{
    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int res = 0;
    char* CIip = NULL;
    char* CIipPort = NULL;
    char* CIhost = NULL;
    ESErrorCode ESerr = ES_ERRCODE_UNKNOWN;

    CICPDelIsThereCP();

    if( param == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] parameter is NULL.");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL ) != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "StateAndNotify is failed.");
        }

        forceSessionStop(CISESS_NULL);
        return NULL;
    }

    ESCloudProvData *eventData = &param->event;

    if( eventData->hostName[0] != 0 )
        CIhost = &(eventData->hostName[0]);
    else if( eventData->ip[0] != 0 )
        CIhost = &(eventData->ip[0]);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "* param add= 0x%X *", param);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "* CIhost   = %s *", CIhost);

    // Cross check if a network is available
    if( ESMGetNetworkStatus() )
    {
        if(gServerBuilder != NULL)
        {
            THINGS_LOG_V(THINGS_INFO, TAG, "##########################");
            THINGS_LOG_V(THINGS_INFO, TAG, "  Start Cloud Connection ");
            THINGS_LOG_V(THINGS_INFO, TAG, "##########################");

            if( CIConnectionPreCheck(CIhost, &CIip) == 0 )
            {
                CIipPort = MakeIpPort(CIip, eventData->port);
                DAStrncpy(gCloudIP, CIip, IP_PORT);
                DAStrncpy(gCloudPort, eventData->port, IP_PORT);

                pthread_mutex_lock(&gESmutex);
                res = StartCIConnection(DM_GetOCFCloudAddress(CIipPort),
                                        eventData,
                                        gServerBuilder->gResArr,
                                        gServerBuilder->res_num,
                                        &param->timeOut);
                switch(res)
                {
                case 1:     // Success
                    usleep(500000);     // 0.5 sec wait (need for Cloud CI Server cooling.)
                    break;
                case  0:    // Fail StartCIConnection.
                    ESerr = ES_ERRCODE_UNKNOWN;
                    break;
                case -1:    // Json Parsing error. or Memory Alloc error. or Invalid argument error.
                    ESerr = ER_ERRCODE_SYSTEM_ERROR;
                    break;
                case -2:    // Invalid Contents of Json file.
                    ESerr = ER_ERRCODE_INVALID_SAVED_CLOUD_DATA;
                    break;
                }
                THINGS_LOG_D(THINGS_DEBUG, TAG, "res = %d, ESerr = %d", res, ESerr);
                pthread_mutex_unlock(&gESmutex);


                if(CIipPort)
                {
                    DAFree(CIipPort);
                    CIipPort = NULL;
                }
                if(CIip)
                {
                    DAFree(CIip);
                    CIip = NULL;
                }
            }
            else
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "AP is Not Connected to Internet.");
                ESerr = ES_ERRCODE_NO_INTERNETCONNECTION;
            }
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ServerBuilder is NULL.");
            ESerr = ER_ERRCODE_SYSTEM_ERROR;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Device is not connected to AP.");
        ESerr = ES_ERRCODE_UNKNOWN;
    }

    if(res != 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "###################################");
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "  Exiting without Cloud Connection ");
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "###################################");

        if( ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ESerr, NULL, NULL ) != 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "StateAndNotify is failed.");
        }

        forceSessionStop(CISESS_NULL);
    }

    DAFree(param);
    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return NULL;
}

/**
 * @brief                 Wait & Call Cloud Connection Entry-Point Function.
 * @details               Called this function when there is already Cloud Connection-try process. \n
 *                        And, this function wait until Cloud Session STOP step.
 * @param param           It contain follow Data Structure. \n
 *                        1. ESCloudProvData : Provisioned Cloud-Data. \n
 *                        2. TimeOut_s : Time-Out value for Cloud-response Time-Out processing.
 * @return                Always return NULL
 */
static void* CIConnectionWaitingLoop(ESCloudEvent_TimeOut *param)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Sleep before CloudRetrySignUP");
    sleep(1);
    if( param == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] parameter is NULL.");
        CICPDelIsThereCP();
        CICPDelPendedData();
        ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
        ESMGetNetworkStatus();  // State return
        return NULL;
    }

    while(gQisCloudThreadRunning != CISESS_STOP)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Waiting ...");
        usleep(100000);
    }

    CICPPendEventData(&(param->event));
    gQisCloudThreadRunning = CISESS_BUSY;
    THINGS_LOG(THINGS_DEBUG, TAG, "Create CloudInit thread");

    CIConnectionInitLoop(param);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return NULL;
}

/**
 * @brief                 Entry-Point when receive Cloud-Provisioning.
 * @details               Managing Cloud session for Cloud Connection safety against Multiple-Provisioning & Auto Sign-IN.
 * @param eventData       Received Cloud-Provisioning Data.
 * @return                Always return NULL
 */
void* cloudDataCbESM(ESCloudProvData *eventData)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "gQisCloudThreadRunning = %d", gQisCloudThreadRunning);

    pthread_t threadHandler;
    ESCloudEvent_TimeOut * clonedData = NULL;
#if 0 // pkcloud
    if (CICPCASIsThereCPIFfalse() == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Already exist data of Cloud Provisioning.");
        return NULL;
    }
#endif
    switch(gQisCloudThreadRunning)
    {
    case CISESS_APDISCON:
        // Backup loose try..
        CICPPendEventData(eventData);
        return NULL;
    case CISESS_BUSY:     // Occupied
    case CISESS_STOP_TRIGGER:     // Re-Start triggered.
    case CISESS_SIGNOUT:     // doing Auto Log-Out
        break;
    case CISESS_STOP:     // can start.
        DelAllRequestHandle();
        SendCnt_Sign_UP = 0;
        CICPPendEventData(eventData);
        gQisCloudThreadRunning = CISESS_BUSY;

        if( (clonedData = cloneDataAddTimeOut(eventData, NULL)) != NULL)
        {
#ifdef OCF_RTOS
        	int retp = pthread_create_rtos(&threadHandler,NULL, (pthread_FuncType)CIConnectionInitLoop, (void *)clonedData, THINGS_STACK_CICONNETION_INIT_THREAD);
#else

        	int retp =  OICThread_create(&threadHandler, NULL, (pthread_FuncType)CIConnectionInitLoop, (void *)clonedData) != 0 )
#endif
        	 if(retp != 0)
        	{
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.");
            DAFree(clonedData);
            clonedData = NULL;
            gQisCloudThreadRunning = CISESS_STOP;
            ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
        	}
        }
        else
        {
        	THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "cloneDataAddTimeOut is failed.");
        }
        return NULL;
    default:
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not Surpport value(%d) of gQisCloudThreadRunning.", gQisCloudThreadRunning);
        return NULL;
    }

    DelAllRequestHandle();
    SendCnt_Sign_UP = 0;

    if( CloudRetrySignUP(eventData, NULL) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "CloudRetrySignUP is failed.");
        CICPDelPendedData();
        CICPDelIsThereCP();
        ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
        ESMGetNetworkStatus();  // State return
    }

//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return NULL;
}

/**
 * @brief                 Cloud Re-SignIN Process.
 * @details               Entry-Pointer for Auto-Sign-IN process.
 * @param timeout         Time-out value how long wait cloud-response when send request. \n
 *                        NULL : Use with default Time-Out Counter. \n
 *                        Other: Use with User-defined Time-Out Counter.
 * @return                Always return 0
 */
int CloudRetrySignIN(TimeOut_s* timeout)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    pthread_t threadHandler;
    ESCloudProvData dummyData;
    ESCloudEvent_TimeOut * clonedData = NULL;

    if( CICPGetIsThereCP() == true )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "There is a CloudProvisioning data. Cloud-retry is skiped.");
        return 1;
    }

    forceSessionStop(CISESS_NULL);

    // Need to check the running state of the thread...
    if(gQisCloudThreadRunning == CISESS_STOP)
    {
        gQisCloudThreadRunning = CISESS_BUSY;
        THINGS_LOG(THINGS_DEBUG, TAG, "Create CloudInit thread");

        // CloudData setting.
        initESCloudProvData(&dummyData);

        if( (clonedData = cloneDataAddTimeOut(&dummyData, timeout)) == NULL ||
#ifdef OCF_RTOS
        		pthread_create_rtos(&threadHandler, NULL, (pthread_FuncType)CIConnectionInitLoop, (void *)clonedData, THINGS_STACK_CICONNETION_INIT_THREAD) != 0 )
#else
        	 OICThread_create(&threadHandler, NULL, (pthread_FuncType)CIConnectionInitLoop, (void *)clonedData) != 0 )
#endif
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.");
            DAFree(clonedData);
            clonedData = NULL;
            gQisCloudThreadRunning = CISESS_STOP;
            ESCloudState_SetAndNotify( ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL );
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return 0;
}

/**
 * @brief                 Cloud Re-SignUP Process.
 * @details               Entry-Pointer for Sign-UP Re-Transmission process.
 * @param EventData       Pended Cloud-Provisioning Data.
 * @param timeout         Time-out value how long wait cloud-response when send request. \n
 *                        NULL : Use with default Time-Out Counter. \n
 *                        Other: Use with User-defined Time-Out Counter.
 * @return                Success : return 1
 *                        Failed :  return 0
 */
static int CloudRetrySignUP(ESCloudProvData* EventData, TimeOut_s* TimeOut)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    pthread_t threadHandler;
    ESCloudEvent_TimeOut * clonedData = NULL;

    if(EventData == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "EventData is NULL.");
        return 0;
    }

    gQisCloudThreadRunning = CISESS_STOP_TRIGGER;
    ESMGetNetworkStatus();  // State return

    if( (clonedData = cloneDataAddTimeOut(EventData, TimeOut)) == NULL ||
#ifdef OCF_RTOS
         pthread_create_rtos(&threadHandler, NULL, (pthread_FuncType)CIConnectionWaitingLoop, (void *)clonedData, THINGS_STACK_CICONNETION_INIT_THREAD) != 0 )
#else
    OICThread_create(&threadHandler, NULL, (pthread_FuncType)CIConnectionWaitingLoop, (void *)clonedData) != 0 )
#endif
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thread is failed.");
        DAFree(clonedData);
        clonedData = NULL;
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.");
    return 1;
}

static void CloudRequestRetry_Trigger(TimeOut_s* TimeOut)
{
	// Stored Backup data is base on time-out.
    ESCloudProvData * pendedEvent = NULL;
    if( (pendedEvent = CICPGetPendedData()) != NULL)
    {
        CICPDelIsThereCP();
        cloudDataCbESM(pendedEvent);       // 1. Last Cloud Provisioning-ed data. or Last Send Failed data.(Sign-UP)
    }
    else
    {
        CloudRetrySignIN(TimeOut);    // default. Cloud retry. (Sign-IN / Resource Publish failed data)
    }
}

static ESCloudProvData* IfFailedThenRetry(OCDoHandle handle, OCStackResult result, int* nErr)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "latestRequest 0x%X, RespCB:0x%X]", GetRequestHandle(), handle);

    ESCloudProvData* pendedData = NULL;
    ESCloudProvData* cloneData = NULL;
    *nErr = 0;

    CICPEnterPenddataBlock();
    if(CASRequestHandle(handle, NULL) == 1 )
    {
        if(result == OC_STACK_COMM_ERROR)        // Don't Send Request Message.
        {
            if(gQisCloudThreadRunning != CISESS_APDISCON)
            {
                CloudRequestRetry_Trigger(NULL);
            }
            *nErr = 1;  // communication error.
        }
        else
        {
            DelAllRequestHandle();

            if( (pendedData = CICPGetPendedData()) == NULL )
            {
                THINGS_LOG_V(THINGS_INFO, TAG, "Not exist Pended Event Data.");
                *nErr = 2;  // not exist pendedData.
                goto GOTO_OUT;
            }

            if( (cloneData = DAMalloc(sizeof(ESCloudProvData))) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed for cloneData.");
                *nErr = 3;  // mem allocation error.
                goto GOTO_OUT;
            }

            cloneESCloudProvData(cloneData, pendedData);
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Response Handle(0x%X) is not exist in the Request Handle list.", handle);
        *nErr = 4;
    }

GOTO_OUT:
    CICPReleasePenddataBlock();

    if( *nErr != 1 && *nErr != 4 )
        CICPDelPendedData();

    return cloneData;
}

void CIStopCloudConnection(void* CBfunc)
{
    forceSessionStop(CISESS_NULL);
    ESMGetNetworkStatus();  // State return
    sleep(1);

    THINGS_resCBfunction = NULL;
    if( CBfunc )
        THINGS_resCBfunction = (OICCloudConResultFuncType*)CBfunc;
}

static void forceSessionStop(CISESS_LEVEL state)
{
    DelAllRequestHandle();
    if( state == CISESS_SIGNOUT)
        gQisCloudThreadRunning = CISESS_SIGNOUT;
    else
        gQisCloudThreadRunning = CISESS_STOP_TRIGGER;
    ESMGetNetworkStatus();  // State return
}

int CIRetryStop_byWiFiCB( bool isRetry )
{
    if(isRetry == true)
    {   // Last Failed Cloud Connection Retry. (Permanent setting)
        DelAllRequestHandle();
        gQisCloudThreadRunning = CISESS_STOP;
        CloudRequestRetry_Trigger(NULL);
    }else if(isRetry == false)
    {   // if Cloud Session valid, then Stop Cloud Connection. And Don't Retry.
        DelAllRequestHandle();
        gQisCloudThreadRunning = CISESS_APDISCON;
        ESMGetNetworkStatus();  // State return
    }

    return 1;
}

// -1 : System Error.  0 : No process.  1 : Sign-IN.
int CIRetryStop_byTCPCB(const char* addrIP, const int port)
{
    int ret = 1;

    pthread_mutex_lock(&gESTCPsessmutex);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    int CIport = 0;

    if( gQisCloudThreadRunning != CISESS_BUSY || gCloudPort[0] == 0 || gCloudIP[0] == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Don't need something process. IP=%s, Port=%s, RunningState=%d", gCloudIP, gCloudPort, gQisCloudThreadRunning);
        ret = 0;
        goto GOTO_OUT;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "IP = %s, port = %d", addrIP, port);        // I expect that ( IP = 52.37.99.111, port = 443 )

    if( (CIport = atoi(gCloudPort)) <= 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] Cloud Port is not invalid.(gCloudPort = %s = %d)", gCloudPort, CIport);
        ret = -1;
        goto GOTO_OUT;
    }

    if( strcmp(gCloudIP, addrIP) != 0 || CIport != port )
    {
        THINGS_LOG_V( THINGS_INFO, TAG, "We Support IP:Port is \"%s:%s\"", gCloudIP, gCloudPort );
        if( strcmp(gCloudIP, addrIP) != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Delete OIC_Ping for IP=%s.(Cur_Cloud=%s)", addrIP, gCloudIP);
            OICPingUNSetMask(addrIP, PING_ST_ISCLOUD);
        }
        ret = 0;
        goto GOTO_OUT;
    }

    // Case Cloud Connection is BUSY & AP Connected.
    if( ESGetCloudLogINState() == false )
    {
        THINGS_LOG_V( THINGS_INFO, TAG, "process Re-Transmission in Time-Out Process instated of this." );
        ret = 0;
        goto GOTO_OUT;
    }

    // Force Strong Re Sign-IN or new Sign-UP start.
    forceSessionStop(CISESS_SIGNOUT);
    CloudRequestRetry_Trigger(NULL);

GOTO_OUT:
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    pthread_mutex_unlock(&gESTCPsessmutex);

    return ret;
}

bool ESCloud_SessionStopTrigger(ESEnrolleeState esState)
{
    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if( gQisCloudThreadRunning != CISESS_APDISCON )
    {
        if((gQisCloudThreadRunning == CISESS_STOP_TRIGGER ||
                (ES_STATE_INIT <= esState && esState <= ES_STATE_CONNECTED_TO_ENROLLER))
                && gQisCloudThreadRunning != CISESS_SIGNOUT
                && ESGetCloudLogINState() == true)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Sign-Out Start.");
            LogInOutToCloud(false, NULL);
            return true;
        }

        THINGS_LOG_D(THINGS_DEBUG, TAG, "esState=%s, GetEnrolleeState()=%s", getProvStatus(esState), getProvStatus(GetEnrolleeState()) );

        if( ES_STATE_INIT <= esState && esState <= ES_STATE_CONNECTED_TO_ENROLLER)
        {
            if( gQisCloudThreadRunning != CISESS_BUSY && gQisCloudThreadRunning != CISESS_APDISCON &&  ESGetCloudLogINState() == false)
            {
                THINGS_LOG_D(THINGS_DEBUG, TAG, "Setting Cloud Semaphore init.");
                gQisCloudThreadRunning = CISESS_STOP;
            }
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "gQisCloudThreadRunning=%d", gQisCloudThreadRunning);

    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return false;
}

static int ESCloudState_SetAndNotify(CloudStatus state, ESErrorCode ESerr,
                                     OCClientResponse * response, CIErrorCode * CIerr )
{
    const char * strState = getProvStatus(state);

    if(    strState == NULL
        || strcmp(strState, "UNKNOWN") == 0
        || state == ES_STATE_INIT
        || state == ES_STATE_CONNECTED_TO_ENROLLER)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "state is invalid value(%d)", state);
        return -1;
    }

    if( ESSetState(state) == ES_ERROR )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Cloud State(%s) is failed.", strState);
        return 1;
    }


    if( response != NULL && CIerr != NULL)
    {
        // Method don't care because issue is not occurred by it.
        if( GetCloudCode(response, OC_REST_POST, CIerr) == 0 )
            *CIerr = ERRCI_UNKNOWN;

        ESSetCloudErrorCode(*CIerr);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Provisioning lastErrCode = %d", ESerr);
        if( ESerr == ES_ERRCODE_UNKNOWN)
            ESSetCloudErrorCode(ERRCI_UNKNOWN);
        else
            ESSetErrorCode(ESerr);
    }

    ProvRSCNotifyAllObservers();

    switch(state)
    {
    case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD :
    case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
        CIfinishCloudCon(0);
        break;
    case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD :
        CIfinishCloudCon(1);
        break;
    }

    return 0;
}

void ESCloud_SignUp_clear(ESCloud_SignUp * data)
{
    if ( data == NULL )
        return ;

    if( data->domain != NULL )
    {
        DAFree( data->domain );
        data->domain = NULL;
    }

    if( data->address != NULL )
    {
        DAFree( data->address );
        data->address = NULL;
    }

    if( data->port != NULL )
    {
        DAFree( data->port );
        data->port = NULL;
    }

    if( data->accessToken != NULL )
    {
        DAFree( data->accessToken );
        data->accessToken = NULL;
    }

    if( data->refreshToken != NULL )
    {
        DAFree( data->refreshToken );
        data->refreshToken = NULL;
    }

    if( data->tokenType != NULL )
    {
        DAFree( data->tokenType );
        data->tokenType = NULL;
    }

    if( data->certificate != NULL )
    {
        DAFree( data->certificate );
        data->certificate = NULL;
    }

    if( data->redirectURI != NULL )
    {
        DAFree( data->redirectURI );
        data->redirectURI = NULL;
    }

    if( data->sid != NULL )
    {
        DAFree( data->sid );
        data->sid = NULL;
    }

    if( data->uid != NULL )
    {
        DAFree( data->uid );
        data->uid = NULL;
    }

    data->expireTime = CLOUD_EXPIRESIN_INVALID;

    DAFree(data);
}

bool ESCloud_SignUp_init(ESCloud_SignUp ** data)
{
    if (data == NULL)
        return false;

    if( *data != NULL )
    {
        ESCloud_SignUp_clear( *data );
        *data = NULL;
    }

    *data = (ESCloud_SignUp*)DAMalloc( sizeof(ESCloud_SignUp) );
    if ( *data == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "ESCloud_SignedUp struct data is NULL.");
        return false;
    }

    (*data)->domain = NULL;
    (*data)->address = NULL;
    (*data)->port = NULL;
    (*data)->accessToken = NULL;
    (*data)->refreshToken = NULL;
    (*data)->certificate = NULL;
    (*data)->expireTime = CLOUD_EXPIRESIN_INVALID;
    (*data)->redirectURI = NULL;
    (*data)->sid = NULL;
    (*data)->tokenType = NULL;
    (*data)->uid = NULL;

    return true;
}

void* ESCloud_Init(OICServerBuilder* serverBuilder)
{
    unsigned int timeV = (unsigned int)time(NULL);

    if( timeV == -1 )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Random Number Initialization is Failed.");
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Random Number Initialization is Success.");
        srand(timeV);
    }

    if( OICPingInit() == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "OICPing init is failed.");
        return NULL;
    }

    DelAllRequestHandle();
    memset(gCloudIP, 0, IP_PORT);
    memset(gCloudPort, 0, IP_PORT);
    gServerBuilder = serverBuilder;
    gResourceLists = NULL;
    gLen = 0;
//    gDelDeviceCompletFunc = NULL;
    SendCnt_Sign_UP = 0;
    ReTranslateRSCPublishCnt = 0;

    CICPInitEventPend();

    CARegisterNetworkMonitorHandler((CAAdapterStateChangedCB)OICAdapterStateCB,
                                    (CAConnectionStateChangedCB)OICTCPsessStateCB);

    return (void*)gServerBuilder;
}

void ESCloud_Terminate(void)
{
    forceSessionStop(CISESS_NULL);
    CICPDelPendedData();
    CICPDelIsThereCP();

    CAUnregisterNetworkMonitorHandler((CAAdapterStateChangedCB)OICAdapterStateCB,
                                        (CAConnectionStateChangedCB)OICTCPsessStateCB);

    ESMGetNetworkStatus();

    ESCloud_SignUp_clear(SignedUp_data);
    SignedUp_data = NULL;
    memset(gCloudAddress, 0, MAX_CI_ADDRESS);
    memset(gCloudIP, 0, IP_PORT);
    memset(gCloudPort, 0, IP_PORT);

    gServerBuilder = NULL;
    gResourceLists = NULL;
    gLen = 0;

//    gDelDeviceCompletFunc = NULL;
    SendCnt_Sign_UP = 0;
    ReTranslateRSCPublishCnt = 0;

    OICPingTerminate();
}

static int GetCloudCode(OCClientResponse* response, OCMethod method, CIErrorCode* err)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter");

    int ret = 0;
    OCRepPayloadValue *val = NULL;
    CAResponseResult_t rawResult = CA_EMPTY;

    if( response == NULL || response->payload == NULL || err == NULL || method == OC_REST_NOMETHOD)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "response(0x%X) or err(0x%X) is NULL", response, err);
        return ret;
    }

    rawResult = OCResultToNetworkResult(response->result, method);
    *err = rawResult * ERR_UPPER_UNIT;

    THINGS_LOG_D(THINGS_DEBUG, TAG, "result = %d, rawResult = %d, method = %d, *err = %d", response->result, rawResult, method, *err);

    val = ((OCRepPayload *)response->payload)->values;

     while (val != NULL)
     {
         if( strcmp(val->name, CLOUD_ERROR_CODE) == 0 )
         {
            if( val->type != OCREP_PROP_INT)
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DataType of Cloud-Err-Code is not Integer.");
            }
            else
            {
                *err += (CIErrorCode)val->i;
                ret = 1;
            }

            break;
         }

         val = val->next;
     }

     THINGS_LOG_D(THINGS_DEBUG, TAG, "*err = %d, ret = %d", *err, ret);

     THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit");
    return ret;
}

static CAResponseResult_t OCResultToNetworkResult(OCStackResult ocCode, OCMethod method)
{
    CAResponseResult_t ret = CA_EMPTY;

    switch(ocCode)
    {
        case OC_STACK_OK:
           switch (method)
           {
               case OC_REST_PUT:
               case OC_REST_POST:
                   // This Response Code is like HTTP 204 "No Content" but only used in
                   // response to POST and PUT requests.
                   ret = CA_CHANGED;
                   break;
               case OC_REST_GET:
                   // This Response Code is like HTTP 200 "OK" but only used in response to
                   // GET requests.
                   ret = CA_CONTENT;
                   break;
               default:
                   // This should not happen but,
                   // give it a value just in case but output an error
                   ret = CA_CONTENT;
                   THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Unexpected OC_STACK_OK return code for method [%d].",
                            method);
            }
            break;
        case OC_STACK_RESOURCE_CREATED:
            ret = CA_CREATED;
            break;
        case OC_STACK_RESOURCE_DELETED:
            ret = CA_DELETED;
            break;
        case OC_STACK_RESOURCE_CHANGED:
            ret = CA_CHANGED;
            break;
        case OC_STACK_INVALID_QUERY:
            ret = CA_BAD_REQ;
            break;
        case OC_STACK_INVALID_OPTION:
            ret = CA_BAD_OPT;
            break;
        case OC_STACK_FORBIDDEN_REQ:
            ret = CA_FORBIDDEN_REQ;
            break;
        case OC_STACK_NO_RESOURCE:
            ret = CA_NOT_FOUND;
            break;
        case OC_STACK_COMM_ERROR:
            ret = CA_RETRANSMIT_TIMEOUT;
            break;
        case OC_STACK_UNAUTHORIZED_REQ:
            ret = CA_UNAUTHORIZED_REQ;
            break;
        case OC_STACK_INTERNAL_SERVER_ERROR:
            ret = CA_INTERNAL_SERVER_ERROR;
            break;
        default:
            break;
    }
    return ret;
}

static char* MakeIpPort(char* pIP, char* pPort)
{
    char* ipport = NULL;
    int length = 0;

    if( pIP == NULL || pPort == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "IP=%s, or Port=%s is NULL.", pIP, pPort);
        return NULL;
    }

    length = strlen(pIP)+strlen(pPort)+2;
    if( (ipport = (char*)DAMalloc(length)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory allocation is failed.");
        return NULL;
    }

    memset(ipport, 0, length);
    if( DAStrcat(ipport, length, pIP) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DAStrcat is failed.");
        DAFree(ipport);
        return NULL;
    }
    ipport[strlen(pIP)] = ':';
    if( DAStrcat(ipport, length, pPort) == NULL)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DAStrcat is failed.");
        DAFree(ipport);
        return NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "IP=%s, Port=%s, ipport=%s", pIP, pPort, ipport);
    return ipport;
}


static ESCloudEvent_TimeOut * cloneDataAddTimeOut(ESCloudProvData* CloudData, TimeOut_s* timeout)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    ESCloudEvent_TimeOut* clonedData = NULL;

    if( CloudData == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "CloudData is NULL.");
        return NULL;
    }

    if( (clonedData = (ESCloudEvent_TimeOut*)DAMalloc(sizeof(ESCloudEvent_TimeOut)*1)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory allocation is failed.");
        return NULL;
    }

    cloneESCloudProvData(&clonedData->event, CloudData);

    if( timeout == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Default Setting to cloned Cloud Data.");
        clonedData->timeOut.cur_counter = DEFAULT_TIMEOUT;
        clonedData->timeOut.cur_num     = DEFAULT_TIMEOUT;
        clonedData->timeOut.ori_num     = DEFAULT_TIMEOUT;
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Custom Setting to cloned Cloud Data.");
        clonedData->timeOut.cur_counter = timeout->cur_counter;
        clonedData->timeOut.cur_num     = timeout->cur_num;
        clonedData->timeOut.ori_num     = timeout->ori_num;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "cur_counter = %d, cur_num = %d, ori_num = %d", clonedData->timeOut.cur_counter, clonedData->timeOut.cur_num, clonedData->timeOut.ori_num);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "clonedData add= 0x%X", clonedData);
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return clonedData;
}

static void CIfinishCloudCon( int result )
{
    char* domain = NULL;
    char* accessToken = NULL;
    ESCloud_SignUp* Cloud_data = NULL;

    if( THINGS_resCBfunction )
    {
            DM_LoadLegacyCloudData(&Cloud_data);   // TODO read Cloud-Data.
            if( Cloud_data != NULL )
            {
                if( Cloud_data->domain != NULL )
                    domain = strdup(Cloud_data->domain);
                if( Cloud_data->accessToken != NULL )
                    accessToken = strdup(Cloud_data->accessToken);
        }

        THINGS_resCBfunction(result, domain, accessToken);
        THINGS_resCBfunction = NULL;
    }

    if( domain )
    {
        DAFree(domain);
        domain = NULL;
    }

    if( accessToken )
    {
        DAFree(accessToken);
        accessToken = NULL;
    }
}

static void PublishDevProfileIntoCloud( TimeOut_s* TimeOut )
{
    if( OCCloudDevProfilePublish(gCloudAddress, handleDevProfileCB, handleDevProfileTimeOut, TimeOut) != OC_STACK_OK )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Publish Device Profile request is failed.");
    }
}
