#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include "bk_sys_ctrl.h"
#include "sys_ctrl.h"
#include "sys_driver.h"
#include "bk_misc.h"
#include "bk_drv_model.h"
#include "bk_uart.h"
#include <driver/int.h>
#include "bk_icu.h"
#include "bk_gpio.h"
#include "reset_reason.h"
#include <os/os.h>

#if (!defined(CONFIG_SOC_BK7256XX))

#if ((defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX)))
#define GPIO_WAKEUP_INT_BAK_ADDR (0x00808000)
#elif ((defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
#define GPIO_WAKEUP_INT_BAK_ADDR (0x10E00000)
#elif (defined(CONFIG_SOC_BK7256XX) && defined(CONFIG_SYS_CPU1))
#define GPIO_WAKEUP_INT_BAK_ADDR (0x00808000)
#else
#define GPIO_WAKEUP_INT_BAK_ADDR (0x0080a084)
#endif
#define GPIO_WAKEUP_INT_STATUS_BAK  (GPIO_WAKEUP_INT_BAK_ADDR + 0x1 * 4)
#define GPIO_WAKEUP_INT_STATUS1_BAK  (GPIO_WAKEUP_INT_BAK_ADDR + 0x2 * 4)
#if defined(CONFIG_SOC_BK7271)
#include "pmu.h"
#endif

#include "bk_normal_sleep.h"

#define DPLL_DIV                0x0
#define DCO_CALIB_26M           0x1
#define DCO_CALIB_60M           0x2
#define DCO_CALIB_80M           0x3
#define DCO_CALIB_120M          0x4
#define DCO_CALIB_180M          0x5

#if (defined(CONFIG_SOC_BK7271))
#define DCO_CALIB_240M          0x6
#define DCO_CLK_SELECT          DCO_CALIB_240M
#define USE_DCO_CLK_POWON       0
#define SCTRL_AUDIO_PLL_SDM     SCTRL_ANALOG_AUDIO_PLL_SDM
#define SCTRL_AUDIO_PLL_CTRL    SCTRL_ANALOG_AUDIO_PLL_CTRL
#elif (defined(CONFIG_SOC_BK7251))
#define DCO_CLK_SELECT          DCO_CALIB_180M
#define USE_DCO_CLK_POWON       1

UINT8  calib_charger[3] = {
	0x23,   //vlcf
	0x15,   //icp
	0x1b    //vcv
};
#else
#define DCO_CLK_SELECT          DCO_CALIB_120M
/* BK7231N could not using DCO as main clock when boot since DCO_AMSEL_BIT should be disable during calibration*/
#define USE_DCO_CLK_POWON       0
#endif

#if (defined(CONFIG_DEEP_PS) && PS_SUPPORT_MANUAL_SLEEP)
static UINT32 ps_block_value = 0;
#endif
#if defined(CONFIG_DEEP_PS)
static UINT32 deep_sleep_gpio_floating_map = 0;
static uint32_t gpio_0_31_status = 0;
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX))
static uint32_t gpio_32_39_status = 0;
static UINT32 deep_sleep_gpio_last_floating_map = 0;
#endif
#endif
#ifndef CONFIG_SOC_BK7256XX
static const DD_OPERATIONS sctrl_op = {
	NULL,
	NULL,
	NULL,
	NULL,
	sctrl_ctrl
};
#endif
static sctrl_cal_bias_cb_t s_cal_bias_callback = NULL;
static sctrl_wifi_phy_wakeup_rf_reinit_cb_t s_wifi_phy_wakeup_rf_reinit_cb = NULL;
static sctrl_wifi_phy_wakeup_wifi_reinit_cb_t s_wifi_phy_wakeup_wifi_reinit_cb = NULL;

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
void sctrl_fix_dpll_div(void);
#endif

/**********************************************************************/

void sctrl_register_cal_bias_callback(sctrl_cal_bias_cb_t cb)
{
	s_cal_bias_callback = cb;
}

void sctrl_register_wifi_phy_wakeup_rf_reinit_callback(sctrl_wifi_phy_wakeup_rf_reinit_cb_t cb)
{
	s_wifi_phy_wakeup_rf_reinit_cb = cb;
}

void sctrl_register_wifi_phy_wakeup_wifi_reinit_callback(sctrl_wifi_phy_wakeup_wifi_reinit_cb_t cb)
{
	s_wifi_phy_wakeup_wifi_reinit_cb = cb;
}

void sctrl_dpll_delay10us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < DPLL_DELAY_TIME_10US; i ++)
		;
}

void sctrl_dpll_delay200us(void)
{
	volatile UINT32 i = 0;

	for (i = 0; i < DPLL_DELAY_TIME_200US; i ++)
		;
}

void sctrl_ps_dpll_delay(UINT32 time)
{
	volatile UINT32 i = 0;

	for (i = 0; i < time; i ++)
		;
}

void sctrl_cali_dpll(UINT8 flag)
{
	UINT32 param;

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	extern void bk7011_update_tx_power_when_cal_dpll(int start_or_stop);

	bk7011_update_tx_power_when_cal_dpll(1);
#endif
	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param &= ~(SPI_TRIG_BIT);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

	if (!flag)
		sctrl_dpll_delay10us();
	else
		sctrl_ps_dpll_delay(60);

	param |= (SPI_TRIG_BIT);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param &= ~(SPI_DET_EN);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

	if (!flag)
		sctrl_dpll_delay200us();
	else
		sctrl_ps_dpll_delay(340);

	param = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
	param |= (SPI_DET_EN);
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	bk7011_update_tx_power_when_cal_dpll(0);
#endif
}

void sctrl_dpll_isr(void)
{
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
	if ((DEVICE_ID_BK7231N_P & DEVICE_ID_MASK) != (sctrl_ctrl(CMD_GET_DEVICE_ID, NULL) & DEVICE_ID_MASK))
	{
		SCTRL_LOGI("BIAS Cali\r\n");
		if (s_cal_bias_callback) {
			s_cal_bias_callback();
		}
	}
#elif (defined(CONFIG_SOC_BK7236A))
	SCTRL_LOGI("BIAS Cali\r\n");
	if (s_cal_bias_callback) {
		s_cal_bias_callback();
	}
#endif
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_CLR_DPLL_UNLOOK_INT_BIT, NULL);
	sctrl_cali_dpll(0);

	SCTRL_LOGI("DPLL Unlock\r\n");
}

void sctrl_dpll_int_open(void)
{
	UINT32 param;

	param = (FIQ_DPLL_UNLOCK_BIT);
#if (defined(CONFIG_SOC_BK7271))
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_FIQ_ENABLE, &param);
	(void)sys_drv_fiq_enable(param);
#else
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_ENABLE, &param);
	(void)sys_drv_int_enable(param);
#endif

#if (!defined(CONFIG_SOC_BK7231))
	param = 1;
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_EN_DPLL_UNLOOK_INT, &param);
#endif
}

void sctrl_dpll_int_close(void)
{
	UINT32 param;

#if (!defined(CONFIG_SOC_BK7231))
	param = 0;
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_EN_DPLL_UNLOOK_INT, &param);
#endif

	param = (FIQ_DPLL_UNLOCK_BIT);
#if (defined(CONFIG_SOC_BK7271))
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_FIQ_DISABLE, &param);
	(void)sys_drv_fiq_disable(param);
#else
	// sddev_control(DD_DEV_TYPE_ICU, CMD_ICU_INT_DISABLE, &param);
	(void)sys_drv_int_disable(param);
#endif
}
void clock_dco_cali(UINT32 speed)
{
	UINT32 reg_val;

	switch (speed) {
#if (defined(CONFIG_SOC_BK7271))
	case DCO_CALIB_240M:
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= DIV_BYPASS_BIT;
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;
#endif

	case DCO_CALIB_180M:
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0xDD & DCO_CNTI_MASK) << DCO_CNTI_POSI);
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
		reg_val |= DIV_BYPASS_BIT;
#endif
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;

	case DCO_CALIB_120M:
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;

	case DCO_CALIB_80M:
		reg_val = sctrl_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x0C5 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;

	case DCO_CALIB_60M:
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= ((0x02 & DCO_DIV_MASK) << DCO_DIV_POSI);
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;

	default:
		reg_val = sys_drv_analog_get(ANALOG_REG1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0xC0 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= ((0x03 & DCO_DIV_MASK) << DCO_DIV_POSI);
		sys_drv_analog_set(ANALOG_REG1, reg_val);
		break;
	}

	reg_val = sys_drv_analog_get(ANALOG_REG1);
	reg_val &= ~(SPI_RST_BIT);
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	reg_val &= ~(DCO_AMSEL_BIT);
#endif
	sys_drv_analog_set(ANALOG_REG1, reg_val);

	reg_val = sys_drv_analog_get(ANALOG_REG1);
	reg_val |= SPI_RST_BIT;
	sys_drv_analog_set(ANALOG_REG1, reg_val);

	reg_val = sys_drv_analog_get(ANALOG_REG1);
	reg_val |= DCO_TRIG_BIT;
	sys_drv_analog_set(ANALOG_REG1, reg_val);

	reg_val = sys_drv_analog_get(ANALOG_REG1);
	reg_val &= ~(DCO_TRIG_BIT);
	sys_drv_analog_set(ANALOG_REG1, reg_val);
}
void sctrl_dco_cali(UINT32 speed)
{
	UINT32 reg_val;

	switch (speed) {
#if (defined(CONFIG_SOC_BK7271))
	case DCO_CALIB_240M:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= DIV_BYPASS_BIT;
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;
#endif

	case DCO_CALIB_180M:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0xDD & DCO_CNTI_MASK) << DCO_CNTI_POSI);
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
		reg_val |= DIV_BYPASS_BIT;
#endif
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;

	case DCO_CALIB_120M:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;

	case DCO_CALIB_80M:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x0C5 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;

	case DCO_CALIB_60M:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0x127 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= ((0x02 & DCO_DIV_MASK) << DCO_DIV_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;

	default:
		reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		reg_val &= ~((DCO_CNTI_MASK << DCO_CNTI_POSI) | (DCO_DIV_MASK << DCO_DIV_POSI));
		reg_val |= ((0xC0 & DCO_CNTI_MASK) << DCO_CNTI_POSI);
		reg_val |= ((0x03 & DCO_DIV_MASK) << DCO_DIV_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
		break;
	}

	reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
	reg_val &= ~(SPI_RST_BIT);
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	reg_val &= ~(DCO_AMSEL_BIT);
#endif
	sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);

	reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
	reg_val |= SPI_RST_BIT;
	sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);

	reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
	reg_val |= DCO_TRIG_BIT;
	sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);

	reg_val = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
	reg_val &= ~(DCO_TRIG_BIT);
	sctrl_analog_set(SCTRL_ANALOG_CTRL1, reg_val);
}

void sctrl_set_cpu_clk_dco(void)
{
	UINT32 reg_val;

	reg_val = REG_READ(SCTRL_CONTROL);
	reg_val &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
	reg_val &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);

	reg_val |= ((MCLK_FIELD_DCO & MCLK_MUX_MASK) << MCLK_MUX_POSI);
	reg_val |= HCLK_DIV2_EN_BIT;

	REG_WRITE(SCTRL_CONTROL, reg_val);
	delay(10);

}


void sctrl_ble_ps_init(void)
{
}

