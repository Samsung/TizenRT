/*
 * Copyright (c) 2009-2019 Arm Limited.
 * Copyright (c) 2019-2020 ArmChina.
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "components/log.h"
#include "components/system.h"
#include "bk7239xx.h"
#include "aon_pmu_driver.h"
#include "sys_driver.h"
#include "driver/uart.h"
#include "wdt_driver.h"
#include "bk_pm_internal_api.h"
#include <modules/pm.h>
#include <driver/pwr_clk.h>
#include "sdkconfig.h"

#include "stack_base.h"

#include <driver/wdt.h>
#include <os/os.h>
#include "bk_aon_wdt.h"
#include <driver/uart.h>
#include <reset_reason.h>
#include "arch_interrupt.h"

#include <tinyara/mm/heap_regioninfo.h>
#include <tinyara/mpu.h>
#include <tinyara/arch.h>
#include <soc/bk7239n/soc.h>
#include "os/mem.h"

#define TAG "arch"

#define SYSTEM_BASE_ADDRESS              SOC_SYS_REG_BASE
#define OTP_APB_BASE_ADDRESS             SOC_OTP_REG_BASE
#define MEM_CHECK_BASE_ADDRESS           SOC_MEM_CHECK_REG_BASE
#define BIT_ID(num)                      (num) 
#define GET_BIT_VAL(val, bit)            ((val >> bit) & 1)
#define SET_BIT_VAL(val, bit, new_val)   ((val & (~ (1 << bit)))|(new_val << bit))

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);
extern void entry_main(void);
extern void bk_mpu_init(void);

extern unsigned char _heap_start, _heap_end;

#if CONFIG_KMM_REGIONS == 1
extern uint32_t RAM_KREGION1_START;
extern uint32_t RAM_KREGION1_SIZE;
#else
extern uint32_t RAM_KREGION0_START;
extern uint32_t RAM_KREGION0_SIZE;
#endif
extern uint32_t RAM_KREGION2_START;
extern uint32_t RAM_KREGION2_SIZE;

#define HEAP_START_ADDRESS    (void*)&_heap_start
#define HEAP_END_ADDRESS      (void*)&_heap_end   
#if CONFIG_KMM_REGIONS == 1
#define HEAP_PSRAM1_START_ADDR   (void*)&RAM_KREGION1_START   
#define HEAP_PSRAM1_SIZE         (void*)&RAM_KREGION1_SIZE    
#else
#define HEAP_PSRAM1_START_ADDR   (void*)&RAM_KREGION0_START   
#define HEAP_PSRAM1_SIZE         (void*)&RAM_KREGION0_SIZE    
#endif
#define HEAP_PSRAM2_START_ADDR   (void*)&RAM_KREGION2_START   
#define HEAP_PSRAM2_SIZE         (void*)&RAM_KREGION2_SIZE  
extern unsigned int __StackLimit;
extern uint32_t _sidle_stack;

#define IDLE_STACK      ((unsigned)&__StackLimit + CONFIG_IDLETHREAD_STACKSIZE)
const uintptr_t g_idle_topstack = IDLE_STACK;

extern uint32_t g_mpu_nregion_allocated;
/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
            void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak));
void HardFault_Handler      (void) __attribute__ ((weak));
void MemManage_Handler      (void) __attribute__ ((weak));
void BusFault_Handler       (void) __attribute__ ((weak));
void UsageFault_Handler     (void) __attribute__ ((weak));
void SecureFault_Handler    (void) __attribute__ ((weak));
void SVC_Handler            (void) __attribute__ ((weak));
void DebugMon_Handler       (void) __attribute__ ((weak));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void);// __attribute__ ((weak, alias("Default_Handler")));

/* Interrupt Handler */
void DMA0_NSEC_Handler               (void) __attribute__ ((weak));
void ENCP_SEC_Handler                (void) __attribute__ ((weak));
void ENCP_NSEC_Handler               (void) __attribute__ ((weak));
void TIMER0_Handler                  (void) __attribute__ ((weak));
void UART0_Handler                   (void) __attribute__ ((weak));
void PWM0_Handler                    (void) __attribute__ ((weak));
void I2C0_Handler                    (void) __attribute__ ((weak));
void SPI0_Handler                    (void) __attribute__ ((weak));
void SARADC_Handler                  (void) __attribute__ ((weak));
void IRDA_Handler                    (void) __attribute__ ((weak));
void SDIO_Handler                    (void) __attribute__ ((weak));
void GDMA_Handler                    (void) __attribute__ ((weak));
void LA_Handler                      (void) __attribute__ ((weak));
void TIMER1_Handler                  (void) __attribute__ ((weak));
void I2C1_Handler                    (void) __attribute__ ((weak));
void UART1_Handler                   (void) __attribute__ ((weak));
void UART2_Handler                   (void) __attribute__ ((weak));
void SPI1_Handler                    (void) __attribute__ ((weak));
void CAN_Handler                     (void) __attribute__ ((weak));
void USB_Handler                     (void) __attribute__ ((weak));
void QSPI0_Handler                   (void) __attribute__ ((weak));
void CKMN_Handler                    (void) __attribute__ ((weak));
void SBC_Handler                     (void) __attribute__ ((weak));
void AUDIO_Handler                   (void) __attribute__ ((weak));
void I2S0_Handler                    (void) __attribute__ ((weak));
void JPEG_ENC_Handler                (void) __attribute__ ((weak));
void JPEG_DEC_Handler                (void) __attribute__ ((weak));
void DISPLAY_Handler                 (void) __attribute__ ((weak));
void DMA2D_Handler                   (void) __attribute__ ((weak));
void PHY_MBP_Handler                 (void) __attribute__ ((weak));
void PHY_RIU_Handler                 (void) __attribute__ ((weak));
void MAC_INT_TX_RX_TIMER_Handler     (void) __attribute__ ((weak));
void MAC_INT_TX_RX_MISC_Handler      (void) __attribute__ ((weak));
void MAC_INT_RX_TRIGGER_Handler      (void) __attribute__ ((weak));
void MAC_INT_TX_TRIGGER_Handler      (void) __attribute__ ((weak));
void MAC_INT_PORT_TRIGGER_Handler    (void) __attribute__ ((weak));
void MAC_INT_GEN_Handler             (void) __attribute__ ((weak));
void GPIO_NS_Handler                 (void) __attribute__ ((weak));
void INT_MAC_WAKEUP_Handler          (void) __attribute__ ((weak));
void DM_Handler                      (void) __attribute__ ((weak));
void BLE_Handler                     (void) __attribute__ ((weak));
void BT_Handler                      (void) __attribute__ ((weak));
void QSPI1_Handler                   (void) __attribute__ ((weak));
void PWM1_Handler                    (void) __attribute__ ((weak));
void I2S1_Handler                    (void) __attribute__ ((weak));
void I2S2_Handler                    (void) __attribute__ ((weak));
void H264_Handler                    (void) __attribute__ ((weak));
void SDMADC_Handler                  (void) __attribute__ ((weak));
void ETHERNET_Handler                (void) __attribute__ ((weak));
void SCAL0_Handler                   (void) __attribute__ ((weak));
void OTP_Handler                     (void) __attribute__ ((weak));
void DPLL_UNLOCK_Handler             (void) __attribute__ ((weak));
void TOUCH_Handler                   (void) __attribute__ ((weak));
void USB_PLUG_Handler                (void) __attribute__ ((weak));
void RTC_Handler                     (void) __attribute__ ((weak));
void GPIO_Handler                    (void) __attribute__ ((weak));
void DMA1_SEC_Handler                (void) __attribute__ ((weak));
void DMA1_NSEC_Handler               (void) __attribute__ ((weak));
void YUV_BUF_Handler                 (void) __attribute__ ((weak));
void ROTT_Handler                    (void) __attribute__ ((weak));
void SCR7816_Handler                  (void) __attribute__ ((weak));
void LIN_Handler                     (void) __attribute__ ((weak));
void SCAL1_Handler                   (void) __attribute__ ((weak));
void MAILBOX_Handler                 (void) __attribute__ ((weak));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern void exception_common(void);


