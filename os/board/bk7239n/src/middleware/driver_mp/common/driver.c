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

#include <common/bk_include.h>
#include "dd_pub.h"
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "sys_driver.h"
#include <driver/int.h>
#include <driver/pwm.h>
#include <driver/timer.h>
#include <driver/gpio.h>
#include <driver/dma.h>
#include <driver/uart.h>
#include <driver/wdt.h>
#include <driver/aon_wdt.h>
#include <driver/trng.h>
#include <driver/efuse.h>
#include <driver/ckmn.h>
#include <os/mem.h>
#include <driver/adc.h>
#include <driver/spi.h>
#include <driver/i2c.h>
#include <driver/aon_rtc.h>
#include <modules/pm.h>
#include <driver/psram.h>
#include <driver/lin.h>
#include "bk_driver.h"
#include "interrupt_base.h"
#include <driver/otp.h>
#include <driver/pwr_clk.h>



#if defined(CONFIG_SECURITY)
#include "bk_security.h"
#endif

#if defined(CONFIG_AON_PMU)
#include "aon_pmu_driver.h"
#endif

#if defined(CONFIG_FLASH)
#include <driver/flash.h>
#endif

#if defined(CONFIG_EASY_FLASH)
#include "bk_ef.h"
#endif

#if ((defined(CONFIG_SDIO_HOST)) || (defined(CONFIG_SDCARD)))
#include "driver/sdio_host.h"
#endif

#if defined(CONFIG_SDCARD)
#include "sdcard.h"
#endif

#if (defined(CONFIG_SDIO_V2P0) && defined(CONFIG_SDIO_SLAVE))
#include "sdio_slave_driver.h"
#if defined(CONFIG_SDIO_TEST_EN)
#include "sdio_test.h"
#endif
#endif

#if defined(CONFIG_QSPI)
#include <driver/qspi.h>
#endif

#if defined(CONFIG_JPEGENC_HW)
#include <driver/jpeg_enc.h>
#endif

#if defined(CONFIG_CALENDAR)
#include <driver/calendar.h>
#endif

#if defined(CONFIG_ATE)
#include <components/ate.h>
#endif

#if defined(CONFIG_TOUCH_PM_SUPPORT)
#include <driver/touch.h>
#endif

#if defined(CONFIG_CHIP_SUPPORT)
#include "modules/chip_support.h"
#endif

#if defined(CONFIG_YUV_BUF)
#include <driver/yuv_buf.h>
#endif

#if defined(CONFIG_H264)
#include <driver/h264.h>
#endif

#if defined(CONFIG_SDMADC)
#include <driver/sdmadc.h>
#endif

#if defined(CONFIG_HW_ROTATE_PFC)
#include <driver/rott_driver.h>
#endif

//TODO only init driver model and necessary drivers
#if defined(CONFIG_POWER_CLOCK_RF)
#define   MODULES_POWER_OFF_ENABLE (1)
#define   ROSC_DEBUG_EN            (0)
#define   MODULES_CLK_ENABLE       (0)
extern void clock_dco_cali(UINT32 speed);
void power_clk_rf_init()
{
    uint32_t param =0;
	/*power on all the modules for bringup test*/

	module_name_t use_module = MODULE_NAME_WIFI;
    /*1. power on all the modules*/
	#if MODULES_POWER_OFF_ENABLE
	    sys_drv_module_power_ctrl(POWER_MODULE_NAME_ENCP,POWER_MODULE_STATE_OFF);
		//sys_drv_module_power_ctrl(POWER_MODULE_NAME_BAKP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_AUDP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_VIDP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_WIFI_PHY,POWER_MODULE_STATE_OFF);
		sys_drv_module_power_ctrl(POWER_MODULE_NAME_CPU1,POWER_MODULE_STATE_OFF);
	#else
	    power_module_name_t module = POWER_MODULE_NAME_MEM1;
        for(module = POWER_MODULE_NAME_MEM1 ; module < POWER_MODULE_NAME_NONE ; module++)
        {
            sys_drv_module_power_ctrl(module,POWER_MODULE_STATE_ON);
        }
    #endif
    /*2. enable the analog clock*/
    sys_drv_module_RF_power_ctrl(use_module ,POWER_MODULE_STATE_ON);

	/*3.enable all the modules clock*/
	#if MODULES_CLK_ENABLE
	dev_clk_pwr_id_t devid = 0;
	for(devid = 0; devid < 32; devid++)
	{
	    sys_drv_dev_clk_pwr_up(devid, CLK_PWR_CTRL_PWR_UP);
    }
	#endif
    /*4.set the cpu0 and matrix clock*/
   /*cpu0:26m ,matrix:26m*/
   //sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 1,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
   /*cpu0:120m ,matrix:120m*/
   //sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 3,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
   /*cpu0:240m ,matrix:120m*/
	//sys_drv_core_bus_clock_ctrl(HIGH_FREQUECY_CLOCK_MODULE_CPU0, 0,0, HIGH_FREQUECY_CLOCK_MODULE_CPU0_MATRIX,0,0);
	#if defined(CONFIG_SYS_CPU0)
	//bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_120M);
	#if defined(CONFIG_ATE_TEST)
		//bk_pm_module_vote_cpu_freq(PM_DEV_ID_DEFAULT,PM_CPU_FRQ_320M);//improve the cpu frequency for save boot time at ate test
	#endif
	#endif
   /*5.config the analog*/
   //sys_drv_analog_set(ANALOG_REG0, param);
   //sys_drv_analog_set(ANALOG_REG0, param);
   //sys_drv_analog_set(ANALOG_REG0, param);

	//config apll
	//param = 0;
	//param = sys_drv_analog_get(ANALOG_REG4);
	//param &= ~(0x1f << 5);
	//param |= (0x14 << 5);
	//sys_drv_analog_set(ANALOG_REG4, param);

	/*set low power low voltage value */
#if 0
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG3);
	param &= ~(0x7 << 29);
	param |= (0x4 << 29);
	sys_drv_analog_set(ANALOG_REG3, param);

	param = sys_drv_analog_get(ANALOG_REG2);
	param |= (0x1 << 25);
	sys_drv_analog_set(ANALOG_REG2, param);
