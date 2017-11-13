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

///
/// This sample provides the way to create cloud sample
///
#include <memory>
#include <iostream>
#include <stdexcept>
#include <condition_variable>
#include <map>
#include <vector>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "ocstack.h"
#include "ocpayload.h"
#include "rd_client.h"
#include "OCPlatform.h"

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "ocprovisioningmanager.h"
#include "mbedtls/ssl_ciphersuites.h"
#include <ca_adapter_net_ssl.h>
#endif // WITH_DTLS__ or __WITH_TLS__

using namespace std;

#define VERIFY_SUCCESS(op)                          \
{                                                   \
    if (op != OC_STACK_OK)                          \
    {                                               \
        cout << #op << " failed!!" << endl;         \
        goto exit;                                  \
    }                                               \
}

#define DEFAULT_CONTEXT_VALUE 0x99
#define DEFAULT_AUTH_SIGNUP "/oic/account"
#define DEFAULT_AUTH_SESSION "/oic/account/session"
#define DEFAULT_AUTH_REFRESH "/oic/account/tokenrefresh"

OCStackResult OCCloudSignup(const char *host, const char *deviceId, const char *authprovider,
                            const char *authcode, OCClientResponseHandler response)
{
    char targetUri[MAX_URI_LENGTH * 2] =
    { 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, DEFAULT_AUTH_SIGNUP);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *) DEFAULT_CONTEXT_VALUE;

    OCRepPayload *registerPayload = OCRepPayloadCreate();
    if (!registerPayload)
    {
        goto no_memory;
    }

    OCRepPayloadSetPropString(registerPayload, "di", deviceId);
    OCRepPayloadSetPropString(registerPayload, "authprovider", authprovider);
    OCRepPayloadSetPropString(registerPayload, "authcode", authcode);

    return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) registerPayload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
    OCRepPayloadDestroy(registerPayload);
    return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudSession(const char *host, const char *query, const char *uId,
                             const char *deviceId, const char *accesstoken, bool isLogin,
                             OCClientResponseHandler response)
{
    char targetUri[MAX_URI_LENGTH * 2] =
    { 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *) DEFAULT_CONTEXT_VALUE;

    OCRepPayload *loginoutPayload = OCRepPayloadCreate();
    if (!loginoutPayload)
    {
        goto no_memory;
    }

    if (uId != NULL)
    {
        OCRepPayloadSetPropString(loginoutPayload, "uid", uId);
    }

    if (deviceId != NULL)
    {
        OCRepPayloadSetPropString(loginoutPayload, "di", deviceId);
    }

    if (accesstoken != NULL)
    {
        OCRepPayloadSetPropString(loginoutPayload, "accesstoken", accesstoken);
    }
    OCRepPayloadSetPropBool(loginoutPayload, "login", isLogin);

    return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) loginoutPayload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
    OCRepPayloadDestroy(loginoutPayload);
    return OC_STACK_NO_MEMORY;
}

//Client should call refresh before expiresin or when receive 4.01 during sign-in
OCStackResult OCCloudRefresh(const char *host, const char *query, const char *uId,
                             const char *deviceId, const char *refreshtoken, OCClientResponseHandler response)
{
    char targetUri[MAX_URI_LENGTH * 2] =
    { 0, };
    snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = (void *) DEFAULT_CONTEXT_VALUE;

    OCRepPayload *refreshPayload = OCRepPayloadCreate();
    if (!refreshPayload)
    {
        goto no_memory;
    }

    OCRepPayloadSetPropString(refreshPayload, "uid", uId);
    OCRepPayloadSetPropString(refreshPayload, "di", deviceId);
    OCRepPayloadSetPropString(refreshPayload, "granttype", "refresh_token");
    OCRepPayloadSetPropString(refreshPayload, "refreshtoken", refreshtoken);

    return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) refreshPayload,
                        CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
    OCRepPayloadDestroy(refreshPayload);
    return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudLogin(const char *host, const char *uId, const char *deviceId,
                           const char *accesstoken, OCClientResponseHandler response)
{
    return OCCloudSession(host, DEFAULT_AUTH_SESSION, uId, deviceId, accesstoken, true, response);
}

OCStackResult OCCloudLogout(const char *host, OCClientResponseHandler response)
{
    return OCCloudSession(host, DEFAULT_AUTH_SESSION, NULL, NULL, NULL, false, response);
}

////////////////////////////////////////Device Sample