static void sctrl_mac_ahb_slave_clock_enable(void)
{
	UINT32 reg;
#if (defined(CONFIG_SOC_BK7271))
	reg = REG_READ(SCTRL_CONTROL);
	REG_WRITE(SCTRL_CONTROL, reg | MAC_HCLK_EN_BIT);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg | MAC_HCLK_EN_BIT);
#endif
}

void sctrl_mac_ahb_slave_clock_disable(void)
{
	UINT32 reg;
#if (defined(CONFIG_SOC_BK7271))
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~MAC_HCLK_EN_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	reg &= ~MAC_HCLK_EN_BIT;
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif
}
#ifndef CONFIG_SOC_BK7256XX
void sctrl_init(void)
{
	UINT32 param;

#if (defined(CONFIG_SOC_BK7271))
	/* told by huaming to avoid reset */
	addGPIO_Reg0xf = 0x38;
	//set link bus clock, DPLL div 4
	param = REG_READ(LBUS_CONF1_REG);
	param &= ~0x7;
	param |= 0x7;
	REG_WRITE(LBUS_CONF1_REG, param);
#endif

	sddev_register_dev(DD_DEV_TYPE_SCTRL, (DD_OPERATIONS *)&sctrl_op);

	/*enable blk clk
	  Attention: ENABLE 26m xtal block(BLK_BIT_26M_XTAL), for protect 32k circuit
	 */
	param = BLK_BIT_26M_XTAL | BLK_BIT_DPLL_480M | BLK_BIT_XTAL2RF | BLK_BIT_DCO;
#if (defined(CONFIG_SOC_BK7271))
	param |= BLK_BIT_AUDIO_PLL;
#endif
	sctrl_ctrl(CMD_SCTRL_BLK_ENABLE, &param);

	/*config main clk*/
#if !USE_DCO_CLK_POWON
	param = REG_READ(SCTRL_CONTROL);
	param &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
	param &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);

#if (defined(CONFIG_SOC_BK7271))
	param &= ~(HCLK_DIV2_MASK << HCLK_DIV2_POSI);
	param |= HCLK_DIV2_EN_BIT;
	param &= ~(MTB_PRIVILEGE_MASK << MTB_PRIVILEGE_POSI);
	param |= (MTB_PRIVILEGE_ACCESS_AHB << MTB_PRIVILEGE_POSI);
#elif (defined(CONFIG_SOC_BK7251))
	/* BK7221U ahb bus max rate is 90MHZ, so ahb bus need div 2 from MCU clock */
	/* AHB bus is very import to AUDIO and DMA */
	param |= HCLK_DIV2_EN_BIT;
#endif // (CONFIG_SOC_BK7251)

#if defined(CONFIG_SYS_REDUCE_NORMAL_POWER)
	param |= ((MCLK_DIV_7 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
	param |= ((MCLK_DIV_5 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
#elif (defined(CONFIG_SOC_BK7271))
	param |= ((MCLK_DIV_3 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
#elif (defined(CONFIG_SOC_BK7236A))
	// for bk7236, no less than 60MHZ, it better 80M above, here set 120M for debug
	param |= ((MCLK_DIV_3 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
#else // CONFIG_SYS_REDUCE_NORMAL_POWER
	param |= ((MCLK_DIV_3 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
#endif // CONFIG_SYS_REDUCE_NORMAL_POWER

	param |= ((MCLK_FIELD_DPLL & MCLK_MUX_MASK) << MCLK_MUX_POSI);

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	param |= (BLE_RF_PTA_EN_BIT);
#endif
	REG_WRITE(SCTRL_CONTROL, param);
#endif // (!USE_DCO_CLK_POWON)

	/*sys_ctrl <0x4c> */
#if (defined(CONFIG_SOC_BK7271))
	param = (0x17 << LDO_VAL_MANUAL_POSI) | (BIAS_CAL_MANUAL_BIT);//0x00151510;    LDO BIAS CALIBRATION
#else
	param = 0x00171710;//0x00151510;    LDO BIAS CALIBRATION
#endif
	REG_WRITE(SCTRL_BIAS, param);

	/*mac & modem power up */
#if (defined(CONFIG_SOC_BK7271))
	sctrl_ctrl(CMD_SCTRL_DSP_POWERUP, NULL);
	sctrl_ctrl(CMD_SCTRL_BT_POWERUP, NULL);
	sctrl_ctrl(CMD_SCTRL_LBUS_POWERUP, NULL);
#else
	sctrl_ctrl(CMD_SCTRL_MAC_POWERUP, NULL);
#endif
	sctrl_ctrl(CMD_SCTRL_MODEM_POWERUP, NULL);

	/*sys_ctrl <0x16>, trig spi */
	//170209,from 0x819A54B to 0x819A55B for auto detect dpll unlock
	//170614 from 0x819A55B to 0x819A59B for more easy to trigger
	//181101 xamp:0xf-0 for xtal may dead
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
#if (defined(CONFIG_XTAL_FREQ_40M))
	param = 0x71125B57;
#else
	param = 0x71104953;//wangjian20200918 Reg0x16<3:1>=1 Reg0x16<9>=0 Reg0x16<13:10>=2
#endif
#elif (defined(CONFIG_SOC_BK7271))
	param = 0x61105B57;
#else
	param = 0x819A59B;
#endif
	sctrl_analog_set(SCTRL_ANALOG_CTRL0, param);

	sctrl_cali_dpll(0);

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	param = 0x3CC019C2;//wangjian20200918 Reg0x17<1>=1
	/* BK7271: 0x17[28:26] sw_c<2:0>   0   R/W DCO Frequency Triming; 000=freq max; 111=freq min; */
#elif (defined(CONFIG_SOC_BK7271))
	param = 0x70C03902;
#else
	param = 0x6AC03102;
#endif

	sctrl_analog_set(SCTRL_ANALOG_CTRL1, param);
	/*do dco Calibration*/
	sctrl_dco_cali(DCO_CLK_SELECT);
#if USE_DCO_CLK_POWON
	sctrl_set_cpu_clk_dco();
#endif

#if (defined(CONFIG_SOC_BK7231))
	param = 0x24006000;
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
	if ((DEVICE_ID_BK7231N_P & DEVICE_ID_MASK) == (sctrl_ctrl(CMD_GET_DEVICE_ID, NULL) & DEVICE_ID_MASK))
	{
		param = 0x580020E2;//wangjian20210422<28:23>=30 as default for BK7231P
	}
	else
	{
		param = 0x500020E2;//0x400020E0; //wangjian20200822 0x40032030->0x48032030->0x48022032//wangjian20200903<17:16>=0//qunshan20201127<28:23>=20
	}
#elif (defined(CONFIG_SOC_BK7236A))
	param = 0x500020E2;//0x400020E0; //wangjian20200822 0x40032030->0x48032030->0x48022032//wangjian20200903<17:16>=0//qunshan20201127<28:23>=20
#elif (defined(CONFIG_SOC_BK7271))
	param = 0x80208B00; //for 32k if enable BLK_BIT_ROSC32K
#else
	param = 0x24006080;   // xtalh_ctune   // 24006080
	param &= ~(XTALH_CTUNE_MASK << XTALH_CTUNE_POSI);
	param |= ((0x10 & XTALH_CTUNE_MASK) << XTALH_CTUNE_POSI);
#endif // (CONFIG_SOC_BK7231)

	sctrl_analog_set(SCTRL_ANALOG_CTRL2, param);

#if (defined(CONFIG_SOC_BK7251))
	param = CHARGE_ANALOG_CTRL3_CHARGE_DEFAULT_VALUE;
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	param = 0x70000000; //wangjiang20200822 0x00000000->0x70000000
#elif (defined(CONFIG_SOC_BK7271))
	param = 0x004CA800;
#else
	param = 0x4FE06C50;
#endif
	sctrl_analog_set(SCTRL_ANALOG_CTRL3, param);

	/*sys_ctrl <0x1a> */
#if (defined(CONFIG_SOC_BK7231))
	param = 0x59E04520;
#elif (defined(CONFIG_SOC_BK7251))
	param = CHARGE_ANALOG_CTRL4_CAL_DEFAULT_VALUE;
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	param = 0x19C04520;
#elif (defined(CONFIG_SOC_BK7271))
	param = 0x800C1000;
#else
	param = 0x59C04520;  // 0x59E04520
#endif // (CONFIG_SOC_BK7231)
	sctrl_analog_set(SCTRL_ANALOG_CTRL4, param);

	/*regist intteruppt handler for Dpll unlock*/
	bk_int_isr_register(INT_SRC_PLL_UNLOCK, sctrl_dpll_isr, NULL);

	sctrl_sub_reset();

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
	if ((DEVICE_ID_BK7231N_P & DEVICE_ID_MASK) != (sctrl_ctrl(CMD_GET_DEVICE_ID, NULL) & DEVICE_ID_MASK)) {
		sctrl_fix_dpll_div();
	}
#endif

	/*sys ctrl clk gating, for rx dma dead*/
	REG_WRITE(SCTRL_CLK_GATING, 0x3f);

	/* increase VDD voltage*/
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	param = 4;//wyg// original=3
#elif defined(CONFIG_SYS_REDUCE_NORMAL_POWER)
	param = 4;
#else
	param = 5;
#endif
	sctrl_ctrl(CMD_SCTRL_SET_VDD_VALUE, &param);

	/*32K Rosc calib*/
#if (defined(CONFIG_SOC_BK7271))
	REG_WRITE(SCTRL_ROSC_CAL, 0x80208B00);
#else
	REG_WRITE(SCTRL_ROSC_CAL, 0x7);
#endif

#if (defined(CONFIG_AUDIO))
#if (defined(CONFIG_SOC_BK7251))
	sctrl_analog_set(SCTRL_ANALOG_CTRL8, 0x0033587C);
	sctrl_analog_set(SCTRL_ANALOG_CTRL9, 0x82204607);
	sctrl_analog_set(SCTRL_ANALOG_CTRL10, 0x80801027);
#elif (defined(CONFIG_SOC_BK7271))
	sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x12A00000);
	sctrl_analog_set(SCTRL_ANALOG_CTRL6, 0x0009A7F0);
	sctrl_analog_set(SCTRL_ANALOG_CTRL8_REAL, 0x84200461);
	sctrl_analog_set(SCTRL_ANALOG_CTRL9_REAL, 0x3CEF9A5F);
	sctrl_analog_set(SCTRL_ANALOG_CTRL8, 0x000A0877);
	sctrl_analog_set(SCTRL_ANALOG_CTRL9, 0x84400000);
	sctrl_analog_set(SCTRL_ANALOG_CTRL10, 0x80800200);
	sctrl_analog_set(SCTRL_ANALOG_CTRL13, 0x21084000);
	sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, 0x00B09350);

	//rtc init
	addPMU_Reg0xd = 681800000;      //from 2000-01-01. date(2000-01-01)=730486
	addPMU_Reg0xf = 0x1;
#endif // (CONFIG_SOC_BK7251)
#endif // CONFIG_AUDIO

	sctrl_mac_ahb_slave_clock_enable();
	//rf_ps_init();
}

#endif
void sctrl_exit(void)
{
	sddev_unregister_dev(DD_DEV_TYPE_SCTRL);
}

void sctrl_modem_core_reset(void)
{
	sctrl_ctrl(CMD_SCTRL_MODEM_CORE_RESET, 0);
}

void sctrl_sub_reset(void)
{
	sctrl_ctrl(CMD_SCTRL_MPIF_CLK_INVERT, 0);
	sctrl_ctrl(CMD_SCTRL_MODEM_CORE_RESET, 0);
	sctrl_ctrl(CMD_SCTRL_MODEM_SUBCHIP_RESET, 0);
	sctrl_ctrl(CMD_SCTRL_MAC_SUBSYS_RESET, 0);
	sctrl_ctrl(CMD_SCTRL_USB_SUBSYS_RESET, 0);
}

void ps_delay(volatile UINT16 times)
{
	UINT32	delay = times;
	while (delay--) ;
}

void sctrl_subsys_power(UINT32 cmd)
{
	UINT32 reg = 0;
	UINT32 reg_val;
	UINT32 reg_word = 0;

	switch (cmd) {
	case CMD_SCTRL_DSP_POWERDOWN:
		reg = SCTRL_DSP_PWR;
		reg_word = DSP_PWD;
		break;

	case CMD_SCTRL_DSP_POWERUP:
		reg = SCTRL_DSP_PWR;
		reg_word = DSP_PWU;
		break;

#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_LBUS_POWERDOWN:
		reg = SCTRL_PMU_STATUS;
		reg_word = SYNC_BUS_PWD;
		break;

	case CMD_SCTRL_LBUS_POWERUP:
		reg = SCTRL_PMU_STATUS;
		reg_word = SYNC_BUS_PWU;
		break;

	case CMD_SCTRL_BT_POWERDOWN:
		reg = SCTRL_BT_PWR;
		reg_word = BT_PWD;
		break;

	case CMD_SCTRL_BT_POWERUP:
		reg = SCTRL_BT_PWR;
		reg_word = BT_PWU;
		break;
#else

#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	case CMD_SCTRL_USB_POWERDOWN:
		reg = SCTRL_USB_PWR;
		reg_val = REG_READ(SCTRL_USB_PWR);
		reg_val &= ~(USB_PWD_MASK << USB_PWD_POSI);
		reg_val |= USB_PWD << USB_PWD_POSI;
		reg_word = reg_val;
		break;

	case CMD_SCTRL_USB_POWERUP:
		reg = SCTRL_USB_PWR;
		reg_val = REG_READ(SCTRL_USB_PWR);
		reg_val &= ~(USB_PWD_MASK << USB_PWD_POSI);
		reg_val |= USB_PWU << USB_PWD_POSI;
		reg_word = reg_val;
		break;
#endif

	case CMD_SCTRL_MAC_POWERDOWN:
		reg = SCTRL_PWR_MAC_MODEM;
		reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
		reg_val &= ~(MAC_PWD_MASK << MAC_PWD_POSI);
		reg_val |= MAC_PWD << MAC_PWD_POSI;
		reg_word = reg_val;
		break;

	case CMD_SCTRL_MAC_POWERUP:
		reg = SCTRL_PWR_MAC_MODEM;
		reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
		reg_val &= ~(MAC_PWD_MASK << MAC_PWD_POSI);
		reg_val |= MAC_PWU << MAC_PWD_POSI;
		reg_word = reg_val;
		break;
#endif
	case CMD_SCTRL_MODEM_POWERDOWN:
		reg = SCTRL_PWR_MAC_MODEM;
		reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
		reg_val &= ~(MODEM_PWD_MASK << MODEM_PWD_POSI);
		reg_val |= MODEM_PWD << MODEM_PWD_POSI;
		reg_word = reg_val;
		break;

#if (!defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_BLE_POWERDOWN:
		reg = SCTRL_USB_PWR;
		reg_val = REG_READ(SCTRL_USB_PWR);
		reg_val &= ~(BLE_PWD_MASK << BLE_PWD_POSI);
		reg_val |= BLE_PWD << BLE_PWD_POSI;
		reg_word = reg_val;
		break;
#endif

	case CMD_SCTRL_MODEM_POWERUP:
		reg = SCTRL_PWR_MAC_MODEM;
		reg_val = REG_READ(SCTRL_PWR_MAC_MODEM);
		reg_val &= ~(MODEM_PWD_MASK << MODEM_PWD_POSI);
		reg_val |= MODEM_PWU << MODEM_PWD_POSI;
		reg_word = reg_val;
		break;

#if (!defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_BLE_POWERUP:
		reg = SCTRL_USB_PWR;
		reg_val = REG_READ(SCTRL_USB_PWR);
		reg_val &= ~(BLE_PWD_MASK << BLE_PWD_POSI);
		reg_val |= BLE_PWU << BLE_PWD_POSI;
		reg_word = reg_val;
		break;
#endif

	default:
		break;
	}

	if (reg)
		REG_WRITE(reg, reg_word);
}

void sctrl_subsys_reset(UINT32 cmd)
{
	UINT32 reg = 0;
	UINT32 reset_word = 0;

	switch (cmd) {
	case CMD_SCTRL_MODEM_SUBCHIP_RESET:
		reg = SCTRL_MODEM_SUBCHIP_RESET_REQ;
		reset_word = MODEM_SUBCHIP_RESET_WORD;
		break;

	case CMD_SCTRL_MAC_SUBSYS_RESET:
		reg = SCTRL_MAC_SUBSYS_RESET_REQ;
		reset_word = MAC_SUBSYS_RESET_WORD;
		break;

#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_USB_SUBSYS_RESET:
		reg = SCTRL_USB_SUBSYS_RESET_REQ;
		reset_word = USB_SUBSYS_RESET_WORD;
		break;

	case CMD_SCTRL_DSP_SUBSYS_RESET:
		reg = SCTRL_DSP_SUBSYS_RESET_REQ;
		reset_word = DSP_SUBSYS_RESET_WORD;
		break;
#endif

	default:
		break;
	}

	if (reg) {
		REG_WRITE(reg, reset_word);
		delay(10);
		REG_WRITE(reg, 0);
	}

	return;
}

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
void sctrl_fix_dpll_div(void)
{
	volatile INT32   i;
	uint32 reg;
	uint32 cpu_clock;

	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

	reg = REG_READ(SCTRL_CONTROL);
	cpu_clock = reg & 0xFF;
	reg = (reg & 0xFFFFFF00) | 0x52;
	REG_WRITE(SCTRL_CONTROL, reg);

	for (i = 0; i < 100; i ++);

	REG_WRITE(SCTRL_MODEM_SUBCHIP_RESET_REQ, MODEM_SUBCHIP_RESET_WORD);
	REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) | (1 << 14));

	for (i = 0; i < 100; i ++);

	REG_WRITE(SCTRL_MODEM_SUBCHIP_RESET_REQ, 0);
	REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) & ~(1 << 14));

	for (i = 0; i < 100; i ++);

	reg = REG_READ(SCTRL_CONTROL);
	reg = (reg & 0xFFFFFF00) | cpu_clock;
	REG_WRITE(SCTRL_CONTROL, reg);

	for (i = 0; i < 100; i ++);

	GLOBAL_INT_RESTORE();
}
#endif

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
void sctrl_mdm_reset(void)
{
	volatile INT32 i;
	GLOBAL_INT_DECLARATION();

	SCTRL_LOGI("sctrl_mdm_reset\r\n");

	// Disable the interrupts
	GLOBAL_INT_DISABLE();

	if (1) {
		/* MAC reset */
		REG_WRITE(SCTRL_MODEM_SUBCHIP_RESET_REQ, MODEM_SUBCHIP_RESET_WORD);
		REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) | DPLL_CLKDIV_RESET_BIT);

		for (i = 0; i < 100; i++);

		REG_WRITE(SCTRL_MODEM_SUBCHIP_RESET_REQ, 0);
		REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) & ~DPLL_CLKDIV_RESET_BIT);

		for (i = 0; i < 100; i++);
	} else {
		/* Modem pwd */
		sctrl_ctrl(CMD_SCTRL_MODEM_POWERDOWN, NULL);
		REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) | DPLL_CLKDIV_RESET_BIT);

		for (i = 0; i < 100; i++);

		/* Modem pwu */
		sctrl_ctrl(CMD_SCTRL_MODEM_POWERUP, NULL);
		REG_WRITE(SCTRL_CONTROL, REG_READ(SCTRL_CONTROL) & ~DPLL_CLKDIV_RESET_BIT);

		for (i = 0; i < 100; i++);
	}

	if (s_wifi_phy_wakeup_rf_reinit_cb) {
		s_wifi_phy_wakeup_rf_reinit_cb();
	}
	if (s_wifi_phy_wakeup_wifi_reinit_cb) {
		s_wifi_phy_wakeup_wifi_reinit_cb();
	}

	// Restore the interrupts
	GLOBAL_INT_RESTORE();
}
#endif


