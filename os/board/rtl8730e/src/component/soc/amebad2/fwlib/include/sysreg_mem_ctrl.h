#ifndef _AMEBAD2_MEM_CTRL_H_
#define _AMEBAD2_MEM_CTRL_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_CTRL_STAT
 * @brief MEMx MBIST control and status register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_STA                        ((u32)0x0000001F << 16)          /*!<R 0x0  [16]: MBIST START_PAUSE [17]: MBIST NRML_DONE [18]: MBIST NRML_FAIL [19]: MBIST DRF_DONE [20]: MBIST DRF_FAIL */
#define CTRL_MBIST_STA(x)                          ((u32)(((x) & 0x0000001F) << 16))
#define CTRL_GET_MBIST_STA(x)                      ((u32)(((x >> 16) & 0x0000001F)))
#define CTRL_BIT_WLBT_FORCE_MBIST                  ((u32)0x00000001 << 8)          /*!<R/W 0x0  select power mode signal of BT and WL memory when mbist testing: 1: reg 0: from BT/WL */
#define CTRL_MASK_MBIST_CTRL                       ((u32)0x000000FF << 0)          /*!<R/W 0x0  [0]: MBIST_CLK_EN [1]: MBIST_RSTB_ALL: MEMx bist reset of all memorys. [3:2]: MBIST_MSEL_ALL MEMx mbist method selection of all memorys 2'b01: normal 2'b10: drf 2'b11: normal+drf [4]: MBIST_RESUME [5]: MBIST_RESUME_PS [6]: MBIST_DYN_READ_EN [7]: MBIST_PHYCLK_STOP */
#define CTRL_MBIST_CTRL(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define CTRL_GET_MBIST_CTRL(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_GROUP_EN_0
 * @brief MEMx system memory group enable control register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_LSYS_E0_GRP_EN             ((u32)0x0000000F << 24)          /*!<R/W 4'hf   */
#define CTRL_MBIST_LSYS_E0_GRP_EN(x)               ((u32)(((x) & 0x0000000F) << 24))
#define CTRL_GET_MBIST_LSYS_E0_GRP_EN(x)           ((u32)(((x >> 24) & 0x0000000F)))
#define CTRL_MASK_MBIST_HPER_E0_GRP_EN             ((u32)0x00000003 << 22)          /*!<R/W 2'h3   */
#define CTRL_MBIST_HPER_E0_GRP_EN(x)               ((u32)(((x) & 0x00000003) << 22))
#define CTRL_GET_MBIST_HPER_E0_GRP_EN(x)           ((u32)(((x >> 22) & 0x00000003)))
#define CTRL_MASK_MBIST_WPOFF_E1_GRP_EN            ((u32)0x000003FF << 12)          /*!<R/W 10'h3ff   */
#define CTRL_MBIST_WPOFF_E1_GRP_EN(x)              ((u32)(((x) & 0x000003FF) << 12))
#define CTRL_GET_MBIST_WPOFF_E1_GRP_EN(x)          ((u32)(((x >> 12) & 0x000003FF)))
#define CTRL_MASK_MBIST_WLK4_E0_GRP_EN             ((u32)0x00000003 << 10)          /*!<R/W 2'h3   */
#define CTRL_MBIST_WLK4_E0_GRP_EN(x)               ((u32)(((x) & 0x00000003) << 10))
#define CTRL_GET_MBIST_WLK4_E0_GRP_EN(x)           ((u32)(((x >> 10) & 0x00000003)))
#define CTRL_MASK_MBIST_HSYS_E0_GRP_EN             ((u32)0x000000FF << 0)          /*!<R/W 8'hff   */
#define CTRL_MBIST_HSYS_E0_GRP_EN(x)               ((u32)(((x) & 0x000000FF) << 0))
#define CTRL_GET_MBIST_HSYS_E0_GRP_EN(x)           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_GROUP_EN_1
 * @brief MEMx system meory group enable control register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_DDRC_BSTC_E0_GRP_EN        ((u32)0x0000000F << 9)          /*!<R/W 4'hf   */
#define CTRL_MBIST_DDRC_BSTC_E0_GRP_EN(x)          ((u32)(((x) & 0x0000000F) << 9))
#define CTRL_GET_MBIST_DDRC_BSTC_E0_GRP_EN(x)      ((u32)(((x >> 9) & 0x0000000F)))
#define CTRL_MASK_MBIST_BTONK4_E1_GRP_EN           ((u32)0x00000003 << 5)          /*!<R/W 2'h3   */
#define CTRL_MBIST_BTONK4_E1_GRP_EN(x)             ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_MBIST_BTONK4_E1_GRP_EN(x)         ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_MASK_MBIST_BTONK4_E0_GRP_EN           ((u32)0x00000003 << 3)          /*!<R/W 2'h3   */
#define CTRL_MBIST_BTONK4_E0_GRP_EN(x)             ((u32)(((x) & 0x00000003) << 3))
#define CTRL_GET_MBIST_BTONK4_E0_GRP_EN(x)         ((u32)(((x >> 3) & 0x00000003)))
#define CTRL_MASK_MBIST_HPER_E1_GRP_EN             ((u32)0x00000007 << 0)          /*!<R/W 3'h7   */
#define CTRL_MBIST_HPER_E1_GRP_EN(x)               ((u32)(((x) & 0x00000007) << 0))
#define CTRL_GET_MBIST_HPER_E1_GRP_EN(x)           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_GROUP_EN_2
 * @brief MEMx system meory group enable control register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_KM4_DCACHE_E0_GRP_EN       ((u32)0x00000007 << 26)          /*!<R/W 3'h7   */
