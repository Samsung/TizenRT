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

#ifndef SLSI_MIB_H__
#define SLSI_MIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>

struct slsi_mib_data {
	u32 dataLength;
	u8 *data;
};

#define SLSI_MIB_MAX_INDEXES 2

#define SLSI_MIB_TYPE_BOOL    0
#define SLSI_MIB_TYPE_UINT    1
#define SLSI_MIB_TYPE_INT     2
#define SLSI_MIB_TYPE_OCTET   3
#define SLSI_MIB_TYPE_NONE    4

struct slsi_mib_value {
	u8 type;
	union {
		bool boolValue;
		s32 intValue;
		u32 uintValue;
		struct slsi_mib_data octetValue;
	} u;
};

struct slsi_mib_entry {
	u16 psid;
	u16 index[SLSI_MIB_MAX_INDEXES];	/* 0 = no Index */
	struct slsi_mib_value value;
};

struct slsi_mib_get_entry {
	u16 psid;
	u16 index[SLSI_MIB_MAX_INDEXES];	/* 0 = no Index */
};

#define SLSI_MIB_STATUS_SUCCESS                     0x0000
#define SLSI_MIB_STATUS_UNKNOWN_PSID                0x0001
#define SLSI_MIB_STATUS_INVALID_INDEX               0x0002
#define SLSI_MIB_STATUS_OUT_OF_RANGE                0x0003
#define SLSI_MIB_STATUS_WRITE_ONLY                  0x0004
#define SLSI_MIB_STATUS_READ_ONLY                   0x0005
#define SLSI_MIB_STATUS_UNKNOWN_INTERFACE_TAG       0x0006
#define SLSI_MIB_STATUS_INVALID_NUMBER_OF_INDICES   0x0007
#define SLSI_MIB_STATUS_ERROR                       0x0008
#define SLSI_MIB_STATUS_UNSUPPORTED_ON_INTERFACE    0x0009
#define SLSI_MIB_STATUS_UNAVAILABLE                 0x000A
#define SLSI_MIB_STATUS_NOT_FOUND                   0x000B
#define SLSI_MIB_STATUS_INCOMPATIBLE                0x000C
#define SLSI_MIB_STATUS_OUT_OF_MEMORY               0x000D
#define SLSI_MIB_STATUS_TO_MANY_REQUESTED_VARIABLES 0x000E
#define SLSI_MIB_STATUS_NOT_TRIED                   0x000F
#define SLSI_MIB_STATUS_FAILURE                     0xFFFF

/*******************************************************************************
 *
 * NAME
 *  slsi_mib_encode_get Functions
 *
 * DESCRIPTION
 *  For use when getting data from the Wifi Stack.
 *  These functions append the encoded data to the "buffer".
 *
 *  index == 0 where there is no index required
 *
 * EXAMPLE
 *  {
 *      static const struct slsi_mib_get_entry getValues[] = {
 *          { PSID1, { 0, 0 } },
 *          { PSID2, { 3, 0 } },
 *      };
 *      struct slsi_mib_data buffer;
 *      slsi_mib_encode_get_list(&buffer,
 *                              sizeof(getValues) / sizeof(struct slsi_mib_get_entry),
 *                              getValues);
 *  }
 *  or
 *  {
 *      struct slsi_mib_data buffer = {0, NULL};
 *      slsi_mib_encode_get(&buffer, PSID1, 0);
 *      slsi_mib_encode_get(&buffer, PSID2, 3);
 *  }
 * RETURN
 *  SlsiResult: See SLSI_MIB_STATUS_*
 *
 *******************************************************************************/
void slsi_mib_encode_get(struct slsi_mib_data *buffer, u16 psid, u16 index);
int slsi_mib_encode_get_list(struct slsi_mib_data *buffer, u16 psidsLength, const struct slsi_mib_get_entry *psids);

/*******************************************************************************
 *
 * NAME
 *  SlsiWifiMibdEncode Functions
 *
 * DESCRIPTION
 *  For use when getting data from the Wifi Stack.
 *
 *  index == 0 where there is no index required
 *
 * EXAMPLE
 *  {
 *      static const struct slsi_mib_get_entry getValues[] = {
 *          { PSID1, { 0, 0 } },
 *          { PSID2, { 3, 0 } },
 *      };
 *      struct slsi_mib_data buffer = rxMibData; # Buffer with encoded Mib Data
 *
 *      getValues = slsi_mib_decode_get_list(&buffer,
 *                                      sizeof(getValues) / sizeof(struct slsi_mib_get_entry),
 *                                      getValues);
 *
 *      print("PSID1 = %d\n", getValues[0].u.uintValue);
 *      print("PSID2.3 = %s\n", getValues[1].u.boolValue?"TRUE":"FALSE");
 *
 *      kmm_free(getValues);
 *
 *  }
 *  or
 *  {
 *      u8* buffer = rxMibData; # Buffer with encoded Mib Data
 *      size_t offset=0;
 *      struct slsi_mib_entry value;
 *
 *      offset += slsi_mib_decode(&buffer[offset], &value);
 *      print("PSID1 = %d\n", value.u.uintValue);
 *
 *      offset += slsi_mib_decode(&buffer[offset], &value);
 *      print("PSID2.3 = %s\n", value.u.boolValue?"TRUE":"FALSE");
 *
 *  }
 *
 *******************************************************************************/
size_t slsi_mib_decode(struct slsi_mib_data *buffer, struct slsi_mib_entry *value);
struct slsi_mib_value *slsi_mib_decode_get_list(struct slsi_mib_data *buffer, u16 psidsLength, const struct slsi_mib_get_entry *psids);

/*******************************************************************************
 *
 * NAME
 *  slsi_mib_encode Functions
 *
 * DESCRIPTION
 *  For use when setting data in the Wifi Stack.
 *  These functions append the encoded data to the "buffer".
 *
 *  index == 0 where there is no index required
 *
 * EXAMPLE
 *  {
 *      u8 octets[2] = {0x00, 0x01};
 *      struct slsi_mib_data buffer = {0, NULL};
 *      slsi_mib_encode_bool(&buffer, PSID1, TRUE, 0);                     # Boolean set with no index
 *      slsi_mib_encode_int(&buffer, PSID2, -1234, 1);                     # Signed Integer set with on index 1
 *      slsi_mib_encode_uint(&buffer, PSID2, 1234, 3);                     # Unsigned Integer set with on index 3
 *      slsi_mib_encode_octet(&buffer, PSID3, sizeof(octets), octets, 0);  # Octet set with no index
 *  }
 *  or
 *  {
 # Unsigned Integer set with on index 3
 #      struct slsi_mib_data buffer = {0, NULL};
 #      struct slsi_mib_entry value;
 #      value.psid = psid;
 #      value.index[0] = 3;
 #      value.index[1] = 0;
 #      value.value.type = SLSI_MIB_TYPE_UINT;
 #      value.value.u.uintValue = 1234;
 #      slsi_mib_encode(buffer, &value);
 #  }
 # RETURN
 #  See SLSI_MIB_STATUS_*
 #
 *******************************************************************************/
u16 slsi_mib_encode(struct slsi_mib_data *buffer, struct slsi_mib_entry *value);
u16 slsi_mib_encode_bool(struct slsi_mib_data *buffer, u16 psid, bool value, u16 index);
u16 slsi_mib_encode_int(struct slsi_mib_data *buffer, u16 psid, s32 value, u16 index);
u16 slsi_mib_encode_uint(struct slsi_mib_data *buffer, u16 psid, u32 value, u16 index);
u16 slsi_mib_encode_octet(struct slsi_mib_data *buffer, u16 psid, size_t dataLength, const u8 *data, u16 index);

/*******************************************************************************
 *
 * NAME
 *  SlsiWifiMib Low level Encode/Decode functions
 *
 *******************************************************************************/
size_t slsi_mib_encode_uint32(u8 *buffer, u32 value);
size_t slsi_mib_encode_int32(u8 *buffer, s32 signedValue);
size_t slsi_mib_encode_octet_str(u8 *buffer, struct slsi_mib_data *octetValue);

size_t slsi_mib_decodeUint32(u8 *buffer, u32 *value);
size_t slsi_mib_decodeInt32(u8 *buffer, s32 *value);
size_t slsi_mib_decode_octet_str(u8 *buffer, struct slsi_mib_data *octetValue);

/*******************************************************************************
 *
 * NAME
 *  SlsiWifiMib Helper Functions
 *
 *******************************************************************************/

/* Find a the offset to psid data in an encoded buffer
 * {
 *      struct slsi_mib_data buffer = rxMibData;                 # Buffer with encoded Mib Data
 *      struct slsi_mib_get_entry value = {PSID1, {0x01, 0x00}};   # Find value for PSID1.1
 *      u8* mibdata = slsi_mib_find(&buffer, &value);
 *      if(mibdata) {print("Mib Data for PSID1.1 Found\n");
 *  }
 */
u8 *slsi_mib_find(struct slsi_mib_data *buffer, const struct slsi_mib_get_entry *entry);

/* Append data to a Buffer */
void slsi_mib_buf_append(struct slsi_mib_data *dst, size_t bufferLength, u8 *buffer);

/*******************************************************************************
 *
 * PSID Definitions
 *
 *******************************************************************************/

/*******************************************************************************
 * NAME          : Dot11TdlsPeerUapsdIndicationWindow
 * PSID          : 53 (0x0035)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : beacon intervals
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  The minimum time that needs to pass after the most recent TPU SP, before
 *  a RAME_TPU_SP indication can be sent to MLME
 *******************************************************************************/
#define SLSI_PSID_DOT11_TDLS_PEER_UAPSD_INDICATION_WINDOW 0x0035

/*******************************************************************************
 * NAME          : Dot11AssociationSaQueryMaximumTimeout
 * PSID          : 100 (0x0064)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 1000
 * DESCRIPTION   :
 *  This attribute specifies the number of time units (TUs) that an AP can
 *  wait, from the scheduling of the first SA Query Request to allow
 *  association process to be started without starting additional SA Query
 *  procedure if a successful SA Query Response is not received.
 *******************************************************************************/
#define SLSI_PSID_DOT11_ASSOCIATION_SA_QUERY_MAXIMUM_TIMEOUT 0x0064

/*******************************************************************************
 * NAME          : Dot11AssociationSaQueryRetryTimeout
 * PSID          : 101 (0x0065)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 201
 * DESCRIPTION   :
 *  This attribute specifies the number of time units (TUs) that an AP waits
 *  between issuing two subsequent SA Query Request frames.
 *******************************************************************************/
#define SLSI_PSID_DOT11_ASSOCIATION_SA_QUERY_RETRY_TIMEOUT 0x0065

/*******************************************************************************
 * NAME          : Dot11PowerCapabilityMaxImplemented
 * PSID          : 112 (0x0070)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute indicates the maximum transmit Power Capability of this
 *  station.
 *******************************************************************************/
#define SLSI_PSID_DOT11_POWER_CAPABILITY_MAX_IMPLEMENTED 0x0070

/*******************************************************************************
 * NAME          : Dot11PowerCapabilityMinImplemented
 * PSID          : 113 (0x0071)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute indicates the minimum transmit Power Capability of this
 *  station.
 *******************************************************************************/
#define SLSI_PSID_DOT11_POWER_CAPABILITY_MIN_IMPLEMENTED 0x0071

/*******************************************************************************
 * NAME          : Dot11RtsThreshold
 * PSID          : 121 (0x0079)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : octet
 * MIN           : 0
 * MAX           : 65536
 * DEFAULT       : 9000
 * DESCRIPTION   :
 *  This attribute shall indicate the size of an MPDU, below which an RTS/CTS
 *  handshake shall not be performed, except as RTS/CTS is used as a cross
 *  modulation protection mechanism as defined in 9.10. An RTS/CTS handshake
 *  shall be performed at the beginning of any frame exchange sequence where
 *  the MPDU is of type Data or Management, the MPDU has an individual
 *  address in the Address1 field, and the length of the MPDU is greater than
 *  this threshold. (For additional details, refer to Table 21 in 9.7.)
 *  Setting this attribute to be larger than the maximum MSDU size shall have
 *  the effect of turning off the RTS/CTS handshake for frames of Data or
 *  Management type transmitted by this STA. Setting this attribute to zero
 *  shall have the effect of turning on the RTS/CTS handshake for all frames
 *  of Data or Management type transmitted by this STA.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RTS_THRESHOLD 0x0079

/*******************************************************************************
 * NAME          : Dot11ShortRetryLimit
 * PSID          : 122 (0x007A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 255
 * DEFAULT       : 15
 * DESCRIPTION   :
 *  This attribute shall indicate the maximum number of transmission attempts
 *  of a frame, the length of which is less than or equal to
 *  dot11RTSThreshold, that shall be made before a failure condition is
 *  indicated.
 *******************************************************************************/
#define SLSI_PSID_DOT11_SHORT_RETRY_LIMIT 0x007A

/*******************************************************************************
 * NAME          : Dot11LongRetryLimit
 * PSID          : 123 (0x007B)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 255
 * DEFAULT       : 4
 * DESCRIPTION   :
 *  This attribute shall indicate the maximum number of transmission attempts
 *  of a frame, the length of which is greater than dot11RTSThreshold, that
 *  shall be made before a failure condition is indicated.
 *******************************************************************************/
#define SLSI_PSID_DOT11_LONG_RETRY_LIMIT 0x007B

/*******************************************************************************
 * NAME          : Dot11FragmentationThreshold
 * PSID          : 124 (0x007C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 256
 * MAX           : 11500
 * DEFAULT       : 3000
 * DESCRIPTION   :
 *  This attribute specifies the current maximum size, in octets, of the MPDU
 *  that may be delivered to the security encapsulation. This maximum size
 *  does not apply when an MSDU is transmitted using an HT-immediate or
 *  HTdelayed Block Ack agreement, or when an MSDU or MMPDU is carried in an
 *  AMPDU that does not contain a VHT single MPDU. Fields added to the frame
 *  by security encapsulation are not counted against the limit specified by
 *  this attribute. Except as described above, an MSDU or MMPDU is fragmented
 *  when the resulting frame has an individual address in the Address1 field,
 *  and the length of the frame is larger than this threshold, excluding
 *  security encapsulation fields. The default value for this attribute is
 *  the lesser of 11500 or the aMPDUMaxLength or the aPSDUMaxLength of the
 *  attached PHY and the value never exceeds the lesser of 11500 or the
 *  aMPDUMaxLength or the aPSDUMaxLength of the attached PHY.
 *******************************************************************************/
#define SLSI_PSID_DOT11_FRAGMENTATION_THRESHOLD 0x007C

/*******************************************************************************
 * NAME          : Dot11MaxTransmitMsduLifetime
 * PSID          : 125 (0x007D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 512
 * DESCRIPTION   :
 *  The elapsed time, after the initial transmission of an MSDU, after which
 *  further attempts to transmit the MSDU shall be terminated.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MAX_TRANSMIT_MSDU_LIFETIME 0x007D

/*******************************************************************************
 * NAME          : Dot11MaxReceiveLifetime
 * PSID          : 126 (0x007E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 512
 * DESCRIPTION   :
 *  The elapsed time, after the initial reception of a fragmented MMPDU or
 *  MSDU, after which further attempts to reassemble the MMPDU or MSDU shall
 *  be terminated.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MAX_RECEIVE_LIFETIME 0x007E

/*******************************************************************************
 * NAME          : Dot11TransmittedFragmentCount
 * PSID          : 140 (0x008C)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall be incremented for an acknowledged MPDU with an
 *  individual address in the address 1 field or an MPDU with a multicast
 *  address in the address 1 field of type Data or Management.
 *******************************************************************************/
#define SLSI_PSID_DOT11_TRANSMITTED_FRAGMENT_COUNT 0x008C

/*******************************************************************************
 * NAME          : Dot11MulticastTransmittedFrameCount
 * PSID          : 141 (0x008D)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment only when the multicast bit is set in the
 *  destination MAC address of a successfully transmitted MSDU. When
 *  operating as a STA in an ESS, where these frames are directed to the AP,
 *  this implies having received an acknowledgment to all associated MPDUs.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MULTICAST_TRANSMITTED_FRAME_COUNT 0x008D

/*******************************************************************************
 * NAME          : Dot11FailedCount
 * PSID          : 142 (0x008E)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when an MSDU is not transmitted successfully
 *  due to the number of transmit attempts exceeding either the
 *  dot11ShortRetryLimit or dot11LongRetryLimit.
 *******************************************************************************/
#define SLSI_PSID_DOT11_FAILED_COUNT 0x008E

