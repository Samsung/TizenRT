/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
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

#ifndef INCLUDED_WWD_CONSTANTS_H_
#define INCLUDED_WWD_CONSTANTS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RESULT_ENUM
#define RESULT_ENUM( prefix, name, value )  prefix ## name = (value)
#endif							/* ifndef RESULT_ENUM */

/* These Enum result values are for WWD errors
 * Values: 1000 - 1999
 */

#define WWD_RESULT_LIST( prefix )  \
	RESULT_ENUM( prefix, SUCCESS,                         0 ),   /**< Success */                           \
	RESULT_ENUM( prefix, PENDING,                         1 ),   /**< Pending */                           \
	RESULT_ENUM( prefix, TIMEOUT,                         2 ),   /**< Timeout */                           \
	RESULT_ENUM( prefix, BADARG,                          5 ),   /**< Bad Arguments */                     \
	RESULT_ENUM( prefix, PARTIAL_RESULTS,              1003 ),   /**< Partial results */                   \
	RESULT_ENUM( prefix, INVALID_KEY,                  1004 ),   /**< Invalid key */                       \
	RESULT_ENUM( prefix, DOES_NOT_EXIST,               1005 ),   /**< Does not exist */                    \
	RESULT_ENUM( prefix, NOT_AUTHENTICATED,            1006 ),   /**< Not authenticated */                 \
	RESULT_ENUM( prefix, NOT_KEYED,                    1007 ),   /**< Not keyed */                         \
	RESULT_ENUM( prefix, IOCTL_FAIL,                   1008 ),   /**< IOCTL fail */                        \
	RESULT_ENUM( prefix, BUFFER_UNAVAILABLE_TEMPORARY, 1009 ),   /**< Buffer unavailable temporarily */    \
	RESULT_ENUM( prefix, BUFFER_UNAVAILABLE_PERMANENT, 1010 ),   /**< Buffer unavailable permanently */    \
	RESULT_ENUM( prefix, CONNECTION_LOST,              1012 ),   /**< Connection lost */                   \
	RESULT_ENUM( prefix, OUT_OF_EVENT_HANDLER_SPACE,   1013 ),   /**< Cannot add extra event handler */    \
	RESULT_ENUM( prefix, SEMAPHORE_ERROR,              1014 ),   /**< Error manipulating a semaphore */    \
	RESULT_ENUM( prefix, FLOW_CONTROLLED,              1015 ),   /**< Packet retrieval cancelled due to flow control */ \
	RESULT_ENUM( prefix, NO_CREDITS,                   1016 ),   /**< Packet retrieval cancelled due to lack of bus credits */ \
	RESULT_ENUM( prefix, NO_PACKET_TO_SEND,            1017 ),   /**< Packet retrieval cancelled due to no pending packets */ \
	RESULT_ENUM( prefix, CORE_CLOCK_NOT_ENABLED,       1018 ),   /**< Core disabled due to no clock */    \
	RESULT_ENUM( prefix, CORE_IN_RESET,                1019 ),   /**< Core disabled - in reset */         \
	RESULT_ENUM( prefix, UNSUPPORTED,                  1020 ),   /**< Unsupported function */             \
	RESULT_ENUM( prefix, BUS_WRITE_REGISTER_ERROR,     1021 ),   /**< Error writing to WLAN register */   \
	RESULT_ENUM( prefix, SDIO_BUS_UP_FAIL,             1022 ),   /**< SDIO bus failed to come up */       \
	RESULT_ENUM( prefix, JOIN_IN_PROGRESS,             1023 ),   /**< Join not finished yet */   \
	RESULT_ENUM( prefix, NETWORK_NOT_FOUND,            1024 ),   /**< Specified network was not found */   \
	RESULT_ENUM( prefix, INVALID_JOIN_STATUS,          1025 ),   /**< Join status error */   \
	RESULT_ENUM( prefix, UNKNOWN_INTERFACE,            1026 ),   /**< Unknown interface specified */ \
	RESULT_ENUM( prefix, SDIO_RX_FAIL,                 1027 ),   /**< Error during SDIO receive */   \
	RESULT_ENUM( prefix, HWTAG_MISMATCH,               1028 ),   /**< Hardware tag header corrupt */   \
	RESULT_ENUM( prefix, RX_BUFFER_ALLOC_FAIL,         1029 ),   /**< Failed to allocate a buffer to receive into */   \
	RESULT_ENUM( prefix, BUS_READ_REGISTER_ERROR,      1030 ),   /**< Error reading a bus hardware register */   \
	RESULT_ENUM( prefix, THREAD_CREATE_FAILED,         1031 ),   /**< Failed to create a new thread */   \
	RESULT_ENUM( prefix, QUEUE_ERROR,                  1032 ),   /**< Error manipulating a queue */   \
	RESULT_ENUM( prefix, BUFFER_POINTER_MOVE_ERROR,    1033 ),   /**< Error moving the current pointer of a packet buffer  */   \
	RESULT_ENUM( prefix, BUFFER_SIZE_SET_ERROR,        1034 ),   /**< Error setting size of packet buffer */   \
	RESULT_ENUM( prefix, THREAD_STACK_NULL,            1035 ),   /**< Null stack pointer passed when non null was reqired */   \
	RESULT_ENUM( prefix, THREAD_DELETE_FAIL,           1036 ),   /**< Error deleting a thread */   \
	RESULT_ENUM( prefix, SLEEP_ERROR,                  1037 ),   /**< Error sleeping a thread */ \
	RESULT_ENUM( prefix, BUFFER_ALLOC_FAIL,            1038 ),   /**< Failed to allocate a packet buffer */ \
	RESULT_ENUM( prefix, NO_PACKET_TO_RECEIVE,         1039 ),   /**< No Packets waiting to be received */ \
	RESULT_ENUM( prefix, INTERFACE_NOT_UP,             1040 ),   /**< Requested interface is not active */ \
	RESULT_ENUM( prefix, DELAY_TOO_LONG,               1041 ),   /**< Requested delay is too long */ \
	RESULT_ENUM( prefix, INVALID_DUTY_CYCLE,           1042 ),   /**< Duty cycle is outside limit 0 to 100 */ \
	RESULT_ENUM( prefix, PMK_WRONG_LENGTH,             1043 ),   /**< Returned pmk was the wrong length */ \
	RESULT_ENUM( prefix, UNKNOWN_SECURITY_TYPE,        1044 ),   /**< AP security type was unknown */ \
	RESULT_ENUM( prefix, WEP_NOT_ALLOWED,              1045 ),   /**< AP not allowed to use WEP - it is not secure - use Open instead */ \
	RESULT_ENUM( prefix, WPA_KEYLEN_BAD,               1046 ),   /**< WPA / WPA2 key length must be between 8 & 64 bytes */ \
	RESULT_ENUM( prefix, FILTER_NOT_FOUND,             1047 ),   /**< Specified filter id not found */ \
	RESULT_ENUM( prefix, SPI_ID_READ_FAIL,             1048 ),   /**< Failed to read 0xfeedbead SPI id from chip */ \
	RESULT_ENUM( prefix, SPI_SIZE_MISMATCH,            1049 ),   /**< Mismatch in sizes between SPI header and SDPCM header */ \
	RESULT_ENUM( prefix, ADDRESS_ALREADY_REGISTERED,   1050 ),   /**< Attempt to register a multicast address twice */ \
	RESULT_ENUM( prefix, SDIO_RETRIES_EXCEEDED,        1051 ),   /**< SDIO transfer failed too many times. */ \
	RESULT_ENUM( prefix, NULL_PTR_ARG,                 1052 ),   /**< Null Pointer argument passed to function. */ \
	RESULT_ENUM( prefix, THREAD_FINISH_FAIL,           1053 ),   /**< Error deleting a thread */ \
	RESULT_ENUM( prefix, WAIT_ABORTED,                 1054 ),   /**< Semaphore/mutex wait has been aborted */ \
	RESULT_ENUM( prefix, SET_BLOCK_ACK_WINDOW_FAIL,    1055 ),   /**< Failed to set block ack window */ \
	RESULT_ENUM( prefix, DELAY_TOO_SHORT,              1056 ),   /**< Requested delay is too short */ \
	RESULT_ENUM( prefix, INVALID_INTERFACE,            1057 ),   /**< Invalid interface provided */ \
	RESULT_ENUM( prefix, WEP_KEYLEN_BAD,               1058 ),   /**< WEP / WEP_SHARED key length must be 5 or 13 bytes */ \
	RESULT_ENUM( prefix, HANDLER_ALREADY_REGISTERED,   1059 ),   /**< EAPOL handler already registered */ \
	RESULT_ENUM( prefix, AP_ALREADY_UP,                1060 ),   /**< Soft AP or P2P group owner already up */ \
	RESULT_ENUM( prefix, EAPOL_KEY_PACKET_M1_TIMEOUT,  1061 ),   /**< Timeout occurred while waiting for EAPOL packet M1 from AP */ \
	RESULT_ENUM( prefix, EAPOL_KEY_PACKET_M3_TIMEOUT,  1062 ),   /**< Timeout occurred while waiting for EAPOL packet M3 from AP, which may indicate incorrect WPA2/WPA passphrase */ \
	RESULT_ENUM( prefix, EAPOL_KEY_PACKET_G1_TIMEOUT,  1063 ),   /**< Timeout occurred while waiting for EAPOL packet G1 from AP */ \
	RESULT_ENUM( prefix, EAPOL_KEY_FAILURE,            1064 ),   /**< Unknown failure occurred during the EAPOL key handshake */ \
	RESULT_ENUM( prefix, MALLOC_FAILURE,               1065 ),   /**< Memory allocation failure */ \
	RESULT_ENUM( prefix, ACCESS_POINT_NOT_FOUND,       1066 ),	 /**< Access point not found */

