//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define _BSD_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#ifdef __USE_MISC
#include <arpa/inet.h>
#include <errno.h>
#endif

#include "octypes.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "rd_client.h"

#include "thingsmalloc.h"
#include "thingslogger.h"
#include "escommon.h"
#include "cloud_connector.h"
#include "oicwaithandler.h"
#include "thingsdatamanager.h"

#include "wifi_common.h"

#define TAG "[cloudcntor]"

#define HOSTENT_INTERNAL_HEADER_SIZE        100

/** Account Device-Profile publish URI.*/
#define OC_RSRVD_ACCOUNT_DEVPROFILE_URI    "/oic/account/profile/device"

static OCRepPayload* makeDevProfilePayload(const STDevice* devInfo);

#ifdef __USE_MISC
#if 0
/**
 * @brief          Look-up IP address for Doman Name.
 * @param pDNS     wanted Domain Name string to look-up IP. (unit: const char*)
 * @param nErr     Variable pointer to store Error value of look-up. (unit: int*)
 * @return         success IP address(unit: string), failure 0
 */
static char* OCCloud_DomainName2IP(const char * pDNS, int * nErr)
{
    size_t i = 0;
    int retVal = -1;
    int length = HOSTENT_INTERNAL_HEADER_SIZE;
    struct hostent pResult;
    struct hostent* pHost = NULL;
    char* buffer = NULL;
    char * pIP = NULL;

    if(pDNS == NULL || nErr == NULL)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] pDNS=%s, nErr=%d", pDNS, nErr);
        return pIP;
    }

    length += strlen(pDNS);
    if( (buffer = (char*)DAMalloc(length)) == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] buffer is NULL.(memory allocation is failed)");
        return pIP;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "pDNS=%s, length=%d", pDNS, length);

    while( (retVal = gethostbyname_r(pDNS, &pResult, buffer, length, &pHost, nErr)) == ERANGE)
    {   // Not enough memory of buffer.
        retVal = -1;
        DAFree(buffer);
        length += HOSTENT_INTERNAL_HEADER_SIZE;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "DNS Server require more memory. re-alloc memory buffer.(length=%d)", length);

        if( (buffer = (char*)DAMalloc(length)) == NULL )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] buffer is NULL.(memory allocation is failed)");
            return pIP;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "pHost=0x%X, retVal=%d, nErr=%d", pHost, retVal, *nErr);

    if (pHost == NULL || retVal != 0)
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "~SocketResolveDNS - Could not resolve host");
        DAFree(buffer);
        return pIP;
    }



    while(pHost->h_addr_list[i] != NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "SocketResolveDNS - Host: %s = %s",
                                 pHost->h_name, inet_ntoa( *(struct in_addr *) (pHost->h_addr_list[i])));

        if (retVal == 0)
        { //if first valid value: it has highest prio so return it
            pIP = strdup(inet_ntoa( *(struct in_addr *) (pHost->h_addr_list[i])));
            if( pIP == NULL || strcmp(pIP, "10.0.0.1")==0 )
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "There is Bug in gethostbyname_r() of GNU-libc library.\n\t\t pIP(%s) for Domain(%s).", pIP, pDNS);
                if(pIP)
                {
                    DAFree(pIP);
                    pIP = NULL;
                }
                retVal = -1;
            }
            else
            {
                THINGS_LOG_D(THINGS_INFO, TAG, "Found IP & Registered to pIP(%s) for Domain(%s).", pIP, pDNS);
                retVal = 1;
            }
        }

      i++;
    }

    if(pIP == NULL )
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Not Found IP. Check your network.");
    }

    DAFree(buffer);
    return pIP;
}

#if 0
/**
 * @brief          Look-up Domain Name for IP address.
 * @param pIP      wanted IP address string to look-up Domain Name. (unit: const char*)
 * @param type     Version type of param pIP. (It's only support AF_INET)
 * @param nErr     Variable pointer to store Error value of look-up. (unit: int*)
 * @return         success Domain Name(unit: string), failure 0
 */
