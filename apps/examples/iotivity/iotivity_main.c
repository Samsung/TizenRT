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
//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

/**
   * @testcase		   :iotivity_lwip_tc
   * @brief		   :Discovery resources and devices using IoTivity
   * @scenario		   :
   * @apicovered	   :
   * @precondition	   :At Linux PC,	1. Build an Iotivity client
   *			   :			 $ ./occlient -t <tc number>
   *			   :At test board, 	1. Make a binary with 'iotivity' example configuration and flash
   *			   :			2. Run the TASH
   *			   :			3. Connect to Wi-Fi and get an IP address using DHCP
   *			   :			 TASH>> iotivity
   * @postcondition	   :
   */

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocserver.h"

/* For WiFi -- */
#include <net/if.h>
#include <netinet/in.h>
#include <apps/netutils/netlib.h>
#ifdef CONFIG_NET_LWIP
#include <net/lwip/tcpip.h>
#include <net/lwip/netif.h>
#include <net/lwip/arch/sys_arch.h>
#include <net/lwip/ipv4/inet.h>
#else
#include <arpa/inet.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/


#define IFHWADDRLEN 6

#if defined(LWIP_HAVE_LOOPIF) && LWIP_HAVE_LOOPIF == 1
#define NET_DEVNUM 1
#else
#define NET_DEVNUM 0
#endif

#if defined(CONFIG_ENC28J60) || defined(CONFIG_WICED)
#define NET_DEVNAME "en"
#elif defined(CONFIG_ARCH_BOARD_SIDK_S5JT200)
#define NET_DEVNAME "wl"
#else
#error "undefined CONFIG_NET_<type>, check your .config"
#endif

#define nlldbg printf

static void lowlevelif_bringup(void)
{
  char ifname[6];
  uint8_t iff;
  int ret = 0;
  snprintf(ifname, IFNAMSIZ, "%s%d", NET_DEVNAME, NET_DEVNUM);
  /* Bring Up the desired network interface */
  nlldbg("LWIP Bringing up network interface (%s)\n", ifname);
  ret = netlib_ifup(ifname);
  if (ret < 0)
  {
	printf("LWIP network interface (%s) bring up failed\n", ifname);
  }
  ret = netlib_getifstatus(ifname, &iff);
  if (ret != OK)
  {
	printf("ERROR: Get %s interface flags error : %d\n", ifname, ret);
  }
  if (iff & IFF_UP)
  {
	printf("LWIP network interface (%s) is UP now\n", ifname);
  }

}

/* -- For WiFi */

//string length of "/a/light/" + std::numeric_limits<int>::digits10 + '\0'"
// 9 + 9 + 1 = 19
const int URI_MAXSIZE = 19;

static int gObserveNotifyType = 3;

int gQuitFlag = 0;
int gLightUnderObservation = 0;

static LightResource Light;
// This variable determines instance number of the Light resource.
// Used by POST method to create a new instance of Light resource.
static int gCurrLightInstance = 0;

static LightResource gLightInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

Observers interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

pthread_t threadId_observe;
pthread_t threadId_presence;

static bool observeThreadStarted = false;

#ifdef WITH_PRESENCE
#define numPresenceResources (2)
#endif

char *gResourceUri = (char *)"/a/light";
const char *dateOfManufacture = "myDateOfManufacture";
const char *deviceName = "myDeviceName";
const char *deviceUUID = "myDeviceUUID";
const char *firmwareVersion = "myFirmwareVersion";
const char *manufacturerName = "myName";
const char *operatingSystemVersion = "myOS";
const char *hardwareVersion = "myHardwareVersion";
const char *platformID = "myPlatformID";
const char *manufacturerUrl = "myManufacturerUrl";
const char *modelNumber = "myModelNumber";
const char *platformVersion = "myPlatformVersion";
const char *supportUrl = "mySupportUrl";
const char *version = "myVersion";
const char *systemTime = "2015-05-15T11.04";

// Entity handler should check for resourceTypeName and ResourceInterface in order to GET
// the existence of a known resource
const char *resourceTypeName = "core.light";
const char *resourceInterface = OC_RSRVD_INTERFACE_DEFAULT;

OCPlatformInfo platformInfo;
OCDeviceInfo deviceInfo;

