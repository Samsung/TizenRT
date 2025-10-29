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

#pragma once

#include <soc/soc.h>
#include "gpio_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct {
	uint32_t gpio_0_31_int_status;
	uint32_t gpio_32_64_int_status;
} gpio_interrupt_status_t;

#define GPIO_LL_REG_BASE            (SOC_AON_GPIO_REG_BASE)
#define GPIO_LL_SYSTEM_REG_BASE     (SOC_SYS_REG_BASE + 0x30 * 4)
#define GPIO_NUM_MAX                (SOC_GPIO_NUM)
#define GPIO_EXIST                  (0xffffffffffff)

static system_gpio_func_mode_hw_t *gpio_system_gpio_func_mode;

static inline void gpio_ll_init(gpio_hw_t *hw)
{
	gpio_system_gpio_func_mode = (system_gpio_func_mode_hw_t *)GPIO_LL_SYSTEM_REG_BASE;
}

static inline void gpio_ll_set_io_mode(gpio_hw_t *hw, uint32 index, const gpio_config_t *config)
{
	uint32 io_mode = 0;

	switch (config->io_mode) {
	case GPIO_OUTPUT_ENABLE:
		io_mode = 0;
		break;

	case GPIO_INPUT_ENABLE:
		io_mode = 3;
		break;

	case GPIO_IO_DISABLE:
		io_mode = 2;
		break;

	default:
		break;
	}

	REG_SET_BITS(&hw->gpio_num[index], GPIO_F_IO_MODE_SET(io_mode), GPIO_F_IO_MODE_EN_M) ;
}

static inline void gpio_ll_set_pull_mode(gpio_hw_t *hw, uint32 index, const gpio_config_t *config)
{
	uint32 pull_mode = 0;

	switch (config->pull_mode) {
	case GPIO_PULL_DISABLE:
		pull_mode = 0;
		break;

	case GPIO_PULL_DOWN_EN:
		pull_mode = 2;
		break;

	case GPIO_PULL_UP_EN:
		pull_mode = 3;
		break;

	default:
		break;
	}

	REG_SET_BITS(&hw->gpio_num[index], GPIO_F_PULL_SET(pull_mode), GPIO_F_PULL_EN_M) ;
}

static inline void gpio_ll_set_func_mode(gpio_hw_t *hw, uint32 index, const gpio_config_t *config)
{
	switch (config->func_mode) {
	case GPIO_SECOND_FUNC_DISABLE:
		REG_CLR_BIT(&hw->gpio_num[index], BIT(6));
		break;
	case GPIO_SECOND_FUNC_ENABLE:
		REG_SET_BIT(&hw->gpio_num[index], BIT(6));
		break;
	default:
		break;
	}
}

static inline void gpio_ll_set_mode(gpio_hw_t *hw, uint32 index, const gpio_config_t *config)
{
	gpio_ll_set_io_mode(hw, index, config);
	gpio_ll_set_pull_mode(hw, index, config);
	gpio_ll_set_func_mode(hw, index, config);
}

//GPIO output enbale low active
static inline void gpio_ll_output_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{
	if (enable)
		enable = 0;
	else
		enable = 1;

	hw->gpio_num[index].cfg.gpio_output_en = enable;
}

//GPIO input enable, high active
static inline void gpio_ll_input_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{
	hw->gpio_num[index].cfg.gpio_input_en = enable;
}

static inline void gpio_ll_pull_up_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{
	hw->gpio_num[index].cfg.gpio_pull_mode = enable;
}

static inline void gpio_ll_pull_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{
	hw->gpio_num[index].cfg.gpio_pull_mode_en = enable;
}

static inline void gpio_ll_second_function_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{

	hw->gpio_num[index].cfg.gpio_2_func_en = enable;
}

static inline void gpio_ll_monitor_input_value_enable(gpio_hw_t *hw, uint32 index, uint32_t enable)
{
	hw->gpio_num[index].cfg.gpio_input_monitor = enable;
}

