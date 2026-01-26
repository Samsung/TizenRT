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

// Notes:
// This file only contain OS-independent components initialization code,
// For OS-dependently initialization, put them to OSK or bk_system.

#include <common/bk_include.h>
#include "bk_sys_ctrl.h"
#include "bk_drv_model.h"
#include <components/ate.h>
#include <driver/wdt.h>
#include "bk_wdt.h"
#include "sys_driver.h"
#include "bk_pm_model.h"
#include "bk_private/bk_init.h"

#if defined(CONFIG_FULLY_HOSTED)
#include "mmgmt.h"
#endif

#include "reset_reason.h"

#if defined(CONFIG_EASY_FLASH)
#include "easyflash.h"
#include "bk_ef.h"
#endif

#include <components/log.h>
#include <components/sensor.h>
#include <driver/trng.h>

#include "bk_arch.h"
#include "bk_private/bk_driver.h"


#if (defined(CONFIG_PSRAM))
#include <driver/psram.h>
#endif
#if (defined(CONFIG_OTP))
#include <driver/otp.h>
#endif

#if defined(CONFIG_TFM_S_TO_NS_DUMP_ENABLE)
#include "tfm_aes_gcm_nsc.h"
#endif
#include "os/mem.h"

#include "cmsis_gcc.h"

#define TAG "init"


int bandgap_init(void)
{
#if ((defined(CONFIG_OTP)) && (defined(CONFIG_SYS_CPU0)))
	uint8_t new_bandgap;
	uint8_t old_bandgap;
	bk_err_t result;

	old_bandgap = (uint8_t)sys_drv_get_bgcalm();

	result = bk_otp_ahb_read(OTP_VDDDIG_BANDGAP, &new_bandgap, sizeof(new_bandgap));
	if ((result != BK_OK) || (new_bandgap == 0) || (new_bandgap > 0x3F)) {
		return BK_OK;
	}

	BK_LOGI(TAG, "bandgap_calm_in_otp=0x%x\r\n", new_bandgap);
	if (old_bandgap != new_bandgap) {
		sys_drv_set_bgcalm(new_bandgap);
	}
#endif
	return BK_OK;
}

int random_init(void)
{
#if ((defined(CONFIG_TRNG_SUPPORT)) && (!defined(CONFIG_SOC_BK7236XX)))
	BK_LOGI(TAG, "trng enable\r\n");
	bk_trng_start();
#endif
	return BK_OK;
}

__IRAM_SEC int wdt_init(void)
{
#if (defined(CONFIG_FREERTOS))
#if defined(CONFIG_INT_WDT)
	BK_LOGD(TAG, "int watchdog enabled, period=%u\r\n", CONFIG_INT_WDT_PERIOD_MS);
	bk_wdt_start(CONFIG_INT_WDT_PERIOD_MS);
#else
#if defined(CONFIG_SOC_BK7236XX))
	BK_LOGI(TAG, "watchdog disabled\r\n");
	bk_wdt_start(CONFIG_INT_WDT_PERIOD_MS);
	bk_wdt_feed();
	bk_wdt_stop();
#endif
#endif //CONFIG_INT_WDT
#endif //CONFIG_FREERTOS

#if defined(CONFIG_TASK_WDT)
	bk_task_wdt_start();
	BK_LOGD(TAG, "task watchdog enabled, period=%u\r\n", CONFIG_TASK_WDT_PERIOD_MS);
#endif

	return BK_OK;
}


__attribute__((unused)) static int pm_init(void)
{

#if defined(CONFIG_DEEP_PS)
	bk_init_deep_wakeup_gpio_status();
#endif
	return BK_OK;
}