OCRepPayload *getPayload(const char *uri, int64_t power, bool state)
{
  OCRepPayload *payload = OCRepPayloadCreate();
  if (!payload)
  {
    printf("Failed to allocate Payload");
    return NULL;
  }

  OCRepPayloadSetUri(payload, uri);
  OCRepPayloadSetPropBool(payload, "state", state);
  OCRepPayloadSetPropInt(payload, "power", power);

  return payload;
}

//This function takes the request as an input and returns the response
OCRepPayload *constructResponse(OCEntityHandlerRequest *ehRequest)
{
  if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
  {
    printf("Incoming payload not a representation");
    return NULL;
  }

  OCRepPayload *input = (OCRepPayload *)ehRequest->payload;

  LightResource *currLightResource = &Light;

  if (ehRequest->resource == gLightInstance[0].handle)
  {
    currLightResource = &gLightInstance[0];
    gResourceUri = (char *) "a/light/0";
  }
  else if (ehRequest->resource == gLightInstance[1].handle)
  {
    currLightResource = &gLightInstance[1];
    gResourceUri = (char *) "a/light/1";
  }

  if (OC_REST_PUT == ehRequest->method)
  {
    // Get pointer to query
    int64_t pow;
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

  return getPayload(gResourceUri, currLightResource->power, currLightResource->state);
}

/*
 * Very simple example of query parsing.
 * The query may have multiple filters separated by ';'.
 * It is upto the entity handler to parse the query for the individual filters,
 * VALIDATE them and respond as it sees fit.

 * This function only returns false if the query is exactly "power<X" and
 * current power is greater than X. If X cannot be parsed for an int,
 * true is returned.
 */
bool checkIfQueryForPowerPassed(char *query)
{
  if (query && strncmp(query, "power<", strlen("power<")) == 0)
  {
    char *pointerToOperator = strstr(query, "<");

    if (pointerToOperator)
    {
      int powerRequested = atoi(pointerToOperator + 1);
      if (Light.power > powerRequested)
      {
        OCLogv(INFO, TAG, "Current power: %d. Requested: <%d", Light.power
               , powerRequested);
        return false;
      }
    }
  }
  return true;
}

/*
 * Application should validate and process these as desired.
 */
OCEntityHandlerResult ValidateQueryParams(OCEntityHandlerRequest *entityHandlerRequest)
{
  OCLogv(INFO, TAG, PCF("Received query %s"), entityHandlerRequest->query);
  OCLog(INFO, TAG, PCF("Not processing query"));
  return OC_EH_OK;
}

OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest,
                                        OCRepPayload **payload)
{
  OCEntityHandlerResult ehResult;
  bool queryPassed = checkIfQueryForPowerPassed(ehRequest->query);

  // Empty payload if the query has no match.
  if (queryPassed)
  {
    OCRepPayload *getResp = constructResponse(ehRequest);
    if (!getResp)
    {
      OCLog(ERROR, TAG, "constructResponse failed");
      return OC_EH_ERROR;
    }

    *payload = getResp;
    ehResult = OC_EH_OK;
  }
  else
  {
    ehResult = OC_EH_OK;
  }

  return ehResult;
}

OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest *ehRequest,
                                        OCRepPayload **payload)
{
  OCEntityHandlerResult ehResult;
  OCRepPayload *putResp = constructResponse(ehRequest);

  if (!putResp)
  {
    OCLog(ERROR, TAG, "Failed to construct Json response");
    return OC_EH_ERROR;
  }

  *payload = putResp;
  ehResult = OC_EH_OK;

  return ehResult;
}