/*******************************************************************************
 * NAME          : Dot11RetryCount
 * PSID          : 143 (0x008F)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when an MSDU is successfully transmitted
 *  after one or more retransmissions.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RETRY_COUNT 0x008F

/*******************************************************************************
 * NAME          : Dot11MultipleRetryCount
 * PSID          : 144 (0x0090)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when an MSDU is successfully transmitted
 *  after more than one retransmission.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MULTIPLE_RETRY_COUNT 0x0090

/*******************************************************************************
 * NAME          : Dot11FrameDuplicateCount
 * PSID          : 145 (0x0091)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when a frame is received that the Sequence
 *  Control field indicates is a duplicate.
 *******************************************************************************/
#define SLSI_PSID_DOT11_FRAME_DUPLICATE_COUNT 0x0091

/*******************************************************************************
 * NAME          : Dot11RtsSuccessCount
 * PSID          : 146 (0x0092)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when a CTS is received in response to an
 *  RTS.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RTS_SUCCESS_COUNT 0x0092

/*******************************************************************************
 * NAME          : Dot11RtsFailureCount
 * PSID          : 147 (0x0093)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when a CTS is not received in response to an
 *  RTS.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RTS_FAILURE_COUNT 0x0093

/*******************************************************************************
 * NAME          : Dot11ReceivedFragmentCount
 * PSID          : 149 (0x0095)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall be incremented for each successfully received MPDU of
 *  type Data or Management.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RECEIVED_FRAGMENT_COUNT 0x0095

/*******************************************************************************
 * NAME          : Dot11MulticastReceivedFrameCount
 * PSID          : 150 (0x0096)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when a MSDU is received with the multicast
 *  bit set in the destination MAC address.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MULTICAST_RECEIVED_FRAME_COUNT 0x0096

/*******************************************************************************
 * NAME          : Dot11FcsErrorCount
 * PSID          : 151 (0x0097)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : -9223372036854775808
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when an FCS error is detected in a received
 *  MPDU.
 *******************************************************************************/
#define SLSI_PSID_DOT11_FCS_ERROR_COUNT 0x0097

/*******************************************************************************
 * NAME          : Dot11TransmittedFrameCount
 * PSID          : 152 (0x0098)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment for each successfully transmitted MSDU.
 *******************************************************************************/
#define SLSI_PSID_DOT11_TRANSMITTED_FRAME_COUNT 0x0098

/*******************************************************************************
 * NAME          : Dot11WepUndecryptableCount
 * PSID          : 153 (0x0099)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when a frame is received with the WEP
 *  subfield of the Frame Control field set to one and the WEPOn value for
 *  the key mapped to the transmitter&apos;s MAC address indicates that the
 *  frame should not have been encrypted or that frame is discarded due to
 *  the receiving STA not implementing the privacy option.
 *******************************************************************************/
#define SLSI_PSID_DOT11_WEP_UNDECRYPTABLE_COUNT 0x0099

/*******************************************************************************
 * NAME          : Dot11ManufacturerProductVersion
 * PSID          : 183 (0x00B7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 128
 * DEFAULT       :
 * DESCRIPTION   :
 *  Printable string used to identify the manufacturer&apos;s product version
 *  of the resource.
 *******************************************************************************/
#define SLSI_PSID_DOT11_MANUFACTURER_PRODUCT_VERSION 0x00B7

/*******************************************************************************
 * NAME          : Dot11RsnaStatsStaAddress
 * PSID          : 430 (0x01AE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The MAC address of the STA to which the statistics in this conceptual row
 *  belong.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_STA_ADDRESS 0x01AE

/*******************************************************************************
 * NAME          : Dot11RsnaStatsTkipicvErrors
 * PSID          : 433 (0x01B1)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Counts the number of TKIP ICV errors encountered when decrypting packets
 *  for the STA.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_TKIPICV_ERRORS 0x01B1

/*******************************************************************************
 * NAME          : Dot11RsnaStatsTkipLocalMicFailures
 * PSID          : 434 (0x01B2)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Counts the number of MIC failures encountered when checking the integrity
 *  of packets received from the STA at this entity.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_TKIP_LOCAL_MIC_FAILURES 0x01B2

/*******************************************************************************
 * NAME          : Dot11RsnaStatsTkipRemoteMicFailures
 * PSID          : 435 (0x01B3)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Counts the number of MIC failures encountered by the STA identified by
 *  dot11RSNAStatsSTAAddress and reported back to this entity.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_TKIP_REMOTE_MIC_FAILURES 0x01B3

/*******************************************************************************
 * NAME          : Dot11RsnaStatsCcmpReplays
 * PSID          : 436 (0x01B4)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of received CCMP MPDUs discarded by the replay mechanism.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_CCMP_REPLAYS 0x01B4

/*******************************************************************************
 * NAME          : Dot11RsnaStatsCcmpDecryptErrors
 * PSID          : 437 (0x01B5)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of received MPDUs discarded by the CCMP decryption algorithm.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_CCMP_DECRYPT_ERRORS 0x01B5

/*******************************************************************************
 * NAME          : Dot11RsnaStatsTkipReplays
 * PSID          : 438 (0x01B6)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Counts the number of TKIP replay errors detected.
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_TKIP_REPLAYS 0x01B6

/*******************************************************************************
 * NAME          : Dot11RsnaStatsRobustMgmtCcmpReplays
 * PSID          : 441 (0x01B9)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of received Robust Management frame MPDUs discarded due to
 *  CCMP replay errors
 *******************************************************************************/
#define SLSI_PSID_DOT11_RSNA_STATS_ROBUST_MGMT_CCMP_REPLAYS 0x01B9

/*******************************************************************************
 * NAME          : UnifiMlmeConnectionTimeout
 * PSID          : 2000 (0x07D0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : milliseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 2000
 * DESCRIPTION   :
 *  Firmware waits for unifiMLMEConnectionTimeOut of no successful Tx/Rx
 *  (including beacon) to/from AP before it disconnects from AP.For STA case
 *  - Setting it to less than 3 seconds may result in frequent disconnection
 *  with the AP
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_CONNECTION_TIMEOUT 0x07D0

/*******************************************************************************
 * NAME          : UnifiMlmeScanChannelMaxScanTime
 * PSID          : 2001 (0x07D1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 14
 * MAX           : 14
 * DEFAULT       : { 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  For testing: overrides max_scan_time. 0 indicates not used.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_CHANNEL_MAX_SCAN_TIME 0x07D1

/*******************************************************************************
 * NAME          : UnifiMlmeScanChannelProbeInterval
 * PSID          : 2002 (0x07D2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 14
 * MAX           : 14
 * DEFAULT       : { 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  For testing: overrides probe interval. 0 indicates not used.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_CHANNEL_PROBE_INTERVAL 0x07D2

/*******************************************************************************
 * NAME          : UnifiMlmeDataReferenceTimeout
 * PSID          : 2005 (0x07D5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65534
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute specifies the maximum time allowed for the data in data
 *  references corresponding to MLME primitives to be made available to the
 *  firmware. The special value 0 specifies an infinite timeout.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_DATA_REFERENCE_TIMEOUT 0x07D5

/*******************************************************************************
 * NAME          : UnifiMlmeScanProbeInterval
 * PSID          : 2007 (0x07D7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 70
 * DESCRIPTION   :
 *  This attribute specifies the time between transmissions of broadcast
 *  probe requests on a given channel when performing an active scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_PROBE_INTERVAL 0x07D7

/*******************************************************************************
 * NAME          : UnifiMlmeScanHighRssiThreshold
 * PSID          : 2008 (0x07D8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       : -100
 * DESCRIPTION   :
 *  This attribute specifies the minimum RSSI necessary for a new station to
 *  enter the coverage area of scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_HIGH_RSSI_THRESHOLD 0x07D8

/*******************************************************************************
 * NAME          : UnifiMlmeScanDeltaRssiThreshold
 * PSID          : 2010 (0x07DA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : dB
 * MIN           : 1
 * MAX           : 255
 * DEFAULT       : 20
 * DESCRIPTION   :
 *  This attribute specifies the magnitude of the change in RSSI for which a
 *  scan result will be issued
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_DELTA_RSSI_THRESHOLD 0x07DA

/*******************************************************************************
 * NAME          : UnifiMlmeScanHighSnrThreshold
 * PSID          : 2011 (0x07DB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dB
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       : -100
 * DESCRIPTION   :
 *  This attribute specifies the minimum SNR necessary for a new station to
 *  enter the coverage area of scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_HIGH_SNR_THRESHOLD 0x07DB

/*******************************************************************************
 * NAME          : UnifiMlmeScanDeltaSnrThreshold
 * PSID          : 2013 (0x07DD)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : dB
 * MIN           : 1
 * MAX           : 255
 * DEFAULT       : 255
 * DESCRIPTION   :
 *  This attribute specifies the magnitude of the change in SNR for a station
 *  in scan for which a scan result will be issued
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_DELTA_SNR_THRESHOLD 0x07DD

/*******************************************************************************
 * NAME          : UnifiMlmeScanMaximumAge
 * PSID          : 2014 (0x07DE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 1
 * MAX           : 2147
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  Not supported
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_MAXIMUM_AGE 0x07DE

/*******************************************************************************
 * NAME          : UnifiMlmeScanMaximumResults
 * PSID          : 2015 (0x07DF)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 40
 * DESCRIPTION   :
 *  This attribute specifies the maximum number of scan results (for all
 *  scans) which will be stored before the oldest result is discarded,
 *  irrespective of its age. The value 0 specifies no maximum.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_MAXIMUM_RESULTS 0x07DF

/*******************************************************************************
 * NAME          : UnifiMlmeAutonomousScanNoisy
 * PSID          : 2016 (0x07E0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  Not supported
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_AUTONOMOUS_SCAN_NOISY 0x07E0

/*******************************************************************************
 * NAME          : UnifiFirmwareBuildId
 * PSID          : 2021 (0x07E5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Numeric build identifier for this firmware build. This should normally be
 *  displayed in decimal. The textual build identifier is available via the
 *  standard dot11manufacturerProductVersion MIB attribute.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FIRMWARE_BUILD_ID 0x07E5

/*******************************************************************************
 * NAME          : UnifiChipVersion
 * PSID          : 2022 (0x07E6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Numeric identifier for the UniFi silicon revision (as returned by the
 *  GBL_CHIP_VERSION hardware register). Other than being different for each
 *  design variant (but not for alternative packaging options), the
 *  particular values returned do not have any significance.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CHIP_VERSION 0x07E6

/*******************************************************************************
 * NAME          : UnifiFirmwarePatchBuildId
 * PSID          : 2023 (0x07E7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Numeric build identifier for the patch set that has been applied to this
 *  firmware image. This should normally be displayed in decimal. For a
 *  patched ROM build there will be two build identifiers, the first will
 *  correspond to the base ROM image, the second will correspond to the patch
 *  set that has been applied.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FIRMWARE_PATCH_BUILD_ID 0x07E7

/*******************************************************************************
 * NAME          : UnifiBasicCapabilities
 * PSID          : 2030 (0x07EE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 0X0730
 * DESCRIPTION   :
 *  This MIB variable indicates basis capabilities of the chip. The 16-bit
 *  field follows the coding of IEEE 802.11 Capability Information.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_BASIC_CAPABILITIES 0x07EE

/*******************************************************************************
 * NAME          : UnifiExtendedCapabilities
 * PSID          : 2031 (0x07EF)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 9
 * MAX           : 9
 * DEFAULT       : { 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  This MIB variable indicates extended capabilities of the chip. Bit field
 *  definition and coding follows IEEE 802.11 Extended Capability Information
 *  Element, with spare subfields for capabilities that are independent from
 *  chip/firmware implementation.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_EXTENDED_CAPABILITIES 0x07EF

/*******************************************************************************
 * NAME          : UnifiHtCapabilities
 * PSID          : 2032 (0x07F0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 21
 * MAX           : 21
 * DEFAULT       : { 0X7E, 0X09, 0X1F, 0XFF, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  This MIB variable indicates the HT capabilities of the chip. See
 *  SC-503520-SP for further details.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_HT_CAPABILITIES 0x07F0

/*******************************************************************************
 * NAME          : UnifiRsnCapabilities
 * PSID          : 2034 (0x07F2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 0X248C
 * DESCRIPTION   :
 *  This MIB variable encodes the RSN Capabilities field of IEEE 802.11 RSN
 *  Information Element.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSN_CAPABILITIES 0x07F2

/*******************************************************************************
 * NAME          : Unifi24G40MhzChannels
 * PSID          : 2035 (0x07F3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  This attribute, when TRUE, enables 40Mz wide channels in the 2.4G band
 *******************************************************************************/
#define SLSI_PSID_UNIFI24_G40_MHZ_CHANNELS 0x07F3