#define SAMPLE_MAX_NUM_POST_INSTANCE  1
typedef struct LIGHTRESOURCE
{
    OCResourceHandle handle;
    bool state;
    int power;
} LightResource;
static LightResource gLightInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

OCRepPayload *responsePayload(int64_t power, bool state)
{
    OCRepPayload *payload = OCRepPayloadCreate();
    if (!payload)
    {
        cout << "Failed to allocate Payload" << endl;
        return nullptr;
    }

    OCRepPayloadSetPropBool(payload, "state", state);
    OCRepPayloadSetPropInt(payload, "power", power);

    return payload;
}

OCRepPayload *constructResponse(OCEntityHandlerRequest *ehRequest)
{
    if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        cout << "Incoming payload not a representation" << endl;
        return nullptr;
    }

    LightResource *currLightResource = NULL;

    if (ehRequest->resource == gLightInstance[0].handle)
    {
        currLightResource = &gLightInstance[0];
    }

    if (OC_REST_PUT == ehRequest->method)
    {
        // Get pointer to query
        int64_t pow;
        OCRepPayload *input = reinterpret_cast< OCRepPayload * >(ehRequest->payload);

        if (OCRepPayloadGetPropInt(input, "power", &pow))
        {
            currLightResource->power = pow;
        }

        bool state;
        if (OCRepPayloadGetPropBool(input, "state", &state))
        {
            currLightResource->state = state;
        }
    }

    return responsePayload(currLightResource->power, currLightResource->state);
}

OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
    OCRepPayload *getResp = constructResponse(ehRequest);
    if (!getResp)
    {
        cout << "constructResponse failed" << endl;
        return OC_EH_ERROR;
    }

    *payload = getResp;

    return OC_EH_OK;
}

OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult;
    OCRepPayload *putResp = constructResponse(ehRequest);

    if (!putResp)
    {
        cout << "Failed to construct Json response" << endl;
        return OC_EH_ERROR;
    }

    *payload = putResp;
    ehResult = OC_EH_OK;

    return ehResult;
}

#define SAMPLE_MAX_NUM_OBSERVATIONS  2
static bool observeThreadStarted = false;
int gLightUnderObservation = 0;
pthread_t threadId_observe;
typedef struct
{
    OCObservationId observationId;
    bool valid;
    OCResourceHandle resourceHandle;
} Observers;
Observers interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

void *ChangeLightRepresentation(void *param)
{
    (void) param;
    OCStackResult result = OC_STACK_ERROR;

    while (true)
    {
        sleep(3);
        gLightInstance[0].power += 1;

        if (gLightUnderObservation)
        {
            cout << " =====> Notifying stack of new power level " << gLightInstance[0].power
                 << endl;
            // Notifying all observers
            result = OCNotifyAllObservers(gLightInstance[0].handle, OC_NA_QOS);

            cout << " =====> Notifying result " << result << endl;
        }
    }
    return NULL;
}

void ProcessObserveRegister(OCEntityHandlerRequest *ehRequest)
{
    cout << "Received observation registration request with observation Id "
         << ehRequest->obsInfo.obsId << endl;

    if (!observeThreadStarted)
    {
        pthread_create(&threadId_observe, NULL, ChangeLightRepresentation, (void *) NULL);
        observeThreadStarted = 1;
    }
    for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++)
    {
        if (interestedObservers[i].valid == false)
        {
            interestedObservers[i].observationId = ehRequest->obsInfo.obsId;
            interestedObservers[i].valid = true;
            gLightUnderObservation = 1;
            break;
        }
    }
}

void ProcessObserveDeregister(OCEntityHandlerRequest *ehRequest)
{
    bool clientStillObserving = false;

    cout << "Received observation deregistration request for observation Id "
         << ehRequest->obsInfo.obsId << endl;
    for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++)
    {
        if (interestedObservers[i].observationId == ehRequest->obsInfo.obsId)
        {
            interestedObservers[i].valid = false;
        }
        if (interestedObservers[i].valid == true)
        {
            // Even if there is one single client observing we continue notifying entity handler
            clientStillObserving = true;
        }
    }
    if (clientStillObserving == false)
        gLightUnderObservation = 0;
}

