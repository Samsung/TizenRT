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

#include "sys_hal.h"
#include "sys_driver.h"
#include "sys_driver_common.h"

uint32_t sys_drv_usb_analog_phy_en(bool ctrl, void *arg)
{
	sys_hal_usb_analog_phy_en(ctrl);
	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_speed_en(bool ctrl, void *arg)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;
	
#if defined(CONFIG_OLD_USB_VERSION)
	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_usb_analog_speed_en(ctrl);
	sys_drv_exit_critical(int_level);
#endif
	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_ckmcu_en(bool ctrl, void *arg)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

#if defined(CONFIG_OLD_USB_VERSION)
	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_usb_analog_ckmcu_en(ctrl);
	sys_drv_exit_critical(int_level);
#endif
	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_dp_capability(uint8_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

#if defined(CONFIG_OLD_USB_VERSION)
	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_usb_analog_dp_capability(value);
	sys_drv_exit_critical(int_level);
#endif

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_dn_capability(uint8_t value)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

#if defined(CONFIG_OLD_USB_VERSION)
	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_set_usb_analog_dn_capability(value);
	sys_drv_exit_critical(int_level);
#endif

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_usb_analog_deepsleep_en(bool ctrl)
{
	uint32_t ret = SYS_DRV_FAILURE;
	ret = sys_amp_res_acquire();
	if(ret != BK_OK)
		return ret;

#if defined(CONFIG_OLD_USB_VERSION)
	uint32_t int_level;
	int_level = sys_drv_enter_critical();
	sys_hal_usb_analog_deepsleep_en(ctrl);
	sys_drv_exit_critical(int_level);
#endif

	if(!ret)
		ret = sys_amp_res_release();
	if(ret != BK_OK)
		return ret;

	return SYS_DRV_SUCCESS;
}

void sys_drv_usb_charge_start()
{
/*
	bk7271: usb charge:
	SCTRL_BASE:0x00800000
	Reg26 ana_reg4 bit[21] aud_pll_vsel
	Reg26 ana_reg4 bit[19]	hw :0  sw:1
	Reg25 ana_reg3 bit[27:28]  11
	Reg26 ana_reg4 bit[13:14] 11  dpll
	Reg25 ana_reg3 bit[18:16]  111

	please check bk7271 VS bk7256
*/
}

void sys_drv_usb_charge_stop(bool ctrl)
{
	uint32_t int_level = sys_drv_enter_critical();

	sys_hal_usb_enable_charge(ctrl);

	sys_drv_exit_critical(int_level);
}

void sys_drv_usb_charge_ctrl(bool ctrl, void *arg)
{
	uint32_t int_level = sys_drv_enter_critical();

	if(ctrl)
		sys_drv_usb_charge_start();
	else
		sys_drv_usb_charge_stop(ctrl);

	sys_drv_exit_critical(int_level);
}

/*
 * bk7271: usb charge calibration:
 * SCTRL_BASE:0x00800000
 * nternal hardware calibration
 * lcf calibration
 * Added 5V voltage on Vusb
 * Added 4.2V voltage on vbatterySet pwd=0
 *
 * Icp calibration
 * Added parallel 60ohm resistor and 100nF capacitor from vbattery to ground.(Removed the external 4.2V)
 * Set pwd=0
 * Porb=0

 * vcv calibration
 * Added 5V voltage on Vusb
 * Added 4.2V voltage on vbattery
 * Set pwd=0
 * vPorb=0
*/
void sys_drv_usb_charge_cal(sys_drv_charge_step_t step, void *arg)
{
	uint32_t int_level = sys_drv_enter_critical();

	switch(step) {
	case CHARGE_STEP1: {
		sys_hal_usb_charge_vlcf_cal();
		break;
	}
	case CHARGE_STEP2: {
		sys_hal_usb_charge_icp_cal();
		break;
	}
	case CHARGE_STEP3: {
		sys_hal_usb_charge_vcv_cal();
		break;
	}
	case CHARGE_STEP4: {
		sys_hal_usb_charge_get_cal();
		break;
	}
	default:
		break;
	}

	sys_drv_exit_critical(int_level);
}