/*******************************************************************************
 * NAME          : UnifiWapiSupported
 * PSID          : 2039 (0x07F7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  This is populated through efuse read.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_WAPI_SUPPORTED 0x07F7

/*******************************************************************************
 * NAME          : UnifiSupportedDataRates
 * PSID          : 2041 (0x07F9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * UNITS         : 500 kbps
 * MIN           : 2
 * MAX           : 16
 * DEFAULT       : { 0X02, 0X04, 0X0B, 0X0C, 0X12, 0X16, 0X18, 0X24, 0X30, 0X48, 0X60, 0X6C }
 * DESCRIPTION   :
 *  Defines the supported non-HT data rates. It is encoded as N+1 octets
 *  where the first octet is N and the subsequent octets each describe a
 *  single supported rate.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SUPPORTED_DATA_RATES 0x07F9

/*******************************************************************************
 * NAME          : UnifiSupportedCipherSuites
 * PSID          : 2042 (0x07FA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 0X3F11
 * DESCRIPTION   :
 *  This MIB variable encodes both the supported cipher suite of IEEE 802.11
 *  RSN Information Element.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SUPPORTED_CIPHER_SUITES 0x07FA

/*******************************************************************************
 * NAME          : UnifiRadioMeasurementActivated
 * PSID          : 2043 (0x07FB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  When TRUE Radio Measurements are supported. The capability is disabled
 *  otherwise.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RADIO_MEASUREMENT_ACTIVATED 0x07FB

/*******************************************************************************
 * NAME          : UnifiRadioMeasurementCapabilities
 * PSID          : 2044 (0x07FC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 5
 * MAX           : 5
 * DEFAULT       : { 0X71, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  This MIB variable indicates the RM Enabled capabilities of the chip. See
 *  SC-503520-SP for further details.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RADIO_MEASUREMENT_CAPABILITIES 0x07FC

/*******************************************************************************
 * NAME          : UnifiVhtActivated
 * PSID          : 2045 (0x07FD)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  This attribute, when TRUE, indicates that use VHT mode. The capability is
 *  disabled otherwise.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_VHT_ACTIVATED 0x07FD

/*******************************************************************************
 * NAME          : UnifiHtActivated
 * PSID          : 2046 (0x07FE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  This attribute, when TRUE, indicates that use HT mode. The capability is
 *  disabled otherwise.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_HT_ACTIVATED 0x07FE

/*******************************************************************************
 * NAME          : UnifiRssiRoamScanTrigger
 * PSID          : 2050 (0x0802)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       : -65
 * DESCRIPTION   :
 *  The current RRSI value below which roaming scan shall start
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSSI_ROAM_SCAN_TRIGGER 0x0802

/*******************************************************************************
 * NAME          : UnifiRssiRoamDeltaTrigger
 * PSID          : 2051 (0x0803)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : dB
 * MIN           : 1
 * MAX           : 255
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  The RRSI on the target AP must be greater than the current AP RSSI by
 *  that value to be oaming candidate
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSSI_ROAM_DELTA_TRIGGER 0x0803

/*******************************************************************************
 * NAME          : UnifiCachedChannelScanPeriod
 * PSID          : 2052 (0x0804)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 30000000
 * DESCRIPTION   :
 *  The scan period for cached channels (microseconds)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CACHED_CHANNEL_SCAN_PERIOD 0x0804

/*******************************************************************************
 * NAME          : UnifiFullRoamScanPeriod
 * PSID          : 2053 (0x0805)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 60000000
 * DESCRIPTION   :
 *  The scan period for all channel allowed by the current regulatory domain
 *  (microseconds)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FULL_ROAM_SCAN_PERIOD 0x0805

/*******************************************************************************
 * NAME          : UnifiRoamScanProbeInterval
 * PSID          : 2054 (0x0806)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : ms
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 20
 * DESCRIPTION   :
 *  DEPRECATED See SSB-4650
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_PROBE_INTERVAL 0x0806

/*******************************************************************************
 * NAME          : UnifiRoamScanBand
 * PSID          : 2055 (0x0807)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 2
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  ) Indicates whether only intra-band or all-band should be used for
 *  roaming scan. 2 - Roaming across band 1 - Roaming within band
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_BAND 0x0807

/*******************************************************************************
 * NAME          : UnifiRoamScanMinActiveChannelTime
 * PSID          : 2056 (0x0808)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 30
 * DESCRIPTION   :
 *  The minimum time to spend on each channel that is actively scanned
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_MIN_ACTIVE_CHANNEL_TIME 0x0808

/*******************************************************************************
 * NAME          : UnifiRoamScanMaxActiveChannelTime
 * PSID          : 2057 (0x0809)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 50
 * DESCRIPTION   :
 *  The maximum time to spend on each channel (providing the channel is
 *  detected as busy during Min_Channel_Time) that is actively scanned. This
 *  shall be greater than or equal to the Min_Channel_Time for active scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_MAX_ACTIVE_CHANNEL_TIME 0x0809

/*******************************************************************************
 * NAME          : UnifiRoamScanMinPassiveChannelTime
 * PSID          : 2058 (0x080A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 110
 * DESCRIPTION   :
 *  The minimum time to spend on each channel that is passively scanned
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_MIN_PASSIVE_CHANNEL_TIME 0x080A

/*******************************************************************************
 * NAME          : UnifiRoamScanMaxPassiveChannelTime
 * PSID          : 2059 (0x080B)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 110
 * DESCRIPTION   :
 *  The maximum time to spend on each channel (providing the channel is
 *  detected as busy during Min_Channel_Time) that is passively scanned. This
 *  shall be greater than or equal to the Min_Channel_Time for passive scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_MAX_PASSIVE_CHANNEL_TIME 0x080B

/*******************************************************************************
 * NAME          : UnifiRoamMode
 * PSID          : 2060 (0x080C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  Enable/Disable on-chip roaming, i.e. roaming scan will be triggered by
 *  firmware if and only if this attribute is true.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_MODE 0x080C

/*******************************************************************************
 * NAME          : UnifiRoamTriggerDelay
 * PSID          : 2062 (0x080E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 10000
 * DESCRIPTION   :
 *  Delay after connection to an AP before allowing a roaming scan to be
 *  triggered.This allows us to stop a roaming scan from getting in the way
 *  of the initial connection protocols. DHCP, Eapol etc.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_TRIGGER_DELAY 0x080E

/*******************************************************************************
 * NAME          : UnifiWesMode
 * PSID          : 2066 (0x0812)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  NCHO Roaming Mode.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_WES_MODE 0x0812

/*******************************************************************************
 * NAME          : UnifiRoamScanControl
 * PSID          : 2067 (0x0813)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  NCHO Roam Scan Control.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_CONTROL 0x0813

/*******************************************************************************
 * NAME          : UnifiDfsScanMode
 * PSID          : 2068 (0x0814)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 2
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  Scan DFS Mode. 0: DFS scan disabled 0: DFS roaming scan disabled. 1: DFS
 *  scan enabled. (passive scanning on DFS channels) 1: DFS roaming scan
 *  enabled. Normal mode. i.e. passive scanning on DFS channels (Default) 2:
 *  DFS scan enabled. (passive scanning on DFS channels) 2: DFS roaming scan
 *  enabled with active scanning on channel list supplied with
 *  MLME-SET-CACHED-CHANNELS.request
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DFS_SCAN_MODE 0x0814

/*******************************************************************************
 * NAME          : UnifiRoamScanHomeTime
 * PSID          : 2069 (0x0815)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The maximum time to spend scanning before pausing for the
 *  unifiRoamScanHomeAwayTime, default of 0 mean has no specific requirement
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_HOME_TIME 0x0815

/*******************************************************************************
 * NAME          : UnifiRoamScanHomeAwayTime
 * PSID          : 2070 (0x0816)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The time to spend NOT scanning after scanning for
 *  unifiRoamScanHomeTime,default of 0 mean has no specific requirement
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_HOME_AWAY_TIME 0x0816

/*******************************************************************************
 * NAME          : UnifiRoamScanNProbe
 * PSID          : 2072 (0x0818)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  The Number of ProbeReq per channel for the Roaming Scan.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAM_SCAN_NPROBE 0x0818

/*******************************************************************************
 * NAME          : UnifiVS2RoamingCount
 * PSID          : 2073 (0x0819)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Number of Roams since Connect or last set to 0. (CCX Voice Services:
 *  Roaming Count)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_VS2_ROAMING_COUNT 0x0819

/*******************************************************************************
 * NAME          : UnifiVS2RoamingDelay
 * PSID          : 2074 (0x081A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : ms
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  milliseconds taken for last roam (CCX Voice Services: Roaming Delay)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_VS2_ROAMING_DELAY 0x081A

/*******************************************************************************
 * NAME          : UnifiApOlbcDuration
 * PSID          : 2076 (0x081C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : milliseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 300
 * DESCRIPTION   :
 *  How long the AP enables reception of BEACON frames to perform Overlapping
 *  Legacy BSS Condition(OLBC). If set to 0 then OLBC is disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_AP_OLBC_DURATION 0x081C

/*******************************************************************************
 * NAME          : UnifiApOlbcInterval
 * PSID          : 2077 (0x081D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : milliseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 2000
 * DESCRIPTION   :
 *  How long between periods of receiving BEACON frames to perform
 *  Overlapping Legacy BSS Condition(OLBC). This value MUST exceed the OBLC
 *  duration MIB unifiApOlbcDuration. If set to 0 then OLBC is disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_AP_OLBC_INTERVAL 0x081D

/*******************************************************************************
 * NAME          : UnifiFrameResponseTimeout
 * PSID          : 2080 (0x0820)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 500
 * DEFAULT       : 200
 * DESCRIPTION   :
 *  How long to wait for a frame (Auth, Assoc, ReAssoc) after Rame replies to
 *  a send frame request
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRAME_RESPONSE_TIMEOUT 0x0820

/*******************************************************************************
 * NAME          : UnifiConnectionFailureTimeout
 * PSID          : 2081 (0x0821)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 4000
 * DEFAULT       : 3000
 * DESCRIPTION   :
 *  How long the complete connection procedure has before the MLME times out
 *  and issues a Connect Indication (fail).
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CONNECTION_FAILURE_TIMEOUT 0x0821

/*******************************************************************************
 * NAME          : UnifiConnectingProbeTimeout
 * PSID          : 2082 (0x0822)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 100
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  How long to wait for a ProbeRsp when syncronising before resending a
 *  ProbeReq
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CONNECTING_PROBE_TIMEOUT 0x0822

/*******************************************************************************
 * NAME          : UnifiDisconnectTimeout
 * PSID          : 2083 (0x0823)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : seconds
 * MIN           : 0
 * MAX           : 3
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  How long the firmware attempts to perform a disconnect (triggered by
 *  MLME_DISCONNECT-REQ) before responding with MLME-DISCONNECT-IND and
 *  aborting the disconnection attempt.This is particulary important when a
 *  SoftAP is attempting to disconnect associated stations which might have
 *  "silently" left the ESS.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DISCONNECT_TIMEOUT 0x0823

/*******************************************************************************
 * NAME          : UnifiFrameResponseCfmTxLifetimeTimeout
 * PSID          : 2084 (0x0824)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  How long to wait to retry a frame (Auth, Assoc, ReAssoc) after TX Cfm
 *  trasnmission_status = TxLifetime.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRAME_RESPONSE_CFM_TX_LIFETIME_TIMEOUT 0x0824

/*******************************************************************************
 * NAME          : UnifiFrameResponseCfmFailureTimeout
 * PSID          : 2085 (0x0825)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 40
 * DESCRIPTION   :
 *  How long to wait to retry a frame (Auth, Assoc, ReAssoc) after TX Cfm
 *  trasnmission_status != Successful | TxLifetime.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRAME_RESPONSE_CFM_FAILURE_TIMEOUT 0x0825

/*******************************************************************************
 * NAME          : UnifiMlmeScanMaxNumberOfProbeSets
 * PSID          : 2087 (0x0827)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  Max number of Probe Request sets that the scan engine will send on a
 *  single channel.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_MAX_NUMBER_OF_PROBE_SETS 0x0827

/*******************************************************************************
 * NAME          : UnifiMlmeScanStopIfLessThanXFrames
 * PSID          : 2088 (0x0828)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 4
 * DESCRIPTION   :
 *  Stop scanning on a channel if less than X Beacons or Probe Responses are
 *  received.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MLME_SCAN_STOP_IF_LESS_THAN_XFRAMES 0x0828

/*******************************************************************************
 * NAME          : UnifiRoamingTriggerTime
 * PSID          : 2090 (0x082A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : us
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Timestamp of last roam tigger. Timestamp of any trigger for roaming.
 *  Caused by Link loss, Rssi, mlme_roam_req etc
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAMING_TRIGGER_TIME 0x082A

/*******************************************************************************
 * NAME          : UnifiRoamingStartTime
 * PSID          : 2091 (0x082B)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : us
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Timestamp of last roam start. Start of a connection attempt to an AP
 *  (Starts at Dataplane Pause)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAMING_START_TIME 0x082B

/*******************************************************************************
 * NAME          : UnifiRoamingOnchipEndTime
 * PSID          : 2092 (0x082C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : us
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Timestamp of last roam end for the onchip portion of the roam.
 *  mlme_roamed_ind to Host
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAMING_ONCHIP_END_TIME 0x082C

/*******************************************************************************
 * NAME          : UnifiRoamingEndTime
 * PSID          : 2093 (0x082D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : us
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Timestamp of last roam end. Keys installed and Dataplane unpaused.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ROAMING_END_TIME 0x082D

/*******************************************************************************
 * NAME          : UnifimcdHackRoamKck
 * PSID          : 2094 (0x082E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 16
 * MAX           : 16
 * DEFAULT       : { 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  KCK
 *******************************************************************************/
#define SLSI_PSID_UNIFIMCD_HACK_ROAM_KCK 0x082E

/*******************************************************************************
 * NAME          : UnifimcdHackRoamKek
 * PSID          : 2095 (0x082F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 16
 * MAX           : 16
 * DEFAULT       : { 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  KEK
 *******************************************************************************/
#define SLSI_PSID_UNIFIMCD_HACK_ROAM_KEK 0x082F

/*******************************************************************************
 * NAME          : UnifiPeerAverageTxDataRate
 * PSID          : 2096 (0x0830)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The average tx rate that are used for transmissions since this entry was
 *  last read;
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PEER_AVERAGE_TX_DATA_RATE 0x0830

/*******************************************************************************
 * NAME          : UnifiPeerRssi
 * PSID          : 2097 (0x0831)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute provides a running average of the Received Signal Strength
 *  Indication (RSSI) for packets received from the peer. The value is only
 *  an indication of the signal strength; it is not an accurate measurement.
 *  The table will be reset when UniFi joins or starts a BSS or is reset. An
 *  entry is reset when the corresponding peer station record is deleted.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PEER_RSSI 0x0831

/*******************************************************************************
 * NAME          : UnifiPioProtectMask
 * PSID          : 2100 (0x0834)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Each of UniFi&apos;s 16 PIO lines may have special (hardware or software)
 *  functionality attached, thereby preventing its use for general purpose
 *  I/O. This attribute provides a bit mask indicating which PIOs are
 *  reserved in this way; a bit set to 1 cannot be set using the
 *  unifiPioDirectionMask or unifiPioLevelMask attributes.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_PROTECT_MASK 0x0834

/*******************************************************************************
 * NAME          : UnifiPioDirectionMask
 * PSID          : 2101 (0x0835)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Each of UniFi&apos;s 16 PIO lines may be configured as either an input or
 *  an output. This attribute sets or gets the current state as a bit mask; a
 *  bit set to 1 sets the corresponding PIO to be an output, otherwise it is
 *  set to be an input. The drive direction of PIOs marked as protected by
 *  unifiPioProtectMask cannot be modified; the corresponding bits are
 *  ignored when written and undefined when read. This attribute is not reset
 *  by an MLME-RESET.request.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_DIRECTION_MASK 0x0835

/*******************************************************************************
 * NAME          : UnifiPioLevelMask
 * PSID          : 2102 (0x0836)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute sets or gets the level of UniFi&apos;s 16 PIO lines as a
 *  bit mask; a bit set to 0 corresponds to a logic low level, anda bit set
 *  to 1 corresponds to a logic high level. Reading this attribute gives the
 *  current state of the lines (for outputs it is the level being driven, and
 *  for inputs it is the sampled levels). Writing this attribute sets the
 *  state of the lines (for outputs it sets the drive level, and for inputs
 *  it configures internal pull-ups or pull-downs). The drive level of PIOs
 *  marked as protected by unifiPioProtectMask cannot be modified; the
 *  corresponding bits are ignored when written. This attribute is not reset
 *  by an MLME-RESET.request.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_LEVEL_MASK 0x0836

/*******************************************************************************
 * NAME          : UnifiPioHostWakeMode
 * PSID          : 2105 (0x0839)
 * PER INTERFACE?: NO
 * TYPE          : unifiPioHostWakeMode
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  If the host&apos;s interface is disabled to save power then it may not be
 *  possible to use the normal interrupt request to wake the host. This
 *  attribute enables a PIO line to be used as an alternative out-of-band
 *  signalling mechanism. Currently defined values are: wake-none (1) - no
 *  out-of-band wake-up mechanism. wake-pulse (2) - pulse a PIO line to wake
 *  the host with polarity specified by unifiPioHostWakePolarity.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_HOST_WAKE_MODE 0x0839

/*******************************************************************************
 * NAME          : UnifiPioHostWakeOutput
 * PSID          : 2106 (0x083A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 15
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute specifies the number of the PIO line to use for waking the
 *  host if unifiPioHostWakeMode is not set to wake-none.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_HOST_WAKE_OUTPUT 0x083A

/*******************************************************************************
 * NAME          : UnifiPioHostWakePolarity
 * PSID          : 2107 (0x083B)
 * PER INTERFACE?: NO
 * TYPE          : unifiPioHostWakePolarity
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute specifies the polarity of PIO signalling used to wake the
 *  host if unifiPioHostWakeMode is not set to wake-none.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_HOST_WAKE_POLARITY 0x083B

/*******************************************************************************
 * NAME          : UnifiPioHostWakeZeal
 * PSID          : 2108 (0x083C)
 * PER INTERFACE?: NO
 * TYPE          : unifiPioHostWakeZeal
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute controls the level of promiscuity of PIO wakeup signalling
 *  generated if unifiPioHostWakeMode is not set to wake-none. The available
 *  options are: wake-normal (0) - only generate a PIO wakeup signal if the
 *  host interface is idle and interrupts are masked. With an SDIO host
 *  interface this mechanism is invoked if either the IENM or IEN1 bit of the
 *  CCCR Int Enable register is not set. wake-always (1) - generate PIO
 *  wakeup signal regardless of host interface activity and interrupt mask
 *  status.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_HOST_WAKE_ZEAL 0x083C

/*******************************************************************************
 * NAME          : UnifiPioHostWakePulseDuration
 * PSID          : 2109 (0x083D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute specifies the length of the pulse signalled to wake the
 *  host on the PIO specified by unifiPioHostWakeOutput if
 *  unifiPioHostWakeMode is set to wake-pulse. The value of zero has a
 *  special meaning and instructs UniFi to toggle the wakeup signal as
 *  quickly as possible.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PIO_HOST_WAKE_PULSE_DURATION 0x083D

/*******************************************************************************
 * NAME          : UnifiUartConfigure
 * PSID          : 2110 (0x083E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  An MLME-SET.request of this attribute causes the UART to be configured
 *  using the values of the other unifiUart* attributes. The value supplied
 *  for this attribute is ignored.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_UART_CONFIGURE 0x083E

/*******************************************************************************
 * NAME          : UnifiUartPios
 * PSID          : 2111 (0x083F)
 * PER INTERFACE?: NO
 * TYPE          : unifiUartPios
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Specification of which PIOs should be connected to the UART. Currently
 *  defined values are: 1 - UART not used; all PIOs are available for other
 *  uses. 2 - Data transmit and receive connected to PIO[12] and PIO[14]
 *  respectively. No hardware handshaking lines. 3 - Data and handshaking
 *  lines connected to PIO[12:15].
 *******************************************************************************/
