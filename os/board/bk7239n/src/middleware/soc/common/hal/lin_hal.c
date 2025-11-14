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

#include "hal.h"
#include "lin_hal.h"
#include <math.h>

#define DOUBLE_EPS           (1e-15)
#define LIN_DIVISOR_IS_ZERO(x) do {\
	if (fabs(x) < DOUBLE_EPS) {\
		HAL_LOGE("the divisor is 0 \r\n");\
		return BK_FAIL;\
	}\
} while(0)

void lin_hal_set_soft_rst(uint32_t v)
{
	lin_ll_set_global_ctrl_soft_rst(v);
}
void lin_hal_bypass_ckg(uint32_t v)
{
	lin_ll_set_global_ctrl_bypass_cfg(v);
}

void lin_hal_master_cfg(lin_dev_t dev)
{
	lin_ll_set_global_ctrl_master(dev);
}

uint32_t lin_hal_get_master(void)
{
	return lin_ll_get_global_ctrl_master();
}
/* reg : data 0 - 7 */
void lin_hal_set_data_byte(uint32_t index, uint8_t v)
{
	lin_ll_set_data_byte(index, v);
}

uint8_t lin_hal_get_data_byte(uint32_t index)
{
	return lin_ll_get_data_byte(index);
}

/* reg :ctrl */
void lin_hal_set_ctrl_start_req(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_MASTER) {
		lin_ll_set_ctrl_start_req(LIN_CTRL_START_REQ_MASK);
	}
}
uint32_t lin_hal_get_ctrl_start_req(void)
{
	return lin_ll_get_ctrl_start_req();
}

void lin_hal_set_ctrl_wakeup(void)
{
	lin_ll_set_ctrl_wakeup(LIN_CTRL_WAKEUP_MASK);
}

uint32_t lin_hal_get_ctrl_wakeup(void)
{
	return lin_ll_get_ctrl_wakeup();
}

void lin_hal_set_ctrl_reset_error(void)
{
	lin_ll_set_ctrl_reset_error(LIN_CTRL_RESET_ERROR_MASK);
}

void lin_hal_set_ctrl_reset_int(void)
{
	lin_ll_set_ctrl_reset_int(LIN_CTRL_RESET_INT_MASK);
}

void lin_hal_set_ctrl_data_ack(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		lin_ll_set_ctrl_data_ack(LIN_CTRL_DATA_ACK_MASK);
	}
}

uint32_t lin_hal_get_ctrl_data_ack(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_ctrl_data_ack();
	}
	return 0;
}

void lin_hal_set_ctrl_transmit(uint32_t v)
{
	lin_ll_set_ctrl_transmit(v & LIN_CTRL_DATA_ACK_MASK);
}

uint32_t lin_hal_get_ctrl_transmit(void)
{
	return lin_ll_get_ctrl_transmit();
}

void lin_hal_set_ctrl_sleep(void)
{
	lin_ll_set_ctrl_sleep(LIN_CTRL_SLEEP_MASK);
}

uint32_t lin_hal_get_ctrl_sleep(void)
{
	return lin_ll_get_ctrl_sleep();
}

void lin_hal_set_ctrl_stop(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		lin_ll_set_ctrl_stop(LIN_CTRL_STOP_MASK);
	}
}

uint32_t lin_hal_get_status_value(void)
{
	return lin_ll_get_status_value();
}

uint32_t lin_hal_get_status_complete(void)
{
	return lin_ll_get_status_complete();
}

uint32_t lin_hal_get_status_wake_up(void)
{
	return lin_ll_get_status_wake_up();
}

uint32_t lin_hal_get_status_error(void)
{
	return lin_ll_get_status_error();
}

uint32_t lin_hal_get_status_int(void)
{
	return lin_ll_get_status_int();
}

uint32_t lin_hal_get_status_data_req(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_status_data_req();
	}
	return 0;
}

uint32_t lin_hal_get_status_aborted(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_status_aborted();
	}

	return 0;
}

uint32_t lin_hal_get_status_bus_idle_timeout(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_status_bus_idle_timeout();
	}
	return 0;
}

uint32_t lin_hal_get_status_lin_active(void)
{
	return lin_ll_get_status_lin_active();
}

/* reg :error */
uint32_t lin_hal_get_error_value(void)
{
	return lin_ll_get_err_value();
}

uint32_t lin_hal_get_error_bit(void)
{
	return lin_ll_get_err_bit();
}

uint32_t lin_hal_get_error_chk(void)
{
	return lin_ll_get_err_chk();
}

uint32_t lin_hal_get_error_timeout(void)
{
	return lin_ll_get_err_timeout();
}

uint32_t lin_hal_get_error_parity(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_err_parity();
	}

	return 0;
}

void lin_hal_set_prescl(uint32_t prescl)
{
	uint32_t prescl1 = (prescl & LIN_BTCFG1_PRESCL1_MASK);
	uint32_t prescl2 = ((prescl >> 2) & LIN_TCFG_PRESCL2_MASK);

	lin_ll_set_btcfg1_prescl1(prescl1);
	lin_ll_set_tcfg_prescl2(prescl2);
}

uint32_t lin_hal_get_prescl(void)
{
	uint32_t prescl;
	uint32_t prescl1;
	uint32_t prescl2;

	prescl1 = lin_ll_get_btcfg1_prescl1();
	prescl2 = lin_ll_get_tcfg_prescl2();
	prescl = (prescl1 & LIN_BTCFG1_PRESCL1_MASK) |
			 ((prescl2 & LIN_TCFG_PRESCL2_MASK) << LIN_BTCFG1_PRESCL1_LEN);

	return prescl;
}


