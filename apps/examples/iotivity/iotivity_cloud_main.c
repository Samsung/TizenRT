/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

///
/// This sample provides the way to create cloud sample
#include<stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ocstack.h"
#include "ocpayload.h"

#include "cloud_connector.h"

#define DEFAULT_CONTEXT_VALUE 0x99
#define DEFAULT_PUBLISH_QUERY "/oic/rd?rt=oic.wk.rdpub"
#define DEFAULT_DISCOVER_QUERY "/oic/res?rt=core.light"

#define nullptr  NULL

/*
 *Global Variables
 */
#define SAMPLE_MAX_NUM_POST_INSTANCE  2
typedef struct LIGHTRESOURCE
{
  OCResourceHandle handle;
  bool state;
  int power;
} LightResource;
static LightResource gLightInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

#define SAMPLE_MAX_NUM_OBSERVATIONS  2
static bool observeThreadStarted = false;
int gCloudLightUnderObservation = 0;
pthread_t threadId_observe;
typedef struct
{
  OCObservationId observationId;
  bool valid;
  OCResourceHandle resourceHandle;
} Observers;
Observers interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

const char *coap_tcp_header = "coap+tcp://";
char *g_host    = nullptr;
char *g_session = nullptr;
char *g_authProviderName = nullptr;
char *g_authProviderCode = nullptr;

int gNumObserveNotifies = 0;
int g_runningMode = 0;

#ifdef CONFIG_NET_LWIP
#define IOTIVITY_TEST_STACKSIZE   16384
#define IOTIVITY_TEST_PRI   100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR

// String Utility Functions Declaration
void  tinyara_free(char **str);
void  tinyara_strcpy(char **dst, const char *src, int *size);
void tinyara_strcat(char **dst, const char *src, int *size);

/*
 *Utility Functions to strings manipulation
 */
void  tinyara_free(char **str)
{
  if (nullptr == str || nullptr == *str) return;
  free(*str);
  *str = nullptr;
}

void  tinyara_strcpy(char **dst, const char *src, int *size)
{
  if (nullptr == dst) return;
  if (nullptr == src) return;
  tinyara_free(dst);

  int src_size = strlen(src);
  int dst_size = src_size + 1;
  *dst = malloc(dst_size);
  strncpy(*dst, src, dst_size);
  if (size)
  {
    *size = dst_size;
  }
}

void tinyara_strcat(char **dst, const char *src, int *size)
{
  if (nullptr == dst) return;
  if (nullptr == src) return;
  if (nullptr == *dst)
  {
    tinyara_strcpy(dst, src, size);
    return;
  }

  int src_size = strlen(src);
  int dst_size = strlen(*dst) + src_size + 1;
  *dst = realloc(*dst, dst_size);
  strncat(*dst, src, dst_size);
  if (size)
  {
    *size = dst_size;
  }
}

OCRepPayload *responsePayload(int64_t power, bool state)
{
  OCRepPayload *payload = OCRepPayloadCreate();
  if (!payload)
  {
    printf("Failed to allocate Payload\n");
    return nullptr;
  }

  OCRepPayloadSetPropBool(payload, "state", state);
  OCRepPayloadSetPropInt(payload, "power", power);

  return payload;
}

