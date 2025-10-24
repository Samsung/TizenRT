/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2017-2022 Arm Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mcuboot_config/mcuboot_config.h"
#include <assert.h>
#include "target.h"
#include "tfm_hal_device_header.h"
#include "mbedtls/memory_buffer_alloc.h"
#include "bootutil/security_cnt.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/image.h"
#include "bootutil/bootutil.h"
#include "bootutil/boot_record.h"
#include "bootutil/fault_injection_hardening.h"
#include "flash_map_backend/flash_map_backend.h"
#include "boot_hal.h"
#include "uart_stdout.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"
#include "sdkconfig.h"
#include "partitions_gen.h"

#define CONFIG_ENABLE_BL2_SECURE_DEBUG (1)

/* user button definition*/
#define SDBG_GPIO_PIN                  (22)
#define SDBG_ENTER_TO_THRESHOLD_MS     (1000)

#if CONFIG_ENABLE_BL2_SECURE_DEBUG
#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SHPR3_REG                    ( *( ( volatile uint32_t * ) 0xe000ed20 ) )
#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT       ( 1UL << 16UL )
#define portMIN_INTERRUPT_PRIORITY            ( 255UL )
#define portNVIC_PENDSV_PRI                   ( portMIN_INTERRUPT_PRIORITY << 16UL )
#define portNVIC_SYSTICK_PRI                  ( portMIN_INTERRUPT_PRIORITY << 24UL )
#define configCPU_BL2_CLOCK_HZ                ( 26000000 ) 
#define configSYSTICK_CLOCK_HZ			      ( configCPU_BL2_CLOCK_HZ )
#define portNVIC_SYSTICK_CLK_BIT		      ( 1UL << 2UL )
#define configTICK_BL2_RATE_HZ                ( 500 )
#define SOC_AON_GPIO_ADDR_BASE                (0x44000400 + 0)
#define SOC_TE200_ADDR_BASE                   (0x4b110000 + 0)

#define SEC_DBG_CONTEX_DECLARE()              uint32_t gpio_sta, tick_ctrl_sta;
#define SEC_DBG_CONTEX_SAVE()                 do{\
													gpio_sta = *((volatile uint32_t *)SOC_AON_GPIO_ADDR_BASE + SDBG_GPIO_PIN * 4);\
													tick_ctrl_sta = portNVIC_SYSTICK_CTRL_REG;\
												}while(0)
#define SEC_DBG_CONTEX_RESTORE()                 do{\
													*((volatile uint32_t *)SOC_AON_GPIO_ADDR_BASE + SDBG_GPIO_PIN * 4) = gpio_sta;\
													portNVIC_SYSTICK_CTRL_REG = tick_ctrl_sta;\
												}while(0)

int secure_debug_agent_entry(void);
int dubhe_driver_init( unsigned long);

static uint32_t s_bl2_tick_cnt = 0;

void systick_init(void)
{
    portNVIC_SHPR3_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SHPR3_REG |= portNVIC_SYSTICK_PRI;

    /* Stop and reset the SysTick. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;
    portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_BL2_RATE_HZ ) - 1UL;
    portNVIC_SYSTICK_CTRL_REG = portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT;

	/*__enable_irq();*/
	__asm volatile ("cpsie i" : : : "memory");
}

void systick_uninit(void)
{
	portNVIC_SYSTICK_CTRL_REG = 0;
	portNVIC_SYSTICK_LOAD_REG = 0;
}

void SysTick_Handler(void)
{
	s_bl2_tick_cnt ++;
}

uint32_t systick_get_tick_cnt(void)
{
	return s_bl2_tick_cnt;
}

uint32_t is_attempt_to_secure_debug(void)
{
	uint32_t val;
	uint32_t sdbg_gpio_reg_addr;

	sdbg_gpio_reg_addr = SOC_AON_GPIO_ADDR_BASE + SDBG_GPIO_PIN * 4;

	/* configure input mode*/
	*((volatile uint32_t *)sdbg_gpio_reg_addr) = (1 << 2);

	/* get the status of pin*/
	val = *((volatile uint32_t *)sdbg_gpio_reg_addr);

	return val & 0x01;
}

uint32_t is_permit_secure_debug(void)
{
	uint32_t ret = 1;
	uint32_t to_threshold_tick;
	uint32_t tick_t0, tick_t1;
	uint32_t press_cnt = 0, release_cnt = 0;
	SEC_DBG_CONTEX_DECLARE();

	SEC_DBG_CONTEX_SAVE();

	tick_t0 = systick_get_tick_cnt();
	to_threshold_tick = SDBG_ENTER_TO_THRESHOLD_MS * configTICK_BL2_RATE_HZ / 1000;
	while(1){
		tick_t1 = systick_get_tick_cnt();
          	/* timeout*/
		if(tick_t1 >= tick_t0 + to_threshold_tick){
			break;
		}

		/* no attempt to enter decure debugging*/
          	if((0 == press_cnt) && ((press_cnt + release_cnt) > 5)){
                	break;
          	}

          	/* the action of pressing or releasing*/
		if(is_attempt_to_secure_debug()){
			press_cnt ++;
		}else{
			release_cnt ++;
		}
	}

	SEC_DBG_CONTEX_RESTORE();

	/* button debouncing:the pressing duration is more than 90% of the total duration*/
	if(!((press_cnt) && (press_cnt > 10 * release_cnt))){
		ret = 0;
	}

	return ret;
}

void bl2_secure_debug(void)
{
	systick_init();

	if(is_permit_secure_debug()){
		dubhe_driver_init(SOC_TE200_ADDR_BASE);
		secure_debug_agent_entry();
	}
	systick_uninit();
}
#else
void bl2_secure_debug(void)
{
	/* dummy routine*/
}

void SysTick_Handler(void)
{
	/* dummy routine*/
}
#endif
// eof