const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  exception_common,                              /* -14 NMI Handler */
  exception_common,                        /* -13 Hard Fault Handler */
  exception_common,                        /* -12 MPU Fault Handler */
  exception_common,                         /* -11 Bus Fault Handler */
  exception_common,                       /* -10 Usage Fault Handler */
  exception_common,                      /*  -9 Secure Fault Handler */
  exception_common,                                        /*     Reserved */
  exception_common,                                        /*     Reserved */
  exception_common,                                        /*     Reserved */
  exception_common,                              /*  -5 SVCall Handler */
  exception_common,                         /*  -4 Debug Monitor Handler */
  exception_common,                                        /*     Reserved */
  exception_common,                           /*  -2 PendSV Handler */
  exception_common,                          /*  -1 SysTick Handler */

  /* Interrupts */
  DMA0_NSEC_Handler,
  ENCP_SEC_Handler,
  ENCP_NSEC_Handler,
  TIMER0_Handler,
  UART0_Handler,
  PWM0_Handler,
  I2C0_Handler,
  SPI0_Handler,
  SARADC_Handler,
  IRDA_Handler,
  SDIO_Handler,
  GDMA_Handler,
  LA_Handler,
  TIMER1_Handler,
  I2C1_Handler,
  UART1_Handler,
  UART2_Handler,
  SPI1_Handler,
  CAN_Handler,
  USB_Handler,
  QSPI0_Handler,
  CKMN_Handler,
  SBC_Handler,
  AUDIO_Handler,
  I2S0_Handler,
  JPEG_ENC_Handler,
  JPEG_DEC_Handler,
  DISPLAY_Handler,
  DMA2D_Handler,
  PHY_MBP_Handler,
  PHY_RIU_Handler,
  MAC_INT_TX_RX_TIMER_Handler,
  MAC_INT_TX_RX_MISC_Handler,
  MAC_INT_RX_TRIGGER_Handler,
  MAC_INT_TX_TRIGGER_Handler,
  MAC_INT_PORT_TRIGGER_Handler,
  MAC_INT_GEN_Handler,
  GPIO_NS_Handler,
  INT_MAC_WAKEUP_Handler,
  DM_Handler,
  BLE_Handler,
  BT_Handler,
  QSPI1_Handler,
  PWM1_Handler,
  I2S1_Handler,
  I2S2_Handler,
  H264_Handler,
  SDMADC_Handler,
  /* BK7236 legacy download mode requires that the flash offset 0x100 is 'BK7236'.*/
  (void (*)(void))0x32374B42,
  (void (*)(void))0x00003633,
  OTP_Handler,
  DPLL_UNLOCK_Handler,
  TOUCH_Handler,
  USB_PLUG_Handler,
  RTC_Handler,
  GPIO_Handler,
  DMA1_SEC_Handler,
  DMA1_NSEC_Handler,
  YUV_BUF_Handler,
  ROTT_Handler,
  SCR7816_Handler,
  NULL,
  NULL,
  MAILBOX_Handler
  /* Interrupts 64 .. 480 are left out */
};


