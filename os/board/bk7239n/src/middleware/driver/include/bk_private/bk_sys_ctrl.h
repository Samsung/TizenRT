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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>

#if (defined(CONFIG_SOC_BK7271))
#include "pmu.h"
#endif

#define SCTRL_DEV_NAME       "sys_ctrl"

#define SCTRL_FAILURE        ((UINT32)-1)
#define SCTRL_SUCCESS        (0)

#define SCTRL_CMD_MAGIC      (0xC123000)
enum
{
	CMD_GET_CHIP_ID              = SCTRL_CMD_MAGIC + 1,
	CMD_GET_DEVICE_ID            = SCTRL_CMD_MAGIC + 2,
	CMD_GET_SCTRL_CONTROL,
	CMD_SET_SCTRL_CONTROL,

	CMD_SCTRL_MCLK_SELECT,
	CMD_SCTRL_MCLK_DIVISION,
	CMD_SCTRL_MCLK_MUX_GET,
	CMD_SCTRL_MCLK_DIV_GET,

	CMD_SCTRL_RESET_SET,
	CMD_SCTRL_RESET_CLR,
	CMD_SCTRL_MODEM_CORE_RESET,
	CMD_SCTRL_MPIF_CLK_INVERT,
	CMD_SCTRL_MODEM_SUBCHIP_RESET,
	CMD_SCTRL_MAC_SUBSYS_RESET,
	CMD_SCTRL_USB_SUBSYS_RESET,
	CMD_SCTRL_DSP_SUBSYS_RESET,
	CMD_SCTRL_BLK_ENABLE,
	CMD_SCTRL_BLK_DISABLE,

	CMD_SCTRL_DSP_POWERDOWN,
	CMD_SCTRL_DSP_POWERUP,
	CMD_SCTRL_USB_POWERDOWN,
	CMD_SCTRL_USB_POWERUP,
	CMD_SCTRL_MAC_POWERDOWN,
	CMD_SCTRL_MAC_POWERUP,
	CMD_SCTRL_MODEM_POWERDOWN,
	CMD_SCTRL_MODEM_POWERUP,
	CMD_SCTRL_BLE_POWERDOWN,
	CMD_SCTRL_BLE_POWERUP,

	#if (defined(CONFIG_SOC_BK7271))
	CMD_SCTRL_LBUS_POWERDOWN,
	CMD_SCTRL_LBUS_POWERUP,
	CMD_SCTRL_BT_POWERDOWN,
	CMD_SCTRL_BT_POWERUP,
	#endif

	CMD_SCTRL_CALI_DPLL,

	CMD_SCTRL_BIAS_REG_SET,
	CMD_SCTRL_BIAS_REG_CLEAN,
	CMD_SCTRL_BIAS_REG_READ,
	CMD_SCTRL_BIAS_REG_WRITE,
	CMD_SCTRL_BIAS_GET_CALI_OUT,

	CMD_SCTRL_ANALOG_CTRL4_SET,
	CMD_SCTRL_ANALOG_CTRL4_CLEAN,
	#if (defined(CONFIG_SOC_BK7271))
	CMD_SCTRL_ANALOG_CTRL9_REAL_SET,
	CMD_SCTRL_ANALOG_CTRL9_REAL_CLEAN,
	#endif

	CMD_SCTRL_SET_FLASH_DCO,
	CMD_SCTRL_SET_FLASH_DPLL,
	CMD_SCTRL_NORMAL_SLEEP,
	CMD_SCTRL_NORMAL_WAKEUP,
	CMD_SCTRL_RTOS_IDLE_SLEEP,
	CMD_SCTRL_RTOS_IDLE_WAKEUP,
	CMD_SCTRL_RTOS_DEEP_SLEEP,

	#if (!defined(CONFIG_SOC_BK7231))
	CMD_SCTRL_SET_XTALH_CTUNE,
	CMD_SCTRL_GET_XTALH_CTUNE,
	CMD_BLE_RF_BIT_SET,
	CMD_BLE_RF_BIT_CLR,
	CMD_BLE_RF_BIT_GET,

	#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
	CMD_BLE_RF_PTA_EN,
	CMD_BLE_RF_PTA_DIS,
	#endif

	CMD_EFUSE_WRITE_BYTE,
	CMD_EFUSE_READ_BYTE,
	CMD_GET_SCTRL_RETETION,
	CMD_SET_SCTRL_RETETION,
	#endif // (!CONFIG_SOC_BK7231)

	#if (!defined(CONFIG_SOC_BK7231)) && (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX))
	CMD_QSPI_VDDRAM_VOLTAGE,
	CMD_QSPI_IO_VOLTAGE,
	#endif // (!CONFIG_SOC_BK7231)

