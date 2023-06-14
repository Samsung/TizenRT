#ifndef _AMEBAD2_PAD_H_
#define _AMEBAD2_PAD_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_GPIOx
 * @brief GPIO group control
 * @{
 *****************************************************************************/
#define PAD_BIT_GPIOx_PD_SLP           ((u32)0x00000001 << 17)          /*!<R/W 0h  PAD pull down enable when system is in sleep. */
#define PAD_BIT_GPIOx_PU_SLP           ((u32)0x00000001 << 16)          /*!<R/W 0h  PAD pull up enable when system is in sleep. */
#define PAD_BIT_GPIOx_DIS              ((u32)0x00000001 << 15)          /*!<R/W 0h  PAD shutdown, only when the whole group's dis signals are all 1, the PAD SHDN will pull down to shutdown the group pad. 1. disable pad 0. enable pad */
#define PAD_BIT_GPIOx_SR               ((u32)0x00000001 << 13)          /*!<R/W 1h  PAD srew rate control */
#define PAD_BIT_GPIOx_SMT              ((u32)0x00000001 << 12)          /*!<R/W 1h  PAD Schmit control */
#define PAD_BIT_GPIOx_E2               ((u32)0x00000001 << 11)          /*!<R/W 1h  PAD driving ability control. 0: low 1: high The actual driving current is depend on pad type. */
#define PAD_BIT_GPIOx_PUPDC            ((u32)0x00000001 << 10)          /*!<R/W 0h  Some pad may have two type of PU/PD resistor, this bit can select it. 1: small resistor 0: big resistor */
#define PAD_BIT_GPIOx_PD               ((u32)0x00000001 << 9)          /*!<R/W 0h  PAD pull down enable when system is in active. */
#define PAD_BIT_GPIOx_PU               ((u32)0x00000001 << 8)          /*!<R/W 0h  PAD pull up enable when system is in active. */
#define PAD_MASK_GPIOx_SEL             ((u32)0x0000001F << 0)          /*!<R/W/ES 0h  PAD pinmux function ID selection */
#define PAD_GPIOx_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define PAD_GET_GPIOx_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PAD_AUD_PAD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_GPIOC_IE              ((u32)0x001FFFFF << 0)          /*!<R/W 0h  Audio share pad input enable. */
#define PAD_GPIOC_IE(x)                ((u32)(((x) & 0x001FFFFF) << 0))
#define PAD_GET_GPIOC_IE(x)            ((u32)(((x >> 0) & 0x001FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PAD_SDIO_PAD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_BIT_GPIOG_P5E              ((u32)0x00000001 << 7)          /*!<R/W 0h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_P4E              ((u32)0x00000001 << 6)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_P3E              ((u32)0x00000001 << 5)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_P2E              ((u32)0x00000001 << 4)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_N5E              ((u32)0x00000001 << 3)          /*!<R/W 0h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_N4E              ((u32)0x00000001 << 2)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_N3E              ((u32)0x00000001 << 1)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
#define PAD_BIT_GPIOG_N2E              ((u32)0x00000001 << 0)          /*!<R/W 1h  SDIO pad drive control. All SDIO pad share the same parameter. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_VOLTAGE_PROBE_OE
 * @brief
 * @{
 *****************************************************************************/
#define PAD_BIT_VPROB_CA7_OE           ((u32)0x00000001 << 0)          /*!<R/W/ES 0h   */
#define PAD_BIT_VPROB_DDRPHY0_OE       ((u32)0x00000001 << 1)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_DDRPHY1_OE       ((u32)0x00000001 << 2)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_LEFT_OE          ((u32)0x00000001 << 3)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_RIGHTBOTTOM_OE   ((u32)0x00000001 << 4)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_RIGHTTOP_OE      ((u32)0x00000001 << 5)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_LP_SYSON_OE      ((u32)0x00000001 << 6)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_WIFI_OE          ((u32)0x00000001 << 7)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_DDRC_OE          ((u32)0x00000001 << 8)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_NP_OE            ((u32)0x00000001 << 9)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_EN0              ((u32)0x00000001 << 10)          /*!<R/W 0h   */
#define PAD_BIT_VPROB_TOPLEFT_OE       ((u32)0x00000001 << 11)          /*!<R/W 0h  add in MP */
/** @} */

/**************************************************************************//**
 * @defgroup REG_DBG_PORT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_BIT_DBG_PORT_EN            ((u32)0x00000001 << 31)          /*!<R/W 0h  enable dbgmux function */
#define PAD_BIT_DBG_PORT_EN2           ((u32)0x00000001 << 30)          /*!<R/WA0 0h  Write this bit to trigger all GPIO pad's FUNC ID changes to debug port function. After writing this bit, the sel bits in every pad register will change to FUNC debug port value. And this bit will be cleared by HW. */
#define PAD_MASK_DBG_PORT_SEL          ((u32)0x000000FF << 0)          /*!<R/W 0h  select the debug port's specific connection with different IP. The specific relationship is refered to the dbg port spec. */
#define PAD_DBG_PORT_SEL(x)            ((u32)(((x) & 0x000000FF) << 0))
#define PAD_GET_DBG_PORT_SEL(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DBG_GPIO_MAP_0
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_GPIO_DMUX_SEL3        ((u32)0x0000001F << 24)          /*!<R/W 3h   */
#define PAD_GPIO_DMUX_SEL3(x)          ((u32)(((x) & 0x0000001F) << 24))
#define PAD_GET_GPIO_DMUX_SEL3(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL2        ((u32)0x0000001F << 16)          /*!<R/W 2h   */
#define PAD_GPIO_DMUX_SEL2(x)          ((u32)(((x) & 0x0000001F) << 16))
#define PAD_GET_GPIO_DMUX_SEL2(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL1        ((u32)0x0000001F << 8)          /*!<R/W 1h   */
#define PAD_GPIO_DMUX_SEL1(x)          ((u32)(((x) & 0x0000001F) << 8))
#define PAD_GET_GPIO_DMUX_SEL1(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL0        ((u32)0x0000001F << 0)          /*!<R/W 0h   */
#define PAD_GPIO_DMUX_SEL0(x)          ((u32)(((x) & 0x0000001F) << 0))
#define PAD_GET_GPIO_DMUX_SEL0(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_DBG_GPIO_MAP_1
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_GPIO_DMUX_SEL7        ((u32)0x0000001F << 24)          /*!<R/W 7h   */
#define PAD_GPIO_DMUX_SEL7(x)          ((u32)(((x) & 0x0000001F) << 24))
#define PAD_GET_GPIO_DMUX_SEL7(x)      ((u32)(((x >> 24) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL6        ((u32)0x0000001F << 16)          /*!<R/W 6h   */
#define PAD_GPIO_DMUX_SEL6(x)          ((u32)(((x) & 0x0000001F) << 16))
#define PAD_GET_GPIO_DMUX_SEL6(x)      ((u32)(((x >> 16) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL5        ((u32)0x0000001F << 8)          /*!<R/W 5h   */
#define PAD_GPIO_DMUX_SEL5(x)          ((u32)(((x) & 0x0000001F) << 8))
#define PAD_GET_GPIO_DMUX_SEL5(x)      ((u32)(((x >> 8) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL4        ((u32)0x0000001F << 0)          /*!<R/W 4h   */
#define PAD_GPIO_DMUX_SEL4(x)          ((u32)(((x) & 0x0000001F) << 0))
#define PAD_GET_GPIO_DMUX_SEL4(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PINMUX_SUB_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_FLASH_PG1B_SE         ((u32)0x0000000F << 12)          /*!<R/W 2h  When TestMode 13 , using 1bit mode to pg MCM flash using ext pin . [0] : if 0 , ext spi_data0 pin fix to input , if 1 ext spi_data0 pin fix to output [1] : if 0 , ext spi_data1 pin fix to input , if 1 ext spi_data1 pin fix to output [2] : if 0 , ext spi_data2 pin fix to input , if 1 ext spi_data2 pin fix to output [3] : if 0 , ext spi_data3 pin fix to input , if 1 ext spi_data3 pin fix to output default is : 0010 which only data1 using as output. */
#define PAD_FLASH_PG1B_SE(x)           ((u32)(((x) & 0x0000000F) << 12))
#define PAD_GET_FLASH_PG1B_SE(x)       ((u32)(((x >> 12) & 0x0000000F)))
#define PAD_BIT_TRANS_GATE_EN          ((u32)0x00000001 << 10)          /*!<R/W 0h   */
#define PAD_BIT_TRANS_GATE_HWPDN       ((u32)0x00000001 << 9)          /*!<R/W 1h   */
#define PAD_MASK_AUX_TEST_SUBMODE      ((u32)0x00000007 << 5)          /*!<R/W 0h  000: aux adc test 001: flash pll 010: i2s pll 011: i2s signal 100: debug clock 101: power */
#define PAD_AUX_TEST_SUBMODE(x)        ((u32)(((x) & 0x00000007) << 5))
#define PAD_GET_AUX_TEST_SUBMODE(x)    ((u32)(((x >> 5) & 0x00000007)))
#define PAD_MASK_SWR_TEST_SEL          ((u32)0x00000003 << 0)          /*!<R/W 0h  00/11: swr core test out 01: aud/psram swr test out 10: ddr mem swr test out */
#define PAD_SWR_TEST_SEL(x)            ((u32)(((x) & 0x00000003) << 0))
#define PAD_GET_SWR_TEST_SEL(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_SWD_SDD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_BIT_SDD_PMUX_EN            ((u32)0x00000001 << 2)          /*!<R/W 0  1: Enable SDIO pinmux enable function 0: Disable */
#define PAD_BIT_SWD_PMUX_EN            ((u32)0x00000001 << 0)          /*!<R/W 1  1: Enable SWD pinmux enable function 0: Disable SWD_LOC is valid only when this bit is 1。 */
/** @} */


/*==========PAD Register Address Definition==========*/
#define REG_PAD_AUD_PAD_CTRL                         0x0120
#define REG_PAD_SDIO_PAD_CTRL                        0x0124
#define REG_VOLTAGE_PROBE_OE                         0x01DC
#define REG_PAD_EXTRA_CTRL                           0x01E0
#define REG_DBG_PORT_CTRL                            0x01E4
#define REG_DBG_GPIO_MAP_0                           0x01E8
#define REG_DBG_GPIO_MAP_1                           0x01EC
#define REG_PINMUX_SUB_CTRL                          0x01F0
#define REG_DBG_PORT_VAL                             0x01F4
#define REG_SWD_SDD_CTRL                             0x01F8

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
