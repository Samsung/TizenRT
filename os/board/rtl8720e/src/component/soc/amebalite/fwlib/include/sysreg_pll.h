#ifndef _AMEBAD2_PLL_H_
#define _AMEBAD2_PLL_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PLL
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PLL_Register_Definitions PLL Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup PLL_CPUPLL_CTRL0
 * @brief
 * @{
 **/
#define PLL_BIT_CPUPLL_POW_ERC                ((u32)0x00000001 << 31)          /*!<R/W 0  erc enable. 0 → 1 */
#define PLL_BIT_CPUPLL_POW_DIV_IN             ((u32)0x00000001 << 30)          /*!<R/W 0  input divider enable (need pwl signal) 0 → 1 */
#define PLL_BIT_CPUPLL_POW_PLL                ((u32)0x00000001 << 29)          /*!<R/W 0  (need pwl signal) 0 → 1 */
#define PLL_MASK_CPUPLL_CP_BIA                ((u32)0x00000007 << 26)          /*!<R/W 000  charge pump current selection . Default=10uA CP current select : 000: Icp=10uA, 001: Icp=15uA, 010: Icp=20uA, 011: Icp=25uA; 100: Icp=30uA, 101: Icp=35uA, 110: Icp=40uA, 111: Icp=450uA; */
#define PLL_CPUPLL_CP_BIA(x)                  ((u32)(((x) & 0x00000007) << 26))
#define PLL_GET_CPUPLL_CP_BIA(x)              ((u32)(((x >> 26) & 0x00000007)))
#define PLL_BIT_CPUPLL_DOGENB                 ((u32)0x00000001 << 25)          /*!<R/W 0  0: watch dog on, 1: watch dog off */
#define PLL_MASK_CPUPLL_PS                    ((u32)0x00000007 << 22)          /*!<R/W 000   */
#define PLL_CPUPLL_PS(x)                      ((u32)(((x) & 0x00000007) << 22))
#define PLL_GET_CPUPLL_PS(x)                  ((u32)(((x >> 22) & 0x00000007)))
#define PLL_BIT_CPUPLL_PSEN                   ((u32)0x00000001 << 21)          /*!<R/W 1  phase selector enable */
#define PLL_MASK_CPUPLL_CP_SET                ((u32)0x00000003 << 19)          /*!<R/W 10  Cp selection. Default=6p 00: Cp=4p, 01: Cp=5p, 10: Cp=6p, 11: Cp=7p 40M = 6p(10), 26M = 6p(10) */
#define PLL_CPUPLL_CP_SET(x)                  ((u32)(((x) & 0x00000003) << 19))
#define PLL_GET_CPUPLL_CP_SET(x)              ((u32)(((x >> 19) & 0x00000003)))
#define PLL_MASK_CPUPLL_RS_SET                ((u32)0x00000007 << 16)          /*!<R/W 110  Rs selection. Default=5k 000: Rs=15k, 001: Rs=13k, 010: Rs=11k, 011: Rs=9k, 100: Rs=7k, 101: Rs=5k, 110: Rs=3k, 111: Rs=1k; 40M = 5k(101), 26M = 5k(101) */
#define PLL_CPUPLL_RS_SET(x)                  ((u32)(((x) & 0x00000007) << 16))
#define PLL_GET_CPUPLL_RS_SET(x)              ((u32)(((x >> 16) & 0x00000007)))
#define PLL_MASK_CPUPLL_CS_SEL                ((u32)0x00000003 << 14)          /*!<R/W 10  Cs selection. Default=60p 00: Cs=40p, 01: Cs=50p, 10: Cs=60p, 11: Cs=70p 40M = 60p(10), 26M = 60p(10) */
#define PLL_CPUPLL_CS_SEL(x)                  ((u32)(((x) & 0x00000003) << 14))
#define PLL_GET_CPUPLL_CS_SEL(x)              ((u32)(((x >> 14) & 0x00000003)))
#define PLL_BIT_CPUPLL_FREF_EDGE              ((u32)0x00000001 << 13)          /*!<R/W 1  0:fref, 1:frefb */
#define PLL_BIT_CPUPLL_CK_EN                  ((u32)0x00000001 << 12)          /*!<R/W 1  output clk enable */
#define PLL_BIT_CPUPLL_CK_EN_D4               ((u32)0x00000001 << 11)          /*!<R/W 0  output clk div 4 enable */
#define PLL_MASK_CPUPLL_DUMMY0                ((u32)0x00000007 << 8)          /*!<R/W 000   */
#define PLL_CPUPLL_DUMMY0(x)                  ((u32)(((x) & 0x00000007) << 8))
#define PLL_GET_CPUPLL_DUMMY0(x)              ((u32)(((x >> 8) & 0x00000007)))
#define PLL_BIT_CPUPLL_XTAL_MANUAL_MODE       ((u32)0x00000001 << 7)          /*!<R/W 1  reg_xtal_maunal_mode. Default=0 0: reg_xtal_sel<3:0> is controlled by REG, xtal_div_sel is controlled by mapping table. 1: reg_xtal_sel<3:0>=4'b1111, xtal_div_sel is controlled by reg_xtal_div_sel_maunal<1:0> */
#define PLL_MASK_CPUPLL_XTAL_DIV_SEL_MANUAL   ((u32)0x00000003 << 5)          /*!<R/W 00  input divider selection in xtal maunal mode. Default=00 00=/1, 10=/2, 11=/4 */
#define PLL_CPUPLL_XTAL_DIV_SEL_MANUAL(x)     ((u32)(((x) & 0x00000003) << 5))
#define PLL_GET_CPUPLL_XTAL_DIV_SEL_MANUAL(x) ((u32)(((x >> 5) & 0x00000003)))
#define PLL_MASK_CPUPLL_VC_THL                ((u32)0x00000003 << 3)          /*!<R/W 00  VC voltage, Default=00 00: VC=1/4Vdd, 01: VC=1/2Vdd, 10=3/4Vdd, 11: VC=Vdd */
#define PLL_CPUPLL_VC_THL(x)                  ((u32)(((x) & 0x00000003) << 3))
#define PLL_GET_CPUPLL_VC_THL(x)              ((u32)(((x >> 3) & 0x00000003)))
#define PLL_MASK_CPUPLL_C3_SET                ((u32)0x00000003 << 1)          /*!<R/W 01  C3 selection. Default=1p 00: 0.5p, 01: 1p, 10: Cs=1.5p, 11: 2p */
#define PLL_CPUPLL_C3_SET(x)                  ((u32)(((x) & 0x00000003) << 1))
#define PLL_GET_CPUPLL_C3_SET(x)              ((u32)(((x >> 1) & 0x00000003)))
#define PLL_BIT_CPUPLL_R3_SET                 ((u32)0x00000001 << 0)          /*!<R/W 1  Rs selection. Default=1k 0: 0 Ohm, 1: 1k Ohm */
/** @} */

/** @defgroup PLL_CPUPLL_CTRL1
 * @brief
 * @{
 **/
#define PLL_MASK_CPUPLL_XTAL_SEL              ((u32)0x0000000F << 28)          /*!<R/W 0000  xtal selection, Default=40MHz 0000= 40MHz, 0001= 25MHz, 0010= 13MHz, 0011= 19.2MHz 0100= 20MHz, 0101= 26MHz, 0110= 38.4MHz, 0111= 17.664MHz 1000= 16MHz, 1001= 14.138MHz, 1010= 12MHz, 1011= 52MHz 1100= 48MHz, 1101= 27MHz, 1110= 24MHz, 1111= none */
#define PLL_CPUPLL_XTAL_SEL(x)                ((u32)(((x) & 0x0000000F) << 28))
#define PLL_GET_CPUPLL_XTAL_SEL(x)            ((u32)(((x >> 28) & 0x0000000F)))
#define PLL_MASK_CPUPLL_FREQ_SEL              ((u32)0x0000000F << 24)          /*!<R/W 0000  output clk selection, Default=400MHz 0000= 400MHz, 0001= X, 0010= X, 0011= X, 0100= 216MHz, 0101=196.608MHz, 0110=192MHz, 0111= 180.632MHz, 1000= 148.5MHz,1001= X, 1010= X, 1011= X, 1100= X, 1101= X, 1110= X, 1111= 500MHz */
#define PLL_CPUPLL_FREQ_SEL(x)                ((u32)(((x) & 0x0000000F) << 24))
#define PLL_GET_CPUPLL_FREQ_SEL(x)            ((u32)(((x >> 24) & 0x0000000F)))
#define PLL_MASK_CPUPLL_TBASE_FREQ            ((u32)0x0000000F << 20)          /*!<R/W 1000  now is empty */
#define PLL_CPUPLL_TBASE_FREQ(x)              ((u32)(((x) & 0x0000000F) << 20))
#define PLL_GET_CPUPLL_TBASE_FREQ(x)          ((u32)(((x >> 20) & 0x0000000F)))
#define PLL_MASK_CPUPLL_STEP_FREQ             ((u32)0x0000000F << 16)          /*!<R/W 0101  step size selection for switching freq. 0000: 2^5 0001: 2^6 0010: 2^7 0011: 2^8 0100: 2^9 0101: 2^10 0110: 2^11 0111: 2^12 */
#define PLL_CPUPLL_STEP_FREQ(x)               ((u32)(((x) & 0x0000000F) << 16))
#define PLL_GET_CPUPLL_STEP_FREQ(x)           ((u32)(((x >> 16) & 0x0000000F)))
#define PLL_BIT_CPUPLL_TRIG_FREQ              ((u32)0x00000001 << 15)          /*!<R/W 0  freq. step up or step down enable */
#define PLL_MASK_CPUPLL_BB_DBG_SEL_AFE_SDM    ((u32)0x0000000F << 11)          /*!<R/W 0000   */
#define PLL_CPUPLL_BB_DBG_SEL_AFE_SDM(x)      ((u32)(((x) & 0x0000000F) << 11))
#define PLL_GET_CPUPLL_BB_DBG_SEL_AFE_SDM(x)  ((u32)(((x >> 11) & 0x0000000F)))
#define PLL_MASK_CPUPLL_DIVN_SDM              ((u32)0x0000003F << 5)          /*!<R/W 6'b1101  393.216M (98.304M / 24.576M) 40M: 000111 26M: 001101 */
#define PLL_CPUPLL_DIVN_SDM(x)                ((u32)(((x) & 0x0000003F) << 5))
#define PLL_GET_CPUPLL_DIVN_SDM(x)            ((u32)(((x >> 5) & 0x0000003F)))
#define PLL_MASK_CPUPLL_CK_OUT_SEL            ((u32)0x0000000F << 1)          /*!<R/W 0000  output divider selection. Default=1110(/16) 0000=/1, 1000=/2, 1100=/8, 1110=/16, 1111=/32 */
#define PLL_CPUPLL_CK_OUT_SEL(x)              ((u32)(((x) & 0x0000000F) << 1))
#define PLL_GET_CPUPLL_CK_OUT_SEL(x)          ((u32)(((x >> 1) & 0x0000000F)))
#define PLL_BIT_CPUPLL_ICP_HALF               ((u32)0x00000001 << 0)          /*!<R/W 1  charge pump current half or not 0: no half 1: half */
/** @} */

/** @defgroup PLL_CPUPLL_CTRL2
 * @brief
 * @{
 **/
#define PLL_MASK_CPUPLL_STEP                  ((u32)0x00001FFF << 19)          /*!<R/W 0  ssc step size selection. */
#define PLL_CPUPLL_STEP(x)                    ((u32)(((x) & 0x00001FFF) << 19))
#define PLL_GET_CPUPLL_STEP(x)                ((u32)(((x >> 19) & 0x00001FFF)))
#define PLL_MASK_CPUPLL_TBASE                 ((u32)0x00000FFF << 7)          /*!<R/W 0  ssc freq selection */
#define PLL_CPUPLL_TBASE(x)                   ((u32)(((x) & 0x00000FFF) << 7))
#define PLL_GET_CPUPLL_TBASE(x)               ((u32)(((x >> 7) & 0x00000FFF)))
#define PLL_BIT_CPUPLL_EN_SSC                 ((u32)0x00000001 << 6)          /*!<R/W 0  ssc enable. Default=0 */
#define PLL_MASK_CPUPLL_DUMMY1                ((u32)0x0000003F << 0)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_CPUPLL_DUMMY1(x)                  ((u32)(((x) & 0x0000003F) << 0))
#define PLL_GET_CPUPLL_DUMMY1(x)              ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup PLL_CPUPLL_CTRL3
 * @brief
 * @{
 **/
#define PLL_MASK_CPUPLL_F0F_SDM               ((u32)0x00001FFF << 19)          /*!<R/W 0  393.216M (98.304M / 24.576M) 40M: 1 0100 1001 0101 26M: 1 1111 1010 1010 */
#define PLL_CPUPLL_F0F_SDM(x)                 ((u32)(((x) & 0x00001FFF) << 19))
#define PLL_GET_CPUPLL_F0F_SDM(x)             ((u32)(((x >> 19) & 0x00001FFF)))
#define PLL_MASK_CPUPLL_F0N_SDM               ((u32)0x00000007 << 16)          /*!<R/W 0  393.216M (98.304M / 24.576M) 40M: 110 26M: 000 */
#define PLL_CPUPLL_F0N_SDM(x)                 ((u32)(((x) & 0x00000007) << 16))
#define PLL_GET_CPUPLL_F0N_SDM(x)             ((u32)(((x >> 16) & 0x00000007)))
#define PLL_BIT_CPUPLL_ORDER_SDM              ((u32)0x00000001 << 15)          /*!<R/W 0  SDM order */
#define PLL_MASK_CPUPLL_DUMMY2                ((u32)0x00007FFF << 0)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_CPUPLL_DUMMY2(x)                  ((u32)(((x) & 0x00007FFF) << 0))
#define PLL_GET_CPUPLL_DUMMY2(x)              ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/** @defgroup PLL_PERIPLL_CTRL0
 * @brief
 * @{
 **/
#define PLL_BIT_PERIPLL_POW_ERC               ((u32)0x00000001 << 31)          /*!<R/W 0  erc enable. 0 → 1 */
#define PLL_BIT_PERIPLL_POW_DIV_IN            ((u32)0x00000001 << 30)          /*!<R/W 0  input divider enable (need pwl signal) 0 → 1 */
#define PLL_BIT_PERIPLL_POW_PLL               ((u32)0x00000001 << 29)          /*!<R/W 0  (need pwl signal) 0 → 1 */
#define PLL_MASK_PERIPLL_CP_BIA               ((u32)0x00000007 << 26)          /*!<R/W 000  charge pump current selection . Default=10uA CP current select : 000: Icp=10uA, 001: Icp=15uA, 010: Icp=20uA, 011: Icp=25uA; 100: Icp=30uA, 101: Icp=35uA, 110: Icp=40uA, 111: Icp=450uA; */
#define PLL_PERIPLL_CP_BIA(x)                 ((u32)(((x) & 0x00000007) << 26))
#define PLL_GET_PERIPLL_CP_BIA(x)             ((u32)(((x >> 26) & 0x00000007)))
#define PLL_BIT_PERIPLL_DOGENB                ((u32)0x00000001 << 25)          /*!<R/W 0  0: watch dog on, 1: watch dog off */
#define PLL_MASK_PERIPLL_PS                   ((u32)0x00000007 << 22)          /*!<R/W 000   */
#define PLL_PERIPLL_PS(x)                     ((u32)(((x) & 0x00000007) << 22))
#define PLL_GET_PERIPLL_PS(x)                 ((u32)(((x >> 22) & 0x00000007)))
#define PLL_BIT_PERIPLL_PSEN                  ((u32)0x00000001 << 21)          /*!<R/W 1  phase selector enable */
#define PLL_MASK_PERIPLL_CP_SET               ((u32)0x00000003 << 19)          /*!<R/W 10  Cp selection. Default=6p 00: Cp=4p, 01: Cp=5p, 10: Cp=6p, 11: Cp=7p 40M = 6p(10), 26M = 6p(10) */
#define PLL_PERIPLL_CP_SET(x)                 ((u32)(((x) & 0x00000003) << 19))
#define PLL_GET_PERIPLL_CP_SET(x)             ((u32)(((x >> 19) & 0x00000003)))
#define PLL_MASK_PERIPLL_RS_SET               ((u32)0x00000007 << 16)          /*!<R/W 110  Rs selection. Default=5k 000: Rs=15k, 001: Rs=13k, 010: Rs=11k, 011: Rs=9k, 100: Rs=7k, 101: Rs=5k, 110: Rs=3k, 111: Rs=1k; 40M = 3k(110), 26M = 3k(110) */
#define PLL_PERIPLL_RS_SET(x)                 ((u32)(((x) & 0x00000007) << 16))
#define PLL_GET_PERIPLL_RS_SET(x)             ((u32)(((x >> 16) & 0x00000007)))
#define PLL_MASK_PERIPLL_CS_SEL               ((u32)0x00000003 << 14)          /*!<R/W 10  Cs selection. Default=60p 00: Cs=40p, 01: Cs=50p, 10: Cs=60p, 11: Cs=70p 40M = 60p(10), 26M = 60p(10) */
#define PLL_PERIPLL_CS_SEL(x)                 ((u32)(((x) & 0x00000003) << 14))
#define PLL_GET_PERIPLL_CS_SEL(x)             ((u32)(((x >> 14) & 0x00000003)))
#define PLL_BIT_PERIPLL_FREF_EDGE             ((u32)0x00000001 << 13)          /*!<R/W 1  0:fref, 1:frefb */
#define PLL_BIT_PERIPLL_CK_EN                 ((u32)0x00000001 << 12)          /*!<R/W 1  output clk enable */
#define PLL_BIT_PERIPLL_CK_EN_D4              ((u32)0x00000001 << 11)          /*!<R/W 0  output clk div 4 enable */
#define PLL_MASK_PERIPLL_DUMMY0               ((u32)0x00000007 << 8)          /*!<R/W 000   */
#define PLL_PERIPLL_DUMMY0(x)                 ((u32)(((x) & 0x00000007) << 8))
#define PLL_GET_PERIPLL_DUMMY0(x)             ((u32)(((x >> 8) & 0x00000007)))
#define PLL_BIT_PERIPLL_XTAL_MANUAL_MODE      ((u32)0x00000001 << 7)          /*!<R/W 1  reg_xtal_maunal_mode. Default=0 0: reg_xtal_sel<3:0> is controlled by REG, xtal_div_sel is controlled by mapping table. 1: reg_xtal_sel<3:0>=4'b1111, xtal_div_sel is controlled by reg_xtal_div_sel_maunal<1:0> */
#define PLL_MASK_PERIPLL_XTAL_DIV_SEL_MANUAL  ((u32)0x00000003 << 5)          /*!<R/W 00  input divider selection in xtal maunal mode. Default=00 00=/1, 10=/2, 11=/4 */
#define PLL_PERIPLL_XTAL_DIV_SEL_MANUAL(x)    ((u32)(((x) & 0x00000003) << 5))
#define PLL_GET_PERIPLL_XTAL_DIV_SEL_MANUAL(x) ((u32)(((x >> 5) & 0x00000003)))
#define PLL_MASK_PERIPLL_VC_THL               ((u32)0x00000003 << 3)          /*!<R/W 00  VC voltage, Default=00 00: VC=1/4Vdd, 01: VC=1/2Vdd, 10=3/4Vdd, 11: VC=Vdd */
#define PLL_PERIPLL_VC_THL(x)                 ((u32)(((x) & 0x00000003) << 3))
#define PLL_GET_PERIPLL_VC_THL(x)             ((u32)(((x >> 3) & 0x00000003)))
#define PLL_MASK_PERIPLL_C3_SET               ((u32)0x00000003 << 1)          /*!<R/W 01  C3 selection. Default=1p 00: 0.5p, 01: 1p, 10: Cs=1.5p, 11: 2p */
#define PLL_PERIPLL_C3_SET(x)                 ((u32)(((x) & 0x00000003) << 1))
#define PLL_GET_PERIPLL_C3_SET(x)             ((u32)(((x >> 1) & 0x00000003)))
#define PLL_BIT_PERIPLL_R3_SET                ((u32)0x00000001 << 0)          /*!<R/W 1  Rs selection. Default=1k 0: 0 Ohm, 1: 1k Ohm */
/** @} */

/** @defgroup PLL_PERIPLL_CTRL1
 * @brief
 * @{
 **/
#define PLL_MASK_PERIPLL_XTAL_SEL             ((u32)0x0000000F << 28)          /*!<R/W 0000  xtal selection, Default=40MHz 0000= 40MHz, 0001= 25MHz, 0010= 13MHz, 0011= 19.2MHz 0100= 20MHz, 0101= 26MHz, 0110= 38.4MHz, 0111= 17.664MHz 1000= 16MHz, 1001= 14.138MHz, 1010= 12MHz, 1011= 52MHz 1100= 48MHz, 1101= 27MHz, 1110= 24MHz, 1111= none */
#define PLL_PERIPLL_XTAL_SEL(x)               ((u32)(((x) & 0x0000000F) << 28))
#define PLL_GET_PERIPLL_XTAL_SEL(x)           ((u32)(((x >> 28) & 0x0000000F)))
#define PLL_MASK_PERIPLL_FREQ_SEL             ((u32)0x0000000F << 24)          /*!<R/W 0000  output clk selection, Default=400MHz 0000= 400MHz, 0001= X, 0010= X, 0011= X, 0100= 216MHz, 0101=196.608MHz, 0110=192MHz, 0111= 180.632MHz, 1000= 148.5MHz,1001= X, 1010= X, 1011= X, 1100= X, 1101= X, 1110= X, 1111= 500MHz */
#define PLL_PERIPLL_FREQ_SEL(x)               ((u32)(((x) & 0x0000000F) << 24))
#define PLL_GET_PERIPLL_FREQ_SEL(x)           ((u32)(((x >> 24) & 0x0000000F)))
#define PLL_MASK_PERIPLL_TBASE_FREQ           ((u32)0x0000000F << 20)          /*!<R/W 1000  now is empty */
#define PLL_PERIPLL_TBASE_FREQ(x)             ((u32)(((x) & 0x0000000F) << 20))
#define PLL_GET_PERIPLL_TBASE_FREQ(x)         ((u32)(((x >> 20) & 0x0000000F)))
#define PLL_MASK_PERIPLL_STEP_FREQ            ((u32)0x0000000F << 16)          /*!<R/W 0101  step size selection for switching freq. 0000: 2^5 0001: 2^6 0010: 2^7 0011: 2^8 0100: 2^9 0101: 2^10 0110: 2^11 0111: 2^12 */
#define PLL_PERIPLL_STEP_FREQ(x)              ((u32)(((x) & 0x0000000F) << 16))
#define PLL_GET_PERIPLL_STEP_FREQ(x)          ((u32)(((x >> 16) & 0x0000000F)))
#define PLL_BIT_PERIPLL_TRIG_FREQ             ((u32)0x00000001 << 15)          /*!<R/W 0  freq. step up or step down enable */
#define PLL_MASK_PERIPLL_BB_DBG_SEL_AFE_SDM   ((u32)0x0000000F << 11)          /*!<R/W 0   */
#define PLL_PERIPLL_BB_DBG_SEL_AFE_SDM(x)     ((u32)(((x) & 0x0000000F) << 11))
#define PLL_GET_PERIPLL_BB_DBG_SEL_AFE_SDM(x) ((u32)(((x >> 11) & 0x0000000F)))
#define PLL_MASK_PERIPLL_DIVN_SDM             ((u32)0x0000003F << 5)          /*!<R/W 6'b1000  1000M */
#define PLL_PERIPLL_DIVN_SDM(x)               ((u32)(((x) & 0x0000003F) << 5))
#define PLL_GET_PERIPLL_DIVN_SDM(x)           ((u32)(((x >> 5) & 0x0000003F)))
#define PLL_MASK_PERIPLL_CK_OUT_SEL           ((u32)0x0000000F << 1)          /*!<R/W 0000  output divider selection. Default=0000(/1) 0000=/1, 1000=/2, 1100=/8, 1110=/16, 1111=/32 */
#define PLL_PERIPLL_CK_OUT_SEL(x)             ((u32)(((x) & 0x0000000F) << 1))
#define PLL_GET_PERIPLL_CK_OUT_SEL(x)         ((u32)(((x >> 1) & 0x0000000F)))
#define PLL_BIT_PERIPLL_ICP_HALF              ((u32)0x00000001 << 0)          /*!<R/W 0  charge pump current half or not 0: no half 1: half */
/** @} */

/** @defgroup PLL_PERIPLL_CTRL2
 * @brief
 * @{
 **/
#define PLL_MASK_PERIPLL_STEP                 ((u32)0x00001FFF << 19)          /*!<R/W 0000000000000  ssc step size selection. */
#define PLL_PERIPLL_STEP(x)                   ((u32)(((x) & 0x00001FFF) << 19))
#define PLL_GET_PERIPLL_STEP(x)               ((u32)(((x >> 19) & 0x00001FFF)))
#define PLL_MASK_PERIPLL_TBASE                ((u32)0x00000FFF << 7)          /*!<R/W 000000000000  ssc freq selection */
#define PLL_PERIPLL_TBASE(x)                  ((u32)(((x) & 0x00000FFF) << 7))
#define PLL_GET_PERIPLL_TBASE(x)              ((u32)(((x >> 7) & 0x00000FFF)))
#define PLL_BIT_PERIPLL_EN_SSC                ((u32)0x00000001 << 6)          /*!<R/W 0  ssc enable. Default=0 */
#define PLL_MASK_PERIPLL_DUMMY1               ((u32)0x0000003F << 0)          /*!<R/W 000000  HW used only , these are for reserved */
#define PLL_PERIPLL_DUMMY1(x)                 ((u32)(((x) & 0x0000003F) << 0))
#define PLL_GET_PERIPLL_DUMMY1(x)             ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup PLL_PERIPLL_CTRL3
 * @brief
 * @{
 **/
#define PLL_MASK_PERIPLL_F0F_SDM              ((u32)0x00001FFF << 19)          /*!<R/W 0000000000000  600M 40M: 0 0000 0000 0000 26M: 1 0011 1011 0001 */
#define PLL_PERIPLL_F0F_SDM(x)                ((u32)(((x) & 0x00001FFF) << 19))
#define PLL_GET_PERIPLL_F0F_SDM(x)            ((u32)(((x >> 19) & 0x00001FFF)))
#define PLL_MASK_PERIPLL_F0N_SDM              ((u32)0x00000007 << 16)          /*!<R/W 000  600M 40M: 000 26M: 000 */
#define PLL_PERIPLL_F0N_SDM(x)                ((u32)(((x) & 0x00000007) << 16))
#define PLL_GET_PERIPLL_F0N_SDM(x)            ((u32)(((x >> 16) & 0x00000007)))
#define PLL_BIT_PERIPLL_ORDER_SDM             ((u32)0x00000001 << 15)          /*!<R/W 0  SDM order */
#define PLL_MASK_PERIPLL_DUMMY2               ((u32)0x00007FFF << 0)          /*!<R/W 000000000000000  HW used only , these are for reserved */
#define PLL_PERIPLL_DUMMY2(x)                 ((u32)(((x) & 0x00007FFF) << 0))
#define PLL_GET_PERIPLL_DUMMY2(x)             ((u32)(((x >> 0) & 0x00007FFF)))
/** @} */

/** @defgroup PLL_TEST
 * @brief
 * @{
 **/
#define PLL_BIT_POW_ERC_TEST                  ((u32)0x00000001 << 15)          /*!<R/W 1  1 for enable test, 0 for disable */
#define PLL_MASK_TEST_DUMMY                   ((u32)0x00000003 << 13)          /*!<R/W 00  dummy */
#define PLL_TEST_DUMMY(x)                     ((u32)(((x) & 0x00000003) << 13))
#define PLL_GET_TEST_DUMMY(x)                 ((u32)(((x >> 13) & 0x00000003)))
#define PLL_BIT_CKMON_SEL                     ((u32)0x00000001 << 12)          /*!<R/W 1  0 for CKMON, 1 for CKMOND div by 4 */
#define PLL_BIT_POW_DIV4                      ((u32)0x00000001 << 11)          /*!<R/W 1  1 for ckmon div 4 , 0 for disable div 4 */
#define PLL_BIT_ISO                           ((u32)0x00000001 << 10)          /*!<R/W 0  1 for ISO ck_fref/ck_frefb */
#define PLL_BIT_REF_CLK_SEL                   ((u32)0x00000001 << 9)          /*!<R/W 0  xtal clk from 0 special route 1 APR */
#define PLL_BIT_CK_MON_EN                     ((u32)0x00000001 << 8)          /*!<R/W 0  ckmon enable */
#define PLL_MASK_CK_SEL                       ((u32)0x0000000F << 4)          /*!<R/W 0000  ckmon clk sel 1111:TL 1110:TL 1101:CK_400M_CPU_I 1100:CK_CPUD4_I 1011:CK_500M_PERI_I 1010:CK_PERID4_I 1001:TL 1000:READY_CPU_I 0111:READY_PERI_I 0110:TL 0101:TL 0100:TL 0011:TL 0010:TL 0001:TL 0000:TL */
#define PLL_CK_SEL(x)                         ((u32)(((x) & 0x0000000F) << 4))
#define PLL_GET_CK_SEL(x)                     ((u32)(((x >> 4) & 0x0000000F)))
#define PLL_MASK_DRV_SEL                      ((u32)0x00000003 << 2)          /*!<R/W 11  AGPIO driving sel */
#define PLL_DRV_SEL(x)                        ((u32)(((x) & 0x00000003) << 2))
#define PLL_GET_DRV_SEL(x)                    ((u32)(((x >> 2) & 0x00000003)))
#define PLL_BIT_GPO                           ((u32)0x00000001 << 1)          /*!<R/W 0  AGPIO output enable */
#define PLL_BIT_GPE                           ((u32)0x00000001 << 0)          /*!<R/W 0  AGPIO enable */
/** @} */

/** @defgroup PLL_CPUPLL_PS
 * @brief
 * @{
 **/
#define PLL_BIT_CPUPLL_PS_DUMMY               ((u32)0x00000001 << 15)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_BIT_CPUPLL_POW_CKGEN              ((u32)0x00000001 << 14)          /*!<R/W 0  pow of ckgen */
#define PLL_MASK_CPUPLL_REG_CKGEN_DIV         ((u32)0x000001FF << 5)          /*!<R/W 000111010  DIV9B , N=DIV<8:0>+2 */
#define PLL_CPUPLL_REG_CKGEN_DIV(x)           ((u32)(((x) & 0x000001FF) << 5))
#define PLL_GET_CPUPLL_REG_CKGEN_DIV(x)       ((u32)(((x >> 5) & 0x000001FF)))
#define PLL_BIT_CPUPLL_EN_CK400M_PS           ((u32)0x00000001 << 4)          /*!<R/W 1   */
#define PLL_BIT_CPUPLL_EN_CK400M              ((u32)0x00000001 << 3)          /*!<R/W 1   */
#define PLL_MASK_CPUPLL_PHASE_SEL             ((u32)0x00000007 << 0)          /*!<R/W 000   */
#define PLL_CPUPLL_PHASE_SEL(x)               ((u32)(((x) & 0x00000007) << 0))
#define PLL_GET_CPUPLL_PHASE_SEL(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup PLL_PERIPLL_PS
 * @brief
 * @{
 **/
#define PLL_BIT_PERIPLL_PS_DUMMY              ((u32)0x00000001 << 15)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_BIT_PERIPLL_POW_CKGEN             ((u32)0x00000001 << 14)          /*!<R/W 0  pow of ckgen */
#define PLL_MASK_PERIPLL_REG_CKGEN_DIV        ((u32)0x000001FF << 5)          /*!<R/W 000111010  DIV9B , N=DIV<8:0>+2 */
#define PLL_PERIPLL_REG_CKGEN_DIV(x)          ((u32)(((x) & 0x000001FF) << 5))
#define PLL_GET_PERIPLL_REG_CKGEN_DIV(x)      ((u32)(((x >> 5) & 0x000001FF)))
#define PLL_BIT_PERIPLL_EN_CK400M_PS          ((u32)0x00000001 << 4)          /*!<R/W 1   */
#define PLL_BIT_PERIPLL_EN_CK400M             ((u32)0x00000001 << 3)          /*!<R/W 1   */
#define PLL_MASK_PERIPLL_PHASE_SEL            ((u32)0x00000007 << 0)          /*!<R/W 000   */
#define PLL_PERIPLL_PHASE_SEL(x)              ((u32)(((x) & 0x00000007) << 0))
#define PLL_GET_PERIPLL_PHASE_SEL(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup PLL_AUX_BG
 * @brief
 * @{
 **/
#define PLL_BIT_POW_BG                        ((u32)0x00000001 << 0)          /*!<R/W 0  power on BG */
#define PLL_BIT_POW_I                         ((u32)0x00000001 << 1)          /*!<R/W 1  power on BG current */
#define PLL_BIT_POW_MBIAS                     ((u32)0x00000001 << 2)          /*!<R/W 0  power on mbias */
#define PLL_BIT_BG_DUMMY0                     ((u32)0x00000001 << 3)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_MASK_R1_TUNE                      ((u32)0x00000007 << 4)          /*!<R/W 100  Add 200k per step fine tune BG (Efuse) */
#define PLL_R1_TUNE(x)                        ((u32)(((x) & 0x00000007) << 4))
#define PLL_GET_R1_TUNE(x)                    ((u32)(((x >> 4) & 0x00000007)))
#define PLL_BIT_BG_DUMMY1                     ((u32)0x00000001 << 7)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_MASK_R2_TUNE                      ((u32)0x00000007 << 8)          /*!<R/W 100  Add 20k per step fine tune BG (Efuse) */
#define PLL_R2_TUNE(x)                        ((u32)(((x) & 0x00000007) << 8))
#define PLL_GET_R2_TUNE(x)                    ((u32)(((x >> 8) & 0x00000007)))
#define PLL_BIT_BG_DUMMY2                     ((u32)0x00000001 << 11)          /*!<R/W 0  HW used only , these are for reserved */
#define PLL_MASK_VR1200_TUNE                  ((u32)0x00000007 << 12)          /*!<R/W 100  Add 40k per step fine tune BG (Efuse) */
#define PLL_VR1200_TUNE(x)                    ((u32)(((x) & 0x00000007) << 12))
#define PLL_GET_VR1200_TUNE(x)                ((u32)(((x >> 12) & 0x00000007)))
#define PLL_BIT_BG_DUMMY3                     ((u32)0x00000001 << 15)          /*!<R/W 0  HW used only , these are for reserved */
/** @} */

/** @defgroup PLL_STATE
 * @brief
 * @{
 **/
#define PLL_BIT_DSP_PLL_RDY                   ((u32)0x00000001 << 1)          /*!<R   PERI PLL clock is ready */
#define PLL_BIT_CPU_PLL_RDY                   ((u32)0x00000001 << 0)          /*!<R   CPU PLL clock is ready */
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup PLL_Exported_Types PLL Exported Types
  * @{
*****************************************************************************/


/**
 * @brief PLL Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t PLL_CPUPLL_CTRL0;                       /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t PLL_CPUPLL_CTRL1;                       /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t PLL_CPUPLL_CTRL2;                       /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t PLL_CPUPLL_CTRL3;                       /*!<  Register,  Address offset: 0x00C */
	__IO uint32_t PLL_PERIPLL_CTRL0;                      /*!<  Register,  Address offset: 0x010 */
	__IO uint32_t PLL_PERIPLL_CTRL1;                      /*!<  Register,  Address offset: 0x014 */
	__IO uint32_t PLL_PERIPLL_CTRL2;                      /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t PLL_PERIPLL_CTRL3;                      /*!<  Register,  Address offset: 0x01C */
	__IO uint32_t PLL_TEST;                               /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t PLL_CPUPLL_PS;                          /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t PLL_PERIPLL_PS;                         /*!<  Register,  Address offset: 0x028 */
	__IO uint32_t PLL_AUX_BG;                             /*!<  Register,  Address offset: 0x02C */
	__I  uint32_t PLL_STATE;                              /*!<  Register,  Address offset: 0x030 */
	__IO uint32_t PLL_DUMMY;                              /*!<  Register,  Address offset: 0x034 */
} PLL_TypeDef;

#define PLL_BASE ((PLL_TypeDef *) (0x4100C440))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */


/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