OCRepPayload *cloudConstructResponse(OCEntityHandlerRequest *ehRequest)
{
  if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
  {
    printf("Incoming payload not a representation\n");
    return nullptr;
  }

  LightResource *currLightResource = nullptr;

  if (ehRequest->resource == gLightInstance[0].handle)
  {
    currLightResource = &gLightInstance[0];
  }
  else if (ehRequest->resource == gLightInstance[1].handle)
  {
    currLightResource = &gLightInstance[1];
  }

  if (OC_REST_PUT == ehRequest->method)
  {
    // Get pointer to query
    int64_t pow;
    OCRepPayload *input = (OCRepPayload *)(ehRequest->payload);

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

OCEntityHandlerResult cloudProcessGetRequest(OCEntityHandlerRequest *ehRequest,
                                             OCRepPayload **payload)
{
  OCRepPayload *getResp = cloudConstructResponse(ehRequest);
  if (!getResp)
  {
    printf("constructResponse failed\n");
    return OC_EH_ERROR;
  }

  *payload = getResp;

  return OC_EH_OK;
}

OCEntityHandlerResult cloudProcessPutRequest(OCEntityHandlerRequest *ehRequest,
                                             OCRepPayload **payload)
{
  OCEntityHandlerResult ehResult;
  OCRepPayload *putResp = cloudConstructResponse(ehRequest);

  if (!putResp)
  {
    printf("Failed to construct Json response\n");
    return OC_EH_ERROR;
  }

  *payload = putResp;
  ehResult = OC_EH_OK;

  return ehResult;
}

void *cloudChangeLightRepresentation(void *param)
{
  (void)param;
  OCStackResult result = OC_STACK_ERROR;

  while (true)
  {
    sleep(3);
    gLightInstance[0].power += 1;
    gLightInstance[1].power += 3;

    if (gCloudLightUnderObservation)
    {
      printf(" =====> Notifying stack of new power level %d \n ", gLightInstance[0].power);
      printf(" =====> Notifying stack of new power level %d \n ", gLightInstance[1].power);
      // Notifying all observers
      result = OCNotifyAllObservers(gLightInstance[0].handle, OC_NA_QOS);
      result = OCNotifyAllObservers(gLightInstance[1].handle, OC_NA_QOS);
      printf(" =====> Notifying result %d \n", (int)result);
    }
  }
  return nullptr;
}

void cloudProcessObserveRegister(OCEntityHandlerRequest *ehRequest)
{
  printf("Received observation registration request with observation Id %u \n", ehRequest->obsInfo.obsId);

  if (!observeThreadStarted)
  {
    pthread_create(&threadId_observe, nullptr, cloudChangeLightRepresentation, (void *)nullptr);
    observeThreadStarted = 1;
  }
  for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++)
  {
    if (interestedObservers[i].valid == false)
    {
      interestedObservers[i].observationId = ehRequest->obsInfo.obsId;
      interestedObservers[i].valid = true;
      gCloudLightUnderObservation = 1;
      break;
    }
  }
}

void cloudProcessObserveDeregister(OCEntityHandlerRequest *ehRequest)
{
  bool clientStillObserving = false;
  printf("Received observation deregistration request for observation Id  %u \n", ehRequest->obsInfo.obsId);
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
    gCloudLightUnderObservation = 0;
}

OCEntityHandlerResult cloudOCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callback)
{
  OCEntityHandlerResult ehResult = OC_EH_OK;
  OCEntityHandlerResponse response = { 0, 0, OC_EH_ERROR, 0, 0, {}, { 0 }, false };

  // Validate pointer
  if (!entityHandlerRequest)
  {
    printf("Invalid request pointer \n");
    return OC_EH_ERROR;
  }

  // Initialize certain response fields
  response.numSendVendorSpecificHeaderOptions = 0;
  memset(response.sendVendorSpecificHeaderOptions,
         0, sizeof response.sendVendorSpecificHeaderOptions);
  memset(response.resourceUri, 0, sizeof response.resourceUri);
  OCRepPayload *payload = nullptr;

  if (flag & OC_REQUEST_FLAG)
  {
    printf("Flag includes OC_REQUEST_FLAG\n");

    if (OC_REST_GET == entityHandlerRequest->method)
    {
      printf("Received OC_REST_GET from client\n");
      ehResult = cloudProcessGetRequest(entityHandlerRequest, &payload);
    }
    else if (OC_REST_PUT == entityHandlerRequest->method)
    {
      printf("Received OC_REST_PUT from client\n");
      ehResult = cloudProcessPutRequest(entityHandlerRequest, &payload);
    }
    else
    {
      printf("Received unsupported method %d from client \n", (int)entityHandlerRequest->method);
      ehResult = OC_EH_ERROR;
    }
    // If the result isn't an error or forbidden, send response
    if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
    {
      // Format the response.  Note this requires some info about the request
      response.requestHandle = entityHandlerRequest->requestHandle;
      response.resourceHandle = entityHandlerRequest->resource;
      response.ehResult = ehResult;
      response.payload = (OCPayload *)(payload);
      response.persistentBufferFlag = 0;

      // Send the response
      if (OCDoResponse(&response) != OC_STACK_OK)
      {
        printf("Error sending response\n");
        ehResult = OC_EH_ERROR;
      }
    }
  }

  if (flag & OC_OBSERVE_FLAG)
  {
    printf("Flag includes OC_OBSERVE_FLAG\n");
    if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
    {
      printf("Received OC_OBSERVE_REGISTER from client\n");
      cloudProcessObserveRegister(entityHandlerRequest);
    }
    else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
    {
      printf("Received OC_OBSERVE_DEREGISTER from client\n");
      cloudProcessObserveDeregister(entityHandlerRequest);
    }
  }

  OCPayloadDestroy(response.payload);
  return ehResult;
}