#endif
	/*tempreture det enable for VIO*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG6);
	param |= (0x1 << SYS_ANA_REG6_EN_TEMPDET_POS)|(0x7 << SYS_ANA_REG6_RXTAL_LP_POS)|(0x7 << SYS_ANA_REG6_RXTAL_HP_POS);
	param &= ~(0x1 << SYS_ANA_REG6_EN_SLEEP_POS);
	sys_drv_analog_set(ANALOG_REG6, param);

#if 0
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG3);
	param = 0x1 << 0; //security boot  bypass
	aon_pmu_drv_reg_set(PMU_REG3,param);
#endif
	/*let rosc to bt/wifi ip*/
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG0x41);
	param |= 0x1 << 24;
	aon_pmu_drv_reg_set(PMU_REG0x41,param);
	/*wake delay of Xtal*/
#if 0
	param = 0;
	param = aon_pmu_drv_reg_get(PMU_REG0x40);
	param |= 0xF << 0;
	aon_pmu_drv_reg_set(PMU_REG0x40,param);
#endif
	/* rosc calibration start*/
	/*a.open rosc debug*/
#if ROSC_DEBUG_EN
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG5);
	param |= 0x1 << SYS_ANA_REG5_CK_TST_ENBALE_POS;
	sys_drv_analog_set(ANALOG_REG5,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG11);
	param |= 0x1 << SYS_ANA_REG11_TEST_EN_POS;
	sys_drv_analog_set(ANALOG_REG11,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param |= (0x1 << SYS_ANA_REG4_ROSC_TSTEN_POS);//Rosc test enable
	sys_drv_analog_set(ANALOG_REG4,param);

#endif
	/*b.config calibration*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param &= ~(SYS_ANA_REG4_ROSC_CAL_INTVAL_MASK << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS);//clear the data
	param |= 0x4 << SYS_ANA_REG4_ROSC_CAL_INTVAL_POS;//Rosc Calibration Interlval 0.25s~2s (4:1s)
	param |= 0x1 << SYS_ANA_REG4_ROSC_CAL_MODE_POS;//0x1: 32K ;0x0: 31.25K
	param |= 0x1 << SYS_ANA_REG4_ROSC_CAL_EN_POS;//Rosc Calibration Enable
	param &= ~(0x1 << SYS_ANA_REG4_ROSC_MANU_EN_POS);//0:close Rosc Calibration Manual Mode
	sys_drv_analog_set(ANALOG_REG4,param);

	/*c.trigger calibration*/
	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param &= ~(0x1 << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);//trigger clear
	sys_drv_analog_set(ANALOG_REG4,param);

	param = 0;
	param = sys_drv_analog_get(ANALOG_REG4);
	param |= (0x1 << SYS_ANA_REG4_ROSC_CAL_TRIG_POS);//trigger enable
	sys_drv_analog_set(ANALOG_REG4,param);
	/* rosc calibration end*/
	/*7.dpll calibration */
	//sys_drv_cali_dpll(0);

	/*8.dco calibration*/
	//clock_dco_cali(0x4);
}
#endif

#if (!defined(CONFIG_ATE_TEST))
int mcheck_section(uint32_t* reg_addr, uint16_t* output, int reg_points_num, int output_points_num, int addr_len, int singe_repair_points)
{
    int reg_index;
    uint32_t reg_value;

    for (reg_index=0; reg_index< reg_points_num; reg_index++)
    {
        // keep the low part of register
        reg_value = reg_addr[reg_index] & 0x0000FFFF;

        // confirm whether the address is valid
        if ((reg_value >> (addr_len-1)) % 2 == 1)
        {
            //printf("MCHECK fail! stop register index:%d\r\n", reg_index);
            return 1;
        }
    }

    return 0;
}

