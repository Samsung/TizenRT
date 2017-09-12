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

#ifndef OC_RESOURCEHANDLER_H
#define OC_RESOURCEHANDLER_H

#include "ocstack.h"
#include "ocstackinternal.h"
#include "ocserverrequest.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Common JSON string components used by the stack to build JSON strings.
 * These details are exposed in ocstackconfig.h file in the form of documentation.
 * Remember to update the documentation there if these are changed.
 */
#define OC_JSON_PREFIX                     "{\"oic\":["
#define OC_JSON_PREFIX_LEN                 (sizeof(OC_JSON_PREFIX) - 1)
#define OC_JSON_SUFFIX                     "]}"
#define OC_JSON_SUFFIX_LEN                 (sizeof(OC_JSON_SUFFIX) - 1)
#define OC_JSON_SEPARATOR                  ','
#define OC_JSON_SEPARATOR_STR              ","

/**
 * Static values for various JSON attributes.
 */
#define OC_RESOURCE_OBSERVABLE   1
#define OC_RESOURCE_SECURE       1

/**
 * Device properties persistent store name.
 */
#define OC_DEVICE_PROPS_FILE_NAME  "device_properties.dat"

/**
 * Device properties name
 */
#define OC_JSON_DEVICE_PROPS_NAME "DeviceProperties"

/**
 *  OIC Virtual resources supported by every OIC device.
 */
typedef enum
{
    /** unknown URI.*/
    OC_UNKNOWN_URI =0,

    /** "/oic/res".*/
    OC_WELL_KNOWN_URI,

    /** "/oic/d" .*/
    OC_DEVICE_URI,

    /** "/oic/p" .*/
    OC_PLATFORM_URI,

    /** "/oic/res/d/type" .*/
    OC_RESOURCE_TYPES_URI,
#ifdef ROUTING_GATEWAY
    /** "/oic/gateway" .*/
    OC_GATEWAY_URI,
#endif
#ifdef WITH_PRESENCE
    /** "/oic/ad" .*/
    OC_PRESENCE,
#endif

#ifdef MQ_BROKER
    /** "/oic/ps" .*/
    OC_MQ_BROKER_URI,
#endif

#ifdef TCP_ADAPTER
    /** "/oic/ping" .*/
    OC_KEEPALIVE_RESOURCE_URI,
#endif

    /** "/oic/introspection" .*/
    OC_INTROSPECTION_URI,

    /** "/oic/introspection/payload" .*/
    OC_INTROSPECTION_PAYLOAD_URI,

    /** Max items in the list */
    OC_MAX_VIRTUAL_RESOURCES    //<s Max items in the list

} OCVirtualResources;

/**
 * The type of query a request/response message is.
 */
typedef enum
{
    STACK_RES_DISCOVERY_NOFILTER = 0,
    STACK_RES_DISCOVERY_IF_FILTER,
    STACK_RES_DISCOVERY_RT_FILTER,
    STACK_DEVICE_DISCOVERY_DI_FILTER,
    STACK_DEVICE_DISCOVERY_DN_FILTER
} StackQueryTypes;

/**
 * The type of handling required to handle a request.
 */
typedef enum
{
    OC_RESOURCE_VIRTUAL = 0,
    OC_RESOURCE_NOT_COLLECTION_WITH_ENTITYHANDLER,
    OC_RESOURCE_NOT_COLLECTION_DEFAULT_ENTITYHANDLER,
    OC_RESOURCE_COLLECTION_WITH_ENTITYHANDLER,
    OC_RESOURCE_COLLECTION_DEFAULT_ENTITYHANDLER,
    OC_RESOURCE_DEFAULT_DEVICE_ENTITYHANDLER,
    OC_RESOURCE_NOT_SPECIFIED
} ResourceHandling;

/**
 * This function returns the virtual resource of the given URI.
 * @return the virtual resource or ::OC_UNKNOWN_URI
 */
OCVirtualResources GetTypeOfVirtualURI(const char* resourceUri);