#if CONFIG_PSRAM
#if defined(CONFIG_XIP_KERNEL) && (CONFIG_XIP_KERNEL == 1)
void bk_psram_code_init(void){
    extern const char __psram_code_text;
    extern const char __psram_code_start__;
    extern const char __psram_code_end__;
    extern const char __psram_data_text;
    extern const char __psram_data_start__;
    extern const char __psram_data_end__;
    extern const char __psram_bss_start__;
    extern const char __psram_bss_end__ ;
    unsigned int size;
    unsigned int size_data;
    unsigned int size_bss;

    /*Copy psram_code section from LMA to VMA*/
    size = &__psram_code_end__ - &__psram_code_start__;

    if(size!=0)
    {
        os_memcpy((void *)&__psram_code_start__, (void *)&__psram_code_text, size);
    }

    /*Copy psram_data section from LMA to VMA*/
    size_data = &__psram_data_end__ - &__psram_data_start__;

    if(size_data!=0)
    {
        os_memcpy((void *)&__psram_data_start__, (void *)&__psram_data_text, size_data);
    }

    /*Clear psram_bss section*/
    size_bss = &__psram_bss_end__ - &__psram_bss_start__;

    if(size_bss!=0)
    {
        os_memset_word((void *)&__psram_bss_start__,0,size_bss);
    }
}
#else
__FLASH_BOOT_CODE void bk_psram_code_init(void){
    extern const char __psram_code_text;
    extern const char __psram_code_start__;
    extern const char __psram_code_end__;
    extern const char __psram_data_text;
    extern const char __psram_data_start__;
    extern const char __psram_data_end__;
    extern const char __psram_bss_start__;
    extern const char __psram_bss_end__ ;
    unsigned int size;
    unsigned int size_data;
    unsigned int size_bss;

    /*Copy psram_code section from LMA to VMA*/
    size = (uint32_t)(&__psram_code_end__ - &__psram_code_start__)/sizeof(uint32_t);

    if(size!=0)
    {
        sys_memcpy_word((void *)&__psram_code_start__, (void *)&__psram_code_text, size);
    }

    /*Copy psram_data section from LMA to VMA*/
    size_data = (uint32_t)(&__psram_data_end__ - &__psram_data_start__)/sizeof(uint32_t);

    if(size_data!=0)
    {
        sys_memcpy_word((void *)&__psram_data_start__, (void *)&__psram_data_text, size_data);
    }

    /*Clear psram_bss section*/
    size_bss = (uint32_t)(&__psram_bss_end__ - &__psram_bss_start__)/sizeof(uint32_t);
    
    if(size_bss!=0)
    {
        sys_memset_word((void *)&__psram_bss_start__,0,size_bss);
    }
}
#endif
#endif // #if CONFIG_PSRAM

__attribute__((section(".build_ver_sections"))) const char build_ver[] = __DATE__ " " __TIME__;
static inline void show_sdk_version(void)
{
#if (defined(CONFIG_CMAKE))
	//BK_LOGI(TAG, "armino rev: %s\r\n", ARMINO_VER);
	BK_LOGI(TAG, "armino rev: %s\r\n", "");
#else
	//BK_LOGI(TAG, "armino rev: %s\r\n", BEKEN_SDK_REV);
	BK_LOGI(TAG, "armino rev: %s\r\n", "");
#endif
	BK_LOGI(TAG, "build_ver: %s\r\n", build_ver);
}

static inline void show_chip_id(void)
{
	BK_LOGI(TAG, "armino soc id:%x_%x\r\n", sys_drv_get_device_id(), sys_drv_get_chip_id());
}

static inline void show_sdk_lib_version(void)
{
#if (defined(CONFIG_SYS_CPU0))
	extern char* bk_get_internal_lib_version(void);
	char* ver = bk_get_internal_lib_version();
	BK_LOGI(TAG, "armino internal lib rev: %s\n", ver);
#endif
}

static void show_armino_version(void)
{
	show_sdk_version();
	show_chip_id();
}

static void show_init_info(void)
{
	show_reset_reason();
	//use tizen build info

}

#if defined(CONFIG_DEBUG_BRINGUP) //Debug code for bring up board