OCEntityHandlerResult ProcessPostRequest(OCEntityHandlerRequest *ehRequest,
                                         OCEntityHandlerResponse *response, OCRepPayload **payload)
{
  OCEntityHandlerResult ehResult = OC_EH_OK;
  OCRepPayload *respPLPost_light = NULL;

  /*
   * The entity handler determines how to process a POST request.
   * Per the REST paradigm, POST can also be used to update representation of existing
   * resource or create a new resource.
   * In the sample below, if the POST is for /a/light then a new instance of the Light
   * resource is created with default representation (if representation is included in
   * POST payload it can be used as initial values) as long as the instance is
   * lesser than max new instance count. Once max instance count is reached, POST on
   * /a/light updated the representation of /a/light (just like PUT)
   */

  if (ehRequest->resource == Light.handle)
  {
    if (gCurrLightInstance < SAMPLE_MAX_NUM_POST_INSTANCE)
    {
      // Create new Light instance
      char newLightUri[URI_MAXSIZE];
      snprintf(newLightUri, URI_MAXSIZE, "/a/light/%d", gCurrLightInstance);

      respPLPost_light = OCRepPayloadCreate();
      OCRepPayloadSetUri(respPLPost_light, gResourceUri);
      OCRepPayloadSetPropString(respPLPost_light, "createduri", newLightUri);

      if (0 == createLightResource(newLightUri, &gLightInstance[gCurrLightInstance]))
      {
        OCLog(INFO, TAG, "Created new Light instance\n");
        gLightInstance[gCurrLightInstance].state = 0;
        gLightInstance[gCurrLightInstance].power = 0;
        gCurrLightInstance++;
        strncpy((char *)response->resourceUri, newLightUri, MAX_URI_LENGTH);
        ehResult = OC_EH_RESOURCE_CREATED;
      }
    }
    else
    {
      // Update repesentation of /a/light
      Light.state = true;
      Light.power = 11;
      respPLPost_light = constructResponse(ehRequest);
    }
  }
  else
  {
    for (int i = 0; i < SAMPLE_MAX_NUM_POST_INSTANCE; i++)
    {
      if (ehRequest->resource == gLightInstance[i].handle)
      {
        gLightInstance[i].state = true;
        gLightInstance[i].power = 22;
        if (i == 0)
        {
          respPLPost_light = constructResponse(ehRequest);
          break;
        }
        else if (i == 1)
        {
          respPLPost_light = constructResponse(ehRequest);
        }
      }
    }
  }

  if ((respPLPost_light != NULL))
  {
    *payload = respPLPost_light;
  }
  else
  {
    OCLog(INFO, TAG, "Payload was NULL");
    ehResult = OC_EH_ERROR;
  }

  return ehResult;
}

OCEntityHandlerResult ProcessDeleteRequest(OCEntityHandlerRequest *ehRequest)
{
  if (ehRequest == NULL)
  {
    OCLog(INFO, TAG, "The ehRequest is NULL");
    return OC_EH_ERROR;
  }
  OCEntityHandlerResult ehResult = OC_EH_OK;

  OCLogv(INFO, TAG, "\n\nExecuting %s for resource %p ", __func__, ehRequest->resource);

  /*
   * In the sample below, the application will:
   * 1a. pass the delete request to the c stack
   * 1b. internally, the c stack figures out what needs to be done and does it accordingly
   *    (e.g. send observers notification, remove observers...)
   * 1c. the c stack returns with the result whether the request is fullfilled.
   * 2. optionally, app removes observers out of its array 'interestedObservers'
   */

  if ((ehRequest != NULL) && (ehRequest->resource == Light.handle))
  {
    //Step 1: Ask stack to do the work.
    OCStackResult result = OCDeleteResource(ehRequest->resource);

    if (result == OC_STACK_OK)
    {
      OCLog(INFO, TAG, "\n\nDelete Resource operation succeeded.");
      ehResult = OC_EH_OK;

      //Step 2: clear observers who wanted to observe this resource at the app level.
      for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++)
      {
        if (interestedObservers[i].resourceHandle == ehRequest->resource)
        {
          interestedObservers[i].valid = false;
          interestedObservers[i].observationId = 0;
          interestedObservers[i].resourceHandle = NULL;
        }
      }
    }
    else if (result == OC_STACK_NO_RESOURCE)
    {
      OCLog(INFO, TAG, "\n\nThe resource doesn't exist or it might have been deleted.");
      ehResult = OC_EH_RESOURCE_DELETED;
    }
    else
    {
      OCLog(INFO, TAG, "\n\nEncountered error from OCDeleteResource().");
      ehResult = OC_EH_ERROR;
    }
  }
  else if (ehRequest->resource != Light.handle)
  {
    //Let's this app not supporting DELETE on some resources so
    //consider the DELETE request is received for a non-support resource.
    OCLogv(INFO, TAG, "\n\nThe request is received for a non-support resource.");
    ehResult = OC_EH_FORBIDDEN;
  }

  return ehResult;
}

OCEntityHandlerResult ProcessNonExistingResourceRequest(OCEntityHandlerRequest *ptr)
{
  OCLogv(INFO, TAG, "\n\nExecuting %s ", __func__);

  return OC_EH_RESOURCE_NOT_FOUND;
}