static char* OCCloud_IP2DomainName(const char * pIP, int type, int * nErr)
{
    struct in_addr iaddr;

    int retVal = -1;
    int length = HOSTENT_INTERNAL_HEADER_SIZE;
    struct hostent pResult;
    struct hostent* pHost = NULL;
    char* buffer = NULL;
    char * pDNS = NULL;

    if(pIP == NULL || nErr == NULL || type != AF_INET)
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] pIP=%s, type=%d, nErr=%d", pIP, type, nErr);
        return pDNS;
    }

    length += strlen(pIP);
    if( (buffer = (char*)DAMalloc(length)) == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] buffer is NULL.(memory allocation is failed)");
        return pDNS;
    }

    if (!inet_aton(pIP, &iaddr))
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] IP converter is failed. pIP=%s", pIP);
        return pDNS;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "pIP=%s, length=%d", pIP, length);

    while( (retVal = gethostbyaddr_r((void*)&iaddr, sizeof(struct in_addr), type, &pResult, buffer, length, &pHost, nErr)) == ERANGE)
    {   // Not enough memory of buffer.
        retVal = -1;
        DAFree(buffer);
        length += HOSTENT_INTERNAL_HEADER_SIZE;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "DNS Server require more memory. re-alloc memory buffer.(length=%d)", length);

        if( (buffer = (char*)DAMalloc(length)) == NULL )
        {
            THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "[Error] buffer is NULL.(memory allocation is failed)");
            return pDNS;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "pHost=0x%X, retVal=%d, nErr=%d", pHost, retVal, *nErr);

    if (pHost == NULL || retVal != 0)
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "~SocketResolveIP - Could not resolve host");
        DAFree(buffer);
        return pDNS;
    }

    if(pHost->h_name != NULL)
    {
        pDNS = strdup(pHost->h_name);
        THINGS_LOG_D(THINGS_INFO, TAG, "Found DomainName & Registered to pDNS(%s) for IP(%s).",pDNS ,pIP);
    }
    else
    {
        THINGS_LOG_D(THINGS_INFO, TAG, "Not Found DomainName. Check your network.");
    }

    DAFree(buffer);
    return pDNS;
}
#endif

static int CICheckDomain(const char* DomainName, char ** pIP)
{
    int nErr = -1;
    char * ip = NULL;

    if( DomainName == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG,"[Error] Domain Name is NULL.");
        return nErr;
    }

    if( (ip = OCCloud_DomainName2IP(DomainName, &nErr)) == NULL )
    {
        switch(nErr)
        {
        case HOST_NOT_FOUND:
            THINGS_LOG_D(THINGS_DEBUG, TAG,"nErr = HOST NOT FOUND");
            break;
        case TRY_AGAIN:
            THINGS_LOG_D(THINGS_DEBUG, TAG,"nErr = TRY_AGAIN");
            break;
        case NO_RECOVERY:
            THINGS_LOG_D(THINGS_DEBUG, TAG,"nErr = NO_RECOVERY");
            break;
        case NO_DATA:
            THINGS_LOG_D(THINGS_DEBUG, TAG,"nErr = NO_DATA");
            break;
        default:
            THINGS_LOG_D(THINGS_DEBUG, TAG,"nErr = %d  UNKOWNED", nErr);
            nErr = -1;
            break;
        }
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG,"ip = %s, nErr = %d, pIP = 0x%X", ip, nErr, pIP);

    if( ip != NULL )
    {
        if( pIP != NULL )
        {
            *pIP = ip;
        }
        else
        {
            free(ip);
        }
    }

    return nErr;
}
#endif
#endif  // __USE_MISC


int CICheckDomain(const char* DomainName, char **pIP)
{

	char ipbuffer[20];
	memset(ipbuffer,0,20);
    char bytes[4];

	int ip4_address = 0;

	wifi_utils_result_e res =  wifi_net_hostname_to_ip4(DomainName, &ip4_address);
	if(res == WIFI_UTILS_SUCCESS)
	{
		bytes[0] = ip4_address & 0XFF;
		bytes[1] = (ip4_address >> 8) & 0XFF;
		bytes[2] = (ip4_address >> 16) & 0XFF;
		bytes[3] = (ip4_address >> 24) & 0XFF;
		//54.85.39.78
		//78.85.39.54

		snprintf(ipbuffer, sizeof(ipbuffer), "%d.%d.%d.%d",bytes[0],bytes[1],bytes[2],bytes[3]);
		//sprintf(ipbuffer,"%s", "54.85.39.78");

		THINGS_LOG_D(THINGS_DEBUG, TAG, "DNS resolved IP for[%s] is =%s",DomainName, ipbuffer);

		if( pIP != NULL )
		{
			*pIP = strdup(ipbuffer);
		}
	}
	else
	{
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, " Dawit Failed to get the IP, hard coding the ip");
		sprintf(ipbuffer,"%s", "13.124.51.231");
		
		usleep(500);
		if( pIP != NULL )
		{
			*pIP = strdup(ipbuffer);
		}
	}

	return 0;
}