#if defined(CONFIG_DEEP_PS)
#if PS_SUPPORT_MANUAL_SLEEP
void sctrl_enter_rtos_idle_sleep(PS_DEEP_CTRL_PARAM deep_param)
{
	DD_HANDLE flash_hdl;
	UINT32 status;
	UINT32 param;
	UINT32 reg;

	if (4 == flash_get_line_mode())
		flash_set_line_mode(2);

#if (!(defined(CONFIG_SOC_BK7256XX) && defined(CONFIG_SYS_CPU1)))
	uart_wait_tx_over();
#endif

	/* close all peri clock*/
	ps_saves[1].peri_clk_cfg = REG_READ(ICU_PERI_CLK_PWD);
	REG_WRITE(ICU_PERI_CLK_PWD, 0xfffff);

	uart2_exit();
#if defined(CONFIG_UART1)
	uart1_exit();
#endif

	reg = REG_READ(SCTRL_ROSC_TIMER);
	reg &= ~(ROSC_TIMER_ENABLE_BIT);
	REG_WRITE(SCTRL_ROSC_TIMER, reg);

	REG_WRITE(SCTRL_GPIO_WAKEUP_EN, 0x0);

	REG_WRITE(SCTRL_BLOCK_EN_MUX, 0x0);

	param = LPO_SELECT_ROSC;
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_LOW_PWR_CLK, &param);

	/* close all peri int*/
	ps_saves[1].int_enable_cfg = REG_READ(ICU_R_INT_EN);
	REG_WRITE(ICU_R_INT_EN, 0);

	/*Enable BK7011:rc_en,ch0_en*/
	rc_cntl_stat_set(0x0);

	/* MAC pwd*/
	REG_WRITE(SCTRL_PWR_MAC_MODEM, MAC_PWD << MAC_PWD_POSI);

	sctrl_mac_ahb_slave_clock_disable();

	/* Mac Subsystem clock 480m disable*/
	reg = REG_READ(SCTRL_CONTROL);
	REG_WRITE(SCTRL_CONTROL, reg | MAC_CLK480M_PWD_BIT);

	/* Modem pwd*/
	REG_WRITE(SCTRL_PWR_MAC_MODEM, MODEM_PWD << MODEM_PWD_POSI);

	/* Modem AHB clock disable*/
