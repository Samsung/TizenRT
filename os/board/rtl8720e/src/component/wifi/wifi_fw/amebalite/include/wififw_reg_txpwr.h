#ifndef _AMEBAD2_TXPWR_H_
#define _AMEBAD2_TXPWR_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RATE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXPWR_CTRL_CLR                                        ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) 1: clr txpwr, sync rst */
#define BIT_MASK_R_FORCE_MODE_IDX                                   ((u32)0x00000007 << 28)          /*!<R/W 0  Force txpwr_bb from specific source 0:FOCRE_NONE (normal) 1:FORCE_PWR_BY_RATE 2:FORCE_MACID 3:FORCE_PWR_LIM 4:FORCE_PWR_RU_LIM 5:FORCE_COEXIST */
#define BIT_R_FORCE_MODE_IDX(x)                                     ((u32)(((x) & 0x00000007) << 28))
#define BIT_GET_R_FORCE_MODE_IDX(x)                                 ((u32)(((x >> 28) & 0x00000007)))
#define BIT_MASK_R_TXAGC_OFDM_REF                                   ((u32)0x000001FF << 19)          /*!<R/W 40h  S(9.2) ofdm ref dbm */
#define BIT_R_TXAGC_OFDM_REF(x)                                     ((u32)(((x) & 0x000001FF) << 19))
#define BIT_GET_R_TXAGC_OFDM_REF(x)                                 ((u32)(((x >> 19) & 0x000001FF)))
#define BIT_MASK_R_TXAGC_CCK_REF                                    ((u32)0x000001FF << 10)          /*!<R/W 40h  S(9.2) cck ref dbm */
#define BIT_R_TXAGC_CCK_REF(x)                                      ((u32)(((x) & 0x000001FF) << 10))
#define BIT_GET_R_TXAGC_CCK_REF(x)                                  ((u32)(((x >> 10) & 0x000001FF)))
#define BIT_R_FORCE_PWR_BY_RATE_EN                                  ((u32)0x00000001 << 9)          /*!<R/W 0  force pwr by rate value_en */
#define BIT_MASK_R_FORCE_PWR_BY_RATE_VALUE                          ((u32)0x000001FF << 0)          /*!<R/W 0  S(9.2) force pwr by rate value (txagc_ref+pwr_by_rate_tbl) */
#define BIT_R_FORCE_PWR_BY_RATE_VALUE(x)                            ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_PWR_BY_RATE_VALUE(x)                        ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RATE_OFST_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXAGC_TBL_RD                                          ((u32)0x00000001 << 26)          /*!<R/W 0  (RTL_only) 1:indirecr read enable */
#define BIT_MASK_R_TXAGC_TBL_RA                                     ((u32)0x0000003F << 20)          /*!<R/W 0  (RTL_only) indirect read of addr of pwr_by_rate */
#define BIT_R_TXAGC_TBL_RA(x)                                       ((u32)(((x) & 0x0000003F) << 20))
#define BIT_GET_R_TXAGC_TBL_RA(x)                                   ((u32)(((x >> 20) & 0x0000003F)))
#define BIT_MASK_R_TXAGC_CCK_HT_OFFSET                              ((u32)0x0000000F << 16)          /*!<R/W 0  S(4.1)CCK-HT pwr offset */
#define BIT_R_TXAGC_CCK_HT_OFFSET(x)                                ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_TXAGC_CCK_HT_OFFSET(x)                            ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_LEGACY_HT_OFFSET                           ((u32)0x0000000F << 12)          /*!<R/W 0  S(4.1)LEG-HT pwr offset */
#define BIT_R_TXAGC_LEGACY_HT_OFFSET(x)                             ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_TXAGC_LEGACY_HT_OFFSET(x)                         ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_HT_OFFSET                                  ((u32)0x0000000F << 8)          /*!<R/W 0  (RTL_only) S(4.1) HT-HT pwr bias = 0 */
#define BIT_R_TXAGC_HT_OFFSET(x)                                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_TXAGC_HT_OFFSET(x)                                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_VHT_HT_OFFSET                              ((u32)0x0000000F << 4)          /*!<R/W 0  S(4.1)VHT-HT pwr offset */
#define BIT_R_TXAGC_VHT_HT_OFFSET(x)                                ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_TXAGC_VHT_HT_OFFSET(x)                            ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_HE_HT_OFFSET                               ((u32)0x0000000F << 0)          /*!<R/W 0  S(4.1)HE-HT pwr offset */
#define BIT_R_TXAGC_HE_HT_OFFSET(x)                                 ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_TXAGC_HE_HT_OFFSET(x)                             ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_LMT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_TF_DCM_EN                                       ((u32)0x00000001 << 26)          /*!<R/W 0  (RTL_only) 1:Force input TF dcm = R_force_TF_dcm_value */
#define BIT_R_FORCE_TF_DCM_VALUE                                    ((u32)0x00000001 << 25)          /*!<R/W 0  (RTL_only) Force input TF dcm value */
#define BIT_R_FORCE_NORM_RSP_HE_TB_EN_ON                            ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) 1:Norm resp always In HE_TB */
#define BIT_R_FORCE_PWR_BY_LIMIT_BF0_EN                             ((u32)0x00000001 << 23)          /*!<R/W 0  (RTL_only) 1: Force pwr_lim_BF0= r_force_pwr_by_limit_bfoff_value (0xd210[17:9]) */
#define BIT_R_FORCE_PWR_BY_LIMIT_BF1_EN                             ((u32)0x00000001 << 22)          /*!<R/W 0  (RTL_only) 1: Force pwr_lim_BF1= r_force_pwr_by_limit_bfon_value (0xd210[8:0]) */
#define BIT_R_TXAGC_PWR_LIM_BF0_EN                                  ((u32)0x00000001 << 21)          /*!<R/W 1h  (RTL_only) None BF pwr limit enable */
#define BIT_R_TXAGC_PWR_LIM_BF1_EN                                  ((u32)0x00000001 << 20)          /*!<R/W 1h  (RTL_only) BF pwr limit enable */
#define BIT_MASK_R_TXAGC_BW20_BW40_OFFSET                           ((u32)0x0000000F << 16)          /*!<R/W 0  S(4.1)BW20 - BW40 pwr offset */
#define BIT_R_TXAGC_BW20_BW40_OFFSET(x)                             ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_TXAGC_BW20_BW40_OFFSET(x)                         ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_RFBW_40M_OFFSET                            ((u32)0x0000000F << 12)          /*!<R/W 0  (RTL_only) S(4.1)BW40 - BW40 pwr offset */
#define BIT_R_TXAGC_RFBW_40M_OFFSET(x)                              ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_TXAGC_RFBW_40M_OFFSET(x)                          ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_BW80_BW40_OFFSET                           ((u32)0x0000000F << 8)          /*!<R/W 0  S(4.1)BW80 - BW40 pwr offset */
#define BIT_R_TXAGC_BW80_BW40_OFFSET(x)                             ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_TXAGC_BW80_BW40_OFFSET(x)                         ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_BW160_BW40_OFFSET                          ((u32)0x0000000F << 4)          /*!<R/W 0  S(4.1)BW160 - BW40 pwr offset */
#define BIT_R_TXAGC_BW160_BW40_OFFSET(x)                            ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_TXAGC_BW160_BW40_OFFSET(x)                        ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_TXAGC_BW80_80_BW40_OFFSET                        ((u32)0x0000000F << 0)          /*!<R/W 0  S(4.1) BW80+80 - BW40 pwr offset */
#define BIT_R_TXAGC_BW80_80_BW40_OFFSET(x)                          ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_TXAGC_BW80_80_BW40_OFFSET(x)                      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_MACID_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXAGC_PWR_BY_MACID_EN                                 ((u32)0x00000001 << 29)          /*!<R/W 1h  (RTL_only) 1:enable pwr_by_macid */
#define BIT_R_FORCE_CCA_PWR_TH_VALUE_EN                             ((u32)0x00000001 << 28)          /*!<R/W 0  1:Force CCA_PWR_TH value_en */
#define BIT_MASK_R_FORCE_CCA_PWR_TH_VALUE                           ((u32)0x000000FF << 20)          /*!<R/W 0  S(8.1)Force CCA_PWR_TH value */
#define BIT_R_FORCE_CCA_PWR_TH_VALUE(x)                             ((u32)(((x) & 0x000000FF) << 20))
#define BIT_GET_R_FORCE_CCA_PWR_TH_VALUE(x)                         ((u32)(((x >> 20) & 0x000000FF)))
#define BIT_R_FORCE_CCA_PWR_TH_EN                                   ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) 1:Force output CCA_PWR_TH_en */
#define BIT_R_FORCE_PWR_BY_MACID_EN                                 ((u32)0x00000001 << 16)          /*!<R/W 0  1:Force output Pwr_by_macid_en */
#define BIT_MASK_R_FORCE_PWR_BY_MACID_VALUE                         ((u32)0x000000FF << 8)          /*!<R/W 0  S(8.1)Force Pwr_by_macid_value */
#define BIT_R_FORCE_PWR_BY_MACID_VALUE(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_FORCE_PWR_BY_MACID_VALUE(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_R_FORCE_PWR_BY_MACID_VALUE_EN                           ((u32)0x00000001 << 7)          /*!<R/W 1h  (RTL_only) 1:Force output Pwr_by_macid_value */
#define BIT_R_TXPWR_LIM_TBL_RD                                      ((u32)0x00000001 << 6)          /*!<R/W 0  (RTL_only) 1: pwr lim tbl indirect read en */
#define BIT_MASK_R_TXPWR_LIM_TBL_RA                                 ((u32)0x0000003F << 0)          /*!<R/W 0  (RTL_only) pwr lim tbl indirect read addr */
#define BIT_R_TXPWR_LIM_TBL_RA(x)                                   ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_TXPWR_LIM_TBL_RA(x)                               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BF_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXAGC_BF_PWR_BOOST_EN                                 ((u32)0x00000001 << 31)          /*!<R/W 1h  1:TxBF will cal pwr boost to BB 0:TxBF pwr_boost =0 & Pwr_lim use BF1_pwr_lim */
#define BIT_MASK_R_HE_ER_SU_PWR_REDUCE_VAL                          ((u32)0x0000003F << 19)          /*!<R/W Ch  (RTL_only) S(6.2) 3dB=0xC */
#define BIT_R_HE_ER_SU_PWR_REDUCE_VAL(x)                            ((u32)(((x) & 0x0000003F) << 19))
#define BIT_GET_R_HE_ER_SU_PWR_REDUCE_VAL(x)                        ((u32)(((x >> 19) & 0x0000003F)))
#define BIT_R_HE_ER_SU_PWR_REDUCE_EN                                ((u32)0x00000001 << 18)          /*!<R/W 0  1:HE_ER_SU pwr lim output will minus 3dB */
#define BIT_MASK_R_FORCE_PWR_BY_LIMIT_BFOFF_VALUE                   ((u32)0x000001FF << 9)          /*!<R/W 0  S(9.2) r_force_pwr_by_limit_BF0_en=1 is valid */
#define BIT_R_FORCE_PWR_BY_LIMIT_BFOFF_VALUE(x)                     ((u32)(((x) & 0x000001FF) << 9))
#define BIT_GET_R_FORCE_PWR_BY_LIMIT_BFOFF_VALUE(x)                 ((u32)(((x >> 9) & 0x000001FF)))
#define BIT_MASK_R_FORCE_PWR_BY_LIMIT_BFON_VALUE                    ((u32)0x000001FF << 0)          /*!<R/W 0  S(9.2) r_force_pwr_by_limit_BF1_en=1 is valid */
#define BIT_R_FORCE_PWR_BY_LIMIT_BFON_VALUE(x)                      ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_PWR_BY_LIMIT_BFON_VALUE(x)                  ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_MACID_REG
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXPWR_REG3                                       ((u32)0x000000FF << 24)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=5 from MAC */
#define BIT_R_TXPWR_REG3(x)                                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_TXPWR_REG3(x)                                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_TXPWR_REG2                                       ((u32)0x000000FF << 16)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=4 from MAC */
#define BIT_R_TXPWR_REG2(x)                                         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_TXPWR_REG2(x)                                     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_TXPWR_REG1                                       ((u32)0x000000FF << 8)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=3 from MAC */
#define BIT_R_TXPWR_REG1(x)                                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_TXPWR_REG1(x)                                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_TXPWR_REG0                                       ((u32)0x000000FF << 0)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=2 from MAC */
#define BIT_R_TXPWR_REG0(x)                                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_TXPWR_REG0(x)                                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_MACID_REG2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXPWR_BY_MACID_TBL_RD                                 ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) 1:Pwr_by_macid indirect read */
#define BIT_MASK_R_TXPWR_BY_MACID_TBL_RA                            ((u32)0x0000007F << 24)          /*!<R/W 0  (RTL_only) Pwr_by_macid indirect read addr */
#define BIT_R_TXPWR_BY_MACID_TBL_RA(x)                              ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXPWR_BY_MACID_TBL_RA(x)                          ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_R_TXPWR_REG5_EN                                         ((u32)0x00000001 << 21)          /*!<R/W 0  pwr by macid en when Pwr_mode=7 from MAC */
#define BIT_R_TXPWR_REG4_EN                                         ((u32)0x00000001 << 20)          /*!<R/W 0  pwr by macid en when Pwr_mode=6 from MAC */
#define BIT_R_TXPWR_REG3_EN                                         ((u32)0x00000001 << 19)          /*!<R/W 0  pwr by macid en when Pwr_mode=5 from MAC */
#define BIT_R_TXPWR_REG2_EN                                         ((u32)0x00000001 << 18)          /*!<R/W 0  pwr by macid en when pwr_mode=4 from MAC */
#define BIT_R_TXPWR_REG1_EN                                         ((u32)0x00000001 << 17)          /*!<R/W 0  pwr by macid en when Pwr_mode=3 from MAC */
#define BIT_R_TXPWR_REG0_EN                                         ((u32)0x00000001 << 16)          /*!<R/W 0  pwr by macid en when Pwr_mode=2 from MAC */
#define BIT_MASK_R_TXPWR_REG5                                       ((u32)0x000000FF << 8)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=7 from MAC */
#define BIT_R_TXPWR_REG5(x)                                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_TXPWR_REG5(x)                                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_TXPWR_REG4                                       ((u32)0x000000FF << 0)          /*!<R/W 0  S(8.1)pwr by macid when Pwr_mode=6 from MAC */
#define BIT_R_TXPWR_REG4(x)                                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_TXPWR_REG4(x)                                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_LTE                                        ((u32)0x000001FF << 19)          /*!<R/W 0  S(9.2) Coexist LTE pwr dbm */
#define BIT_R_TXAGC_LTE(x)                                          ((u32)(((x) & 0x000001FF) << 19))
#define BIT_GET_R_TXAGC_LTE(x)                                      ((u32)(((x >> 19) & 0x000001FF)))
#define BIT_R_TXPWR_RU_LIM_EN                                       ((u32)0x00000001 << 18)          /*!<R/W 1h  (RTL_only) 1:Ru lim indirect read */
#define BIT_R_TXPWR_RU_LIM_TBL_RD                                   ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) 1:Ru lim indirect read */
#define BIT_MASK_R_TXPWR_RU_LIM_TBL_RA                              ((u32)0x0000003F << 11)          /*!<R/W 0  (RTL_only) Ru lim indirect read addr */
#define BIT_R_TXPWR_RU_LIM_TBL_RA(x)                                ((u32)(((x) & 0x0000003F) << 11))
#define BIT_GET_R_TXPWR_RU_LIM_TBL_RA(x)                            ((u32)(((x >> 11) & 0x0000003F)))
#define BIT_R_FORCE_PWR_BY_RU_LIMIT_EN                              ((u32)0x00000001 << 10)          /*!<R/W 0  1: RU_lim = r_force_pwr_by_ru_limit_value */
#define BIT_R_FORCE_PWR_BY_RU_LIMIT_EN_VALUE                        ((u32)0x00000001 << 9)          /*!<R/W 1h  (ru lim val is valid in HE_TB) */
#define BIT_MASK_R_FORCE_PWR_BY_RU_LIMIT_VALUE                      ((u32)0x000001FF << 0)          /*!<R/W 0  Force RU lim_val enable */
#define BIT_R_FORCE_PWR_BY_RU_LIMIT_VALUE(x)                        ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_PWR_BY_RU_LIMIT_VALUE(x)                    ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_COEXT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CCK_NORM_TERM                                    ((u32)0x0000007F << 18)          /*!<R/W 7Ah  (RTL_only) u(6.4) Map txpwr_bb to txpwr_mac 0x7A=20log(20/11)=2.625 */
#define BIT_R_CCK_NORM_TERM(x)                                      ((u32)(((x) & 0x0000007F) << 18))
#define BIT_GET_R_CCK_NORM_TERM(x)                                  ((u32)(((x >> 18) & 0x0000007F)))
#define BIT_MASK_R_TXPWR_MAC_MAX_BND                                ((u32)0x0000003F << 12)          /*!<R/W 3Fh  (RTL_only) u(6.0) txpwr mac max bnd dB */
#define BIT_R_TXPWR_MAC_MAX_BND(x)                                  ((u32)(((x) & 0x0000003F) << 12))
#define BIT_GET_R_TXPWR_MAC_MAX_BND(x)                              ((u32)(((x >> 12) & 0x0000003F)))
#define BIT_MASK_R_TXAGC_BT                                         ((u32)0x000001FF << 3)          /*!<R/W 0  S(9.2) Coexist BT pwr dbm */
#define BIT_R_TXAGC_BT(x)                                           ((u32)(((x) & 0x000001FF) << 3))
#define BIT_GET_R_TXAGC_BT(x)                                       ((u32)(((x >> 3) & 0x000001FF)))
#define BIT_R_TXAGC_LTE_EN                                          ((u32)0x00000001 << 2)          /*!<R/W 1h  Coexist LTE_en */
#define BIT_R_TXAGC_BT_EN                                           ((u32)0x00000001 << 1)          /*!<R/W 1h  Coexist BT_en */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SWING_LEG_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXBIAS_LEGACY_BELOW_TH_VAL                       ((u32)0x00000003 << 29)          /*!<R/W 0  Bbswing , txbias mcs below th sel this */
#define BIT_R_TXBIAS_LEGACY_BELOW_TH_VAL(x)                         ((u32)(((x) & 0x00000003) << 29))
#define BIT_GET_R_TXBIAS_LEGACY_BELOW_TH_VAL(x)                     ((u32)(((x >> 29) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_LEGACY_OV_TH_VAL                          ((u32)0x00000003 << 27)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_LEGACY_OV_TH_VAL(x)                            ((u32)(((x) & 0x00000003) << 27))
#define BIT_GET_R_TXBIAS_LEGACY_OV_TH_VAL(x)                        ((u32)(((x >> 27) & 0x00000003)))
#define BIT_MASK_R_TXBBSWING_LEGACY_BELOW_TH_VAL                    ((u32)0x0000000F << 23)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBBSWING_LEGACY_BELOW_TH_VAL(x)                      ((u32)(((x) & 0x0000000F) << 23))
#define BIT_GET_R_TXBBSWING_LEGACY_BELOW_TH_VAL(x)                  ((u32)(((x >> 23) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_LEGACY_OV_TH_VAL                       ((u32)0x0000000F << 19)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBBSWING_LEGACY_OV_TH_VAL(x)                         ((u32)(((x) & 0x0000000F) << 19))
#define BIT_GET_R_TXBBSWING_LEGACY_OV_TH_VAL(x)                     ((u32)(((x >> 19) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_TXBIAS_LEGACY_TH                       ((u32)0x0000000F << 15)          /*!<R/W 4h  0~4 Bbswing txbias mcs th */
#define BIT_R_TXBBSWING_TXBIAS_LEGACY_TH(x)                         ((u32)(((x) & 0x0000000F) << 15))
#define BIT_GET_R_TXBBSWING_TXBIAS_LEGACY_TH(x)                     ((u32)(((x >> 15) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_CCK_BELOW_TH_VAL                          ((u32)0x00000003 << 13)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBIAS_CCK_BELOW_TH_VAL(x)                            ((u32)(((x) & 0x00000003) << 13))
#define BIT_GET_R_TXBIAS_CCK_BELOW_TH_VAL(x)                        ((u32)(((x >> 13) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_CCK_OV_TH_VAL                             ((u32)0x00000003 << 11)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_CCK_OV_TH_VAL(x)                               ((u32)(((x) & 0x00000003) << 11))
#define BIT_GET_R_TXBIAS_CCK_OV_TH_VAL(x)                           ((u32)(((x >> 11) & 0x00000003)))
#define BIT_MASK_R_TXBBSWING_CCK_BELOW_TH_VAL                       ((u32)0x0000000F << 7)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBBSWING_CCK_BELOW_TH_VAL(x)                         ((u32)(((x) & 0x0000000F) << 7))
#define BIT_GET_R_TXBBSWING_CCK_BELOW_TH_VAL(x)                     ((u32)(((x >> 7) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_CCK_OV_TH_VAL                          ((u32)0x0000000F << 3)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBBSWING_CCK_OV_TH_VAL(x)                            ((u32)(((x) & 0x0000000F) << 3))
#define BIT_GET_R_TXBBSWING_CCK_OV_TH_VAL(x)                        ((u32)(((x >> 3) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_TXBIAS_CCK_TH                          ((u32)0x00000007 << 0)          /*!<R/W 0  0~4 Bbswing txbias mcs th */
#define BIT_R_TXBBSWING_TXBIAS_CCK_TH(x)                            ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_TXBBSWING_TXBIAS_CCK_TH(x)                        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SWING_VHT_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXBIAS_VHT_OV_TH_VAL                             ((u32)0x00000003 << 30)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_VHT_OV_TH_VAL(x)                               ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_TXBIAS_VHT_OV_TH_VAL(x)                           ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_TXBBSWING_VHT_BELOW_TH_VAL                       ((u32)0x0000000F << 26)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBBSWING_VHT_BELOW_TH_VAL(x)                         ((u32)(((x) & 0x0000000F) << 26))
#define BIT_GET_R_TXBBSWING_VHT_BELOW_TH_VAL(x)                     ((u32)(((x >> 26) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_VHT_OV_TH_VAL                          ((u32)0x0000000F << 22)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBBSWING_VHT_OV_TH_VAL(x)                            ((u32)(((x) & 0x0000000F) << 22))
#define BIT_GET_R_TXBBSWING_VHT_OV_TH_VAL(x)                        ((u32)(((x >> 22) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_TXBIAS_VHT_TH                          ((u32)0x0000000F << 18)          /*!<R/W 0  0~12 Bbswing txbias mcs th */
#define BIT_R_TXBBSWING_TXBIAS_VHT_TH(x)                            ((u32)(((x) & 0x0000000F) << 18))
#define BIT_GET_R_TXBBSWING_TXBIAS_VHT_TH(x)                        ((u32)(((x >> 18) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HT_BELOW_TH_VAL                           ((u32)0x00000003 << 16)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBIAS_HT_BELOW_TH_VAL(x)                             ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_TXBIAS_HT_BELOW_TH_VAL(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HT_OV_TH_VAL                              ((u32)0x00000003 << 14)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_HT_OV_TH_VAL(x)                                ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_TXBIAS_HT_OV_TH_VAL(x)                            ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_TXBBSWING_HT_BELOW_TH_VAL                        ((u32)0x0000000F << 10)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBBSWING_HT_BELOW_TH_VAL(x)                          ((u32)(((x) & 0x0000000F) << 10))
#define BIT_GET_R_TXBBSWING_HT_BELOW_TH_VAL(x)                      ((u32)(((x >> 10) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_HT_OV_TH_VAL                           ((u32)0x0000000F << 6)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBBSWING_HT_OV_TH_VAL(x)                             ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_R_TXBBSWING_HT_OV_TH_VAL(x)                         ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_TXBIAS_HT_TH                           ((u32)0x0000000F << 2)          /*!<R/W 0  0~8 Bbswing txbias mcs th */
#define BIT_R_TXBBSWING_TXBIAS_HT_TH(x)                             ((u32)(((x) & 0x0000000F) << 2))
#define BIT_GET_R_TXBBSWING_TXBIAS_HT_TH(x)                         ((u32)(((x >> 2) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SWING_HE_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_DPD_BY_RATE_OFF_LEGACY_BELOW_TH_VAL                   ((u32)0x00000001 << 28)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_DPD_BY_RATE_OFF_LEGACY_OV_TH_VAL                      ((u32)0x00000001 << 27)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_MASK_R_DPD_BY_RATE_OFF_LEGACY_TH                        ((u32)0x0000000F << 23)          /*!<R/W 0  4~8 Bbswing txbias mcs th */
#define BIT_R_DPD_BY_RATE_OFF_LEGACY_TH(x)                          ((u32)(((x) & 0x0000000F) << 23))
#define BIT_GET_R_DPD_BY_RATE_OFF_LEGACY_TH(x)                      ((u32)(((x >> 23) & 0x0000000F)))
#define BIT_R_DPD_BY_RATE_OFF_CCK_BELOW_TH_VAL                      ((u32)0x00000001 << 22)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_DPD_BY_RATE_OFF_CCK_OV_TH_VAL                         ((u32)0x00000001 << 21)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_MASK_R_DPD_BY_RATE_OFF_CCK_TH                           ((u32)0x00000007 << 18)          /*!<R/W 0  0~4 Bbswing txbias mcs th */
#define BIT_R_DPD_BY_RATE_OFF_CCK_TH(x)                             ((u32)(((x) & 0x00000007) << 18))
#define BIT_GET_R_DPD_BY_RATE_OFF_CCK_TH(x)                         ((u32)(((x >> 18) & 0x00000007)))
#define BIT_MASK_R_TXBIAS_HE_BELOW_TH_VAL                           ((u32)0x00000003 << 16)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBIAS_HE_BELOW_TH_VAL(x)                             ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_TXBIAS_HE_BELOW_TH_VAL(x)                         ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HE_OV_TH_VAL                              ((u32)0x00000003 << 14)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_HE_OV_TH_VAL(x)                                ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_TXBIAS_HE_OV_TH_VAL(x)                            ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_TXBBSWING_HE_BELOW_TH_VAL                        ((u32)0x0000000F << 10)          /*!<R/W 0  Bbswing ,txbias mcs below th sel this */
#define BIT_R_TXBBSWING_HE_BELOW_TH_VAL(x)                          ((u32)(((x) & 0x0000000F) << 10))
#define BIT_GET_R_TXBBSWING_HE_BELOW_TH_VAL(x)                      ((u32)(((x >> 10) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_HE_OV_TH_VAL                           ((u32)0x0000000F << 6)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBBSWING_HE_OV_TH_VAL(x)                             ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_R_TXBBSWING_HE_OV_TH_VAL(x)                         ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_MASK_R_TXBBSWING_TXBIAS_HE_TH                           ((u32)0x0000000F << 2)          /*!<R/W 0  0~12 Bbswing txbias mcs th */
#define BIT_R_TXBBSWING_TXBIAS_HE_TH(x)                             ((u32)(((x) & 0x0000000F) << 2))
#define BIT_GET_R_TXBBSWING_TXBIAS_HE_TH(x)                         ((u32)(((x >> 2) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_VHT_BELOW_TH_VAL                          ((u32)0x00000003 << 0)          /*!<R/W 0  Bbswing ,txbias mcs ov th sel this */
#define BIT_R_TXBIAS_VHT_BELOW_TH_VAL(x)                            ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_TXBIAS_VHT_BELOW_TH_VAL(x)                        ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SWING_OTHER_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_CFIR_BY_RATE_OFF_LEGACY_BELOW_TH_VAL                  ((u32)0x00000001 << 28)          /*!<R/W 0  dpd by rate off mcs below th sel this */
#define BIT_R_CFIR_BY_RATE_OFF_LEGACY_OV_TH_VAL                     ((u32)0x00000001 << 27)          /*!<R/W 0  dpd by rate off mcs ov th sel this */
#define BIT_MASK_R_CFIR_BY_RATE_OFF_LEGACY_TH                       ((u32)0x0000000F << 23)          /*!<R/W 0  4~8 dpd by rate off mcs ov th */
#define BIT_R_CFIR_BY_RATE_OFF_LEGACY_TH(x)                         ((u32)(((x) & 0x0000000F) << 23))
#define BIT_GET_R_CFIR_BY_RATE_OFF_LEGACY_TH(x)                     ((u32)(((x >> 23) & 0x0000000F)))
#define BIT_R_CFIR_BY_RATE_OFF_CCK_BELOW_TH_VAL                     ((u32)0x00000001 << 22)          /*!<R/W 0  dpd by rate off mcs below th sel this */
#define BIT_R_CFIR_BY_RATE_OFF_CCK_OV_TH_VAL                        ((u32)0x00000001 << 21)          /*!<R/W 0  dpd by rate off mcs ov th sel this */
#define BIT_MASK_R_CFIR_BY_RATE_OFF_CCK_TH                          ((u32)0x00000007 << 18)          /*!<R/W 0  0~4 dpd by rate off mcs th */
#define BIT_R_CFIR_BY_RATE_OFF_CCK_TH(x)                            ((u32)(((x) & 0x00000007) << 18))
#define BIT_GET_R_CFIR_BY_RATE_OFF_CCK_TH(x)                        ((u32)(((x >> 18) & 0x00000007)))
#define BIT_R_DPD_BY_RATE_OFF_HE_BELOW_TH_VAL                       ((u32)0x00000001 << 17)          /*!<R/W 0  cfir by rate_off mcs below th sel this */
#define BIT_R_DPD_BY_RATE_OFF_HE_OV_TH_VAL                          ((u32)0x00000001 << 16)          /*!<R/W 0  cfir by rate_off mcs ov th sel this */
#define BIT_MASK_R_DPD_BY_RATE_OFF_HE_TH                            ((u32)0x0000000F << 12)          /*!<R/W 0  0~12 cfir by rate_off mcs th */
#define BIT_R_DPD_BY_RATE_OFF_HE_TH(x)                              ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_DPD_BY_RATE_OFF_HE_TH(x)                          ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_R_DPD_BY_RATE_OFF_VHT_BELOW_TH_VAL                      ((u32)0x00000001 << 11)          /*!<R/W 0  cfir by rate_off mcs below th sel this */
#define BIT_R_DPD_BY_RATE_OFF_VHT_OV_TH_VAL                         ((u32)0x00000001 << 10)          /*!<R/W 0  cfir by rate_off mcs ov th sel this */
#define BIT_MASK_R_DPD_BY_RATE_OFF_VHT_TH                           ((u32)0x0000000F << 6)          /*!<R/W 0  0~12 cfir by rate_off mcs th */
#define BIT_R_DPD_BY_RATE_OFF_VHT_TH(x)                             ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_R_DPD_BY_RATE_OFF_VHT_TH(x)                         ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_R_DPD_BY_RATE_OFF_HT_BELOW_TH_VAL                       ((u32)0x00000001 << 5)          /*!<R/W 0  cfir by rate_off mcs below th sel this */
#define BIT_R_DPD_BY_RATE_OFF_HT_OV_TH_VAL                          ((u32)0x00000001 << 4)          /*!<R/W 0  cfir by rate_off mcs ov th sel this */
#define BIT_MASK_R_DPD_BY_RATE_OFF_HT_TH                            ((u32)0x0000000F << 0)          /*!<R/W 0  0~8 cfir by rate_off mcs th */
#define BIT_R_DPD_BY_RATE_OFF_HT_TH(x)                              ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_DPD_BY_RATE_OFF_HT_TH(x)                          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SWING_OTHER_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_CFIR_BY_RATE_OFF_SR_DONT_APPLY                        ((u32)0x00000001 << 30)          /*!<R/W 0  (RTL_only) 1:Disable at SR */
#define BIT_R_DPD_BY_RATE_OFF_SR_DONT_APPLY                         ((u32)0x00000001 << 29)          /*!<R/W 0  (RTL_only) 1:Disable at SR */
#define BIT_R_TXBBSWING_TXBIAS_SR_DONT_APPLY                        ((u32)0x00000001 << 28)          /*!<R/W 0  (RTL_only) 1:Disable at SR */
#define BIT_R_CFIR_BY_RATE_OFF_HE_BELOW_TH_VAL                      ((u32)0x00000001 << 17)          /*!<R/W 0  dpd by rate off mcs below th sel this */
#define BIT_R_CFIR_BY_RATE_OFF_HE_OV_TH_VAL                         ((u32)0x00000001 << 16)          /*!<R/W 0  dpd by rate off mcs ov th sel this */
#define BIT_MASK_R_CFIR_BY_RATE_OFF_HE_TH                           ((u32)0x0000000F << 12)          /*!<R/W 0  0~12 dpd by rate off mcs ov th */
#define BIT_R_CFIR_BY_RATE_OFF_HE_TH(x)                             ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CFIR_BY_RATE_OFF_HE_TH(x)                         ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_R_CFIR_BY_RATE_OFF_VHT_BELOW_TH_VAL                     ((u32)0x00000001 << 11)          /*!<R/W 0  dpd by rate off mcs below th sel this */
#define BIT_R_CFIR_BY_RATE_OFF_VHT_OV_TH_VAL                        ((u32)0x00000001 << 10)          /*!<R/W 0  dpd by rate off mcs ov th sel this */
#define BIT_MASK_R_CFIR_BY_RATE_OFF_VHT_TH                          ((u32)0x0000000F << 6)          /*!<R/W 0  0~12 dpd by rate off mcs ov th */
#define BIT_R_CFIR_BY_RATE_OFF_VHT_TH(x)                            ((u32)(((x) & 0x0000000F) << 6))
#define BIT_GET_R_CFIR_BY_RATE_OFF_VHT_TH(x)                        ((u32)(((x >> 6) & 0x0000000F)))
#define BIT_R_CFIR_BY_RATE_OFF_HT_BELOW_TH_VAL                      ((u32)0x00000001 << 5)          /*!<R/W 0  dpd by rate off mcs below th sel this */
#define BIT_R_CFIR_BY_RATE_OFF_HT_OV_TH_VAL                         ((u32)0x00000001 << 4)          /*!<R/W 0  dpd by rate off mcs ov th sel this */
#define BIT_MASK_R_CFIR_BY_RATE_OFF_HT_TH                           ((u32)0x0000000F << 0)          /*!<R/W 0  0~8 dpd by rate off mcs ov th */
#define BIT_R_CFIR_BY_RATE_OFF_HT_TH(x)                             ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_CFIR_BY_RATE_OFF_HT_TH(x)                         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_SR_PWR_CTRL_DBG_EN                                    ((u32)0x00000001 << 31)          /*!<R/W 1h  (RTL_only) 0:saving pwr */
#define BIT_MASK_R_SR_RATE_TBL_RD_MCS_TXDIFF                        ((u32)0x0000001F << 20)          /*!<R/W 0  (RTL_only) Rd MEM idx 0~47 */
#define BIT_R_SR_RATE_TBL_RD_MCS_TXDIFF(x)                          ((u32)(((x) & 0x0000001F) << 20))
#define BIT_GET_R_SR_RATE_TBL_RD_MCS_TXDIFF(x)                      ((u32)(((x >> 20) & 0x0000001F)))
#define BIT_R_SR_RATE_MAP_TBL_RD_MCS_SEL                            ((u32)0x00000001 << 19)          /*!<R/W 0  (RTL_only) 1:cckle 0~11 0:ht/vht/he 0~11 */
#define BIT_R_TXPWR_SR_FORCE_OFF                                    ((u32)0x00000001 << 18)          /*!<R/W 0  (RTL_only) 1:force sr mode off */
#define BIT_R_TXPWR_STA_UL_FORCE_OFF                                ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) 1:force sta mode off */
#define BIT_R_SR_TXPWR_PD_WITH_PD_MACID                             ((u32)0x00000001 << 16)          /*!<R/W 1h  (RTL_only) 1:txpwr_PD from internal buffer 0: from MAC */
#define BIT_R_SR_RATE_MAP_TBL_RD                                    ((u32)0x00000001 << 15)          /*!<R/W 0  (RTL_only) 1:indirect read of addr */
#define BIT_MASK_R_SR_RATE_MAP_TBL_RD_MCS                           ((u32)0x0000000F << 11)          /*!<R/W 0  (RTL_only) indirect read of addr */
#define BIT_R_SR_RATE_MAP_TBL_RD_MCS(x)                             ((u32)(((x) & 0x0000000F) << 11))
#define BIT_GET_R_SR_RATE_MAP_TBL_RD_MCS(x)                         ((u32)(((x >> 11) & 0x0000000F)))
#define BIT_R_TXPWR_CTRL_NORM_RESP_DBG_EN                           ((u32)0x00000001 << 10)          /*!<R/W 1h  (RTL_only) 0:dbg clk off for saving pwr */
#define BIT_R_TXAGC_PSEUDO_PWR_EN                                   ((u32)0x00000001 << 9)          /*!<R/W 0  (RTL_only) 1: force on norm resp out txpwr_bb */
#define BIT_MASK_R_TXAGC_PSEUDO_PWR                                 ((u32)0x000001FF << 0)          /*!<R/W 0  (RTL_only) S(9.2) force norm resp out txpwr_bb */
#define BIT_R_TXAGC_PSEUDO_PWR(x)                                   ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_TXAGC_PSEUDO_PWR(x)                               ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MCS_TH_HE                                        ((u32)0x0000000F << 24)          /*!<R/W Bh  SR MCS0~MCS11 */
#define BIT_R_MCS_TH_HE(x)                                          ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_R_MCS_TH_HE(x)                                      ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_R_MCS_TH_VHT                                       ((u32)0x0000000F << 20)          /*!<R/W Bh  SR MCS0~MCS11 */
#define BIT_R_MCS_TH_VHT(x)                                         ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_R_MCS_TH_VHT(x)                                     ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_R_MCS_TH_HT_MOD8                                   ((u32)0x0000000F << 16)          /*!<R/W 7h  SR MCS0~MCS7 */
#define BIT_R_MCS_TH_HT_MOD8(x)                                     ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_MCS_TH_HT_MOD8(x)                                 ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_MCS_TH_LEGACY                                    ((u32)0x0000000F << 12)          /*!<R/W 7h  SR 4~11 */
#define BIT_R_MCS_TH_LEGACY(x)                                      ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_MCS_TH_LEGACY(x)                                  ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_MCS_TH_CCK                                       ((u32)0x0000000F << 8)          /*!<R/W 3h  SR 0~3 */
#define BIT_R_MCS_TH_CCK(x)                                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_MCS_TH_CCK(x)                                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_TXPWR_REF                                        ((u32)0x0000007F << 0)          /*!<R/W 15h  SR txpwr ref(txpwr_mcs diff) */
#define BIT_R_TXPWR_REF(x)                                          ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXPWR_REF(x)                                      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_BYPASS_ABORT_TX_FLAG                                  ((u32)0x00000001 << 29)          /*!<R/W 0  1: force off abort_tx flag STA_UL */
#define BIT_MASK_R_PL_TOLER_RANGE                                   ((u32)0x000001FF << 20)          /*!<R/W 0  U(9.2) STA_UL */
#define BIT_R_PL_TOLER_RANGE(x)                                     ((u32)(((x) & 0x000001FF) << 20))
#define BIT_GET_R_PL_TOLER_RANGE(x)                                 ((u32)(((x >> 20) & 0x000001FF)))
#define BIT_MASK_R_PWR_BB_MIN_DBM                                   ((u32)0x000001FF << 9)          /*!<R/W 1FCh  S(9.2) STA_UL */
#define BIT_R_PWR_BB_MIN_DBM(x)                                     ((u32)(((x) & 0x000001FF) << 9))
#define BIT_GET_R_PWR_BB_MIN_DBM(x)                                 ((u32)(((x >> 9) & 0x000001FF)))
#define BIT_MASK_R_PWR_ERROR_TOLER                                  ((u32)0x000000FF << 0)          /*!<R/W 8h  S(9.2) STA_UL */
#define BIT_R_PWR_ERROR_TOLER(x)                                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_PWR_ERROR_TOLER(x)                                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MACID3                                           ((u32)0x0000007F << 24)          /*!<R/W 0  STA_UL */
#define BIT_R_MACID3(x)                                             ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_MACID3(x)                                         ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_MACID2                                           ((u32)0x0000007F << 16)          /*!<R/W 0  STA_UL */
#define BIT_R_MACID2(x)                                             ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_MACID2(x)                                         ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_MACID1                                           ((u32)0x0000007F << 8)          /*!<R/W 0  STA_UL */
#define BIT_R_MACID1(x)                                             ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_MACID1(x)                                         ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_MACID0                                           ((u32)0x0000007F << 0)          /*!<R/W 0  STA_UL */
#define BIT_R_MACID0(x)                                             ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_MACID0(x)                                         ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CFO_COMP_SR                                      ((u32)0x00000007 << 24)          /*!<R/W 7h  (RTL_only) STA_UL no need to set */
#define BIT_R_CFO_COMP_SR(x)                                        ((u32)(((x) & 0x00000007) << 24))
#define BIT_GET_R_CFO_COMP_SR(x)                                    ((u32)(((x >> 24) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP_NORM_RESP                               ((u32)0x00000007 << 20)          /*!<R/W 7h  (RTL_only) STA_UL no need to set */
#define BIT_R_CFO_COMP_NORM_RESP(x)                                 ((u32)(((x) & 0x00000007) << 20))
#define BIT_GET_R_CFO_COMP_NORM_RESP(x)                             ((u32)(((x >> 20) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP4                                        ((u32)0x00000007 << 16)          /*!<R/W 6h  (RTL_only) STA_UL Default 6 no need to set */
#define BIT_R_CFO_COMP4(x)                                          ((u32)(((x) & 0x00000007) << 16))
#define BIT_GET_R_CFO_COMP4(x)                                      ((u32)(((x >> 16) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP3                                        ((u32)0x00000007 << 12)          /*!<R/W 7h  STA_UL */
#define BIT_R_CFO_COMP3(x)                                          ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_R_CFO_COMP3(x)                                      ((u32)(((x >> 12) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP2                                        ((u32)0x00000007 << 8)          /*!<R/W 7h  STA_UL */
#define BIT_R_CFO_COMP2(x)                                          ((u32)(((x) & 0x00000007) << 8))
#define BIT_GET_R_CFO_COMP2(x)                                      ((u32)(((x >> 8) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP1                                        ((u32)0x00000007 << 4)          /*!<R/W 7h  STA_UL */
#define BIT_R_CFO_COMP1(x)                                          ((u32)(((x) & 0x00000007) << 4))
#define BIT_GET_R_CFO_COMP1(x)                                      ((u32)(((x >> 4) & 0x00000007)))
#define BIT_MASK_R_CFO_COMP0                                        ((u32)0x00000007 << 0)          /*!<R/W 7h  STA_UL */
#define BIT_R_CFO_COMP0(x)                                          ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_CFO_COMP0(x)                                      ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TF_RDY_TXBF_FORCE_OFF                                 ((u32)0x00000001 << 22)          /*!<R/W 1h  (RTL_only) Jorden Sta_UL doesnt have TxBF */
#define BIT_R_FORCE_PL_UPPER_EN_EQUL_N_TX_DIVIDE2                   ((u32)0x00000001 << 21)          /*!<R/W 0  (RTL_only) 1:PL_upper_en = is N_tx/2 */
#define BIT_R_FORCE_N_TX_DIVIDE2_OFF                                ((u32)0x00000001 << 20)          /*!<R/W 0  (RTL_only) 1:force off N_tx/2 */
#define BIT_MASK_R_STA_PWR_CTRL_PWRMAX_LIM_MAX                      ((u32)0x000001FF << 11)          /*!<R/W FFh  (RTL_only) S(9.2) Limit pwr from norm resp */
#define BIT_R_STA_PWR_CTRL_PWRMAX_LIM_MAX(x)                        ((u32)(((x) & 0x000001FF) << 11))
#define BIT_GET_R_STA_PWR_CTRL_PWRMAX_LIM_MAX(x)                    ((u32)(((x >> 11) & 0x000001FF)))
#define BIT_MASK_R_STA_PWR_CTRL_PWRMAX_LIM_MIN                      ((u32)0x000001FF << 0)          /*!<R/W 100h  (RTL_only) S(9.2) Limit pwr from norm resp */
#define BIT_R_STA_PWR_CTRL_PWRMAX_LIM_MIN(x)                        ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_STA_PWR_CTRL_PWRMAX_LIM_MIN(x)                    ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_STA_PWR_CTRL_RPL_LIM_MIN                         ((u32)0x000003FF << 16)          /*!<R/W 248h  (RTL_only) S(10.2) Min=-110 (-440) Limit RPL */
#define BIT_R_STA_PWR_CTRL_RPL_LIM_MIN(x)                           ((u32)(((x) & 0x000003FF) << 16))
#define BIT_GET_R_STA_PWR_CTRL_RPL_LIM_MIN(x)                       ((u32)(((x >> 16) & 0x000003FF)))
#define BIT_MASK_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MAX                 ((u32)0x000000FF << 8)          /*!<R/W 0  (RTL_only) S(8.0) Max:0 Limit RSSI_Target */
#define BIT_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MAX(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MAX(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MIN                 ((u32)0x000000FF << 0)          /*!<R/W 92h  (RTL_only) S(8.0) Min=-110 Limit RSSI_Target */
#define BIT_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MIN(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_STA_PWR_CTRL_RSSI_TARGET_LIM_MIN(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_SR_TXPWR_RESP_RDY                                     ((u32)0x00000001 << 28)          /*!<R/W 0  (RTL_only) 1: trig when r_SR_txpwr_resp_rdy_force_on */
#define BIT_R_SR_TXPWR_PD_RDY                                       ((u32)0x00000001 << 27)          /*!<R/W 0  (RTL_only) 1: trig when r_SR_txpwr_pd_rdy_force_on */
#define BIT_R_SR_TXPWR_RATE_RDY                                     ((u32)0x00000001 << 26)          /*!<R/W 0  (RTL_only) 1: trig when r_SR_txpwr_rate_rdy_force_on */
#define BIT_R_TF_RDY                                                ((u32)0x00000001 << 25)          /*!<R/W 0  (RTL_only) 1: trig when r_TF_rdy_force_on */
#define BIT_R_TXCTRL_INFO_RDY                                       ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) 1: trig when r_TxCtrl_info_rdy_force_on */
#define BIT_R_FORCE_PL_UPPER_EN_VAL                                 ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_FORCE_PL_UPPER_EN_ON                                  ((u32)0x00000001 << 22)          /*!<R/W 0  (RTL_only) 1 : PL_upper_en = r_force_PL_upper_en_val */
#define BIT_R_SR_TXPWR_RESP_RDY_FORCE_ON                            ((u32)0x00000001 << 15)          /*!<R/W 0  (RTL_only) 1:force off SR resp trigger */
#define BIT_R_SR_TXPWR_PD_RDY_FORCE_ON                              ((u32)0x00000001 << 14)          /*!<R/W 0  (RTL_only) 1:force off SR MCS flow trigger */
#define BIT_R_SR_TXPWR_RATE_RDY_FORCE_ON                            ((u32)0x00000001 << 13)          /*!<R/W 0  (RTL_only) 1:force off SR PD flow trigger */
#define BIT_R_TF_RDY_FORCE_ON                                       ((u32)0x00000001 << 12)          /*!<R/W 0  (RTL_only) 1:force off TF trigger */
#define BIT_R_TXCTRL_INFO_RDY_FORCE_ON                              ((u32)0x00000001 << 11)          /*!<R/W 0  (RTL_only) 1:force off norm resp trigger */
#define BIT_R_STA_PWR_CTRL_DBG_EN                                   ((u32)0x00000001 << 10)          /*!<R/W 0  (RTL_only) 0: pwr saving */
#define BIT_MASK_R_STA_PWR_CTRL_RPL_LIM_MAX                         ((u32)0x000003FF << 0)          /*!<R/W 0  (RTL_only) S(10.2) Max:0 Limit RPL */
#define BIT_R_STA_PWR_CTRL_RPL_LIM_MAX(x)                           ((u32)(((x) & 0x000003FF) << 0))
#define BIT_GET_R_STA_PWR_CTRL_RPL_LIM_MAX(x)                       ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_CTRL6
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_STA_PWR_CTRL_AP_TXPWR_LIM_MIN                    ((u32)0x0000007F << 24)          /*!<R/W 6Ch  (RTL_only) S(7.0 ) MIN=-20 Limit AP TXPWR */
#define BIT_R_STA_PWR_CTRL_AP_TXPWR_LIM_MIN(x)                      ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_STA_PWR_CTRL_AP_TXPWR_LIM_MIN(x)                  ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_STA_PWR_CTRL_AP_TXPWR_LIM_MAX                    ((u32)0x0000007F << 16)          /*!<R/W 28h  (RTL_only) S(7.0 ) MAX=+40 Limit AP TXPWR */
#define BIT_R_STA_PWR_CTRL_AP_TXPWR_LIM_MAX(x)                      ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_STA_PWR_CTRL_AP_TXPWR_LIM_MAX(x)                  ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_R_TXPWR_FORCE_RATE_EN                                   ((u32)0x00000001 << 12)          /*!<R/W 0  (RTL_only) No use */
#define BIT_MASK_R_TXPWR_FORCE_RATE                                 ((u32)0x00000FFF << 0)          /*!<R/W 0  (RTL_only) No use */
#define BIT_R_TXPWR_FORCE_RATE(x)                                   ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_R_TXPWR_FORCE_RATE(x)                               ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_NORM_FORCE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_MACID_EN                                        ((u32)0x00000001 << 30)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_MASK_R_FORCE_MACID_VALUE                                ((u32)0x0000007F << 23)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_MACID_VALUE(x)                                  ((u32)(((x) & 0x0000007F) << 23))
#define BIT_GET_R_FORCE_MACID_VALUE(x)                              ((u32)(((x >> 23) & 0x0000007F)))
#define BIT_R_FORCE_BW80P80_EN_EN                                   ((u32)0x00000001 << 22)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_BW80P80_EN_VALUE                                ((u32)0x00000001 << 21)          /*!<R/W 1h  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RF_BW_IDX_EN                                    ((u32)0x00000001 << 20)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_MASK_R_FORCE_RF_BW_IDX_VALUE                            ((u32)0x00000003 << 18)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RF_BW_IDX_VALUE(x)                              ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_R_FORCE_RF_BW_IDX_VALUE(x)                          ((u32)(((x >> 18) & 0x00000003)))
#define BIT_R_FORCE_DCM_EN                                          ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_DCM_VALUE                                       ((u32)0x00000001 << 16)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_DBW_IDX_EN                                      ((u32)0x00000001 << 15)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_MASK_R_FORCE_DBW_IDX_VALUE                              ((u32)0x00000003 << 13)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_DBW_IDX_VALUE(x)                                ((u32)(((x) & 0x00000003) << 13))
#define BIT_GET_R_FORCE_DBW_IDX_VALUE(x)                            ((u32)(((x >> 13) & 0x00000003)))
#define BIT_R_FORCE_MAX_RATE_EN                                     ((u32)0x00000001 << 12)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_MASK_R_FORCE_MAX_RATE_VALUE                             ((u32)0x00000FFF << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_MAX_RATE_VALUE(x)                               ((u32)(((x) & 0x00000FFF) << 0))
#define BIT_GET_R_FORCE_MAX_RATE_VALUE(x)                           ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_NORM_FORCE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TXAGC_BF_PWR_BOOST_FORCE_VAL_EN                       ((u32)0x00000001 << 29)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_TXAGC_BF_PWR_BOOST_FORCE_VAL                     ((u32)0x0000001F << 24)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_TXAGC_BF_PWR_BOOST_FORCE_VAL(x)                       ((u32)(((x) & 0x0000001F) << 24))
#define BIT_GET_R_TXAGC_BF_PWR_BOOST_FORCE_VAL(x)                   ((u32)(((x >> 24) & 0x0000001F)))
#define BIT_R_FORCE_HE_ER_SU_EN_EN                                  ((u32)0x00000001 << 23)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_HE_ER_SU_EN_VALUE                               ((u32)0x00000001 << 22)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_MACID_CCA_TH_EN_EN                              ((u32)0x00000001 << 21)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_MACID_CCA_TH_EN_VALUE                           ((u32)0x00000001 << 20)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_BT_GRANT_EN                                     ((u32)0x00000001 << 19)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_BT_GRANT_VALUE                                  ((u32)0x00000001 << 18)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RX_LTE_EN                                       ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_RX_LTE_VALUE                                    ((u32)0x00000001 << 16)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TXBF_EN_EN                                      ((u32)0x00000001 << 15)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_TXBF_EN_VALUE                                   ((u32)0x00000001 << 14)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TXSC_EN                                         ((u32)0x00000001 << 13)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_TXSC_VALUE                                 ((u32)0x0000000F << 9)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TXSC_VALUE(x)                                   ((u32)(((x) & 0x0000000F) << 9))
#define BIT_GET_R_FORCE_TXSC_VALUE(x)                               ((u32)(((x >> 9) & 0x0000000F)))
#define BIT_R_FORCE_NTX_EN                                          ((u32)0x00000001 << 6)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_NTX_VALUE                                       ((u32)0x00000001 << 5)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_PWR_MODE_EN                                     ((u32)0x00000001 << 3)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_PWR_MODE_VALUE                             ((u32)0x00000007 << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_PWR_MODE_VALUE(x)                               ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_FORCE_PWR_MODE_VALUE(x)                           ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_FORCE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_SR_RESP_DCM_EN                                  ((u32)0x00000001 << 30)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_SR_RESP_DCM_VALUE                               ((u32)0x00000001 << 29)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_RATE_DCM_EN                                  ((u32)0x00000001 << 28)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_SR_RATE_DCM_VALUE                               ((u32)0x00000001 << 27)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_TXPWR_PD_EN                                  ((u32)0x00000001 << 26)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_SR_TXPWR_PD_VALUE                          ((u32)0x0000003F << 20)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_TXPWR_PD_VALUE(x)                            ((u32)(((x) & 0x0000003F) << 20))
#define BIT_GET_R_FORCE_SR_TXPWR_PD_VALUE(x)                        ((u32)(((x >> 20) & 0x0000003F)))
#define BIT_R_FORCE_SR_RATE_IDX_EN                                  ((u32)0x00000001 << 19)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_SR_RATE_IDX_VALUE                          ((u32)0x00000FFF << 7)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_RATE_IDX_VALUE(x)                            ((u32)(((x) & 0x00000FFF) << 7))
#define BIT_GET_R_FORCE_SR_RATE_IDX_VALUE(x)                        ((u32)(((x >> 7) & 0x00000FFF)))
#define BIT_R_FORCE_SR_TXPWR_TOLERANCE_EN                           ((u32)0x00000001 << 6)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_SR_TXPWR_TOLERANCE_VALUE                   ((u32)0x0000003F << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_TXPWR_TOLERANCE_VALUE(x)                     ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_FORCE_SR_TXPWR_TOLERANCE_VALUE(x)                 ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_FORCE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_RPL_EN                                          ((u32)0x00000001 << 29)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_RPL_VALUE                                  ((u32)0x000001FF << 20)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RPL_VALUE(x)                                    ((u32)(((x) & 0x000001FF) << 20))
#define BIT_GET_R_FORCE_RPL_VALUE(x)                                ((u32)(((x >> 20) & 0x000001FF)))
#define BIT_R_FORCE_SR_RESP_RATE_IDX_EN                             ((u32)0x00000001 << 19)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_SR_RESP_RATE_IDX_VALUE                     ((u32)0x00000FFF << 7)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_RESP_RATE_IDX_VALUE(x)                       ((u32)(((x) & 0x00000FFF) << 7))
#define BIT_GET_R_FORCE_SR_RESP_RATE_IDX_VALUE(x)                   ((u32)(((x >> 7) & 0x00000FFF)))
#define BIT_R_FORCE_SR_RESP_TXPWR_PD_EN                             ((u32)0x00000001 << 6)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_SR_RESP_TXPWR_PD_VALUE                     ((u32)0x0000003F << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_SR_RESP_TXPWR_PD_VALUE(x)                       ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_FORCE_SR_RESP_TXPWR_PD_VALUE(x)                   ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_FORCE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_TF_AP_TX_PWR_EN                                 ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_TF_AP_TX_PWR_VALUE                         ((u32)0x0000003F << 25)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TF_AP_TX_PWR_VALUE(x)                           ((u32)(((x) & 0x0000003F) << 25))
#define BIT_GET_R_FORCE_TF_AP_TX_PWR_VALUE(x)                       ((u32)(((x >> 25) & 0x0000003F)))
#define BIT_R_FORCE_TF_RATE_IDX_EN                                  ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_TF_RATE_IDX_VALUE                          ((u32)0x00000FFF << 12)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TF_RATE_IDX_VALUE(x)                            ((u32)(((x) & 0x00000FFF) << 12))
#define BIT_GET_R_FORCE_TF_RATE_IDX_VALUE(x)                        ((u32)(((x >> 12) & 0x00000FFF)))
#define BIT_R_FORCE_RPL_UPPER_EN_EN                                 ((u32)0x00000001 << 11)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_R_FORCE_RPL_UPPER_EN_VALUE                              ((u32)0x00000001 << 10)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RPL_UPPER_EN                                    ((u32)0x00000001 << 9)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_RPL_UPPER_VALUE                            ((u32)0x000001FF << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RPL_UPPER_VALUE(x)                              ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_RPL_UPPER_VALUE(x)                          ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_FORCE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_RU_ALLOC_EN                                     ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_RU_ALLOC_VALUE                             ((u32)0x000000FF << 16)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_RU_ALLOC_VALUE(x)                               ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_FORCE_RU_ALLOC_VALUE(x)                           ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_R_FORCE_TF_MACID_EN                                     ((u32)0x00000001 << 15)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_TF_MACID_VALUE                             ((u32)0x0000007F << 8)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TF_MACID_VALUE(x)                               ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_FORCE_TF_MACID_VALUE(x)                           ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_R_FORCE_TF_RSSI_TARGET_EN                               ((u32)0x00000001 << 7)          /*!<R/W 0  (RTL_only) Force input from MAC en */
#define BIT_MASK_R_FORCE_TF_RSSI_TARGET_VALUE                       ((u32)0x0000007F << 0)          /*!<R/W 0  (RTL_only) Force input from MAC */
#define BIT_R_FORCE_TF_RSSI_TARGET_VALUE(x)                         ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_FORCE_TF_RSSI_TARGET_VALUE(x)                     ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_NORM_FORCE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_OUT_CCA_PWR_TH_EN_ON                            ((u32)0x00000001 << 18)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_R_FORCE_OUT_CCA_PWR_TH_EN                               ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_CCA_PWR_TH_ON                               ((u32)0x00000001 << 16)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_CCA_PWR_TH                             ((u32)0x000000FF << 8)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_CCA_PWR_TH(x)                               ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_FORCE_OUT_CCA_PWR_TH(x)                           ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_TXPWR_CTRL_DBG_SEL                               ((u32)0x0000003F << 0)          /*!<R/W 0  (RTL_only) dbg port address */
#define BIT_R_TXPWR_CTRL_DBG_SEL(x)                                 ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_TXPWR_CTRL_DBG_SEL(x)                             ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_UL_FORCE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_OUT_CFO_COMP_ON                                 ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_CFO_COMP                               ((u32)0x00000007 << 28)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_CFO_COMP(x)                                 ((u32)(((x) & 0x00000007) << 28))
#define BIT_GET_R_FORCE_OUT_CFO_COMP(x)                             ((u32)(((x >> 28) & 0x00000007)))
#define BIT_R_FORCE_OUT_ABORT_TX_IDX_ON                             ((u32)0x00000001 << 27)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_ABORT_TX_IDX                           ((u32)0x00000003 << 25)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_ABORT_TX_IDX(x)                             ((u32)(((x) & 0x00000003) << 25))
#define BIT_GET_R_FORCE_OUT_ABORT_TX_IDX(x)                         ((u32)(((x >> 25) & 0x00000003)))
#define BIT_R_FORCE_OUT_MIN_TX_PWR_FLAG_ON                          ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_R_FORCE_OUT_MIN_TX_PWR_FLAG                             ((u32)0x00000001 << 23)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_UPH_ON                                      ((u32)0x00000001 << 22)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_UPH                                    ((u32)0x0000001F << 17)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_UPH(x)                                      ((u32)(((x) & 0x0000001F) << 17))
#define BIT_GET_R_FORCE_OUT_UPH(x)                                  ((u32)(((x >> 17) & 0x0000001F)))
#define BIT_R_FORCE_OUT_STA_TXPWR_MAC_ON                            ((u32)0x00000001 << 16)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_STA_TXPWR_MAC                          ((u32)0x0000003F << 10)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_STA_TXPWR_MAC(x)                            ((u32)(((x) & 0x0000003F) << 10))
#define BIT_GET_R_FORCE_OUT_STA_TXPWR_MAC(x)                        ((u32)(((x >> 10) & 0x0000003F)))
#define BIT_R_FORCE_OUT_STA_TXPWR_BB_ON                             ((u32)0x00000001 << 9)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_STA_TXPWR_BB                           ((u32)0x000001FF << 0)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_STA_TXPWR_BB(x)                             ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_OUT_STA_TXPWR_BB(x)                         ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_NORM_FORCE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_OUT_TXAGC_BBSWING_ON                            ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_TXAGC_BBSWING                          ((u32)0x0000000F << 27)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_TXAGC_BBSWING(x)                            ((u32)(((x) & 0x0000000F) << 27))
#define BIT_GET_R_FORCE_OUT_TXAGC_BBSWING(x)                        ((u32)(((x >> 27) & 0x0000000F)))
#define BIT_R_FORCE_OUT_TXBIAS_ON                                   ((u32)0x00000001 << 26)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_TXBIAS                                 ((u32)0x00000003 << 24)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_TXBIAS(x)                                   ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_R_FORCE_OUT_TXBIAS(x)                               ((u32)(((x >> 24) & 0x00000003)))
#define BIT_R_FORCE_OUT_TXPWR_BFON_BOOST_DB_SEG0_ON                 ((u32)0x00000001 << 23)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_TXPWR_BFON_BOOST_DB_SEG0               ((u32)0x0000001F << 18)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_TXPWR_BFON_BOOST_DB_SEG0(x)                 ((u32)(((x) & 0x0000001F) << 18))
#define BIT_GET_R_FORCE_OUT_TXPWR_BFON_BOOST_DB_SEG0(x)             ((u32)(((x >> 18) & 0x0000001F)))
#define BIT_R_FORCE_OUT_TXPWR_BB_ON                                 ((u32)0x00000001 << 17)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_TXPWR_BB                               ((u32)0x000001FF << 8)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_TXPWR_BB(x)                                 ((u32)(((x) & 0x000001FF) << 8))
#define BIT_GET_R_FORCE_OUT_TXPWR_BB(x)                             ((u32)(((x >> 8) & 0x000001FF)))
#define BIT_R_FORCE_OUT_TXPWR_MAC_ON                                ((u32)0x00000001 << 6)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_TXPWR_MAC                              ((u32)0x0000003F << 0)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_TXPWR_MAC(x)                                ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_FORCE_OUT_TXPWR_MAC(x)                            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_NORM_FORCE4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_BT_GRANT_WL_FORCE_OFF                                 ((u32)0x00000001 << 31)          /*!<R/W 0  (RTL_only) Force off grant_wl */
#define BIT_R_BT_GRANT_TX_FORCE_OFF                                 ((u32)0x00000001 << 30)          /*!<R/W 0  (RTL_only) Force off grant_tx */
#define BIT_R_BT_GRANT_RX_FORCE_OFF                                 ((u32)0x00000001 << 29)          /*!<R/W 0  (RTL_only) Force off grant_rx */
#define BIT_R_BT_GRANT_WL_VAL                                       ((u32)0x00000001 << 28)          /*!<R/W 0  (RTL_only) grant_wl flag=0 means coexist act */
#define BIT_R_BT_GRANT_TX_VAL                                       ((u32)0x00000001 << 27)          /*!<R/W 1h  (RTL_only) grant_rx flag=1 means coexist act */
#define BIT_R_BT_GRANT_RX_VAL                                       ((u32)0x00000001 << 26)          /*!<R/W 1h  (RTL_only) grant_rx flag=1 means coexist act */
#define BIT_R_FORCE_OUT_SR_DCM_ON                                   ((u32)0x00000001 << 25)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_R_FORCE_OUT_SR_DCM                                      ((u32)0x00000001 << 24)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_SR_MCS_ON                                   ((u32)0x00000001 << 23)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_SR_MCS                                 ((u32)0x00000FFF << 11)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_SR_MCS(x)                                   ((u32)(((x) & 0x00000FFF) << 11))
#define BIT_GET_R_FORCE_OUT_SR_MCS(x)                               ((u32)(((x >> 11) & 0x00000FFF)))
#define BIT_R_FORCE_OUT_SR_PD_THREHOLD_ON                           ((u32)0x00000001 << 10)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_MASK_R_FORCE_OUT_SR_PD_THREHOLD                         ((u32)0x0000003F << 4)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_SR_PD_THREHOLD(x)                           ((u32)(((x) & 0x0000003F) << 4))
#define BIT_GET_R_FORCE_OUT_SR_PD_THREHOLD(x)                       ((u32)(((x >> 4) & 0x0000003F)))
#define BIT_R_FORCE_OUT_CFIR_BY_RATE_OFF_ON                         ((u32)0x00000001 << 3)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_R_FORCE_OUT_CFIR_BY_RATE_OFF                            ((u32)0x00000001 << 2)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_FORCE_OUT_DPD_BY_RATE_OFF_ON                          ((u32)0x00000001 << 1)          /*!<R/W 0  (RTL_only) Force output to MAC en */
#define BIT_R_FORCE_OUT_DPD_BY_RATE_OFF                             ((u32)0x00000001 << 0)          /*!<R/W 0  (RTL_only) Force output to MAC */
#define BIT_R_RU_PWR_LIM_MUX_EN                                     ((u32)0x00000001 << 31)          /*!<R/W 0  1:by_rate→macid_lim→RU_lim 0:by_rate->macid->BF0_pwr_lim->RU_lim */
#define BIT_R_RU_NORM_EN                                            ((u32)0x00000001 << 30)          /*!<R/W 0  (RTL_only) RU_norm_enable (RTL_ONLY TEST ) */
#define BIT_MASK_R_RU_NORM_RU106                                    ((u32)0x000003FF << 20)          /*!<R/W 39h  (RTL_only) S(10.4) RU_lim tbl -RU norm,0x39=3.5625dB */
#define BIT_R_RU_NORM_RU106(x)                                      ((u32)(((x) & 0x000003FF) << 20))
#define BIT_GET_R_RU_NORM_RU106(x)                                  ((u32)(((x >> 20) & 0x000003FF)))
#define BIT_MASK_R_RU_NORM_RU52                                     ((u32)0x000003FF << 10)          /*!<R/W 6Bh  (RTL_only) S(10.4) RU_lim tbl -RU norm,0x6b=6.6875dB */
#define BIT_R_RU_NORM_RU52(x)                                       ((u32)(((x) & 0x000003FF) << 10))
#define BIT_GET_R_RU_NORM_RU52(x)                                   ((u32)(((x >> 10) & 0x000003FF)))
#define BIT_MASK_R_RU_NORM_RU26                                     ((u32)0x000003FF << 0)          /*!<R/W 9Bh  (RTL_only) S(10.4) RU_lim tbl -RU norm ,0x9b=9.6875dB */
#define BIT_R_RU_NORM_RU26(x)                                       ((u32)(((x) & 0x000003FF) << 0))
#define BIT_GET_R_RU_NORM_RU26(x)                                   ((u32)(((x >> 0) & 0x000003FF)))
#define BIT_R_STA_UL_LOG_NORM_EN                                    ((u32)0x00000001 << 31)          /*!<R/W 0  1:sta ul txpwr log norm en 0:log norm=10log(ntx)-10log(dbw) */
#define BIT_R_STA_UL_INI_UPD_PHASE_DIS                              ((u32)0x00000001 << 30)          /*!<R/W 0  1:tst non eco bug */
#define BIT_R_SRAM_RST_FORCE_VAL                                    ((u32)0x00000001 << 16)          /*!<R/W 0  rst sram by cr */
#define BIT_R_SRAM_RST_FORCE_ON                                     ((u32)0x00000001 << 15)          /*!<R/W 0  1:rst sram by cr */
#define BIT_R_MCS2TXINFO_EN                                         ((u32)0x00000001 << 14)          /*!<R/W 0  1:txinfo_bbswing=MCS */
#define BIT_MASK_R_FORCE_OUT_PWRCOM_CH20_WITH_DATA_VAL              ((u32)0x000000FF << 6)          /*!<R/W 0  1:force out off */
#define BIT_R_FORCE_OUT_PWRCOM_CH20_WITH_DATA_VAL(x)                ((u32)(((x) & 0x000000FF) << 6))
#define BIT_GET_R_FORCE_OUT_PWRCOM_CH20_WITH_DATA_VAL(x)            ((u32)(((x >> 6) & 0x000000FF)))
#define BIT_R_FORCE_OUT_PWRCOM_CH20_WITH_DATA_ON                    ((u32)0x00000001 << 5)          /*!<R/W 0  1:force out on */
#define BIT_R_STA_UL_MU_OFF_DIS                                     ((u32)0x00000001 << 4)          /*!<R/W 0  1:MU do sta_ul */
#define BIT_R_PWRCOM_CH20_WITH_DATA_BIT_REV                         ((u32)0x00000001 << 3)          /*!<R/W 0  bit rev,txpwr_lim */
#define BIT_R_PWRCOM_CH20_WITH_DATA_BIT_INV                         ((u32)0x00000001 << 2)          /*!<R/W 0  bit inv,txpwr_lim */
#define BIT_R_FUNC_RST_FORCE_VAL                                    ((u32)0x00000001 << 1)          /*!<R/W 1h  func rst from txpwr_ctrl _CR */
#define BIT_R_FUNC_RST_FORCE_ON                                     ((u32)0x00000001 << 0)          /*!<R/W 0  Don’t obey func rst from MAC */
#define BIT_MASK_R_STA_UL_LOG_NORM_BW160_1TX                        ((u32)0x000000FF << 24)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW160_1TX(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_STA_UL_LOG_NORM_BW160_1TX(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW80_1TX                         ((u32)0x000000FF << 16)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW80_1TX(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_STA_UL_LOG_NORM_BW80_1TX(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW40_1TX                         ((u32)0x000000FF << 8)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW40_1TX(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_STA_UL_LOG_NORM_BW40_1TX(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW20_1TX                         ((u32)0x000000FF << 0)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW20_1TX(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_STA_UL_LOG_NORM_BW20_1TX(x)                       ((u32)(((x >> 0) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW260_2TX                        ((u32)0x000000FF << 24)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW260_2TX(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_STA_UL_LOG_NORM_BW260_2TX(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW80_2TX                         ((u32)0x000000FF << 16)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW80_2TX(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_STA_UL_LOG_NORM_BW80_2TX(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW40_2TX                         ((u32)0x000000FF << 8)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW40_2TX(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_STA_UL_LOG_NORM_BW40_2TX(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_STA_UL_LOG_NORM_BW20_2TX                         ((u32)0x000000FF << 0)          /*!<R/W 0  s(8,2) sta ul txpwr log norm */
#define BIT_R_STA_UL_LOG_NORM_BW20_2TX(x)                           ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_STA_UL_LOG_NORM_BW20_2TX(x)                       ((u32)(((x >> 0) & 0x000000FF)))
#define BIT_MASK_R_TXBIAS_HT_TH_1                                   ((u32)0x0000000F << 27)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HT_TH_1(x)                                     ((u32)(((x) & 0x0000000F) << 27))
#define BIT_GET_R_TXBIAS_HT_TH_1(x)                                 ((u32)(((x >> 27) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HT_TH_0                                   ((u32)0x0000000F << 23)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HT_TH_0(x)                                     ((u32)(((x) & 0x0000000F) << 23))
#define BIT_GET_R_TXBIAS_HT_TH_0(x)                                 ((u32)(((x >> 23) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_LEGACY_TH_2                               ((u32)0x0000000F << 17)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_LEGACY_TH_2(x)                                 ((u32)(((x) & 0x0000000F) << 17))
#define BIT_GET_R_TXBIAS_LEGACY_TH_2(x)                             ((u32)(((x >> 17) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_LEGACY_TH_1                               ((u32)0x0000000F << 13)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_LEGACY_TH_1(x)                                 ((u32)(((x) & 0x0000000F) << 13))
#define BIT_GET_R_TXBIAS_LEGACY_TH_1(x)                             ((u32)(((x >> 13) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_LEGACY_TH_0                               ((u32)0x0000000F << 9)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_LEGACY_TH_0(x)                                 ((u32)(((x) & 0x0000000F) << 9))
#define BIT_GET_R_TXBIAS_LEGACY_TH_0(x)                             ((u32)(((x >> 9) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_CCK_TH_2                                  ((u32)0x00000007 << 6)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_CCK_TH_2(x)                                    ((u32)(((x) & 0x00000007) << 6))
#define BIT_GET_R_TXBIAS_CCK_TH_2(x)                                ((u32)(((x >> 6) & 0x00000007)))
#define BIT_MASK_R_TXBIAS_CCK_TH_1                                  ((u32)0x00000007 << 3)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_CCK_TH_1(x)                                    ((u32)(((x) & 0x00000007) << 3))
#define BIT_GET_R_TXBIAS_CCK_TH_1(x)                                ((u32)(((x >> 3) & 0x00000007)))
#define BIT_MASK_R_TXBIAS_CCK_TH_0                                  ((u32)0x00000007 << 0)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_CCK_TH_0(x)                                    ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_TXBIAS_CCK_TH_0(x)                                ((u32)(((x >> 0) & 0x00000007)))
#define BIT_MASK_R_TXBIAS_CCK_REGION_3                              ((u32)0x00000003 << 30)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_CCK_REGION_3(x)                                ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_TXBIAS_CCK_REGION_3(x)                            ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_CCK_REGION_2                              ((u32)0x00000003 << 28)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_CCK_REGION_2(x)                                ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_R_TXBIAS_CCK_REGION_2(x)                            ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HE_TH_2                                   ((u32)0x0000000F << 24)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HE_TH_2(x)                                     ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_R_TXBIAS_HE_TH_2(x)                                 ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HE_TH_1                                   ((u32)0x0000000F << 20)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HE_TH_1(x)                                     ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_R_TXBIAS_HE_TH_1(x)                                 ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HE_TH_0                                   ((u32)0x0000000F << 16)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HE_TH_0(x)                                     ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_TXBIAS_HE_TH_0(x)                                 ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_VHT_TH_2                                  ((u32)0x0000000F << 12)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_VHT_TH_2(x)                                    ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_TXBIAS_VHT_TH_2(x)                                ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_VHT_TH_1                                  ((u32)0x0000000F << 8)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_VHT_TH_1(x)                                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_TXBIAS_VHT_TH_1(x)                                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_VHT_TH_0                                  ((u32)0x0000000F << 4)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_VHT_TH_0(x)                                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_TXBIAS_VHT_TH_0(x)                                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HT_TH_2                                   ((u32)0x0000000F << 0)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HT_TH_2(x)                                     ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_TXBIAS_HT_TH_2(x)                                 ((u32)(((x >> 0) & 0x0000000F)))
#define BIT_MASK_R_TXBIAS_HE_REGION_3                               ((u32)0x00000003 << 14)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HE_REGION_3(x)                                 ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_TXBIAS_HE_REGION_3(x)                             ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HE_REGION_2                               ((u32)0x00000003 << 12)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HE_REGION_2(x)                                 ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_R_TXBIAS_HE_REGION_2(x)                             ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_VHT_REGION_3                              ((u32)0x00000003 << 10)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_VHT_REGION_3(x)                                ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_R_TXBIAS_VHT_REGION_3(x)                            ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_VHT_REGION_2                              ((u32)0x00000003 << 8)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_VHT_REGION_2(x)                                ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_TXBIAS_VHT_REGION_2(x)                            ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HT_REGION_3                               ((u32)0x00000003 << 6)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HT_REGION_3(x)                                 ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_TXBIAS_HT_REGION_3(x)                             ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_HT_REGION_2                               ((u32)0x00000003 << 4)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_HT_REGION_2(x)                                 ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_TXBIAS_HT_REGION_2(x)                             ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_LEGACY_REGION_3                           ((u32)0x00000003 << 2)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_LEGACY_REGION_3(x)                             ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_R_TXBIAS_LEGACY_REGION_3(x)                         ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_R_TXBIAS_LEGACY_REGION_2                           ((u32)0x00000003 << 0)          /*!<R/W 0  txbias region */
#define BIT_R_TXBIAS_LEGACY_REGION_2(x)                             ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_TXBIAS_LEGACY_REGION_2(x)                         ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_CCK11M                                     ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_CCK11M(x)                                       ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_CCK11M(x)                                   ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_CCK5P5M                                    ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_CCK5P5M(x)                                      ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_CCK5P5M(x)                                  ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_CCK2M                                      ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_CCK2M(x)                                        ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_CCK2M(x)                                    ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_CCK1M                                      ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_CCK1M(x)                                        ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_CCK1M(x)                                    ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_LEGACY18M                                  ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY18M(x)                                    ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_LEGACY18M(x)                                ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY12M                                  ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY12M(x)                                    ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_LEGACY12M(x)                                ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY9M                                   ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY9M(x)                                     ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_LEGACY9M(x)                                 ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY6M                                   ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY6M(x)                                     ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_LEGACY6M(x)                                 ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_LEGACY54M                                  ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY54M(x)                                    ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_LEGACY54M(x)                                ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY48M                                  ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY48M(x)                                    ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_LEGACY48M(x)                                ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY36M                                  ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY36M(x)                                    ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_LEGACY36M(x)                                ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_LEGACY24M                                  ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_LEGACY24M(x)                                    ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_LEGACY24M(x)                                ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS3_1NSS                        ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS3_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS3_1NSS(x)                      ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS2_1NSS                        ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS2_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS2_1NSS(x)                      ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS1_1NSS                        ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS1_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS1_1NSS(x)                      ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS0_1NSS                        ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS0_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS0_1NSS(x)                      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS7_1NSS                        ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS7_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS7_1NSS(x)                      ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS6_1NSS                        ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS6_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS6_1NSS(x)                      ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS5_1NSS                        ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS5_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS5_1NSS(x)                      ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS4_1NSS                        ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS4_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS4_1NSS(x)                      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS11_1NSS                       ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS11_1NSS(x)                         ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS11_1NSS(x)                     ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS10_1NSS                       ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS10_1NSS(x)                         ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS10_1NSS(x)                     ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS9_1NSS                        ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS9_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS9_1NSS(x)                      ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_MCS8_1NSS                        ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_MCS8_1NSS(x)                          ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_NONLEGACY_MCS8_1NSS(x)                      ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_BY_RATE_TABLE6
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_NONLEGACY_DCM_MCS4_1NSS                    ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_DCM_MCS4_1NSS(x)                      ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_TXAGC_NONLEGACY_DCM_MCS4_1NSS(x)                  ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_DCM_MCS3_1NSS                    ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_DCM_MCS3_1NSS(x)                      ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_NONLEGACY_DCM_MCS3_1NSS(x)                  ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_DCM_MCS1_1NSS                    ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_DCM_MCS1_1NSS(x)                      ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_TXAGC_NONLEGACY_DCM_MCS1_1NSS(x)                  ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_NONLEGACY_DCM_MCS0_1NSS                    ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_NONLEGACY_DCM_MCS0_1NSS(x)                      ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_NONLEGACY_DCM_MCS0_1NSS(x)                  ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_LMT_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_MAX_CCK_1TX_BW20M                          ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_MAX_CCK_1TX_BW20M(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_MAX_CCK_1TX_BW20M(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_LMT_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_TXAGC_MAX_1TX_BW20_0                             ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_MAX_1TX_BW20_0(x)                               ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_TXAGC_MAX_1TX_BW20_0(x)                           ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_TXAGC_MAX_LEGACY_NON_DUP_1TX                     ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_TXAGC_MAX_LEGACY_NON_DUP_1TX(x)                       ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_TXAGC_MAX_LEGACY_NON_DUP_1TX(x)                   ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_3                          ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_3(x)                            ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_3(x)                        ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_2                          ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_2(x)                            ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_2(x)                        ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_1                          ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_1(x)                            ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_1(x)                        ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_0                          ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_0(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_0(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_7                          ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_7(x)                            ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_7(x)                        ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_6                          ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_6(x)                            ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_6(x)                        ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_5                          ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_5(x)                            ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_5(x)                        ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU26_BW160_4                          ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU26_BW160_4(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU26_BW160_4(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_3                          ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_3(x)                            ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_3(x)                        ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_2                          ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_2(x)                            ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_2(x)                        ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_1                          ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_1(x)                            ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_1(x)                        ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_0                          ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_0(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_0(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_7                          ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_7(x)                            ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_7(x)                        ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_6                          ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_6(x)                            ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_6(x)                        ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_5                          ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_5(x)                            ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_5(x)                        ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU52_BW160_4                          ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU52_BW160_4(x)                            ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU52_BW160_4(x)                        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_3                         ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_3(x)                           ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_3(x)                       ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_2                         ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_2(x)                           ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_2(x)                       ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_1                         ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_1(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_1(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_0                         ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_0(x)                           ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_0(x)                       ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_RU_LMT_TABLE5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_7                         ((u32)0x0000007F << 24)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_7(x)                           ((u32)(((x) & 0x0000007F) << 24))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_7(x)                       ((u32)(((x >> 24) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_6                         ((u32)0x0000007F << 16)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_6(x)                           ((u32)(((x) & 0x0000007F) << 16))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_6(x)                       ((u32)(((x >> 16) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_5                         ((u32)0x0000007F << 8)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_5(x)                           ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_5(x)                       ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_RU_LIM_0TX_RU106_BW160_4                         ((u32)0x0000007F << 0)          /*!<R/W 0  s(7.1) */
#define BIT_R_RU_LIM_0TX_RU106_BW160_4(x)                           ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_RU_LIM_0TX_RU106_BW160_4(x)                       ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_MACID_LMT_TABLEx
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_MACID_x_CCA_PWR_TH_EN                                 ((u32)0x00000001 << 26)          /*!<R/W 0  u(1.0) */
#define BIT_R_MACID_x_TXPWR1_EN                                     ((u32)0x00000001 << 25)          /*!<R/W 0  u(1.0) */
#define BIT_R_MACID_x_TXPWR0_EN                                     ((u32)0x00000001 << 24)          /*!<R/W 0  u(1.0) */
#define BIT_MASK_R_MACID_x_CCA_PWR_TH                               ((u32)0x000000FF << 16)          /*!<R/W 0  s(8.1) */
#define BIT_R_MACID_x_CCA_PWR_TH(x)                                 ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_MACID_x_CCA_PWR_TH(x)                             ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_MACID_x_TXPWR1                                   ((u32)0x000000FF << 8)          /*!<R/W 0  s(8.1) */
#define BIT_R_MACID_x_TXPWR1(x)                                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_MACID_x_TXPWR1(x)                                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_MACID_x_TXPWR0                                   ((u32)0x000000FF << 0)          /*!<R/W 0  s(8.1) */
#define BIT_R_MACID_x_TXPWR0(x)                                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_MACID_x_TXPWR0(x)                                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_MCSx_TXDIFF_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MCSx_TXDIFF_5DB_MCS_OFFSET                       ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_5DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_MCSx_TXDIFF_5DB_MCS_OFFSET(x)                     ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_4DB_MCS_OFFSET                       ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_4DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_MCSx_TXDIFF_4DB_MCS_OFFSET(x)                     ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_3DB_MCS_OFFSET                       ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_3DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_MCSx_TXDIFF_3DB_MCS_OFFSET(x)                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_2DB_MCS_OFFSET                       ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_2DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_MCSx_TXDIFF_2DB_MCS_OFFSET(x)                     ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_1DB_MCS_OFFSET                       ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_1DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_MCSx_TXDIFF_1DB_MCS_OFFSET(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_MCSx_TXDIFF_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MCSx_TXDIFF_10DB_MCS_OFFSET                      ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_10DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_MCSx_TXDIFF_10DB_MCS_OFFSET(x)                    ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_9DB_MCS_OFFSET                       ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_9DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_MCSx_TXDIFF_9DB_MCS_OFFSET(x)                     ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_8DB_MCS_OFFSET                       ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_8DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_MCSx_TXDIFF_8DB_MCS_OFFSET(x)                     ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_7DB_MCS_OFFSET                       ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_7DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_MCSx_TXDIFF_7DB_MCS_OFFSET(x)                     ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_6DB_MCS_OFFSET                       ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_6DB_MCS_OFFSET(x)                         ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_MCSx_TXDIFF_6DB_MCS_OFFSET(x)                     ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_MCSx_TXDIFF_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MCSx_TXDIFF_15DB_MCS_OFFSET                      ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_15DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_MCSx_TXDIFF_15DB_MCS_OFFSET(x)                    ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_14DB_MCS_OFFSET                      ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_14DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_MCSx_TXDIFF_14DB_MCS_OFFSET(x)                    ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_13DB_MCS_OFFSET                      ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_13DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_MCSx_TXDIFF_13DB_MCS_OFFSET(x)                    ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_12DB_MCS_OFFSET                      ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_12DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_MCSx_TXDIFF_12DB_MCS_OFFSET(x)                    ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_11DB_MCS_OFFSET                      ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_11DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_MCSx_TXDIFF_11DB_MCS_OFFSET(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_SR_MCSx_TXDIFF_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_MCSx_TXDIFF_20DB_MCS_OFFSET                      ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_20DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_MCSx_TXDIFF_20DB_MCS_OFFSET(x)                    ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_19DB_MCS_OFFSET                      ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_19DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_MCSx_TXDIFF_19DB_MCS_OFFSET(x)                    ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_18DB_MCS_OFFSET                      ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_18DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_MCSx_TXDIFF_18DB_MCS_OFFSET(x)                    ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_17DB_MCS_OFFSET                      ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_17DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_MCSx_TXDIFF_17DB_MCS_OFFSET(x)                    ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_MCSx_TXDIFF_16DB_MCS_OFFSET                      ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) mcs0~11 */
#define BIT_R_MCSx_TXDIFF_16DB_MCS_OFFSET(x)                        ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_MCSx_TXDIFF_16DB_MCS_OFFSET(x)                    ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_cckxM_TXDIFF_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CCKxM_TXDIFF_5DB_RATE_OFFSET                     ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_5DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_CCKxM_TXDIFF_5DB_RATE_OFFSET(x)                   ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_4DB_RATE_OFFSET                     ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_4DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CCKxM_TXDIFF_4DB_RATE_OFFSET(x)                   ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_3DB_RATE_OFFSET                     ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_3DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_CCKxM_TXDIFF_3DB_RATE_OFFSET(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_2DB_RATE_OFFSET                     ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_2DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_CCKxM_TXDIFF_2DB_RATE_OFFSET(x)                   ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_1DB_RATE_OFFSET                     ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_1DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_CCKxM_TXDIFF_1DB_RATE_OFFSET(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_cckxM_TXDIFF_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CCKxM_TXDIFF_10DB_RATE_OFFSET                    ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_10DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_CCKxM_TXDIFF_10DB_RATE_OFFSET(x)                  ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_9DB_RATE_OFFSET                     ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_9DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CCKxM_TXDIFF_9DB_RATE_OFFSET(x)                   ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_8DB_RATE_OFFSET                     ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_8DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_CCKxM_TXDIFF_8DB_RATE_OFFSET(x)                   ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_7DB_RATE_OFFSET                     ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_7DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_CCKxM_TXDIFF_7DB_RATE_OFFSET(x)                   ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_6DB_RATE_OFFSET                     ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_6DB_RATE_OFFSET(x)                       ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_CCKxM_TXDIFF_6DB_RATE_OFFSET(x)                   ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_cckxM_TXDIFF_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CCKxM_TXDIFF_15DB_RATE_OFFSET                    ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_15DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_CCKxM_TXDIFF_15DB_RATE_OFFSET(x)                  ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_14DB_RATE_OFFSET                    ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_14DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CCKxM_TXDIFF_14DB_RATE_OFFSET(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_13DB_RATE_OFFSET                    ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_13DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_CCKxM_TXDIFF_13DB_RATE_OFFSET(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_12DB_RATE_OFFSET                    ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_12DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_CCKxM_TXDIFF_12DB_RATE_OFFSET(x)                  ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_11DB_RATE_OFFSET                    ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_11DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_CCKxM_TXDIFF_11DB_RATE_OFFSET(x)                  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_cckxM_TXDIFF_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CCKxM_TXDIFF_20DB_RATE_OFFSET                    ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_20DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_CCKxM_TXDIFF_20DB_RATE_OFFSET(x)                  ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_19DB_RATE_OFFSET                    ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_19DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_CCKxM_TXDIFF_19DB_RATE_OFFSET(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_18DB_RATE_OFFSET                    ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_18DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_CCKxM_TXDIFF_18DB_RATE_OFFSET(x)                  ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_17DB_RATE_OFFSET                    ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_17DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_CCKxM_TXDIFF_17DB_RATE_OFFSET(x)                  ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_CCKxM_TXDIFF_16DB_RATE_OFFSET                    ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) cck 0~3, 0-cck1M,1=cck2M,2=cck5p5M,3=cck11M */
#define BIT_R_CCKxM_TXDIFF_16DB_RATE_OFFSET(x)                      ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_CCKxM_TXDIFF_16DB_RATE_OFFSET(x)                  ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_legacyxM_TXDIFF_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_LEGACYxM_TXDIFF_5DB_RATE_OFFSET                  ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_5DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_LEGACYxM_TXDIFF_5DB_RATE_OFFSET(x)                ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_4DB_RATE_OFFSET                  ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_4DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_LEGACYxM_TXDIFF_4DB_RATE_OFFSET(x)                ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_3DB_RATE_OFFSET                  ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_3DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_LEGACYxM_TXDIFF_3DB_RATE_OFFSET(x)                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_2DB_RATE_OFFSET                  ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_2DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_LEGACYxM_TXDIFF_2DB_RATE_OFFSET(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_1DB_RATE_OFFSET                  ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_1DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_LEGACYxM_TXDIFF_1DB_RATE_OFFSET(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_legacyxM_TXDIFF_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_LEGACYxM_TXDIFF_10DB_RATE_OFFSET                 ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_10DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_LEGACYxM_TXDIFF_10DB_RATE_OFFSET(x)               ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_9DB_RATE_OFFSET                  ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_9DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_LEGACYxM_TXDIFF_9DB_RATE_OFFSET(x)                ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_8DB_RATE_OFFSET                  ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_8DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_LEGACYxM_TXDIFF_8DB_RATE_OFFSET(x)                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_7DB_RATE_OFFSET                  ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_7DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_LEGACYxM_TXDIFF_7DB_RATE_OFFSET(x)                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_6DB_RATE_OFFSET                  ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_6DB_RATE_OFFSET(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_LEGACYxM_TXDIFF_6DB_RATE_OFFSET(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_legacyxM_TXDIFF_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_LEGACYxM_TXDIFF_15DB_RATE_OFFSET                 ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_15DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_LEGACYxM_TXDIFF_15DB_RATE_OFFSET(x)               ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_14DB_RATE_OFFSET                 ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_14DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_LEGACYxM_TXDIFF_14DB_RATE_OFFSET(x)               ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_13DB_RATE_OFFSET                 ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_13DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_LEGACYxM_TXDIFF_13DB_RATE_OFFSET(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_12DB_RATE_OFFSET                 ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_12DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_LEGACYxM_TXDIFF_12DB_RATE_OFFSET(x)               ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_11DB_RATE_OFFSET                 ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_11DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_LEGACYxM_TXDIFF_11DB_RATE_OFFSET(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_legacyxM_TXDIFF_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_LEGACYxM_TXDIFF_20DB_RATE_OFFSET                 ((u32)0x0000000F << 16)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_20DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_LEGACYxM_TXDIFF_20DB_RATE_OFFSET(x)               ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_19DB_RATE_OFFSET                 ((u32)0x0000000F << 12)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_19DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_LEGACYxM_TXDIFF_19DB_RATE_OFFSET(x)               ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_18DB_RATE_OFFSET                 ((u32)0x0000000F << 8)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_18DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_LEGACYxM_TXDIFF_18DB_RATE_OFFSET(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_17DB_RATE_OFFSET                 ((u32)0x0000000F << 4)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_17DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_LEGACYxM_TXDIFF_17DB_RATE_OFFSET(x)               ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_LEGACYxM_TXDIFF_16DB_RATE_OFFSET                 ((u32)0x0000000F << 0)          /*!<R/W 0  U(4.0) legacy 4~11,4=legacy6M,5=legacy9M,6=legacy12M,7=legacy18M,8=legacy24M,9=legacy36M,10=legacy48M,11=legacy54M */
#define BIT_R_LEGACYxM_TXDIFF_16DB_RATE_OFFSET(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_LEGACYxM_TXDIFF_16DB_RATE_OFFSET(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_BF_1STS_ANT_SEL_A                                     ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_MASK_R_GRP_2TX_PATH_EN                                  ((u32)0x0000000F << 27)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 27))
#define BIT_GET_R_GRP_2TX_PATH_EN(x)                                ((u32)(((x >> 27) & 0x0000000F)))
#define BIT_MASK_R_GRP_1TX_PATH_EN                                  ((u32)0x0000000F << 23)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 23))
#define BIT_GET_R_GRP_1TX_PATH_EN(x)                                ((u32)(((x >> 23) & 0x0000000F)))
#define BIT_MASK_R_FORCE_MACID_ANT_PATH_EN_VAL                      ((u32)0x0000000F << 19)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_PATH_EN_VAL(x)                        ((u32)(((x) & 0x0000000F) << 19))
#define BIT_GET_R_FORCE_MACID_ANT_PATH_EN_VAL(x)                    ((u32)(((x >> 19) & 0x0000000F)))
#define BIT_MASK_R_FC_PATH_EN                                       ((u32)0x0000000F << 15)          /*!<R/W 0   */
#define BIT_R_FC_PATH_EN(x)                                         ((u32)(((x) & 0x0000000F) << 15))
#define BIT_GET_R_FC_PATH_EN(x)                                     ((u32)(((x >> 15) & 0x0000000F)))
#define BIT_MASK_R_BF_2STS_PATH_EN                                  ((u32)0x0000000F << 11)          /*!<R/W 0   */
#define BIT_R_BF_2STS_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 11))
#define BIT_GET_R_BF_2STS_PATH_EN(x)                                ((u32)(((x >> 11) & 0x0000000F)))
#define BIT_MASK_R_BF_1STS_PATH_EN                                  ((u32)0x0000000F << 7)          /*!<R/W 0   */
#define BIT_R_BF_1STS_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 7))
#define BIT_GET_R_BF_1STS_PATH_EN(x)                                ((u32)(((x >> 7) & 0x0000000F)))
#define BIT_MASK_R_USR_LIST_TBL_RA                                  ((u32)0x0000007F << 0)          /*!<R/W 0   */
#define BIT_R_USR_LIST_TBL_RA(x)                                    ((u32)(((x) & 0x0000007F) << 0))
#define BIT_GET_R_USR_LIST_TBL_RA(x)                                ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_NORM_1STS_PATH_EN                                ((u32)0x0000000F << 28)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_PATH_EN(x)                                  ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_R_NORM_1STS_PATH_EN(x)                              ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_R_HANG_PROOF_PATH_EN                               ((u32)0x0000000F << 24)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_PATH_EN(x)                                 ((u32)(((x) & 0x0000000F) << 24))
#define BIT_GET_R_HANG_PROOF_PATH_EN(x)                             ((u32)(((x >> 24) & 0x0000000F)))
#define BIT_MASK_R_GRP_8TX_PATH_EN                                  ((u32)0x0000000F << 20)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 20))
#define BIT_GET_R_GRP_8TX_PATH_EN(x)                                ((u32)(((x >> 20) & 0x0000000F)))
#define BIT_MASK_R_GRP_7TX_PATH_EN                                  ((u32)0x0000000F << 16)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 16))
#define BIT_GET_R_GRP_7TX_PATH_EN(x)                                ((u32)(((x >> 16) & 0x0000000F)))
#define BIT_MASK_R_GRP_6TX_PATH_EN                                  ((u32)0x0000000F << 12)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 12))
#define BIT_GET_R_GRP_6TX_PATH_EN(x)                                ((u32)(((x >> 12) & 0x0000000F)))
#define BIT_MASK_R_GRP_5TX_PATH_EN                                  ((u32)0x0000000F << 8)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_GRP_5TX_PATH_EN(x)                                ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_GRP_4TX_PATH_EN                                  ((u32)0x0000000F << 4)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_GRP_4TX_PATH_EN(x)                                ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_GRP_3TX_PATH_EN                                  ((u32)0x0000000F << 0)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_PATH_EN(x)                                    ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_GRP_3TX_PATH_EN(x)                                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BF_1STS_PATH_MAP_A                               ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_BF_1STS_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_BF_1STS_PATH_MAP_A(x)                             ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_FORCE_PAR_MODE_VAL                               ((u32)0x00000007 << 27)          /*!<R/W 0   */
#define BIT_R_FORCE_PAR_MODE_VAL(x)                                 ((u32)(((x) & 0x00000007) << 27))
#define BIT_GET_R_FORCE_PAR_MODE_VAL(x)                             ((u32)(((x >> 27) & 0x00000007)))
#define BIT_MASK_R_FORCE_MACID_ANT_SPATIAL_EXPAN_NUM_VAL            ((u32)0x00000007 << 24)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SPATIAL_EXPAN_NUM_VAL(x)              ((u32)(((x) & 0x00000007) << 24))
#define BIT_GET_R_FORCE_MACID_ANT_SPATIAL_EXPAN_NUM_VAL(x)          ((u32)(((x >> 24) & 0x00000007)))
#define BIT_MASK_R_FC_SPATIAL_EXPAN_NUM                             ((u32)0x00000007 << 21)          /*!<R/W 0   */
#define BIT_R_FC_SPATIAL_EXPAN_NUM(x)                               ((u32)(((x) & 0x00000007) << 21))
#define BIT_GET_R_FC_SPATIAL_EXPAN_NUM(x)                           ((u32)(((x >> 21) & 0x00000007)))
#define BIT_MASK_R_FC_NTX                                           ((u32)0x00000007 << 18)          /*!<R/W 0   */
#define BIT_R_FC_NTX(x)                                             ((u32)(((x) & 0x00000007) << 18))
#define BIT_GET_R_FC_NTX(x)                                         ((u32)(((x >> 18) & 0x00000007)))
#define BIT_MASK_R_BF_2STS_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 15)          /*!<R/W 0   */
#define BIT_R_BF_2STS_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 15))
#define BIT_GET_R_BF_2STS_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 15) & 0x00000007)))
#define BIT_MASK_R_BF_1STS_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 12)          /*!<R/W 0   */
#define BIT_R_BF_1STS_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_R_BF_1STS_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 12) & 0x00000007)))
#define BIT_MASK_R_RESP_2STS_PATH_EN                                ((u32)0x0000000F << 8)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_PATH_EN(x)                                  ((u32)(((x) & 0x0000000F) << 8))
#define BIT_GET_R_RESP_2STS_PATH_EN(x)                              ((u32)(((x >> 8) & 0x0000000F)))
#define BIT_MASK_R_RESP_1STS_PATH_EN                                ((u32)0x0000000F << 4)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_PATH_EN(x)                                  ((u32)(((x) & 0x0000000F) << 4))
#define BIT_GET_R_RESP_1STS_PATH_EN(x)                              ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_MASK_R_NORM_2STS_PATH_EN                                ((u32)0x0000000F << 0)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_PATH_EN(x)                                  ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_NORM_2STS_PATH_EN(x)                              ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BF_1STS_PATH_MAP_B                               ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_BF_1STS_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_BF_1STS_PATH_MAP_B(x)                             ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_NORM_1STS_SPATIAL_EXPAN_NUM                      ((u32)0x00000007 << 27)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_SPATIAL_EXPAN_NUM(x)                        ((u32)(((x) & 0x00000007) << 27))
#define BIT_GET_R_NORM_1STS_SPATIAL_EXPAN_NUM(x)                    ((u32)(((x >> 27) & 0x00000007)))
#define BIT_MASK_R_HANG_PROOF_SPATIAL_EXPAN_NUM                     ((u32)0x00000007 << 24)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_SPATIAL_EXPAN_NUM(x)                       ((u32)(((x) & 0x00000007) << 24))
#define BIT_GET_R_HANG_PROOF_SPATIAL_EXPAN_NUM(x)                   ((u32)(((x >> 24) & 0x00000007)))
#define BIT_MASK_R_GRP_8TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 21)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 21))
#define BIT_GET_R_GRP_8TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 21) & 0x00000007)))
#define BIT_MASK_R_GRP_7TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 18)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 18))
#define BIT_GET_R_GRP_7TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 18) & 0x00000007)))
#define BIT_MASK_R_GRP_6TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 15)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 15))
#define BIT_GET_R_GRP_6TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 15) & 0x00000007)))
#define BIT_MASK_R_GRP_5TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 12)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 12))
#define BIT_GET_R_GRP_5TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 12) & 0x00000007)))
#define BIT_MASK_R_GRP_4TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 9)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 9))
#define BIT_GET_R_GRP_4TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 9) & 0x00000007)))
#define BIT_MASK_R_GRP_3TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 6)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 6))
#define BIT_GET_R_GRP_3TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 6) & 0x00000007)))
#define BIT_MASK_R_GRP_2TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 3)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 3))
#define BIT_GET_R_GRP_2TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 3) & 0x00000007)))
#define BIT_MASK_R_GRP_1TX_SPATIAL_EXPAN_NUM                        ((u32)0x00000007 << 0)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_SPATIAL_EXPAN_NUM(x)                          ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_GRP_1TX_SPATIAL_EXPAN_NUM(x)                      ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_BF_1STS_ANT_SEL_B                                     ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_MASK_R_FORCE_MACID_ANT_MAP_A_VAL                        ((u32)0x00000003 << 29)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_A_VAL(x)                          ((u32)(((x) & 0x00000003) << 29))
#define BIT_GET_R_FORCE_MACID_ANT_MAP_A_VAL(x)                      ((u32)(((x >> 29) & 0x00000003)))
#define BIT_MASK_R_FC_PATH_MAP_D                                    ((u32)0x00000003 << 27)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_D(x)                                      ((u32)(((x) & 0x00000003) << 27))
#define BIT_GET_R_FC_PATH_MAP_D(x)                                  ((u32)(((x >> 27) & 0x00000003)))
#define BIT_MASK_R_FC_PATH_MAP_C                                    ((u32)0x00000003 << 25)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_C(x)                                      ((u32)(((x) & 0x00000003) << 25))
#define BIT_GET_R_FC_PATH_MAP_C(x)                                  ((u32)(((x >> 25) & 0x00000003)))
#define BIT_MASK_R_FC_PATH_MAP_B                                    ((u32)0x00000003 << 23)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_B(x)                                      ((u32)(((x) & 0x00000003) << 23))
#define BIT_GET_R_FC_PATH_MAP_B(x)                                  ((u32)(((x >> 23) & 0x00000003)))
#define BIT_MASK_R_FC_PATH_MAP_A                                    ((u32)0x00000003 << 21)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_A(x)                                      ((u32)(((x) & 0x00000003) << 21))
#define BIT_GET_R_FC_PATH_MAP_A(x)                                  ((u32)(((x >> 21) & 0x00000003)))
#define BIT_MASK_R_BF_2STS_PATH_MAP_D                               ((u32)0x00000003 << 19)          /*!<R/W 0   */
#define BIT_R_BF_2STS_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 19))
#define BIT_GET_R_BF_2STS_PATH_MAP_D(x)                             ((u32)(((x >> 19) & 0x00000003)))
#define BIT_MASK_R_BF_2STS_PATH_MAP_C                               ((u32)0x00000003 << 17)          /*!<R/W 0   */
#define BIT_R_BF_2STS_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 17))
#define BIT_GET_R_BF_2STS_PATH_MAP_C(x)                             ((u32)(((x >> 17) & 0x00000003)))
#define BIT_MASK_R_BF_2STS_PATH_MAP_B                               ((u32)0x00000003 << 15)          /*!<R/W 0   */
#define BIT_R_BF_2STS_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 15))
#define BIT_GET_R_BF_2STS_PATH_MAP_B(x)                             ((u32)(((x >> 15) & 0x00000003)))
#define BIT_MASK_R_BF_2STS_PATH_MAP_A                               ((u32)0x00000003 << 13)          /*!<R/W 0   */
#define BIT_R_BF_2STS_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 13))
#define BIT_GET_R_BF_2STS_PATH_MAP_A(x)                             ((u32)(((x >> 13) & 0x00000003)))
#define BIT_MASK_R_BF_1STS_PATH_MAP_D                               ((u32)0x00000003 << 11)          /*!<R/W 0   */
#define BIT_R_BF_1STS_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 11))
#define BIT_GET_R_BF_1STS_PATH_MAP_D(x)                             ((u32)(((x >> 11) & 0x00000003)))
#define BIT_MASK_R_BF_1STS_PATH_MAP_C                               ((u32)0x00000003 << 9)          /*!<R/W 0   */
#define BIT_R_BF_1STS_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 9))
#define BIT_GET_R_BF_1STS_PATH_MAP_C(x)                             ((u32)(((x >> 9) & 0x00000003)))
#define BIT_MASK_R_RESP_2STS_SPATIAL_EXPAN_NUM                      ((u32)0x00000007 << 6)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_SPATIAL_EXPAN_NUM(x)                        ((u32)(((x) & 0x00000007) << 6))
#define BIT_GET_R_RESP_2STS_SPATIAL_EXPAN_NUM(x)                    ((u32)(((x >> 6) & 0x00000007)))
#define BIT_MASK_R_RESP_1STS_SPATIAL_EXPAN_NUM                      ((u32)0x00000007 << 3)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_SPATIAL_EXPAN_NUM(x)                        ((u32)(((x) & 0x00000007) << 3))
#define BIT_GET_R_RESP_1STS_SPATIAL_EXPAN_NUM(x)                    ((u32)(((x >> 3) & 0x00000007)))
#define BIT_MASK_R_NORM_2STS_SPATIAL_EXPAN_NUM                      ((u32)0x00000007 << 0)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_SPATIAL_EXPAN_NUM(x)                        ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_NORM_2STS_SPATIAL_EXPAN_NUM(x)                    ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GRP_4TX_PATH_MAP_A                               ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_GRP_4TX_PATH_MAP_A(x)                             ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_GRP_3TX_PATH_MAP_D                               ((u32)0x00000003 << 28)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_R_GRP_3TX_PATH_MAP_D(x)                             ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_R_GRP_3TX_PATH_MAP_C                               ((u32)0x00000003 << 26)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_R_GRP_3TX_PATH_MAP_C(x)                             ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_R_GRP_3TX_PATH_MAP_B                               ((u32)0x00000003 << 24)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_R_GRP_3TX_PATH_MAP_B(x)                             ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_R_GRP_3TX_PATH_MAP_A                               ((u32)0x00000003 << 22)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_R_GRP_3TX_PATH_MAP_A(x)                             ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_R_GRP_2TX_PATH_MAP_D                               ((u32)0x00000003 << 20)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_R_GRP_2TX_PATH_MAP_D(x)                             ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_R_GRP_2TX_PATH_MAP_C                               ((u32)0x00000003 << 18)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_R_GRP_2TX_PATH_MAP_C(x)                             ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_R_GRP_2TX_PATH_MAP_B                               ((u32)0x00000003 << 16)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_GRP_2TX_PATH_MAP_B(x)                             ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_R_GRP_2TX_PATH_MAP_A                               ((u32)0x00000003 << 14)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_GRP_2TX_PATH_MAP_A(x)                             ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_GRP_1TX_PATH_MAP_D                               ((u32)0x00000003 << 12)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_R_GRP_1TX_PATH_MAP_D(x)                             ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_R_GRP_1TX_PATH_MAP_C                               ((u32)0x00000003 << 10)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_R_GRP_1TX_PATH_MAP_C(x)                             ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_R_GRP_1TX_PATH_MAP_B                               ((u32)0x00000003 << 8)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_GRP_1TX_PATH_MAP_B(x)                             ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_GRP_1TX_PATH_MAP_A                               ((u32)0x00000003 << 6)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_GRP_1TX_PATH_MAP_A(x)                             ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_FORCE_MACID_ANT_MAP_D_VAL                        ((u32)0x00000003 << 4)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_D_VAL(x)                          ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_FORCE_MACID_ANT_MAP_D_VAL(x)                      ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_FORCE_MACID_ANT_MAP_C_VAL                        ((u32)0x00000003 << 2)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_C_VAL(x)                          ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_R_FORCE_MACID_ANT_MAP_C_VAL(x)                      ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_R_FORCE_MACID_ANT_MAP_B_VAL                        ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_B_VAL(x)                          ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_FORCE_MACID_ANT_MAP_B_VAL(x)                      ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR6
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_GRP_8TX_PATH_MAP_A                               ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_GRP_8TX_PATH_MAP_A(x)                             ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_GRP_7TX_PATH_MAP_D                               ((u32)0x00000003 << 28)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_R_GRP_7TX_PATH_MAP_D(x)                             ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_R_GRP_7TX_PATH_MAP_C                               ((u32)0x00000003 << 26)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_R_GRP_7TX_PATH_MAP_C(x)                             ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_R_GRP_7TX_PATH_MAP_B                               ((u32)0x00000003 << 24)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_R_GRP_7TX_PATH_MAP_B(x)                             ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_R_GRP_7TX_PATH_MAP_A                               ((u32)0x00000003 << 22)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_R_GRP_7TX_PATH_MAP_A(x)                             ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_R_GRP_6TX_PATH_MAP_D                               ((u32)0x00000003 << 20)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_R_GRP_6TX_PATH_MAP_D(x)                             ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_R_GRP_6TX_PATH_MAP_C                               ((u32)0x00000003 << 18)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_R_GRP_6TX_PATH_MAP_C(x)                             ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_R_GRP_6TX_PATH_MAP_B                               ((u32)0x00000003 << 16)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_GRP_6TX_PATH_MAP_B(x)                             ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_R_GRP_6TX_PATH_MAP_A                               ((u32)0x00000003 << 14)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_GRP_6TX_PATH_MAP_A(x)                             ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_GRP_5TX_PATH_MAP_D                               ((u32)0x00000003 << 12)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_R_GRP_5TX_PATH_MAP_D(x)                             ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_R_GRP_5TX_PATH_MAP_C                               ((u32)0x00000003 << 10)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_R_GRP_5TX_PATH_MAP_C(x)                             ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_R_GRP_5TX_PATH_MAP_B                               ((u32)0x00000003 << 8)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_GRP_5TX_PATH_MAP_B(x)                             ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_GRP_5TX_PATH_MAP_A                               ((u32)0x00000003 << 6)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_PATH_MAP_A(x)                                 ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_GRP_5TX_PATH_MAP_A(x)                             ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_GRP_4TX_PATH_MAP_D                               ((u32)0x00000003 << 4)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_GRP_4TX_PATH_MAP_D(x)                             ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_GRP_4TX_PATH_MAP_C                               ((u32)0x00000003 << 2)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_R_GRP_4TX_PATH_MAP_C(x)                             ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_R_GRP_4TX_PATH_MAP_B                               ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_GRP_4TX_PATH_MAP_B(x)                             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR7
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_RESP_1STS_PATH_MAP_A                             ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_PATH_MAP_A(x)                               ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_RESP_1STS_PATH_MAP_A(x)                           ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_NORM_2STS_PATH_MAP_D                             ((u32)0x00000003 << 28)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_PATH_MAP_D(x)                               ((u32)(((x) & 0x00000003) << 28))
#define BIT_GET_R_NORM_2STS_PATH_MAP_D(x)                           ((u32)(((x >> 28) & 0x00000003)))
#define BIT_MASK_R_NORM_2STS_PATH_MAP_C                             ((u32)0x00000003 << 26)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_PATH_MAP_C(x)                               ((u32)(((x) & 0x00000003) << 26))
#define BIT_GET_R_NORM_2STS_PATH_MAP_C(x)                           ((u32)(((x >> 26) & 0x00000003)))
#define BIT_MASK_R_NORM_2STS_PATH_MAP_B                             ((u32)0x00000003 << 24)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_PATH_MAP_B(x)                               ((u32)(((x) & 0x00000003) << 24))
#define BIT_GET_R_NORM_2STS_PATH_MAP_B(x)                           ((u32)(((x >> 24) & 0x00000003)))
#define BIT_MASK_R_NORM_2STS_PATH_MAP_A                             ((u32)0x00000003 << 22)          /*!<R/W 0   */
#define BIT_R_NORM_2STS_PATH_MAP_A(x)                               ((u32)(((x) & 0x00000003) << 22))
#define BIT_GET_R_NORM_2STS_PATH_MAP_A(x)                           ((u32)(((x >> 22) & 0x00000003)))
#define BIT_MASK_R_NORM_1STS_PATH_MAP_D                             ((u32)0x00000003 << 20)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_PATH_MAP_D(x)                               ((u32)(((x) & 0x00000003) << 20))
#define BIT_GET_R_NORM_1STS_PATH_MAP_D(x)                           ((u32)(((x >> 20) & 0x00000003)))
#define BIT_MASK_R_NORM_1STS_PATH_MAP_C                             ((u32)0x00000003 << 18)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_PATH_MAP_C(x)                               ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_R_NORM_1STS_PATH_MAP_C(x)                           ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_R_NORM_1STS_PATH_MAP_B                             ((u32)0x00000003 << 16)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_PATH_MAP_B(x)                               ((u32)(((x) & 0x00000003) << 16))
#define BIT_GET_R_NORM_1STS_PATH_MAP_B(x)                           ((u32)(((x >> 16) & 0x00000003)))
#define BIT_MASK_R_NORM_1STS_PATH_MAP_A                             ((u32)0x00000003 << 14)          /*!<R/W 0   */
#define BIT_R_NORM_1STS_PATH_MAP_A(x)                               ((u32)(((x) & 0x00000003) << 14))
#define BIT_GET_R_NORM_1STS_PATH_MAP_A(x)                           ((u32)(((x >> 14) & 0x00000003)))
#define BIT_MASK_R_HANG_PROOF_PATH_MAP_D                            ((u32)0x00000003 << 12)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_PATH_MAP_D(x)                              ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_R_HANG_PROOF_PATH_MAP_D(x)                          ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_R_HANG_PROOF_PATH_MAP_C                            ((u32)0x00000003 << 10)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_PATH_MAP_C(x)                              ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_R_HANG_PROOF_PATH_MAP_C(x)                          ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_R_HANG_PROOF_PATH_MAP_B                            ((u32)0x00000003 << 8)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_PATH_MAP_B(x)                              ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_HANG_PROOF_PATH_MAP_B(x)                          ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_HANG_PROOF_PATH_MAP_A                            ((u32)0x00000003 << 6)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_PATH_MAP_A(x)                              ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_HANG_PROOF_PATH_MAP_A(x)                          ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_GRP_8TX_PATH_MAP_D                               ((u32)0x00000003 << 4)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_PATH_MAP_D(x)                                 ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_GRP_8TX_PATH_MAP_D(x)                             ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_GRP_8TX_PATH_MAP_C                               ((u32)0x00000003 << 2)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_PATH_MAP_C(x)                                 ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_R_GRP_8TX_PATH_MAP_C(x)                             ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_R_GRP_8TX_PATH_MAP_B                               ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_PATH_MAP_B(x)                                 ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_GRP_8TX_PATH_MAP_B(x)                             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR8
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FC_NTX_EN                                             ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_D_EN                                       ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_D                                          ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_C_EN                                       ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_C                                          ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_B_EN                                       ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_B                                          ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_A_EN                                       ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_FC_ANT_SEL_A                                          ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_CLR_PROOF_CNT                                         ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_CBW80P80_EN                                           ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_BYPASS_ANT_SEL_HETB_BW80P80_EN                        ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_BF_2STS_ANT_SEL_D                                     ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_BF_2STS_ANT_SEL_C                                     ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_BF_2STS_ANT_SEL_B                                     ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_BF_2STS_ANT_SEL_A                                     ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_BF_1STS_ANT_SEL_D                                     ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_BF_1STS_ANT_SEL_C                                     ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_MASK_R_RESP_2STS_PATH_MAP_D                             ((u32)0x00000003 << 12)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_PATH_MAP_D(x)                               ((u32)(((x) & 0x00000003) << 12))
#define BIT_GET_R_RESP_2STS_PATH_MAP_D(x)                           ((u32)(((x >> 12) & 0x00000003)))
#define BIT_MASK_R_RESP_2STS_PATH_MAP_C                             ((u32)0x00000003 << 10)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_PATH_MAP_C(x)                               ((u32)(((x) & 0x00000003) << 10))
#define BIT_GET_R_RESP_2STS_PATH_MAP_C(x)                           ((u32)(((x >> 10) & 0x00000003)))
#define BIT_MASK_R_RESP_2STS_PATH_MAP_B                             ((u32)0x00000003 << 8)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_PATH_MAP_B(x)                               ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_RESP_2STS_PATH_MAP_B(x)                           ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_RESP_2STS_PATH_MAP_A                             ((u32)0x00000003 << 6)          /*!<R/W 0   */
#define BIT_R_RESP_2STS_PATH_MAP_A(x)                               ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_RESP_2STS_PATH_MAP_A(x)                           ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_RESP_1STS_PATH_MAP_D                             ((u32)0x00000003 << 4)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_PATH_MAP_D(x)                               ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_RESP_1STS_PATH_MAP_D(x)                           ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_RESP_1STS_PATH_MAP_C                             ((u32)0x00000003 << 2)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_PATH_MAP_C(x)                               ((u32)(((x) & 0x00000003) << 2))
#define BIT_GET_R_RESP_1STS_PATH_MAP_C(x)                           ((u32)(((x >> 2) & 0x00000003)))
#define BIT_MASK_R_RESP_1STS_PATH_MAP_B                             ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define BIT_R_RESP_1STS_PATH_MAP_B(x)                               ((u32)(((x) & 0x00000003) << 0))
#define BIT_GET_R_RESP_1STS_PATH_MAP_B(x)                           ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR9
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_GRP_2TX_ANT_SEL_C                                     ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_ANT_SEL_B                                     ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_ANT_SEL_A                                     ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_ANT_SEL_D                                     ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_ANT_SEL_C                                     ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_ANT_SEL_B                                     ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_GRP_1TX_ANT_SEL_A                                     ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_FORCE_PAR_MODE_ON                                     ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_INPUT_MUMIMO_NSTS_HANG_PROOF_EN_VAL       ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_INPUT_MUMIMO_NSTS_HANG_PROOF_EN_ON        ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_HANG_PROOF_EN_VAL                         ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_HANG_PROOF_EN_ON                          ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SPATIAL_EXPAN_NUM_ON                  ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_D_VAL                             ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_D_ON                              ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_C_VAL                             ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_C_ON                              ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_B_VAL                             ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_B_ON                              ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_A_VAL                             ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_SEL_A_ON                              ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_PATH_EN_ON                            ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_D_ON                              ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_C_ON                              ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_B_ON                              ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_ANT_MAP_A_ON                              ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_FC_SPATIAL_EXPAN_NUM_EN                               ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_D_EN                                      ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_C_EN                                      ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_B_EN                                      ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_FC_PATH_MAP_A_EN                                      ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_FC_PATH_EN_EN                                         ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR10
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_INPUT_MUMIMO_NSTS_HANG_PROOF_EN                       ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_OPT                                        ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_EN                                         ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_ANT_SEL_D                                  ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_ANT_SEL_C                                  ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_ANT_SEL_B                                  ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_HANG_PROOF_ANT_SEL_A                                  ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_ANT_SEL_D                                     ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_ANT_SEL_C                                     ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_ANT_SEL_B                                     ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_GRP_8TX_ANT_SEL_A                                     ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_ANT_SEL_D                                     ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_ANT_SEL_C                                     ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_ANT_SEL_B                                     ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_GRP_7TX_ANT_SEL_A                                     ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_ANT_SEL_D                                     ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_ANT_SEL_C                                     ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_ANT_SEL_B                                     ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_GRP_6TX_ANT_SEL_A                                     ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_ANT_SEL_D                                     ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_ANT_SEL_C                                     ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_ANT_SEL_B                                     ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_GRP_5TX_ANT_SEL_A                                     ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_ANT_SEL_D                                     ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_ANT_SEL_C                                     ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_ANT_SEL_B                                     ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_GRP_4TX_ANT_SEL_A                                     ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_ANT_SEL_D                                     ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_ANT_SEL_C                                     ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_ANT_SEL_B                                     ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_GRP_3TX_ANT_SEL_A                                     ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_GRP_2TX_ANT_SEL_D                                     ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR11
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_HANG_PROOF_ON                                   ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_USR_LIST_TBL_RD                                       ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_RESP_2STSANT_SEL_D                                    ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_RESP_2STSANT_SEL_C                                    ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_RESP_2STSANT_SEL_B                                    ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_RESP_2STSANT_SEL_A                                    ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_RESP_1STSANT_SEL_D                                    ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_RESP_1STSANT_SEL_C                                    ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_RESP_1STSANT_SEL_B                                    ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_RESP_1STSANT_SEL_A                                    ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_NORM_2STSANT_SEL_D                                    ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_NORM_2STSANT_SEL_C                                    ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_NORM_2STSANT_SEL_B                                    ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_NORM_2STSANT_SEL_A                                    ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_NORM_1STSANT_SEL_D                                    ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_NORM_1STSANT_SEL_C                                    ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_NORM_1STSANT_SEL_B                                    ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_NORM_1STSANT_SEL_A                                    ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_N_TX_CAL_MOD_HANG_PROOF                               ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_N_TX_CAL_MOD                                          ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_INPUT_MUMIMO_NSTS_HANG_PROOF_OPT                      ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR12
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_FORCE_PWRSR_TXPWR_PD_VAL                         ((u32)0x0000003F << 26)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRSR_TXPWR_PD_VAL(x)                           ((u32)(((x) & 0x0000003F) << 26))
#define BIT_GET_R_FORCE_PWRSR_TXPWR_PD_VAL(x)                       ((u32)(((x >> 26) & 0x0000003F)))
#define BIT_MASK_R_FORCE_PWRCOM_RU_ALLOC_VAL                        ((u32)0x000000FF << 18)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_RU_ALLOC_VAL(x)                          ((u32)(((x) & 0x000000FF) << 18))
#define BIT_GET_R_FORCE_PWRCOM_RU_ALLOC_VAL(x)                      ((u32)(((x >> 18) & 0x000000FF)))
#define BIT_MASK_R_FORCE_PWRUL_RPL_VAL                              ((u32)0x000001FF << 9)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_RPL_VAL(x)                                ((u32)(((x) & 0x000001FF) << 9))
#define BIT_GET_R_FORCE_PWRUL_RPL_VAL(x)                            ((u32)(((x >> 9) & 0x000001FF)))
#define BIT_MASK_R_FORCE_PWRUL_RPL_UP_VAL                           ((u32)0x000001FF << 0)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_RPL_UP_VAL(x)                             ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_FORCE_PWRUL_RPL_UP_VAL(x)                         ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR13
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_DBG_ANT_SEL                                      ((u32)0x0000000F << 28)          /*!<R/W 0   */
#define BIT_R_DBG_ANT_SEL(x)                                        ((u32)(((x) & 0x0000000F) << 28))
#define BIT_GET_R_DBG_ANT_SEL(x)                                    ((u32)(((x >> 28) & 0x0000000F)))
#define BIT_MASK_R_FORCE_PWRSR_TXPWR_TOLER_VAL                      ((u32)0x0000003F << 22)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRSR_TXPWR_TOLER_VAL(x)                        ((u32)(((x) & 0x0000003F) << 22))
#define BIT_GET_R_FORCE_PWRSR_TXPWR_TOLER_VAL(x)                    ((u32)(((x >> 22) & 0x0000003F)))
#define BIT_MASK_R_FORCE_PWRUL_TAR_RSSI_VAL                         ((u32)0x0000007F << 15)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_TAR_RSSI_VAL(x)                           ((u32)(((x) & 0x0000007F) << 15))
#define BIT_GET_R_FORCE_PWRUL_TAR_RSSI_VAL(x)                       ((u32)(((x >> 15) & 0x0000007F)))
#define BIT_MASK_R_FORCE_PWRCOM_MACID_VAL                           ((u32)0x0000007F << 8)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_MACID_VAL(x)                             ((u32)(((x) & 0x0000007F) << 8))
#define BIT_GET_R_FORCE_PWRCOM_MACID_VAL(x)                         ((u32)(((x >> 8) & 0x0000007F)))
#define BIT_MASK_R_FORCE_PWRRU_CH20_WITH_DATA_VAL                   ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRRU_CH20_WITH_DATA_VAL(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_FORCE_PWRRU_CH20_WITH_DATA_VAL(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR14
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_FORCE_PWRCOM_CTRL_VAL                            ((u32)0x00000007 << 29)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CTRL_VAL(x)                              ((u32)(((x) & 0x00000007) << 29))
#define BIT_GET_R_FORCE_PWRCOM_CTRL_VAL(x)                          ((u32)(((x >> 29) & 0x00000007)))
#define BIT_MASK_R_FORCE_PWRANT_NSTS_VAL                            ((u32)0x00000007 << 26)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_NSTS_VAL(x)                              ((u32)(((x) & 0x00000007) << 26))
#define BIT_GET_R_FORCE_PWRANT_NSTS_VAL(x)                          ((u32)(((x >> 26) & 0x00000007)))
#define BIT_MASK_R_FORCE_MACID_PWR_MODE_VAL                         ((u32)0x00000007 << 23)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_PWR_MODE_VAL(x)                           ((u32)(((x) & 0x00000007) << 23))
#define BIT_GET_R_FORCE_MACID_PWR_MODE_VAL(x)                       ((u32)(((x >> 23) & 0x00000007)))
#define BIT_MASK_R_FORCE_PWRCOM_TXSC_VAL                            ((u32)0x0000000F << 19)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_TXSC_VAL(x)                              ((u32)(((x) & 0x0000000F) << 19))
#define BIT_GET_R_FORCE_PWRCOM_TXSC_VAL(x)                          ((u32)(((x >> 19) & 0x0000000F)))
#define BIT_MASK_R_FORCE_PWRCOM_PPDU_TYPE_VAL                       ((u32)0x0000000F << 15)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_PPDU_TYPE_VAL(x)                         ((u32)(((x) & 0x0000000F) << 15))
#define BIT_GET_R_FORCE_PWRCOM_PPDU_TYPE_VAL(x)                     ((u32)(((x >> 15) & 0x0000000F)))
#define BIT_MASK_R_FORCE_OUT_PWRCOM_MAX_MCS_VAL                     ((u32)0x0000000F << 11)          /*!<R/W 0   */
#define BIT_R_FORCE_OUT_PWRCOM_MAX_MCS_VAL(x)                       ((u32)(((x) & 0x0000000F) << 11))
#define BIT_GET_R_FORCE_OUT_PWRCOM_MAX_MCS_VAL(x)                   ((u32)(((x >> 11) & 0x0000000F)))
#define BIT_MASK_R_FORCE_PWRCOM_MCS_VAL                             ((u32)0x0000001F << 6)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_MCS_VAL(x)                               ((u32)(((x) & 0x0000001F) << 6))
#define BIT_GET_R_FORCE_PWRCOM_MCS_VAL(x)                           ((u32)(((x >> 6) & 0x0000001F)))
#define BIT_MASK_R_FORCE_PWRUL_AP_TX_PWR_VAL                        ((u32)0x0000003F << 0)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_AP_TX_PWR_VAL(x)                          ((u32)(((x) & 0x0000003F) << 0))
#define BIT_GET_R_FORCE_PWRUL_AP_TX_PWR_VAL(x)                      ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR15
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_MACID_TXBF_EN_VAL                               ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_TXBF_EN_ON                                ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_RESP_EN_VAL                               ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_RESP_EN_ON                                ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_PWR_MODE_ON                               ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_NORM_EN_VAL                               ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_NORM_EN_ON                                ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_NDP_EN_VAL                                ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_NDP_EN_ON                                 ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_MUMIMO_EN_VAL                             ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_MUMIMO_EN_ON                              ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_CCA_TH_ALLOW_VAL                          ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_FORCE_MACID_CCA_TH_ALLOW_ON                           ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_FORCE_CCA_TH_ALLOW_VAL                                ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_FORCE_CCA_TH_ALLOW_ON                                 ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_CLR_ANT_SET                                           ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_ANT_SEL_DBG_EN                                        ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_MASK_R_FORCE_PWRCOM_PRECODING_MODE_IDX_VAL              ((u32)0x00000003 << 7)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_PRECODING_MODE_IDX_VAL(x)                ((u32)(((x) & 0x00000003) << 7))
#define BIT_GET_R_FORCE_PWRCOM_PRECODING_MODE_IDX_VAL(x)            ((u32)(((x >> 7) & 0x00000003)))
#define BIT_MASK_R_FORCE_PWRCOM_DBW_IDX_VAL                         ((u32)0x00000003 << 5)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_DBW_IDX_VAL(x)                           ((u32)(((x) & 0x00000003) << 5))
#define BIT_GET_R_FORCE_PWRCOM_DBW_IDX_VAL(x)                       ((u32)(((x >> 5) & 0x00000003)))
#define BIT_MASK_R_FORCE_PWRCOM_CBW_IDX_VAL                         ((u32)0x00000003 << 3)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CBW_IDX_VAL(x)                           ((u32)(((x) & 0x00000003) << 3))
#define BIT_GET_R_FORCE_PWRCOM_CBW_IDX_VAL(x)                       ((u32)(((x >> 3) & 0x00000003)))
#define BIT_MASK_R_FORCE_PWRRU_GRP_NTX_VAL                          ((u32)0x00000007 << 0)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRRU_GRP_NTX_VAL(x)                            ((u32)(((x) & 0x00000007) << 0))
#define BIT_GET_R_FORCE_PWRRU_GRP_NTX_VAL(x)                        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR16
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_FORCE_PWRUL_RPL_UP_EN_ON                              ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_RPL_ON                                    ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_AP_TX_PWR_ON                              ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRTSSI_FAST_MODE_EN_VAL                        ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRTSSI_FAST_MODE_EN_ON                         ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRSR_TXPWR_TOLER_ON                            ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRSR_TXPWR_PD_ON                               ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRRU_GRP_NTX_ON                                ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRRU_CH20_WITH_DATA_ON                         ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_TXSC_ON                                  ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_STBC_EN_VAL                              ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_STBC_EN_ON                               ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_RX_LTE_VAL                               ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_RX_LTE_ON                                ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_RU_ALLOC_ON                              ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_PRECODING_MODE_IDX_ON                    ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_PPDU_TYPE_ON                             ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_MCS_ON                                   ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_MACID_ON                                 ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_DCM_VAL                                  ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_DCM_ON                                   ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_DBW_IDX_ON                               ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CTRL_REG_VAL                             ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CTRL_REG_ON                              ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CTRL_ON                                  ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRCOM_CBW_IDX_ON                               ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_NSTS_ON                                  ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_NDP_EN_VAL                               ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_NDP_EN_ON                                ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_ACTIVE_TX_EN_VAL                         ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRANT_ACTIVE_TX_EN_ON                          ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_FORCE_OUT_PWRCOM_MAX_MCS_ON                           ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR17
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TSSI_CTRL_MAC_PWRCOM_CH20_WITH_DATA_BIT_REV           ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_TSSI_CTRL_MAC_PWRCOM_CH20_WITH_DATA_BIT_INV           ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_SR_MCS_MASK_DIS                                       ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_PWRRU_CH20_WITH_DATA_HANG_PROOF_EN                    ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_MUMIMO_APPLY_PWRLIMBF_EN                              ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_HERU_MASK_DIS                                         ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_HERU_CH20_WITH_DATA_FROM_F2P_DIS                      ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_TAR_RSSI_ON                               ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_RPL_UP_ON                                 ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_FORCE_PWRUL_RPL_UP_EN_VAL                             ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TCM_WRAPPER_CR18
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_RU_ALLOC_DBW160_FIX_EN                                ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TX_BANDEDGE_CFG_FORCE_OUT_ON                ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_MSR_ATHESTF_FORCE_OUT_VAL              ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_MSR_ATHESTF_FORCE_OUT_ON               ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_FAST_MODE_EN_FORCE_OUT_VAL             ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_FAST_MODE_EN_FORCE_OUT_ON              ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_DIFF_SEG0_DB_FORCE_OUT_ON              ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_PWR_OFST_SEG0_DB_FORCE_OUT_ON               ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_MAC_DEF_LEGACY_RATE_IDX_BEGIN_AT_4_ON                 ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_MASK_R_PWRTSSICT_TX_BANDEDGE_CFG_FORCE_OUT_VAL          ((u32)0x00000003 << 18)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TX_BANDEDGE_CFG_FORCE_OUT_VAL(x)            ((u32)(((x) & 0x00000003) << 18))
#define BIT_GET_R_PWRTSSICT_TX_BANDEDGE_CFG_FORCE_OUT_VAL(x)        ((u32)(((x >> 18) & 0x00000003)))
#define BIT_MASK_R_PWRTSSICT_TSSI_DIFF_SEG0_DB_FORCE_OUT_VAL        ((u32)0x000001FF << 9)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_TSSI_DIFF_SEG0_DB_FORCE_OUT_VAL(x)          ((u32)(((x) & 0x000001FF) << 9))
#define BIT_GET_R_PWRTSSICT_TSSI_DIFF_SEG0_DB_FORCE_OUT_VAL(x)      ((u32)(((x >> 9) & 0x000001FF)))
#define BIT_MASK_R_PWRTSSICT_PWR_OFST_SEG0_DB_FORCE_OUT_VAL         ((u32)0x000001FF << 0)          /*!<R/W 0   */
#define BIT_R_PWRTSSICT_PWR_OFST_SEG0_DB_FORCE_OUT_VAL(x)           ((u32)(((x) & 0x000001FF) << 0))
#define BIT_GET_R_PWRTSSICT_PWR_OFST_SEG0_DB_FORCE_OUT_VAL(x)       ((u32)(((x >> 0) & 0x000001FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_3                    ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_3(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_3(x)                  ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_2                    ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_2(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_2(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_1                    ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_1(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_1(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_0                    ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_0(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_0(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_7                    ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_7(x)                      ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_7(x)                  ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_6                    ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_6(x)                      ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_6(x)                  ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_5                    ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_5(x)                      ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_5(x)                  ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW160_4                    ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW160_4(x)                      ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW160_4(x)                  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE2
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW80_0                     ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW80_0(x)                       ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW80_0(x)                   ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW40_1                     ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW40_1(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW40_1(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW40_0                     ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW40_0(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW40_0(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW20                       ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW20(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW20(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE3
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_0                       ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_0(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_0(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW80_3                     ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW80_3(x)                       ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW80_3(x)                   ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW80_2                     ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW80_2(x)                       ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW80_2(x)                   ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_NONZERO_CFG_SBW80_1                     ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONZERO_CFG_SBW80_1(x)                       ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_NONZERO_CFG_SBW80_1(x)                   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE4
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_4                       ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_4(x)                         ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_4(x)                     ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_3                       ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_3(x)                         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_3(x)                     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_2                       ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_2(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_2(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_1                       ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_1(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_1(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE5
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW20                          ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW20(x)                            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW20(x)                        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_7                       ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_7(x)                         ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_7(x)                     ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_6                       ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_6(x)                         ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_6(x)                     ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW160_5                       ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW160_5(x)                         ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW160_5(x)                     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE6
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW80_1                        ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW80_1(x)                          ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW80_1(x)                      ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW80_0                        ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW80_0(x)                          ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW80_0(x)                      ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW40_1                        ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW40_1(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW40_1(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW40_0                        ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW40_0(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW40_0(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE7
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BMODE_INIT_OFST_SEG0_DB                          ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_BMODE_INIT_OFST_SEG0_DB(x)                            ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_BMODE_INIT_OFST_SEG0_DB(x)                        ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_BMODE_INIT_DIFF_SEG0_DB                          ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_BMODE_INIT_DIFF_SEG0_DB(x)                            ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_BMODE_INIT_DIFF_SEG0_DB(x)                        ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW80_3                        ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW80_3(x)                          ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW80_3(x)                      ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_BANDEDGE_ZERO_CFG_SBW80_2                        ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_ZERO_CFG_SBW80_2(x)                          ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_BANDEDGE_ZERO_CFG_SBW80_2(x)                      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE8
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CBW_SUB20_3                                      ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_3(x)                                        ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_CBW_SUB20_3(x)                                    ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_2                                      ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_2(x)                                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_CBW_SUB20_2(x)                                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_1                                      ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_1(x)                                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_CBW_SUB20_1(x)                                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_0                                      ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_0(x)                                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CBW_SUB20_0(x)                                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE9
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CBW_SUB20_7                                      ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_7(x)                                        ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_CBW_SUB20_7(x)                                    ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_6                                      ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_6(x)                                        ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_CBW_SUB20_6(x)                                    ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_5                                      ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_5(x)                                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_CBW_SUB20_5(x)                                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_CBW_SUB20_4                                      ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_CBW_SUB20_4(x)                                        ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CBW_SUB20_4(x)                                    ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE10
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW40               ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW40(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW40(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW20               ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW20(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW20(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW160              ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW160(x)                ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW160(x)            ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_COMMON_DIFF_DB                           ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_CH_COMB_COMMON_DIFF_DB(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CH_COMB_COMMON_DIFF_DB(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE11
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_ZERO_BW40                  ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_ZERO_BW40(x)                    ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_ZERO_BW40(x)                ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_ZERO_BW20                  ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_ZERO_BW20(x)                    ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_ZERO_BW20(x)                ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_ZERO_BW160                 ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_ZERO_BW160(x)                   ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_ZERO_BW160(x)               ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW80               ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW80(x)                 ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_NONZERO_BW80(x)             ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE12
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_F_PWR_OFST_OUT_SEG0_DB                           ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_F_PWR_OFST_OUT_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_F_PWR_OFST_OUT_SEG0_DB(x)                         ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_F_CH20_WITH_DATA_NONLEGACY_VAL                   ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_F_CH20_WITH_DATA_NONLEGACY_VAL(x)                     ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_F_CH20_WITH_DATA_NONLEGACY_VAL(x)                 ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_F_CH20_WITH_DATA_LEGACY_VAL                      ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_F_CH20_WITH_DATA_LEGACY_VAL(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_F_CH20_WITH_DATA_LEGACY_VAL(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_CH_COMB_OFST_BANDEDGE_ZERO_BW80                  ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_CH_COMB_OFST_BANDEDGE_ZERO_BW80(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_CH_COMB_OFST_BANDEDGE_ZERO_BW80(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE13
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_HEMU_INIT_DIFF_SEG0_DB                           ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_HEMU_INIT_DIFF_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_HEMU_INIT_DIFF_SEG0_DB(x)                         ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_HEER_INIT_OFST_SEG0_DB                           ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_HEER_INIT_OFST_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_HEER_INIT_OFST_SEG0_DB(x)                         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_HEER_INIT_DIFF_SEG0_DB                           ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_HEER_INIT_DIFF_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_HEER_INIT_DIFF_SEG0_DB(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_F_TSSI_DIFF_OUT_SEG0_DB                          ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_F_TSSI_DIFF_OUT_SEG0_DB(x)                            ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_F_TSSI_DIFF_OUT_SEG0_DB(x)                        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE14
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_HETB_INIT_DIFF_ATHESTF_SEG0_DB                   ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_HETB_INIT_DIFF_ATHESTF_SEG0_DB(x)                     ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_HETB_INIT_DIFF_ATHESTF_SEG0_DB(x)                 ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_HESU_INIT_OFST_SEG0_DB                           ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_HESU_INIT_OFST_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_HESU_INIT_OFST_SEG0_DB(x)                         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_HESU_INIT_DIFF_SEG0_DB                           ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_HESU_INIT_DIFF_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_HESU_INIT_DIFF_SEG0_DB(x)                         ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_HEMU_INIT_OFST_SEG0_DB                           ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_HEMU_INIT_OFST_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_HEMU_INIT_OFST_SEG0_DB(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE15
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_HT_INIT_DIFF_SEG0_DB                             ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_HT_INIT_DIFF_SEG0_DB(x)                               ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_HT_INIT_DIFF_SEG0_DB(x)                           ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_HETB_INIT_OFST_SEG0_DB                           ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_HETB_INIT_OFST_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_HETB_INIT_OFST_SEG0_DB(x)                         ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_HETB_INIT_OFST_ATHESTF_SEG0_DB                   ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_HETB_INIT_OFST_ATHESTF_SEG0_DB(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_HETB_INIT_OFST_ATHESTF_SEG0_DB(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_HETB_INIT_DIFF_SEG0_DB                           ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_HETB_INIT_DIFF_SEG0_DB(x)                             ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_HETB_INIT_DIFF_SEG0_DB(x)                         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE16
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_VHT_INIT_DIFF_SEG0_DB                            ((u32)0x000000FF << 24)          /*!<R/W 0   */
#define BIT_R_VHT_INIT_DIFF_SEG0_DB(x)                              ((u32)(((x) & 0x000000FF) << 24))
#define BIT_GET_R_VHT_INIT_DIFF_SEG0_DB(x)                          ((u32)(((x >> 24) & 0x000000FF)))
#define BIT_MASK_R_LEGACY_INIT_OFST_SEG0_DB                         ((u32)0x000000FF << 16)          /*!<R/W 0   */
#define BIT_R_LEGACY_INIT_OFST_SEG0_DB(x)                           ((u32)(((x) & 0x000000FF) << 16))
#define BIT_GET_R_LEGACY_INIT_OFST_SEG0_DB(x)                       ((u32)(((x >> 16) & 0x000000FF)))
#define BIT_MASK_R_LEGACY_INIT_DIFF_SEG0_DB                         ((u32)0x000000FF << 8)          /*!<R/W 0   */
#define BIT_R_LEGACY_INIT_DIFF_SEG0_DB(x)                           ((u32)(((x) & 0x000000FF) << 8))
#define BIT_GET_R_LEGACY_INIT_DIFF_SEG0_DB(x)                       ((u32)(((x >> 8) & 0x000000FF)))
#define BIT_MASK_R_HT_INIT_OFST_SEG0_DB                             ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_HT_INIT_OFST_SEG0_DB(x)                               ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_HT_INIT_OFST_SEG0_DB(x)                           ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE17
 * @brief
 * @{
 *****************************************************************************/
#define BIT_MASK_R_BANDEDGE_CFG                                     ((u32)0x00000003 << 30)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_CFG(x)                                       ((u32)(((x) & 0x00000003) << 30))
#define BIT_GET_R_BANDEDGE_CFG(x)                                   ((u32)(((x >> 30) & 0x00000003)))
#define BIT_MASK_R_F_RATE_IDX                                       ((u32)0x0000000F << 26)          /*!<R/W 0   */
#define BIT_R_F_RATE_IDX(x)                                         ((u32)(((x) & 0x0000000F) << 26))
#define BIT_GET_R_F_RATE_IDX(x)                                     ((u32)(((x >> 26) & 0x0000000F)))
#define BIT_MASK_R_F_FORMAT                                         ((u32)0x0000000F << 22)          /*!<R/W 0   */
#define BIT_R_F_FORMAT(x)                                           ((u32)(((x) & 0x0000000F) << 22))
#define BIT_GET_R_F_FORMAT(x)                                       ((u32)(((x >> 22) & 0x0000000F)))
#define BIT_MASK_R_BANDEDGE_NONLEGACY_RATEIDX_TH                    ((u32)0x0000000F << 18)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_NONLEGACY_RATEIDX_TH(x)                      ((u32)(((x) & 0x0000000F) << 18))
#define BIT_GET_R_BANDEDGE_NONLEGACY_RATEIDX_TH(x)                  ((u32)(((x >> 18) & 0x0000000F)))
#define BIT_MASK_R_BANDEDGE_LEGACY_RATEIDX_TH                       ((u32)0x0000000F << 14)          /*!<R/W 0   */
#define BIT_R_BANDEDGE_LEGACY_RATEIDX_TH(x)                         ((u32)(((x) & 0x0000000F) << 14))
#define BIT_GET_R_BANDEDGE_LEGACY_RATEIDX_TH(x)                     ((u32)(((x >> 14) & 0x0000000F)))
#define BIT_MASK_R_TSSI_CTRL_MAC_DBG_SEL                            ((u32)0x0000003F << 8)          /*!<R/W 0   */
#define BIT_R_TSSI_CTRL_MAC_DBG_SEL(x)                              ((u32)(((x) & 0x0000003F) << 8))
#define BIT_GET_R_TSSI_CTRL_MAC_DBG_SEL(x)                          ((u32)(((x >> 8) & 0x0000003F)))
#define BIT_MASK_R_VHT_INIT_OFST_SEG0_DB                            ((u32)0x000000FF << 0)          /*!<R/W 0   */
#define BIT_R_VHT_INIT_OFST_SEG0_DB(x)                              ((u32)(((x) & 0x000000FF) << 0))
#define BIT_GET_R_VHT_INIT_OFST_SEG0_DB(x)                          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE18
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_F_HEER_BYPASS_CBW_FLTR_EN                             ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_F_HEER_BANDEDGE_IND_VAL                               ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_F_HEER_BANDEDGE_IND_EN                                ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_F_FORMAT_EN                                           ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_F_FAST_MODE_COMP_INIT_EN                              ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_F_DBW_EN                                              ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_F_CH20_WITH_DATA_EN                                   ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_F_CBW_EN                                              ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_F_BMODE_OFST_OUT_EN                                   ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_F_BMODE_DIFF_OUT_EN                                   ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_F_BMODE_BYPASS_DBW_FLTR_EN                            ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_F_BMODE_BYPASS_CBW_FLTR_EN                            ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_F_BMODE_BANDEDGE_IND_VAL                              ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_F_BMODE_BANDEDGE_IND_EN                               ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_SUSTBC_EN                              ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_SUDCM_EN                               ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_SUBF_EN                                ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_NDP_EN                                 ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_MU_EN                                  ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_F_BANDEDGE_CFG_HETB_EN                                ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_CH20_WITH_DATA_NONLEGACY_SWAP                         ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_CH20_WITH_DATA_LEGACY_SWAP                            ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_MASK_R_F_PRECODING_MODE_IDX                             ((u32)0x00000003 << 8)          /*!<R/W 0   */
#define BIT_R_F_PRECODING_MODE_IDX(x)                               ((u32)(((x) & 0x00000003) << 8))
#define BIT_GET_R_F_PRECODING_MODE_IDX(x)                           ((u32)(((x >> 8) & 0x00000003)))
#define BIT_MASK_R_F_DBW                                            ((u32)0x00000003 << 6)          /*!<R/W 0   */
#define BIT_R_F_DBW(x)                                              ((u32)(((x) & 0x00000003) << 6))
#define BIT_GET_R_F_DBW(x)                                          ((u32)(((x >> 6) & 0x00000003)))
#define BIT_MASK_R_F_CBW                                            ((u32)0x00000003 << 4)          /*!<R/W 0   */
#define BIT_R_F_CBW(x)                                              ((u32)(((x) & 0x00000003) << 4))
#define BIT_GET_R_F_CBW(x)                                          ((u32)(((x >> 4) & 0x00000003)))
#define BIT_MASK_R_F_TXSC                                           ((u32)0x0000000F << 0)          /*!<R/W 0   */
#define BIT_R_F_TXSC(x)                                             ((u32)(((x) & 0x0000000F) << 0))
#define BIT_GET_R_F_TXSC(x)                                         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE19
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_F_IS_SU_STBC                                          ((u32)0x00000001 << 31)          /*!<R/W 0   */
#define BIT_R_F_IS_SU_DCM_EN                                        ((u32)0x00000001 << 30)          /*!<R/W 0   */
#define BIT_R_F_IS_SU_DCM                                           ((u32)0x00000001 << 29)          /*!<R/W 0   */
#define BIT_R_F_IS_NDP_EN                                           ((u32)0x00000001 << 28)          /*!<R/W 0   */
#define BIT_R_F_IS_NDP                                              ((u32)0x00000001 << 27)          /*!<R/W 0   */
#define BIT_R_F_HT_OFST_OUT_EN                                      ((u32)0x00000001 << 26)          /*!<R/W 0   */
#define BIT_R_F_HT_DIFF_OUT_EN                                      ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define BIT_R_F_HT_BYPASS_DBW_FLTR_EN                               ((u32)0x00000001 << 24)          /*!<R/W 0   */
#define BIT_R_F_HT_BYPASS_CBW_FLTR_EN                               ((u32)0x00000001 << 23)          /*!<R/W 0   */
#define BIT_R_F_HT_BANDEDGE_IND_VAL                                 ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_F_HT_BANDEDGE_IND_EN                                  ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_F_HETB_OFST_OUT_EN                                    ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_F_HETB_DIFF_OUT_EN                                    ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_F_HETB_BYPASS_DBW_FLTR_EN                             ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_F_HETB_BYPASS_CBW_FLTR_EN                             ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_F_HETB_BANDEDGE_IND_VAL                               ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_F_HETB_BANDEDGE_IND_EN                                ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_F_HESU_OFST_OUT_EN                                    ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_F_HESU_DIFF_OUT_EN                                    ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_F_HESU_BYPASS_DBW_FLTR_EN                             ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_F_HESU_BYPASS_CBW_FLTR_EN                             ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_F_HESU_BANDEDGE_IND_VAL                               ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_F_HESU_BANDEDGE_IND_EN                                ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_F_HEMU_OFST_OUT_EN                                    ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_F_HEMU_DIFF_OUT_EN                                    ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_F_HEMU_BYPASS_DBW_FLTR_EN                             ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_F_HEMU_BYPASS_CBW_FLTR_EN                             ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_F_HEMU_BANDEDGE_IND_VAL                               ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_F_HEMU_BANDEDGE_IND_EN                                ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_F_HEER_OFST_OUT_EN                                    ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_F_HEER_DIFF_OUT_EN                                    ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_F_HEER_BYPASS_DBW_FLTR_EN                             ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AX_PWR_TSSI_TABLE20
 * @brief
 * @{
 *****************************************************************************/
#define BIT_R_TSSI_CTRL_MAC_USE_NORMAL_RATE_IDX_ONLY                ((u32)0x00000001 << 22)          /*!<R/W 0   */
#define BIT_R_TSSI_CTRL_MAC_DBG_DIS                                 ((u32)0x00000001 << 21)          /*!<R/W 0   */
#define BIT_R_TSSI_CTRL_MAC_CLR                                     ((u32)0x00000001 << 20)          /*!<R/W 0   */
#define BIT_R_HETB_PWR_MSR_MODE                                     ((u32)0x00000001 << 19)          /*!<R/W 0   */
#define BIT_R_F_VHT_OFST_OUT_EN                                     ((u32)0x00000001 << 18)          /*!<R/W 0   */
#define BIT_R_F_VHT_DIFF_OUT_EN                                     ((u32)0x00000001 << 17)          /*!<R/W 0   */
#define BIT_R_F_VHT_BYPASS_DBW_FLTR_EN                              ((u32)0x00000001 << 16)          /*!<R/W 0   */
#define BIT_R_F_VHT_BYPASS_CBW_FLTR_EN                              ((u32)0x00000001 << 15)          /*!<R/W 0   */
#define BIT_R_F_VHT_BANDEDGE_IND_VAL                                ((u32)0x00000001 << 14)          /*!<R/W 0   */
#define BIT_R_F_VHT_BANDEDGE_IND_EN                                 ((u32)0x00000001 << 13)          /*!<R/W 0   */
#define BIT_R_F_TXSC_EN                                             ((u32)0x00000001 << 12)          /*!<R/W 0   */
#define BIT_R_F_TSSI_CTRL_OUT_EN                                    ((u32)0x00000001 << 11)          /*!<R/W 0   */
#define BIT_R_F_TSSI_CTRL_FAST_MODE_IDX_EN                          ((u32)0x00000001 << 10)          /*!<R/W 0   */
#define BIT_R_F_TSSI_CTRL_FAST_MODE_IDX                             ((u32)0x00000001 << 9)          /*!<R/W 0   */
#define BIT_R_F_RATE_IDX_EN                                         ((u32)0x00000001 << 8)          /*!<R/W 0   */
#define BIT_R_F_PRECODING_MODE_IDX_EN                               ((u32)0x00000001 << 7)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_OFST_OUT_EN                                  ((u32)0x00000001 << 6)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_DIFF_OUT_EN                                  ((u32)0x00000001 << 5)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_BYPASS_DBW_FLTR_EN                           ((u32)0x00000001 << 4)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_BYPASS_CBW_FLTR_EN                           ((u32)0x00000001 << 3)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_BANDEDGE_IND_VAL                             ((u32)0x00000001 << 2)          /*!<R/W 0   */
#define BIT_R_F_LEGACY_BANDEDGE_IND_EN                              ((u32)0x00000001 << 1)          /*!<R/W 0   */
#define BIT_R_F_IS_SU_STBC_EN                                       ((u32)0x00000001 << 0)          /*!<R/W 0   */
/** @} */


/*==========TXPWR Register Address Definition==========*/
#define REG_AX_PWR_RATE_CTRL                         0xD200
#define REG_AX_PWR_RATE_OFST_CTRL                    0xD204
#define REG_AX_PWR_LMT_CTRL                          0xD208
#define REG_AX_PWR_MACID_CTRL                        0xD20C
#define REG_AX_PWR_BF_CTRL                           0xD210
#define REG_AX_PWR_MACID_REG                         0xD214
#define REG_AX_PWR_MACID_REG2                        0xD218
#define REG_AX_PWR_RU_LMT_CTRL                       0xD21C
#define REG_AX_PWR_COEXT_CTRL                        0xD220
#define REG_AX_PWR_SWING_LEG_CTRL                    0xD224
#define REG_AX_PWR_SWING_VHT_CTRL                    0xD228
#define REG_AX_PWR_SWING_HE_CTRL                     0xD22C
#define REG_AX_PWR_SWING_OTHER_CTRL0                 0xD230
#define REG_AX_PWR_SWING_OTHER_CTRL1                 0xD234
#define REG_AX_PWR_SR_CTRL0                          0xD238
#define REG_AX_PWR_SR_CTRL1                          0xD23C
#define REG_AX_PWR_UL_CTRL0                          0xD240
#define REG_AX_PWR_UL_CTRL1                          0xD244
#define REG_AX_PWR_UL_CTRL2                          0xD248
#define REG_AX_PWR_UL_CTRL3                          0xD24C
#define REG_AX_PWR_UL_CTRL4                          0xD250
#define REG_AX_PWR_UL_CTRL5                          0xD254
#define REG_AX_PWR_UL_CTRL6                          0xD258
#define REG_AX_PWR_NORM_FORCE0                       0xD25C
#define REG_AX_PWR_NORM_FORCE1                       0xD260
#define REG_AX_PWR_SR_FORCE0                         0xD264
#define REG_AX_PWR_SR_FORCE1                         0xD268
#define REG_AX_PWR_SR_FORCE2                         0xD26C
#define REG_AX_PWR_UL_FORCE0                         0xD270
#define REG_AX_PWR_NORM_FORCE2                       0xD274
#define REG_AX_PWR_UL_FORCE1                         0xD278
#define REG_AX_PWR_NORM_FORCE3                       0xD27C
#define REG_AX_PWR_NORM_FORCE4                       0xD280
#define REG_AX_PWR_BY_RATE_TABLE0                    0xD2C0
#define REG_AX_PWR_BY_RATE_TABLE1                    0xD2C4
#define REG_AX_PWR_BY_RATE_TABLE2                    0xD2C8
#define REG_AX_PWR_BY_RATE_TABLE3                    0xD2CC
#define REG_AX_PWR_BY_RATE_TABLE4                    0xD2D0
#define REG_AX_PWR_BY_RATE_TABLE5                    0xD2D4
#define REG_AX_PWR_BY_RATE_TABLE6                    0xD2D8
#define REG_AX_PWR_LMT_TABLE0                        0xD2EC
#define REG_AX_PWR_LMT_TABLE1                        0xD2F0
#define REG_AX_PWR_RU_LMT_TABLE0                     0xD33C
#define REG_AX_PWR_RU_LMT_TABLE1                     0xD340
#define REG_AX_PWR_RU_LMT_TABLE2                     0xD344
#define REG_AX_PWR_RU_LMT_TABLE3                     0xD348
#define REG_AX_PWR_RU_LMT_TABLE4                     0xD34C
#define REG_AX_PWR_RU_LMT_TABLE5                     0xD350
#define REG_AX_PWR_MACID_LMT_TABLE0                  0xD36C
#define REG_AX_PWR_MACID_LMT_TABLE1                  0xD370
#define REG_AX_PWR_MACID_LMT_TABLE2                  0xD374
#define REG_AX_PWR_MACID_LMT_TABLE3                  0xD378
#define REG_AX_PWR_MACID_LMT_TABLE4                  0xD37C
#define REG_AX_PWR_MACID_LMT_TABLE5                  0xD380
#define REG_AX_PWR_MACID_LMT_TABLE6                  0xD384
#define REG_AX_PWR_MACID_LMT_TABLE7                  0xD388
#define REG_AX_PWR_MACID_LMT_TABLE8                  0xD38C
#define REG_AX_PWR_MACID_LMT_TABLE9                  0xD390
#define REG_AX_PWR_MACID_LMT_TABLE10                 0xD394
#define REG_AX_PWR_MACID_LMT_TABLE11                 0xD398
#define REG_AX_PWR_MACID_LMT_TABLE12                 0xD39C
#define REG_AX_PWR_MACID_LMT_TABLE13                 0xD3A0
#define REG_AX_PWR_MACID_LMT_TABLE14                 0xD3A4
#define REG_AX_PWR_MACID_LMT_TABLE15                 0xD3A8
#define REG_AX_PWR_SR_MCS0_TXDIFF_TABLE0             0xD56C
#define REG_AX_PWR_SR_MCS0_TXDIFF_TABLE1             0xD570
#define REG_AX_PWR_SR_MCS0_TXDIFF_TABLE2             0xD574
#define REG_AX_PWR_SR_MCS0_TXDIFF_TABLE3             0xD578
#define REG_AX_PWR_SR_MCS1_TXDIFF_TABLE0             0xD57C
#define REG_AX_PWR_SR_MCS1_TXDIFF_TABLE1             0xD580
#define REG_AX_PWR_SR_MCS1_TXDIFF_TABLE2             0xD584
#define REG_AX_PWR_SR_MCS1_TXDIFF_TABLE3             0xD588
#define REG_AX_PWR_SR_MCS2_TXDIFF_TABLE0             0xD58C
#define REG_AX_PWR_SR_MCS2_TXDIFF_TABLE1             0xD590
#define REG_AX_PWR_SR_MCS2_TXDIFF_TABLE2             0xD594
#define REG_AX_PWR_SR_MCS2_TXDIFF_TABLE3             0xD598
#define REG_AX_PWR_SR_MCS3_TXDIFF_TABLE0             0xD59C
#define REG_AX_PWR_SR_MCS3_TXDIFF_TABLE1             0xD5A0
#define REG_AX_PWR_SR_MCS3_TXDIFF_TABLE2             0xD5A4
#define REG_AX_PWR_SR_MCS3_TXDIFF_TABLE3             0xD5A8
#define REG_AX_PWR_SR_MCS4_TXDIFF_TABLE0             0xD5AC
#define REG_AX_PWR_SR_MCS4_TXDIFF_TABLE1             0xD5B0
#define REG_AX_PWR_SR_MCS4_TXDIFF_TABLE2             0xD5B4
#define REG_AX_PWR_SR_MCS4_TXDIFF_TABLE3             0xD5B8
#define REG_AX_PWR_SR_MCS5_TXDIFF_TABLE0             0xD5BC
#define REG_AX_PWR_SR_MCS5_TXDIFF_TABLE1             0xD5C0
#define REG_AX_PWR_SR_MCS5_TXDIFF_TABLE2             0xD5C4
#define REG_AX_PWR_SR_MCS5_TXDIFF_TABLE3             0xD5C8
#define REG_AX_PWR_SR_MCS6_TXDIFF_TABLE0             0xD5CC
#define REG_AX_PWR_SR_MCS6_TXDIFF_TABLE1             0xD5D0
#define REG_AX_PWR_SR_MCS6_TXDIFF_TABLE2             0xD5D4
#define REG_AX_PWR_SR_MCS6_TXDIFF_TABLE3             0xD5D8
#define REG_AX_PWR_SR_MCS7_TXDIFF_TABLE0             0xD5DC
#define REG_AX_PWR_SR_MCS7_TXDIFF_TABLE1             0xD5E0
#define REG_AX_PWR_SR_MCS7_TXDIFF_TABLE2             0xD5E4
#define REG_AX_PWR_SR_MCS7_TXDIFF_TABLE3             0xD5E8
#define REG_AX_PWR_SR_MCS8_TXDIFF_TABLE0             0xD5EC
#define REG_AX_PWR_SR_MCS8_TXDIFF_TABLE1             0xD5F0
#define REG_AX_PWR_SR_MCS8_TXDIFF_TABLE2             0xD5F4
#define REG_AX_PWR_SR_MCS8_TXDIFF_TABLE3             0xD5F8
#define REG_AX_PWR_SR_MCS9_TXDIFF_TABLE0             0xD5FC
#define REG_AX_PWR_SR_MCS9_TXDIFF_TABLE1             0xD600
#define REG_AX_PWR_SR_MCS9_TXDIFF_TABLE2             0xD604
#define REG_AX_PWR_SR_MCS9_TXDIFF_TABLE3             0xD608
#define REG_AX_PWR_SR_MCS10_TXDIFF_TABLE0            0xD60C
#define REG_AX_PWR_SR_MCS10_TXDIFF_TABLE1            0xD610
#define REG_AX_PWR_SR_MCS10_TXDIFF_TABLE2            0xD614
#define REG_AX_PWR_SR_MCS10_TXDIFF_TABLE3            0xD618
#define REG_AX_PWR_SR_MCS11_TXDIFF_TABLE0            0xD61C
#define REG_AX_PWR_SR_MCS11_TXDIFF_TABLE1            0xD620
#define REG_AX_PWR_SR_MCS11_TXDIFF_TABLE2            0xD624
#define REG_AX_PWR_SR_MCS11_TXDIFF_TABLE3            0xD628
#define REG_AX_PWR_CCK1M_TXDIFF_TABLE0               0xD62C
#define REG_AX_PWR_CCK1M_TXDIFF_TABLE1               0xD630
#define REG_AX_PWR_CCK1M_TXDIFF_TABLE2               0xD634
#define REG_AX_PWR_CCK1M_TXDIFF_TABLE3               0xD638
#define REG_AX_PWR_CCK2M_TXDIFF_TABLE0               0xD63C
#define REG_AX_PWR_CCK2M_TXDIFF_TABLE1               0xD640
#define REG_AX_PWR_CCK2M_TXDIFF_TABLE2               0xD644
#define REG_AX_PWR_CCK2M_TXDIFF_TABLE3               0xD648
#define REG_AX_PWR_CCK5P5M_TXDIFF_TABLE0             0xD64C
#define REG_AX_PWR_CCK5P5M_TXDIFF_TABLE1             0xD650
#define REG_AX_PWR_CCK5P5M_TXDIFF_TABLE2             0xD654
#define REG_AX_PWR_CCK5P5M_TXDIFF_TABLE3             0xD658
#define REG_AX_PWR_CCK11M_TXDIFF_TABLE0              0xD65C
#define REG_AX_PWR_CCK11M_TXDIFF_TABLE1              0xD660
#define REG_AX_PWR_CCK11M_TXDIFF_TABLE2              0xD664
#define REG_AX_PWR_CCK11M_TXDIFF_TABLE3              0xD668
#define REG_AX_PWR_LEGACY6M_TXDIFF_TABLE0            0xD66C
#define REG_AX_PWR_LEGACY6M_TXDIFF_TABLE1            0xD670
#define REG_AX_PWR_LEGACY6M_TXDIFF_TABLE2            0xD674
#define REG_AX_PWR_LEGACY6M_TXDIFF_TABLE3            0xD678
#define REG_AX_PWR_LEGACY9M_TXDIFF_TABLE0            0xD67C
#define REG_AX_PWR_LEGACY9M_TXDIFF_TABLE1            0xD680
#define REG_AX_PWR_LEGACY9M_TXDIFF_TABLE2            0xD684
#define REG_AX_PWR_LEGACY9M_TXDIFF_TABLE3            0xD688
#define REG_AX_PWR_LEGACY12M_TXDIFF_TABLE0           0xD68C
#define REG_AX_PWR_LEGACY12M_TXDIFF_TABLE1           0xD690
#define REG_AX_PWR_LEGACY12M_TXDIFF_TABLE2           0xD694
#define REG_AX_PWR_LEGACY12M_TXDIFF_TABLE3           0xD698
#define REG_AX_PWR_LEGACY18M_TXDIFF_TABLE0           0xD69C
#define REG_AX_PWR_LEGACY18M_TXDIFF_TABLE1           0xD6A0
#define REG_AX_PWR_LEGACY18M_TXDIFF_TABLE2           0xD6A4
#define REG_AX_PWR_LEGACY18M_TXDIFF_TABLE3           0xD6A8
#define REG_AX_PWR_LEGACY24M_TXDIFF_TABLE0           0xD6AC
#define REG_AX_PWR_LEGACY24M_TXDIFF_TABLE1           0xD6B0
#define REG_AX_PWR_LEGACY24M_TXDIFF_TABLE2           0xD6B4
#define REG_AX_PWR_LEGACY24M_TXDIFF_TABLE3           0xD6B8
#define REG_AX_PWR_LEGACY36M_TXDIFF_TABLE0           0xD6BC
#define REG_AX_PWR_LEGACY36M_TXDIFF_TABLE1           0xD6C0
#define REG_AX_PWR_LEGACY36M_TXDIFF_TABLE2           0xD6C4
#define REG_AX_PWR_LEGACY36M_TXDIFF_TABLE3           0xD6C8
#define REG_AX_PWR_LEGACY48M_TXDIFF_TABLE0           0xD6CC
#define REG_AX_PWR_LEGACY48M_TXDIFF_TABLE1           0xD6D0
#define REG_AX_PWR_LEGACY48M_TXDIFF_TABLE2           0xD6D4
#define REG_AX_PWR_LEGACY48M_TXDIFF_TABLE3           0xD6D8
#define REG_AX_PWR_LEGACY54M_TXDIFF_TABLE0           0xD6DC
#define REG_AX_PWR_LEGACY54M_TXDIFF_TABLE1           0xD6E0
#define REG_AX_PWR_LEGACY54M_TXDIFF_TABLE2           0xD6E4
#define REG_AX_PWR_LEGACY54M_TXDIFF_TABLE3           0xD6E8
#define REG_AX_PWR_TCM_WRAPPER_CR0                   0xD800
#define REG_AX_PWR_TCM_WRAPPER_CR1                   0xD804
#define REG_AX_PWR_TCM_WRAPPER_CR2                   0xD808
#define REG_AX_PWR_TCM_WRAPPER_CR3                   0xD80C
#define REG_AX_PWR_TCM_WRAPPER_CR4                   0xD810
#define REG_AX_PWR_TCM_WRAPPER_CR5                   0xD814
#define REG_AX_PWR_TCM_WRAPPER_CR6                   0xD818
#define REG_AX_PWR_TCM_WRAPPER_CR7                   0xD81C
#define REG_AX_PWR_TCM_WRAPPER_CR8                   0xD820
#define REG_AX_PWR_TCM_WRAPPER_CR9                   0xD824
#define REG_AX_PWR_TCM_WRAPPER_CR10                  0xD828
#define REG_AX_PWR_TCM_WRAPPER_CR11                  0xD82C
#define REG_AX_PWR_TCM_WRAPPER_CR12                  0xD830
#define REG_AX_PWR_TCM_WRAPPER_CR13                  0xD834
#define REG_AX_PWR_TCM_WRAPPER_CR14                  0xD838
#define REG_AX_PWR_TCM_WRAPPER_CR15                  0xD83C
#define REG_AX_PWR_TCM_WRAPPER_CR16                  0xD840
#define REG_AX_PWR_TCM_WRAPPER_CR17                  0xD844
#define REG_AX_PWR_TCM_WRAPPER_CR18                  0xD848
//#define REG_DUMMY                                    0xD900
#define REG_AX_PWR_TSSI_TABLE0                       0xD908
#define REG_AX_PWR_TSSI_TABLE1                       0xD90C
#define REG_AX_PWR_TSSI_TABLE2                       0xD910
#define REG_AX_PWR_TSSI_TABLE3                       0xD914
#define REG_AX_PWR_TSSI_TABLE4                       0xD918
#define REG_AX_PWR_TSSI_TABLE5                       0xD91C
#define REG_AX_PWR_TSSI_TABLE6                       0xD920
#define REG_AX_PWR_TSSI_TABLE7                       0xD924
#define REG_AX_PWR_TSSI_TABLE8                       0xD928
#define REG_AX_PWR_TSSI_TABLE9                       0xD92C
#define REG_AX_PWR_TSSI_TABLE10                      0xD930
#define REG_AX_PWR_TSSI_TABLE11                      0xD934
#define REG_AX_PWR_TSSI_TABLE12                      0xD938
#define REG_AX_PWR_TSSI_TABLE13                      0xD93C
#define REG_AX_PWR_TSSI_TABLE14                      0xD940
#define REG_AX_PWR_TSSI_TABLE15                      0xD944
#define REG_AX_PWR_TSSI_TABLE16                      0xD948
#define REG_AX_PWR_TSSI_TABLE17                      0xD94C
#define REG_AX_PWR_TSSI_TABLE18                      0xD950
#define REG_AX_PWR_TSSI_TABLE19                      0xD954
#define REG_AX_PWR_TSSI_TABLE20                      0xD958
#define REG_USR_LIST_SRAM                            0xDB00

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
