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

#include "arch_delay.h"
#include "sys_ll.h"
#include <soc/soc.h>

/**
 * The delay_us should NOT depends on CPU frequency since it's very unaccurate!
 *
 * Test Result:
 *
 * CPU_FREQ   | cnt       | us
 * -----------------------------------------
 * 26000000   | 1         | 1.4
 *            | 2         | 1.8
 *            | 3         | 2.1
 *            | 6         | 3.1
 *            | 9         | 4.1
 *            | 11        | 5
 *            | 26        | 10
 *            | 55        | 20
 *            | 141       | 50
 *            | 288       | 100
 *            | 2900      | 1000
 * 120000000  | 8         | 1
 *            | 22        | 2
 *            | 35        | 3
 *            | 50        | 4
 *            | 62        | 5
 *            | 128       | 10
 *            | 260       | 20
 *            | 660       | 50
 *            | 1330      | 100
 *            | 14000     | 1000
 */
void arch_delay_cnt(volatile uint32_t cnt)
{
	while (cnt --);
}

void arch_delay_us(uint32_t us)
{
	//TODO
}
