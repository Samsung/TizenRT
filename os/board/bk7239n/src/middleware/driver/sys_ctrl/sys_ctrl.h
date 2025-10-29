#ifndef _SCTRL_H_
#define _SCTRL_H_

#include <components/log.h>
#include <common/sys_config.h>
#include <common/bk_include.h>

#if (defined(CONFIG_SOC_BK7271))
#include "bk_lbus.h"
#endif

#define SCTRL_DEBUG   0

#if SCTRL_DEBUG
#define SCTRL_TAG "sctrl"
#define SCTRL_LOGI(...) BK_LOGI(SCTRL_TAG, ##__VA_ARGS__)
#define SCTRL_LOGW(...) BK_LOGW(SCTRL_TAG, ##__VA_ARGS__)
#define SCTRL_LOGE(...) BK_LOGE(SCTRL_TAG, ##__VA_ARGS__)
#define SCTRL_LOGD(...) BK_LOGD(SCTRL_TAG, ##__VA_ARGS__)
#else
#define SCTRL_LOGI     os_null_printf
#endif

#if SCTRL_DEBUG
#define SCTRL_PRT      BK_LOGI
#else
#define SCTRL_PRT      os_null_printf
#endif

#define DPLL_DELAY_EN                         1
#define DPLL_DELAY_TIME_10US	              120
#define DPLL_DELAY_TIME_200US	              3400

#define SCTRL_BASE                            (0x00800000)

#define SCTRL_CHIP_ID                         (SCTRL_BASE + 00 * 4)
#define CHIP_ID_DEFAULT                          (0x7111)
#define CHIP_ID_BK7231N                          (0x7231C)

#define SCTRL_DEVICE_ID                       (SCTRL_BASE + 01 * 4)
#define DEVICE_ID_MASK                           (0xFFFF0000) //as wangjian advised, only check high 16 bits
#define DEVICE_ID_DEFAULT                        (0x20150414)
#define DEVICE_ID_BK7231N_B                      (0x20521024)
#define DEVICE_ID_BK7231N_C                      (0x20521025)
#define DEVICE_ID_BK7231N_D                      (0x20521026)
#define DEVICE_ID_BK7231N_E                      (0x20521027)
#define DEVICE_ID_BK7231N_F                      (0x20521028)
#define DEVICE_ID_BK7231N_N                      (0x20521010)
#define DEVICE_ID_BK7231N_P                      (0x20A21020)

#define SCTRL_CONTROL                         (SCTRL_BASE + 02 * 4)

#if (defined(CONFIG_SOC_BK7271))
#define BT_IMEM_BASE_ADDR                     (0x10000000)

#define MAC_HCLK_EN_BIT                          (1 << 27)
#define PHY_HCLK_EN_BIT                          (1 << 26)
#define MTB_PRIVILEGE_POSI                          (24)
#define MTB_PRIVILEGE_MASK                          (0x3)
#define MTB_PRIVILEGE_ACCESS_NONE                   (0x0)
#define MTB_PRIVILEGE_ACCESS_AHB_PART               (0x1)
#define MTB_PRIVILEGE_ACCESS_AHB                    (0x2)
#define MTB_PRIVILEGE_ACCESS_AHB_DTCM               (0x3)

#define addPMU_Reg0xd                                           *((volatile unsigned long *) (0x00800200+0xd*4))
#define addPMU_Reg0xf                                           *((volatile unsigned long *) (0x00800200+0xf*4))
#define addGPIO_Reg0xf                                          *((volatile unsigned long *) (0x00800300+0xf*4))
#endif

#if (defined(CONFIG_SOC_BK7251))
#define EFUSE_VDD25_EN                           (1 << 23)

#if (defined(CONFIG_SOC_BK7251)) || (defined(CONFIG_SOC_BK7271))
#define FLASH_SPI_MUX_BIT                        (1 << 22)
#endif // (CONFIG_SOC_BK7251)

#define PSRAM_VDDPAD_VOLT_POSI                   (20)
#define PSRAM_VDDPAD_VOLT_MASK                   (0x3)
#define FLASH_SCK_IOCAP_POSI                     (18)
#define FLASH_SCK_IOCAP_MASK                     (0x3)
#define QSPI_IO_VOLT_POSI                        (16)
#define QSPI_IO_VOLT_MASK                        (0x3)
#define BLE_RF_EN_BIT                            (1 << 15)
#elif (defined(CONFIG_SOC_BK7231U))
#define EFUSE_VDD25_EN                           (1 << 23)
#define PSRAM_VDDPAD_VOLT_POSI                   (20)
#define PSRAM_VDDPAD_VOLT_MASK                   (0x3)
#define FLASH_SCK_IOCAP_POSI                     (18)
#define FLASH_SCK_IOCAP_MASK                     (0x3)
#define QSPI_IO_VOLT_POSI                        (16)
#define QSPI_IO_VOLT_MASK                        (0x3)
#define BLE_RF_EN_BIT                            (1 << 15)
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define BLE_RF_PTA_EN_BIT                        (1 << 24)
#define EFUSE_VDD25_EN                           (1 << 23)
#define FLASH_SPI_MUX_BIT                        (1 << 22)
#define FLASH_SCK_IOCAP_POSI                     (18)
#define FLASH_SCK_IOCAP_MASK                     (0x3)
#define BLE_RF_EN_BIT                            (1 << 15)
#elif (!defined(CONFIG_SOC_BK7231))
#define EFUSE_VDD25_EN                           (1 << 23)
#define PSRAM_VDDPAD_VOLT_POSI                   (20)
#define PSRAM_VDDPAD_VOLT_MASK                   (0x3)
#define FLASH_SCK_IOCAP_POSI                     (18)
#define FLASH_SCK_IOCAP_MASK                     (0x3)
#define QSPI_IO_VOLT_POSI                        (16)
#define QSPI_IO_VOLT_MASK                        (0x3)
#define BLE_RF_EN_BIT                            (1 << 15)
#endif // (CONFIG_SOC_BK7251)

