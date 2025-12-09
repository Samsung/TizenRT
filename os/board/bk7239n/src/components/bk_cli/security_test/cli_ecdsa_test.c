// Copyright 2020-2025 Beken
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

#include <os/os.h>
#include "cli.h"
#include <tinyara/security_hal.h>

extern int armino_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign);
extern int armino_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx);

#define SEC_MAX_BUF_SIZE (64)

static void sec_ecdsa_help(void)
{
	cli_printf("Usage: ecdsa {sign} {curve} {index} \n");
}

void sec_ecdsa_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		sec_ecdsa_help();
		return;
	}

	int ret = 0;

    static uint8_t g_ecdsa_hash_data[32] = {
        0xa5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
    };

    hal_data hash = {
        .data = g_ecdsa_hash_data,
        .data_len = 32,
    };

	hal_data data_in = {0};
	hal_data data_out = {0};

    if (os_strcmp(argv[1], "sign") == 0) {
        CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t curve = os_strtoul(argv[2], NULL, 10);
		uint32_t index = os_strtoul(argv[3], NULL, 10);


        hal_ecdsa_mode mode = {0};
        mode.curve = curve;
        mode.hash_t = HAL_HASH_SHA256;

        data_out.data = (uint8_t *)os_malloc(SEC_MAX_BUF_SIZE);
        if (data_out.data == NULL) {
            CLI_LOGE("Failed to allocate memory for data_out\r\n");
            return;
        }
        data_out.data_len = SEC_MAX_BUF_SIZE;

        os_memset(data_out.data, 0, SEC_MAX_BUF_SIZE);

        ret = armino_hal_ecdsa_sign_md(mode, &hash, index, &data_out);
        if (ret != HAL_SUCCESS) {
            CLI_LOGE("ECDSA sign failed: %d\r\n", ret);
            os_free(data_out.data);
            return;
        }

        cli_printf("ECDSA sign success\r\n");

        index++;

        ret = armino_hal_ecdsa_verify_md(mode, &hash, &data_out, index);
        if (ret != HAL_SUCCESS) {
            CLI_LOGE("ECDSA verify failed: %d\r\n", ret);
            os_free(data_out.data);
            return;
        }

        cli_printf("ECDSA verify success\r\n");

        os_free(data_out.data);

    } else {
        sec_ecdsa_help();
        return;
    }

    return;
}


#define ECDSA_TEST_CMD_CNT (sizeof(s_ecdsa_test_commands) / sizeof(struct cli_command))
static const struct cli_command s_ecdsa_test_commands[] = {
	{"ecdsa", "ecdsa {sign} {index} {hex_data}", sec_ecdsa_command },
};

int cli_ecdsa_test_init(void)
{
	return cli_register_commands(s_ecdsa_test_commands, ECDSA_TEST_CMD_CNT);
}