static inline void gpio_ll_set_capacity(gpio_hw_t *hw, uint32 index, uint32_t capacity)
{
	hw->gpio_num[index].cfg.gpio_capacity = capacity;
}


static inline uint32 gpio_ll_check_func_mode_enable(gpio_hw_t *hw, uint32 index)
{
	return (hw->gpio_num[index].cfg.gpio_2_func_en == 1);
}

static inline uint32 gpio_ll_check_output_enable(gpio_hw_t *hw, uint32 index)
{
	return (hw->gpio_num[index].cfg.gpio_output_en == 0);
}

static inline uint32 gpio_ll_check_input_enable(gpio_hw_t *hw, uint32 index)
{
	return (hw->gpio_num[index].cfg.gpio_input_en == 1);
}


static inline void gpio_ll_set_output_value(gpio_hw_t *hw, uint32 index, bool value)
{
	hw->gpio_num[index].cfg.gpio_output = value;
}

static inline bool gpio_ll_get_input_value(gpio_hw_t *hw, uint32 index)
{
	return hw->gpio_num[index].cfg.gpio_input;
}

#define gpio_ll_set_gpio_output_value(hw,index,value) gpio_ll_set_output_value(hw, index, value)
#define gpio_ll_get_gpio_input_value(hw,index) gpio_ll_get_input_value(hw, index)

static inline void gpio_ll_set_interrupt_type(gpio_hw_t *hw, uint32 index, gpio_int_type_t mode)
{
	hw->gpio_num[index].cfg.gpio_int_type = mode;
}

static inline void gpio_ll_enable_interrupt(gpio_hw_t *hw, uint32 index)
{
	hw->gpio_num[index].cfg.gpio_int_en = 1;
}

static inline void gpio_ll_disable_interrupt(gpio_hw_t *hw, uint32 index)
{
	hw->gpio_num[index].cfg.gpio_int_en = 0;
}

static inline void gpio_ll_get_interrupt_status(gpio_hw_t *hw, gpio_interrupt_status_t *gpio_status)
{
	gpio_status->gpio_0_31_int_status = REG_READ(&hw->gpio_0_31_int_st);
	//gpio_status->gpio_32_64_int_status = REG_READ(&hw->gpio_32_47_int_st);
}

static inline void gpio_ll_clear_interrupt_status(gpio_hw_t *hw, gpio_interrupt_status_t *gpio_status)
{
	if (gpio_status->gpio_0_31_int_status) {
		REG_WRITE(&hw->gpio_0_31_int_st, gpio_status->gpio_0_31_int_status);
	}
	// if (gpio_status->gpio_32_64_int_status) {
	// 	REG_WRITE(&hw->gpio_32_47_int_st, gpio_status->gpio_32_64_int_status);
	// }
}

static inline void gpio_ll_clear_chan_interrupt_status(gpio_hw_t *hw, uint32 index)
{
	hw->gpio_num[index].cfg.gpio_int_clear = 1;
}

static inline bool gpio_ll_is_interrupt_triggered(gpio_hw_t *hw, uint32 index, gpio_interrupt_status_t *gpio_status)
{
	if (index < GPIO_32)
		return !!((gpio_status->gpio_0_31_int_status) & (GPIO_F_INT_EN << (GPIO_F_INT_EN_MS(index))));
	else
		return !!((gpio_status->gpio_32_64_int_status) & (GPIO_F_INT_EN << (GPIO_F_INT_EN_MS(index - GPIO_32))));
}

