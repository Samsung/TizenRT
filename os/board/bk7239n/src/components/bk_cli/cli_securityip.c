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
#include  "driver/securityip.h"

static void cli_securityip_help(void)
{
	CLI_LOGI("securityip_driver init/deinit\r\n");
	CLI_LOGI("securityip_trng cfg/get\r\n");
}

static void cli_securityip_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_securityip_driver_init());
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_securityip_driver_deinit());
	} else {
		cli_securityip_help();
		return;
	}
}

static void cli_securityip_system_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "version") == 0) {
		
		CLI_LOGI("get securityip system version sucess!\r\n");
	} else if (os_strcmp(argv[1], "state") == 0) {
		
		CLI_LOGI("get securityip system state sucess!\r\n");
	} else if (os_strcmp(argv[1], "reset") == 0) {
		
		CLI_LOGI("securityip system reset!\r\n");
	} else {
		cli_securityip_help();
		return;
	}
}

static void cli_securityip_trng_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_securityip_help();
		return;
	}

	if (os_strcmp(argv[1], "cfg") == 0) {
		BK_LOG_ON_ERR(bk_securityip_trng_def_cfg());
	} else if (os_strcmp(argv[1], "get") == 0) {
		uint8_t * RandNum_p;
		uint8_t * Num_p;

		uint32_t random_size = os_strtoul(argv[2], NULL, 10)&0xffff;
		Num_p = RandNum_p = (uint8_t *)malloc(random_size+4);
		memset(RandNum_p, 0xaa, random_size+4);

		BK_LOG_ON_ERR(bk_securityip_get_trng(random_size, RandNum_p));
		CLI_LOGI("get random num is \r\n");
		for(int i=0; i<random_size + 4; i++){
			if((*Num_p == 0xaa)&&(*(Num_p+1) == 0xaa)){
				bk_printf("  \r\n");
				CLI_LOGI("get random num size = %d bytes!\r\n",i);
				break;
			}
			bk_printf("%02x",*Num_p);
			Num_p++;
		}
		free(RandNum_p);
	} else {
		cli_securityip_help();
		return;
	}
	
}


#define SECURITYIP_CMD_CNT (sizeof(s_securityip_commands) / sizeof(struct cli_command))
static const struct cli_command s_securityip_commands[] = {
	{"securityip_driver", "securityip_driver {init|deinit}", cli_securityip_driver_cmd},
	{"securityip_system", "securityip_system {version|state|reset}", cli_securityip_system_cmd},
	{"securityip_trng", "securityip_trng {cfg|get}", cli_securityip_trng_cmd},
};

int cli_securityip_init(void)
{
	return cli_register_commands(s_securityip_commands, SECURITYIP_CMD_CNT);
}


