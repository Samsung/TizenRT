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

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include <driver/hal/hal_gpio_types.h>
#include "iomx_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	uint32_t gpio_0_23_int_status;
} iomx_gpio_interrupt_status_t;

#define IOMX_LL_REG_BASE   (SOC_IOMX_REG_BASE)
#define GPIO_LL_REG_BASE   (SOC_IOMX_REG_BASE + 0x40)
#define GPIO_NUM_MAX       (SOC_GPIO_NUM)

//reg dev_id:
static inline void iomx_ll_set_dev_id_value(uint32_t v) {
	iomx_dev_id_t *r = (iomx_dev_id_t*)(IOMX_LL_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_dev_id_value(void) {
	iomx_dev_id_t *r = (iomx_dev_id_t*)(IOMX_LL_REG_BASE + (0x0 << 2));
	return r->v;
}

//reg ver_id:
static inline void iomx_ll_set_ver_id_value(uint32_t v) {
	iomx_ver_id_t *r = (iomx_ver_id_t*)(IOMX_LL_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_ver_id_value(void) {
	iomx_ver_id_t *r = (iomx_ver_id_t*)(IOMX_LL_REG_BASE + (0x1 << 2));
	return r->v;
}

//reg clkg_reset:
static inline void iomx_ll_set_clkg_reset_value(uint32_t v) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_clkg_reset_value(void) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void iomx_ll_set_clkg_reset_soft_reset(uint32_t v) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	r->soft_reset = v;
}

static inline uint32_t iomx_ll_get_clkg_reset_soft_reset(void) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	return r->soft_reset;
}

static inline void iomx_ll_set_clkg_reset_clkg_bypass(uint32_t v) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	r->clkg_bypass = v;
}

static inline uint32_t iomx_ll_get_clkg_reset_clkg_bypass(void) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	return r->clkg_bypass;
}

static inline void iomx_ll_set_clkg_reset_gpio_atpg_rd_en(uint32_t v) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	r->gpio_atpg_rd_en = v;
}

static inline uint32_t iomx_ll_get_clkg_reset_gpio_atpg_rd_en(void) {
	iomx_clkg_reset_t *r = (iomx_clkg_reset_t*)(IOMX_LL_REG_BASE + (0x2 << 2));
	return r->gpio_atpg_rd_en;
}