static inline void gpio_ll_set_perial_mode(gpio_hw_t *hw, uint32 index, uint32_t mode)
{
	if (index < GPIO_8)
		REG_MCHAN_SET_FIELD(index, &gpio_system_gpio_func_mode->gpio_sys_num[0], GPIO_F_PERIAL_MODE, mode);
	else if (index < GPIO_16)
		REG_MCHAN_SET_FIELD(index-GPIO_8, &gpio_system_gpio_func_mode->gpio_sys_num[1], GPIO_F_PERIAL_MODE, mode);
	else if (index < GPIO_24)
		REG_MCHAN_SET_FIELD(index-GPIO_16, &gpio_system_gpio_func_mode->gpio_sys_num[2], GPIO_F_PERIAL_MODE, mode);
	else if (index < GPIO_32)
		REG_MCHAN_SET_FIELD(index-GPIO_24, &gpio_system_gpio_func_mode->gpio_sys_num[3], GPIO_F_PERIAL_MODE, mode);
	else if (index < GPIO_40)
		REG_MCHAN_SET_FIELD(index-GPIO_32, &gpio_system_gpio_func_mode->gpio_sys_num[4], GPIO_F_PERIAL_MODE, mode);
	else
		REG_MCHAN_SET_FIELD(index-GPIO_40, &gpio_system_gpio_func_mode->gpio_sys_num[5], GPIO_F_PERIAL_MODE, mode);
}

static inline uint32 gpio_ll_get_perial_mode(gpio_hw_t *hw, uint32 index)
{
	if (index < GPIO_8)
		return (REG_MCHAN_GET_FIELD(index, &gpio_system_gpio_func_mode->gpio_sys_num[0], GPIO_F_PERIAL_MODE));
	else if (index < GPIO_16)
		return ((REG_MCHAN_GET_FIELD(index-GPIO_8, &gpio_system_gpio_func_mode->gpio_sys_num[1], GPIO_F_PERIAL_MODE)));
	else if (index < GPIO_24)
		return ((REG_MCHAN_GET_FIELD(index-GPIO_16, &gpio_system_gpio_func_mode->gpio_sys_num[2], GPIO_F_PERIAL_MODE)));
	else if (index < GPIO_32)
		return ((REG_MCHAN_GET_FIELD(index-GPIO_24, &gpio_system_gpio_func_mode->gpio_sys_num[3], GPIO_F_PERIAL_MODE)));
	else if (index < GPIO_40)
		return ((REG_MCHAN_GET_FIELD(index-GPIO_32, &gpio_system_gpio_func_mode->gpio_sys_num[4], GPIO_F_PERIAL_MODE)));
	else
		return ((REG_MCHAN_GET_FIELD(index-GPIO_40, &gpio_system_gpio_func_mode->gpio_sys_num[5], GPIO_F_PERIAL_MODE)));
}

#define gpio_ll_set_gpio_perial_mode(hw, index, mode) gpio_ll_set_perial_mode(hw, index, mode)
#define gpio_ll_get_gpio_perial_mode(hw, index) gpio_ll_get_perial_mode(hw, index)

#if CONFIG_GPIO_WAKEUP_SUPPORT
/* gpio bak configs:just bak only low 16bits of GPIO config:input/output/pull en/... , not include interrupt */
static inline void gpio_ll_bak_configs(uint16_t *gpio_cfg, uint32_t count)
{
	uint32_t  i = 0;

	if (gpio_cfg == NULL)
		return;

	for (i = 0; i < count; i++) {
		gpio_cfg[i] = (uint16_t)REG_READ(GPIO_LL_REG_BASE+i*4);
	}
}

/* gpio bak configs:just restore only low 16bits of GPIO config:input/output/pull en/... , not include interrupt */
static inline void gpio_ll_restore_configs(uint16_t *gpio_cfg, uint32_t count)
{
	uint32_t  i = 0;
	uint32_t val = 0;

	if (gpio_cfg == NULL)
		return;

	for (i = 0; i < count; i++) {
		val = REG_READ(GPIO_LL_REG_BASE+i*4);
		val &= 0xffff0000;
		val |= gpio_cfg[i];
		REG_WRITE((GPIO_LL_REG_BASE+i*4), val);
	}
}

static inline void gpio_ll_bak_int_type_configs(uint32_t *gpio_int_cfg, uint32_t count)
{
	uint32_t  i = 0;

	if (gpio_int_cfg == NULL)
		return;

	for (i = 0; i < count; i++) {
		gpio_int_cfg[i] = REG_READ(GPIO_LL_REG_BASE+(i + 0x40)*4);
	}
}