#define SLSI_PSID_UNIFI_UART_PIOS 0x083F

/*******************************************************************************
 * NAME          : UnifiClockFrequency
 * PSID          : 2140 (0x085C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : kHz
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute can be used to query the nominal frequency of the external
 *  clock source or crystal oscillator used by UniFi. The clock frequency is
 *  a system parameter and can not be modified by this MIB key.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CLOCK_FREQUENCY 0x085C

/*******************************************************************************
 * NAME          : UnifiCrystalFrequencyTrim
 * PSID          : 2141 (0x085D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 63
 * DEFAULT       : 31
 * DESCRIPTION   :
 *  The IEEE 802.11 standard requires a frequency accuracy of either +/- 20
 *  ppm or +/- 25 ppm depending on the physical layer being used. If
 *  UniFi&apos;s frequency reference is a crystal then this attribute should
 *  be used to tweak the oscillating frequency to compensate for design- or
 *  device-specific variations. Each step change trims the frequency by
 *  approximately 2 ppm.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CRYSTAL_FREQUENCY_TRIM 0x085D

/*******************************************************************************
 * NAME          : UnifiDeepSleepConfig
 * PSID          : 2142 (0x085E)
 * PER INTERFACE?: NO
 * TYPE          : unifiDeepSleepConfig
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  UniFi uses various sleep modes to save power. The lowest power mode is
 *  called deep sleep; this turns off the crystal oscillator or stops
 *  requesting an external clock. The resulting loss of responsiveness and
 *  timing accuracy is sometimes undesirable, so use of deep sleep can be
 *  disabled or restricted using this attribute. Other sleep modes are not
 *  affected. The possible values are: 0 - Deep sleep will not be used. 1 -
 *  Deep sleep will be used whenever possible, including when UniFi is in
 *  power save mode while in a BSS. 2 - Deep sleep will only be used when
 *  UniFi is not part of a BSS. It will not be used in the IEEE 802.11
 *  standard power save modes.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DEEP_SLEEP_CONFIG 0x085E

/*******************************************************************************
 * NAME          : UnifiExternalClockDetect
 * PSID          : 2146 (0x0862)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  If UniFi is running with an external fast clock source, i.e.
 *  unifiExternalFastClockRequest is set, it is common for this clock to be
 *  shared with other devices. Setting this attribute to true causes UniFi to
 *  detect when the clock is present (presumably in response to a request
 *  from another device), and to perform any pending activities at that time
 *  rather than requesting the clock again some time later. This is likely to
 *  reduce overall system power consumption by reducing the total time that
 *  the clock needs to be active.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_EXTERNAL_CLOCK_DETECT 0x0862

/*******************************************************************************
 * NAME          : UnifiAnaIoSettingEnum
 * PSID          : 2148 (0x0864)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This counter shall increment when an ACK is not received when expected.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ANA_IO_SETTING_ENUM 0x0864

/*******************************************************************************
 * NAME          : UnifiExternalFastClockRequest
 * PSID          : 2149 (0x0865)
 * PER INTERFACE?: NO
 * TYPE          : unifiExternalFastClockRequest
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  It is possible to supply UniFi with an external fast reference clock, as
 *  an alternative to using a crystal. If such a clock is used then it is
 *  only required when UniFi is active. A signal can be output on PIO[2] or
 *  if the version of UniFi in use is the UF602x or later, any PIO may be
 *  used (see unifiExternalFastClockRequestPIO) to indicate when UniFi
 *  requires a fast clock. Setting this attribute makes this signal become
 *  active and determines the type of signal output. 0 - No clock request. 1
 *  - Non inverted, totem pole. 2 - Inverted, totem pole. 3 - Open drain. 4 -
 *  Open source.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_EXTERNAL_FAST_CLOCK_REQUEST 0x0865

/*******************************************************************************
 * NAME          : UnifiWatchdogTimeout
 * PSID          : 2152 (0x0868)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : ms
 * MIN           : 1
 * MAX           : 65535
 * DEFAULT       : 1500
 * DESCRIPTION   :
 *  This attribute specifies the maximum time the background may be busy or
 *  locked out for. If this time is exceeded, UniFi will reset. If this key
 *  is set to 65535 then the watchdog will be disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_WATCHDOG_TIMEOUT 0x0868

/*******************************************************************************
 * NAME          : UnifiDeepSleepWakeupDelay
 * PSID          : 2154 (0x086A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 65535
 * DEFAULT       : 8
 * DESCRIPTION   :
 *  When UniFi wakes from deep sleep there is normally a need for a delay
 *  between UniFi waking up and the fast clock settling to the correct
 *  frequency. If UniFi is driving the crystal itself then this delay is the
 *  time needed for the crystal to settle. If UniFi is configured to use an
 *  external clock then this is the maximum time that it will take the system
 *  component that supplies the external clock to produce a stable output.
 *  This wakeup delay is configured by this MIB entry. The delay should
 *  include both the time for the crystal or external clock to settle and an
 *  additional time for the internal PLL to lock. The delay is specified in
 *  slow clock cycles. The slow clock is nominally 8 kHz giving a period of
 *  125 microseconds.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DEEP_SLEEP_WAKEUP_DELAY 0x086A

/*******************************************************************************
 * NAME          : UnifiExternalFastClockRequestPio
 * PSID          : 2158 (0x086E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 15
 * DEFAULT       : 9
 * DESCRIPTION   :
 *  If an external fast reference clock is being supplied to UniFi as an
 *  alternative to a crystal (see unifiExternalFastClockRequest) and the
 *  version of UniFi in use is the UF602x or later, any PIO may be used as
 *  the external fast clock request output from UniFi. This MIB key
 *  determines the PIO to use.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_EXTERNAL_FAST_CLOCK_REQUEST_PIO 0x086E

/*******************************************************************************
 * NAME          : UnifiRssi
 * PSID          : 2200 (0x0898)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute provides a running average of the Received Signal Strength
 *  Indication (RSSI) for packets received by UniFi&apos;s radio. The value
 *  should only be treated as an indication of the signal strength; it is not
 *  an accurate measurement. The result is only meaningful if the
 *  unifiRxExternalGain attribute is set to the correct calibration value. If
 *  UniFi is part of a BSS, only frames originating from devices in the BSS
 *  are reported (so far as this can be determined). The average is reset
 *  when UniFi joins or starts a BSS or is reset.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSSI 0x0898

/*******************************************************************************
 * NAME          : UnifiSnr
 * PSID          : 2202 (0x089A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dB
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute provides a running average of the Signal to Noise Ratio
 *  (SNR) for packets received by UniFi&apos;s radio.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SNR 0x089A

/*******************************************************************************
 * NAME          : UnifiRxRetryCount
 * PSID          : 2203 (0x089B)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This is a counter that increments whenever an MPDU is successfully
 *  received that has the retry bit set in the frame control field,
 *  indicating that the MPDU is a retransmission of a unicast frame.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RX_RETRY_COUNT 0x089B

/*******************************************************************************
 * NAME          : UnifiSwTxTimeout
 * PSID          : 2204 (0x089C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  This MIB sets the maximum time in seconds for a frame to be queued in
 *  firmware, ready to be sent, but not yet actually pumped to hardware.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SW_TX_TIMEOUT 0x089C

/*******************************************************************************
 * NAME          : UnifiRateStatsRxSuccessCount
 * PSID          : 2206 (0x089E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of successful receptions of complete management and data
 *  frames at the rate indexed by unifiRateStatsIndex.This number will wrap
 *  to zero after the range is exceeded.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RATE_STATS_RX_SUCCESS_COUNT 0x089E

/*******************************************************************************
 * NAME          : UnifiRateStatsTxSuccessCount
 * PSID          : 2207 (0x089F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of successful (acknowledged) unicast transmissions of complete
 *  data or management frames the rate indexed by unifiRateStatsIndex. This
 *  number will wrap to zero after the range is exceeded.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RATE_STATS_TX_SUCCESS_COUNT 0x089F

/*******************************************************************************
 * NAME          : UnifiTxDataRate
 * PSID          : 2208 (0x08A0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The bit rate currently in use for transmissions of unicast data frames;
 *  On an infrastructure BSS, this is the data rate used in communicating
 *  with the associated access point, if there is none, an error is returned
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_DATA_RATE 0x08A0

/*******************************************************************************
 * NAME          : UnifiSnrExtraOffsetCck
 * PSID          : 2209 (0x08A1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dB
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       : 8
 * DESCRIPTION   :
 *  This offset is added to SNR values received at 802.11b data rates. This
 *  accounts for differences in the RF pathway between 802.11b and 802.11g
 *  demodulators. The offset applies to values of unifiSNR as well as SNR
 *  values in scan indications. This attribute is not used in 5GHz mode.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SNR_EXTRA_OFFSET_CCK 0x08A1

/*******************************************************************************
 * NAME          : UnifiRssiMaxAveragingPeriod
 * PSID          : 2210 (0x08A2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 1
 * MAX           : 65535
 * DEFAULT       : 1024
 * DESCRIPTION   :
 *  This attribute limits the period over which the value of unifiRSSI is
 *  averaged. If no more than unifiRSSIMinReceivedFrames frames have been
 *  received in the period, then the value of unifiRSSI is reset to the value
 *  of the next measurement and the rolling average is restarted. This
 *  ensures that the value is timely (although possibly poorly averaged) when
 *  little data is being received.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSSI_MAX_AVERAGING_PERIOD 0x08A2

/*******************************************************************************
 * NAME          : UnifiRssiMinReceivedFrames
 * PSID          : 2211 (0x08A3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 65535
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  See the description of unifiRSSIMaxAveragingPeriod for how the
 *  combination of attributes is used.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RSSI_MIN_RECEIVED_FRAMES 0x08A3

/*******************************************************************************
 * NAME          : UnifiRateStatsRate
 * PSID          : 2212 (0x08A4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : 500 kbps
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The rate corresponding to the current table entry. The value is rounded
 *  to the nearest number of units where necessary. Most rates do not require
 *  rounding, but when short guard interval is in effect the rates are no
 *  longer multiples of the base unit. Note that there may be two occurrences
 *  of the value 130: the first corresponds to MCS index 7, and the second,
 *  if present, to MCS index 6 with short guard interval.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RATE_STATS_RATE 0x08A4

/*******************************************************************************
 * NAME          : UnifiDiscardedFrameCount
 * PSID          : 2214 (0x08A6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This is a counter that indicates the number of data and management frames
 *  that have been processed by the UniFi hardware but were discarded before
 *  being processed by the firmware. It does not include frames not processed
 *  by the hardware because they were not addressed to the local device, nor
 *  does it include frames discarded by the firmware in the course of normal
 *  MAC processing (which include, for example, frames in an appropriate
 *  encryption state and multicast frames not requested by the host).
 *  Typically this counter indicates lost data frames for which there was no
 *  buffer space; however, other cases may cause the counter to increment,
 *  such as receiving a retransmitted frame that was already successfully
 *  processed. Hence this counter should not be treated as a reliable guide
 *  to lost frames. The counter wraps to 0 after 65535.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DISCARDED_FRAME_COUNT 0x08A6

/*******************************************************************************
 * NAME          : UnifiIbssBeaconRateStart
 * PSID          : 2215 (0x08A7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X0005000A
 * DESCRIPTION   :
 *  In Oxygen it is required that in the first X seconds after joining an
 *  IBSS at least Y beacons must be transmitted. With this MIB it is possible
 *  to set the number of seconds X and the number of beacons Y where the most
 *  significant 16 bits is the number of seconds and the least significant 16
 *  bits is the number of beacons. If seconds or beacons is zero the feature
 *  is disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_IBSS_BEACON_RATE_START 0x08A7

/*******************************************************************************
 * NAME          : UnifiIbssBeaconRateOnGoing
 * PSID          : 2216 (0x08A8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X000A0001
 * DESCRIPTION   :
 *  In Oxygen it is required that after the first N seconds which can be set
 *  by unifiIBSSBeaconRateStart at least Y beacons must be transmitted for
 *  every X seconds. With this MIB it is possible to set the number of
 *  seconds X and the number of beacons Y where the most significant 16 bits
 *  is the number of seconds and the least significant 16 bits is the number
 *  of beacons. If seconds or beacons is zero the feature is disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_IBSS_BEACON_RATE_ON_GOING 0x08A8

/*******************************************************************************
 * NAME          : UnifiReceiverLeaderTimeout
 * PSID          : 2217 (0x08A9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 8
 * DESCRIPTION   :
 *  This attribute shall indicate the maximum number of seconds during which
 *  the leader has not received any action frames from the multicast
 *  transmitter before leader think the multicaster has cancelled its leader
 *  relation. After timer triggers TIMEOUT, FW shall cancel and unconfigure
 *  the HW
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RECEIVER_LEADER_TIMEOUT 0x08A9

/*******************************************************************************
 * NAME          : UnifiCurrentTsfTime
 * PSID          : 2218 (0x08AA)
 * PER INTERFACE?: NO
 * TYPE          : INT64
 * MIN           : -9223372036854775808
 * MAX           : 9223372036854775807
 * DEFAULT       :
 * DESCRIPTION   :
 *  Get TSF time (last 32 bits) for the specified VIF. VIF index can't be 0
 *  as that is treated as global VIF For station VIF - Correct BSS TSF wil
 *  only be reported after MLME-CONNECT.indication(success) indication to
 *  host. Note that if MAC Hardware is switched off then TSF returned is
 *  estimated value
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CURRENT_TSF_TIME 0x08AA

/*******************************************************************************
 * NAME          : UnifiTxFailureThreshold
 * PSID          : 2219 (0x08AB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Oxygen Fast TX Failure Event Notification. This value is the number
 *  of consecutive transmission failures for a peer device before the
 *  notification event is sent to host. value set to 0 disables this
 *  functionality
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_FAILURE_THRESHOLD 0x08AB

/*******************************************************************************
 * NAME          : UnifiRmcActionPeriod
 * PSID          : 2220 (0x08AC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : ms
 * MIN           : 0
 * MAX           : 1000
 * DEFAULT       : 300
 * DESCRIPTION   :
 *  This variable specifies the repetition period at which the Leader Select
 *  Action frame shall be transmitted.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RMC_ACTION_PERIOD 0x08AC

/*******************************************************************************
 * NAME          : UnifiRmcLeaderReselectPeriod
 * PSID          : 2221 (0x08AD)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 360
 * DEFAULT       : 30
 * DESCRIPTION   :
 *  This variable specifies the rate at which the F/W will determine whether
 *  or not the current Receiver Leader should change based on link quality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RMC_LEADER_RESELECT_PERIOD 0x08AD

/*******************************************************************************
 * NAME          : UnifiRmcFailureThreshold
 * PSID          : 2222 (0x08AE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 3
 * DESCRIPTION   :
 *  This value is the number of consecutive RMC transmission failures
 *  triggering the reselection of the RMC Receiver Leader. value set to 0
 *  disables this functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RMC_FAILURE_THRESHOLD 0x08AE

/*******************************************************************************
 * NAME          : UnifiBaConfig
 * PSID          : 2225 (0x08B1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X003FFF00
 * DESCRIPTION   :
 *  Block Ack Configuration. It is composed of A-MSDU supported, TX MPDU per
 *  A-MPDU, RX Buffer size, TX Buffer size and Block Ack Timeout.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_BA_CONFIG 0x08B1

/*******************************************************************************
 * NAME          : UnifiNoAckActivationCount
 * PSID          : 2240 (0x08C0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of frames that are discarded due to HW No-ack activated during
 *  test. This number will wrap to zero after the range is exceeded.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_NO_ACK_ACTIVATION_COUNT 0x08C0

/*******************************************************************************
 * NAME          : UnifiRxFcsErrorCount
 * PSID          : 2241 (0x08C1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of received frames that are discarded due to bad FCS (CRC).
 *  This number will wrap to zero after the range is exceeded.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RX_FCS_ERROR_COUNT 0x08C1

/*******************************************************************************
 * NAME          : UnifiAckFailureCount
 * PSID          : 2242 (0x08C2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of frames transmitted that were not acked.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ACK_FAILURE_COUNT 0x08C2

/*******************************************************************************
 * NAME          : UnifiBlockAckMissedCount
 * PSID          : 2243 (0x08C3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The number of instances that the Block Ack was expected but not received.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_BLOCK_ACK_MISSED_COUNT 0x08C3

/*******************************************************************************
 * NAME          : UnifiSwToHwQueueStats
 * PSID          : 2250 (0x08CA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The timing statistics of packets being queued between SW-HW
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SW_TO_HW_QUEUE_STATS 0x08CA

/*******************************************************************************
 * NAME          : UnifiHostToSwQueueStats
 * PSID          : 2251 (0x08CB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  The timing statistics of packets being queued between HOST-SW
 *******************************************************************************/