void ProcessObserveRegister(OCEntityHandlerRequest *ehRequest)
{
  OCLogv(INFO, TAG, "Received observation registration request with observation Id %d",
         ehRequest->obsInfo.obsId);

  if (!observeThreadStarted)
  {
    pthread_create(&threadId_observe, NULL, ChangeLightRepresentation, (void *)NULL);
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

  OCLogv(INFO, TAG, "Received observation deregistration request for observation Id %d",
         ehRequest->obsInfo.obsId);
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

OCEntityHandlerResult
OCDeviceEntityHandlerCb(OCEntityHandlerFlag flag,
                        OCEntityHandlerRequest *entityHandlerRequest,
                        char *uri,
                        void *callbackParam)
{
  OCLogv(INFO, TAG, "Inside device default entity handler - flags: 0x%x, uri: %s", flag, uri);

  OCEntityHandlerResult ehResult = OC_EH_OK;
  OCEntityHandlerResponse response;

  // Validate pointer
  if (!entityHandlerRequest)
  {
    OCLog(ERROR, TAG, "Invalid request pointer");
    return OC_EH_ERROR;
  }
  // Initialize certain response fields
  response.numSendVendorSpecificHeaderOptions = 0;
  memset(response.sendVendorSpecificHeaderOptions, 0,
         sizeof response.sendVendorSpecificHeaderOptions);
  memset(response.resourceUri, 0, sizeof response.resourceUri);
  OCRepPayload *payload = NULL;


  if (flag & OC_REQUEST_FLAG)
  {
    OCLog(INFO, TAG, "Flag includes OC_REQUEST_FLAG");

    if (entityHandlerRequest->resource == NULL)
    {
      OCLog(INFO, TAG, "Received request from client to a non-existing resource");
      ehResult = ProcessNonExistingResourceRequest(entityHandlerRequest);
    }
    else if (OC_REST_GET == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_GET from client");
      ehResult = ProcessGetRequest(entityHandlerRequest, &payload);
    }
    else if (OC_REST_PUT == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_PUT from client");
      ehResult = ProcessPutRequest(entityHandlerRequest, &payload);
    }
    else if (OC_REST_DELETE == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_DELETE from client");
      ehResult = ProcessDeleteRequest(entityHandlerRequest);
    }
    else
    {
      OCLogv(INFO, TAG, "Received unsupported method %d from client",
             entityHandlerRequest->method);
      ehResult = OC_EH_ERROR;
    }
    // If the result isn't an error or forbidden, send response
    if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
    {
      // Format the response.  Note this requires some info about the request
      response.requestHandle = entityHandlerRequest->requestHandle;
      response.resourceHandle = entityHandlerRequest->resource;
      response.ehResult = ehResult;
      response.payload = (OCPayload *)payload;
      // Indicate that response is NOT in a persistent buffer
      response.persistentBufferFlag = 0;

      // Send the response
      if (OCDoResponse(&response) != OC_STACK_OK)
      {
        OCLog(ERROR, TAG, "Error sending response");
        ehResult = OC_EH_ERROR;
      }
    }
  }
  if (flag & OC_OBSERVE_FLAG)
  {
    OCLog(INFO, TAG, "Flag includes OC_OBSERVE_FLAG");
    if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
    {
      OCLog(INFO, TAG, "Received OC_OBSERVE_REGISTER from client");
    }
    else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
    {
      OCLog(INFO, TAG, "Received OC_OBSERVE_DEREGISTER from client");
    }
  }

  return ehResult;
}

OCEntityHandlerResult
OCNOPEntityHandlerCb(OCEntityHandlerFlag flag,
                     OCEntityHandlerRequest *entityHandlerRequest,
                     void *callbackParam)
{
  // This is callback is associated with the 2 presence notification
  // resources. They are non-operational.
  return OC_EH_OK;
}

OCEntityHandlerResult
OCEntityHandlerCb(OCEntityHandlerFlag flag,
                  OCEntityHandlerRequest *entityHandlerRequest, void *callback)
{
  OCLogv(INFO, TAG, "Inside entity handler - flags: 0x%x", flag);

  OCEntityHandlerResult ehResult = OC_EH_OK;
  OCEntityHandlerResponse response = { 0, 0, OC_EH_ERROR, 0, 0, { }, { 0 }, false };

  // Validate pointer
  if (!entityHandlerRequest)
  {
    OCLog(ERROR, TAG, "Invalid request pointer");
    return OC_EH_ERROR;
  }

  // Initialize certain response fields
  response.numSendVendorSpecificHeaderOptions = 0;
  memset(response.sendVendorSpecificHeaderOptions,
         0, sizeof response.sendVendorSpecificHeaderOptions);
  memset(response.resourceUri, 0, sizeof response.resourceUri);
  OCRepPayload *payload = NULL;

  if (flag & OC_REQUEST_FLAG)
  {
    OCLog(INFO, TAG, "Flag includes OC_REQUEST_FLAG");

    if (OC_REST_GET == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_GET from client");
      ehResult = ProcessGetRequest(entityHandlerRequest, &payload);
    }
    else if (OC_REST_PUT == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_PUT from client");
      ehResult = ProcessPutRequest(entityHandlerRequest, &payload);
    }
    else if (OC_REST_POST == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_POST from client");
      ehResult = ProcessPostRequest(entityHandlerRequest, &response, &payload);
    }
    else if (OC_REST_DELETE == entityHandlerRequest->method)
    {
      OCLog(INFO, TAG, "Received OC_REST_DELETE from client");
      ehResult = ProcessDeleteRequest(entityHandlerRequest);
    }
    else
    {
      OCLogv(INFO, TAG, "Received unsupported method %d from client",
             entityHandlerRequest->method);
      ehResult = OC_EH_ERROR;
    }
    // If the result isn't an error or forbidden, send response
    if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN)))
    {
      // Format the response.  Note this requires some info about the request
      response.requestHandle = entityHandlerRequest->requestHandle;
      response.resourceHandle = entityHandlerRequest->resource;
      response.ehResult = ehResult;
      response.payload = (OCPayload *)payload;
      // Indicate that response is NOT in a persistent buffer
      response.persistentBufferFlag = 0;

      // Handle vendor specific options
      if (entityHandlerRequest->rcvdVendorSpecificHeaderOptions &&
          entityHandlerRequest->numRcvdVendorSpecificHeaderOptions)
      {
        OCLog(INFO, TAG, "Received vendor specific options");
        uint8_t i = 0;
        OCHeaderOption *rcvdOptions =
          entityHandlerRequest->rcvdVendorSpecificHeaderOptions;
        for (i = 0; i < entityHandlerRequest->numRcvdVendorSpecificHeaderOptions; i++)
        {
          if (((OCHeaderOption)rcvdOptions[i]).protocolID == OC_COAP_ID)
          {
            OCLogv(INFO, TAG, "Received option with OC_COAP_ID and ID %u with",
                   ((OCHeaderOption)rcvdOptions[i]).optionID);

            OCLogBuffer(INFO, TAG, ((OCHeaderOption)rcvdOptions[i]).optionData,
                        MAX_HEADER_OPTION_DATA_LENGTH);
          }
        }
        OCHeaderOption *sendOptions = response.sendVendorSpecificHeaderOptions;
        uint8_t option2[] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
        uint8_t option3[] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
        sendOptions[0].protocolID = OC_COAP_ID;
        sendOptions[0].optionID = 2248;
        memcpy(sendOptions[0].optionData, option2, sizeof(option2));
        sendOptions[0].optionLength = 10;
        sendOptions[1].protocolID = OC_COAP_ID;
        sendOptions[1].optionID = 2600;
        memcpy(sendOptions[1].optionData, option3, sizeof(option3));
        sendOptions[1].optionLength = 10;
        response.numSendVendorSpecificHeaderOptions = 2;
      }

      // Send the response
      if (OCDoResponse(&response) != OC_STACK_OK)
      {
        OCLog(ERROR, TAG, "Error sending response");
        ehResult = OC_EH_ERROR;
      }
    }
  }
  if (flag & OC_OBSERVE_FLAG)
  {
    OCLog(INFO, TAG, "Flag includes OC_OBSERVE_FLAG");

    if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
    {
      OCLog(INFO, TAG, "Received OC_OBSERVE_REGISTER from client");
      ProcessObserveRegister(entityHandlerRequest);
    }
    else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
    {
      OCLog(INFO, TAG, "Received OC_OBSERVE_DEREGISTER from client");
      ProcessObserveDeregister(entityHandlerRequest);
    }
  }

  OCPayloadDestroy(response.payload);
  return ehResult;
}

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
  gQuitFlag = 1;
}