static inline void gpio_ll_restore_int_type_configs(uint32_t *gpio_int_cfg, uint32_t count)
{
	uint32_t  i = 0;

	if (gpio_int_cfg == NULL)
		return;

	for (i = 0; i < count; i ++) {
		REG_WRITE((GPIO_LL_REG_BASE+(i + 0x40)*4), gpio_int_cfg[i]);
	}
}

static inline void gpio_ll_bak_int_enable_configs(uint32_t *gpio_int_cfg, uint32_t count)
{
	uint32_t  i = 0;

	if (gpio_int_cfg == NULL)
		return;

	for(i = 0; i < count; i++) {
		gpio_int_cfg[i] = REG_READ(GPIO_LL_REG_BASE + (i + 0x43) * 4);
	}
}

static inline void gpio_ll_restore_int_enable_configs(uint32_t *gpio_int_cfg, uint32_t count)
{
	uint32_t  i = 0;

	if(gpio_int_cfg == NULL)
		return;

	for(i = 0; i < count; i ++) {
		REG_WRITE((GPIO_LL_REG_BASE + (i + 0x43) * 4), gpio_int_cfg[i]);
	}
}

/* gpio switch to low power status:set all gpios to input mode */
static inline void gpio_ll_switch_to_low_power_status(uint64_t skip_io)
{
	uint32_t  i = 0;
	uint32_t val = 0;

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(skip_io & (0x1ULL << i))
			continue;
		val = REG_READ(GPIO_LL_REG_BASE+i*4);
		val &= 0xffff0000;
		val |= 0x0008;
		REG_WRITE((GPIO_LL_REG_BASE+i*4), val);
	}
}


#else
/* gpio save */
static inline void gpio_ll_reg_save(uint32_t*  gpio_cfg)
{
	int  i = 0;
	if(gpio_cfg == NULL)
		return;

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		//if(GPIO_EXIST & BIT(i))//temp modify
		{
			gpio_cfg[i] = REG_READ(GPIO_LL_REG_BASE+i*4);
		}
	}
}

/* gpio restore */
static inline void gpio_ll_reg_restore(uint32_t*  gpio_cfg)
{
	int i = 0;
	if(gpio_cfg == NULL)
		return;
	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		//if(GPIO_EXIST & BIT(i))//temp modify
		{
			REG_WRITE(GPIO_LL_REG_BASE+i*4, gpio_cfg[i]);
		}
	}
}

