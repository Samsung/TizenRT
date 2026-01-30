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
#include "driver/flash.h"

#if CONFIG_APP_BINARY_SEPARATION && CONFIG_BINARY_SIGNING
extern int up_verify_kernelsignature(uint32_t address);
extern int up_verify_usersignature(uint32_t address);

static void cli_signature_verify_help(void)
{
	CLI_LOGI("sign_verify {kernel|app} - Verify signature of kernel or app\r\n");
	CLI_LOGI("secure_read {address} {size} - Read data from secure flash\r\n");
}

void sec_sign_verify_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 3) {
		cli_signature_verify_help();
		return;
	}
	int32_t ret = 1;
	uint32_t address = os_strtoul(argv[2], NULL, 16);
	
	if (os_strcmp(argv[1], "kernel") == 0) {
		CLI_LOGI("Verifying kernel signature\r\n");
		ret = up_verify_kernelsignature(address);
		if (ret != 0) {
			CLI_LOGE("Kernel signature verification failed\r\n");
		} else {
			CLI_LOGI("Kernel signature verification passed\r\n");
		}
	} else if (os_strcmp(argv[1], "app") == 0) {
        CLI_LOGI("Verifying app signature\r\n");
        ret = up_verify_usersignature(address);
        if (ret != 0) {
            CLI_LOGE("App signature verification failed\r\n");
        } else {
            CLI_LOGI("App signature verification passed\r\n");
        }
	} else {
		cli_signature_verify_help();
		return;
	}
}

#endif

#define SIGNATURE_VERIFY_CMD_CNT (sizeof(s_signature_verify_commands) / sizeof(struct cli_command))
static const struct cli_command s_signature_verify_commands[] = {
#if CONFIG_APP_BINARY_SEPARATION && CONFIG_BINARY_SIGNING
	{"sign_verify", "sign_verify {kernel|app} {address}", sec_sign_verify_command },
#endif
};

int cli_signature_verify_init(void)
{
	return cli_register_commands(s_signature_verify_commands, SIGNATURE_VERIFY_CMD_CNT);
}