/**
 * @brief              Entry Point for checking about internet & Cloud connection.
 * @param DomainName   wanted Domain-Name string to look-up IP address. (unit: const char*)
 * @param pIP          Variable pointer to store Founded IP address. (unit: char**)
 * @return             success : return 0 \n
 *                     failure : return > 0
 */
int CIConnectionPreCheck(const char* DomainName, char ** pIP)
{
    int nErr = 0;   // Success return value

    THINGS_LOG_D(THINGS_DEBUG, TAG, "DomainName=%s, pIP=0x%X", DomainName, pIP);
#if 1

    if( CICheckDomain("samsung.com", NULL) != 0 )
    {
        if( DomainName == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "AP is Not Connected to Internet.");
            return -1;
        }
    }

    if( DomainName != NULL )
    {
        nErr = CICheckDomain(DomainName, pIP);
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Found IP = %s, nErr = %d", *pIP, nErr);
    }
#else
    printf("cloud_connector.c: Not defined \"__USE_MISC\". So, It's Not support that checking AP internet connection.");
    THINGS_LOG_V(THINGS_INFO, TAG, "It's Not Support Checking whether AP is connected to internet.(Not defined __USE_MISC)");

    if(pIP != NULL && DomainName != NULL)
    {
        *pIP = strdup(DomainName);
    }
#endif

    return nErr;
}

