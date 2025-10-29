// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!
//
#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"
#include "tfm_secure_api.h"

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_mpc_nsc_stub(void)
{
	return 0;
}

static const uint32_t mpc_base[] = {0x41100000, 0x41110000, 0x41120000, 0x41130000, 0x41200000, 0x41210000, 0x41220000, 0x41230000, 0x41240000, 0x41250000, 0x41260000};
static const char* mpc_dev[] =     {"psram",    "qspi0",    "qspi1",    "otp2",     "flash",    "mem0",     "mem1",     "mem2",     "mem3",     "mem4",     "mem5"};

__tfm_psa_secure_gateway_no_naked_attributes__ void psa_mpc_dump(void)
{
	uint32_t base = 0;
	uint32_t block_max = 0;

	for (int i = 0; i < sizeof(mpc_base)/sizeof(uint32_t); i++) {
		printf("--------%s---------\r\n", mpc_dev[i]);
		base = mpc_base[i];
		block_max = REG_READ(base + 0x10);

		printf("base: %x\r\n", base);
		printf("block_max: %d\r\n", (int)block_max);
		printf("block_size: %d\r\n", REG_READ(base + 0x14));
		printf("lut:\r\n");
		for (int j = 0; j <= block_max; j++) {
			REG_WRITE(base + 0x18, j);
			printf("%08x ", REG_READ(base + 0x1c));
			if (j && (j%4 == 0)) {
				printf("\r\n");
			}
		}
		printf("\r\n");
	}
}

__tfm_psa_secure_gateway_no_naked_attributes__ void psa_mpc_set_secure_attribute(uint32_t dev, uint32_t addr, uint32_t block_num, uint32_t type)
{
	bk_mpc_set_secure_attribute(dev, addr, block_num, type);
	printf("mpc: dev=%d, addr=%x, block_num=%d, type=%d\r\n", (int)dev, addr, (int)block_num, (int)type);
}