#define SLSI_PSID_UNIFI_HOST_TO_SW_QUEUE_STATS 0x08CB

/*******************************************************************************
 * NAME          : UnifiQueueStatsEnable
 * PSID          : 2252 (0x08CC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  Enables recording timing statistics of packets being queued between
 *  HOST-SW-HW
 *******************************************************************************/
#define SLSI_PSID_UNIFI_QUEUE_STATS_ENABLE 0x08CC

/*******************************************************************************
 * NAME          : UnifiGoogleMaxNumberOfPeriodicScans
 * PSID          : 2260 (0x08D4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 6
 * DESCRIPTION   :
 *  Max number of periodic scans for Google scan functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GOOGLE_MAX_NUMBER_OF_PERIODIC_SCANS 0x08D4

/*******************************************************************************
 * NAME          : UnifiGoogleMaxRssiSampleSize
 * PSID          : 2261 (0x08D5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 8
 * DESCRIPTION   :
 *  Max number of RSSI samples used for averaging RSSI in Google scan
 *  functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GOOGLE_MAX_RSSI_SAMPLE_SIZE 0x08D5

/*******************************************************************************
 * NAME          : UnifiGoogleMaxHotlistAPs
 * PSID          : 2262 (0x08D6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 64
 * DESCRIPTION   :
 *  Max number of entries for hotlist APs in Google scan functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GOOGLE_MAX_HOTLIST_APS 0x08D6

/*******************************************************************************
 * NAME          : UnifiGoogleMaxSignificantWifiChangeAPs
 * PSID          : 2263 (0x08D7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 64
 * DESCRIPTION   :
 *  Max number of entries for significant WiFi change APs in Google scan
 *  functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GOOGLE_MAX_SIGNIFICANT_WIFI_CHANGE_APS 0x08D7

/*******************************************************************************
 * NAME          : UnifiGoogleMaxBssidHistoryEntries
 * PSID          : 2264 (0x08D8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Max number of BSSID/RSSI that the device can hold in Google scan
 *  functionality.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GOOGLE_MAX_BSSID_HISTORY_ENTRIES 0x08D8

/*******************************************************************************
 * NAME          : UnifiCoexCdlConfig
 * PSID          : 2424 (0x0978)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 17
 * MAX           : 17
 * DEFAULT       : { 0X02, 0X03, 0X04, 0X54, 0X54, 0X50, 0X54, 0X00, 0X01, 0X02, 0X03, 0XEE, 0XEE, 0XEE, 0XEE, 0XEE, 0XEE }
 * DESCRIPTION   :
 *  These contain the spectral thresholds corresponding to the absolute
 *  difference between channel values: octet 0 -
 *  collision_table.spectral_threshs.thresh_2 octet 1 -
 *  collision_table.spectral_threshs.thresh_1 octet 2 -
 *  collision_table.spectral_threshs.thresh_0 These correspond to the TX and
 *  RX collision table settings: octet 3 -
 *  collision_table.local_tx_remote_rx_collision_table octet 4 -
 *  collision_table.local_tx_remote_tx_collision_table octet 5 -
 *  collision_table.local_rx_remote_rx_collision_table octet 6 -
 *  collision_table.local_rx_remote_tx_collision_table These contain settings
 *  of the priority difference thresholding: octet 7 -
 *  priority_difference_table.thresh_neg octet 8 -
 *  priority_difference_table.thresh_0 octet 9 -
 *  priority_difference_table.thresh_1 octet 10 -
 *  priority_difference_table.thresh_2 And finally the following describes
 *  the action tables These are 16 bit fields with LSB first and MSB second
 *  (for histrorical reasons) octet 11 - action_table.abort_table (lsb) octet
 *  12 - action_table.abort_table (msb) octet 13 - action_table.lcl_table_1
 *  (lsb) octet 14 - action_table.lcl_table_1 (msb) octet 15 -
 *  action_table.lcl_table_2 (lsb) octet 16 - action_table.lcl_table_2 (msb)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_CDL_CONFIG 0x0978

/*******************************************************************************
 * NAME          : UnifiHighTemperatureCutOffThreshold
 * PSID          : 2446 (0x098E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : Celsius
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       : 255
 * DESCRIPTION   :
 *  This attribute specifies the temperature threshold at which wifi
 *  transmission will be paused. Normal values for this MIB is between +85 to
 * +125 degree Celsius. Setting the value to 255 will disable the Cut off
 *  mechanism. Deprecated - Condor onwards
 *******************************************************************************/
#define SLSI_PSID_UNIFI_HIGH_TEMPERATURE_CUT_OFF_THRESHOLD 0x098E

/*******************************************************************************
 * NAME          : UnifiLowTemperatureResumeThreshold
 * PSID          : 2447 (0x098F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : Celsius
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       : 105
 * DESCRIPTION   :
 *  This attribute specifies the temperature threshold below which wifi
 *  transmission will be resumed. Normal values for this MIB is between +85
 *  to +125 degree Celsius. Setting the value to 255 will disable the resume
 *  mechanism. Its value should always be less than
 *  unifiHighTemperatureCutOffThreshold value. Deprecated - Condor onwards
 *******************************************************************************/
#define SLSI_PSID_UNIFI_LOW_TEMPERATURE_RESUME_THRESHOLD 0x098F

/*******************************************************************************
 * NAME          : UnifiFastPowerSaveTimeout
 * PSID          : 2500 (0x09C4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 50000
 * DESCRIPTION   :
 *  UniFi implements a proprietary power management mode called Fast Power
 *  Save that balances network performance against power consumption. In this
 *  mode UniFi delays entering power save mode until it detects that there
 *  has been no exchange of data for the duration specified by this
 *  attribute.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FAST_POWER_SAVE_TIMEOUT 0x09C4

/*******************************************************************************
 * NAME          : UnifiIbssKeepAlivePeriod
 * PSID          : 2501 (0x09C5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 2147
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  AdHoc/IBSS/Oxygen Mode: This variable specifies the interval between
 *  sending keep-alive (Null frame) packets to associated station in an Ad
 *  Hoc network if there has been no unicast Rx/Tx activities. Setting it to
 *  0 Disables it. This MIB should be set before the VIF is created. If set
 *  to less than 10s (but not 0) it defaults to 10.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_IBSS_KEEP_ALIVE_PERIOD 0x09C5

/*******************************************************************************
 * NAME          : UnifiStaKeepAlivePeriod
 * PSID          : 2502 (0x09C6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 2147
 * DEFAULT       : 30
 * DESCRIPTION   :
 *  Station/P2P Client Mode: This variable specifies the interval between
 *  sending keep-alive (Null frame) packets while associated to an access
 *  point during periods of idleness (i.e. when there is no unicast transmit
 *  or receive activity).Setting it to 0 Disables it. This MIB should be set
 *  before the VIF is created. If set to less than 10s (but not 0) it
 *  defaults to 10.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_STA_KEEP_ALIVE_PERIOD 0x09C6

/*******************************************************************************
 * NAME          : UnifiApKeepAlivePeriod
 * PSID          : 2503 (0x09C7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 2147
 * DEFAULT       :
 * DESCRIPTION   :
 *  Ap Mode:This variable specifies the interval between sending keep-alive
 *  (Null frame) packetsto associated stations if there has been no unicast
 *  Rx/Tx activities.Setting it to 0 Disables it. This MIB should be set
 *  before the VIF is created. Min value when different to 0 is 10s.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_AP_KEEP_ALIVE_PERIOD 0x09C7

/*******************************************************************************
 * NAME          : UnifiGoKeepAlivePeriod
 * PSID          : 2504 (0x09C8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : s
 * MIN           : 0
 * MAX           : 2147
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  P2P GO Mode:This variable specifies the interval between sending
 *  keep-alive (Null frame) packetsto associated P2P Clients if there has
 *  been no unicast Rx/Tx activities. Setting it to 0 Disables it.This MIB
 *  should be set before the VIF is created. min value when different to 0 is
 *  10s.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_GO_KEEP_ALIVE_PERIOD 0x09C8

/*******************************************************************************
 * NAME          : UnifiApBeaconMaxDrift
 * PSID          : 2507 (0x09CB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 0XFFFF
 * DESCRIPTION   :
 *  The maximum drift in microseconds we will allow for each beacon sent when
 *  we're trying to move it to get a 50% duty cycle between GO and STA in
 *  multiple VIF scenario. We'll delay our TX beacon by a maximum of this
 *  value until we reach our target TBTT. We have 3 possible cases for this
 *  value: a) ap_beacon_max_drift = 0x0000 - Feature disabled b)
 *  ap_beacon_max_drift between 0x0001 and 0xFFFE - Each time we transmit the
 *  beacon we'll move it a little bit forward but never more than this. (Not
 *  implemented yet) c) ap_beacon_max_drift = 0xFFFF - Move the beacon to the
 *  desired position in one shot.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_AP_BEACON_MAX_DRIFT 0x09CB

/*******************************************************************************
 * NAME          : UnifiDisableLegacyPowerSave
 * PSID          : 2510 (0x09CE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  This affects Station VIF power save behaviour. Setting it to 1 will
 *  disable legacy power save (i.e. we wil use fast power save to retrieve
 *  data) Note that this MIB actually disables full power save mode (i.e
 *  sending trigger to retrieve frames which will be PS-POLL for legacy and
 *  QOS-NULL for UAPSD)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DISABLE_LEGACY_POWER_SAVE 0x09CE

/*******************************************************************************
 * NAME          : UnifiPowerSaveExtraListenTime
 * PSID          : 2512 (0x09D0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 1000
 * DESCRIPTION   :
 *  This attribute specifies an extra time during which UniFi will listen to
 *  transmissions in Power Save or Fast Power Save after transmission or
 *  reception of unicast frames and before returning to sleep. This ensures
 *  that the peer device has no further information to transmit, such as a
 *  retry of a frame from an access point with the More Data flag clear. This
 *  attribute may be set to zero, in which case UniFi will not stay awake for
 *  any extra period. As the extra listen time has implications for power
 *  saving, it is only applied when the device has not applied it and has not
 *  been in active mode within the last unifiPowerSaveExtraListenDelay
 *  microseconds. Changes to this attribute take effect when starting or
 *  joining a new BSS.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_POWER_SAVE_EXTRA_LISTEN_TIME 0x09D0

/*******************************************************************************
 * NAME          : UnifiPowerSaveExtraListenDelay
 * PSID          : 2513 (0x09D1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 1000000
 * DESCRIPTION   :
 *  A timeout used after the logic described for
 *  unifiPowerSaveExtraListenTime has been applied during which it will not
 *  be applied again. This attribute should not be set to zero if
 *  unifiPowerSaveExtraListenTime is non-zero since UniFi will then be kept
 *  awake permanently. Changes to this attribute take effect when starting or
 *  joining a new BSS.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_POWER_SAVE_EXTRA_LISTEN_DELAY 0x09D1

/*******************************************************************************
 * NAME          : UnifiPowerManagementDelayTimeout
 * PSID          : 2514 (0x09D2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 30000
 * DESCRIPTION   :
 *  When UniFi enters power save mode it signals the new state by setting the
 *  power management bit in the frame control field of a NULL frame. It then
 *  remains active for the period since the previous unicast reception, or
 *  since the transmission of the NULL frame, whichever is later. This
 *  attribute controls the maximum time during which UniFi will continue to
 *  listen for data. This allows any buffered data on a remote device to be
 *  cleared. Note that this attribute specifies an upper limit on the
 *  timeout. UniFi internally implements a proprietary algorithm to adapt the
 *  timeout depending upon the situation.This is used by firmware when
 *  current station VIF is only station VIF which can be scheduled
 *******************************************************************************/
#define SLSI_PSID_UNIFI_POWER_MANAGEMENT_DELAY_TIMEOUT 0x09D2

/*******************************************************************************
 * NAME          : UnifiApsdServicePeriodTimeout
 * PSID          : 2515 (0x09D3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 20000
 * DESCRIPTION   :
 *  During Unscheduled Automated Power Save Delivery (U-APSD), UniFi may
 *  trigger a service period in order to fetch data from the access point.
 *  The service period is normally terminated by a frame from the access
 *  point with the EOSP (End Of Service Period) flag set, at which point
 *  UniFi returns to sleep. However, if the access point is temporarily
 *  inaccessible, UniFi would stay awake indefinitely. This attribute
 *  specifies a timeout starting from the point where the trigger frame has
 *  been sent. If the timeout expires and no data has been received from the
 *  access point, UniFi will behave as if the service period had been ended
 *  normally and return to sleep. This timeout takes precedence over
 *  unifiPowerSaveExtraListenTime if both would otherwise be applicable.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_APSD_SERVICE_PERIOD_TIMEOUT 0x09D3

/*******************************************************************************
 * NAME          : UnifiConcurrentPowerManagementDelayTimeout
 * PSID          : 2516 (0x09D4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 2000
 * DESCRIPTION   :
 *  When UniFi enters power save mode it signals the new state by setting the
 *  power management bit in the frame control field of a NULL frame. It then
 *  remains active for the period since the previous unicast reception, or
 *  since the transmission of the NULL frame, whichever is later. This
 *  attribute controls the maximum time during which UniFi will continue to
 *  listen for data. This allows any buffered data on a remote device to be
 *  cleared.This is same as unifiPowerManagementDelayTimeout but this value
 *  is considered only when we are doing multivif operations and other VIFs
 *  are waiting to be scheduled.Note that firmware automatically chooses one
 *  of unifiPowerManagementDelayTimeout and
 *  unifiConcurrentPowerManagementDelayTimeout depending upon the current
 *  situation.It is sensible to set unifiPowerManagementDelayTimeout to be
 *  always more thanunifiConcurrentPowerManagementDelayTimeout.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CONCURRENT_POWER_MANAGEMENT_DELAY_TIMEOUT 0x09D4

/*******************************************************************************
 * NAME          : UnifiStationQosInfo
 * PSID          : 2517 (0x09D5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This MIB variable indicates the QoS capability for a non-AP Station, and
 *  is encoded as per IEEE 802.11 QoS Capability.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_STATION_QOS_INFO 0x09D5

/*******************************************************************************
 * NAME          : UnifiListenInterval
 * PSID          : 2519 (0x09D7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : beacon intervals
 * MIN           : 0
 * MAX           : 100
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  Defines the Beacon Listen Interval
 *******************************************************************************/
#define SLSI_PSID_UNIFI_LISTEN_INTERVAL 0x09D7

