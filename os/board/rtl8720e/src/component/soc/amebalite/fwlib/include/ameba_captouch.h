#ifndef _AMEBA_CAPTOUCH_H_
#define _AMEBA_CAPTOUCH_H_

/** @addtogroup CAPTOUCH
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * CAPTOUCH:
  *		- Base Address: CAPTOUCH_DEV
  *		- Clock source : 32.768KHz
  *		- Scan interval timer: 1.024KHz(32.768KHz/32)
  *		- Debounce Timer: Configurable
  *		- Channel: 9
  *		- IRQ: CapTouch_IRQ
  *
  *****************************************************************************************
  * How to use Normal CapTouch
  *****************************************************************************************
  *      To use the normal CapTouch mode, the following steps are mandatory:
  *
  *      1. Enable CapTouch peripheral clock
  *
  *      2. Configure the CapTouch pinmux.
  *
  *      3. Init Captouch parameters:
  *			CapTouch_StructInit(CapTouch_InitTypeDef* CapTouch_InitStruct)
  *
  *      4. Init Hardware use step3 parameters:
  *			CapTouch_Init(CAPTOUCH_TypeDef *CapTouch, CapTouch_InitTypeDef* CapTouch_InitStruct)
  *
  *      5. Enable the NVIC and the corresponding interrupt using following function if you need
  *			to use interrupt mode.
  *			CapTouch_INTConfig(): CapTouch IRQ Enable set
  *			CapTouch_INTMask(): CapTouch IRQ mask set
  *			InterruptRegister(): register the captouch irq handler
  *			InterruptEn(): Enable the NVIC interrupt
  *
  *      6. Enable CapTouch module using CapTouch_Cmd().
  *
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup CAPTOUCH
  * @brief Captouch driver modules
  * @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup CAPTOUCH_Register_Definitions CAPTOUCH Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup CT_CTC_CTRL
 * @brief Captouch Control Register
 * @{
 **/
#define CT_BIT_BASELINE_INI              ((u32)0x00000001 << 8)          /*!<R/W/EC 1h  1: Baseline initial function enable, HW will clear this bit to "0" after baseline initial */
#define CT_MASK_DEBOUNCE_TIME            ((u32)0x00000003 << 5)          /*!<R/W 0h  2'b00: 2 times 2'b01: 3 times 2'b10: 4 times 2'b11: 5 times */
#define CT_DEBOUNCE_TIME(x)              ((u32)(((x) & 0x00000003) << 5))
#define CT_GET_DEBOUNCE_TIME(x)          ((u32)(((x >> 5) & 0x00000003)))
#define CT_BIT_DEBOUNCE_EN               ((u32)0x00000001 << 4)          /*!<R/W 0h  0: Disable 1: Enable */
#define CT_BIT_ENABLE                    ((u32)0x00000001 << 0)          /*!<R/W 0h  0: CTC disable 1: CTC enable */
/** @} */

/** @defgroup CT_SCAN_PERIOD
 * @brief Scan Parameters Register
 * @{
 **/