#if (defined(CONFIG_SOC_BK7271))
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~PHY_HCLK_EN_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	reg &= ~PHY_HCLK_EN_BIT;
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif

	/* Modem Subsystem clock 480m disable*/
	reg = REG_READ(SCTRL_CONTROL);
	REG_WRITE(SCTRL_CONTROL, reg | MODEM_CLK480M_PWD_BIT);

	/* Flash 26MHz clock select dco clock*/
	flash_hdl = ddev_open(DD_DEV_TYPE_FLASH, &status, 0);
	BK_ASSERT(DD_HANDLE_UNVALID != flash_hdl); /* ASSERT VERIFIED */
	ddev_control(flash_hdl, CMD_FLASH_SET_DCO, 0);

	/* MCLK(main clock) select:dco*/ /* MCLK division*/
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);
	reg &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
	REG_WRITE(SCTRL_CONTROL, reg);

	REG_WRITE(SCTRL_ROSC_CAL, 0x35);
	REG_WRITE(SCTRL_ROSC_CAL, 0x37);

	ps_delay(10);

	/*close 32K Rosc calib*/
	REG_WRITE(SCTRL_ROSC_CAL, 0x36);

	ps_block_value = REG_READ(SCTRL_BLOCK_EN_CFG) & BLOCK_EN_VALID_MASK;
	/* ALL disable*/
	reg = 0x0;
	reg &= ~(BLOCK_EN_WORD_MASK << BLOCK_EN_WORD_POSI);
	reg = reg | (BLOCK_EN_WORD_PWD << BLOCK_EN_WORD_POSI);
	reg &= ~(BLOCK_EN_VALID_MASK);
	REG_WRITE(SCTRL_BLOCK_EN_CFG, reg);

	/* center bias power down*/
	reg = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
	reg &= (~CENTRAL_BAIS_ENABLE_BIT);
	sctrl_analog_set(SCTRL_ANALOG_CTRL2, reg);

	reg = 0xFFFFFFFF;
	REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS, reg);

	if (deep_param.deep_wkway == PS_DEEP_WAKEUP_RTC
		&& deep_param.param != 0xffffffff) {
		reg = REG_READ(SCTRL_ROSC_TIMER);
		reg |= ROSC_TIMER_INT_STATUS_BIT;
		REG_WRITE(SCTRL_ROSC_TIMER, reg);
		reg = REG_READ(SCTRL_ROSC_TIMER);
		reg &= ~(ROSC_TIMER_PERIOD_MASK << ROSC_TIMER_PERIOD_POSI);
		reg |= (deep_param.param << ROSC_TIMER_PERIOD_POSI);
		REG_WRITE(SCTRL_ROSC_TIMER, reg);
		reg = REG_READ(SCTRL_ROSC_TIMER);
		reg |= ROSC_TIMER_ENABLE_BIT;
		REG_WRITE(SCTRL_ROSC_TIMER, reg);

		reg = 0x0;
		reg &= ~(BLOCK_EN_WORD_MASK << BLOCK_EN_WORD_POSI);
		reg = reg | (BLOCK_EN_WORD_PWD << BLOCK_EN_WORD_POSI);
		reg |= (BLK_EN_ANALOG_SYS_LDO | BLK_EN_DIGITAL_CORE);
		REG_WRITE(SCTRL_BLOCK_EN_CFG, reg);
		reg = REG_READ(SCTRL_BLOCK_EN_MUX);
		reg &= (~0x1FF);
		reg |= (0x1 << 8);
		REG_WRITE(SCTRL_BLOCK_EN_MUX, reg);
	} else if (deep_param.deep_wkway == PS_DEEP_WAKEUP_GPIO) {
		reg = 0xFFFFFFFF;
		REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS, reg);
		reg = deep_param.gpio_lv;
		REG_WRITE(SCTRL_GPIO_WAKEUP_TYPE, reg);
		reg = deep_param.param;
		REG_WRITE(SCTRL_GPIO_WAKEUP_EN, reg);

	}

	ps_delay(10);
	/* arm clock disable */
	reg = REG_READ(SCTRL_SLEEP);
	reg &= ~(SLEEP_MODE_MASK << SLEEP_MODE_POSI);
	reg = reg | SLEEP_MODE_CFG_LOW_VOL_WORD;
	REG_WRITE(SCTRL_SLEEP, reg);
	delay(5);
}

void sctrl_exit_rtos_idle_sleep(void)
{
	UINT32 reg;

	/* center bias power on*/
	reg = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
	reg |= CENTRAL_BAIS_ENABLE_BIT;
	sctrl_analog_set(SCTRL_ANALOG_CTRL2, reg);

	/*dpll(480m) & 26m xtal & xtal2rf enable*/
	reg = REG_READ(SCTRL_BLOCK_EN_MUX);
	reg &= ~(0x1 << 4);
	REG_WRITE(SCTRL_BLOCK_EN_MUX, reg);
	reg = REG_READ(SCTRL_BLOCK_EN_CFG);
	reg &= ~(BLOCK_EN_WORD_MASK << BLOCK_EN_WORD_POSI);
	reg |= (BLOCK_EN_WORD_PWD << BLOCK_EN_WORD_POSI);
	reg |= ps_block_value;
	REG_WRITE(SCTRL_BLOCK_EN_CFG, reg);

	delay(20);  //delay 1.1ms for 26MHz DCO clock. need change for other dco clock frequency
	sctrl_cali_dpll(1);
	/* dpll division reset release*/
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~(0x1 << 14);
	REG_WRITE(SCTRL_CONTROL, reg);

	/* MCLK(main clock) select:dpll*//* MCLK division*/
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
	reg &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);
	reg |= ((MCLK_DIV_7 & MCLK_DIV_MASK) << MCLK_DIV_POSI);
	reg |= ((MCLK_FIELD_DPLL & MCLK_MUX_MASK) << MCLK_MUX_POSI);
	REG_WRITE(SCTRL_CONTROL, reg);

	/*open 32K Rosc calib*/
	REG_WRITE(SCTRL_ROSC_CAL, 0x35);
	REG_WRITE(SCTRL_ROSC_CAL, 0x37);

	sctrl_mac_ahb_slave_clock_enable();

	/* Mac Subsystem clock 480m enable*/
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~MAC_CLK480M_PWD_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);

	/* Modem AHB clock enable*/
#if (defined(CONFIG_SOC_BK7271))
	reg = REG_READ(SCTRL_CONTROL);
	REG_WRITE(SCTRL_CONTROL, reg | PHY_HCLK_EN_BIT);
#else
	reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
	REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg | PHY_HCLK_EN_BIT);
#endif

	/* Modem Subsystem clock 480m enable*/
	reg = REG_READ(SCTRL_CONTROL);
	reg &= ~MODEM_CLK480M_PWD_BIT;
	REG_WRITE(SCTRL_CONTROL, reg);

	/* Modem pwd*/
	reg = REG_READ(SCTRL_PWR_MAC_MODEM);
	reg &= ~(MODEM_PWD_MASK << MODEM_PWD_POSI);
	reg = reg | (0 << MODEM_PWD_POSI);
	REG_WRITE(SCTRL_PWR_MAC_MODEM, reg);

	/*Enable BK7011:rc_en,ch0_en*/
	rc_cntl_stat_set(0x09);

	/* recovery periphral unit clock config*/
	REG_WRITE(ICU_PERI_CLK_PWD, ps_saves[1].peri_clk_cfg);

	/* recovery periphral unit int config*/
	REG_WRITE(ICU_R_INT_EN, ps_saves[1].int_enable_cfg);

	/*open 32K Rosc calib*/
	REG_WRITE(SCTRL_ROSC_CAL, 0x35);
	REG_WRITE(SCTRL_ROSC_CAL, 0x37);

	uart2_init();
#if defined(CONFIG_UART1)
	uart1_init();
#endif

	if (4 == flash_get_line_mode())
		flash_set_line_mode(4);

	SCTRL_LOGI("idle wake up!\r\n");
}
#endif



int bk_init_deep_wakeup_gpio_status(void)
{
	gpio_0_31_status = REG_READ(GPIO_WAKEUP_INT_STATUS_BAK);
#if(!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX))
	gpio_32_39_status = REG_READ(GPIO_WAKEUP_INT_STATUS1_BAK);
#endif
	REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS, 0xFFFFFFFF);
	#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS1, 0xFFFFFFFF);
	#endif
	return 0;
}
int bk_misc_wakeup_get_gpio_num(void)
{
	int wakeup_gpio_num = -1;

	REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS, 0xFFFFFFFF);
	#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	REG_WRITE(SCTRL_GPIO_WAKEUP_INT_STATUS1, 0xFFFFFFFF);
	#endif

	if((0 == gpio_0_31_status)
	#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	&& (0 == gpio_32_39_status)
	#endif
	)
	{
		wakeup_gpio_num = -1;
		return wakeup_gpio_num;
	}

	if(gpio_0_31_status)
	{
		for(int i=0;i<32;i++)
		{
			if(gpio_0_31_status&0x01)
			{
				wakeup_gpio_num = i;
				break;
			}

			gpio_0_31_status = gpio_0_31_status >> 1;
		}
	}
	#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	else if(gpio_32_39_status)
	{
		for(int i=32;i<40;i++)
		{
			if(gpio_32_39_status&0x01)
			{
				wakeup_gpio_num = i;
				break;
			}

			gpio_32_39_status = gpio_32_39_status >> 1;
		}
	}
	#endif

	return wakeup_gpio_num;
}

UINT32 sctrl_get_deep_sleep_wake_soure(void)
{
	RESET_SOURCE_STATUS waked_source = 0;

#if (!defined(CONFIG_SOC_BK7271))
	if (REG_READ(SCTRL_ROSC_TIMER) & ROSC_TIMER_INT_STATUS_BIT)
	{
		waked_source = RESET_SOURCE_DEEPPS_RTC;
	}
	else if(gpio_0_31_status
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	|| gpio_32_39_status
#endif
			)
		waked_source = RESET_SOURCE_DEEPPS_GPIO;
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	else if (REG_READ(SCTRL_USB_PLUG_WAKEUP) & (USB_PLUG_IN_INT_BIT | USB_PLUG_OUT_INT_BIT))
		waked_source = RESET_SOURCE_DEEPPS_USB;
#endif
#endif

	return waked_source;
}

void sctrl_set_deep_sleep_gpio_floating_map(UINT32 gpio_floating_map)
{
	deep_sleep_gpio_floating_map = gpio_floating_map;
}

UINT32 sctrl_get_deep_sleep_gpio_floating_map(void)
{
	return deep_sleep_gpio_floating_map;
}

#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX))
void sctrl_set_deep_sleep_gpio_last_floating_map(UINT32 gpio_last_floating_map)
{
	deep_sleep_gpio_last_floating_map = gpio_last_floating_map;
}

UINT32 sctrl_get_deep_sleep_gpio_last_floating_map(void)
{
	return deep_sleep_gpio_last_floating_map;
}
#endif

#endif

#if (!defined(CONFIG_SOC_BK7231)) && (!defined(CONFIG_SOC_BK7271))
static int sctrl_read_efuse(void *param)
{
	UINT32 reg, ret = -1;
	EFUSE_OPER_PTR efuse;
	efuse = (EFUSE_OPER_PTR)param;

	if (efuse) {
		reg = REG_READ(SCTRL_EFUSE_CTRL);
		reg &= ~(EFUSE_OPER_ADDR_MASK << EFUSE_OPER_ADDR_POSI);
		reg &= ~(EFUSE_OPER_DIR);

		reg |= ((efuse->addr & EFUSE_OPER_ADDR_MASK) << EFUSE_OPER_ADDR_POSI);
		reg |= (EFUSE_OPER_EN);
		REG_WRITE(SCTRL_EFUSE_CTRL, reg);

		do {
			reg = REG_READ(SCTRL_EFUSE_CTRL);
		} while (reg & EFUSE_OPER_EN);

		reg = REG_READ(SCTRL_EFUSE_OPTR);
		if (reg & EFUSE_OPER_RD_DATA_VALID) {
			efuse->data = ((reg >> EFUSE_OPER_RD_DATA_POSI) & EFUSE_OPER_RD_DATA_MASK);
			ret = 0;
		} else
			efuse->data = 0xFF;
	}
	return ret;
}

