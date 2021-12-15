/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#define BLE_ERR_UNKNOWN_CMD                     0x01

/* Connection does not exist, or connection open request was cancelled. */
#define BLE_ERR_UNKNOWN_CONN_ID                 0x02

#define BLE_ERR_HARDWARE_FAIL                   0x03
#define BLE_ERR_PAGE_TIMEOUT                    0x04

/* Pairing or authentication failed due to incorrect results in the pairing or authentication procedure. 
 * This could be due to an incorrect PIN or Link Key */
#define BLE_ERR_AUTHEN_FAIL                     0x05

/* Pairing failed because of missing PIN, or authentication failed because of missing Key */
#define BLE_ERR_KEY_MISSING                     0x06

/* memory_capacity_exceeded	Controller is out of memory. */
#define BLE_ERR_MEMORY_FULL                     0x07

/* Link supervision timeout has expired. */
#define BLE_ERR_CONN_TIMEOUT                    0x08

/* Controller is at limit of connections it can support. */
#define BLE_ERR_MAX_NUM_CONN                    0x09

/* The Synchronous Connection Limit to a Device Exceeded error code indicates 
 * that the Controller has reached the limit to the number of synchronous connections that can be achieved to a device. */
#define BLE_ERR_MAX_NUM_SCO_CONN                0x0A  /* 10 */

/* The ACL Connection Already Exists error code indicates 
 * that an attempt to create a new ACL Connection to a device when there is already a connection to this device. */
#define BLE_ERR_ACL_CONN_EXIST                  0x0B  /* 11 */

/* Command requested cannot be executed because the Controller is in a state 
 * where it cannot process this command at this time. */
#define BLE_ERR_CMD_DISALLOWED                  0x0C  /* 12 */

/* The Connection Rejected Due To Limited Resources error code indicates that an incoming connection was rejected due to limited resources. */
#define BLE_ERR_REJECT_LIMITED_RESOURCE         0x0D  /* 13 */

/* The Connection Rejected Due To Security Reasons error code indicates 
 * that a connection was rejected due to security requirements not being fulfilled, like authentication or pairing. */
#define BLE_ERR_REJECT_SECURITY_REASON          0x0E  /* 14 */

/* The Connection was rejected because this device does not accept the BD_ADDR. 
 * This may be because the device will only accept connections from specific BD_ADDRs. */
#define BLE_ERR_REJECT_UNACCEPTABLE_ADDR        0x0F  /* 15 */

/* The Connection Accept Timeout has been exceeded for this connection attempt. */
#define BLE_ERR_HOST_TIMEOUT                    0x10  /* 16 */

/* A feature or parameter value in the HCI command is not supported. */
#define BLE_ERR_UNSUPPORTED_PARAM               0x11  /* 17 */

/* Command contained invalid parameters. */
#define BLE_ERR_INVALID_PARAM                   0x12  /* 18 */

/* User on the remote device terminated the connection. */
#define BLE_ERR_REMOTE_USER_TERMINATE           0x13  /* 19 */

/* The remote device terminated the connection because of low resources */
#define BLE_ERR_REMOTE_LOW_RESOURCE             0x14  /* 20 */

/* Remote Device Terminated Connection due to Power Off */
#define BLE_ERR_REMOTE_POWER_OFF                0x15  /* 21 */

/* Local device terminated the connection. */
#define BLE_ERR_LOCAL_HOST_TERMINATE            0x16  /* 22 */

/* The Controller is disallowing an authentication or pairing procedure 
 * because too little time has elapsed since the last authentication or pairing attempt failed. */
#define BLE_ERR_REPEATED_ATTEMPTS               0x17  /* 23 */

/* The device does not allow pairing. 
 * This can be for example, when a device only allows pairing during a certain time window after some user input allows pairing */
#define BLE_ERR_PARING_NOT_ALLOWED              0x18  /* 24 */

#define BLE_ERR_UNKNOWN_LMP_PDU                 0x19  /* 25 */

/* The remote device does not support the feature associated with the issued command. */
#define BLE_ERR_UNSUPPORTED_REMOTE_FEAT         0x1A  /* 26 */

#define BLE_ERR_SCO_OFFSET_REJECTED             0x1B  /* 27 */
#define BLE_ERR_SCO_INTERVAL_REJECTED           0x1C  /* 28 */
#define BLE_ERR_SCO_AIR_MODE_REJECTED           0x1D  /* 29 */
#define BLE_ERR_INVALID_LMP_PARAM               0x1E  /* 30 */

/* No other error code specified is appropriate to use. */
#define BLE_ERR_UNSPECIFIED_ERROR               0x1F  /* 31 */

#define BLE_ERR_UNSUPPORTED_LMP_PARAM           0x20  /* 32 */
#define BLE_ERR_ROLE_CHANGE_NOT_ALLOWED         0x21  /* 33 */

/* Connection terminated due to link-layer procedure timeout. */
#define BLE_ERR_LMP_RESPONSE_TIMEOUT            0x22  /* 34 */

