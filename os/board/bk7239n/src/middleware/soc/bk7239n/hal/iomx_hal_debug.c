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

#include "hal_config.h"
#include "iomx_hw.h"
#include "iomx_hal.h"

typedef void (*iomx_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	iomx_dump_fn_t fn;
} iomx_reg_fn_map_t;

static void iomx_dump_dev_id(void)
{
	SOC_LOGI("dev_id: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0x0 << 2)));
}

static void iomx_dump_ver_id(void)
{
	SOC_LOGI("ver_id: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0x1 << 2)));
}

static void iomx_dump_clkg_reset(void)
{
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t *)(SOC_IOMX_REG_BASE + (0x2 << 2));

	SOC_LOGI("clkg_reset: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	soft_reset: %8x\r\n", r->soft_reset);
	SOC_LOGI("	clkg_bypass: %8x\r\n", r->clkg_bypass);
	SOC_LOGI("	gpio_atpg_rd_en: %8x\r\n", r->gpio_atpg_rd_en);
	SOC_LOGI("	reserved_bit_3_31: %8x\r\n", r->reserved_bit_3_31);
}

static void iomx_dump_dev_status(void)
{
	SOC_LOGI("dev_status: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0x3 << 2)));
}

static void iomx_dump_gpio_intsta(void)
{
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t *)(SOC_IOMX_REG_BASE + (0xc0 << 2));

	SOC_LOGI("gpio_intsta: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xc0 << 2)));
	SOC_LOGI("	gpio_intsta: %8x\r\n", r->gpio_intsta);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void iomx_dump_gpio_input(void)
{
	iomx_gpio_input_t *r = (iomx_gpio_input_t *)(SOC_IOMX_REG_BASE + (0xc4 << 2));

	SOC_LOGI("gpio_input: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xc4 << 2)));
	SOC_LOGI("	gpio_input: %8x\r\n", r->gpio_input);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void iomx_dump_gpio_output(void)
{
	iomx_gpio_output_t *r = (iomx_gpio_output_t *)(SOC_IOMX_REG_BASE + (0xc8 << 2));

	SOC_LOGI("gpio_output: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xc8 << 2)));
	SOC_LOGI("	gpio_output: %8x\r\n", r->gpio_output);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void iomx_dump_gpio_func_o_atpg(void)
{
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t *)(SOC_IOMX_REG_BASE + (0xcc << 2));

	SOC_LOGI("gpio_func_o_atpg: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xcc << 2)));
	SOC_LOGI("	gpio_func_o_atpg: %8x\r\n", r->gpio_func_o_atpg);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void iomx_dump_gpio_func_ie_atpg(void)
{
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t *)(SOC_IOMX_REG_BASE + (0xd0 << 2));

	SOC_LOGI("gpio_func_ie_atpg: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xd0 << 2)));
	SOC_LOGI("	gpio_func_ie_atpg: %8x\r\n", r->gpio_func_ie_atpg);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void iomx_dump_gpio_func_oen_atpg(void)
{
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t *)(SOC_IOMX_REG_BASE + (0xd4 << 2));

	SOC_LOGI("gpio_func_oen_atpg: %8x\r\n", REG_READ(SOC_IOMX_REG_BASE + (0xd4 << 2)));
	SOC_LOGI("	gpio_func_oen_atpg: %8x\r\n", r->gpio_func_oen_atpg);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

#if CFG_HAL_DEBUG_IOMX
static __attribute__((__unused__)) iomx_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, iomx_dump_dev_id},
	{0x1, 0x1, iomx_dump_ver_id},
	{0x2, 0x2, iomx_dump_clkg_reset},
	{0x3, 0x3, iomx_dump_dev_status},
	{0xc0, 0xc0, iomx_dump_gpio_intsta},
	{0xc4, 0xc4, iomx_dump_gpio_input},
	{0xc8, 0xc8, iomx_dump_gpio_output},
	{0xcc, 0xcc, iomx_dump_gpio_func_o_atpg},
	{0xd0, 0xd0, iomx_dump_gpio_func_ie_atpg},
	{0xd4, 0xd4, iomx_dump_gpio_func_oen_atpg},
	{-1, -1, 0}
};

void iomx_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}

void gpio_regs_dump(void)
{
	uint32_t gpio_id;
	iomx_gpio_cfg_t *gpio_cfgs = (iomx_gpio_cfg_t *)GPIO_LL_REG_BASE;

	SOC_LOGI("base=%x\n", (uint32_t)gpio_cfgs);
	for(gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id ++){
		SOC_LOGI("gpio[%d] cfg: %x\n", gpio_id, gpio_cfgs[gpio_id].v);
	}
}

void gpio_struct_dump(gpio_id_t gpio_index)
{
	iomx_gpio_cfg_t *cfg = (iomx_gpio_cfg_t *)(GPIO_LL_REG_BASE + (gpio_index << 2));

	SOC_LOGI("base=%x\n", (uint32_t)cfg);

	SOC_LOGI("gpio[%d] cfg: %x\n", gpio_index, cfg->v);
	SOC_LOGI("	gpio_input:         %8x\r\n", cfg->gpio_input);
	SOC_LOGI("	gpio_output:        %8x\r\n", cfg->gpio_output);
	SOC_LOGI("	resv4:              %8x\r\n", cfg->resv4);
	SOC_LOGI("	resv3:              %8x\r\n", cfg->resv3);
	SOC_LOGI("	gpio_pull_mode:     %8x\r\n", cfg->gpio_pull_mode);
	SOC_LOGI("	gpio_pull_ena:      %8x\r\n", cfg->gpio_pull_ena);
	SOC_LOGI("	resv2:              %8x\r\n", cfg->resv2);
	SOC_LOGI("	input_monitor:      %8x\r\n", cfg->input_monitor);
	SOC_LOGI("	gpio_capacity:      %8x\r\n", cfg->gpio_capacity);
	SOC_LOGI("	gpio_int_type:      %8x\r\n", cfg->gpio_int_type);
	SOC_LOGI("	gpio_int_ena:       %8x\r\n", cfg->gpio_int_ena);
	SOC_LOGI("	gpio_int_clear:     %8x\r\n", cfg->gpio_int_clear);
	SOC_LOGI("	resv1:              %8x\r\n", cfg->resv1);
	SOC_LOGI("	gpio_state:         %8x\r\n", cfg->gpio_state);
	SOC_LOGI("	resv0:              %8x\r\n", cfg->resv0);
	SOC_LOGI("	gpio_func_sel:      %8x\r\n", cfg->gpio_func_sel);

	SOC_LOGI("\r\n");

	iomx_dump_gpio_intsta();
}
#endif