void *ChangeLightRepresentation(void *param)
{
  (void)param;
  OCStackResult result = OC_STACK_ERROR;

  uint8_t j = 0;
  uint8_t numNotifies = (SAMPLE_MAX_NUM_OBSERVATIONS) / 2;
  OCObservationId obsNotify[numNotifies];

  while (!gQuitFlag)
  {
    sleep(3);
    Light.power += 5;
    if (gLightUnderObservation)
    {
      OCLogv(INFO, TAG, " =====> Notifying stack of new power level %d\n", Light.power);
      if (gObserveNotifyType == 1)
      {
        // Notify list of observers. Alternate observers on the list will be notified.
        j = 0;
        for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; (i = i + 2))
        {
          if (interestedObservers[i].valid == true)
          {
            obsNotify[j] = interestedObservers[i].observationId;
            j++;
          }
        }

        OCRepPayload *payload = getPayload(gResourceUri, Light.power, Light.state);
        result = OCNotifyListOfObservers(Light.handle, obsNotify, j,
                                         payload, OC_NA_QOS);
        OCRepPayloadDestroy(payload);
      }
      else if (gObserveNotifyType == 0)
      {
        // Notifying all observers
        result = OCNotifyAllObservers(Light.handle, OC_NA_QOS);
        if (OC_STACK_NO_OBSERVERS == result)
        {
          OCLog(INFO, TAG,
                "=======> No more observers exist, stop sending observations");
          gLightUnderObservation = 0;
        }
      }
      else
      {
        OCLog(ERROR, TAG, "Incorrect notification type selected");
      }
    }
  }
  return NULL;
}