/*******************************************************************************
 * NAME          : UnifiRadioCalibrationMode
 * PSID          : 2521 (0x09D9)
 * PER INTERFACE?: NO
 * TYPE          : unifiRadioCalibrationMode
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute is used to control the radio calibration operations
 *  performed by UniFi. If this key is left in the &apos;calibrate-auto&apos;
 *  state then the firmware will perform radio calibrations whenever they are
 *  required. In this mode a calibration is always performed after the first
 *  MLME-RESET - this calibration is required to enable the radio. This key
 *  can be set to the &apos;calibrate-now&apos; value to hint to the firmware
 *  that a calibration should be performed now. This can be useful if the
 *  host software knows that the radio has not been used for some time, but
 *  that it is about to be used. If this key is set to the
 *  &apos;no-calibrate&apos; setting then no radio calibrations will be
 *  performed by the firmware until it is commanded to (by setting the key to
 *  either &apos;calibrate-auto&apos; or &apos;calibrate-now&apos;. If this
 *  key is set to this state before the first MLME-RESET then the radio will
 *  not be able to be used. The &apos;no-calibrate&apos; mode will allow the
 *  fastest booting and will ensure that no RF power is emitted from the
 *  device. In this mode the radio will not work, even for receive.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RADIO_CALIBRATION_MODE 0x09D9

/*******************************************************************************
 * NAME          : UnifiFragmentationDuration
 * PSID          : 2524 (0x09DC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  A limit on transmission time for a data frame. If the data payload would
 *  take longer than unifiFragmentationDuration to transmit, UniFi will
 *  attempt to fragment the frame to ensure that the data portion of each
 *  fragment is within the limit. The limit imposed by the fragmentation
 *  threshold is also respected, and no more than 16 fragments may be
 *  generated. If the value is zero no limit is imposed. The value may be
 *  changed dynamically during connections. Note that the limit is a
 *  guideline and may not always be respected. In particular, the data rate
 *  is finalised after fragmentation in order to ensure responsiveness to
 *  conditions, the calculation is not performed to high accuracy, and octets
 *  added during encryption are not included in the duration calculation.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRAGMENTATION_DURATION 0x09DC

/*******************************************************************************
 * NAME          : UnifiDtimWaitTimeout
 * PSID          : 2529 (0x09E1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 50000
 * DESCRIPTION   :
 *  If UniFi is in power save and receives a Traffic Indication Map from its
 *  associated access point with a DTIM indication, it will wait a maximum
 *  time given by this attribute for succeeding broadcast or multicast
 *  traffic, or until it receives such traffic with the &apos;more data&apos;
 *  flag clear. Any reception of broadcast or multicast traffic with the
 *  &apos;more data&apos; flag set, or any reception of unicast data, resets
 *  the timeout. The timeout can be turned off by setting the value to zero;
 *  in that case UniFi will remain awake indefinitely waiting for broadcast
 *  or multicast data. Otherwise, the value should be larger than that of
 *  unifiPowerSaveExtraListenTime.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DTIM_WAIT_TIMEOUT 0x09E1

/*******************************************************************************
 * NAME          : UnifiScanMaxProbeTransmitLifetime
 * PSID          : 2531 (0x09E3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 1
 * MAX           : 4294967295
 * DEFAULT       : 64
 * DESCRIPTION   :
 *  If the value of this attribute is non zero, it is used during active
 *  scans as the maximum lifetime for probe requests insteadof the value of
 *  dot11MaxTransmitMSDULifetime.It is the elapsed time after the initial
 *  transmissionat which further attempts to transmit the probe are
 *  terminated.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SCAN_MAX_PROBE_TRANSMIT_LIFETIME 0x09E3

/*******************************************************************************
 * NAME          : UnifiPowerSaveTransitionPacketThreshold
 * PSID          : 2532 (0x09E4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  If VIF has these many packet queued/transmitted/received in last
 *  unifiFastPowerSaveTransitionPeriod then firmware may decide to come out
 *  of aggressive power save mode. This is applicable to STA (CLI) and GO
 *  (VIF).Note that this is only a guideline. Firmware internal factors may
 *  override this MIB.Also see unifiExitPowerSavePeriod and
 *  unifiAggressivePowerSaveTransitionPeriod.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_POWER_SAVE_TRANSITION_PACKET_THRESHOLD 0x09E4

/*******************************************************************************
 * NAME          : UnifiProbeResponseLifetime
 * PSID          : 2533 (0x09E5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 500
 * DESCRIPTION   :
 *  This mib entry is used to indicate the lifetime of proberesponse frame in
 *  unit of ms.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PROBE_RESPONSE_LIFETIME 0x09E5

/*******************************************************************************
 * NAME          : UnifiProbeResponseMaxRetry
 * PSID          : 2534 (0x09E6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  This mib entry is used to indicate the number of retries of probe
 *  response frame.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PROBE_RESPONSE_MAX_RETRY 0x09E6

/*******************************************************************************
 * NAME          : UnifiExitPowerSavePeriod
 * PSID          : 2535 (0x09E7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 200
 * DESCRIPTION   :
 *  Period over which firmware counts number of packet
 *  transmitted/queued/received to decide to come out of aggressive power
 *  save mode.This is applicable to STA (CLI) and GO (AP) VIF. Note that this
 *  is only a guideline. Firmware internal factors may override this MIB.
 *  Also see unifiPowerSaveTransitionPacketThreshold and
 *  unifiAggressivePowerSaveTransitionPeriod
 *******************************************************************************/
#define SLSI_PSID_UNIFI_EXIT_POWER_SAVE_PERIOD 0x09E7

/*******************************************************************************
 * NAME          : UnifiAggressivePowerSaveTransitionPeriod
 * PSID          : 2536 (0x09E8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  Defines how many unifiExitPowerSavePeriod firmware should wait in which
 *  VIF had received/transmitted/queued less than
 *  unifiPowerSaveTransitionPacketThreshold packets - before entering
 *  aggressive power save mode (when not in aggressive power save mode) This
 *  is applicable to STA (CLI) and GO (AP) VIF. Note that this is only a
 *  guideline. Firmware internal factors may override this MIB. Also see
 *  unifiPowerSaveTransitionPacketThreshold and unifiExitPowerSavePeriod.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_AGGRESSIVE_POWER_SAVE_TRANSITION_PERIOD 0x09E8

/*******************************************************************************
 * NAME          : UnifiActiveTimeAfterMoreBit
 * PSID          : 2537 (0x09E9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 30
 * DESCRIPTION   :
 *  After seeing the "more" bit set in a message from the AP, the STA will
 *  goto active mode for this duration of time. After this time, traffic
 *  information is evaluated to determine whether the STA should stay active
 *  or go to powersave. Setting this value to 0 means that the described
 *  functionality is disabled.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_ACTIVE_TIME_AFTER_MORE_BIT 0x09E9

/*******************************************************************************
 * NAME          : UnifiForcedScheduleDuration
 * PSID          : 2538 (0x09EA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 50
 * DESCRIPTION   :
 *  Defines the time to keep a VIF scheduled after an outgoing packet is
 *  queued, if the "Immediate_Response_Expected" bit is set Tx control
 *  associated with a frame transmission request. The firmware may choose to
 *  override this value based on internal logic.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FORCED_SCHEDULE_DURATION 0x09EA

/*******************************************************************************
 * NAME          : UnifiVhtCapabilities
 * PSID          : 2540 (0x09EC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 12
 * MAX           : 12
 * DEFAULT       : { 0X21, 0X71, 0X80, 0X01, 0XFE, 0XFF, 0X00, 0X00, 0XFE, 0XFF, 0X00, 0X00 }
 * DESCRIPTION   :
 *  This MIB variable indicates the VHT capabilities of the chip. see
 *  SC-503520-SP
 *******************************************************************************/
#define SLSI_PSID_UNIFI_VHT_CAPABILITIES 0x09EC

/*******************************************************************************
 * NAME          : UnifiMaxVifScheduleDuration
 * PSID          : 2541 (0x09ED)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 20
 * DESCRIPTION   :
 *  Default time for which a non-scan VIF can be scheduled. Applies to
 *  multiVIF scenario. This is used as a guideline to firmware. Internal
 *  firmware logic or BSS state (e.g. NOA) may cut short the schedule..
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MAX_VIF_SCHEDULE_DURATION 0x09ED

/*******************************************************************************
 * NAME          : UnifiMaxClient
 * PSID          : 2550 (0x09F6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Restricts the maximum number of associated STAs for SoftAP.Defaulted to 0
 *  to allow the Soft AP to allow as many associated STAs as it can support.
 *  The Soft AP may restrict the number of associated STAs to less than this
 *  value (if non-zero), if the Soft AP is unable to support that many
 *  associated STAs. If non-zero the number of associated STAs will not
 *  exceed this value.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_MAX_CLIENT 0x09F6

/*******************************************************************************
 * NAME          : UnifiNoaDuration
 * PSID          : 2552 (0x09F8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This MIB represents the absence period of P2P NoA in microsecond. If Host
 *  want to enable NoA, it needs to set both the unifiNoaDuration and the
 *  unifiNoaCount.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_NOA_DURATION 0x09F8

/*******************************************************************************
 * NAME          : UnifiNoaCount
 * PSID          : 2553 (0x09F9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  This mib represents the count of P2P NoA. If the count is 255, then the
 *  P2P NoA is the periodic NoA(Infinite). If Host want to enable NoA, it
 *  needs to set both the unifiNoaDuration and the unifiNoaCount.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_NOA_COUNT 0x09F9

/*******************************************************************************
 * NAME          : UnifiNoaInterval
 * PSID          : 2554 (0x09FA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 100
 * DESCRIPTION   :
 *  This MIB represents the interval of P2P NoA in Time Unit(TU).
 *******************************************************************************/
#define SLSI_PSID_UNIFI_NOA_INTERVAL 0x09FA

/*******************************************************************************
 * NAME          : UnifiNoaStartOffset
 * PSID          : 2555 (0x09FB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 100
 * DESCRIPTION   :
 *  This MIB represents the start offset of P2P NoA in Time Unit(TU). Firware
 *  will calculate the actual start time using this value.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_NOA_START_OFFSET 0x09FB

/*******************************************************************************
 * NAME          : UnifiCtWindow
 * PSID          : 2557 (0x09FD)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  This value represents the CTWindow value.If this MIB is zero, the
 *  CTwindow in NoA Attribute will be deleted.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CT_WINDOW 0x09FD

/*******************************************************************************
 * NAME          : UnifiTdlsActivated
 * PSID          : 2558 (0x09FE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  This attribute, when TRUE, indicates that use TDLS mode. The TDLS is
 *  disabled otherwise.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_ACTIVATED 0x09FE

/*******************************************************************************
 * NAME          : UnifiTdlsTpThresholdPktSecs
 * PSID          : 2559 (0x09FF)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 100
 * DESCRIPTION   :
 *  his MIB shall be used for the argument "throughput_threshold_pktsecs" of
 *  RAME-MLME-ENABLE-PEER-TRAFFIC-REPORTING.request signal defined in
 *  SC-505422-DD.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_TP_THRESHOLD_PKT_SECS 0x09FF

/*******************************************************************************
 * NAME          : UnifiTdlsRssiThreshold
 * PSID          : 2560 (0x0A00)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       : -75
 * DESCRIPTION   :
 *  This MIB shall be used for the FW initiated TDLS Discovery/Setup
 *  procedure. If the RSSI of a received TDLS Discovery Response frame is
 *  greater than this value, the TDLS FSM shall initiate the TDLS Setup
 *  procedure.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_RSSI_THRESHOLD 0x0A00

/*******************************************************************************
 * NAME          : UnifiTdlsMaximumRetry
 * PSID          : 2561 (0x0A01)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 3
 * DESCRIPTION   :
 *  Transmission of a TDLS Action frame or a TDLS Discovery Response Public
 *  Action frame shall be retried unifiTdlsMaximumRetry times until the frame
 *  is transmitted successfully.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_MAXIMUM_RETRY 0x0A01

/*******************************************************************************
 * NAME          : UnifiTdlsTpMonitorSecs
 * PSID          : 2562 (0x0A02)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  add description
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_TP_MONITOR_SECS 0x0A02

/*******************************************************************************
 * NAME          : UnifiTdlsBasicHtMcsSet
 * PSID          : 2563 (0x0A03)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 16
 * MAX           : 16
 * DEFAULT       : { 0XFF, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  The TDLS FSM shall monitor the number of transmitted packet count per a
 *  TDLS peer for unifiTdlsTPMonitorSecs seconds to decide to tear down the
 *  TDLS link (see 7.12.1).
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_BASIC_HT_MCS_SET 0x0A03

/*******************************************************************************
 * NAME          : UnifiTdlsBasicVhtMcsSet
 * PSID          : 2564 (0x0A04)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 2
 * MAX           : 2
 * DEFAULT       : { 0XFE, 0XFF }
 * DESCRIPTION   :
 *  This MIB shall be used to build the VHT Operation element in the TDLS
 *  Setup Confirm frame
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_BASIC_VHT_MCS_SET 0x0A04

/*******************************************************************************
 * NAME          : Dot11TdlsDiscoveryRequestWindow
 * PSID          : 2565 (0x0A05)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  The TDLS FSM shall not transmit the TDLS Discovery Request frame within
 *  dot11TDLSDiscoveryRequestWindow DTIM intervals after transmitting TDLS
 *  Discovery Request frame.
 *******************************************************************************/
#define SLSI_PSID_DOT11_TDLS_DISCOVERY_REQUEST_WINDOW 0x0A05

/*******************************************************************************
 * NAME          : Dot11TdlsResponseTimeout
 * PSID          : 2566 (0x0A06)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 5
 * DESCRIPTION   :
 *  If no TDLS Setup Response frame is received within
 *  dot11TDLSResponseTimeout, or if a TDLS Setup Response frame is received
 *  with a nonzero status code, the TDLS initiator STA shall terminate the
 *  setup procedure and discard the TDLS Setup Response frame.
 *******************************************************************************/
#define SLSI_PSID_DOT11_TDLS_RESPONSE_TIMEOUT 0x0A06

/*******************************************************************************
 * NAME          : Dot11TdlsChannelSwitchActivated
 * PSID          : 2567 (0x0A07)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  If dot11TDLSChannelSwitchActivated is TRUE, it need to send TDLS channel
 *  switch response packet as corresponeding
 *******************************************************************************/
#define SLSI_PSID_DOT11_TDLS_CHANNEL_SWITCH_ACTIVATED 0x0A07

/*******************************************************************************
 * NAME          : UnifiTdlsDesignForTestMode
 * PSID          : 2568 (0x0A08)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X00000000
 * DESCRIPTION   :
 *  This MIB shall be used to set TDLS design for test mode
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_DESIGN_FOR_TEST_MODE 0x0A08

/*******************************************************************************
 * NAME          : UnifiTdlsKeyLifeTimeInterval
 * PSID          : 2577 (0x0A11)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X000FFFFF
 * DESCRIPTION   :
 *  This MIB shall be used to build the Key Lifetime Interval in the TDLS
 *  Setup Request frame.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TDLS_KEY_LIFE_TIME_INTERVAL 0x0A11

/*******************************************************************************
 * NAME          : UnifiOxygenDesignForTestMode
 * PSID          : 2583 (0x0A17)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 0X00000000
 * DESCRIPTION   :
 *  This is only used for the test purpose that can verify the requirements
 *  of IBSS/OXYGEN feature
 *******************************************************************************/
#define SLSI_PSID_UNIFI_OXYGEN_DESIGN_FOR_TEST_MODE 0x0A17