/* These Enum result values are returned directly from the WLAN during an ioctl or iovar call.
 * Values: 1100 - 1200
 */
#define WLAN_ENUM_OFFSET  (2000)

#define WLAN_RESULT_LIST( prefix ) \
	RESULT_ENUM( prefix, ERROR,                       2001 ),  /**< Generic Error */                     \
	RESULT_ENUM( prefix, BADARG,                      2002 ),  /**< Bad Argument */                      \
	RESULT_ENUM( prefix, BADOPTION,                   2003 ),  /**< Bad option */                        \
	RESULT_ENUM( prefix, NOTUP,                       2004 ),  /**< Not up */                            \
	RESULT_ENUM( prefix, NOTDOWN,                     2005 ),  /**< Not down */                          \
	RESULT_ENUM( prefix, NOTAP,                       2006 ),  /**< Not AP */                            \
	RESULT_ENUM( prefix, NOTSTA,                      2007 ),  /**< Not STA  */                          \
	RESULT_ENUM( prefix, BADKEYIDX,                   2008 ),  /**< BAD Key Index */                     \
	RESULT_ENUM( prefix, RADIOOFF,                    2009 ),  /**< Radio Off */                         \
	RESULT_ENUM( prefix, NOTBANDLOCKED,               2010 ),  /**< Not  band locked */                  \
	RESULT_ENUM( prefix, NOCLK,                       2011 ),  /**< No Clock */                          \
	RESULT_ENUM( prefix, BADRATESET,                  2012 ),  /**< BAD Rate valueset */                 \
	RESULT_ENUM( prefix, BADBAND,                     2013 ),  /**< BAD Band */                          \
	RESULT_ENUM( prefix, BUFTOOSHORT,                 2014 ),  /**< Buffer too short */                  \
	RESULT_ENUM( prefix, BUFTOOLONG,                  2015 ),  /**< Buffer too long */                   \
	RESULT_ENUM( prefix, BUSY,                        2016 ),  /**< Busy */                              \
	RESULT_ENUM( prefix, NOTASSOCIATED,               2017 ),  /**< Not Associated */                    \
	RESULT_ENUM( prefix, BADSSIDLEN,                  2018 ),  /**< Bad SSID len */                      \
	RESULT_ENUM( prefix, OUTOFRANGECHAN,              2019 ),  /**< Out of Range Channel */              \
	RESULT_ENUM( prefix, BADCHAN,                     2020 ),  /**< Bad Channel */                       \
	RESULT_ENUM( prefix, BADADDR,                     2021 ),  /**< Bad Address */                       \
	RESULT_ENUM( prefix, NORESOURCE,                  2022 ),  /**< Not Enough Resources */              \
	RESULT_ENUM( prefix, UNSUPPORTED,                 2023 ),  /**< Unsupported */                       \
	RESULT_ENUM( prefix, BADLEN,                      2024 ),  /**< Bad length */                        \
	RESULT_ENUM( prefix, NOTREADY,                    2025 ),  /**< Not Ready */                         \
	RESULT_ENUM( prefix, EPERM,                       2026 ),  /**< Not Permitted */                     \
	RESULT_ENUM( prefix, NOMEM,                       2027 ),  /**< No Memory */                         \
	RESULT_ENUM( prefix, ASSOCIATED,                  2028 ),  /**< Associated */                        \
	RESULT_ENUM( prefix, RANGE,                       2029 ),  /**< Not In Range */                      \
	RESULT_ENUM( prefix, NOTFOUND,                    2030 ),  /**< Not Found */                         \
	RESULT_ENUM( prefix, WME_NOT_ENABLED,             2031 ),  /**< WME Not Enabled */                   \
	RESULT_ENUM( prefix, TSPEC_NOTFOUND,              2032 ),  /**< TSPEC Not Found */                   \
	RESULT_ENUM( prefix, ACM_NOTSUPPORTED,            2033 ),  /**< ACM Not Supported */                 \
	RESULT_ENUM( prefix, NOT_WME_ASSOCIATION,         2034 ),  /**< Not WME Association */               \
	RESULT_ENUM( prefix, SDIO_ERROR,                  2035 ),  /**< SDIO Bus Error */                    \
	RESULT_ENUM( prefix, WLAN_DOWN,                   2036 ),  /**< WLAN Not Accessible */               \
	RESULT_ENUM( prefix, BAD_VERSION,                 2037 ),  /**< Incorrect version */                 \
	RESULT_ENUM( prefix, TXFAIL,                      2038 ),  /**< TX failure */                        \
	RESULT_ENUM( prefix, RXFAIL,                      2039 ),  /**< RX failure */                        \
	RESULT_ENUM( prefix, NODEVICE,                    2040 ),  /**< Device not present */                \
	RESULT_ENUM( prefix, UNFINISHED,                  2041 ),  /**< To be finished */                    \
	RESULT_ENUM( prefix, NONRESIDENT,                 2042 ),  /**< access to nonresident overlay */     \
	RESULT_ENUM( prefix, DISABLED,                    2043 ),  /**< Disabled in this build */

/**
 * Common result type for WICED functions
 */
typedef enum {
	WWD_RESULT_LIST(WWD_)
	WLAN_RESULT_LIST(WWD_WLAN_)
}
wwd_result_t;

/** @endcond */

#ifdef __cplusplus
}								/* extern "C" */
#endif
#endif							/* ifndef INCLUDED_WWD_CONSTANTS_H_ */
