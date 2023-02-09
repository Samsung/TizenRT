#ifndef _AMEBAD2_LSYS_H_
#define _AMEBAD2_LSYS_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup LSYS
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup LSYS_Register_Definitions LSYS Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REG_LSYS_PWC
 * @brief
 * @{
 **/
#define LSYS_BIT_PWC_BT                            ((u32)0x00000001 << 1)          /*!<R/W 0  This is BT function total power cut control 1: Enable BT power cut 0: Disable */
#define LSYS_PWC_BT(x)                             ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_PWC_BT(x)                         ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_POW_ON_SOC                        ((u32)0x00000001 << 0)          /*!<R/W 0  1:SYS PMC request to enable HP SOC power cut and diable ISO 0: SYS PMC request to disable HP SOC power cut and enable ISO */
#define LSYS_POW_ON_SOC(x)                         ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_POW_ON_SOC(x)                     ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_ISO
 * @brief
 * @{
 **/
#define LSYS_BIT_ISO_BT                            ((u32)0x00000001 << 1)          /*!<R/W 1  1: isolate signals from PD_LP_BT to other power domain 0: release iso */
#define LSYS_ISO_BT(x)                             ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_ISO_BT(x)                         ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_ISO_HP_SOC                        ((u32)0x00000001 << 0)          /*!<R 0  isolate signals from PD_HP_SOC to other power domain */
#define LSYS_ISO_HP_SOC(x)                         ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_ISO_HP_SOC(x)                     ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_FEN_GRP0
 * @brief
 * @{
 **/
#define APBPeriph_TIM14                            ((u32)(0  << 30) | (0x00000001 << 29))  /*!<R/W 0  1: enable TIMER 14 function 0: disable */
#define APBPeriph_TIM13                            ((u32)(0  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: enable TIMER 13 function 0: disable */
#define APBPeriph_TIM12                            ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: enable TIMER 12 function 0: disable */
#define APBPeriph_TIM11                            ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: enable TIMER 11 function 0: disable */
#define APBPeriph_TIM10                            ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: enable TIMER 10 function 0: disable */
#define APBPeriph_TIM_PWM                          ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: enable PWM TIMER function 0: disable */
#define APBPeriph_TIM_PULSE                        ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: enable PULSE TIMER function 0: disable */
#define APBPeriph_AC_AIP                           ((u32)(0  << 30) | (0x00000001 << 22))  /*!<R/W 0  1: enable audio codec analog ip active function, need level shift to 3.3V for audio aip 0: disable */
#define APBPeriph_SPORT1                           ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: enable SPORT1 function 0: disable */
#define APBPeriph_SPORT0                           ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: enable SPORT0 function 0: disable */
#define APBPeriph_AC                               ((u32)(0  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: enable audio codec digital ip active function 0: disable */
#define APBPeriph_TRNG                             ((u32)(0  << 30) | (0x00000001 << 18))  /*!<R/W 0  1: enable TRNG function 0: disable */
#define APBPeriph_ECDSA                            ((u32)(0  << 30) | (0x00000001 << 17))  /*!<R/W 0  1: enable ecdsa function 0: disable */
#define APBPeriph_LEDC                             ((u32)(0  << 30) | (0x00000001 << 16))  /*!<R/W 0  1: enable LEDC function 0: disable */
#define APBPeriph_SPI1                             ((u32)(0  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: enable SPI1 function 0: disable */
#define APBPeriph_SPI0                             ((u32)(0  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: enable SPI0 function 0: disable */
#define APBPeriph_PSRAM                            ((u32)(0  << 30) | (0x00000001 << 13))  /*!<R/W 0  1: enable PSRAM function 0: disable */
#define APBPeriph_AIP                              ((u32)(0  << 30) | (0x00000001 << 12))  /*!<R/W 1  AIP control function enable 1: enable 0: disable */
#define APBPeriph_SCE                              ((u32)(0  << 30) | (0x00000001 << 11))  /*!<R/W 0  Flash sce function enable 1: enable sce 0: disable */
#define APBPeriph_FLASH                            ((u32)(0  << 30) | (0x00000001 << 10))  /*!<R/W 0  Flash spic function enable 1: enable flash 0: disable flash */
#define APBPeriph_SHA                              ((u32)(0  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: enable IPSEC SHA function 0: disable */
#define APBPeriph_AES                              ((u32)(0  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: enable IPSEC AES function 0: disable */
#define APBPeriph_LX                               ((u32)(0  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: enable LX1 system function 0: disable */
#define APBPeriph_GDMA                             ((u32)(0  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: enable GDMA function 0: disable */
#define APBPeriph_IPC                              ((u32)(0  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: enable IPC function 0: reset */
#define APBPeriph_KM4                              ((u32)(0  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: enable KM4 0: reset */
#define APBPeriph_KR4                              ((u32)(0  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: enable KR4 0: reset */
#define APBPeriph_PLFM                             ((u32)(0  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: enable platform 0: reset */
#define APBPeriph_HSOC                             ((u32)(0  << 30) | (0x00000001 << 0))   /*!<R/W 0  1: enable hs soc sytem 0: reset all hs soc sytem , include km4/km0/plfm/hifi */
/** @} */

/** @defgroup REG_LSYS_FEN_GRP1
 * @brief
 * @{
 **/
#define APBPeriph_ZGB                              ((u32)(1  << 30) | (0x00000001 << 27))  /*!<R/W 0  zigbee function enable 1: enable 0: disable */
#define APBPeriph_THM                              ((u32)(1  << 30) | (0x00000001 << 25))  /*!<R/W 0  thermal function enable 1: enable 0: disable */
#define APBPeriph_CTC                              ((u32)(1  << 30) | (0x00000001 << 24))  /*!<R/W 0  captouch function enable 1: enable 0: disable */
#define APBPeriph_ADC                              ((u32)(1  << 30) | (0x00000001 << 23))  /*!<R/W 0  lp adc function enable 1: enable 0: disable */
#define APBPeriph_HIFI                             ((u32)(1  << 30) | (0x00000001 << 22))  /*!<R/W 0  HIFI core function enable 1: enable 0: disable */
#define APBPeriph_DTIM                             ((u32)(1  << 30) | (0x00000001 << 20))  /*!<R/W 1  Debug timer function enable 1: enable 0: disable */
#define APBPeriph_TIM00                            ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM01                            ((u32)(1  << 30) | (0x00000001 << 13))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM02                            ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM03                            ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM04                            ((u32)(1  << 30) | (0x00000001 << 16))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM05                            ((u32)(1  << 30) | (0x00000001 << 17))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM06                            ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM07                            ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_I2C1                             ((u32)(1  << 30) | (0x00000001 << 11))  /*!<R/W 0  I2C1 function enable 1: enable 0: disable */
#define APBPeriph_I2C0                             ((u32)(1  << 30) | (0x00000001 << 10))  /*!<R/W 0  I2C0 function enable 1: enable 0: disable */
#define APBPeriph_UART0                            ((u32)(1  << 30) | (0x00000001 << 6))   /*!<R/W 0  Uart function enable (Note uart3 if for BT) 1: enable 0: disable */
#define APBPeriph_UART1                            ((u32)(1  << 30) | (0x00000001 << 7))   /*!<R/W 0  Uart function enable (Note uart3 if for BT) 1: enable 0: disable */
#define APBPeriph_UART2                            ((u32)(1  << 30) | (0x00000001 << 8))   /*!<R/W 0  Uart function enable (Note uart3 if for BT) 1: enable 0: disable */
#define APBPeriph_UART3                            ((u32)(1  << 30) | (0x00000001 << 9))   /*!<R/W 0  Uart function enable (Note uart3 if for BT) 1: enable 0: disable */
#define APBPeriph_LOGUART                          ((u32)(1  << 30) | (0x00000001 << 5))   /*!<R/W 0  Uartlog function enable 1: enable 0: disable */
#define APBPeriph_GPIO                             ((u32)(1  << 30) | (0x00000001 << 4))   /*!<R/W 0  SGPIO function enable 1: enable 0: disable */
#define APBPeriph_WLAFE                            ((u32)(1  << 30) | (0x00000001 << 2))   /*!<R/W 0  WL RFAFE control function enable 1: enable 0: disable */
#define APBPeriph_BTON                             ((u32)(1  << 30) | (0x00000001 << 1))   /*!<R/W 0  BTON function enable 1: enable 0: disable */
#define APBPeriph_WLON                             ((u32)(1  << 30) | (0x00000001 << 0))   /*!<R/W 0  WLON function enable 1: enable 0: disable */
/** @} */

/** @defgroup REG_LSYS_CKE_GRP0
 * @brief
 * @{
 **/
#define APBPeriph_TIM14_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 29))  /*!<R/W 0  1: Enable TIMER 14 clock 0: Disable */
#define APBPeriph_TIM13_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: Enable TIMER 13 clock 0: Disable */
#define APBPeriph_TIM12_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: Enable TIMER 12 clock 0: Disable */
#define APBPeriph_TIM11_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: Enable TIMER 11 clock 0: Disable */
#define APBPeriph_TIM10_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: Enable TIMER 10 clock 0: Disable */
#define APBPeriph_TIM_PWM_CLOCK                    ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: Enable PWM TIMER clock 0: Disable */
#define APBPeriph_TIM_PULSE_CLOCK                  ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: Enable PULSE TIMER clock 0: Disable */
#define APBPeriph_SPORT1_CLOCK                     ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: Enable sport1 clock 0: Disable */
#define APBPeriph_SPORT0_CLOCK                     ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: Enable sport0 clock 0: Disable */
#define APBPeriph_AC_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: Enable audio codec clock 0: Disable */
#define APBPeriph_TRNG_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 18))  /*!<R/W 0  1: Enable trng bus clock 0: Disable */
#define APBPeriph_ECDSA_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 17))  /*!<R/W 0  1: Enable ecdsa engine clock 0: Disable */
#define APBPeriph_LEDC_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 16))  /*!<R/W 0  1: Enable ledc bus clock 0: Disable */
#define APBPeriph_SPI1_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: Enable SPI1 clock 0: Disable */
#define APBPeriph_SPI0_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: Enable SPI0 clock 0: Disable */
#define APBPeriph_PSRAM_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 13))  /*!<R/W 0  1: Enable PSRAM bus clock 0: Disable */
#define APBPeriph_FLASH_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable SPIC source clock */
#define APBPeriph_SHA_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: Enable ipsec SHA clock 0: Disable */
#define APBPeriph_AES_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: Enable ipsec AES clock 0: Disable */
#define APBPeriph_LX_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable lx1 bus clock 0: Disable */
#define APBPeriph_GDMA_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: Enable GDMA0 clock 0: Disable */
#define APBPeriph_IPC_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: Enable IPC clock 0: Disable */
#define APBPeriph_KM4_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable KM4 clock 0: Disable */
#define APBPeriph_KR4_CLOCK                        ((u32)(0  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: Enable KR4 clock 0: Disable */
#define APBPeriph_PLFM_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: Enable platform clock 0: Disable */
#define APBPeriph_LPON_CLOCK                       ((u32)(0  << 30) | (0x00000001 << 0))   /*!<R/W 1  1.Enable lpon clk 0: Disable Note: this bit is controlled HW PMC, SW shall not modify */
/** @} */

/** @defgroup REG_LSYS_CKE_GRP1
 * @brief
 * @{
 **/
#define APBPeriph_WMAC_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: Enable WMAC AXI and AHB bus clock 0: Disable */
#define APBPeriph_THM_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 25))  /*!<R/W 1  1: Enable LS thm clock 0: Disable */
#define APBPeriph_CTC_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: Enable Captouch clock 0: Disable */
#define APBPeriph_ADC_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: Enable LS ADC clock 0: Disable */
#define APBPeriph_HIFI_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 22))  /*!<R/W 0  1: Enable HIFI core clock 0: Disable */
#define APBPeriph_PLL_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 21))  /*!<R/W 1  1: Enable PLL dividor for KM4/KR4/SRAM 0: Disable when modify KM4/KR4/SRAM dividor param, need set this bit 0 and 1 before switch KM4/KR4/SRAM to PLL from XTAL/OSC4M */
#define APBPeriph_DTIM_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 20))  /*!<R/W 1  1: Enable LS dtim clk 0: Disable */
#define APBPeriph_TIM00_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM01_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 13))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM02_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM03_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM04_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 16))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM05_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 17))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM06_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_TIM07_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable 0: Disable */
#define APBPeriph_I2C1_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: Enable HS I2C0 clock 0: Disable */
#define APBPeriph_I2C0_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable LS I2C clock 0: Disable */
#define APBPeriph_UART0_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 6))   /*!<R/W 0  1: Enable Uart clock 0: Disable */
#define APBPeriph_UART1_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable Uart clock 0: Disable */
#define APBPeriph_UART2_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: Enable Uart clock 0: Disable */
#define APBPeriph_UART3_CLOCK                      ((u32)(1  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: Enable Uart clock 0: Disable */
#define APBPeriph_LOGUART_CLOCK                    ((u32)(1  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: Enable Uartlog clock 0: Disable */
#define APBPeriph_GPIO_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable GPIO clock 0: Disable */
#define APBPeriph_ZGB_CLOCK                        ((u32)(1  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: enable zigbee clock 0: Disable */
#define APBPeriph_BTON_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 1))   /*!<R/W 0  1: Enable BTON clock 0: Disable */
#define APBPeriph_WLON_CLOCK                       ((u32)(1  << 30) | (0x00000001 << 0))   /*!<R/W 0  1: Enable WLON clock 0: Disable */
/** @} */

/** @defgroup REG_LSYS_CKE_GRP2
 * @brief
 * @{
 **/
#define LSYS_MASK_CKE_XTAL_GRP                     ((u32)0x00000007 << 8)          /*!<R/W 3'b111  XTAL 40M clock used for different use , when neither func used [0]: LP system [1]: 40M peri [2]: 2M peri Note : This field is SW used, HW won't modify */
#define LSYS_CKE_XTAL_GRP(x)                       ((u32)(((x) & 0x00000007) << 8))
#define LSYS_GET_CKE_XTAL_GRP(x)                   ((u32)(((x >> 8) & 0x00000007)))
#define LSYS_MASK_CKE_OSC4M_GRP                    ((u32)0x0000003F << 0)          /*!<R/W 6'h0  OSC4M used for different use , when neither func used , osc4m can be disabled . 0: LP system 4M 1: OSC2M peri 2: WL/BT common 3: WL ana clock 4: BT ana clock 5: RSVD Note : This field is SW used, HW won't modify */
#define LSYS_CKE_OSC4M_GRP(x)                      ((u32)(((x) & 0x0000003F) << 0))
#define LSYS_GET_CKE_OSC4M_GRP(x)                  ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup REG_LSYS_CKSL_GRP0
 * @brief
 * @{
 **/
#define LSYS_BIT_CKSL_LOGUART                      ((u32)0x00000001 << 29)          /*!<R/W 0  Select loguart trx clock 0: clock is xtal(source is dicided by cksl_loguart_xtal) 1: clock is osc2M */
#define LSYS_CKSL_LOGUART(x)                       ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_CKSL_LOGUART(x)                   ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_MASK_CKSL_UART                        ((u32)0x0000000F << 25)          /*!<R/W 0  Select uart rx clock for every uart . cksl_uart[n](n=0,1,2,3) 0: rx clock is xtal(source is dicided by cksl_uart_xtal) 1: rx clock is osc2M */
#define LSYS_CKSL_UART(x)                          ((u32)(((x) & 0x0000000F) << 25))
#define LSYS_GET_CKSL_UART(x)                      ((u32)(((x >> 25) & 0x0000000F)))
#define LSYS_BIT_CKSL_LOGUART_XTAL                 ((u32)0x00000001 << 24)          /*!<R/W 0  loguart_xtal clk select 0:clk_xtal_digi 1:clk_xtal_digi_div2 */
#define LSYS_CKSL_LOGUART_XTAL(x)                  ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_CKSL_LOGUART_XTAL(x)              ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_MASK_CKSL_UART_XTAL                   ((u32)0x0000000F << 20)          /*!<R/W 0  uart xtal clk select, cksl_uart_xtal[n](n=0,1,2,3) 0: clk_xtal_digi 1:clk_xtal_digi_div2 */
#define LSYS_CKSL_UART_XTAL(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define LSYS_GET_CKSL_UART_XTAL(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define LSYS_BIT_CKSL_SPORT                        ((u32)0x00000001 << 18)          /*!<R/W 1h  sport clock selection 0: equal to audio codec main clock (40M) 1: from cpu/dsp pll , after pll selection(cksl_i2s_pll) */
#define LSYS_CKSL_SPORT(x)                         ((u32)(((x) & 0x00000001) << 18))
#define LSYS_GET_CKSL_SPORT(x)                     ((u32)(((x >> 18) & 0x00000001)))
#define LSYS_BIT_CKSL_AC                           ((u32)0x00000001 << 17)          /*!<R/W 1h  audio codec main clock selection 0: cpupll divided 40M 1: xtal 40M */
#define LSYS_CKSL_AC(x)                            ((u32)(((x) & 0x00000001) << 17))
#define LSYS_GET_CKSL_AC(x)                        ((u32)(((x >> 17) & 0x00000001)))
#define LSYS_BIT_CKSL_I2S_PLL                      ((u32)0x00000001 << 16)          /*!<R/W 0  source pll slection for sport clk 0: from cpu pll divider output 1: from dsp pll divider output */
#define LSYS_CKSL_I2S_PLL(x)                       ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_CKSL_I2S_PLL(x)                   ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_BIT_CKSL_VADM                         ((u32)0x00000001 << 15)          /*!<R/W 0  vad share memory clock select . Note, it must first change this bit to 1,and then shutdown hs plfm . When wakeup , it must first enable hs plfm , finally set this bit to 0 . 0: used by hs plfm clock 1: used by vad buffer ctrol */
#define LSYS_CKSL_VADM(x)                          ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_CKSL_VADM(x)                      ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_CKSL_CTC_CORE                     ((u32)0x00000001 << 12)          /*!<R/W 0h  CTC core clock selection 0: 131k 1: xtal2m divider Clock */
#define LSYS_CKSL_CTC_CORE(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_CKSL_CTC_CORE(x)                  ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_CKSL_ADC2M                        ((u32)0x00000001 << 11)          /*!<R/W 0  lp adc 2m clk selection 0: divider from osc4m 1: divider from xtal */
#define LSYS_CKSL_ADC2M(x)                         ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_CKSL_ADC2M(x)                     ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_BIT_CKSL_CTC                          ((u32)0x00000001 << 10)          /*!<R/W 1h  CTC REG clock selection 0: 131k 1: lbus clock */
#define LSYS_CKSL_CTC(x)                           ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_CKSL_CTC(x)                       ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_CKSL_ADC                          ((u32)0x00000001 << 9)          /*!<R/W 0  ADC INT clock selection 0: lbus clock 1: OSC2M */
#define LSYS_CKSL_ADC(x)                           ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_CKSL_ADC(x)                       ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_CKSL_GPIO                         ((u32)0x00000001 << 8)          /*!<R/W 0  GPIO INT clock selecton: 0: lbus clock 1: 32.768K clock from SDM */
#define LSYS_CKSL_GPIO(x)                          ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_CKSL_GPIO(x)                      ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_MASK_CKSL_PSRAM                       ((u32)0x00000003 << 6)          /*!<R/W 0  psram phy clock seleck cksl_psram[0]: 0--source clk is from cpu pll, 1--source clk is from dsp pll cksl_psram[1]: 0--final clk is from cpu/dsp pll divider 1: lbus clk 10/11: from lbus clock 00: from cpu pll divider output 01: from dsp pll divider output */
#define LSYS_CKSL_PSRAM(x)                         ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_CKSL_PSRAM(x)                     ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_MASK_CKSL_SPIC                        ((u32)0x00000003 << 4)          /*!<R/W 0  SPIC clock seleck 10/11: from lbus clock 00: from dsp pll divider output 01: from cpu pll divider output */
#define LSYS_CKSL_SPIC(x)                          ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_CKSL_SPIC(x)                      ((u32)(((x >> 4) & 0x00000003)))
#define LSYS_BIT_CKSL_HIFI                         ((u32)0x00000001 << 3)          /*!<R/W 0  HIFI core/bus clock seletion 1'b1: lbus clk 1'b0: from dsp pll divider */
#define LSYS_CKSL_HIFI(x)                          ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_CKSL_HIFI(x)                      ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_MASK_CKSL_PLFM                        ((u32)0x00000003 << 1)          /*!<R/W 0h  KM4/KR4/HBUS/SRAM clock seletion, they are sync except HBUS 00/01: from lbus clock 10: from cpu pll divider output 11: from dsp pll divider output */
#define LSYS_CKSL_PLFM(x)                          ((u32)(((x) & 0x00000003) << 1))
#define LSYS_GET_CKSL_PLFM(x)                      ((u32)(((x >> 1) & 0x00000003)))
#define LSYS_BIT_CKSL_LBUS                         ((u32)0x00000001 << 0)          /*!<R/W 0h  lbus clk select 0: osc4m 1:xtal */
#define LSYS_CKSL_LBUS(x)                          ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_CKSL_LBUS(x)                      ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_CKSL_GRP1
 * @brief
 * @{
 **/
#define LSYS_MASK_CKSL_XTAL                        ((u32)0x0000000F << 0)          /*!<R/W 0  xtal selection, Default=40MHz 0000: 40MHz 0001: 25MHz 0010: 13MHz 0011: 19.2MHz 0100: 20MHz 0101: 26MHz 0110: 38.4MHz 0111: 17.664MHz 1000: 16MHz 1001: 14.138MHz 1010: 12MHz 1011: 52MHz 1100: 48MHz 1101: 27MHz 1110: 24MHz 1111: none */
#define LSYS_CKSL_XTAL(x)                          ((u32)(((x) & 0x0000000F) << 0))
#define LSYS_GET_CKSL_XTAL(x)                      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_LSYS_CKD_GRP0
 * @brief
 * @{
 **/
#define LSYS_MASK_CKD_GDMA_AXI                     ((u32)0x0000000F << 28)          /*!<R/WPD 5h  gdma_axi clock divider, it is based on cpu/dsp pll 0~1: not valid 2: div3 3: div4 4: div5 5: div6 other: divider by this value + 1 */
#define LSYS_CKD_GDMA_AXI(x)                       ((u32)(((x) & 0x0000000F) << 28))
#define LSYS_GET_CKD_GDMA_AXI(x)                   ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_MASK_CKD_PSRAM                        ((u32)0x0000000F << 24)          /*!<R/WPD 1h  psram clock divider, it is based on cpu/dsp pll 0: div1 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_PSRAM(x)                          ((u32)(((x) & 0x0000000F) << 24))
#define LSYS_GET_CKD_PSRAM(x)                      ((u32)(((x >> 24) & 0x0000000F)))
#define LSYS_MASK_CKD_HBUS                         ((u32)0x0000000F << 20)          /*!<R/WPD 5h  hbus clock divider, it is based on cpu/dsp pll 0~1: not valid 2: div3 3: div4 4: div5 other: divider by this value + 1 */
#define LSYS_CKD_HBUS(x)                           ((u32)(((x) & 0x0000000F) << 20))
#define LSYS_GET_CKD_HBUS(x)                       ((u32)(((x >> 20) & 0x0000000F)))
#define LSYS_MASK_CKD_SRAM                         ((u32)0x0000000F << 16)          /*!<R/WPD 3h  sram clock divider, it is based on sys pll 0 : not valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_SRAM(x)                           ((u32)(((x) & 0x0000000F) << 16))
#define LSYS_GET_CKD_SRAM(x)                       ((u32)(((x >> 16) & 0x0000000F)))
#define LSYS_MASK_CKD_HIFI_BUS                     ((u32)0x0000000F << 12)          /*!<R/WPD 1h  HIFI bus clock divider, it is based on ckd_hifi_core 0: div1 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_HIFI_BUS(x)                       ((u32)(((x) & 0x0000000F) << 12))
#define LSYS_GET_CKD_HIFI_BUS(x)                   ((u32)(((x >> 12) & 0x0000000F)))
#define LSYS_MASK_CKD_HIFI_CORE                    ((u32)0x0000000F << 8)          /*!<R/WPD 0h  HIFI core clock divider, it is based on cpu/dsp pll 0: div1 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_HIFI_CORE(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define LSYS_GET_CKD_HIFI_CORE(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define LSYS_MASK_CKD_KR4                          ((u32)0x0000000F << 4)          /*!<R/WPD 3h  KR4 clock divider, it is based on cpu/dsp pll 0: div1 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_KR4(x)                            ((u32)(((x) & 0x0000000F) << 4))
#define LSYS_GET_CKD_KR4(x)                        ((u32)(((x >> 4) & 0x0000000F)))
#define LSYS_MASK_CKD_KM4                          ((u32)0x0000000F << 0)          /*!<R/WPD 3h  KM4 clock divider, it is based on cpu/dsp pll 0: div1 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_KM4(x)                            ((u32)(((x) & 0x0000000F) << 0))
#define LSYS_GET_CKD_KM4(x)                        ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_LSYS_CKD_GRP1
 * @brief
 * @{
 **/
#define LSYS_MASK_CKD_XTAL_2M                      ((u32)0x00007FFF << 17)          /*!<R/W 0x7D0  Output divided clock frequency for UART: BIT_LSYS_UART_XTAL_DIV_FREQ/1000 (MHz) */
#define LSYS_CKD_XTAL_2M(x)                        ((u32)(((x) & 0x00007FFF) << 17))
#define LSYS_GET_CKD_XTAL_2M(x)                    ((u32)(((x >> 17) & 0x00007FFF)))
#define LSYS_BIT_CKD_XTAL_2M_EN                    ((u32)0x00000001 << 16)          /*!<R/W 0  1: enable XTAL division circuit for UART */
#define LSYS_CKD_XTAL_2M_EN(x)                     ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_CKD_XTAL_2M_EN(x)                 ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_MASK_CKD_XTAL2M_CTC                   ((u32)0x0000000F << 12)          /*!<R/WPD Fh  xtal 2m divider for captouch core 0: no valid 1: div2 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_XTAL2M_CTC(x)                     ((u32)(((x) & 0x0000000F) << 12))
#define LSYS_GET_CKD_XTAL2M_CTC(x)                 ((u32)(((x >> 12) & 0x0000000F)))
#define LSYS_MASK_CKD_I2S_PLL                      ((u32)0x0000000F << 8)          /*!<R/WPD 5h  is2 pll divider, it is based on cpu/dsp pll output 0~2 : not valid 3: div4 4: div5 other: divider by this value + 1 */
#define LSYS_CKD_I2S_PLL(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define LSYS_GET_CKD_I2S_PLL(x)                    ((u32)(((x >> 8) & 0x0000000F)))
#define LSYS_MASK_CKD_ECDSA                        ((u32)0x0000000F << 4)          /*!<R/WPD 5h  ECDSA clock divider, it is based on cpu pll output 0~1: not valid 2: div3 3/other: divider by this value + 1 */
#define LSYS_CKD_ECDSA(x)                          ((u32)(((x) & 0x0000000F) << 4))
#define LSYS_GET_CKD_ECDSA(x)                      ((u32)(((x >> 4) & 0x0000000F)))
#define LSYS_MASK_CKD_AC                           ((u32)0x0000000F << 0)          /*!<R/WPD 0xE  audio main clock divider, it is based on cpu pll output 0~6 : not valid 7: div8 8: div9 other: divider by this value + 1 */
#define LSYS_CKD_AC(x)                             ((u32)(((x) & 0x0000000F) << 0))
#define LSYS_GET_CKD_AC(x)                         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_LSYS_SYSTEM_CFG0
 * @brief
 * @{
 **/
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
#define LSYS_MASK_HW_CPU_ID                        ((u32)0x00000003 << 6)          /*!<R 0  Different CPU reading this register will return different value. If read by KR4: return 0 If read by KM4: return 1 if read by HIFI5: return 2 */
#define LSYS_HW_CPU_ID(x)                          ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_HW_CPU_ID(x)                      ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_MASK_HW_VER_ID                        ((u32)0x00000003 << 4)          /*!<R 0  Vendor ID Download FW version 00: UMC 01: SMIC 10: TSMC */
#define LSYS_HW_VER_ID(x)                          ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_HW_VER_ID(x)                      ((u32)(((x >> 4) & 0x00000003)))
/** @} */

/** @defgroup REG_LSYS_SYSTEM_CFG1
 * @brief
 * @{
 **/
#define LSYS_MASK_PTRP_ICFG                        ((u32)0x0000000F << 28)          /*!<R 0  Trapped ICFG data value 0: normall mode XXXX: the other mode (Test Mode) */
#define LSYS_PTRP_ICFG(x)                          ((u32)(((x) & 0x0000000F) << 28))
#define LSYS_GET_PTRP_ICFG(x)                      ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_MASK_ICFG_SW                          ((u32)0x0000000F << 24)          /*!<R/W 0  icfg configured by SIC/SW */
#define LSYS_ICFG_SW(x)                            ((u32)(((x) & 0x0000000F) << 24))
#define LSYS_GET_ICFG_SW(x)                        ((u32)(((x >> 24) & 0x0000000F)))
#define LSYS_BIT_ICFG_SEL                          ((u32)0x00000001 << 20)          /*!<R/W 0  If 1, using icfg_sw to replace trapped icfg value. */
#define LSYS_ICFG_SEL(x)                           ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_ICFG_SEL(x)                       ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_PLIC_TIMER_CKEN                   ((u32)0x00000001 << 13)          /*!<R/W 1  clock enable of kr4 plic timer */
#define LSYS_PLIC_TIMER_CKEN(x)                    ((u32)(((x) & 0x00000001) << 13))
#define LSYS_GET_PLIC_TIMER_CKEN(x)                ((u32)(((x >> 13) & 0x00000001)))
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
#define LSYS_BIT_KR4_IS_NP                         ((u32)0x00000001 << 1)          /*!<R/W 1  1: kr4 is Network Processor, and is the LPS controller 0: km4 is Network Processor, and is the LPS controller */
#define LSYS_KR4_IS_NP(x)                          ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_KR4_IS_NP(x)                      ((u32)(((x >> 1) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_PLAT_CTRL
 * @brief
 * @{
 **/
#define LSYS_BIT_KM4_SEV_IN                        ((u32)0x00000001 << 28)          /*!<WA0 0h  write the bit to 1 will trig sev to km4 */
#define LSYS_KM4_SEV_IN(x)                         ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_KM4_SEV_IN(x)                     ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_BIT_SPI1_MST                          ((u32)0x00000001 << 27)          /*!<R/W 0h  1: SPI1 used as master 0: SPI1 used as slaver */
#define LSYS_SPI1_MST(x)                           ((u32)(((x) & 0x00000001) << 27))
#define LSYS_GET_SPI1_MST(x)                       ((u32)(((x >> 27) & 0x00000001)))
#define LSYS_BIT_SPI0_MST                          ((u32)0x00000001 << 26)          /*!<R/W 0h  1: SPI0 used as master 0: SPI0 used as slaver */
#define LSYS_SPI0_MST(x)                           ((u32)(((x) & 0x00000001) << 26))
#define LSYS_GET_SPI0_MST(x)                       ((u32)(((x >> 26) & 0x00000001)))
#define LSYS_BIT_SPIC_SPDUP_SIM                    ((u32)0x00000001 << 25)          /*!<R/W 0  1: SPIC will wait 256 spic_clk cycles before ready 0: SPIC will wait 2^16 spic_clk cycles before ready */
#define LSYS_SPIC_SPDUP_SIM(x)                     ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_SPIC_SPDUP_SIM(x)                 ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_PSRAM_SPDUPSIM                    ((u32)0x00000001 << 24)          /*!<R/W 0  psramc simulatin speedup enable */
#define LSYS_PSRAM_SPDUPSIM(x)                     ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_PSRAM_SPDUPSIM(x)                 ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_PSRAM_SPIC_BIG_ENDIAN             ((u32)0x00000001 << 23)          /*!<R/W 0  1: big endian 0: little endian */
#define LSYS_PSRAM_SPIC_BIG_ENDIAN(x)              ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_PSRAM_SPIC_BIG_ENDIAN(x)          ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_MASK_GDMA0_CCM                        ((u32)0x00000003 << 21)          /*!<R/W 0h  GDMA0 clock control mode */
#define LSYS_GDMA0_CCM(x)                          ((u32)(((x) & 0x00000003) << 21))
#define LSYS_GET_GDMA0_CCM(x)                      ((u32)(((x >> 21) & 0x00000003)))
#define LSYS_BIT_PLFM_MST0_SEL                     ((u32)0x00000001 << 20)          /*!<R/W 1  platfrom m0 select 0: KR4 1: OTPC */
#define LSYS_PLFM_MST0_SEL(x)                      ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_PLFM_MST0_SEL(x)                  ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_SHARE_BT_MEM                      ((u32)0x00000001 << 19)          /*!<R/W 0  1: Enable KM4 to share BT memory */
#define LSYS_SHARE_BT_MEM(x)                       ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_SHARE_BT_MEM(x)                   ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_BIT_SHARE_WL_MEM                      ((u32)0x00000001 << 18)          /*!<R/W 0  1: Enable KM4 to share WL memory */
#define LSYS_SHARE_WL_MEM(x)                       ((u32)(((x) & 0x00000001) << 18))
#define LSYS_GET_SHARE_WL_MEM(x)                   ((u32)(((x >> 18) & 0x00000001)))
#define LSYS_BIT_KR4_RETENTION_MODE                ((u32)0x00000001 << 17)          /*!<R/W 0  1: Enable, when KR4 reset, cache will not be cleared 0: Disable, when KR4 reset, cache will be cleared by HW */
#define LSYS_KR4_RETENTION_MODE(x)                 ((u32)(((x) & 0x00000001) << 17))
#define LSYS_GET_KR4_RETENTION_MODE(x)             ((u32)(((x >> 17) & 0x00000001)))
#define LSYS_BIT_KM4_RETENTION_MODE                ((u32)0x00000001 << 16)          /*!<R/W 0  1: Enable, when KM4 reset, cache will not be cleared 0: Disable, when KM4 reset, cache will be cleared by HW */
#define LSYS_KM4_RETENTION_MODE(x)                 ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_KM4_RETENTION_MODE(x)             ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_BIT_KR4_L2MEM_EN                      ((u32)0x00000001 << 11)          /*!<R/W 0  reset value of l2mem enable */
#define LSYS_KR4_L2MEM_EN(x)                       ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_KR4_L2MEM_EN(x)                   ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_MASK_HS_SRAM2_RD_WEIGHT               ((u32)0x00000007 << 8)          /*!<R/W 2h  Excuted read command number continuously if both read and write command reached */
#define LSYS_HS_SRAM2_RD_WEIGHT(x)                 ((u32)(((x) & 0x00000007) << 8))
#define LSYS_GET_HS_SRAM2_RD_WEIGHT(x)             ((u32)(((x >> 8) & 0x00000007)))
#define LSYS_MASK_HS_SRAM1_RD_WEIGHT               ((u32)0x00000007 << 5)          /*!<R/W 2h  Excuted read command number continuously if both read and write command reached */
#define LSYS_HS_SRAM1_RD_WEIGHT(x)                 ((u32)(((x) & 0x00000007) << 5))
#define LSYS_GET_HS_SRAM1_RD_WEIGHT(x)             ((u32)(((x >> 5) & 0x00000007)))
#define LSYS_MASK_HS_SRAM0_RD_WEIGHT               ((u32)0x00000007 << 2)          /*!<R/W 2h  Excuted read command number continuously if both read and write command reached */
#define LSYS_HS_SRAM0_RD_WEIGHT(x)                 ((u32)(((x) & 0x00000007) << 2))
#define LSYS_GET_HS_SRAM0_RD_WEIGHT(x)             ((u32)(((x >> 2) & 0x00000007)))
#define LSYS_BIT_HUK_PROT                          ((u32)0x00000001 << 0)          /*!<R/WH 0  0: only cpu can read HUK key 1: no one can read HUK key This bit will be latch to high when it was write to 1 until reset again . */
#define LSYS_HUK_PROT(x)                           ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_HUK_PROT(x)                       ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_PLAT_STATUS
 * @brief
 * @{
 **/
#define LSYS_BIT_INIT_XO_RANGE_EN                  ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define LSYS_INIT_XO_RANGE_EN(x)                   ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_INIT_XO_RANGE_EN(x)               ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_BIT_KR4_DBG_MODE                      ((u32)0x00000001 << 5)          /*!<R 0  KR4 debug status */
#define LSYS_KR4_DBG_MODE(x)                       ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_KR4_DBG_MODE(x)                   ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_KR4_SLP_MODE                      ((u32)0x00000001 << 4)          /*!<R 0  KR4 sleep status */
#define LSYS_KR4_SLP_MODE(x)                       ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_KR4_SLP_MODE(x)                   ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_KM4_DBGRESTARTED_M                ((u32)0x00000001 << 3)          /*!<R 0  KM4 debug restart status */
#define LSYS_KM4_DBGRESTARTED_M(x)                 ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_KM4_DBGRESTARTED_M(x)             ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_KM4_DBG_MODE                      ((u32)0x00000001 << 2)          /*!<R 0  KM4 halt status */
#define LSYS_KM4_DBG_MODE(x)                       ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_KM4_DBG_MODE(x)                   ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_KM4_LOCKUP_M                      ((u32)0x00000001 << 1)          /*!<R 0  KM4 lockup status */
#define LSYS_KM4_LOCKUP_M(x)                       ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_KM4_LOCKUP_M(x)                   ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_KM4_SLP_MODE                      ((u32)0x00000001 << 0)          /*!<R 0  KM4 sleep status */
#define LSYS_KM4_SLP_MODE(x)                       ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_KM4_SLP_MODE(x)                   ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_PLAT_INFO
 * @brief
 * @{
 **/
#define LSYS_MASK_SYSON_IOERR_INFO                 ((u32)0x00001FFF << 1)          /*!<R 0  indicate information of platform master non-secure access the masked syson reg bit2~0: master id, bit12~3: low bit11~2 address of syson reg */
#define LSYS_SYSON_IOERR_INFO(x)                   ((u32)(((x) & 0x00001FFF) << 1))
#define LSYS_GET_SYSON_IOERR_INFO(x)               ((u32)(((x >> 1) & 0x00001FFF)))
#define LSYS_BIT_ISR_SYSON_IOERR                   ((u32)0x00000001 << 0)          /*!<RW1C 0  indicate error that platform master non-secure access the masked syson reg */
#define LSYS_ISR_SYSON_IOERR(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_ISR_SYSON_IOERR(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_KM4_XO_CTRL0
 * @brief
 * @{
 **/
#define LSYS_MASK_INIT_XO_BASE                     ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define LSYS_INIT_XO_BASE(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_INIT_XO_BASE(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_KM4_XO_CTRL1
 * @brief
 * @{
 **/
#define LSYS_MASK_INIT_XO_TOP                      ((u32)0xFFFFFFFF << 0)          /*!<R/W 0   */
#define LSYS_INIT_XO_TOP(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_INIT_XO_TOP(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_HIFI_CTRL0
 * @brief
 * @{
 **/
#define LSYS_BIT_HIFI_WARMRST_REQ                  ((u32)0x00000001 << 15)          /*!<R/W 0  hifi warm reset request, high active */
#define LSYS_HIFI_WARMRST_REQ(x)                   ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_HIFI_WARMRST_REQ(x)               ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_HIFI_NMI_INTR                     ((u32)0x00000001 << 14)          /*!<WA0 0  hifi edge-triggered non-maskable interrupt, one pulse */
#define LSYS_HIFI_NMI_INTR(x)                      ((u32)(((x) & 0x00000001) << 14))
#define LSYS_GET_HIFI_NMI_INTR(x)                  ((u32)(((x >> 14) & 0x00000001)))
#define LSYS_MASK_HIFI_SHARE_MEM                   ((u32)0x00000003 << 12)          /*!<R/W 0  0:D ram don’t share memory to DTCM 1:D ram share 128K memory to DTCM 2:D ram share 256K memory to DTCM, 3 is rsvd */
#define LSYS_HIFI_SHARE_MEM(x)                     ((u32)(((x) & 0x00000003) << 12))
#define LSYS_GET_HIFI_SHARE_MEM(x)                 ((u32)(((x >> 12) & 0x00000003)))
#define LSYS_BIT_HIFI_PDEBUG_EN                    ((u32)0x00000001 << 6)          /*!<R/W 0  traceport debug enable, high active */
#define LSYS_HIFI_PDEBUG_EN(x)                     ((u32)(((x) & 0x00000001) << 6))
#define LSYS_GET_HIFI_PDEBUG_EN(x)                 ((u32)(((x >> 6) & 0x00000001)))
#define LSYS_BIT_HIFI_OCD_HALT_ON_RESET            ((u32)0x00000001 << 5)          /*!<R/W 0  on-chip debug mode(OCD) enable, high active */
#define LSYS_HIFI_OCD_HALT_ON_RESET(x)             ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_HIFI_OCD_HALT_ON_RESET(x)         ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_HIFI_RUN_STALL                    ((u32)0x00000001 << 4)          /*!<R/W 0  When asserted, stall the processor and shut off much of the clock tree */
#define LSYS_HIFI_RUN_STALL(x)                     ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_HIFI_RUN_STALL(x)                 ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_HIFI_STAT_VECTOR_SEL              ((u32)0x00000001 << 3)          /*!<R/W 0  When asserted, select External Reset Vector allows the position of the reset */
#define LSYS_HIFI_STAT_VECTOR_SEL(x)               ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_HIFI_STAT_VECTOR_SEL(x)           ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_HIFI_PRESETN                      ((u32)0x00000001 << 2)          /*!<R/W 0  APB reset, low active */
#define LSYS_HIFI_PRESETN(x)                       ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_HIFI_PRESETN(x)                   ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_HIFI_DRESETN                      ((u32)0x00000001 << 1)          /*!<R/W 0  debug reset,low active */
#define LSYS_HIFI_DRESETN(x)                       ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_HIFI_DRESETN(x)                   ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_HIFI_BRESETN                      ((u32)0x00000001 << 0)          /*!<R/W 0  core reset, low active */
#define LSYS_HIFI_BRESETN(x)                       ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_HIFI_BRESETN(x)                   ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_HIFI_CTRL1
 * @brief
 * @{
 **/
#define LSYS_MASK_HIFI_ALT_RESET_VEC               ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  External Reset Vector */
#define LSYS_HIFI_ALT_RESET_VEC(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_HIFI_ALT_RESET_VEC(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_HIFI_STATUS
 * @brief
 * @{
 **/
#define LSYS_MASK_HIFI_DBG_STATUS                  ((u32)0x0000003F << 16)          /*!<R 0  trace port debug status */
#define LSYS_HIFI_DBG_STATUS(x)                    ((u32)(((x) & 0x0000003F) << 16))
#define LSYS_GET_HIFI_DBG_STATUS(x)                ((u32)(((x >> 16) & 0x0000003F)))
#define LSYS_MASK_HIFI_DBG_DATA                    ((u32)0x0000001F << 8)          /*!<R 0  trace port debug data */
#define LSYS_HIFI_DBG_DATA(x)                      ((u32)(((x) & 0x0000001F) << 8))
#define LSYS_GET_HIFI_DBG_DATA(x)                  ((u32)(((x >> 8) & 0x0000001F)))
#define LSYS_BIT_HIFI_DBG_MODE                     ((u32)0x00000001 << 2)          /*!<R 0  core is on ocd mode but not maskable by SW */
#define LSYS_HIFI_DBG_MODE(x)                      ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_HIFI_DBG_MODE(x)                  ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_HIFI_OCD_MODE                     ((u32)0x00000001 << 1)          /*!<R 0  core is on ocd mode */
#define LSYS_HIFI_OCD_MODE(x)                      ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_HIFI_OCD_MODE(x)                  ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_HIFI_WAIT_MODE                    ((u32)0x00000001 << 0)          /*!<R 0  core is on wait for interrupt mode */
#define LSYS_HIFI_WAIT_MODE(x)                     ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_HIFI_WAIT_MODE(x)                 ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_HIFI_DBG_PC
 * @brief
 * @{
 **/
#define LSYS_MASK_HIFI_DBG_PC                      ((u32)0xFFFFFFFF << 0)          /*!<R 0  trace program counter */
#define LSYS_HIFI_DBG_PC(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_HIFI_DBG_PC(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_HIFI_DBG_INST
 * @brief
 * @{
 **/
#define LSYS_MASK_HIFI_DBG_INST                    ((u32)0xFFFFFFFF << 0)          /*!<R 0  Instruction information */
#define LSYS_HIFI_DBG_INST(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_HIFI_DBG_INST(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_SPIC_CTRL
 * @brief
 * @{
 **/
#define LSYS_MASK_DPHY_RD_DELAY_CNT                ((u32)0x0000000F << 17)          /*!<R/W 0  spic_dphy output dealy cnt, when set < 3, output data will occur at fourth or fifth axi clock from the rising edge of spi_rx_data_en; when set ≥ 3, data will occur after DPHY_RD_DELAY_CNT + 2 axi clock from the rising edge of spi_rx_data_en */
#define LSYS_DPHY_RD_DELAY_CNT(x)                  ((u32)(((x) & 0x0000000F) << 17))
#define LSYS_GET_DPHY_RD_DELAY_CNT(x)              ((u32)(((x >> 17) & 0x0000000F)))
#define LSYS_BIT_DPHY_EN                           ((u32)0x00000001 << 16)          /*!<R/W 0  when set, enable spic_dphy function to read data from flash */
#define LSYS_DPHY_EN(x)                            ((u32)(((x) & 0x00000001) << 16))
#define LSYS_GET_DPHY_EN(x)                        ((u32)(((x >> 16) & 0x00000001)))
#define LSYS_BIT_FLASH_CAL_EN                      ((u32)0x00000001 << 15)          /*!<R/W 0  Flash calibration enable 1: delay flash sout for calibration 0: bypass flash sout to spic */
#define LSYS_FLASH_CAL_EN(x)                       ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_FLASH_CAL_EN(x)                   ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_FLASH_DIV_EN                      ((u32)0x00000001 << 13)          /*!<R/W 0  1: enable to generate flash clock (no phase shift) divided by 600M pll clock, HW detect this signal's rising edge to start the no phase shift division circuit. */
#define LSYS_FLASH_DIV_EN(x)                       ((u32)(((x) & 0x00000001) << 13))
#define LSYS_GET_FLASH_DIV_EN(x)                   ((u32)(((x >> 13) & 0x00000001)))
#define LSYS_BIT_FLASH_PS_DIV_EN                   ((u32)0x00000001 << 12)          /*!<R/W 0  1: enable to generate flash clock (with phase shift) divided by 600M pll clock, HW detect this signal's rising edge to start the phase shift clock division circuit. */
#define LSYS_FLASH_PS_DIV_EN(x)                    ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_FLASH_PS_DIV_EN(x)                ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_MASK_FLASH_DIV_INT                    ((u32)0x0000000F << 8)          /*!<R/W 1  Flash clock division ratio, integrate part 0: not support 1: divided by 2 2: divided by 3 3: divided by 4 .. 7: divided by 8 8: divided by 9 9: divided by 10 Note: SPI clock frequency is this divided clock/2 */
#define LSYS_FLASH_DIV_INT(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define LSYS_GET_FLASH_DIV_INT(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define LSYS_MASK_FLASH_PS_PHASE                   ((u32)0x0000001F << 2)          /*!<R/W 0  Flash clock phase shift in units of cpu/dsp pll clock cycles. If selected pll is 500MHz, the step is 2ns. 0: shift 0 step 1: shift 1 step … 19: shift 19 step. Note, this value cannot be set more than FLASH_DIV_INT */
#define LSYS_FLASH_PS_PHASE(x)                     ((u32)(((x) & 0x0000001F) << 2))
#define LSYS_GET_FLASH_PS_PHASE(x)                 ((u32)(((x >> 2) & 0x0000001F)))
#define LSYS_BIT_FLASH_PS_DIV_RDY                  ((u32)0x00000001 << 1)          /*!<R 0  Ready flag of Flash clock with phase shift, Read only (input signal) */
#define LSYS_FLASH_PS_DIV_RDY(x)                   ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_FLASH_PS_DIV_RDY(x)               ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_FLASH_DIV_RDY                     ((u32)0x00000001 << 0)          /*!<R 0  Ready flag of Flash clock, Read only (input signal) */
#define LSYS_FLASH_DIV_RDY(x)                      ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_FLASH_DIV_RDY(x)                  ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_SW_RST_CTRL
 * @brief
 * @{
 **/
#define LSYS_BIT_KM4_SYS_RST                       ((u32)0x00000001 << 31)          /*!<R/WE/EC 0  Only when REG_SW_RST_TRIG is 0x96969696 , this reg can be write */
#define LSYS_KM4_SYS_RST(x)                        ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_KM4_SYS_RST(x)                    ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_BIT_KR4_SYS_RST                       ((u32)0x00000001 << 30)          /*!<R/WE/EC 0  Only when REG_SW_RST_TRIG is 0x96969696 , this reg can be write */
#define LSYS_KR4_SYS_RST(x)                        ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_KR4_SYS_RST(x)                    ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_HIFI_SYS_RST                      ((u32)0x00000001 << 29)          /*!<R/WE/EC 0  Only when REG_SW_RST_TRIG is 0x96969696 , this reg can be write */
#define LSYS_HIFI_SYS_RST(x)                       ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_HIFI_SYS_RST(x)                   ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_PERIALL_RST_EN                    ((u32)0x00000001 << 1)          /*!<R/W 0  1. warm reset request by one cpu will reset all peri and local cpu 0. warm reset request by one cpu will only reset local cpu Note : peri rst will use sysrst mst also */
#define LSYS_PERIALL_RST_EN(x)                     ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_PERIALL_RST_EN(x)                 ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_OTHERCPU_RST_EN                   ((u32)0x00000001 << 0)          /*!<R/W 0  1. warm reset request by one cpu will reset other cpu's core and local cpu 0. warm reset request by one cpu will only reset local cpu */
#define LSYS_OTHERCPU_RST_EN(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_OTHERCPU_RST_EN(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_SW_RST_TRIG
 * @brief
 * @{
 **/
#define LSYS_MASK_TRIG_RST                         ((u32)0xFFFFFFFF << 0)          /*!<R/WPD 0  System reset trigger register Write 0x96969696 to enable the write of reset control Write 0x69696969 will trigger the system if REG_SW_RST_CTRL set . */
#define LSYS_TRIG_RST(x)                           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_TRIG_RST(x)                       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_BOOT_CFG
 * @brief
 * @{
 **/
#define LSYS_BIT_BOOT_WAKE_FROM_PS_HS              ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define LSYS_BOOT_WAKE_FROM_PS_HS(x)               ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_BOOT_WAKE_FROM_PS_HS(x)           ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_BOOT_PATCH_FUNC0                  ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define LSYS_BOOT_PATCH_FUNC0(x)                   ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_BOOT_PATCH_FUNC0(x)               ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_BIT_BOOT_PATCH_FUNC1                  ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define LSYS_BOOT_PATCH_FUNC1(x)                   ((u32)(((x) & 0x00000001) << 27))
#define LSYS_GET_BOOT_PATCH_FUNC1(x)               ((u32)(((x >> 27) & 0x00000001)))
#define LSYS_BIT_BOOT_KR4_RUN                      ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define LSYS_BOOT_KR4_RUN(x)                       ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_BOOT_KR4_RUN(x)                   ((u32)(((x >> 25) & 0x00000001)))
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

/** @defgroup REG_LSYS_SW_PATCH
 * @brief
 * @{
 **/
#define LSYS_MASK_SW_PATCH_SADDR                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x20020000  SW patch code start addr. OTP physical addr 0x500-0x6FF will autoload to this addr . */
#define LSYS_SW_PATCH_SADDR(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_SW_PATCH_SADDR(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_CHIP_INFO
 * @brief
 * @{
 **/
#define LSYS_MASK_CHIP_INFO_EN                     ((u32)0x0000000F << 28)          /*!<R/W 0  Enable Internal Chip Information display. 0xA: enable. Others: disable. Note: CHIP_INFO can be read only if CHIP_INFO_EN[3:0]=0xA, otherwise the read result will return 0x0. */
#define LSYS_CHIP_INFO_EN(x)                       ((u32)(((x) & 0x0000000F) << 28))
#define LSYS_GET_CHIP_INFO_EN(x)                   ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_MASK_RL_VER                           ((u32)0x0000000F << 16)          /*!<R 0  CHIP cut verison : 0: A-cut 1: B-cut … */
#define LSYS_RL_VER(x)                             ((u32)(((x) & 0x0000000F) << 16))
#define LSYS_GET_RL_VER(x)                         ((u32)(((x >> 16) & 0x0000000F)))
#define LSYS_MASK_RL_NO                            ((u32)0x0000FFFF << 0)          /*!<R 0x6842  CHIP RL NUMBER : 0x6842 */
#define LSYS_RL_NO(x)                              ((u32)(((x) & 0x0000FFFF) << 0))
#define LSYS_GET_RL_NO(x)                          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup REG_LSYS_BT_CTRL0
 * @brief
 * @{
 **/
#define LSYS_BIT_BT_USE_EXT_BT                     ((u32)0x00000001 << 26)          /*!<R/W 0  1: select external BT 0: select internal BT */
#define LSYS_BT_USE_EXT_BT(x)                      ((u32)(((x) & 0x00000001) << 26))
#define LSYS_GET_BT_USE_EXT_BT(x)                  ((u32)(((x >> 26) & 0x00000001)))
#define LSYS_BIT_WL_USE_REQ                        ((u32)0x00000001 << 25)          /*!<R/W 0  who uses loguart trx pad depends on the xor of WL_USE_REQ and BT_USE_REQ, wifi can send use request by change the value of WL_USE_REQ */
#define LSYS_WL_USE_REQ(x)                         ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_WL_USE_REQ(x)                     ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_BTWL_USE_LOGUART_PAD_ST           ((u32)0x00000001 << 24)          /*!<R   The state of xor of WL_USE_REQ and BT_USE_REQ. 0: loguart use loguart trx pad 1: bt hci use loguart trx pad */
#define LSYS_BTWL_USE_LOGUART_PAD_ST(x)            ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_BTWL_USE_LOGUART_PAD_ST(x)        ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_FORCE_LOGUART_USE_LOGUART_PAD_B   ((u32)0x00000001 << 23)          /*!<R/W 0  Force wifi soc to use loguart pad mandatory if necessary 0: loguart use loguart trx pad 1: who uses loguart trx pad depends on BTWL_USE_LOGUART_PAD_ST */
#define LSYS_FORCE_LOGUART_USE_LOGUART_PAD_B(x)    ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_FORCE_LOGUART_USE_LOGUART_PAD_B(x) ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_BIT_BT_EN_TST_SI_CTRL_RFC             ((u32)0x00000001 << 20)          /*!<R/W 1  进入Test Mode 8,9 , 且同時 BT_EN_TST_SI_CTRL_RFC=1 , 才將RFC 端的SI 3 wire交由專線BT_SI_*_TST 控制； 若I2C 想要access RFC,设置BT_EN_TST_SI_CTRL_RFC=0 或者 將BT_SI_ENB_TST pull high(如果BT_SI_ENB_TST 有出 PIN) */
#define LSYS_BT_EN_TST_SI_CTRL_RFC(x)              ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_BT_EN_TST_SI_CTRL_RFC(x)          ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_BT_USE_EXT_UART                   ((u32)0x00000001 << 19)          /*!<R/W 0  1: BT use external uart 0: BT use internal uart */
#define LSYS_BT_USE_EXT_UART(x)                    ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_BT_USE_EXT_UART(x)                ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_MASK_BT_GPIO_FUN_SEL                  ((u32)0x00000007 << 16)          /*!<R/W 0  BT GPIO function select, 5:I2C mode */
#define LSYS_BT_GPIO_FUN_SEL(x)                    ((u32)(((x) & 0x00000007) << 16))
#define LSYS_GET_BT_GPIO_FUN_SEL(x)                ((u32)(((x >> 16) & 0x00000007)))
#define LSYS_BIT_BT_WAKE_HOST_POL                  ((u32)0x00000001 << 15)          /*!<R/W 1  1:BT_WAKE_HOST to CPU and pinmux 0:pinmux to CPU for external trig */
#define LSYS_BT_WAKE_HOST_POL(x)                   ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_BT_WAKE_HOST_POL(x)               ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_HOST_WAKE_BT_POL                  ((u32)0x00000001 << 14)          /*!<R/W 1  1:HOST_WAKE_BT to BT and pinmux 0:pinmux to BT for external trig */
#define LSYS_HOST_WAKE_BT_POL(x)                   ((u32)(((x) & 0x00000001) << 14))
#define LSYS_GET_HOST_WAKE_BT_POL(x)               ((u32)(((x >> 14) & 0x00000001)))
#define LSYS_BIT_HOST_WAKE_BT                      ((u32)0x00000001 << 13)          /*!<R/W 0  CPU set this bit to 1 to wake BT */
#define LSYS_HOST_WAKE_BT(x)                       ((u32)(((x) & 0x00000001) << 13))
#define LSYS_GET_HOST_WAKE_BT(x)                   ((u32)(((x >> 13) & 0x00000001)))
#define LSYS_BIT_BT_AHB_ACCESS_GNT                 ((u32)0x00000001 << 12)          /*!<R 0  access grant for system master access BT reg through AHB bus */
#define LSYS_BT_AHB_ACCESS_GNT(x)                  ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_BT_AHB_ACCESS_GNT(x)              ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_BT_AHB_ACCESS_REQ                 ((u32)0x00000001 << 11)          /*!<R/W 0  system master set this bit to 1 and wait BT_AHB_ACCESS_GNT=1 before access BT reg through AHB bus, when access finish, shall set this bit to 0 */
#define LSYS_BT_AHB_ACCESS_REQ(x)                  ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_BT_AHB_ACCESS_REQ(x)              ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_MASK_BT_OFF_PC_WT                     ((u32)0x00000003 << 6)          /*!<R/W 0  云 */
#define LSYS_BT_OFF_PC_WT(x)                       ((u32)(((x) & 0x00000003) << 6))
#define LSYS_GET_BT_OFF_PC_WT(x)                   ((u32)(((x >> 6) & 0x00000003)))
#define LSYS_BIT_DSWR_REF_BT                       ((u32)0x00000001 << 3)          /*!<R/W 1  core swr parameter refer BT status */
#define LSYS_DSWR_REF_BT(x)                        ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_DSWR_REF_BT(x)                    ((u32)(((x >> 3) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_BT_CTRL1
 * @brief
 * @{
 **/
#define LSYS_MASK_BT_PMC_STATE                     ((u32)0x00007FFF << 0)          /*!<R 0  indicate bt_misc PMC current state, for debug */
#define LSYS_BT_PMC_STATE(x)                       ((u32)(((x) & 0x00007FFF) << 0))
#define LSYS_GET_BT_PMC_STATE(x)                   ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/** @defgroup REG_LSYS_AIP_CTRL0
 * @brief
 * @{
 **/
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

/** @defgroup REG_LSYS_AIP_CTRL1
 * @brief
 * @{
 **/
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

/** @defgroup REG_LSYS_DW_GPIO_CTRL
 * @brief
 * @{
 **/
#define LSYS_MASK_GPIO_DIVCNT_PB                   ((u32)0x0000007F << 8)          /*!<R/W 0  divider for dw gpiob debounce clk */
#define LSYS_GPIO_DIVCNT_PB(x)                     ((u32)(((x) & 0x0000007F) << 8))
#define LSYS_GET_GPIO_DIVCNT_PB(x)                 ((u32)(((x >> 8) & 0x0000007F)))
#define LSYS_MASK_GPIO_DIVCNT_PA                   ((u32)0x0000007F << 0)          /*!<R/W 0  divider for dw gpioa debounce clk */
#define LSYS_GPIO_DIVCNT_PA(x)                     ((u32)(((x) & 0x0000007F) << 0))
#define LSYS_GET_GPIO_DIVCNT_PA(x)                 ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup REG_LSYS_LXBUS_DBG0
 * @brief
 * @{
 **/
#define LSYS_MASK_TIMEOUT_ADDR_MST                 ((u32)0xFFFFFFFF << 0)          /*!<R 0  lx sys bus addr debug out */
#define LSYS_TIMEOUT_ADDR_MST(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_TIMEOUT_ADDR_MST(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_LXBUS_DBG1
 * @brief
 * @{
 **/
#define LSYS_MASK_TIMEOUT_ADDR_SLV                 ((u32)0xFFFFFFFF << 0)          /*!<R 0  lx sys bus addr debug out */
#define LSYS_TIMEOUT_ADDR_SLV(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_TIMEOUT_ADDR_SLV(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_LXBUS_DBG_CTRL
 * @brief
 * @{
 **/
#define LSYS_BIT_LX1_LOCK_OK                       ((u32)0x00000001 << 12)          /*!<R 0  lock lx1 bus lock ok */
#define LSYS_LX1_LOCK_OK(x)                        ((u32)(((x) & 0x00000001) << 12))
#define LSYS_GET_LX1_LOCK_OK(x)                    ((u32)(((x >> 12) & 0x00000001)))
#define LSYS_BIT_LX1_LOCK_BUS                      ((u32)0x00000001 << 8)          /*!<R/W 0  lock lx1 bus for debug */
#define LSYS_LX1_LOCK_BUS(x)                       ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_LX1_LOCK_BUS(x)                   ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_MASK_BUSTIMEOUT                       ((u32)0x00000003 << 4)          /*!<R/W 0  lx sys bus arb timeout thresh */
#define LSYS_BUSTIMEOUT(x)                         ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_BUSTIMEOUT(x)                     ((u32)(((x >> 4) & 0x00000003)))
#define LSYS_BIT_ENBUSTIMEOUT                      ((u32)0x00000001 << 0)          /*!<R/W 0  lx sys bus arb timeout enable */
#define LSYS_ENBUSTIMEOUT(x)                       ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_ENBUSTIMEOUT(x)                   ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_FPGA_CLK_INFO
 * @brief
 * @{
 **/
#define LSYS_MASK_FPGA_KR4_CLK                     ((u32)0x000000FF << 24)          /*!<R 0  FPGA KR4 clock */
#define LSYS_FPGA_KR4_CLK(x)                       ((u32)(((x) & 0x000000FF) << 24))
#define LSYS_GET_FPGA_KR4_CLK(x)                   ((u32)(((x >> 24) & 0x000000FF)))
#define LSYS_MASK_FPGA_KM4_CLK                     ((u32)0x000000FF << 16)          /*!<R 0  FPGA KM4 clock */
#define LSYS_FPGA_KM4_CLK(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define LSYS_GET_FPGA_KM4_CLK(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define LSYS_MASK_FPGA_PSRAM_CLK                   ((u32)0x000000FF << 8)          /*!<R 0  FPGA PSRAM clock */
#define LSYS_FPGA_PSRAM_CLK(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define LSYS_GET_FPGA_PSRAM_CLK(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define LSYS_MASK_FPGA_HIFI_CLK                    ((u32)0x000000FF << 0)          /*!<R 0  FPGA HIFI clock */
#define LSYS_FPGA_HIFI_CLK(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define LSYS_GET_FPGA_HIFI_CLK(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_LSYS_WDG_GLBRST
 * @brief
 * @{
 **/
#define LSYS_BIT_EN_HIFI_RST_GLB                   ((u32)0x00000001 << 31)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, the global reset occurs only caused by timeout */
#define LSYS_EN_HIFI_RST_GLB(x)                    ((u32)(((x) & 0x00000001) << 31))
#define LSYS_GET_EN_HIFI_RST_GLB(x)                ((u32)(((x >> 31) & 0x00000001)))
#define LSYS_BIT_EN_KR4_RST_GLB                    ((u32)0x00000001 << 30)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, if the kr4 is np, the global reset occurs only caused by timeout */
#define LSYS_EN_KR4_RST_GLB(x)                     ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_EN_KR4_RST_GLB(x)                 ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_EN_KM4_RST_GLB                    ((u32)0x00000001 << 29)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, if the km4 is np, the global reset occurs only caused by timeout */
#define LSYS_EN_KM4_RST_GLB(x)                     ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_EN_KM4_RST_GLB(x)                 ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_CLR_WDG1_RST                      ((u32)0x00000001 << 11)          /*!<WA0 0  write 1 to clear the km4_s_wdg_rst, glb_wdg_rst is not included */
#define LSYS_CLR_WDG1_RST(x)                       ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_CLR_WDG1_RST(x)                   ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_BIT_CLR_WDG2_RST                      ((u32)0x00000001 << 10)          /*!<WA0 0  write 1 to clear the km4_ns_wdg_rst, glb_wdg_rst is not included */
#define LSYS_CLR_WDG2_RST(x)                       ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_CLR_WDG2_RST(x)                   ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_CLR_WDG3_RST                      ((u32)0x00000001 << 9)          /*!<WA0 0  write 1 to clear the kr4_wdg_rst, glb_wdg_rst is not included */
#define LSYS_CLR_WDG3_RST(x)                       ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_CLR_WDG3_RST(x)                   ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_CLR_WDG4_RST                      ((u32)0x00000001 << 8)          /*!<WA0 0  write 1 to clear the dsp_wdg_rst, glb_wdg_rst is not included */
#define LSYS_CLR_WDG4_RST(x)                       ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_CLR_WDG4_RST(x)                   ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_MASK_WDG_TIMEOUT                      ((u32)0x000000FF << 0)          /*!<R/W 0  threshold of wdg timer, if wdg_timer >= wdg_timeout, will trigger global reset. Unit is 10us 0 means not support */
#define LSYS_WDG_TIMEOUT(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define LSYS_GET_WDG_TIMEOUT(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_LSYS_PRS_CTRL
 * @brief
 * @{
 **/
#define LSYS_MASK_PRS_SRC_SEL                      ((u32)0x0000000F << 28)          /*!<R/W 0  PRS trigger source select: 000: none 001: gpio trigger 010: comparator output 011: audio zcd_0 100: audio zcd_1 101: audio zcd_2 */
#define LSYS_PRS_SRC_SEL(x)                        ((u32)(((x) & 0x0000000F) << 28))
#define LSYS_GET_PRS_SRC_SEL(x)                    ((u32)(((x >> 28) & 0x0000000F)))
#define LSYS_BIT_PRS_OUT_PWM_TRIG                  ((u32)0x00000001 << 21)          /*!<R/W 0  enable for output trigger to pwm */
#define LSYS_PRS_OUT_PWM_TRIG(x)                   ((u32)(((x) & 0x00000001) << 21))
#define LSYS_GET_PRS_OUT_PWM_TRIG(x)               ((u32)(((x >> 21) & 0x00000001)))
#define LSYS_BIT_PRS_OUT_TIM_EN                    ((u32)0x00000001 << 20)          /*!<R/W 0  enable for output en signal to basic timer */
#define LSYS_PRS_OUT_TIM_EN(x)                     ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_PRS_OUT_TIM_EN(x)                 ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_PRS_IN_TIM_OF                     ((u32)0x00000001 << 19)          /*!<R/W 0  enable for basic timer overflow input */
#define LSYS_PRS_IN_TIM_OF(x)                      ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_PRS_IN_TIM_OF(x)                  ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_BIT_PRS_RVS_EN                        ((u32)0x00000001 << 18)          /*!<R/W 0  invert polarity for PRS input, 0: posedge polarity 1: negedge polarity */
#define LSYS_PRS_RVS_EN(x)                         ((u32)(((x) & 0x00000001) << 18))
#define LSYS_GET_PRS_RVS_EN(x)                     ((u32)(((x >> 18) & 0x00000001)))
#define LSYS_MASK_PRS_DBC_CNT                      ((u32)0x0000FFFF << 2)          /*!<R/W 0  debounce count for PRS funciton */
#define LSYS_PRS_DBC_CNT(x)                        ((u32)(((x) & 0x0000FFFF) << 2))
#define LSYS_GET_PRS_DBC_CNT(x)                    ((u32)(((x >> 2) & 0x0000FFFF)))
/** @} */

/** @defgroup REG_LSYS_SYSRST_MSK0
 * @brief
 * @{
 **/
#define LSYS_BIT_SYSRST_MSK_TIM14                  ((u32)0x00000001 << 30)          /*!<R/W 0  1: mask HP TIMER14 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM14(x)                   ((u32)(((x) & 0x00000001) << 30))
#define LSYS_GET_SYSRST_MSK_TIM14(x)               ((u32)(((x >> 30) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM13                  ((u32)0x00000001 << 29)          /*!<R/W 0  1: mask HP TIMER13 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM13(x)                   ((u32)(((x) & 0x00000001) << 29))
#define LSYS_GET_SYSRST_MSK_TIM13(x)               ((u32)(((x >> 29) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM12                  ((u32)0x00000001 << 28)          /*!<R/W 0  1: mask HP TIMER12 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM12(x)                   ((u32)(((x) & 0x00000001) << 28))
#define LSYS_GET_SYSRST_MSK_TIM12(x)               ((u32)(((x >> 28) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM11                  ((u32)0x00000001 << 27)          /*!<R/W 0  1: mask HP TIMER11 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM11(x)                   ((u32)(((x) & 0x00000001) << 27))
#define LSYS_GET_SYSRST_MSK_TIM11(x)               ((u32)(((x >> 27) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM10                  ((u32)0x00000001 << 26)          /*!<R/W 0  1: mask HP TIMER10 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM10(x)                   ((u32)(((x) & 0x00000001) << 26))
#define LSYS_GET_SYSRST_MSK_TIM10(x)               ((u32)(((x >> 26) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM_PWM                ((u32)0x00000001 << 25)          /*!<R/W 0  1: mask HP TIMER_PWM reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM_PWM(x)                 ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_SYSRST_MSK_TIM_PWM(x)             ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TIM_PULSE              ((u32)0x00000001 << 24)          /*!<R/W 0  1: mask HP TIMER_PULSE reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM_PULSE(x)               ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_SYSRST_MSK_TIM_PULSE(x)           ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_LEDC                   ((u32)0x00000001 << 23)          /*!<R/W 0  1: mask LEDC reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_LEDC(x)                    ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_SYSRST_MSK_LEDC(x)                ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SPORT1                 ((u32)0x00000001 << 21)          /*!<R/W 0  1: mask SPORT1 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_SPORT1(x)                  ((u32)(((x) & 0x00000001) << 21))
#define LSYS_GET_SYSRST_MSK_SPORT1(x)              ((u32)(((x >> 21) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SPORT0                 ((u32)0x00000001 << 20)          /*!<R/W 0  1: mask SPORT0 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_SPORT0(x)                  ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_SYSRST_MSK_SPORT0(x)              ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_AC                     ((u32)0x00000001 << 19)          /*!<R/W 0  1: mask audio codec active reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_AC(x)                      ((u32)(((x) & 0x00000001) << 19))
#define LSYS_GET_SYSRST_MSK_AC(x)                  ((u32)(((x >> 19) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_TRNG                   ((u32)0x00000001 << 18)          /*!<R/W 0  1: mask TRNG reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TRNG(x)                    ((u32)(((x) & 0x00000001) << 18))
#define LSYS_GET_SYSRST_MSK_TRNG(x)                ((u32)(((x >> 18) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_ECDSA                  ((u32)0x00000001 << 17)          /*!<R/W 0  1: mask ecdsa reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_ECDSA(x)                   ((u32)(((x) & 0x00000001) << 17))
#define LSYS_GET_SYSRST_MSK_ECDSA(x)               ((u32)(((x >> 17) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SPI1                   ((u32)0x00000001 << 15)          /*!<R/W 0  1: mask SPI1 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_SPI1(x)                    ((u32)(((x) & 0x00000001) << 15))
#define LSYS_GET_SYSRST_MSK_SPI1(x)                ((u32)(((x >> 15) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SPI0                   ((u32)0x00000001 << 14)          /*!<R/W 0  1: mask SPI0 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_SPI0(x)                    ((u32)(((x) & 0x00000001) << 14))
#define LSYS_GET_SYSRST_MSK_SPI0(x)                ((u32)(((x >> 14) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SPIC                   ((u32)0x00000001 << 10)          /*!<R/W 0  mask Flash spic reset when sys reset happen mask 1: mask flash 0: disable flash */
#define LSYS_SYSRST_MSK_SPIC(x)                    ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_SYSRST_MSK_SPIC(x)                ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_SHA                    ((u32)0x00000001 << 9)          /*!<R/W 0  1: mask IPSEC SHA reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_SHA(x)                     ((u32)(((x) & 0x00000001) << 9))
#define LSYS_GET_SYSRST_MSK_SHA(x)                 ((u32)(((x >> 9) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_AES                    ((u32)0x00000001 << 8)          /*!<R/W 0  1: mask IPSEC AES reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_AES(x)                     ((u32)(((x) & 0x00000001) << 8))
#define LSYS_GET_SYSRST_MSK_AES(x)                 ((u32)(((x >> 8) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_LX                     ((u32)0x00000001 << 7)          /*!<R/W 0  1: mask LX reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_LX(x)                      ((u32)(((x) & 0x00000001) << 7))
#define LSYS_GET_SYSRST_MSK_LX(x)                  ((u32)(((x >> 7) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_GDMA                   ((u32)0x00000001 << 6)          /*!<R/W 0  1: mask GDMA reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_GDMA(x)                    ((u32)(((x) & 0x00000001) << 6))
#define LSYS_GET_SYSRST_MSK_GDMA(x)                ((u32)(((x >> 6) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_HIFI                   ((u32)0x00000001 << 4)          /*!<R/W 0  1: mask DSP (HIFI5) reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_HIFI(x)                    ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_SYSRST_MSK_HIFI(x)                ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_KM4                    ((u32)0x00000001 << 3)          /*!<R/W 0  1: mask KM4 reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_KM4(x)                     ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_SYSRST_MSK_KM4(x)                 ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_KR4                    ((u32)0x00000001 << 2)          /*!<R/W 0  1: mask KR4 reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_KR4(x)                     ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_SYSRST_MSK_KR4(x)                 ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_PLFM                   ((u32)0x00000001 << 1)          /*!<R/W 0  1: mask platform 0: reset */
#define LSYS_SYSRST_MSK_PLFM(x)                    ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_SYSRST_MSK_PLFM(x)                ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_HSOC                   ((u32)0x00000001 << 0)          /*!<R/W 1  1: mask HP_SOC system reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_HSOC(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_SYSRST_MSK_HSOC(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_SYSRST_MSK1
 * @brief
 * @{
 **/
#define LSYS_BIT_SYSRST_MSK_ZGB                    ((u32)0x00000001 << 27)          /*!<R/W 0  1: mask zgb reset when sys reset happen 0: disable Note: for zgb is in wlan off domain , after turn wlan off , it need an 0->1 transiton */
#define LSYS_SYSRST_MSK_ZGB(x)                     ((u32)(((x) & 0x00000001) << 27))
#define LSYS_GET_SYSRST_MSK_ZGB(x)                 ((u32)(((x >> 27) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_PSRAM                  ((u32)0x00000001 << 26)          /*!<R/W 0  1: mask PSRAM reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_PSRAM(x)                   ((u32)(((x) & 0x00000001) << 26))
#define LSYS_GET_SYSRST_MSK_PSRAM(x)               ((u32)(((x >> 26) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_THM                    ((u32)0x00000001 << 25)          /*!<R/W 0  mask thml reset when sys reset happen mask */
#define LSYS_SYSRST_MSK_THM(x)                     ((u32)(((x) & 0x00000001) << 25))
#define LSYS_GET_SYSRST_MSK_THM(x)                 ((u32)(((x >> 25) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_CTC                    ((u32)0x00000001 << 24)          /*!<R/W 0  1: mask Captouch ADC reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_CTC(x)                     ((u32)(((x) & 0x00000001) << 24))
#define LSYS_GET_SYSRST_MSK_CTC(x)                 ((u32)(((x >> 24) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_ADC                    ((u32)0x00000001 << 23)          /*!<R/W 0  1: mask LS ADC reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_ADC(x)                     ((u32)(((x) & 0x00000001) << 23))
#define LSYS_GET_SYSRST_MSK_ADC(x)                 ((u32)(((x >> 23) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_IPC                    ((u32)0x00000001 << 22)          /*!<R/W 0  1: mask IPC reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_IPC(x)                     ((u32)(((x) & 0x00000001) << 22))
#define LSYS_GET_SYSRST_MSK_IPC(x)                 ((u32)(((x >> 22) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_WDG                    ((u32)0x00000001 << 21)          /*!<R/W 0  1: mask LS WDG 1~4 reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_WDG(x)                     ((u32)(((x) & 0x00000001) << 21))
#define LSYS_GET_SYSRST_MSK_WDG(x)                 ((u32)(((x >> 21) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_DTIM                   ((u32)0x00000001 << 20)          /*!<R/W 0  1: mask LP dtim reset when sys reset happen mask 0: reset */
#define LSYS_SYSRST_MSK_DTIM(x)                    ((u32)(((x) & 0x00000001) << 20))
#define LSYS_GET_SYSRST_MSK_DTIM(x)                ((u32)(((x >> 20) & 0x00000001)))
#define LSYS_MASK_SYSRST_MSK_TIM_BASIC             ((u32)0x000000FF << 12)          /*!<R/W 0  1: mask LP basic TIMER0-7 reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_TIM_BASIC(x)               ((u32)(((x) & 0x000000FF) << 12))
#define LSYS_GET_SYSRST_MSK_TIM_BASIC(x)           ((u32)(((x >> 12) & 0x000000FF)))
#define LSYS_BIT_SYSRST_MSK_I2C1                   ((u32)0x00000001 << 11)          /*!<R/W 0  1: mask HS I2C1 reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_I2C1(x)                    ((u32)(((x) & 0x00000001) << 11))
#define LSYS_GET_SYSRST_MSK_I2C1(x)                ((u32)(((x >> 11) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_I2C0                   ((u32)0x00000001 << 10)          /*!<R/W 0  1: mask LS I2C0 reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_I2C0(x)                    ((u32)(((x) & 0x00000001) << 10))
#define LSYS_GET_SYSRST_MSK_I2C0(x)                ((u32)(((x >> 10) & 0x00000001)))
#define LSYS_MASK_SYSRST_MSK_UART                  ((u32)0x0000000F << 6)          /*!<R/W 0  1: mask UART reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_UART(x)                    ((u32)(((x) & 0x0000000F) << 6))
#define LSYS_GET_SYSRST_MSK_UART(x)                ((u32)(((x >> 6) & 0x0000000F)))
#define LSYS_BIT_SYSRST_MSK_LOGUART                ((u32)0x00000001 << 5)          /*!<R/W 0  1: mask LOGUART reset when sys reset happen 0: disable */
#define LSYS_SYSRST_MSK_LOGUART(x)                 ((u32)(((x) & 0x00000001) << 5))
#define LSYS_GET_SYSRST_MSK_LOGUART(x)             ((u32)(((x >> 5) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_GPIO                   ((u32)0x00000001 << 4)          /*!<R/W 1  1: mask LS GPIO reset when sys reset happen 0: reset */
#define LSYS_SYSRST_MSK_GPIO(x)                    ((u32)(((x) & 0x00000001) << 4))
#define LSYS_GET_SYSRST_MSK_GPIO(x)                ((u32)(((x >> 4) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_AIP                    ((u32)0x00000001 << 3)          /*!<R/W 1  1: mask aip reset when sys reset happen mask 0: reset */
#define LSYS_SYSRST_MSK_AIP(x)                     ((u32)(((x) & 0x00000001) << 3))
#define LSYS_GET_SYSRST_MSK_AIP(x)                 ((u32)(((x >> 3) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_WLAFE_CTRL             ((u32)0x00000001 << 2)          /*!<R/W 0  1: mask WL RFAFE control circuit, shall be 1 when mask WL or BT use WL RFAFE 0: reset */
#define LSYS_SYSRST_MSK_WLAFE_CTRL(x)              ((u32)(((x) & 0x00000001) << 2))
#define LSYS_GET_SYSRST_MSK_WLAFE_CTRL(x)          ((u32)(((x >> 2) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_BTON                   ((u32)0x00000001 << 1)          /*!<R/W 0  mask BTON reset when sys reset happen 1: mask 0: disable */
#define LSYS_SYSRST_MSK_BTON(x)                    ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_SYSRST_MSK_BTON(x)                ((u32)(((x >> 1) & 0x00000001)))
#define LSYS_BIT_SYSRST_MSK_WLON                   ((u32)0x00000001 << 0)          /*!<R/W 0  mask WLON reset when sys reset happen mask 1: mask 0: disable */
#define LSYS_SYSRST_MSK_WLON(x)                    ((u32)(((x) & 0x00000001) << 0))
#define LSYS_GET_SYSRST_MSK_WLON(x)                ((u32)(((x >> 0) & 0x00000001)))
/** @} */

/** @defgroup REG_LSYS_DUMMY_AIP_CTRL1
 * @brief
 * @{
 **/
#define LSYS_MASK_AIP_CTRL_RSVD1                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'h0000ffff  reserved for aip control */
#define LSYS_AIP_CTRL_RSVD1(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_AIP_CTRL_RSVD1(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_LSYS_DUMMY_AIP_INFO1
 * @brief
 * @{
 **/
#define LSYS_MASK_AIP_INFO_RSVD1                   ((u32)0xFFFFFFFF << 0)          /*!<R 0  reserved for aip info indication */
#define LSYS_AIP_INFO_RSVD1(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define LSYS_GET_AIP_INFO_RSVD1(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========LSYS Register Address Definition==========*/
#define REG_LSYS_PWC                                 0x0200
#define REG_LSYS_ISO                                 0x0202
#define REG_LSYS_FEN_GRP0                            0x0204
#define REG_LSYS_FEN_GRP1                            0x0208
#define REG_LSYS_CKE_GRP0                            0x020C
#define REG_LSYS_CKE_GRP1                            0x0210
#define REG_LSYS_CKE_GRP2                            0x0214
#define REG_LSYS_CKSL_GRP0                           0x0218
#define REG_LSYS_CKSL_GRP1                           0x021C
#define REG_LSYS_CKD_GRP0                            0x0220
#define REG_LSYS_CKD_GRP1                            0x0224
#define REG_LSYS_SYSTEM_CFG0                         0x0228
#define REG_LSYS_SYSTEM_CFG1                         0x022C
#define REG_LSYS_PLAT_CTRL                           0x0230
#define REG_LSYS_PLAT_STATUS                         0x0234
#define REG_LSYS_PLAT_INFO                           0x0238
#define REG_LSYS_KM4_XO_CTRL0                        0x0240
#define REG_LSYS_KM4_XO_CTRL1                        0x0244
#define REG_LSYS_HIFI_CTRL0                          0x0248
#define REG_LSYS_HIFI_CTRL1                          0x024C
#define REG_LSYS_HIFI_STATUS                         0x0250
#define REG_LSYS_HIFI_DBG_PC                         0x0254
#define REG_LSYS_HIFI_DBG_INST                       0x0258
#define REG_LSYS_SPIC_CTRL                           0x025C
#define REG_LSYS_SW_RST_CTRL                         0x0260
#define REG_LSYS_SW_RST_TRIG                         0x0264
#define REG_LSYS_BOOT_REASON_SW                      0x0268
#define REG_LSYS_BOOT_CFG                            0x026C
#define REG_LSYS_SW_PATCH                            0x0270
#define REG_LSYS_CHIP_INFO                           0x0274
#define REG_LSYS_BT_CTRL0                            0x0280
#define REG_LSYS_BT_CTRL1                            0x0284
#define REG_LSYS_AIP_CTRL0                           0x0288
#define REG_LSYS_AIP_CTRL1                           0x028C
#define REG_LSYS_DW_GPIO_CTRL                        0x02A4
#define REG_LSYS_LXBUS_DBG0                          0x02A8
#define REG_LSYS_LXBUS_DBG1                          0x02AC
#define REG_LSYS_LXBUS_DBG_CTRL                      0x02B0
#define REG_LSYS_FPGA_CLK_INFO                       0x02B4
#define REG_LSYS_FLASH_PARA_ADDR                     0x02B8
#define REG_LSYS_SCAN_CTRL                           0x02BC
#define REG_LSYS_WDG_GLBRST                          0x02C0
#define REG_LSYS_PRS_CTRL                            0x02C4
#define REG_LSYS_DUMMY_089                           0x02CC
#define REG_LSYS_SYSRST_MSK0                         0x02D0
#define REG_LSYS_SYSRST_MSK1                         0x02D4
#define REG_LSYS_BOUNDARY_CHK                        0x02F0
#define REG_LSYS_DUMMY_090                           0x02F4
#define REG_LSYS_DUMMY_AIP_CTRL1                     0x02F8
#define REG_LSYS_DUMMY_AIP_INFO1                     0x02FC

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */
//Please add your defination here

enum CHIP_TYPE {
	CHIP_TYPE_ASIC = 0,
	CHIP_TYPE_FPGA = 1,
	CHIP_TYPE_PALADIUM = 2,
	CHIP_TYPE_RTLSIM = 3
};

enum CORE_Vol_TYPE {
	CORE_VOL_0P9 = 0,
	CORE_VOL_1P0 = 1,
};

#define BIT_FORCE_USE_S0_ICFG			LSYS_PTRP_ICFG(0x0000000B)
#define BIT_MCM_FLASH_PROBE_ICFG		LSYS_PTRP_ICFG(0x0000000A)
#define BIT_MCM_FLASH_PG1B_ICFG		LSYS_PTRP_ICFG(0x00000009)

/*******************  Macro definition for Port M0 Master ID ********************/
#define SYSM0_MASTER_SIC                                       0
#define SYSM0_MASTER_OTPC                                    1
#define SYSM0_MASTER_KM0                                      2

/*****************************Macro definition for OTP SYSCFG1**************/
#define LSYS_BIT_DIS_BOOT_DELAY       ((u32)0x00000001 << 0)

/*****************************Macro definition for HIFI CTRL0**************/
#define LSYS_BIT_DIS_MEM_SHARE       ((u32)0x00000000 << 0)
#define LSYS_BIT_SHARE_128K_MEM       ((u32)0x00000001 << 0)
#define LSYS_BIT_SHARE_256K_MEM       ((u32)0x00000002 << 0)


/**************************************************************************//**
 * @defgroup AMEBA_RRAM
 * @{
 * @brief AMEBA_RRAM Declaration
 * @ Total size of retention memory is 512Byte,Half retention mem resvd for system
 * @ the Max space for RRAM_TypeDef is 0x80, user can alloc space from RRAM_USER_RSVD
 *****************************************************************************/
typedef struct {
	__IO u8 RRAM_SYS_RSVD[256];/* resvd for system */

	__IO u32 IPC_Share_Mem; /* RETENTION_RAM_SYS_OFFSET 0x80 */
	__IO u32 PLLD_Clk_Info;

	__IO u32 IMQ_HEAP_ADDR[4];
	__IO u32 IMQ_HEAP_SIZE[4];
	__IO u32 IMQ_INIT_DONE;

	__IO u32 AP_WAKEUP_STATUS;
	__IO u32 PSRAM_CKSL_BK;

	__IO u8 PSRAM_TYPE;
	__IO u8 PSRAM_RESUMECNT_BOOT;
	__IO u8 CORE_Vol_Flag;

	__IO u8 SWRCALIB_AON_LDO_09;
	__IO u8 SWRCALIB_PWM_125; //store mode1 PWM125 autoload by HW, used by Reduce Voltage
	__IO u8 SWRCALIB_PWM_135; //store mode1 PWM135 prefetched from efuse, used by Raise Voltage
	__IO u8 SWRCALIB_CORE_LDO_09; //store CORE_LDO_09 autoload by HW, used by Reduce Voltage in mode1
	__IO u8 SWRCALIB_CORE_LDO_10; //store CORE_LDO_10 prefetched from efuse, used by Raise Voltage in mode1
	__IO u8 SWRCALIB_PWM_09; //store mode0 PWM09 autoload by HW, used by Reduce Voltage
	__IO u8 SWRCALIB_PWM_10; //store mode0 PWM10 prefetched from efuse, used by Raise Voltage

	__IO u8 PMC_CORE_ROLE_Flag;
	__IO u8 kr4_BackupTbl_Done;

	__IO u8 RRAM_USER_RSVD[193];
} RRAM_TypeDef;
/** @} */

/* MANUAL_GEN_END */

#endif


/** @} */

/** @} */