	CMD_SCTRL_SET_ANALOG0,
	CMD_SCTRL_SET_ANALOG1,
	CMD_SCTRL_SET_ANALOG2,
	CMD_SCTRL_SET_ANALOG3,
	CMD_SCTRL_SET_ANALOG4,
	CMD_SCTRL_SET_ANALOG5,
	CMD_SCTRL_SET_ANALOG6,
	CMD_SCTRL_SET_ANALOG7,
	CMD_SCTRL_SET_ANALOG8,
	CMD_SCTRL_SET_ANALOG9,
	CMD_SCTRL_SET_ANALOG10,
	CMD_SCTRL_GET_ANALOG0,
	CMD_SCTRL_GET_ANALOG1,
	CMD_SCTRL_GET_ANALOG2,
	CMD_SCTRL_GET_ANALOG3,
	CMD_SCTRL_GET_ANALOG4,
	CMD_SCTRL_GET_ANALOG5,
	CMD_SCTRL_GET_ANALOG6,
	CMD_SCTRL_GET_ANALOG7,
	CMD_SCTRL_GET_ANALOG8,
	CMD_SCTRL_GET_ANALOG9,
	CMD_SCTRL_GET_ANALOG10,

	#if (defined(CONFIG_SOC_BK7251)) || (defined(CONFIG_SOC_BK7271))
	CMD_SCTRL_OPEN_DAC_ANALOG,
	CMD_SCTRL_CLOSE_DAC_ANALOG,
	CMD_SCTRL_OPEN_ADC_MIC_ANALOG,
	CMD_SCTRL_CLOSE_ADC_MIC_ANALOG,
	CMD_SCTRL_ENALBLE_ADC_LINE_IN,
	CMD_SCTRL_DISALBLE_ADC_LINE_IN,
	CMD_SCTRL_SET_DAC_VOLUME_ANALOG,
	CMD_SCTRL_SET_LINEIN_VOLUME_ANALOG,
	CMD_SCTRL_SET_VOLUME_PORT,
	CMD_SCTRL_SET_AUD_DAC_MUTE,
	CMD_SCTRL_AUDIO_PLL,
	CMD_SCTRL_USB_CHARGE_CAL,
	CMD_SCTRL_USB_CHARGE_START,
	CMD_SCTRL_USB_CHARGE_STOP,

	CMD_SCTRL_UNCONDITIONAL_MAC_DOWN,
	CMD_SCTRL_UNCONDITIONAL_MAC_UP,

	#endif // (CONFIG_SOC_BK7251) || (CONFIG_SOC_BK7271)
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7256XX))
    CMD_SCTRL_FIX_DPLL_DIV,
#endif

    CMD_SCTRL_MODEM_AHB_CLOCK_DISABLE,
	CMD_SCTRL_MODEM_AHB_CLOCK_ENABLE,
    CMD_SCTRL_MODEM_CLOCK480M_DISABLE,
	CMD_SCTRL_MODEM_CLOCK480M_ENABLE,
    CMD_SCTRL_MAC_AHB_CLOCK_DISABLE,
	CMD_SCTRL_MAC_AHB_CLOCK_ENABLE,
    CMD_SCTRL_MAC_CLOCK480M_DISABLE,
	CMD_SCTRL_MAC_CLOCK480M_ENABLE,
	CMD_SCTRL_SET_LOW_PWR_CLK,
	CMD_SCTRL_SET_GADC_SEL,
	CMD_SCTRL_SET_VDD_VALUE,
	CMD_SCTRL_GET_VDD_VALUE,
	CMD_SCTRL_BLOCK_EN_MUX_SET,

	#if (defined(CONFIG_SOC_BK7271))
	CMD_SCTRL_OVERCLOCK,
	#endif
};

/*PSRAM_VDDPAD_VOLT */
#define PSRAM_VDD_1_8V                                       (0x0)
#define PSRAM_VDD_2_5V                                       (0x1)
#define PSRAM_VDD_3_3V                                       (0x2)
#define PSRAM_VDD_3_3V_DEF                                   (0x3)

#define QSPI_IO_1_8V                                         (0x0)
#define QSPI_IO_2_5V                                         (0x1)
#define QSPI_IO_3_3V                                         (0x2)
#define QSPI_IO_3V_DEF                                       (0x3)

/*CMD_SCTRL_MCLK_SELECT*/
#define MCLK_SELECT_DCO                                      (0x0)
#define MCLK_SELECT_26M_XTAL                                 (0x1)
#define MCLK_SELECT_DPLL                                     (0x2)
#define MCLK_SELECT_LPO                                      (0x3)

