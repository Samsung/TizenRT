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

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)
#include "securevirtualresourcetypes.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Macro to verify the validity of cbor operation.
 */
#define VERIFY_CBOR_SUCCESS(log_tag, err, log_message) \
    if ((CborNoError != (CborError)(err)) && (CborErrorOutOfMemory != (CborError)(err))) \
    { \
        if ((log_tag) && (log_message)) \
        { \
            OIC_LOG_V(ERROR, (log_tag), "%s with cbor error: \'%s\'.", \
                    (log_message), (cbor_error_string((CborError)err))); \
        } \
        goto exit; \
    } \

#define VERIFY_PARAM_NON_NULL(log_tag, err, log_message) \
    if (NULL == (err)) \
    { \
        OIC_LOG_V(FATAL, (log_tag), "%s", (log_message)); \
        goto exit;\
    } \


typedef struct OCResource OCResource;

void OCPayloadDestroy(OCPayload* payload);

// Representation Payload
OCRepPayload* OCRepPayloadCreate();

size_t calcDimTotal(const size_t dimensions[MAX_REP_ARRAY_DEPTH]);

OCRepPayload* OCRepPayloadClone(const OCRepPayload* payload);

OCRepPayload* OCRepPayloadBatchClone(const OCRepPayload* repPayload);

void OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child);

bool OCRepPayloadSetUri(OCRepPayload* payload, const char* uri);

bool OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType);
bool OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface);
bool OCRepPayloadAddModelVersion(OCRepPayload* payload, const char* dmv);

bool OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType);
bool OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface);

bool OCRepPayloadIsNull(const OCRepPayload* payload, const char* name);
bool OCRepPayloadSetNull(OCRepPayload* payload, const char* name);

bool OCRepPayloadSetPropInt(OCRepPayload* payload, const char* name, int64_t value);
bool OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value);

bool OCRepPayloadSetPropDouble(OCRepPayload* payload, const char* name, double value);
bool OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value);

/**
 * This function allocates memory for the byte string and sets it in the payload.
 *
 * @param payload      Pointer to the payload to which byte string needs to be added.
 * @param name         Name of the byte string.
 * @param value        Byte string and it's length.
 *
 * @return true on success, false upon failure.
 */
bool OCRepPayloadSetPropByteString(OCRepPayload* payload, const char* name, OCByteString value);

/**
 * This function sets the byte string in the payload.
 *
 * @param payload      Pointer to the payload to which byte string needs to be added.
 * @param name         Name of the byte string.
 * @param value        Byte string and it's length.
 *
 * @return true on success, false upon failure.
 */
bool OCRepPayloadSetPropByteStringAsOwner(OCRepPayload* payload, const char* name,
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
bool OCRepPayloadGetPropByteString(const OCRepPayload* payload, const char* name,
        OCByteString* value);

bool OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value);
bool OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value);
bool OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value);

bool OCRepPayloadSetPropBool(OCRepPayload* payload, const char* name, bool value);
bool OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value);

bool OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value);
bool OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name, OCRepPayload* value);
bool OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value);

