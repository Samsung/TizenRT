/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/* Note: this is an auto-generated file. */

#ifndef _FAPI_H__
#define _FAPI_H__

#include "utils_scsc.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "max_buf.h"
#ifdef __cplusplus
extern "C" {
#endif
#define __bitwise
typedef u16 __bitwise __le16;
typedef u32 __bitwise __le32;

#define FAPI_SIG_TYPE_MASK 0x0F00
#define FAPI_SIG_TYPE_REQ  0x0000
#define FAPI_SIG_TYPE_CFM  0x0100
#define FAPI_SIG_TYPE_RES  0x0200
#define FAPI_SIG_TYPE_IND  0x0300

#define FAPI_SAP_TYPE_MASK  0xF000
#define FAPI_SAP_TYPE_MA    0x1000
#define FAPI_SAP_TYPE_MLME  0x2000
#define FAPI_SAP_TYPE_DEBUG 0x8000
#define FAPI_SAP_TYPE_TEST  0x9000

#define FAPI_DEBUG_SAP_ENG_VERSION               0x0000
#define FAPI_CONTROL_SAP_ENG_VERSION             0x0000
#define FAPI_TEST_SAP_ENG_VERSION                0x0000
#define FAPI_DATA_SAP_ENG_VERSION                0x0000
#define FAPI_CONTROL_SAP_VERSION                 0x0d01
#define FAPI_TEST_SAP_VERSION                    0x0d01
#define FAPI_DEBUG_SAP_VERSION                   0x0d01
#define FAPI_DATA_SAP_VERSION                    0x0d01

#define FAPI_ACLPOLICY_BLACKLIST   0x0000
#define FAPI_ACLPOLICY_WHITELIST   0x0001

#define FAPI_AUTHENTICATIONTYPE_OPEN_SYSTEM   0x0000
#define FAPI_AUTHENTICATIONTYPE_SHARED_KEY    0x0001
#define FAPI_AUTHENTICATIONTYPE_LEAP          0x0080

#define FAPI_BANDWIDTH_20_MHZ    0x0
#define FAPI_BANDWIDTH_40_MHZ    0x1
#define FAPI_BANDWIDTH_80_MHZ    0x2
#define FAPI_BANDWIDTH_180_MHZ   0x3

#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_0   0x0001
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_1   0x0002
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_2   0x0004
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_3   0x0008
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_4   0x0010
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_5   0x0020
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_6   0x0040
#define FAPI_BLOCKACKCONTROL_BLOCKACK_PRIORITY_7   0x0080

#define FAPI_BLOCKACKPOLICY_DELAYED_BLOCKACK     0x0
#define FAPI_BLOCKACKPOLICY_IMMEDIATE_BLOCKACK   0x1

#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ   0x00
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ   0x01
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ   0x02

#define FAPI_CWSTARTFLAGS_NONE           0x0000
#define FAPI_CWSTARTFLAGS_SCAN_CHANNEL   0x0001

#define FAPI_CWTYPE_SINE       0x0000
#define FAPI_CWTYPE_RAMP       0x0001
#define FAPI_CWTYPE_TWO_TONE   0x0002
#define FAPI_CWTYPE_DC         0x0003
#define FAPI_CWTYPE_PRN        0x0004

#define FAPI_DATARATE_11B20_1MBPS           0x0001
#define FAPI_DATARATE_11B20_2MBPS           0x0002
#define FAPI_DATARATE_11B20_5M5BPS          0x0005
#define FAPI_DATARATE_11B20_11MBPS          0x000b
#define FAPI_DATARATE_11A20_6MBPS           0x2006
#define FAPI_DATARATE_11A20_9MBPS           0x2009
#define FAPI_DATARATE_11A20_12MBPS          0x200c
#define FAPI_DATARATE_11A20_18MBPS          0x2012
#define FAPI_DATARATE_11A20_24MBPS          0x2018
#define FAPI_DATARATE_11A20_36MBPS          0x2024
#define FAPI_DATARATE_11A20_48MBPS          0x2030
#define FAPI_DATARATE_11A20_54MBPS          0x2036
#define FAPI_DATARATE_11N20_6M5BPS          0x4000
#define FAPI_DATARATE_11N20_13MBPS          0x4001
#define FAPI_DATARATE_11N20_19M5BPS         0x4002
#define FAPI_DATARATE_11N20_26MBPS          0x4003
#define FAPI_DATARATE_11N20_39MBPS          0x4004
#define FAPI_DATARATE_11N20_52MBPS          0x4005
#define FAPI_DATARATE_11N20_58M5BPS         0x4006
#define FAPI_DATARATE_11N20_65MBPS          0x4007
#define FAPI_DATARATE_11N20_7M2BPS_SGI      0x4040
#define FAPI_DATARATE_11N20_14M4BPS_SGI     0x4041
#define FAPI_DATARATE_11N20_21M7BPS_SGI     0x4042
#define FAPI_DATARATE_11N20_28M9BPS_SGI     0x4043
#define FAPI_DATARATE_11N20_43M3BPS_SGI     0x4044
#define FAPI_DATARATE_11N20_57M8BPS_SGI     0x4045
#define FAPI_DATARATE_11N20_65MBPS_SGI      0x4046
#define FAPI_DATARATE_11N20_72M2BPS_SGI     0x4047
#define FAPI_DATARATE_11N40_13M5BPS         0x4400
#define FAPI_DATARATE_11N40_27MBPS          0x4401
#define FAPI_DATARATE_11N40_40M5BPS         0x4402
#define FAPI_DATARATE_11N40_54MBPS          0x4403
#define FAPI_DATARATE_11N40_81MBPS          0x4404
#define FAPI_DATARATE_11N40_108MBPS         0x4405
#define FAPI_DATARATE_11N40_121M5BPS        0x4406
#define FAPI_DATARATE_11N40_135MBPS         0x4407
#define FAPI_DATARATE_11N40_6MBPS           0x4420
#define FAPI_DATARATE_11N40_15MBPS_SGI      0x4440
#define FAPI_DATARATE_11N40_30MBPS_SGI      0x4441
#define FAPI_DATARATE_11N40_45MBPS_SGI      0x4442
#define FAPI_DATARATE_11N40_60MBPS_SGI      0x4443
#define FAPI_DATARATE_11N40_90MBPS_SGI      0x4444
#define FAPI_DATARATE_11N40_120MBPS_SGI     0x4445
#define FAPI_DATARATE_11N40_135MBPS_SGI     0x4446
#define FAPI_DATARATE_11N40_150MBPS_SGI     0x4447
#define FAPI_DATARATE_11N40_6M7BPS_SGI      0x4460
#define FAPI_DATARATE_11AC20_6M5BPS         0x6000
#define FAPI_DATARATE_11AC20_13MBPS         0x6001
#define FAPI_DATARATE_11AC20_19M5BPS        0x6002
#define FAPI_DATARATE_11AC20_26MBPS         0x6003
#define FAPI_DATARATE_11AC20_39MBPS         0x6004
#define FAPI_DATARATE_11AC20_52MBPS         0x6005
#define FAPI_DATARATE_11AC20_58M5BPS        0x6006
#define FAPI_DATARATE_11AC20_65MBPS         0x6007
#define FAPI_DATARATE_11AC20_78MBPS         0x6008
#define FAPI_DATARATE_11AC20_7M2BPS_SGI     0x6040
#define FAPI_DATARATE_11AC20_14M4BPS_SGI    0x6041
#define FAPI_DATARATE_11AC20_21M7BPS_SGI    0x6042
#define FAPI_DATARATE_11AC20_28M9BPS_SGI    0x6043
#define FAPI_DATARATE_11AC20_43M3BPS_SGI    0x6044
#define FAPI_DATARATE_11AC20_57M8BPS_SGI    0x6045
#define FAPI_DATARATE_11AC20_65MBPS_SGI     0x6046
#define FAPI_DATARATE_11AC20_72M2BPS_SGI    0x6047
#define FAPI_DATARATE_11AC20_86M7BPS_SGI    0x6048
#define FAPI_DATARATE_11AC40_13M5BPS        0x6400
#define FAPI_DATARATE_11AC40_27MBPS         0x6401
#define FAPI_DATARATE_11AC40_40M5BPS        0x6402
#define FAPI_DATARATE_11AC40_54MBPS         0x6403
#define FAPI_DATARATE_11AC40_81MBPS         0x6404
#define FAPI_DATARATE_11AC40_108MBPS        0x6405
#define FAPI_DATARATE_11AC40_121M5BPS       0x6406
#define FAPI_DATARATE_11AC40_135MBPS        0x6407
#define FAPI_DATARATE_11AC40_162MBPS        0x6408
#define FAPI_DATARATE_11AC40_180MBPS        0x6409
#define FAPI_DATARATE_11AC40_15MBPS_SGI     0x6440
#define FAPI_DATARATE_11AC40_30MBPS_SGI     0x6441
#define FAPI_DATARATE_11AC40_45MBPS_SGI     0x6442
#define FAPI_DATARATE_11AC40_60MBPS_SGI     0x6443
#define FAPI_DATARATE_11AC40_90MBPS_SGI     0x6444
#define FAPI_DATARATE_11AC40_120MBPS_SGI    0x6445
#define FAPI_DATARATE_11AC40_135MBPS_SGI    0x6446
#define FAPI_DATARATE_11AC40_150MBPS_SGI    0x6447
#define FAPI_DATARATE_11AC40_180MBPS_SGI    0x6448
#define FAPI_DATARATE_11AC40_200MBPS_SGI    0x6449
#define FAPI_DATARATE_11AC80_29M3BPS        0x6800
#define FAPI_DATARATE_11AC80_58M5BPS        0x6801
#define FAPI_DATARATE_11AC80_87M8BPS        0x6802
#define FAPI_DATARATE_11AC80_117MBPS        0x6803
#define FAPI_DATARATE_11AC80_175M5BPS       0x6804
#define FAPI_DATARATE_11AC80_234MBPS        0x6805
#define FAPI_DATARATE_11AC80_263M3BPS       0x6806
#define FAPI_DATARATE_11AC80_292M5BPS       0x6807
#define FAPI_DATARATE_11AC80_351MBPS        0x6808
#define FAPI_DATARATE_11AC80_390MBPS        0x6809
#define FAPI_DATARATE_11AC80_32M5BPS_SGI    0x6840
#define FAPI_DATARATE_11AC80_65MBPS_SGI     0x6841
#define FAPI_DATARATE_11AC80_97M5BPS_SGI    0x6842
#define FAPI_DATARATE_11AC80_130MBPS_SGI    0x6843
#define FAPI_DATARATE_11AC80_195MBPS_SGI    0x6844
#define FAPI_DATARATE_11AC80_260MBPS_SGI    0x6845
#define FAPI_DATARATE_11AC80_292M5BPS_SGI   0x6846
#define FAPI_DATARATE_11AC80_325MBPS_SGI    0x6847
#define FAPI_DATARATE_11AC80_390MBPS_SGI    0x6848
#define FAPI_DATARATE_11AC80_433M3BPS_SGI   0x6849
#define FAPI_DATARATE_CTR_TOTAL             0xe000
#define FAPI_DATARATE_CTR_NO_ERROR          0xe001
#define FAPI_DATARATE_CTR_CRC_ERROR         0xe002
#define FAPI_DATARATE_CTR_BAD_SIGNAL        0xe003
#define FAPI_DATARATE_CTR_PTW70             0xe004
#define FAPI_DATARATE_CTR_DUPLICATE         0xe005

#define FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME   0x0000
#define FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME    0x0001
#define FAPI_DATAUNITDESCRIPTOR_AMSDU_SUBFRAME     0x0002
#define FAPI_DATAUNITDESCRIPTOR_AMSDU              0x0003

#define FAPI_DEVICESTATE_IDLE         0
#define FAPI_DEVICESTATE_RX_RUNNING   1
#define FAPI_DEVICESTATE_TX_RUNNING   2
#define FAPI_DEVICESTATE_CW_RUNNING   3

#define FAPI_DIRECTION_TRANSMIT   0x0000
#define FAPI_DIRECTION_RECEIVE    0x0001
#define FAPI_DIRECTION_ANY        0x0003

#define FAPI_EPNOPOLICY_HIDDEN         0x0001
#define FAPI_EPNOPOLICY_A_BAND         0x0002
#define FAPI_EPNOPOLICY_G_BAND         0x0004
#define FAPI_EPNOPOLICY_STRICT_MATCH   0x0008
#define FAPI_EPNOPOLICY_AUTH_OPEN      0x0010
#define FAPI_EPNOPOLICY_AUTH_PSK       0x0200
#define FAPI_EPNOPOLICY_AUTH_EAPOL     0x0400

#define FAPI_HIDDENSSID_NOT_HIDDEN           0x0000
#define FAPI_HIDDENSSID_HIDDEN_ZERO_LENGTH   0x0001
#define FAPI_HIDDENSSID_HIDDEN_ZERO_DATA     0x0002

#define FAPI_HT_NON_HT_RATE   0x1
#define FAPI_HT_HT_RATE       0x2
#define FAPI_HT_VHT_RATE      0x3

#define FAPI_KEYTYPE_GROUP      0x0000
#define FAPI_KEYTYPE_PAIRWISE   0x0001
#define FAPI_KEYTYPE_PEERKEY    0x0002
#define FAPI_KEYTYPE_DEFAULT    0x0004
#define FAPI_KEYTYPE_OXYGEN     0x0008
#define FAPI_KEYTYPE_IGTK       0x0010

#define FAPI_MESSAGETYPE_EAP_MESSAGE          0x0001
#define FAPI_MESSAGETYPE_EAPOL_KEY_M123       0x0002
#define FAPI_MESSAGETYPE_EAPOL_KEY_M4         0x0003
#define FAPI_MESSAGETYPE_ARP                  0x0004
#define FAPI_MESSAGETYPE_DHCP                 0x0005
#define FAPI_MESSAGETYPE_NEIGHBOR_DISCOVERY   0x0006
#define FAPI_MESSAGETYPE_WAI_MESSAGE          0x0007
#define FAPI_MESSAGETYPE_IEEE80211_ACTION     0x0010

#define FAPI_PACKETFILTERMODE_OPT_OUT         0x01
#define FAPI_PACKETFILTERMODE_OPT_IN          0x02
#define FAPI_PACKETFILTERMODE_OPT_OUT_SLEEP   0x04
#define FAPI_PACKETFILTERMODE_OPT_IN_SLEEP    0x08

#define FAPI_PMFCONTROL_DISABLED    0x0000
#define FAPI_PMFCONTROL_OPTIONAL    0x0001
#define FAPI_PMFCONTROL_MANDATORY   0x0002

#define FAPI_POWERMANAGEMENTMODE_ACTIVE_MODE   0x0000
#define FAPI_POWERMANAGEMENTMODE_POWER_SAVE    0x0001

#define FAPI_PRIMARYCHANNELPOSITION_P0               0x00
#define FAPI_PRIMARYCHANNELPOSITION_P1               0x01
#define FAPI_PRIMARYCHANNELPOSITION_P2               0x02
#define FAPI_PRIMARYCHANNELPOSITION_P3               0x03
#define FAPI_PRIMARYCHANNELPOSITION_P4               0x04
#define FAPI_PRIMARYCHANNELPOSITION_P5               0x05
#define FAPI_PRIMARYCHANNELPOSITION_P6               0x06
#define FAPI_PRIMARYCHANNELPOSITION_P7               0x07
#define FAPI_PRIMARYCHANNELPOSITION_NOT_APPLICABLE   0x08

#define FAPI_PRIORITY_QOS_UP0      0x0000
#define FAPI_PRIORITY_QOS_UP1      0x0001
#define FAPI_PRIORITY_QOS_UP2      0x0002
#define FAPI_PRIORITY_QOS_UP3      0x0003
#define FAPI_PRIORITY_QOS_UP4      0x0004
#define FAPI_PRIORITY_QOS_UP5      0x0005
#define FAPI_PRIORITY_QOS_UP6      0x0006
#define FAPI_PRIORITY_QOS_UP7      0x0007
#define FAPI_PRIORITY_CONTENTION   0x8000

#define FAPI_PROCEDURETYPE_UNKNOWN              0x0000
#define FAPI_PROCEDURETYPE_CONNECTION_STARTED   0x0001
#define FAPI_PROCEDURETYPE_DEVICE_DISCOVERED    0x0002
#define FAPI_PROCEDURETYPE_ROAMING_STARTED      0x0003

#define FAPI_PROCESSOR_DEFAULT   0x0
#define FAPI_PROCESSOR_CHIPCPU   0x1
#define FAPI_PROCESSOR_HOSTCPU   0x3

#define FAPI_PURPOSE_BEACON                 0x0001
#define FAPI_PURPOSE_PROBE_RESPONSE         0x0002
#define FAPI_PURPOSE_ASSOCIATION_RESPONSE   0x0004
#define FAPI_PURPOSE_LOCAL                  0x0008
#define FAPI_PURPOSE_ASSOCIATION_REQUEST    0x0010
#define FAPI_PURPOSE_PROBE_REQUEST          0x0020

#define FAPI_REASONCODE_UNSPECIFIED_REASON                              0x0001
#define FAPI_REASONCODE_DEAUTHENTICATED_INVALID_AUTHENTICATION          0x0002
#define FAPI_REASONCODE_DEAUTHENTICATED_LEAVING_ESS                     0x0003
#define FAPI_REASONCODE_DEAUTHENTICATED_INACTIVITY                      0x0004
#define FAPI_REASONCODE_DEAUTHENTICATED_INVALID_CLASS_2_FRAME           0x0006
#define FAPI_REASONCODE_DEAUTHENTICATED_INVALID_CLASS_3_FRAME           0x0007
#define FAPI_REASONCODE_DEAUTHENTICATED_UNACCEPTABLE_POWER_CAPABILITY   0x000a
#define FAPI_REASONCODE_DEAUTHENTICATED_4_WAY_HANDSHAKE_TIMEOUT         0x000f
#define FAPI_REASONCODE_DEAUTHENTICATED_GROUP_HANDSHAKE_TIMEOUT         0x0010
#define FAPI_REASONCODE_DEAUTHENTICATED_HANDSHAKE_ELEMENT_MISMATCH      0x0011
#define FAPI_REASONCODE_DEAUTHENTICATED_REASON_INVALID_RSNE             0x0014
#define FAPI_REASONCODE_DEAUTHENTICATED_802_1_X_AUTH_FAILED             0x0017
#define FAPI_REASONCODE_TDLS_PEER_UNREACHABLE                           0x0019
#define FAPI_REASONCODE_TDLS_TEARDOWN_UNSPECIFIED_REASON                0x001a
#define FAPI_REASONCODE_QOS_UNSPECIFIED_REASON                          0x0020
#define FAPI_REASONCODE_QOS_EXCESSIVE_NOT_ACK                           0x0022
#define FAPI_REASONCODE_QOS_TXOP_LIMIT_EXCEEDED                         0x0023
#define FAPI_REASONCODE_QSTA_LEAVING                                    0x0024
#define FAPI_REASONCODE_END                                             0x0025
#define FAPI_REASONCODE_UNKNOWN                                         0x0026
#define FAPI_REASONCODE_TIMEOUT                                         0x0027
#define FAPI_REASONCODE_KEEP_ALIVE_FAILURE                              0x0028
#define FAPI_REASONCODE_START                                           0x0029
#define FAPI_REASONCODE_SYNCHRONISATION_LOSS                            0x8003
#define FAPI_REASONCODE_SECURITY_REQUIRED                               0x8004
#define FAPI_REASONCODE_ROAMING_FAILURE_LINK_LOSS_NO_CANDIDATE          0x8005

#define FAPI_REPORTMODE_BUFFER_FULL         0x0001
#define FAPI_REPORTMODE_END_OF_SCAN_CYCLE   0x0002
#define FAPI_REPORTMODE_REAL_TIME           0x0004
#define FAPI_REPORTMODE_NO_BATCH            0x0008

#define FAPI_RESULTCODE_SUCCESS                           0x0000
#define FAPI_RESULTCODE_UNSPECIFIED_FAILURE               0x0001
#define FAPI_RESULTCODE_INVALID_PARAMETERS                0x0026
#define FAPI_RESULTCODE_REJECTED_INVALID_IE               0x0028
#define FAPI_RESULTCODE_NOT_ALLOWED                       0x0030
#define FAPI_RESULTCODE_NOT_PRESENT                       0x0031
#define FAPI_RESULTCODE_TRANSMISSION_FAILURE              0x004f
#define FAPI_RESULTCODE_TIMEOUT                           0x8000
#define FAPI_RESULTCODE_TOO_MANY_SIMULTANEOUS_REQUESTS    0x8001
#define FAPI_RESULTCODE_BSS_ALREADY_STARTED_OR_JOINED     0x8002
#define FAPI_RESULTCODE_NOT_SUPPORTED                     0x8003
#define FAPI_RESULTCODE_INVALID_STATE                     0x8004
#define FAPI_RESULTCODE_INSUFFICIENT_RESOURCE             0x8006
#define FAPI_RESULTCODE_MISSING_INFORMATION_ELEMENT       0x8007
#define FAPI_RESULTCODE_INVALID_VIRTUAL_INTERFACE_INDEX   0x800a
#define FAPI_RESULTCODE_HOST_REQUEST_SUCCESS              0x800b
#define FAPI_RESULTCODE_HOST_REQUEST_FAILED               0x800c
#define FAPI_RESULTCODE_COMEBACK_TIME_RUNNING             0x800d
#define FAPI_RESULTCODE_INVALID_FREQUENCY                 0x800e

#define FAPI_RXSTARTFLAGS_NONE           0x0000
#define FAPI_RXSTARTFLAGS_SCAN_CHANNEL   0x0001
#define FAPI_RXSTARTFLAGS_FILTERING      0x0002
#define FAPI_RXSTARTFLAGS_PERIODIC       0x0004
#define FAPI_RXSTARTFLAGS_ACK            0x0008

#define FAPI_SCANPOLICY_PASSIVE     0x01
#define FAPI_SCANPOLICY_TEST_MODE   0x02
#define FAPI_SCANPOLICY_ANY_RA      0x04
#define FAPI_SCANPOLICY_2_4GHZ      0x08
#define FAPI_SCANPOLICY_5GHZ        0x10
#define FAPI_SCANPOLICY_NON_DFS     0x20
#define FAPI_SCANPOLICY_DFS         0x40

#define FAPI_SCANTYPE_INITIAL_SCAN                 0x0001
#define FAPI_SCANTYPE_FULL_SCAN                    0x0002
#define FAPI_SCANTYPE_SCHEDULED_SCAN               0x0003
#define FAPI_SCANTYPE_P2P_SCAN_FULL                0x0004
#define FAPI_SCANTYPE_P2P_SCAN_SOCIAL              0x0005
#define FAPI_SCANTYPE_OBSS_SCAN                    0x0006
#define FAPI_SCANTYPE_AP_AUTO_CHANNEL_SELECTION    0x0007
#define FAPI_SCANTYPE_PNO_SCAN                     0x0008
#define FAPI_SCANTYPE_GSCAN                        0x0009
#define FAPI_SCANTYPE_MEASUREMENT_SCAN             0x000a
#define FAPI_SCANTYPE_BACKGROUND_ROAMING_SCAN      0x000b
#define FAPI_SCANTYPE_PANIC_LIMITED_ROAMING_SCAN   0x000c
#define FAPI_SCANTYPE_PANIC_FULL_ROAMING_SCAN      0x000d

#define FAPI_STATSSTOPBITMAP_STATS_RADIO              0x0001
#define FAPI_STATSSTOPBITMAP_STATS_RADIO_CCA          0x0002
#define FAPI_STATSSTOPBITMAP_STATS_RADIO_CHANNELS     0x0004
#define FAPI_STATSSTOPBITMAP_STATS_RADIO_SCAN         0x0008
#define FAPI_STATSSTOPBITMAP_STATS_IFACE              0x0010
#define FAPI_STATSSTOPBITMAP_STATS_IFACE_TXRATE       0x0020
#define FAPI_STATSSTOPBITMAP_STATS_IFACE_AC           0x0040
#define FAPI_STATSSTOPBITMAP_STATS_IFACE_CONTENSION   0x0080

#define FAPI_TDLSACTION_DISCOVERY        0x0000
#define FAPI_TDLSACTION_SETUP            0x0001
#define FAPI_TDLSACTION_TEARDOWN         0x0002
#define FAPI_TDLSACTION_CHANNEL_SWITCH   0x0003

#define FAPI_TDLSEVENT_CONNECTED      0x0001
#define FAPI_TDLSEVENT_DISCONNECTED   0x0002
#define FAPI_TDLSEVENT_DISCOVERED     0x0003

#define FAPI_TRANSMISSIONSTATUS_SUCCESSFUL                0x0000
#define FAPI_TRANSMISSIONSTATUS_RETRY_LIMIT               0x0001
#define FAPI_TRANSMISSIONSTATUS_TX_LIFETIME               0x0002
#define FAPI_TRANSMISSIONSTATUS_NO_BSS                    0x0003
#define FAPI_TRANSMISSIONSTATUS_EXCESSIVE_DATA_LENGTH     0x0004
#define FAPI_TRANSMISSIONSTATUS_UNAVAILABLE_KEY_MAPPING   0x0005
#define FAPI_TRANSMISSIONSTATUS_UNSPECIFIED_FAILURE       0x0006

#define FAPI_TXDATATYPE_DATA_WORD     0x0000
#define FAPI_TXDATATYPE_DATA_RANDOM   0x0001

#define FAPI_TXREADFLAGS_NONE             0x0000
#define FAPI_TXREADFLAGS_FRAME_COUNTING   0x0001
#define FAPI_TXREADFLAGS_THERMAL_CUTOUT   0x0002

#define FAPI_TXSETPARAMSFLAGS_NONE                  0x0000
#define FAPI_TXSETPARAMSFLAGS_ACK                   0x0001
#define FAPI_TXSETPARAMSFLAGS_DUPLICATE_80          0x0002
#define FAPI_TXSETPARAMSFLAGS_DUPLICATE_40          0x0004
#define FAPI_TXSETPARAMSFLAGS_THERMAL_DUTY_CYCLE    0x0008
#define FAPI_TXSETPARAMSFLAGS_CS                    0x0010
#define FAPI_TXSETPARAMSFLAGS_SCAN_CHANNEL          0x0020
#define FAPI_TXSETPARAMSFLAGS_SHORT_PREAMBLE        0x0040
#define FAPI_TXSETPARAMSFLAGS_DISABLE_SCRAMBLER     0x0080
#define FAPI_TXSETPARAMSFLAGS_LDPC                  0x0100
#define FAPI_TXSETPARAMSFLAGS_DISABLE_SPREADER      0x0400
#define FAPI_TXSETPARAMSFLAGS_GREENFIELD_PREAMBLE   0x0800
#define FAPI_TXSETPARAMSFLAGS_IBSS_FRAMES           0x2000

#define FAPI_TYPEOFAIRPOWER_EIRP   0x00
#define FAPI_TYPEOFAIRPOWER_TPO    0x01
#define FAPI_TYPEOFAIRPOWER_RAW    0x02

#define FAPI_USAGE_NO_USE   0x0
#define FAPI_USAGE_USE      0x1

#define FAPI_VIFTYPE_UNSYNCHRONISED   0x0000
#define FAPI_VIFTYPE_ADHOC            0x0001
#define FAPI_VIFTYPE_STATION          0x0002
#define FAPI_VIFTYPE_AP               0x0003

#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P0               0x0000
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P1               0x0001
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P2               0x0002
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P3               0x0003
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P4               0x0004
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P5               0x0005
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P6               0x0006
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_P7               0x0007
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_20MHZ_PRIMARYCHANNELPOSITION_NOT_APPLICABLE   0x0008
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P0               0x0100
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P1               0x0101
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P2               0x0102
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P3               0x0103
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P4               0x0104
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P5               0x0105
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P6               0x0106
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_P7               0x0107
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_40MHZ_PRIMARYCHANNELPOSITION_NOT_APPLICABLE   0x0108
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P0               0x0200
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P1               0x0201
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P2               0x0202
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P3               0x0203
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P4               0x0204
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P5               0x0205
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P6               0x0206
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_P7               0x0207
#define FAPI_CHANNELBANDWIDTH_BANDWIDTH_80MHZ_PRIMARYCHANNELPOSITION_NOT_APPLICABLE   0x0208

#define MA_UNITDATA_REQ                       0x1000
#define MA_SPARE_SIGNAL_1_REQ                 0x1001
#define MA_SPARE_SIGNAL_2_REQ                 0x1002
#define MA_SPARE_SIGNAL_3_REQ                 0x1003
#define MA_UNITDATA_CFM                       0x1100
#define MA_SPARE_SIGNAL_1_CFM                 0x1101
#define MA_SPARE_SIGNAL_2_CFM                 0x1102
#define MA_SPARE_SIGNAL_3_CFM                 0x1103
#define MA_SPARE_SIGNAL_1_RES                 0x1200
#define MA_SPARE_SIGNAL_2_RES                 0x1201
#define MA_SPARE_SIGNAL_3_RES                 0x1202
#define MA_UNITDATA_IND                       0x1300
#define MA_BLOCKACK_IND                       0x1301
#define MA_TX_FAILURE_IND                     0x1302
#define MA_SPARE_SIGNAL_1_IND                 0x1303
#define MA_SPARE_SIGNAL_2_IND                 0x1304
#define MA_SPARE_SIGNAL_3_IND                 0x1305
#define MLME_GET_REQ                          0x2001
#define MLME_SET_REQ                          0x2002
#define MLME_POWERMGT_REQ                     0x2003
#define MLME_ADD_INFO_ELEMENTS_REQ            0x2004
#define MLME_ADD_SCAN_REQ                     0x2005
#define MLME_DEL_SCAN_REQ                     0x2006
#define MLME_ADD_VIF_REQ                      0x2007
#define MLME_DEL_VIF_REQ                      0x2008
#define MLME_START_REQ                        0x2009
#define MLME_SET_CHANNEL_REQ                  0x200a
#define MLME_CONNECT_REQ                      0x200b
#define MLME_REASSOCIATE_REQ                  0x200c
#define MLME_ROAM_REQ                         0x200d
#define MLME_DISCONNECT_REQ                   0x200e
#define MLME_REGISTER_ACTION_FRAME_REQ        0x200f
#define MLME_SEND_FRAME_REQ                   0x2010
#define MLME_RESET_DWELL_TIME_REQ             0x2011
#define MLME_SET_TRAFFIC_PARAMETERS_REQ       0x2012
#define MLME_DEL_TRAFFIC_PARAMETERS_REQ       0x2013
#define MLME_SET_PACKET_FILTER_REQ            0x2014
#define MLME_SET_IP_ADDRESS_REQ               0x2015
#define MLME_SET_ACL_REQ                      0x2016
#define MLME_SET_ROAMING_PARAMETERS_REQ       0x2017
#define MLME_SETKEYS_REQ                      0x2018
#define MLME_GET_KEY_SEQUENCE_REQ             0x201a
#define MLME_SET_CCKM_KEYS_REQ                0x201b
#define MLME_SET_PMK_REQ                      0x201c
#define MLME_START_TXRMC_REQ                  0x201d
#define MLME_STOP_TXRMC_REQ                   0x201e
#define MLME_SET_CACHED_CHANNELS_REQ          0x201f
#define MLME_BLOCKACK_CONTROL_REQ             0x2020
#define MLME_TDLS_ACTION_REQ                  0x2021
#define MLME_CHANNEL_SWITCH_REQ               0x2022
#define MLME_MONITOR_RSSI_REQ                 0x2023
#define MLME_START_LINK_STATISTICS_REQ        0x2024
#define MLME_STOP_LINK_STATISTICS_REQ         0x2025
#define MLME_SET_BSSID_HOTLIST_REQ            0x2026
#define MLME_SET_PNO_LIST_REQ                 0x2027
#define MLME_SPARE_SIGNAL_1_REQ               0x2028
#define MLME_SPARE_SIGNAL_2_REQ               0x2029
#define MLME_SPARE_SIGNAL_3_REQ               0x202a
#define MLME_GET_CFM                          0x2101
#define MLME_SET_CFM                          0x2102
#define MLME_POWERMGT_CFM                     0x2103
#define MLME_ADD_INFO_ELEMENTS_CFM            0x2104
#define MLME_ADD_SCAN_CFM                     0x2105
#define MLME_DEL_SCAN_CFM                     0x2106
#define MLME_ADD_VIF_CFM                      0x2107
#define MLME_DEL_VIF_CFM                      0x2108
#define MLME_START_CFM                        0x2109
#define MLME_SET_CHANNEL_CFM                  0x210a
#define MLME_CONNECT_CFM                      0x210b
#define MLME_REASSOCIATE_CFM                  0x210c
#define MLME_ROAM_CFM                         0x210d
#define MLME_DISCONNECT_CFM                   0x210e
#define MLME_REGISTER_ACTION_FRAME_CFM        0x210f
#define MLME_SEND_FRAME_CFM                   0x2110
#define MLME_RESET_DWELL_TIME_CFM             0x2111
#define MLME_SET_TRAFFIC_PARAMETERS_CFM       0x2112
#define MLME_DEL_TRAFFIC_PARAMETERS_CFM       0x2113
#define MLME_SET_PACKET_FILTER_CFM            0x2114
#define MLME_SET_IP_ADDRESS_CFM               0x2115
#define MLME_SET_ACL_CFM                      0x2116
#define MLME_SET_ROAMING_PARAMETERS_CFM       0x2117
#define MLME_SETKEYS_CFM                      0x2118
#define MLME_GET_KEY_SEQUENCE_CFM             0x211a
#define MLME_SET_CCKM_KEYS_CFM                0x211b
#define MLME_SET_PMK_CFM                      0x211c
#define MLME_START_TXRMC_CFM                  0x211d
#define MLME_STOP_TXRMC_CFM                   0x211e
#define MLME_SET_CACHED_CHANNELS_CFM          0x211f
#define MLME_BLOCKACK_CONTROL_CFM             0x2120
#define MLME_TDLS_ACTION_CFM                  0x2121
#define MLME_CHANNEL_SWITCH_CFM               0x2122
#define MLME_MONITOR_RSSI_CFM                 0x2123
#define MLME_START_LINK_STATISTICS_CFM        0x2124
#define MLME_STOP_LINK_STATISTICS_CFM         0x2125
#define MLME_SET_BSSID_HOTLIST_CFM            0x2126
#define MLME_SET_PNO_LIST_CFM                 0x2127
#define MLME_SPARE_SIGNAL_1_CFM               0x2128
#define MLME_SPARE_SIGNAL_2_CFM               0x2129
#define MLME_SPARE_SIGNAL_3_CFM               0x212a
#define MLME_CONNECT_RES                      0x2200
#define MLME_CONNECTED_RES                    0x2201
#define MLME_REASSOCIATE_RES                  0x2202
#define MLME_ROAMED_RES                       0x2203
#define MLME_TDLS_PEER_RES                    0x2204
#define MLME_SPARE_SIGNAL_1_RES               0x2205
#define MLME_SPARE_SIGNAL_2_RES               0x2206
#define MLME_SPARE_SIGNAL_3_RES               0x2207
#define MLME_SCAN_IND                         0x2300
#define MLME_SCAN_DONE_IND                    0x2301
#define MLME_LISTEN_END_IND                   0x2302
#define MLME_CONNECT_IND                      0x2303
#define MLME_CONNECTED_IND                    0x2304
#define MLME_REASSOCIATE_IND                  0x2305
#define MLME_ROAM_IND                         0x2306
#define MLME_ROAMED_IND                       0x2307
#define MLME_DISCONNECT_IND                   0x2308
#define MLME_DISCONNECTED_IND                 0x2309
#define MLME_PROCEDURE_STARTED_IND            0x230a
#define MLME_MIC_FAILURE_IND                  0x230b
#define MLME_FRAME_TRANSMISSION_IND           0x230c
#define MLME_RECEIVED_FRAME_IND               0x230d
#define MLME_RMC_LEADER_SELECTED_IND          0x230e
#define MLME_TDLS_PEER_IND                    0x230f
#define MLME_AP_LOSS_IND                      0x2310
#define MLME_SIGNIFICANT_CHANGE_IND           0x2311
#define MLME_RSSI_REPORT_IND                  0x2312
#define MLME_AC_PRIORITY_UPDATE_IND           0x2313
#define MLME_SPARE_SIGNAL_1_IND               0x2314
#define MLME_SPARE_SIGNAL_2_IND               0x2315
#define MLME_SPARE_SIGNAL_3_IND               0x2316
#define DEBUG_GENERIC_REQ                     0x8000
#define DEBUG_PKT_SINK_START_REQ              0x8001
#define DEBUG_PKT_SINK_STOP_REQ               0x8002
#define DEBUG_PKT_SINK_REPORT_REQ             0x8003
#define DEBUG_PKT_GEN_START_REQ               0x8004
#define DEBUG_PKT_GEN_STOP_REQ                0x8005
#define DEBUG_PKT_GEN_REPORT_REQ              0x8006
#define DEBUG_SPARE_SIGNAL_1_REQ              0x8007
#define DEBUG_SPARE_SIGNAL_2_REQ              0x8008
#define DEBUG_SPARE_SIGNAL_3_REQ              0x8009
#define DEBUG_GENERIC_CFM                     0x8100
#define DEBUG_SPARE_SIGNAL_1_CFM              0x8101
#define DEBUG_SPARE_SIGNAL_2_CFM              0x8102
#define DEBUG_SPARE_SIGNAL_3_CFM              0x8103
#define DEBUG_SPARE_SIGNAL_1_RES              0x8200
#define DEBUG_SPARE_SIGNAL_2_RES              0x8201
#define DEBUG_SPARE_SIGNAL_3_RES              0x8202
#define DEBUG_WORD12IND                       0x8301
#define DEBUG_FAULT_IND                       0x8302
#define DEBUG_GENERIC_IND                     0x8303
#define DEBUG_PKT_SINK_REPORT_IND             0x8304
#define DEBUG_PKT_GEN_REPORT_IND              0x8305
#define DEBUG_SPARE_SIGNAL_1_IND              0x8306
#define DEBUG_SPARE_SIGNAL_2_IND              0x8307
#define DEBUG_SPARE_SIGNAL_3_IND              0x8308
#define TEST_BLOCK_REQUESTS_REQ               0x9000
#define TEST_PANIC_REQ                        0x9001
#define TEST_SUSPEND_REQ                      0x9002
#define TEST_RESUME_REQ                       0x9003
#define RADIO_LOGGING_REQ                     0x9004
#define WLANLITE_CW_START_REQ                 0x9005
#define WLANLITE_CW_STOP_REQ                  0x9006
#define WLANLITE_TX_SET_PARAMS_REQ            0x9007
#define WLANLITE_TX_START_REQ                 0x9008
#define WLANLITE_TX_READ_REQ                  0x9009
#define WLANLITE_TX_STOP_REQ                  0x900a
#define WLANLITE_RX_START_REQ                 0x900b
#define WLANLITE_RX_READ_REQ                  0x900c
#define WLANLITE_RX_STOP_REQ                  0x900d
#define WLANLITE_STATUS_REQ                   0x900e
#define TEST_SPARE_SIGNAL_1_REQ               0x900f
#define TEST_SPARE_SIGNAL_2_REQ               0x9010
#define TEST_SPARE_SIGNAL_3_REQ               0x9011
#define RADIO_LOGGING_CFM                     0x9100
#define WLANLITE_CW_START_CFM                 0x9101
#define WLANLITE_TX_SET_PARAMS_CFM            0x9102
#define WLANLITE_CW_STOP_CFM                  0x9103
#define WLANLITE_TX_START_CFM                 0x9104
#define WLANLITE_TX_READ_CFM                  0x9105
#define WLANLITE_TX_STOP_CFM                  0x9106
#define WLANLITE_RX_START_CFM                 0x9107
#define WLANLITE_RX_READ_CFM                  0x9108
#define WLANLITE_RX_STOP_CFM                  0x9109
#define WLANLITE_STATUS_CFM                   0x910a
#define TEST_SPARE_SIGNAL_1_CFM               0x910b
#define TEST_SPARE_SIGNAL_2_CFM               0x910c
#define TEST_SPARE_SIGNAL_3_CFM               0x910d
#define TEST_SPARE_SIGNAL_1_RES               0x9200
#define TEST_SPARE_SIGNAL_2_RES               0x9201
#define TEST_SPARE_SIGNAL_3_RES               0x9202
#define RADIO_LOGGING_IND                     0x9300
#define TEST_SPARE_SIGNAL_1_IND               0x9301
#define TEST_SPARE_SIGNAL_2_IND               0x9302
#define TEST_SPARE_SIGNAL_3_IND               0x9303

struct fapi_signal_header {
	__le16 id;
	__le16 receiver_pid;
	__le16 sender_pid;
	__le32 fw_reference;
} STRUCT_PACKED;

struct fapi_vif_signal_header {
	__le16 id;
	__le16 receiver_pid;
	__le16 sender_pid;
	__le32 fw_reference;
	__le16 vif;
} STRUCT_PACKED;

struct fapi_signal {
	__le16 id;
	__le16 receiver_pid;
	__le16 sender_pid;
	__le32 fw_reference;

	union {
		struct {
			__le16 vif;
			__le16 host_tag;
			__le16 priority;
			__le16 peer_index;
			__le16 data_unit_descriptor;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_unitdata_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_1_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_2_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_3_req;
		struct {
			__le16 vif;
			__le16 transmission_status;
			__le16 host_tag;
			__le16 sequence_number;
			__le16 peer_index;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_unitdata_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_1_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_2_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_3_cfm;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_1_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_2_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_3_res;
		struct {
			__le16 vif;
			__le16 data_unit_descriptor;
			__le16 sequence_number;
			__le16 priority;
			__le16 peer_index;
			__le16 proprieraty_information_length;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_unitdata_ind;
		struct {
			__le16 vif;
			u8 peer_qsta_address[ETH_ALEN];
			__le16 sequence_number;
			__le16 reason_code;
			__le16 blockack_parameter_set;
			__le16 direction;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_blockack_ind;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_tx_failure_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_1_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_2_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED ma_spare_signal_3_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_get_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_req;
		struct {
			__le16 vif;
			__le16 power_management_mode;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_powermgt_req;
		struct {
			__le16 vif;
			__le16 purpose;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_info_elements_req;
		struct {
			__le16 vif;
			__le16 scan_id;
			__le16 scan_type;
			u8 device_address[ETH_ALEN];
			__le16 report_mode_bitmap;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_scan_req;
		struct {
			__le16 vif;
			__le16 scan_id;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_scan_req;
		struct {
			__le16 vif;
			u8 interface_address[ETH_ALEN];
			__le16 virtual_interface_type;
			u8 device_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_vif_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_vif_req;
		struct {
			__le16 vif;
			u8 bssid[ETH_ALEN];
			__le16 beacon_period;
			__le16 dtim_period;
			__le16 capability_information;
			__le16 authentication_type;
			__le16 hidden_ssid;
			__le16 channel_frequency;
			__le16 channel_information;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_req;
		struct {
			__le16 vif;
			__le16 availability_duration;
			__le16 availability_interval;
			__le16 count;
			__le16 channel_frequency;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_channel_req;
		struct {
			__le16 vif;
			u8 bssid[ETH_ALEN];
			__le16 authentication_type;
			__le16 channel_frequency;
			__le16 pmf_control;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connect_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reassociate_req;
		struct {
			__le16 vif;
			u8 bssid[ETH_ALEN];
			__le16 channel_frequency;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_roam_req;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 reason_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_disconnect_req;
		struct {
			__le16 vif;
			__le32 action_frame_category_bitmap_active;
			__le32 action_frame_category_bitmap_suspended;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_register_action_frame_req;
		struct {
			__le16 vif;
			__le16 host_tag;
			__le16 data_unit_descriptor;
			__le16 message_type;
			__le16 channel_frequency;
			__le32 dwell_time;
			__le32 period;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_send_frame_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reset_dwell_time_req;
		struct {
			__le16 vif;
			__le16 user_priority;
			__le16 medium_time;
			__le16 minimun_data_rate;
			u8 peer_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_traffic_parameters_req;
		struct {
			__le16 vif;
			__le16 user_priority;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_traffic_parameters_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_packet_filter_req;
		struct {
			__le16 vif;
			__le16 ip_version;
			u8 multicast_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_ip_address_req;
		struct {
			__le16 vif;
			__le16 entries;
			__le16 acl_policy;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_acl_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_roaming_parameters_req;
		struct {
			__le16 vif;
			__le16 length;
			__le16 key_id;
			__le16 key_type;
			u8 address[ETH_ALEN];
			__le16 sequence_number[8];
			__le32 cipher_suite_selector;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_setkeys_req;
		struct {
			__le16 vif;
			__le16 key_id;
			__le16 key_type;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_get_key_sequence_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_cckm_keys_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_pmk_req;
		struct {
			__le16 vif;
			u8 multicast_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_txrmc_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_stop_txrmc_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_cached_channels_req;
		struct {
			__le16 vif;
			__le16 blockack_control_bitmap;
			__le16 direction;
			u8 peer_sta_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_blockack_control_req;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 tdls_action;
			__le16 channel_frequency;
			__le16 channel_information;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_tdls_action_req;
		struct {
			__le16 vif;
			__le16 channel_frequency;
			__le16 channel_information;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_channel_switch_req;
		struct {
			__le16 vif;
			__le16 low_rssi_threshold;
			__le16 high_rssi_threshold;
			__le16 rssi_monitoring_enabled;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_monitor_rssi_req;
		struct {
			__le16 vif;
			__le16 mpdu_size_threshold;
			__le16 aggressive_statistics_gathering_enabled;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_link_statistics_req;
		struct {
			__le16 vif;
			__le16 statistics_stop_bitmap;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_stop_link_statistics_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_bssid_hotlist_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_pno_list_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_1_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_2_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_3_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_get_cfm;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_powermgt_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_info_elements_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le16 scan_id;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_scan_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le16 scan_id;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_scan_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_add_vif_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_vif_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_channel_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connect_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reassociate_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_roam_cfm;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_disconnect_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_register_action_frame_cfm;
		struct {
			__le16 vif;
			__le16 host_tag;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_send_frame_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reset_dwell_time_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_traffic_parameters_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_del_traffic_parameters_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_packet_filter_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_ip_address_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_acl_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_roaming_parameters_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_setkeys_cfm;
		struct {
			__le16 vif;
			__le16 sequence_number[8];
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_get_key_sequence_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_cckm_keys_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_pmk_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_txrmc_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_stop_txrmc_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_cached_channels_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_blockack_control_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_tdls_action_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_channel_switch_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_monitor_rssi_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_start_link_statistics_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_stop_link_statistics_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_bssid_hotlist_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_set_pno_list_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_1_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_2_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_3_cfm;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connect_res;
		struct {
			__le16 vif;
			__le16 peer_index;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connected_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reassociate_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_roamed_res;
		struct {
			__le16 vif;
			__le16 peer_index;
			__le16 tdls_event;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_tdls_peer_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_1_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_2_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_3_res;
		struct {
			__le16 vif;
			__le16 channel_frequency;
			__le16 rssi;
			__le16 scan_id;
			__le16 hotlisted_ap;
			__le16 preferrednetwork_ap;
			__le16 anqp_elements_length;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_scan_ind;
		struct {
			__le16 vif;
			__le16 scan_id;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_scan_done_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_listen_end_ind;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connect_ind;
		struct {
			__le16 vif;
			__le16 peer_index;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_connected_ind;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_reassociate_ind;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_roam_ind;
		struct {
			__le16 vif;
			__le16 temporal_keys_required;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_roamed_ind;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 transmission_status;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_disconnect_ind;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 reason_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_disconnected_ind;
		struct {
			__le16 vif;
			__le16 procedure_type;
			__le16 peer_index;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_procedure_started_ind;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 key_type;
			__le16 key_id;
			__le16 key_sequence_number[8];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_mic_failure_ind;
		struct {
			__le16 vif;
			__le16 host_tag;
			__le16 transmission_status;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_frame_transmission_ind;
		struct {
			__le16 vif;
			__le16 data_unit_descriptor;
			__le16 channel_frequency;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_received_frame_ind;
		struct {
			__le16 vif;
			u8 multicast_address[ETH_ALEN];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_rmc_leader_selected_ind;
		struct {
			__le16 vif;
			u8 peer_sta_address[ETH_ALEN];
			__le16 peer_index;
			__le16 tdls_event;
			__le16 reason_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_tdls_peer_ind;
		struct {
			__le16 vif;
			__le16 entries;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_ap_loss_ind;
		struct {
			__le16 vif;
			__le16 number_of_results;
			__le16 number_of_rssi_entries;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_significant_change_ind;
		struct {
			__le16 vif;
			u8 bssid[ETH_ALEN];
			__le16 low_rssi_threshold;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_rssi_report_ind;
		struct {
			__le16 vif;
			__le16 ac_priority;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_ac_priority_update_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_1_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_2_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED mlme_spare_signal_3_ind;
		struct {
			__le16 vif;
			__le16 debug_words[8];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_generic_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 interval;
			__le16 packets_per_interval;
			__le16 reserved_padding;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_sink_start_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_sink_stop_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 report_interval;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_sink_report_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 interval;
			__le16 size;
			__le16 packets_per_interval;
			__le32 ipv4destination_address;
			__le16 packets_per_interrupt;
			__le16 use_streaming;
			u8 reserved_padding;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_gen_start_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_gen_stop_req;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 report_interval;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_gen_report_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_1_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_2_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_3_req;
		struct {
			__le16 vif;
			__le16 debug_words[8];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_generic_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_1_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_2_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_3_cfm;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_1_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_2_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_3_res;
		struct {
			__le16 vif;
			__le16 module_id;
			__le16 module_sub_id;
			__le32 timestamp;
			__le16 debug_words[12];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_word12_ind;
		struct {
			__le16 vif;
			__le16 faultid;
			__le16 count;
			__le32 timestamp;
			__le32 arg;
			__le16 cpu;
			__le16 reserved_padding;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_fault_ind;
		struct {
			__le16 vif;
			__le16 debug_words[8];
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_generic_ind;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 duration;
			__le32 received_packets;
			__le32 recieved_octets;
			__le32 kbps;
			__le16 idle_ratio;
			__le16 int_latency;
			__le16 free_kbytes;
			__le16 reserved_padding;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_sink_report_ind;
		struct {
			__le16 vif;
			__le16 end_point;
			__le16 direction;
			__le32 duration;
			__le32 received_packets;
			__le32 failed_count;
			__le32 recieved_octets;
			__le32 kbps;
			__le16 idle_ratio;
			__le16 int_latency;
			__le16 free_kbytes;
			__le16 reserved_padding;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_pkt_gen_report_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_1_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_2_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED debug_spare_signal_3_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_block_requests_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_panic_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_suspend_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_resume_req;
		struct {
			__le32 logging_source;
			__le32 logging_frequency;
			__le32 trigger_mode;
			__le32 delay;
			__le32 buffer_size;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED radio_logging_req;
		struct {
			__le16 freq;
			__le16 power;
			__le16 flags;
			__le16 type;
			__le16 amplitude;
			__le32 freq1;
			__le32 freq2;
			__le16 phase;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_cw_start_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_cw_stop_req;
		struct {
			__le16 freq;
			__le16 rate;
			__le16 channel_information;
			__le16 power;
			__le16 length;
			__le32 interval;
			__le16 flags;
			__le16 channel_description_index;
			__le16 distance_to_band_edge_half_mhz;
			__le16 band_oob_rule_selector;
			__le16 pa_voltage;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_set_params_req;
		struct {
			__le32 num_frames_to_send;
			__le16 data_type;
			__le16 data_param;
			u8 dest_addr[ETH_ALEN];
			u8 src_addr[ETH_ALEN];
			u8 bssid[ETH_ALEN];
			__le16 num_mpdus_per_ampdu;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_start_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_read_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_stop_req;
		struct {
			__le16 freq;
			__le16 channel_information;
			__le16 flags;
			u8 mac_addr[ETH_ALEN];
			__le32 on_duration;
			__le32 off_duration;
			__le16 num_mpdus_per_ampdu;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_start_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_read_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_stop_req;
		struct {
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_status_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_1_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_2_req;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_3_req;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED radio_logging_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_cw_start_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_set_params_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_cw_stop_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_start_cfm;
		struct {
			__le16 result_code;
			__le16 flags;
			__le32 ctr_frames_left_to_send;
			__le32 transmission_back_off;
			__le16 wanted_power_target;
			__le16 final_power_target;
			__le16 oob_constraint;
			__le16 last_trim_pa_temperature;
			__le16 current_pa_temperature;
			__le16 last_trim_ambient_temperature;
			__le16 current_ambient_temperature;
			__le16 temp_power_adjust;
			__le32 ctr_frames_success;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_read_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_tx_stop_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_start_cfm;
		struct {
			__le16 result_code;
			__le32 freq_offset_cur;
			__le32 freq_offset_avg;
			__le32 rssi_cur;
			__le32 rssi_avg;
			__le32 snr_cur;
			__le32 snr_avg;
			__le32 interval;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_read_cfm;
		struct {
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_rx_stop_cfm;
		struct {
			__le16 result_code;
			__le16 device_state;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED wlanlite_status_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_1_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_2_cfm;
		struct {
			__le16 vif;
			__le16 result_code;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_3_cfm;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_1_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_2_res;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_3_res;
		struct {
			__le32 sequence_number;
			__le32 more_data;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED radio_logging_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_1_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_2_ind;
		struct {
			__le16 vif;
			__le32 spare_1;
			__le32 spare_2;
			__le32 spare_3;
			u8 dr[0];
		} STRUCT_PACKED test_spare_signal_3_ind;
	} u;
} STRUCT_PACKED;

static inline struct max_buff *fapi_alloc_f(size_t sig_size, size_t data_size, u16 id, u16 vif, const char *file, int line)
{
	struct max_buff *mbuf = slsi_mbuf_alloc(sig_size + data_size);
	struct fapi_vif_signal_header *header;

	WARN_ON(sig_size < sizeof(struct fapi_signal_header));
	if (WARN_ON(!mbuf)) {
		return NULL;
	}

	mbuf->fapi.sig_length = sig_size;
	mbuf->fapi.data_length = sig_size;
	header = (struct fapi_vif_signal_header *)mbuf_put(mbuf, sig_size);
	header->id = cpu_to_le16(id);
	header->receiver_pid = 0;
	header->sender_pid = 0;
	header->fw_reference = 0;
	header->vif = vif;
	return mbuf;
}
#define fapi_sig_size(mp_name)                  ((u16)offsetof(struct fapi_signal, u.mp_name.dr))
#define fapi_alloc(mp_name, mp_id, mp_vif, mp_datalen) fapi_alloc_f(fapi_sig_size(mp_name), mp_datalen, mp_id, mp_vif, __FILE__, __LINE__)
#define fapi_get_buff(mp_mbuf, mp_name) (((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name)
#define fapi_get_u16(mp_mbuf, mp_name) le16_to_cpu(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name)
#define fapi_get_u32(mp_mbuf, mp_name) le32_to_cpu(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name)
#define fapi_set_u16(mp_mbuf, mp_name, mp_value) (((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name = cpu_to_le16(mp_value))
#define fapi_set_u32(mp_mbuf, mp_name, mp_value) (((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name = cpu_to_le32(mp_value))
#define fapi_get_s16(mp_mbuf, mp_name) ((s16)le16_to_cpu(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name))
#define fapi_get_s32(mp_mbuf, mp_name) ((s32)le32_to_cpu(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name))
#define fapi_set_s16(mp_mbuf, mp_name, mp_value) (((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name = cpu_to_le16((u16)mp_value))
#define fapi_set_s32(mp_mbuf, mp_name, mp_value) (((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name = cpu_to_le32((u32)mp_value))
#define fapi_set_memcpy(mp_mbuf, mp_name, mp_value) memcpy(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name, mp_value, sizeof(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name))
#define fapi_set_memset(mp_mbuf, mp_name, mp_value) memset(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name, mp_value, sizeof(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->mp_name))
/* Helper to get and set high/low 16 bits from u32 signals */
#define fapi_get_high16_u32(mp_mbuf, mp_name) ((fapi_get_u32((mp_mbuf), mp_name) & 0xffff0000) >> 16)
#define fapi_set_high16_u32(mp_mbuf, mp_name, mp_value) fapi_set_u32((mp_mbuf), mp_name, (fapi_get_u32((mp_mbuf), mp_name) & 0xffff) | ((mp_value) << 16))
#define fapi_get_low16_u32(mp_mbuf, mp_name) (fapi_get_u32((mp_mbuf), mp_name) & 0xffff)
#define fapi_set_low16_u32(mp_mbuf, mp_name, mp_value) fapi_set_u32((mp_mbuf), mp_name, (fapi_get_u32((mp_mbuf), mp_name) & 0xffff0000) | (mp_value))
/* Helper to get signal and data */
#define fapi_get_sigid(mp_mbuf) le16_to_cpu(((struct fapi_signal *)slsi_mbuf_get_data(mp_mbuf))->id)
#define fapi_get_siglen(mp_mbuf) (mp_mbuf->fapi.sig_length)
#define fapi_get_datalen(mp_mbuf) (mp_mbuf->fapi.data_length - mp_mbuf->fapi.sig_length)
#define fapi_get_data(mp_mbuf) (slsi_mbuf_get_data(mp_mbuf) + fapi_get_siglen(mp_mbuf))
#define fapi_get_vif(mp_mbuf) le16_to_cpu(((struct fapi_vif_signal_header *)slsi_mbuf_get_data(mp_mbuf))->vif)
/* Helper to get the struct slsi_80211_mgmt from the data */
#define fapi_get_mgmt(mp_mbuf) ((struct slsi_80211_mgmt *)fapi_get_data(mp_mbuf))
#define fapi_get_mgmtlen(mp_mbuf) fapi_get_datalen(mp_mbuf)
static inline u8 *fapi_append_data(struct max_buff *mbuf, const u8 *data, size_t data_len)
{
	u8 *p;

	/* Check if enough space is available at the tail of mbuf */
	if (WARN_ON((mbuf->buffer_len - (mbuf->data_offset + mbuf->data_len)) < data_len)) {
		return NULL;
	}

	p = mbuf_put(mbuf, data_len);
	mbuf->fapi.data_length += data_len;
	if (data) {
		memcpy(p, data, data_len);
	}
	return p;
}

static inline bool fapi_is_mlme(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SAP_TYPE_MASK) == FAPI_SAP_TYPE_MLME;
}

static inline bool fapi_is_ma(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SAP_TYPE_MASK) == FAPI_SAP_TYPE_MA;
}

static inline bool fapi_is_debug(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SAP_TYPE_MASK) == FAPI_SAP_TYPE_DEBUG;
}

static inline bool fapi_is_test(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SAP_TYPE_MASK) == FAPI_SAP_TYPE_TEST;
}

static inline bool fapi_is_req(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SIG_TYPE_MASK) == FAPI_SIG_TYPE_REQ;
}

static inline bool fapi_is_cfm(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SIG_TYPE_MASK) == FAPI_SIG_TYPE_CFM;
}

static inline bool fapi_is_res(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SIG_TYPE_MASK) == FAPI_SIG_TYPE_RES;
}

static inline bool fapi_is_ind(struct max_buff *mbuf)
{
	return (fapi_get_sigid(mbuf) & FAPI_SIG_TYPE_MASK) == FAPI_SIG_TYPE_IND;
}

static inline u16 fapi_get_expected_size_table(struct max_buff *mbuf, const u16 fapi_size_table[], size_t table_size, u16 start_id)
{
	u16 id = fapi_get_sigid(mbuf);

	if (id < start_id) {
		return 0;
	}

	if (id - start_id >= table_size) {
		return 0;
	}

	return fapi_size_table[id - start_id];
}

static inline u16 fapi_get_expected_size(struct max_buff *mbuf)
{
	static const u16 fapi_ma_req_size_table[] = {
		fapi_sig_size(ma_unitdata_req),
		fapi_sig_size(ma_spare_signal_1_req),
		fapi_sig_size(ma_spare_signal_2_req),
		fapi_sig_size(ma_spare_signal_3_req),
	};

	static const u16 fapi_mlme_req_size_table[] = {
		fapi_sig_size(mlme_get_req),
		fapi_sig_size(mlme_set_req),
		fapi_sig_size(mlme_powermgt_req),
		fapi_sig_size(mlme_add_info_elements_req),
		fapi_sig_size(mlme_add_scan_req),
		fapi_sig_size(mlme_del_scan_req),
		fapi_sig_size(mlme_add_vif_req),
		fapi_sig_size(mlme_del_vif_req),
		fapi_sig_size(mlme_start_req),
		fapi_sig_size(mlme_set_channel_req),
		fapi_sig_size(mlme_connect_req),
		fapi_sig_size(mlme_reassociate_req),
		fapi_sig_size(mlme_roam_req),
		fapi_sig_size(mlme_disconnect_req),
		fapi_sig_size(mlme_register_action_frame_req),
		fapi_sig_size(mlme_send_frame_req),
		fapi_sig_size(mlme_reset_dwell_time_req),
		fapi_sig_size(mlme_set_traffic_parameters_req),
		fapi_sig_size(mlme_del_traffic_parameters_req),
		fapi_sig_size(mlme_set_packet_filter_req),
		fapi_sig_size(mlme_set_ip_address_req),
		fapi_sig_size(mlme_set_acl_req),
		fapi_sig_size(mlme_set_roaming_parameters_req),
		fapi_sig_size(mlme_setkeys_req),
		0,
		fapi_sig_size(mlme_get_key_sequence_req),
		fapi_sig_size(mlme_set_cckm_keys_req),
		fapi_sig_size(mlme_set_pmk_req),
		fapi_sig_size(mlme_start_txrmc_req),
		fapi_sig_size(mlme_stop_txrmc_req),
		fapi_sig_size(mlme_set_cached_channels_req),
		fapi_sig_size(mlme_blockack_control_req),
		fapi_sig_size(mlme_tdls_action_req),
		fapi_sig_size(mlme_channel_switch_req),
		fapi_sig_size(mlme_monitor_rssi_req),
		fapi_sig_size(mlme_start_link_statistics_req),
		fapi_sig_size(mlme_stop_link_statistics_req),
		fapi_sig_size(mlme_set_bssid_hotlist_req),
		fapi_sig_size(mlme_set_pno_list_req),
		fapi_sig_size(mlme_spare_signal_1_req),
		fapi_sig_size(mlme_spare_signal_2_req),
		fapi_sig_size(mlme_spare_signal_3_req),
	};

	static const u16 fapi_debug_req_size_table[] = {
		fapi_sig_size(debug_generic_req),
		fapi_sig_size(debug_pkt_sink_start_req),
		fapi_sig_size(debug_pkt_sink_stop_req),
		fapi_sig_size(debug_pkt_sink_report_req),
		fapi_sig_size(debug_pkt_gen_start_req),
		fapi_sig_size(debug_pkt_gen_stop_req),
		fapi_sig_size(debug_pkt_gen_report_req),
		fapi_sig_size(debug_spare_signal_1_req),
		fapi_sig_size(debug_spare_signal_2_req),
		fapi_sig_size(debug_spare_signal_3_req),
	};

	static const u16 fapi_test_req_size_table[] = {
		fapi_sig_size(test_block_requests_req),
		fapi_sig_size(test_panic_req),
		fapi_sig_size(test_suspend_req),
		fapi_sig_size(test_resume_req),
		fapi_sig_size(radio_logging_req),
		fapi_sig_size(wlanlite_cw_start_req),
		fapi_sig_size(wlanlite_cw_stop_req),
		fapi_sig_size(wlanlite_tx_set_params_req),
		fapi_sig_size(wlanlite_tx_start_req),
		fapi_sig_size(wlanlite_tx_read_req),
		fapi_sig_size(wlanlite_tx_stop_req),
		fapi_sig_size(wlanlite_rx_start_req),
		fapi_sig_size(wlanlite_rx_read_req),
		fapi_sig_size(wlanlite_rx_stop_req),
		fapi_sig_size(wlanlite_status_req),
		fapi_sig_size(test_spare_signal_1_req),
		fapi_sig_size(test_spare_signal_2_req),
		fapi_sig_size(test_spare_signal_3_req),
	};

	static const u16 fapi_ma_cfm_size_table[] = {
		fapi_sig_size(ma_unitdata_cfm),
		fapi_sig_size(ma_spare_signal_1_cfm),
		fapi_sig_size(ma_spare_signal_2_cfm),
		fapi_sig_size(ma_spare_signal_3_cfm),
	};

	static const u16 fapi_mlme_cfm_size_table[] = {
		fapi_sig_size(mlme_get_cfm),
		fapi_sig_size(mlme_set_cfm),
		fapi_sig_size(mlme_powermgt_cfm),
		fapi_sig_size(mlme_add_info_elements_cfm),
		fapi_sig_size(mlme_add_scan_cfm),
		fapi_sig_size(mlme_del_scan_cfm),
		fapi_sig_size(mlme_add_vif_cfm),
		fapi_sig_size(mlme_del_vif_cfm),
		fapi_sig_size(mlme_start_cfm),
		fapi_sig_size(mlme_set_channel_cfm),
		fapi_sig_size(mlme_connect_cfm),
		fapi_sig_size(mlme_reassociate_cfm),
		fapi_sig_size(mlme_roam_cfm),
		fapi_sig_size(mlme_disconnect_cfm),
		fapi_sig_size(mlme_register_action_frame_cfm),
		fapi_sig_size(mlme_send_frame_cfm),
		fapi_sig_size(mlme_reset_dwell_time_cfm),
		fapi_sig_size(mlme_set_traffic_parameters_cfm),
		fapi_sig_size(mlme_del_traffic_parameters_cfm),
		fapi_sig_size(mlme_set_packet_filter_cfm),
		fapi_sig_size(mlme_set_ip_address_cfm),
		fapi_sig_size(mlme_set_acl_cfm),
		fapi_sig_size(mlme_set_roaming_parameters_cfm),
		fapi_sig_size(mlme_setkeys_cfm),
		0,
		fapi_sig_size(mlme_get_key_sequence_cfm),
		fapi_sig_size(mlme_set_cckm_keys_cfm),
		fapi_sig_size(mlme_set_pmk_cfm),
		fapi_sig_size(mlme_start_txrmc_cfm),
		fapi_sig_size(mlme_stop_txrmc_cfm),
		fapi_sig_size(mlme_set_cached_channels_cfm),
		fapi_sig_size(mlme_blockack_control_cfm),
		fapi_sig_size(mlme_tdls_action_cfm),
		fapi_sig_size(mlme_channel_switch_cfm),
		fapi_sig_size(mlme_monitor_rssi_cfm),
		fapi_sig_size(mlme_start_link_statistics_cfm),
		fapi_sig_size(mlme_stop_link_statistics_cfm),
		fapi_sig_size(mlme_set_bssid_hotlist_cfm),
		fapi_sig_size(mlme_set_pno_list_cfm),
		fapi_sig_size(mlme_spare_signal_1_cfm),
		fapi_sig_size(mlme_spare_signal_2_cfm),
		fapi_sig_size(mlme_spare_signal_3_cfm),
	};

	static const u16 fapi_debug_cfm_size_table[] = {
		fapi_sig_size(debug_generic_cfm),
		fapi_sig_size(debug_spare_signal_1_cfm),
		fapi_sig_size(debug_spare_signal_2_cfm),
		fapi_sig_size(debug_spare_signal_3_cfm),
	};

	static const u16 fapi_test_cfm_size_table[] = {
		fapi_sig_size(radio_logging_cfm),
		fapi_sig_size(wlanlite_cw_start_cfm),
		fapi_sig_size(wlanlite_tx_set_params_cfm),
		fapi_sig_size(wlanlite_cw_stop_cfm),
		fapi_sig_size(wlanlite_tx_start_cfm),
		fapi_sig_size(wlanlite_tx_read_cfm),
		fapi_sig_size(wlanlite_tx_stop_cfm),
		fapi_sig_size(wlanlite_rx_start_cfm),
		fapi_sig_size(wlanlite_rx_read_cfm),
		fapi_sig_size(wlanlite_rx_stop_cfm),
		fapi_sig_size(wlanlite_status_cfm),
		fapi_sig_size(test_spare_signal_1_cfm),
		fapi_sig_size(test_spare_signal_2_cfm),
		fapi_sig_size(test_spare_signal_3_cfm),
	};

	static const u16 fapi_ma_ind_size_table[] = {
		fapi_sig_size(ma_unitdata_ind),
		fapi_sig_size(ma_blockack_ind),
		fapi_sig_size(ma_tx_failure_ind),
		fapi_sig_size(ma_spare_signal_1_ind),
		fapi_sig_size(ma_spare_signal_2_ind),
		fapi_sig_size(ma_spare_signal_3_ind),
	};

	static const u16 fapi_mlme_ind_size_table[] = {
		fapi_sig_size(mlme_scan_ind),
		fapi_sig_size(mlme_scan_done_ind),
		fapi_sig_size(mlme_listen_end_ind),
		fapi_sig_size(mlme_connect_ind),
		fapi_sig_size(mlme_connected_ind),
		fapi_sig_size(mlme_reassociate_ind),
		fapi_sig_size(mlme_roam_ind),
		fapi_sig_size(mlme_roamed_ind),
		fapi_sig_size(mlme_disconnect_ind),
		fapi_sig_size(mlme_disconnected_ind),
		fapi_sig_size(mlme_procedure_started_ind),
		fapi_sig_size(mlme_mic_failure_ind),
		fapi_sig_size(mlme_frame_transmission_ind),
		fapi_sig_size(mlme_received_frame_ind),
		fapi_sig_size(mlme_rmc_leader_selected_ind),
		fapi_sig_size(mlme_tdls_peer_ind),
		fapi_sig_size(mlme_ap_loss_ind),
		fapi_sig_size(mlme_significant_change_ind),
		fapi_sig_size(mlme_rssi_report_ind),
		fapi_sig_size(mlme_ac_priority_update_ind),
		fapi_sig_size(mlme_spare_signal_1_ind),
		fapi_sig_size(mlme_spare_signal_2_ind),
		fapi_sig_size(mlme_spare_signal_3_ind),
	};

	static const u16 fapi_debug_ind_size_table[] = {
		fapi_sig_size(debug_word12_ind),
		fapi_sig_size(debug_fault_ind),
		fapi_sig_size(debug_generic_ind),
		fapi_sig_size(debug_pkt_sink_report_ind),
		fapi_sig_size(debug_pkt_gen_report_ind),
		fapi_sig_size(debug_spare_signal_1_ind),
		fapi_sig_size(debug_spare_signal_2_ind),
		fapi_sig_size(debug_spare_signal_3_ind),
	};

	static const u16 fapi_test_ind_size_table[] = {
		fapi_sig_size(radio_logging_ind),
		fapi_sig_size(test_spare_signal_1_ind),
		fapi_sig_size(test_spare_signal_2_ind),
		fapi_sig_size(test_spare_signal_3_ind),
	};

	static const u16 fapi_mlme_res_size_table[] = {
		fapi_sig_size(mlme_connect_res),
		fapi_sig_size(mlme_connected_res),
		fapi_sig_size(mlme_reassociate_res),
		fapi_sig_size(mlme_roamed_res),
		fapi_sig_size(mlme_tdls_peer_res),
		fapi_sig_size(mlme_spare_signal_1_res),
		fapi_sig_size(mlme_spare_signal_2_res),
		fapi_sig_size(mlme_spare_signal_3_res),
	};

	static const u16 fapi_ma_res_size_table[] = {
		fapi_sig_size(ma_spare_signal_1_res),
		fapi_sig_size(ma_spare_signal_2_res),
		fapi_sig_size(ma_spare_signal_3_res),
	};

	static const u16 fapi_debug_res_size_table[] = {
		fapi_sig_size(debug_spare_signal_1_res),
		fapi_sig_size(debug_spare_signal_2_res),
		fapi_sig_size(debug_spare_signal_3_res),
	};

	static const u16 fapi_test_res_size_table[] = {
		fapi_sig_size(test_spare_signal_1_res),
		fapi_sig_size(test_spare_signal_2_res),
		fapi_sig_size(test_spare_signal_3_res),
	};

	if (fapi_is_mlme(mbuf) && fapi_is_req(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_mlme_req_size_table, ARRAY_SIZE(fapi_mlme_req_size_table), MLME_GET_REQ);
	} else if (fapi_is_mlme(mbuf) && fapi_is_cfm(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_mlme_cfm_size_table, ARRAY_SIZE(fapi_mlme_cfm_size_table), MLME_GET_CFM);
	} else if (fapi_is_mlme(mbuf) && fapi_is_res(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_mlme_res_size_table, ARRAY_SIZE(fapi_mlme_res_size_table), MLME_CONNECT_RES);
	} else if (fapi_is_mlme(mbuf) && fapi_is_ind(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_mlme_ind_size_table, ARRAY_SIZE(fapi_mlme_ind_size_table), MLME_SCAN_IND);
	} else if (fapi_is_ma(mbuf) && fapi_is_req(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_ma_req_size_table, ARRAY_SIZE(fapi_ma_req_size_table), MA_UNITDATA_REQ);
	} else if (fapi_is_ma(mbuf) && fapi_is_cfm(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_ma_cfm_size_table, ARRAY_SIZE(fapi_ma_cfm_size_table), MA_UNITDATA_CFM);
	} else if (fapi_is_ma(mbuf) && fapi_is_ind(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_ma_ind_size_table, ARRAY_SIZE(fapi_ma_ind_size_table), MA_UNITDATA_IND);
	} else if (fapi_is_debug(mbuf) && fapi_is_req(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_debug_req_size_table, ARRAY_SIZE(fapi_debug_req_size_table), DEBUG_GENERIC_REQ);
	} else if (fapi_is_debug(mbuf) && fapi_is_cfm(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_debug_cfm_size_table, ARRAY_SIZE(fapi_debug_cfm_size_table), DEBUG_GENERIC_CFM);
	} else if (fapi_is_debug(mbuf) && fapi_is_ind(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_debug_ind_size_table, ARRAY_SIZE(fapi_debug_ind_size_table), DEBUG_WORD12IND);
	} else if (fapi_is_test(mbuf) && fapi_is_req(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_test_req_size_table, ARRAY_SIZE(fapi_test_req_size_table), TEST_BLOCK_REQUESTS_REQ);
	} else if (fapi_is_test(mbuf) && fapi_is_cfm(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_test_cfm_size_table, ARRAY_SIZE(fapi_test_cfm_size_table), RADIO_LOGGING_CFM);
	} else if (fapi_is_test(mbuf) && fapi_is_ind(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_test_ind_size_table, ARRAY_SIZE(fapi_test_ind_size_table), RADIO_LOGGING_IND);
	} else if (fapi_is_mlme(mbuf) && fapi_is_res(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_ma_res_size_table, ARRAY_SIZE(fapi_ma_res_size_table), MA_SPARE_SIGNAL_1_RES);
	} else if (fapi_is_mlme(mbuf) && fapi_is_res(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_debug_res_size_table, ARRAY_SIZE(fapi_debug_res_size_table), DEBUG_SPARE_SIGNAL_1_RES);
	} else if (fapi_is_mlme(mbuf) && fapi_is_res(mbuf)) {
		return fapi_get_expected_size_table(mbuf, fapi_test_res_size_table, ARRAY_SIZE(fapi_test_res_size_table), TEST_SPARE_SIGNAL_1_RES);
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
#endif							/* _FAPI_H__ */