#define DPLL_CLKDIV_RESET_BIT                    (1 << 14)
#define SDIO_CLK_INVERT_BIT                      (1 << 13)
#define MPIF_CLK_INVERT_BIT                      (1 << 12)
#define MAC_CLK480M_PWD_BIT                      (1 << 11)
#define MODEM_CLK480M_PWD_BIT                    (1 << 10)

#if (defined(CONFIG_SOC_BK7271))
#define HCLK_DIV2_POSI                           (2)
#define HCLK_DIV2_MASK                           (0x3)
#define HCLK_DIV2_EN_BIT                         (1 << 2)
#else
#define HCLK_DIV2_EN_BIT                         (1 << 9)
#endif

#define FLASH_26M_MUX_BIT                        (1 << 8)

#define MCLK_DIV_MASK                            (0x0F)
#define MCLK_DIV_POSI                            (4)

#define MCLK_MUX_MASK                            (0x03)
#define MCLK_MUX_POSI                            (0)
#define MCLK_FIELD_DCO                           (0x0)
#define MCLK_FIELD_26M_XTAL                      (0x1)
#define MCLK_FIELD_DPLL                          (0x2)
#define MCLK_FIELD_LPO                           (0x3)

#define SCTRL_CLK_GATING                      (SCTRL_BASE + 3 * 4)
#define MAC_PI_CLKGATING_BIT                     (1 << 8)
#define MAC_PI_RX_CLKGATING_BIT                  (1 << 7)
#define MAC_PI_TX_CLKGATING_BIT                  (1 << 6)
#define MAC_PRI_CLKGATING_BIT                    (1 << 5)
#define MAC_CRYPT_CLKGATING_BIT                  (1 << 4)
#define MAC_CORE_TX_CLKGATING_BIT                (1 << 3)
#define MAC_CORE_RX_CLKGATING_BIT                (1 << 2)
#define MAC_WT_CLKGATING_BIT                     (1 << 1)
#define MAC_MPIF_CLKGATING_BIT                   (1 << 0)

#define SCTRL_RESET                           (SCTRL_BASE + 4 * 4)
#if (defined(CONFIG_SOC_BK7271))
#define SCTRL_RESET_MASK                         (0x1FFFFF)
#define MAC_SUBSYS_RESET_BIT                     (1 << 20)
#define MODEM_CORE_RESET_BIT                     (1 << 19)
#define MODEM_SUBCHIP_RESET_BIT                  (1 << 16)
#else
#define SCTRL_RESET_MASK                         (0x7F)

#if (!defined(CONFIG_SOC_BK7231))
#define BLE_SUBSYS_RESET                         (1 << 8)
#endif // (!CONFIG_SOC_BK7231)

#define MAC_WAKEUP_ARM                           (1 << 7)
#define MODEM_CORE_RESET_BIT                     (1 << 6)

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define BOOT_MODE_BIT                            (1 << 9)
#define MAC_SUBSYS_RESET_BIT                     (1 << 2)
#else
#define TL410_EXT_WAIT_BIT                       (1 << 5)
#define USB_SUBSYS_RESET_BIT                     (1 << 4)
#define TL410_BOOT_BIT                           (1 << 3)
#define MAC_SUBSYS_RESET_BIT                     (1 << 2)
#define DSP_SUBSYS_RESET_BIT                     (1 << 1)
#endif

#define MODEM_SUBCHIP_RESET_BIT                  (1 << 0)
#endif

#define SCTRL_REG_RESV5                       (SCTRL_BASE + 5 * 4)
#define SCTRL_REG_RESV6                       (SCTRL_BASE + 6 * 4)

#define SCTRL_FLASH_DELAY                     (SCTRL_BASE + 7 * 4)
#define FLASH_DELAY_CYCLE_POSI                   (0)
#define FLASH_DELAY_CYCLE_MASK                   (0xFFF)

#define SCTRL_MODEM_SUBCHIP_RESET_REQ         (SCTRL_BASE + 8 * 4)

#if (defined(CONFIG_SOC_BK7271))
#define MODEM_SUBCHIP_RESET_WORD                 (0x7171e802)
#else
#define MODEM_SUBCHIP_RESET_WORD                 (0x7111e802)
#endif

#if (defined(CONFIG_SOC_BK7271))
#define SCTRL_MAC_SUBSYS_RESET_REQ            (SCTRL_BASE + 12 * 4)
#define MAC_SUBSYS_RESET_POSI                    (0)
#define MAC_SUBSYS_RESET_MASK                    (0xFFFF)
#define MAC_SUBSYS_RESET_WORD                    (0xE802U)
#else
#define SCTRL_MAC_SUBSYS_RESET_REQ            (SCTRL_BASE + 9 * 4)
#define MAC_SUBSYS_RESET_WORD                    (0x7111C802)
#endif

