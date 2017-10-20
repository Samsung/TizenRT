//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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

#ifndef OCPAYLOAD_H_
#define OCPAYLOAD_H_

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <stdbool.h>
#include <inttypes.h>
#include "octypes.h"
#include "platform_features.h"

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)
#include "experimental/securevirtualresourcetypes.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Macro to verify the validity of cbor operation.
 */
#define VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(log_tag, err, log_message) \
    if ((CborNoError != (err)) && (CborErrorOutOfMemory != (err))) \
    { \
        if ((log_tag) && (log_message)) \
        { \
            OIC_LOG_V(ERROR, (log_tag), "%s with cbor error: \'%s\'.", \
                    (log_message), (cbor_error_string(err))); \
        } \
        goto exit; \
    } \

#define VERIFY_PARAM_NON_NULL(log_tag, err, log_message) \
    if (NULL == (err)) \
    { \
        OIC_LOG_V(FATAL, (log_tag), "%s", (log_message)); \
        goto exit;\
    } \

#define VERIFY_CBOR_NOT_OUTOFMEMORY(log_tag, err, log_message) \
    if (CborErrorOutOfMemory == (err)) \
    { \
        if ((log_tag) && (log_message)) \
        { \
            OIC_LOG_V(ERROR, (log_tag), "%s with cbor error: \'%s\'.", \
                    (log_message), (cbor_error_string(err))); \
        } \
        goto exit; \
    } \

typedef struct OCResource OCResource;

void OC_CALL OCPayloadDestroy(OCPayload* payload);

// Representation Payload
OCRepPayload* OC_CALL OCRepPayloadCreate();

size_t OC_CALL calcDimTotal(const size_t dimensions[MAX_REP_ARRAY_DEPTH]);

OCRepPayload* OC_CALL OCRepPayloadClone(const OCRepPayload* payload);

OCRepPayload* OC_CALL OCRepPayloadBatchClone(const OCRepPayload* repPayload);

void OC_CALL OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child);

bool OC_CALL OCRepPayloadSetUri(OCRepPayload* payload, const char* uri);

bool OC_CALL OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType);
bool OC_CALL OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface);

bool OC_CALL OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType);
bool OC_CALL OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface);

bool OC_CALL OCRepPayloadIsNull(const OCRepPayload* payload, const char* name);
bool OC_CALL OCRepPayloadSetNull(OCRepPayload* payload, const char* name);

bool OC_CALL OCRepPayloadSetPropInt(OCRepPayload* payload, const char* name, int64_t value);
bool OC_CALL OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value);

bool OC_CALL OCRepPayloadSetPropDouble(OCRepPayload* payload, const char* name, double value);
bool OC_CALL OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value);