#define CTRL_MBIST_KM4_DCACHE_E0_GRP_EN(x)         ((u32)(((x) & 0x00000007) << 26))
#define CTRL_GET_MBIST_KM4_DCACHE_E0_GRP_EN(x)     ((u32)(((x >> 26) & 0x00000007)))
#define CTRL_MASK_MBIST_KM4_ICACHE_E0_GRP_EN       ((u32)0x0000001F << 21)          /*!<R/W 5'h1f   */
#define CTRL_MBIST_KM4_ICACHE_E0_GRP_EN(x)         ((u32)(((x) & 0x0000001F) << 21))
#define CTRL_GET_MBIST_KM4_ICACHE_E0_GRP_EN(x)     ((u32)(((x >> 21) & 0x0000001F)))
#define CTRL_MASK_MBIST_KM4_ITCM0_E0_GRP_EN        ((u32)0x000007FF << 10)          /*!<R/W 11'h7ff   */
#define CTRL_MBIST_KM4_ITCM0_E0_GRP_EN(x)          ((u32)(((x) & 0x000007FF) << 10))
#define CTRL_GET_MBIST_KM4_ITCM0_E0_GRP_EN(x)      ((u32)(((x >> 10) & 0x000007FF)))
#define CTRL_MASK_MBIST_KM0_DCACHE_E0_GRP_EN       ((u32)0x00000003 << 4)          /*!<R/W 2'h3   */
#define CTRL_MBIST_KM0_DCACHE_E0_GRP_EN(x)         ((u32)(((x) & 0x00000003) << 4))
#define CTRL_GET_MBIST_KM0_DCACHE_E0_GRP_EN(x)     ((u32)(((x >> 4) & 0x00000003)))
#define CTRL_MASK_MBIST_KM0_ICACHE_E0_GRP_EN       ((u32)0x00000003 << 2)          /*!<R/W 2'h3   */
#define CTRL_MBIST_KM0_ICACHE_E0_GRP_EN(x)         ((u32)(((x) & 0x00000003) << 2))
#define CTRL_GET_MBIST_KM0_ICACHE_E0_GRP_EN(x)     ((u32)(((x >> 2) & 0x00000003)))
#define CTRL_MASK_MBIST_KM0_ITCM0_E0_GRP_EN        ((u32)0x00000003 << 0)          /*!<R/W 2'h3   */
#define CTRL_MBIST_KM0_ITCM0_E0_GRP_EN(x)          ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_MBIST_KM0_ITCM0_E0_GRP_EN(x)      ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_RSTB_0
 * @brief MEMx MBIST reset control register [31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_RSTB_31_0                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  valid MBIST_RSTB range is [MBIST_GRP_NUM-1:0]. MBIST_GRP_NUM = 34, they are: *system memory group: 25 *bt special group: 1 *km0_e0 group : 3 *km4_e0 group : 3 *mipic_e0 group: 2 */
#define CTRL_MBIST_RSTB_31_0(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_RSTB_31_0(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_RSTB_LOOP_MODE_1
 * @brief MEMx MBIST reset & loop_mode control register [47:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_LOOP_MODE_47_32            ((u32)0x0000FFFF << 16)          /*!<R/W 16'h0  valid MBIST_LOOP_MODE range is [MBIST_GRP_NUM-1:0]. */
#define CTRL_MBIST_LOOP_MODE_47_32(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define CTRL_GET_MBIST_LOOP_MODE_47_32(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define CTRL_MASK_MBIST_RSTB_47_32                 ((u32)0x0000FFFF << 0)          /*!<R/W 16'hffff  valid MBIST_RSTB range is [MBIST_GRP_NUM-1:0]. MBIST_GRP_NUM = 34, they are: *system memory group: 25 *bt special group: 1 *km0_e0 group : 3 *km4_e0 group : 3 *mipic_e0 group: 2 */
#define CTRL_MBIST_RSTB_47_32(x)                   ((u32)(((x) & 0x0000FFFF) << 0))
#define CTRL_GET_MBIST_RSTB_47_32(x)               ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_LOOP_MODE_0
 * @brief MEMx MBIST loop mode controlregister
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_LOOP_MODE_31_0             ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'h0  valid MBIST_LOOP_MODE range is [MBIST_GRP_NUM-1:0] */
#define CTRL_MBIST_LOOP_MODE_31_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_LOOP_MODE_31_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_MSEL_0
 * @brief MEMx MBIST method select register control bit[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_MSEL_31_0                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  valid MBIST_MSEL range is [MBIST_GRP_NUM*2-1:0] MBIST_MSEL[2*X] = 1 eanbles BIST_MODE mode of groupX; MBIST_MSEL[2*X+1] = 1 eanbles DRF_BIST_MODE mode of groupX. */
#define CTRL_MBIST_MSEL_31_0(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_MSEL_31_0(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_MSEL_1
 * @brief MEMx MBIST method select control register bit[63:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_MSEL_63_32                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  valid MBIST_MSEL range is [MBIST_GRP_NUM*2-1:0] MBIST_MSEL[2*X] = 1 eanbles BIST_MODE mode of groupX; MBIST_MSEL[2*X+1] = 1 eanbles DRF_BIST_MODE mode of groupX. */
#define CTRL_MBIST_MSEL_63_32(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_MSEL_63_32(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_MSEL_2
 * @brief MEMx MBIST method select control register bit[95:64]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_MSEL_95_64                 ((u32)0xFFFFFFFF << 0)          /*!<R/W 32'hffffffff  valid MBIST_MSEL range is [MBIST_GRP_NUM*2-1:0] MBIST_MSEL[2*X] = 1 eanbles BIST_MODE mode of groupX; MBIST_MSEL[2*X+1] = 1 eanbles DRF_BIST_MODE mode of groupX. */
#define CTRL_MBIST_MSEL_95_64(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_MSEL_95_64(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_START_PAUSE_STAT
 * @brief MEMx mbist start pause status register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_START_PAUSE_STAT           ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_START_PAUSE_STAT(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_START_PAUSE_STAT(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_DONE_STAT
 * @brief MEMx MBIST done status register
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_DONE_STAT                  ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_DONE_STAT(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_DONE_STAT(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_NRML_STAT_0
 * @brief MEMx MBIST fail status register bit[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_NRML_STAT_31_0        ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_NRML_STAT_31_0(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_NRML_STAT_31_0(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_NRML_STAT_1
 * @brief MEMx MBIST fail status register bit[63:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_NRML_STAT_63_32       ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_NRML_STAT_63_32(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_NRML_STAT_63_32(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_NRML_STAT_2
 * @brief MEMx MBIST fail status register bit[95:64]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_NRML_STAT_95_64       ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_NRML_STAT_95_64(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_NRML_STAT_95_64(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_DRF_STAT_0
 * @brief MEMx drf mbist fail status register bit[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_DRF_STAT_31_0         ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_DRF_STAT_31_0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_DRF_STAT_31_0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_DRF_STAT_1
 * @brief MEMx drf mbist fail status register bit[63:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_DRF_STAT_63_32        ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_DRF_STAT_63_32(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_DRF_STAT_63_32(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MBIST_FAIL_DRF_STAT_2
 * @brief MEMx drf mbist fail status register bit[95:64]
 * @{
 *****************************************************************************/
#define CTRL_MASK_MBIST_FAIL_DRF_STAT_95_64        ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_MBIST_FAIL_DRF_STAT_95_64(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_MBIST_FAIL_DRF_STAT_95_64(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_LSPMC_E0_MISR_DOUT_0
 * @brief lspmc_e0 ROM mbist misr_dout[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_LSPMC_E0_MISR_DOUT_31_0      ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_ROM_LSPMC_E0_MISR_DOUT_31_0(x)        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_LSPMC_E0_MISR_DOUT_31_0(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_LSPMC_E0_MISR_DOUT_1
 * @brief lspmc_e0 ROM mbist misr_dout[35:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_LSPMC_E0_MISR_DOUT_35_32     ((u32)0x0000000F << 0)          /*!<R 0x0   */
#define CTRL_ROM_LSPMC_E0_MISR_DOUT_35_32(x)       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_ROM_LSPMC_E0_MISR_DOUT_35_32(x)   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_AONPMC_E0_MISR_DOUT_0
 * @brief aonpmc_e0 ROM mbist misr_dout[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_AONPMC_E0_MISR_DOUT_31_0     ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_ROM_AONPMC_E0_MISR_DOUT_31_0(x)       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_AONPMC_E0_MISR_DOUT_31_0(x)   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_AONPMC_E0_MISR_DOUT_1
 * @brief aonpmc_e0 ROM mbist misr_dout[35:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_AONPMC_E0_MISR_DOUT_35_32    ((u32)0x0000000F << 0)          /*!<R 0x0   */
#define CTRL_ROM_AONPMC_E0_MISR_DOUT_35_32(x)      ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_ROM_AONPMC_E0_MISR_DOUT_35_32(x)  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_CA7BOOT_E0_MISR_DOUT_0
 * @brief catboot_e0 ROM mbist misr_dout[31:0]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_CA7BOOT_E0_MISR_DOUT_31_0    ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_ROM_CA7BOOT_E0_MISR_DOUT_31_0(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_CA7BOOT_E0_MISR_DOUT_31_0(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_ROM_CA7BOOT_E0_MISR_DOUT_1
 * @brief catboot_e0 ROM mbist misr_dout[63:32]
 * @{
 *****************************************************************************/
#define CTRL_MASK_ROM_CA7BOOT_E0_MISR_DOUT_63_32   ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_ROM_CA7BOOT_E0_MISR_DOUT_63_32(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_ROM_CA7BOOT_E0_MISR_DOUT_63_32(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_LRET_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_LRET_E0_SLP                       ((u32)0x00000001 << 14)          /*!<R/W/ES 1'b0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_LRET_E0_RET                       ((u32)0x00000001 << 13)          /*!<R/W/ES 1'b0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_LRET_E0_NAP                       ((u32)0x00000001 << 12)          /*!<R/W/ES 1'b0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_LRET_E0_RAE                       ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, 0.8V */
#define CTRL_MASK_LRET_E0_WAEM                     ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2] x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_LRET_E0_WAEM(x)                       ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_LRET_E0_WAEM(x)                   ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_LRET_E0_WAE                       ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, 0.8V */
#define CTRL_BIT_LRET_E0_DVSE                      ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_BIT_LRET_E0_TMCK                      ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_MASK_LRET_E0_DVS                      ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_LRET_E0_DVS(x)                        ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_LRET_E0_DVS(x)                    ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_LSYS_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_LSYS_SLP_SLEP                     ((u32)0x00000001 << 26)          /*!<R/W 0x0  deep sleep mode. SW control: set this register before chip goes to SLEP HW control: when chip goes to SLEP,PMC will load this reg value to LSYS_E0_SLP/LRET_E0_SLP/KM0_DCACHE_DATA_SLP/KM0_DCACHE_TAG_SLP/KM0_ICACHE_DATA_SLP/KM0_ICACHE_TAG_SLP (set_pst_lmem=1 & val_pst_lmem=0) */
#define CTRL_BIT_LSYS_RET_SLEP                     ((u32)0x00000001 << 25)          /*!<R/W 0x1  deep sleep mode. SW control: set this register before chip goes to SLEP HW control: when chip goes to SLEP,PMC will load this reg value to LSYS_E0_RET/LRET_E0_RET/KM0_DCACHE_DATA_RET/KM0_DCACHE_TAG_RET/KM0_ICACHE_DATA_RET/KM0_ICACHE_TAG_RET (set_pst_lmem=1 & val_pst_lmem=0) */
#define CTRL_BIT_LSYS_NAP_SLEP                     ((u32)0x00000001 << 24)          /*!<R/W 0x0  deep sleep mode. SW control: set this register before chip goes to SLEP HW control: when chip goes to SLEP,PMC will load this reg value to LSYS_E0_NAP/LRET_E0_NAP/KM0_DCACHE_DATA_NAP/KM0_DCACHE_TAG_NAP/KM0_ICACHE_DATA_NAP/KM0_ICACHE_TAG_NAP (set_pst_lmem=1 & val_pst_lmem=0)when chip goes to SLEP */
#define CTRL_BIT_LSYS_SLP_NORM                     ((u32)0x00000001 << 22)          /*!<R/W 0x0  Active mode. HW control: when chip goes to ACTIVE, PMC will load this reg value to LSYS_E0_SLP/LRET_E0_SLP/KM0_DCACHE_DATA_SLP/KM0_DCACHE_TAG_SLP/KM0_ICACHE_DATA_SLP/KM0_ICACHE_TAG_SLP (set_pst_lmem=1 & val_pst_lmem=1). */
#define CTRL_BIT_LSYS_RET_NORM                     ((u32)0x00000001 << 21)          /*!<R/W 0x0  Active mode. HW control: when chip goes to ACTIVE, PMC will load this reg value to LSYS_E0_RET/LRET_E0_RET/KM0_DCACHE_DATA_RET/KM0_DCACHE_TAG_RET/KM0_ICACHE_DATA_RET/KM0_ICACHE_TAG_RET (set_pst_lmem=1 & val_pst_lmem=1). */
#define CTRL_BIT_LSYS_NAP_NORM                     ((u32)0x00000001 << 20)          /*!<R/W 0x0  Active mode. HW control: when chip goes to ACTIVE, PMC will load this reg value to LSYS_E0_NAP/LRET_E0_NAP/KM0_DCACHE_DATA_NAP/KM0_DCACHE_TAG_NAP/KM0_ICACHE_DATA_NAP/KM0_ICACHE_TAG_NAP (set_pst_lmem=1 & val_pst_lmem=1). */
#define CTRL_BIT_LSYS_E0_SLP                       ((u32)0x00000001 << 16)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_LSYS_E0_RET                       ((u32)0x00000001 << 15)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_LSYS_E0_NAP                       ((u32)0x00000001 << 14)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_LSYS_E0_RAE_0P8                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_LSYS_E0_WAEM_0P8                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_LSYS_E0_WAEM_0P8(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_LSYS_E0_WAEM_0P8(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_LSYS_E0_WAE_0P8                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_LSYS_E0_TMCK_0P8                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_LSYS_E0_DVSE_0P8                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_LSYS_E0_DVS_0P8                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_LSYS_E0_DVS_0P8(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_LSYS_E0_DVS_0P8(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_LSYS_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_LSYS_E0_RAE_1P0                   ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_LSYS_E0_WAEM_1P0                 ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V */
#define CTRL_LSYS_E0_WAEM_1P0(x)                   ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_LSYS_E0_WAEM_1P0(x)               ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_LSYS_E0_WAE_1P0                   ((u32)0x00000001 << 20)          /*!<R/W 1'b0  *WAEM[3:0] x 0.8V: 4'b1010 */
#define CTRL_BIT_LSYS_E0_TMCK_1P0                  ((u32)0x00000001 << 19)          /*!<R/W 1'b0  *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BIT_LSYS_E0_DVSE_1P0                  ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_LSYS_E0_DVS_1P0                  ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_LSYS_E0_DVS_1P0(x)                    ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_LSYS_E0_DVS_1P0(x)                ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_LSYS_E0_RAE_0P9                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_LSYS_E0_WAEM_0P9                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_LSYS_E0_WAEM_0P9(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_LSYS_E0_WAEM_0P9(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_LSYS_E0_WAE_0P9                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_LSYS_E0_TMCK_0P9                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_LSYS_E0_DVSE_0P9                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_LSYS_E0_DVS_0P9                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_LSYS_E0_DVS_0P9(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_LSYS_E0_DVS_0P9(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HSYS_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HSYS_E0_SLP                       ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_HSYS_E0_RET                       ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_HSYS_E0_NAP                       ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_HSYS_E0_RAE_0P8                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_HSYS_E0_WAEM_0P8                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_HSYS_E0_WAEM_0P8(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_HSYS_E0_WAEM_0P8(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_HSYS_E0_WAE_0P8                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_HSYS_E0_TMCK_0P8                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_HSYS_E0_DVSE_0P8                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_HSYS_E0_DVS_0P8                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_HSYS_E0_DVS_0P8(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_HSYS_E0_DVS_0P8(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HSYS_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HSYS_E0_RAE_1P0                   ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_HSYS_E0_WAEM_1P0                 ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_HSYS_E0_WAEM_1P0(x)                   ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_HSYS_E0_WAEM_1P0(x)               ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_HSYS_E0_WAE_1P0                   ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_HSYS_E0_TMCK_1P0                  ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_HSYS_E0_DVSE_1P0                  ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_HSYS_E0_DVS_1P0                  ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_HSYS_E0_DVS_1P0(x)                    ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_HSYS_E0_DVS_1P0(x)                ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_HSYS_E0_RAE_0P9                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_HSYS_E0_WAEM_0P9                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_HSYS_E0_WAEM_0P9(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_HSYS_E0_WAEM_0P9(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_HSYS_E0_WAE_0P9                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_HSYS_E0_TMCK_0P9                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_HSYS_E0_DVSE_0P9                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_HSYS_E0_DVS_0P9                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_HSYS_E0_DVS_0P9(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_HSYS_E0_DVS_0P9(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_CA7BOOT_E0_FTC_ULL_ROM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_CA7BOOT_E0_SLP                    ((u32)0x00000001 << 5)          /*!<R/W 0x0  FTC_ULL+ SPROM, PG bit2, not used in btrom */
#define CTRL_BIT_CA7BOOT_E0_DVSE                   ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULL+ SPROM */
#define CTRL_MASK_CA7BOOT_E0_DVS                   ((u32)0x0000000F << 0)          /*!<R/W 0x3  FTC_ULL+ SPROM */
#define CTRL_CA7BOOT_E0_DVS(x)                     ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_CA7BOOT_E0_DVS(x)                 ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WLK4_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WLK4_E0_SLP                       ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_WLK4_E0_RET                       ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_WLK4_E0_NAP                       ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_WLK4_E0_RAE_0P8                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_WLK4_E0_WAEM_0P8                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_WLK4_E0_WAEM_0P8(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WLK4_E0_WAEM_0P8(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WLK4_E0_WAE_0P8                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_WLK4_E0_TMCK_0P8                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WLK4_E0_DVSE_0P8                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WLK4_E0_DVS_0P8                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WLK4_E0_DVS_0P8(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WLK4_E0_DVS_0P8(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WLK4_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WLK4_E0_RAE_1P0                   ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_WLK4_E0_WAEM_1P0                 ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_WLK4_E0_WAEM_1P0(x)                   ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_WLK4_E0_WAEM_1P0(x)               ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_WLK4_E0_WAE_1P0                   ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_WLK4_E0_TMCK_1P0                  ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WLK4_E0_DVSE_1P0                  ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WLK4_E0_DVS_1P0                  ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WLK4_E0_DVS_1P0(x)                    ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_WLK4_E0_DVS_1P0(x)                ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_WLK4_E0_RAE_0P9                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_WLK4_E0_WAEM_0P9                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_WLK4_E0_WAEM_0P9(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WLK4_E0_WAEM_0P9(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WLK4_E0_WAE_0P9                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_WLK4_E0_TMCK_0P9                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WLK4_E0_DVSE_0P9                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WLK4_E0_DVS_0P9                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WLK4_E0_DVS_0P9(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WLK4_E0_DVS_0P9(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E0_SLP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_WPOFF_E0_RET                      ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_WPOFF_E0_NAP                      ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_WPOFF_E0_RAE_0P8                  ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_WPOFF_E0_WAEM_0P8                ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPOFF_E0_WAEM_0P8(x)                  ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WPOFF_E0_WAEM_0P8(x)              ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E0_WAE_0P8                  ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_WPOFF_E0_TMCK_0P8                 ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPOFF_E0_DVSE_0P8                 ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPOFF_E0_DVS_0P8                 ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPOFF_E0_DVS_0P8(x)                   ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WPOFF_E0_DVS_0P8(x)               ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E0_RAE_1P0                  ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_WPOFF_E0_WAEM_1P0                ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPOFF_E0_WAEM_1P0(x)                  ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_WPOFF_E0_WAEM_1P0(x)              ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E0_WAE_1P0                  ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_WPOFF_E0_TMCK_1P0                 ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPOFF_E0_DVSE_1P0                 ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPOFF_E0_DVS_1P0                 ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPOFF_E0_DVS_1P0(x)                   ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_WPOFF_E0_DVS_1P0(x)               ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_WPOFF_E0_RAE_0P9                  ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_WPOFF_E0_WAEM_0P9                ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPOFF_E0_WAEM_0P9(x)                  ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WPOFF_E0_WAEM_0P9(x)              ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E0_WAE_0P9                  ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_WPOFF_E0_TMCK_0P9                 ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPOFF_E0_DVSE_0P9                 ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPOFF_E0_DVS_0P9                 ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPOFF_E0_DVS_0P9(x)                   ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WPOFF_E0_DVS_0P9(x)               ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E1_FTC_ULP_RW
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E1_WAE_0P8                  ((u32)0x00000001 << 16)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V WAE=1: Turn on the write assist circuit. */
#define CTRL_BIT_WPOFF_E1_RAE_0P8                  ((u32)0x00000001 << 15)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V RAE=1: Turn on the read assist circuit. */
#define CTRL_BIT_WPOFF_E1_NAP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW: pg_b0 CSB=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_WPOFF_E1_TMCK                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW Internal timing bypass signal. TMCK=1: WL pulse will be controlled by the clock duty cycle. TMCK=0: default value, RAM access normally. */
#define CTRL_BIT_WPOFF_E1_WAE_0P9_1P0              ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW WAE=1: Turn on the write assist circuit. WAE=0: Turn off the write assist circuit. */
#define CTRL_MASK_WPOFF_E1_WAEM                    ((u32)0x0000000F << 8)          /*!<R/W 0xA  FTC_ULP_SPRAM_RW WAEM[3:2] controls the write assist circuit timing margin when WAE=1. WAEM[3:2]=00: turn off the time margin of the write assist circuit. WAEM[3:2]=01: Lossest timing margin WAEM[3:2]=10: WAEM[3:2]=11: Tightest timing margin. WAEM[1:0] controls the write assist circuit voltage margin when WAE=1. WAEM[1:0]=00: Turn off the voltage margin of the write assist circuit WAEM[1:0]=01: Minimum voltage margin WAEM[1:0]=10: WAEM[1:0]=11: Maximum voltage margin. */
#define CTRL_WPOFF_E1_WAEM(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_WPOFF_E1_WAEM(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E1_RAE_0P9_1P0              ((u32)0x00000001 << 7)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW RAE=1: turn on the read assist circuit. RAE=0: turn off the read assist circuit. */
#define CTRL_MASK_WPOFF_E1_RAEM                    ((u32)0x00000003 << 5)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW adjust the margin of read assist circuit when RAE=1. RAEM=00: default WL voltage drop RAEM=01 RAEM=10 RAEM=11: maximum WL voltage drop */
#define CTRL_WPOFF_E1_RAEM(x)                      ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_WPOFF_E1_RAEM(x)                  ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_BIT_WPOFF_E1_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_SPRAM_RW DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_WPOFF_E1_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xC  FTC_ULP_SPRAM_RW DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 ... */
#define CTRL_WPOFF_E1_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E1_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E2_FTC_ULP_RW_TPRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E2_WAE_0P8                  ((u32)0x00000001 << 16)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V WAE=1: Turn on the write assist circuit. */
#define CTRL_BIT_WPOFF_E2_RAE_0P8                  ((u32)0x00000001 << 15)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V RAE=1: Turn on the read assist circuit. */
#define CTRL_BIT_WPOFF_E2_NAP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM:pg_b0 CSA(B)=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_WPOFF_E2_TMCK                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_BIT_WPOFF_E2_WAE_0P9_1P0              ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E2_WAEM                    ((u32)0x0000000F << 8)          /*!<R/W 0xA  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E2_WAEM(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_WPOFF_E2_WAEM(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E2_RAE_0P9_1P0              ((u32)0x00000001 << 7)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E2_RAEM                    ((u32)0x00000003 << 5)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E2_RAEM(x)                      ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_WPOFF_E2_RAEM(x)                  ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_BIT_WPOFF_E2_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E2_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xA  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E2_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E2_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E3_FTC_ULP_RW
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E3_WAE_0P8                  ((u32)0x00000001 << 16)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V WAE=1: Turn on the write assist circuit. */
#define CTRL_BIT_WPOFF_E3_RAE_0P8                  ((u32)0x00000001 << 15)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V RAE=1: Turn on the read assist circuit. */
#define CTRL_BIT_WPOFF_E3_NAP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULP_RW_RF: pg_b0 CSB=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_WPOFF_E3_TMCK                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULP_RW_RF Internal timing bypass signal. TMCK=1: WL pulse will be controlled by the clock duty cycle. TMCK=0: default value, RAM access normally. */
#define CTRL_BIT_WPOFF_E3_WAE_0P9_1P0              ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULP_RW_RF WAE=1: Turn on the write assist circuit. WAE=0: Turn off the write assist circuit. */
#define CTRL_MASK_WPOFF_E3_WAEM                    ((u32)0x0000000F << 8)          /*!<R/W 0xA  FTC_ULP_RW_RF WAEM[3:2] controls the write assist circuit timing margin when WAE=1. WAEM[3:2]=00: turn off the time margin of the write assist circuit. WAEM[3:2]=01: Lossest timing margin WAEM[3:2]=10: WAEM[3:2]=11: Tightest timing margin. WAEM[1:0] controls the write assist circuit voltage margin when WAE=1. WAEM[1:0]=00: Turn off the voltage margin of the write assist circuit WAEM[1:0]=01: Minimum voltage margin WAEM[1:0]=10: WAEM[1:0]=11: Maximum voltage margin. */
#define CTRL_WPOFF_E3_WAEM(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_WPOFF_E3_WAEM(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E3_RAE_0P9_1P0              ((u32)0x00000001 << 7)          /*!<R/W 0x0  FTC_ULP_RW_RF RAE=1: turn on the read assist circuit. RAE=0: turn off the read assist circuit. */
#define CTRL_MASK_WPOFF_E3_RAEM                    ((u32)0x00000003 << 5)          /*!<R/W 0x0  FTC_ULP_RW_RF adjust the margin of read assist circuit when RAE=1. RAEM=00: default WL voltage drop RAEM=01 RAEM=10 RAEM=11: maximum WL voltage drop */
#define CTRL_WPOFF_E3_RAEM(x)                      ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_WPOFF_E3_RAEM(x)                  ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_BIT_WPOFF_E3_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_RW_RF DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_WPOFF_E3_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xC  FTC_ULP_RW_RF DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_WPOFF_E3_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E3_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E4_FTC_ULP_RW
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E4_WAE_0P8                  ((u32)0x00000001 << 16)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V WAE=1: Turn on the write assist circuit. */
#define CTRL_BIT_WPOFF_E4_RAE_0P8                  ((u32)0x00000001 << 15)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V RAE=1: Turn on the read assist circuit. */
#define CTRL_BIT_WPOFF_E4_NAP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULP_RW_RF: pg_b0 CSB=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_WPOFF_E4_TMCK                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULP_RW_RF Internal timing bypass signal. TMCK=1: WL pulse will be controlled by the clock duty cycle. TMCK=0: default value, RAM access normally. */
#define CTRL_BIT_WPOFF_E4_WAE_0P9_1P0              ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULP_RW_RF WAE=1: Turn on the write assist circuit. WAE=0: Turn off the write assist circuit. */
#define CTRL_MASK_WPOFF_E4_WAEM                    ((u32)0x0000000F << 8)          /*!<R/W 0xA  FTC_ULP_RW_RF WAEM[3:2] controls the write assist circuit timing margin when WAE=1. WAEM[3:2]=00: turn off the time margin of the write assist circuit. WAEM[3:2]=01: Lossest timing margin WAEM[3:2]=10: WAEM[3:2]=11: Tightest timing margin. WAEM[1:0] controls the write assist circuit voltage margin when WAE=1. WAEM[1:0]=00: Turn off the voltage margin of the write assist circuit WAEM[1:0]=01: Minimum voltage margin WAEM[1:0]=10: WAEM[1:0]=11: Maximum voltage margin. */
#define CTRL_WPOFF_E4_WAEM(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_WPOFF_E4_WAEM(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E4_RAE_0P9_1P0              ((u32)0x00000001 << 7)          /*!<R/W 0x0  FTC_ULP_RW_RF RAE=1: turn on the read assist circuit. RAE=0: turn off the read assist circuit. */
#define CTRL_MASK_WPOFF_E4_RAEM                    ((u32)0x00000003 << 5)          /*!<R/W 0x0  FTC_ULP_RW_RF adjust the margin of read assist circuit when RAE=1. RAEM=00: default WL voltage drop RAEM=01 RAEM=10 RAEM=11: maximum WL voltage drop */
#define CTRL_WPOFF_E4_RAEM(x)                      ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_WPOFF_E4_RAEM(x)                  ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_BIT_WPOFF_E4_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_RW_RF DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_WPOFF_E4_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xC  FTC_ULP_RW_RF DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_WPOFF_E4_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E4_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E5_FTC_ULP_TPSRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E5_SLP                      ((u32)0x00000001 << 8)          /*!<R/W 0x0   */
#define CTRL_BIT_WPOFF_E5_RET                      ((u32)0x00000001 << 7)          /*!<R/W 0x0   */
#define CTRL_BIT_WPOFF_E5_NAP                      ((u32)0x00000001 << 6)          /*!<R/W 0x0   */
#define CTRL_BIT_WPOFF_E5_TMCK                     ((u32)0x00000001 << 5)          /*!<R/W 0x0  FTC_ULP_2P_RF */
#define CTRL_BIT_WPOFF_E5_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_2P_RF */
#define CTRL_MASK_WPOFF_E5_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xA  FTC_ULP_2P_RF */
#define CTRL_WPOFF_E5_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E5_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPOFF_E6_FTC_ULP_RW_PG_TPRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPOFF_E6_WAE_0P8                  ((u32)0x00000001 << 18)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V WAE=1: Turn on the write assist circuit. */
#define CTRL_BIT_WPOFF_E6_RAE_0P8                  ((u32)0x00000001 << 17)          /*!<R/W 1'b1  FTC_ULP_SPRAM_RW,0.8V RAE=1: Turn on the read assist circuit. */
#define CTRL_BIT_WPOFF_E6_SLP                      ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM:pg_b2 */
#define CTRL_BIT_WPOFF_E6_RET                      ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM:pg_b1 */
#define CTRL_BIT_WPOFF_E6_NAP                      ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM:pg_b0 CSA(B)=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_WPOFF_E6_TMCK                     ((u32)0x00000001 << 13)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_BIT_WPOFF_E6_WAE_0P9_1P0              ((u32)0x00000001 << 12)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E6_WAEM                    ((u32)0x0000000F << 8)          /*!<R/W 0xA  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E6_WAEM(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define CTRL_GET_WPOFF_E6_WAEM(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define CTRL_BIT_WPOFF_E6_RAE_0P9_1P0              ((u32)0x00000001 << 7)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E6_RAEM                    ((u32)0x00000003 << 5)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E6_RAEM(x)                      ((u32)(((x) & 0x00000003) << 5))
#define CTRL_GET_WPOFF_E6_RAEM(x)                  ((u32)(((x >> 5) & 0x00000003)))
#define CTRL_BIT_WPOFF_E6_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_RW_TPRAM */
#define CTRL_MASK_WPOFF_E6_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xA  FTC_ULP_RW_TPRAM */
#define CTRL_WPOFF_E6_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_WPOFF_E6_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPON_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPON_E0_SLP                       ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_WPON_E0_RET                       ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_WPON_E0_NAP                       ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_WPON_E0_RAE_0P8                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_WPON_E0_WAEM_0P8                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPON_E0_WAEM_0P8(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WPON_E0_WAEM_0P8(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WPON_E0_WAE_0P8                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_WPON_E0_TMCK_0P8                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPON_E0_DVSE_0P8                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPON_E0_DVS_0P8                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPON_E0_DVS_0P8(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WPON_E0_DVS_0P8(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_WPON_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_WPON_E0_RAE_1P0                   ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_WPON_E0_WAEM_1P0                 ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPON_E0_WAEM_1P0(x)                   ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_WPON_E0_WAEM_1P0(x)               ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_WPON_E0_WAE_1P0                   ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_WPON_E0_TMCK_1P0                  ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPON_E0_DVSE_1P0                  ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPON_E0_DVS_1P0                  ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPON_E0_DVS_1P0(x)                    ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_WPON_E0_DVS_1P0(x)                ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_WPON_E0_RAE_0P9                   ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_WPON_E0_WAEM_0P9                 ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_WPON_E0_WAEM_0P9(x)                   ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_WPON_E0_WAEM_0P9(x)               ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_WPON_E0_WAE_0P9                   ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_WPON_E0_TMCK_0P9                  ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_WPON_E0_DVSE_0P9                  ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_WPON_E0_DVS_0P9                  ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_WPON_E0_DVS_0P9(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_WPON_E0_DVS_0P9(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HPER_E0_SNPS_ULP_SPRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HPER_E0_LS                        ((u32)0x00000001 << 6)          /*!<R/W 0x0  SNPS_ULP_SPRAM: pg_b0 LS=1 ram is in light sleep mode, memory array's data retained. */
#define CTRL_BIT_HPER_E0_TEST1                     ((u32)0x00000001 << 5)          /*!<R/W 0x0  SNPS_ULP_SPRAM Test1 pin to bypass self-timed circuit. The external clock controls the read and write controls. The self time bypass mode occpuied High phase of external clock. The self time bypass mode may used to determine time margin of internal self-timed circuitry. */
#define CTRL_BIT_HPER_E0_RME                       ((u32)0x00000001 << 4)          /*!<R/W 0x0  SNPS_ULP_SPRAM RME selects between internal and external RM[2:0] settings, RM[3] is not controlled by RME. RME=1: select external RM[2:0] RME=0: select internal RM[2:0] */
#define CTRL_MASK_HPER_E0_RM                       ((u32)0x0000000F << 0)          /*!<R/W 0x2  SNPS_ULP_SPRAM RM[1:0] are used to trade-off betwwen speed and robustness(yield). 2'b11: fast 2'b10: default 2'b01: slow 2'b00: VDDmin RM[3:2] are used for test purpose. */
#define CTRL_HPER_E0_RM(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_HPER_E0_RM(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HPER_E1_SNPS_ULP_SPRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HPER_E1_LS                        ((u32)0x00000001 << 6)          /*!<R/W 0x0  SNPS_ULP_SPRAM: pg_b0 LS=1 ram is in light sleep mode, memory array's data retained. */
#define CTRL_BIT_HPER_E1_TEST1                     ((u32)0x00000001 << 5)          /*!<R/W 0x0  SNPS_ULP_SPRAM Test1 pin to bypass self-timed circuit. The external clock controls the read and write controls. The self time bypass mode occpuied High phase of external clock. The self time bypass mode may used to determine time margin of internal self-timed circuitry. */
#define CTRL_BIT_HPER_E1_RME                       ((u32)0x00000001 << 4)          /*!<R/W 0x0  SNPS_ULP_SPRAM RME selects between internal and external RM[2:0] settings, RM[3] is not controlled by RME. RME=1: select external RM[2:0] RME=0: select internal RM[2:0] */
#define CTRL_MASK_HPER_E1_RM                       ((u32)0x0000000F << 0)          /*!<R/W 0x2  SNPS_ULP_SPRAM RM[1:0] are used to trade-off betwwen speed and robustness(yield). 2'b11: fast 2'b10: default 2'b01: slow 2'b00: VDDmin RM[3:2] are used for test purpose. */
#define CTRL_HPER_E1_RM(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_HPER_E1_RM(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HPER_E2_SNPS_ULP_TPSRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HPER_E2_LS                        ((u32)0x00000001 << 12)          /*!<R/W 0x0  SNPS_ULP_TPRAM: pg_b0 LS=1 ram is in light sleep mode, memory array's data retained. */
#define CTRL_BIT_HPER_E2_TEST1B                    ((u32)0x00000001 << 11)          /*!<R/W 0x0  SNPS_ULP_TPRAM */
#define CTRL_BIT_HPER_E2_TEST1A                    ((u32)0x00000001 << 10)          /*!<R/W 0x0  SNPS_ULP_TPRAM */
#define CTRL_BIT_HPER_E2_RMEB                      ((u32)0x00000001 << 9)          /*!<R/W 0x0  SNPS_ULP_TPRAM */
#define CTRL_MASK_HPER_E2_RMB                      ((u32)0x0000000F << 5)          /*!<R/W 0x2  SNPS_ULP_TPRAM */
#define CTRL_HPER_E2_RMB(x)                        ((u32)(((x) & 0x0000000F) << 5))
#define CTRL_GET_HPER_E2_RMB(x)                    ((u32)(((x >> 5) & 0x0000000F)))
#define CTRL_BIT_HPER_E2_RMEA                      ((u32)0x00000001 << 4)          /*!<R/W 0x0  SNPS_ULP_TPRAM */
#define CTRL_MASK_HPER_E2_RMA                      ((u32)0x0000000F << 0)          /*!<R/W 0x2  SNPS_ULP_TPRAM */
#define CTRL_HPER_E2_RMA(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_HPER_E2_RMA(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_HPER_E3_FTC_ULP_SRAM_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_HPER_E3_NAP                       ((u32)0x00000001 << 6)          /*!<R/W 1'b0  FTC_ULP SPRAM without R/W assist & without power gating */
#define CTRL_BIT_HPER_E3_TMCK                      ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_BIT_HPER_E3_DVSE                      ((u32)0x00000001 << 4)          /*!<R/W 1'b0   */
#define CTRL_MASK_HPER_E3_DVS                      ((u32)0x0000000F << 0)          /*!<R/W 4'b1100   */
#define CTRL_HPER_E3_DVS(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_HPER_E3_DVS(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTONK4_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTONK4_E0_SLP                     ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_BTONK4_E0_RET                     ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_BTONK4_E0_NAP                     ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_BTONK4_E0_RAE_0P8                 ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_BTONK4_E0_WAEM_0P8               ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E0_WAEM_0P8(x)                 ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTONK4_E0_WAEM_0P8(x)             ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E0_WAE_0P8                 ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_BTONK4_E0_TMCK_0P8                ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E0_DVSE_0P8                ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E0_DVS_0P8                ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E0_DVS_0P8(x)                  ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTONK4_E0_DVS_0P8(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTONK4_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTONK4_E0_RAE_1P0                 ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_BTONK4_E0_WAEM_1P0               ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E0_WAEM_1P0(x)                 ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_BTONK4_E0_WAEM_1P0(x)             ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E0_WAE_1P0                 ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_BTONK4_E0_TMCK_1P0                ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E0_DVSE_1P0                ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E0_DVS_1P0                ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E0_DVS_1P0(x)                  ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_BTONK4_E0_DVS_1P0(x)              ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_BTONK4_E0_RAE_0P9                 ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_BTONK4_E0_WAEM_0P9               ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E0_WAEM_0P9(x)                 ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTONK4_E0_WAEM_0P9(x)             ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E0_WAE_0P9                 ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_BTONK4_E0_TMCK_0P9                ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E0_DVSE_0P9                ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E0_DVS_0P9                ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E0_DVS_0P9(x)                  ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTONK4_E0_DVS_0P9(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTONK4_E1_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTONK4_E1_SLP                     ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_BTONK4_E1_RET                     ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_BTONK4_E1_NAP                     ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_BTONK4_E1_RAE_0P8                 ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_BTONK4_E1_WAEM_0P8               ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E1_WAEM_0P8(x)                 ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTONK4_E1_WAEM_0P8(x)             ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E1_WAE_0P8                 ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_BTONK4_E1_TMCK_0P8                ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E1_DVSE_0P8                ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E1_DVS_0P8                ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E1_DVS_0P8(x)                  ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTONK4_E1_DVS_0P8(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTONK4_E1_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTONK4_E1_RAE_1P0                 ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_BTONK4_E1_WAEM_1P0               ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E1_WAEM_1P0(x)                 ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_BTONK4_E1_WAEM_1P0(x)             ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E1_WAE_1P0                 ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_BTONK4_E1_TMCK_1P0                ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E1_DVSE_1P0                ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E1_DVS_1P0                ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E1_DVS_1P0(x)                  ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_BTONK4_E1_DVS_1P0(x)              ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_BTONK4_E1_RAE_0P9                 ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_BTONK4_E1_WAEM_0P9               ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTONK4_E1_WAEM_0P9(x)                 ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTONK4_E1_WAEM_0P9(x)             ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTONK4_E1_WAE_0P9                 ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_BTONK4_E1_TMCK_0P9                ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTONK4_E1_DVSE_0P9                ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTONK4_E1_DVS_0P9                ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTONK4_E1_DVS_0P9(x)                  ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTONK4_E1_DVS_0P9(x)              ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTOFFK4_E0_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTOFFK4_E0_SLP                    ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_BTOFFK4_E0_RET                    ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_BTOFFK4_E0_NAP                    ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_BTOFFK4_E0_RAE_0P8                ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_BTOFFK4_E0_WAEM_0P8              ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E0_WAEM_0P8(x)                ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTOFFK4_E0_WAEM_0P8(x)            ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E0_WAE_0P8                ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_BTOFFK4_E0_TMCK_0P8               ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E0_DVSE_0P8               ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E0_DVS_0P8               ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E0_DVS_0P8(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTOFFK4_E0_DVS_0P8(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTOFFK4_E0_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTOFFK4_E0_RAE_1P0                ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_BTOFFK4_E0_WAEM_1P0              ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E0_WAEM_1P0(x)                ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_BTOFFK4_E0_WAEM_1P0(x)            ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E0_WAE_1P0                ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_BTOFFK4_E0_TMCK_1P0               ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E0_DVSE_1P0               ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E0_DVS_1P0               ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E0_DVS_1P0(x)                 ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_BTOFFK4_E0_DVS_1P0(x)             ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_BTOFFK4_E0_RAE_0P9                ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_BTOFFK4_E0_WAEM_0P9              ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E0_WAEM_0P9(x)                ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTOFFK4_E0_WAEM_0P9(x)            ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E0_WAE_0P9                ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_BTOFFK4_E0_TMCK_0P9               ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E0_DVSE_0P9               ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E0_DVS_0P9               ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E0_DVS_0P9(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTOFFK4_E0_DVS_0P9(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTOFFK4_E1_FTC_ULL_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTOFFK4_E1_SLP                    ((u32)0x00000001 << 16)          /*!<R/W 0x0  FTC_ULL+ SPRAM SLP */
#define CTRL_BIT_BTOFFK4_E1_RET                    ((u32)0x00000001 << 15)          /*!<R/W 0x0  FTC_ULL+ SPRAM RET */
#define CTRL_BIT_BTOFFK4_E1_NAP                    ((u32)0x00000001 << 14)          /*!<R/W 0x0  FTC_ULL+ SPRAM NAP */
#define CTRL_BIT_BTOFFK4_E1_RAE_0P8                ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_BTOFFK4_E1_WAEM_0P8              ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E1_WAEM_0P8(x)                ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTOFFK4_E1_WAEM_0P8(x)            ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E1_WAE_0P8                ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_BTOFFK4_E1_TMCK_0P8               ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E1_DVSE_0P8               ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E1_DVS_0P8               ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E1_DVS_0P8(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTOFFK4_E1_DVS_0P8(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BTOFFK4_E1_FTC_ULL_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BTOFFK4_E1_RAE_1P0                ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_BTOFFK4_E1_WAEM_1P0              ((u32)0x0000000F << 21)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E1_WAEM_1P0(x)                ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_BTOFFK4_E1_WAEM_1P0(x)            ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E1_WAE_1P0                ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_BTOFFK4_E1_TMCK_1P0               ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E1_DVSE_1P0               ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E1_DVS_1P0               ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E1_DVS_1P0(x)                 ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_BTOFFK4_E1_DVS_1P0(x)             ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_BTOFFK4_E1_RAE_0P9                ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_BTOFFK4_E1_WAEM_0P9              ((u32)0x0000000F << 7)          /*!<R/W 4'b1010  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1010 *WAEM[3:0] x 0.9v/1.0v: 4'b1010 */
#define CTRL_BTOFFK4_E1_WAEM_0P9(x)                ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_BTOFFK4_E1_WAEM_0P9(x)            ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_BTOFFK4_E1_WAE_0P9                ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_BTOFFK4_E1_TMCK_0P9               ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_BTOFFK4_E1_DVSE_0P9               ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_BTOFFK4_E1_DVS_0P9               ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_BTOFFK4_E1_DVS_0P9(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_BTOFFK4_E1_DVS_0P9(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_DDRC_RRB_E0_FTC_ULP_TPSRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_DDRC_RRB_E0_NAP                   ((u32)0x00000001 << 6)          /*!<R/W 0x0  FTC_ULP_2P_RF: pg_b0 CSA(B)=1 & NAP=1 reduce peripheral power. Cell array's data retained. */
#define CTRL_BIT_DDRC_RRB_E0_TMCK                  ((u32)0x00000001 << 5)          /*!<R/W 0x0  FTC_ULP_2P_RF */
#define CTRL_BIT_DDRC_RRB_E0_DVSE                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  FTC_ULP_2P_RF */
#define CTRL_MASK_DDRC_RRB_E0_DVS                  ((u32)0x0000000F << 0)          /*!<R/W 0xA  FTC_ULP_2P_RF */
#define CTRL_DDRC_RRB_E0_DVS(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_DDRC_RRB_E0_DVS(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_DDRC_BSTC_E0_SNPS_ULP_SPRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_DDRC_BSTC_E0_LS                   ((u32)0x00000001 << 6)          /*!<R/W 0x0  SNPS_ULP_SPRAM: pg_b0 LS=1 ram is in light sleep mode, memory array's data retained. */
#define CTRL_BIT_DDRC_BSTC_E0_TEST1                ((u32)0x00000001 << 5)          /*!<R/W 0x0  SNPS_ULP_SPRAM Test1 pin to bypass self-timed circuit. The external clock controls the read and write controls. The self time bypass mode occpuied High phase of external clock. The self time bypass mode may used to determine time margin of internal self-timed circuitry. */
#define CTRL_BIT_DDRC_BSTC_E0_RME                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  SNPS_ULP_SPRAM RME selects between internal and external RM[2:0] settings, RM[3] is not controlled by RME. RME=1: select external RM[2:0] RME=0: select internal RM[2:0] */
#define CTRL_MASK_DDRC_BSTC_E0_RM                  ((u32)0x0000000F << 0)          /*!<R/W 0x2  SNPS_ULP_SPRAM RM[1:0] are used to trade: off betwwen speed and robustness(yield). 2'b11: fast 2'b10: default 2'b01: slow 2'b00: VDDmin RM[3:2] are used for test purpose. */
#define CTRL_DDRC_BSTC_E0_RM(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_DDRC_BSTC_E0_RM(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_DCACHE_DATA_FTC_ULL_SPRAM_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_DCACHE_DATA_SLP               ((u32)0x00000001 << 16)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_KM0_DCACHE_DATA_RET               ((u32)0x00000001 << 15)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_KM0_DCACHE_DATA_NAP               ((u32)0x00000001 << 14)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_KM0_DCACHE_DATA_STOV              ((u32)0x00000001 << 13)          /*!<R/W 0x0  Not used. */
#define CTRL_BIT_KM0_DCACHE_DATA_RAE_0P8           ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_KM0_DCACHE_DATA_WAEM_0P8         ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_DATA_WAEM_0P8(x)           ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_DCACHE_DATA_WAEM_0P8(x)       ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_DATA_WAE_0P8           ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_KM0_DCACHE_DATA_TMCK_0P8          ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_DATA_DVSE_0P8          ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_DATA_DVS_0P8          ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_DATA_DVS_0P8(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_DCACHE_DATA_DVS_0P8(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_DCACHE_DATA_FTC_ULL_SPRAM_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_DCACHE_DATA_RAE_1P0           ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_KM0_DCACHE_DATA_WAEM_1P0         ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_DATA_WAEM_1P0(x)           ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_KM0_DCACHE_DATA_WAEM_1P0(x)       ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_DATA_WAE_1P0           ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_KM0_DCACHE_DATA_TMCK_1P0          ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_DATA_DVSE_1P0          ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_DATA_DVS_1P0          ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_DATA_DVS_1P0(x)            ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_KM0_DCACHE_DATA_DVS_1P0(x)        ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_KM0_DCACHE_DATA_RAE_0P9           ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_KM0_DCACHE_DATA_WAEM_0P9         ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_DATA_WAEM_0P9(x)           ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_DCACHE_DATA_WAEM_0P9(x)       ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_DATA_WAE_0P9           ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_KM0_DCACHE_DATA_TMCK_0P9          ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_DATA_DVSE_0P9          ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_DATA_DVS_0P9          ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_DATA_DVS_0P9(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_DCACHE_DATA_DVS_0P9(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_DCACHE_TAG_FTC_ULL_SPRAM_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_DCACHE_TAG_SLP                ((u32)0x00000001 << 16)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_KM0_DCACHE_TAG_RET                ((u32)0x00000001 << 15)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_KM0_DCACHE_TAG_NAP                ((u32)0x00000001 << 14)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_KM0_DCACHE_TAG_RAE_0P8            ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_KM0_DCACHE_TAG_WAEM_0P8          ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_TAG_WAEM_0P8(x)            ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_DCACHE_TAG_WAEM_0P8(x)        ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_TAG_WAE_0P8            ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_KM0_DCACHE_TAG_TMCK_0P8           ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_TAG_DVSE_0P8           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_TAG_DVS_0P8           ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_TAG_DVS_0P8(x)             ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_DCACHE_TAG_DVS_0P8(x)         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_DCACHE_TAG_FTC_ULL_SPRAM_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_DCACHE_TAG_RAE_1P0            ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_KM0_DCACHE_TAG_WAEM_1P0          ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_TAG_WAEM_1P0(x)            ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_KM0_DCACHE_TAG_WAEM_1P0(x)        ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_TAG_WAE_1P0            ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_KM0_DCACHE_TAG_TMCK_1P0           ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_TAG_DVSE_1P0           ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_TAG_DVS_1P0           ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_TAG_DVS_1P0(x)             ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_KM0_DCACHE_TAG_DVS_1P0(x)         ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_KM0_DCACHE_TAG_RAE_0P9            ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_KM0_DCACHE_TAG_WAEM_0P9          ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_DCACHE_TAG_WAEM_0P9(x)            ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_DCACHE_TAG_WAEM_0P9(x)        ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_DCACHE_TAG_WAE_0P9            ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_KM0_DCACHE_TAG_TMCK_0P9           ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_DCACHE_TAG_DVSE_0P9           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_DCACHE_TAG_DVS_0P9           ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_DCACHE_TAG_DVS_0P9(x)             ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_DCACHE_TAG_DVS_0P9(x)         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ICACHE_DATA_FTC_ULL_SPRAM_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_ICACHE_DATA_SLP               ((u32)0x00000001 << 16)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_KM0_ICACHE_DATA_RET               ((u32)0x00000001 << 15)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_KM0_ICACHE_DATA_NAP               ((u32)0x00000001 << 14)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_KM0_ICACHE_DATA_RAE_0P8           ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_KM0_ICACHE_DATA_WAEM_0P8         ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_DATA_WAEM_0P8(x)           ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_ICACHE_DATA_WAEM_0P8(x)       ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_DATA_WAE_0P8           ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_KM0_ICACHE_DATA_TMCK_0P8          ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_DATA_DVSE_0P8          ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_DATA_DVS_0P8          ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_DATA_DVS_0P8(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_ICACHE_DATA_DVS_0P8(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ICACHE_DATA_FTC_ULL_SPRAM_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_ICACHE_DATA_RAE_1P0           ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_KM0_ICACHE_DATA_WAEM_1P0         ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_DATA_WAEM_1P0(x)           ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_KM0_ICACHE_DATA_WAEM_1P0(x)       ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_DATA_WAE_1P0           ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_KM0_ICACHE_DATA_TMCK_1P0          ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_DATA_DVSE_1P0          ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_DATA_DVS_1P0          ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_DATA_DVS_1P0(x)            ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_KM0_ICACHE_DATA_DVS_1P0(x)        ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_KM0_ICACHE_DATA_RAE_0P9           ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_KM0_ICACHE_DATA_WAEM_0P9         ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_DATA_WAEM_0P9(x)           ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_ICACHE_DATA_WAEM_0P9(x)       ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_DATA_WAE_0P9           ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_KM0_ICACHE_DATA_TMCK_0P9          ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_DATA_DVSE_0P9          ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_DATA_DVS_0P9          ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_DATA_DVS_0P9(x)            ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_ICACHE_DATA_DVS_0P9(x)        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ICACHE_TAG_FTC_ULL_SPRAM_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_ICACHE_TAG_SLP                ((u32)0x00000001 << 16)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_SLP_SLEP to this reg when chip goes to SLEP ; will load LSYS_SLP_NORM to this reg when chip goes to ACTIVE ; SLP = 1 : Shut-down mode */
#define CTRL_BIT_KM0_ICACHE_TAG_RET                ((u32)0x00000001 << 15)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_RET_SLEP to this reg when chip goes to SLEP ; will load LSYS_RET_NORM to this reg when chip goes to ACTIVE ; RET = 1 : Deep-sleep mode */
#define CTRL_BIT_KM0_ICACHE_TAG_NAP                ((u32)0x00000001 << 14)          /*!<R/W/ES 0x0  SW control: SW can write this register when chip is ACTIVE. ECO: add HW control: will load LSYS_NAP_SLEP to this reg when chip goes to SLEP ; will load LSYS_NAP_NORM to this reg when chip goes to ACTIVE ; NAP = 1 : Light-sleep mode */
#define CTRL_BIT_KM0_ICACHE_TAG_RAE_0P8            ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE, x0.8V */
#define CTRL_MASK_KM0_ICACHE_TAG_WAEM_0P8          ((u32)0x0000000F << 7)          /*!<R/W 4'b1011  FTC_ULL+ SPRAM WAEM[3:2], x0.8V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_TAG_WAEM_0P8(x)            ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_ICACHE_TAG_WAEM_0P8(x)        ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_TAG_WAE_0P8            ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE, x0.8V */
#define CTRL_BIT_KM0_ICACHE_TAG_TMCK_0P8           ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_TAG_DVSE_0P8           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_TAG_DVS_0P8           ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_TAG_DVS_0P8(x)             ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_ICACHE_TAG_DVS_0P8(x)         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ICACHE_TAG_FTC_ULL_SPRAM_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_ICACHE_TAG_RAE_1P0            ((u32)0x00000001 << 25)          /*!<R/W 1'b0  FTC_ULL+ SPRAM RAE x1.0V */
#define CTRL_MASK_KM0_ICACHE_TAG_WAEM_1P0          ((u32)0x0000000F << 21)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[3:2] x1.0V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_TAG_WAEM_1P0(x)            ((u32)(((x) & 0x0000000F) << 21))
#define CTRL_GET_KM0_ICACHE_TAG_WAEM_1P0(x)        ((u32)(((x >> 21) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_TAG_WAE_1P0            ((u32)0x00000001 << 20)          /*!<R/W 1'b0  FTC_ULL+ SPRAM WAE x1.0V */
#define CTRL_BIT_KM0_ICACHE_TAG_TMCK_1P0           ((u32)0x00000001 << 19)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_TAG_DVSE_1P0           ((u32)0x00000001 << 17)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_TAG_DVS_1P0           ((u32)0x00000003 << 14)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_TAG_DVS_1P0(x)             ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_KM0_ICACHE_TAG_DVS_1P0(x)         ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_BIT_KM0_ICACHE_TAG_RAE_0P9            ((u32)0x00000001 << 11)          /*!<R/W 1'b1  FTC_ULL+ SPRAM RAE x0.9V */
#define CTRL_MASK_KM0_ICACHE_TAG_WAEM_0P9          ((u32)0x0000000F << 7)          /*!<R/W 4'b1001  FTC_ULL+ SPRAM WAEM[1:0] x0.9V *WAEM[3:0] x 0.8V: 4'b1011 *WAEM[3:0] x 0.9v/1.0v: 4'b1001 */
#define CTRL_KM0_ICACHE_TAG_WAEM_0P9(x)            ((u32)(((x) & 0x0000000F) << 7))
#define CTRL_GET_KM0_ICACHE_TAG_WAEM_0P9(x)        ((u32)(((x >> 7) & 0x0000000F)))
#define CTRL_BIT_KM0_ICACHE_TAG_WAE_0P9            ((u32)0x00000001 << 6)          /*!<R/W 1'b1  FTC_ULL+ SPRAM WAE x0.9V */
#define CTRL_BIT_KM0_ICACHE_TAG_TMCK_0P9           ((u32)0x00000001 << 5)          /*!<R/W 1'b0  FTC_ULL+ SPRAM TMCK */
#define CTRL_BIT_KM0_ICACHE_TAG_DVSE_0P9           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  FTC_ULL+ SPRAM DVSE */
#define CTRL_MASK_KM0_ICACHE_TAG_DVS_0P9           ((u32)0x00000003 << 0)          /*!<R/W 2'b01  FTC_ULL+ DVS default is 2'b01 in 0.8v/0.9v,1.0v. */
#define CTRL_KM0_ICACHE_TAG_DVS_0P9(x)             ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM0_ICACHE_TAG_DVS_0P9(x)         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_FTC_ULL_ROM
 * @brief KM0 itcm parameter register
 * @{
 *****************************************************************************/
#define CTRL_BIT_KM0_ITCM_MISR_SEL                 ((u32)0x00000001 << 5)          /*!<R/W 0x0  select km0 itcm rom misr_dataout to register */
#define CTRL_BIT_KM0_ITCM_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  for all rom memory */
#define CTRL_MASK_KM0_ITCM_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0x3  for all rom memory */
#define CTRL_KM0_ITCM_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_KM0_ITCM_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ITCM_MISR_DATA_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_MASK_KM0_ITCM_MISR_DATA_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_KM0_ITCM_MISR_DATA_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM0_ITCM_MISR_DATA_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM0_ITCM_MISR_DATA_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_MASK_KM0_ITCM_MISR_DATA_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_KM0_ITCM_MISR_DATA_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM0_ITCM_MISR_DATA_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM4_PARAM
 * @brief parameter for km4 memory
 * @{
 *****************************************************************************/
#define CTRL_MASK_KM4_ITCM_MISR_SEL                ((u32)0x0000000F << 24)          /*!<R/W 0x0  select km4 itcm rom misr_dataout */
#define CTRL_KM4_ITCM_MISR_SEL(x)                  ((u32)(((x) & 0x0000000F) << 24))
#define CTRL_GET_KM4_ITCM_MISR_SEL(x)              ((u32)(((x >> 24) & 0x0000000F)))
#define CTRL_BIT_KM4_LS_ALL                        ((u32)0x00000001 << 19)          /*!<R/W 0x0  LS for all km4 memory. LS=1 ram is in light sleep mode, memory array's data retained. */
#define CTRL_BIT_KM4_DS_ALL                        ((u32)0x00000001 << 18)          /*!<R/W 0x0   */
#define CTRL_BIT_KM4_SD_ALL                        ((u32)0x00000001 << 17)          /*!<R/W 0x0   */
#define CTRL_BIT_KM4_TEST1_ALL                     ((u32)0x00000001 << 11)          /*!<R/W 0x0  Test1 pin to bypass self-timed circuit. The external clock controls the read and write controls. The self time bypass mode occpuied High phase of external clock. The self time bypass mode may used to determine time margin of internal self-timed circuitry. */
#define CTRL_BIT_KM4_RME_ALL                       ((u32)0x00000001 << 10)          /*!<R/W 0x0  RME for all km4 memory. RME selects between internal and external RM[2:0] settings, RM[3] is not controlled by RME. RME=1: select external RM[2:0] RME=0: select internal RM[2:0] */
#define CTRL_MASK_KM4_RM32_ALL                     ((u32)0x00000003 << 8)          /*!<R/W 0x0  RM32 for all km4 memory. RM[3:2] are used for test purpose. */
#define CTRL_KM4_RM32_ALL(x)                       ((u32)(((x) & 0x00000003) << 8))
#define CTRL_GET_KM4_RM32_ALL(x)                   ((u32)(((x >> 8) & 0x00000003)))
#define CTRL_MASK_KM4_ITCM0_E0_RM10                ((u32)0x00000003 << 6)          /*!<R/W 0x2  SNPS_ULP_SPROM, for KM4 itcm0~7 : RM[1:0] are used to trade-off betwwen speed and robustness(yield). 2'b11 - fast 2'b10 - default 2'b01 - slow 2'b00 - VDDmin RM[3:2] are used for test purpose. */
#define CTRL_KM4_ITCM0_E0_RM10(x)                  ((u32)(((x) & 0x00000003) << 6))
#define CTRL_GET_KM4_ITCM0_E0_RM10(x)              ((u32)(((x >> 6) & 0x00000003)))
#define CTRL_MASK_KM4_ICACHE_E0_RM10               ((u32)0x00000003 << 2)          /*!<R/W 0x2  SNPS_ULP_SPRAM RM[1:0] are used to trade-off betwwen speed and robustness(yield). 2'b11 - fast 2'b10 - default 2'b01 - slow 2'b00 - VDDmin RM[3:2] are used for test purpose. */
#define CTRL_KM4_ICACHE_E0_RM10(x)                 ((u32)(((x) & 0x00000003) << 2))
#define CTRL_GET_KM4_ICACHE_E0_RM10(x)             ((u32)(((x >> 2) & 0x00000003)))
#define CTRL_MASK_KM4_DCACHE_E0_RM10               ((u32)0x00000003 << 0)          /*!<R/W 0x2  SNPS_ULP_SPRAM RM[1:0] are used to trade-off betwwen speed and robustness(yield). 2'b11 - fast 2'b10 - default 2'b01 - slow 2'b00 - VDDmin RM[3:2] are used for test purpose. */
#define CTRL_KM4_DCACHE_E0_RM10(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_KM4_DCACHE_E0_RM10(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM4_ITCM_MISR_DATA_0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_MASK_KM4_ITCM_MISR_DATA_31_0          ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_KM4_ITCM_MISR_DATA_31_0(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM4_ITCM_MISR_DATA_31_0(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_KM4_ITCM_MISR_DATA_1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_MASK_KM4_ITCM_MISR_DATA_63_32         ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_KM4_ITCM_MISR_DATA_63_32(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_KM4_ITCM_MISR_DATA_63_32(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_BIST_CTRL
 * @brief BT mbist control of BT
 * @{
 *****************************************************************************/
#define CTRL_MASK_BT_RPT_SEL                       ((u32)0x0000000F << 20)          /*!<R/W 0x0   */
#define CTRL_BT_RPT_SEL(x)                         ((u32)(((x) & 0x0000000F) << 20))
#define CTRL_GET_BT_RPT_SEL(x)                     ((u32)(((x >> 20) & 0x0000000F)))
#define CTRL_BIT_BT_DS                             ((u32)0x00000001 << 18)          /*!<R/W 0x0   */
#define CTRL_BIT_BT_LVDRF_CLKDIS                   ((u32)0x00000001 << 17)          /*!<R/W 0x0   */
#define CTRL_BIT_BT_LOOP_MODE                      ((u32)0x00000001 << 16)          /*!<R/W 0x0   */
#define CTRL_MASK_BT_CELL_EN                       ((u32)0x000000FF << 8)          /*!<R/W 0xff   */
#define CTRL_BT_CELL_EN(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define CTRL_GET_BT_CELL_EN(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define CTRL_MASK_BT_GRP_SEL                       ((u32)0x0000000F << 0)          /*!<R/W 0xf   */
#define CTRL_BT_GRP_SEL(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_BT_GRP_SEL(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_BIST_RPT
 * @brief BT mbist report
 * @{
 *****************************************************************************/
#define CTRL_MASK_BT_BIST_RPT                      ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_BT_BIST_RPT(x)                        ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_RPT(x)                    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_BIST_DONE
 * @brief BT mbist done
 * @{
 *****************************************************************************/
#define CTRL_MASK_BT_BIST_DONE                     ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_BT_BIST_DONE(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_DONE(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_BIST_FAIL
 * @brief BT mbist fail
 * @{
 *****************************************************************************/
#define CTRL_MASK_BT_BIST_FAIL                     ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_BT_BIST_FAIL(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_FAIL(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_BIST_DRF_PAUSE
 * @brief BT mbist drf start pause
 * @{
 *****************************************************************************/
#define CTRL_MASK_BT_BIST_DRF_PAUSE                ((u32)0xFFFFFFFF << 0)          /*!<R 0x0   */
#define CTRL_BT_BIST_DRF_PAUSE(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CTRL_GET_BT_BIST_DRF_PAUSE(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_BT_FTC_ULP_1PRF0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_BT_ROM_DVSE                       ((u32)0x00000001 << 20)          /*!<R/W 0x0  connect to r_RME_ROM of BT. DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_BT_ROM_DVS                       ((u32)0x0000000F << 16)          /*!<R/W 0x5  connect to r_RM_ROM of BT. DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_BT_ROM_DVS(x)                         ((u32)(((x) & 0x0000000F) << 16))
#define CTRL_GET_BT_ROM_DVS(x)                     ((u32)(((x >> 16) & 0x0000000F)))
#define CTRL_BIT_BT_1PSR_DVSE                      ((u32)0x00000001 << 15)          /*!<R/W 0x0  connect to r_RME_1PSR of BT. DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_BT_1PSR_DVS                      ((u32)0x0000000F << 11)          /*!<R/W 0xB  connect to r_RM_1PSR of BT. DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_BT_1PSR_DVS(x)                        ((u32)(((x) & 0x0000000F) << 11))
#define CTRL_GET_BT_1PSR_DVS(x)                    ((u32)(((x >> 11) & 0x0000000F)))
#define CTRL_BIT_BT_1PRF1_DVSE                     ((u32)0x00000001 << 10)          /*!<R/W 0x0  connect to r_RME_1PRF1 of BT. DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_BT_1PRF1_DVS                     ((u32)0x0000000F << 6)          /*!<R/W 0xC  connect to r_RM_1PRF1 of BT. DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_BT_1PRF1_DVS(x)                       ((u32)(((x) & 0x0000000F) << 6))
#define CTRL_GET_BT_1PRF1_DVS(x)                   ((u32)(((x >> 6) & 0x0000000F)))
#define CTRL_BIT_BT_TMCK_ALL                       ((u32)0x00000001 << 5)          /*!<R/W 0x0  connect to r_TEST1 of BT. Internal timing bypass signal. TMCK=1: WL pulse will be controlled by the clock duty cycle. TMCK=0: default value, RAM access normally. */
#define CTRL_BIT_BT_1PRF0_DVSE                     ((u32)0x00000001 << 4)          /*!<R/W 0x0  connect to r_RME_1PRF0 of BT. DVSE=1: delay option enable. DVSE=0: delay option disable. */
#define CTRL_MASK_BT_1PRF0_DVS                     ((u32)0x0000000F << 0)          /*!<R/W 0xC  connect to r_RM_1PRF0 of BT. DVS[0] controls the SRAM sensing pulse when DVSE=1. DVS[0]=1: short pulse width DVS[0]=0: long pulse width dvs[3:1] is delay option control words when DVSE=1. DVS[3:1] = 000: loosest margin. DVS[3:1] = 001 DVS[3:1] = 010 DVS[3:1] = 011 … */
#define CTRL_BT_1PRF0_DVS(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_BT_1PRF0_DVS(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_MIPI_FTC_2PRAM
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_MASK_DSI_CMD_DVS                      ((u32)0x0000000F << 0)          /*!<R/W 4'b1111   */
#define CTRL_DSI_CMD_DVS(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define CTRL_GET_DSI_CMD_DVS(x)                    ((u32)(((x >> 0) & 0x0000000F)))
#define CTRL_BIT_DSI_CMD_DVSE                      ((u32)0x00000001 << 4)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_CMD_TMCKA                     ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_CMD_TMCKB                     ((u32)0x00000001 << 6)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_CMD_NAP                       ((u32)0x00000001 << 7)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_CMD_RET                       ((u32)0x00000001 << 8)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_CMD_SLP                       ((u32)0x00000001 << 9)          /*!<R/W 1'b0   */
#define CTRL_MASK_DSI_VIDEO_DVS                    ((u32)0x0000000F << 10)          /*!<R/W 4'b1111   */
#define CTRL_DSI_VIDEO_DVS(x)                      ((u32)(((x) & 0x0000000F) << 10))
#define CTRL_GET_DSI_VIDEO_DVS(x)                  ((u32)(((x >> 10) & 0x0000000F)))
#define CTRL_BIT_DSI_VIDEO_DVSE                    ((u32)0x00000001 << 14)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_VIDEO_TMCKA                   ((u32)0x00000001 << 15)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_VIDEO_TMCKB                   ((u32)0x00000001 << 16)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_VIDEO_NAP                     ((u32)0x00000001 << 17)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_VIDEO_RET                     ((u32)0x00000001 << 18)          /*!<R/W 1'b0   */
#define CTRL_BIT_DSI_VIDEO_SLP                     ((u32)0x00000001 << 19)          /*!<R/W 1'b0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_SCAN_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_SCAN_PLL_CTRL_MIPI                ((u32)0x00000001 << 27)          /*!<R/W 0x0  scan pll ctrl mipi */
#define CTRL_BIT_SCAN_PLL_CTRL_USB                 ((u32)0x00000001 << 26)          /*!<R/W 0x0  scan pll ctrl usb */
#define CTRL_MASK_SCAN_OCC_DBG_EN_DDRC_PLFM        ((u32)0x00000003 << 24)          /*!<R/W 0x0  scan occ dbg en ddrc_plfm */
#define CTRL_SCAN_OCC_DBG_EN_DDRC_PLFM(x)          ((u32)(((x) & 0x00000003) << 24))
#define CTRL_GET_SCAN_OCC_DBG_EN_DDRC_PLFM(x)      ((u32)(((x >> 24) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_DDRC             ((u32)0x00000003 << 22)          /*!<R/W 0x0  scan occ dbg en ddrc */
#define CTRL_SCAN_OCC_DBG_EN_DDRC(x)               ((u32)(((x) & 0x00000003) << 22))
#define CTRL_GET_SCAN_OCC_DBG_EN_DDRC(x)           ((u32)(((x >> 22) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_DPLL             ((u32)0x00000003 << 20)          /*!<R/W 0x0  scan occ dbg en dpll */
#define CTRL_SCAN_OCC_DBG_EN_DPLL(x)               ((u32)(((x) & 0x00000003) << 20))
#define CTRL_GET_SCAN_OCC_DBG_EN_DPLL(x)           ((u32)(((x >> 20) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_MIPI             ((u32)0x00000003 << 18)          /*!<R/W 0x0  scan occ dbg en mipi */
#define CTRL_SCAN_OCC_DBG_EN_MIPI(x)               ((u32)(((x) & 0x00000003) << 18))
#define CTRL_GET_SCAN_OCC_DBG_EN_MIPI(x)           ((u32)(((x >> 18) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_USB              ((u32)0x00000003 << 16)          /*!<R/W 0x0  scan occ dbg en usb */
#define CTRL_SCAN_OCC_DBG_EN_USB(x)                ((u32)(((x) & 0x00000003) << 16))
#define CTRL_GET_SCAN_OCC_DBG_EN_USB(x)            ((u32)(((x >> 16) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_AP               ((u32)0x00000003 << 14)          /*!<R/W 0x0  scan occ dbg en ap */
#define CTRL_SCAN_OCC_DBG_EN_AP(x)                 ((u32)(((x) & 0x00000003) << 14))
#define CTRL_GET_SCAN_OCC_DBG_EN_AP(x)             ((u32)(((x >> 14) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_SDH              ((u32)0x00000003 << 12)          /*!<R/W 0x0  scan occ dbg en sdh */
#define CTRL_SCAN_OCC_DBG_EN_SDH(x)                ((u32)(((x) & 0x00000003) << 12))
#define CTRL_GET_SCAN_OCC_DBG_EN_SDH(x)            ((u32)(((x >> 12) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_SEC              ((u32)0x00000003 << 10)          /*!<R/W 0x0  scan occ dbg en sec */
#define CTRL_SCAN_OCC_DBG_EN_SEC(x)                ((u32)(((x) & 0x00000003) << 10))
#define CTRL_GET_SCAN_OCC_DBG_EN_SEC(x)            ((u32)(((x >> 10) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_PSRAM            ((u32)0x00000003 << 8)          /*!<R/W 0x0  scan occ dbg en psram */
#define CTRL_SCAN_OCC_DBG_EN_PSRAM(x)              ((u32)(((x) & 0x00000003) << 8))
#define CTRL_GET_SCAN_OCC_DBG_EN_PSRAM(x)          ((u32)(((x >> 8) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_SPIC             ((u32)0x00000003 << 6)          /*!<R/W 0x0  scan occ dbg en spic */
#define CTRL_SCAN_OCC_DBG_EN_SPIC(x)               ((u32)(((x) & 0x00000003) << 6))
#define CTRL_GET_SCAN_OCC_DBG_EN_SPIC(x)           ((u32)(((x >> 6) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_HPERI            ((u32)0x00000003 << 4)          /*!<R/W 0x0  scan occ dbg en hperi */
#define CTRL_SCAN_OCC_DBG_EN_HPERI(x)              ((u32)(((x) & 0x00000003) << 4))
#define CTRL_GET_SCAN_OCC_DBG_EN_HPERI(x)          ((u32)(((x >> 4) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_HBUS             ((u32)0x00000003 << 2)          /*!<R/W 0x0  scan occ dbg en hbus */
#define CTRL_SCAN_OCC_DBG_EN_HBUS(x)               ((u32)(((x) & 0x00000003) << 2))
#define CTRL_GET_SCAN_OCC_DBG_EN_HBUS(x)           ((u32)(((x >> 2) & 0x00000003)))
#define CTRL_MASK_SCAN_OCC_DBG_EN_NP               ((u32)0x00000003 << 0)          /*!<R/W 0x0  scan occ dbg en np */
#define CTRL_SCAN_OCC_DBG_EN_NP(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CTRL_GET_SCAN_OCC_DBG_EN_NP(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_CTRL_SCAN_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define CTRL_BIT_SCAN_RET_EN                       ((u32)0x00000001 << 22)          /*!<R/W 0x0  scan retention test enable */
#define CTRL_MASK_SCAN_PLL_OUT_SEL                 ((u32)0x00000007 << 19)          /*!<R/W 0x0  scan pll out sel: 2'b000: nppll out 2'b001: wlpll out 2'b010: appll out 2'b011: usb0 pll out 2'b100: usb1 pll out 2'b101: mipi pll out 2'b110: dpll out */
#define CTRL_SCAN_PLL_OUT_SEL(x)                   ((u32)(((x) & 0x00000007) << 19))
#define CTRL_GET_SCAN_PLL_OUT_SEL(x)               ((u32)(((x >> 19) & 0x00000007)))
#define CTRL_BIT_SCAN_CFT4                         ((u32)0x00000001 << 18)          /*!<R/W 0x0  scan QFN88 package ft mode4 */
#define CTRL_BIT_SCAN_CFT3                         ((u32)0x00000001 << 17)          /*!<R/W 0x0  scan QFN88 package ft mode3 */
#define CTRL_BIT_SCAN_CFT2                         ((u32)0x00000001 << 16)          /*!<R/W 0x0  scan QFN88 package ft mode2 */
#define CTRL_BIT_SCAN_CFT1                         ((u32)0x00000001 << 15)          /*!<R/W 0x0  scan QFN88 package ft mode1 */
#define CTRL_BIT_SCAN_CFT0                         ((u32)0x00000001 << 14)          /*!<R/W 0x0  scan QFN88 package ft mode0 */
#define CTRL_BIT_SCAN_BFT2                         ((u32)0x00000001 << 13)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode2 */
#define CTRL_BIT_SCAN_BFT1                         ((u32)0x00000001 << 12)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode1 */
#define CTRL_BIT_SCAN_BFT0                         ((u32)0x00000001 << 11)          /*!<R/W 0x0  scan QFN144 NON-MCM package ft mode0 */
#define CTRL_BIT_SCAN_AFT2                         ((u32)0x00000001 << 10)          /*!<R/W 0x0  scan QFN144 MCM package ft mode2 */
#define CTRL_BIT_SCAN_AFT1                         ((u32)0x00000001 << 9)          /*!<R/W 0x0  scan QFN144 MCM package ft mode1 */
#define CTRL_BIT_SCAN_AFT0                         ((u32)0x00000001 << 8)          /*!<R/W 0x0  scan QFN144 MCM package ft mode0 */
#define CTRL_BIT_SCAN_CP2                          ((u32)0x00000001 << 7)          /*!<R/W 0x0  scan cp mode2 */
#define CTRL_BIT_SCAN_CP1                          ((u32)0x00000001 << 6)          /*!<R/W 0x0  scan cp mode1 */
#define CTRL_BIT_SCAN_CP0                          ((u32)0x00000001 << 5)          /*!<R/W 0x0  scan cp mode0 */
#define CTRL_BIT_SCAN_SPC_DISABLE                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  scan spc disable */
#define CTRL_BIT_SCAN_PLL_BYPASS                   ((u32)0x00000001 << 3)          /*!<R/W 0x0  scan pll bypass */
#define CTRL_BIT_SCAN_COMPRESS                     ((u32)0x00000001 << 2)          /*!<R/W 0x0  scan compress */
#define CTRL_BIT_SCAN_PAR_LATPS                    ((u32)0x00000001 << 1)          /*!<WA0 0x0  scan latch up pulse */
#define CTRL_BIT_SCAN_MODE                         ((u32)0x00000001 << 0)          /*!<WA0 0x0  scan mode pulse */
/** @} */


/*==========MEM_CTRL Register Address Definition==========*/
#define REG_CTRL_MBIST_CTRL_STAT                     0x0000
#define REG_CTRL_MBIST_GROUP_EN_0                    0x0004
#define REG_CTRL_MBIST_GROUP_EN_1                    0x0008
#define REG_CTRL_MBIST_GROUP_EN_2                    0x000C
#define REG_CTRL_MBIST_RSTB_0                        0x0010
#define REG_CTRL_MBIST_RSTB_LOOP_MODE_1              0x0014
#define REG_CTRL_MBIST_LOOP_MODE_0                   0x0018
#define REG_CTRL_MBIST_MSEL_0                        0x001C
#define REG_CTRL_MBIST_MSEL_1                        0x0020
#define REG_CTRL_MBIST_MSEL_2                        0x0024
#define REG_CTRL_MBIST_START_PAUSE_STAT              0x0028
#define REG_CTRL_MBIST_DONE_STAT                     0x002C
#define REG_CTRL_MBIST_FAIL_NRML_STAT_0              0x0030
#define REG_CTRL_MBIST_FAIL_NRML_STAT_1              0x0034
#define REG_CTRL_MBIST_FAIL_NRML_STAT_2              0x0038
#define REG_CTRL_MBIST_FAIL_DRF_STAT_0               0x003C
#define REG_CTRL_MBIST_FAIL_DRF_STAT_1               0x0040
#define REG_CTRL_MBIST_FAIL_DRF_STAT_2               0x0044
#define REG_CTRL_ROM_LSPMC_E0_MISR_DOUT_0            0x0048
#define REG_CTRL_ROM_LSPMC_E0_MISR_DOUT_1            0x004C
#define REG_CTRL_ROM_AONPMC_E0_MISR_DOUT_0           0x0050
#define REG_CTRL_ROM_AONPMC_E0_MISR_DOUT_1           0x0054
#define REG_CTRL_ROM_CA7BOOT_E0_MISR_DOUT_0          0x0058
#define REG_CTRL_ROM_CA7BOOT_E0_MISR_DOUT_1          0x005C
#define REG_CTRL_LRET_E0_FTC_ULL_0                   0x0060
#define REG_CTRL_LRET_E0_FTC_ULL_1                   0x0064
#define REG_CTRL_LSYS_E0_FTC_ULL_0                   0x0068
#define REG_CTRL_LSYS_E0_FTC_ULL_1                   0x006C
#define REG_CTRL_LSPMC_E0_FTC_ULP_ROM                0x0070
#define REG_CTRL_HSYS_E0_FTC_ULL_0                   0x0074
#define REG_CTRL_HSYS_E0_FTC_ULL_1                   0x0078
#define REG_CTRL_CA7BOOT_E0_FTC_ULL_ROM              0x007C
#define REG_CTRL_WLK4_E0_FTC_ULL_0                   0x0080
#define REG_CTRL_WLK4_E0_FTC_ULL_1                   0x0084
#define REG_CTRL_WPOFF_E0_FTC_ULL_0                  0x0088
#define REG_CTRL_WPOFF_E0_FTC_ULL_1                  0x008C
#define REG_CTRL_WPOFF_E1_FTC_ULP_RW                 0x0090
#define REG_CTRL_WPOFF_E2_FTC_ULP_RW_TPRAM           0x0094
#define REG_CTRL_WPOFF_E3_FTC_ULP_RW                 0x0098
#define REG_CTRL_WPOFF_E4_FTC_ULP_RW                 0x009C
#define REG_CTRL_WPOFF_E5_FTC_ULP_TPSRAM             0x00A0
#define REG_CTRL_WPOFF_E6_FTC_ULP_RW_PG_TPRAM        0x00A4
#define REG_CTRL_WPON_E0_FTC_ULL_0                   0x00A8
#define REG_CTRL_WPON_E0_FTC_ULL_1                   0x00AC
#define REG_CTRL_HPER_E0_SNPS_ULP_SPRAM              0x00B0
#define REG_CTRL_HPER_E1_SNPS_ULP_SPRAM              0x00B4
#define REG_CTRL_HPER_E2_SNPS_ULP_TPSRAM             0x00B8
#define REG_CTRL_HPER_E3_FTC_ULP_SRAM_0              0x00BC
#define REG_CTRL_HPER_E3_FTC_ULP_SRAM_1              0x00C0
#define REG_CTRL_BTONK4_E0_FTC_ULL_0                 0x00C4
#define REG_CTRL_BTONK4_E0_FTC_ULL_1                 0x00C8
#define REG_CTRL_BTONK4_E1_FTC_ULL_0                 0x0130
#define REG_CTRL_BTONK4_E1_FTC_ULL_1                 0x0134
#define REG_CTRL_BTOFFK4_E0_FTC_ULL_0                0x0138
#define REG_CTRL_BTOFFK4_E0_FTC_ULL_1                0x013C
#define REG_CTRL_BTOFFK4_E1_FTC_ULL_0                0x00CC
#define REG_CTRL_BTOFFK4_E1_FTC_ULL_1                0x00D0
#define REG_CTRL_DDRC_RRB_E0_FTC_ULP_TPSRAM          0x00D4
#define REG_CTRL_DDRC_BSTC_E0_SNPS_ULP_SPRAM         0x00D8
#define REG_CTRL_KM0_DCACHE_DATA_FTC_ULL_SPRAM_0     0x00DC
#define REG_CTRL_KM0_DCACHE_DATA_FTC_ULL_SPRAM_1     0x00E0
#define REG_CTRL_KM0_DCACHE_TAG_FTC_ULL_SPRAM_0      0x00E4
#define REG_CTRL_KM0_DCACHE_TAG_FTC_ULL_SPRAM_1      0x00E8
#define REG_CTRL_KM0_ICACHE_DATA_FTC_ULL_SPRAM_0     0x00EC
#define REG_CTRL_KM0_ICACHE_DATA_FTC_ULL_SPRAM_1     0x00F0
#define REG_CTRL_KM0_ICACHE_TAG_FTC_ULL_SPRAM_0      0x00F4
#define REG_CTRL_KM0_ICACHE_TAG_FTC_ULL_SPRAM_1      0x00F8
#define REG_CTRL_KM0_FTC_ULL_ROM                     0x00FC
#define REG_CTRL_KM0_ITCM_MISR_DATA_0                0x0100
#define REG_CTRL_KM0_ITCM_MISR_DATA_1                0x0104
#define REG_CTRL_KM4_PARAM                           0x0108
#define REG_CTRL_KM4_ITCM_MISR_DATA_0                0x010C
#define REG_CTRL_KM4_ITCM_MISR_DATA_1                0x0110
#define REG_CTRL_BT_BIST_CTRL                        0x0114
#define REG_CTRL_BT_BIST_RPT                         0x0118
#define REG_CTRL_BT_BIST_DONE                        0x011C
#define REG_CTRL_BT_BIST_FAIL                        0x0120
#define REG_CTRL_BT_BIST_DRF_PAUSE                   0x0124
#define REG_CTRL_BT_FTC_ULP_1PRF0                    0x0128
#define REG_CTRL_MIPI_FTC_2PRAM                      0x012C
#define REG_CTRL_SCAN_CTRL1                          0x0140
#define REG_CTRL_SCAN_CTRL0                          0x0144

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
#define SYSTEM_MEM_CTRL_BASE     0x42009200

/* MANUAL_GEN_END */

#endif
