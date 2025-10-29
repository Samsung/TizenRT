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
#include "cmsis.h"
#include "tfm_int_target_nsc.h"
#include "tfm_secure_api.h"

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_int_target_nsc_stub(void)
{
	return 0;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void psa_int_target(uint32_t inttype, bool target_to_s)
{
	printf("target %d to %s\r\n", (int)inttype, target_to_s ? "s" : "ns");

	if (target_to_s) {
		NVIC_ClearTargetState(inttype);
	} else {
		NVIC_SetTargetState(inttype);
	}
}