/* LL procedure has collided with the same transaction or procedure that is already in progress. */
#define BLE_ERR_LMP_ERROR_TRANS_COLLISION       0x23  /* 35 */

#define BLE_ERR_LMP_PDU_NOT_ALLOWED             0x24  /* 36 */

/* The requested encryption mode is not acceptable at this time. */
#define BLE_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE     0x25  /* 37 */

/* Link key cannot be changed because a fixed unit key is being used. */
#define BLE_ERR_UNIT_KEY_USED                   0x26  /* 38 */

#define BLE_ERR_QOS_NOT_SUPPORTED               0x27  /* 39 */

/* LMP PDU or LL PDU that includes an instant cannot be performed because the instant when this would have occurred has passed. */
#define BLE_ERR_INSTANT_PASSED                  0x28  /* 40 */

/* It was not possible to pair as a unit key was requested and it is not supported. */
#define BLE_ERR_PAIR_UNIT_KEY_NOT_SUPPORT       0x29  /* 41 */

/* LMP transaction was started that collides with an ongoing transaction. */
#define BLE_ERR_DIFF_TRANS_COLLISION            0x2A  /* 42 */

#define BLE_ERR_QOS_UNACCEPTABLE_PARAM          0x2C  /* 44 */
#define BLE_ERR_QOS_REJECT                      0x2D  /* 45 */

/* The Controller cannot perform channel assessment because it is not supported. */
#define BLE_ERR_CHANN_ASSESS_NOT_SUPPORT        0x2E  /* 46 */

/* The HCI command or LMP PDU sent is only possible on an encrypted link. */
#define BLE_ERR_INSUFFICIENT_SECURITY           0x2F  /* 47 */

/* A parameter value requested is outside the mandatory range of parameters for the given HCI command or LMP PDU. */
#define BLE_ERR_PARAM_OUT_OF_RANGE              0x30  /* 48 */

#define BLE_ERR_ROLE_SWITCH_PANDING             0x32  /* 50 */
#define BLE_ERR_RESERVED_SLOT_VIOLATION         0x34  /* 52 */
#define BLE_ERR_ROLE_SWITCH_FAILED              0x35  /* 53 */
#define BLE_ERR_EXT_INQUIRY_RSP_TOO_LARGE       0x36  /* 54 */

/* The IO capabilities request or response was rejected because the sending Host does not support Secure Simple Pairing even though the receiving Link Manager does. */
#define BLE_ERR_SSP_NOT_SUPPORTED_BY_HOST       0x37  /* 55 */

/* The Host is busy with another pairing operation and unable to support the requested pairing. The receiving device should retry pairing again later. */
#define BLE_ERR_HOST_BUSY_PAIRING               0x38  /* 56 */

/* The Controller could not calculate an appropriate value for the Channel selection operation. */
#define BLE_ERR_REJECT_NO_SUITABLE_CHANN        0x39  /* 57 */

/* Operation was rejected because the controller is busy and unable to process the request. */
#define BLE_ERR_CONTROLLER_BUSY                 0x3A  /* 58 */

/* Remote device terminated the connection because of an unacceptable connection interval. */
#define BLE_ERR_UNACCEPTABLE_CONN_INTERVAL      0x3B  /* 59 */

/* Advertising for a fixed duration completed or, for directed advertising, that advertising completed without a connection being created. */
#define BLE_ERR_DIRECTED_ADV_TIMEOUT            0x3C  /* 60 */

/* Connection was terminated because the Message Integrity Check (MIC) failed on a received packet. */
#define BLE_ERR_MIC_FAILURE                     0x3D  /* 61 */

/* LL initiated a connection but the connection has failed to be established. Controller did not receive any packets from remote end. */
#define BLE_ERR_FAIL_TO_ESTABLISH_CONN          0x3E  /* 62 */

/* The MAC of the 802.11 AMP was requested to connect to a peer, but the connection failed. */
#define BLE_ERR_MAC_CONN_FAIL                   0x3F  /* 63 */

/* The master, at this time, is unable to make a coarse adjustment to the piconet clock, using the supplied parameters. Instead the master will attempt to move the clock using clock dragging. */
#define BLE_ERR_COARSE_CLOCK_ADJUST_REJECTED    0x40

#define BLE_ERR_TYPE0_SUBMAP_NOT_DEFINED        0x41

/* A command was sent from the Host that should identify an Advertising or Sync handle, but the Advertising or Sync handle does not exist. */
#define BLE_ERR_UNKNOWN_ADVERTISING_IDENTIFIER  0x42

/* Number of operations requested has been reached and has indicated the completion of the activity (e.g., advertising or scanning). */
#define BLE_ERR_LIMIT_REACHED                   0x43

/* A request to the Controller issued by the Host and still pending was successfully canceled. */
#define BLE_ERR_OPERATION_CANCELLED_BY_HOST     0x44

/* An attempt was made to send or receive a packet that exceeds the maximum allowed packet length. */
#define BLE_ERR_PACKET_TOO_LONG                 0x45