/* config gpio wakeup type and enable. @type_l: low/high or pos/nege. @type_h: level or edge */
static inline void gpio_ll_wakeup_enable(uint64_t index, uint64_t type_l, uint64_t type_h)
{
	int 	   i = 0;
	uint32_t   rdata = 0;
	uint64_t   index_ini = index;

	/* clear all int enable */
	REG_WRITE(GPIO_LL_REG_BASE+0x43*4, 0x0);
	REG_WRITE(GPIO_LL_REG_BASE+0x44*4, 0x0);

	rdata = REG_READ(GPIO_LL_REG_BASE+0x40*4);
	for(i = 0; i < 16; i ++)
	{
		if(index & BIT(i))
		{
			REG_WRITE(GPIO_LL_REG_BASE+i*4, 0x3c);
			rdata &= ~(0x3 << i*2);
			if(type_l & BIT(i))  rdata |= BIT(2*i);
			if(type_h & BIT(i))  rdata |= BIT(2*i+1);
		}
		else
		{
			REG_WRITE(GPIO_LL_REG_BASE+i*4, 0x08);
		}
	}
	REG_WRITE(GPIO_LL_REG_BASE+0x40*4, rdata);

	index  = index >> 16;
	type_l = type_l >> 16;
	type_h = type_h >> 16;

	rdata = REG_READ(GPIO_LL_REG_BASE+0x41*4);
	for(i = 0; i < 16; i ++)
	{
		if(index & BIT(i))
		{
			REG_WRITE(GPIO_LL_REG_BASE+0x40+i*4, 0x0c);
			rdata &= ~(0x3 << i*2);
			if(type_l & BIT(i))  rdata |= BIT(2*i);
			if(type_h & BIT(i))  rdata |= BIT(2*i+1);
		}
		else
		{
			REG_WRITE(GPIO_LL_REG_BASE+0x40+i*4, 0x08);
		}
	}

	REG_WRITE(GPIO_LL_REG_BASE+0x41*4, rdata);

	index  = index >> 16;
	type_l = type_l >> 16;
	type_h = type_h >> 16;

	rdata = REG_READ(GPIO_LL_REG_BASE+0x42*4);
	for(i = 0; i < 16; i ++)
	{
		if(index & BIT(i))
		{
			REG_WRITE(GPIO_LL_REG_BASE+0x80+i*4, 0x0c);
			rdata &= ~(0x3 << i*2);
			if(type_l & BIT(i))  rdata |= BIT(2*i);
			if(type_h & BIT(i))  rdata |= BIT(2*i+1);
		}
		else
		{
			REG_WRITE(GPIO_LL_REG_BASE+0x80+i*4, 0x08);
		}
	}

	REG_WRITE(GPIO_LL_REG_BASE+0x42*4, rdata);

extern void delay(INT32 num);
	delay(10);
	REG_WRITE(GPIO_LL_REG_BASE+0x47*4, 0xffffffff);
	REG_WRITE(GPIO_LL_REG_BASE+0x48*4, 0xffffffff);

	index = index_ini;
	rdata = 0;
	for(i = 0; i < 32; i ++)
	{
		if(index & BIT(i))
		{
			rdata |= BIT(i);
		}
	}

	REG_WRITE(GPIO_LL_REG_BASE+0x43*4, rdata);

	index = index >> 32;
	rdata = 0;
	for(i = 0; i < 16; i ++)
	{
		if(index & BIT(i))
		{
			rdata |= BIT(i);
		}
	}

	REG_WRITE(GPIO_LL_REG_BASE+0x44*4, rdata);

}
static inline void gpio_ll_wakeup_interrupt_clear()
{

	uint64_t	int_state = 0;
	uint32_t	int_state1 = 0;
	uint32_t	int_en_state = 0;
	uint32_t	int_en_state1 = 0;

	uint32_t	i = 0;

	int_state = REG_READ(GPIO_LL_REG_BASE+0x46*4);
	int_state1 = REG_READ(GPIO_LL_REG_BASE+0x45*4);

	int_en_state = REG_READ(GPIO_LL_REG_BASE+0x43*4);
	int_en_state1 = REG_READ(GPIO_LL_REG_BASE+0x44*4);

	int_state = (int_state << 32) | (int_state1);

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(int_state & BIT(i))
		{
			break;
		}
	}

	if(i > 31)
	{
		int_en_state1 &=  ~(BIT(i));
		REG_WRITE(GPIO_LL_REG_BASE+0x44*4, int_en_state1);
	}
	else
	{
		int_en_state &=  ~(BIT(i));
		REG_WRITE(GPIO_LL_REG_BASE+0x43*4, int_en_state);
	}

	REG_WRITE(GPIO_LL_REG_BASE+0x47*4, (REG_READ(GPIO_LL_REG_BASE+0x47*4))|int_state);
	REG_WRITE(GPIO_LL_REG_BASE+0x48*4, (REG_READ(GPIO_LL_REG_BASE+0x48*4))|(int_state >> 32));

	extern void delay(INT32 num);
	delay(10);
	REG_WRITE(GPIO_LL_REG_BASE+0x47*4, 0xffffffff);
	REG_WRITE(GPIO_LL_REG_BASE+0x48*4, 0xffffffff);

}

#endif

#ifdef __cplusplus
}
#endif