int cloudcreateLightResource(char *uri, LightResource *lightResource)
{
  if (!uri)
  {
    printf("Resource URI cannot be nullptr\n");
    return -1;
  }

  lightResource->state = false;
  lightResource->power = 0;
  OCStackResult res = OCCreateResource(&(lightResource->handle),
                                       "core.light",
                                       "oc.mi.def",
                                       uri,
                                       cloudOCEntityHandlerCb,
                                       nullptr,
                                       OC_DISCOVERABLE | OC_OBSERVABLE);
  printf("Created Light resource with result: %d \n", (int)res);


  return res;
}

OCStackApplicationResult handlePublishCB(void *ctx,
                                         OCDoHandle handle,
                                         OCClientResponse *clientResponse)
{
  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Publish callback received\n");
  }
  printf("Publish resource response received, code:%d \n", (int)clientResponse->result);

  return OC_STACK_KEEP_TRANSACTION;
}

void PublishResources(char *host, char *additionalQuery)
{
  printf("Running as Server mode\n");

  char *requestQuery = nullptr; int requestQuerySize;
  tinyara_strcpy(&requestQuery, DEFAULT_PUBLISH_QUERY, &requestQuerySize);
  tinyara_strcat(&requestQuery, additionalQuery, &requestQuerySize);
  printf("Publishing resources...\n");
  printf("%s,%s", host, requestQuery);

  if (cloudcreateLightResource((char *)"/a/light/0", &gLightInstance[0]) != 0)
  {
    printf("Unable to create sample resource\n");
  }

  if (cloudcreateLightResource((char *)"/a/light/1", &gLightInstance[1]) != 0)
  {
    printf("Unable to create sample resource\n");
  }

  if (OCCloudPublish(host, requestQuery, &handlePublishCB, 2,
                     gLightInstance[0].handle, gLightInstance[1].handle) != OC_STACK_OK)
  {
    printf("Unable to publish resources to cloud\n");
  }
  tinyara_free(&requestQuery);
}

/*
 *Client Sample
 */
void PrintRepresentation(OCRepPayloadValue *val)
{
  while (val)
  {
    printf("Key: %s Value:", val->name);
    switch (val->type)
    {
      case OCREP_PROP_NULL:
        printf("NULL\n");
        break;

      case OCREP_PROP_INT:
        printf("%d\n", val->i);
        break;

      case OCREP_PROP_DOUBLE:
        printf("%f\n", val->d);
        break;

      case OCREP_PROP_BOOL:
        printf("%u\n", val->b);
        break;

      case OCREP_PROP_STRING:
        printf("%s\n", val->str);
        break;

      case OCREP_PROP_BYTE_STRING:
        printf("%s\n", "[ByteString]");
        break;

      case OCREP_PROP_OBJECT:
        printf("%s\n", "[Object]");
        break;

      case OCREP_PROP_ARRAY:
        printf("%s\n", "[Array]");
        break;
    }

    val = val->next;
  }
}


OCStackApplicationResult obsReqCB(void *ctx, OCDoHandle handle,
                                  OCClientResponse *clientResponse)
{
  printf("Observe response received from %s \n", clientResponse->resourceUri);

  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Put callback received\n");
  }

  if (clientResponse)
  {
    if (clientResponse->payload == nullptr)
    {
      printf("No payload received\n");
    }

    OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

    PrintRepresentation(val);

    gNumObserveNotifies++;
    if (gNumObserveNotifies > 5) //large number to test observing in DELETE case.
    {
      printf("Cancelling with OC_HIGH_QOS\n");
      if (OCCancel(handle, OC_HIGH_QOS, nullptr, 0) != OC_STACK_OK)
      {
        printf("Observe cancel error\n");
      }
    }
    if (clientResponse->sequenceNumber == OC_OBSERVE_REGISTER)
    {
      printf("This also serves as a registration confirmation\n");
    }
    else if (clientResponse->sequenceNumber == OC_OBSERVE_DEREGISTER)
    {
      printf("This also serves as a deregistration confirmation\n");
      return OC_STACK_DELETE_TRANSACTION;
    }
    else if (clientResponse->sequenceNumber == OC_OBSERVE_NO_OPTION)
    {
      printf("This also tells you that registration/deregistration failed\n");
      return OC_STACK_DELETE_TRANSACTION;
    }
  }
  else
  {
    printf("obsReqCB received Null clientResponse\n");
  }

  return OC_STACK_KEEP_TRANSACTION;
}