#ifdef __WITH_TLS__
bool OCRepPayloadSetPropPubDataType(OCRepPayload *payload, const char *name, const OicSecKey_t *value);
bool OCRepPayloadSetPropPubDataTypeAsOwner(OCRepPayload *payload, const char *name, const OicSecKey_t *value);
bool OCRepPayloadGetPropPubDataType(const OCRepPayload *payload, const char *name, OicSecKey_t *value);
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
bool OCRepPayloadSetByteStringArrayAsOwner(OCRepPayload* payload, const char* name,
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
bool OCRepPayloadSetByteStringArray(OCRepPayload* payload, const char* name,
        const OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

/**
 * This function gets the byte string array from the payload.
 *
 * @param payload      Pointer to the payload from which byte string array needs to be retrieved.
 * @param name         Name of the byte string array.
 * @param value        Byte string array.
 * @param dimensions   Number of byte strings in above array.
 *
 * @note: Caller needs to invoke OICFree on 'bytes' field of all array elements after it is
 *        finished using the byte string array.
 *
 * @return true on success, false upon failure.
 */
bool OCRepPayloadGetByteStringArray(const OCRepPayload* payload, const char* name,
        OCByteString** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OCRepPayloadSetIntArrayAsOwner(OCRepPayload* payload, const char* name,
        int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadSetIntArray(OCRepPayload* payload, const char* name,
        const int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadGetIntArray(const OCRepPayload* payload, const char* name,
        int64_t** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OCRepPayloadSetDoubleArrayAsOwner(OCRepPayload* payload, const char* name,
        double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadSetDoubleArray(OCRepPayload* payload, const char* name,
        const double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadGetDoubleArray(const OCRepPayload* payload, const char* name,
        double** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OCRepPayloadSetStringArrayAsOwner(OCRepPayload* payload, const char* name,
        char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadSetStringArray(OCRepPayload* payload, const char* name,
        const char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadGetStringArray(const OCRepPayload* payload, const char* name,
        char*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OCRepPayloadSetBoolArrayAsOwner(OCRepPayload* payload, const char* name,
        bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadSetBoolArray(OCRepPayload* payload, const char* name,
        const bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadGetBoolArray(const OCRepPayload* payload, const char* name,
        bool** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

bool OCRepPayloadSetPropObjectArrayAsOwner(OCRepPayload* payload, const char* name,
        OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadSetPropObjectArray(OCRepPayload* payload, const char* name,
        const OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);
bool OCRepPayloadGetPropObjectArray(const OCRepPayload* payload, const char* name,
        OCRepPayload*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH]);

void OCRepPayloadDestroy(OCRepPayload* payload);

// Discovery Payload
OCDiscoveryPayload* OCDiscoveryPayloadCreate();

OCSecurityPayload* OCSecurityPayloadCreate(const uint8_t* securityData, size_t size);
void OCSecurityPayloadDestroy(OCSecurityPayload* payload);

#if !defined(TCP_ADAPTER) || defined(DISABLE_TCP_SERVER)
void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort);
#else
void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort, uint16_t tcpPort);
#endif
void OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload, OCResourcePayload* res);
bool OCResourcePayloadAddStringLL(OCStringLL **payload, const char* type);

size_t OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload);
OCResourcePayload* OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload, size_t index);

void OCDiscoveryResourceDestroy(OCResourcePayload* payload);
void OCDiscoveryPayloadDestroy(OCDiscoveryPayload* payload);

// Presence Payload
OCPresencePayload* OCPresencePayloadCreate(uint32_t seqNum, uint32_t maxAge,
        OCPresenceTrigger trigger, const char* resourceType);
void OCPresencePayloadDestroy(OCPresencePayload* payload);

// Helper API
OCStringLL* CloneOCStringLL (OCStringLL* ll);
void OCFreeOCStringLL(OCStringLL* ll);

/**
 * This function creates a list from a string (with separated contents if several)
 * @param text         single string or CSV text fields
 * @return newly allocated linked list
 * @note separator is ',' (according to rfc4180, ';' is not valid)
 **/
OCStringLL* OCCreateOCStringLL(const char* text);

/**
 * This function creates a string from a list (with separated contents if several)
 * @param ll           Pointer to list
 * @return newly allocated string. Caller takes ownership and must later free this memory with OICFree.
 * @note separator is ',' (according to rfc4180)
 **/
char* OCCreateString(const OCStringLL* ll);

/**
 * This function copies contents (and allocates if necessary)
 * @param dest existing bytestring (or null to allocate here)
 * @param source existing bytestring
 * @return true of success false on any errors
 **/
bool OCByteStringCopy(OCByteString *dest, const OCByteString *source);

#ifdef __cplusplus
}
#endif

#endif
