/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * external/ntpclient/ntpv3.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __APPS_NETUTILS_NTPCLIENT_NTPV3_H
#define __APPS_NETUTILS_NTPCLIENT_NTPV3_H 1

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The NTP version is described in RFC 1305 (NTP version 3), Appendix A:
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |LI | VN  |Mode |    Stratum    |     Poll      |   Precision   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                          Root Delay                           |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                       Root Dispersion                         |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                     Reference Identifier                      |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                   Reference Timestamp (64)                    |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                   Originate Timestamp (64)                    |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                    Receive Timestamp (64)                     |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                    Transmit Timestamp (64)                    |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                 Key Identifier (optional) (32)                |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                                                               |
 *   |                 Message Digest (optional) (128)               |
 *   |                                                               |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define NTP_DATAGRAM_MINSIZE (12 * sizeof(uint32_t))
#define NTP_DATAGRAM_MAXSIZE (17 * sizeof(uint32_t))

/*  Leap Indicator (LI):  2-bit code warning of impending leap-second to be
 *  inserted at the end of the last day of the current month. Bits are coded
 *  as follows:
 *
 *    00      no warning
 *    01      last minute has 61 seconds
 *    10      last minute has 59 seconds
 *    11      alarm condition (clock not synchronized)
 *
 *  Version Number (VN):  This is a three-bit integer indicating the NTP
 *  version number.  In this case, three (3).
 *
 *  Mode: This is a three-bit integer indicating the mode, with values
 *  defined as follows:
 *
 *    0 reserved
 *    1 symmetric active
 *    2 symmetric passive
 *    3 client
 *    4 server
 *    5 broadcast
 *    6 reserved for NTP control message (see Appendix B)
 *    7 reserved for private use
 *
 * Stratum: This is a eight-bit integer indicating the stratum level of the
 * local clock, with values defined as follows:
 *
 *    0 unspecified
 *    1 primary reference (e.g., radio clock)
 *    2-255 secondary reference (via NTP)
 *
 * Poll Interval: This is an eight-bit signed integer indicating the maximum
 * interval between successive messages, in seconds to the nearest power of
 * two.
 *
 * Precision: This is an eight-bit signed integer indicating the precision
 * of the local clock, in seconds to the nearest power of two.
 *
 * Root Delay: This is a 32-bit signed fixed-point number indicating the
 * total roundtrip delay to the primary reference source, in seconds with
 * fraction point between bits 15 and 16. Note that this variable can take
 * on both positive and negative values, depending on clock precision and
 * skew.
 *
 * Root Dispersion: This is a 32-bit signed fixed-point number indicating
 * the maximum error relative to the primary reference source, in seconds
 * with fraction point between bits 15 and 16. Only positive values greater
 * than zero are possible.
 *
 * Reference Clock Identifier: This is a 32-bit code identifying the
 * particular reference clock. In the case of stratum 0 (unspecified) or
 * stratum 1 (primary reference), this is a four-octet, left-justified,
 * zero-padded ASCII string.
 *
 * While not enumerated as part of the NTP specification, the following are
 * suggested ASCII identifiers:
 *
 *    Stratum Code     Meaning
 *      0     DCN      DCN routing protocol
 *      0     NIST     NIST public modem
 *      0     TSP      TSP time protocol
 *      0     DTS      Digital Time Service
 *      1     ATOM     Atomic clock (calibrated)
 *      1     VLF      VLF radio (OMEGA, etc.)
 *      1     callsign Generic radio
 *      1     LORC     LORAN-C radionavigation
 *      1     GOES     GOES UHF environment satellite
 *      1     GPS      GPS UHF satellite positioning
 *
 * In the case of stratum 2 and greater (secondary reference) this is the
 * four-octet Internet address of the primary reference host.
 *
 * Reference Timestamp: This is the local time at which the local clock was
 * last set or corrected, in 64-bit timestamp format.
 *
 * Originate Timestamp: This is the local time at which the request departed
 * the client host for the service host, in 64-bit timestamp format.
 *
 * Receive Timestamp: This is the local time at which the request arrived at
 * the service host, in 64-bit timestamp format.
 *
 * Transmit Timestamp: This is the local time at which the reply departed
 * the service host for the client host, in 64-bit timestamp format.
 *
 * Authenticator (optional): When the NTP authentication mechanism is
 * implemented, this contains the authenticator information defined in
 * Appendix C.
 */

#define MKLVM(l, v, m) ((uint8_t)(l) << 6 | (uint8_t)(v) << 3 | (uint8_t)(m))
#define GETLI(lvm)   ((uint8_t)(lvm) >> 6)
#define GETVN(lvm)   ((uint8_t)(lvm) >> 3) & 7)
#define GETMODE(lvm) ((uint8_t)(lvm) & 7)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct ntp_datagram_s {
	uint8_t lvm;				/* LI | VN | Mode */
	uint8_t stratum;			/* Stratum */
	uint8_t poll;				/* Poll interval */
	uint8_t precision;			/* Precision */
	uint8_t rootdelay[4];		/* Root Delay */
	uint8_t rootdispersion[4];	/* Root dispersion */
	uint8_t refid[4];			/* Reference Clock Identifier */
	uint8_t reftimestamp[8];	/* Rererence Timestamp */
	uint8_t origtimestamp[8];	/* Originate Timestamp */
	uint8_t recvtimestamp[8];	/* Receive Timestamp */
	uint8_t xmittimestamp[8];	/* Transmit Timestamp */
	uint8_t keyid[4];			/* Authenticator data */
	uint8_t digest1[4];
	uint8_t digest2[4];
	uint8_t digest3[4];
	uint8_t digest4[4];
};

#endif							/* __APPS_NETUTILS_NTPCLIENT_NTPV3_H */
