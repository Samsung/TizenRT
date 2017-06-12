/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/**
 * @file
 * This file contains HTTP parsing functions
 */

#ifndef COAP_HTTP_PARSER_H_
#define COAP_HTTP_PARSER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <unistd.h>
#include "uarraylist.h"
#include "octypes.h"

#define CHP_MAX_HF_DATA_LENGTH 1024
#define CHP_MAX_HF_NAME_LENGTH 255
#define JSON_CONTENT_TYPE "application/json"
#define CBOR_CONTENT_TYPE "application/cbor"
#define ACCEPT_MEDIA_TYPE (CBOR_CONTENT_TYPE "; q=1.0, " JSON_CONTENT_TYPE "; q=0.5")

// HTTP Option types
#define HTTP_OPTION_CACHE_CONTROL   "cache-control"
#define HTTP_OPTION_ACCEPT          "accept"
#define HTTP_OPTION_IF_MATCH        "if-match"
#define HTTP_OPTION_IF_NONE_MATCH   "if-none-match"
#define HTTP_OPTION_ETAG            "etag"
#define HTTP_OPTION_CONTENT_TYPE    "content-type"
#define HTTP_OPTION_CONTENT_LENGTH  "content-length"
#define HTTP_OPTION_EXPIRES         "expires"

/**
 * @enum HttpResponseResult_t
 * Enums for HTTP Response values
 */
typedef enum
{
    /* Response status code - START HERE */
    CHP_EMPTY = 0,                       /**< Empty */
    CHP_SUCCESS = 200,                   /**< Success */
    CHP_CREATED = 201,                   /**< Created */
    CHP_ACCEPTED = 202,                  /**< Accepted */
    CHP_NO_CONTENT = 204,                /**< No Content */
    CHP_RESET_CONTENT = 205,             /**< Reset content */
    CHP_NOT_MODIFIED = 304,              /**< Not Modified */
    CHP_BAD_REQ = 400,                   /**< Bad Request */
    CHP_UNAUTHORIZED_REQ = 401,          /**< Unauthorized Request */
    CHP_FORBIDDEN_REQ = 403,             /**< Forbidden Request */
    CHP_NOT_FOUND = 404,                 /**< Not found */
    CHP_NOT_ACCEPTABLE = 406,            /**< Not Acceptable */
    CHP_REQUEST_ENTITY_TOO_LARGE = 413,  /**< Request Entity Too Large */
    CHP_REQUEST_URI_TOO_LARGE = 414,     /**< Request URI Too Large */
    CHP_UNSUPPORTED_MEDIA_TYPE = 415,    /**< Unsupported Media type */
    CHP_INTERNAL_SERVER_ERROR = 500,     /**< Internal server Error */
    CHP_NOT_IMPLEMENTED = 501,           /**< Not Implemented */
    CHP_BAD_GATEWAY = 502,               /**< Bad Gateway */
    CHP_SERVICE_UNAVAILABLE = 503,       /**< Service Unavailable */
    CHP_GATEWAY_TIMEOUT = 504,           /**< Gateway Timeout */
    CHP_VERSION_NOT_SUPPORTED = 505      /**< Version not supported */
    /* Response status code - END HERE */
} HttpResponseResult_t;

/**
 * Header fields structure to be filled
 *
 * This structure is used to hold header information.
 */
typedef struct
{
    uint16_t optionLength;                                  /**< Option length. **/
    char optionName[CHP_MAX_HF_NAME_LENGTH];                /**< Option name. **/
    char optionData[CHP_MAX_HF_DATA_LENGTH];                /**< Option data values. **/
} HttpHeaderOption_t;

typedef enum
{
    CHP_GET = 1,   /**< GET */
    CHP_POST,      /**< POST */
    CHP_PUT,       /**< PUT */
    CHP_DELETE,    /**< DELETE */
    CHP_INVALID
}HttpMethod_t;

typedef struct HttpRequest_t
{
    unsigned short httpMajor;
    unsigned short httpMinor;
    HttpMethod_t method;
    u_arraylist_t *headerOptions;
    char resourceUri[CHP_MAX_HF_DATA_LENGTH];
    void *payload;
    size_t payloadLength;
    bool payloadCached;
    char payloadFormat[CHP_MAX_HF_DATA_LENGTH];
    char acceptFormat[CHP_MAX_HF_DATA_LENGTH];
}HttpRequest_t;

typedef struct HttpResponse_t
{
    unsigned short httpMajor;
    unsigned short httpMinor;
    HttpResponseResult_t status;
    u_arraylist_t *headerOptions;
    char dataFormat[CHP_MAX_HF_DATA_LENGTH];
    void *payload;
    size_t payloadLength;
}HttpResponse_t;

typedef void (*CHPResponseCallback)(const HttpResponse_t *response, void *context);

/**
 * Function to initialize Parser and HTTP stack.
 */
OCStackResult CHPParserInitialize();

/**
 * Function to terminate parser and HTTP stack.
 */
OCStackResult CHPParserTerminate();

/**
 * Function to initiate TCP session and post HTTP request. If the method returns
 * success, payload might be cached by the parser (req->payloadCached) and caller shall not free the
 * payload if the flag is set.
 * @param[in]   req         Object containing HTTP request information.
 * @param[in]   httpcb      Callback for http response.
 * @param[in]   context     Any app specific context for request
 */
OCStackResult CHPPostHttpRequest(HttpRequest_t *req, CHPResponseCallback httpcb,
                                 void *context);

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 * @param  ret  return value
 */
#define VERIFY_NON_NULL_RET(arg, log_tag, log_message, ret) \
    if (NULL == (arg)) \
    { \
        OIC_LOG_V(ERROR, (log_tag), "Invalid input:%s", (log_message)); \
        return (ret); \
    } \

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 */
#define VERIFY_NON_NULL(arg, log_tag, log_message) \
    VERIFY_NON_NULL_RET((arg), (log_tag), (log_message), CA_STATUS_INVALID_PARAM)

/**
 * Macro to verify the validity of input argument.
 *
 * @param  arg  log level
 * @param  log_tag  log tag
 * @param  log_message  log message
 */
#define VERIFY_NON_NULL_VOID(arg, log_tag, log_message) \
    if (NULL == (arg)) { \
        OIC_LOG_V(ERROR, (log_tag), "Invalid input:%s", (log_message)); \
        return; \
    } \

#ifdef __cplusplus
}
#endif
#endif