/**
 * This function allocates memory for the byte string and sets it in the payload.
 *
 * @param payload      Pointer to the payload to which byte string needs to be added.
 * @param name         Name of the byte string.
 * @param value        Byte string and it's length.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadSetPropByteString(OCRepPayload* payload, const char* name, OCByteString value);

/**
 * This function sets the byte string in the payload.
 *
 * @param payload      Pointer to the payload to which byte string needs to be added.
 * @param name         Name of the byte string.
 * @param value        Byte string and it's length.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadSetPropByteStringAsOwner(OCRepPayload* payload, const char* name,
        OCByteString* value);

/**
 * This function gets the byte string from the payload.
 *
 * @param payload      Pointer to the payload from which byte string needs to be retrieved.
 * @param name         Name of the byte string.
 * @param value        Byte string and it's length.
 *
 * @note: Caller needs to invoke OCFree on value.bytes after it is finished using the byte string.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadGetPropByteString(const OCRepPayload* payload, const char* name,
        OCByteString* value);

bool OC_CALL OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value);
bool OC_CALL OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value);
bool OC_CALL OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value);

bool OC_CALL OCRepPayloadSetPropBool(OCRepPayload* payload, const char* name, bool value);
bool OC_CALL OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value);

bool OC_CALL OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value);
bool OC_CALL OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name, OCRepPayload* value);
bool OC_CALL OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value);

#ifdef __WITH_TLS__
bool OC_CALL OCRepPayloadSetPropPubDataType(OCRepPayload *payload, const char *name, const OicSecKey_t *value);
bool OC_CALL OCRepPayloadSetPropPubDataTypeAsOwner(OCRepPayload *payload, const char *name, const OicSecKey_t *value);
bool OC_CALL OCRepPayloadGetPropPubDataType(const OCRepPayload *payload, const char *name, OicSecKey_t *value);
#endif

/**
 * This function allocates memory for the byte string array and sets it in the payload.
 *
 * @param payload      Pointer to the payload to which byte string array needs to be added.
 * @param name         Name of the byte string.
 * @param array        Byte string array.
 * @param dimensions   Number of byte strings in above array.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadSetByteStringArrayAsOwner(OCRepPayload* payload, const char* name,
        OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

/**
 * This function sets the byte string array in the payload.
 *
 * @param payload      Pointer to the payload to which byte string array needs to be added.
 * @param name         Name of the byte string.
 * @param array        Byte string array.
 * @param dimensions   Number of byte strings in above array.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadSetByteStringArray(OCRepPayload* payload, const char* name,
        const OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

/**
 * This function gets the byte string array from the payload.
 *
 * @param payload      Pointer to the payload from which byte string array needs to be retrieved.
 * @param name         Name of the byte string array.
 * @param array        Byte string array.
 * @param dimensions   Number of byte strings in above array.
 *
 * @note: Caller needs to invoke OICFree on 'bytes' field of all array elements after it is
 *        finished using the byte string array.
 *
 * @return true on success, false upon failure.
 */