#define SCTRL_USB_SUBSYS_RESET_REQ            (SCTRL_BASE + 10 * 4)
#define USB_SUBSYS_RESET_WORD                    (0x7111C12B)

#define SCTRL_DSP_SUBSYS_RESET_REQ            (SCTRL_BASE + 11 * 4)
#define DSP_SUBSYS_RESET_WORD                    (0x7111C410)

#define SCTRL_MODEM_CORE_RESET_PHY_HCLK       (SCTRL_BASE + 12 * 4)
#define MODEM_CORE_RESET_POSI                    (16)
#define MODEM_CORE_RESET_MASK                    (0xFFFF)
#define MODEM_CORE_RESET_WORD                    (0xE802U)
#if (!defined(CONFIG_SOC_BK7271))
#define MAC_HCLK_EN_BIT                          (1 << 1)
#define PHY_HCLK_EN_BIT                          (1 << 0)
#endif

#define SCTRL_DEBUG13                         (SCTRL_BASE + 13 * 4)
#define DBGMACCLKSEL_POSI                        (20)
#define DBGMACCLKSEL_MASK                        (0x1)
#define SMPSOURCE_POSI                           (16)
#define SMPSOURCE_MASK                           (0xF)
#define DBGMDMBANK1MUX_POSI                      (8)
#define DBGMDMBANK1MUX_MASK                      (0x1F)
#define DBGMDMBANK0MUX_POSI                      (02)
#define DBGMDMBANK0MUX_MASK                      (0x1F)

#define SCTRL_DEBUG14_MODEM_BANK              (SCTRL_BASE + 14 * 4)
#define SCTRL_DEBUG15_MAC_BANK                (SCTRL_BASE + 15 * 4)


#define SCTRL_ANALOG_SPI                      (SCTRL_BASE + 16 * 4)
#define ANA_SPI_FREQ_DIV_POSI                    (26)
#define ANA_SPI_FREQ_DIV_MASK                    (0x3F)
#if (defined(CONFIG_SOC_BK7231))
#define ANA_SPI_STATE_POSI                       (0)
#define ANA_SPI_STAET_MASK                       (0x1F)
#elif (defined(CONFIG_SOC_BK7231U)) || (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define ANA_SPI_STATE_POSI                       (0)
#define ANA_SPI_STAET_MASK                       (0x7F)
#elif (defined(CONFIG_SOC_BK7251))
#define ANA_SPI_STATE_POSI                       (0)
#define ANA_SPI_STAET_MASK                       (0x7FF)
#elif (defined(CONFIG_SOC_BK7271))
#define ANA_SPI_STATE_POSI                       (0)
#define ANA_SPI_STAET_MASK                       (0xFFFF)
#endif

#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
#define SCTRL_LA_SAMPLE                       (SCTRL_BASE + 18 * 4)
#define LA_SMP_LEN_POSI                          (16)
#define LA_SMP_LEN_MASK                          (0xFFFF)
#define LA_SMP_FINISH_BIT                        (1 << 3)
#define LA_SMP_CLK_INVERT                        (1 << 2)
#define LA_SMP_MODE_POSI                         (0)
#define LA_SMP_MODE_MASK                         (0x3)

#define SCTRL_LA_SAMPLE_VALUE                 (SCTRL_BASE + 19 * 4)
#define SCTRL_LA_SAMPLE_MASK                  (SCTRL_BASE + 20 * 4)
#define SCTRL_LA_SAMPLE_DMA_START_ADDR        (SCTRL_BASE + 21 * 4)
#endif

#define SCTRL_ANALOG_CTRL0                    (SCTRL_BASE + 22 * 4)
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define LPEN_DPLL                                (1 << 27)
#endif
#define SPI_TRIG_BIT                             (1 << 19)
#define SPI_DET_EN                               (1 << 4)

#define SCTRL_ANALOG_CTRL1                    (SCTRL_BASE + 23 * 4)
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define ROSC_TSTEN_BIT                           (1 << 31)
#define DCO_TSTEN_BIT                            (1 << 30)
#define DCO_DIV_POSI                             (27)
#define DCO_DIV_MASK                             (0x7U)
#define DCO_AMSEL_BIT                            (1 << 26)
#define SPI_RST_BIT                              (1 << 25)
#define DCO_CNTI_POSI                            (16)
#define DCO_CNTI_MASK                            (0x1FFU)
#define DCO_TRIG_BIT                             (1 << 15)
#define DCO_ICTRL_POSI                           (11)
#define DCO_ICTRL_MASK                           (0x7U)
#define DCO_MSW_POSI                             (2)
#define DCO_MSW_MASK                             (0x1FFU)
#define DPLL_VREFSEL_BIT                         (1 << 1)
#else
#define DIV_BYPASS_BIT                           (1 << 31)
#define SPI_RST_BIT                              (1 << 25)
#define DCO_CNTI_POSI                            (16)
#define DCO_CNTI_MASK                            (0x1FFU)
#define DCO_TRIG_BIT                             (1 << 15)
#define DCO_DIV_POSI                             (12)
#define DCO_DIV_MASK                             (0x3U)
#endif