//reg dev_status:
static inline void iomx_ll_set_dev_status_value(uint32_t v) {
	iomx_dev_status_t *r = (iomx_dev_status_t*)(IOMX_LL_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_dev_status_value(void) {
	iomx_dev_status_t *r = (iomx_dev_status_t*)(IOMX_LL_REG_BASE + (0x3 << 2));
	return r->v;
}

//reg common gpio_cfg using gpio_index
static inline void iomx_ll_set_gpio_cfg_value(uint32_t v, uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_cfg_value(uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_pull_mode(uint32_t v, uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_pull_mode = v;
}

static inline void iomx_ll_set_gpio_pull_ena(uint32_t v, uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_pull_ena = v;
}

static inline void iomx_ll_set_input_monitor(uint32_t v, uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->input_monitor = v;
}

static inline void iomx_ll_set_gpio_capacity(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_capacity = v;
}

static inline uint32_t iomx_ll_get_gpio_capacity(uint32_t id) {
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->gpio_capacity;
}

static inline void iomx_ll_set_gpio_output(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_output = v;
}

static inline bool iomx_ll_get_gpio_input(uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->gpio_input;
}

static inline bool iomx_ll_get_gpio_output(uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->gpio_output;
}

static inline void iomx_ll_set_gpio_interrupt_type(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_int_type = v;
}

static inline void iomx_ll_set_gpio_interrupt_ena(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_int_ena = v;
}

static inline void iomx_ll_set_gpio_interrupt_clear(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_int_clear = v;
}

static inline uint32_t iomx_ll_get_gpio_state(uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->gpio_state;
}

static inline uint32_t iomx_ll_get_gpio_function_sel(uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	return r->gpio_func_sel;
}

static inline void iomx_ll_set_gpio_function_sel(uint32_t v, uint32_t id)
{
	iomx_gpio_cfg_t *r = (iomx_gpio_cfg_t*)(IOMX_LL_REG_BASE + ((0x40 + id) << 2));
	r->gpio_func_sel = v;
}

static inline void iomx_ll_get_interrupt_status(iomx_gpio_interrupt_status_t *gpio_status)
{
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t*)(IOMX_LL_REG_BASE + (0xc0 << 2));
	gpio_status->gpio_0_23_int_status = r->gpio_intsta;
}

static inline bool iomx_ll_is_interrupt_triggered(uint32_t index, iomx_gpio_interrupt_status_t *gpio_status)
{
	return !!((gpio_status->gpio_0_23_int_status) & (0x1 << index));
}

//reg gpio_intsta:

static inline void iomx_ll_set_gpio_intsta_overview_value(uint32_t v) {
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t*)(IOMX_LL_REG_BASE + (0xc0 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_intsta_overview_value(void) {
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t*)(IOMX_LL_REG_BASE + (0xc0 << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_intsta_overview(uint32_t v) {
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t*)(IOMX_LL_REG_BASE + (0xc0 << 2));
	r->gpio_intsta = v;
}

static inline uint32_t iomx_ll_get_gpio_intsta_overview(void) {
	iomx_gpio_intsta_t *r = (iomx_gpio_intsta_t*)(IOMX_LL_REG_BASE + (0xc0 << 2));
	return r->gpio_intsta;
}

//reg gpio_input:

static inline void iomx_ll_set_gpio_input_overview_value(uint32_t v) {
	iomx_gpio_input_t *r = (iomx_gpio_input_t*)(IOMX_LL_REG_BASE + (0xc4 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_input_overview_value(void) {
	iomx_gpio_input_t *r = (iomx_gpio_input_t*)(IOMX_LL_REG_BASE + (0xc4 << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_input_overview(uint32_t v) {
	iomx_gpio_input_t *r = (iomx_gpio_input_t*)(IOMX_LL_REG_BASE + (0xc4 << 2));
	r->gpio_input = v;
}

static inline uint32_t iomx_ll_get_gpio_input_overview(void) {
	iomx_gpio_input_t *r = (iomx_gpio_input_t*)(IOMX_LL_REG_BASE + (0xc4 << 2));
	return r->gpio_input;
}

//reg gpio_output:

static inline void iomx_ll_set_gpio_output_overview_value(uint32_t v) {
	iomx_gpio_output_t *r = (iomx_gpio_output_t*)(IOMX_LL_REG_BASE + (0xc8 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_output_overview_value(void) {
	iomx_gpio_output_t *r = (iomx_gpio_output_t*)(IOMX_LL_REG_BASE + (0xc8 << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_output_overview(uint32_t v) {
	iomx_gpio_output_t *r = (iomx_gpio_output_t*)(IOMX_LL_REG_BASE + (0xc8 << 2));
	r->gpio_output = v;
}

static inline uint32_t iomx_ll_get_gpio_output_overview(void) {
	iomx_gpio_output_t *r = (iomx_gpio_output_t*)(IOMX_LL_REG_BASE + (0xc8 << 2));
	return r->gpio_output;
}

//reg gpio_func_o_atpg:

static inline void iomx_ll_set_gpio_func_o_atpg_value(uint32_t v) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_func_o_atpg_value(void) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_func_o_atpg_gpio_func_o_atpg(uint32_t v) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	r->gpio_func_o_atpg = v;
}

static inline uint32_t iomx_ll_get_gpio_func_o_atpg_gpio_func_o_atpg(void) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	return r->gpio_func_o_atpg;
}

static inline void iomx_ll_set_gpio_func_o_atpg_reserved_24_31(uint32_t v) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t iomx_ll_get_gpio_func_o_atpg_reserved_24_31(void) {
	iomx_gpio_func_o_atpg_t *r = (iomx_gpio_func_o_atpg_t*)(IOMX_LL_REG_BASE + (0xcc << 2));
	return r->reserved_24_31;
}

//reg gpio_func_ie_atpg:

static inline void iomx_ll_set_gpio_func_ie_atpg_value(uint32_t v) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_func_ie_atpg_value(void) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_func_ie_atpg_gpio_func_ie_atpg(uint32_t v) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	r->gpio_func_ie_atpg = v;
}

static inline uint32_t iomx_ll_get_gpio_func_ie_atpg_gpio_func_ie_atpg(void) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	return r->gpio_func_ie_atpg;
}

static inline void iomx_ll_set_gpio_func_ie_atpg_reserved_24_31(uint32_t v) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t iomx_ll_get_gpio_func_ie_atpg_reserved_24_31(void) {
	iomx_gpio_func_ie_atpg_t *r = (iomx_gpio_func_ie_atpg_t*)(IOMX_LL_REG_BASE + (0xd0 << 2));
	return r->reserved_24_31;
}

//reg gpio_func_oen_atpg:

static inline void iomx_ll_set_gpio_func_oen_atpg_value(uint32_t v) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	r->v = v;
}

static inline uint32_t iomx_ll_get_gpio_func_oen_atpg_value(void) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	return r->v;
}

static inline void iomx_ll_set_gpio_func_oen_atpg_gpio_func_oen_atpg(uint32_t v) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	r->gpio_func_oen_atpg = v;
}

static inline uint32_t iomx_ll_get_gpio_func_oen_atpg_gpio_func_oen_atpg(void) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	return r->gpio_func_oen_atpg;
}

static inline void iomx_ll_set_gpio_func_oen_atpg_reserved_24_31(uint32_t v) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t iomx_ll_get_gpio_func_oen_atpg_reserved_24_31(void) {
	iomx_gpio_func_oen_atpg_t *r = (iomx_gpio_func_oen_atpg_t*)(IOMX_LL_REG_BASE + (0xd4 << 2));
	return r->reserved_24_31;
}
#ifdef __cplusplus
}
#endif