#ifdef WITH_PRESENCE
void *presenceNotificationGenerator(void *param)
{
  uint8_t secondsBeforePresence = 10;
  OCLogv(INFO, TAG, "Will send out presence in %u seconds", secondsBeforePresence);
  sleep(secondsBeforePresence);
  (void)param;
  OCDoHandle presenceNotificationHandles[numPresenceResources];
  OCStackResult res = OC_STACK_OK;

//    std::array<std::string, numPresenceResources> presenceNotificationResources { {
//        std::string("core.fan"),
//        std::string("core.led") } };
//    std::array<std::string, numPresenceResources> presenceNotificationUris { {
//        std::string("/a/fan"),
//        std::string("/a/led") } };

  char *presenceNotificationResources[numPresenceResources] = {"core.fan", "core.led"};
  char *presenceNotificationUris[numPresenceResources] = {"/a/fan", "/a/led"};

  for (int i = 0; i < numPresenceResources; i++)
  {
    if (res == OC_STACK_OK)
    {
      sleep(1);
      res = OCCreateResource(&presenceNotificationHandles[i],
                             presenceNotificationResources[i],
                             OC_RSRVD_INTERFACE_DEFAULT,
                             presenceNotificationUris[i],
                             OCNOPEntityHandlerCb,
                             NULL,
                             OC_DISCOVERABLE | OC_OBSERVABLE);
    }
    if (res != OC_STACK_OK)
    {
      OCLogv(ERROR, TAG, "\"Presence Notification Generator\" failed to create resource "
             "%s with result %s.", presenceNotificationResources[i],
             getResult(res));
      break;
    }
    OCLogv(INFO, TAG, PCF("Created %s for presence notification"),
           presenceNotificationUris[i]);
  }
  sleep(5);
  for (int i = 0; i < numPresenceResources; i++)
  {
    if (res == OC_STACK_OK)
    {
      res = OCDeleteResource(presenceNotificationHandles[i]);
    }
    if (res != OC_STACK_OK)
    {
      OCLogv(ERROR, TAG, "\"Presence Notification Generator\" failed to delete "\
             "resource %s.", presenceNotificationResources[i]);
      break;
    }
    OCLogv(INFO, TAG, PCF("Deleted %s for presence notification"),
           presenceNotificationUris[i]);
  }

  OCLog(INFO, TAG, "================ stopping presence");
  OCStopPresence();

  return NULL;
}
#endif


int createLightResource(char *uri, LightResource *lightResource)
{
  if (!uri)
  {
    OCLog(ERROR, TAG, "Resource URI cannot be NULL");
    return -1;
  }

  lightResource->state = false;
  lightResource->power = 0;
  OCStackResult res = OCCreateResource(&(lightResource->handle),
                                       "core.light",
                                       "oc.mi.def",
                                       uri,
                                       OCEntityHandlerCb,
                                       NULL,
                                       OC_DISCOVERABLE | OC_OBSERVABLE);
  OCLogv(INFO, TAG, "Created Light resource with result: %s", getResult(res));

  return 0;
}