#define SCTRL_ANALOG_CTRL2                    (SCTRL_BASE + 24 * 4)
#define CENTRAL_BAIS_ENABLE_BIT                  (1 << 13)
#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define XTALH_CTUNE_POSI                         (2)
#define XTALH_CTUNE_MASK                         (0x7FU)
#define BANDGAP_CAL_MANUAL_POSI                  (23)
#define BANDGAP_CAL_MANUAL_MASK                  (0x3FU)
#elif (!defined(CONFIG_SOC_BK7231)) && (!defined(CONFIG_SOC_BK7271))
#define XTALH_CTUNE_POSI                         (2)
#define XTALH_CTUNE_MASK                         (0x3FU)
#endif // (CONFIG_SOC_BK7231)
#define TRXT_TST_ENABLE_BIT                      (1 << 12)

#define SCTRL_ANALOG_CTRL3                    (SCTRL_BASE + 25 * 4)
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define CHARGE_LC2CVDLYLV_MASK                 (0x7)
#define CHARGE_LC2CVDLYLV_POS                  (29)
#define CHARGE_VLCSWLV_MASK                     (0xF)
#define CHARGE_VLCSWLV_POS                      (23)
#define CHARGE_LCP_MASK                        (0x1F)
#define CHARGE_LCP_POS                         (8)
#endif

#define SCTRL_ANALOG_CTRL4                    (SCTRL_BASE + 26 * 4)
#if (defined(CONFIG_SOC_BK7251))
#define VSEL_SYS_LDO_POSI                        (27)
#define VSEL_SYS_LDO_MASK                        (0x3)
#define CHARGE_MANMODE_MASK                      (0x7)
#define CHARGE_MANMODE_POS                       (16)
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define VREF_SEL_BIT                             (1 << 30)
#define GADC_CAL_SEL_POSI                        (19)
#define GADC_CAL_SEL_MASK                        (0x3)
#elif (defined(CONFIG_SOC_BK7271))
#define BT_ENABLE_1V                             (1 << 23)
#define BT_ENaBLE_LDO_1V                         (1 << 22)
#define XTALH_CTUNE_POSI                         (7)
#define XTALH_CTUNE_MASK                         (0x3FU)
#else
#endif

#define SCTRL_ANALOG_CTRL5                    (SCTRL_BASE + 0x1B*4)

#if (defined(CONFIG_SOC_BK7271))
#define SCTRL_ANALOG_CTRL6               (SCTRL_BASE + 0x1C*4)
#define SCTRL_ANALOG_CTRL7               (SCTRL_BASE + 0x1D*4)
#define SCTRL_ANALOG_CTRL8_REAL               (SCTRL_BASE + 0x1E*4)
#define SCTRL_ANALOG_CTRL9_REAL               (SCTRL_BASE + 0x1F*4)
#define ANALOG_LDO_ENABLE                     (1 << 13)
#define VBG_SEL_HIGH_POSI                     (10)
#define VBG_SEL_HIGH_MASK                     (0x3)
#define VBG_SEL_LOW_POSI                      (8)
#define VBG_SEL_LOW_MASK                      (0x3)

#define SCTRL_ANALOG_CTRL8                    (SCTRL_BASE + 0x20*4) //ana_regA actually
#define LINE_IN_EN                               (1 << 21)
#define LINE2_IN_EN                              (1 << 20)
#define LINE_IN_GAIN_MASK                        (0x3)
#define LINE_IN_GAIN_POSI                        (22)
#define AUD_DAC_GAIN_MASK                        (0x3F)
#define AUD_DAC_GAIN_POSI                        (1)
#define AUD_DAC_MUTE_EN                          (1 << 0)

#define SCTRL_ANALOG_CTRL9                    (SCTRL_BASE + 0x21*4) //ana_regB actually
#define DAC_DIFF_EN                              (1 << 31)
#define EN_AUD_DAC_L                             (1 << 30)
#define EN_AUD_DAC_R                             (1 << 29)
#define DAC_PA_OUTPUT_EN                         (1 << 24)
#define DAC_DRIVER_OUTPUT_EN                     (1 << 23)
#define AUD_DAC_DGA_EN                           (1 << 6)

#define SCTRL_ANALOG_CTRL10                   (SCTRL_BASE + 0x22*4) //ana_regC actually
#define DAC_N_END_OUPT_L                         (1 << 8)
#define DAC_N_END_OUPT_R                         (1 << 7)
#define DAC_VSEL_MASK                            (0x3)
#define DAC_VSEL_POSI                            (1)

#define SCTRL_ANALOG_CTRL13                   (SCTRL_BASE + 0x23*4) //ana_regD for audio
#define AUDIO_DCO_EN                             (1 << 10)
#define MIC1_PWR_DOWN                            (1 << 29)
#define MIC2_PWR_DOWN                            (1 << 24)
#define MIC3_PWR_DOWN                            (1 << 19)
#define MIC5_PWR_DOWN                            (1 << 15)
#define MIC4_PWR_DOWN                            (1 << 14)
#define MIC6_PWR_DOWN                            (1 << 12)

#define SCTRL_ANALOG_AUDIO_PLL_SDM            (SCTRL_BASE + 0x24*4) //ana_regE for audio

