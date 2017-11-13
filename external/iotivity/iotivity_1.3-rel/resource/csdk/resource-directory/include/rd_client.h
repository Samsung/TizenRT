//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#ifndef _RESOURCE_DIRECTORY_CLIENT_H_
#define _RESOURCE_DIRECTORY_CLIENT_H_

// Iotivity Base CAPI
#include "ocstack.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef RD_CLIENT

/** Default TTL to use when publishing resources. */
#define OIC_RD_PUBLISH_TTL 86400

/** JSON media type. */
#define OC_MEDIA_TYPE_APPLICATION_JSON   "application/json"

/**
 * Discover Local RD across the network.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param connectivityType  Type of connectivity indicating the interface.
 * @param cbBiasFactor      Asynchronous callback function that is invoked by the stack when
 *                          response is received. The callback is generated for each response
 *                          received.
 * @param qos               Quality of service.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCRDDiscover(OCDoHandle *handle, OCConnectivityType connectivityType,
                           OCCallbackData *cbBiasFactor, OCQualityOfService qos);

/**
 * Publish RD resource to Resource Directory.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param host              The address of the RD.
 * @param connectivityType  Type of connectivity indicating the interface.
 * @param resourceHandles   This is the resource handle which we need to register to RD.
 * @param nHandles          The counts of resource handle.
 * @param ttl               The time to live, in seconds.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          response is received. The callback is generated for each response
 *                          received.
 * @param qos               Quality of service.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCRDPublish(OCDoHandle *handle, const char *host,
                          OCConnectivityType connectivityType,
                          OCResourceHandle *resourceHandles, uint8_t nHandles, uint32_t ttl,
                          OCCallbackData *cbData, OCQualityOfService qos);

/**
 * Publish RD resource to Resource Directory with a specific id.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param host              The address of the RD.
 * @param id                An unique identifier of publishing device.
 * @param connectivityType  Type of connectivity indicating the interface.
 * @param resourceHandles   This is the resource handle which we need to register to RD.
 * @param nHandles          The counts of resource handle.
 * @param ttl               The time to live, in seconds.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          response is received. The callback is generated for each response
 *                          received.
 * @param qos               Quality of service.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCRDPublishWithDeviceId(OCDoHandle *handle, const char *host,
                                      const unsigned char *id,
                                      OCConnectivityType connectivityType,
                                      OCResourceHandle *resourceHandles, uint8_t nHandles, uint32_t ttl,
                                      OCCallbackData *cbData, OCQualityOfService qos);

/**
 * Delete RD resource from Resource Directory.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param host              The address of the RD.
 * @param connectivityType  Type of connectivity indicating the interface.
 * @param resourceHandles   This is the resource handle which we need to delete to RD.
 * @param nHandles          The counts of resource handle.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          response is received. The callback is generated for each response
 *                          received.
 * @param qos               Quality of service.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult OC_CALL OCRDDelete(OCDoHandle *handle, const char *host,
                         OCConnectivityType connectivityType,
                         OCResourceHandle *resourceHandles, uint8_t nHandles,
                         OCCallbackData *cbData, OCQualityOfService qos);

/**
 * Delete RD resource from Resource Directory.
 *
 * @param handle            To refer to the request sent out on behalf of
 *                          calling this API. This handle can be used to cancel this operation
 *                          via the OCCancel API.
 *                          @note: This reference is handled internally, and should not be free'd by
 *                          the consumer.  A NULL handle is permitted in the event where the caller
 *                          has no use for the return value.
 * @param host              The address of the RD.
 * @param id                An unique identifier of publishing device.
 * @param connectivityType  Type of connectivity indicating the interface.
 * @param resourceHandles   This is the resource handle which we need to delete to RD.
 * @param nHandles          The counts of resource handle.
 * @param cbData            Asynchronous callback function that is invoked by the stack when
 *                          response is received. The callback is generated for each response
 *                          received.
 * @param qos               Quality of service.
 *
 * @return
 *   - ::OC_STACK_OK on success
 *   - ::OC_STACK_INVALID_CALLBACK host, id, or cbData has not been provided
 *   - ::OC_STACK_INVALID_URI generated URI exceeds MAX_URI_LENGTH try fewer resourceHandles
 *   - some other value upon failure.
 */
OCStackResult OC_CALL OCRDDeleteWithDeviceId(OCDoHandle *handle, const char *host,
                                     const unsigned char *id,
                                     OCConnectivityType connectivityType,
                                     OCResourceHandle *resourceHandles, uint8_t nHandles,
                                     OCCallbackData *cbData, OCQualityOfService qos);

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_RESOURCE_DIRECTORY_CLIENT_H_