void ObserveResource(char **uri, char *additionalQuery)
{
  OCCallbackData cbData;
  cbData.cb = obsReqCB;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
  cbData.cd = nullptr;

  tinyara_strcat(uri, additionalQuery, nullptr);
  printf("Request OBSERVE to resource %s\n", *uri);

  OCStackResult res = OCDoResource(nullptr, OC_REST_OBSERVE, *uri, nullptr, nullptr,
                                   CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, nullptr, 0);
  printf("Requesting OBSERVE res=%d\n", (int)res);
}

OCStackApplicationResult putReqCB(void *ctx, OCDoHandle handle,
                                  OCClientResponse *clientResponse)
{
  printf("Put response received from %s\n ", clientResponse->resourceUri);

  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Put callback received\n");
  }

  if (clientResponse->payload == nullptr)
  {
    printf("No payload received\n");
  }

  OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

  PrintRepresentation(val);

  char *requestUri = nullptr; int requestUriSize;
  tinyara_strcpy(&requestUri, g_host, &requestUriSize);
  tinyara_strcat(&requestUri, clientResponse->resourceUri, &requestUriSize);

  ObserveResource(&requestUri, "");

  tinyara_free(&requestUri);

  return OC_STACK_KEEP_TRANSACTION;
}

OCPayload *putRequestPayload()
{
  OCRepPayload *payload = OCRepPayloadCreate();

  if (!payload)
  {
    printf("Failed to create put payload object\n");
    exit(1);
  }

  OCRepPayloadSetPropInt(payload, "power", 15);
  OCRepPayloadSetPropBool(payload, "state", true);

  return (OCPayload *)payload;
}

void PutResource(char **uri, char *additionalQuery)
{
  OCCallbackData cbData;
  cbData.cb = putReqCB;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
  cbData.cd = nullptr;

  tinyara_strcat(uri, additionalQuery, nullptr);
  printf("Request PUT to resource%s\n", *uri);

  OCStackResult res = OCDoResource(nullptr, OC_REST_PUT, *uri, nullptr, putRequestPayload(),
                                   CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, nullptr, 0);

  printf("Requesting PUT res=%d\n", res);
}

OCStackApplicationResult handleGetCB(void *ctx,
                                     OCDoHandle handle,
                                     OCClientResponse *clientResponse)
{
  printf("Get response received from %s\n", clientResponse->resourceUri);

  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Get callback received\n");
  }

  if (clientResponse->payload == nullptr)
  {
    printf("No payload received\n");
  }

  if (clientResponse->payload != nullptr &&
      clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
  {
    OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

    PrintRepresentation(val);

    char *requestUri = nullptr; int requestUriSize;
    tinyara_strcpy(&requestUri, g_host, &requestUriSize);
    tinyara_strcat(&requestUri, clientResponse->resourceUri, &requestUriSize);

    PutResource(&requestUri, "");

    tinyara_free(&requestUri);
  }

  return OC_STACK_KEEP_TRANSACTION;
}

void GetResource(char **uri, char *additionalQuery)
{
  OCCallbackData cbData;
  cbData.cb = handleGetCB;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
  cbData.cd = nullptr;

  tinyara_strcat(uri, additionalQuery, nullptr);
  printf("Request GET to resource %s\n", *uri);

  OCStackResult res = OCDoResource(nullptr, OC_REST_GET, *uri, nullptr, nullptr,
                                   CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, nullptr, 0);

  printf("Requesting GET res=%d\n", (int)res);
}

