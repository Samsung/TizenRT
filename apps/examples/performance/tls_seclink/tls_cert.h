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
#ifndef __TLS_SECLINK_TEST_CERT_H__
#define __TLS_SECLINK_TEST_CERT_H__

#define SERVER_CERT	"-----BEGIN CERTIFICATE-----\r\n" \
					"MIIBsjCCAVgCFGrow3xBGpUYGxgDOENMySl0ASSPMAoGCCqGSM49BAMCMFgxCzAJ\r\n" \
					"BgNVBAYTAktSMQ4wDAYDVQQHDAVTZW91bDEQMA4GA1UECgwHU2Ftc3VuZzEQMA4G\r\n" \
					"A1UECwwHU2Ftc3VuZzEVMBMGA1UEAwwMVGl6ZW5SVCBUZXN0MB4XDTIxMDIwNDA0\r\n" \
					"MjYyM1oXDTQxMDEzMDA0MjYyM1owXzELMAkGA1UEBhMCS1IxDjAMBgNVBAgMBVNl\r\n" \
					"b3VsMRAwDgYDVQQKDAdTYW1zdW5nMRAwDgYDVQQLDAdTYW1zdW5nMRwwGgYDVQQD\r\n" \
					"DBNUaXplblJUIFRlc3QgRGV2aWNlMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE\r\n" \
					"HKO17GWWIo9dvbsLRuwvw/KM69khyUS4/l27RcoYLQtXzvfrxjRHUk6iSdlPSFZv\r\n" \
					"ItPn/DcR6ycQVxSCZKI2UTAKBggqhkjOPQQDAgNIADBFAiEAie1TxnyEK6OAOTIT\r\n" \
					"Lu66ec0IGuM06WK5i6vnJqXRYrUCIBXU5R+yHo9wRG6ECjPZyRIKU+8dKKQLfiRu\r\n" \
					"51SR216I\r\n" \
					"-----END CERTIFICATE-----\r\n"

#define SERVER_KEY	"-----BEGIN EC PRIVATE KEY-----\r\n" \
					"MHcCAQEEIOyxmMiJiAECcinp9OgmKyfGbbsiEhxcDuDYTfN+kLyWoAoGCCqGSM49\r\n" \
					"AwEHoUQDQgAEHKO17GWWIo9dvbsLRuwvw/KM69khyUS4/l27RcoYLQtXzvfrxjRH\r\n" \
					"Uk6iSdlPSFZvItPn/DcR6ycQVxSCZKI2UQ==\r\n" \
					"-----END EC PRIVATE KEY-----\r\n"

#define ROOT_CA		"-----BEGIN CERTIFICATE-----\r\n" \
					"MIIBrDCCAVECFDD4y94hNpySzp425ivNkzx2wiwpMAoGCCqGSM49BAMCMFgxCzAJ\r\n" \
					"BgNVBAYTAktSMQ4wDAYDVQQHDAVTZW91bDEQMA4GA1UECgwHU2Ftc3VuZzEQMA4G\r\n" \
					"A1UECwwHU2Ftc3VuZzEVMBMGA1UEAwwMVGl6ZW5SVCBUZXN0MB4XDTIxMDIwNDA0\r\n" \
					"MTk1MloXDTQxMDEzMDA0MTk1MlowWDELMAkGA1UEBhMCS1IxDjAMBgNVBAcMBVNl\r\n" \
					"b3VsMRAwDgYDVQQKDAdTYW1zdW5nMRAwDgYDVQQLDAdTYW1zdW5nMRUwEwYDVQQD\r\n" \
					"DAxUaXplblJUIFRlc3QwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASEEH3lE1wK\r\n" \
					"wp9/hB718VwK5uDCVgpjQ3AqbdUOeHVY5TAR/RboiY1u4mUcJ0w+xDetPHntQp6m\r\n" \
					"1U8WFAtUmPj/MAoGCCqGSM49BAMCA0kAMEYCIQCLR6kqr5ZvxkU5FaPp2BqfrAAb\r\n" \
					"2rkFGTYZXKjDGX6W9wIhAIDRb3CsdlEb6HF+QXa1dNXd45bruqD6z+DRX3DZ4QAS\r\n" \
					"-----END CERTIFICATE-----\r\n"

#define SERVER_NAME "TizenRT Test Device"

#endif // __TLS_SECLINK_TEST_CERT_H__