#define SCTRL_ANALOG_AUDIO_PLL_CTRL           (SCTRL_BASE + 0x25*4) //ana_regF for audio
#define AUDIO_PLL_AUDIO_EN                       (1 << 31)
#define AUDIO_PLL_CKSEL                          (1 << 29)
#define AUDIO_PLL_SPI_TRIGGER                    (1 << 18)
#define AUDIO_PLL_RESET                          (1 << 3)

#elif (!defined(CONFIG_SOC_BK7231))
#define SCTRL_ANALOG_CTRL6                    (SCTRL_BASE + 0x1C*4)
#define DPLL_CLK_FOR_AUDIO_EN                    (1 << 31)
#define DPLL_CLK_FOR_USB_EN                      (1 << 30)
#define DPLL_DIVIDER_CLK_SEL                     (1 << 29)
#define DPLL_DIVIDER_CTRL_MASK                   (0x7)
#define DPLL_DIVIDER_CTRL_POSI                   (26)
#define DPLL_CLKOUT_PAD_EN                       (1 << 25)
#define DPLL_XTAL26M_CLK_AUDIO_EN                (1 << 24)
#define DPLL_REF_CLK_SELECT                      (1 << 23)
#define DPLL_CHARGE_PUMP_CUR_CTRL_MASK           (0x7)
#define DPLL_CHARGE_PUMP_CUR_CTRL_POSI           (20)
#define DPLL_DPLL_VCO_BAND_MANUAL_EN             (1 << 19)
#define DPLL_SPI_TRIGGER                         (1 << 18)
#define DPLL_CLK_REF_LOOP_SEL                    (1 << 17)
#define DPLL_KVCO_CTRL_MASK                      (0x3)
#define DPLL_KVCO_CTRL_POSI                      (15)
#define DPLL_VSEL_CAL                            (1 << 14)
#define DPLL_RP_CTRL_LPF_MASK                    (0x7)
#define DPLL_RP_CTRL_LPF_POSI                    (11)
#define DPLL_RESET                               (1 << 3)

#define SCTRL_EFUSE_CTRL                      (SCTRL_BASE + 0x1D*4)
#define EFUSE_OPER_EN                            (1 << 0)
#define EFUSE_OPER_DIR                           (1 << 1)
#define EFUSE_OPER_ADDR_POSI                     (8)
#define EFUSE_OPER_ADDR_MASK                     (0x1F)
#define EFUSE_OPER_WR_DATA_POSI                  (16)
#define EFUSE_OPER_WR_DATA_MASK                  (0xFF)

#define SCTRL_EFUSE_OPTR                      (SCTRL_BASE + 0x1E*4)
#define EFUSE_OPER_RD_DATA_POSI                  (0)
#define EFUSE_OPER_RD_DATA_MASK                  (0xFF)
#define EFUSE_OPER_RD_DATA_VALID                 (1 << 8)


#define SCTRL_DMA_PRIO_VAL                    (SCTRL_BASE + 0x1F*4)

#define SCTRL_BLE_SUBSYS_RESET_REQ            (SCTRL_BASE + 0x20*4)
#if (defined(CONFIG_SOC_BK7251))
#define SCTRL_CHARGE_STATUS                   (SCTRL_BASE + 0x21*4)
#define CHARGE_VCAL_MASK                        (0x3F)
#define CHARGE_VCAL_POS                         (5)
#define CHARGE_LCAL_MASK                        (0x1F)
#define CHARGE_LCAL_POS                         (11)
#define CHARGE_VCVCAL_MASK                      (0x1F)
#define CHARGE_VCVCAL_POS                       (0)

#define SCTRL_ANALOG_CTRL7                    (SCTRL_BASE + 0x22*4)

#define SCTRL_ANALOG_CTRL8                    (SCTRL_BASE + 0x23*4)
#define LINE_IN_EN                               (1 << 24)
#define LINE_IN_GAIN_MASK                        (0x3)
#define LINE_IN_GAIN_POSI                        (22)
#define SPI_PWD_AUD_ADC_L                        (1 << 21)
#define SPI_PWD_AUD_ADC_R                        (1 << 20)
#define AUD_DAC_GAIN_MASK                        (0x1F)
#define AUD_DAC_GAIN_POSI                        (2)
#define AUD_DAC_MUTE_EN                          (1 << 0)

#define SCTRL_ANALOG_CTRL9                    (SCTRL_BASE + 0x24*4)
#define DAC_DIFF_EN                              (1 << 31)
#define EN_AUD_DAC_L                             (1 << 30)
#define EN_AUD_DAC_R                             (1 << 29)
#define DAC_PA_OUTPUT_EN                         (1 << 24)
#define DAC_DRIVER_OUTPUT_EN                     (1 << 23)
#define AUD_DAC_DGA_EN                           (1 << 6)

#define SCTRL_ANALOG_CTRL10                   (SCTRL_BASE + 0x25*4)
#define DAC_N_END_OUPT_L                         (1 << 8)
#define DAC_N_END_OUPT_R                         (1 << 7)
#define DAC_VSEL_MASK                            (0x3)
#define DAC_VSEL_POSI                            (1)
#elif (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define SCTRL_ANALOG_STATE                    (SCTRL_BASE + 0x21*4)
#define ANALOG_STATE_DPLL_UNLOCK                 (1 << 15)
#define ANALOG_STATE_BGCAL_MASK                  (0x3FU)
#define ANALOG_STATE_BGCAL_POSI                  (2)
#define ANALOG_STATE_MASK                        (0xFF)
#define ANALOG_STATE_POS                         (0)
#endif // (CONFIG_SOC_BK7251)
#endif // (!CONFIG_SOC_BK7251)

