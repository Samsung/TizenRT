// Copyright 2024-2025 Beken
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
#include <os/mem.h>
#include "tz_test.h"
#include "tfm_otp_nsc.h"
#include "_otp.h"
#include "tfm_ns_interface.h"

static void cli_psa_help(void)
{
	CLI_LOGI("psa_crypto [aes_cbc|aes_gcm|ecdh|ecdsa|hmac|tls_client\r\n");
	CLI_LOGI("psa_ps [genkey|encdec|attr|get|destroy\r\n");
}


static void cli_psa_custom_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}

	if (os_strcmp(argv[1], "aes_cbc") == 0) {
		tz_aes_cbc_test_main();
	} else if (os_strcmp(argv[1], "aes_gcm") == 0) {
		tz_aes_gcm_test_main();
	} else if (os_strcmp(argv[1], "ecdh") == 0) {
		//ecdh_main();
	} else if (os_strcmp(argv[1], "ecdsa") == 0) {
		//tz_ecdsa_test_main();
	} else if (os_strcmp(argv[1], "hmac") == 0) {
		//tz_hmac_test_main();
	} else if (os_strcmp(argv[1], "sha256") == 0) {
		//sha256_main();
	} else if (os_strcmp(argv[1], "tls_client") == 0) {
		//psa_tls_client_main();
	} else if (os_strcmp(argv[1], "rsa") == 0) {
		#if defined(CONFIG_PSA_RSA_TEST)
		psa_rsa_test_main();
		#endif
	} else {

	}
}

#if defined(CONFIG_TFM_PS)
static void cli_psa_key_manage_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}
	char kn[16];
	uint32_t key_id;

	if (os_strcmp(argv[1], "genkey") == 0) {
		os_strcpy(kn, argv[2]);
		key_id = os_strtoul(argv[3], NULL, 10);
		ps_generate_key_manage(kn, key_id);
	} else if (os_strcmp(argv[1], "encdec") == 0) {
		os_strcpy(kn, argv[2]);
		ps_aes_encrypt_decrypt(kn);
	} else if (os_strcmp(argv[1], "attr") == 0) {
		os_strcpy(kn, argv[2]);
		ps_get_key_attributes(kn);
	} else if (os_strcmp(argv[1], "get") == 0) {
		os_strcpy(kn, argv[2]);
		ps_get_key_id_by_name(kn);
	} else if (os_strcmp(argv[1], "destroy") == 0) {
		os_strcpy(kn, argv[2]);
		ps_destroy_key_by_name(kn);
	} else if (os_strcmp(argv[1], "destroyid") == 0) {
		key_id = os_strtoul(argv[2], NULL, 10);
		ps_destroy_key_by_id(key_id);
	} else
	{
		cli_psa_help();
	}
}
#endif

#if defined(CONFIG_TFM_DUBHE_KEY_LADDER_NSC)
static void cli_psa_key_ladder_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}

	if (os_strcmp(argv[1], "cbc") == 0) {
		CLI_LOGD("key_ladder cbc test ret =%d\r\n ",te200_key_ladder_aes_cbc_main());
	}else if (os_strcmp(argv[1], "ecb") == 0) {
		CLI_LOGD("key_ladder ecb test ret =%d\r\n ",te200_key_ladder_aes_ecb_main());
	} else
	{
		cli_psa_help();
	}
	
}
#endif
static void cli_psa_key_config_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;

	/*model key*/
	uint8_t model_key[] = {0x3d, 0x00, 0x03, 0x22, 0xbd, 0x7d, 0x2b, 0xd6,
                           0x41, 0x68, 0xc1, 0xde, 0x15, 0x14, 0xcb, 0x73};

	CLI_LOGI("model key write:\r\n");

	tfm_ns_interface_lock();
	bk_otp_update_nsc(1,OTP_MODEL_KEY,model_key,16);
	tfm_ns_interface_unlock();

	tfm_ns_interface_lock();
	ret = bk_otp_read_nsc(OTP_MODEL_KEY);
	tfm_ns_interface_unlock();

	if(ret != 0)
	{
		CLI_LOGE("%s write model key fail ret:%d\r\n",__func__,ret);
	}
	/*ek2*/
	CLI_LOGI("ek2 write:\r\n");
	uint8_t ek2[] = {0x38, 0x41, 0xcb, 0xdc, 0x2a, 0xe7, 0x87, 0x72,
                     0x48, 0x64, 0x7f, 0x35, 0xf5, 0x0a, 0x76, 0x0c};

	tfm_ns_interface_lock();
	bk_otp_update_nsc(2,OTP_EK_2,ek2,16);
	tfm_ns_interface_unlock();

	tfm_ns_interface_lock();
	ret = bk_otp_read_nsc(OTP_EK_2);
	tfm_ns_interface_unlock();

	if(ret != 0)
	{
		CLI_LOGE("%s write ek2 fail ret:%d\r\n",__func__,ret);
	}
	/*ek3*/
	CLI_LOGI("ek3 write:\r\n");
	uint8_t ek3[] = {0xe5, 0x2e, 0x76, 0x27, 0x44, 0x40, 0x5b, 0x9e,
                     0xed, 0xed, 0xfc, 0x2d, 0xd6, 0x66, 0x05, 0x50};

	tfm_ns_interface_lock();
	bk_otp_update_nsc(2,OTP_EK_3,ek3,16);
	tfm_ns_interface_unlock();

	tfm_ns_interface_lock();
	ret = bk_otp_read_nsc(OTP_EK_3);
	tfm_ns_interface_unlock();

	if(ret != 0)
	{
		CLI_LOGE("%s write ek3 fail ret:%d\r\n",__func__,ret);
	}

	/*ek11*/
	CLI_LOGI("ek11 write:\r\n");
	uint8_t ek11[] = {0xb5, 0x8c, 0xd0, 0xe8, 0xfb, 0x9a, 0xb8, 0x3b,
					0xa4, 0xd1, 0xb0, 0x46, 0x25, 0xfe, 0xe6, 0x24,
					0xd6, 0xca, 0xff, 0xf8, 0x90, 0x25, 0x31, 0xe2,
					0x29, 0xd9, 0x58, 0xb5, 0xd7, 0x29, 0x1c, 0x7d};

	tfm_ns_interface_lock();				
	bk_otp_update_nsc(2,OTP_EK_11,ek11,32);
	tfm_ns_interface_unlock();

	tfm_ns_interface_lock();
	ret = bk_otp_read_nsc(OTP_EK_11);
	tfm_ns_interface_unlock();
	
	if(ret != 0)
	{
		CLI_LOGE("%s write ek11 fail ret:%d\r\n",__func__,ret);
	}
}


#define PSA_CUSTOMIZATION_CMD_CNT (sizeof(s_psa_customization_commands) / sizeof(struct cli_command))
static const struct cli_command s_psa_customization_commands[] = {
	{"psa_custom", "psa_custom [aes_cbc|aes_gcm|ecdh|ecdsa|hmac|tls_client", cli_psa_custom_cmd},
#if defined(CONFIG_TFM_PS)
	{"psa_ps", "psa_ps [genkey|encdec|attr|destroy", cli_psa_key_manage_cmd},
#endif
#if defined(CONFIG_TFM_DUBHE_KEY_LADDER_NSC)
	{"key_ladder", "key_ladder [cbc|ecb]", cli_psa_key_ladder_cmd},
#endif
	{"psa_key_config", "psa_key_config ", cli_psa_key_config_cmd},
};
int cli_psa_customization_init(void)
{
	return cli_register_commands(s_psa_customization_commands, PSA_CUSTOMIZATION_CMD_CNT);
}
