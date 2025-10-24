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


#include <common/bk_include.h>
#include <driver/gpio.h>
#include <driver/int.h>
#include <driver/timer.h>
#include <os/os.h>
#include <os/mem.h>
#include "sys_driver.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include "scr_driver.h"
#include "aon_pmu_driver.h"
#include "scr_hal.h"

typedef struct {
	scr_isr_t callback;
	void *param;
} scr_callback_t;

static scr_callback_t s_scr_isr[BK_SCR_INTR_ALL] = {0};
static uint32_t scr_intr_status = 0;

bk_err_t bk_scr_gpio_init(gpio_scr_map_group_t group)
{
	switch (group) {
		case GPIO_SCR_MAP_GROUP0:
			gpio_dev_unmap(GPIO_0);
			gpio_dev_map(GPIO_0, GPIO_DEV_SCR_IO);
			gpio_dev_unmap(GPIO_1);
			gpio_dev_map(GPIO_1, GPIO_DEV_SCR_CLK);
			gpio_dev_unmap(GPIO_2);
			gpio_dev_map(GPIO_2, GPIO_DEV_SCR_RSTN);
			gpio_dev_unmap(GPIO_3);
			gpio_dev_map(GPIO_3, GPIO_DEV_SCR_VCC);
			break;

		case GPIO_SCR_MAP_GROUP1:
			gpio_dev_unmap(GPIO_30);
			gpio_dev_map(GPIO_30, GPIO_DEV_SCR_CLK);
			gpio_dev_unmap(GPIO_31);
			gpio_dev_map(GPIO_31, GPIO_DEV_SCR_IO);
			gpio_dev_unmap(GPIO_32);
			gpio_dev_map(GPIO_32, GPIO_DEV_SCR_RSTN);
			gpio_dev_unmap(GPIO_43);
			gpio_dev_map(GPIO_43, GPIO_DEV_SCR_VCC);
			break;

		case GPIO_SCR_MAP_GROUP2:
			gpio_dev_unmap(GPIO_40);
			gpio_dev_map(GPIO_40, GPIO_DEV_SCR_CLK);
			gpio_dev_unmap(GPIO_41);
			gpio_dev_map(GPIO_41, GPIO_DEV_SCR_IO);
			gpio_dev_unmap(GPIO_42);
			gpio_dev_map(GPIO_42, GPIO_DEV_SCR_RSTN);
			gpio_dev_unmap(GPIO_43);
			gpio_dev_map(GPIO_43, GPIO_DEV_SCR_VCC);
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_scr_gpio_config(gpio_scr_map_group_t group, scr_gpio_type gpio_type)
{
	switch (group) {
		case GPIO_SCR_MAP_GROUP0:
			if (gpio_type == BK_SCR_GPIO_PAUSE) {
				gpio_dev_unprotect_unmap(GPIO_0);
				bk_gpio_pull_up(GPIO_0);
				gpio_dev_unprotect_unmap(GPIO_1);
				if (scr_hal_get_ctrl(BK_SCR_CTRL_CLK_STOP_VAL) == 0) {
					bk_gpio_pull_down(GPIO_1);
				} else {
					bk_gpio_pull_up(GPIO_1);
				}
				gpio_dev_unprotect_unmap(GPIO_2);
				bk_gpio_pull_up(GPIO_2);
				gpio_dev_unprotect_unmap(GPIO_3);
				bk_gpio_pull_up(GPIO_3);
			} else {
				gpio_scr_sel(GPIO_SCR_MAP_GROUP0);
			}
			break;

		case GPIO_SCR_MAP_GROUP1:
			if (gpio_type == BK_SCR_GPIO_PAUSE) {
				gpio_dev_unprotect_unmap(GPIO_30);
				if (scr_hal_get_ctrl(BK_SCR_CTRL_CLK_STOP_VAL) == 0) {
					bk_gpio_pull_down(GPIO_30);
				} else {
					bk_gpio_pull_up(GPIO_30);
				}
				gpio_dev_unprotect_unmap(GPIO_31);
				bk_gpio_pull_up(GPIO_31);
				gpio_dev_unprotect_unmap(GPIO_32);
				bk_gpio_pull_up(GPIO_32);
				gpio_dev_unprotect_unmap(GPIO_43);
				bk_gpio_pull_up(GPIO_43);
			} else {
				gpio_scr_sel(GPIO_SCR_MAP_GROUP1);
			}
			break;

		case GPIO_SCR_MAP_GROUP2:
			if (gpio_type == BK_SCR_GPIO_PAUSE) {
				gpio_dev_unprotect_unmap(GPIO_40);
				if (scr_hal_get_ctrl(BK_SCR_CTRL_CLK_STOP_VAL) == 0) {
					bk_gpio_pull_down(GPIO_40);
				} else {
					bk_gpio_pull_up(GPIO_40);
				}
				gpio_dev_unprotect_unmap(GPIO_41);
				bk_gpio_pull_up(GPIO_41);
				gpio_dev_unprotect_unmap(GPIO_42);
				bk_gpio_pull_up(GPIO_42);
				gpio_dev_unprotect_unmap(GPIO_43);
				bk_gpio_pull_up(GPIO_43);
			} else {
				gpio_scr_sel(GPIO_SCR_MAP_GROUP2);
			}
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_scr_init(gpio_scr_map_group_t group)
{
	if (group >= GPIO_SCR_MAP_GROUP_MAX) {
		return BK_FAIL;
	}

	bk_int_isr_register(INT_SRC_7816, scr_isr, NULL);

	sys_hal_set_7816_int_en(1); //enable 7816 interrupt
	sys_hal_set_scr_clk(1); //enable 7816 clock
	bk_scr_gpio_init(group); //init 7816 gpio

	scr_hal_soft_reset();

	return BK_OK;
}

bk_err_t bk_scr_soft_reset(void)
{
	scr_hal_soft_reset();

	return BK_OK;
}

bk_err_t bk_scr_bypass_ckg(void)
{
	scr_hal_bypass_ckg();

	return BK_OK;
}

bk_err_t bk_scr_set_control(scr_ctrl_idx_t module, uint32_t value)
{
	scr_hal_set_ctrl(module, value);

	return BK_OK;
}

uint32_t bk_scr_get_control(scr_ctrl_idx_t module)
{
	return scr_hal_get_ctrl(module);
}

bk_err_t bk_scr_set_intr(scr_intr_idx_t module, uint32_t value, scr_isr_t isr, void *param)
{
	scr_hal_set_intr(module, value);
	if (module < BK_SCR_INTR_ALL) {
		s_scr_isr[module].callback = isr;
		s_scr_isr[module].param = param;
	}

	return BK_OK;
}

uint32_t bk_scr_get_intr(scr_intr_idx_t module)
{
	return scr_hal_get_intr(module);
}

bk_err_t bk_scr_reset_intr_status(void)
{
	scr_intr_status = 0;

	return BK_OK;
}

uint32_t bk_scr_get_intr_status(void)
{
	return scr_intr_status;
}

uint32_t bk_scr_check_intr_status(scr_intr_idx_t scr_idx, uint32_t intr_status)
{
	return scr_hal_check_intr_status(scr_idx, intr_status);
}

uint32_t bk_scr_is_fifo_full(scr_type_t type)
{
	uint32_t ret;

	switch (type) {
		case BK_SCR_TYPE_RX:
			ret = scr_hal_get_rx_fifo_full();
			break;

		case BK_SCR_TYPE_TX:
			ret = scr_hal_get_tx_fifo_full();
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}

uint32_t bk_scr_is_fifo_empty(scr_type_t type)
{
	uint32_t ret;

	switch (type) {
		case BK_SCR_TYPE_RX:
			ret = scr_hal_get_rx_fifo_empty();
			break;

		case BK_SCR_TYPE_TX:
			ret = scr_hal_get_tx_fifo_empty();
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}

bk_err_t bk_scr_flush_fifo(scr_type_t type)
{
	switch (type) {
		case BK_SCR_TYPE_RX:
			scr_hal_set_rx_fifo_flush();
			break;

		case BK_SCR_TYPE_TX:
			scr_hal_set_tx_fifo_flush();
			break;

		default:
			return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_scr_set_fifo_threshold(scr_type_t type, uint32_t value)
{
	switch (type) {
		case BK_SCR_TYPE_RX:
			scr_hal_set_rx_fifo_threshold(value);
			break;

		case BK_SCR_TYPE_TX:
			scr_hal_set_tx_fifo_threshold(value);
			break;

		default:
			return BK_FAIL;
	}

	return BK_OK;
}

uint32_t bk_scr_get_fifo_threshold(scr_type_t type)
{
	uint32_t ret = 0;

	switch (type) {
		case BK_SCR_TYPE_RX:
			ret = scr_hal_get_rx_fifo_threshold();
			break;

		case BK_SCR_TYPE_TX:
			ret = scr_hal_get_tx_fifo_threshold();
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}

uint32_t bk_scr_get_fifo_counter(scr_type_t type)
{
	uint32_t ret = 0;

	switch (type) {
		case BK_SCR_TYPE_RX:
			ret = scr_hal_get_rx_fifo_counter();
			break;

		case BK_SCR_TYPE_TX:
			ret = scr_hal_get_tx_fifo_counter();
			break;

		default:
			return BK_FAIL;
	}

	return ret;
}

bk_err_t bk_scr_set_repeat(scr_type_t type, uint32_t value)
{
	switch (type) {
		case BK_SCR_TYPE_RX:
			scr_hal_set_rx_repeat(value);
			break;

		case BK_SCR_TYPE_TX:
			scr_hal_set_tx_repeat(value);
			break;

		default:
			return BK_FAIL;
	}

	return BK_OK;
}

uint32_t bk_scr_get_repeat(scr_type_t type)
{
	switch (type) {
		case BK_SCR_TYPE_RX:
			scr_hal_get_rx_repeat();
			break;

		case BK_SCR_TYPE_TX:
			scr_hal_get_tx_repeat();
			break;

		default:
			return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_scr_set_sc_clk_div(uint32_t value)
{
	scr_hal_set_smart_card_clock_divisor(value); //26M/2/scr_clk_div; B class clock is 1~4M clock.
	return BK_OK;
}

uint32_t bk_scr_get_sc_clk_div(void)
{
	return scr_hal_get_smart_card_clock_divisor();
}

bk_err_t bk_scr_set_baud_clk_div(uint32_t value)
{
	scr_hal_set_baud_clock_divisor(value); //baud = clk/(F/D)
	return BK_OK;
}

uint32_t bk_scr_get_baud_clk_div(void)
{
	return scr_hal_get_baud_clock_divisor();
}

bk_err_t bk_scr_set_sc_guardtime(uint32_t value)
{
	scr_hal_set_smart_card_guardtime(value);
	return BK_OK;
}

bk_err_t bk_scr_set_act_deact_time(uint32_t value)
{
	scr_hal_set_active_deactive_time(value);
	return BK_OK;
}

uint32_t bk_scr_get_act_deact_time(void)
{
	return scr_hal_get_active_deactive_time();
}

bk_err_t bk_scr_set_reset_duration(uint32_t value)
{
	scr_hal_set_reset_duration(value);
	return BK_OK;
}

uint32_t bk_scr_get_reset_duration(void)
{
	return scr_hal_get_reset_duration();
}

bk_err_t bk_scr_set_ATR_start_limit(uint32_t value)
{
	scr_hal_set_ATR_start_limit(value);
	return BK_OK;
}

uint32_t bk_scr_get_ATR_start_limit(void)
{
	return scr_hal_get_ATR_start_limit();
}

bk_err_t bk_scr_set_c2c_delay_limit(uint32_t value)
{
	scr_hal_set_two_char_delay_limit(value);
	return BK_OK;
}

uint32_t bk_scr_get_c2c_delay_limit(void)
{
	return scr_hal_get_two_char_delay_limit();
}

bk_err_t bk_scr_set_baud_tune(uint32_t value)
{
	scr_hal_set_baud_tune(value);
	return BK_OK;
}

uint32_t bk_scr_get_baud_tune(void)
{
	return scr_hal_get_baud_tune();
}

uint32_t bk_scr_read_data(uint8_t *buf, uint32_t len)
{
	int i;
	uint32_t data_len, read_len;

	if (buf == NULL || len == 0) {
		return 0;
	}

	data_len = scr_hal_get_rx_fifo_counter();
	read_len = len < data_len ? len : data_len;

	for (i = 0; i < read_len; i++) {
		buf[i] = (scr_hal_get_fifo_data() & 0xFF);
	}

	return read_len;
}

bk_err_t bk_scr_write_data(uint8_t *buf, uint32_t len)
{
	int i, j;
	uint32_t num = 0;

	if (buf == NULL || len == 0) {
		return BK_FAIL;
	}

	num = (len / SCR_FIFO_SIZE);
	for (i = 0; i < num; i++) {
		while (!scr_hal_get_tx_fifo_empty());
		for ( j= 0; j < SCR_FIFO_SIZE; j++) {
			scr_hal_set_fifo_data(buf[j]);
		}
		buf += SCR_FIFO_SIZE;
	}

	num = (len % SCR_FIFO_SIZE);
	while (!scr_hal_get_tx_fifo_empty());
	for (i = 0; i < num; i++) {
		scr_hal_set_fifo_data(buf[i]);
	}

	return BK_OK;
}

bk_err_t bk_scr_register_isr(scr_intr_idx_t scr_idx, scr_isr_t isr, void *param)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_scr_isr[scr_idx].callback = isr;
	s_scr_isr[scr_idx].param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

bk_err_t bk_scr_unregister_isr(scr_intr_idx_t scr_idx)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_scr_isr[scr_idx].callback = 0;
	s_scr_isr[scr_idx].param = 0;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

void scr_isr(void)
{
	scr_intr_status = scr_hal_get_intr_status(BK_SCR_INTR_ALL);

	if (scr_hal_get_intr_status(BK_SCR_INTR_TX_FIFO_DONE)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TX_FIFO_DONE);
		if (s_scr_isr[BK_SCR_INTR_TX_FIFO_DONE].callback) {
			s_scr_isr[BK_SCR_INTR_TX_FIFO_DONE].callback(s_scr_isr[BK_SCR_INTR_TX_FIFO_DONE].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_TX_FIFO_EMPTY)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TX_FIFO_EMPTY);
		if (s_scr_isr[BK_SCR_INTR_TX_FIFO_EMPTY].callback) {
			s_scr_isr[BK_SCR_INTR_TX_FIFO_EMPTY].callback(s_scr_isr[BK_SCR_INTR_TX_FIFO_EMPTY].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_RX_FIFO_FULL)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_RX_FIFO_FULL);
		if (s_scr_isr[BK_SCR_INTR_RX_FIFO_FULL].callback) {
			s_scr_isr[BK_SCR_INTR_RX_FIFO_FULL].callback(s_scr_isr[BK_SCR_INTR_RX_FIFO_FULL].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_CLK_STOP_RUN)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_CLK_STOP_RUN);
		if (s_scr_isr[BK_SCR_INTR_CLK_STOP_RUN].callback) {
			s_scr_isr[BK_SCR_INTR_CLK_STOP_RUN].callback(s_scr_isr[BK_SCR_INTR_CLK_STOP_RUN].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_TX_DONE)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TX_DONE);
		if (s_scr_isr[BK_SCR_INTR_TX_DONE].callback) {
			s_scr_isr[BK_SCR_INTR_TX_DONE].callback(s_scr_isr[BK_SCR_INTR_TX_DONE].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_RX_DONE)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_RX_DONE);
		if (s_scr_isr[BK_SCR_INTR_RX_DONE].callback) {
			s_scr_isr[BK_SCR_INTR_RX_DONE].callback(s_scr_isr[BK_SCR_INTR_RX_DONE].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_TX_PERR)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TX_PERR);
		if (s_scr_isr[BK_SCR_INTR_TX_PERR].callback) {
			s_scr_isr[BK_SCR_INTR_TX_PERR].callback(s_scr_isr[BK_SCR_INTR_TX_PERR].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_RX_PERR)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_RX_PERR);
		if (s_scr_isr[BK_SCR_INTR_RX_PERR].callback) {
			s_scr_isr[BK_SCR_INTR_RX_PERR].callback(s_scr_isr[BK_SCR_INTR_RX_PERR].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_C2C_FULL)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_C2C_FULL);
		if (s_scr_isr[BK_SCR_INTR_C2C_FULL].callback) {
			s_scr_isr[BK_SCR_INTR_C2C_FULL].callback(s_scr_isr[BK_SCR_INTR_C2C_FULL].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_RX_THD)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_RX_THD);
		if (s_scr_isr[BK_SCR_INTR_RX_THD].callback) {
			s_scr_isr[BK_SCR_INTR_RX_THD].callback(s_scr_isr[BK_SCR_INTR_RX_THD].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_ATR_FAIL)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_ATR_FAIL);
		if (s_scr_isr[BK_SCR_INTR_ATR_FAIL].callback) {
			s_scr_isr[BK_SCR_INTR_ATR_FAIL].callback(s_scr_isr[BK_SCR_INTR_ATR_FAIL].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_ATR_DONE)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_ATR_DONE);
		if (s_scr_isr[BK_SCR_INTR_ATR_DONE].callback) {
			s_scr_isr[BK_SCR_INTR_ATR_DONE].callback(s_scr_isr[BK_SCR_INTR_ATR_DONE].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_SC_ACT)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_SC_ACT);
		if (s_scr_isr[BK_SCR_INTR_SC_ACT].callback) {
			s_scr_isr[BK_SCR_INTR_SC_ACT].callback(s_scr_isr[BK_SCR_INTR_SC_ACT].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_SC_DEACT)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_SC_DEACT);
		if (s_scr_isr[BK_SCR_INTR_SC_DEACT].callback) {
			s_scr_isr[BK_SCR_INTR_SC_DEACT].callback(s_scr_isr[BK_SCR_INTR_SC_DEACT].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_TX_THD)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TX_THD);
		if (s_scr_isr[BK_SCR_INTR_TX_THD].callback) {
			s_scr_isr[BK_SCR_INTR_TX_THD].callback(s_scr_isr[BK_SCR_INTR_TX_THD].param);
		}
	}

	if (scr_hal_get_intr_status(BK_SCR_INTR_TCK_ERR)) {
		scr_hal_clr_intr_status(BK_SCR_INTR_TCK_ERR);
		if (s_scr_isr[BK_SCR_INTR_TCK_ERR].callback) {
			s_scr_isr[BK_SCR_INTR_TCK_ERR].callback(s_scr_isr[BK_SCR_INTR_TCK_ERR].param);
		}
	}
}