#if (defined(CONFIG_SOC_BK7271))
#define SCTRL_ROSC_CAL                        SCTRL_ANALOG_CTRL2
#define ROSC_CAL_MANU_CIN_POSI                   (1)
#define ROSC_CAL_MANU_CIN_MASK                   (0x3F)
#define ROSC_CAL_MANU_EN_BIT                     (1 << 0)
#define ROSC_CAL_INTVAL_POSI                     (22)
#define ROSC_CAL_INTVAL_MASK                     (0x3FF)
#define ROSC_CAL_MODE_BIT                        (1 << 10)
#define ROSC_CAL_TRIG_BIT                        (1 << 11)
#define ROSC_CAL_EN_BIT                          (1 << 9) //confirmed by huaming

#define SCTRL_BIAS                            SCTRL_ANALOG_CTRL3
#define ROSC_CAL_MANU_FIN_POSI                   (23)
#define ROSC_CAL_MANU_FIN_MASK                   (0x1FF)
#define LDO_VAL_MANUAL_POSI                      (16)
#define LDO_VAL_MANUAL_MASK                      (0x1F)
#define BIAS_CAL_MANUAL_BIT                      (1 << 22)
#define BIAS_CAL_TRIGGER_BIT                     (1 << 21)

#define REG_GPIO_EXTRAL_INT_CFG              (SCTRL_BASE + 42*4)
#define DPLL_UNLOCK_INT_EN                    (1 << 0)
#define AUDIO_DPLL_UNLOCK_INT_EN              (1 << 1)

#define REG_GPIO_EXTRAL_INT_CFG1              (SCTRL_BASE + 43*4)
#define DPLL_UNLOCK_INT                       (1 << 0)
#define AUDIO_DPLL_UNLOCK_INT                 (1 << 1)
#define USB_PLUG_IN_INT_EN                    (1 << 4)
#define USB_PLUG_OUT_INT_EN                   (1 << 5)
#define USB_PLUG_IN_INT                       (1 << 6)
#define USB_PLUG_OUT_INT                      (1 << 7)
#define GPIO_EXTRAL_INT_MASK                  (DPLL_UNLOCK_INT | AUDIO_DPLL_UNLOCK_INT | USB_PLUG_IN_INT | USB_PLUG_OUT_INT)

#else
#define SCTRL_LOW_PWR_CLK                     (SCTRL_BASE + 64 * 4)
#define LPO_CLK_MUX_POSI                         (0)
#define LPO_CLK_MUX_MASK                         (0x3)
#define LPO_SRC_ROSC                             (0x0)
#define LPO_SRC_32K_XTAL                         (0x1)
#define LPO_SRC_32K_DIV                          (0x2)

#define DEEP_SLEEP_LPO_SRC        LPO_SRC_32K_XTAL

#define SCTRL_SLEEP                           (SCTRL_BASE + 65 * 4)
#define PROCORE_DLY_POSI                            (20)
#define PROCORE_DLY_MASK                            (0xF)

#define GPIO_SLEEP_SWITCH_BIT                    (1 << 19)
#define DCO_PWD_SLEEP_BIT                        (1 << 17)
#define FLASH_PWD_SLEEP_BIT                      (1 << 18)

#define ROSC_PWD_DEEPSLEEP_BIT                   (1 << 16)
#define SLEEP_MODE_POSI                          (0)
#define SLEEP_MODE_MASK                          (0xFFFF)
#define SLEEP_MODE_CFG_NORMAL_VOL_WORD           (0x4F89)
#define SLEEP_MODE_CFG_LOW_VOL_WORD              (0xB706)
#define SLEEP_MODE_CFG_DEEP_WORD                 (0xADC1)

#define SCTRL_DIGTAL_VDD                      (SCTRL_BASE + 66 * 4)
#define DIG_VDD_ACTIVE_POSI                      (4)
#define DIG_VDD_ACTIVE_MASK                      (0x7)
#define DIG_VDD_SLEEP_POSI                       (0)
#define DIG_VDD_SLEEP_MASK                       (0x7)
#define VDD_1_DOT_51                             (0x7)
#define VDD_1_DOT_38                             (0x6)
#define VDD_1_DOT_25                             (0x5)
#define VDD_1_DOT_12                             (0x4)
#define VDD_0_DOT_99                             (0x3)
#define VDD_0_DOT_86                             (0x2)
#define VDD_0_DOT_73                             (0x1)
#define VDD_0_DOT_60                             (0x0)

#define SCTRL_PWR_MAC_MODEM                   (SCTRL_BASE + 67 * 4)
#define MAC_PWD_POSI                             (16)
#define MAC_PWD_MASK                             (0xFFFFU)
#define MODEM_PWD_POSI                           (0)
#define MODEM_PWD_MASK                           (0xFFFFU)
#define MAC_PWD                                  (0xD802U)
#define MODEM_PWD                                (0xD802U)
#define MAC_PWU                                  (0x0001U)
#define MODEM_PWU                                (0x0001U)