/**
 * @brief                  Send Sign-UP request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param deviceId         Identifier of device. (unit: const char*)
 * @param eventData        Data structure pointer for Cloud-Provisioning Data.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudSignup(const char *host,
                            const char *deviceId,
                            const ESCloudProvData* eventData,
                            OCClientResponseHandler response,
                            CheckTimeOutCallFunc timeoutHandler,
                            TimeOut_s* timeout )
{
    OCDoHandle gReqHandle = NULL;
    OCStackResult result = OC_STACK_ERROR;
    char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
    OCCallbackData cbData;
    OCRepPayload *registerPayload = NULL;

    if( host == NULL || deviceId == NULL || eventData == NULL ||
        (eventData->accesstoken[0] == 0 && eventData->authCode[0] == 0) )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid eventData.");
        goto no_memory;
    }

    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_URI);
    THINGS_LOG_D(THINGS_INFO, TAG, "Signing up to : %s", targetUri);

    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

    registerPayload = OCRepPayloadCreate();
    if (!registerPayload)
    {
        goto no_memory;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "di              : %s", deviceId);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "authprovider    : %s", eventData->authProvider);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "devicetype      : resource_server");

    OCRepPayloadSetPropString(registerPayload, "di", deviceId);
    OCRepPayloadSetPropString(registerPayload, "authprovider", eventData->authProvider);
    OCRepPayloadSetPropString(registerPayload, "devicetype", "resource_server");

    if( eventData->accesstoken[0] != 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "accesstoken     : %s", eventData->accesstoken);
        OCRepPayloadSetPropString(registerPayload, "accesstoken", eventData->accesstoken);

        if( eventData->uid[0] != 0 )
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "uid             : %s", eventData->uid);
            OCRepPayloadSetPropString(registerPayload, "uid", eventData->uid);
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "authcode        : %s", eventData->authCode);
        OCRepPayloadSetPropString(registerPayload, "authcode", eventData->authCode);
    }

    if( eventData->clientID != NULL && eventData->clientID[0] != 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "clientid        : %s", eventData->clientID);
        OCRepPayloadSetPropString(registerPayload, OC_RSRVD_ES_CLIENTID, eventData->clientID);
    }

    THINGS_LOG_D(THINGS_INFO, TAG, "OCToCATransportFlags(CT_ADAPTER_TCP)");

#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    //CASelectCipherSuite(0x35, (1 << 4));
#endif

    if( isEmptyRequestHandle() == true )
    {
        result = OCDoResource(&gReqHandle, OC_REST_POST, targetUri, NULL, (OCPayload *)registerPayload,
                            CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

        if(result == OC_STACK_OK && timeoutHandler != NULL)
        {
            if( AddRequestHandle(gReqHandle) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] ReqHandles array space is small.");
                goto no_memory;
            }

            CreateTimeOutProcess(gReqHandle, timeoutHandler, timeout);
        }
    }

    return result;

no_memory:
    OCRepPayloadDestroy(registerPayload);
    return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Sign-IN request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User. (unit: const char*)
 * @param deviceId         Identifier of device. (unit: const char*)
 * @param accesstoken      received access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudSession(const char *host, const char *uId,
                             const char *deviceId,
                             const char *accesstoken,
                             bool isLogin, OCClientResponseHandler response,
                             CheckTimeOutCallFunc timeoutHandler,
                             TimeOut_s* timeout)
{
    OCDoHandle gReqHandle = NULL;
    OCStackResult result = OC_STACK_ERROR;
    char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
    char * coreVer = NULL;
    char * IoTivityVer = NULL;

    if( accesstoken == NULL || accesstoken[0] == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "accesstoken is invalid.(0x%X)", accesstoken);
        return OC_STACK_ERROR;
    }

    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_SESSION_URI);

    THINGS_LOG_D(THINGS_INFO, TAG, "Initiate Session with : %s", targetUri);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

    OCRepPayload *loginoutPayload = OCRepPayloadCreate();
    if (!loginoutPayload)
    {
        goto no_memory;
    }

    if( OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, (void**)&coreVer) != OC_STACK_OK ||
            (IoTivityVer = strdup(IOTIVITY_VERSION)) == NULL )
//            OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, (void**)&IoTivityVer) != OC_STACK_OK )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Getting Core_Spec_Ver or IoTivity_Ver is failed.");
        goto no_memory;
    }

    THINGS_LOG_D(THINGS_INFO, TAG, "coreversion      : %s", coreVer);
    THINGS_LOG_D(THINGS_INFO, TAG, "verticalversion  : %s", IoTivityVer);

    if( uId != NULL )
    {
        OCRepPayloadSetPropString(loginoutPayload, KEY_ID_USER, uId);
    }
    OCRepPayloadSetPropString(loginoutPayload, KEY_ID_DEVICE, deviceId);
    OCRepPayloadSetPropString(loginoutPayload, KEY_TOKEN_ACCESS, accesstoken);
    OCRepPayloadSetPropBool(loginoutPayload,   KEY_LOGINOUT, isLogin);
    OCRepPayloadSetPropString(loginoutPayload, KEY_ICORE_VER, coreVer);
    OCRepPayloadSetPropString(loginoutPayload, KEY_IOTIVITY_VER, IoTivityVer);

#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    //CASelectCipherSuite(0x35, (1 << 4));
#endif

    if( isEmptyRequestHandle() == true )
    {
        result = OCDoResource(&gReqHandle, OC_REST_POST, targetUri, NULL, (OCPayload *)loginoutPayload,
                            CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

        if(result == OC_STACK_OK && timeoutHandler != NULL )
        {
            if( AddRequestHandle(gReqHandle) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] ReqHandles array space is small.");
                goto no_memory;
            }

            CreateTimeOutProcess(gReqHandle, timeoutHandler, timeout);
        }
    }

    if( IoTivityVer )
    {
        DAFree(IoTivityVer);
        IoTivityVer = NULL;
    }
    if( coreVer )
    {
        DAFree(coreVer);
        coreVer = NULL;
    }
    return result;

no_memory:
    if( IoTivityVer )
    {
        DAFree(IoTivityVer);
        IoTivityVer = NULL;
    }
    if( coreVer )
    {
        DAFree(coreVer);
        coreVer = NULL;
    }
    OCRepPayloadDestroy(loginoutPayload);
    return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Resources Publish request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*)\n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param list             Resource List pointer for Publishing to Cloud.
 * @param length           Count of Resource in Resource List(list).
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudRscPublish(char* host, OicResource** list, int length,
                                OCClientResponseHandler response,
                                CheckTimeOutCallFunc timeoutHandler,
                                TimeOut_s* timeout)
{
    OCStackResult result = OC_STACK_ERROR;
    THINGS_LOG_D(THINGS_INFO, TAG, "Resource Publish Start.");

    OCDoHandle gReqHandle = NULL;
    OCCallbackData cbData;
    cbData.cb = response;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCResourceHandle    resourceHandles[length+2];

    THINGS_LOG_D(THINGS_INFO, TAG, "Publishing [%d] number of resources ", length);
    int iter = 0;
    for( iter = 0 ; iter < length ; iter++)
    {
        resourceHandles[iter] = list[iter]->resourceHandle;

        THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s ", list[iter]->uri);
    }

    uint8_t numResource = 0;
    OCGetNumberOfResources(&numResource);
    if(numResource > 0)
    {
        for(uint8_t index = 0 ; index < numResource ; index++)
        {
            OCResourceHandle handle =  OCGetResourceHandle(index);

            const char* uri = OCGetResourceUri(handle);

            if(0 == strcmp( uri, OC_RSRVD_DEVICE_URI)
                ||0 == strcmp( uri, OC_RSRVD_PLATFORM_URI) )
            {
                resourceHandles[iter++] = handle;
            }
        }
    }

    if( isEmptyRequestHandle() == true )
    {
        result = OCRDPublish(&gReqHandle, host,
                          CT_ADAPTER_TCP,
                          resourceHandles,
                          iter,
                          &cbData,
                          OC_LOW_QOS);

        if(result == OC_STACK_OK && timeoutHandler != NULL )
        {
            if( AddRequestHandle(gReqHandle) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] ReqHandles array space is small.");
                return OC_STACK_ERROR;
            }

            CreateTimeOutProcess(gReqHandle, timeoutHandler, timeout);
        }
    }

    return result;
}

OCStackResult OCCloudRscPublishWithDeviceID(char* host,
                                            const char* id,
                                            OicResource** list,
                                            int length,
                                            OCClientResponseHandler response,
                                            CheckTimeOutCallFunc timeoutHandler,
                                            TimeOut_s* timeout)
{
    OCStackResult result = OC_STACK_ERROR;
    THINGS_LOG_D(THINGS_INFO, TAG, "Resource Publish Start.");

    OCDoHandle gReqHandle = NULL;
    OCCallbackData cbData;
    cbData.cb = response;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    OCResourceHandle    resourceHandles[length+2];

    THINGS_LOG_V(THINGS_INFO, TAG, "Device ID ==> %s ", id);
    THINGS_LOG_V(THINGS_INFO, TAG, "Publishing [%d] number of resources ", length);

    int iter = 0;
    for( iter = 0 ; iter < length ; iter++)
    {
        resourceHandles[iter] = list[iter]->resourceHandle;
        THINGS_LOG_D(THINGS_DEBUG, TAG, "URI : %s ", list[iter]->uri);
    }

    if( iter > 0 )
    {
        result = OCRDPublishWithDeviceId(&gReqHandle, host,
                                        id,
                                        CT_ADAPTER_TCP,
                                        resourceHandles,
                                        iter,
                                        &cbData,
                                        OC_LOW_QOS);
    }

//    if(result == OC_STACK_OK && timeoutHandler != NULL )
//    {
//        if( AddRequestHandle(gReqHandle) == NULL )
//        {
//            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] ReqHandles array space is small.");
//            return OC_STACK_ERROR;
//        }
//
//        CreateTimeOutProcess(gReqHandle, timeoutHandler, timeout);
//    }

    return result;
}

/**
 * @brief                  Send Dev-Profile Publish request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*)\n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudDevProfilePublish(char* host,
                                OCClientResponseHandler response,
                                CheckTimeOutCallFunc timeoutHandler,
                                TimeOut_s* timeout)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Device-profile Publish Start.");

    long devCnt = 0;
    STDevice* devInfo = NULL;
    int             cntArrayPayload = 0;
    OCRepPayload ** arrayPayload = NULL;
    OCRepPayload *  payload = NULL;
    OCCallbackData cbData;
    OCDoHandle gReqHandle = NULL;
    OCStackResult result = OC_STACK_ERROR;
    char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
    char * coreVer = NULL;
    char * IoTivityVer = NULL;

    if( (devCnt = DM_GetNumOfDevCnt()) < 1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist device to publish profile-Info of Device.");
        return OC_STACK_ERROR;
    }

    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_DEVPROFILE_URI);

    THINGS_LOG_D(THINGS_INFO, TAG, "Initiate DevProfile with : %s", targetUri);


    cbData.cb = response;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    payload = OCRepPayloadCreate();
    if (!payload)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] memory allocation is failed.(payload)");
        goto no_memory;
    }

    if( (arrayPayload = (OCRepPayload**)DAMalloc(sizeof(OCRepPayload*) * devCnt)) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] memory allocation is failed.(arrayPayload)");
        goto no_memory;
    }

    // Add All-device-info
    for( unsigned long i = 0; i < devCnt; i++ )
    {
        if( (devInfo = DM_GetInfoOfDev( i )) == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] device info structure is NULL.(seq=%d)", i);
            goto no_memory;
        }

        if( devInfo->isPhysical == 1 )
        {
            if( (arrayPayload[cntArrayPayload] = makeDevProfilePayload(devInfo)) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "It's failed making payload of Device-Info.(seq=%d)", cntArrayPayload);
                goto no_memory;
            }
            cntArrayPayload++;
        }
    }

    if( cntArrayPayload < 1 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] pysical device is not exist. So, Can not Sending request for Device-Profile.");
        goto no_memory;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {cntArrayPayload, 0, 0};
    OCRepPayloadSetPropObjectArray(payload, "devices", arrayPayload, dimensions);


    if( isEmptyRequestHandle() == true )
    {
        result = OCDoResource(&gReqHandle, OC_REST_POST, targetUri, NULL, (OCPayload *)payload,
                CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

        if(result == OC_STACK_OK && timeoutHandler != NULL )
        {
            if( AddRequestHandle((OCDoHandle)1) == NULL )
            {
                THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] ReqHandles array space is small.");
                /*for SVACE Warning*/
                if( arrayPayload )
				{
					for(int i=0; i < cntArrayPayload; i++)
						OCPayloadDestroy((OCPayload*)arrayPayload[i]);
					DAFree(arrayPayload);
					arrayPayload = NULL;
				}
                return OC_STACK_ERROR;
            }

            CreateTimeOutProcess((OCDoHandle)1, timeoutHandler, timeout);
        }
    }

    if( arrayPayload )
    {
        for(int i=0; i < cntArrayPayload; i++)
            OCPayloadDestroy((OCPayload*)arrayPayload[i]);
        DAFree(arrayPayload);
        arrayPayload = NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit. -Success-");
    return result;

no_memory:

    if( arrayPayload )
    {
        for(int i=0; i < cntArrayPayload; i++)
            OCPayloadDestroy((OCPayload*)arrayPayload[i]);
        DAFree(arrayPayload);
        arrayPayload = NULL;
    }
    OCRepPayloadDestroy(payload);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit. -Fail-");
    return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Refresh access-token request to Cloud.\n
 *                         Set should call refresh before expiresin of refresh-token or when receive 4.01 during sign-in
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User.
 * @param deviceId         Identifier of Device.
 * @param refreshtoken     Token for refreshing access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudRefresh(const char *host, const char *uId,
                                 const char *deviceId, const char *refreshtoken, OCClientResponseHandler response)
{
    char    targetUri[MAX_URI_LENGTH * 2] = { 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_TOKEN_REFRESH_URI);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

    OCRepPayload *refreshPayload = OCRepPayloadCreate();
    if (!refreshPayload)
    {
        goto no_memory;
    }

    if( uId != NULL )
    {
        OCRepPayloadSetPropString(refreshPayload, KEY_ID_USER, uId);
    }
    OCRepPayloadSetPropString(refreshPayload, KEY_ID_DEVICE, deviceId);
    OCRepPayloadSetPropString(refreshPayload, KEY_TYPE_GRANT, VALUE_TYPE_GRANT_TOKEN);
    OCRepPayloadSetPropString(refreshPayload, KEY_TOKEN_ACCESS_REFRESH, refreshtoken);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "targetURI    =%s", targetUri);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "uid          =%s", uId);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "di           =%s", deviceId);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "granttype    =%s", VALUE_TYPE_GRANT_TOKEN);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "refreshtoken =%s", refreshtoken);

