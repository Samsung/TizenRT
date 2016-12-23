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

/*
 *  Certificate generation and signing
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "tls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "tls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf     printf
#endif

#include "tls/x509_crt.h"
#include "tls/x509_csr.h"
#include "tls/entropy.h"
#include "tls/ctr_drbg.h"
#include "tls/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MBEDTLS_HARDWARE_RSA)

#if defined(MBEDTLS_X509_CSR_PARSE_C)
#define USAGE_CSR                                                           \
	"    request_file=%%s     default: (empty)\n"                           \
	"                        If request_file is specified, subject_key,\n"  \
	"                        subject_pwd and subject_name are ignored!\n"
#else
#define USAGE_CSR ""
#endif							/* MBEDTLS_X509_CSR_PARSE_C */

#define DFL_ISSUER_CRT          ""
#define DFL_REQUEST_FILE        ""
#define DFL_SUBJECT_KEY         "subject.key"
#define DFL_ISSUER_KEY          "ca.key"
#define DFL_SUBJECT_PWD         ""
#define DFL_ISSUER_PWD          ""
#define DFL_OUTPUT_FILENAME     "cert.crt"
#define DFL_SUBJECT_NAME        "CN=Cert,O=mbed TLS,C=UK"
#define DFL_ISSUER_NAME         "CN=CA,O=mbed TLS,C=UK"
#define DFL_NOT_BEFORE          "20010101000000"
#define DFL_NOT_AFTER           "20301231235959"
#define DFL_SERIAL              "1"
#define DFL_SELFSIGN            0
#define DFL_IS_CA               0
#define DFL_MAX_PATHLEN         -1
#define DFL_KEY_USAGE           0
#define DFL_NS_CERT_TYPE        0

#define USAGE \
	"\n usage: cert_write param=<>...\n"                \
	"\n acceptable parameters:\n"                       \
	USAGE_CSR                                           \
	"    subject_key=%%s      default: subject.key\n"   \
	"    subject_pwd=%%s      default: (empty)\n"       \
	"    subject_name=%%s     default: CN=Cert,O=mbed TLS,C=UK\n"   \
	"\n"                                                \
	"    issuer_crt=%%s       default: (empty)\n"       \
	"                        If issuer_crt is specified, issuer_name is\n"  \
	"                        ignored!\n"                \
	"    issuer_name=%%s      default: CN=CA,O=mbed TLS,C=UK\n"     \
	"\n"                                                \
	"    selfsign=%%d         default: 0 (false)\n"     \
	"                        If selfsign is enabled, issuer_name and\n" \
	"                        issuer_key are required (issuer_crt and\n" \
	"                        subject_* are ignored\n"   \
	"    issuer_key=%%s       default: ca.key\n"        \
	"    issuer_pwd=%%s       default: (empty)\n"       \
	"    output_file=%%s      default: cert.crt\n"      \
	"    serial=%%s           default: 1\n"             \
	"    not_before=%%s       default: 20010101000000\n"\
	"    not_after=%%s        default: 20301231235959\n"\
	"    is_ca=%%d            default: 0 (disabled)\n"  \
	"    max_pathlen=%%d      default: -1 (none)\n"     \
	"    key_usage=%%s        default: (empty)\n"       \
	"                        Comma-separated-list of values:\n"     \
	"                          digital_signature\n"     \
	"                          non_repudiation\n"       \
	"                          key_encipherment\n"      \
	"                          data_encipherment\n"     \
	"                          key_agreement\n"         \
	"                          key_certificate_sign\n"  \
	"                          crl_sign\n"              \
	"    ns_cert_type=%%s     default: (empty)\n"       \
	"                        Comma-separated-list of values:\n"     \
	"                          ssl_client\n"            \
	"                          ssl_server\n"            \
	"                          email\n"                 \
	"                          object_signing\n"        \
	"                          ssl_ca\n"                \
	"                          email_ca\n"              \
	"                          object_signing_ca\n"     \
	"\n"

/*
 * global options
 */