static int check_efuse_can_write(UINT8 new_byte, UINT8 old_byte)
{
	if (new_byte == old_byte) {
		// no need to read
		return 1;
	}

	for (int i = 0; i < 8; i++) {
		UINT8 old_bit = ((old_byte >> i) & 0x01);
		UINT8 new_bit = ((new_byte >> i) & 0x01);

		if ((old_bit) && (!new_bit)) {
			// can not change old from 1 to 0
			return 0;
		}
	}

	return 2;
}

static int sctrl_write_efuse(void *param)
{
	UINT32 reg, ret = -1;
	EFUSE_OPER_ST *efuse, efuse_bak;

#if (defined(CONFIG_SOC_BK7251))
	SCTRL_LOGI("BK7251 cannot write efuse via register\r\n");
	goto wr_exit;
#endif

	efuse = (EFUSE_OPER_PTR)param;
	if (efuse) {
		efuse_bak.addr = efuse->addr;
		efuse_bak.data = efuse->data;
		if (sctrl_read_efuse(&efuse_bak) == 0) {
			//read before write, ensure this byte and this bit no wrote
			ret = check_efuse_can_write(efuse->data, efuse_bak.data);
			if (ret == 0) {
				ret = -1;
				goto wr_exit;
			} else if (ret == 1) {
				ret = 0;
				goto wr_exit;
			}
		}

		// enable vdd2.5v first
		reg = REG_READ(SCTRL_CONTROL);
		reg |= EFUSE_VDD25_EN;
		REG_WRITE(SCTRL_CONTROL, reg);

		reg = REG_READ(SCTRL_EFUSE_CTRL);
		reg &= ~(EFUSE_OPER_ADDR_MASK << EFUSE_OPER_ADDR_POSI);
		reg &= ~(EFUSE_OPER_WR_DATA_MASK << EFUSE_OPER_WR_DATA_POSI);

		reg |= EFUSE_OPER_DIR;
		reg |= ((efuse->addr & EFUSE_OPER_ADDR_MASK) << EFUSE_OPER_ADDR_POSI);
		reg |= ((efuse->data & EFUSE_OPER_WR_DATA_MASK) << EFUSE_OPER_WR_DATA_POSI);
		reg |= EFUSE_OPER_EN;
		REG_WRITE(SCTRL_EFUSE_CTRL, reg);

		do {
			reg = REG_READ(SCTRL_EFUSE_CTRL);
		} while (reg & EFUSE_OPER_EN);

		// disable vdd2.5v at last
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~EFUSE_VDD25_EN;
		REG_WRITE(SCTRL_CONTROL, reg);

		// check, so read
		reg = efuse->data;
		efuse->data = 0;
		if (sctrl_read_efuse(param) == 0) {
			if (((UINT8)reg) == efuse->data)
				ret = 0;
		}
	}

wr_exit:
	return ret;
}


#endif // (!CONFIG_SOC_BK7231)

#if defined(CONFIG_USB_CHARGE)
#if (defined(CONFIG_SOC_BK7271))
#define CHARGER_CTRL_0 SCTRL_ANALOG_CTRL7
#define CHARGER_CTRL_1 SCTRL_ANALOG_CTRL8_REAL
#else
#define CHARGER_CTRL_0 SCTRL_ANALOG_CTRL3
#define CHARGER_CTRL_1 SCTRL_ANALOG_CTRL4
#endif

#if (defined(CONFIG_SOC_BK7251))
UINT32 usb_charge_oper_val(UINT32 elect)
{
	if (elect >= 450) {
		/*EXTERNAL CC elect*/
		if (elect > 750)
			elect = 750;
		return (elect - 450) / 20;
	} else {
		/*INTERNAL CC elect*/
		if (elect > 250)
			elect = 250;
		else if (elect < 20)
			elect = 20;

		if (elect >= 100 && elect <= 250)
			return (elect - 100) / 10 + 16;
		else
			return (elect - 20) / 10;
	}
}

UINT32 usb_power_is_pluged(void)
{
	UINT32 reg;
	reg = sctrl_analog_get(SCTRL_CHARGE_STATUS);
	return (reg & (1 << 21));
}

void charger_module_enable(UINT32 enable)
{
	sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 12)) | (!!enable << 12));
}

void charger_vlcf_calibration(UINT32 type)
{

	if (type == 0) {
		/*Internal hardware calibration*/
		/*vlcf calibration*/
		/*>>> Added 5V voltage on Vusb*/
		/*>>> Added 4.2V voltage on vbattery*/
		/*>>> Set pwd=0*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 12));
		/*charge mode select*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
		/*calEn*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 21));
		/*softCalen*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 20));
		/*vlcfSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 18));
		/*IcalSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 17));
		/*vcvSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 16));
		/*vlcf_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 21));
		/*vlcf_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 21));
		/*Wait for at least 4 clock cycles*/
		delay_ms(1);
		/*vlcf_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 21));
		/*Waiting for 1ms, calibration finished*/
		delay_ms(1);
		/*Read the value vcal<5:0>, Recorded*/
		calib_charger[0] = (sctrl_analog_get(SCTRL_CHARGE_STATUS) >> CHARGE_VCAL_POS) & CHARGE_VCAL_MASK ;
	}
}

void charger_icp_calibration(UINT32 type)
{

	if (type == 0) {
		/*Internal hardware calibration*/
		/*Icp calibration*/
		/*>>> Added parallel 60ohm resistor and 100nF capacitor from vbattery to ground.(Removed the external 4.2V)*/
		/*>>> Set pwd=0*/
		/*>>> Porb=0*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 12));
		/*Icp=60mA*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
											  & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (0x4 << CHARGE_LCP_POS));
		/*calEn*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 21));
		/*softCalen*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 20));
		/*vlcfSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 18));
		/*vcal<5:0>=previous vlcf calibration value*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
											  & ~(CHARGE_VCAL_MASK << 0)) | (calib_charger[0] << 0));
		/*IcalSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 17));
		/*vcvSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 16));
		/*Ical_trig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 20));
		/*Ical_trig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 20));
		/*Wait for at least 4 clock cycles*/
		delay_ms(1);
		/*Ical_trig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 20));
		/*Waiting for 1ms, calibration finished*/
		delay_ms(1);
		/*Read the value Ical<4:0>, Recorded*/
		calib_charger[1] = (sctrl_analog_get(SCTRL_CHARGE_STATUS) >> CHARGE_LCAL_POS) & CHARGE_LCAL_MASK ;
	}
}

void charger_vcv_calibration(UINT32 type)
{

	if (type == 0) {
		/*Internal hardware calibration*/
		/*vcv calibration*/
		/*>>> Added 5V voltage on Vusb*/
		/*>>> Added 4.2V voltage on vbattery*/
		/*>>> Set pwd=0*/
		/*>>> Porb=0*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 12));
		/*charge mode select*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
		/*Icp=60mA*/
		/*sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
		*                                      & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (0x4 << CHARGE_LCP_POS));
		**/
		/*calEn*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 21));
		/*softCalen*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 20));
		/*vlcfSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 18));
		/*vcal<5:0>=previous vlcf calibration value*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
											  & ~(CHARGE_VCAL_MASK << 0)) | (calib_charger[0] << 0));
		/*IcalSel*/
		//sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 17));
		/*Ical<4:0>=previous Ical calibration value*/
		/*sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)\
		*                                     & ~(CHARGE_LCAL_MASK << 27)) | (calib_charger[1] << 27));
		*/
		/*vcvSel*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 16));
		/*vcv_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 19));
		/*vcv_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 19));
		/*Wait for at least 4 clock cycles*/
		delay_ms(1);
		/*vcv_caltrig*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 19));
		/*Waiting for 1ms, calibration finished*/
		delay_ms(1);
		/*Read the value vcvcal<4:0>, Recorded*/
		calib_charger[2] = (sctrl_analog_get(SCTRL_CHARGE_STATUS) >> CHARGE_VCVCAL_POS) & CHARGE_VCVCAL_MASK ;
	}
}

void charger_calib_get(UINT8 value[])
{
	value[0] = calib_charger[0];
	value[1] = calib_charger[1];
	value[2] = calib_charger[2];
	return;
}

void charger_calib_set(UINT8 value[])
{
	if (!value[0] || !value[1] || !value[2])
		return;

	calib_charger[0] = value[0];
	calib_charger[1] = value[1];
	calib_charger[2] = value[2];
	return;
}

UINT32 charger_is_full(void)
{
	UINT32 reg;
	reg = sctrl_analog_get(SCTRL_CHARGE_STATUS);
	return (reg & (1 << 20));
}