/*CMD_SCTRL_MCLK_DIVISION*/
#define MCLK_DIV_0                               (0)
#define MCLK_DIV_1                               (1)
#define MCLK_DIV_2                               (2)
#define MCLK_DIV_3                               (3)
#define MCLK_DIV_4                               (4)
#define MCLK_DIV_5                               (5)
#define MCLK_DIV_6                               (6)
#define MCLK_DIV_7                               (7)
#define MCLK_DIV_8                               (8)
#define MCLK_DIV_9                               (9)
#define MCLK_DIV_10                              (10)
#define MCLK_DIV_11                              (11)
#define MCLK_DIV_12                              (12)
#define MCLK_DIV_13                              (13)
#define MCLK_DIV_14                              (14)
#define MCLK_DIV_15                              (15)


/*CMD_SCTRL_BLK_ENABLE CMD_SCTRL_BLK_DISABLE*/
#if (!defined(CONFIG_SOC_BK7271))
#if (defined(CONFIG_SOC_BK7231))
#define BLK_BIT_LINEIN                           (1 << 19)
#define BLK_BIT_MIC_R_CHANNEL                    (1 << 18)
#define BLK_BIT_MIC_L_CHANNEL                    (1 << 17)
#define BLK_BIT_AUDIO_R_CHANNEL                  (1 << 16)
#define BLK_BIT_AUDIO_L_CHANNEL                  (1 << 15)
#define BLK_BIT_USB                              (1 << 14)
#elif (defined(CONFIG_SOC_BK7231U))
#define BLK_BIT_NC                               (1 << 19)
#define BLK_BIT_MIC_QSPI_RAM_OR_FLASH            (1 << 18)
#define BLK_BIT_MIC_PGA                          (1 << 17)
#define BLK_BIT_AUDIO_PLL                        (1 << 16)
#define BLK_BIT_AUDIO_RANDOM_GENERATOR           (1 << 15)
#define BLK_BIT_USB                              (1 << 14)
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
#define BLK_BIT_AUDIO_RANDOM_GENERATOR           (1 << 15)
#elif (defined(CONFIG_SOC_BK7251))
#define BLK_BIT_NC                               (1 << 19)
#define BLK_BIT_MIC_QSPI_RAM_OR_FLASH            (1 << 18)
#define BLK_BIT_AUDIO                            (1 << 17)
#define BLK_BIT_AUDIO_PLL                        (1 << 16)
#define BLK_BIT_AUDIO_RANDOM_GENERATOR           (1 << 15)
#define BLK_BIT_USB                              (1 << 14)
#endif // (CONFIG_SOC_BK7231)
#define BLK_BIT_SARADC                           (1 << 13)
#define BLK_BIT_TEMPRATURE_SENSOR                (1 << 12)
#define BLK_BIT_26M_XTAL_LOW_POWER               (1 << 11)
#define BLK_BIT_XTAL2RF                          (1 << 10)
#define BLK_BIT_IO_LDO_LOW_POWER                 (1 << 09)
#define BLK_BIT_ANALOG_SYS_LDO                   (1 << 08)
#define BLK_BIT_DIGITAL_CORE_LDO_LOW_POWER       (1 << 07)
#define BLK_BIT_DIGITAL_CORE                     (1 << 06)
#define BLK_BIT_DPLL_480M                        (1 << 05)
#define BLK_BIT_32K_XTAL                         (1 << 04)
#define BLK_BIT_26M_XTAL                         (1 << 03)
#define BLK_BIT_ROSC32K                          (1 << 02)
#define BLK_BIT_DCO                              (1 << 01)
#define BLK_BIT_FLASH                            (1 << 00)
#endif

/* CMD_SCTRL_RESET _SET/_CLR*/
#define PARAM_MODEM_CORE_RESET_BIT               (1 << 6)
#define PARAM_TL410_EXT_WAIT_BIT                 (1 << 5)
#define PARAM_USB_SUBSYS_RESET_BIT               (1 << 4)
#define PARAM_TL410_BOOT_BIT                     (1 << 3)
#define PARAM_MAC_SUBSYS_RESET_BIT               (1 << 2)
#define PARAM_DSP_SUBSYS_RESET_BIT               (1 << 1)
#define PARAM_MODEM_SUBCHIP_RESET_BIT            (1 << 0)

/* CMD_GET_SCTRL_CONTROL CMD_SET_SCTRL_CONTROL*/
#define MCLK_MODE_DCO                            (0x0)
#define MCLK_MODE_26M_XTAL                       (0x1)
#define MCLK_MODE_DPLL                           (0x2)
#define MCLK_MODE_LPO                            (0x3)