#define SCTRL_DSP_PWR                         (SCTRL_BASE + 68 * 4)
#define DSP_PWD_POSI                             (0)
#define DSP_PWD_MASK                             (0xFFFF)
#define DSP_PWD                                  (0xD410)
#define DSP_PWU                                  (0x0001)

#define SCTRL_USB_PWR                         (SCTRL_BASE + 69 * 4)
#define BLE_PWD_POSI                             (16)
#define BLE_PWD_MASK                             (0xFFFF)
#define BLE_PWD                                  (0xDB1E)
#define BLE_PWU                                  (0x0001)
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
#define USB_PWD_POSI                             (0)
#define USB_PWD_MASK                             (0xFFFF)
#define USB_PWD                                  (0xD12B)
#define USB_PWU                                  (0x0001)
#endif

#define SCTRL_PMU_STATUS                      (SCTRL_BASE + 70 * 4)
#define PMU_MAC_POWER_DOWN_BIT                   (1 << 3)
#define PMU_MODEM_POWER_DOWN_BIT                 (1 << 2)
#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX)) && (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
#define PMU_DSP_POWER_DOWN_BIT                   (1 << 1)
#define PMU_USB_POWER_DOWN_BIT                   (1 << 0)
#endif

#define SCTRL_ROSC_TIMER                      (SCTRL_BASE + 71 * 4)
#define ROSC_TIMER_PERIOD_POSI                   (16)
#define ROSC_TIMER_PERIOD_MASK                   (0xFFFF)
#define ROSC_TIMER_INT_STATUS_BIT                (1 << 8)
#define ROSC_TIMER_ENABLE_BIT                    (1 << 0)

#define SCTRL_GPIO_WAKEUP_EN                  (SCTRL_BASE + 72 * 4)
#define SCTRL_GPIO_WAKEUP_TYPE                (SCTRL_BASE + 73 * 4)
#define SCTRL_GPIO_WAKEUP_INT_STATUS          (SCTRL_BASE + 74 * 4)
#define GPIO_WAKEUP_ENABLE_FLAG                  (1)
#define GPIO_WAKEUP_TYPE_POSITIVE_EDGE           (0)
#define GPIO_WAKEUP_TYPE_NEGATIVE_EDGE           (1)

#if (!defined(CONFIG_SOC_BK7231N)) && (!defined(CONFIG_SOC_BK7236A)) && (!defined(CONFIG_SOC_BK7256XX))
#define SCTRL_USB_PLUG_WAKEUP                   (SCTRL_BASE + 78 * 4)
#define USB_PLUG_IN_EN_BIT                      (1 << 0)
#define USB_PLUG_OUT_EN_BIT                     (1 << 1)
#define USB_PLUG_IN_INT_BIT                     (1 << 2)
#define USB_PLUG_OUT_INT_BIT                    (1 << 3)
#endif

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX))
#define SCTRL_GPIO_WAKEUP_TYPE_SELECT           (SCTRL_BASE + 78 * 4)
#else
#define SCTRL_GPIO_WAKEUP_EN1                  (SCTRL_BASE + 81 * 4)
#define SCTRL_GPIO_WAKEUP_TYPE1                (SCTRL_BASE + 82 * 4)
#define SCTRL_GPIO_WAKEUP_INT_STATUS1          (SCTRL_BASE + 83 * 4)
#endif

enum {
	SYSCTRL_GPIO00_ID = 0,
	SYSCTRL_GPIO01_ID,
	SYSCTRL_GPIO02_ID,
	SYSCTRL_GPIO03_ID,
	SYSCTRL_GPIO04_ID,
	SYSCTRL_GPIO05_ID,
	SYSCTRL_GPIO06_ID,
	SYSCTRL_GPIO07_ID,
	SYSCTRL_GPIO08_ID,
	SYSCTRL_GPIO09_ID,
	SYSCTRL_GPIO10_ID,
	SYSCTRL_GPIO11_ID,
	SYSCTRL_GPIO12_ID,
	SYSCTRL_GPIO13_ID,
	SYSCTRL_GPIO14_ID,
	SYSCTRL_GPIO15_ID,
	SYSCTRL_GPIO16_ID,
	SYSCTRL_GPIO17_ID,
	SYSCTRL_GPIO18_ID,
	SYSCTRL_GPIO19_ID,
	SYSCTRL_GPIO20_ID,
	SYSCTRL_GPIO21_ID,
	SYSCTRL_GPIO22_ID,
	SYSCTRL_GPIO23_ID,
	SYSCTRL_GPIO24_ID,
	SYSCTRL_GPIO25_ID,
	SYSCTRL_GPIO26_ID,
	SYSCTRL_GPIO27_ID,
	SYSCTRL_GPIO28_ID,
	SYSCTRL_GPIO29_ID,
	SYSCTRL_GPIO30_ID,
	SYSCTRL_GPIO31_ID
};


#define SCTRL_BLOCK_EN_CFG                    (SCTRL_BASE + 75 * 4)
#define BLOCK_EN_WORD_POSI                       (20)
#define BLOCK_EN_WORD_MASK                       (0xFFFUL)
#define BLOCK_EN_WORD_PWD                        (0xA5CUL)

#define BLOCK_EN_VALID_MASK                      (0xFFFFF)