void charger_start(void *param)
{
	UINT32 charge_cal_type ;
	CHARGE_OPER_ST *chrg;

	chrg = (CHARGE_OPER_ST *)param;

	if (! usb_power_is_pluged()) {
		SCTRL_LOGI("%s: not pluged\r\n", __FUNCTION__);
		return;
	}

	charger_calib_set(chrg->cal);
	SCTRL_LOGI("%s: %d %d %d %x %x %x\r\n", __FUNCTION__, chrg->type, chrg->step, chrg->elect,
			  calib_charger[0], calib_charger[1], calib_charger[2]);
	if (chrg->step == STEP_START) {
		if (chrg->type == INTERNAL_HW_MODE || chrg->type == INTERNAL_SW_MODE) {
			/*Internal*/
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 21));

			if (chrg->type == INTERNAL_HW_MODE) {
				/*Internal ,hw control*/
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 19));
			} else if (chrg->type == INTERNAL_SW_MODE) {
				/*Internal ,sw control*/
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			}

			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 28));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 27));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 12));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 13)); //vcvcalEn_spilv
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 11));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 18));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 17));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 16));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
												  & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (0x1f << CHARGE_LCP_POS));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
												  & ~(CHARGE_VCAL_MASK << 0)) | (calib_charger[0] << 0));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)\
												  & ~(CHARGE_LCAL_MASK << 27)) | (calib_charger[1] << 27));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)\
												  & ~(CHARGE_LCAL_MASK << 22)) | (calib_charger[2] << 22));

			if (chrg->type == INTERNAL_HW_MODE) {
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
													  & ~(CHARGE_LC2CVDLYLV_MASK << CHARGE_LC2CVDLYLV_POS)) | (4 << CHARGE_LC2CVDLYLV_POS));
			} else if (chrg->type == INTERNAL_SW_MODE) {
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
													  & ~(CHARGE_LC2CVDLYLV_MASK << CHARGE_LC2CVDLYLV_POS)));
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 22));
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
													  & ~(CHARGE_VLCSWLV_MASK << CHARGE_VLCSWLV_POS)) | (8 << CHARGE_VLCSWLV_POS));
			}

		} else if (chrg->type == EXTERNAL_HW_MODE || chrg->type == EXTERNAL_SW_MODE) {
			/*External*/
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 21));

			if (chrg->type == EXTERNAL_HW_MODE) {
				/*External ,hw control*/
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 19));
			} else if (chrg->type == EXTERNAL_SW_MODE) {
				/*External ,sw control*/
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			}

			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 28));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 27));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 12));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 13)); //vcvcalEn_spilv

			if (chrg->type == EXTERNAL_HW_MODE)
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 11));
			else if (chrg->type == EXTERNAL_SW_MODE) {
				//sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4)& ~(1 << 11));
				sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 11));
			}

			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 18));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 17));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 16));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
												  & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (0x1f << CHARGE_LCP_POS));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
												  & ~(CHARGE_VCAL_MASK << 0)) | (calib_charger[0] << 0));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)\
												  & ~(CHARGE_LCAL_MASK << 27)) | (calib_charger[1] << 27));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)\
												  & ~(CHARGE_LCAL_MASK << 22)) | (calib_charger[2] << 22));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 28));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
												  & ~(CHARGE_LC2CVDLYLV_MASK << CHARGE_LC2CVDLYLV_POS)));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 15));

			if (chrg->type == EXTERNAL_HW_MODE) {
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) | (1 << 22));
				sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
													  & ~(CHARGE_VLCSWLV_MASK << CHARGE_VLCSWLV_POS)) | (0xf << CHARGE_VLCSWLV_POS));
			}
		}
	} else if (chrg->step == STEP_TRICKLE) {
		/*trickle charge*/
		if (chrg->type == INTERNAL_SW_MODE || chrg->type == EXTERNAL_SW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)  \
												  & ~(CHARGE_MANMODE_MASK << CHARGE_MANMODE_POS)) | (4 << CHARGE_MANMODE_POS));
		}
	} else if (chrg->step == STEP_EXTER_CC) {
		/*External CC charge*/
		if (chrg->type == EXTERNAL_SW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
												  & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (usb_charge_oper_val(chrg->elect) << CHARGE_LCP_POS));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)  \
												  & ~(CHARGE_MANMODE_MASK << CHARGE_MANMODE_POS)) | (2 << CHARGE_MANMODE_POS));
		}
	} else if (chrg->step == STEP_INTER_CC) {
		/*Internal CC charge*/
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3)\
											  & ~(CHARGE_LCP_MASK << CHARGE_LCP_POS)) | (usb_charge_oper_val(chrg->elect) << CHARGE_LCP_POS));

		if (chrg->type == INTERNAL_SW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)  \
												  & ~(CHARGE_MANMODE_MASK << CHARGE_MANMODE_POS)) | (2 << CHARGE_MANMODE_POS));
		}

		if (chrg->type == EXTERNAL_SW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)  \
												  & ~(CHARGE_MANMODE_MASK << CHARGE_MANMODE_POS)) | (2 << CHARGE_MANMODE_POS));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 15));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 11));
		}
	} else if (chrg->step == STEP_INTER_CV) {
		/*Internal CV charge*/
		if (chrg->type == INTERNAL_SW_MODE || chrg->type == EXTERNAL_SW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) | (1 << 19));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, (sctrl_analog_get(SCTRL_ANALOG_CTRL4)  \
												  & ~(CHARGE_MANMODE_MASK << CHARGE_MANMODE_POS)) | (1 << CHARGE_MANMODE_POS));
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 15));
		}

		if (chrg->type == EXTERNAL_SW_MODE)
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 11));

		if (chrg->type == EXTERNAL_HW_MODE) {
			sctrl_analog_set(SCTRL_ANALOG_CTRL4, sctrl_analog_get(SCTRL_ANALOG_CTRL4) & ~(1 << 11));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, sctrl_analog_get(SCTRL_ANALOG_CTRL3) & ~(1 << 22));
			sctrl_analog_set(SCTRL_ANALOG_CTRL3, (sctrl_analog_get(SCTRL_ANALOG_CTRL3) \
												  & ~(CHARGE_VLCSWLV_MASK << CHARGE_VLCSWLV_POS)));
		}
	}

}

void charger_stop(UINT32 type)
{
	SCTRL_LOGI("%s\r\n", __FUNCTION__);
	charger_module_enable(0);
}
#endif
#endif

UINT32 sctrl_ctrl(UINT32 cmd, void *param)
{
	UINT32 ret;
	UINT32 reg;
	GLOBAL_INT_DECLARATION();

	ret = SCTRL_SUCCESS;
	GLOBAL_INT_DISABLE();
	switch (cmd) {
	case CMD_SCTRL_NORMAL_SLEEP:
		//sctrl_hw_sleep(*(UINT32 *)param);
		break;

	case CMD_SCTRL_NORMAL_WAKEUP:
		//sctrl_hw_wakeup();
		break;

#if defined(CONFIG_DEEP_PS)
#if PS_SUPPORT_MANUAL_SLEEP
	case CMD_SCTRL_RTOS_IDLE_SLEEP:
		sctrl_enter_rtos_idle_sleep(*(PS_DEEP_CTRL_PARAM *)param);
		break;

	case CMD_SCTRL_RTOS_IDLE_WAKEUP:
		sctrl_exit_rtos_idle_sleep();
		break;
#endif

	case CMD_SCTRL_RTOS_DEEP_SLEEP:
		//sctrl_enter_rtos_deep_sleep((PS_DEEP_CTRL_PARAM *)param);
		break;
#endif

	case CMD_GET_CHIP_ID:
		ret = REG_READ(SCTRL_CHIP_ID);
		break;

	case CMD_SCTRL_SET_FLASH_DPLL:
		reg = REG_READ(SCTRL_CONTROL);
		reg |= FLASH_26M_MUX_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_SET_FLASH_DCO:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~FLASH_26M_MUX_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_DSP_POWERDOWN:
	case CMD_SCTRL_USB_POWERDOWN:
	case CMD_SCTRL_MODEM_POWERDOWN:
	case CMD_SCTRL_MAC_POWERDOWN:
	case CMD_SCTRL_DSP_POWERUP:
	case CMD_SCTRL_USB_POWERUP:
	case CMD_SCTRL_MAC_POWERUP:
	case CMD_SCTRL_MODEM_POWERUP:
	case CMD_SCTRL_BLE_POWERDOWN:
	case CMD_SCTRL_BLE_POWERUP:

#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_LBUS_POWERDOWN:
	case CMD_SCTRL_LBUS_POWERUP:
	case CMD_SCTRL_BT_POWERDOWN:
	case CMD_SCTRL_BT_POWERUP:
#endif
		sctrl_subsys_power(cmd);
		break;

	case CMD_GET_DEVICE_ID:
		ret = REG_READ(SCTRL_DEVICE_ID);
		break;

	case CMD_GET_SCTRL_CONTROL:
		*((UINT32 *)param) = REG_READ(SCTRL_CONTROL);
		break;

	case CMD_SET_SCTRL_CONTROL:
		REG_WRITE(SCTRL_CONTROL, *((UINT32 *)param));
		break;

	case CMD_SCTRL_MCLK_SELECT:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(MCLK_MUX_MASK << MCLK_MUX_POSI);
		reg |= ((*(UINT32 *)param) & MCLK_MUX_MASK) << MCLK_MUX_POSI;

#if (defined(CONFIG_SOC_BK7251))
		if (((reg >> MCLK_MUX_POSI) & MCLK_MUX_MASK) == MCLK_SELECT_DPLL) {
			if ((((reg >> MCLK_DIV_POSI) & MCLK_DIV_MASK) <= MCLK_DIV_7))
				reg &= ~HCLK_DIV2_EN_BIT;
			else
				reg |= HCLK_DIV2_EN_BIT;
		}
#endif

		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_MCLK_DIVISION:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(MCLK_DIV_MASK << MCLK_DIV_POSI);
		reg |= ((*(UINT32 *)param) & MCLK_DIV_MASK) << MCLK_DIV_POSI;

#if (defined(CONFIG_SOC_BK7251))
		if (((reg >> MCLK_MUX_POSI) & MCLK_MUX_MASK) == MCLK_SELECT_DPLL) {
			if ((((reg >> MCLK_DIV_POSI) & MCLK_DIV_MASK) <= MCLK_DIV_7))
				reg &= ~HCLK_DIV2_EN_BIT;
			else
				reg |= HCLK_DIV2_EN_BIT;
		}
#endif

		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_MCLK_MUX_GET:
		reg = ((REG_READ(SCTRL_CONTROL) >> MCLK_MUX_POSI) & MCLK_MUX_MASK);
		*(UINT32 *)param = reg;
		break;

	case CMD_SCTRL_MCLK_DIV_GET:
		reg = ((REG_READ(SCTRL_CONTROL) >> MCLK_DIV_POSI) & MCLK_DIV_MASK);
		*(UINT32 *)param = reg;
		break;

	case CMD_SCTRL_RESET_SET:
		reg = REG_READ(SCTRL_RESET);
		reg |= ((*(UINT32 *)param) & SCTRL_RESET_MASK);
		REG_WRITE(SCTRL_RESET, reg);
		break;

	case CMD_SCTRL_RESET_CLR:
		reg = REG_READ(SCTRL_RESET);
		reg &= ~((*(UINT32 *)param) & SCTRL_RESET_MASK);
		REG_WRITE(SCTRL_RESET, reg);
		break;

	case CMD_SCTRL_MODEM_SUBCHIP_RESET:
	case CMD_SCTRL_USB_SUBSYS_RESET:
	case CMD_SCTRL_DSP_SUBSYS_RESET:
		sctrl_subsys_reset(cmd);
		break;

	case CMD_SCTRL_MAC_SUBSYS_RESET:
#if (!defined(CONFIG_SOC_BK7271))
		sctrl_subsys_reset(cmd);
#else
		ret = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
		ret = ret & (~((MAC_SUBSYS_RESET_MASK) << MAC_SUBSYS_RESET_POSI));
		reg = ret | ((MAC_SUBSYS_RESET_WORD & MAC_SUBSYS_RESET_MASK)
					 << MAC_SUBSYS_RESET_POSI);
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

		delay(1);
		reg = ret;
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

		/*resetting, and waiting for done*/
		reg = REG_READ(SCTRL_RESET);
		while (reg & MODEM_CORE_RESET_BIT) {
			delay(10);
			reg = REG_READ(SCTRL_RESET);
		}
		ret = SCTRL_SUCCESS;
#endif
		break;

	case CMD_SCTRL_MODEM_CORE_RESET:
		ret = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
		ret = ret & (~((MODEM_CORE_RESET_MASK) << MODEM_CORE_RESET_POSI));
		reg = ret | ((MODEM_CORE_RESET_WORD & MODEM_CORE_RESET_MASK)
					 << MODEM_CORE_RESET_POSI);
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

		delay(1);
		reg = ret;
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);

		/*resetting, and waiting for done*/
		reg = REG_READ(SCTRL_RESET);
		while (reg & MODEM_CORE_RESET_BIT) {
			delay(10);
			reg = REG_READ(SCTRL_RESET);
		}
		ret = SCTRL_SUCCESS;
		break;

	case CMD_SCTRL_MPIF_CLK_INVERT:
		reg = REG_READ(SCTRL_CONTROL);
		reg |= MPIF_CLK_INVERT_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_BLK_ENABLE:
		reg = REG_READ(SCTRL_BLOCK_EN_CFG);
		reg &= (~(BLOCK_EN_WORD_MASK << BLOCK_EN_WORD_POSI));
		reg |= (BLOCK_EN_WORD_PWD & BLOCK_EN_WORD_MASK) << BLOCK_EN_WORD_POSI;
		reg |= ((*(UINT32 *)param) & BLOCK_EN_VALID_MASK);
		REG_WRITE(SCTRL_BLOCK_EN_CFG, reg);
		break;

	case CMD_SCTRL_BLK_DISABLE:
		reg = REG_READ(SCTRL_BLOCK_EN_CFG);
		reg &= (~(BLOCK_EN_WORD_MASK << BLOCK_EN_WORD_POSI));
		reg |= (BLOCK_EN_WORD_PWD & BLOCK_EN_WORD_MASK) << BLOCK_EN_WORD_POSI;
		reg &= ~((*(UINT32 *)param) & BLOCK_EN_VALID_MASK);
		REG_WRITE(SCTRL_BLOCK_EN_CFG, reg);
		break;

	case CMD_SCTRL_BIAS_REG_SET:
		reg = REG_READ(SCTRL_BIAS);
		reg |= (*(UINT32 *)param);
#if (defined(CONFIG_SOC_BK7271))
		sctrl_analog_set(SCTRL_BIAS, reg);
#else
		REG_WRITE(SCTRL_BIAS, reg);
#endif
		break;

	case CMD_SCTRL_BIAS_REG_CLEAN:
		reg = REG_READ(SCTRL_BIAS);
		reg &= ~(*(UINT32 *)param);
#if (defined(CONFIG_SOC_BK7271))
		sctrl_analog_set(SCTRL_BIAS, reg);
#else
		REG_WRITE(SCTRL_BIAS, reg);
#endif
		break;

	case CMD_SCTRL_BIAS_REG_READ:
		ret = REG_READ(SCTRL_BIAS);
		break;

	case CMD_SCTRL_BIAS_GET_CALI_OUT:
#if (defined(CONFIG_SOC_BK7271))
		reg = REG_READ(PMU_STATUS);
		ret = ((reg >> PMU_BAIS_CAL_OUT_POSI) & PMU_BAIS_CAL_OUT_MASK);
#endif
		break;

	case CMD_SCTRL_BIAS_REG_WRITE:
#if (defined(CONFIG_SOC_BK7271))
		sctrl_analog_set(SCTRL_BIAS, *(UINT32 *)param);
#else
		REG_WRITE(SCTRL_BIAS, *(UINT32 *)param);
#endif
		break;

	case CMD_SCTRL_ANALOG_CTRL4_SET:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		reg |= (*(UINT32 *)param);
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
		break;

	case CMD_SCTRL_ANALOG_CTRL4_CLEAN:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		reg &= ~(*(UINT32 *)param);
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
		break;

#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_ANALOG_CTRL9_REAL_SET:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9_REAL);
		reg |= (*(UINT32 *)param);
		sctrl_analog_set(SCTRL_ANALOG_CTRL9_REAL, reg);
		break;

	case CMD_SCTRL_ANALOG_CTRL9_REAL_CLEAN:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9_REAL);
		reg &= ~(*(UINT32 *)param);
		sctrl_analog_set(SCTRL_ANALOG_CTRL9_REAL, reg);
		break;
