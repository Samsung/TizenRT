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
 *
 * This file contains common utility functions to manage the CA msg
 * fragmentation and reassemebly.
 */


#ifndef CA_FRAGMENTATION_H_
#define CA_FRAGMENTATION_H_

#include "cacommon.h"
#include "logger.h"

/**
 * The maximum port value for BLE packet format
 */
#define CA_SUPPORTED_BLE_MAX_PORT  127

/**
 * The minimum port value for BLE packet format
 */
#define CA_SUPPORTED_BLE_MIN_PORT  1

/**
 * The multicaset packet remote port value
 */
#define CA_BLE_MULTICAST_PORT  0

/**
 * The header size for ble fragmentation.
 * Specific header descriptions are below.
 */
#define CA_BLE_HEADER_SIZE 2

/**
 * The length header size for ble fragmentation.
 * Length header is embedded in first packet of entire CoAP PDU.
 */
#define CA_BLE_LENGTH_HEADER_SIZE 4

/**
 * Current Header version.
 */
#define HEADER_VERSION 1

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * This enum value is used to make the CA BLE packet header.
 * 1st bit is used to check whether the packet is start packet or not.
 * Start packet should be marked ad CA_BLE_PACKET_START(1) and any other
 * packet is marked as CA_BLE_PACKET_NOT_START(0).
 */
typedef enum {
    CA_BLE_PACKET_NOT_START     = 0,
    CA_BLE_PACKET_START         = 1
} CABLEPacketStart_t;

/**
 * This enum value is used to make the CA BLE packet header.
 * 9th bit is uesd to check the packet use secure logic(dtls) or not.
 * Secure packet should be marking CA_BLE_PACKET_SECURE(1) and other
 * packet is makred CA_BLE_PACKET_NON_SECURE(0).
 */
typedef enum {
    CA_BLE_PACKET_NON_SECURE    = 0,
    CA_BLE_PACKET_SECURE        = 1
} CABLEPacketSecure_t;


/*****************************************************************
 * @file The CA Header format
 * CA Header will be defined by 2 bytes of Header.
 * First one bit : Header type that is start packet or not.
 * 2nd to 8th bit : Own unique port value.
 * 9th bit: Secure type using dtls(1) or not(0).
 * 10th to 16th bit : Remote endpoint unique port value.
 *
 * Start packet has additional 4 bytes size length header which
 * represent total packet size.
 *****************************************************************/

