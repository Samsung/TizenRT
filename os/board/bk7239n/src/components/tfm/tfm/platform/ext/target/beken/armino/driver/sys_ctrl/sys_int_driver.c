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

//NOTICE:This function is disable the INTERRUPT SOURCE, not the INTERRUPT TARGET.
int32 sys_drv_int_disable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_int_disable(param);

	return ret;
}

//NOTICE:This function is enable the INTERRUPT SOURCE, not the INTERRUPT TARGET.
int32 sys_drv_int_enable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_int_enable(param);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_drv_int_group2_disable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_int_group2_disable(param);

	return ret;
}

//NOTICE:Temp add for BK7256 product which has more then 32 Interrupt sources
int32 sys_drv_int_group2_enable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_int_group2_enable(param);

	return ret;
}

#if 0    //optimize code size
int32 sys_drv_fiq_disable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_fiq_disable(param);

	return ret;
}

int32 sys_drv_fiq_enable(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_fiq_enable(param);

	return ret;
}
#endif

//	uint32_t int_level = rtos_disable_int();
int32 sys_drv_global_int_disable(uint32 param)
{
	int32 ret = 0;

	return ret;
}

// rtos_enable_int(int_level);
int32 sys_drv_global_int_enable(uint32 param)
{
	return sys_hal_global_int_enable(param);
}

//NOTICE:INT source status, not INT target status(IRQ,FIQ,NVIC,PLIC,INTC status)
uint32 sys_drv_get_int_source_status(void)
{
	return sys_hal_get_int_status();
}

uint32_t sys_drv_get_cpu0_gpio_int_st(void)
{
    return sys_hal_get_cpu0_gpio_int_st();
}

//NOTICE:INT source status is read only and can't be set, we'll delete them.
int32 sys_drv_set_int_source_status(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_set_int_status(param);

	return ret;
}

#if 0    //optimize code size
uint32 sys_drv_get_fiq_reg_status(void)
{
	int32 ret = 0;

	ret = sys_hal_get_fiq_reg_status();

	return ret;
}

uint32 sys_drv_set_fiq_reg_status(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_set_fiq_reg_status(param);

	return ret;
}
#endif 

uint32 sys_drv_get_intr_raw_status(void)
{
	int32 ret = 0;

	ret = sys_hal_get_intr_raw_status();

	return ret;
}

uint32 sys_drv_set_intr_raw_status(uint32 param)
{
	int32 ret = 0;

	ret = sys_hal_set_intr_raw_status(param);

	return ret;
}

void sys_drv_sadc_int_enable(void)
{

    sys_hal_sadc_int_enable();

}

void sys_drv_sadc_int_disable(void)
{

    sys_hal_sadc_int_disable();

}

uint32_t sys_drv_cpu_fft_int_en(uint32_t value)
{
	sys_hal_cpu_fft_int_en(value);

	return SYS_DRV_SUCCESS;
}

#if CONFIG_SDIO_V2P0
void sys_driver_set_cpu0_sdio_int_en(uint32_t value)
{
	sys_hal_set_cpu0_sdio_int_en(value);
}

void sys_driver_set_cpu1_sdio_int_en(uint32_t value)
{
	sys_hal_set_cpu1_sdio_int_en(value);
}
#endif

uint32_t sys_drv_i2s_int_en(uint32_t value)
{
	sys_hal_i2s_int_en(value);

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s1_int_en(uint32_t value)
{
	sys_hal_i2s1_int_en(value);

	return SYS_DRV_SUCCESS;
}

uint32_t sys_drv_i2s2_int_en(uint32_t value)
{
	sys_hal_i2s2_int_en(value);

	return SYS_DRV_SUCCESS;
}