__attribute__((used, section(".vectors_iram"))) \
const VECTOR_TABLE_Type __VECTOR_TABLE_IRAM[] = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  /* Vectors 2 - n point directly at the generic handler */
 [2 ...(15 + 64)] = exception_common

  /* Interrupts 64 .. 480 are left out */
};


#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif


#define ENTRY_SECTION  __attribute__((section(".fix.reset_entry")))

__FLASH_BOOT_CODE void set_reboot_tag(uint32_t tag)
{
	REG_WRITE(REBOOT_TAG_ADDR, tag);
}

uint32_t sys_get_reboot_tag(void)
{
	return REG_READ(REBOOT_TAG_ADDR);
}

/*
 * sets mem-repair module use OTP param if it has been set by CP-TEST.
 */
static inline void boot_mem_check(void)
{
	if(GET_BIT_VAL(REG_READ(SYSTEM_BASE_ADDRESS + 0x10*4), BIT_ID(3)) != 0)
	{
		REG_WRITE((SYSTEM_BASE_ADDRESS + 0x10*4), SET_BIT_VAL(REG_READ(SYSTEM_BASE_ADDRESS + 0x10*4), BIT_ID(3), 0));
	}

	if(GET_BIT_VAL(REG_READ(SYSTEM_BASE_ADDRESS + 0xC*4), BIT_ID(15)) != 1)
	{
		REG_WRITE((SYSTEM_BASE_ADDRESS + 0xC*4), SET_BIT_VAL(REG_READ(SYSTEM_BASE_ADDRESS + 0xC*4), BIT_ID(15), 1));
	}

	if((GET_BIT_VAL(REG_READ(OTP_APB_BASE_ADDRESS + 0xB2*4), BIT_ID(0)) != 1)||(GET_BIT_VAL(REG_READ(OTP_APB_BASE_ADDRESS + 0xB2*4), BIT_ID(1)) != 1))
	{
		REG_WRITE((OTP_APB_BASE_ADDRESS + 0xB2*4), SET_BIT_VAL(REG_READ(OTP_APB_BASE_ADDRESS + 0xB2*4), BIT_ID(0), 3));
	}

	if(GET_BIT_VAL(REG_READ(OTP_APB_BASE_ADDRESS + 0xB1*4), BIT_ID(0)) == 0)
	{
		if((REG_READ(OTP_APB_BASE_ADDRESS + 0x7c8) & 0xF ) == 0x7)
		{
			REG_WRITE((MEM_CHECK_BASE_ADDRESS + 0x2*4), 0x7);
		}
	}
}