#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    //CASelectCipherSuite(0x35, (1 << 4));
#endif

    return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)refreshPayload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
    OCRepPayloadDestroy(refreshPayload);
    return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudTopicPublishTopic(const char *host,
                                        const char *topic,
                                        OCRepPayload *message,
                                        OCClientResponseHandler response)
{
    char targetUri[MAX_URI_LENGTH * 2] = { 0, };

    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, topic);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

    if (!message)
    {
        goto no_memory;
    }

#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
    //CASelectCipherSuite(0x35, (1 << 4));
#endif
    return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *)message,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
    OCRepPayloadDestroy(message);
    return OC_STACK_NO_MEMORY;
}


static OCRepPayload* makeDevProfilePayload(const STDevice* devInfo)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, "Enter.");

    bool res = false;
    char* manuFactory = NULL;
    char* coreVer = NULL;
    char* IoTivityVer = NULL;
    OCRepPayload* payload = NULL;

    if( devInfo == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "input argument is invalid.(devInfo is null)");
        return NULL;
    }

    if( (payload = OCRepPayloadCreate()) == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.(payload)");
        return NULL;
    }

    if( OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, (void**)&coreVer) != OC_STACK_OK )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Getting CoreVersion of IoTivity is failed.");
        goto GOTO_OUT;
    }

    IoTivityVer = strdup(IOTIVITY_VERSION);

    if( OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, (void**)&manuFactory) != OC_STACK_OK )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Getting manufacturer-name is failed.");
        goto GOTO_OUT;
    }

    OCRepPayloadSetPropString(payload, "di", devInfo->device_id);
    OCRepPayloadSetPropString(payload, "n", devInfo->name);
    OCRepPayloadSetPropString(payload, "icv", coreVer);
    OCRepPayloadSetPropString(payload, "dmv", IoTivityVer);