#if (defined(CONFIG_SOC_BK7231))
#define BLK_EN_BIT_LINEIN                        (1 << 19)
#define BLK_EN_MIC_R_CHANNEL                     (1 << 18)
#define BLK_EN_MIC_L_CHANNEL                     (1 << 17)
#define BLK_EN_AUDIO_R_CHANNEL                   (1 << 16)
#define BLK_EN_AUDIO_L_CHANNEL                   (1 << 15)
#elif (defined(CONFIG_SOC_BK7231U)) || (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236A)) || (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define BLK_EN_NC                                (1 << 19)
#define BLK_EN_MIC_QSPI_RAM_OR_FLASH             (1 << 18)
#define BLK_EN_MIC_PGA                           (1 << 17)
#define BLK_EN_AUDIO_PLL                         (1 << 16)
#define BLK_EN_AUDIO_RANDOM_GENERATOR            (1 << 15)
#elif (defined(CONFIG_SOC_BK7251))
#define BLK_EN_NC                                (1 << 19)
#define BLK_EN_MIC_QSPI_RAM_OR_FLASH             (1 << 18)
#define BLK_EN_AUDIO                             (1 << 17)
#define BLK_EN_AUDIO_PLL                         (1 << 16)
#define BLK_EN_AUDIO_RANDOM_GENERATOR            (1 << 15)
#endif // (CONFIG_SOC_BK7231)

#define BLK_EN_SARADC                            (1 << 13)
#define BLK_EN_TEMPRATURE_SENSOR                 (1 << 12)
#define BLK_EN_26M_XTAL_LOW_POWER                (1 << 11)
#define BLK_EN_XTAL2RF                           (1 << 10)
#define BLK_EN_IO_LDO_LOW_POWER                  (1 << 09)
#define BLK_EN_ANALOG_SYS_LDO                    (1 << 8)
#define BLK_EN_DIGITAL_CORE_LDO_LOW_POWER        (1 << 07)
#define BLK_EN_DIGITAL_CORE                      (1 << 06)
#define BLK_EN_DPLL_480M                         (1 << 05)
#define BLK_EN_32K_XTAL                          (1 << 04)
#define BLK_EN_26M_XTAL                          (1 << 03)
#define BLK_EN_ROSC32K                           (1 << 02)
#define BLK_EN_DCO                               (1 << 01)
#define BLK_EN_FLASH                             (1 << 00)

#define SCTRL_BIAS                            (SCTRL_BASE + 76 * 4)
#define BIAS_CAL_OUT_POSI                        (16)
#define BIAS_CAL_OUT_MASK                        (0x1F)
#define LDO_VAL_MANUAL_POSI                      (8)
#define LDO_VAL_MANUAL_MASK                      (0x1F)
#define BIAS_CAL_MANUAL_BIT                      (1 << 4)
#define BIAS_CAL_TRIGGER_BIT                     (1 << 0)

#define SCTRL_ROSC_CAL                        (SCTRL_BASE + 77 * 4)
#define ROSC_CAL_MANU_FIN_POSI                   (22)
#define ROSC_CAL_MANU_FIN_MASK                   (0x1FF)
#define ROSC_CAL_MANU_CIN_POSI                   (17)
#define ROSC_CAL_MANU_CIN_MASK                   (0x1F)
#define ROSC_CAL_MANU_EN_BIT                     (1 << 16)
#define ROSC_CAL_INTVAL_POSI                     (4)
#define ROSC_CAL_INTVAL_MASK                     (0x7)
#define ROSC_CAL_MODE_BIT                        (1 << 2)
#define ROSC_CAL_TRIG_BIT                        (1 << 1)
#define ROSC_CAL_EN_BIT                          (1 << 0)

#define SCTRL_BLOCK_EN_MUX						(SCTRL_BASE + 79 * 4)
#endif
#define SCTRL_ROSC_TIMER_H		                (SCTRL_BASE + 80 * 4)
#define ROSC_TIMER_H_PERIOD_POSI                 (0)
#define ROSC_TIMER_H_PERIOD_MASK                 (0xFFFF)

#define SCTRL_SW_RETENTION                     (SCTRL_BASE + 84 * 4)
#define DCO_CNTI_120M                           (0x127U)  // set DCO out clk with 120M

#include "bk_arm_arch.h"

__inline static UINT32 sctrl_analog_get(UINT32 address)
{
	while (REG_READ(SCTRL_ANALOG_SPI) & (ANA_SPI_STAET_MASK << ANA_SPI_STATE_POSI));
	return REG_READ(address);
}

__inline static void sctrl_analog_set(UINT32 address, UINT32 value)
{
	REG_WRITE(address, value);
	while (REG_READ(SCTRL_ANALOG_SPI) & (ANA_SPI_STAET_MASK << ANA_SPI_STATE_POSI));
}

typedef struct  sctrl_ps_save_values {
	UINT32    peri_clk_cfg;
	UINT32    int_enable_cfg;
} SCTRL_PS_SAVE_VALUES;

#define PS_CLOSE_PERI_CLK            0

/*******************************************************************************
* Function Declarations
*******************************************************************************/
extern UINT32 sctrl_ctrl(UINT32 cmd, void *parm);
extern void sctrl_sub_reset(void);

#if defined(CONFIG_BLE_PS)
void sctrl_ble_ps_init(void);
#endif
#endif // _SCTRL_H_
// eof