int mcheck_ate(uint16_t check_res[48])
{
    uint32_t* reg_start_addr;
    int res = -1;

#if (defined(CONFIG_SOC_BK7236XX))
    reg_start_addr = (uint32_t*)SOC_MEM_CHECK_REG_BASE;

    // SMEM2-5 and h265, overall 20 points, no share
    res = mcheck_section(reg_start_addr + 8, check_res + 0, 20, 20, 16, 4);

    // SMEM0-1 overall 8 points, share 4 points
    res += mcheck_section(reg_start_addr + 28, check_res + 20, 8, 4, 15, 4);

   // cpu0_0->cpu2_0 overall 12 points, share 6 points
    res += mcheck_section(reg_start_addr + 36, check_res + 24, 12, 6, 14, 4);

   // cpu0_1->cpu1_3 overall 16 points, share 8 points
    res += mcheck_section(reg_start_addr + 48, check_res + 30, 16, 8, 13, 2);

   // cpu2_3->pram overall 14 points, share 8 points
    res += mcheck_section(reg_start_addr + 64, check_res + 38, 14, 8, 13, 2);

   // usb2->dmad overall 4 points, share 2 points
    res += mcheck_section(reg_start_addr + 78, check_res + 46, 4, 2, 12, 2);
#else
    (void)reg_start_addr;
    res = 0;
#endif

    return res;
}
#endif

int driver_init(void)
{
//BRING_UP_TAG_START: bk7239n BRING UP code, disable here to boot up, need enable later
	//sys_drv_init();
//BRING_UP_TAG_END
#if defined(CONFIG_AON_PMU)
	aon_pmu_drv_init();
#endif

#if defined(CONFIG_POWER_CLOCK_RF)
	power_clk_rf_init();
#endif

	interrupt_init();

	//gpio driver is inited at tizen starup period
	//bk_gpio_driver_init();

	//Important notice!!!!!
	//ATE uses UART TX PIN as the detect ATE mode pin,
	//so it should be called after GPIO init and before UART init.
	//or caused ATE can't work or UART can't work
// #if defined(CONFIG_ATE)
// 	bk_ate_init();
// #endif

	//Important notice!
	//Before UART is initialized, any call of BK_LOG_RAW/os_print/BK_LOGx may
	//cause problems, such as crash etc!
	//uart driver is inited at tizen starup period
	//bk_uart_driver_init();



#if defined(CONFIG_FLASH)
	bk_flash_driver_init();
#endif

#if defined(CONFIG_EASY_FLASH)
	easyflash_init();
#endif
#if 0
#if defined(CONFIG_SECURITY)
	bk_secrity_init();
#endif
#endif
#if defined(CONFIG_OTP)
	bk_otp_driver_init();
#endif

#if defined(CONFIG_PWM)
	//bk_pwm_driver_init();
#endif

#if defined(CONFIG_TIMER)
	bk_timer_driver_init();
#endif

#if defined(CONFIG_GENERAL_DMA)
	bk_dma_driver_init();
#endif

	//wdt driver is inited at starup period
	//bk_wdt_driver_init();

// #if defined(CONFIG_TRNG_SUPPORT)
// 	bk_trng_driver_init();
// #endif

#if defined(CONFIG_SARADC)
	bk_adc_driver_init();
#endif

#if defined(CONFIG_SPI)
	//bk_spi_driver_init();
#endif

#if defined(CONFIG_I2C)
	bk_i2c_driver_init();
#endif

#if defined(CONFIG_QSPI)
	bk_qspi_driver_init();
#endif


#if defined(CONFIG_AON_RTC) || defined(CONFIG_ANA_RTC) //Move the rtc init here for getting the more time to caculate the tick of rtc at the bellow.
	bk_aon_rtc_driver_init();
#endif

#if ((defined(CONFIG_SDIO_HOST)) || (defined(CONFIG_SDCARD)))
	bk_sdio_host_driver_init();
#endif

#if defined(CONFIG_CALENDAR)
	bk_calendar_driver_init();
#endif

//call it after LOG is valid.
#if defined(CONFIG_ATE)
	SYS_DRV_LOGI("ate enabled is %d\r\n", ate_is_enabled());
#endif


#if defined(CONFIG_SDMADC)
	//bk_sdmadc_driver_init();
#endif

#if defined(CONFIG_CKMN)
	bk_ckmn_driver_init();
#endif


#if defined(CONFIG_MBEDTLS_ACCELERATOR)
	extern int dubhe_driver_init( unsigned long dbh_base_addr );
	dubhe_driver_init(SOC_SHANHAI_BASE);
#endif

	SYS_DRV_LOGI("driver_init end\r\n");

	return 0;
}

