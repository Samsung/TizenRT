// Copyright 2022-2023 Beken
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
void psa_reg_nsc_stub(void)
{
	return 0;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void psa_reg_write(uint32_t addr, uint32_t value)
{
	uint32_t old = REG_READ(addr);
	REG_WRITE(addr, value);
	printf("write: addr=%x, %x => %x\r\n", addr, old, value);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t psa_reg_read(uint32_t addr)
{
	uint32_t v = REG_READ(addr);
	printf("r: addr=%x, value=%x\r\n", addr, v);
	return v;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void psa_reg_dump(uint32_t addr, uint32_t size)
{
	uint32_t v;

	for (int i = 0; i < (size + 3)/ 4; i++) {
		v = REG_READ(addr);
		if (i && (i % 4 == 0)) {
			printf("\r\n");
		}
		printf("%08x ", v);
		addr += 4;
	}
}
