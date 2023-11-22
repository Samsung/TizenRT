#ifndef _AMEBAD2_LSYS_H_
#define _AMEBAD2_LSYS_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_LSYS_PWC
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_PWC_HP_DDRPHY                     ((u32)0x00000001 << 12)          /*!<R/W 0  1: Enable ddr phy core power cut 0: Disable */
#define LSYS_PWC_HP_DDRPHY(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_PWC_HP_DDRPHY(x)                  ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_PWC_LP_BTON                       ((u32)0x00000001 << 9)          /*!<R/W 0  This is BT function total power cut control , the BT off power cut is follow it , and is controlled by BT IP . 1: Enable BT power cut 0: Disable */
#define LSYS_PWC_LP_BTON(x)                        ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_PWC_LP_BTON(x)                    ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_PWC_HP_PLAT                       ((u32)0x00000001 << 3)          /*!<R/W 0  This is the default power dommain. 1: Enable HP plat power cut 0: Disable */
#define LSYS_PWC_HP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_PWC_HP_PLAT(x)                    ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_PWC_LP_PLAT                       ((u32)0x00000001 << 1)          /*!<R/W 0  1: Enable Lp plat power cut 0: Disable */
#define LSYS_PWC_LP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_PWC_LP_PLAT(x)                    ((u32)(((x >> 1) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_POR
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_POR_HP_DDRPHY                     ((u32)0x00000001 << 12)          /*!<R/W 0  0: global reset PD_HP_DDRPHY power domain 1: release reset */
#define LSYS_POR_HP_DDRPHY(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_POR_HP_DDRPHY(x)                  ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_POR_LP_BTON                       ((u32)0x00000001 << 9)          /*!<R/W 0  0: global reset PD_LP_BTON power domain 1: release por */
#define LSYS_POR_LP_BTON(x)                        ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_POR_LP_BTON(x)                    ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_POR_HP_PLAT                       ((u32)0x00000001 << 3)          /*!<R/W 0  0: global reset PD_HP_PLFM power domain 1: release por */
#define LSYS_POR_HP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_POR_HP_PLAT(x)                    ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_POR_LP_PLAT                       ((u32)0x00000001 << 1)          /*!<R/W 0  0: global reset PD_LP_PLAT power domain 1: release por */
#define LSYS_POR_LP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_POR_LP_PLAT(x)                    ((u32)(((x >> 1) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_ISO
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_ISO_HP_DDRPHY                     ((u32)0x00000001 << 12)          /*!<R/W 1  1: isolate signals from PD_HP_DDRPHY to other power domain 0: release iso */
#define LSYS_ISO_HP_DDRPHY(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_ISO_HP_DDRPHY(x)                  ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_ISO_LP_BTON                       ((u32)0x00000001 << 9)          /*!<R/W 1  1: isolate signals from PD_LP_BTON to other power domain 0: release iso */
#define LSYS_ISO_LP_BTON(x)                        ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_ISO_LP_BTON(x)                    ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_ISO_HP_PLAT                       ((u32)0x00000001 << 3)          /*!<R/W 1  1: isolate signals from PD_HP_PLFM to other power domain 0: release iso */
#define LSYS_ISO_HP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_ISO_HP_PLAT(x)                    ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_ISO_LP_PLAT                       ((u32)0x00000001 << 1)          /*!<R/W 1  1: isolate signals from PD_LP_PLAT to other power domain 0: release iso */
#define LSYS_ISO_LP_PLAT(x)                        ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_ISO_LP_PLAT(x)                    ((u32)(((x >> 1) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_FEN_GRP0
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_I2C2                             ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: enable HS I2C1 function 0: reset */
#define APBPeriph_I2C1                             ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: enable HS I2C0 function 0: reset */
#define APBPeriph_I2C0                             ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: enable LS I2C function 0: reset */
#define APBPeriph_WLAFE                            ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: enable WL RFAFE control circuit, shall be 1 when enable WL or BT use WL RFAFE 0: reset */
#define APBPeriph_GPIO                             ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: enable LS GPIO function 0: reset */
#define APBPeriph_CTC                              ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: enable Captouch ADC function 0: reset */
#define APBPeriph_ADC                              ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: enable LS ADC function 0: reset */
#define APBPeriph_LOGUART                          ((u32)(0  << 30) | (0x00000001 << 19) | (0x00000001 << 18)) /*!<R/W 0  Uartlog function reset 1: enable 0: reset */
#define APBPeriph_IPC_LP                           ((u32)(0  << 30) | (0x00000001 << 16))  /*!<R/W 0  1: enable LS IPC function 0: reset */
#define APBPeriph_BTON                             ((u32)(0  << 30) | (0x00000001 << 13))  /*!<R/W 0  BTON function enable 1: enable 0: disable */
#define APBPeriph_AIP                              ((u32)(0  << 30) | (0x00000001 << 12))  /*!<R/W 1  Debug thermal function enable */
#define APBPeriph_THM                              ((u32)(0  << 30) | (0x00000001 << 11))  /*!<R/W 1  Debug thermal function enable */
#define APBPeriph_DTIM                             ((u32)(0  << 30) | (0x00000001 << 10))  /*!<R/W 1  Debug timer function enable */
#define APBPeriph_SCE                              ((u32)(0  << 30) | (0x00000001 << 9))   /*!<R/W 0  Flash sce function enable 1: enable sce 0: disable */
#define APBPeriph_FLASH                            ((u32)(0  << 30) | (0x00000001 << 8))   /*!<R/W 0  Flash spic function enable 1: enable flash 0: disable flash */
#define APBPeriph_WLON                             ((u32)(0  << 30) | (0x00000001 << 7))   /*!<R/W 0  WLON function enable 1: enable 0: disable */
#define APBPeriph_NP                               ((u32)(0  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: enable KM4 0: reset */
#define APBPeriph_LP                               ((u32)(0  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: enable KM0 0: reset */
#define APBPeriph_HPLFM                            ((u32)(0  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: enable HP platform 0: reset */
#define APBPeriph_LPLFM                            ((u32)(0  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: enable LP platform 0: reset */
#define APBPeriph_SIC                              ((u32)(0  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: enable SIC function 0: reset */
#define APBPeriph_HPON                             ((u32)(0  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: enable HS SYSON Function 0: reset */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_FEN_GRP1
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_ZGB                              ((u32)(1  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: enable zgb function 0: disable Note: for zgb is in wlan off domain , after turn wlan off , it need an 0->1 transiton */
#define APBPeriph_PSRAM                            ((u32)(1  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: enable PSRAM function 0: disable */
#define APBPeriph_AUDIO                            ((u32)(1  << 30) | (0x00000001 << 26))  /*!<R/W 0  When audio function enable , HS_SYSON_RAM domain must be on , for HSRAM memory at least in retention mode . 1: enable audio function 0: disable */
#define APBPeriph_AC                               ((u32)(1  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: enable audio codec active function 0: disable */
#define APBPeriph_DDRP                             ((u32)(1  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: enable ddr phy function 0: disable */
#define APBPeriph_DDRC                             ((u32)(1  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: enable ddr controller function 0: disable */
#define APBPeriph_IRDA                             ((u32)(1  << 30) | (0x00000001 << 22))  /*!<R/W 0  1: enable girda function 0: disable */
#define APBPeriph_ECDSA                            ((u32)(1  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: enable ecdsa function 0: disable */
#define APBPeriph_ED25519                          ((u32)(1  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: enable ed25519 function 0: disable */
#define APBPeriph_RSA                              ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: enable rsa function 0: disable */
#define APBPeriph_IPC_HP                           ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 0  1: enable IPC function 0: disable */
#define APBPeriph_LEDC                             ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: enable SPORT function 0: disable */
#define APBPeriph_TRNG                             ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: enable TRNG function 0: disable */
#define APBPeriph_USB                              ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 0  1: enable USB function 0: disable */
#define APBPeriph_SDH                              ((u32)(1  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: enable SDH function 0: disable */
#define APBPeriph_SPI1                             ((u32)(1  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: enable SPI1 function 0: disable */
#define APBPeriph_SPI0                             ((u32)(1  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: enable SPI0 function 0: disable */
#define APBPeriph_GDMA                             ((u32)(1  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: enable GDMA0 function 0: disable */
#define APBPeriph_LCDC                             ((u32)(1  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: enable LCDC function 0: disable */
#define APBPeriph_IPSEC                            ((u32)(1  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: enable IPSEC function 0: disable */
#define APBPeriph_LX1                              ((u32)(1  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: enable LX1 system function 0: disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_FEN_GRP2
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_TIM0                             ((u32)(2  << 30) | (0x00000001 << 16))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM1                             ((u32)(2  << 30) | (0x00000001 << 17))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM2                             ((u32)(2  << 30) | (0x00000001 << 18))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM3                             ((u32)(2  << 30) | (0x00000001 << 19))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM4                             ((u32)(2  << 30) | (0x00000001 << 20))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM5                             ((u32)(2  << 30) | (0x00000001 << 21))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM6                             ((u32)(2  << 30) | (0x00000001 << 22))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM7                             ((u32)(2  << 30) | (0x00000001 << 23))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM8                             ((u32)(2  << 30) | (0x00000001 << 24))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM9                             ((u32)(2  << 30) | (0x00000001 << 25))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM10                            ((u32)(2  << 30) | (0x00000001 << 26))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM11                            ((u32)(2  << 30) | (0x00000001 << 27))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM12                            ((u32)(2  << 30) | (0x00000001 << 28))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM13                            ((u32)(2  << 30) | (0x00000001 << 29))  /*!<R/W 14'h0  Timer0-13 function enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_SPORT0                           ((u32)(2  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: enable SPORT function 0: disable */
#define APBPeriph_SPORT1                           ((u32)(2  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: enable SPORT function 0: disable */
#define APBPeriph_SPORT2                           ((u32)(2  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: enable SPORT function 0: disable */
#define APBPeriph_SPORT3                           ((u32)(2  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: enable SPORT function 0: disable */
#define APBPeriph_UART0                            ((u32)(2  << 30) | (0x00000001 << 4) | (0x00000001 << 0)) /*!<R/W 0  Uart function reset (Note uart3 if for BT) */
#define APBPeriph_UART1                            ((u32)(2  << 30) | (0x00000001 << 5) | (0x00000001 << 1)) /*!<R/W 0  Uart function reset (Note uart3 if for BT) */
#define APBPeriph_UART2                            ((u32)(2  << 30) | (0x00000001 << 6) | (0x00000001 << 2)) /*!<R/W 0  Uart function reset (Note uart3 if for BT) */
#define APBPeriph_UART3                            ((u32)(2  << 30) | (0x00000001 << 7) | (0x00000001 << 3)) /*!<R/W 0  Uart function reset (Note uart3 if for BT) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKE_GRP0
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_I2C2_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: Enable HS I2C1 clock */
#define APBPeriph_I2C1_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: Enable HS I2C0 clock */
#define APBPeriph_I2C0_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: Enable LS I2C clock */
#define APBPeriph_DTIM_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 1  1: Enable LS dtim clk */
#define APBPeriph_ECDSA_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: Enable ecdsa engine clock */
#define APBPeriph_ED25519_CLOCK                    ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: Enable ed25519 engine clock */
#define APBPeriph_AIP_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 18))  /*!<R/W 1  1: Enable apb clk for aip use , include xtal/pll/regu control */
#define APBPeriph_THM_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 17))  /*!<R/W 1  1: Enable LS thm clock */
#define APBPeriph_ADC_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: Enable LS ADC clock */
#define APBPeriph_CTC_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: Enable Captouch clock */
#define APBPeriph_LOGUART_CLOCK                    ((u32)(0  << 30) | (0x00000001 << 13))  /*!<R/W 0  1: Enable Uartlog clock */
#define APBPeriph_IPC_LP_CLOCK                     ((u32)(0  << 30) | (0x00000001 << 12))  /*!<R/W 0  1: Enable LS IPC clock */
#define APBPeriph_PX_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 11))  /*!<R/W 1  1: Enable Pinmux clock */
#define APBPeriph_BTON_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable BT AON clock */
#define APBPeriph_WLON_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: Enable WLAN AON clock */
#define APBPeriph_GPIO_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: Enable GPIO clock */
#define APBPeriph_FLASH_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable SPIC source clock */
#define APBPeriph_NP_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: Enable KM4 clock */
#define APBPeriph_LP_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: Enable KM0 clock */
#define APBPeriph_HPLFM_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable hs platform clock */
#define APBPeriph_LPLFM_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable ls platform clock */
#define APBPeriph_SIC_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: Enable SIC clock */
#define APBPeriph_HPON_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: Enable HSYSON register clock; 0: gated SYSON Register clock */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKE_GRP1
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_ZGB_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: Enable zgb bus clock 0: Disable */
#define APBPeriph_PSRAM_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: Enable PSRAM bus clock 0: Disable */
#define APBPeriph_AC_CLOCK                         ((u32)(1  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: Enable audio codec clock 0: Disable */
#define APBPeriph_DDRP_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: Enable ddr phy clock 0: Disable */
#define APBPeriph_DDRC_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 0  1: Enable ddrc clock 0: Disable */
#define APBPeriph_IRDA_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 17))  /*!<R/W 0  1: Enable irda clock 0: Disable */
#define APBPeriph_IPC_HP_CLOCK                     ((u32)(1  << 30) | (0x00000001 << 16))  /*!<R/W 0  1: Enable IPC clock 0: Disable */
#define APBPeriph_SDH_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 13))  /*!<R/W 0  1: Enable SD HOST bus clock 0: Disable */
#define APBPeriph_TRNG_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 0  1: Enable trng bus clock 0: Disable */
#define APBPeriph_LEDC_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: Enable ledc bus clock 0: Disable */
#define APBPeriph_USB_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable USB bus clock 0: Disable */
#define APBPeriph_SPI1_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: Enable SPI1 clock 0: Disable */
#define APBPeriph_SPI0_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: Enable SPI0 clock 0: Disable */
#define APBPeriph_WMAC_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 5))   /*!<R/W 0   */
#define APBPeriph_GDMA_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable GDMA0 clock 0: Disable */
#define APBPeriph_LCDCMIPI_CLOCK                   ((u32)(1  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable LCDC&MIPI clock 0: Disable */
#define APBPeriph_IPSEC_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: Enable ipsec clock 0: Disable */
#define APBPeriph_LX1_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: Enable lx1 bus clock 0: Disable */
#define APBPeriph_HPERI_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 0))   /*!<R/W 1  1: Enable hperi clock . If IP using hperi clk , this bit must be set . 0: Disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKE_GRP2
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_TIM0_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 16))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM1_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 17))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM2_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 18))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM3_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 19))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM4_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 20))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM5_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 21))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM6_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 22))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM7_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 23))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM8_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 24))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM9_CLOCK                       ((u32)(2  << 30) | (0x00000001 << 25))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM10_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 26))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM11_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 27))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM12_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 28))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_TIM13_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 29))  /*!<R/W 14'h00  Timer0-13 clock enable 0-7 : lp basic timer 8 : pwm timer 9: pulse timer 13-10 : hp basic timer */
#define APBPeriph_SPORT0_CLOCK                     ((u32)(2  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable 4path sport clock 0: Disable */
#define APBPeriph_SPORT1_CLOCK                     ((u32)(2  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: Enable 4path sport clock 0: Disable */
#define APBPeriph_SPORT2_CLOCK                     ((u32)(2  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: Enable 4path sport clock 0: Disable */
#define APBPeriph_SPORT3_CLOCK                     ((u32)(2  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable 4path sport clock 0: Disable */
#define APBPeriph_UART0_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 0))   /*!<R/W 0  1: Enable Uart1 clock 0: Disable */
#define APBPeriph_UART1_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: Enable Uart1 clock 0: Disable */
#define APBPeriph_UART2_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: Enable Uart1 clock 0: Disable */
#define APBPeriph_UART3_CLOCK                      ((u32)(2  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable Uart1 clock 0: Disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKE_GRP
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CKE_XTAL_LPS_GRP                 ((u32)0x00000003 << 16)          /*!<R/W 2'h3  XTAL lps clock(312.5K) for different use , when neither func used . [0]: system SDM 32K [1]: bt lps clk Note : This field only used for HW debug , software needn't to config it for normal function. */
#define LSYS_CKE_XTAL_LPS_GRP(x)                   ((u32)(((x) & 0x00000003) << 16))
#define LSYS_GET_CKE_XTAL_LPS_GRP(x)               ((u32)(((x >> 16) & 0x00000003)))
#define LSYS_MASK_CKE_XTAL_GRP                     ((u32)0x0000000F << 8)          /*!<R/W 4'hf  XTAL 40M clock used for different use , when neither func used [0]: LP system [1]: 40M peri [2]: 2M peri [3]: 40M for NP when xtal is in LPS mode Note : This field only used for HW debug , software needn't to config it for normal function. */
#define LSYS_CKE_XTAL_GRP(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define LSYS_GET_CKE_XTAL_GRP(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define LSYS_MASK_CKE_OSC4M_GRP                    ((u32)0x000000FF << 0)          /*!<R/W 8'h01  OSC4M used for different use , when neither func used , osc4m can be disabled . 0: LP system 4M 1: OSC2M peri 2: WL/BT common 3: WL ana clock 4: BT ana clock 5: SDH ana clock 6: aip func control 7: vad Note : This field only used for HW debug , software needn't to config it for normal function. Not use, move to aon reg: cke_osc4m_force */
#define LSYS_CKE_OSC4M_GRP(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define LSYS_GET_CKE_OSC4M_GRP(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKSL_GRP0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_CKSL_PSRAM                        ((u32)0x00000001 << 31)          /*!<R/W 1  psram phy clock seleck 0: xtal 40M 1: nppll divided clock , max to 300M */
#define LSYS_CKSL_PSRAM(x)                         ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_CKSL_PSRAM(x)                     ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_BIT_CKSL_HIPC                         ((u32)0x00000001 << 30)          /*!<R/W 0  hs_ipc clock select 0: hapb clk 1: lpon clk(4M) */
#define LSYS_CKSL_HIPC(x)                          ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_CKSL_HIPC(x)                      ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_CKSL_VAD                          ((u32)0x00000001 << 29)          /*!<R/W 0  audio codec vad clock select 0: osc4m output 1: xtal 40M divider 10 output . Note : make xtal enter lps mode when vad work to save power */
#define LSYS_CKSL_VAD(x)                           ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_CKSL_VAD(x)                       ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_CKSL_ADC2M                        ((u32)0x00000001 << 28)          /*!<R/W 0  ADC 2M seleck 0: osc4M divider 2 1: xtal 40M divider out */
#define LSYS_CKSL_ADC2M(x)                         ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_CKSL_ADC2M(x)                     ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_MASK_CKSL_SPORT                       ((u32)0x0000000F << 24)          /*!<R/W 4'b1111  sport clock selection 0: equal to audio codec main clock (40M) 1: from I2S pll , after i2s pll selection and divider */
#define LSYS_CKSL_SPORT(x)                         ((u32)(((x) & 0x0000000F) << 24))
#define LSYS_GET_CKSL_SPORT(x)                     ((u32)(((x >> 24) & 0x0000000F)))
#define LSYS_MASK_CKSL_XTAL                        ((u32)0x0000000F << 20)          /*!<R/W 0  xtal selection, Default=40MHz 0000: 40MHz 0001: 25MHz 0010: 13MHz 0011: 19.2MHz 0100: 20MHz 0101: 26MHz 0110: 38.4MHz 0111: 17.664MHz 1000: 16MHz 1001: 14.138MHz 1010: 12MHz 1011: 52MHz 1100: 48MHz 1101: 27MHz 1110: 24MHz 1111: none */
#define LSYS_CKSL_XTAL(x)                          ((u32)(((x) & 0x0000000F) << 20))
#define LSYS_GET_CKSL_XTAL(x)                      ((u32)(((x >> 20) & 0x0000000F)))
#define LSYS_BIT_CKSL_UART2M                       ((u32)0x00000001 << 19)          /*!<R/W 0  Uart 2M seleck 0: osc4M divider 2 1: xtal 40M divider out */
#define LSYS_CKSL_UART2M(x)                        ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_CKSL_UART2M(x)                    ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_MASK_CKSL_UART                        ((u32)0x00000007 << 16)          /*!<R/W 0  Select uart rx clock for every uart . (Note uart3 is BT uart , always 40M) 0: rx clock is xtal 40M 1: rx clock is 2M (osc2M or xtal divided 2M) */
#define LSYS_CKSL_UART(x)                          ((u32)(((x) & 0x00000007) << 16))
#define LSYS_GET_CKSL_UART(x)                      ((u32)(((x >> 16) & 0x00000007)))
#define LSYS_BIT_CKSL_SDM                          ((u32)0x00000001 << 15)          /*!<R/W 0h  sdm 128 clock select , this must be fixed after initial selection. 0:osc128 1:xtal lps 312.5K */
#define LSYS_CKSL_SDM(x)                           ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_CKSL_SDM(x)                       ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_CKSL_VADM                         ((u32)0x00000001 << 14)          /*!<R/W 0h  vad share memory clock select . Note, it must first change this bit to 1,and then shutdown hs plfm . When wakeup , it must first enable hs plfm , finally set this bit to 0 . 0: used by hs plfm clock 1: used by vad buffer ctrol */
#define LSYS_CKSL_VADM(x)                          ((u32)(((x) & 0x00000001) << 14))
#define LSYS_GET_CKSL_VADM(x)                      ((u32)(((x >> 14) & 0x00000001)))
#define LSYS_BIT_CKSL_AC                           ((u32)0x00000001 << 13)          /*!<R/W 1h  audio codec main clock selection 0: syspll divided 40M 1: xtal 40M */
#define LSYS_CKSL_AC(x)                            ((u32)(((x) & 0x00000001) << 13))
#define LSYS_GET_CKSL_AC(x)                        ((u32)(((x >> 13) & 0x00000001)))
#define LSYS_BIT_CKSL_LOGUART2M                    ((u32)0x00000001 << 12)          /*!<R/W 0  Loguart 2M seleck 0: osc4M divider 2 1: xtal 40M divider out */
#define LSYS_CKSL_LOGUART2M(x)                     ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_CKSL_LOGUART2M(x)                 ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_CKSL_LOGUART                      ((u32)0x00000001 << 11)          /*!<R/W 0  Select loguart trx clock 0: clock is xtal 40M 1: clock is 2M */
#define LSYS_CKSL_LOGUART(x)                       ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_CKSL_LOGUART(x)                   ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_BIT_CKSL_HBUS                         ((u32)0x00000001 << 10)          /*!<R/W 1h  HP bus clock seletion 1: from nppll divider output , which default is 100M 0: from xtal 40M clock */
#define LSYS_CKSL_HBUS(x)                          ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_CKSL_HBUS(x)                      ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_CKSL_NP                           ((u32)0x00000001 << 8)          /*!<R/W 1h  NP(KM4) clock seletion 1: from nppll divider output , which default is 320M 0: from xtal 40M clock */
#define LSYS_CKSL_NP(x)                            ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_CKSL_NP(x)                        ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_BIT_CKSL_CTC                          ((u32)0x00000001 << 6)          /*!<R/W 1h  CTC REG clock selection 0: 131k 1: LS APB Clock */
#define LSYS_CKSL_CTC(x)                           ((u32)(((x) & 0x00000001) << 6))
#define LSYS_GET_CKSL_CTC(x)                       ((u32)(((x >> 6) & 0x00000001)))
#define LSYS_BIT_CKSL_ADC                          ((u32)0x00000001 << 5)          /*!<R/W 0  ADC INT clock selection 0: LS APB clock 1: OSC2M */
#define LSYS_CKSL_ADC(x)                           ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_CKSL_ADC(x)                       ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_CKSL_GPIO                         ((u32)0x00000001 << 4)          /*!<R/W 0  GPIO INT clock selecton: 0: LS APB clock 1: 32.768K clock from SDM */
#define LSYS_CKSL_GPIO(x)                          ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_CKSL_GPIO(x)                      ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_MASK_CKSL_SPIC                        ((u32)0x00000003 << 2)          /*!<R/W 1  SPIC clock sel: 00/10: OSC4M 01: XTAL 40M 11: nppll divided out,max to 200M */
#define LSYS_CKSL_SPIC(x)                          ((u32)(((x) & 0x00000003) << 2))
#define LSYS_GET_CKSL_SPIC(x)                      ((u32)(((x >> 2) & 0x00000003)))
#define LSYS_MASK_CKSL_LSOC                        ((u32)0x00000003 << 0)          /*!<R/W/ES 0  KM0 cpu clock seletion 2'b00/2'b01: ANA4M 2'b10/2'b11: XTAL */
#define LSYS_CKSL_LSOC(x)                          ((u32)(((x) & 0x00000003) << 0))
#define LSYS_GET_CKSL_LSOC(x)                      ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKD_GRP0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CKD_MIPI                         ((u32)0x0000003F << 24)          /*!<R/WPD 6'd14  mipi vo clock divider, it is based on nppll clock Max timing signoff clock is 66.7M . Default is 800/12 = 66.7 divider by this value + 1 */
#define LSYS_CKD_MIPI(x)                           ((u32)(((x) & 0x0000003F) << 24))
#define LSYS_GET_CKD_MIPI(x)                       ((u32)(((x >> 24) & 0x0000003F)))
#define LSYS_MASK_CKD_HPERI                        ((u32)0x00000007 << 20)          /*!<R/WPD 4h  audio main clock divider, it is based on np pll output 0~2 : not valid 3: div4 4: div5 other: divider by this value + 1 */
#define LSYS_CKD_HPERI(x)                          ((u32)(((x) & 0x00000007) << 20))
#define LSYS_GET_CKD_HPERI(x)                      ((u32)(((x >> 20) & 0x00000007)))
#define LSYS_MASK_CKD_AC                           ((u32)0x0000000F << 16)          /*!<R/WPD 3h  audio main clock divider, it is based on np pll output 0~2 : not valid 3: div20 4: div21 other: divider by this value + 17 */
#define LSYS_CKD_AC(x)                             ((u32)(((x) & 0x0000000F) << 16))
#define LSYS_GET_CKD_AC(x)                         ((u32)(((x >> 16) & 0x0000000F)))
#define LSYS_MASK_CKD_HBUS                         ((u32)0x0000000F << 12)          /*!<R/WPD 9h  ahb clock divider, it is based on np pll output 0~6 : not valid 7: div8 8: div9 other: divider by this value + 1 */
#define LSYS_CKD_HBUS(x)                           ((u32)(((x) & 0x0000000F) << 12))
#define LSYS_GET_CKD_HBUS(x)                       ((u32)(((x >> 12) & 0x0000000F)))
#define LSYS_MASK_CKD_PSRAM                        ((u32)0x00000007 << 8)          /*!<R/WPD 1h  PSRAM phy clock divider, it is based on np pll (600M). 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_PSRAM(x)                          ((u32)(((x) & 0x00000007) << 8))
#define LSYS_GET_CKD_PSRAM(x)                      ((u32)(((x >> 8) & 0x00000007)))
#define LSYS_MASK_CKD_PLFM                         ((u32)0x00000007 << 4)          /*!<R/WPD 1h  PLFM clock divider, it is based on np clock . 0: not valid 1: div2 (it is better to fix in this value) 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_PLFM(x)                           ((u32)(((x) & 0x00000007) << 4))
#define LSYS_GET_CKD_PLFM(x)                       ((u32)(((x >> 4) & 0x00000007)))
#define LSYS_MASK_CKD_NP                           ((u32)0x00000007 << 0)          /*!<R/WPD 2h  NP clock divider, it is based on np pll (800M). 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_NP(x)                             ((u32)(((x) & 0x00000007) << 0))
#define LSYS_GET_CKD_NP(x)                         ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_CKD_GRP1
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CKD_XTAL_2M                      ((u32)0x00007FFF << 1)          /*!<R/W 0x7D0  Output divided clock frequency for UART: BIT_LSYS_UART_XTAL_DIV_FREQ/1000 (MHz) */
#define LSYS_CKD_XTAL_2M(x)                        ((u32)(((x) & 0x00007FFF) << 1))
#define LSYS_GET_CKD_XTAL_2M(x)                    ((u32)(((x >> 1) & 0x00007FFF)))
#define LSYS_BIT_CKD_XTAL_2M_EN                    ((u32)0x00000001 << 0)          /*!<R/W 0  1: enable XTAL division circuit for UART */
#define LSYS_CKD_XTAL_2M_EN(x)                     ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_CKD_XTAL_2M_EN(x)                 ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SW_RST_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_APSYS_RST                         ((u32)0x00000001 << 31)          /*!<R/WE 0  Only when REG_SW_RST_TRIG is 0x69696969 , this reg can be write */
#define LSYS_APSYS_RST(x)                          ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_APSYS_RST(x)                      ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_BIT_NPSYS_RST                         ((u32)0x00000001 << 30)          /*!<R/WE 0  Only when REG_SW_RST_TRIG is 0x69696969 , this reg can be write */
#define LSYS_NPSYS_RST(x)                          ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_NPSYS_RST(x)                      ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_LPSYS_RST                         ((u32)0x00000001 << 29)          /*!<R/WE 0  Only when REG_SW_RST_TRIG is 0x69696969 , this reg can be write */
#define LSYS_LPSYS_RST(x)                          ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_LPSYS_RST(x)                      ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_PERIALL_RST_EN                    ((u32)0x00000001 << 1)          /*!<R/W 0  1. warm reset request by one cpu will reset all peri and local cpu 0. warm reset request by one cpu will only reset local cpu Note : peri rst will use sysrst mst also */
#define LSYS_PERIALL_RST_EN(x)                     ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_PERIALL_RST_EN(x)                 ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_OTHERCPU_RST_EN                   ((u32)0x00000001 << 0)          /*!<R/W 0  1. warm reset request by one cpu will reset other cpu's core and local cpu 0. warm reset request by one cpu will only reset local cpu */
#define LSYS_OTHERCPU_RST_EN(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_OTHERCPU_RST_EN(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SW_RST_TRIG
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_TRIG_RST                         ((u32)0xFFFFFFFF << 0)          /*!<R/WPD 0  System reset trigger register Write 0x96969696 to enable the write of reset control Write 0x69696969 will trigger the system if REG_SW_RST_CTRL set . */
#define LSYS_TRIG_RST(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_TRIG_RST(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_LPLAT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_SPIC_SPDUP_SIM                    ((u32)0x00000001 << 25)          /*!<R/W 0  1: SPIC will wait 256 spic_clk cycles before ready 0: SPIC will wait 2^16 spic_clk cycles before ready */
#define LSYS_SPIC_SPDUP_SIM(x)                     ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_SPIC_SPDUP_SIM(x)                 ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_PLFM_AUTO_ICG_EN                  ((u32)0x00000001 << 24)          /*!<R/W 0  1: Enable LS platform auto clock-gating for power save 0: Disable */
#define LSYS_PLFM_AUTO_ICG_EN(x)                   ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_PLFM_AUTO_ICG_EN(x)               ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_MASK_PLFM_MST0_SEL                    ((u32)0x00000003 << 20)          /*!<R/W 1h  LS platfrom m0 select 0: SIC 1: OTPC 2/3: KM0 Default is OTPC for autoload , after done , pmc will set thi field to KM0 , or SIC(testmode) */
#define LSYS_PLFM_MST0_SEL(x)                      ((u32)(((x) & 0x00000003) << 20))
#define LSYS_GET_PLFM_MST0_SEL(x)                  ((u32)(((x >> 20) & 0x00000003)))
#define LSYS_BIT_KM0_RETENTION_MODE                ((u32)0x00000001 << 16)          /*!<R/W 0  1: Enable, when KM0 reset, Cache will not be cleared 0: Disable, when KM0 reset, Cache will be cleared by HW */
#define LSYS_KM0_RETENTION_MODE(x)                 ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_KM0_RETENTION_MODE(x)             ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_BIT_HUK_PROT                          ((u32)0x00000001 << 0)          /*!<R/WH 0  0: only cpu can read HUK key 1: no one can read HUK key This bit will be latch to high when it was write to 1 until reset again . */
#define LSYS_HUK_PROT(x)                           ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_HUK_PROT(x)                       ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_LPLAT_STATUS
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_KM0_DBGRESTARTED_M                ((u32)0x00000001 << 7)          /*!<R 0  KM0 debug restart status */
#define LSYS_KM0_DBGRESTARTED_M(x)                 ((u32)(((x) & 0x00000001) << 7))
#define LSYS_GET_KM0_DBGRESTARTED_M(x)             ((u32)(((x >> 7) & 0x00000001)))
#define LSYS_BIT_KM0_HALTED_M                      ((u32)0x00000001 << 6)          /*!<R 0  KM0 halt status */
#define LSYS_KM0_HALTED_M(x)                       ((u32)(((x) & 0x00000001) << 6))
#define LSYS_GET_KM0_HALTED_M(x)                   ((u32)(((x >> 6) & 0x00000001)))
#define LSYS_BIT_KM0_LOCKUP_M                      ((u32)0x00000001 << 5)          /*!<R 0  KM0 lockup status */
#define LSYS_KM0_LOCKUP_M(x)                       ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_KM0_LOCKUP_M(x)                   ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_KM0_SL_SLEEPSYS_R                 ((u32)0x00000001 << 4)          /*!<R 0  KM0 sleep status */
#define LSYS_KM0_SL_SLEEPSYS_R(x)                  ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_KM0_SL_SLEEPSYS_R(x)              ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_LS_IDLE                           ((u32)0x00000001 << 0)          /*!<R 0  Indicate LS plfm is in IDLE state , thus can change m0_select */
#define LSYS_LS_IDLE(x)                            ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_LS_IDLE(x)                        ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SW_PATCH
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_SW_PATCH_SADDR                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x23008000  SW patch code start addr. OTP physical addr 0x500-0x6FF will autoload to this addr . */
#define LSYS_SW_PATCH_SADDR(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_SW_PATCH_SADDR(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SPORT_CLK
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CKD_SPORT3                       ((u32)0x00000007 << 28)          /*!<R/WPD 1h  Sport clock divider, it is based on np i2s1 pll and i2s2 pll mux out 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_SPORT3(x)                         ((u32)(((x) & 0x00000007) << 28))
#define LSYS_GET_CKD_SPORT3(x)                     ((u32)(((x >> 28) & 0x00000007)))
#define LSYS_MASK_CKD_SPORT2                       ((u32)0x00000007 << 24)          /*!<R/WPD 1h  Sport clock divider, it is based on np i2s1 pll and i2s2 pll mux out 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_SPORT2(x)                         ((u32)(((x) & 0x00000007) << 24))
#define LSYS_GET_CKD_SPORT2(x)                     ((u32)(((x >> 24) & 0x00000007)))
#define LSYS_MASK_CKD_SPORT1                       ((u32)0x00000007 << 20)          /*!<R/WPD 1h  Sport clock divider, it is based on np i2s1 pll and i2s2 pll mux out 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_SPORT1(x)                         ((u32)(((x) & 0x00000007) << 20))
#define LSYS_GET_CKD_SPORT1(x)                     ((u32)(((x >> 20) & 0x00000007)))
#define LSYS_MASK_CKD_SPORT0                       ((u32)0x00000007 << 16)          /*!<R/WPD 1h  Sport clock divider, it is based on np i2s1 pll and i2s2 pll mux out 0: not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_SPORT0(x)                         ((u32)(((x) & 0x00000007) << 16))
#define LSYS_GET_CKD_SPORT0(x)                     ((u32)(((x >> 16) & 0x00000007)))
#define LSYS_MASK_CKSL_I2S3                        ((u32)0x00000003 << 6)          /*!<R/W 0h  I2S pll selection for sport3 00/10: select i2s pll1 which is 98.304M 01: select i2s pll2 which is 45.158M 11: select i2s pll1 which is 24.576M */
#define LSYS_CKSL_I2S3(x)                          ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_CKSL_I2S3(x)                      ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_MASK_CKSL_I2S2                        ((u32)0x00000003 << 4)          /*!<R/W 0h  I2S pll selection for sport2 00/10: select i2s pll1 which is 98.304M 01: select i2s pll2 which is 45.158M 11: select i2s pll1 which is 24.576M */
#define LSYS_CKSL_I2S2(x)                          ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_CKSL_I2S2(x)                      ((u32)(((x >> 4) & 0x00000003)))
#define LSYS_MASK_CKSL_I2S1                        ((u32)0x00000003 << 2)          /*!<R/W 0h  I2S pll selection for sport1 00/10: select i2s pll1 which is 98.304M 01: select i2s pll2 which is 45.158M 11: select i2s pll1 which is 24.576M */
#define LSYS_CKSL_I2S1(x)                          ((u32)(((x) & 0x00000003) << 2))
#define LSYS_GET_CKSL_I2S1(x)                      ((u32)(((x >> 2) & 0x00000003)))
#define LSYS_MASK_CKSL_I2S0                        ((u32)0x00000003 << 0)          /*!<R/W 0h  I2S pll selection for sport0 00/10: select i2s pll1 which is 98.304M 01: select i2s pll2 which is 45.158M 11: select i2s pll1 which is 24.576M */
#define LSYS_CKSL_I2S0(x)                          ((u32)(((x) & 0x00000003) << 0))
#define LSYS_GET_CKSL_I2S0(x)                      ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BT_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_WL_USE_REQ                        ((u32)0x00000001 << 25)          /*!<R/W 0  who uses loguart trx pad depends on the xor of WL_USE_REQ and BT_USE_REQ, wifi can send use request by change the value of WL_USE_REQ */
#define LSYS_WL_USE_REQ(x)                         ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_WL_USE_REQ(x)                     ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_BTWL_USE_LOGUART_PAD_ST           ((u32)0x00000001 << 24)          /*!<R   The state of xor of WL_USE_REQ and BT_USE_REQ. 0: loguart use loguart trx pad 1: bt hci use loguart trx pad */
#define LSYS_BTWL_USE_LOGUART_PAD_ST(x)            ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_BTWL_USE_LOGUART_PAD_ST(x)        ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_FORCE_LOGUART_USE_LOGUART_PAD_B   ((u32)0x00000001 << 23)          /*!<R/W 0  Force wifi soc to use loguart pad mandatory if necessary 0: loguart use loguart trx pad 1: who uses loguart trx pad depends on BTWL_USE_LOGUART_PAD_ST */
#define LSYS_FORCE_LOGUART_USE_LOGUART_PAD_B(x)    ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_FORCE_LOGUART_USE_LOGUART_PAD_B(x) ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_BIT_BT_HW_EN_BTAFE_2REGU              ((u32)0x00000001 << 22)          /*!<R/W 0  Control 3.3V power reset of BT afe, need level shift to 3.3V by regu, SW shall set the bit to1 when BT use BT AFE */
#define LSYS_BT_HW_EN_BTAFE_2REGU(x)               ((u32)(((x) & 0x00000001) << 22))
#define LSYS_GET_BT_HW_EN_BTAFE_2REGU(x)           ((u32)(((x >> 22) & 0x00000001)))
#define LSYS_BIT_BT_HW_EN_BTAFE                    ((u32)0x00000001 << 21)          /*!<R/W 0  Core power reset of BT afe, SW shall set the bit to1 when BT use BT AFE */
#define LSYS_BT_HW_EN_BTAFE(x)                     ((u32)(((x) & 0x00000001) << 21))
#define LSYS_GET_BT_HW_EN_BTAFE(x)                 ((u32)(((x >> 21) & 0x00000001)))
#define LSYS_BIT_BT_EN_TST_SI_CTRL_RFC             ((u32)0x00000001 << 20)          /*!<R/W 1   */
#define LSYS_BT_EN_TST_SI_CTRL_RFC(x)              ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_BT_EN_TST_SI_CTRL_RFC(x)          ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_BT_USE_EXT_UART                   ((u32)0x00000001 << 19)          /*!<R/W 0  1: BT use external uart 0: BT use internal uart */
#define LSYS_BT_USE_EXT_UART(x)                    ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_BT_USE_EXT_UART(x)                ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_MASK_BT_GPIO_FUN_SEL                  ((u32)0x00000007 << 16)          /*!<R/W 0  BT GPIO function select, 5:I2C mode */
#define LSYS_BT_GPIO_FUN_SEL(x)                    ((u32)(((x) & 0x00000007) << 16))
#define LSYS_GET_BT_GPIO_FUN_SEL(x)                ((u32)(((x >> 16) & 0x00000007)))
#define LSYS_BIT_BT_WAKE_HOST_POL                  ((u32)0x00000001 << 15)          /*!<R/W 1  1: BT_WAKE_HOST to CPU and pinmux 0: pinmux to CPU for external trig */
#define LSYS_BT_WAKE_HOST_POL(x)                   ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_BT_WAKE_HOST_POL(x)               ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_HOST_WAKE_BT_POL                  ((u32)0x00000001 << 14)          /*!<R/W 1  1: HOST_WAKE_BT to BT and pinmux 0: pinmux to BT for external trig */
#define LSYS_HOST_WAKE_BT_POL(x)                   ((u32)(((x) & 0x00000001) << 14))
#define LSYS_GET_HOST_WAKE_BT_POL(x)               ((u32)(((x >> 14) & 0x00000001)))
#define LSYS_BIT_HOST_WAKE_BT                      ((u32)0x00000001 << 13)          /*!<R/W 0  CPU set this bit to 1 to wake BT */
#define LSYS_HOST_WAKE_BT(x)                       ((u32)(((x) & 0x00000001) << 13))
#define LSYS_GET_HOST_WAKE_BT(x)                   ((u32)(((x >> 13) & 0x00000001)))
#define LSYS_BIT_BT_AHB_ACCESS_GNT                 ((u32)0x00000001 << 12)          /*!<R 0  Access grant for system master access BT reg through AHB bus */
#define LSYS_BT_AHB_ACCESS_GNT(x)                  ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_BT_AHB_ACCESS_GNT(x)              ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_BT_AHB_ACCESS_REQ                 ((u32)0x00000001 << 11)          /*!<R/W 0  System master set this bit to 1 and wait BT_AHB_ACCESS_GNT=1 before access BT reg through AHB bus, when access finish, shall set this bit to 0 */
#define LSYS_BT_AHB_ACCESS_REQ(x)                  ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_BT_AHB_ACCESS_REQ(x)              ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_BIT_BT_PLL_LDO_MODE                   ((u32)0x00000001 << 10)          /*!<R/W 0  1: system force AFE PLL LDO to Power cut mode 0: AFE PLL LDO mode is controlled by BTON parameter */
#define LSYS_BT_PLL_LDO_MODE(x)                    ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_BT_PLL_LDO_MODE(x)                ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_BT_AFEPLL_EN                      ((u32)0x00000001 << 9)          /*!<R/W 0  1: system force AFE PLL enable 0: AFE PLL enable by BT PMC */
#define LSYS_BT_AFEPLL_EN(x)                       ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_BT_AFEPLL_EN(x)                   ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_BT_AFELDO_EN                      ((u32)0x00000001 << 8)          /*!<R/W 0  1: system force AFE LDO enable 0: AFE LDO enable by BT PMC */
#define LSYS_BT_AFELDO_EN(x)                       ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_BT_AFELDO_EN(x)                   ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_MASK_BT_OFF_PC_WT                     ((u32)0x00000003 << 6)          /*!<R/W 0  Wait time select for BT off power cut stable */
#define LSYS_BT_OFF_PC_WT(x)                       ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_BT_OFF_PC_WT(x)                   ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_BIT_ASWR_REF_BT_AUD                   ((u32)0x00000001 << 5)          /*!<R/W 0  Audio swr parameter refer BT status */
#define LSYS_ASWR_REF_BT_AUD(x)                    ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_ASWR_REF_BT_AUD(x)                ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_ASWR_REF_BT_MEM                   ((u32)0x00000001 << 4)          /*!<R/W 0  Memory swr parameter refer BT status */
#define LSYS_ASWR_REF_BT_MEM(x)                    ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_ASWR_REF_BT_MEM(x)                ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_DSWR_REF_BT                       ((u32)0x00000001 << 3)          /*!<R/W 1  Core swr parameter refer BT status */
#define LSYS_DSWR_REF_BT(x)                        ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_DSWR_REF_BT(x)                    ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_BT_HW_EN_BTRF_2REGU               ((u32)0x00000001 << 2)          /*!<R/W 0  Control 3.3V power reset of BT rfc, need level shift to 3.3V by regu, SW shall set the bit to1 when BT use BT RFC */
#define LSYS_BT_HW_EN_BTRF_2REGU(x)                ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_BT_HW_EN_BTRF_2REGU(x)            ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_BT_HW_EN_BTRF                     ((u32)0x00000001 << 1)          /*!<R/W 0  Core power reset of BT rfc, SW shall set the bit to1 when BT use BT RFC */
#define LSYS_BT_HW_EN_BTRF(x)                      ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_BT_HW_EN_BTRF(x)                  ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_BT_USE_WL_RFAFE                   ((u32)0x00000001 << 0)          /*!<R/W 0  Indicate BT select WL RFAFE, shall not switch when BT is on */
#define LSYS_BT_USE_WL_RFAFE(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_BT_USE_WL_RFAFE(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BT_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_BT_PMC_STATE                     ((u32)0x00007FFF << 0)          /*!<R 0  Indicate BT PMC current state, for debug */
#define LSYS_BT_PMC_STATE(x)                       ((u32)(((x) & 0x00007FFF) << 0))
#define LSYS_GET_BT_PMC_STATE(x)                   ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_AIP_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_CK_CAL_START                      ((u32)0x00000001 << 31)          /*!<R/W/ES 0x0  Start clk calibration . This bit will auto-set to 0 when cal done . */
#define LSYS_CK_CAL_START(x)                       ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_CK_CAL_START(x)                   ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_MASK_CK_CAL_SEL                       ((u32)0x00000003 << 16)          /*!<R/W 0x0  Calibration clock selection . The refrence clock is xtal clock. 0: OSC4M 1: AON100K 2: AON128K for SDM */
#define LSYS_CK_CAL_SEL(x)                         ((u32)(((x) & 0x00000003) << 16))
#define LSYS_GET_CK_CAL_SEL(x)                     ((u32)(((x >> 16) & 0x00000003)))
#define LSYS_MASK_CK_CAL_RPT                       ((u32)0x0000FFFF << 0)          /*!<R   Clock calibration result */
#define LSYS_CK_CAL_RPT(x)                         ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_CK_CAL_RPT(x)                     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_AIP_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_BG_PWR                            ((u32)0x00000001 << 8)          /*!<R/W 1  1: power on ddrphy bandgap 0: shutdown bg */
#define LSYS_BG_PWR(x)                             ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_BG_PWR(x)                         ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_BIT_BG_ON_USB2                        ((u32)0x00000001 << 5)          /*!<R/W 1  Bandgap USB2 current enable */
#define LSYS_BG_ON_USB2(x)                         ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_BG_ON_USB2(x)                     ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_BG_ON_MIPI                        ((u32)0x00000001 << 4)          /*!<R/W 1  Bandgap MIPI current enable */
#define LSYS_BG_ON_MIPI(x)                         ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_BG_ON_MIPI(x)                     ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_BG_LPF                            ((u32)0x00000001 << 2)          /*!<R/W 1  Bandgap low pass filter */
#define LSYS_BG_LPF(x)                             ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_BG_LPF(x)                         ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_MASK_BG_ALL                           ((u32)0x00000003 << 0)          /*!<R/W 0x3  Bandgap enable mode */
#define LSYS_BG_ALL(x)                             ((u32)(((x) & 0x00000003) << 0))
#define LSYS_GET_BG_ALL(x)                         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_AIP_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_XTAL_LPS_EN_DIGI_CK               ((u32)0x00000001 << 24)          /*!<R/W 0  0: disable XTAL clock for digital when XTAL enter LPS mode 1: enable XTAL clock for digital when XTAL enter LPS mode */
#define LSYS_XTAL_LPS_EN_DIGI_CK(x)                ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_XTAL_LPS_EN_DIGI_CK(x)            ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_MASK_ANAIP_CTL_TUTIME                 ((u32)0x0000003F << 16)          /*!<R/W 0xF  Timeunit of SWR/XTAL arbitration circuit, in unit of ANA4M, Default time unit: 4us */
#define LSYS_ANAIP_CTL_TUTIME(x)                   ((u32)(((x) & 0x0000003F) << 16))
#define LSYS_GET_ANAIP_CTL_TUTIME(x)               ((u32)(((x >> 16) & 0x0000003F)))
#define LSYS_MASK_XTAL_BG2HPMOD_CYC                ((u32)0x000000FF << 8)          /*!<R/W 0x1F  The time interval between BG enable to XTAL switch to HP Mode, the timeunit is control by 0x13C[21:16]: BIT_LSYS_ANAIP_CTL_TUTIME */
#define LSYS_XTAL_BG2HPMOD_CYC(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define LSYS_GET_XTAL_BG2HPMOD_CYC(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define LSYS_MASK_XTAL_DBNC_CYC                    ((u32)0x000000FF << 0)          /*!<R/W 0x1F  De-bounce time of XTAL enable request from WIFI & SYSON PMC, the timeunit is control by 0x13C[21:16]: BIT_LSYS_ANAIP_CTL_TUTIME */
#define LSYS_XTAL_DBNC_CYC(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define LSYS_GET_XTAL_DBNC_CYC(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BOOT_CFG
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_BOOT_WAKE_FROM_PS_HS              ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define LSYS_BOOT_WAKE_FROM_PS_HS(x)               ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_BOOT_WAKE_FROM_PS_HS(x)           ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_BOOT_PATCH_FUNC0                  ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define LSYS_BOOT_PATCH_FUNC0(x)                   ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_BOOT_PATCH_FUNC0(x)               ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_BIT_BOOT_PATCH_FUNC1                  ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define LSYS_BOOT_PATCH_FUNC1(x)                   ((u32)(((x) & 0x00000001) << 27))
#define LSYS_GET_BOOT_PATCH_FUNC1(x)               ((u32)(((x >> 27) & 0x00000001)))
#define LSYS_BIT_BOOT_KM0_RUN                      ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define LSYS_BOOT_KM0_RUN(x)                       ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_BOOT_KM0_RUN(x)                   ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_BOOT_WAKE_FROM_PS_LS              ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define LSYS_BOOT_WAKE_FROM_PS_LS(x)               ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_BOOT_WAKE_FROM_PS_LS(x)           ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_BOOT_IMG3_EXIST                   ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define LSYS_BOOT_IMG3_EXIST(x)                    ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_BOOT_IMG3_EXIST(x)                ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_MASK_ROM_VERSION_SW                   ((u32)0x0000FFFF << 0)          /*!<R/W 0   */
#define LSYS_ROM_VERSION_SW(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_ROM_VERSION_SW(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_FPGA_CLK_INFO
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_FPGA_AP_CLK                      ((u32)0x000000FF << 24)          /*!<R 0  FPGA AP clock */
#define LSYS_FPGA_AP_CLK(x)                        ((u32)(((x) & 0x000000FF) << 24))
#define LSYS_GET_FPGA_AP_CLK(x)                    ((u32)(((x >> 24) & 0x000000FF)))
#define LSYS_MASK_FPGA_NP_CLK                      ((u32)0x000000FF << 16)          /*!<R 0  FPGA NP clock */
#define LSYS_FPGA_NP_CLK(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define LSYS_GET_FPGA_NP_CLK(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define LSYS_MASK_FPGA_PSRAM_CLK                   ((u32)0x000000FF << 8)          /*!<R 0  FPGA PSRAM clock */
#define LSYS_FPGA_PSRAM_CLK(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define LSYS_GET_FPGA_PSRAM_CLK(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define LSYS_MASK_FPGA_FLASH_CLK                   ((u32)0x000000FF << 0)          /*!<R 0  FPGA Flash clock */
#define LSYS_FPGA_FLASH_CLK(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define LSYS_GET_FPGA_FLASH_CLK(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SCAN_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CHIP_INFO_EN                     ((u32)0x0000000F << 28)          /*!<R/W 0  Only when this field set to 4'hA, rl_ver/rl_no can be read out . */
#define LSYS_CHIP_INFO_EN(x)                       ((u32)((x) & 0x0000000F) << 28)
#define LSYS_GET_CHIP_INFO_EN(x)                   ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_MASK_RL_VER                           ((u32)0x0000000F << 16)          /*!<RMSK0 0  CHIP cut verison : 0: A-cut 1: B-cut … */
#define LSYS_RL_VER(x)                             ((u32)(((x) & 0x0000000F) << 16))
#define LSYS_GET_RL_VER(x)                         ((u32)(((x >> 16) & 0x0000000F)))
#define LSYS_MASK_RL_NO                            ((u32)0x0000FFFF << 0)          /*!<RMSK0 0x6678  CHIP RL NUMBER : 0x6678 */
#define LSYS_RL_NO(x)                              ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_RL_NO(x)                          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SYSTEM_CFG0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_ENG_MODE                          ((u32)0x00000001 << 31)          /*!<R 0  Indicate PKG_ENG_SEL pad value 0: normal package; 1: engineering mode */
#define LSYS_ENG_MODE(x)                           ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_ENG_MODE(x)                       ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_BIT_ENG_MODE_AON                      ((u32)0x00000001 << 30)          /*!<R/WA0 0  Wrtie 1 to set aon enter eng_mode */
#define LSYS_ENG_MODE_AON(x)                       ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_ENG_MODE_AON(x)                   ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_BD_OPT1                           ((u32)0x00000001 << 29)          /*!<R 0  Bonding option-ext_clk */
#define LSYS_BD_OPT1(x)                            ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_BD_OPT1(x)                        ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_BD_OPT0                           ((u32)0x00000001 << 28)          /*!<R 0  Bonding option-ext_rstb */
#define LSYS_BD_OPT0(x)                            ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_BD_OPT0(x)                        ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_BIT_PTRP_BOOTSEL                      ((u32)0x00000001 << 20)          /*!<R 0  Power on trapped bootsel value */
#define LSYS_PTRP_BOOTSEL(x)                       ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_PTRP_BOOTSEL(x)                   ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_PTRP_UARTDL                       ((u32)0x00000001 << 19)          /*!<R 0  Power on trapped uart download value 0: Normal boot 1: uartdownload boot TODO: it may be overwrited by efuse autoload */
#define LSYS_PTRP_UARTDL(x)                        ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_PTRP_UARTDL(x)                    ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_BIT_PTRP_TMODE                        ((u32)0x00000001 << 17)          /*!<R 0  0: normal mode 1: test mode */
#define LSYS_PTRP_TMODE(x)                         ((u32)(((x) & 0x00000001) << 17))
#define LSYS_GET_PTRP_TMODE(x)                     ((u32)(((x >> 17) & 0x00000001)))
#define LSYS_BIT_PTRP_OTPBYP                       ((u32)0x00000001 << 16)          /*!<R 0  1: force efuse autoload fail */
#define LSYS_PTRP_OTPBYP(x)                        ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_PTRP_OTPBYP(x)                    ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_MASK_HW_CHIP_TYPE                     ((u32)0x00000003 << 8)          /*!<R 0  Chip type 00: ASIC 01: FPGA 10: PALADIUM 11: RTL_SIM */
#define LSYS_HW_CHIP_TYPE(x)                       ((u32)(((x) & 0x00000003) << 8))
#define LSYS_GET_HW_CHIP_TYPE(x)                   ((u32)(((x >> 8) & 0x00000003)))
#define LSYS_MASK_HW_CPU_ID                        ((u32)0x00000003 << 6)          /*!<R 0  Different CPU reading this register will return different value. If read by LP: return 0 If read by NP: return 1 if read by AP: return 2 */
#define LSYS_HW_CPU_ID(x)                          ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_HW_CPU_ID(x)                      ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_MASK_HW_VER_ID                        ((u32)0x00000003 << 4)          /*!<R 0  Vendor ID Download FW version 00: UMC 01: SMIC 10: TSMC */
#define LSYS_HW_VER_ID(x)                          ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_HW_VER_ID(x)                      ((u32)(((x >> 4) & 0x00000003)))
#define LSYS_MASK_HW_CHIP_ID                       ((u32)0x0000000F << 0)          /*!<R 4'b0000  Chip ID 0000: A-cut 0001: B-cut 0010: C-cut 0011: D-cut ... */
#define LSYS_HW_CHIP_ID(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define LSYS_GET_HW_CHIP_ID(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SYSTEM_CFG1
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_PTRP_ICFG                        ((u32)0x0000000F << 28)          /*!<R 0  Trapped ICFG data value 0: normall mode XXXX: the other mode (Test Mode) */
#define LSYS_PTRP_ICFG(x)                          ((u32)(((x) & 0x0000000F) << 28))
#define LSYS_GET_PTRP_ICFG(x)                      ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_MASK_ICFG_SW                          ((u32)0x0000000F << 24)          /*!<R/W 4'd2  Using soft virtual icfg in follow case : 1. icfg_sel is 1,if set icfg_sw = 0 , it will force to normal mode 2. testmode and icfg_sel is 0 and ptrp_icfg is 0 */
#define LSYS_ICFG_SW(x)                            ((u32)(((x) & 0x0000000F) << 24))
#define LSYS_GET_ICFG_SW(x)                        ((u32)(((x >> 24) & 0x0000000F)))
#define LSYS_BIT_ICFG_SEL                          ((u32)0x00000001 << 20)          /*!<R/W 0  If 1, using icfg_sw to replace trapped icfg value. */
#define LSYS_ICFG_SEL(x)                           ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_ICFG_SEL(x)                       ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_SYSPLL_CLKRDY                     ((u32)0x00000001 << 12)          /*!<R 0  System pll ready flag 1: ready */
#define LSYS_SYSPLL_CLKRDY(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_SYSPLL_CLKRDY(x)                  ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_OSC32K_CLKRDY                     ((u32)0x00000001 << 10)          /*!<R 0  OSC 128k ready flag 1: ready */
#define LSYS_OSC32K_CLKRDY(x)                      ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_OSC32K_CLKRDY(x)                  ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_XTAL32K_CLKRDY                    ((u32)0x00000001 << 9)          /*!<R 0  XTAL 32k ready flag 1: ready */
#define LSYS_XTAL32K_CLKRDY(x)                     ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_XTAL32K_CLKRDY(x)                 ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_XTAL_CLKRDY                       ((u32)0x00000001 << 8)          /*!<R 0  XTAL clock stable 1: Clock Stable */
#define LSYS_XTAL_CLKRDY(x)                        ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_XTAL_CLKRDY(x)                    ((u32)(((x >> 8) & 0x00000001)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_PRI_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_WIFI_ARPRI                       ((u32)0x00000003 << 30)          /*!<R/W 0  Wlan read pri control */
#define LSYS_WIFI_ARPRI(x)                         ((u32)(((x) & 0x00000003) << 30))
#define LSYS_GET_WIFI_ARPRI(x)                     ((u32)(((x >> 30) & 0x00000003)))
#define LSYS_MASK_WIFI_AWPRI                       ((u32)0x00000003 << 28)          /*!<R/W 0  Wlan write pri control */
#define LSYS_WIFI_AWPRI(x)                         ((u32)(((x) & 0x00000003) << 28))
#define LSYS_GET_WIFI_AWPRI(x)                     ((u32)(((x >> 28) & 0x00000003)))
#define LSYS_MASK_NP_CPU_ARPRI                     ((u32)0x00000003 << 26)          /*!<R/W 0  np cpu read pri control */
#define LSYS_NP_CPU_ARPRI(x)                       ((u32)(((x) & 0x00000003) << 26))
#define LSYS_GET_NP_CPU_ARPRI(x)                   ((u32)(((x >> 26) & 0x00000003)))
#define LSYS_MASK_NP_CPU_AWPRI                     ((u32)0x00000003 << 24)          /*!<R/W 0  np cpu write pri control */
#define LSYS_NP_CPU_AWPRI(x)                       ((u32)(((x) & 0x00000003) << 24))
#define LSYS_GET_NP_CPU_AWPRI(x)                   ((u32)(((x >> 24) & 0x00000003)))
#define LSYS_MASK_M1_TO_S0_ARPRI                   ((u32)0x00000003 << 22)          /*!<R/W 0  m1 to sys read pri control */
#define LSYS_M1_TO_S0_ARPRI(x)                     ((u32)(((x) & 0x00000003) << 22))
#define LSYS_GET_M1_TO_S0_ARPRI(x)                 ((u32)(((x >> 22) & 0x00000003)))
#define LSYS_MASK_M1_TO_S0_AWPRI                   ((u32)0x00000003 << 20)          /*!<R/W 0  m1 to sys write pri control */
#define LSYS_M1_TO_S0_AWPRI(x)                     ((u32)(((x) & 0x00000003) << 20))
#define LSYS_GET_M1_TO_S0_AWPRI(x)                 ((u32)(((x >> 20) & 0x00000003)))
#define LSYS_MASK_M1_TO_OTHER_ARPRI                ((u32)0x00000003 << 18)          /*!<R/W 0  m1 to other read pri control */
#define LSYS_M1_TO_OTHER_ARPRI(x)                  ((u32)(((x) & 0x00000003) << 18))
#define LSYS_GET_M1_TO_OTHER_ARPRI(x)              ((u32)(((x >> 18) & 0x00000003)))
#define LSYS_MASK_M1_TO_OTHER_AWPRI                ((u32)0x00000003 << 16)          /*!<R/W 0  m1 to other write pri control */
#define LSYS_M1_TO_OTHER_AWPRI(x)                  ((u32)(((x) & 0x00000003) << 16))
#define LSYS_GET_M1_TO_OTHER_AWPRI(x)              ((u32)(((x >> 16) & 0x00000003)))
#define LSYS_MASK_LX_ARPRI                         ((u32)0x00000003 << 14)          /*!<R/W 0  lx read pri control */
#define LSYS_LX_ARPRI(x)                           ((u32)(((x) & 0x00000003) << 14))
#define LSYS_GET_LX_ARPRI(x)                       ((u32)(((x >> 14) & 0x00000003)))
#define LSYS_MASK_LX_AWPRI                         ((u32)0x00000003 << 12)          /*!<R/W 0  lx write pri control */
#define LSYS_LX_AWPRI(x)                           ((u32)(((x) & 0x00000003) << 12))
#define LSYS_GET_LX_AWPRI(x)                       ((u32)(((x >> 12) & 0x00000003)))
#define LSYS_MASK_LCDC_ARPRI                       ((u32)0x00000003 << 10)          /*!<R/W 0  LCDC read pri control */
#define LSYS_LCDC_ARPRI(x)                         ((u32)(((x) & 0x00000003) << 10))
#define LSYS_GET_LCDC_ARPRI(x)                     ((u32)(((x >> 10) & 0x00000003)))
#define LSYS_MASK_LCDC_AWPRI                       ((u32)0x00000003 << 8)          /*!<R/W 0  LCDC write pri control */
#define LSYS_LCDC_AWPRI(x)                         ((u32)(((x) & 0x00000003) << 8))
#define LSYS_GET_LCDC_AWPRI(x)                     ((u32)(((x >> 8) & 0x00000003)))
#define LSYS_MASK_GDMA_ARPRI                       ((u32)0x00000003 << 6)          /*!<R/W 0  GDMA read pri control */
#define LSYS_GDMA_ARPRI(x)                         ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_GDMA_ARPRI(x)                     ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_MASK_GDMA_AWPRI                       ((u32)0x00000003 << 4)          /*!<R/W 0  GDMA write pri control */
#define LSYS_GDMA_AWPRI(x)                         ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_GDMA_AWPRI(x)                     ((u32)(((x >> 4) & 0x00000003)))
#define LSYS_MASK_CM0_ARPRI                        ((u32)0x00000003 << 2)          /*!<R/W 0  LS CM0 read pri control */
#define LSYS_CM0_ARPRI(x)                          ((u32)(((x) & 0x00000003) << 2))
#define LSYS_GET_CM0_ARPRI(x)                      ((u32)(((x >> 2) & 0x00000003)))
#define LSYS_MASK_CM0_AWPRI                        ((u32)0x00000003 << 0)          /*!<R/W 0  LS CM0 write pri control */
#define LSYS_CM0_AWPRI(x)                          ((u32)(((x) & 0x00000003) << 0))
#define LSYS_GET_CM0_AWPRI(x)                      ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_CM0_AREXPIRYTIMESTAMP            ((u32)0x0000FFFF << 16)          /*!<R/W 0  ls cm0 read expirytimestamp control */
#define LSYS_CM0_AREXPIRYTIMESTAMP(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_CM0_AREXPIRYTIMESTAMP(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_CM0_AWEXPIRYTIMESTAMP            ((u32)0x0000FFFF << 0)          /*!<R/W 0  ls cm0 write expirytimestamp control */
#define LSYS_CM0_AWEXPIRYTIMESTAMP(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_CM0_AWEXPIRYTIMESTAMP(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_GDMA_AREXPIRYTIMESTAMP           ((u32)0x0000FFFF << 16)          /*!<R/W 0  gdma read expirytimestamp control */
#define LSYS_GDMA_AREXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_GDMA_AREXPIRYTIMESTAMP(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_GDMA_AWEXPIRYTIMESTAMP           ((u32)0x0000FFFF << 0)          /*!<R/W 0  gdma write expirytimestamp control */
#define LSYS_GDMA_AWEXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_GDMA_AWEXPIRYTIMESTAMP(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_LCDC_AREXPIRYTIMESTAMP           ((u32)0x0000FFFF << 16)          /*!<R/W 0  lcdc read expirytimestamp control */
#define LSYS_LCDC_AREXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_LCDC_AREXPIRYTIMESTAMP(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_LCDC_AWEXPIRYTIMESTAMP           ((u32)0x0000FFFF << 0)          /*!<R/W 0  lcdc write expirytimestamp control */
#define LSYS_LCDC_AWEXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_LCDC_AWEXPIRYTIMESTAMP(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL3
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_LX_AREXPIRYTIMESTAMP             ((u32)0x0000FFFF << 16)          /*!<R/W 0  lx read expirytimestamp control */
#define LSYS_LX_AREXPIRYTIMESTAMP(x)               ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_LX_AREXPIRYTIMESTAMP(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_LX_AWEXPIRYTIMESTAMP             ((u32)0x0000FFFF << 0)          /*!<R/W 0  lx write expirytimestamp control */
#define LSYS_LX_AWEXPIRYTIMESTAMP(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_LX_AWEXPIRYTIMESTAMP(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL4
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_M1_TO_OTHER_AREXPIRYTIMESTAMP    ((u32)0x0000FFFF << 16)          /*!<R/W 0  m1 to other read expirytimestamp control */
#define LSYS_M1_TO_OTHER_AREXPIRYTIMESTAMP(x)      ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_M1_TO_OTHER_AREXPIRYTIMESTAMP(x)  ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_M1_TO_OTHER_AWEXPIRYTIMESTAMP    ((u32)0x0000FFFF << 0)          /*!<R/W 0  m1 to other write expirytimestamp control */
#define LSYS_M1_TO_OTHER_AWEXPIRYTIMESTAMP(x)      ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_M1_TO_OTHER_AWEXPIRYTIMESTAMP(x)  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL5
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_M1_TO_S0_AREXPIRYTIMESTAMP       ((u32)0x0000FFFF << 16)          /*!<R/W 0  m1 to sys read expirytimestamp control */
#define LSYS_M1_TO_S0_AREXPIRYTIMESTAMP(x)         ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_M1_TO_S0_AREXPIRYTIMESTAMP(x)     ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_M1_TO_S0_AWEXPIRYTIMESTAMP       ((u32)0x0000FFFF << 0)          /*!<R/W 0  m1 to sys write expirytimestamp control */
#define LSYS_M1_TO_S0_AWEXPIRYTIMESTAMP(x)         ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_M1_TO_S0_AWEXPIRYTIMESTAMP(x)     ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL6
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_NP_CPU_AREXPIRYTIMESTAMP         ((u32)0x0000FFFF << 16)          /*!<R/W 0  np cpu read expirytimestamp control */
#define LSYS_NP_CPU_AREXPIRYTIMESTAMP(x)           ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_NP_CPU_AREXPIRYTIMESTAMP(x)       ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_NP_CPU_AWEXPIRYTIMESTAMP         ((u32)0x0000FFFF << 0)          /*!<R/W 0  np cpu write expirytimestamp control */
#define LSYS_NP_CPU_AWEXPIRYTIMESTAMP(x)           ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_NP_CPU_AWEXPIRYTIMESTAMP(x)       ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_BUS_EXPSTAMP_CTRL7
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_WIFI_AREXPIRYTIMESTAMP           ((u32)0x0000FFFF << 16)          /*!<R/W 0  wlan read expirytimestamp control */
#define LSYS_WIFI_AREXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define LSYS_GET_WIFI_AREXPIRYTIMESTAMP(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define LSYS_MASK_WIFI_AWEXPIRYTIMESTAMP           ((u32)0x0000FFFF << 0)          /*!<R/W 0  wlan write expirytimestamp control */
#define LSYS_WIFI_AWEXPIRYTIMESTAMP(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_WIFI_AWEXPIRYTIMESTAMP(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SYSRST_MSK0
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_MSK_I2C2                         ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: mask HS I2C1 reset when sys reset happen 0: reset */
#define APBPeriph_MSK_I2C1                         ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: mask HS I2C0 reset when sys reset happen 0: reset */
#define APBPeriph_MSK_I2C0                         ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: mask LS I2C reset when sys reset happen 0: reset */
#define APBPeriph_WLAFE                            ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: mask WL RFAFE control circuit, shall be 1 when mask WL or BT use WL RFAFE 0: reset */
#define APBPeriph_MSK_GPIO                         ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 1  1: mask LS GPIO reset when sys reset happen 0: reset */
#define APBPeriph_MSK_CTC                          ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: mask Captouch ADC reset when sys reset happen 0: reset */
#define APBPeriph_MSK_ADC                          ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: mask LS ADC reset when sys reset happen 0: reset */
#define APBPeriph_LOGUART                          ((u32)(0  << 30) | (0x00000001 << 19) | (0x00000001 << 18)) /*!<R/W 0  mask reset when sys reset happen reset 1: mask 0: reset */
#define APBPeriph_MSK_IPC_LP                       ((u32)(0  << 30) | (0x00000001 << 16))  /*!<R/W 0  1: mask LS IPC reset when sys reset happen 0: reset */
#define APBPeriph_MSK_BTON                         ((u32)(0  << 30) | (0x00000001 << 13))  /*!<R/W 0  mask reset when sys reset happen mask 1: mask 0: disable */
#define APBPeriph_MSK_AIP                          ((u32)(0  << 30) | (0x00000001 << 12))  /*!<R/W 1  mask aip reset when sys reset happen mask */
#define APBPeriph_MSK_THM                          ((u32)(0  << 30) | (0x00000001 << 11))  /*!<R/W 0  mask thml reset when sys reset happen mask */
#define APBPeriph_MSK_DTIM                         ((u32)(0  << 30) | (0x00000001 << 10))  /*!<R/W 0  mask dtim reset when sys reset happen mask */
#define APBPeriph_MSK_SCE                          ((u32)(0  << 30) | (0x00000001 << 9))   /*!<R/W 0  mask rsip reset when sys reset happen mask 1: mask sce 0: disable */
#define APBPeriph_FLASH                            ((u32)(0  << 30) | (0x00000001 << 8))   /*!<R/W 0  mask Flash spic reset when sys reset happen mask 1: mask flash 0: disable flash */
#define APBPeriph_MSK_WLON                         ((u32)(0  << 30) | (0x00000001 << 7))   /*!<R/W 0  mask WLON reset when sys reset happen mask 1: mask 0: disable */
#define APBPeriph_MSK_NP                           ((u32)(0  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: mask KM4 0: reset */
#define APBPeriph_MSK_LP                           ((u32)(0  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: mask KM0 0: reset */
#define APBPeriph_MSK_HPLFM                        ((u32)(0  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: mask HP platform 0: reset */
#define APBPeriph_MSK_LPLFM                        ((u32)(0  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: mask LP platform 0: reset */
#define APBPeriph_MSK_SIC                          ((u32)(0  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: mask SIC reset when sys reset happen 0: reset */
#define APBPeriph_MSK_HPON                         ((u32)(0  << 30) | (0x00000001 << 1))   /*!<R/W 1  1: mask HS SYSON reset when sys reset happen 0: reset */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SYSRST_MSK1
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_MSK_ZGB                          ((u32)(1  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: mask zgb reset when sys reset happen 0: disable Note: for zgb is in wlan off domain , after turn wlan off , it need an 0->1 transiton */
#define APBPeriph_MSK_PSRAM                        ((u32)(1  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: mask PSRAM reset when sys reset happen 0: disable */
#define APBPeriph_MSK_AUDIO                        ((u32)(1  << 30) | (0x00000001 << 26))  /*!<R/W 0  When audio reset when sys reset happen mask , HS_SYSON_RAM domain must be on , for HSRAM memory at least in retention mode . 1: mask audio reset when sys reset happen 0: disable */
#define APBPeriph_MSK_AC                           ((u32)(1  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: mask audio codec active reset when sys reset happen 0: disable */
#define APBPeriph_MSK_DDRP                         ((u32)(1  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: mask ddr phy reset when sys reset happen 0: disable */
#define APBPeriph_MSK_DDRC                         ((u32)(1  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: mask ddr controller reset when sys reset happen 0: disable */
#define APBPeriph_MSK_IRDA                         ((u32)(1  << 30) | (0x00000001 << 22))  /*!<R/W 0  1: mask girda reset when sys reset happen 0: disable */
#define APBPeriph_MSK_ECDSA                        ((u32)(1  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: mask ecdsa reset when sys reset happen 0: disable */
#define APBPeriph_MSK_ED25519                      ((u32)(1  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: mask ed25519 reset when sys reset happen 0: disable */
#define APBPeriph_MSK_RSA                          ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: mask rsa reset when sys reset happen 0: disable */
#define APBPeriph_MSK_IPC_HP                       ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 0  1: mask IPC reset when sys reset happen 0: disable */
#define APBPeriph_MSK_LEDC                         ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: mask SPORT reset when sys reset happen 0: disable */
#define APBPeriph_MSK_TRNG                         ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: mask TRNG reset when sys reset happen 0: disable */
#define APBPeriph_MSK_USB                          ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 0  1: mask USB reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SDH                          ((u32)(1  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: mask SDH reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SPI1                         ((u32)(1  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: mask SPI1 reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SPI0                         ((u32)(1  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: mask SPI0 reset when sys reset happen 0: disable */
#define APBPeriph_MSK_GDMA                         ((u32)(1  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: mask GDMA0 reset when sys reset happen 0: disable */
#define APBPeriph_MSK_LCDC                         ((u32)(1  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: mask LCDC reset when sys reset happen 0: disable */
#define APBPeriph_MSK_IPSEC                        ((u32)(1  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: mask IPSEC reset when sys reset happen 0: disable */
#define APBPeriph_MSK_LX1                          ((u32)(1  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: mask LX1 system reset when sys reset happen 0: disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_SYSRST_MSK2
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_MSK_TIM0                         ((u32)(2  << 30) | (0x00000001 << 16))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM1                         ((u32)(2  << 30) | (0x00000001 << 17))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM2                         ((u32)(2  << 30) | (0x00000001 << 18))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM3                         ((u32)(2  << 30) | (0x00000001 << 19))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM4                         ((u32)(2  << 30) | (0x00000001 << 20))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM5                         ((u32)(2  << 30) | (0x00000001 << 21))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM6                         ((u32)(2  << 30) | (0x00000001 << 22))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM7                         ((u32)(2  << 30) | (0x00000001 << 23))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM8                         ((u32)(2  << 30) | (0x00000001 << 24))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM9                         ((u32)(2  << 30) | (0x00000001 << 25))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM10                        ((u32)(2  << 30) | (0x00000001 << 26))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM11                        ((u32)(2  << 30) | (0x00000001 << 27))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM12                        ((u32)(2  << 30) | (0x00000001 << 28))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_TIM13                        ((u32)(2  << 30) | (0x00000001 << 29))  /*!<R/W 14'h0  mask Timer0-7 reset when sys reset happen mask */
#define APBPeriph_MSK_SPORT0                       ((u32)(2  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: mask SPORT reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SPORT1                       ((u32)(2  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: mask SPORT reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SPORT2                       ((u32)(2  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: mask SPORT reset when sys reset happen 0: disable */
#define APBPeriph_MSK_SPORT3                       ((u32)(2  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: mask SPORT reset when sys reset happen 0: disable */
#define APBPeriph_UART0                            ((u32)(2  << 30) | (0x00000001 << 4) | (0x00000001 << 0)) /*!<R/W 0  Uart reset when sys reset happen reset (Note uart3 if for BT) */
#define APBPeriph_UART1                            ((u32)(2  << 30) | (0x00000001 << 5) | (0x00000001 << 1)) /*!<R/W 0  Uart reset when sys reset happen reset (Note uart3 if for BT) */
#define APBPeriph_UART2                            ((u32)(2  << 30) | (0x00000001 << 6) | (0x00000001 << 2)) /*!<R/W 0  Uart reset when sys reset happen reset (Note uart3 if for BT) */
#define APBPeriph_UART3                            ((u32)(2  << 30) | (0x00000001 << 7) | (0x00000001 << 3)) /*!<R/W 0  Uart reset when sys reset happen reset (Note uart3 if for BT) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_DUMMY_094
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_MASK_GPIOA_IE                         ((u32)0x000001FF << 0)          /*!<R/W 9'h1ff  ECO: GPIOA input enable */
#define LSYS_GPIOA_IE(x)                           ((u32)(((x) & 0x000001FF) << 0))
#define LSYS_GET_GPIOA_IE(x)                       ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_LSYS_DUMMY_098
 * @brief
 * @{
 *****************************************************************************/
#define LSYS_BIT_PWDPAD15N_CA                      ((u32)0x00000001 << 0)          /*!<R/W 0  ECO: control DDRPHY CA pad power down 0: power down 1: enable Before DDRPHY ready , this bit must set 1 to iso DRAM */
#define LSYS_PWDPAD15N_CA(x)                       ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_PWDPAD15N_CA(x)                   ((u32)(((x >> 0) & 0x00000001)))
#define LSYS_BIT_PWDPAD15N_DQ                      ((u32)0x00000001 << 1)          /*!<R/W 0  ECO: control DDRPHY DQ pad power down 0: power down 1: enable Before DDRPHY ready , this bit must set 1 to iso DRAM Note: DQ pad is shared as psram */
#define LSYS_PWDPAD15N_DQ(x)                       ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_PWDPAD15N_DQ(x)                   ((u32)(((x >> 1) & 0x00000001)))
/** @} */


/*==========LSYS Register Address Definition==========*/
#define REG_LSYS_PWC                                 0x0200
#define REG_LSYS_POR                                 0x0202
#define REG_LSYS_ISO                                 0x0204
#define REG_LSYS_FEN_GRP0                            0x0208
#define REG_LSYS_FEN_GRP1                            0x020C
#define REG_LSYS_FEN_GRP2                            0x0210
#define REG_LSYS_CKE_GRP0                            0x0214
#define REG_LSYS_CKE_GRP1                            0x0218
#define REG_LSYS_CKE_GRP2                            0x021C
#define REG_LSYS_CKE_GRP                             0x0220
#define REG_LSYS_CKSL_GRP0                           0x0224
#define REG_LSYS_CKD_GRP0                            0x0228
#define REG_LSYS_CKD_GRP1                            0x022C
#define REG_LSYS_SW_RST_CTRL                         0x0238
#define REG_LSYS_SW_RST_TRIG                         0x023C
#define REG_LSYS_LPLAT_CTRL                          0x0240
#define REG_LSYS_LPLAT_STATUS                        0x0244
#define REG_LSYS_SW_PATCH                            0x0248
#define REG_LSYS_SPORT_CLK                           0x024C
#define REG_LSYS_BT_CTRL0                            0x0250
#define REG_LSYS_BT_CTRL1                            0x0254
#define REG_LSYS_AIP_CTRL0                           0x0258
#define REG_LSYS_AIP_CTRL1                           0x025C
#define REG_LSYS_AIP_CTRL2                           0x0260
#define REG_LSYS_BOOT_REASON_SW                      0x0264
#define REG_LSYS_BOOT_CFG                            0x0268
#define REG_LSYS_FPGA_CLK_INFO                       0x026C
#define REG_LSYS_FLASH_PARA_ADDR                     0x0270
#define REG_LSYS_SCAN_CTRL                           0x0274
#define REG_LSYS_SYSTEM_CFG0                         0x027C
#define REG_LSYS_SYSTEM_CFG1                         0x0280
#define REG_LSYS_BUS_PRI_CTRL                        0x0290
#define REG_LSYS_BUS_EXPSTAMP_CTRL0                  0x02A0
#define REG_LSYS_BUS_EXPSTAMP_CTRL1                  0x02A4
#define REG_LSYS_BUS_EXPSTAMP_CTRL2                  0x02A8
#define REG_LSYS_BUS_EXPSTAMP_CTRL3                  0x02AC
#define REG_LSYS_BUS_EXPSTAMP_CTRL4                  0x02B0
#define REG_LSYS_BUS_EXPSTAMP_CTRL5                  0x02B4
#define REG_LSYS_BUS_EXPSTAMP_CTRL6                  0x02B8
#define REG_LSYS_BUS_EXPSTAMP_CTRL7                  0x02BC
#define REG_LSYS_DUMMY_088                           0x02C0
#define REG_LSYS_DUMMY_089                           0x02C4
#define REG_LSYS_SYSRST_MSK0                         0x02D0
#define REG_LSYS_SYSRST_MSK1                         0x02D4
#define REG_LSYS_SYSRST_MSK2                         0x02D8
#define REG_LSYS_BOUNDARY_CHK                        0x02F0
#define REG_LSYS_DUMMY_090                           0x02F4
#define REG_LSYS_DUMMY_094                           0x02F8
#define REG_LSYS_DUMMY_098                           0x02FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */
//Please add your defination here

enum CHIP_TYPE {
	CHIP_TYPE_ASIC = 0,
	CHIP_TYPE_FPGA = 1,
	CHIP_TYPE_PALADIUM = 2,
	CHIP_TYPE_RTLSIM = 3
};

#define BIT_FORCE_USE_S0_ICFG			LSYS_PTRP_ICFG(0x0000000e)
#define BIT_MCM_FLASH_PG1B_ICFG			LSYS_PTRP_ICFG(0x0000000d)
#define BIT_MCM_FLASH_PG_ICFG			LSYS_PTRP_ICFG(0x0000000c)

/*******************  Macro definition for Port M0 Master ID ********************/
#define SYSM0_MASTER_SIC                                       0
#define SYSM0_MASTER_OTPC                                    1
#define SYSM0_MASTER_KM0                                      2

/*****************************Macro definition for OTP SYSCFG1**************/
#define LSYS_BIT_DIS_BOOT_DELAY       ((u32)0x00000001 << 0)

/******************Macro definition for REG_LSYS_BOOT_REASON_SW **************/
#define REG_LSYS_AP_STATUS_SW			0x0266
//using Absolute Address plat_get_my_entrypoint in plat_helpers.S !! change carefully!!
#define LSYS_BIT_AP_WAKE_FROM_PG		((u32)0x00000001 << 0)
#define LSYS_BIT_AP_RUNNING				((u32)0x00000001 << 1)
#define LSYS_BIT_AP_ENABLE				((u32)0x00000001 << 2)

#define REG_LSYS_NP_STATUS_SW			0x0267
#define LSYS_BIT_NP_RUNNING				((u32)0x00000001 << 0)

/* MANUAL_GEN_END */

#endif
