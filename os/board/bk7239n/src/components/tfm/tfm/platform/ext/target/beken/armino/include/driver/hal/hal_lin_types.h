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

#pragma once

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIN_RATE_20KBIT      (20 * 1000) /* bit/s */
#define LIN_CLK_26M          (26 * 1000 * 1000) /* Hz */


#define LIN_BAUD_RATE_MIN    (1000)
#define LIN_BAUD_RATE_MAX    LIN_RATE_20KBIT

typedef enum {
	LIN_SLAVE = 0,
	LIN_MASTER = 1,
} lin_dev_t;

typedef enum {
	LIN_CHAN_0 = 0,		/**< lin gpio0/gpio1/gpuo2*/
	LIN_CHAN_1,		/**< lin gpio30/gpio31/gpuo42 */
	LIN_CHAN_2,		/**< lin gpio40/gpio41/gpuo42 */
	LIN_CHAN_MAX,
} lin_channel_t;

#ifdef __cplusplus
}
#endif