#endif

	case CMD_SCTRL_CALI_DPLL:
		sctrl_cali_dpll(0);
		break;

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	case CMD_BLE_RF_PTA_EN:
		reg = REG_READ(SCTRL_CONTROL);
		reg |= (BLE_RF_PTA_EN_BIT);
		REG_WRITE(SCTRL_CONTROL, reg);
		break;
	case CMD_BLE_RF_PTA_DIS:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(BLE_RF_PTA_EN_BIT);
		REG_WRITE(SCTRL_CONTROL, reg);
		break;
#endif


#if (!defined(CONFIG_SOC_BK7231))
#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_SET_XTALH_CTUNE:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		reg &= ~(XTALH_CTUNE_MASK << XTALH_CTUNE_POSI);
		reg |= (((*(UINT32 *)param) &XTALH_CTUNE_MASK) << XTALH_CTUNE_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
		break;

	case CMD_SCTRL_GET_XTALH_CTUNE:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		ret = ((reg >> XTALH_CTUNE_POSI) & XTALH_CTUNE_MASK);
		break;
#else
	case CMD_SCTRL_SET_XTALH_CTUNE:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
		reg &= ~(XTALH_CTUNE_MASK << XTALH_CTUNE_POSI);
		reg |= (((*(UINT32 *)param) &XTALH_CTUNE_MASK) << XTALH_CTUNE_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL2, reg);
		break;

	case CMD_SCTRL_GET_XTALH_CTUNE:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
		ret = ((reg >> XTALH_CTUNE_POSI) & XTALH_CTUNE_MASK);
		break;

	case CMD_BLE_RF_BIT_SET:
		reg = REG_READ(SCTRL_CONTROL);
		reg |= BLE_RF_EN_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_BLE_RF_BIT_CLR:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(BLE_RF_EN_BIT);
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_BLE_RF_BIT_GET:
		reg = REG_READ(SCTRL_CONTROL);
		*((UINT32 *)param) = reg & (BLE_RF_EN_BIT);
		break;

	case CMD_EFUSE_WRITE_BYTE:
		ret = sctrl_write_efuse(param);
		break;

	case CMD_EFUSE_READ_BYTE:
		ret = sctrl_read_efuse(param);
		break;
#endif

#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	case CMD_QSPI_VDDRAM_VOLTAGE:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(PSRAM_VDDPAD_VOLT_MASK << PSRAM_VDDPAD_VOLT_POSI);
		reg |= (((*(UINT32 *)param) & PSRAM_VDDPAD_VOLT_MASK) << PSRAM_VDDPAD_VOLT_POSI);
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_QSPI_IO_VOLTAGE:
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~(QSPI_IO_VOLT_MASK << QSPI_IO_VOLT_POSI);
		reg |= (((*(UINT32 *)param) & QSPI_IO_VOLT_MASK) << QSPI_IO_VOLT_POSI);
		REG_WRITE(SCTRL_CONTROL, reg);
		break;
#endif
#endif // (!CONFIG_SOC_BK7231)

#if (defined(CONFIG_SOC_BK7251)) || (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_OPEN_DAC_ANALOG:
#if (defined(CONFIG_SOC_BK7271))
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, 0x300B194E);
		sctrl_analog_set(SCTRL_ANALOG_CTRL9, 0x82205600);
		sctrl_analog_set(SCTRL_ANALOG_CTRL10, 0x80803390);
		sctrl_analog_set(SCTRL_ANALOG_CTRL13, 0x2108DB28);

		sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, 0x00B09350 | AUDIO_PLL_AUDIO_EN | AUDIO_PLL_RESET);
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL13);
		reg |= AUDIO_DCO_EN;
		sctrl_analog_set(SCTRL_ANALOG_CTRL13, reg);
#endif
		//cause increase CONFIG_MCU_PS elect
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9);
		reg |= EN_AUD_DAC_L | EN_AUD_DAC_R
			   | DAC_PA_OUTPUT_EN | DAC_DRIVER_OUTPUT_EN
			   | AUD_DAC_DGA_EN;
		sctrl_analog_set(SCTRL_ANALOG_CTRL9, reg);

		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL10);
		reg |= DAC_N_END_OUPT_L | DAC_N_END_OUPT_R;
		sctrl_analog_set(SCTRL_ANALOG_CTRL10, reg);
		break;

	case CMD_SCTRL_CLOSE_DAC_ANALOG:
		//cause reduce CONFIG_MCU_PS elect
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9);
		reg &= ~(EN_AUD_DAC_L | EN_AUD_DAC_R
				 | DAC_PA_OUTPUT_EN | DAC_DRIVER_OUTPUT_EN
				 | AUD_DAC_DGA_EN);
		sctrl_analog_set(SCTRL_ANALOG_CTRL9, reg);

		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL10);
		reg &= ~(DAC_N_END_OUPT_L | DAC_N_END_OUPT_R);
		sctrl_analog_set(SCTRL_ANALOG_CTRL10, reg);
		break;

#if (!defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_OPEN_ADC_MIC_ANALOG:
		//cause increase CONFIG_MCU_PS elect
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg &= ~(SPI_PWD_AUD_ADC_L | SPI_PWD_AUD_ADC_R);
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		break;

	case CMD_SCTRL_CLOSE_ADC_MIC_ANALOG:
		//cause reduce CONFIG_MCU_PS elect
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg |= (SPI_PWD_AUD_ADC_L | SPI_PWD_AUD_ADC_R);
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		break;
#else
	case CMD_SCTRL_OPEN_ADC_MIC_ANALOG:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL13);
		reg &= ~(MIC1_PWR_DOWN | MIC2_PWR_DOWN | MIC3_PWR_DOWN | MIC4_PWR_DOWN | MIC5_PWR_DOWN | MIC6_PWR_DOWN);
		sctrl_analog_set(SCTRL_ANALOG_CTRL13, reg);
		break;

	case CMD_SCTRL_CLOSE_ADC_MIC_ANALOG:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL13);
		reg |= (MIC1_PWR_DOWN | MIC2_PWR_DOWN | MIC3_PWR_DOWN | MIC4_PWR_DOWN | MIC5_PWR_DOWN | MIC6_PWR_DOWN);
		sctrl_analog_set(SCTRL_ANALOG_CTRL13, reg);
		break;
#endif

	case CMD_SCTRL_ENALBLE_ADC_LINE_IN:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg |= LINE_IN_EN;
#if (defined(CONFIG_SOC_BK7271))
		reg |= LINE2_IN_EN;
#endif
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);

#if (defined(CONFIG_SOC_BK7271))/*set gpio 2 and 3 to high impendance*/
		reg = GPIO_CFG_PARAM(GPIO2, GMODE_SET_HIGH_IMPENDANCE);
		sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_CFG, &reg);
		reg = GPIO_CFG_PARAM(GPIO3, GMODE_SET_HIGH_IMPENDANCE);
		sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_CFG, &reg);
#endif
		break;

	case CMD_SCTRL_DISALBLE_ADC_LINE_IN:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg &= ~LINE_IN_EN;
#if (defined(CONFIG_SOC_BK7271))
		reg &= ~LINE2_IN_EN;