/**
 * This function is used to generate the CA BLE variable related to
 * maintain the fragmentation logic. The variable is used in BLE send routine.
 *
 * @param[in]   dataLength      Original packet size about data to send.
 * @param[out]  midPacketCount  Number of mid packet except first and last
 *                              packet.
 * @param[out]  remainingLen    Size of last packet before adding header.
 * @param[out]  totalLengh      The total length of the data.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAGenerateVariableForFragmentation(size_t dataLength,
                                              uint32_t *midPacketCount,
                                              size_t *remainingLen,
                                              size_t *totalLength,
                                              uint16_t mtuSize);

/**
 * This function is used to generate the CA BLE header to
 * maintain the fragmentation logic. The header structure explained
 * above will be formed and returned to the caller.
 *
 * @param[in,out] header       Pointer to the octet array that will
 *                             contain the generated header.
 * @param[in]     type         Enum value to check start packet or not.
 *                             it will be embedded in bit 1 of the header
 * @param[in]     sourcePort   Source(own) port of the unique(in device)
 *                             value. it will be embedded in bits 2~8 of
 *                             the header.
 * @param[in]     secure       Enum value to check whether secure or not.
 *                             it will be embedded in bit 9 of the header.
 * @param[in]     destPort     Destination(remote endpoint) port of the
 *                             unique(in device) value. it will be embedded
 *                             in bits 10~16 of the header.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAGenerateHeader(uint8_t *header,
                            CABLEPacketStart_t type,
                            const uint8_t sourcePort,
                            CABLEPacketSecure_t secure,
                            const uint8_t destPort);

/**
 * This function is used to generate the CA BLE length header to
 * maintain the fragmentation logic. The header structure explained
 * above will be formed and returned to the caller.
 *
 * @param[in,out] header       Pointer to the octet array that will
 *                             contain the generated length header.
 * @param[in]     headerLength Length about header array. it should be
 *                             same as CA_BLE_LENGTH_HEADER_SIZE.
 * @param[in]     dataLength   The total length of data size. it will
 *                             be embedded in 4 bytes of the length header.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAGenerateHeaderPayloadLength(uint8_t *header,
                                         size_t headerLength,
                                         size_t dataLength);

/**
 * This function is used to make the CA BLE first data segment to
 * maintain the fragmentation logic. start data segment is included
 * 2 bytes header, 4 bytes length header and transmit data.
 *
 * @param[out]  dataSegment    Pointer to the octet array that will
 *                             contain the generated data packet.
 * @param[in]   data           Data to the octet array that required
 *                             transmittin to remote device. it will
 *                             be embedded in 7th byte to data length.
 * @param[in]   dataLength     The length of data size.
 * @param[in]   dataHeader     Pointer to the octet array that contain
 *                             data header.
 * @param[in]   lengthHeader   Pointer to the octet array that contain
 *                             length header.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAMakeFirstDataSegment(uint8_t *dataSegment,
                                  const uint8_t *data,
                                  const uint32_t dataLength,
                                  const uint8_t *dataHeader,
                                  const uint8_t *lengthHeader);

/**
 * This function is used to make the CA BLE second to end data segment
 * to maintain the fragmentation logic. start data segment is included
 * 2 bytes header and transmit data.
 *
 * @param[out]  dataSegment            Pointer to the octet array that will
 *                                     contain the generated data packet.
 * @param[in]   segmentPayloadLength   The length of data segment payload.
 * @param[in]   sourceData             Data to the octet array that required
 *                                     transmission to remote device. it will
 *                                     be embedded in 7th byte to data length.
 * @param[in]   sourceDataLength       The length of total data.
 * @param[in]   segmentNum             Index to determine whether some of the
 *                                     total data
 * @param[in]   dataHeader             Pointer to the octet array that contain
 *                                     data header.
 * @param[in]   mtuSize                MTU size.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAMakeRemainDataSegment(uint8_t *dataSegment,
                                   const uint32_t segmentPayloadLength,
                                   const uint8_t *sourceData,
                                   const uint32_t sourceDataLength,
                                   const uint32_t segmentNum,
                                   const uint8_t *dataHeader,
                                   uint16_t mtuSize);

/**
 * This function is used to parse the header in the receiver end. This
 * function will provide the information of the type of the packet, source(remote) /
 * destination port info and secure infomation.
 *
 * @param[in]   header      Pointer to the octet array data which contains
 *                          the header information.  Note that pointer should
 *                          point to two bytes of data header which needs to
 *                          be parsed.
 * @param[out]  type        Enum value to check start packet or not.
 *                          it will be embedded in bit 1 of the header
 * @param[out]  sourcePort  Source(own) port of the unique(in device)
 *                          value. it will be embedded in bits 2~8 of
 *                          the header.
 * @param[out]  secure      Enum value to check whether secure or not.
 *                          it will be embedded in bit 9 of the header.
 * @param[out]  destPort    Destination(remote endpoint) port of the
 *                          unique(in device) value. it will be embedded
 *                          in bits 10~16 of the header.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAParseHeader(const uint8_t *header,
                         CABLEPacketStart_t *startFlag,
                         uint16_t *sourcePort,
                         CABLEPacketSecure_t *secureFlag,
                         uint16_t *destPort);

/**
 * This function is used to parse the length header in the receiver end. This
 * function will provide the total data length about defragmented data.
 *
 * @param[in]   header        Pointer to the octet array that will
 *                            contain length infomation.
 * @param[out]  headerLength  Length about header array. it should be
 *                            same as CA_BLE_LENGTH_HEADER_SIZE.
 * @param[out]  dataLength    The total length of data size. it will
 *                            be embedded in 4 bytes of the length header.
 *
 * @return ::CA_STATUS_OK on success. One of the CA_STATUS_FAILED
 *           or other error values on error.
 * @retval ::CA_STATUS_OK             Successful
 * @retval ::CA_STATUS_INVALID_PARAM  Invalid input arguments
 * @retval ::CA_STATUS_FAILED         Operation failed
 */
CAResult_t CAParseHeaderPayloadLength(uint8_t *header,
                                      size_t headerLength,
                                      uint32_t *dataLength);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* CA_FRAGMENTATION_H_ */