bool OC_CALL OCRepPayloadGetByteStringArray(const OCRepPayload* payload, const char* name,
        OCByteString** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OC_CALL OCRepPayloadSetIntArrayAsOwner(OCRepPayload* payload, const char* name,
        int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadSetIntArray(OCRepPayload* payload, const char* name,
        const int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadGetIntArray(const OCRepPayload* payload, const char* name,
        int64_t** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OC_CALL OCRepPayloadSetDoubleArrayAsOwner(OCRepPayload* payload, const char* name,
        double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadSetDoubleArray(OCRepPayload* payload, const char* name,
        const double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadGetDoubleArray(const OCRepPayload* payload, const char* name,
        double** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OC_CALL OCRepPayloadSetStringArrayAsOwner(OCRepPayload* payload, const char* name,
        char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadSetStringArray(OCRepPayload* payload, const char* name,
        const char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadGetStringArray(const OCRepPayload* payload, const char* name,
        char*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OC_CALL OCRepPayloadSetBoolArrayAsOwner(OCRepPayload* payload, const char* name,
        bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadSetBoolArray(OCRepPayload* payload, const char* name,
        const bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadGetBoolArray(const OCRepPayload* payload, const char* name,
        bool** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OC_CALL OCRepPayloadSetPropObjectArrayAsOwner(OCRepPayload* payload, const char* name,
        OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadSetPropObjectArray(OCRepPayload* payload, const char* name,
        const OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OC_CALL OCRepPayloadGetPropObjectArray(const OCRepPayload* payload, const char* name,
        OCRepPayload*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

void OC_CALL OCRepPayloadDestroy(OCRepPayload* payload);

// Discovery Payload
OCDiscoveryPayload* OC_CALL OCDiscoveryPayloadCreate();

OCSecurityPayload* OC_CALL OCSecurityPayloadCreate(const uint8_t* securityData, size_t size);
void OC_CALL OCSecurityPayloadDestroy(OCSecurityPayload* payload);

OCIntrospectionPayload* OC_CALL OCIntrospectionPayloadCreateFromCbor(const uint8_t* cborData,
                                                             size_t size);
void OC_CALL OCIntrospectionPayloadDestroy(OCIntrospectionPayload* payload);

#ifndef TCP_ADAPTER
void OC_CALL OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort);
#else
void OC_CALL OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort, uint16_t tcpPort);
#endif
void OC_CALL OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload,
                                      OCResourcePayload* res);
bool OC_CALL OCResourcePayloadAddStringLL(OCStringLL **payload, const char* type);

size_t OC_CALL OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload);
OCResourcePayload* OC_CALL OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload,
                                                 size_t index);

size_t OC_CALL OCEndpointPayloadGetEndpointCount(OCEndpointPayload* payload);
OCEndpointPayload* OC_CALL OCEndpointPayloadGetEndpoint(OCEndpointPayload* payload,
                                                size_t index);

void OC_CALL OCResourcePayloadAddNewEndpoint(OCResourcePayload* payload,
                                     OCEndpointPayload* endpoint);
void OC_CALL OCDiscoveryEndpointDestroy(OCEndpointPayload* payload);
void OC_CALL OCDiscoveryResourceDestroy(OCResourcePayload* payload);
void OC_CALL OCDiscoveryPayloadDestroy(OCDiscoveryPayload* payload);

// Presence Payload
OCPresencePayload* OC_CALL OCPresencePayloadCreate(uint32_t seqNum, uint32_t maxAge,
        OCPresenceTrigger trigger, const char* resourceType);
void OC_CALL OCPresencePayloadDestroy(OCPresencePayload* payload);

// Diagnostic Payload
OCDiagnosticPayload* OC_CALL OCDiagnosticPayloadCreate(const char *message);
void OC_CALL OCDiagnosticPayloadDestroy(OCDiagnosticPayload* payload);

// Helper API
OCStringLL* OC_CALL CloneOCStringLL (OCStringLL* ll);
void OC_CALL OCFreeOCStringLL(OCStringLL* ll);

/**
 * This function creates a list from a string (with separated contents if several)
 * @param text         single string or CSV text fields
 * @return newly allocated linked list
 * @note separator is ',' (according to rfc4180, ';' is not valid)
 **/
OCStringLL* OC_CALL OCCreateOCStringLL(const char* text);

/**
 * This function creates a string from a list (with separated contents if several)
 * @param ll           Pointer to list
 * @return newly allocated string. Caller takes ownership and must later free this memory with OICFree.
 * @note separator is ',' (according to rfc4180)
 **/
char* OC_CALL OCCreateString(const OCStringLL* ll);

/**
 * This function copies contents
 * @param dest existing bytestring.  The existing contents will be OICFree()'d.
 * @param source existing bytestring
 * @return true of success false on any errors
 **/
bool OC_CALL OCByteStringCopy(OCByteString *dest, const OCByteString *source);

/**
 * This function creates the payloadArray for links parameter of collection resource.
 * @param[in] resourceUri Resource uri (this should be a collection resource)
 * @param[in] ehRequest parameter received from Entity Handler for client request
 * @param[in] insertSelfLink flag to specify whether links array can contain a self link
 * @param[out] createdArraySize return value array size, Null is allowed if no need to know size
 *
 * @note: API usage
 *   OCRepPayload **linkArr = OCLinksPayloadArrayCreate(uri, ehRequest, false, &ArraySize);
 *   For links parameter setting  (baseline interface response)
 *    OCRepPayloadSetPropObjectArrayAsOwner(payload, OC_RSRVD_LINKS, linkArr, {ArraySize, 0,0});
 *   For arrayPayload setting (linklist interface response)
 *     payload = linkArr[0]; payload->next = linkArr[1]; ....
 *     OICFree(linkArr)
 * @note: The destroy of OCRepPayloadArray is not supported. Instead, use
 *        OCRepPayloadDestroy(...) to destroy RepPayload of the collection Resource
 *
 * @return linksRepPayloadArray The *RepPayload Array pointer for links parameter of collection.
 **/
OCRepPayload** OC_CALL OCLinksPayloadArrayCreate(const char *resourceUri,
                       OCEntityHandlerRequest *ehRequest, bool insertSelfLink, size_t* createdArraySize);

#ifdef __cplusplus
}
#endif

#endif
