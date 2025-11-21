#include "type.h"
#include "bl_config.h"
#include "system.h"
#include "bootloader.h"
#include "bl_uart.h"
#include "pal_log.h"
#include "hal_platform.h"
#include "cmsis.h"

#if (CPU_RISCV)
#include "int.h"
#endif

#if (CPU_ARM)
#include "intc.h"
#endif

#if (CPU_BA22)
#include "exception.h"
#include "spr_defs.h"
#endif
#include "boot_protocol.h"
/***************************************************CHANGE ***************************/
#define IO_FUNC_UART0_RXD  82
#define IO_FUNC_UART0_TXD  83
#define IOMX_FUNC_CFG10_REG               *((volatile unsigned long*)(SOC_IOMX_REG_BASE + (0x4a * 4)))
#define IOMX_FUNC_CFG11_REG               *((volatile unsigned long*)(SOC_IOMX_REG_BASE + (0x4b * 4)))
#define IOMX_SOFT_RST_REG                 *((volatile unsigned long*)(SOC_IOMX_REG_BASE + (2 * 4)))

void BK3000_ICU_Initial(void)
{
	UART0_RESET;
	UART0_ENABLE;
	SPI0_ENABLE;
	UART0_INT_ENABLE;
}

void control_set_to_flash_not_enable_jtag()
{
	*(volatile uint32_t *)0x44000600 = 0x5A000A;
	*(volatile uint32_t *)0x44000600 = 0xA5000A;
	while(1);
}

void control_set_to_flash()
{
	//hal_jtag_enable();
	control_set_to_flash_not_enable_jtag();
}

extern u16 COUNT;
void uart_enable(uint8_t flag,uint8_t rx_thr) 
{
	if (flag == 0)
		boot_uart_init(UART_BAUDRATE_DEFAULE,rx_thr);
	else
	{
		boot_uart_init(UART_BAUDRATE_DEFAULE,rx_thr);//UART_BAUDRATE_2000000
	}

	IOMX_SOFT_RST_REG |= 1;
	IOMX_FUNC_CFG10_REG |= ((IO_FUNC_UART0_RXD & 0xFF) << 24);
	IOMX_FUNC_CFG11_REG |= ((IO_FUNC_UART0_TXD & 0xFF) << 24);
}

void PMU_uninit( void )
{
	UART0_DISABLE      ;
	SPI0_DISABLE       ;
	UART0_INT_DISABLE  ;
	addSYSTEM_Reg0xa &= ~(1 << 12); //restore 40M
	SET_FLASHCTRL_RW_FLASH;	
}

void BK3000_start_wdt(uint32_t val)
{
	/* On FPGA, the WDT clock is 25Mhz, it will trigger watchdog soon,
	 * since FPGA doesn't support reset, it triggers NMI instead of
	 * WDT reset.
	 * */
#if CONFIG_WDT
	BK3000_WDT_CONFIG = 0x5A0000 | val;
	BK3000_WDT_CONFIG = 0xA50000 | val;
#endif
}

/************************************CHANGE ***************************************************/
/***************************************************************************************/
void uart_disable(void) {
	REG_APB3_UART_CFG = 0;
}

void arch_enable_uart_int(void)
{
    NVIC_EnableIRQ(4);
}

void SYSirq_Initialise(void) {
	arch_enable_uart_int();
}

void bl_init(void) {
	bsp_initial();
	SYSirq_Initialise();
}

#ifdef FLASH_DEBUG
void set_flash_clk(unsigned char clk_conf) {
    unsigned int temp0;
    temp0 = reg_FLASH_CONF;
    reg_FLASH_CONF = (  (clk_conf << BIT_FLASH_CLK_CONF)
                      | (temp0    &  SET_MODE_SEL)
                      | (temp0    &  SET_FWREN_FLASH_CPU)
                      | (temp0    &  SET_WRSR_DATA)
                      | (temp0    &  SET_CRC_EN));
}
#endif

void watchdog_stop(void) {
	BK3000_WDT_CONFIG = 0x5A0000;
	BK3000_WDT_CONFIG = 0xA50000;
	return;
}

void BK3000_ICU_Initial(void);
void bsp_initial(void)
{
	BK3000_ICU_Initial();
}
extern u8 uart_link_check_flag ;

#define addEFUSE_Reg0x2                                         *((volatile unsigned long *) (0x44880000+0x2*4))
#define addEFUSE_Reg0x4                                         *((volatile unsigned long *) (0x44880000+0x4*4))
#define addEFUSE_Reg0x5                                         *((volatile unsigned long *) (0x44880000+0x5*4))
#define get_EFUSE_Reg0x1_efuse_read_data_valid                  ((addEFUSE_Reg0x5 & 0x100) >> 8)

void deep_sleep_to_flash()
{
	PAL_LOG_DEBUG("deep sleep rst, pmu reg0=%x\n", BK3000_PMU_RESET_REASON_GET);
	if( DEEP_SLEEP_RESTART )//restart from deepsleep ,so go to flash
	{
#if(CHIP_BK7256)
    		addEFUSE_Reg0x0 = 0x01 | (0x1e << 8);
    		while(get_EFUSE_Reg0x1_efuse_read_data_valid == 0);

		if( (addEFUSE_Reg0x1 & 0x4) == 0 ){    //efuse 0x1e[2] 0: deepsleep fast boot 1: deepsleep sb boot
    			control_set_to_flash();
    		}
#elif(CHIP_BK7236)
		addEFUSE_Reg0x2 = 0x03;
		addEFUSE_Reg0x4 = 0x01 | (0x0 << 8);
		while(get_EFUSE_Reg0x1_efuse_read_data_valid == 0);

		if( (addEFUSE_Reg0x5 & 0x4) == 0 ){    //efuse 0x0[2] 0: deepsleep fast boot 1: deepsleep sb boot
			control_set_to_flash();
		}
#else
		control_set_to_flash() ;
#endif
	}
}


void legacy_boot_main(void)
{
	int restart_t = 5;

	bl_init();
	uart_enable(1,12);
	u32 i = timer_init(restart_t * 30);
	boot_uart_init(UART_BAUDRATE_DEFAULE,RX_FIFO_THR_COUNT);
	while(i--)//wait 5ms
	{
		boot_rx_frm_handler();
		BK3000_start_wdt(0xA000);
		if(uart_link_check_flag == 1)
		{
			while(1)
			{
				boot_rx_frm_handler();
				BK3000_start_wdt(0xA000);
			}
		}
	}

	PMU_uninit();  //must need this change the way of operating flash.
	BK3000_start_wdt(0xA000);
    return ;
}