bool DuplicateString(char **targetString, const char *sourceString)
{
  if (!sourceString)
  {
    return false;
  }
  else
  {
    *targetString = (char *) malloc(strlen(sourceString) + 1);

    if (*targetString)
    {
      strncpy(*targetString, sourceString, (strlen(sourceString) + 1));
      return true;
    }
  }
  return false;
}

OCStackResult SetPlatformInfo(const char *platformID, const char *manufacturerName,
                              const char *manufacturerUrl, const char *modelNumber, const char *dateOfManufacture,
                              const char *platformVersion, const char *operatingSystemVersion, const char *hardwareVersion,
                              const char *firmwareVersion, const char *supportUrl, const char *systemTime)
{

  bool success = true;

  if (manufacturerName != NULL && (strlen(manufacturerName) > MAX_MANUFACTURER_NAME_LENGTH))
  {
    return OC_STACK_INVALID_PARAM;
  }

  if (manufacturerUrl != NULL && (strlen(manufacturerUrl) > MAX_MANUFACTURER_URL_LENGTH))
  {
    return OC_STACK_INVALID_PARAM;
  }

  if (!DuplicateString(&platformInfo.platformID, platformID))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.manufacturerName, manufacturerName))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.modelNumber, modelNumber))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.platformVersion, platformVersion))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.operatingSystemVersion, operatingSystemVersion))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.hardwareVersion, hardwareVersion))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.firmwareVersion, firmwareVersion))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.supportUrl, supportUrl))
  {
    success = false;
  }

  if (!DuplicateString(&platformInfo.systemTime, systemTime))
  {
    success = false;
  }

  if (success)
  {
    return OC_STACK_OK;
  }

  DeletePlatformInfo();
  return OC_STACK_ERROR;
}

OCStackResult SetDeviceInfoName(const char *deviceName)
{
  if (!DuplicateString(&deviceInfo.deviceName, deviceName))
  {
    return OC_STACK_ERROR;
  }
  return OC_STACK_OK;
}

static void PrintUsage()
{
  OCLog(INFO, TAG, "Usage : ocserver -o <0|1>");
  OCLog(INFO, TAG, "-o 0 : Notify all observers");
  OCLog(INFO, TAG, "-o 1 : Notify list of observers");
}

#ifdef RA_ADAPTER
static void jidbound(char *jid)
{
  OCLogv(INFO, TAG, "\n\n    Bound JID: %s\n\n", jid);
}
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
#if defined(CONFIG_NET_LWIP) && defined(CONFIG_TASH)
int iotivity_main_cb(int argc, char *argv[])
#else
int iotivity_main(int argc, char *argv[])
#endif
#endif
{

#ifndef CONFIG_NET_LWIP_DHCPC
  lowlevelif_bringup();
#endif

#ifdef RA_ADAPTER
  char host[] = "localhost";
  char user[] = "test1";
  char pass[] = "intel123";
  char empstr[] = "";
  OCRAInfo_t rainfo = {};

  rainfo.hostname = host;
  rainfo.port = 5222;
  rainfo.xmpp_domain = host;
  rainfo.username = user;
  rainfo.password = pass;
  rainfo.resource = empstr;
  rainfo.user_jid = empstr;
  rainfo.jidbound = jidbound;
#endif

  /* For Test -- */
  gObserveNotifyType = 0;
  /* -- For Test */

  //printf("#################### STEP 1 ##########################");
  /*
  int opt = 0;
  while ((opt = getopt(3, args, "o:s:p:d:u:w:r:j:")) != -1)
  {
      switch(opt)
      {
          case 'o':
              gObserveNotifyType = atoi(optarg);
              break;
  #ifdef RA_ADAPTER
          case 's':
              rainfo.hostname = optarg;
              break;
          case 'p':
              rainfo.port = atoi(optarg);
              break;
          case 'd':
              rainfo.xmpp_domain = optarg;
              break;
          case 'u':
              rainfo.username = optarg;
              break;
          case 'w':
              rainfo.password = optarg;
              break;
          case 'j':
              rainfo.user_jid = optarg;
              break;
          case 'r':
              rainfo.resource = optarg;
              break;
  #endif
          default:
              PrintUsage();
              return -1;
      }
  }
  */

  if ((gObserveNotifyType != 0) && (gObserveNotifyType != 1))
  {
    PrintUsage();
    return -1;
  }

#ifdef RA_ADAPTER
  OCSetRAInfo(&rainfo);
#endif
  printf("\n[Iotivity Demo - IOTIVITY version is %s] : OCServer is starting...\n", IOTIVITY_VERSION);
  if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
  {
    printf("OCStack init error\n");
    return 0;
  }

  //printf("#################### STEP 2 ##########################\n");

#ifdef WITH_PRESENCE
  if (OCStartPresence(0) != OC_STACK_OK)
  {
    OCLog(ERROR, TAG, "OCStack presence/discovery error");
    return 0;
  }
#endif

  //printf("#################### STEP 3 ##########################\n");

  OCSetDefaultDeviceEntityHandler(OCDeviceEntityHandlerCb, NULL);
  OCStackResult registrationResult =
    SetPlatformInfo(platformID, manufacturerName, manufacturerUrl, modelNumber,
                    dateOfManufacture, platformVersion,  operatingSystemVersion,  hardwareVersion,
                    firmwareVersion,  supportUrl, systemTime);
  if (registrationResult != OC_STACK_OK)
  {
    printf("Platform info setting failed locally!\n");
    exit(EXIT_FAILURE);
  }

  //printf("#################### STEP 4 ##########################\n");

  registrationResult = OCSetPlatformInfo(platformInfo);

  if (registrationResult != OC_STACK_OK)
  {
    printf("Platform Registration failed!");
    exit(EXIT_FAILURE);
  }

  //printf("#################### STEP 5 ##########################\n");

  registrationResult = SetDeviceInfoName(deviceName);

  if (registrationResult != OC_STACK_OK)
  {
    printf("Device info setting failed locally!");
    exit(EXIT_FAILURE);
  }

  //printf("#################### STEP 6 ##########################\n");

  registrationResult = OCSetDeviceInfo(deviceInfo);

  if (registrationResult != OC_STACK_OK)
  {
    printf("Device Registration failed!");
    exit(EXIT_FAILURE);
  }

  //printf("#################### STEP 7 ##########################\n");

  /*
   * Declare and create the example resource: Light
   */
  createLightResource(gResourceUri, &Light);

  //printf("#################### STEP 8 ##########################\n");

  // Initialize observations data structure for the resource
  for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++)
  {
    interestedObservers[i].valid = false;
  }

  //printf("#################### STEP 9 ##########################\n");


  /*
   * Create a thread for generating changes that cause presence notifications
   * to be sent to clients
   */