// This is a function called back when a device is discovered
OCStackApplicationResult discoveryReqCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
  if (ctx == (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Callback Context for DISCOVER query recvd successfully\n");
  }

  if (clientResponse)
  {
    printf("StackResult: %d\n", (int)clientResponse->result);

    OCDiscoveryPayload *payload = (OCDiscoveryPayload *)clientResponse->payload;
    if (!payload)
    {
      printf("Empty payload\n");
      return OC_STACK_DELETE_TRANSACTION;
    }

    OCResourcePayload *resource = (OCResourcePayload *)payload->resources;
    if (!resource)
    {
      printf("No resources in payload\n");
      return OC_STACK_DELETE_TRANSACTION;
    }

    while (resource)
    {
      printf("Found Resource %s\n", resource->uri);
      char *requestUri = nullptr; int requestUriSize;
      tinyara_strcpy(&requestUri, g_host, &requestUriSize);
      tinyara_strcat(&requestUri, resource->uri, &requestUriSize);

      GetResource(&requestUri, "");

      tinyara_free(&requestUri);

      resource = resource->next;
    }
  }
  else
  {
    printf("discoveryReqCB received Null clientResponse\n");
  }
  return OC_STACK_KEEP_TRANSACTION;
}

void DiscoverResources(char *host, char *additionalQuery)
{
  printf("Running as Client mode\n");

  char *requestQuery = nullptr; int requestQuerySize;
  tinyara_strcpy(&requestQuery, host, requestQuerySize);
  tinyara_strcat(&requestQuery, DEFAULT_DISCOVER_QUERY, requestQuerySize);
  tinyara_strcat(&requestQuery, additionalQuery, requestQuerySize);

  printf("Finding resources...\n");
  printf("%s\n", requestQuery);

  OCCallbackData cbData;

  cbData.cb = discoveryReqCB;
  cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
  cbData.cd = nullptr;

  if (OCDoResource(nullptr, OC_REST_DISCOVER, requestQuery, nullptr, 0, CT_ADAPTER_TCP,
                   OC_LOW_QOS, &cbData, nullptr, 0) != OC_STACK_OK)
  {
    printf("Unable to find resources from cloud\n");
  }

  tinyara_free(&requestQuery);
}

/////////////////////////////////////////////Common sample

