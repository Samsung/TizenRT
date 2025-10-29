#ifndef __PMU_H__
#define __PMU_H__

#define PMU_BASE                                     (0x00800200)

#define SCTRL_LOW_PWR_CLK                     (PMU_BASE + 0 * 4)
#define LPO_CLK_MUX_POSI                         (0)
#define LPO_CLK_MUX_MASK                         (0x3)
#define LPO_SRC_ROSC                             (0x0)
#define LPO_SRC_32K_XTAL                         (0x1)
#define LPO_SRC_32K_DIV                          (0x2)

#define SCTRL_SLEEP                           (PMU_BASE + 1 * 4)
#define PROCORE_DLY_POSI                            (20)
#define PROCORE_DLY_MASK                            (0xF)
#define DCO_PWD_SLEEP_BIT                        (1 << 18)
#define FLASH_PWD_SLEEP_BIT                      (1 << 17)
#define ROSC_PWD_DEEPSLEEP_BIT                   (1 << 16)
#define SLEEP_MODE_POSI                          (0)
#define SLEEP_MODE_MASK                          (0xFFFF)
#define SLEEP_MODE_CFG_NORMAL_VOL_WORD           (0x4F89)
#define SLEEP_MODE_CFG_LOW_VOL_WORD              (0xB706)
#define SLEEP_MODE_CFG_DEEP_WORD                 (0xADC1)

#define SCTRL_DIGTAL_VDD                      (PMU_BASE + 2 * 4)
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

#define SCTRL_PWR_MAC_MODEM                   (PMU_BASE + 3 * 4)
#define MODEM_PWD_POSI                           (0)
#define MODEM_PWD_MASK                           (0xFFFFU)
#define MODEM_PWD                                (0xD802U)
#define MODEM_PWU                                (0x0000U)

#define SCTRL_DSP_PWR                         (PMU_BASE + 4 * 4)
#define DSP_PWD_POSI                             (0)
#define DSP_PWD_MASK                             (0xFFFF)
#define DSP_PWD                                  (0xD410)
#define DSP_PWU                                  (0x0000)

#define ICU_TL410_CLK_PWD                            SCTRL_DSP_PWR //need_confirm
#define TL410_CLK_PWD_VALUE                          (0xD410)

#define SCTRL_BT_PWR                         (PMU_BASE + 5 * 4)
#define BT_PWD_POSI                             (0)
#define BT_PWD_MASK                             (0xFFFF)
#define BT_PWD                                  (0xDB1E)
#define BT_PWU                                  (0x0000)

#define SCTRL_BLOCK_EN_CFG                    (PMU_BASE + 9 * 4)
#define BLOCK_EN_WORD_POSI                       (20)
#define BLOCK_EN_WORD_MASK                       (0xFFFUL)
#define BLOCK_EN_WORD_PWD                        (0xA5CUL)

#define BLOCK_EN_VALID_MASK                      (0xFFFFF)
#define BLK_BIT_MIC_QSPI_RAM_OR_FLASH            (1 << 19)
#define BLK_BIT_LCD                              (1 << 18)
#define BLK_BIT_AUDIO_PLL                        (1 << 17)
#define BLK_BIT_AUDIO_RANDOM_GENERATOR           (1 << 16)
#define BLK_BIT_USB2                             (1 << 15)
#define BLK_BIT_USB                              (1 << 14)
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

#define SCTRL_PMU_STATUS                         (PMU_BASE + 6 * 4)
#define SYNC_BUS_PWD_POSI                        (0)
#define SYNC_BUS_PWD_MASK                        (0xFFFF)
#define SYNC_BUS_PWD                             (0xD884)
#define SYNC_BUS_PWU                             (0x0000)

#define SCTRL_BLOCK_EN_MUX                     (PMU_BASE + 11 * 4)

#define PMU_RTC_WKU                            (PMU_BASE + 15 * 4)

#define PMU_CO_MCU_CONFIG                      (PMU_BASE + 0x13 * 4)
#define BT_CLK_ENABLE                            (1 << 05)
#define DSP_CLK_ENABLE                           (1 << 04)
#define BT_SW_RESET_ENABLE                       (1 << 03)
#define DSP_SW_RESET_ENABLE                      (1 << 02)
#define BT_PROGRAM_DOWNLOAD_EN                   (1 << 01)
#define DSP_PROGRAM_DOWNLOAD_EN                  (1 << 00)

#define PMU_STATUS                             (PMU_BASE + 18 * 4)
#define PMU_BAIS_CAL_OUT_POSI                    (12)
#define PMU_BAIS_CAL_OUT_MASK                    (0x1F)
#endif
// eof


