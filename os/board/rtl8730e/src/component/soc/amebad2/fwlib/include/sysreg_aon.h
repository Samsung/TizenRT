#ifndef _AMEBAD2_AON_H_
#define _AMEBAD2_AON_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_AON_PWC
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_LPPAD_RSTB                 ((u32)0x00000001 << 29)          /*!<R/W 0  1: Release LP PAD RSTB33 (except GPIOF) 0: Global Assert LP PAD RSTB33 */
#define AON_BIT_LPPAD_SHDN                 ((u32)0x00000001 << 28)          /*!<R/W 0  1: Global Enable LP PAD (except GPIOF) 0: Shutdown LP PAD Note: Default value must be fixed after AON PMC done. */
#define AON_BIT_GPIOA_RSTB                 ((u32)0x00000001 << 23)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOB_RSTB                 ((u32)0x00000001 << 22)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOC_RSTB                 ((u32)0x00000001 << 21)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD This is for audio pad, may not use */
#define AON_BIT_GPIOD_RSTB                 ((u32)0x00000001 << 20)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOE_RSTB                 ((u32)0x00000001 << 19)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOF_RSTB                 ((u32)0x00000001 << 18)          /*!<R/W 0  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOG_RSTB                 ((u32)0x00000001 << 17)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOH_RSTB                 ((u32)0x00000001 << 16)          /*!<R/W 1  1: Enable GPIOA group pad 0: Reset PAD */
#define AON_BIT_GPIOA_SHDN                 ((u32)0x00000001 << 15)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOB_SHDN                 ((u32)0x00000001 << 14)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOC_SHDN                 ((u32)0x00000001 << 13)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD This is for audio pad, may not use */
#define AON_BIT_GPIOD_SHDN                 ((u32)0x00000001 << 12)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOE_SHDN                 ((u32)0x00000001 << 11)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOF_SHDN                 ((u32)0x00000001 << 10)          /*!<R/W 0  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOG_SHDN                 ((u32)0x00000001 << 9)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_BIT_GPIOH_SHDN                 ((u32)0x00000001 << 8)          /*!<R/W 1  1: Enable GPIOA group pad 0: Shutdown PAD */
#define AON_MASK_PC_STEP                   ((u32)0x00000003 << 4)          /*!<R/W 2'd2  power cut enable time step. 0: not valid 1: 10us 2: 20us 3: 30us */
#define AON_PC_STEP(x)                     ((u32)(((x) & 0x00000003) << 4))
#define AON_GET_PC_STEP(x)                 ((u32)(((x >> 4) & 0x00000003)))
#define AON_BIT_PSW_AON_RET_OUT            ((u32)0x00000001 << 3)          /*!<R 0  Aon off domain power switch state ,active 1. 1:it means power enable OK */
#define AON_BIT_PSW_AON_RET                ((u32)0x00000001 << 2)          /*!<R/W 0  Aon off domain power switch enable ,active 1. 1:enable 0:disable */
#define AON_BIT_PWC_AON_OTP                ((u32)0x00000001 << 0)          /*!<R/W 0  OTP core power cut enable,active 1. for enable, first enable this bit, then enable bit1 for disable, first disable bit 1, then disable this bit 1:enable 0:disable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_ISO
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_ISO_AON_RET                ((u32)0x00000001 << 2)          /*!<R/W 1  1: ISO signals from aon off power domain 0: Release */
#define AON_BIT_ISO_AON_OTP                ((u32)0x00000001 << 1)          /*!<R/W 1  1: ISO signals from otp core power domain 0: Release */
#define AON_BIT_ISO_LP_SYSON_RAM           ((u32)0x00000001 << 0)          /*!<R/W 1  When SWR core power on, PD_LP_SYSON_RAM is on. 1: ISO signals from LP SYSON_RAM power domain 0: Release */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_ROR
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_POR_AON_RET                ((u32)0x00000001 << 2)          /*!<R/W 0  0: reset aon off power domain 1: Release */
#define AON_BIT_POR_AON_OTP                ((u32)0x00000001 << 1)          /*!<R/W 0  0: reset aon otp power domain 1: Release */
#define AON_BIT_POR_SYSON                  ((u32)0x00000001 << 0)          /*!<R/W 0  0: reset syson power domain 1: Release */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_FEN
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_SDM32K                   ((u32)(3  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: Enable 32.768K SDM function 0: Disable */
#define APBPeriph_OTP                      ((u32)(3  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: Enable OTP function 0: Disable */
#define APBPeriph_RTC                      ((u32)(3  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: Enable RTC function 0: Disable */
#define APBPeriph_PX                       ((u32)(3  << 30) | (0x00000001 << 17))  /*!<R/W 1  1: Enable pinmux and pad setting 0: Reset */
#define APBPeriph_LPON                     ((u32)(3  << 30) | (0x00000001 << 16))  /*!<R/W 0  LS global reset 1: Enable LS function 0: Disable */
#define APBPeriph_GDET_VD09_POS            ((u32)(3  << 30) | (0x00000001 << 15))  /*!<R/W 0  1: Enable glitch detect for swr0p9 posedge 0: disable */
#define APBPeriph_GDET_VD09_NEG            ((u32)(3  << 30) | (0x00000001 << 14))  /*!<R/W 0  1: Enable glitch detect for swr0p9 negedge 0: disable */
#define APBPeriph_GDET1_VD18_POS           ((u32)(3  << 30) | (0x00000001 << 13))  /*!<R/W 0  1: Enable glitch detect for swr psram posedge 0: Reset AON timer */
#define APBPeriph_GDET1_VD18_NEG           ((u32)(3  << 30) | (0x00000001 << 12))  /*!<R/W 0  1: Enable glitch detect for swr psram negedge 0: disable */
#define APBPeriph_GDET2_VD18_POS           ((u32)(3  << 30) | (0x00000001 << 11))  /*!<R/W 0  1: Enable glitch detect for swr mem posedge 0: disable */
#define APBPeriph_GDET2_VD18_NEG           ((u32)(3  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable glitch detect for swr mem negedge 0: disable */
#define APBPeriph_GDET_VD33_POS            ((u32)(3  << 30) | (0x00000001 << 9))   /*!<R/W 0  1: Enable glitch detect for vd33 posedge 0: disable */
#define APBPeriph_GDET_VD33_NEG            ((u32)(3  << 30) | (0x00000001 << 8))   /*!<R/W 0  1: Enable glitch detect for vd33 negedge 0: disable */
#define APBPeriph_ATIM                     ((u32)(3  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable AON timer 0: Reset AON timer Note: AON timer starts to run only when event happens. */
#define APBPeriph_IWDG                     ((u32)(3  << 30) | (0x00000001 << 2))   /*!<R/WE 0  1: Enable IWDG by HW autoload. Once enabled, IWDG can't be disabled unless por asserts. */
#define APBPeriph_BOR_ACC                  ((u32)(3  << 30) | (0x00000001 << 1))   /*!<R/W 0  Enable high acc bor reset function, default is 0 when power on */
#define APBPeriph_BOR                      ((u32)(3  << 30) | (0x00000001 << 0))   /*!<R/W 0  Enable bor reset function, default is 0 when power on */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CLK
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_OSC4M_FORCE0_CLOCK       ((u32)(3  << 30) | (0x00000001 << 24))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE1_CLOCK       ((u32)(3  << 30) | (0x00000001 << 25))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE2_CLOCK       ((u32)(3  << 30) | (0x00000001 << 26))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE3_CLOCK       ((u32)(3  << 30) | (0x00000001 << 27))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE4_CLOCK       ((u32)(3  << 30) | (0x00000001 << 28))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE5_CLOCK       ((u32)(3  << 30) | (0x00000001 << 29))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE6_CLOCK       ((u32)(3  << 30) | (0x00000001 << 30))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_OSC4M_FORCE7_CLOCK       ((u32)(3  << 30) | (0x00000001 << 31))  /*!<R/W 0  [i] = 1 : Force osc4m enable for certain path . [i] = 0 : osc4m enable for certain path is determined by normal control [24]:otpc [25]:aip xtal [26]:peri [27]:platform [28]:VAD [29]:wlan,bt Note, this field is for SW control,[30-31]is retain for SW */
#define APBPeriph_SDM32K_CLOCK             ((u32)(3  << 30) | (0x00000001 << 10))  /*!<R/W 0  1: Enable 32.768K SDM module's clock 0: Disable */
#define APBPeriph_LP4M_CLOCK               ((u32)(3  << 30) | (0x00000001 << 8))   /*!<R/W/ES 0  1: Enable lp syson plfm 4m clock 0: Disable Note: this bit will set/clr by HW pmc */
#define APBPeriph_RTC_CLOCK                ((u32)(3  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable RTC clock 0: Disable */
#define APBPeriph_ATIM_CLOCK               ((u32)(3  << 30) | (0x00000001 << 1))   /*!<R/W 1  1: Enable AON timer clock 0: Disable */
#define APBPeriph_AON_CLOCK                ((u32)(3  << 30) | (0x00000001 << 0))   /*!<R/W 1  if 1, AON clock will be gated except PMC and this register. Note: AON clock can't be shutdown by analog need. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CKSL
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_CKSL_32K                  ((u32)0x00000003 << 4)          /*!<R/WE 0h  This field is used to select 32K clock. 00/01: SDM32K 10: Crystal 32K 11: External 32K */
#define AON_CKSL_32K(x)                    ((u32)(((x) & 0x00000003) << 4))
#define AON_GET_CKSL_32K(x)                ((u32)(((x >> 4) & 0x00000003)))
#define AON_BIT_CKSL_OTP                   ((u32)0x00000001 << 0)          /*!<R/W 0h  This field is used to select OTP clock. 1'b0: AON clock 1'b1: lbus clock (max. 26MHz) */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_BOOT_REASON_HW
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_RSTF_GDET_VD33_POS         ((u32)0x00000001 << 27)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect vd33 pos happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET_VD33_NEG         ((u32)0x00000001 << 26)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect vd33 neg happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET2_VD18_POS        ((u32)0x00000001 << 25)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr mem pos happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET2_VD18_NEG        ((u32)0x00000001 << 24)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr mem neg happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET1_VD18_POS        ((u32)0x00000001 << 23)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr psram pos happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET1_VD18_NEG        ((u32)0x00000001 << 22)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr psram neg happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET_VD09_POS         ((u32)0x00000001 << 21)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr core pos happen. Write 1 to clear. */
#define AON_BIT_RSTF_GDET_VD09_NEG         ((u32)0x00000001 << 20)          /*!<RW1CB 0h  This bit will be set to 1 when glitch detect swr core neg happen. Write 1 to clear. */
#define AON_BIT_RSTF_AP_WARM2PERI          ((u32)0x00000001 << 18)          /*!<RW1CB 0h  This bit will be set to 1 when ap warm resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_KM4_WARM2PERI         ((u32)0x00000001 << 17)          /*!<RW1CB 0h  This bit will be set to 1 when km4 warm resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_KM0_WARM2PERI         ((u32)0x00000001 << 16)          /*!<RW1CB 0h  This bit will be set to 1 when km0 warm resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_WDG4                  ((u32)0x00000001 << 15)          /*!<RW1CB 0h  This bit will be set to 1 when WDG resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_WDG3                  ((u32)0x00000001 << 14)          /*!<RW1CB 0h  This bit will be set to 1 when WDG resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_WDG2                  ((u32)0x00000001 << 13)          /*!<RW1CB 0h  This bit will be set to 1 when WDG resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_WDG1                  ((u32)0x00000001 << 12)          /*!<RW1CB 0h  This bit will be set to 1 when WDG resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_IWDG                  ((u32)0x00000001 << 11)          /*!<RW1CB 0h  This bit will be set to 1 when WDG resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_APSYS                 ((u32)0x00000001 << 10)          /*!<RW1C 0h  This bit will be set to 1 when CA7 resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_NPSYS                 ((u32)0x00000001 << 9)          /*!<RW1C 0h  This bit will be set to 1 when KM4 resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_LPSYS                 ((u32)0x00000001 << 8)          /*!<RW1C 0h  This bit will be set to 1 when KM0 resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_DSLP                  ((u32)0x00000001 << 7)          /*!<RW1C 0h  This is boot reason for dslp exit, clear by SW write to 1. */
#define AON_BIT_RSTF_BOR_ACC               ((u32)0x00000001 << 5)          /*!<RW1C 0h  This bit will be set to 1 when BOR ACC resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_BOR                   ((u32)0x00000001 << 4)          /*!<RW1C 0h  This bit will be set to 1 when BOR resets the system. Write 1 to clear. */
#define AON_BIT_RSTF_THM                   ((u32)0x00000001 << 3)          /*!<RW1C 0h  This bit will be set to 1 when THERMAL resets the system. Write 1 to clear. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_AON_WAK_EVT
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_GDET_VD33_POS_ISR_EVT      ((u32)0x00000001 << 27)          /*!<R 0h  glitch detect vd33 pos event happen. */
#define AON_BIT_GDET_VD33_NEG_ISR_EVT      ((u32)0x00000001 << 26)          /*!<R 0h  glitch detect vd33 neg event happen. */
#define AON_BIT_GDET2_VD18_POS_ISR_EVT     ((u32)0x00000001 << 25)          /*!<R 0h  glitch detect swr mem pos event happen. */
#define AON_BIT_GDET2_VD18_NEG_ISR_EVT     ((u32)0x00000001 << 24)          /*!<R 0h  glitch detect swr mem neg event happen. */
#define AON_BIT_GDET1_VD18_POS_ISR_EVT     ((u32)0x00000001 << 23)          /*!<R 0h  glitch detect swr psram pos event happen. */
#define AON_BIT_GDET1_VD18_NEG_ISR_EVT     ((u32)0x00000001 << 22)          /*!<R 0h  glitch detect swr psram neg event happen. */
#define AON_BIT_GDET_VD09_POS_ISR_EVT      ((u32)0x00000001 << 21)          /*!<R 0h  glitch detect swr core pos event happen. */
#define AON_BIT_GDET_VD09_NEG_ISR_EVT      ((u32)0x00000001 << 20)          /*!<R 0h  glitch detect swr core neg event happen. */
#define AON_BIT_BOR_ACC_ISR_EVT            ((u32)0x00000001 << 9)          /*!<R 0  bor acc interrupt event occurs. */
#define AON_BIT_RTC_ISR_EVT                ((u32)0x00000001 << 8)          /*!<R 0  RTC interrupt wake event occurs. */
#define AON_BIT_GPIO_PIN3_WAKDET_EVT       ((u32)0x00000001 << 7)          /*!<R 0h  Wake event for wake pin3 1: wake event detected 0: no wake event Note: aon gpio only used to wake hw pmc, to interrupt SW, it must enable GPIO interrupt in LP GPIO IP, and the poly must be same as AON GPIO setting. */
#define AON_BIT_GPIO_PIN2_WAKDET_EVT       ((u32)0x00000001 << 6)          /*!<R 0h  Wake event for wake pin2 1: wake event detected 0: no wake event Note: aon gpio only used to wake hw pmc, to interrupt SW, it must enable GPIO interrupt in LP GPIO IP, and the poly must be same as AON GPIO setting. */
#define AON_BIT_GPIO_PIN1_WAKDET_EVT       ((u32)0x00000001 << 5)          /*!<R 0h  Wake event for wake pin1 1: wake event detected 0: no wake event Note: aon gpio only used to wake hw pmc, to interrupt SW, it must enable GPIO interrupt in LP GPIO IP, and the poly must be same as AON GPIO setting. */
#define AON_BIT_GPIO_PIN0_WAKDET_EVT       ((u32)0x00000001 << 4)          /*!<R 0h  Wake event for wake pin0 1: wake event detected 0: no wake event Note: aon gpio only used to wake hw pmc, to interrupt SW, it must enable GPIO interrupt in LP GPIO IP, and the poly must be same as AON GPIO setting. */
#define AON_BIT_TIM_ISR_EVT                ((u32)0x00000001 << 3)          /*!<R 0  AON timer interrupt event occurs. */
#define AON_BIT_BOR_ISR_EVT                ((u32)0x00000001 << 2)          /*!<R 0  bor interrupt event occurs. */
#define AON_BIT_CHIPEN_LP_ISR_EVT          ((u32)0x00000001 << 1)          /*!<R 0  Chipen long press interrupt event occurs. */
#define AON_BIT_CHIPEN_SP_ISR_EVT          ((u32)0x00000001 << 0)          /*!<R 0  Chipen short press interrupt event occurs. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_TIM_CNT
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_TIM_CNT_CUR               ((u32)0xFFFFFFFF << 0)          /*!<R 0h  Aon timer current count value. */
#define AON_TIM_CNT_CUR(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define AON_GET_TIM_CNT_CUR(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_TIM_SET
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_TIM_CNT_SET               ((u32)0xFFFFFFFF << 0)          /*!<R/WPD 0h  Set aon time a countdown value, when timer count down, it will trigger a aon_tim event. */
#define AON_TIM_CNT_SET(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define AON_GET_TIM_CNT_SET(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_TIM_IER
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_TIM_IE                     ((u32)0x00000001 << 0)          /*!<R/W 0h  Timer interrupt enable 0:disable 1:enable */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_TIM_ISR
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_TIM_ISR                    ((u32)0x00000001 << 0)          /*!<RW1C 0h  Timer interrupt status 0:no interrupt 1:interrupt occurs */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_GPIO_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_GPIO_WAKDET_POLY          ((u32)0x0000000F << 24)          /*!<R/W 0h  Wake pin polarity 0: low active 1: high active bit3: wake pin3 polarity bit2: wake pin2 polarity bit1: wake pin1 polarity bit0: wake pin0 polarity Note: once a wake pin is set, the pads PU/PD will be force by this poly value, and soft config have no use. */
#define AON_GPIO_WAKDET_POLY(x)            ((u32)(((x) & 0x0000000F) << 24))
#define AON_GET_GPIO_WAKDET_POLY(x)        ((u32)(((x >> 24) & 0x0000000F)))
#define AON_MASK_GPIO_DBNC_CYC             ((u32)0x00007FFF << 8)          /*!<R/W 100h  The GPIO signal will be filtered by the number of debounce cycles of specified in this register. (100k/128k) */
#define AON_GPIO_DBNC_CYC(x)               ((u32)(((x) & 0x00007FFF) << 8))
#define AON_GET_GPIO_DBNC_CYC(x)           ((u32)(((x >> 8) & 0x00007FFF)))
#define AON_BIT_GPIO_DBNC_FEN              ((u32)0x00000001 << 7)          /*!<R/W 0  1: Enable gpio debouncer function 0: Disable Default disable for simualtion issue */
#define AON_MASK_GPIO_WAKE_FEN             ((u32)0x0000000F << 0)          /*!<R/W 0  1: Enable wake pin function 0: Disable Every GPIO has its own enable. When aon pmc is not in DSLP, wake pins will not effect PMC state althrougth this bit is set. */
#define AON_GPIO_WAKE_FEN(x)               ((u32)(((x) & 0x0000000F) << 0))
#define AON_GET_GPIO_WAKE_FEN(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_GPIO_WAKE_ISR
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_GPIO_PIN3_WAKE_ISR         ((u32)0x00000001 << 3)          /*!<RW1CB 0  Wake pin3 interrupt status 0:no interrupt 1:interrupt occurs */
#define AON_BIT_GPIO_PIN2_WAKE_ISR         ((u32)0x00000001 << 2)          /*!<RW1CB 0  Wake pin2 interrupt status 0:no interrupt 1:interrupt occurs */
#define AON_BIT_GPIO_PIN1_WAKE_ISR         ((u32)0x00000001 << 1)          /*!<RW1CB 0  Wake pin1 interrupt status 0:no interrupt 1:interrupt occurs */
#define AON_BIT_GPIO_PIN0_WAKE_ISR         ((u32)0x00000001 << 0)          /*!<RW1CB 0  Wake pin0 interrupt status 0:no interrupt 1:interrupt occurs */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CHIPEN_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_CHIPEN_ST                  ((u32)0x00000001 << 18)          /*!<R 0  CHIP_EN pin status 0: logic '0' on CHIP_EN pin 1: logic '1' on CHIP_EN pin */
#define AON_MASK_CHIPEN_INTR_MODE          ((u32)0x00000003 << 16)          /*!<R/W/ES 0  Chipen operation mode. 00: reset mode. When chipen pad low, hw will reset all chip 01: interrup mode. It will enable short/long press function. 1X: pulse reset mode. The transation from other value to 2'd2 will gen a posedge to set RST_PULSE_EN in HV domain. Once in pulse reset mode , this field can't be modified again , and always read out 2'b11 */
#define AON_CHIPEN_INTR_MODE(x)            ((u32)(((x) & 0x00000003) << 16))
#define AON_GET_CHIPEN_INTR_MODE(x)        ((u32)(((x >> 16) & 0x00000003)))
#define AON_MASK_CHIPEN_TACK               ((u32)0x00000003 << 12)          /*!<R/W 0  00: 50ms 01: 100ms 10: 200ms 11: 400ms */
#define AON_CHIPEN_TACK(x)                 ((u32)(((x) & 0x00000003) << 12))
#define AON_GET_CHIPEN_TACK(x)             ((u32)(((x >> 12) & 0x00000003)))
#define AON_MASK_CHIPEN_TLP                ((u32)0x00000007 << 8)          /*!<R/W 0  000: 1000ms 001: 1500ms 010: 2000ms 011: 2500ms 100: 3000ms 101: 3500ms 110: 4000ms 111: 4500ms */
#define AON_CHIPEN_TLP(x)                  ((u32)(((x) & 0x00000007) << 8))
#define AON_GET_CHIPEN_TLP(x)              ((u32)(((x >> 8) & 0x00000007)))
#define AON_MASK_CHIPEN_TSP                ((u32)0x00000007 << 4)          /*!<R/W 0  000: 0ms 001: 50ms 010: 100ms 011: 150ms 100: 200ms 101: 250ms 110: 300ms 111: 350ms */
#define AON_CHIPEN_TSP(x)                  ((u32)(((x) & 0x00000007) << 4))
#define AON_GET_CHIPEN_TSP(x)              ((u32)(((x >> 4) & 0x00000007)))
#define AON_MASK_CHIPEN_TDBC               ((u32)0x00000007 << 0)          /*!<R/W 3'd1  000: 0us 001: 100us 010: 500us 011: 1000us 100: 2000us 101: 4000us 110: 8000us 111: 16000us */
#define AON_CHIPEN_TDBC(x)                 ((u32)(((x) & 0x00000007) << 0))
#define AON_GET_CHIPEN_TDBC(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CHIPEN_ISR
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_CHIPEN_LP_ISR              ((u32)0x00000001 << 1)          /*!<RW1C 0  chipen long press interrupt status 0:no interrupt 1:interrupt occurs */
#define AON_BIT_CHIPEN_SP_ISR              ((u32)0x00000001 << 0)          /*!<RW1C 0  chipen short press interrupt status 0:no interrupt 1:interrupt occurs */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_BOR_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_BOR_ACC_INTR_MODE          ((u32)0x00000001 << 17)          /*!<R/W 0  High accuracy BOR operation mode. 0: reset mode. When analog detect bor, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_BIT_BOR_INTR_MODE              ((u32)0x00000001 << 16)          /*!<R/W 0  BOR operation mode. 0: reset mode. When analog detect bor, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_MASK_BOR_ACC_TDBC              ((u32)0x00000007 << 4)          /*!<R/W 3'd1  000: 0us 001: 100us 010: 500us 011: 1000us 100: 2000us 101: 4000us 110: 8000us 111: 16000us */
#define AON_BOR_ACC_TDBC(x)                ((u32)(((x) & 0x00000007) << 4))
#define AON_GET_BOR_ACC_TDBC(x)            ((u32)(((x >> 4) & 0x00000007)))
#define AON_MASK_BOR_TDBC                  ((u32)0x00000007 << 0)          /*!<R/W 3'd1  000: 0us 001: 100us 010: 500us 011: 1000us 100: 2000us 101: 4000us 110: 8000us 111: 16000us */
#define AON_BOR_TDBC(x)                    ((u32)(((x) & 0x00000007) << 0))
#define AON_GET_BOR_TDBC(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_BOR_ISR
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_GDET_VD33_POS_ISR          ((u32)0x00000001 << 11)          /*!<RW1C 0h  glitch detect vd33 pos interrupt statusen. */
#define AON_BIT_GDET_VD33_NEG_ISR          ((u32)0x00000001 << 10)          /*!<RW1C 0h  glitch detect vd33 neg interrupt statusen. */
#define AON_BIT_GDET2_VD18_POS_ISR         ((u32)0x00000001 << 9)          /*!<RW1C 0h  glitch detect swr mem pos interrupt statusen. */
#define AON_BIT_GDET2_VD18_NEG_ISR         ((u32)0x00000001 << 8)          /*!<RW1C 0h  glitch detect swr mem neg interrupt statusen. */
#define AON_BIT_GDET1_VD18_POS_ISR         ((u32)0x00000001 << 7)          /*!<RW1C 0h  glitch detect swr psram pos interrupt statusen. */
#define AON_BIT_GDET1_VD18_NEG_ISR         ((u32)0x00000001 << 6)          /*!<RW1C 0h  glitch detect swr psram neg interrupt statusen. */
#define AON_BIT_GDET_VD09_POS_ISR          ((u32)0x00000001 << 5)          /*!<RW1C 0h  glitch detect swr core pos interrupt statusen. */
#define AON_BIT_GDET_VD09_NEG_ISR          ((u32)0x00000001 << 4)          /*!<RW1C 0h  glitch detect swr core neg interrupt statusen. */
#define AON_BIT_BOR_ACC_ISR                ((u32)0x00000001 << 1)          /*!<RW1C 0  BOR ACC interrupt status 0:no interrupt 1:interrupt occurs */
#define AON_BIT_BOR_ISR                    ((u32)0x00000001 << 0)          /*!<RW1C 0  BOR interrupt status 0:no interrupt 1:interrupt occurs */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_PMC_STL_SWR               ((u32)0x00000003 << 2)          /*!<R/W 2h  wait time for SWR stable 0: 64*T (T = 10us) 1: 128*T 2: 256*T 3: 512*T */
#define AON_PMC_STL_SWR(x)                 ((u32)(((x) & 0x00000003) << 2))
#define AON_GET_PMC_STL_SWR(x)             ((u32)(((x >> 2) & 0x00000003)))
#define AON_MASK_PMC_STL_RET               ((u32)0x00000003 << 0)          /*!<R/W 2h  wait time after aon ret power switch enable. 0: 32*T (T = 10us) 1: 64*T 2: 128*T 3: 256*T */
#define AON_PMC_STL_RET(x)                 ((u32)(((x) & 0x00000003) << 0))
#define AON_GET_PMC_STL_RET(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_OPT
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_SNOOZE_PDCK                ((u32)0x00000001 << 5)          /*!<R/W 1  Select xtal auto pdck function for sdm calibration use xtal lps mode 1:Enable 0:Disable */
#define AON_BIT_RTC_XTAL_MODE              ((u32)0x00000001 << 4)          /*!<R/W 1  Select xtal mode when xtal used for sdm calibration. 1: Normal 0: LPS */
#define AON_BIT_PST_WAIT_XTAL              ((u32)0x00000001 << 3)          /*!<R/W 0  1: When POF , polling xtal invalid signal and then run forward 0: not polling xtal invalid */
#define AON_BIT_PST_DSLP_ERPC              ((u32)0x00000001 << 2)          /*!<R/W 1  1: Enable AON_RET power cut when PMC enter deep-sleep mode 0: Disable */
#define AON_BIT_PST_DSLP_ERCK              ((u32)0x00000001 << 1)          /*!<R/W 0  1: Enable 100K clock for AON register access when PMC enter deep-sleep mode 0: Disable */
#define AON_BIT_PST_DSLP_LDHM              ((u32)0x00000001 << 0)          /*!<R/W 0  1: Enable AON LDO keep in active mode when PMC enter deep-sleep mode 0: Let AON LDO enter sleep mode when DSLP */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_GLITCH_DET_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_GDET_VD33_INTR_MODE        ((u32)0x00000001 << 19)          /*!<R/W 0  gdet vd33 output mode. 0: reset mode. When analog detect event, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_BIT_GDET2_VD18_INTR_MODE       ((u32)0x00000001 << 18)          /*!<R/W 0  gdet swr mem output mode. 0: reset mode. When analog detect event, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_BIT_GDET1_VD18_INTR_MODE       ((u32)0x00000001 << 17)          /*!<R/W 0  gdet swr psram output mode. 0: reset mode. When analog detect event, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_BIT_GDET_VD09_INTR_MODE        ((u32)0x00000001 << 16)          /*!<R/W 0  gdet swr core output mode. 0: reset mode. When analog detect event, hw will trigger a bor reset. 1: interrup mode. It will raise an interrup first. If this bit set to 0, then trigger a bor reset. */
#define AON_BIT_GDET_VD33_CLR              ((u32)0x00000001 << 14)          /*!<R/WA0 0  1: clear analog latched detect statue 0: enable analog detect latch Note : this field will auto clear to 0 aftert write 1 */
#define AON_BIT_GDET_VD33_SEL_POS          ((u32)0x00000001 << 13)          /*!<R/W 0  gdet voltage select for pos detect */
#define AON_BIT_GDET_VD33_SEL_NEG          ((u32)0x00000001 << 12)          /*!<R/W 0  gdet voltage select for net detect */
#define AON_BIT_GDET2_VD18_CLR             ((u32)0x00000001 << 10)          /*!<R/WA0 0  1: clear analog latched detect statue 0: enable analog detect latch Note : this field will auto clear to 0 aftert write 1 */
#define AON_BIT_GDET2_VD18_SEL_POS         ((u32)0x00000001 << 9)          /*!<R/W 0  gdet voltage select for pos detect */
#define AON_BIT_GDET2_VD18_SEL_NEG         ((u32)0x00000001 << 8)          /*!<R/W 0  gdet voltage select for net detect */
#define AON_BIT_GDET1_VD18_CLR             ((u32)0x00000001 << 6)          /*!<R/WA0 0  1: clear analog latched detect statue 0: enable analog detect latch Note : this field will auto clear to 0 aftert write 1 */
#define AON_BIT_GDET1_VD18_SEL_POS         ((u32)0x00000001 << 5)          /*!<R/W 0  gdet voltage select for pos detect */
#define AON_BIT_GDET1_VD18_SEL_NEG         ((u32)0x00000001 << 4)          /*!<R/W 0  gdet voltage select for net detect */
#define AON_BIT_GDET_VD09_CLR              ((u32)0x00000001 << 2)          /*!<R/WA0 0  1: clear analog latched detect statue 0: enable analog detect latch Note : this field will auto clear to 0 aftert write 1 */
#define AON_BIT_GDET_VD09_SEL_POS          ((u32)0x00000001 << 1)          /*!<R/W 0  gdet voltage select for pos detect */
#define AON_BIT_GDET_VD09_SEL_NEG          ((u32)0x00000001 << 0)          /*!<R/W 0  gdet voltage select for net detect */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_BOOT_CFG
 * @brief
 * @{
 *****************************************************************************/
#define AON_BIT_DSLP_RETENTION_RAM_PATCH   ((u32)0x00000001 << 0)          /*!<R/W 0  Deepsleep retention RAM patch */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP0_1
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR1_G0_5_0             ((u32)0x0000003F << 26)          /*!<R/W 0h  Instruction1 of AON PMC patch group0 */
#define AON_INSTR1_G0_5_0(x)               ((u32)(((x) & 0x0000003F) << 26))
#define AON_GET_INSTR1_G0_5_0(x)           ((u32)(((x >> 26) & 0x0000003F)))
#define AON_MASK_INSTR0_G0                 ((u32)0x0003FFFF << 8)          /*!<R/W 0h  Instruction0 of AON PMC patch group0 */
#define AON_INSTR0_G0(x)                   ((u32)(((x) & 0x0003FFFF) << 8))
#define AON_GET_INSTR0_G0(x)               ((u32)(((x >> 8) & 0x0003FFFF)))
#define AON_MASK_SADDR_G0                  ((u32)0x0000007F << 0)          /*!<R/W 0h  Start address of the first instruction of AON PMC patch group0 */
#define AON_SADDR_G0(x)                    ((u32)(((x) & 0x0000007F) << 0))
#define AON_GET_SADDR_G0(x)                ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP0_2
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR2_G0_17_0            ((u32)0x0003FFFF << 12)          /*!<R/W 0h  Instruction2 of AON PMC patch group0 */
#define AON_INSTR2_G0_17_0(x)              ((u32)(((x) & 0x0003FFFF) << 12))
#define AON_GET_INSTR2_G0_17_0(x)          ((u32)(((x >> 12) & 0x0003FFFF)))
#define AON_MASK_INSTR1_G0_17_6            ((u32)0x00000FFF << 0)          /*!<R/W 0h  Instruction1 of AON PMC patch group0 */
#define AON_INSTR1_G0_17_6(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_INSTR1_G0_17_6(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP1_1
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR1_G1_5_0             ((u32)0x0000003F << 26)          /*!<R/W 0h  Instruction1 of AON PMC patch group1 */
#define AON_INSTR1_G1_5_0(x)               ((u32)(((x) & 0x0000003F) << 26))
#define AON_GET_INSTR1_G1_5_0(x)           ((u32)(((x >> 26) & 0x0000003F)))
#define AON_MASK_INSTR0_G1                 ((u32)0x0003FFFF << 8)          /*!<R/W 0h  Instruction0 of AON PMC patch group1 */
#define AON_INSTR0_G1(x)                   ((u32)(((x) & 0x0003FFFF) << 8))
#define AON_GET_INSTR0_G1(x)               ((u32)(((x >> 8) & 0x0003FFFF)))
#define AON_MASK_SADDR_G1                  ((u32)0x0000007F << 0)          /*!<R/W 0h  Start address of the first instruction of AON PMC patch group1 */
#define AON_SADDR_G1(x)                    ((u32)(((x) & 0x0000007F) << 0))
#define AON_GET_SADDR_G1(x)                ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP1_2
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR2_G1_17_0            ((u32)0x0003FFFF << 12)          /*!<R/W 0h  Instruction2 of AON PMC patch group1 */
#define AON_INSTR2_G1_17_0(x)              ((u32)(((x) & 0x0003FFFF) << 12))
#define AON_GET_INSTR2_G1_17_0(x)          ((u32)(((x >> 12) & 0x0003FFFF)))
#define AON_MASK_INSTR1_G1_17_6            ((u32)0x00000FFF << 0)          /*!<R/W 0h  Instruction1 of AON PMC patch group1 */
#define AON_INSTR1_G1_17_6(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_INSTR1_G1_17_6(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP2_1
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR1_G2_5_0             ((u32)0x0000003F << 26)          /*!<R/W 0h  Instruction1 of AON PMC patch group2 */
#define AON_INSTR1_G2_5_0(x)               ((u32)(((x) & 0x0000003F) << 26))
#define AON_GET_INSTR1_G2_5_0(x)           ((u32)(((x >> 26) & 0x0000003F)))
#define AON_MASK_INSTR0_G2                 ((u32)0x0003FFFF << 8)          /*!<R/W 0h  Instruction0 of AON PMC patch group2 */
#define AON_INSTR0_G2(x)                   ((u32)(((x) & 0x0003FFFF) << 8))
#define AON_GET_INSTR0_G2(x)               ((u32)(((x >> 8) & 0x0003FFFF)))
#define AON_MASK_SADDR_G2                  ((u32)0x0000007F << 0)          /*!<R/W 0h  Start address of the first instruction of AON PMC patch group2 */
#define AON_SADDR_G2(x)                    ((u32)(((x) & 0x0000007F) << 0))
#define AON_GET_SADDR_G2(x)                ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_PATCH_GRP2_2
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_INSTR2_G2_17_0            ((u32)0x0003FFFF << 12)          /*!<R/W 0h  Instruction2 of AON PMC patch group2 */
#define AON_INSTR2_G2_17_0(x)              ((u32)(((x) & 0x0003FFFF) << 12))
#define AON_GET_INSTR2_G2_17_0(x)          ((u32)(((x >> 12) & 0x0003FFFF)))
#define AON_MASK_INSTR1_G2_17_6            ((u32)0x00000FFF << 0)          /*!<R/W 0h  Instruction1 of AON PMC patch group2 */
#define AON_INSTR1_G2_17_6(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_INSTR1_G2_17_6(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_ADDR01
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_PMC_PATCH_ADDR0           ((u32)0x00000FFF << 0)          /*!<R/W 12'h000  Have used for LPON patch */
#define AON_PMC_PATCH_ADDR0(x)             ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_PMC_PATCH_ADDR0(x)         ((u32)(((x >> 0) & 0x00000FFF)))
#define AON_MASK_PMC_PATCH_ADDR1           ((u32)0x00000FFF << 16)          /*!<R/W 12'h004  Have used for LPON patch */
#define AON_PMC_PATCH_ADDR1(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define AON_GET_PMC_PATCH_ADDR1(x)         ((u32)(((x >> 16) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_ADDR23
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_PMC_PATCH_ADDR2           ((u32)0x00000FFF << 0)          /*!<R/W 12'h00A  Have used for LPON patch */
#define AON_PMC_PATCH_ADDR2(x)             ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_PMC_PATCH_ADDR2(x)         ((u32)(((x >> 0) & 0x00000FFF)))
#define AON_MASK_PMC_PATCH_ADDR3           ((u32)0x00000FFF << 16)          /*!<R/W 0h  HW used only , these are for pmc patch */
#define AON_PMC_PATCH_ADDR3(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define AON_GET_PMC_PATCH_ADDR3(x)         ((u32)(((x >> 16) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_ADDR45
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_PMC_PATCH_ADDR4           ((u32)0x00000FFF << 0)          /*!<R/W 0h  HW used only , these are for pmc patch */
#define AON_PMC_PATCH_ADDR4(x)             ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_PMC_PATCH_ADDR4(x)         ((u32)(((x >> 0) & 0x00000FFF)))
#define AON_MASK_PMC_PATCH_ADDR5           ((u32)0x00000FFF << 16)          /*!<R/W 0h  HW used only , these are for pmc patch */
#define AON_PMC_PATCH_ADDR5(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define AON_GET_PMC_PATCH_ADDR5(x)         ((u32)(((x >> 16) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_PMC_ADDR67
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_PMC_PATCH_ADDR6           ((u32)0x00000FFF << 0)          /*!<R/W 0h  HW used only , these are for pmc patch */
#define AON_PMC_PATCH_ADDR6(x)             ((u32)(((x) & 0x00000FFF) << 0))
#define AON_GET_PMC_PATCH_ADDR6(x)         ((u32)(((x >> 0) & 0x00000FFF)))
#define AON_MASK_PMC_PATCH_ADDR7           ((u32)0x00000FFF << 16)          /*!<R/W 0h  HW used only , these are for pmc patch */
#define AON_PMC_PATCH_ADDR7(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define AON_GET_PMC_PATCH_ADDR7(x)         ((u32)(((x >> 16) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_SYSRST_MSK
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_MSK_SDM32K               ((u32)(3  << 30) | (0x00000001 << 26))  /*!<R/W 1  1: mask 32.768K SDM reset when sys reset happen 0: reset by sys reset */
#define APBPeriph_MSK_OTP                  ((u32)(3  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: mask OTP reset when sys reset happen 0: reset by sys reset */
#define APBPeriph_MSK_RTC                  ((u32)(3  << 30) | (0x00000001 << 19))  /*!<R/W 1  1: mask RTC reset when sys reset happen 0: reset by sys reset */
#define APBPeriph_MSK_PX                   ((u32)(3  << 30) | (0x00000001 << 17))  /*!<R/W 1  1: mask pinmux and pad setting 0: reset by sys reset */
#define APBPeriph_MSK_LPON                 ((u32)(3  << 30) | (0x00000001 << 16))  /*!<R/W 1  LS global reset 1: mask LS reset when sys reset happen 0: reset by sys reset */
#define APBPeriph_MSK_ATIM                 ((u32)(3  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: mask AON timer 0: reset by sys reset Note: AON timer starts to run only when event happens. */
/** @} */


/*==========AON Register Address Definition==========*/
#define REG_AON_PWC                                  0x0000
#define REG_AON_ISO                                  0x0004
#define REG_AON_ROR                                  0x0006
#define REG_AON_FEN                                  0x0008
#define REG_AON_CLK                                  0x000C
#define REG_AON_CKSL                                 0x0010
#define REG_AON_BOOT_REASON_HW                       0x0020
#define REG_AON_AON_WAK_EVT                          0x0024
#define REG_AON_TIM_CNT                              0x0030
#define REG_AON_TIM_SET                              0x0034
#define REG_AON_TIM_IER                              0x0038
#define REG_AON_TIM_ISR                              0x003C
#define REG_AON_GPIO_CTRL                            0x0040
#define REG_AON_GPIO_WAKE_ISR                        0x0044
#define REG_AON_CHIPEN_CTRL                          0x0050
#define REG_AON_CHIPEN_ISR                           0x0054
#define REG_AON_BOR_CTRL                             0x0058
#define REG_AON_BOR_ISR                              0x005C
#define REG_AON_PMC_CTRL                             0x0060
#define REG_AON_PMC_OPT                              0x0064
#define REG_AON_GLITCH_DET_CTRL                      0x0070
#define REG_AON_BOOT_CFG                             0x0080
#define REG_AON_PMC_PATCH_GRP0_1                     0x0090
#define REG_AON_PMC_PATCH_GRP0_2                     0x0094
#define REG_AON_PMC_PATCH_GRP1_1                     0x0098
#define REG_AON_PMC_PATCH_GRP1_2                     0x009C
#define REG_AON_PMC_PATCH_GRP2_1                     0x00A0
#define REG_AON_PMC_PATCH_GRP2_2                     0x00A4
#define REG_AON_PMC_ADDR01                           0x00B0
#define REG_AON_PMC_ADDR23                           0x00B4
#define REG_AON_PMC_ADDR45                           0x00B8
#define REG_AON_PMC_ADDR67                           0x00BC
#define REG_AON_SYSRST_MSK                           0x00D0
#define REG_AON_AON_BACKUP0                          0x00E0
#define REG_AON_AON_BACKUP1                          0x00E4
#define REG_AON_AON_BACKUP2                          0x00E8
#define REG_AON_AON_BACKUP3                          0x00EC
#define REG_AON_BOUNDARY_CHK                         0x00F0
#define REG_AON_DUMMY_1F4                            0x00F4
#define REG_AON_DUMMY_1F8                            0x00F8
#define REG_AON_DUMMY_1FC                            0x00FC

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

typedef struct {
	u8 RRAM_SYS_RSVD[256];             /* Rsvd for system */

	u32 IMQ_HEAP_ADDR;
	u32 IMQ_HEAP_SIZE;
	u32 IMQ_INIT_DONE;
	u8 VOL_TYPE;
	u8 APPLL_STATE;
	u8 MEM_TYPE;
	u8 CHIP_INFO;
	u8 PSRAM_LATENCY;
	u8 RRAM_USER_RSVD[112];            /* Rsvd for user */

	u8 RRAM_PCR_RSVD[128];             /* Rsvd for PCR */
} RRAM_TypeDef;

/* MANUAL_GEN_END */

#endif