/*CMD_SCTRL_BIAS_REG_SET CMD_SCTRL_BIAS_REG_CLEAN*/
#if (defined(CONFIG_SOC_BK7271))
#define PARAM_BIAS_CAL_OUT_POSI                  (12)
#define PARAM_BIAS_CAL_OUT_MASK                  (0x1F)
#define PARAM_LDO_VAL_MANUAL_POSI                (16)
#define PARAM_LDO_VAL_MANUAL_MASK                (0x1F)
#define PARAM_BIAS_CAL_MANUAL_BIT                (1 << 22)
#define PARAM_BIAS_CAL_TRIGGER_BIT               (1 << 21)
#else
#define PARAM_BIAS_CAL_OUT_POSI                  (16)
#define PARAM_BIAS_CAL_OUT_MASK                  (0x1F)
#define PARAM_LDO_VAL_MANUAL_POSI                (8)
#define PARAM_LDO_VAL_MANUAL_MASK                (0x1F)
#define PARAM_BIAS_CAL_MANUAL_BIT                (1 << 4)
#define PARAM_BIAS_CAL_TRIGGER_BIT               (1 << 0)
#endif

/*CMD_SCTRL_ANALOG_CTRL4_SET CMD_SCTRL_ANALOG_CTRL4_CLEAN*/
#define PARAM_VSEL_SYS_LDO_POSI                  (27)
#define PARAM_VSEL_SYS_LDO_MASK                  (0x3)
#define PARAM_SARADC_BT_TXSEL_BIT                (1 << 5)
#define PARAM_SARADC_VREF_SEL_BIT                (1 << 4)

#if (defined(CONFIG_SOC_BK7231U))
#define DEFAULT_TXID_XTAL                        (0x19)
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A))
#if (defined(CONFIG_XTAL_FREQ_40M))
#define DEFAULT_TXID_XTAL                        (0x70)
#else
#define DEFAULT_TXID_XTAL                        (0x10)//(0x37)
#endif
#elif (defined(CONFIG_SOC_BK7251)) || (defined(CONFIG_SOC_BK7271))
#define DEFAULT_TXID_XTAL                        (0x10)
#endif // (CONFIG_SOC_BK7231U)

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
#define PARAM_XTALH_CTUNE_MASK                   (0x7F)

#define PARAM_AUD_DAC_GAIN_MASK                  (0x1F)
#elif (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define PARAM_XTALH_CTUNE_MASK                   (0xFF)

#define PARAM_AUD_DAC_GAIN_MASK                  (0x1F)
#elif (!defined(CONFIG_SOC_BK7231))
#define PARAM_XTALH_CTUNE_MASK                   (0x3F)

#define PARAM_AUD_DAC_GAIN_MASK                  (0x1F)
#endif // (!CONFIG_SOC_BK7231)

/*CMD_SCTRL_SET_LOW_PWR_CLK*/
#define LPO_SELECT_ROSC                             (0x0)
#define LPO_SELECT_32K_XTAL                         (0x1)
#define LPO_SELECT_32K_DIV                          (0x2)

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
#define SW_RETENTION_WDT_FLAG                       (1 << 16)
#define SW_RETENTION_WDT_FLAG_POS                   (16)
#define SW_RETENTION_VAL_MASK                       (0XFFFF)
#define SW_RETENTION_VAL_POS                        (0)
#endif


typedef union
{
    UINT32 val;
    struct
    {
        UINT32 mclk_mux: 2;
        UINT32 resv0: 2;
        UINT32 mclk_div: 4;
        UINT32 flash_26m_select: 1;
        UINT32 hclk_div2_en: 1;
        UINT32 modem_clk480m_pwd: 1;
        UINT32 mac_clk480m_pwd: 1;
        UINT32 mpif_clk_inv: 1;
        UINT32 sdio_clk_inv: 1;
        UINT32 resv1: 18;
    } bits;
} SYS_CTRL_U;

typedef struct efuse_oper_st
{
    UINT8 addr;
    UINT8 data;
} EFUSE_OPER_ST, *EFUSE_OPER_PTR;

typedef enum
{
    INTERNAL_HW_MODE = 0,
    INTERNAL_SW_MODE = 1,
    EXTERNAL_HW_MODE = 2,
    EXTERNAL_SW_MODE = 3,
} CHARGE_TYPE;

typedef enum
{
    STEP_STOP = 0,
    STEP_START = 1,
    STEP_TRICKLE = 2,
    STEP_EXTER_CC = 3,
    STEP_INTER_CC = 4,
    STEP_INTER_CV = 5,

} CHARGE_STEP;

