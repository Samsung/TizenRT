#ifndef _AMEBAD2_PAD_H_
#define _AMEBAD2_PAD_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PAD
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PAD_Register_Definitions PAD Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REG_GPIOx
 * @brief GPIO group control
 * @{
 **/
#define PAD_BIT_GPIOx_DIS           ((u32)0x00000001 << 15)          /*!<R/W 0h  PAD shutdown, only when the whole group's dis signals are all 1, the PAD SHDN will pull down to shutdown the group pad. 1. disable pad 0. enable pad */
#define PAD_BIT_GPIOx_SR            ((u32)0x00000001 << 13)          /*!<R/W 1h  PAD srew rate control */
#define PAD_BIT_GPIOx_SMT           ((u32)0x00000001 << 12)          /*!<R/W 1h  PAD Schmit control */
#define PAD_BIT_GPIOx_E2            ((u32)0x00000001 << 11)          /*!<R/W 1h  PAD driving ability control. 0: low 1: high The actual driving current is depend on pad type. */
#define PAD_BIT_GPIOx_PUPDC         ((u32)0x00000001 << 10)          /*!<R/W 0h  Some pad may have two type of PU/PD resistor, this bit can select it. 1: small resistor 0: big resistor */
#define PAD_MASK_GPIOx_SEL          ((u32)0x000000FF << 0)          /*!<R/W/ES 0h  PAD pinmux function ID selection */
#define PAD_GPIOx_SEL(x)            ((u32)(((x) & 0x000000FF) << 0))
#define PAD_GET_GPIOx_SEL(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_PU_1
 * @brief
 * @{
 **/
#define PAD_MASK_PA_PU              ((u32)0xFFFFFFFF << 0)          /*!<R/W 100013h  PAD pull up enable when system is in active. */
#define PAD_PA_PU(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PU(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_PU_2
 * @brief
 * @{
 **/
#define PAD_MASK_PB_PU              ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in active. */
#define PAD_PB_PU(x)                ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PU(x)            ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup REG_PD_1
 * @brief
 * @{
 **/
#define PAD_MASK_PA_PD              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in active. */
#define PAD_PA_PD(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PD(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_PD_2
 * @brief
 * @{
 **/
#define PAD_MASK_PB_PD              ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in active. */
#define PAD_PB_PD(x)                ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PD(x)            ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup REG_PU_SLP_1
 * @brief
 * @{
 **/
#define PAD_MASK_PA_PU_SLP          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in sleep. */
#define PAD_PA_PU_SLP(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PU_SLP(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_PU_SLP_2
 * @brief
 * @{
 **/
#define PAD_MASK_PB_PU_SLP          ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in sleep. */
#define PAD_PB_PU_SLP(x)            ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PU_SLP(x)        ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup REG_PD_SLP_1
 * @brief
 * @{
 **/
#define PAD_MASK_PA_PD_SLP          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in sleep. */
#define PAD_PA_PD_SLP(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PD_SLP(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_PD_SLP_2
 * @brief
 * @{
 **/
#define PAD_MASK_PB_PD_SLP          ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in sleep. */
#define PAD_PB_PD_SLP(x)            ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PD_SLP(x)        ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/** @defgroup REG_DDR_PAD_CTRL0
 * @brief
 * @{
 **/
#define PAD_BIT_PWDPAD_DQ_EN        ((u32)0x00000001 << 31)          /*!<R/W 0  enable to shutdown dq pads when idle, for psram must be set after configuring pwdpad_idle_val Note: only stop psram auto axi access, manually enable/disable this bit when using psram user axi access */
#define PAD_MASK_PWDPAD_RESUME_VAL  ((u32)0x0000001F << 25)          /*!<R/W 5'h1f  dq pads resume counter setting, only for psram, auto axi access will be stopped until resume done */
#define PAD_PWDPAD_RESUME_VAL(x)    ((u32)(((x) & 0x0000001F) << 25))
#define PAD_GET_PWDPAD_RESUME_VAL(x) ((u32)(((x >> 25) & 0x0000001F)))
#define PAD_MASK_PWDPAD_IDLE_VAL    ((u32)0x000001FF << 16)          /*!<R/W 9'h1ff  idle counter setting, only for psram, shutdown dq pads after reaching this value, must > 0 */
#define PAD_PWDPAD_IDLE_VAL(x)      ((u32)(((x) & 0x000001FF) << 16))
#define PAD_GET_PWDPAD_IDLE_VAL(x)  ((u32)(((x >> 16) & 0x000001FF)))
#define PAD_MASK_DDR_PD             ((u32)0x00000FFF << 0)          /*!<R/W 0  Power-down mode, reserved pin */
#define PAD_DDR_PD(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define PAD_GET_DDR_PD(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup REG_DDR_PAD_CTRL1
 * @brief
 * @{
 **/
#define PAD_BIT_DDR2_MODE           ((u32)0x00000001 << 31)          /*!<R/W 0  DDR2 mode control, set ‘1’ for DDR2 mode */
#define PAD_BIT_DDR_DTN             ((u32)0x00000001 << 30)          /*!<R/W 0  Input duty fine tuning */
#define PAD_BIT_DDR_DTP             ((u32)0x00000001 << 29)          /*!<R/W 0  Input duty fine tuning */
#define PAD_MASK_DDR_NT             ((u32)0x0000003F << 23)          /*!<R/W 0  IO output NMOS driving select: active-high, set ‘1’ to increase output N driving capability */
#define PAD_DDR_NT(x)               ((u32)(((x) & 0x0000003F) << 23))
#define PAD_GET_DDR_NT(x)           ((u32)(((x >> 23) & 0x0000003F)))
#define PAD_MASK_DDR_NTB            ((u32)0x0000003F << 17)          /*!<R/W 0  IOB output NMOS driving select: active-high, set ‘1’ to increase output N driving capability */
#define PAD_DDR_NTB(x)              ((u32)(((x) & 0x0000003F) << 17))
#define PAD_GET_DDR_NTB(x)          ((u32)(((x >> 17) & 0x0000003F)))
#define PAD_BIT_DDR_VREF_RANGE      ((u32)0x00000001 << 16)          /*!<R/W 0  VREF range selector, 0 : 38%~53%, 1 : 44%~59% */
#define PAD_BIT_DDR_PD_REF          ((u32)0x00000001 << 15)          /*!<R/W 1  Power-down mode: active-high, set ‘1’ to disable REF and VREF_DLSS */
#define PAD_BIT_DDR_PWDPADN_2REGU   ((u32)0x00000001 << 14)          /*!<R/W 0  Core power down mode control, need level shift to 3.3V Set “0” for core power down mode and disable all level shifters Set “1” for normal operating condition. */
#define PAD_MASK_DDR_PT             ((u32)0x0000003F << 6)          /*!<R/W 0  IO output PMOS driving select: active-high, set ‘1’ to increase output P driving capability */
#define PAD_DDR_PT(x)               ((u32)(((x) & 0x0000003F) << 6))
#define PAD_GET_DDR_PT(x)           ((u32)(((x >> 6) & 0x0000003F)))
#define PAD_MASK_DDR_PTB            ((u32)0x0000003F << 0)          /*!<R/W 0  IOB output PMOS driving select: active-high, set ‘1’ to increase output P driving capability */
#define PAD_DDR_PTB(x)              ((u32)(((x) & 0x0000003F) << 0))
#define PAD_GET_DDR_PTB(x)          ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup REG_DDR_PAD_CTRL2
 * @brief
 * @{
 **/
#define PAD_MASK_DDR_S              ((u32)0x0000003F << 25)          /*!<R/W 0  REF voltage fine-tune, add 1% for 1 bit, total 15 bit */
#define PAD_DDR_S(x)                ((u32)(((x) & 0x0000003F) << 25))
#define PAD_GET_DDR_S(x)            ((u32)(((x >> 25) & 0x0000003F)))
#define PAD_BIT_DDR_TE              ((u32)0x00000001 << 24)          /*!<R/W 0  Termination resistor enable: active-high, set ‘1’ to enable ODT when “E” is ‘0’. */
#define PAD_MASK_DDR_TTCN           ((u32)0x0000003F << 18)          /*!<R/W 0  IO N-ODT coarse trimming: active-high */
#define PAD_DDR_TTCN(x)             ((u32)(((x) & 0x0000003F) << 18))
#define PAD_GET_DDR_TTCN(x)         ((u32)(((x >> 18) & 0x0000003F)))
#define PAD_MASK_DDR_TTCNB          ((u32)0x0000003F << 12)          /*!<R/W 0  IOB N-ODT coarse trimming: active-high */
#define PAD_DDR_TTCNB(x)            ((u32)(((x) & 0x0000003F) << 12))
#define PAD_GET_DDR_TTCNB(x)        ((u32)(((x >> 12) & 0x0000003F)))
#define PAD_MASK_DDR_TTCP           ((u32)0x0000003F << 6)          /*!<R/W 0  IO P-ODT coarse trimming: active-high */
#define PAD_DDR_TTCP(x)             ((u32)(((x) & 0x0000003F) << 6))
#define PAD_GET_DDR_TTCP(x)         ((u32)(((x >> 6) & 0x0000003F)))
#define PAD_MASK_DDR_TTCPB          ((u32)0x0000003F << 0)          /*!<R/W 0  IOB P-ODT coarse trimming: active-high */
#define PAD_DDR_TTCPB(x)            ((u32)(((x) & 0x0000003F) << 0))
#define PAD_GET_DDR_TTCPB(x)        ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup REG_PAD_AUD_PAD_CTRL
 * @brief
 * @{
 **/
#define PAD_MASK_GPIOE_IE           ((u32)0x000001FF << 0)          /*!<R/W 0h  Audio share pad input enable. */
#define PAD_GPIOE_IE(x)             ((u32)(((x) & 0x000001FF) << 0))
#define PAD_GET_GPIOE_IE(x)         ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup REG_VOLTAGE_PROBE_OE
 * @brief
 * @{
 **/
#define PAD_MASK_VPROB_SYSON_OE     ((u32)0x0000000F << 0)          /*!<R/W 0h   */
#define PAD_VPROB_SYSON_OE(x)       ((u32)(((x) & 0x0000000F) << 0))
#define PAD_GET_VPROB_SYSON_OE(x)   ((u32)(((x >> 0) & 0x0000000F)))
#define PAD_MASK_VPROB_SOC_OE       ((u32)0x0000000F << 4)          /*!<R/W 0h   */
#define PAD_VPROB_SOC_OE(x)         ((u32)(((x) & 0x0000000F) << 4))
#define PAD_GET_VPROB_SOC_OE(x)     ((u32)(((x >> 4) & 0x0000000F)))
#define PAD_MASK_VPROB_BT_OE        ((u32)0x00000003 << 8)          /*!<R/W 0h   */
#define PAD_VPROB_BT_OE(x)          ((u32)(((x) & 0x00000003) << 8))
#define PAD_GET_VPROB_BT_OE(x)      ((u32)(((x >> 8) & 0x00000003)))
#define PAD_BIT_VPROB_EN0           ((u32)0x00000001 << 10)          /*!<R/W 0h   */
/** @} */

/** @defgroup REG_I2S_CTRL
 * @brief
 * @{
 **/
#define PAD_BIT_UART3_SEL_EXT_PAD   ((u32)0x00000001 << 16)          /*!<R/W 0  NOTE：this bit reserved, UART3 selection controlled by 0x4100C280.bit[19]. */
#define PAD_BIT_SP1_DIO3_MUXSEL     ((u32)0x00000001 << 7)          /*!<R/W 0  SPORT1 DIO3 function mux 0: DIN_3 func 1: DOUT_0 func */
#define PAD_BIT_SP1_DIO2_MUXSEL     ((u32)0x00000001 << 6)          /*!<R/W 0  SPORT1 DIO2 function mux 0: DIN_2 func 1: DOUT_1 func */
#define PAD_BIT_SP1_DIO1_MUXSEL     ((u32)0x00000001 << 5)          /*!<R/W 0  SPORT1 DIO1 function mux 0: DIN_1 func 1: DOUT_2 func */
#define PAD_BIT_SP1_DIO0_MUXSEL     ((u32)0x00000001 << 4)          /*!<R/W 0  SPORT1 DIO0 function mux 0: DIN_0 func 1: DOUT_3 func */
#define PAD_BIT_SP0_DIO3_MUXSEL     ((u32)0x00000001 << 3)          /*!<R/W 0  SPORT0 DIO3 function mux 0: DIN_3 func 1: DOUT_0 func */
#define PAD_BIT_SP0_DIO2_MUXSEL     ((u32)0x00000001 << 2)          /*!<R/W 0  SPORT0 DIO2 function mux 0: DIN_2 func 1: DOUT_1 func */
#define PAD_BIT_SP0_DIO1_MUXSEL     ((u32)0x00000001 << 1)          /*!<R/W 0  SPORT0 DIO1 function mux 0: DIN_1 func 1: DOUT_2 func */
#define PAD_BIT_SP0_DIO0_MUXSEL     ((u32)0x00000001 << 0)          /*!<R/W 0  SPORT0 DIO0 function mux 0: DIN_0 func 1: DOUT_3 func */
/** @} */

/** @defgroup REG_DBG_PORT_CTRL
 * @brief
 * @{
 **/
#define PAD_BIT_DBG_PORT_EN         ((u32)0x00000001 << 31)          /*!<R/W 0h   */
#define PAD_BIT_DBG_PORT_EN2        ((u32)0x00000001 << 30)          /*!<R/WA0 0h   */
#define PAD_MASK_DBG_PORT_SEL       ((u32)0x000000FF << 0)          /*!<R/W 0h   */
#define PAD_DBG_PORT_SEL(x)         ((u32)(((x) & 0x000000FF) << 0))
#define PAD_GET_DBG_PORT_SEL(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_DBG_GPIO_MAP_0
 * @brief
 * @{
 **/
#define PAD_MASK_GPIO_DMUX_SEL3     ((u32)0x0000001F << 24)          /*!<R/W 3h   */
#define PAD_GPIO_DMUX_SEL3(x)       ((u32)(((x) & 0x0000001F) << 24))
#define PAD_GET_GPIO_DMUX_SEL3(x)   ((u32)(((x >> 24) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL2     ((u32)0x0000001F << 16)          /*!<R/W 2h   */
#define PAD_GPIO_DMUX_SEL2(x)       ((u32)(((x) & 0x0000001F) << 16))
#define PAD_GET_GPIO_DMUX_SEL2(x)   ((u32)(((x >> 16) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL1     ((u32)0x0000001F << 8)          /*!<R/W 1h   */
#define PAD_GPIO_DMUX_SEL1(x)       ((u32)(((x) & 0x0000001F) << 8))
#define PAD_GET_GPIO_DMUX_SEL1(x)   ((u32)(((x >> 8) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL0     ((u32)0x0000001F << 0)          /*!<R/W 0h   */
#define PAD_GPIO_DMUX_SEL0(x)       ((u32)(((x) & 0x0000001F) << 0))
#define PAD_GET_GPIO_DMUX_SEL0(x)   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_DBG_GPIO_MAP_1
 * @brief
 * @{
 **/
#define PAD_MASK_GPIO_DMUX_SEL7     ((u32)0x0000001F << 24)          /*!<R/W 7h   */
#define PAD_GPIO_DMUX_SEL7(x)       ((u32)(((x) & 0x0000001F) << 24))
#define PAD_GET_GPIO_DMUX_SEL7(x)   ((u32)(((x >> 24) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL6     ((u32)0x0000001F << 16)          /*!<R/W 6h   */
#define PAD_GPIO_DMUX_SEL6(x)       ((u32)(((x) & 0x0000001F) << 16))
#define PAD_GET_GPIO_DMUX_SEL6(x)   ((u32)(((x >> 16) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL5     ((u32)0x0000001F << 8)          /*!<R/W 5h   */
#define PAD_GPIO_DMUX_SEL5(x)       ((u32)(((x) & 0x0000001F) << 8))
#define PAD_GET_GPIO_DMUX_SEL5(x)   ((u32)(((x >> 8) & 0x0000001F)))
#define PAD_MASK_GPIO_DMUX_SEL4     ((u32)0x0000001F << 0)          /*!<R/W 4h   */
#define PAD_GPIO_DMUX_SEL4(x)       ((u32)(((x) & 0x0000001F) << 0))
#define PAD_GET_GPIO_DMUX_SEL4(x)   ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_PINMUX_SUB_CTRL
 * @brief
 * @{
 **/
#define PAD_MASK_AUX_TEST_SUBMODE   ((u32)0x00000007 << 5)          /*!<R/W 0h  000: aux adc test 001: flash pll 010: debug clock and power */
#define PAD_AUX_TEST_SUBMODE(x)     ((u32)(((x) & 0x00000007) << 5))
#define PAD_GET_AUX_TEST_SUBMODE(x) ((u32)(((x >> 5) & 0x00000007)))
/** @} */

/** @defgroup REG_SWD_SDD_CTRL
 * @brief
 * @{
 **/
#define PAD_BIT_KM4_SWD_SEL         ((u32)0x00000001 << 3)          /*!<R/W 1  select the share swd signal to KM4 connection write the inverse value to take effect */
#define PAD_BIT_SWD_PMUX_EN         ((u32)0x00000001 << 0)          /*!<R/W 1  1: Force enable share SWD pinmux function 0: Disable */
/** @} */

/** @defgroup REG_PAD_REG_DUMMY_0
 * @brief
 * @{
 **/
#define PAD_MASK_GPIOD_IE           ((u32)0x000003FF << 0)          /*!<R/W 3ffh  ECO:GPIOD5~14 IE signals */
#define PAD_GPIOD_IE(x)             ((u32)(((x) & 0x000003FF) << 0))
#define PAD_GET_GPIOD_IE(x)         ((u32)(((x >> 0) & 0x000003FF)))
/** @} */


/*==========PAD Register Address Definition==========*/
#define REG_PU_1                                     0x0100
#define REG_PU_2                                     0x0104
#define REG_PD_1                                     0x0108
#define REG_PD_2                                     0x010C
#define REG_PU_SLP_1                                 0x0110
#define REG_PU_SLP_2                                 0x0114
#define REG_PD_SLP_1                                 0x0118
#define REG_PD_SLP_2                                 0x011C
#define REG_PAD_REG_DUMMY_1                          0x0120
#define REG_DDR_PAD_CTRL0                            0x01BC
#define REG_DDR_PAD_CTRL1                            0x01C0
#define REG_DDR_PAD_CTRL2                            0x01C4
#define REG_PAD_AUD_PAD_CTRL                         0x01D8
#define REG_VOLTAGE_PROBE_OE                         0x01DC
#define REG_I2S_CTRL                                 0x01E0
#define REG_DBG_PORT_CTRL                            0x01E4
#define REG_DBG_GPIO_MAP_0                           0x01E8
#define REG_DBG_GPIO_MAP_1                           0x01EC
#define REG_PINMUX_SUB_CTRL                          0x01F0
#define REG_SWD_SDD_CTRL                             0x01F4
#define REG_PAD_REG_DUMMY_0                          0x01F8

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/**************************************************************************//**
 * @defgroup AMEBAD_PINMUX
 * @{
 * @brief AMEBAD_PINMUX Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t REG_GPIOx[52];			/*!< Pad control register */
} PINMUX_TypeDef;
/** @} */




/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