//    OCRepPayloadSetPropString(payload, "vrv", );
    OCRepPayloadSetPropString(payload, "rt", devInfo->type);
    OCRepPayloadSetPropString(payload, "mnmn", manuFactory);
    OCRepPayloadSetPropString(payload, "mnmo", devInfo->modelNum);
    OCRepPayloadSetPropString(payload, "mnpv", devInfo->ver_p);
    OCRepPayloadSetPropString(payload, "mnos", devInfo->ver_os);
    OCRepPayloadSetPropString(payload, "mnhw", devInfo->ver_hw);
    OCRepPayloadSetPropString(payload, "mnfv", devInfo->ver_fw);
    OCRepPayloadSetPropString(payload, "vid", devInfo->vender_id);

    THINGS_LOG_D(THINGS_DEBUG, TAG, "di   = %s", devInfo->device_id);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "n    = %s", devInfo->name);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "icv  = %s", coreVer);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "dmv  = %s", IoTivityVer);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "rt   = %s", devInfo->type);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnmn = %s", manuFactory);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnmo = %s", devInfo->modelNum);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnpv = %s", devInfo->ver_p);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnos = %s", devInfo->ver_os);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnhw = %s", devInfo->ver_hw);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "mnfv = %s", devInfo->ver_fw);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "vid  = %s", devInfo->vender_id);

    res = true;

GOTO_OUT:
    if( coreVer )
    {
        DAFree(coreVer);
        coreVer = NULL;
    }

    if( IoTivityVer )
    {
        DAFree(IoTivityVer);
        IoTivityVer = NULL;
    }

    if( manuFactory )
    {
        DAFree(manuFactory);
        manuFactory = NULL;
    }

    if( res == false )
    {
        OCPayloadDestroy((OCPayload*)payload);
        payload = NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit. payload=0x%X", payload);
    return payload;
}