typedef struct charge_oper_st
{
    CHARGE_TYPE type;
    CHARGE_STEP step;
    UINT32 elect;
    UINT8 cal[3];
} CHARGE_OPER_ST, *CHARGE_OPER_PTR;

#define CHARGE_ANALOG_CTRL3_CAL_DEFAULT_VALUE       (0x180004A0)
#define CHARGE_ANALOG_CTRL3_CHARGE_DEFAULT_VALUE    (0x180704A0)
#define CHARGE_ANALOG_CTRL4_CAL_DEFAULT_VALUE       (0xC2400520)
#define CHARGE_ANALOG_CTRL4_CHARGE_DEFAULT_VALUE    (0xC2401520)


#define AUDIO_DAC_VOL_DIFF_MODE                      (0)
#define AUDIO_DAC_VOL_SINGLE_MODE                    (1)

#define AUDIO_DAC_ANALOG_UNMUTE                      (0)
#define AUDIO_DAC_ANALOG_MUTE                        (1)

#define EFUSE_ENCRYPT_WORD_ADDR                      (0)
#define EFUSE_ENCRYPT_WORD_LEN                       (16)
#define EFUSE_CHARGE_CAL_ADDR                        (16)
#define EFUSE_CHARGE_CAL_LEN                         (4)
#define EFUSE_UID_ADDR                               (20)
#define EFUSE_UID_LEN                                (4)
#define EFUSE_MAC_START_ADDR                         (24)
#define EFUSE_MAC_LEN                                (6)
#define EFUSE_USER_AREA_ADDR                         (30)
#define EFUSE_USER_AREA_LEN                          (1)
#define EFUSE_CTRL_ADDR                              (31)
#define EFUSE_USER_AREA_LEN                          (1)
#define EFUSE_INIT_VAL                               (0x0)

#define EFUSE_CTRL_JTAG_DISABLE                      (1 << 7)
#define EFUSE_CTRL_FLASH_DOWNLOAD_DISABLE            (1 << 6)
#define EFUSE_CTRL_ENCRYPT_EN                        (1 << 5)
#define EFUSE_CTRL_ENCRYPT_DISABLE_READ              (1 << 4)
#define EFUSE_CTRL_ENCRYPT_DISABLE_WRITE             (1 << 3)
#define EFUSE_CTRL_UID_DISABLE_WRITE                 (1 << 2)
#define EFUSE_CTRL_MAC_DISABLE_WRITE                 (1 << 1)
#define EFUSE_CTRL_ALL_AREA_DISABLE_WRITE            (1 << 0)

typedef void (*sctrl_cal_bias_cb_t)(void);
typedef void (*sctrl_wifi_phy_wakeup_rf_reinit_cb_t)(void);
typedef void (*sctrl_wifi_phy_wakeup_wifi_reinit_cb_t)(void);

/*******************************************************************************
* Function Declarations
*******************************************************************************/
extern void sctrl_init(void);
extern void sctrl_exit(void);
extern void sctrl_normal_exit_sleep(void);
extern void sctrl_normal_enter_sleep(UINT32 peri_clk);
extern void sctrl_mcu_exit(void);
extern void sctrl_mcu_init(void);
extern void sctrl_mcu_sleep(UINT32 );
extern UINT32 sctrl_mcu_wakeup(void);
extern void sctrl_ps_dump(void);
extern void sctrl_flash_select_dco(void);
extern UINT32 charger_is_full(void);
extern UINT32 usb_power_is_pluged(void);
extern UINT32 sctrl_get_deep_sleep_wake_soure(void);
extern void rf_ps_enable_set(void);
extern void rf_ps_enable_clear(void);
extern int rf_ps_enabled(void);
extern UINT32 sctrl_get_deep_sleep_gpio_floating_map(void);
extern int bk_misc_wakeup_get_gpio_num(void);
extern int bk_init_deep_wakeup_gpio_status(void);
extern UINT32 sctrl_get_deep_sleep_gpio_last_floating_map(void);
extern void sctrl_set_deep_sleep_gpio_last_floating_map(UINT32);
extern void sctrl_set_deep_sleep_gpio_floating_map(UINT32);
extern void sctrl_register_cal_bias_callback(sctrl_cal_bias_cb_t cb);
extern void sctrl_register_wifi_phy_wakeup_rf_reinit_callback(sctrl_wifi_phy_wakeup_rf_reinit_cb_t cb);
extern void sctrl_register_wifi_phy_wakeup_wifi_reinit_callback(sctrl_wifi_phy_wakeup_wifi_reinit_cb_t cb);

#ifdef __cplusplus
}
#endif