OCEntityHandlerResult OCEntityHandlerCb(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest *entityHandlerRequest, void * /*callback*/)
{
    OCEntityHandlerResult ehResult = OC_EH_OK;
    OCEntityHandlerResponse response =
    {
        0, 0, OC_EH_ERROR, 0, 0,
        { },
        { 0 }, false
    };

    // Validate pointer
    if (!entityHandlerRequest)
    {
        cout << "Invalid request pointer" << endl;
        return OC_EH_ERROR;
    }

    // Initialize certain response fields
    response.numSendVendorSpecificHeaderOptions = 0;
    memset(response.sendVendorSpecificHeaderOptions, 0,
           sizeof response.sendVendorSpecificHeaderOptions);
    memset(response.resourceUri, 0, sizeof response.resourceUri);
    OCRepPayload *payload = nullptr;

    if (flag & OC_REQUEST_FLAG)
    {
        cout << "Flag includes OC_REQUEST_FLAG" << endl;

        if (OC_REST_GET == entityHandlerRequest->method)
        {
            cout << "Received OC_REST_GET from client" << endl;
            ehResult = ProcessGetRequest(entityHandlerRequest, &payload);
        }
        else if (OC_REST_PUT == entityHandlerRequest->method)
        {
            cout << "Received OC_REST_PUT from client" << endl;
            ehResult = ProcessPutRequest(entityHandlerRequest, &payload);
        }
        else
        {
            cout << "Received unsupported method %d from client " << entityHandlerRequest->method
                 << endl;
            ehResult = OC_EH_ERROR;
        }
        // If the result isn't an error or forbidden, send response
        if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
        {
            // Format the response.  Note this requires some info about the request
            response.requestHandle = entityHandlerRequest->requestHandle;
            response.resourceHandle = entityHandlerRequest->resource;
            response.ehResult = ehResult;
            response.payload = reinterpret_cast< OCPayload * >(payload);
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                cout << "Error sending response" << endl;
                ehResult = OC_EH_ERROR;
            }
        }
    }

    if (flag & OC_OBSERVE_FLAG)
    {
        cout << "Flag includes OC_OBSERVE_FLAG" << endl;
        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
        {
            cout << "Received OC_OBSERVE_REGISTER from client" << endl;
            ProcessObserveRegister(entityHandlerRequest);
        }
        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
        {
            cout << "Received OC_OBSERVE_DEREGISTER from client" << endl;
            ProcessObserveDeregister(entityHandlerRequest);
        }
    }

    OCPayloadDestroy(response.payload);
    return ehResult;
}

int createLightResource(char *uri, LightResource *lightResource)
{
    if (!uri)
    {
        cout << "Resource URI cannot be NULL" << endl;
        return -1;
    }

    lightResource->state = false;
    lightResource->power = 0;
    OCStackResult res = OCCreateResource(&(lightResource->handle), "core.light", "oc.mi.def", uri,
                                         OCEntityHandlerCb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    cout << "Created Light resource with result:" << res << endl;

    return res;
}

OCStackApplicationResult handlePublishCB(void *ctx, OCDoHandle /*handle*/,
        OCClientResponse *clientResponse)
{
    if (ctx != (void *) DEFAULT_CONTEXT_VALUE)
    {
        cout << "Invalid Publish callback received" << endl;
    }

    cout << "Publish resource response received, code: " << clientResponse->result << endl;

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackResult SetDeviceInfo()
{
    OCResourceHandle resourceHandle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
    if (resourceHandle == NULL)
    {
        cout << "Device Resource does not exist." << endl;
        goto exit;
    }

    VERIFY_SUCCESS(OCBindResourceTypeToResource(resourceHandle, "oic.d.light"));

    if (OCGetServerInstanceIDString() == NULL)
    {
        cout << "Device ID generation failed"  << endl;
        goto exit;
    }

    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, "Living Room Light"));
    VERIFY_SUCCESS(OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                      "91debd4b-c875-47e1-9e1d-b1cd32baeb01"));

    cout << "Device information published successfully." << endl;
    return OC_STACK_OK;

exit:
    return OC_STACK_ERROR;
}

void PublishResources(string host)
{
    cout << "Publishing resources..." << endl;

    if (createLightResource((char *) "/a/light/0", &gLightInstance[0]) != 0)
    {
        cout << "Unable to create sample resource" << endl;
    }

    OCResourceHandle resourceHandles[1] =
    { gLightInstance[0].handle, };
    OCCallbackData cbData;
    cbData.cb = handlePublishCB;
    cbData.context = (void *) DEFAULT_CONTEXT_VALUE;
    cbData.cd = NULL;

    cout << "Publishing default resources" << endl;

    OCStackResult res = SetDeviceInfo();

    if (res != OC_STACK_OK)
    {
        cout << "Publishing device info failed" << endl;
    }

    res = OCRDPublish(nullptr, host.c_str(), CT_ADAPTER_TCP, NULL, 0, OIC_RD_PUBLISH_TTL, &cbData, OC_LOW_QOS);
    if (res != OC_STACK_OK)
    {
        cout << "Unable to publish default resources to cloud" << endl;
    }

    cout << "Publishing user resources" << endl;

    res = OCRDPublish(nullptr, host.c_str(), CT_ADAPTER_TCP, resourceHandles, 1, OIC_RD_PUBLISH_TTL, &cbData, OC_LOW_QOS);
    if (res != OC_STACK_OK)
    {
        cout << "Unable to publish user resources to cloud" << endl;
    }
}