/*******************************************************************************
 * NAME          : UnifiPrivateBbbTxFilterConfig
 * PSID          : 4071 (0x0FE7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 0X17
 * DESCRIPTION   :
 *  This MIB entry is written directly to the BBB_TX_FILTER_CONFIG register.
 *  Only the lower eight bits of this register are implemented . Bits 0-3 are
 *  the &apos;Tx Gain&apos;, bits 6-8 are the &apos;Tx Delay&apos;. This
 *  register should only be changed by an expert.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PRIVATE_BBB_TX_FILTER_CONFIG 0x0FE7

/*******************************************************************************
 * NAME          : UnifiPrivateSleepMode
 * PSID          : 4073 (0x0FE9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  UniFi has various sleep modes. By setting bits in this key it is possible
 *  to disable each separately. The bits are: 0 Do not allow full shallow
 *  sleep (as if the radio were active) 1 Do not allow any form of shallow
 *  sleep 2 Do not allow deep sleep 3 Do not allow any clocks to be turned
 *  off (this does not, however, turn any additional clocks on) Note that
 *  this specifies the maximum sleep level, not the current sleep level:
 *  unless the value is set to 14 or 15, UniFi may decide to enter use one of
 *  the remaining forms of sleep that are allowed.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PRIVATE_SLEEP_MODE 0x0FE9

/*******************************************************************************
 * NAME          : UnifiPrivateSwagcFrontEndGain
 * PSID          : 4075 (0x0FEB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  Gain of the path between chip and antenna when LNA is on.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PRIVATE_SWAGC_FRONT_END_GAIN 0x0FEB

/*******************************************************************************
 * NAME          : UnifiPrivateSwagcFrontEndLoss
 * PSID          : 4076 (0x0FEC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  Loss of the path between chip and antenna when LNA is off.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PRIVATE_SWAGC_FRONT_END_LOSS 0x0FEC

/*******************************************************************************
 * NAME          : UnifiPrivateSwagcExtThresh
 * PSID          : 4077 (0x0FED)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       : -25
 * DESCRIPTION   :
 *  Signal level at which external LNA will be used for AGC purposes.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PRIVATE_SWAGC_EXT_THRESH 0x0FED

/*******************************************************************************
 * NAME          : UnifiTxRxTokenBalance
 * PSID          : 4132 (0x1024)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : pages
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This element adjusts the comparison between memory data pages used on the
 *  Tx direction and the ones used in the Rx direction to decide which
 *  direction should get new ones when contention happens. Usually the
 *  comparison aims to penalise the one that has more pages than than the
 *  other, if contention exists, but this mib will artificially penalise the
 *  to-host direction, if negative or the from-host direction, if positive.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_RX_TOKEN_BALANCE 0x1024

/*******************************************************************************
 * NAME          : UnifiRaaSpeculationInterval
 * PSID          : 4140 (0x102C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 100
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It defines the repeatable amount of time,
 *  in ms, that firmware will start to send speculation frames.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_SPECULATION_INTERVAL 0x102C

/*******************************************************************************
 * NAME          : UnifiRaaSpeculationPeriod
 * PSID          : 4141 (0x102D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 10
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It defines the max amount of time, in ms,
 *  that firmware will use for sending speculation frames
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_SPECULATION_PERIOD 0x102D

/*******************************************************************************
 * NAME          : UnifiRaaNumbSpeculationFrames
 * PSID          : 4142 (0x102E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 9
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It defines the max amount of speculation
 *  frames that firmware is allowed to send.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_NUMB_SPECULATION_FRAMES 0x102E

/*******************************************************************************
 * NAME          : UnifiRaaTxSuccessesCount
 * PSID          : 4143 (0x102F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It represents the number of transmitted
 *  frames that were acked at a given rate.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_TX_SUCCESSES_COUNT 0x102F

/*******************************************************************************
 * NAME          : UnifiRaaTxFailuresCount
 * PSID          : 4144 (0x1030)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It represents the number of transmitted
 *  frames that were NOT acked at a given rate.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_TX_FAILURES_COUNT 0x1030

/*******************************************************************************
 * NAME          : UnifiRaaTxPer
 * PSID          : 4145 (0x1031)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It represents the Packet Error Rate for a
 *  given rate on the RAA rate stats.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_TX_PER 0x1031

/*******************************************************************************
 * NAME          : UnifiRaaResetStats
 * PSID          : 4146 (0x1032)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It Resets the stats table used by the RAA.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_RESET_STATS 0x1032

/*******************************************************************************
 * NAME          : UnifiRaaTxMtPer
 * PSID          : 4147 (0x1033)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  For Rate Adaptation Algorithm. It represents the Maximum Tolerable Packet
 *  Error Rate for each rate.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_TX_MT_PER 0x1033

/*******************************************************************************
 * NAME          : UnifiRaaTxHostRate
 * PSID          : 4148 (0x1034)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This MIB is use for host to set a fixed TX rate. Ideally this should be
 *  done by the driver, but since there isn't support for it yet, the best
 *  solution is to set it through this MIB. Default is 0 so that the getter
 *  of this MIB nows that this means "host did not specified any rate".
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RAA_TX_HOST_RATE 0x1034

/*******************************************************************************
 * NAME          : UnifiCoexCmebtcdlPriorityTable
 * PSID          : 5014 (0x1396)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 20
 * MAX           : 20
 * DEFAULT       : { 0X99, 0X99, 0X99, 0XC9, 0X5C, 0X59, 0X55, 0X55, 0X55, 0X55, 0X55, 0X55, 0X55, 0X55, 0X99, 0X55, 0X64, 0X5D, 0X5C, 0X00 }
 * DESCRIPTION   :
 *  This string contains the coexistence configuration for the CME BT CDL
 *  priority table. Description: octet 0 LS Nibble - page - CDL priority for
 *  page octet 0 MS Nibble - page_scan - CDL priority for page scan octet 1
 *  LS Nibble - inquiry - CDL priority for inquiry octet 1 MS Nibble -
 *  inquiry_scan - CDL priority for inquiry scan octet 2 LS Nibble -
 *  role_switch - CDL priority for role_switch octet 2 MS Nibble -
 *  lmp_to_master - CDL priority for LMP to master octet 3 LS Nibble -
 *  lmp_from_master - CDL priority for LMP from master octet 3 MS Nibble -
 *  esco - CDL priority for SCO/eSCO octet 4 LS Nibble - esco_retrans - CDL
 *  priority for eSCO retransmissions octet 4 MS Nibble - polling - CDL
 *  priority for polling octet 5 LS Nibble - sniff - CDL priority for start
 *  of sniff octet 5 MS Nibble - bulk_acl - CDL priority for bulk ACL octet 6
 *  LS Nibble - broadcast - CDL priority for broadcast transmissions octet 6
 *  MS Nibble - park - CDL priority for park octet 7 LS Nibble - band_scan -
 *  CDL priority for band scan octet 7 MS Nibble - cond_scan - CDL priority
 *  for conditional scan octet 8 LS Nibble - trim - CDL priority for radio
 *  trim octet 8 MS Nibble - ble_nonconnectable_advert - CDL priority for BLE
 *  non-connectable advertising octet 9 LS Nibble - ble_discoverable_advert -
 *  CDL priority for BLE discoverable advertising octet 9 MS Nibble -
 *  ble_undirected_advert - CDL priority for BLE connectable undirected
 *  advertising octet 10 LS Nibble - ble_directed_advert - CDL priority for
 *  BLE connectable directed advertising octet 10 MS Nibble -
 *  ble_advert_scan_response - CDL priority for BLE advertising scan response
 *  octet 11 LS Nibble - ble_passive_scan - CDL priority for BLE passive
 *  scanning octet 11 MS Nibble - ble_active_scan - CDL priority for BLE
 *  active scanning octet 12 LS Nibble - ble_active_scan_response - CDL
 *  priority for BLE active scanning scanresponse octet 12 MS Nibble -
 *  ble_initiator - CDL priority for BLE initiator octet 13 LS Nibble -
 *  ble_master_establishment - CDL priority for BLE connection establishment
 *  (master) octet 13 MS Nibble - ble_slave_establishment - CDL priority for
 *  BLE connection establishment (slave) octet 14 LS Nibble -
 *  ble_master_anchor - CDL priority for BLE anchor point (master) octet 14
 *  MS Nibble - ble_slave_anchor - CDL priority for BLE anchor point (slave)
 *  octet 15 LS Nibble - ble_master_data - CDL priority for BLE data (master)
 *  octet 15 MS Nibble - ble_slave_data - CDL priority for BLE data (slave)
 *  octet 16 LS Nibble - ant_priority_0 - CDL priority for ANT low priority
 *  packets octet 16 MS Nibble - ant_priority_1 - CDL priority for ANT medium
 *  priority packets octet 17 LS Nibble - ant_priority_2 - CDL priority for
 *  ANT high priority packets octet 17 MS Nibble - low_priority_sniff - CDL
 *  priority for low priority sniff packets octet 18 LS Nibble -
 *  a2dp_puncture_mode - CDL priority for A2DP puncture mode packets octet 18
 *  MS Nibble - a2dp_non_puncture_mode - CDL priority for A2DP non-puncture
 *  mode packets octet 19 LS Nibble - spare - spare entry to complete 16-bit
 *  word octet 19 MS Nibble - spare - spare entry to complete 16-bit word
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_CMEBTCDL_PRIORITY_TABLE 0x1396

/*******************************************************************************
 * NAME          : UnifiCoexCmewlancdlRxPriorityTable
 * PSID          : 5015 (0x1397)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 4
 * MAX           : 4
 * DEFAULT       : { 0XA4, 0X4A, 0X6A, 0X0A }
 * DESCRIPTION   :
 *  This string contains the coexistence configuration for the CME WLAN CDL
 *  receive priority table. Description: MS nibble LS nibble octet 0 - TX/RX
 *  high priority control (A) TX/RX CT Window (4) octet 1 - Min. VIF Duration
 *  (4) TX/RX During WLAN Keep Alive (a) octet 2 - RX During WLAN Opportunity
 *  (6) RX Multicast Rx (a) octet 3 - [unused] (0) RX Beacon (a) (*) used to
 *  ensure that the CDL RX priority is set correctly before trying to send a
 *  PS NULL, or CTS-TO-SELF NB: These values are dynamically changed by the
 *  WLAN firmware during normal WLAN operation.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_CMEWLANCDL_RX_PRIORITY_TABLE 0x1397

/*******************************************************************************
 * NAME          : UnifiCoexCmewlancdlTxPriorityTable
 * PSID          : 5016 (0x1398)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 3
 * MAX           : 3
 * DEFAULT       : { 0X8A, 0X88, 0X0A }
 * DESCRIPTION   :
 *  This string contains the coexistence configuration for the CME WLAN CDL
 *  transmit priority table. Description: MS nibble LS nibble octet 0 -
 *  Broadcast/Multicast non-Beacon Frames Beacons octet 1 - Data and
 *  Management Frames Control Frames octet 2 - [unused] High Priority Control
 *  Frames NB: These values are dynamically changed by the WLAN firmware
 *  during normal WLAN operation.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_CMEWLANCDL_TX_PRIORITY_TABLE 0x1398

/*******************************************************************************
 * NAME          : UnifiCoexAllowAnaDisable
 * PSID          : 5017 (0x1399)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  This allows WL to turn ANA protection OFF. This is needed in EVT1.1 where
 *  BT and WL activity adversely affect each other, resulting in PUNCTURING
 *  MODE not working (The failure case is that BT cannot TX during WL RX
 *  activity).
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_ALLOW_ANA_DISABLE 0x1399

/*******************************************************************************
 * NAME          : UnifiCoexPowerManagementDelayTimeout
 * PSID          : 5018 (0x139A)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : microseconds
 * MIN           : 0
 * MAX           : 2147483647
 * DEFAULT       : 2000
 * DESCRIPTION   :
 *  When UniFi enters power save mode it signals the new state by setting the
 *  power management bit in the frame control field of a NULL frame. It then
 *  remains active for the period since the previous unicast reception, or
 *  since the transmission of the NULL frame, whichever is later. This
 *  attribute controls the maximum time during which UniFi will continue to
 *  listen for data. This allows any buffered data on a remote device to be
 *  cleared. Note that this attribute specifies the timeout used during BT
 *  scan and connect coexistence activities, and is much more aggressive that
 *  the default specified by unifiPowerManagementDelayTimeout
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COEX_POWER_MANAGEMENT_DELAY_TIMEOUT 0x139A

/*******************************************************************************
 * NAME          : UnifiFrontEndControlConfigParam
 * PSID          : 5021 (0x139D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 32
 * MAX           : 32
 * DEFAULT       : { 0X50, 0X51, 0X52, 0X20, 0X14, 0X30, 0X20, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00 }
 * DESCRIPTION   :
 *  The WLAN subsystem on BT-WLAN combo ICs incorporates flexible logic for
 *  directly controlling external RF front-end components. This attribute
 *  specifies a front-end control signal configuration set for the WLAN
 *  subsystem. Each octet selects the front-end control signal that should be
 *  output on a given pin. The 32 octets correspond to settings for PIOs 0 to
 *  31, with the first representing PIO 0 and the last representing PIO 31.
 *  The value that each octet takes defines the behaviour of the pin, with
 *  the available options detailed in the following. NB: the numbers
 *  correspond to an enum in \c mac\hal\halfec.h, and should not be changed
 *  for reasons of backward compatibility. General Signals -_-_-_-_-_-_-_-
 *  0x00 FEC_PIN_UNUSED Not used for front end control. If this option is
 *  selected then the specified PIO may still be used for a purpose other
 *  than front-end control. It is inadvisable to change a given pin between
 *  PIN_UNUSED and other settings in different rows of the table. Internal
 *  Bluetooth-only signals -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- 0x14 FEC_BT_TX
 *  Driven high when the internal Bluetooth subsystem is transmitting and low
 *  at all other times. WLAN 2.4GHz signals -_-_-_-_-_-_-_-_-_- 0x20
 *  FEC_WL2G4_TX Driven high when the WLAN subsystem is transmitting on the
 *  2.4GHz band. WLAN 2.4GHz - Internal Bluetooth signals
 *  -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_ 0x30 FEC_WL2G4BT_RX_WL_LNA
 *  Driven high when the WLAN subsystem is receiving via its own (WLAN) LNA
 *  on the 2.4GHz band, or the internal Bluetooth subsystem is receiving via
 *  the WLANs LNA. WLAN 5GHz signals -_-_-_-_-_-_-_-_- 0x50 FEC_WL5G_TX
 *  Driven high when the WLAN subsystem is transmitting on the 5GHz band.
 *  0x51 FEC_WL5G_RX Driven high when the WLAN subsystem is receiving on the
 *  5GHz band. 0x52 FEC_WL5G_TXRX Driven high when the WLAN subsystem is
 *  either transmitting or receiving on the 5GHz band.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRONT_END_CONTROL_CONFIG_PARAM 0x139D

/*******************************************************************************
 * NAME          : UnifiFrontEndControlMutex
 * PSID          : 5024 (0x13A0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 4095
 * DEFAULT       : 0X0FF0
 * DESCRIPTION   :
 *  This attribute applies to the WLAN subsystem on combo ICs and allows the
 *  combination of activities that must be mutually exclusive because of the
 *  external front-end configuration to be specified. The value is a bit mask
 *  where a set bit indicates that the corresponding combination cannot occur
 *  simultaneously. Bit 0(LSB) : Bluetooth transmit and WLAN 5GHz transmit
 *  Bit 1 : Bluetooth transmit and WLAN 5GHz receive Bit 2 : Bluetooth
 *  receive and WLAN 5GHz transmit Bit 3 : Bluetooth receive and WLAN 5GHz
 *  receive Bit 4 : Bluetooth transmit and WLAN 2.4GHz transmit Bit 5 :
 *  Bluetooth transmit and WLAN 2.4GHz receive, WLAN using WLAN LNA Bit 6 :
 *  Bluetooth transmit and WLAN 2.4GHz receive, WLAN using BT LNA Bit 7 :
 *  Bluetooth receive and WLAN 2.4GHz transmit, BT using WLAN LNA Bit 8 :
 *  Bluetooth receive and WLAN 2.4GHz transmit, BT using BT LNA Bit 9 :
 *  Bluetooth receive and WLAN 2.4GHz receive, both WLAN and BT using BT LNA
 *  Bit 10 : Bluetooth receive and WLAN 2.4GHz receive, both WLAN and BT
 *  using WLAN LNA Bit 11 : Bluetooth receive and WLAN 2.4GHz receive, WLAN
 *  using WLAN LNA and BT using BT LNA Bit 7 and Bit 10 should always be set
 *  when the WLAN subsystem is operating with an externalBluetooth device, as
 *  any external Bluetooth device will never be able to access the WLAN
 *  subsystems internal LNA. Bit 6 and Bit 7 should always be set when the
 *  WLAN subsystem is operating with the internal BT subsystem, since it is
 *  not possible for the internal BT LNA to be used during BT transmit.
 *  Similarly, it is not possible for the internal WLAN LNA to be used during
 *  WLAN transmit.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_FRONT_END_CONTROL_MUTEX 0x13A0

/*******************************************************************************
 * NAME          : UnifiDebugControl
 * PSID          : 5029 (0x13A5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  The set of Debug control values used to enable/disable various levels of
 *  debug output on a per module basis
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DEBUG_CONTROL 0x13A5

/*******************************************************************************
 * NAME          : UnifiTxUsingLdpcEnabled
 * PSID          : 5030 (0x13A6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : TRUE
 * DESCRIPTION   :
 *  This attribute, when TRUE, indicates that LDPC will be used to code
 *  packets, for transmit only. If set to FALSE, chip will not send LDPC
 *  coded packets even if peer supports it. To advertise reception of LDPC
 *  coded packets,enable bit 0 of unifiHtCapabilities, and bit 4 of
 *  unifiVhtCapabilities.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_USING_LDPC_ENABLED 0x13A6

/*******************************************************************************
 * NAME          : UnifiTxSettings
 * PSID          : 5031 (0x13A7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_SETTINGS 0x13A7

/*******************************************************************************
 * NAME          : UnifiTxGainSettings
 * PSID          : 5032 (0x13A8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter gain settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_GAIN_SETTINGS 0x13A8

/*******************************************************************************
 * NAME          : UnifiTxAntennaConnectionLossFrequency
 * PSID          : 5033 (0x13A9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 3940
 * MAX           : 12000
 * DEFAULT       :
 * DESCRIPTION   :
 *  The corresponding set of frequency values for
 *  TxAntennaConnectionLossTable
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_ANTENNA_CONNECTION_LOSS_FREQUENCY 0x13A9

/*******************************************************************************
 * NAME          : UnifiTxAntennaConnectionLoss
 * PSID          : 5034 (0x13AA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  The set of Antenna Connection Loss value, which is used for TPO/EIRP
 *  conversion
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_ANTENNA_CONNECTION_LOSS 0x13AA

/*******************************************************************************
 * NAME          : UnifiTxAntennaMaxGainFrequency
 * PSID          : 5035 (0x13AB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 3940
 * MAX           : 12000
 * DEFAULT       :
 * DESCRIPTION   :
 *  The corresponding set of frequency values for TxAntennaMaxGain
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_ANTENNA_MAX_GAIN_FREQUENCY 0x13AB

/*******************************************************************************
 * NAME          : UnifiTxAntennaMaxGain
 * PSID          : 5036 (0x13AC)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  The set of Antenna Max Gain value, which is used for TPO/EIRP conversion
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_ANTENNA_MAX_GAIN 0x13AC

/*******************************************************************************
 * NAME          : UnifiRxExternalGainFrequency
 * PSID          : 5037 (0x13AD)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 3940
 * MAX           : 12000
 * DEFAULT       :
 * DESCRIPTION   :
 *  The set of RSSI offset value
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RX_EXTERNAL_GAIN_FREQUENCY 0x13AD

/*******************************************************************************
 * NAME          : UnifiRxExternalGain
 * PSID          : 5038 (0x13AE)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  The table giving frequency-dependent RSSI offset value
 *******************************************************************************/
