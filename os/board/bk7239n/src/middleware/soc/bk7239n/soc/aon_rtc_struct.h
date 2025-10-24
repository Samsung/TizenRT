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

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	union {
		struct {
			uint32_t cnt_reset: 1; /**< bit[0] reset current counter(tick) value to 0 */
			uint32_t cnt_stop: 1; /**< bit[1] pause the current counter(tick). RW: 0: goon increase tick cnt, 1: pause the current counter(tick) */
			uint32_t up_int_en: 1; /**< bit[2] the upper tick value interrupt enable:if the tick cnt increase to the upper tick value, will report int to CPU, value is in ; */
			uint32_t tick_int_en: 1; /**< bit[3] the selected value interrupt enable:if the tick cnt increase to the selected tick value, will report irq to CPU */
			uint32_t up_int_sts: 1; /**< bit[4] the intrrupt status of whehter the current counter(tick) value is equal upper tick value */
			uint32_t tick_int_sts: 1; /**< bit[5] the intrrupt status of whehter the current counter(tick) value is equal selected tick value */
			uint32_t en: 1; /**< bit[6] enable the 32k clk of rtc core */
		};
		uint32_t ctrl_v;
	} ctrl;

	/* RW:set the upper tick value[31:0];
	 * if the counter(tick) value is equal upper, the counter(tick) value will count form 0;
	 * it will update up_int_sts and report irq to cpu if enable the irq:up_int_en;
	 */
	uint32_t upper_val;

	/* RW:set the selected tick value[31:0];
	 * it will update select_int_sts and report irq to cpu if the counter(tick) value is equal this value,and enable the irq:select_int_en.
	 */
	uint32_t tick_val;

	uint32_t counter_val; //RO:current tick value of the counter[31:0]
	uint32_t up_val_lpo; //RO:update upper value to lpo[31:0]
	uint32_t tick_val_lpo; //RO:update tick value to lpo[31:0]

	/* RW:set the upper tick value[63:32];
	 * if the counter(tick) value is equal upper, the counter(tick) value will count form 0;
	 * it will update up_int_sts and report irq to cpu if enable the irq:up_int_en;
	 */
	uint32_t upper_val_hi;

	/* RW:set the selected tick value[63:32];
	 * it will update select_int_sts and report irq to cpu if the counter(tick) value is equal this value,and enable the irq:select_int_en.
	 */
	uint32_t tick_val_hi;

	uint32_t up_val_lpo_hi; //RO:update upper value to lpo[63:32]
	uint32_t tick_val_lpo_hi; //RO:update tick value to lpo[63:32]
	uint32_t counter_val_hi; //RO:current tick value of the counter[63:32]
}aon_rtc_hw_t;

#ifdef __cplusplus
}
#endif