#define CT_MASK_SAMPLE_AVE_CTRL          ((u32)0x00000007 << 16)          /*!<R/W 6h  ADC sampled number for average function average number=2^(sample_ave_ctrl+2) */
#define CT_SAMPLE_AVE_CTRL(x)            ((u32)(((x) & 0x00000007) << 16))
#define CT_GET_SAMPLE_AVE_CTRL(x)        ((u32)(((x >> 16) & 0x00000007)))
#define CT_MASK_SCAN_INTERVAL            ((u32)0x00000FFF << 0)          /*!<R/W 3Ch  Code: 0~4095 (0~0xFFF), unit is 1.024KHz cycle (1/32 32.768KHz). When this register is set to 0 or 1, HW will scan continuously and have no sleep time. Recommend value: 60~480ms */
#define CT_SCAN_INTERVAL(x)              ((u32)(((x) & 0x00000FFF) << 0))
#define CT_GET_SCAN_INTERVAL(x)          ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup CT_ETC_CTRL
 * @brief Environment Tracking Control Register
 * @{
 **/
#define CT_MASK_BASELINE_UPD_STEP        ((u32)0x0000000F << 12)          /*!<R/W 1h  Baseline update step for all channel */
#define CT_BASELINE_UPD_STEP(x)          ((u32)(((x) & 0x0000000F) << 12))
#define CT_GET_BASELINE_UPD_STEP(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define CT_MASK_BASELINE_WT_FACTOR       ((u32)0x0000000F << 8)          /*!<R/W 2h  Baseline update weight factor (ETC_factor_th) for all channel ETC_factor_th=2^baseline_wt_factor */
#define CT_BASELINE_WT_FACTOR(x)         ((u32)(((x) & 0x0000000F) << 8))
#define CT_GET_BASELINE_WT_FACTOR(x)     ((u32)(((x >> 8) & 0x0000000F)))
#define CT_MASK_ETC_SCAN_INTERVAL        ((u32)0x0000007F << 1)          /*!<R/W 2h  ETC update interval between scan period (sleep time) for all channel Interval=(etc_scan_interval+1)*scan_period */
#define CT_ETC_SCAN_INTERVAL(x)          ((u32)(((x) & 0x0000007F) << 1))
#define CT_GET_ETC_SCAN_INTERVAL(x)      ((u32)(((x >> 1) & 0x0000007F)))
#define CT_BIT_ETC_FUNC_CTRL             ((u32)0x00000001 << 0)          /*!<R/W 0h  Environmental cap tracking calibration function 0: Disable 1: Enable */
/** @} */

/** @defgroup CT_SNR_INF
 * @brief Signal Noise Ratio Register
 * @{
 **/
#define CT_BIT_SNR_UPD_ALWAYS            ((u32)0x00000001 << 31)          /*!<R/W 0  1: Update SNR info no matter touch or not (used only in debug function) */
#define CT_MASK_SNR_NOISE_DATA           ((u32)0x00000FFF << 16)          /*!<R/W/ES 0  Noise peak to peak signal raw data for SNR monitor */
#define CT_SNR_NOISE_DATA(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define CT_GET_SNR_NOISE_DATA(x)         ((u32)(((x >> 16) & 0x00000FFF)))
#define CT_MASK_SNR_TOUCH_DATA           ((u32)0x00000FFF << 0)          /*!<R/W/ES 0h  Raw data of touch(signal-baseline)for SNR monitor */
#define CT_SNR_TOUCH_DATA(x)             ((u32)(((x) & 0x00000FFF) << 0))
#define CT_GET_SNR_TOUCH_DATA(x)         ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup CT_DEBUG_MODE_CTRL
 * @brief Debug and Channel Swith Mode Register
 * @{
 **/
#define CT_MASK_CH_CTRL                  ((u32)0x0000000F << 5)          /*!<R/W 0h  Scan channel control 0000: channel 0 0001: channel 1 0010: channel 2 0011: channel 3 0100: channel 4 0101: channel 5 0110: channel 6 0111: channel 7 1000: channel 8 (guard sensor) */
#define CT_CH_CTRL(x)                    ((u32)(((x) & 0x0000000F) << 5))
#define CT_GET_CH_CTRL(x)                ((u32)(((x >> 5) & 0x0000000F)))
#define CT_BIT_CH_SWITCH_CTRL            ((u32)0x00000001 << 4)          /*!<R/W 1h  Scan channel switch control 0: Auto switch function disable (manual switch by ch_ctrl) 1: Auto switch to next channel (channel 0 --> channel 3 --> channel 0) */
#define CT_BIT_CONTINUE_SAMPLE_EN        ((u32)0x00000001 << 1)          /*!<R/W 0  continue sample one channel mode enable, only take effect in debug mode 1: enable 0: disable */
#define CT_BIT_DEBUG_EN                  ((u32)0x00000001 << 0)          /*!<R/W 0  Debug mode enable 0: Disable 1: Enable */
/** @} */

/** @defgroup CT_RAW_CODE_FIFO_STATUS
 * @brief FIFO Status Register
 * @{
 **/
#define CT_BIT_AFIFO_CLEAR               ((u32)0x00000001 << 16)          /*!<WA0 0  clear the FIFO data */
#define CT_MASK_AFIFO_FULL_LVL           ((u32)0x0000003F << 10)          /*!<R/W 0  user define the full level for afifo, the valid value is from 0 to 63 */
#define CT_AFIFO_FULL_LVL(x)             ((u32)(((x) & 0x0000003F) << 10))
#define CT_GET_AFIFO_FULL_LVL(x)         ((u32)(((x >> 10) & 0x0000003F)))
#define CT_MASK_AFIFO_VALID_CNT          ((u32)0x0000007F << 3)          /*!<R 0h  Raw code FIFO valid cnt(push unit number which can be read) */
#define CT_AFIFO_VALID_CNT(x)            ((u32)(((x) & 0x0000007F) << 3))
#define CT_GET_AFIFO_VALID_CNT(x)        ((u32)(((x >> 3) & 0x0000007F)))
#define CT_BIT_AFIFO_OVER_LVL            ((u32)0x00000001 << 2)          /*!<R 0  0: Raw code FIFO not over level 1: Raw code FIFO over level */
#define CT_BIT_AFIFO_EMPTY               ((u32)0x00000001 << 1)          /*!<R 1  0: Raw code FIFO not empty 1: Raw code FIFO empty */
#define CT_BIT_AFIFO_FULL                ((u32)0x00000001 << 0)          /*!<R 0  0: Raw code FIFO not oveflow (not full) 1: Raw code FIFO overflow(full) */
/** @} */

/** @defgroup CT_RAW_CODE_FIFO_READ
 * @brief FIFO Read Register
 * @{
 **/
#define CT_BIT_AFIFO_RD_DATA_VLD         ((u32)0x00000001 << 31)          /*!<R 0h  Read data from raw code FIFO valid */
#define CT_MASK_AFIFO_RD_DATA            ((u32)0x0000FFFF << 0)          /*!<RP 0h  Read data from raw code FIFO [11:0] : data [15:12] : channel number */
#define CT_AFIFO_RD_DATA(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define CT_GET_AFIFO_RD_DATA(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup CT_INTERRUPT_ENABLE
 * @brief Interrupt Enable Register
 * @{
 **/
#define CT_BIT_AFIFO_OVERLVL_INTR_EN     ((u32)0x00000001 << 21)          /*!<R/W 0h  Raw code FIFO over level enable */
#define CT_BIT_GUARD_RELEASE_INTR_EN     ((u32)0x00000001 << 20)          /*!<R/W 0h  Guard sensor release enable */
#define CT_BIT_GUARD_PRESS_INTR_EN       ((u32)0x00000001 << 19)          /*!<R/W 0h  Guard sensor press enable */
#define CT_BIT_OVER_N_NOISE_TH_INTR_EN   ((u32)0x00000001 << 18)          /*!<R/W 0h  Negative noise threshold overflow enable */
#define CT_BIT_AFIFO_OVERFLOW_INTR_EN    ((u32)0x00000001 << 17)          /*!<R/W 0h  Raw code FIFO overflow enable */
#define CT_BIT_OVER_P_NOISE_TH_INTR_EN   ((u32)0x00000001 << 16)          /*!<R/W 0h  Positive noise threshold overflow enable */
#define CT_MASK_TOUCH_RELEASE_INTR_EN    ((u32)0x000000FF << 8)          /*!<R/W 0h  Channelx single touch release enable, x is 0~7 */
#define CT_TOUCH_RELEASE_INTR_EN(x)      ((u32)(((x) & 0x000000FF) << 8))
#define CT_GET_TOUCH_RELEASE_INTR_EN(x)  ((u32)(((x >> 8) & 0x000000FF)))
#define CT_MASK_TOUCH_PRESS_INTR_EN      ((u32)0x000000FF << 0)          /*!<R/W 0h  Channelx single touch press enable, x is 0~7 */
#define CT_TOUCH_PRESS_INTR_EN(x)        ((u32)(((x) & 0x000000FF) << 0))
#define CT_GET_TOUCH_PRESS_INTR_EN(x)    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CT_INTERRUPT_STATUS
 * @brief interrupt status register
 * @{
 **/
#define CT_BIT_AFIFO_OVERLVL_INTR        ((u32)0x00000001 << 21)          /*!<R 0h  Raw code FIFO over level interrupt */
#define CT_BIT_GUARD_RELEASE_INTR        ((u32)0x00000001 << 20)          /*!<R 0h  Guard sensor release interrupt */
#define CT_BIT_GUARD_PRESS_INTR          ((u32)0x00000001 << 19)          /*!<R 0h  Guard sensor press interrupt */
#define CT_BIT_OVER_N_NOISE_TH_INTR      ((u32)0x00000001 << 18)          /*!<R 0h  Negative noise threshold overflow interrupt */
#define CT_BIT_AFIFO_OVERFLOW_INTR       ((u32)0x00000001 << 17)          /*!<R 0h  Raw code FIFO overflow interrupt */
#define CT_BIT_OVER_P_NOISE_TH_INTR      ((u32)0x00000001 << 16)          /*!<R 0h  Positive noise threshold overflow interrupt */
#define CT_MASK_TOUCH_RELEASE_INTR       ((u32)0x000000FF << 8)          /*!<R 0h  Channelx single touch release interrupt, x is 0~7 */
#define CT_TOUCH_RELEASE_INTR(x)         ((u32)(((x) & 0x000000FF) << 8))
#define CT_GET_TOUCH_RELEASE_INTR(x)     ((u32)(((x >> 8) & 0x000000FF)))
#define CT_MASK_TOUCH_PRESS_INTR         ((u32)0x000000FF << 0)          /*!<R 0h  Channelx single touch press interrupt, x is 0~7 */
#define CT_TOUCH_PRESS_INTR(x)           ((u32)(((x) & 0x000000FF) << 0))
#define CT_GET_TOUCH_PRESS_INTR(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CT_RAW_INTERRUPT_STATUS
 * @brief Raw Interrupt Status Register
 * @{
 **/
#define CT_BIT_AFIFO_OVERLVL             ((u32)0x00000001 << 21)          /*!<R 0h  Raw code FIFO over level raw interrupt */
#define CT_BIT_GUARD_RELEASE             ((u32)0x00000001 << 20)          /*!<R 0h  Guard sensor release raw interrupt */
#define CT_BIT_GUARD_PRESS               ((u32)0x00000001 << 19)          /*!<R 0h  Guard sensor press raw interrupt */
#define CT_BIT_OVER_N_NOISE_TH           ((u32)0x00000001 << 18)          /*!<R 0h  Negative noise threshold overflow raw interrupt */
#define CT_BIT_AFIFO_OVERFLOW            ((u32)0x00000001 << 17)          /*!<R 0h  Raw code FIFO overflow raw interrupt */
#define CT_BIT_OVER_P_NOISE_TH           ((u32)0x00000001 << 16)          /*!<R 0h  Positive noise threshold overflow raw interrupt */
#define CT_MASK_TOUCH_RELEASE            ((u32)0x000000FF << 8)          /*!<R 0h  Channelx single touch release raw interrupt, x is 0~7 */
#define CT_TOUCH_RELEASE(x)              ((u32)(((x) & 0x000000FF) << 8))
#define CT_GET_TOUCH_RELEASE(x)          ((u32)(((x >> 8) & 0x000000FF)))
#define CT_MASK_TOUCH_PRESS              ((u32)0x000000FF << 0)          /*!<R 0h  Channelx single touch press raw interrupt, x is 0~7 */
#define CT_TOUCH_PRESS(x)                ((u32)(((x) & 0x000000FF) << 0))
#define CT_GET_TOUCH_PRESS(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CT_INTERRUPT_ALL_CLR
 * @brief Interrupt All Clear Register
 * @{
 **/
#define CT_BIT_INTR_ALL_CLR              ((u32)0x00000001 << 0)          /*!<R/WA0 0h  Write "1" to this register to clear the combined interrupts, all individual interrupts, interrupt status register and raw interrupt status register. */
/** @} */

/** @defgroup CT_INTERRUPT_STATUS_CLR
 * @brief Interrupt Clear Register
 * @{
 **/
#define CT_BIT_AFIFO_OVERLVL_CLR         ((u32)0x00000001 << 21)          /*!<R/WA0 0h  Clear raw code FIFO over level raw interrupt and related register */
#define CT_BIT_GUARD_RELEASE_CLR         ((u32)0x00000001 << 20)          /*!<R/WA0 0h  Clear guard sensor release interrupt and related register */
#define CT_BIT_GUARD_PRESS_CLR           ((u32)0x00000001 << 19)          /*!<R/WA0 0h  Clear guard sensor press interrupt and related register */
#define CT_BIT_OVER_N_NOISE_TH_CLR       ((u32)0x00000001 << 18)          /*!<R/WA0 0h  Clear negative noise threshold overflow interrupt and related register */
#define CT_BIT_AFIFO_OVERFLOW_CLR        ((u32)0x00000001 << 17)          /*!<R/WA0 0h  Clear raw code FIFO overflow interrupt and related register */
#define CT_BIT_OVER_P_NOISE_TH_CLR       ((u32)0x00000001 << 16)          /*!<R/WA0 0h  Clear positive noise threshold overflow interrupt and related register */
#define CT_MASK_TOUCH_RELEASE_CLR        ((u32)0x000000FF << 8)          /*!<R/WA0 0h  Clear channelx single touch release interrupt and related register */
#define CT_TOUCH_RELEASE_CLR(x)          ((u32)(((x) & 0x000000FF) << 8))
#define CT_GET_TOUCH_RELEASE_CLR(x)      ((u32)(((x >> 8) & 0x000000FF)))
#define CT_MASK_TOUCH_PRESS_CLR          ((u32)0x000000FF << 0)          /*!<R/WA0 0h  Clear channelx single touch press interrupt and related register */
#define CT_TOUCH_PRESS_CLR(x)            ((u32)(((x) & 0x000000FF) << 0))
#define CT_GET_TOUCH_PRESS_CLR(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CT_DEBUG_SEL
 * @brief Debug Select Register
 * @{
 **/
#define CT_MASK_DBG_SEL                  ((u32)0x00000007 << 0)          /*!<R/W 0h  Debug information selection 0: dbg_clkrst 1: dbg_apbslv 2: dbg_ctrl 3: dbg_decision 4: dbg_adc_arb Others: dbg_clkrst */
#define CT_DBG_SEL(x)                    ((u32)(((x) & 0x00000007) << 0))
#define CT_GET_DBG_SEL(x)                ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup CT_DEBUG_PORT
 * @brief Debug Register
 * @{
 **/
#define CT_MASK_DBG_PORT                 ((u32)0xFFFFFFFF << 0)          /*!<R 0h  Debug port output value,depend on dbg_sel value */
#define CT_DBG_PORT(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CT_GET_DBG_PORT(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup CT_ECO_USE0
 * @brief ECO USE0 Register
 * @{
 **/
#define CT_MASK_ECO_USE0                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  For ECO use */
#define CT_ECO_USE0(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CT_GET_ECO_USE0(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup CT_ECO_USE1
 * @brief ECO USE1 Register
 * @{
 **/
#define CT_MASK_ECO_USE1                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  For ECO use */
#define CT_ECO_USE1(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CT_GET_ECO_USE1(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup CT_CTC_COMP_VERSION
 * @brief Version Register
 * @{
 **/
#define CT_MASK_COMP_VERSION             ((u32)0xFFFFFFFF << 0)          /*!<R/W 20180123h  CTC version number */
#define CT_COMP_VERSION(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CT_GET_COMP_VERSION(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup CT_CHx_CTRL
 * @brief Channel x Control Register
 * @{
 **/
#define CT_MASK_CHx_D_TOUCH_TH           ((u32)0x00000FFF << 16)          /*!<R/W 64h  Difference threshold data of touch judgement for channel x. It needs to be configured during development, (0x0~0xFFF) (0~4095). Init number=0x1E Recommend data=80%*(signal-baseline) */
#define CT_CHx_D_TOUCH_TH(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define CT_GET_CHx_D_TOUCH_TH(x)         ((u32)(((x >> 16) & 0x00000FFF)))
#define CT_MASK_CHx_BASELINE             ((u32)0x00000FFF << 4)          /*!<R/W/ES 0h  Digital baseline data of channel x. Init number=0x0 It can be initialed by HW when baseline initial function enabled, and can be updated automatically by HW when ETC function enabled. It could be configured by manual tuning. */
#define CT_CHx_BASELINE(x)               ((u32)(((x) & 0x00000FFF) << 4))
#define CT_GET_CHx_BASELINE(x)           ((u32)(((x >> 4) & 0x00000FFF)))
#define CT_BIT_CHx_EN                    ((u32)0x00000001 << 0)          /*!<R/W 0h  Cap sensor activity control of channel x 0: Disable 1: Enable */
/** @} */

/** @defgroup CT_CHx_NOISE_TH
 * @brief Channel x Noise Threshold Register
 * @{
 **/
#define CT_MASK_CHx_N_ENT                ((u32)0x00000FFF << 16)          /*!<R/W 28h  The environmental negative noise threshold: the negative maximum capacitance change of raw data that is still considered an environment change. */
#define CT_CHx_N_ENT(x)                  ((u32)(((x) & 0x00000FFF) << 16))
#define CT_GET_CHx_N_ENT(x)              ((u32)(((x >> 16) & 0x00000FFF)))
#define CT_MASK_CHx_P_ENT                ((u32)0x00000FFF << 0)          /*!<R/W 28h  The environmental positive noise threshold: the positive maximum capacitance change of raw data that is still considered an environment change. */
#define CT_CHx_P_ENT(x)                  ((u32)(((x) & 0x00000FFF) << 0))
#define CT_GET_CHx_P_ENT(x)              ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup CT_CHx_MBIAS_ATH
 * @brief Channel x Mbias
 * @{
 **/
#define CT_MASK_CHx_A_TOUCH_TH           ((u32)0x00000FFF << 16)          /*!<R/W/ES 0h  Absolute threshold data of touch judgement for channel x. It can be updated by HW when baseline initial function or ETC function enabled, a_touch_th=baseline-d_touch_th, manual tuning when ETC disabled, (0x0~0xFFF). Recommended data=80%*signal */
#define CT_CHx_A_TOUCH_TH(x)             ((u32)(((x) & 0x00000FFF) << 16))
#define CT_GET_CHx_A_TOUCH_TH(x)         ((u32)(((x >> 16) & 0x00000FFF)))
#define CT_MASK_CHx_MBIAS                ((u32)0x0000003F << 0)          /*!<R/W 20h  Touch mbias current [5:0] 8u/4u/2u/1u/0.5u/0.25u for channel x */
#define CT_CHx_MBIAS(x)                  ((u32)(((x) & 0x0000003F) << 0))
#define CT_GET_CHx_MBIAS(x)              ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup CT_CHx_DATA_INF
 * @brief Channel x Dtouch Register
 * @{
 **/
#define CT_BIT_CHx_DIFF_DATA_POLARITY    ((u32)0x00000001 << 28)          /*!<R/W/ES 0h  Polarity of chx_diff_data 0: chx_data_ave<baseline 1: chx_data_ave>=baseline */
#define CT_MASK_CHx_DIFF_DATA            ((u32)0x00000FFF << 16)          /*!<R/W/ES 0h  Difference digital data of channelx between chx_data_ave and baseline */
#define CT_CHx_DIFF_DATA(x)              ((u32)(((x) & 0x00000FFF) << 16))
#define CT_GET_CHx_DIFF_DATA(x)          ((u32)(((x >> 16) & 0x00000FFF)))
#define CT_MASK_CHx_DATA_AVE             ((u32)0x00000FFF << 0)          /*!<R/W/ES 0h  Average of channel x raw code */
#define CT_CHx_DATA_AVE(x)               ((u32)(((x) & 0x00000FFF) << 0))
#define CT_GET_CHx_DATA_AVE(x)           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup CT_ANA_ADC_REG0X_LPAD
 * @brief Analog ADC REG0X_LPAD Register
 * @{
 **/
#define CT_MASK_ADC_REG0X_LPAD31_3       ((u32)0x1FFFFFFF << 3)          /*!<R/W 16000254h  Bit[31:3] of analog ADC port REG0X_LPAD. For every bit: [31:30]: VSET[1: 0] 11: 1.89V 10: 1.80V 01: 1.71V 00: 1.62V [29]: sel_ln 1: Comparator low noise for LSB 6 bits [28]: sel_lna 1: Comparator low noise for ALL bits [27:23]: dummy [22]: reg_enTG 1: Enable transmission gate [21:16]: bypassen[5:0], input bypass resistor 1: 1V mode 0: 3.3V mode [15]: reg_diode 1: Enable diode protect [14]: reg_meta 1: Enable meta detect [13: 12]: reg_mpdec<1: 0>, meta detect time 00: short [11]: reg_revckda 1: Rever ckdata [10:8]: reg_vref_sel[2: 0] 000: 0.70V 001: 0.75V 010: 0.80V 011: 0.85V 100: 0.90V 101: 0.95V 110: 1.00V 111: 1.05V [7:5]: reg_ldo_sel[2: 0] 000: 0.70V 001: 0.75V 010: 0.80V 011: 0.85V 100: 0.90V 101: 0.95V 110: 1.00V 111: 1.05V [4]: reg_delay, clk delay 0: short [3]: reg_ck 1: Clock from external */
#define CT_ADC_REG0X_LPAD31_3(x)         ((u32)(((x) & 0x1FFFFFFF) << 3))
#define CT_GET_ADC_REG0X_LPAD31_3(x)     ((u32)(((x >> 3) & 0x1FFFFFFF)))
#define CT_BIT_ADC_REG0X_LPAD2           ((u32)0x00000001 << 2)          /*!<R 0h  Bit[2] of analog ADC port REG0X_LPAD, pow_ref, pow ADC refgen */
#define CT_BIT_ADC_REG0X_LPAD1           ((u32)0x00000001 << 1)          /*!<R 0h  Bit[1] of analog ADC port REG0X_LPAD, pow, pow adc */
#define CT_BIT_ADC_REG0X_LPAD0           ((u32)0x00000001 << 0)          /*!<R 0h  Bit[0] of analog ADC port REG0X_LPAD, power LDO/mbias/res ladder, enable power cut/ldo */
/** @} */

/** @defgroup CT_ANA_ADC_REG1X_LPAD
 * @brief Analog ADC REG1X_LPAD Register
 * @{
 **/
#define CT_MASK_ADC_REG1X_LPAD31_6       ((u32)0x03FFFFFF << 6)          /*!<R/W 0h  [31:8]: Reserved [7:6]: reg_vcmin[1: 0], vcmin voltage set 00: 0.45V 01: 0.44V 10: 0.43V 11: 0.42V */
#define CT_ADC_REG1X_LPAD31_6(x)         ((u32)(((x) & 0x03FFFFFF) << 6))
#define CT_GET_ADC_REG1X_LPAD31_6(x)     ((u32)(((x >> 6) & 0x03FFFFFF)))
/** @} */

/** @defgroup CT_ANA_ADC_REG0X_LPSD
 * @brief Analog ADC REG0X_LPSD Register
 * @{
 **/
#define CT_MASK_ADC_REG0X_LPSD31_14      ((u32)0x0003FFFF << 14)          /*!<R/W 490h  dummy */
#define CT_ADC_REG0X_LPSD31_14(x)        ((u32)(((x) & 0x0003FFFF) << 14))
#define CT_GET_ADC_REG0X_LPSD31_14(x)    ((u32)(((x >> 14) & 0x0003FFFF)))
#define CT_BIT_ADC_REG0X_LPSD13          ((u32)0x00000001 << 13)          /*!<R 0h  POW_I, power on BG current */
#define CT_BIT_ADC_REG0X_LPSD12          ((u32)0x00000001 << 12)          /*!<R 0h  POW_BG, power on BG */
#define CT_MASK_ADC_REG0X_LPSD11_0       ((u32)0x00000FFF << 0)          /*!<R/W 0h  dummy */
#define CT_ADC_REG0X_LPSD11_0(x)         ((u32)(((x) & 0x00000FFF) << 0))
#define CT_GET_ADC_REG0X_LPSD11_0(x)     ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup CT_ANA_ADC_TIME_SET
 * @brief Analog ADC Time Register
 * @{
 **/
#define CT_BIT_ADC_DUMMY_CTRL            ((u32)0x00000001 << 23)          /*!<R/W 1h  1: REG0X_LPAD[2] is sync with REG0X_LPAD[0]. At this time, adc_reg0x_lpad2 can't take effect. 0: the delay of REG0X_LPAD[2] is controlled by adc_dummy_set. At this time, adc_reg0x_lpad2 can take effect. */
#define CT_MASK_ADC_DUMMY_SET            ((u32)0x0000007F << 16)          /*!<R/W 0h  Time between REG0X_LPAD[0] and REG0X_LPAD[2] , unit is ctc clock cycle. This time is valid only when adc_dummy_ctrl is 0. time = (adc_dummy_set+1)*cycle */
#define CT_ADC_DUMMY_SET(x)              ((u32)(((x) & 0x0000007F) << 16))
#define CT_GET_ADC_DUMMY_SET(x)          ((u32)(((x >> 16) & 0x0000007F)))
#define CT_MASK_ADC_PC_SET               ((u32)0x000000FF << 8)          /*!<R/W 16h  Time between REG0X_LPSD12 and REG0X_LPAD[0] power LDO/mbias/res ladder, unit is ctc clock cycle. time = (adc_pc_set+2)*cycle */
#define CT_ADC_PC_SET(x)                 ((u32)(((x) & 0x000000FF) << 8))
#define CT_GET_ADC_PC_SET(x)             ((u32)(((x >> 8) & 0x000000FF)))
#define CT_MASK_ADC_REF_SET              ((u32)0x000000FF << 0)          /*!<R/W 6h  Time between REG0X_LPAD[2] power_ref and REG0X_LPAD[1] pow_adc, unit is ctc clock cycle. time = (adc_ref_set+2)*cycle */
#define CT_ADC_REF_SET(x)                ((u32)(((x) & 0x000000FF) << 0))
#define CT_GET_ADC_REF_SET(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CT_CTC_HW_TestMode
 * @brief Captouch_Hardware_Test_Mode  Register
 * @{
 **/
#define CT_BIT_HW_CLOCK_TIE_VALUE        ((u32)0x00000001 << 1)          /*!<R/W 0h  this value is the clock tied value in captouch hardware test mode */
#define CT_BIT_HW_TMODE_EN               ((u32)0x00000001 << 0)          /*!<R/W 0h  0: hardware test mode disabled 1: hardware test mode enabled, the clock sent to AUX Analog from captouch is tie to fixed value, which is depent on hw_clock_tie_value( 410h[1] ) */
/** @} */

/** @defgroup CT_CTC_DUMMY
 * @brief Captouch Dummy Register
 * @{
 **/
#define CT_MASK_DUMMY1                   ((u32)0x00003FFF << 18)          /*!<R/W 0h  dummy register */
#define CT_DUMMY1(x)                     ((u32)(((x) & 0x00003FFF) << 18))
#define CT_GET_DUMMY1(x)                 ((u32)(((x >> 18) & 0x00003FFF)))
#define CT_MASK_BG_POW_I_CNT             ((u32)0x0000000F << 14)          /*!<R/W 3h  ECO: counter for BG_POW_I , unit is ctc clock cycle. */
#define CT_BG_POW_I_CNT(x)               ((u32)(((x) & 0x0000000F) << 14))
#define CT_GET_BG_POW_I_CNT(x)           ((u32)(((x >> 14) & 0x0000000F)))
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup CAPTOUCH_Exported_Types CAPTOUCH Exported Types
  * @{
*****************************************************************************/

/** @brief CAPTOUCH port
*****************************************************************************/
typedef struct {

	__IO uint32_t CT_CHx_CTRL;                            /*!< CHANNEL x CONTROL REGISTER */
	__IO uint32_t CT_CHx_NOISE_TH;                        /*!< CHANNEL x NOISE THRESHOLD REGISTER */
	__IO uint32_t CT_CHx_MBIAS_ATH;                       /*!< CHANNEL x MBIAS Register */
	__IO uint32_t CT_CHx_DATA_INF;                        /*!< CHANNEL x DTOUCH REGISTER */
} CT_CH_TypeDef;





/**
 * @brief CAPTOUCH Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t CT_CTC_CTRL;                            /*!< CAPTOUCH CONTROL REGISTER,  Address offset: 0x000 */
	__IO uint32_t CT_SCAN_PERIOD;                         /*!< SCAN PARAMETERS REGISTER,  Address offset: 0x004 */
	__IO uint32_t CT_ETC_CTRL;                            /*!< ENVIRONMENT TRACKING CONTROL REGISTER,  Address offset: 0x008 */
	__IO uint32_t CT_SNR_INF;                             /*!< SIGNAL NOISE RATIO REGISTER,  Address offset: 0x00C */
	__IO uint32_t CT_DEBUG_MODE_CTRL;                     /*!< DEBUG AND CHANNEL SWITH MODE REGISTER,  Address offset: 0x010 */
	__IO uint32_t CT_RAW_CODE_FIFO_STATUS;                /*!< FIFO STATUS REGISTER,  Address offset: 0x014 */
	__IO uint32_t CT_RAW_CODE_FIFO_READ;                  /*!< FIFO READ REGISTER,  Address offset: 0x018 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x1C */
	__IO uint32_t CT_INTERRUPT_ENABLE;                    /*!< INTERRUPT ENABLE REGISTER,  Address offset: 0x020 */
	__I  uint32_t CT_INTERRUPT_STATUS;                    /*!< INTERRUPT STATUS REGISTER,  Address offset: 0x024 */
	__I  uint32_t CT_RAW_INTERRUPT_STATUS;                /*!< RAW INTERRUPT STATUS REGISTER,  Address offset: 0x028 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x2C */
	__IO uint32_t CT_INTERRUPT_ALL_CLR;                   /*!< INTERRUPT ALL CLEAR REGISTER,  Address offset: 0x030 */
	__IO uint32_t CT_INTERRUPT_STATUS_CLR;                /*!< INTERRUPT CLEAR REGISTER,  Address offset: 0x034 */
	__IO uint32_t RSVD2[2];                               /*!<  Reserved,  Address offset:0x38-0x3C */
	__IO uint32_t CT_DEBUG_SEL;                           /*!< DEBUG SELECT REGISTER,  Address offset: 0x040 */
	__I  uint32_t CT_DEBUG_PORT;                          /*!< DEBUG REGISTER,  Address offset: 0x044 */
	__IO uint32_t CT_ECO_USE0;                            /*!< ECO USE0 REGISTER,  Address offset: 0x048 */
	__IO uint32_t CT_ECO_USE1;                            /*!< ECO USE1 REGISTER,  Address offset: 0x04C */
	__IO uint32_t CT_CTC_COMP_VERSION;                    /*!< VERSION REGISTER,  Address offset: 0x050 */
	__IO uint32_t RSVD3[43];                              /*!<  Reserved,  Address offset:0x54-0xFC */
	CT_CH_TypeDef CT_CH[9];                               /*!< CAPTOUCH IP have 9 ports,  Address offset: 0x100-0x18C */
	__IO uint32_t RSVD4[156];                              /*!<  Reserved,  Address offset:0x190-0x3FC */
	__IO uint32_t CT_ANA_ADC_REG0X_LPAD;                  /*!< ANALOG ADC REG0X_LPAD REGISTER,  Address offset: 0x400 */
	__IO uint32_t CT_ANA_ADC_REG1X_LPAD;                  /*!< ANALOG ADC REG1X_LPAD REGISTER,  Address offset: 0x404 */
	__IO uint32_t CT_ANA_ADC_REG0X_LPSD;                  /*!< ANALOG ADC REG0X_LPSD REGISTER,  Address offset: 0x408 */
	__IO uint32_t CT_ANA_ADC_TIME_SET;                    /*!< ANALOG ADC TIME REGISTER,  Address offset: 0x40C */
	__IO uint32_t CT_CTC_HW_TESTMODE;                     /*!< CAPTOUCH_HARDWARE_TEST_MODE  REGISTER,  Address offset: 0x410 */
	__IO uint32_t CT_CTC_DUMMY;                           /*!< CAPTOUCH DUMMY REGISTER,  Address offset: 0x414 */
} CAPTOUCH_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup CAPTOUCH_Exported_Types CAPTOUCH Exported Types
  * @{
  */

/**
  * @brief  CapTouch Channel Init Structure Definition
  */
typedef struct {

	u16 CT_DiffThrehold;		/*!< Difference threshold data of touch judgement for channelx :
							1. Configured during development; (0x0~0xFFF) (0~4095)
							2. Init number=0x0, need to be configured
							3. recommend data=80%*(signal-baseline); */

	u8 CT_MbiasCurrent;		/*!< Channelx mbias current tuning(sensitivity tuning).
							Touch bias current BIT[5] ~ BIT[0]: 8uA/4uA/2uA/1uA/0.5uA/0.25uA.
							The sensitivity parameter is used to increase or decrease the strength
							of the sensor signal (difference count). A higher value of sensitivity (bias current)
							setting leads to a stronger signal from the sensors (more difference
							count for the same capacitance change), but also increases the response
							time and average power consumption.*/

	u16 CT_ETCNNoiseThr;		/*!< Specifies negetive noise threshold of ETC.
							This parameter must be set to a value in the 0x1-0xfff range.
							The noise threshold indicates the raw data of the maximum capacitance change
							caused by environmental change. The CTC system provides configurable
							positive noise threshold and negative noise threshold for optimal calibration.
							The recommend value of noise threshold is 40%* touch threshold. Users need
							tune the two noise thresholds for different applications and noise environment.*/

	u16 CT_ETCPNoiseThr;		/*!< Specifies positive threshold of ETC.
							This parameter must be set to a value in the 0x1-0xfff range. */

	u8 CT_CHEnable;			/*!< Specifies this channel is enable or not */
} CapTouch_CHInitTypeDef;

/**
  * @brief  CapTouch Init Structure Definition
  */
typedef struct {
	u32 CT_DebounceEn;			/*!< Specifies CapTouch press event Debounce Enable.
							This parameter must be set to a value in the 0x0-1 range. The de-bounce
							parameter can be configured by register CTC_CTRL.
							For example, when de-bounce is enabled and configured as 00 (2 times scan),
							finger touch interrupt will not be sent to the host until 2 times continue
							finger touch event is detected. Sensor de-bounce function is suitable for
							both button application and proximity detection.*/

	u32 CT_SampleCnt;			/*!< Specifies sample cnt for average function,sample cnt = 2*exp(CT_SampleCnt+2).
							This parameter can be a value of 0x0-0x7*/

	u32 CT_ScanInterval;			/*!< Specifies scan interval of every key.
							This parameter must be set to a value in the 0x1-0xfff range. */

	u32 CT_ETCStep;				/*!< Specifies baseline update setp of ETC.
							This parameter must be set to a value in the 0x0-0xfff range. */

	u32 CT_ETCFactor;			/*!< Specifies CapTouch ETC Factor.
							This parameter must be set to a value in the 0x0-0xf range. */

	u32 CT_ETCScanInterval;		/*!< Specifies ETC scan interval
							This parameter can be set to a value in the 0x1-0x7f range*/

	CapTouch_CHInitTypeDef CT_Channel[9];			/*!< Specifies the initialization parameters for each channel */

} CapTouch_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup CAPTOUCH_Exported_Constants CAPTOUCH Exported Constants
  * @{
  */

/** @defgroup CapTouch_Peripheral_Definitions
  * @{
  */
#define IS_CAPTOUCH_ALL_PERIPH(PERIPH) ((PERIPH) == CAPTOUCH_DEV)
/**
  * @}
  */

/** @defgroup CapTouch_INT_Operation
  * @{
  */
#define  CT_CHX_PRESS_INT(x)			((u32)0x00000001 << (x))
#define  CT_CHX_RELEASE_INT(x)			((u32)0x00000001 << ((x) + 8))

#define CT_ALL_INT_EN					(CT_MASK_TOUCH_PRESS_INTR_EN | \
											CT_MASK_TOUCH_RELEASE_INTR_EN | \
											CT_BIT_OVER_P_NOISE_TH_INTR_EN |\
											CT_BIT_AFIFO_OVERFLOW_INTR_EN |\
											CT_BIT_OVER_N_NOISE_TH_INTR_EN |\
											CT_BIT_GUARD_PRESS_INTR_EN |\
											CT_BIT_GUARD_RELEASE_INTR_EN)

#define IS_CT_INT_EN(IT)				(((IT) & ~CT_ALL_INT_EN) == 0)
#define IS_CT_INT_CLR(IT)				(((IT) & ~CT_ALL_INT_EN) == 0)
/**
  * @}
  */

/** @defgroup CapTouch_Channel_Number
  * @{
  */
#define  CT_CHANNEL_NUM					9
#define IS_CT_CHANNEL(CHANNEL_NUM)		(CHANNEL_NUM < CT_CHANNEL_NUM)
/**
  * @}
  */

/** @defgroup CapTouch_Noise_Threshold_Type
  * @{
  */
#define  P_NOISE_THRES					0
#define  N_NOISE_THRES					1
/**
  * @}
  */

/** @defgroup CapTouch_Data_Setting
  * @{
  */
#define CTC_CH_ID(x)				((u32)((x >> 12) & 0x0000000F))
#define CTC_CH_DATA(x)				((u32)((x) & 0x00000FFF))
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup CAPTOUCH_Exported_Functions CAPTOUCH Exported Functions
  * @{
  */

/** @defgroup CAPTOUCH_Exported_Normal_Functions CAPTOUCH Normal Functions
  * @{
  */
void CapTouch_StructInit(CapTouch_InitTypeDef *CapTouch_InitStruct);
void CapTouch_Init(CAPTOUCH_TypeDef *CapTouch, CapTouch_InitTypeDef *CapTouch_InitStruct);
void CapTouch_Cmd(CAPTOUCH_TypeDef *CapTouch, u8 NewState);
void CapTouch_INTConfig(CAPTOUCH_TypeDef *CapTouch, uint32_t CapTouch_IT, u8 newState);
void CapTouch_INTClearPendingBit(CAPTOUCH_TypeDef *CapTouch, u32 CapTouch_IT);
u32 CapTouch_GetRawISR(CAPTOUCH_TypeDef *CapTouch);
u32 CapTouch_GetISR(CAPTOUCH_TypeDef *CapTouch);

void CapTouch_SetScanInterval(CAPTOUCH_TypeDef *CapTouch, u32 Interval);
void CapTouch_ChCmd(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u8 NewState);
u32 CapTouch_GetChStatus(CAPTOUCH_TypeDef *CapTouch, u32 Channel);
void CapTouch_SetChDiffThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u32 Threshold);
void CapTouch_SetChMbias(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u8 Mbias);
u32 CapTouch_GetChDiffThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel);
u32 CapTouch_GetChAbsThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel);
u32 CapTouch_GetNoiseThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u8 type);
void CapTouch_SetNNoiseThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u16 Threshold);
void CapTouch_SetPNoiseThres(CAPTOUCH_TypeDef *CapTouch, u8 Channel, u16 Threshold);
u32 CapTouch_GetChBaseline(CAPTOUCH_TypeDef *CapTouch, u8 Channel);
u32 CapTouch_GetChAveData(CAPTOUCH_TypeDef *CapTouch, u8 Channel);
/**
  * @}
  */

/** @defgroup CAPTOUCH_Exported_Debug_Functions CAPTOUCH Debug Functions
  * @{
  */
void CapTouch_DbgCmd(CAPTOUCH_TypeDef *CapTouch, u8 NewState);
void CapTouch_DbgDumpReg(CAPTOUCH_TypeDef *CapTouch);
void CapTouch_DbgDumpETC(CAPTOUCH_TypeDef *CapTouch, u32 ch);
u32 CapTouch_DbgRawData(CAPTOUCH_TypeDef *CapTouch);
/**
  * @}
  */

/**
  * @}
  */

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
