#ifndef _AMEBAD2_MEM_CTRL_H_
#define _AMEBAD2_MEM_CTRL_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup MEM_CTRL
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup MEM_CTRL_Register_Definitions MEM_CTRL Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup REG_CTRL_MBIST_ALL_CTRL
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_STA                       ((u32)0x0000001F << 16)          /*!<R 5'h0  Total mbist status [16] MBIST DRF_START_PAUSE [17] MBIST NRML_DONE_ALL [18] MBIST NRML_FAIL_ALL [19] MBIST DRF_DONE_ALL [20] MBIST DRF_FAIL_ALL */
#define CTRL_MBIST_STA(x)                         ((u32)(((x) & 0x0000001F) << 16))
#define CTRL_GET_MBIST_STA(x)                     ((u32)(((x >> 16) & 0x0000001F)))
#define CTRL_MASK_MBIST_CTRL                      ((u32)0x000000FF << 0)          /*!<R/W 8'h0  Total mbist control [0] MBIST_CLK_EN_ALL BIS_CLK enable of all system memory groups. [1] MBIST_RSTB_ALL Reset controll mbist circuit of all system memory groups. [3:2]MBIST_MSEL_ALL mbist method selection of all memory groups: 2'b01: enable normal mode mbist 2'b10: enable DRF mode mbist 2'b11: enable normal&DRF mode mbist [4]MBIST_RESUME [5]MBIST_RESUME_PS [6]MBIST_DYN_READ_EN If MBIST_MSEL_ALL[1]=1 & MBIST_DYN_READ_EN=1, run VDDR mode mbist. [7]MBIST_PHYCLK_STOP */
#define CTRL_MBIST_CTRL(x)                        ((u32)(((x) & 0x000000FF) << 0))
#define CTRL_GET_MBIST_CTRL(x)                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_DONE_STA0
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_DONE                      ((u32)0x7FFFFFFF << 0)          /*!<R 31'b0  30 mbist_group in es003 1 special group bt_e0 */
#define CTRL_MBIST_DONE(x)                        ((u32)(((x) & 0x7FFFFFFF) << 0))
#define CTRL_GET_MBIST_DONE(x)                    ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_START_PAUSE0
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_START_PAUSE               ((u32)0x7FFFFFFF << 0)          /*!<R 31'b0   */
#define CTRL_MBIST_START_PAUSE(x)                 ((u32)(((x) & 0x7FFFFFFF) << 0))
#define CTRL_GET_MBIST_START_PAUSE(x)             ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_NORM_FAIL0
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_NORM_FAIL_31_0            ((u32)0xFFFFFFFF << 0)          /*!<R 32'b0   */
#define CTRL_MBIST_NORM_FAIL_31_0(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_NORM_FAIL_31_0(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_NORM_FAIL1
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_NORM_FAIL_63_32           ((u32)0xFFFFFFFF << 0)          /*!<R 32'b0   */
#define CTRL_MBIST_NORM_FAIL_63_32(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_NORM_FAIL_63_32(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_NORM_FAIL2
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_NORM_FAIL_87_64           ((u32)0x00FFFFFF << 0)          /*!<R 24'b0  87 memory block in es003 1 special bt block */
#define CTRL_MBIST_NORM_FAIL_87_64(x)             ((u32)(((x) & 0x00FFFFFF) << 0))
#define CTRL_GET_MBIST_NORM_FAIL_87_64(x)         ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_DRF_FAIL0
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_DRF_FAIL_31_0             ((u32)0xFFFFFFFF << 0)          /*!<R 32'b0   */
#define CTRL_MBIST_DRF_FAIL_31_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_DRF_FAIL_31_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_DRF_FAIL1
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_DRF_FAIL_63_32            ((u32)0xFFFFFFFF << 0)          /*!<R 32'b0   */
#define CTRL_MBIST_DRF_FAIL_63_32(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_DRF_FAIL_63_32(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_DRF_FAIL2
 * @brief
 * @{
 **/
#define CTRL_MASK_MBIST_DRF_FAIL_87_64            ((u32)0x00FFFFFF << 0)          /*!<R 24'b0  87 memory block in es003 1 special bt block */
#define CTRL_MBIST_DRF_FAIL_87_64(x)              ((u32)(((x) & 0x00FFFFFF) << 0))
#define CTRL_GET_MBIST_DRF_FAIL_87_64(x)          ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup REG_CTRL_BT_BIST_DONE
 * @brief
 * @{
 **/
#define CTRL_MASK_BT_BIST_DONE                    ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_BT_BIST_DONE(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_DONE(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_BT_BIST_FAIL
 * @brief
 * @{
 **/
#define CTRL_MASK_BT_BIST_FAIL                    ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_BT_BIST_FAIL(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_FAIL(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_BT_BIST_DRF_PASUE
 * @brief
 * @{
 **/
#define CTRL_MASK_BT_BIST_DRF_PAUSE               ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_BT_BIST_DRF_PAUSE(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_DRF_PAUSE(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MISR_DOUT_SEL
 * @brief
 * @{
 **/
#define CTRL_MASK_KR4_ITCM0_MISR_SEL              ((u32)0x0000000F << 4)          /*!<R/W 4'h0  Used for KR4 itcm rom mbist, select which rom's misr_dataout[63:0] can be read from KR4_ITCM_ROM_MISR_DOUT. */
#define CTRL_KR4_ITCM0_MISR_SEL(x)                ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_KR4_ITCM0_MISR_SEL(x)            ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_MASK_KM4_ITCM0_MISR_SEL              ((u32)0x0000000F << 0)          /*!<R/W 4'h0  Used for KM4 itcm rom mbist, select which rom's misr_dataout[63:0] can be read from KM4_ITCM_ROM_MISR_DOUT. */
#define CTRL_KM4_ITCM0_MISR_SEL(x)                ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_KM4_ITCM0_MISR_SEL(x)            ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR0
 * @brief
 * @{
 **/
#define CTRL_MASK_ROM_LSPMC_E0_MISR_DOUT_31_0     ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_ROM_LSPMC_E0_MISR_DOUT_31_0(x)       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_LSPMC_E0_MISR_DOUT_31_0(x)   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR1
 * @brief
 * @{
 **/
#define CTRL_MASK_ROM_LSPMC_E0_MISR_DOUT_35_32    ((u32)0x0000000F << 0)          /*!<R 4'h0   */
#define CTRL_ROM_LSPMC_E0_MISR_DOUT_35_32(x)      ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_ROM_LSPMC_E0_MISR_DOUT_35_32(x)  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR2
 * @brief
 * @{
 **/
#define CTRL_MASK_ROM_AONPMC_E0_MISR_DOUT_31_0    ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_ROM_AONPMC_E0_MISR_DOUT_31_0(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_AONPMC_E0_MISR_DOUT_31_0(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR3
 * @brief
 * @{
 **/
#define CTRL_MASK_ROM_AONPMC_E0_MISR_DOUT_35_32   ((u32)0x0000000F << 0)          /*!<R 4'h0   */
#define CTRL_ROM_AONPMC_E0_MISR_DOUT_35_32(x)     ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_ROM_AONPMC_E0_MISR_DOUT_35_32(x) ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR4
 * @brief
 * @{
 **/
#define CTRL_MASK_KM4_ITCM0_ROM_MISR_DOUT_31_0    ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_KM4_ITCM0_ROM_MISR_DOUT_31_0(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM4_ITCM0_ROM_MISR_DOUT_31_0(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR5
 * @brief
 * @{
 **/
#define CTRL_MASK_KM4_ITCM0_ROM_MISR_DOUT_63_32   ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_KM4_ITCM0_ROM_MISR_DOUT_63_32(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM4_ITCM0_ROM_MISR_DOUT_63_32(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR6
 * @brief
 * @{
 **/
#define CTRL_MASK_KR4_ITCM0_ROM_MISR_DOUT_31_0    ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_KR4_ITCM0_ROM_MISR_DOUT_31_0(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KR4_ITCM0_ROM_MISR_DOUT_31_0(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_MBIST_ROM_MISR7
 * @brief
 * @{
 **/
#define CTRL_MASK_KR4_ITCM0_ROM_MISR_DOUT_63_32   ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0   */
#define CTRL_KR4_ITCM0_ROM_MISR_DOUT_63_32(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KR4_ITCM0_ROM_MISR_DOUT_63_32(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_BT_BIST_RPT
 * @brief
 * @{
 **/
#define CTRL_MASK_BT_BIST_RPT                     ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0  bt_rpt_sel 0~3 : read bt_bist_rpt to get BT bist status; bt_rpt_sel 4~15 : read bt_bist_rpt to get BT rom misr_dout; */
#define CTRL_BT_BIST_RPT(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_RPT(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup REG_CTRL_CON_BT_E0_CTRL1
 * @brief
 * @{
 **/
#define CTRL_MASK_BT_RPT_SEL                      ((u32)0x0000000F << 20)          /*!<R/W 0x0  Select which rom's misr_dataout or bist_status of each memory can be read from bt_bist_rpt. */
#define CTRL_BT_RPT_SEL(x)                        ((u32)(((x) & 0x0000000F) << 20))
#define CTRL_GET_BT_RPT_SEL(x)                    ((u32)(((x >> 20) & 0x0000000F)))
#define CTRL_MASK_GRP_BT_E0_GRP_EN                ((u32)0x0000000F << 8)          /*!<R/W 4'hf  Devide all 30 BT memory blocks into 4 groups. grp_bt_e0_grp_en[N]=1 N=0~3: enable mbist of bt_e0 group N. */
#define CTRL_GRP_BT_E0_GRP_EN(x)                  ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_GRP_BT_E0_GRP_EN(x)              ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_MASK_GRP_BT_E0_CELL_EN               ((u32)0x000000FF << 0)          /*!<R/W 8'hff  grp_bt_e0_cell_en[M]=1 M=0~7: enable mbist of mem cell M in groups whose grp_bt_e0_grp_en set 1. */
#define CTRL_GRP_BT_E0_CELL_EN(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define CTRL_GET_GRP_BT_E0_CELL_EN(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup REG_CTRL_BT_FTC_MEM_CTRL
 * @brief
 * @{
 **/
#define CTRL_BIT_BT_DS_ALL                        ((u32)0x00000001 << 23)          /*!<R/W 1'b0  1 - All BT memory switch to Deep Sleep mode 0 - Power mode of BT memory are controlled by BTPMC */
#define CTRL_BIT_BT_LVDRF_CLKDIS                  ((u32)0x00000001 << 22)          /*!<R/W 0x0   */
#define CTRL_BIT_BT_TMCK_ALL                      ((u32)0x00000001 << 20)          /*!<R/W 1'b0   */
#define CTRL_BIT_BT_ROM_DVSE                      ((u32)0x00000001 << 19)          /*!<R/W 1'b0   */
#define CTRL_MASK_BT_ROM_DVS                      ((u32)0x0000000F << 15)          /*!<R/W 4'b0101  FSNOU_A_SPNN */
#define CTRL_BT_ROM_DVS(x)                        ((u32)(((x) & 0x0000000F) << 15))
#define CTRL_GET_BT_ROM_DVS(x)                    ((u32)(((x >> 15) & 0x0000000F)))
#define CTRL_BIT_BT_1PSR_DVSE                     ((u32)0x00000001 << 14)          /*!<R/W 1'b0  Not used in Lite */
#define CTRL_MASK_BT_1PSR_DVS                     ((u32)0x0000000F << 10)          /*!<R/W 4'b1011  Not used in Lite */
#define CTRL_BT_1PSR_DVS(x)                       ((u32)(((x) & 0x0000000F) << 10))
#define CTRL_GET_BT_1PSR_DVS(x)                   ((u32)(((x >> 10) & 0x0000000F)))
#define CTRL_BIT_BT_1PRF1_DVSE                    ((u32)0x00000001 << 9)          /*!<R/W 1'b0   */
#define CTRL_MASK_BT_1PRF1_DVS                    ((u32)0x0000000F << 5)          /*!<R/W 4'b1100  FSNOU_A_SYNL */
#define CTRL_BT_1PRF1_DVS(x)                      ((u32)(((x) & 0x0000000F) << 5))
#define CTRL_GET_BT_1PRF1_DVS(x)                  ((u32)(((x >> 5) & 0x0000000F)))
#define CTRL_BIT_BT_1PRF0_DVSE                    ((u32)0x00000001 << 4)          /*!<R/W 1'b0   */
#define CTRL_MASK_BT_1PRF0_DVS                    ((u32)0x0000000F << 0)          /*!<R/W 4'b1100  FSNOU_A_SYNL */
#define CTRL_BT_1PRF0_DVS(x)                      ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_BT_1PRF0_DVS(x)                  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_GRP_EN_CTRL0
 * @brief Used to on
 * @{
 **/
#define CTRL_MASK_GRP_EN_BTONK4_E1                ((u32)0x00000003 << 28)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_BTONK4_E1(x)                  ((u32)(((x) & 0x00000003) << 28))
#define CTRL_GET_GRP_EN_BTONK4_E1(x)              ((u32)(((x >> 28) & 0x00000003)))
#define CTRL_MASK_GRP_EN_BTONK4_E0                ((u32)0x00000003 << 26)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_BTONK4_E0(x)                  ((u32)(((x) & 0x00000003) << 26))
#define CTRL_GET_GRP_EN_BTONK4_E0(x)              ((u32)(((x >> 26) & 0x00000003)))
#define CTRL_MASK_GRP_EN_WPOFF_E6                 ((u32)0x00000003 << 24)          /*!<R/W 0x3  Not used, TPRAM/DPRAM don't need grp_en */
#define CTRL_GRP_EN_WPOFF_E6(x)                   ((u32)(((x) & 0x00000003) << 24))
#define CTRL_GET_GRP_EN_WPOFF_E6(x)               ((u32)(((x >> 24) & 0x00000003)))
#define CTRL_MASK_GRP_EN_WPOFF_E5                 ((u32)0x00000003 << 22)          /*!<R/W 0x3  Not used, TPRAM/DPRAM don't need grp_en */
#define CTRL_GRP_EN_WPOFF_E5(x)                   ((u32)(((x) & 0x00000003) << 22))
#define CTRL_GET_GRP_EN_WPOFF_E5(x)               ((u32)(((x >> 22) & 0x00000003)))
#define CTRL_MASK_GRP_EN_WPOFF_E2                 ((u32)0x00000003 << 20)          /*!<R/W 0x3  Not used, TPRAM/DPRAM don't need grp_en */
#define CTRL_GRP_EN_WPOFF_E2(x)                   ((u32)(((x) & 0x00000003) << 20))
#define CTRL_GET_GRP_EN_WPOFF_E2(x)               ((u32)(((x >> 20) & 0x00000003)))
#define CTRL_MASK_GRP_EN_WPOFF_E1                 ((u32)0x000003FF << 10)          /*!<R/W 0x3ff   */
#define CTRL_GRP_EN_WPOFF_E1(x)                   ((u32)(((x) & 0x000003FF) << 10))
#define CTRL_GET_GRP_EN_WPOFF_E1(x)               ((u32)(((x >> 10) & 0x000003FF)))
#define CTRL_MASK_GRP_EN_WLK4_E0                  ((u32)0x00000003 << 8)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_WLK4_E0(x)                    ((u32)(((x) & 0x00000003) << 8))
#define CTRL_GET_GRP_EN_WLK4_E0(x)                ((u32)(((x >> 8) & 0x00000003)))
#define CTRL_MASK_GRP_EN_HSYS_E1                  ((u32)0x0000000F << 4)          /*!<R/W 0xf   */
#define CTRL_GRP_EN_HSYS_E1(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_GRP_EN_HSYS_E1(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_MASK_GRP_EN_HSYS_E0                  ((u32)0x0000000F << 0)          /*!<R/W 0xf   */
#define CTRL_GRP_EN_HSYS_E0(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_GRP_EN_HSYS_E0(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_GRP_EN_CTRL1
 * @brief Used to on
 * @{
 **/
#define CTRL_MASK_GRP_EN_KM4_ICACHE_E0            ((u32)0x00000003 << 26)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_KM4_ICACHE_E0(x)              ((u32)(((x) & 0x00000003) << 26))
#define CTRL_GET_GRP_EN_KM4_ICACHE_E0(x)          ((u32)(((x >> 26) & 0x00000003)))
#define CTRL_MASK_GRP_EN_KM4_DCACHE_E0            ((u32)0x00000003 << 24)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_KM4_DCACHE_E0(x)              ((u32)(((x) & 0x00000003) << 24))
#define CTRL_GET_GRP_EN_KM4_DCACHE_E0(x)          ((u32)(((x >> 24) & 0x00000003)))
#define CTRL_MASK_GRP_EN_HIFI5_E5                 ((u32)0x0000000F << 20)          /*!<R/W 0xf   */
#define CTRL_GRP_EN_HIFI5_E5(x)                   ((u32)(((x) & 0x0000000F) << 20))
#define CTRL_GET_GRP_EN_HIFI5_E5(x)               ((u32)(((x >> 20) & 0x0000000F)))
#define CTRL_MASK_GRP_EN_HIFI5_E4                 ((u32)0x0000000F << 16)          /*!<R/W 0xf   */
#define CTRL_GRP_EN_HIFI5_E4(x)                   ((u32)(((x) & 0x0000000F) << 16))
#define CTRL_GET_GRP_EN_HIFI5_E4(x)               ((u32)(((x >> 16) & 0x0000000F)))
#define CTRL_MASK_GRP_EN_HIFI5_E3                 ((u32)0x0000000F << 12)          /*!<R/W 0xf   */
#define CTRL_GRP_EN_HIFI5_E3(x)                   ((u32)(((x) & 0x0000000F) << 12))
#define CTRL_GET_GRP_EN_HIFI5_E3(x)               ((u32)(((x >> 12) & 0x0000000F)))
#define CTRL_MASK_GRP_EN_HIFI5_E2                 ((u32)0x0000003F << 6)          /*!<R/W 0x3f   */
#define CTRL_GRP_EN_HIFI5_E2(x)                   ((u32)(((x) & 0x0000003F) << 6))
#define CTRL_GET_GRP_EN_HIFI5_E2(x)               ((u32)(((x >> 6) & 0x0000003F)))
#define CTRL_MASK_GRP_EN_HIFI5_E1                 ((u32)0x0000003F << 0)          /*!<R/W 0x3f   */
#define CTRL_GRP_EN_HIFI5_E1(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define CTRL_GET_GRP_EN_HIFI5_E1(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup REG_CTRL_MBIST_GRP_EN_CTRL2
 * @brief Used to on
 * @{
 **/
#define CTRL_MASK_GRP_EN_KR4_ITCM0_E0             ((u32)0x0000003F << 13)          /*!<R/W 0x3f   */
#define CTRL_GRP_EN_KR4_ITCM0_E0(x)               ((u32)(((x) & 0x0000003F) << 13))
#define CTRL_GET_GRP_EN_KR4_ITCM0_E0(x)           ((u32)(((x >> 13) & 0x0000003F)))
#define CTRL_MASK_GRP_EN_KR4_ICACHE_E0            ((u32)0x00000003 << 11)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_KR4_ICACHE_E0(x)              ((u32)(((x) & 0x00000003) << 11))
#define CTRL_GET_GRP_EN_KR4_ICACHE_E0(x)          ((u32)(((x >> 11) & 0x00000003)))
#define CTRL_MASK_GRP_EN_KR4_DCACHE_E0            ((u32)0x00000003 << 9)          /*!<R/W 0x3   */
#define CTRL_GRP_EN_KR4_DCACHE_E0(x)              ((u32)(((x) & 0x00000003) << 9))
#define CTRL_GET_GRP_EN_KR4_DCACHE_E0(x)          ((u32)(((x >> 9) & 0x00000003)))
#define CTRL_MASK_GRP_EN_KM4_ITCM0_E0             ((u32)0x000001FF << 0)          /*!<R/W 0x1ff   */
#define CTRL_GRP_EN_KM4_ITCM0_E0(x)               ((u32)(((x) & 0x000001FF) << 0))
#define CTRL_GET_GRP_EN_KM4_ITCM0_E0(x)           ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/** @defgroup REG_CTRL_GRP_PWR_MSK0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_PMSK_WPOFF_E6               ((u32)0x00000003 << 30)          /*!<R/W 0x3  WIFI-PMC controlled memory group: bit[N] = 1: power mode of memory block N is controlled by wifi-PMC power signals bit[N] = 0: power port of memory block N is tie0. */
#define CTRL_GRP_PMSK_WPOFF_E6(x)                 ((u32)(((x) & 0x00000003) << 30))
#define CTRL_GET_GRP_PMSK_WPOFF_E6(x)             ((u32)(((x >> 30) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_WPOFF_E5               ((u32)0x00000003 << 28)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_WPOFF_E5(x)                 ((u32)(((x) & 0x00000003) << 28))
#define CTRL_GET_GRP_PMSK_WPOFF_E5(x)             ((u32)(((x >> 28) & 0x00000003)))
#define CTRL_BIT_GRP_PMSK_WPOFF_E4                ((u32)0x00000001 << 27)          /*!<R/W 0x1   */
#define CTRL_BIT_GRP_PMSK_WPOFF_E3                ((u32)0x00000001 << 26)          /*!<R/W 0x1   */
#define CTRL_MASK_GRP_PMSK_WPOFF_E2               ((u32)0x00000003 << 24)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_WPOFF_E2(x)                 ((u32)(((x) & 0x00000003) << 24))
#define CTRL_GET_GRP_PMSK_WPOFF_E2(x)             ((u32)(((x >> 24) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_WPOFF_E1               ((u32)0x000003FF << 14)          /*!<R/W 0x3ff   */
#define CTRL_GRP_PMSK_WPOFF_E1(x)                 ((u32)(((x) & 0x000003FF) << 14))
#define CTRL_GET_GRP_PMSK_WPOFF_E1(x)             ((u32)(((x >> 14) & 0x000003FF)))
#define CTRL_BIT_GRP_PMSK_WPOFF_E0                ((u32)0x00000001 << 13)          /*!<R/W 0x1   */
#define CTRL_MASK_GRP_PMSK_WLK4_E0                ((u32)0x00000003 << 11)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_WLK4_E0(x)                  ((u32)(((x) & 0x00000003) << 11))
#define CTRL_GET_GRP_PMSK_WLK4_E0(x)              ((u32)(((x >> 11) & 0x00000003)))
#define CTRL_BIT_GRP_PMSK_AONPMC_E0               ((u32)0x00000001 << 10)          /*!<R/W 0x1   */
#define CTRL_BIT_GRP_PMSK_LSPMC_E0                ((u32)0x00000001 << 9)          /*!<R/W 0x1   */
#define CTRL_BIT_GRP_PMSK_LRET_E0                 ((u32)0x00000001 << 8)          /*!<R/W 0x1   */
#define CTRL_MASK_GRP_PMSK_HSYS_E1                ((u32)0x0000000F << 4)          /*!<R/W 0xf   */
#define CTRL_GRP_PMSK_HSYS_E1(x)                  ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_GRP_PMSK_HSYS_E1(x)              ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_MASK_GRP_PMSK_HSYS_E0                ((u32)0x0000000F << 0)          /*!<R/W 0xf  SYSPMC controlled memory groups: bit[N] = 1: power mode of memory block N is controlled by rw_hsys_e0_pwr_slp[2:0]; bit[N] = 0: power mode of memory block N is controlled by rw_hsys_e0_pwr_act[2:0] */
#define CTRL_GRP_PMSK_HSYS_E0(x)                  ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_GRP_PMSK_HSYS_E0(x)              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_CTRL_GRP_PWR_MSK1
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_PMSK_HIFI5_E5               ((u32)0x0000000F << 28)          /*!<R/W 0xf   */
#define CTRL_GRP_PMSK_HIFI5_E5(x)                 ((u32)(((x) & 0x0000000F) << 28))
#define CTRL_GET_GRP_PMSK_HIFI5_E5(x)             ((u32)(((x >> 28) & 0x0000000F)))
#define CTRL_MASK_GRP_PMSK_HIFI5_E4               ((u32)0x0000000F << 24)          /*!<R/W 0xf   */
#define CTRL_GRP_PMSK_HIFI5_E4(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define CTRL_GET_GRP_PMSK_HIFI5_E4(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define CTRL_MASK_GRP_PMSK_HIFI5_E3               ((u32)0x0000000F << 20)          /*!<R/W 0xf   */
#define CTRL_GRP_PMSK_HIFI5_E3(x)                 ((u32)(((x) & 0x0000000F) << 20))
#define CTRL_GET_GRP_PMSK_HIFI5_E3(x)             ((u32)(((x >> 20) & 0x0000000F)))
#define CTRL_MASK_GRP_PMSK_HIFI5_E2               ((u32)0x0000003F << 14)          /*!<R/W 0x3f   */
#define CTRL_GRP_PMSK_HIFI5_E2(x)                 ((u32)(((x) & 0x0000003F) << 14))
#define CTRL_GET_GRP_PMSK_HIFI5_E2(x)             ((u32)(((x >> 14) & 0x0000003F)))
#define CTRL_MASK_GRP_PMSK_HIFI5_E1               ((u32)0x0000003F << 8)          /*!<R/W 0x3f   */
#define CTRL_GRP_PMSK_HIFI5_E1(x)                 ((u32)(((x) & 0x0000003F) << 8))
#define CTRL_GET_GRP_PMSK_HIFI5_E1(x)             ((u32)(((x >> 8) & 0x0000003F)))
#define CTRL_BIT_GRP_PMSK_HIFI5_E0                ((u32)0x00000001 << 7)          /*!<R/W 0x1   */
#define CTRL_BIT_GRP_PMSK_BTOFFK4_E1              ((u32)0x00000001 << 6)          /*!<R/W 0x1   */
#define CTRL_BIT_GRP_PMSK_BTOFFK4_E0              ((u32)0x00000001 << 5)          /*!<R/W 0x1   */
#define CTRL_MASK_GRP_PMSK_BTONK4_E1              ((u32)0x00000003 << 3)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_BTONK4_E1(x)                ((u32)(((x) & 0x00000003) << 3))
#define CTRL_GET_GRP_PMSK_BTONK4_E1(x)            ((u32)(((x >> 3) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_BTONK4_E0              ((u32)0x00000003 << 1)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_BTONK4_E0(x)                ((u32)(((x) & 0x00000003) << 1))
#define CTRL_GET_GRP_PMSK_BTONK4_E0(x)            ((u32)(((x >> 1) & 0x00000003)))
#define CTRL_BIT_GRP_PMSK_WPON_E0                 ((u32)0x00000001 << 0)          /*!<R/W 0x1   */
/** @} */

/** @defgroup REG_CTRL_GRP_PWR_MSK2
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_PMSK_KR4_ITCM0_E0           ((u32)0x0000003F << 17)          /*!<R/W 0x3f  CPU cache/rom: bit[N]=1 : power mode of memory block N is controlled by rw_itcm_pwr_act[2:0] bit[N/=0: power port of memory block N is tie0. */
#define CTRL_GRP_PMSK_KR4_ITCM0_E0(x)             ((u32)(((x) & 0x0000003F) << 17))
#define CTRL_GET_GRP_PMSK_KR4_ITCM0_E0(x)         ((u32)(((x >> 17) & 0x0000003F)))
#define CTRL_MASK_GRP_PMSK_KR4_ICACHE_E0          ((u32)0x00000003 << 15)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_KR4_ICACHE_E0(x)            ((u32)(((x) & 0x00000003) << 15))
#define CTRL_GET_GRP_PMSK_KR4_ICACHE_E0(x)        ((u32)(((x >> 15) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_KR4_DCACHE_E0          ((u32)0x00000003 << 13)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_KR4_DCACHE_E0(x)            ((u32)(((x) & 0x00000003) << 13))
#define CTRL_GET_GRP_PMSK_KR4_DCACHE_E0(x)        ((u32)(((x >> 13) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_KM4_ITCM0_E0           ((u32)0x000001FF << 4)          /*!<R/W 0x1ff   */
#define CTRL_GRP_PMSK_KM4_ITCM0_E0(x)             ((u32)(((x) & 0x000001FF) << 4))
#define CTRL_GET_GRP_PMSK_KM4_ITCM0_E0(x)         ((u32)(((x >> 4) & 0x000001FF)))
#define CTRL_MASK_GRP_PMSK_KM4_ICACHE_E0          ((u32)0x00000003 << 2)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_KM4_ICACHE_E0(x)            ((u32)(((x) & 0x00000003) << 2))
#define CTRL_GET_GRP_PMSK_KM4_ICACHE_E0(x)        ((u32)(((x >> 2) & 0x00000003)))
#define CTRL_MASK_GRP_PMSK_KM4_DCACHE_E0          ((u32)0x00000003 << 0)          /*!<R/W 0x3   */
#define CTRL_GRP_PMSK_KM4_DCACHE_E0(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_GRP_PMSK_KM4_DCACHE_E0(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup REG_CTRL_RSEL0
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL0_TEST1                      ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL0_RM                        ((u32)0x0000000F << 1)          /*!<R/W 4'b0010   */
#define CTRL_RSEL0_RM(x)                          ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL0_RM(x)                      ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL0_RME                        ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_RSEL1
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL1_TEST1                      ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL1_RM                        ((u32)0x0000000F << 1)          /*!<R/W 4'b0010   */
#define CTRL_RSEL1_RM(x)                          ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL1_RM(x)                      ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL1_RME                        ((u32)0x00000001 << 0)          /*!<R/W 1'b0  SNPS_ULP dummy parameter */
/** @} */

/** @defgroup REG_CTRL_RSEL2
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL2_TMCK                       ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL2_DVS                       ((u32)0x0000000F << 1)          /*!<R/W 4'b1011   */
#define CTRL_RSEL2_DVS(x)                         ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL2_DVS(x)                     ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL2_DVSE                       ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_RSEL3
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL3_TMCK                       ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL3_DVS                       ((u32)0x0000000F << 1)          /*!<R/W 4'b1100   */
#define CTRL_RSEL3_DVS(x)                         ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL3_DVS(x)                     ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL3_DVSE                       ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_RSEL4
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL4_TMCK                       ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL4_DVS                       ((u32)0x0000000F << 1)          /*!<R/W 4'b1010   */
#define CTRL_RSEL4_DVS(x)                         ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL4_DVS(x)                     ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL4_DVSE                       ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_RSEL5
 * @brief
 * @{
 **/
#define CTRL_MASK_RSEL5_DVS                       ((u32)0x0000000F << 1)          /*!<R/W 4'b0101   */
#define CTRL_RSEL5_DVS(x)                         ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL5_DVS(x)                     ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL5_DVSE                       ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_RSEL6
 * @brief
 * @{
 **/
#define CTRL_BIT_RSEL6_TMCK                       ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_MASK_RSEL6_DVS                       ((u32)0x0000000F << 1)          /*!<R/W 4'b1010   */
#define CTRL_RSEL6_DVS(x)                         ((u32)(((x) & 0x0000000F) << 1))
#define CTRL_GET_RSEL6_DVS(x)                     ((u32)(((x >> 1) & 0x0000000F)))
#define CTRL_BIT_RSEL6_DVSE                       ((u32)0x00000001 << 0)          /*!<R/W 1'b0  FTC_ULP dummy parameter */
/** @} */

/** @defgroup REG_CTRL_ASEL0
 * @brief
 * @{
 **/
#define CTRL_MASK_ASEL0_WAEM                      ((u32)0x0000000F << 4)          /*!<R/W 4'b1010   */
#define CTRL_ASEL0_WAEM(x)                        ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_ASEL0_WAEM(x)                    ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_BIT_ASEL0_WAE                        ((u32)0x00000001 << 3)          /*!<R/W 1'b1   */
#define CTRL_MASK_ASEL0_RAEM                      ((u32)0x00000003 << 1)          /*!<R/W 2'b00   */
#define CTRL_ASEL0_RAEM(x)                        ((u32)(((x) & 0x00000003) << 1))
#define CTRL_GET_ASEL0_RAEM(x)                    ((u32)(((x >> 1) & 0x00000003)))
#define CTRL_BIT_ASEL0_RAE                        ((u32)0x00000001 << 0)          /*!<R/W 1'b1   */
/** @} */

/** @defgroup REG_CTRL_ASEL1
 * @brief
 * @{
 **/
#define CTRL_MASK_ASEL1_WAEM                      ((u32)0x0000000F << 4)          /*!<R/W 4'b1010   */
#define CTRL_ASEL1_WAEM(x)                        ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_ASEL1_WAEM(x)                    ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_BIT_ASEL1_WAE                        ((u32)0x00000001 << 3)          /*!<R/W 1'b0   */
#define CTRL_MASK_ASEL1_RAEM                      ((u32)0x00000003 << 1)          /*!<R/W 2'b00   */
#define CTRL_ASEL1_RAEM(x)                        ((u32)(((x) & 0x00000003) << 1))
#define CTRL_GET_ASEL1_RAEM(x)                    ((u32)(((x >> 1) & 0x00000003)))
#define CTRL_BIT_ASEL1_RAE                        ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_ASEL2
 * @brief
 * @{
 **/
#define CTRL_MASK_ASEL2_WAEM                      ((u32)0x0000000F << 4)          /*!<R/W 4'b1010   */
#define CTRL_ASEL2_WAEM(x)                        ((u32)(((x) & 0x0000000F) << 4))
#define CTRL_GET_ASEL2_WAEM(x)                    ((u32)(((x >> 4) & 0x0000000F)))
#define CTRL_BIT_ASEL2_WAE                        ((u32)0x00000001 << 3)          /*!<R/W 1'b0   */
#define CTRL_MASK_ASEL2_RAEM                      ((u32)0x00000003 << 1)          /*!<R/W 2'b00   */
#define CTRL_ASEL2_RAEM(x)                        ((u32)(((x) & 0x00000003) << 1))
#define CTRL_GET_ASEL2_RAEM(x)                    ((u32)(((x >> 1) & 0x00000003)))
#define CTRL_BIT_ASEL2_RAE                        ((u32)0x00000001 << 0)          /*!<R/W 1'b0   */
/** @} */

/** @defgroup REG_CTRL_GRP_x_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_x_MSEL                      ((u32)0x00000003 << 18)          /*!<R/W 2'b11  01 - This memory group can do normal mbist; 10 - This memory group can do DRF mbist; 11 - This memory group can do normal & DRF mbist */
#define CTRL_GRP_x_MSEL(x)                        ((u32)(((x) & 0x00000003) << 18))
#define CTRL_GET_GRP_x_MSEL(x)                    ((u32)(((x >> 18) & 0x00000003)))
#define CTRL_BIT_GRP_x_LOOP_MODE                  ((u32)0x00000001 << 17)          /*!<R/W 1'b0  0 - Assist loop mode 1 - Don’t assist loop mode */
#define CTRL_BIT_GRP_x_RSTN                       ((u32)0x00000001 << 16)          /*!<R/W 1'b1  0 - Reset mbist circuit of this memory group; 1 - Release mbist circuit of this memory group. */
#define CTRL_MASK_GRP_x_PWR_LPS                   ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Reserved for grp_x_pwr_lps for memory groups power controlled by SYSPMC. Setted before chip goes to SLEEP.Used to controll power mode of this memory group when chip is in SLEEP. For FTC_ULP mem, pwr_lps[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_lps[2:0] map to {SD,DS,LS}. 000 - active 001 - light sleep 010 - deep sleep 100 - shut down */
#define CTRL_GRP_x_PWR_LPS(x)                     ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_x_PWR_LPS(x)                 ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_x_PWR_ACT                   ((u32)0x00000007 << 8)          /*!<R/W 3'b000  Used to controll power mode of this memory group when chip is ACTIVE. For FTC_ULP mem, pwr_act[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_act[2:0] map to {SD,DS,LS}. 000 - active 001 - light sleep 010 - deep sleep 100 - shut down */
#define CTRL_GRP_x_PWR_ACT(x)                     ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_x_PWR_ACT(x)                 ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_x_RM_SEL                    ((u32)0x0000001F << 0)          /*!<R/W 5'h0  Search es003 LUT sheet "rsel" table for read-write margin paramters based on memory type . */
#define CTRL_GRP_x_RM_SEL(x)                      ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_x_RM_SEL(x)                  ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HSYS_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HSYS_E0_RM_SEL              ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR, FSNOU_A_SHNL */
#define CTRL_GRP_HSYS_E0_RM_SEL(x)                ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HSYS_E0_RM_SEL(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HSYS_E1_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HSYS_E1_RM_SEL              ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR, FSNOU_A_SHNL */
#define CTRL_GRP_HSYS_E1_RM_SEL(x)                ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HSYS_E1_RM_SEL(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_LRET_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_LRET_E0_PWR_LPS             ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Reserved for grp_x_pwr_lps for memory groups power controlled by SYSPMC. Setted before chip goes to SLEEP.Used to controll power mode of this memory group when chip is in SLEEP. For FTC_ULP mem, pwr_lps[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_lps[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_LRET_E0_PWR_LPS(x)               ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_LRET_E0_PWR_LPS(x)           ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_LRET_E0_PWR_ACT             ((u32)0x00000007 << 8)          /*!<R/W 3'b100  Used to controll power mode of this memory group when chip is ACTIVE. For FTC_ULP mem, pwr_act[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_act[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_LRET_E0_PWR_ACT(x)               ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_LRET_E0_PWR_ACT(x)           ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_LRET_E0_ASSIST_SEL          ((u32)0x00000007 << 5)          /*!<R/W 3'h0  RW 0.8V */
#define CTRL_GRP_LRET_E0_ASSIST_SEL(x)            ((u32)(((x) & 0x00000007) << 5))
#define CTRL_GET_GRP_LRET_E0_ASSIST_SEL(x)        ((u32)(((x >> 5) & 0x00000007)))
#define CTRL_MASK_GRP_LRET_E0_RM_SEL              ((u32)0x0000001F << 0)          /*!<R/W 5'h3  1PRF, FSNOU_A_SYNL */
#define CTRL_GRP_LRET_E0_RM_SEL(x)                ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_LRET_E0_RM_SEL(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_LSPMC_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_LSPMC_E0_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h5  FTC_ULP ROM */
#define CTRL_GRP_LSPMC_E0_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_LSPMC_E0_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_AONPMC_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_AONPMC_E0_RM_SEL            ((u32)0x0000001F << 0)          /*!<R/W 5'h5  FTC_ULP ROM */
#define CTRL_GRP_AONPMC_E0_RM_SEL(x)              ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_AONPMC_E0_RM_SEL(x)          ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WLK4_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WLK4_E0_RM_SEL              ((u32)0x0000001F << 0)          /*!<R/W 5'h3  1PRF,FSNOU_A_SYNL */
#define CTRL_GRP_WLK4_E0_RM_SEL(x)                ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WLK4_E0_RM_SEL(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E0_PWR_LPS            ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Used to controll power mode of this memory group when chip is SLEEP. SNPS mem map to {SD,DS,LS}. SNPS_with_PG memory don’t assign DS/SD in scan mode */
#define CTRL_GRP_WPOFF_E0_PWR_LPS(x)              ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_WPOFF_E0_PWR_LPS(x)          ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_WPOFF_E0_PWR_ACT            ((u32)0x00000007 << 8)          /*!<R/W 3'b000  Used to controll power mode of this memory group when chip is ACTIVE. SNPS mem map to {SD,DS,LS}. SNPS_with_PG memory don’t assign DS/SD in scan mode */
#define CTRL_GRP_WPOFF_E0_PWR_ACT(x)              ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_WPOFF_E0_PWR_ACT(x)          ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_WPOFF_E0_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  1PRF,SNPS */
#define CTRL_GRP_WPOFF_E0_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E0_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E1_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E1_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  1PRF,SNPS */
#define CTRL_GRP_WPOFF_E1_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E1_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E2_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E2_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h4  2PRF,FSNOU_A_SZNN */
#define CTRL_GRP_WPOFF_E2_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E2_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E3_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E3_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  1PRF,SNPS */
#define CTRL_GRP_WPOFF_E3_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E3_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E4_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E4_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  1PRF,SNPS */
#define CTRL_GRP_WPOFF_E4_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E4_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E5_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E5_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h4  2PRF,FSNOU_A_SZNL */
#define CTRL_GRP_WPOFF_E5_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E5_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPOFF_E6_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPOFF_E6_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h4  2PRF,FSNOU_A_SZNL */
#define CTRL_GRP_WPOFF_E6_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPOFF_E6_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_WPON_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_WPON_E0_RM_SEL              ((u32)0x0000001F << 0)          /*!<R/W 5'h3  1prf,FSNOU_A_SYNL */
#define CTRL_GRP_WPON_E0_RM_SEL(x)                ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_WPON_E0_RM_SEL(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_BTONK4_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_BTONK4_E0_RM_SEL            ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR,FSNOU_A_SHNL */
#define CTRL_GRP_BTONK4_E0_RM_SEL(x)              ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_BTONK4_E0_RM_SEL(x)          ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_BTONK4_E1_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_BTONK4_E1_RM_SEL            ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR,FSNOU_A_SHNL */
#define CTRL_GRP_BTONK4_E1_RM_SEL(x)              ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_BTONK4_E1_RM_SEL(x)          ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_BTOFFK4_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_BTOFFK4_E0_PWR_LPS          ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Reserved for grp_x_pwr_lps for memory groups power controlled by SYSPMC. Setted before chip goes to SLEEP.Used to controll power mode of this memory group when chip is in SLEEP. For FTC_ULP mem, pwr_lps[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_lps[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_BTOFFK4_E0_PWR_LPS(x)            ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_BTOFFK4_E0_PWR_LPS(x)        ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_BTOFFK4_E0_PWR_ACT          ((u32)0x00000007 << 8)          /*!<R/W 3'000  Used to controll power mode of this memory group when chip is ACTIVE. For FTC_ULP mem, pwr_act[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_act[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_BTOFFK4_E0_PWR_ACT(x)            ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_BTOFFK4_E0_PWR_ACT(x)        ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_BTOFFK4_E0_RM_SEL           ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR,FSNOU_A_SHNL */
#define CTRL_GRP_BTOFFK4_E0_RM_SEL(x)             ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_BTOFFK4_E0_RM_SEL(x)         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_BTOFFK4_E1_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_BTOFFK4_E1_PWR_LPS          ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Reserved for grp_x_pwr_lps for memory groups power controlled by SYSPMC. Setted before chip goes to SLEEP.Used to controll power mode of this memory group when chip is in SLEEP. For FTC_ULP mem, pwr_lps[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_lps[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_BTOFFK4_E1_PWR_LPS(x)            ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_BTOFFK4_E1_PWR_LPS(x)        ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_BTOFFK4_E1_PWR_ACT          ((u32)0x00000007 << 8)          /*!<R/W 3'b000  Used to controll power mode of this memory group when chip is ACTIVE. For FTC_ULP mem, pwr_act[2:0] map to {SLP,RET,NAP}; For SNPS mem, pwr_act[2:0] map to {SD,DS,LS}. 100 - active 101 - light sleep 110 - deep sleep 000 - shut down */
#define CTRL_GRP_BTOFFK4_E1_PWR_ACT(x)            ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_BTOFFK4_E1_PWR_ACT(x)        ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_BTOFFK4_E1_RM_SEL           ((u32)0x0000001F << 0)          /*!<R/W 5'h2  1PSR,FSNOU_A_SHNL */
#define CTRL_GRP_BTOFFK4_E1_RM_SEL(x)             ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_BTOFFK4_E1_RM_SEL(x)         ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E0_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E0_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E0_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E1_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E1_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E1_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E1_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E2_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E2_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E2_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E2_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E3_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E3_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E3_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E3_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E4_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E4_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E4_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E4_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_HIFI5_E5_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_HIFI5_E5_PWR_LPS            ((u32)0x00000007 << 12)          /*!<R/W 3'b010  Used to controll power mode of this memory group when chip is SLEEP. SNPS mem map to {SD,DS,LS}. SNPS_with_PG memory don’t assign DS/SD in scan mode */
#define CTRL_GRP_HIFI5_E5_PWR_LPS(x)              ((u32)(((x) & 0x00000007) << 12))
#define CTRL_GET_GRP_HIFI5_E5_PWR_LPS(x)          ((u32)(((x >> 12) & 0x00000007)))
#define CTRL_MASK_GRP_HIFI5_E5_PWR_ACT            ((u32)0x00000007 << 8)          /*!<R/W 3'b000  Used to controll power mode of this memory group when chip is ACTIVE. SNPS mem map to {SD,DS,LS}. SNPS_with_PG memory don’t assign DS/SD in scan mode */
#define CTRL_GRP_HIFI5_E5_PWR_ACT(x)              ((u32)(((x) & 0x00000007) << 8))
#define CTRL_GET_GRP_HIFI5_E5_PWR_ACT(x)          ((u32)(((x >> 8) & 0x00000007)))
#define CTRL_MASK_GRP_HIFI5_E5_RM_SEL             ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_HIFI5_E5_RM_SEL(x)               ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_HIFI5_E5_RM_SEL(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KM4_ITCM0_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KM4_ITCM0_E0_RM_SEL         ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KM4_ITCM0_E0_RM_SEL(x)           ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KM4_ITCM0_E0_RM_SEL(x)       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KM4_ICACHE_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KM4_ICACHE_E0_RM_SEL        ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KM4_ICACHE_E0_RM_SEL(x)          ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KM4_ICACHE_E0_RM_SEL(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KM4_DCACHE_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KM4_DCACHE_E0_RM_SEL        ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KM4_DCACHE_E0_RM_SEL(x)          ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KM4_DCACHE_E0_RM_SEL(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KR4_ITCM0_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KR4_ITCM0_E0_RM_SEL         ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KR4_ITCM0_E0_RM_SEL(x)           ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KR4_ITCM0_E0_RM_SEL(x)       ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KR4_ICACHE_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KR4_ICACHE_E0_RM_SEL        ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KR4_ICACHE_E0_RM_SEL(x)          ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KR4_ICACHE_E0_RM_SEL(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_KR4_DCACHE_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_KR4_DCACHE_E0_RM_SEL        ((u32)0x0000001F << 0)          /*!<R/W 5'h0  SNPS */
#define CTRL_GRP_KR4_DCACHE_E0_RM_SEL(x)          ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_KR4_DCACHE_E0_RM_SEL(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_GRP_BT_E0_CTRL0
 * @brief
 * @{
 **/
#define CTRL_MASK_GRP_BT_E0_RM_SEL                ((u32)0x0000001F << 0)          /*!<R/W 5'h0  special group */
#define CTRL_GRP_BT_E0_RM_SEL(x)                  ((u32)(((x) & 0x0000001F) << 0))
#define CTRL_GET_GRP_BT_E0_RM_SEL(x)              ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup REG_CTRL_SCAN_CTRL1
 * @brief
 * @{
 **/
#define CTRL_MASK_SCAN_OCC_DBG_EN_SEC             ((u32)0x00000003 << 18)          /*!<R/W 0x0  scan occ dbg en sec */
#define CTRL_SCAN_OCC_DBG_EN_SEC(x)               ((u32)(((x) & 0x00000003) << 18))
#define CTRL_GET_SCAN_OCC_DBG_EN_SEC(x)           ((u32)(((x >> 18) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_ECDSA           ((u32)0x00000003 << 16)          /*!<R/W 0x0  scan occ dbg en ecdsa */
#define CTRL_SCAN_OCC_DBG_EN_ECDSA(x)             ((u32)(((x) & 0x00000003) << 16))
#define CTRL_GET_SCAN_OCC_DBG_EN_ECDSA(x)         ((u32)(((x >> 16) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_PSRAM           ((u32)0x00000003 << 14)          /*!<R/W 0x0  scan occ dbg en psram */
#define CTRL_SCAN_OCC_DBG_EN_PSRAM(x)             ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_SCAN_OCC_DBG_EN_PSRAM(x)         ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_SPIC            ((u32)0x00000003 << 12)          /*!<R/W 0x0  scan occ dbg en spic */
#define CTRL_SCAN_OCC_DBG_EN_SPIC(x)              ((u32)(((x) & 0x00000003) << 12))
#define CTRL_GET_SCAN_OCC_DBG_EN_SPIC(x)          ((u32)(((x >> 12) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_WMACAXI         ((u32)0x00000003 << 10)          /*!<R/W 0x0  scan occ dbg en wmacaxi */
#define CTRL_SCAN_OCC_DBG_EN_WMACAXI(x)           ((u32)(((x) & 0x00000003) << 10))
#define CTRL_GET_SCAN_OCC_DBG_EN_WMACAXI(x)       ((u32)(((x >> 10) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_SRAM2           ((u32)0x00000003 << 8)          /*!<R/W 0x0  scan occ dbg en sram2 */
#define CTRL_SCAN_OCC_DBG_EN_SRAM2(x)             ((u32)(((x) & 0x00000003) << 8))
#define CTRL_GET_SCAN_OCC_DBG_EN_SRAM2(x)         ((u32)(((x >> 8) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_HBUS            ((u32)0x00000003 << 6)          /*!<R/W 0x0  scan occ dbg en hbus */
#define CTRL_SCAN_OCC_DBG_EN_HBUS(x)              ((u32)(((x) & 0x00000003) << 6))
#define CTRL_GET_SCAN_OCC_DBG_EN_HBUS(x)          ((u32)(((x >> 6) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_GDMAXI          ((u32)0x00000003 << 4)          /*!<R/W 0x0  scan occ dbg en gdmaxi */
#define CTRL_SCAN_OCC_DBG_EN_GDMAXI(x)            ((u32)(((x) & 0x00000003) << 4))
#define CTRL_GET_SCAN_OCC_DBG_EN_GDMAXI(x)        ((u32)(((x >> 4) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_PLFM            ((u32)0x00000003 << 2)          /*!<R/W 0x0  scan occ dbg en plfm */
#define CTRL_SCAN_OCC_DBG_EN_PLFM(x)              ((u32)(((x) & 0x00000003) << 2))
#define CTRL_GET_SCAN_OCC_DBG_EN_PLFM(x)          ((u32)(((x >> 2) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_HFCORE          ((u32)0x00000003 << 0)          /*!<R/W 0x0  scan occ dbg en hifi core */
#define CTRL_SCAN_OCC_DBG_EN_HFCORE(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_SCAN_OCC_DBG_EN_HFCORE(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup REG_CTRL_SCAN_CTRL0
 * @brief
 * @{
 **/
#define CTRL_BIT_SCAN_RET_EN                      ((u32)0x00000001 << 19)          /*!<R/W 0x0  scan retention test enable */
#define CTRL_MASK_SCAN_PLL_OUT_SEL                ((u32)0x00000007 << 16)          /*!<R/W 0x0  scan pll out sel */
#define CTRL_SCAN_PLL_OUT_SEL(x)                  ((u32)(((x) & 0x00000007) << 16))
#define CTRL_GET_SCAN_PLL_OUT_SEL(x)              ((u32)(((x >> 16) & 0x00000007)))
#define CTRL_BIT_SCAN_CFT2                        ((u32)0x00000001 << 15)          /*!<R/W 0x0  scan QFN88 package ft mode2 */
#define CTRL_BIT_SCAN_CFT1                        ((u32)0x00000001 << 14)          /*!<R/W 0x0  scan QFN88 package ft mode1 */
#define CTRL_BIT_SCAN_CFT0                        ((u32)0x00000001 << 13)          /*!<R/W 0x0  scan QFN88 package ft mode0 */
#define CTRL_BIT_SCAN_BFT2                        ((u32)0x00000001 << 12)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode2 */
#define CTRL_BIT_SCAN_BFT1                        ((u32)0x00000001 << 11)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode1 */
#define CTRL_BIT_SCAN_BFT0                        ((u32)0x00000001 << 10)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode0 */
#define CTRL_BIT_SCAN_AFT2                        ((u32)0x00000001 << 9)          /*!<R/W 0x0  scan QFN144 MCM package ft mode2 */
#define CTRL_BIT_SCAN_AFT1                        ((u32)0x00000001 << 8)          /*!<R/W 0x0  scan QFN144 MCM package ft mode1 */
#define CTRL_BIT_SCAN_AFT0                        ((u32)0x00000001 << 7)          /*!<R/W 0x0  scan QFN144 MCM package ft mode0 */
#define CTRL_BIT_SCAN_CP1                         ((u32)0x00000001 << 6)          /*!<R/W 0x0  scan cp mode1 */
#define CTRL_BIT_SCAN_CP0                         ((u32)0x00000001 << 5)          /*!<R/W 0x0  scan cp mode0 */
#define CTRL_BIT_SCAN_SPC_DISABLE                 ((u32)0x00000001 << 4)          /*!<R/W 0x0  scan spc disable */
#define CTRL_BIT_SCAN_PLL_BYPASS                  ((u32)0x00000001 << 3)          /*!<R/W 0x0  scan pll bypass */
#define CTRL_BIT_SCAN_COMPRESS                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  scan compress */
#define CTRL_BIT_SCAN_PAR_LATPS                   ((u32)0x00000001 << 1)          /*!<R/W 0x0  Used as scan latch enable bit . If want to change scan par from func setting , set this bit to 1 first , then write func register will also effect scan shadow register . */
#define CTRL_BIT_SCAN_MODE_SETPS                  ((u32)0x00000001 << 0)          /*!<WA0 0x0  scan mode pulse */
/** @} */


/*==========MEM_CTRL Register Address Definition==========*/
#define REG_CTRL_MBIST_ALL_CTRL                      0x0000
#define REG_CTRL_MBIST_DONE_STA0                     0x0004
#define REG_CTRL_MBIST_START_PAUSE0                  0x0008
#define REG_CTRL_MBIST_NORM_FAIL0                    0x000C
#define REG_CTRL_MBIST_NORM_FAIL1                    0x0010
#define REG_CTRL_MBIST_NORM_FAIL2                    0x0014
#define REG_CTRL_MBIST_DRF_FAIL0                     0x0018
#define REG_CTRL_MBIST_DRF_FAIL1                     0x001C
#define REG_CTRL_MBIST_DRF_FAIL2                     0x0020
#define REG_CTRL_BT_BIST_DONE                        0x0024
#define REG_CTRL_BT_BIST_FAIL                        0x0028
#define REG_CTRL_BT_BIST_DRF_PASUE                   0x002C
#define REG_CTRL_MISR_DOUT_SEL                       0x0038
#define REG_CTRL_MBIST_ROM_MISR0                     0x003C
#define REG_CTRL_MBIST_ROM_MISR1                     0x0040
#define REG_CTRL_MBIST_ROM_MISR2                     0x0044
#define REG_CTRL_MBIST_ROM_MISR3                     0x0048
#define REG_CTRL_MBIST_ROM_MISR4                     0x004C
#define REG_CTRL_MBIST_ROM_MISR5                     0x0050
#define REG_CTRL_MBIST_ROM_MISR6                     0x0054
#define REG_CTRL_MBIST_ROM_MISR7                     0x0058
#define REG_CTRL_BT_BIST_RPT                         0x0060
#define REG_CTRL_CON_BT_E0_CTRL1                     0x0064
#define REG_CTRL_BT_FTC_MEM_CTRL                     0x0068
#define REG_CTRL_MBIST_GRP_EN_CTRL0                  0x0070
#define REG_CTRL_MBIST_GRP_EN_CTRL1                  0x0074
#define REG_CTRL_MBIST_GRP_EN_CTRL2                  0x0078
#define REG_CTRL_GRP_PWR_MSK0                        0x0080
#define REG_CTRL_GRP_PWR_MSK1                        0x0084
#define REG_CTRL_GRP_PWR_MSK2                        0x0088
#define REG_CTRL_RSEL0                               0x0090
#define REG_CTRL_RSEL1                               0x0094
#define REG_CTRL_RSEL2                               0x0098
#define REG_CTRL_RSEL3                               0x009C
#define REG_CTRL_RSEL4                               0x0100
#define REG_CTRL_RSEL5                               0x0104
#define REG_CTRL_RSEL6                               0x0108
#define REG_CTRL_ASEL0                               0x010C
#define REG_CTRL_ASEL1                               0x0110
#define REG_CTRL_ASEL2                               0x0114
#define REG_CTRL_GRP_HSYS_E0_CTRL0                   0x0134
#define REG_CTRL_GRP_HSYS_E1_CTRL0                   0x0138
#define REG_CTRL_GRP_LRET_E0_CTRL0                   0x013C
#define REG_CTRL_GRP_LSPMC_E0_CTRL0                  0x0140
#define REG_CTRL_GRP_AONPMC_E0_CTRL0                 0x0144
#define REG_CTRL_GRP_WLK4_E0_CTRL0                   0x0148
#define REG_CTRL_GRP_WPOFF_E0_CTRL0                  0x014C
#define REG_CTRL_GRP_WPOFF_E1_CTRL0                  0x0150
#define REG_CTRL_GRP_WPOFF_E2_CTRL0                  0x0154
#define REG_CTRL_GRP_WPOFF_E3_CTRL0                  0x0158
#define REG_CTRL_GRP_WPOFF_E4_CTRL0                  0x015C
#define REG_CTRL_GRP_WPOFF_E5_CTRL0                  0x0160
#define REG_CTRL_GRP_WPOFF_E6_CTRL0                  0x0164
#define REG_CTRL_GRP_WPON_E0_CTRL0                   0x0168
#define REG_CTRL_GRP_BTONK4_E0_CTRL0                 0x016C
#define REG_CTRL_GRP_BTONK4_E1_CTRL0                 0x0170
#define REG_CTRL_GRP_BTOFFK4_E0_CTRL0                0x0174
#define REG_CTRL_GRP_BTOFFK4_E1_CTRL0                0x0178
#define REG_CTRL_GRP_HIFI5_E0_CTRL0                  0x017C
#define REG_CTRL_GRP_HIFI5_E1_CTRL0                  0x0180
#define REG_CTRL_GRP_HIFI5_E2_CTRL0                  0x0184
#define REG_CTRL_GRP_HIFI5_E3_CTRL0                  0x0188
#define REG_CTRL_GRP_HIFI5_E4_CTRL0                  0x018C
#define REG_CTRL_GRP_HIFI5_E5_CTRL0                  0x0190
#define REG_CTRL_GRP_KM4_ITCM0_E0_CTRL0              0x0194
#define REG_CTRL_GRP_KM4_ICACHE_E0_CTRL0             0x0198
#define REG_CTRL_GRP_KM4_DCACHE_E0_CTRL0             0x019C
#define REG_CTRL_GRP_KR4_ITCM0_E0_CTRL0              0x01A0
#define REG_CTRL_GRP_KR4_ICACHE_E0_CTRL0             0x01A4
#define REG_CTRL_GRP_KR4_DCACHE_E0_CTRL0             0x01A8
#define REG_CTRL_GRP_BT_E0_CTRL0                     0x01AC
#define REG_CTRL_SCAN_CTRL1                          0x01C0
#define REG_CTRL_SCAN_CTRL0                          0x01C4
#define REG_CTRL_DUMMY_1F4                           0x01F4
#define REG_CTRL_DUMMY_1F8                           0x01F8
#define REG_CTRL_DUMMY_1FC                           0x01FC

/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/** @defgroup MEM_CTRL_LPS_define
  * @{
  */
#define BIT_MEM_CTRL_LPS_ACTIVE		0x0
#define BIT_MEM_CTRL_LPS_LS			0x1
#define BIT_MEM_CTRL_LPS_DS			0x2
#define BIT_MEM_CTRL_LPS_SD			0x4

/** @defgroup MEM_CTRL_LPS_define (BIT14 of LRET, BTOFF netlist if reverse, bit14 = 0 represents slp)
  * @{
  */
#define BIT_MEM_CTRL_LPS_ACTIVE_REV		0x4		//active
#define BIT_MEM_CTRL_LPS_LS_REV			0x5		//light sleep
#define BIT_MEM_CTRL_LPS_DS_REV			0x6		//deep sleep
#define BIT_MEM_CTRL_LPS_SD_REV			0x0		//shutdown


/** @defgroup MEM_CTRL_ACT_define
  * @{
  */
#define BIT_MEM_CTRL_ACT_ACTIVE		0x000
#define BIT_MEM_CTRL_ACT_LS			0x001
#define BIT_MEM_CTRL_ACT_DS			0x010
#define BIT_MEM_CTRL_ACT_SD			0x100


//Please add your defination here
#define SYSTEM_MEM_CTRL_BASE     0x4100C600

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