#endif
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		break;

	case CMD_SCTRL_SET_DAC_VOLUME_ANALOG:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg &= ~(AUD_DAC_GAIN_MASK << AUD_DAC_GAIN_POSI);
		reg |= (((*(UINT32 *)param) & AUD_DAC_GAIN_MASK) << AUD_DAC_GAIN_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		break;

	case CMD_SCTRL_SET_LINEIN_VOLUME_ANALOG:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		reg &= ~(LINE_IN_GAIN_MASK << LINE_IN_GAIN_POSI);
		reg |= (((*(UINT32 *)param) & LINE_IN_GAIN_MASK) << LINE_IN_GAIN_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		break;

	case CMD_SCTRL_SET_VOLUME_PORT:
		if ((*(UINT32 *)param) == AUDIO_DAC_VOL_DIFF_MODE) {
			reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9);
			reg |= (DAC_DIFF_EN);
			sctrl_analog_set(SCTRL_ANALOG_CTRL9, reg);

			reg = sctrl_analog_get(SCTRL_ANALOG_CTRL10);
			reg |= (DAC_N_END_OUPT_L | DAC_N_END_OUPT_R);
			reg &= ~(DAC_VSEL_MASK << DAC_VSEL_POSI);
			reg |= ((0x3 & DAC_VSEL_MASK) << DAC_VSEL_POSI);;
			sctrl_analog_set(SCTRL_ANALOG_CTRL10, reg);
		} else if ((*(UINT32 *)param) == AUDIO_DAC_VOL_SINGLE_MODE) {
			reg = sctrl_analog_get(SCTRL_ANALOG_CTRL9);
			reg &= ~(DAC_DIFF_EN);
			sctrl_analog_set(SCTRL_ANALOG_CTRL9, reg);

			reg = sctrl_analog_get(SCTRL_ANALOG_CTRL10);
			reg &= ~(DAC_N_END_OUPT_L | DAC_N_END_OUPT_R);
			reg &= ~(DAC_VSEL_MASK << DAC_VSEL_POSI);
			reg |= ((0 & DAC_VSEL_MASK) << DAC_VSEL_POSI);;
			sctrl_analog_set(SCTRL_ANALOG_CTRL10, reg);
		}
		break;

	case CMD_SCTRL_SET_AUD_DAC_MUTE:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		if ((*(UINT32 *)param) == AUDIO_DAC_ANALOG_MUTE) {
			reg |= (AUD_DAC_MUTE_EN);
			sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		} else if ((*(UINT32 *)param) == AUDIO_DAC_ANALOG_UNMUTE) {
			reg &= ~(AUD_DAC_MUTE_EN);
			sctrl_analog_set(SCTRL_ANALOG_CTRL8, reg);
		}
		break;
#endif // (CONFIG_SOC_BK7251) || (CONFIG_SOC_BK7271)

#if (defined(CONFIG_SOC_BK7271))
	case CMD_SCTRL_AUDIO_PLL:
		if ((*(UINT32 *)param) == 8000) {					//8KHz
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_SDM, 0x3C7EA932);
			reg = sctrl_analog_get(SCTRL_ANALOG_AUDIO_PLL_CTRL);
			reg &= ~(AUDIO_PLL_CKSEL);
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		} else if ((*(UINT32 *)param) == 16000) {				//16KHz
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_SDM, 0x3C7EA932);
			reg = sctrl_analog_get(SCTRL_ANALOG_AUDIO_PLL_CTRL);
			reg &= ~(AUDIO_PLL_CKSEL);
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		} else if ((*(UINT32 *)param) == 44100) {				//44.1KHz
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_SDM, 0x37945EA6);
			reg = sctrl_analog_get(SCTRL_ANALOG_AUDIO_PLL_CTRL);
			reg |= (AUDIO_PLL_CKSEL);
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		} else if ((*(UINT32 *)param) == 48000) {				//48KHz
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_SDM, 0x3C7EA932);
			reg = sctrl_analog_get(SCTRL_ANALOG_AUDIO_PLL_CTRL);
			reg |= (AUDIO_PLL_CKSEL);
			sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		}
		reg = sctrl_analog_get(SCTRL_ANALOG_AUDIO_PLL_CTRL);
		reg &= ~(AUDIO_PLL_SPI_TRIGGER);
		sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		reg |= (AUDIO_PLL_SPI_TRIGGER);
		sctrl_analog_set(SCTRL_ANALOG_AUDIO_PLL_CTRL, reg);
		break;
#endif

#if ((!defined(CONFIG_SOC_BK7231)) && (!defined(CONFIG_SOC_BK7271)))
	case CMD_SCTRL_SET_ANALOG6:
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL6);
		reg |= (DPLL_CLK_FOR_AUDIO_EN | DPLL_DIVIDER_CLK_SEL | DPLL_RESET);
		sctrl_analog_set(SCTRL_ANALOG_CTRL6, reg);
		break;
#endif
	case CMD_SCTRL_SET_ANALOG0:
		sctrl_analog_set(SCTRL_ANALOG_CTRL0, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG1:
		sctrl_analog_set(SCTRL_ANALOG_CTRL1, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG2:
		sctrl_analog_set(SCTRL_ANALOG_CTRL2, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG3:
		sctrl_analog_set(SCTRL_ANALOG_CTRL3, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG4:
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG5:
		sctrl_analog_set(SCTRL_ANALOG_CTRL5, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_GET_ANALOG0:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL0);
		break;
	case CMD_SCTRL_GET_ANALOG1:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL1);
		break;
	case CMD_SCTRL_GET_ANALOG2:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL2);
		break;
	case CMD_SCTRL_GET_ANALOG3:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL3);
		break;
	case CMD_SCTRL_GET_ANALOG4:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		break;
	case CMD_SCTRL_GET_ANALOG5:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL5);
		break;
#if (defined(CONFIG_SOC_BK7251))
	case CMD_SCTRL_SET_ANALOG7:
		sctrl_analog_set(SCTRL_ANALOG_CTRL7, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG8:
		sctrl_analog_set(SCTRL_ANALOG_CTRL8, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG9:
		sctrl_analog_set(SCTRL_ANALOG_CTRL9, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_SET_ANALOG10:
		sctrl_analog_set(SCTRL_ANALOG_CTRL10, (*(UINT32 *)param));
		break;
	case CMD_SCTRL_GET_ANALOG7:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL7);
		break;
	case CMD_SCTRL_GET_ANALOG8:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL8);
		break;
	case CMD_SCTRL_GET_ANALOG9:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL9);
		break;
	case CMD_SCTRL_GET_ANALOG10:
		ret = sctrl_analog_get(SCTRL_ANALOG_CTRL10);
		break;

	case CMD_SCTRL_AUDIO_PLL:
		if ((*(UINT32 *)param) == 48000000)						//48MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3B13B13B);
		else if ((*(UINT32 *)param) == 48128000)					//48.128MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3B3C05AC);
		else if ((*(UINT32 *)param) == 48384000)					//48.384MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3B8CAE8E);
		else if ((*(UINT32 *)param) == 49152000)					//49.152MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3C7EA932);
		else if ((*(UINT32 *)param) == 49392000)					//49.392MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3CCA4785);
		else if ((*(UINT32 *)param) == 50688000)					//50.688MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3E629E7C);
		else if ((*(UINT32 *)param) == 50803200)					//50.8032MHz
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3E86EA7A);
		else
			sctrl_analog_set(SCTRL_ANALOG_CTRL5, 0x3B13B13B);
		break;
#endif

#if defined(CONFIG_USB_CHARGE)
	case CMD_SCTRL_USB_CHARGE_CAL:
		if (1 == ((CHARGE_OPER_PTR)param)->step)
			charger_vlcf_calibration(0);
		else if (2 == ((CHARGE_OPER_PTR)param)->step)
			charger_icp_calibration(0);
		else if (3 == ((CHARGE_OPER_PTR)param)->step)
			charger_vcv_calibration(0);
		else if (4 == ((CHARGE_OPER_PTR)param)->step)
			charger_calib_get(((CHARGE_OPER_PTR)param)->cal);
		break;
	case CMD_SCTRL_USB_CHARGE_START:
		charger_start(param);
		break;
	case CMD_SCTRL_USB_CHARGE_STOP:
		charger_stop((*(UINT32 *)param));
		break;
#endif

	case CMD_SCTRL_SET_LOW_PWR_CLK:
		reg = REG_READ(SCTRL_LOW_PWR_CLK);
		reg &= ~(LPO_CLK_MUX_MASK);
		reg |= ((*(UINT32 *)param) << LPO_CLK_MUX_POSI);
		REG_WRITE(SCTRL_LOW_PWR_CLK, reg);
		break;

	case CMD_SCTRL_SET_GADC_SEL:
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
		reg = sctrl_analog_get(SCTRL_ANALOG_CTRL4);
		reg &= ~(GADC_CAL_SEL_MASK << GADC_CAL_SEL_POSI);
		reg |= (((*(UINT32 *)param) & GADC_CAL_SEL_MASK) << GADC_CAL_SEL_POSI);
		sctrl_analog_set(SCTRL_ANALOG_CTRL4, reg);
#endif
		break;

	case CMD_SCTRL_SET_VDD_VALUE:
		reg = REG_READ(SCTRL_DIGTAL_VDD);
		reg &= (~(DIG_VDD_ACTIVE_MASK << DIG_VDD_ACTIVE_POSI));
		reg |= ((*(UINT32 *)param) << DIG_VDD_ACTIVE_POSI);
		REG_WRITE(SCTRL_DIGTAL_VDD, reg);
		break;
	case CMD_SCTRL_GET_VDD_VALUE:
		reg = REG_READ(SCTRL_DIGTAL_VDD);
		ret = (reg >> DIG_VDD_ACTIVE_POSI) & DIG_VDD_ACTIVE_MASK;
		break;

	case CMD_GET_SCTRL_RETETION:
		*((UINT32 *)param) = REG_READ(SCTRL_SW_RETENTION);
		break;

	case CMD_SET_SCTRL_RETETION:
		REG_WRITE(SCTRL_SW_RETENTION, *((UINT32 *)param));
		break;

	case CMD_SCTRL_MODEM_AHB_CLOCK_DISABLE:
		/* Modem AHB clock disable*/
#if (defined(CONFIG_SOC_BK7271))
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~PHY_HCLK_EN_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
#else
		reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
		reg &= ~PHY_HCLK_EN_BIT;
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg);
#endif
		break;
	case CMD_SCTRL_MODEM_CLOCK480M_DISABLE:
		/* Modem Subsystem clock 480m disable*/
		reg = REG_READ(SCTRL_CONTROL);
		REG_WRITE(SCTRL_CONTROL, reg | MODEM_CLK480M_PWD_BIT);
		break;

	case CMD_SCTRL_MODEM_AHB_CLOCK_ENABLE:
		/* Modem AHB clock enable*/
#if (defined(CONFIG_SOC_BK7271))
		reg = REG_READ(SCTRL_CONTROL);
		REG_WRITE(SCTRL_CONTROL, reg | PHY_HCLK_EN_BIT);
#else
		reg = REG_READ(SCTRL_MODEM_CORE_RESET_PHY_HCLK);
		REG_WRITE(SCTRL_MODEM_CORE_RESET_PHY_HCLK, reg | PHY_HCLK_EN_BIT);
#endif
		break;

	case CMD_SCTRL_MODEM_CLOCK480M_ENABLE:
		/* Modem Subsystem clock 480m enable*/
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~MODEM_CLK480M_PWD_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_MAC_AHB_CLOCK_DISABLE:
		sctrl_mac_ahb_slave_clock_disable();
		break;
	case CMD_SCTRL_MAC_CLOCK480M_DISABLE:
		/* Mac Subsystem clock 480m disable*/
		reg = REG_READ(SCTRL_CONTROL);
		REG_WRITE(SCTRL_CONTROL, reg | MAC_CLK480M_PWD_BIT);
		break;
	case CMD_SCTRL_MAC_AHB_CLOCK_ENABLE:
		sctrl_mac_ahb_slave_clock_enable();
		break;
	case CMD_SCTRL_MAC_CLOCK480M_ENABLE:
		/* Mac Subsystem clock 480m enable*/
		reg = REG_READ(SCTRL_CONTROL);
		reg &= ~MAC_CLK480M_PWD_BIT;
		REG_WRITE(SCTRL_CONTROL, reg);
		break;

	case CMD_SCTRL_BLOCK_EN_MUX_SET:
		reg = REG_READ(SCTRL_BLOCK_EN_MUX);
		reg &= ~(0x1FF);
		reg |= *((UINT32 *)param);
		REG_WRITE(SCTRL_BLOCK_EN_MUX, reg);
		break;

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
	case CMD_SCTRL_FIX_DPLL_DIV:
		if ((DEVICE_ID_BK7231N_P & DEVICE_ID_MASK) != (sctrl_ctrl(CMD_GET_DEVICE_ID, NULL) & DEVICE_ID_MASK)) {
			sctrl_fix_dpll_div();
		}
#endif
	default:
		ret = SCTRL_FAILURE;
		break;
	}
	GLOBAL_INT_RESTORE();

	return ret;
}
#endif
// EOF

