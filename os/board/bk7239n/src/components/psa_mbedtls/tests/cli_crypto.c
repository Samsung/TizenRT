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
#include "crypto_test.h"

static void cli_psa_help(void)
{
	CLI_LOGI("psa_crypto [aes_cbc|aes_gcm|ecdh|ecdsa|hmac|tls_client\r\n");
	CLI_LOGI("psa_aes_key [genkey|encdec|attr|destroy\r\n");
}

static void cli_psa_crypto_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}

	if (os_strcmp(argv[1], "aes_cbc") == 0) {
		aes_cbc_main();
	} else if (os_strcmp(argv[1], "aes_gcm") == 0) {
		aes_gcm_main();
	} else if (os_strcmp(argv[1], "ecdh") == 0) {
		ecdh_main();
	} else if (os_strcmp(argv[1], "ecdsa") == 0) {
		ecdsa_main();
	} else if (os_strcmp(argv[1], "hmac") == 0) {
		hmac_main();
	} else if (os_strcmp(argv[1], "sha256") == 0) {
		sha256_main();
	} else if (os_strcmp(argv[1], "sha384") == 0) {
		sha384_main();
	} else if (os_strcmp(argv[1], "tls_client") == 0) {
		psa_tls_client_main();
	} else {

	}
}

static void cli_psa_key_manage_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		return;
	}

	if (os_strcmp(argv[1], "genkey") == 0) {
		generate_key_manage();
	} else if (os_strcmp(argv[1], "encdec") == 0) {
		aes_encrypt_decrypt();
	} else if (os_strcmp(argv[1], "attr") == 0) {
		get_key_attributes();
	} else if (os_strcmp(argv[1], "destroy") == 0) {
		if (argc < 3) {
			return;
		}
		destroy_key(os_strtoul(argv[2], NULL, 10));
	}else
	{
		cli_psa_help();
	}
}


#define PSA_CRYPTO_CMD_CNT (sizeof(s_psa_crypto_commands) / sizeof(struct cli_command))
static const struct cli_command s_psa_crypto_commands[] = {
	{"psa_crypto", "psa_crypto [aes_cbc|aes_gcm|ecdh|ecdsa|hmac|sha256|sha384|tls_client]", cli_psa_crypto_cmd},
	{"psa_aes_key", "psa_aes_key [genkey|encdec|attr|destroy", cli_psa_key_manage_cmd},
};

int cli_psa_crypto_init(void)
{
	return cli_register_commands(s_psa_crypto_commands, PSA_CRYPTO_CMD_CNT);
}