/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN ENTRY_SECTION __attribute__((naked))void Reset_Handler(void)
{
#if defined(CONFIG_DEEP_LV)
    if (dlv_is_startup()) {
        __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
        /* CMSIS System Initialization */
        SystemInit_iram();
        dlv_startup();
    }
#endif

#if defined(CONFIG_AON_PMU_POR_TIMING_SUPPORT)
    aon_pmu_driver_save_time(true);
#endif

    //boot_mem_check(); //disable it since OTP issue

    STARTUP_PERF(15);

    __set_MSPLIM((uint32_t)(&_sidle_stack));

    /* CMSIS System Initialization */
    SystemInit();

   //clear mannully reboot flag
   set_reboot_tag(0);

   sys_drv_early_init();

    /* Enter PreMain (C library entry point) */
    __PROGRAM_START();

}

#if defined(CONFIG_SWD_DEBUG_MODE)
void set_jtag_mode(void) {
	
    bk_err_t ret = BK_OK;

    close_wdt();
    (void)sys_drv_set_jtag_mode(0);

#if defined(CONFIG_BEKEN_EVB)
    /* clear the default SWD pin configuration */
    gpio_dev_unprotect_unmap(GPIO_0);
    gpio_dev_unprotect_unmap(GPIO_1);


    /* Set the SWD pin configuration */
	gpio_dev_unprotect_unmap(CONFIG_SWD_DEBUG_MODE_PIN_DIO);
	gpio_dev_unprotect_unmap(CONFIG_SWD_DEBUG_MODE_PIN_CLK);

    ret = gpio_dev_unprotect_map(CONFIG_SWD_DEBUG_MODE_PIN_DIO, GPIO_DEV_SWD_IO);
	ret = gpio_dev_unprotect_map(CONFIG_SWD_DEBUG_MODE_PIN_CLK, GPIO_DEV_SW_CLK);
#else 
    gpio_dev_unprotect_unmap(GPIO_0);
    gpio_dev_unprotect_unmap(GPIO_1);

    ret = gpio_dev_unprotect_map(GPIO_1, GPIO_DEV_SWD_IO);
    ret = gpio_dev_unprotect_map(GPIO_0, GPIO_DEV_SW_CLK);
#endif	
}
#endif