OCStackApplicationResult handleLoginoutCB(void *ctx,
                                          OCDoHandle handle,
                                          OCClientResponse *clientResponse)
{
  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Login/out callback received\n");
  }

  printf("Login/out response received code: %\n", (int)clientResponse->result);

  if (clientResponse->payload != nullptr &&
      clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
  {
    printf("PAYLOAD_TYPE_REPRESENTATION received\n");

    OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

    while (val)
    {
      printf("Key: %s Value: %s\n", val->name , val->str);
      val = val->next;
    }

    if (g_runningMode == 1)
    {
      PublishResources(g_host, "");
    }
    else if (g_runningMode == 2)
    {
      DiscoverResources(g_host, "");
    }
  }

  return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult handleRegisterCB(void *ctx,
                                          OCDoHandle handle,
                                          OCClientResponse *clientResponse)
{
  if (ctx != (void *)DEFAULT_CONTEXT_VALUE)
  {
    printf("Invalid Register callback received\n");
  }

  printf("Register response received code: %d\n", (int)clientResponse->result);

  if (clientResponse->payload != nullptr &&
      clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
  {

    printf("PAYLOAD_TYPE_REPRESENTATION received\n");
    printf("You can login using received session variable after disconnected or reboot\n");

    OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;

    while (val)
    {
      printf("Key: %s Value: %s\n", val->name , val->str);
      if (0 == strncmp(val->name, "session", 7))
      {
        tinyara_strcpy(&g_session, val->str, nullptr);
        printf("Registered Session = '%s'\n", g_session);
      }
      val = val->next;
    }
  }

  return OC_STACK_KEEP_TRANSACTION;
}

void PrintUsage()
{
  printf("\n");
  printf("Usage : \n");
  printf("TASH>> cloud_device <addr:port> <auth provider> <auth code>\n");
  printf("         <addr:port>: Cloud Address, \"172.16.10.60:5683\"\n");
  printf("         <auth provider>: String value\n");
  printf("         <auth provider>: String value\n");
  printf("example: TASH>> iotivity_cloud 172.16.10.60:5683 github e37776fae12f62f0cd06 \n");
  printf("\n");

  printf("TASH>> cloud_device <addr:port> <session identifier>\n");
  printf("         <addr:port>: Cloud Address, \"172.16.10.60:5683\"\n");
  printf("         <session>: Session String value\n");
  printf("example: TASH>> iotivity_cloud 172.16.10.60:5683 g8ENZUi9mqBkywLY \n");
  printf("\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
#ifndef CONFIG_NET_LWIP
int iotivity_cloud_main(int argc, char *argv[])
#else
int iotivity_cloud_main_cb(int argc, char *argv[])
#endif
#endif
{
  if (g_session == nullptr || g_host == nullptr)
  {
    printf("Incorrect Parameters!!!\n");
    return 1;
  }

  printf("IoTivity Cloud Address  = '%s'\n", g_host);
  printf("IoTivityCloud Session Value =  '%s'\n", g_session);

  if (OCInit(nullptr, 0, OC_CLIENT_SERVER) != OC_STACK_OK)
  {
    printf("OCStack init error\n");
    return 2;
  }

  OCStackResult res;

  g_runningMode = 1;

  res = OC_STACK_ERROR;

  res = OCCloudLogin(g_host, g_session, handleLoginoutCB);

  printf("OCCloudLogin return %d\n", (int)res);

  while (true)
  {
    if (OCProcess() != OC_STACK_OK)
    {
      printf("OCProcess process error\n");
    }

    usleep(100000);
  }

  if (OCStop() != OC_STACK_OK)
  {
    printf("OCStop process error\n");
  }

  tinyara_free(&g_host);
  tinyara_free(&g_session);
  return 0;
}

int iotivity_cloud_main_get_session(int argc, char *argv[])
{
  if (g_authProviderName == nullptr || g_authProviderCode == nullptr || g_host == nullptr)
  {
    printf("Incorrect Parameters!!!\n");
    return 1;
  }

  printf("IoTivity Cloud Address  = '%s'\n", g_host);
  printf("OAuth Provider Name     = '%s'\n", g_authProviderName);
  printf("OAuth Provider Code     = '%s'\n", g_authProviderCode);

  if (OCInit(nullptr, 0, OC_CLIENT) != OC_STACK_OK)
  {
    printf("OCStack init error\n");
    return 2;
  }

  OCStackResult res;

  printf("Register account to cloud using %s %s\n", g_authProviderName, g_authProviderCode);

  res = OCCloudRegisterLogin(g_host, g_authProviderName, g_authProviderCode, handleRegisterCB);

  printf("OCCloudRegisterLogin return %d\n", (int)res);

  printf("Waiting response..\n");

  while (true)
  {
    if (OCProcess() != OC_STACK_OK)
    {
      printf("OCProcess process error\n");
    }
    usleep(100000);
    if (g_session != nullptr) break;
  }

  printf("IoTivityCloud Session Value =  '%s'\n", g_session);

  if (OCStop() != OC_STACK_OK)
  {
    printf("OCStop process error\n");
  }

  tinyara_free(&g_authProviderName);
  tinyara_free(&g_authProviderCode);

  task_create("iotivity-cloud-test", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE,
              iotivity_cloud_main_cb, (FAR char * const *)nullptr);

  return 0;
}

int iotivity_cloud_main(int argc, char *argv[])
{
  if (argc == 4)
  {
    tinyara_strcpy(&g_host, coap_tcp_header, nullptr);
    tinyara_strcat(&g_host, argv[1], nullptr);

    tinyara_strcpy(&g_authProviderName, argv[2], nullptr);
    tinyara_strcpy(&g_authProviderCode, argv[3], nullptr);

    task_create("iotivity-cloud-get-session", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE,
                iotivity_cloud_main_get_session, (FAR char * const *)nullptr);
    return 0;
  }

  if (argc == 3)
  {
    tinyara_strcpy(&g_host, coap_tcp_header, nullptr);
    tinyara_strcat(&g_host, argv[1], nullptr);

    tinyara_strcpy(&g_session, argv[2], nullptr);

    task_create("iotivity-cloud-test", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE,
                iotivity_cloud_main_cb, (FAR char * const *)nullptr);
    return 0;
  }

  PrintUsage();
  return 0;
}
#endif