#define SLSI_PSID_UNIFI_RX_EXTERNAL_GAIN 0x13AE

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintRuleSelectorBitmask
 * PSID          : 5039 (0x13AF)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Bitmask indicating which out-of-band constraint rule selectors this rule
 *  applies to (LSB/b0 corresponds to rule selector 0, MSB/b32 corresponds to
 *  rule selector 31)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_RULE_SELECTOR_BITMASK 0x13AF

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintModulationBitmask
 * PSID          : 5040 (0x13B0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Bitmask indicating which modulation types this rule applies to
 *  (LSB/b0=DSSS/CCK, b1= BPSK/QPSK, b2= 16-QAM, b3= 64-QAM (non-HT), and
 *  b4=64-QAM HT)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_MODULATION_BITMASK 0x13B0

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintBandwidthBitmask
 * PSID          : 5041 (0x13B1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Bitmask indicating which channel bandwidths this rule applies to
 *  (LSB/b0=5MHz, b1=10MHz, b2=20MHz, b3=40MHz, etc.)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_BANDWIDTH_BITMASK 0x13B1

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintDistanceToBandEdge
 * PSID          : 5042 (0x13B2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Minimum distance to nearest band edge in 500 kHz units for which this
 *  constraint becomes is applicable.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_DISTANCE_TO_BAND_EDGE 0x13B2

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintMaxPowerEirp
 * PSID          : 5043 (0x13B3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  Maximum power (EIRP) for this particular constraint - specified in units
 *  of quarter dBm.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_MAX_POWER_EIRP 0x13B3

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintSpectralShapingConfig
 * PSID          : 5044 (0x13B4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       :
 * DESCRIPTION   :
 *  Spectral shaping configuration to be used for this particular constraint.
 *  The value is specific to the radio hardware and should only be altered
 *  under advice from the IC supplier.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_SPECTRAL_SHAPING_CONFIG 0x13B4

/*******************************************************************************
 * NAME          : UnifiTxPowerAdjustFrequency
 * PSID          : 5049 (0x13B9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 4800
 * MAX           : 12000
 * DEFAULT       :
 * DESCRIPTION   :
 *  Frequency reference point for a row in unifiTxPowerAdjustTable, specified
 *  in 500 kHz units.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_POWER_ADJUST_FREQUENCY 0x13B9

/*******************************************************************************
 * NAME          : UnifiTxPowerAdjustTemperature
 * PSID          : 5050 (0x13BA)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  Temperature reference point for a row in unifiTxPowerAdjustTable,
 *  specified in degrees Celsius
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_POWER_ADJUST_TEMPERATURE 0x13BA

/*******************************************************************************
 * NAME          : UnifiTxPowerAdjustDelta
 * PSID          : 5051 (0x13BB)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -128
 * MAX           : 127
 * DEFAULT       :
 * DESCRIPTION   :
 *  Delta adjustment in quarter dB for a row in unifiTxPowerAdjustTable.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_POWER_ADJUST_DELTA 0x13BB

/*******************************************************************************
 * NAME          : UnifiTxPowerDetectorResponse
 * PSID          : 5055 (0x13BF)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter detector response settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_POWER_DETECTOR_RESPONSE 0x13BF

/*******************************************************************************
 * NAME          : UnifiTxDetectorTemperatureCompensation
 * PSID          : 5056 (0x13C0)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter detector temperature compensation settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_DETECTOR_TEMPERATURE_COMPENSATION 0x13C0

/*******************************************************************************
 * NAME          : UnifiTxDetectorFrequencyCompensation
 * PSID          : 5057 (0x13C1)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter detector frequency compensation settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_DETECTOR_FREQUENCY_COMPENSATION 0x13C1

/*******************************************************************************
 * NAME          : UnifiTxOpenLoopTemperatureCompensation
 * PSID          : 5058 (0x13C2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter open-loop temperature compensation settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OPEN_LOOP_TEMPERATURE_COMPENSATION 0x13C2

/*******************************************************************************
 * NAME          : UnifiTxOpenLoopFrequencyCompensation
 * PSID          : 5059 (0x13C3)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 255
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter open-loop frequency compensation settings
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OPEN_LOOP_FREQUENCY_COMPENSATION 0x13C3

/*******************************************************************************
 * NAME          : UnifiTxOfdmSelect
 * PSID          : 5060 (0x13C4)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 4
 * MAX           : 4
 * DEFAULT       :
 * DESCRIPTION   :
 *  Hardware specific transmitter OFDM selection settings.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OFDM_SELECT 0x13C4

/*******************************************************************************
 * NAME          : UnifiTxDigGain
 * PSID          : 5061 (0x13C5)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 16
 * MAX           : 16
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute used to specify gain specific modulation power
 *  optimisation.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_DIG_GAIN 0x13C5

/*******************************************************************************
 * NAME          : UnifiChipTemperature
 * PSID          : 5062 (0x13C6)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : celcius
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute used to read the chip temperature as seen by WLAN radio
 *  firmware
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CHIP_TEMPERATURE 0x13C6

/*******************************************************************************
 * NAME          : UnifiBatteryVoltage
 * PSID          : 5063 (0x13C7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * UNITS         : millivolt
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute used to read the battery voltage
 *******************************************************************************/
#define SLSI_PSID_UNIFI_BATTERY_VOLTAGE 0x13C7

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintRegulatoryDomainBitmask
 * PSID          : 5064 (0x13C8)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Bitmask indicating which regulatory domains this rule applies to
 *  FCC=bit0, ETSI=bit1, JAPAN=bit2
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_REGULATORY_DOMAIN_BITMASK 0x13C8

/*******************************************************************************
 * NAME          : UnifiTxOobConstraintBandEdgeBitmask
 * PSID          : 5065 (0x13C9)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  Bitmask indicating which band edges this rule applies to
 *  RICE_BAND_EDGE_ISM_24G_LOWER = bit 0, RICE_BAND_EDGE_ISM_24G_UPPER = bit
 *  1, RICE_BAND_EDGE_U_NII_1_LOWER = bit 2, RICE_BAND_EDGE_U_NII_1_UPPER =
 *  bit 3, RICE_BAND_EDGE_U_NII_2_LOWER = bit 4, RICE_BAND_EDGE_U_NII_2_UPPER
 *  = bit 5, RICE_BAND_EDGE_U_NII_2E_LOWER = bit 6,
 *  RICE_BAND_EDGE_U_NII_2E_UPPER = bit 7, RICE_BAND_EDGE_U_NII_3_LOWER = bit
 *  8, RICE_BAND_EDGE_U_NII_3_UPPER = bit 9
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TX_OOB_CONSTRAINT_BAND_EDGE_BITMASK 0x13C9

/*******************************************************************************
 * NAME          : UnifiCurrentTxpowerLevel
 * PSID          : 6020 (0x1784)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * UNITS         : dBm
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       :
 * DESCRIPTION   :
 *  This attribute shall indicate the maximum air power for the VIF,
 *  currently used. Expressed in 0.25 dBm units, with the 6 most significant
 *  bits encoding the integer part, and the two least significant bits
 *  encoding the fractional part of the Current Tx Power and subsequently
 *  taking the 2s complement representation for negative values.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CURRENT_TXPOWER_LEVEL 0x1784

/*******************************************************************************
 * NAME          : UnifiUserSetTxpowerLevel
 * PSID          : 6021 (0x1785)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 127
 * DEFAULT       : 127
 * DESCRIPTION   :
 *  Maximum air power for the VIF, set by the user. Expressed in 0.25 dBm
 *  units, with the 6 most significant bits encoding the integer part, and
 *  the two least significant bits encoding the fractional part of the Max
 *  Air Power and subsequently taking the 2s complement representation for
 *  negative values.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_USER_SET_TXPOWER_LEVEL 0x1785

/*******************************************************************************
 * NAME          : UnifiTpcMaxPowerThreshold
 * PSID          : 6022 (0x1786)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       : -55
 * DESCRIPTION   :
 *  Below this RSSI(dBm) threshold, firmware will switch to max power allowed
 *  by current regulatory. If it has been previously reduced due to
 *  unifiTPCMinPowerThreshold.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TPC_MAX_POWER_THRESHOLD 0x1786

/*******************************************************************************
 * NAME          : UnifiTpcMinPowerThreshold
 * PSID          : 6023 (0x1787)
 * PER INTERFACE?: NO
 * TYPE          : SlsiInt16
 * MIN           : -32768
 * MAX           : 32767
 * DEFAULT       : -45
 * DESCRIPTION   :
 *  Above this RSSI(dBm) threshold, firmware will switch to minimum power
 *  that our hardware can support - provided it is lower than current
 *  regulatory limit. Setting it to zero disables the MIB and revert the
 *  power to default state (as specified by regulatory). Disable will only
 *  apply to the next VIF created after changing the value.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_TPC_MIN_POWER_THRESHOLD 0x1787

/*******************************************************************************
 * NAME          : UnifiCcxSupportedVersion
 * PSID          : 6030 (0x178E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 3
 * DESCRIPTION   :
 *  This value enables/disables CCX to specified version. Initialised on
 *  system boot. Possible values: 0-_>No CCX supported 1-_>CCX v4 supported
 *  2-_>CCX v6 (Lite) version supported 3-_>CCXv4 and CCXv6 versions
 *  supported.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CCX_SUPPORTED_VERSION 0x178E

/*******************************************************************************
 * NAME          : UnifiCcxLiteFoundationSupportedServices
 * PSID          : 6031 (0x178F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 1
 * MAX           : 2
 * DEFAULT       : 2
 * DESCRIPTION   :
 *  This value should show which version of CCX Lite Foundation services is
 *  supported by the device. Only valid if CCX Lite supported. Possible
 *  values: 1-_>CCX Lite Foundation Services v1 supported 2-_>CCX Lite
 *  Foundation Services v2 supported (v2 includes v1 as well)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CCX_LITE_FOUNDATION_SUPPORTED_SERVICES 0x178F

/*******************************************************************************
 * NAME          : UnifiCcxLiteVoiceSupportedServices
 * PSID          : 6032 (0x1790)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 1
 * DESCRIPTION   :
 *  This value should show which version of of CCX Lite Voice services is
 *  supported by the device. Only valid if CCX Lite supported. Possible
 *  values: 0-_>CCX Lite Voice not supported 1-_>CCX Lite Voice Services v1
 *  supported 2-_>CCX Lite Voice Services v2 supported (v2 includes v1 as
 *  well)
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CCX_LITE_VOICE_SUPPORTED_SERVICES 0x1790

/*******************************************************************************
 * NAME          : UnifiCcxLiteLocationSupportedServices
 * PSID          : 6033 (0x1791)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       :
 * DESCRIPTION   :
 *  This value should show which version of of CCX Lite Location services is
 *  supported by the device. Only valid if CCX Lite supported. Possible
 *  values: 0-_>CCX Lite Location not supported 1-_>CCX Lite Location
 *  Services v1 supported
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CCX_LITE_LOCATION_SUPPORTED_SERVICES 0x1791

/*******************************************************************************
 * NAME          : UnifiCcxVoiceFailureThreshold
 * PSID          : 6034 (0x1792)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint16
 * MIN           : 0
 * MAX           : 65535
 * DEFAULT       : 3
 * DESCRIPTION   :
 *  If more than this number of CCX Voice frames fail consecutively then a
 *  failure will be reported to MLME, which may cause roaming.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_CCX_VOICE_FAILURE_THRESHOLD 0x1792

/*******************************************************************************
 * NAME          : UnifiPmfAssociationComebackTimeDelta
 * PSID          : 6050 (0x17A2)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint32
 * UNITS         : TU
 * MIN           : 0
 * MAX           : 4294967295
 * DEFAULT       : 1100
 * DESCRIPTION   :
 *  This MIB indicates a delta time for the assocication comeback time
 *  element in the SA Query request frame. The association comeback time in
 *  the SA Query request frame will be set to TSF +
 *  unifiPMFAssociationComebackTimeDelta.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_PMF_ASSOCIATION_COMEBACK_TIME_DELTA 0x17A2

/*******************************************************************************
 * NAME          : UnifiDebugEnable
 * PSID          : 6071 (0x17B7)
 * PER INTERFACE?: NO
 * TYPE          : SlsiBool
 * MIN           : 0
 * MAX           : 1
 * DEFAULT       : FALSE
 * DESCRIPTION   :
 *  Debug to host state. Debug is either is sent to the host or it isn't.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DEBUG_ENABLE 0x17B7

/*******************************************************************************
 * NAME          : UnifiRegulatoryParameters
 * PSID          : 8011 (0x1F4B)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 3
 * MAX           : 73
 * DEFAULT       :
 * DESCRIPTION   :
 *  Regulatory parameters. Each row of the table contains the regulatory
 *  rules for one country: octet 0 - first character of alpha2 code for
 *  country octet 1 - second character of alpha2 code for country octet 2 -
 *  regulatory domain for the country Followed by the rules for the country,
 *  numbered 0..n in this description octet 7n+3 - LSB start frequency octet
 *  7n+4 - MSB start frequency octet 7n+5 - LSB end frequency octet 7n+6 -
 *  MSB end frequency octet 7n+7 - maximum bandwidth octet 7n+8 - maximum
 *  power octet 7n+9 - rule flags
 *******************************************************************************/
#define SLSI_PSID_UNIFI_REGULATORY_PARAMETERS 0x1F4B

/*******************************************************************************
 * NAME          : UnifiSupportedChannels
 * PSID          : 8012 (0x1F4C)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 0
 * MAX           : 20
 * DEFAULT       :  {0X01,0X0E,0X24,0X04,0X34,0X04,0X64,0X0C,0X95,0X05}
 * DESCRIPTION   :
 *  Supported 20MHz channel centre frequency grouped in sub-bands. For each
 *  sub-band: starting channel number, followed by number of channels
 *******************************************************************************/
#define SLSI_PSID_UNIFI_SUPPORTED_CHANNELS 0x1F4C

/*******************************************************************************
 * NAME          : UnifiDefaultCountry
 * PSID          : 8013 (0x1F4D)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 3
 * MAX           : 3
 * DEFAULT       : 00
 * DESCRIPTION   :
 *  Allows setting of default country code.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_DEFAULT_COUNTRY 0x1F4D

/*******************************************************************************
 * NAME          : UnifiCountryList
 * PSID          : 8014 (0x1F4E)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 2
 * MAX           : 500
 * DEFAULT       : (Too Large to display)
 * DESCRIPTION   :
 *  Defines the ordered list of countries present in unifiRegulatoryTable.
 *  Each country is coded as 2 ASCII characters. If unifiRegulatoryTable is
 *  modified, such as a country is either added, deleted or its relative
 *  location is modified, this MIB has to be updated as well.
 *******************************************************************************/
#define SLSI_PSID_UNIFI_COUNTRY_LIST 0x1F4E

/*******************************************************************************
 * NAME          : UnifiOperatingClassParamters
 * PSID          : 8015 (0x1F4F)
 * PER INTERFACE?: NO
 * TYPE          : SlsiUint8
 * MIN           : 6
 * MAX           : 73
 * DEFAULT       :
 * DESCRIPTION   :
 *  Operating Class parameters. Each row of the table contains the regulatory
 *  rules for one country: octet 0 - for Region Cone octet 1 - for Operating
 *  Class ID octet 2 ~ 3 - for Channel Starting Frequency octet 4 - for
 *  Channel Spacing octet 5 - for Number of Elements in Channel Set octet n -
 *  for Channel Set octet end - for Behavior Limits Set
 *******************************************************************************/
#define SLSI_PSID_UNIFI_OPERATING_CLASS_PARAMTERS 0x1F4F

#ifdef __cplusplus
}
#endif
#endif							/* SLSI_MIB_H__ */
