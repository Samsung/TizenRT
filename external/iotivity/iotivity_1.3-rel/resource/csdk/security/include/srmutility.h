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

#ifndef IOTVT_SRM_UTILITY_H
#define IOTVT_SRM_UTILITY_H

#include "ocstack.h"
#include "experimental/securevirtualresourcetypes.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include <coap/uri.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct OicParseQueryIter OicParseQueryIter_t;

/**
 * OicRestQueryIter data structure is used for book-keeping
 * sub-REST query's attribute's and value's, starting location &
 * length between calls to GetNextQuery(). This struct needs
 * to be first initialized with ParseQueryIterInit().
 *
 */
struct OicParseQueryIter
{
    unsigned char * attrPos;    /**< stating location of attribute. */
    size_t attrLen;             /**< length of the attribute. */
    unsigned char * valPos;     /**< starting location of value. */
    size_t valLen;              /**< length of the value. */
    coap_parse_iterator_t pi;   /**< coap struct for tokenizing the query.*/
};

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS(TAG, OC_STACK_OK == foo(), ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#define VERIFY_SUCCESS(tag, op, logLevel) do{ if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); goto exit; } }while(0)

/**
 * Macro to verify expression evaluates to bool true.
 * eg: VERIFY_TRUE_OR_EXIT(TAG, OC_STACK_OK == foo(), ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#define VERIFY_TRUE_OR_EXIT(tag, op, logLevel) do{ if (!(op)) \
            {OIC_LOG_V((logLevel), tag, "%s:" #op "evaluates to false!",__func__); \
            goto exit; } }while(0)

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS_RETURN(TAG, OC_STACK_OK == foo(), ERROR, OC_STACK_ERROR);
 */
#define VERIFY_SUCCESS_RETURN(tag, op, logLevel, retValue) do { if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); return retValue;} } while(0)

/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NOT_NULL(TAG, ptrData, ERROR);
 * @note Invoking function must define "exit:" label for goto functionality to work correctly.
 */
#define VERIFY_NOT_NULL(tag, arg, logLevel) do{ if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); goto exit; } }while(0)

/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NOT_NULL_RETURN(TAG, ptrData, ERROR, OC_STACK_ERROR);
 */
#define VERIFY_NOT_NULL_RETURN(tag, arg, logLevel, retValue) do { if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); return retValue; } } while(0)

/**
 * This method initializes the @ref OicParseQueryIter_t struct.
 *
 * @param query is the REST query, to be parsed.
 * @param parseIter is the @ref OicParseQueryIter_t struct, to be initialized based on the query.
 */
void ParseQueryIterInit(const unsigned char * query, OicParseQueryIter_t * parseIter);

/**
 * This method fills the @ref OicParseQueryIter_t struct with next REST query's
 * attribute's and value's information.
 *
 * @param parseIter is the @ref OicParseQueryIter_t struct, has next query's attribute's
 *  & value's info.
 *
 * @return reference to the @ref OicParseQueryIter_t if it has parsed query info, else
 * NULL if it has no query to parse.
 */
OicParseQueryIter_t * GetNextQuery(OicParseQueryIter_t * parseIter);

/**
 * Function to getting string of ownership transfer method
 *
 * @param oxmType ownership transfer method
 *
 * @return string value of ownership transfer method
 */
const char* GetOxmString(OicSecOxm_t oxmType);

/**
 * This method converts UUID to canonical format string.
 *
 * @param uuid Device UUID
 * @param strUuid converted UUID in canonical format
 * @return OC_STACK_OK for success.
 *
 * @note Caller needs to invoke OICFree after done using the return pointer
 */
OCStackResult ConvertUuidToStr(const OicUuid_t* uuid, char** strUuid);


/**
 * This method converts string UUID to OicUuid_t.
 *
 * @param strUuid Device UUID in string format
 * @param uuid converted UUID in OicUuid_t format
 * @return OC_STACK_OK for success.
 *
 */
OCStackResult OC_CALL ConvertStrToUuid(const char* strUuid, OicUuid_t* uuid);

/**
 * Compares two OicUuid_t structs.
 *
 * @return true if the two OicUuid_t structs are equal, else false.
 */
bool UuidCmp(const OicUuid_t *firstId, const OicUuid_t *secondId);

extern const OicUuid_t THE_NIL_UUID;

/**
 * OicUuid_t to Nil UUID {.id={0000000000000000}}
 *
 * @return true if the OicUuid_t is the Nil UUID
 */
bool IsNilUuid(const OicUuid_t *uuid);

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
/**
 * API to save the seed value to generate device UUID.
 * Seed value MUST be unique per device (e.g. MAC address)
 *
 * @param seed buffer of seed value.
 * @param seedSize byte length of seed
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult OC_CALL SetDeviceIdSeed(const uint8_t* seed, size_t seedSize);
#endif

/**
 * Log OicUuid_t structs.
 */
void LogUuid(const OicUuid_t* uuid);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //IOTVT_SRM_UTILITY_H