#ifdef WITH_PRESENCE
  pthread_create(&threadId_presence, NULL, presenceNotificationGenerator, (void *)NULL);
#endif

  //printf("#################### STEP 10 #########################");

  // Break from loop with Ctrl-C
  OCLog(INFO, TAG, "Entering ocserver main loop...");
  OCLog(INFO, TAG, "Will send out presence in 10 seconds");


  DeletePlatformInfo();

  //printf("#################### STEP 11 #########################");

  DeleteDeviceInfo();

  //printf("#################### STEP 12 #########################");

  while (!gQuitFlag)
  {
    usleep(10000);
    if (OCProcess() != OC_STACK_OK)
    {
      OCLog(ERROR, TAG, "OCStack process error");
      return 0;
    }
  }

  //printf("#################### STEP 13 #########################");

  if (observeThreadStarted)
  {
    pthread_cancel(threadId_observe);
    pthread_join(threadId_observe, NULL);
  }

  //printf("#################### STEP 14 #########################");

  pthread_cancel(threadId_presence);
  pthread_join(threadId_presence, NULL);

  printf("Exiting ocserver main loop...");

  if (OCStop() != OC_STACK_OK)
  {
    printf("OCStack process error");
  }

  return 0;
}

#if defined(CONFIG_NET_LWIP) && defined(CONFIG_TASH)

#define IOTIVITY_TEST_STACKSIZE   16384
#define IOTIVITY_TEST_PRI   100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR


int iotivity_main(int argc, char *argv[])
{
#if 0
  pthread_attr_t     attr;
  struct sched_param sparam;
  pthread_t tid;

  pthread_attr_init(&attr);
  sparam.sched_priority = IOTIVITY_TEST_PRI;
  pthread_attr_setschedparam(&attr, &sparam);
  pthread_attr_setschedpolicy(&attr, IOTIVITY_TEST_SCHED_POLICIY);
  pthread_attr_setstacksize(&attr, IOTIVITY_TEST_STACKSIZE);
  pthread_create(&tid, &attr, iotivity_main_cb, NULL);
  pthread_setname_np(tid, "iotivity-test");
#endif

  task_create("iotivity-test", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE, iotivity_main_cb,
              (FAR char * const *)NULL);
  return 0;
}

#endif