struct options {
	const char *issuer_crt;		/* filename of the issuer certificate   */
	const char *request_file;	/* filename of the certificate request  */
	const char *subject_key;	/* filename of the subject key file     */
	const char *issuer_key;		/* filename of the issuer key file      */
	const char *subject_pwd;	/* password for the subject key file    */
	const char *issuer_pwd;		/* password for the issuer key file     */
	const char *output_file;	/* where to store the constructed key file  */
	const char *subject_name;	/* subject name for certificate         */
	const char *issuer_name;	/* issuer name for certificate          */
	const char *not_before;		/* validity period not before           */
	const char *not_after;		/* validity period not after            */
	const char *serial;			/* serial number string                 */
	int selfsign;				/* selfsign the certificate             */
	int is_ca;					/* is a CA certificate                  */
	int max_pathlen;			/* maximum CA path length               */
	unsigned char key_usage;	/* key usage flags                      */
	unsigned char ns_cert_type;	/* NS cert type                         */
} opt;

int write_certificate(mbedtls_x509write_cert *crt, unsigned char *output, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	//output = (unsigned char *)malloc(4096);
	memset(output, 0, 2048);
	if ((ret = mbedtls_x509write_crt_pem(crt, output, 2048, f_rng, p_rng)) < 0) {
		return (ret);
	}
	return (0);
}