void lin_hal_set_div(uint32_t div)
{
	uint32_t div1 = (div & LIN_BTCFG0_BT_DIV1_MASK);
	uint32_t div2 = ((div >> LIN_BTCFG0_BT_DIV1_LEN) & LIN_BTCFG1_BT_DIV2_MASK);

	lin_ll_set_btcfg0_bt_div1(div1);
	lin_ll_set_btcfg1_bt_div2(div2);
}

uint32_t lin_hal_get_div(void)
{
	uint32_t div;
	uint32_t div1;
	uint32_t div2;

	div1 = lin_ll_get_btcfg0_bt_div1();
	div2 = lin_ll_get_btcfg1_bt_div2();
	div = (div1 & LIN_BTCFG0_BT_DIV1_MASK) |
			 ((div2 & LIN_BTCFG1_BT_DIV2_MASK) << LIN_BTCFG0_BT_DIV1_LEN);

	return div;
}

void lin_hal_set_mul(uint32_t mul)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_MASTER) {
		lin_ll_set_btcfg1_bt_mul(mul);
	}
}

uint32_t lin_hal_get_mul(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_MASTER) {
		return lin_ll_get_btcfg1_bt_mul();
	}

	return 0;
}

void lin_hal_set_ident_value(uint32_t ident)
{
	lin_ll_set_ident_value(ident);
}

uint32_t lin_hal_get_ident_value(void)
{
	return lin_ll_get_ident_value();
}

void lin_hal_set_ident_id(uint32_t id)
{
	lin_ll_set_ident_id(id);
}

uint32_t lin_hal_get_ident_id(void)
{
	return lin_ll_get_ident_id();
}

void lin_hal_set_type_data_length(uint32_t data_length)
{
	lin_ll_set_type_data_length(data_length);
}

uint32_t lin_hal_get_type_data_length(void)
{
	return lin_ll_get_type_data_length();
}

void lin_hal_set_type_enh_check(uint32_t enh_check)
{
	lin_ll_set_type_enh_check(enh_check);
}

uint32_t lin_hal_get_type_enh_check(void)
{
	return lin_ll_get_type_enh_check();
}

void lin_hal_set_tcfg_wup_repeat_time(uint32_t time)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		lin_ll_set_tcfg_wup_repeat_time(time);
	}
}

uint32_t lin_hal_get_tcfg_wup_repeat_time(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_tcfg_wup_repeat_time();
	}

	return 0;
}

void lin_hal_set_tcfg_bus_inactivity_time(uint32_t time)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		lin_ll_set_tcfg_bus_inactivity_time(time);
	}
}

uint32_t lin_hal_get_tcfg_bus_inactivity_time(void)
{
	uint32_t master = lin_ll_get_global_ctrl_master();
	if (master == LIN_SLAVE) {
		return lin_ll_get_tcfg_bus_inactivity_time();
	}

	return 0;
}

void lin_hal_set_rate(double rate)
{
	uint32_t mul;
	uint32_t prescl;
	uint32_t div;
	uint32_t master = lin_ll_get_global_ctrl_master();

	if (rate < LIN_BAUD_RATE_MIN) {
		rate = LIN_BAUD_RATE_MIN;
	} else if (rate > LIN_BAUD_RATE_MAX) {
		rate = LIN_BAUD_RATE_MAX;
	}

	if (master == LIN_MASTER) {
		mul = floor(LIN_RATE_20KBIT / rate -1);
		prescl = floor(log2(LIN_CLK_26M / ((mul + 1) * rate * 200) - 1));
		div = floor(LIN_CLK_26M / (pow(2, prescl + 1) * (mul + 1) * rate));

		lin_hal_set_mul(mul);
		lin_hal_set_prescl(prescl);
		lin_hal_set_div(div);
	} else if (master == LIN_SLAVE) {
		/* the configutation of the slave is only relate to the clock */
		prescl = floor(log2(LIN_CLK_26M /( LIN_RATE_20KBIT* 200) -1));
		div = floor(LIN_CLK_26M / (pow(2, prescl + 1) * LIN_RATE_20KBIT));

		lin_hal_set_prescl( prescl);
		lin_hal_set_div(div);
	}
}

double lin_hal_get_rate(void)
{
	double mul = lin_hal_get_mul();
	uint32_t prescl = lin_hal_get_prescl();
	double div = lin_hal_get_div();
	uint32_t master = lin_ll_get_global_ctrl_master();
	double rate = 0.0;

	LIN_DIVISOR_IS_ZERO(div);

	if (master == LIN_MASTER) {
		rate = floor(LIN_CLK_26M/(pow(2, prescl + 1) * div * (mul + 1)));
	}

	return rate;
}

bk_err_t lin_hal_init(void)
{
	lin_hal_set_soft_rst(0);
	lin_hal_set_soft_rst(1);

    return BK_OK;
}

bk_err_t lin_hal_deinit(void)
{
	lin_hal_set_soft_rst(0);
	
    return BK_OK;
}

#if (defined(CONFIG_LIN_PM_CB_SUPPORT))
void lin_hal_backup(uint32_t *pm_backup)
{
	lin_ll_backup(pm_backup);
}

void lin_hal_restore(uint32_t *pm_backup)
{
	lin_ll_restore(pm_backup);
}
#endif
