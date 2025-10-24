// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cli.h"

#include "ccm.h"
#include "gcm.h"
#include "aes.h"
#include "hkdf.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "rsa.h"

static void cli_mbedtls_help(void)
{
//Note!!! Required CONFIG_MBEDTLS and MBEDTLS_SELF_TEST
//Note!!! ecdsa Using hardware acceleration, need to be defined MBEDTLS_ECDSA_VERIFY_ALT and MBEDTLS_ECDSA_SIGN_ALT

	CLI_LOGI("mbedtls_sha 1/256/512\r\n");
	CLI_LOGI("mbedtls_ecdsa test 0/1 loop_count\r\n");
	CLI_LOGI("mbedtls_aes ccm/gcm/test 0/1\r\n");
	CLI_LOGI("mbedtls_rsa pkcs1v15/pss\r\n");
	CLI_LOGI("mbedtls_hkdf test 0/1 type okm_size\r\n");
	CLI_LOGI("mbedtls_pkcs5 test 0/1 type loop length\r\n");
}

extern int bk_mbedtls_rsa_self_test( int verbose, int type );

static void cli_mbedtls_sha_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv){
	int ret = -1;
	if (argc < 2) {
		cli_mbedtls_help();
		return;
	}
	if(os_strcmp(argv[1], "256") == 0){
		ret = mbedtls_sha256_self_test(0);
	}else if (os_strcmp(argv[1], "512") == 0){
		ret = mbedtls_sha512_self_test(0);
	}else if (os_strcmp(argv[1], "1") == 0){
        ret = mbedtls_sha1_self_test(0);
    }else{
        cli_mbedtls_help();
    }
	if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

extern int mbedtls_ecdsa_self_test( int verbose ,uint32_t loop);
static void cli_mbedtls_ecdsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_mbedtls_help();
		return;
	}
	int verbose;
	uint32_t loop_count;
	verbose    = os_strtoul(argv[2], NULL, 10);
	loop_count = os_strtoul(argv[3], NULL, 10);

	if (os_strcmp(argv[1], "test") == 0) {
		mbedtls_ecdsa_self_test(verbose, loop_count);
	}else
	{
		cli_mbedtls_help();
	}
}

static void cli_mbedtls_rsa_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_mbedtls_help();
		return;
	}	
#if defined(MBEDTLS_RSA_ALT)
	if(os_strcmp(argv[1],"pkcs1v15")==0)
	{
		bk_mbedtls_rsa_self_test(1, 0);
	}
	if(os_strcmp(argv[1],"pss")==0)
	{
		bk_mbedtls_rsa_self_test(1, 1);
	}
#else
	if((os_strcmp(argv[1],"pkcs1v15")==0)||(os_strcmp(argv[1],"pss")==0))
		mbedtls_rsa_self_test(1);
#endif
}

static void cli_mbedtls_aes_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_mbedtls_help();
		return;
	}
	int verbose;
    int ret = -1;
	verbose = os_strtoul(argv[2], NULL, 10);
	
	if (os_strcmp(argv[1], "ccm") == 0) {
		ret = mbedtls_ccm_self_test(verbose);
	}else if (os_strcmp(argv[1], "gcm") == 0) {
		ret = mbedtls_gcm_self_test(verbose);
	}else if (os_strcmp(argv[1], "test") == 0) {
	    ret = mbedtls_aes_self_test(verbose);
	}else{
		cli_mbedtls_help();
	}
    if (0 == ret)
		CLI_LOGI("passed\r\n");
	else
		CLI_LOGE("failed\r\n");
}

extern int mbedtls_hkdf_self_test( int verbose, mbedtls_md_type_t type, uint32_t okm_size);
static void cli_mbedtls_hkdf_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 5) {
		cli_mbedtls_help();
		return;
	}
	int verbose;
	uint32_t size;
	mbedtls_md_type_t type;

	verbose = os_strtoul(argv[2], NULL, 10);
	type    = os_strtoul(argv[3], NULL, 10);
	size    = os_strtoul(argv[4], NULL, 10);

	if(type > MBEDTLS_MD_RIPEMD160)
	{
		CLI_LOGE("input type err!\r\n");
		return;
	}

	if (os_strcmp(argv[1], "test") == 0) {
		mbedtls_hkdf_self_test(verbose, type, size);
	}else{
		cli_mbedtls_help();
	}
}

extern int mbedtls_pkcs5_self_test( int verbose );
extern int mbedtls_pkcs5_self_test2( int verbose, mbedtls_md_type_t type, uint32_t loop, uint32_t length);
static void cli_mbedtls_pkcs5_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_mbedtls_help();
		return;
	}
	int verbose;
	mbedtls_md_type_t type;
	uint32_t loop_count;
	uint32_t len;

	verbose    = os_strtoul(argv[2], NULL, 10);

	if (os_strcmp(argv[1], "test") == 0) {
		mbedtls_pkcs5_self_test(verbose);
	}else if (os_strcmp(argv[1], "test1") == 0) {
		type       = os_strtoul(argv[3], NULL, 10);
		loop_count = os_strtoul(argv[4], NULL, 10);
		len        = os_strtoul(argv[5], NULL, 10);

		mbedtls_pkcs5_self_test2(verbose, type, loop_count, len);
	}else{
		cli_mbedtls_help();
	}
}

#define mbedtls_CMD_CNT (sizeof(s_mbedtls_commands) / sizeof(struct cli_command))
static const struct cli_command s_mbedtls_commands[] = {
	{"mbedtls_sha", "mbedtls_sha {1|256|512}", cli_mbedtls_sha_cmd},
	{"mbedtls_ecdsa", "mbedtls_ecdsa {test} {verbose} {loop_count}", cli_mbedtls_ecdsa_cmd},
	{"mbedtls_rsa", "mbedtls_rsa {pkcs1v15|pss}", cli_mbedtls_rsa_cmd},
	{"mbedtls_aes", "mbedtls_aes {ccm|gcm|test} {verbose}", cli_mbedtls_aes_cmd},
	{"mbedtls_hkdf", "mbedtls_hkdf {test} {verbose} {type} {okm_size}", cli_mbedtls_hkdf_cmd},
	{"mbedtls_pkcs5", "mbedtls_pkcs5 {test} {verbose} {type} {loop} {length}", cli_mbedtls_pkcs5_cmd}
};

int cli_mbedtls_init(void)
{
	return cli_register_commands(s_mbedtls_commands, mbedtls_CMD_CNT);
}