int load_subject_key(mbedtls_pk_context *pk, mbedtls_rsa_context *rsa, uint8_t cert_bitlen)
{
	int ret;
	uint8_t modd[300];
	uint32_t modd_size = 0;
	uint8_t expp[100];
	uint32_t expp_size;
	const mbedtls_pk_info_t *pk_info;

	//ret = mbedtls_pk_parse_keyfile( &loaded_subject_key, opt.subject_key,
	//                         opt.subject_pwd );

	if ((pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) == NULL) {
		return (MBEDTLS_ERR_PK_UNKNOWN_PK_ALG);
	}

	if ((ret = mbedtls_pk_setup(pk, pk_info)) != 0) {
		return MBEDTLS_ERR_PK_UNKNOWN_PK_ALG;
	}

	SsafeLite_Get_PublicKey(cert_bitlen, modd, &modd_size, expp, &expp_size);

	if ((ret = mbedtls_mpi_read_binary(&(rsa->N), modd, modd_size)) != 0 || (ret = mbedtls_mpi_read_binary(&(rsa->E), expp, expp_size)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_mpi_read_binary returned %d\n\n", ret);
		return -1;;
	}

	rsa->len = (mbedtls_mpi_bitlen(&(rsa->N)) + 7) >> 3;

	// mbedtls_rsa_free((mbedtls_rsa_context *)(pk->pk_ctx));
	pk->pk_ctx = (void *)rsa;

	return 0;
}

int mbedtls_load_hard_pvt_key(mbedtls_pk_context *ctx, const char *pwd)
{
	int ret;
//       size_t n;
	/*
	   unsigned char buf[]="-----BEGIN RSA PRIVATE KEY-----\n\
	   MIIEogIBAAKCAQEApZrYdgiUUh5gjyxzfn8CVqO5660PME7j0ZCNlh+2HuprL+1o\n\
	   YesLUZIBqQtwaEXg+fTVTe46TxIrGmyRohn0jWuAIs4RD7Ss1zGmxzeJD2ERjRIX\n\
	   2VtdVuF5Jf16+QtjoiSyodnp7PbVgqlTuZfi1jj9Bwwa2H5mcpNq+8oEaA38ZJqb\n\
	   4G/Df8kjm+MOw8Svwwsp+CZocws4QeITQ+I6/mA5RCV+mFNfCvWu3QNa2NoWRzXb\n\
	   U37QRoD9hcTCgMcQHvDEQiJv5svwn4U2dQtlAL2Vv+YyDyHPJlMJO/XSp39bLz2o\n\
	   6ErsQb+Eud3zzkud5mDyl9eChgDsf/9z/CUvjQIDAQABAoIBABwd4TiRFlWm8K/+\n\
	   VRGaENSS8cMQ2+5Qxp9TEqb1dV9kX5SAk2Nkm8RarMt9tLUPgDYQGpMjqmS4afcP\n\
	   jd5e18WXBedi40SuYVYcCPvz8uJYkblI5CG0QHG60G3hzpowE9d4sZeHPw9LjU6k\n\
	   3yE4p3vFqzgiFwDA3TYAz342e28LAg78UeZXLLo3nURUqeaewhNi9dZcV6HG1Bdp\n\
	   a9YyO5K+mhbcijW9O7ITNzVTZ/Q2uXBu3wsGoNhf6ce4mE98t4oMU4IEc06Qidq1\n\
	   DZip8+HgV9UzTnUUGTw0vWt5UC91JM3o4D+nlG9b4kZcIxBhOKFVvnd1wM8mlA2m\n\
	   VgJDn8ECgYEA8S/zWZ7m4SMSqrTwXWWfLSBXmcnnoVWm6qfBRKbtrXoCv1Kad2i+\n\
	   9bf+s+YmUMUq/QyIafL4/4fR4wCBt5jFvwAYAdF+3sL08vY0Duj7P5cxbL2b2qxy\n\
	   cOUhbDk8Ispcba0MLQdqXSDgaAKOrntrU+lQ9JHAEetmmNA/jobjJl0CgYEAr8aO\n\
	   QLMG6mHzVoH+lQocA+6fv7qbfx9zrMbjX1YpLOND8TmEj7oA1XkufWzUZ79ez89X\n\
	   nY2UA4V+cKUAyAu2Ju5OCXR7aX+YfnMRgoYRj10qHFHMuv9T3JpHIjGetR1dXBo7\n\
	   SAIad0V90tWrZ4vhk8JuiHPsImHKFb8EcTxBOvECgYAKD8QJIxXiooHu2SmF2+1u\n\
	   BoFhCwk8s8QI2gK4CHa12/ctYBbNkHMW8RwRCTLj/7WxZMp0EQzOLC5XV/gRXrAx\n\
	   LaHSwZiXVm69jAPT82g32OjY7ngIAVWs3AIu75xQnzyH3sY5QqvvQqXZ8PFe2gs6\n\
	   LuuPqFzo4ncfPODJ41qqiQKBgHo1hV/MWOmKcoHZ3aPHpJTDSoQl/MiUmHsuyJTX\n\
	   NceEtw83G0hGXSAOodkIWmFLd2LqWJMtBRSxsqgkwHdVVS6aDt2aA9fko6R+4lSq\n\
	   W/keIBISeVImBStihERs1Vg881T+0+qLs9TrFsEIV6SlZ4+w5eKvHiUFclhON4t3\n\
	   Bi8xAoGAWJPJIdGpo6x1DRjwwk6yw6Jcg4DHZeqIKRv7O9hqdhhOPAqgsHqHcr8s\n\
	   f7XH+JZAZTmH3MDVGybF+TRgiWgyHehlukrwFgePpCz89b91MyMAA3LYFdzahgTP\n\
	   Ef7Dw2S6PnXZ4uzlZQCRcSsgUk4RE593+hE3wdb82+9ftZYkNks=\n\
	   -----END RSA PRIVATE KEY-----\n\0";
	 */
	//if ( ( ret = mbedtls_pk_load_file( path, &buf, &n ) ) != 0 )
	//      return ( ret );
	const char mbedtls_test_srv_key_rsa[] =
		"-----BEGIN RSA PRIVATE KEY-----\r\n"
		"MIIEpAIBAAKCAQEAwU2j3efNHdEE10lyuJmsDnjkOjxKzzoTFtBa5M2jAIin7h5r\r\n"
		"lqdStJDvLXJ6PiSa/LY0rCT1d+AmZIycsCh9odrqjObJHJa8/sEEUrM21KP64bF2\r\n"
		"2JDBYbRmUjaiJlOqq3ReB30Zgtsq2B+g2Q0cLUlm91slc0boC4pPaQy1AJDh2oIQ\r\n"
		"Zn2uVCuLZXmRoeJhw81ASQjuaAzxi4bSRr/QuKoRAx5/VqgaHkQYDw+Fi9qLRF7i\r\n"
		"GMZiL8dmjfpd2H3zJ4kpAcWQDj8n8TDISg7v1t7HxydrxwU9esQCPJodPg/oNJhb\r\n"
		"y3NLUpbYEaIsgIhpOVrTD7DeWS8Rx/fqEgEwlwIDAQABAoIBAQCXR0S8EIHFGORZ\r\n"
		"++AtOg6eENxD+xVs0f1IeGz57Tjo3QnXX7VBZNdj+p1ECvhCE/G7XnkgU5hLZX+G\r\n"
		"Z0jkz/tqJOI0vRSdLBbipHnWouyBQ4e/A1yIJdlBtqXxJ1KE/ituHRbNc4j4kL8Z\r\n"
		"/r6pvwnTI0PSx2Eqs048YdS92LT6qAv4flbNDxMn2uY7s4ycS4Q8w1JXnCeaAnYm\r\n"
		"WYI5wxO+bvRELR2Mcz5DmVnL8jRyml6l6582bSv5oufReFIbyPZbQWlXgYnpu6He\r\n"
		"GTc7E1zKYQGG/9+DQUl/1vQuCPqQwny0tQoX2w5tdYpdMdVm+zkLtbajzdTviJJa\r\n"
		"TWzL6lt5AoGBAN86+SVeJDcmQJcv4Eq6UhtRr4QGMiQMz0Sod6ettYxYzMgxtw28\r\n"
		"CIrgpozCc+UaZJLo7UxvC6an85r1b2nKPCLQFaggJ0H4Q0J/sZOhBIXaoBzWxveK\r\n" "nupceKdVxGsFi8CDy86DBfiyFivfBj+47BbaQzPBj7C4rK7UlLjab2rDAoGBAN2u\r\n" "AM2gchoFiu4v1HFL8D7lweEpi6ZnMJjnEu/dEgGQJFjwdpLnPbsj4c75odQ4Gz8g\r\n" "sw9lao9VVzbusoRE/JGI4aTdO0pATXyG7eG1Qu+5Yc1YGXcCrliA2xM9xx+d7f+s\r\n" "mPzN+WIEg5GJDYZDjAzHG5BNvi/FfM1C9dOtjv2dAoGAF0t5KmwbjWHBhcVqO4Ic\r\n" "BVvN3BIlc1ue2YRXEDlxY5b0r8N4XceMgKmW18OHApZxfl8uPDauWZLXOgl4uepv\r\n" "whZC3EuWrSyyICNhLY21Ah7hbIEBPF3L3ZsOwC+UErL+dXWLdB56Jgy3gZaBeW7b\r\n" "vDrEnocJbqCm7IukhXHOBK8CgYEAwqdHB0hqyNSzIOGY7v9abzB6pUdA3BZiQvEs\r\n" "3LjHVd4HPJ2x0N8CgrBIWOE0q8+0hSMmeE96WW/7jD3fPWwCR5zlXknxBQsfv0gP\r\n" "3BC5PR0Qdypz+d+9zfMf625kyit4T/hzwhDveZUzHnk1Cf+IG7Q+TOEnLnWAWBED\r\n" "ISOWmrUCgYAFEmRxgwAc/u+D6t0syCwAYh6POtscq9Y0i9GyWk89NzgC4NdwwbBH\r\n" "4AgahOxIxXx2gxJnq3yfkJfIjwf0s2DyP0kY2y6Ua1OeomPeY9mrIS4tCuDQ6LrE\r\n" "TB6l9VGoxJL4fyHnZb8L5gGvnB1bbD8cL6YPaDiOhcRseC9vBiEuVg==\r\n" "-----END RSA PRIVATE KEY-----\r\n";
	const size_t mbedtls_test_srv_key_rsa_len = sizeof(mbedtls_test_srv_key_rsa);
	//n = strlen(buf);
	if (pwd == NULL)
		//ret = mbedtls_pk_parse_key( ctx, buf, n, NULL, 0 );
	{
		ret = mbedtls_pk_parse_key(ctx, mbedtls_test_srv_key_rsa, mbedtls_test_srv_key_rsa_len, NULL, 0);
	} else
		//ret = mbedtls_pk_parse_key( ctx, buf, n,
		//                            (const unsigned char *) pwd, strlen( pwd ) );
	{
		ret = mbedtls_pk_parse_key(ctx, mbedtls_test_srv_key_rsa, mbedtls_test_srv_key_rsa_len, (const unsigned char *)pwd, strlen(pwd));
	}

	return (ret);
}

int fetch_public_key_to_certificate(unsigned char *result_certificate, uint8_t cert_bitlen /* int argc, char *argv[] */)
{
	int ret = 0;
	//mbedtls_x509_crt issuer_crt;
	mbedtls_pk_context loaded_issuer_key, loaded_subject_key;
	mbedtls_pk_context *issuer_key = &loaded_issuer_key, *subject_key = &loaded_subject_key;
	mbedtls_rsa_context rsa;
	char buf[1024];
//    char issuer_name[128];
	int i;
#if defined(MBEDTLS_X509_CSR_PARSE_C)
	char subject_name[128];
	mbedtls_x509_csr csr;
#endif
	mbedtls_x509write_cert crt;
	mbedtls_mpi serial;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;

	const char *pers = "crt example app";
	unsigned char se_fw_image[1];
	/*
	 * Set to sane values
	 */
	mbedtls_x509write_crt_init(&crt);
	mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
	mbedtls_pk_init(&loaded_issuer_key);
	mbedtls_pk_init(&loaded_subject_key);
	mbedtls_mpi_init(&serial);
	mbedtls_ctr_drbg_init(&ctr_drbg);
#if defined(MBEDTLS_X509_CSR_PARSE_C)
	mbedtls_x509_csr_init(&csr);
#endif
//    mbedtls_x509_crt_init( &issuer_crt );
	memset(buf, 0, 1024);
#if defined(MBEDTLS_PKCS1_V15)
	mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
#endif
#if defined(MBEDTLS_PKCS1_V21)
	mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V21, 0);
#endif
	/*
	   if( argc == 0 )
	   {
	   usage:
	   mbedtls_printf( USAGE );
	   ret = 1;
	   goto exit;
	   }
	 */
	opt.issuer_crt = DFL_ISSUER_CRT;
	opt.request_file = DFL_REQUEST_FILE;
	opt.subject_key = DFL_SUBJECT_KEY;
	opt.issuer_key = DFL_ISSUER_KEY;
	opt.subject_pwd = DFL_SUBJECT_PWD;
	opt.issuer_pwd = DFL_ISSUER_PWD;
	opt.output_file = DFL_OUTPUT_FILENAME;
	opt.subject_name = DFL_SUBJECT_NAME;
	opt.issuer_name = DFL_ISSUER_NAME;
	opt.not_before = DFL_NOT_BEFORE;
	opt.not_after = DFL_NOT_AFTER;
	opt.serial = DFL_SERIAL;
	opt.selfsign = DFL_SELFSIGN;
	opt.is_ca = DFL_IS_CA;
	opt.max_pathlen = DFL_MAX_PATHLEN;
	opt.key_usage = DFL_KEY_USAGE;
	opt.ns_cert_type = DFL_NS_CERT_TYPE;

	/*----------------------------------*/
	opt.is_ca = 0;
	opt.max_pathlen = 0;
	opt.issuer_name = "CN=TinyAraTestCA,O=Samsung,C=KOR";
	opt.subject_name = "CN=TinyAraTest,O=Samsung,C=KOR";
	opt.not_before = "20130101000000";
	opt.not_after = "20251231235959";
	/*  opt.key_usage |= MBEDTLS_X509_KU_DIGITAL_SIGNATURE;
	   opt.key_usage |= MBEDTLS_X509_KU_NON_REPUDIATION;
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_ENCIPHERMENT;
	   opt.key_usage |= MBEDTLS_X509_KU_DATA_ENCIPHERMENT;
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_AGREEMENT;
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_CERT_SIGN;
	   opt.key_usage |= MBEDTLS_X509_KU_CRL_SIGN;
	 */

	/*
	   for( i = 1; i < argc; i++ )
	   {

	   p = argv[i];
	   if( ( q = strchr( p, '=' ) ) == NULL )
	   goto usage;
	   *q++ = '\0';

	   if( strcmp( p, "request_file" ) == 0 )
	   opt.request_file = q;
	   else if( strcmp( p, "subject_key" ) == 0 )
	   opt.subject_key = q;
	   else if( strcmp( p, "issuer_key" ) == 0 )
	   opt.issuer_key = q;
	   else if( strcmp( p, "subject_pwd" ) == 0 )
	   opt.subject_pwd = q;
	   else if( strcmp( p, "issuer_pwd" ) == 0 )
	   opt.issuer_pwd = q;
	   else if( strcmp( p, "issuer_crt" ) == 0 )
	   opt.issuer_crt = q;
	   else if( strcmp( p, "output_file" ) == 0 )
	   opt.output_file = q;
	   else if( strcmp( p, "subject_name" ) == 0 )
	   {
	   opt.subject_name = q;
	   }
	   else if( strcmp( p, "issuer_name" ) == 0 )
	   {
	   opt.issuer_name = q;
	   }
	   else if( strcmp( p, "not_before" ) == 0 )
	   {
	   opt.not_before = q;
	   }
	   else if( strcmp( p, "not_after" ) == 0 )
	   {
	   opt.not_after = q;
	   }
	   else if( strcmp( p, "serial" ) == 0 )
	   {
	   opt.serial = q;
	   }
	   else if( strcmp( p, "selfsign" ) == 0 )
	   {
	   opt.selfsign = atoi( q );
	   if( opt.selfsign < 0 || opt.selfsign > 1 )
	   goto usage;
	   }
	   else if( strcmp( p, "is_ca" ) == 0 )
	   {
	   opt.is_ca = atoi( q );
	   if( opt.is_ca < 0 || opt.is_ca > 1 )
	   goto usage;
	   }
	   else if( strcmp( p, "max_pathlen" ) == 0 )
	   {
	   opt.max_pathlen = atoi( q );
	   if( opt.max_pathlen < -1 || opt.max_pathlen > 127 )
	   goto usage;
	   }
	   else if( strcmp( p, "key_usage" ) == 0 )
	   {
	   while( q != NULL )
	   {
	   if( ( r = strchr( q, ',' ) ) != NULL )
	   *r++ = '\0';

	   if( strcmp( q, "digital_signature" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_DIGITAL_SIGNATURE;
	   else if( strcmp( q, "non_repudiation" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_NON_REPUDIATION;
	   else if( strcmp( q, "key_encipherment" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_ENCIPHERMENT;
	   else if( strcmp( q, "data_encipherment" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_DATA_ENCIPHERMENT;
	   else if( strcmp( q, "key_agreement" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_AGREEMENT;
	   else if( strcmp( q, "key_cert_sign" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_KEY_CERT_SIGN;
	   else if( strcmp( q, "crl_sign" ) == 0 )
	   opt.key_usage |= MBEDTLS_X509_KU_CRL_SIGN;
	   else
	   goto usage;

	   q = r;
	   }
	   }
	   else if( strcmp( p, "ns_cert_type" ) == 0 )
	   {
	   while( q != NULL )
	   {
	   if( ( r = strchr( q, ',' ) ) != NULL )
	   *r++ = '\0';

	   if( strcmp( q, "ssl_client" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_SSL_CLIENT;
	   else if( strcmp( q, "ssl_server" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_SSL_SERVER;
	   else if( strcmp( q, "email" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_EMAIL;
	   else if( strcmp( q, "object_signing" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_OBJECT_SIGNING;
	   else if( strcmp( q, "ssl_ca" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_SSL_CA;
	   else if( strcmp( q, "email_ca" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_EMAIL_CA;
	   else if( strcmp( q, "object_signing_ca" ) == 0 )
	   opt.ns_cert_type |= MBEDTLS_X509_NS_CERT_TYPE_OBJECT_SIGNING_CA;
	   else
	   goto usage;

	   q = r;
	   }
	   }
	   else
	   goto usage;
	   }
	 */
	mbedtls_printf("\n");

	/*
	 * 0. Seed the PRNG
	 */
	mbedtls_printf("  . Seeding the random number generator...");
	fflush(stdout);

	mbedtls_entropy_init(&entropy);
	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_ctr_drbg_seed returned %d - %s\n", ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	// Parse serial to MPI
	//
	mbedtls_printf("  . Reading serial number...");
	fflush(stdout);

	if ((ret = mbedtls_mpi_read_string(&serial, 10, opt.serial)) != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_mpi_read_string returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	// Parse issuer certificate if present
	//
#if 0
	if (!opt.selfsign && strlen(opt.issuer_crt)) {
		/*
		 * 1.0.a. Load the certificates
		 */
		mbedtls_printf("  . Loading the issuer certificate ...");
		fflush(stdout);

		if ((ret = mbedtls_x509_crt_parse_file(&issuer_crt, opt.issuer_crt)) != 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse_file returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		ret = mbedtls_x509_dn_gets(issuer_name, sizeof(issuer_name), &issuer_crt.subject);
		if (ret < 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509_dn_gets returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		opt.issuer_name = issuer_name;

		mbedtls_printf(" ok\n");
	}
#endif

#if defined(MBEDTLS_X509_CSR_PARSE_C)
	// Parse certificate request if present
	//
	if (!opt.selfsign && strlen(opt.request_file)) {
		/*
		 * 1.0.b. Load the CSR
		 */
		mbedtls_printf("  . Loading the certificate request ...");
		fflush(stdout);

		if ((ret = mbedtls_x509_csr_parse_file(&csr, opt.request_file)) != 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509_csr_parse_file returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		ret = mbedtls_x509_dn_gets(subject_name, sizeof(subject_name), &csr.subject);
		if (ret < 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509_dn_gets returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		opt.subject_name = subject_name;
		subject_key = &csr.pk;

		mbedtls_printf(" ok\n");
	}
#endif							/* MBEDTLS_X509_CSR_PARSE_C */

	/*
	 * 1.1. Load the keys
	 */
	if (!opt.selfsign && !strlen(opt.request_file)) {
		mbedtls_printf("  . Loading the subject key ...");
		fflush(stdout);
		/* -------------------------------------------------------------------------------------------- */
		//ret = mbedtls_pk_parse_keyfile( &loaded_subject_key, opt.subject_key,
		//                         opt.subject_pwd );
//        if( ret != 0 )
		//      {
//            mbedtls_strerror( ret, buf, 1024 );
//           mbedtls_printf( " failed\n  !  mbedtls_pk_parse_keyfile returned -0x%02x - %s\n\n", -ret, buf );
//            goto exit;
//        }
		ret = load_subject_key(&loaded_subject_key, &rsa, cert_bitlen);
		if (ret != 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  load_subject_key returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}
		/* ----------------------------------------------------------------------------------------------------- */
		mbedtls_printf(" ok\n");
	}

	mbedtls_printf("  . Loading the issuer key ...");
	fflush(stdout);

	/* -------------------------------------------------------------------------------------------- */
//    ret = mbedtls_pk_parse_keyfile( &loaded_issuer_key, opt.issuer_key,
//                             opt.issuer_pwd );
	ret = mbedtls_load_hard_pvt_key(&loaded_issuer_key, opt.issuer_pwd);	//get_CA_pvt_key(&loaded_issuer_key);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_load_hard_pvt_key -x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	/* -------------------------------------------------------------------------------------------- */
	// Check if key and issuer certificate match
	//
#if 0
	if (strlen(opt.issuer_crt)) {
		if (!mbedtls_pk_can_do(&issuer_crt.pk, MBEDTLS_PK_RSA) || mbedtls_mpi_cmp_mpi(&mbedtls_pk_rsa(issuer_crt.pk)->N, &mbedtls_pk_rsa(*issuer_key)->N) != 0 || mbedtls_mpi_cmp_mpi(&mbedtls_pk_rsa(issuer_crt.pk)->E, &mbedtls_pk_rsa(*issuer_key)->E) != 0) {
			mbedtls_printf(" failed\n  !  issuer_key does not match issuer certificate\n\n");
			ret = -1;
			goto exit;
		}
	}
#endif
	mbedtls_printf(" ok\n");

	if (opt.selfsign) {
		opt.subject_name = opt.issuer_name;
		subject_key = issuer_key;
	}

	mbedtls_x509write_crt_set_subject_key(&crt, subject_key);
	mbedtls_x509write_crt_set_issuer_key(&crt, issuer_key);

	/*
	 * 1.0. Check the names for validity
	 */
	if ((ret = mbedtls_x509write_crt_set_subject_name(&crt, opt.subject_name)) != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_subject_name returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	if ((ret = mbedtls_x509write_crt_set_issuer_name(&crt, opt.issuer_name)) != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_issuer_name returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf("  . Setting certificate values ...");
	fflush(stdout);

	ret = mbedtls_x509write_crt_set_serial(&crt, &serial);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_serial returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	ret = mbedtls_x509write_crt_set_validity(&crt, opt.not_before, opt.not_after);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_validity returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	mbedtls_printf("  . Adding the Basic Constraints extension ...");
	fflush(stdout);

	ret = mbedtls_x509write_crt_set_basic_constraints(&crt, opt.is_ca, opt.max_pathlen);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  x509write_crt_set_basic_contraints returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

#if defined(MBEDTLS_SHA1_C)
	mbedtls_printf("  . Adding the Subject Key Identifier ...");
	fflush(stdout);

	ret = mbedtls_x509write_crt_set_subject_key_identifier(&crt);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_subject_key_identifier returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	mbedtls_printf("  . Adding the Authority Key Identifier ...");
	fflush(stdout);

	ret = mbedtls_x509write_crt_set_authority_key_identifier(&crt);
	if (ret != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_authority_key_identifier returned -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");
#endif							/* MBEDTLS_SHA1_C */

	if (opt.key_usage) {
		mbedtls_printf("  . Adding the Key Usage extension ...");
		fflush(stdout);

		ret = mbedtls_x509write_crt_set_key_usage(&crt, opt.key_usage);
		if (ret != 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_key_usage returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		mbedtls_printf(" ok\n");
	}

	if (opt.ns_cert_type) {
		mbedtls_printf("  . Adding the NS Cert Type extension ...");
		fflush(stdout);

		ret = mbedtls_x509write_crt_set_ns_cert_type(&crt, opt.ns_cert_type);
		if (ret != 0) {
			mbedtls_strerror(ret, buf, 1024);
			mbedtls_printf(" failed\n  !  mbedtls_x509write_crt_set_ns_cert_type returned -0x%02x - %s\n\n", -ret, buf);
			goto exit;
		}

		mbedtls_printf(" ok\n");
	}

	/*
	 * 1.2. Writing the request
	 */

	mbedtls_printf("  . Writing the certificate...\n");
	fflush(stdout);

	if ((ret = write_certificate(&crt, result_certificate, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
		mbedtls_strerror(ret, buf, 1024);
		mbedtls_printf(" failed\n  !  write_certifcate -0x%02x - %s\n\n", -ret, buf);
		goto exit;
	}

	mbedtls_printf(" ok\n");

exit:
	mbedtls_x509write_crt_free(&crt);
//    mbedtls_pk_free( &loaded_subject_key );
	mbedtls_pk_free(&loaded_issuer_key);
	mbedtls_rsa_free(&rsa);
	mbedtls_mpi_free(&serial);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
#if defined(_WIN32)
	mbedtls_printf("  + Press Enter to exit this program.\n");
	fflush(stdout);
	getchar();
#endif

	return (ret);
}
#endif							/* MBEDTLS_HARDWARE_RSA */