/**
 * Default entity handler (ie. callback) to be used for resources with
 * no entity handler.
 */
OCEntityHandlerResult defaultResourceEHandler(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest * request, void* callbackParam);

/**
 * Find and retrieve pointer to a resource associated with a specific resource
 * URI.
 * @return pointer to found resource
 */
OCResource * OC_CALL FindResourceByUri(const char* resourceUri);

/**
 * This function checks whether the specified resource URI aligns with a pre-existing
 * virtual resource; returns false otherwise.
 * @return true or false.
 */
bool IsVirtualResource(const char* resourceUri);

/**
 * Parameter handling returns by-reference the type of resource handling
 * required by the internal stack based on the specified request.
 *
 * @param request the OCServerRequest to the internal stack
 * @param handling the resource handling required by the internal stack
 * @param resource the OCResource from the stack
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult DetermineResourceHandling (const OCServerRequest *request,
                                         ResourceHandling *handling,
                                         OCResource **resource);

/**
 * Processes the specified request based on the type of resource handling
 * resHandling.
 *
 * @param resHandling the type of resource handling to be used
 * @param resource the resource to process the request on
 * @param request the request to process
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult ProcessRequest(ResourceHandling resHandling,
                             OCResource *resource,
                             OCServerRequest *request);

/**
 * Internal API used to save all of the platform's information for use in platform
 * discovery requests.
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SavePlatformInfo(OCPlatformInfo info);

/**
 * Internal API used to save all of the device's information for use in platform
 * discovery requests.
 * @param info       Device name is received from the application.
 *                   DeviceID, spec version and data model version are initialized by the stack.
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SaveDeviceInfo(OCDeviceInfo info);

/**
 * Internal API used to clear the platform information.
 */
void DeletePlatformInfo();

/**
 * Internal API used to clear the device information.
 */
void DeleteDeviceInfo();

/*
 * Prepare payload for resource representation.
 */
OCStackResult BuildResponseRepresentation(const OCResource *resourcePtr,
                    OCRepPayload** payload, OCDevAddr *devAddr);

/**
 * A helper function that Maps an @ref OCEntityHandlerResult type to an
 * @ref OCStackResult type.
 */
OCStackResult EntityHandlerCodeToOCStackCode(OCEntityHandlerResult ehResult);

/**
 * Data structure for holding enhanced device information
 */
typedef struct _OCDeviceProperties
{
    /** Protocol Independent Id.*/
    char protocolIndependentId[UUID_STRING_SIZE];
} OCDeviceProperties;

/**
 * Internal API used to initialize device properties.
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InitializeDeviceProperties();

/**
 * Internal API used to clean up device properties.
 * @param deviceProperties Pointer to OCDeviceProperties to clean up.
 */
void CleanUpDeviceProperties(OCDeviceProperties **deviceProperties);

/**
 * This method converts OCDeviceProperties into CBOR format.
 * @param deviceProperties   Pointer to OCDeviceProperties to convert to CBOR.
 * @param payload            JSON payload converted to CBOR. Passed parameter should not be NULL.
 * @note Caller needs to invoke OICFree when they are finished using the returned string.
 * @param size               Size of the cbor payload. Passed parameter should not be NULL.
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult DevicePropertiesToCBORPayload(const OCDeviceProperties *deviceProperties, uint8_t **payload, size_t *size);

/**
 * This method converts CBOR data into OCDeviceProperties format.
 * @param payload            CBOR payload to convert to OCDeviceProperties.
 * @param size               Size of the cborPayload.
 * @param deviceProperties   CBOR payload converted to OCDeviceProperties. Passed parameter should not be NULL.
 * @note Caller needs to invoke CleanUpDeviceProperties after they are finished using the returned pointer.
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult CBORPayloadToDeviceProperties(const uint8_t *payload, size_t size, OCDeviceProperties **deviceProperties);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif //OC_RESOURCEHANDLER_H
