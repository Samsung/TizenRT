#ifndef _AMEBAD2_DDRC_FRONT_H_
#define _AMEBAD2_DDRC_FRONT_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup DDRC_FRONT
  * @brief DDRC_FRONT driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup DDRC_FRONT_Register_Definitions DDRC_FRONT Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup DDRC_FE_DIR_0
 * @brief Design name register
 * @{
 **/
#define DDRC_MASK_DESIGN_NAME              ((u32)0xFFFFFFFF << 0)          /*!<R 0x52_316  Design name is RXI-316 */
#define DDRC_DESIGN_NAME(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DESIGN_NAME(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_1
 * @brief Design version register
 * @{
 **/
#define DDRC_MASK_DESIGN_VERSION           ((u32)0xFFFFFFFF << 0)          /*!<R 0x0100  Design version */
#define DDRC_DESIGN_VERSION(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DESIGN_VERSION(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_2
 * @brief Design revision register
 * @{
 **/
#define DDRC_MASK_DESIGN_REVISION          ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Design revision */
#define DDRC_DESIGN_REVISION(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DESIGN_REVISION(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_3
 * @brief Instance number register
 * @{
 **/
#define DDRC_MASK_DESIGN_INSTANCE          ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Instance number */
#define DDRC_DESIGN_INSTANCE(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DESIGN_INSTANCE(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_4
 * @brief Implementation year
 * @{
 **/
#define DDRC_MASK_IMPL_YEAR                ((u32)0xFFFFFFFF << 0)          /*!<R 0x2019  Year of design generation */
#define DDRC_IMPL_YEAR(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_IMPL_YEAR(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_5
 * @brief Implementation date
 * @{
 **/
#define DDRC_MASK_IMPL_DATE                ((u32)0xFFFFFFFF << 0)          /*!<R 0x0000  Date of design generation (MM_DD_HH_mm) */
#define DDRC_IMPL_DATE(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_IMPL_DATE(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_6
 * @brief Developer
 * @{
 **/
#define DDRC_MASK_DEVELOPER                ((u32)0xFFFFFFFF << 0)          /*!<R 0x52_0000  Developer of design generation (RXXXX) */
#define DDRC_DEVELOPER(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DEVELOPER(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_FE_DIR_7
 * @brief Project number
 * @{
 **/
#define DDRC_MASK_PROJ_NUMBER              ((u32)0xFFFFFFFF << 0)          /*!<R 0x0000  Project number (RL number) */
#define DDRC_PROJ_NUMBER(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_PROJ_NUMBER(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_CFG_CM_FE_PA
 * @brief Port arbiter page limitation value
 * @{
 **/
#define DDRC_BIT_FORWARD_URGENT            ((u32)0x00000001 << 31)          /*!<R/W 0x0  Forward urgent signals to backend through go_to_first */
#define DDRC_BIT_PAGEMATCH_ENABLE          ((u32)0x00000001 << 16)          /*!<R/W 1  Choose page match transaction continuously */
#define DDRC_MASK_AR_PAGELIMIT_PRI0        ((u32)0x0000000F << 12)          /*!<R/W 0x0  The signal width of priority0 page-match limit value */
#define DDRC_AR_PAGELIMIT_PRI0(x)          ((u32)(((x) & 0x0000000F) << 12))
#define DDRC_GET_AR_PAGELIMIT_PRI0(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define DDRC_MASK_AR_PAGELIMIT_NPRI0       ((u32)0x0000000F << 8)          /*!<R/W 0x0  The signal width of non-priority0 page-match limit value */
#define DDRC_AR_PAGELIMIT_NPRI0(x)         ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_AR_PAGELIMIT_NPRI0(x)     ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_AW_PAGELIMIT_PRI0        ((u32)0x0000000F << 4)          /*!<R/W 0x0  The signal width of priority0 page-match limit value */
#define DDRC_AW_PAGELIMIT_PRI0(x)          ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_AW_PAGELIMIT_PRI0(x)      ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_AW_PAGELIMIT_NPRI0       ((u32)0x0000000F << 0)          /*!<R/W 0x0  The signal width of non-priority0 page-match limit value */
#define DDRC_AW_PAGELIMIT_NPRI0(x)         ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_AW_PAGELIMIT_NPRI0(x)     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_CFG_CM_FE_RRB
 * @brief RRB LP read entry number
 * @{
 **/
#define DDRC_MASK_MAX_NUM_LPR_ENTRIES_CH1  ((u32)0x000000FF << 8)          /*!<R/W 0x20  Maximum number of free slots reserved for LP read commands in channel 1. */
#define DDRC_MAX_NUM_LPR_ENTRIES_CH1(x)    ((u32)(((x) & 0x000000FF) << 8))
#define DDRC_GET_MAX_NUM_LPR_ENTRIES_CH1(x) ((u32)(((x >> 8) & 0x000000FF)))
#define DDRC_MASK_MAX_NUM_LPR_ENTRIES_CH0  ((u32)0x000000FF << 0)          /*!<R/W 0x20  Maximum number of free slots reserved for LP read commands in channel 0. */
#define DDRC_MAX_NUM_LPR_ENTRIES_CH0(x)    ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_MAX_NUM_LPR_ENTRIES_CH0(x) ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_CFG_APC_CKG
 * @brief Auto clock gating control register for APC
 * @{
 **/
#define DDRC_BIT_CGATE_EN_WD_BB            ((u32)0x00000001 << 22)          /*!<R/W 0x1  Enable auto clock gating of wd bb reg in all APC */
#define DDRC_BIT_CGATE_EN_AW_BB            ((u32)0x00000001 << 21)          /*!<R/W 0x1  Enable auto clock gating of aw bb reg in all APC */
#define DDRC_BIT_CGATE_EN_AR_BB            ((u32)0x00000001 << 20)          /*!<R/W 0x1  Enable auto clock gating of ar bb reg in all APC */
#define DDRC_BIT_CGATE_EN_RD_FIFO2         ((u32)0x00000001 << 19)          /*!<R/W 0x1  Enable auto clock gating of rd fifo2 in all APC */
#define DDRC_BIT_CGATE_EN_AR_MAP1          ((u32)0x00000001 << 18)          /*!<R/W 0x1  Enable auto clock gating of ar addmap1 in all APC */
#define DDRC_BIT_CGATE_EN_AR_REG2          ((u32)0x00000001 << 17)          /*!<R/W 0x1  Enable auto clock gating of ar reg2 in all APC */
#define DDRC_BIT_CGATE_EN_AR_CHOP          ((u32)0x00000001 << 16)          /*!<R/W 0x1  Enable auto clock gating of ar chop in all APC */
#define DDRC_BIT_CGATE_EN_RD_FIFO1         ((u32)0x00000001 << 15)          /*!<R/W 0x1  Enable auto clock gating of rd fifo1 in all APC */
#define DDRC_BIT_CGATE_EN_AR_REG1          ((u32)0x00000001 << 14)          /*!<R/W 0x1  Enable auto clock gating of ar reg1 in all APC */
#define DDRC_BIT_CGATE_EN_AR_MERGE         ((u32)0x00000001 << 13)          /*!<R/W 0x1  Enable auto clock gating of ar merge in all APC */
#define DDRC_BIT_CGATE_EN_RD_FIFO0         ((u32)0x00000001 << 12)          /*!<R/W 0x1  Enable auto clock gating of rd FIFO0 in all APC */
#define DDRC_BIT_CGATE_EN_AR_REG0          ((u32)0x00000001 << 11)          /*!<R/W 0x1  Enable auto clock gating of ar reg0 in all APC */
#define DDRC_BIT_CGATE_EN_AR_QUEUE         ((u32)0x00000001 << 10)          /*!<R/W 0x1  Enable auto clock gating of ar queue in all APC */
#define DDRC_BIT_CGATE_EN_AR_MAP0          ((u32)0x00000001 << 9)          /*!<R/W 0x1  Enable auto clock gating of ar addmap0 in all APC */
#define DDRC_BIT_CGATE_EN_AW_MAP           ((u32)0x00000001 << 8)          /*!<R/W 0x1  Enable auto clock gating of aw addmap in all APC */
#define DDRC_BIT_CGATE_EN_AW_PACK          ((u32)0x00000001 << 7)          /*!<R/W 0x1  Enable auto clock gating of aw pack in all APC */
#define DDRC_BIT_CGATE_EN_AW_REG2          ((u32)0x00000001 << 6)          /*!<R/W 0x1  Enable auto clock gating of aw reg2 in all APC */
#define DDRC_BIT_CGATE_EN_AW_CHOP          ((u32)0x00000001 << 5)          /*!<R/W 0x1  Enable auto clock gating of aw chop in all APC */
#define DDRC_BIT_CGATE_EN_AW_REG1          ((u32)0x00000001 << 4)          /*!<R/W 0x1  Enable auto clock gating of aw reg1 in all APC */
#define DDRC_BIT_CGATE_EN_AW_MERGE         ((u32)0x00000001 << 3)          /*!<R/W 0x1  Enable auto clock gating of aw merge in all APC */
#define DDRC_BIT_CGATE_EN_WD_FIFO          ((u32)0x00000001 << 2)          /*!<R/W 0x1  Enable auto clock gating of wd FIFO in all APC */
#define DDRC_BIT_CGATE_EN_AW_FIFO          ((u32)0x00000001 << 1)          /*!<R/W 0x1  Enable auto clock gating of aw FIFO in all APC */
#define DDRC_BIT_CGATE_EN_PORT_AGEN        ((u32)0x00000001 << 0)          /*!<R/W 0x1  Enable auto clock gating of port agent in all APC */
/** @} */

/** @defgroup DDRC_CFG_FE_CKG
 * @brief Auto clock gating control register for FE
 * @{
 **/
#define DDRC_BIT_CGATE_EN_DNSIZER1         ((u32)0x00000001 << 7)          /*!<R/W 0x1  Enable auto clock gating of channel 1 dnsizer2 */
#define DDRC_BIT_CGATE_EN_DNSIZER0         ((u32)0x00000001 << 6)          /*!<R/W 0x1  Enable auto clock gating of channel 0 dnsizer2 */
#define DDRC_BIT_CGATE_EN_REGBLK           ((u32)0x00000001 << 5)          /*!<R/W 0x1  Enable auto clock gating of register block except ckg registers */
#define DDRC_BIT_CGATE_EN_ERRLOG           ((u32)0x00000001 << 4)          /*!<R/W 0x1  Enable auto clock gating of error log */
#define DDRC_BIT_CGATE_EN_DEFSLV           ((u32)0x00000001 << 3)          /*!<R/W 0x1  Enable auto clock gating of default slave */
#define DDRC_BIT_CGATE_EN_RRB              ((u32)0x00000001 << 2)          /*!<R/W 0x1  Enable auto clock gating of rrb */
#define DDRC_BIT_CGATE_EN_PA1              ((u32)0x00000001 << 1)          /*!<R/W 0x1  Enable auto clock gating of port arbiter 1 */
#define DDRC_BIT_CGATE_EN_PA0              ((u32)0x00000001 << 0)          /*!<R/W 0x1  Enable auto clock gating of port arbiter 0 */
/** @} */

/** @defgroup DDRC_ELR_PLD0
 * @brief Error transaction information 0
 * @{
 **/
#define DDRC_MASK_ERR_BSTINDEX             ((u32)0x000000FF << 24)          /*!<R 0x0  Burst index inside a burst that trigger fault. Always 0 in RXI-316 */
#define DDRC_ERR_BSTINDEX(x)               ((u32)(((x) & 0x000000FF) << 24))
#define DDRC_GET_ERR_BSTINDEX(x)           ((u32)(((x >> 24) & 0x000000FF)))
#define DDRC_MASK_ERR_BSTLEN               ((u32)0x000000FF << 16)          /*!<R 0x0  Burst length of error transaction after chopped. */
#define DDRC_ERR_BSTLEN(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_ERR_BSTLEN(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_ERR_BSTTYPE              ((u32)0x00000007 << 11)          /*!<R 0x0  BurstSeq of the error request. */
#define DDRC_ERR_BSTTYPE(x)                ((u32)(((x) & 0x00000007) << 11))
#define DDRC_GET_ERR_BSTTYPE(x)            ((u32)(((x >> 11) & 0x00000007)))
#define DDRC_MASK_ERR_CMD                  ((u32)0x00000007 << 8)          /*!<R 0x0  Cmd of the error request. 3'h1: write 3'h2: read */
#define DDRC_ERR_CMD(x)                    ((u32)(((x) & 0x00000007) << 8))
#define DDRC_GET_ERR_CMD(x)                ((u32)(((x >> 8) & 0x00000007)))
#define DDRC_MASK_ERR_SRC                  ((u32)0x000000FF << 0)          /*!<R 0x0  Error source. Always 0 in RXI-316 */
#define DDRC_ERR_SRC(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_ERR_SRC(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_ELR_PLD1
 * @brief Error transaction information 1
 * @{
 **/
#define DDRC_MASK_ERR_MREQINFO             ((u32)0x000001FF << 23)          /*!<R 0x0  Request information {axlock, axprot,axcache} of error transaction */
#define DDRC_ERR_MREQINFO(x)               ((u32)(((x) & 0x000001FF) << 23))
#define DDRC_GET_ERR_MREQINFO(x)           ((u32)(((x >> 23) & 0x000001FF)))
#define DDRC_MASK_ERR_SIZE                 ((u32)0x00000007 << 16)          /*!<R 0x0  AXI Size of the error request. */
#define DDRC_ERR_SIZE(x)                   ((u32)(((x) & 0x00000007) << 16))
#define DDRC_GET_ERR_SIZE(x)               ((u32)(((x >> 16) & 0x00000007)))
/** @} */

/** @defgroup DDRC_ELR_ID
 * @brief ID of error transaction
 * @{
 **/
#define DDRC_MASK_ERR_ID                   ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  AXI ID of the error request. This ID combines AXI ID and port ID */
#define DDRC_ERR_ID(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_ERR_ID(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_ELR_ADR0
 * @brief Lower part address of error transaction
 * @{
 **/
#define DDRC_MASK_LOWER PART OF ADDRESS    ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Address[31:0] of the error request. */
#define DDRC_LOWER PART OF ADDRESS(x)      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_LOWER PART OF ADDRESS(x)  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_ELR_ADR1
 * @brief Higher part address of error transaction
 * @{
 **/
#define DDRC_MASK_HIGHER PART OF ADDRESS   ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Address[63:32] of the error request. */
#define DDRC_HIGHER PART OF ADDRESS(x)     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_HIGHER PART OF ADDRESS(x) ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_ELR_CODE
 * @brief Error type
 * @{
 **/
#define DDRC_MASK_ERROR CORE               ((u32)0x000000FF << 0)          /*!<R 0x0  The error code of error request. 8'b0: Secure fault */
#define DDRC_ERROR CORE(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_ERROR CORE(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_ELR_CTRL
 * @brief Error interrupt clear
 * @{
 **/
#define DDRC_BIT_ELR_INTR_CLR              ((u32)0x00000001 << 0)          /*!<W 0x0  Error log interrupt clear. Writing 1’b1 to the register will trigger a 1-cycle pulse interrupt clear. It has to write 1’b1 to this register every time when the error exception occurs. Note that this register is write-only, the return value of reading this register is meaningless */
/** @} */

/** @defgroup DDRC_CFG_Px_RQOS
 * @brief Portx Read QoS value
 * @{
 **/
#define DDRC_MASK_RQOS_MAP_REGION2         ((u32)0x00000003 << 24)          /*!<R/W 0x0  Traffic class of region 2. 0: LPR 1: VPR 2: HPR */
#define DDRC_RQOS_MAP_REGION2(x)           ((u32)(((x) & 0x00000003) << 24))
#define DDRC_GET_RQOS_MAP_REGION2(x)       ((u32)(((x >> 24) & 0x00000003)))
#define DDRC_MASK_RQOS_MAP_REGION1         ((u32)0x00000003 << 20)          /*!<R/W 0x0  Traffic class of region 1. 0: LPR 1: VPR 2: HPR */
#define DDRC_RQOS_MAP_REGION1(x)           ((u32)(((x) & 0x00000003) << 20))
#define DDRC_GET_RQOS_MAP_REGION1(x)       ((u32)(((x >> 20) & 0x00000003)))
#define DDRC_MASK_RQOS_MAP_REGION0         ((u32)0x00000003 << 16)          /*!<R/W 0x0  Traffic class of region 0. 0: LPR 1: VPR 2: HPR */
#define DDRC_RQOS_MAP_REGION0(x)           ((u32)(((x) & 0x00000003) << 16))
#define DDRC_GET_RQOS_MAP_REGION0(x)       ((u32)(((x >> 16) & 0x00000003)))
#define DDRC_MASK_RQOS_MAP_LEVEL2          ((u32)0x0000000F << 8)          /*!<R/W 0x0  Separation level2 indicating the end of region 1 mapping. Region1 starts from (rqos_map_level1 + 1) to rqos_map_level2. Region2 starts from (rqos_map_level2 + 1) to 15. e.g. {rqos_map_level1, rqos_map_level2, rqos_map_region0, rqos_map_region1, rqos_map_region0} = {6, 13, 0, 1, 2}. QoS 0~6 is LPR, QoS 7~13 is VPR, and QoS 14~15 is HRP. */
#define DDRC_RQOS_MAP_LEVEL2(x)            ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_RQOS_MAP_LEVEL2(x)        ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_RQOS_MAP_LEVEL1          ((u32)0x0000000F << 0)          /*!<R/W 0x0  Separation level1 indicating the end of region 0 mapping. Region0 starts from 0 to rqos_map_level1. e.g. rqos_map_level1 is 6, and rqos_map_region0 is 0. QoS 0 ~ 6 is LPR. */
#define DDRC_RQOS_MAP_LEVEL1(x)            ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_RQOS_MAP_LEVEL1(x)        ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_CFG_Px_WQOS
 * @brief Portx Write QoS value
 * @{
 **/
#define DDRC_MASK_WQOS_MAP_REGION2         ((u32)0x00000003 << 24)          /*!<R/W 0x0  Traffic class of region 2. 0: LPW 1: VPW */
#define DDRC_WQOS_MAP_REGION2(x)           ((u32)(((x) & 0x00000003) << 24))
#define DDRC_GET_WQOS_MAP_REGION2(x)       ((u32)(((x >> 24) & 0x00000003)))
#define DDRC_MASK_WQOS_MAP_REGION1         ((u32)0x00000003 << 20)          /*!<R/W 0x0  Traffic class of region 1. 0: LPW 1: VPW */
#define DDRC_WQOS_MAP_REGION1(x)           ((u32)(((x) & 0x00000003) << 20))
#define DDRC_GET_WQOS_MAP_REGION1(x)       ((u32)(((x >> 20) & 0x00000003)))
#define DDRC_MASK_WQOS_MAP_REGION0         ((u32)0x00000003 << 16)          /*!<R/W 0x0  Traffic class of region 0. 0: LPW 1: VPW */
#define DDRC_WQOS_MAP_REGION0(x)           ((u32)(((x) & 0x00000003) << 16))
#define DDRC_GET_WQOS_MAP_REGION0(x)       ((u32)(((x >> 16) & 0x00000003)))
#define DDRC_MASK_WQOS_MAP_LEVEL2          ((u32)0x0000000F << 8)          /*!<R/W 0x0  Separation level2 indicating the end of region 1 mapping. */
#define DDRC_WQOS_MAP_LEVEL2(x)            ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_WQOS_MAP_LEVEL2(x)        ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_WQOS_MAP_LEVEL1          ((u32)0x0000000F << 0)          /*!<R/W 0x0  Separation level1 indicating the end of region 0 mapping. */
#define DDRC_WQOS_MAP_LEVEL1(x)            ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_WQOS_MAP_LEVEL1(x)        ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_CFG_Px_TIMEOUT
 * @brief Portx timeout value
 * @{
 **/
#define DDRC_MASK_WR_PORT_TIMEOUT_VALUE    ((u32)0x00000FFF << 12)          /*!<R/W 0x0  Write port timeout counter */
#define DDRC_WR_PORT_TIMEOUT_VALUE(x)      ((u32)(((x) & 0x00000FFF) << 12))
#define DDRC_GET_WR_PORT_TIMEOUT_VALUE(x)  ((u32)(((x >> 12) & 0x00000FFF)))
#define DDRC_MASK_RD_PORT_TIMEOUT_VALUE    ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Read port timeout counter */
#define DDRC_RD_PORT_TIMEOUT_VALUE(x)      ((u32)(((x) & 0x00000FFF) << 0))
#define DDRC_GET_RD_PORT_TIMEOUT_VALUE(x)  ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup DDRC_CFG_Px_RDQUEUE
 * @brief Portx read queue arbitration
 * @{
 **/
#define DDRC_BIT_PAGEMATCH_LOCK            ((u32)0x00000001 << 3)          /*!<R/W 1  Lock page match port for dual port */
#define DDRC_MASK_PAGE_LIMITATION          ((u32)0x00000007 << 0)          /*!<R/W 0x0  page-match limitation value */
#define DDRC_PAGE_LIMITATION(x)            ((u32)(((x) & 0x00000007) << 0))
#define DDRC_GET_PAGE_LIMITATION(x)        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup DDRC_CFG_Px_EXPIRYL
 * @brief Portx expiry value (Bit31 ~ Bit0)
 * @{
 **/
#define DDRC_MASK_EXPIRY_TIME_LOW          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Expiry time bit 31 ~ 0 of a transaction. The maximum available value depends on STAMP_WIDTH (2^STAMP_WIDTH). */
#define DDRC_EXPIRY_TIME_LOW(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_EXPIRY_TIME_LOW(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_CFG_Px_EXPIRYH
 * @brief Portx expiry value (Bit63 ~ Bit32)
 * @{
 **/
#define DDRC_MASK_EXPIRY_TIME_HIGH         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  Expiry time bit 63 ~ 32 of a transaction. The maximum available value depends on STAMP_WIDTH (2^STAMP_WIDTH). */
#define DDRC_EXPIRY_TIME_HIGH(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_EXPIRY_TIME_HIGH(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_CFG_Px_PORTCTRL
 * @brief Portx control
 * @{
 **/
#define DDRC_BIT_BLOCK_COMMAND_REQUEST     ((u32)0x00000001 << 0)          /*!<R/W 1  Blocks new transactions from entering the front-end. 1: block AR and AW channels. 0: turn on AR and AW channels */
/** @} */

/** @defgroup DDRC_CFG_Px_PORTSTATS
 * @brief Portx status
 * @{
 **/
#define DDRC_BIT_BLOCK_COMMAND_STATUS      ((u32)0x00000001 << 0)          /*!<RO 0  AR and AW channel blocking status. 1: both AR and AW are blocked. 0: any of them is not blocked. */
/** @} */
/** @} */

/* Exported types --------------------------------------------------------*/

/** @defgroup DDRC_FRONT_Exported_Types DDRC_FRONT Exported Types
  * @{
*****************************************************************************/

/** @brief DDRC_FRONT port
*****************************************************************************/
typedef struct {

	__IO uint32_t DDRC_CFG_Px_RQOS;                       /*!< PORTx READ QOS VALUE Register */
	__IO uint32_t DDRC_CFG_Px_WQOS;                       /*!< PORTx WRITE QOS VALUE Register */
	__IO uint32_t DDRC_CFG_Px_TIMEOUT;                    /*!< PORTx TIMEOUT VALUE Register */
	__IO uint32_t DDRC_CFG_Px_RDQUEUE;                    /*!< PORTx READ QUEUE ARBITRATION Register */
	__IO uint32_t DDRC_CFG_Px_EXPIRYL;                    /*!< PORTx EXPIRY VALUE (BIT31 ~ BIT0) Register */
	__IO uint32_t DDRC_CFG_Px_EXPIRYH;                    /*!< PORTx EXPIRY VALUE (BIT63 ~ BIT32) Register */
	__IO uint32_t DDRC_CFG_Px_PORTCTRL;                   /*!< PORTx CONTROL Register */
	__IO uint32_t DDRC_CFG_Px_PORTSTATS;                  /*!< PORTx STATUS Register */
	__IO uint32_t RSVD[8];
} Port_Ctrl_TypeDef;





/**
 * @brief DDRC_FRONT Register Declaration
 *****************************************************************************/
typedef struct {
	__I  uint32_t DDRC_FE_DIR_0;                          /*!< DESIGN NAME REGISTER,  Address offset: 0X000 */
	__I  uint32_t DDRC_FE_DIR_1;                          /*!< DESIGN VERSION REGISTER,  Address offset: 0X004 */
	__I  uint32_t DDRC_FE_DIR_2;                          /*!< DESIGN REVISION REGISTER,  Address offset: 0X008 */
	__I  uint32_t DDRC_FE_DIR_3;                          /*!< INSTANCE NUMBER REGISTER,  Address offset: 0X00C */
	__I  uint32_t DDRC_FE_DIR_4;                          /*!< IMPLEMENTATION YEAR Register,  Address offset: 0X010 */
	__I  uint32_t DDRC_FE_DIR_5;                          /*!< IMPLEMENTATION DATE Register,  Address offset: 0X014 */
	__I  uint32_t DDRC_FE_DIR_6;                          /*!< DEVELOPER Register,  Address offset: 0X018 */
	__I  uint32_t DDRC_FE_DIR_7;                          /*!< PROJECT NUMBER Register,  Address offset: 0X01C */
	__IO uint32_t RSVD0[56];                              /*!<  Reserved,  Address offset:0x20-0xFC */
	__IO uint32_t DDRC_CFG_CM_FE_PA;                      /*!< PORT ARBITER PAGE LIMITATION VALUE Register,  Address offset: 0X100 */
	__IO uint32_t DDRC_CFG_CM_FE_RRB;                     /*!< RRB LP READ ENTRY NUMBER Register,  Address offset: 0X104 */
	__IO uint32_t RSVD1[30];                              /*!<  Reserved,  Address offset:0x108-0x17C */
	__IO uint32_t DDRC_CFG_APC_CKG;                       /*!< AUTO CLOCK GATING CONTROL REGISTER FOR APC Register,  Address offset: 0X180 */
	__IO uint32_t DDRC_CFG_FE_CKG;                        /*!< AUTO CLOCK GATING CONTROL REGISTER FOR FE Register,  Address offset: 0X184 */
	__IO uint32_t RSVD2[30];                              /*!<  Reserved,  Address offset:0x188-0x1FC */
	__IO uint32_t DDRC_ELR_PLD0;                          /*!< ERROR TRANSACTION INFORMATION 0 Register,  Address offset: 0X200 */
	__IO uint32_t DDRC_ELR_PLD1;                          /*!< ERROR TRANSACTION INFORMATION 1 Register,  Address offset: 0X204 */
	__I  uint32_t DDRC_ELR_ID;                            /*!< ID OF ERROR TRANSACTION Register,  Address offset: 0X208 */
	__I  uint32_t DDRC_ELR_ADR0;                          /*!< LOWER PART ADDRESS OF ERROR TRANSACTION Register,  Address offset: 0X20C */
	__I  uint32_t DDRC_ELR_ADR1;                          /*!< HIGHER PART ADDRESS OF ERROR TRANSACTION Register,  Address offset: 0X210 */
	__IO uint32_t DDRC_ELR_CODE;                          /*!< ERROR TYPE Register,  Address offset: 0X214 */
	__IO uint32_t RSVD3[9];                               /*!<  Reserved,  Address offset:0x218-0x238 */
	__IO uint32_t DDRC_ELR_CTRL;                          /*!< ERROR INTERRUPT CLEAR Register,  Address offset: 0X23C */
	__IO uint32_t RSVD4[368];                              /*!<  Reserved,  Address offset:0x240-0x7FC */
	Port_Ctrl_TypeDef Port[7];                            /*!< DDRC_FRONT IP have 7 ports,  Address offset: 0X800-0X99C */
} DDRCF_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