#define NCV_SIM                     0x1
#define	TEST_ID_MAX					100
#if (NCV_SIM == 0)
#define UART_BAUD_RATE1           115200
#else
#define UART_BAUD_RATE1           115200
#endif

#define UART_CLK_FREQ            26
int print_str(char * st)
{
  while (*st) {
		       addUART0_Reg0x3 = *st;
	           st++;
	          }
  return 1;
}
void UartDbgInit()
{
	unsigned int     uart_clk_div;

   // clrf_SYS_Reg0x3_uart0_pwd ;   //open periph
	//*((volatile unsigned long *) (0x44010000+0xc*4)) =  0x4;
	setf_SYSTEM_Reg0xc_uart0_cken ; //uart0 enable


	//*((volatile unsigned long *) (0x44000400+10*4))  = 0x40 ; //second_func
    //*((volatile unsigned long *) (0x44000400+11*4))  = 0x40 ; //second_func
    addAON_GPIO_Reg0xa = 0x40 ;  //second_func
    addAON_GPIO_Reg0xb = 0x40 ;  //second_func


    uart_clk_div = (UART_CLK_FREQ*1000000)/UART_BAUD_RATE1 - 1;




    addUART0_Reg0x0 = (uart_clk_div << posUART0_Reg0x0_UART_CLK_DIVID) |
                      (0x0          << posUART0_Reg0x0_UART_STOP_LEN ) |
					  #if (NCV_SIM == 0)
                      (0x0          << posUART0_Reg0x0_UART_PAR_MODE ) |
                      (0x0          << posUART0_Reg0x0_UART_PAR_EN   ) |
					  #else
					            (0x0          << posUART0_Reg0x0_UART_PAR_MODE ) |
                      (0x0          << posUART0_Reg0x0_UART_PAR_EN   ) |
					  #endif
                      (0x3          << posUART0_Reg0x0_UART_LEN      ) |
                      (0x0          << posUART0_Reg0x0_UART_IRDA     ) |
                      (0x1          << posUART0_Reg0x0_UART_RX_ENABLE) |
                      (0x1          << posUART0_Reg0x0_UART_TX_ENABLE) ;

    addUART0_Reg0x1 = 0x00004010;
    addUART0_Reg0x4 = 0x42;
    addUART0_Reg0x6 = 0x0;
    addUART0_Reg0x7 = 0x0;

	//  setf_SYS_Reg0x10_int_uart0_en; //enable uart_int irq
   // *((volatile unsigned long *) (0x44010000+0x20*4)) =  0x10;  //enable uart_int
    addSYSTEM_Reg0x20 = 0x10 ;  //enable uart_int

}
#endif

__attribute__((weak)) \
void *__stack_chk_guard = NULL;

// Intialize random stack guard, must after trng start.
void bk_stack_guard_setup(void)
{
    BK_LOGI(TAG, "Intialize random stack guard.\r\n");
#if defined(CONFIG_TRNG_SUPPORT)
    __stack_chk_guard = (void *)bk_rand();
#endif
}

__attribute__((weak)) \
void __stack_chk_fail (void)
{
    BK_DUMP_OUT("Stack guard warning, local buffer overflow!!!\r\n");
    BK_ASSERT(0);
}

int components_init(void)
{
// #if defined(CONFIG_RESET_REASON)
// 	reset_reason_init();
// #endif
	app_phy_init();

	if(driver_init())
		return BK_FAIL;

#if (defined(CONFIG_TEMP_DETECT) || defined(CONFIG_VOLT_DETECT))
	bk_sensor_init();
#endif

	pm_init();

	show_init_info();

	bandgap_init();
	random_init();

#if (defined(CONFIG_SYS_CPU0))
	//wdt_init();
#endif

	bk_stack_guard_setup();

#if defined(CONFIG_TFM_S_TO_NS_DUMP_ENABLE)
	extern void bk_security_to_nosecurity_dump_register_callback(void);
	bk_security_to_nosecurity_dump_register_callback();
#endif
	return BK_OK;
}