/////////////////////////////////////////////Common sample
void printRepresentation(OCRepPayloadValue *value)
{
    while (value)
    {
        cout << "Key: " << value->name;
        switch (value->type)
        {
            case OCREP_PROP_NULL:
                cout << " Value: None" << endl;
                break;
            case OCREP_PROP_INT:
                cout << " Value: " << value->i << endl;
                break;
            case OCREP_PROP_DOUBLE:
                cout << " Value: " << value->d << endl;
                break;
            case OCREP_PROP_BOOL:
                cout << " Value: " << value->b << endl;
                break;
            case OCREP_PROP_STRING:
                cout << " Value: " << value->str << endl;
                break;
            case OCREP_PROP_BYTE_STRING:
                cout << " Value: Byte String" << endl;
                break;
            case OCREP_PROP_OBJECT:
                cout << " Value: Object" << endl;
                break;
            case OCREP_PROP_ARRAY:
                cout << " Value: Array" << endl;
                break;
        }
        value = value->next;
    }
}



string g_host;

OCStackApplicationResult handleLoginoutCB(void *ctx, OCDoHandle /*handle*/,
        OCClientResponse *clientResponse)
{
    if (ctx != (void *) DEFAULT_CONTEXT_VALUE)
    {
        cout << "Invalid Login/out callback received" << endl;
    }

    cout << "Login/out response received code: " << clientResponse->result << endl;

    if (clientResponse->payload != NULL
        && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        cout << "PAYLOAD_TYPE_REPRESENTATION received" << endl;

        OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;

        printRepresentation(val);
    }

    if (clientResponse->result < 5)
    {
        PublishResources(g_host);
    }

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult handleRegisterCB(void *ctx, OCDoHandle /*handle*/,
        OCClientResponse *clientResponse)
{
    if (ctx != (void *) DEFAULT_CONTEXT_VALUE)
    {
        cout << "Invalid Register callback received" << endl;
    }

    cout << "Register response received code: " << clientResponse->result << endl;

    if (clientResponse->payload != NULL
        && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        cout << "PAYLOAD_TYPE_REPRESENTATION received" << endl;
        cout << "You can Sign-In using retrieved accesstoken when disconnected or reboot" << endl;

        OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;

        printRepresentation(val);
    }

    return OC_STACK_KEEP_TRANSACTION;
}

void PrintUsage()
{
    cout << endl;
    cout << "Usage : thin_room_light <addr:port> <uid> <accesstoken>\n";
    cout << "<addr:port>: Cloud Address, \"127.0.0.1:5683\"\n";
    cout
            << "<accesstoken>: String value, Provided by response of onboarding scenario\n\tor kind of registration portal\n\n";
    cout << "sample: \"thin_room_light 127.0.0.1:5683\"\n\t-Sign-Up mode\n\n";
    cout
            << "sample: \"thin_room_light 127.0.0.1:5683 abcdefg 1234567890123456\"\n\t-Sign-in and Publish resource to registered account\n\n";

    cout << endl;
    cout << "Usage : thin_room_light <addr:port> <uid> <refreshtoken> 1\n";
    cout
            << "<refreshtoken>: String value, Provided by response of onboarding scenario\n\tor kind of registration portal\n\n";
    cout
            << "sample: \"thin_room_light 127.0.0.1:5683 abcdefg 6543210987654321 1\"\n\t-token refresh and get renewed access token\n\n";

}
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
int saveTrustCert(void)
{
    OCStackResult res = OC_STACK_ERROR;
    uint16_t g_credId = 0;

    cout << "Save Trust Cert. Chain into Cred of SVR" <<endl;

    ByteArray trustCertChainArray = {0, 0};

    FILE *fp = fopen("rootca.crt", "rb+");

    if (fp)
    {
        size_t fsize;
        if (fseeko(fp, 0, SEEK_END) == 0 && (fsize = ftello(fp)) > 0)
        {
            trustCertChainArray.data = (uint8_t *)malloc(fsize);
            trustCertChainArray.len = fsize;
            if (NULL == trustCertChainArray.data)
            {
                cout << "Failed to allocate memory" << endl;
                fclose(fp);
                return res;
            }
            rewind(fp);
            if (fsize != fread(trustCertChainArray.data, 1, fsize, fp))
            {
                 cout << "Certiface not read completely" << endl;
            }
            fclose(fp);
        }
    }

    res = OCSaveTrustCertChain(trustCertChainArray.data, trustCertChainArray.len, OIC_ENCODING_PEM,&g_credId);

    if(OC_STACK_OK != res)
    {
        cout << "OCSaveTrustCertChainBin API error" << endl;
        return res;
    }
    cout << "CredId of Saved Trust Cert. Chain into Cred of SVR : " << g_credId << endl;

    return res;
}
#endif

static FILE *client_open(const char *path, const char *mode)
{
	if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
	{
		return fopen("./thin_room_light.dat", mode);
	}
	else
	{
		return fopen(path, mode);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Put \"[host-ipaddress:port] \" for sign-up"
			<< endl;
		cout << "Put \"[host-ipaddress:port] [uid] [accessToken] \" for sign-in and publish resources" <<
			endl;
		cout << "Put \"[host-ipaddress:port] [uid] [refreshToken] refresh \" for accessToken to refresh" <<
			endl;
		return 0;
	}

    string uId;
    string accessToken;
    string refreshToken;
    string authProvider;
    string authCode;
    
	OCMode stackMode = OC_CLIENT_SERVER;
    switch (argc)
    {
        case 2:
            cout << "Put auth provider name(ex: github)" << endl;
            cin >> authProvider;
            cout << "Put auth code(provided by auth provider)" << endl;
            cin >> authCode;
            break;

        case 4:
            uId = argv[2];
            accessToken = argv[3];
            break;

        case 5:
            uId = argv[2];
            refreshToken = argv[3];
            break;

        default:
            PrintUsage();
            return 0;
    }

	g_host = "coap+tcp://";

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	g_host = "coaps+tcp://";
#endif

	g_host += argv[1];

    cout << "Host " << g_host.c_str() << endl;

    OCPersistentStorage ps
    { client_open, fread, fwrite, fclose, unlink };
    if (OCRegisterPersistentStorageHandler(&ps) != OC_STACK_OK)
    {
        cout << "OCStack init persistent storage error" << endl;
        return 0;
    }

    if (OCInit(NULL, 0, stackMode) != OC_STACK_OK)
    {
        cout << "OCStack init error" << endl;
        return 0;
    }

    OCStackResult res = OC_STACK_ERROR;


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	cout << "Security Mode" << endl;
	if (CA_STATUS_OK != saveTrustCert())
	{
		cout << "saveTrustCert returned an error" << endl;
	}

	uint16_t cipher = MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256;
	if (CA_STATUS_OK != CASelectCipherSuite(cipher, CA_ADAPTER_TCP))
	{
		cout << "CASelectCipherSuite returned an error" << endl;
	}
#endif


    switch (argc)
    {
        case 2:
            cout << "Sign-Up to cloud using " << authProvider << " " << authCode << endl;
            res = OCCloudSignup(g_host.c_str(), OCGetServerInstanceIDString(), authProvider.c_str(),
                                authCode.c_str(), handleRegisterCB);
            cout << "OCCloudSignup return " << res << endl;
            break;

        case 4:
            cout << "Sign-In to cloud using " << accessToken << endl;
            res = OCCloudLogin(g_host.c_str(), uId.c_str(), OCGetServerInstanceIDString(),
                               accessToken.c_str(), handleLoginoutCB);
            cout << "OCCloudLogin return " << res << endl;
            break;

        case 5:
            cout << "Token refresh to cloud using the refresh token " << refreshToken << endl;
            res = OCCloudRefresh(g_host.c_str(), DEFAULT_AUTH_REFRESH, uId.c_str(),
                                 OCGetServerInstanceIDString(), refreshToken.c_str(), handleRegisterCB);
            cout << "OCCloudRefresh return " << res << endl;
            break;

        default:
            PrintUsage();
            return 0;
    }

    cout << "Waiting response.." << endl;

    while (true)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            cout << "OCProcess process error" << endl;
        }

        sleep(1);
    }

    if (OCStop() != OC_STACK_OK)
    {
        cout << "OCStop process error" << endl;
    }

    return 0;
}