void os_heap_init(void)
{

#if CONFIG_KMM_REGIONS == 1
    kregionx_start[0] = (void *)HEAP_START_ADDRESS;
    kregionx_size[0] = (size_t)(HEAP_END_ADDRESS - HEAP_START_ADDRESS);

#elif CONFIG_KMM_REGIONS >= 2  
    kregionx_start[0] = (void *)HEAP_PSRAM1_START_ADDR; 
    kregionx_size[0] = (size_t)HEAP_PSRAM1_SIZE;  
#if CONFIG_KMM_REGIONS == 3
    kregionx_start[1] = (void *)HEAP_START_ADDRESS;
    kregionx_start[2] = (void *)HEAP_PSRAM2_START_ADDR;
    kregionx_size[1]  = (size_t)(HEAP_END_ADDRESS - HEAP_START_ADDRESS);
    kregionx_size[2]  = (size_t)HEAP_PSRAM2_SIZE;
#elif CONFIG_KMM_REGIONS > 3
#error "Need to check here for heap."
#else
    kregionx_start[1] = (void *)HEAP_START_ADDRESS;
    kregionx_size[1]  = (size_t)(HEAP_END_ADDRESS - HEAP_START_ADDRESS);
#endif


#endif    //CONFIG_KMM_REGIONS == 1
}

__FLASH_BOOT_CODE void _start(void)
{
#if defined(CONFIG_PSRAM)
#if defined(CONFIG_XIP_KERNEL) && (CONFIG_XIP_KERNEL == 1)
    bk_psram_init();
#endif
#ifdef CONFIG_BUILD_PROTECTED
    bk_psram_code_init();
#endif
#endif

#if defined(CONFIG_MPU)
    mpu_init();
    bk_mpu_init();
#endif // CONFIG_MPU

#if defined(CONFIG_DCACHE)
    if (SCB->CLIDR & SCB_CLIDR_DC_Msk)
      SCB_EnableDCache();

    SCB_CleanInvalidateDCache();
    SCB_InvalidateICache();
#endif

#if !defined(CONFIG_WATCHDOG_FOR_IRQ)
    extern void close_wdt(void);
    close_wdt();
#endif

    os_heap_init();


#if defined(CONFIG_ATE_TEST) && defined(CONFIG_RESET_REASON)
    extern int cmd_do_memcheck(void);
    cmd_do_memcheck();
#endif

    /*power manager init*/
    pm_hardware_init();

    bk_gpio_driver_init();
	//Important notice!!!!!
	//ATE uses UART TX PIN as the detect ATE mode pin,
	//so it should be called after GPIO init and before UART init.
	//or caused ATE can't work or UART can't work
#if defined(CONFIG_ATE)
	bk_ate_init();
#endif
    bk_uart_driver_init();
    bk_flash_driver_init();

#if defined(CONFIG_ARMV8M_MPU) && defined(CONFIG_BUILD_PROTECTED)
	/* Initialize number of mpu regions for board specific purpose */
	mpu_set_nregion_board_specific(g_mpu_nregion_allocated);

	up_mpuinitialize();
#endif

#if defined(CONFIG_WATCHDOG)
	bk_wdt_driver_init();
#endif

#if defined(CONFIG_TRNG_SUPPORT)
	bk_trng_driver_init();
#endif

#if defined(CONFIG_SWD_DEBUG_MODE)
    set_jtag_mode();
#endif

#if defined(CONFIG_RESET_REASON)
	reset_reason_init();
#endif
    /* Call os_start() */
    extern void os_start(void);
    os_start();

    /* Shoulnd't get here */

    for (;;) ;

}

void user_except_handler_ex(uint32_t reset_reason, uint32_t lr, uint32_t sp);

__STATIC_FORCEINLINE void dump_system_info(uint32_t rr, uint32_t lr, uint32_t sp) {
    user_except_handler_ex(rr, lr, sp);
}

#define dump_fault_info(rr) \
{\
	uint32_t lr = __get_LR();\
	uint32_t sp = __get_MSP();\
\
    __asm volatile \
    (\
        "	push {r4-r11}								\n"\
    );\
\
	dump_system_info(rr, lr, sp);\
\
	while(1);\
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
__attribute__((naked)) void Default_Handler(void)
{
	dump_fault_info(RESET_SOURCE_DEFAULT_EXCEPTION);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif

