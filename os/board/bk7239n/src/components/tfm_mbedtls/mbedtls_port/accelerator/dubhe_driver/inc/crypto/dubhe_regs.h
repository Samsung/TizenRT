/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef __DBH_REGS_H__
#define __DBH_REGS_H__

#include <stdint.h>
#ifdef REG_DEBUG
#include "pal_log.h"
#endif

/* Top register map */
#define DBH_BASE_TOP_CTRL             0x0000UL
#define DBH_BASE_TOP_STAT             0x0100UL
#define DBH_BASE_SECURE_TRNG_CTRL     0x0200UL
#define DBH_BASE_SECURE_TRNG_POOL     0x0280UL
#define DBH_BASE_DBG_CTRL             0x0300UL
#define DBH_BASE_OTP_MGR              0x0400UL
#define DBH_BASE_OTP_SPACE            0x1000UL
#define DBH_BASE_SECURE_ACA           0x3000UL
#define DBH_BASE_SECURE_SCA           0x3200UL
#define DBH_BASE_SECURE_HASH          0x3280UL
#define DBH_BASE_NORMAL_ACA           0x3400UL
#define DBH_BASE_NORMAL_SCA           0x3600UL
#define DBH_BASE_NORMAL_HASH          0x3680UL
#define DBH_BASE_NORMAL_TRNG_POOL     0x3800UL

#if defined(DUBHE_SECURE)
#define DBH_BASE_ACA DBH_BASE_SECURE_ACA
#define DBH_BASE_SCA DBH_BASE_SECURE_SCA
#define DBH_BASE_HASH DBH_BASE_SECURE_HASH
#define DBH_BASE_TRNG_CTRL DBH_BASE_SECURE_TRNG_CTRL
#define DBH_BASE_TRNG_POOL DBH_BASE_SECURE_TRNG_POOL
#else
#define DBH_BASE_ACA DBH_BASE_NORMAL_ACA
#define DBH_BASE_SCA DBH_BASE_NORMAL_SCA
#define DBH_BASE_HASH DBH_BASE_NORMAL_HASH
#define DBH_BASE_TRNG_POOL DBH_BASE_NORMAL_TRNG_POOL
#endif

/* Top control: clock_ctrl register */
#define DBH_CLK_CTRL_REG_OFFSET 0x00UL
#define DBH_CLK_CTRL_HASH_EN_BIT_SHIFT      0x00UL
#define DBH_CLK_CTRL_HASH_EN_BIT_SIZE       0x01UL
#define DBH_CLK_CTRL_SCA_EN_BIT_SHIFT       0x01UL
#define DBH_CLK_CTRL_SCA_EN_BIT_SIZE        0x01UL
#define DBH_CLK_CTRL_ACA_EN_BIT_SHIFT       0x02UL
#define DBH_CLK_CTRL_ACA_EN_BIT_SIZE        0x01UL
#define DBH_CLK_CTRL_OTP_EN_BIT_SHIFT       0x03UL
#define DBH_CLK_CTRL_OTP_EN_BIT_SIZE        0x01UL
#define DBH_CLK_CTRL_TRNG_EN_BIT_SHIFT      0x04UL
#define DBH_CLK_CTRL_TRNG_EN_BIT_SIZE       0x01UL
#define DBH_CLK_CTRL_DMA_AHB_EN_BIT_SHIFT   0x05UL
#define DBH_CLK_CTRL_DMA_AHB_EN_BIT_SIZE    0x01UL

/* Top control: reset_ctrl register */
#define DBH_RESET_CTRL_REG_OFFSET   0x04UL
#define DBH_RESET_CTRL_HASH_BIT_SHIFT   0x00UL
#define DBH_RESET_CTRL_HASH_BIT_SIZE    0x01UL
#define DBH_RESET_CTRL_SCA_BIT_SHIFT    0x01UL
#define DBH_RESET_CTRL_SCA_BIT_SIZE     0x01UL
#define DBH_RESET_CTRL_ACA_BIT_SHIFT    0x02UL
#define DBH_RESET_CTRL_ACA_BIT_SIZE     0x01UL
#define DBH_RESET_CTRL_OTP_BIT_SHIFT    0x03UL
#define DBH_RESET_CTRL_OTP_BIT_SIZE     0x01UL
#define DBH_RESET_CTRL_TRNG_BIT_SHIFT   0x04UL
#define DBH_RESET_CTRL_TRNG_BIT_SIZE    0x01UL

/* Top control: top_intr_stat register */
#define DBH_TOP_CTRL_INTR_STAT_REG_OFFSET   0x34UL

/* Top control: top_intr_mask register */
#define DBH_TOP_CTRL_INTR_MASK_REG_OFFSET   0x38UL

/* Top status: dubhe_ver register */
#define DBH_VER_REG_OFFSET  0x00UL
#define DBH_VER_VALUE_BIT_SHIFT     0x00UL
#define DBH_VER_VALUE_BIT_SIZE      0x20UL

/* Top status: dubhe_conf1 */
#define DBH_CFG1_REG_OFFSET 0x04UL
#define DBG_CFG1_OTP_TEST_WORD_SIZE_BIT_SHIFT   0x00UL
#define DBG_CFG1_OTP_TEST_WORD_SIZE_BIT_SIZE    0x0BUL
#define DBG_CFG1_RN_POOL_EXT_HOST_SEC_BIT_SHIFT 0x0FUL
#define DBG_CFG1_RN_POOL_EXT_HOST_SEC_BIT_SIZE  0x01UL
#define DBG_CFG1_RN_POOL_HOST_NUM_BIT_SHIFT     0x10UL
#define DBG_CFG1_RN_POOL_HOST_NUM_BIT_SIZE      0x02UL
#define DBG_CFG1_TRNG_INTER_SRC_EXIST_BIT_SHIFT 0x12UL
#define DBG_CFG1_TRNG_INTER_SRC_EXIST_BIT_SIZE  0x01UL
#define DBG_CFG1_OTP_INIT_VALUE_BIT_SHIFT       0x13UL
#define DBG_CFG1_OTP_INIT_VALUE_BIT_SIZE        0x01UL
#define DBG_CFG1_OTP_EXIST_BIT_SHIFT            0x14UL
#define DBG_CFG1_OTP_EXIST_BIT_SIZE             0x01UL
#define DBG_CFG1_ACA_SRAM_SIZE_BIT_SHIFT        0x15UL
#define DBG_CFG1_ACA_SRAM_SIZE_BIT_SIZE         0x02UL
#define DBG_CFG1_ACA_EXT_HOST_SEC_BIT_SHIFT     0x17UL
#define DBG_CFG1_ACA_EXT_HOST_SEC_BIT_SIZE      0x01UL
#define DBG_CFG1_ACA_HOST_NUM_BIT_SHIFT         0x18UL
#define DBG_CFG1_ACA_HOST_NUM_BIT_SIZE          0x02UL
#define DBG_CFG1_HASH_EXT_HOST_SEC_BIT_SHIFT    0x1AUL
#define DBG_CFG1_HASH_EXT_HOST_SEC_BIT_SIZE     0x01UL
#define DBG_CFG1_HASH_HOST_NUM_BIT_SHIFT        0x1BUL
#define DBG_CFG1_HASH_HOST_NUM_BIT_SIZE         0x02UL
#define DBG_CFG1_SCA_EXT_HOST_SEC_BIT_SHIFT     0x1DUL
#define DBG_CFG1_SCA_EXT_HOST_SEC_BIT_SIZE      0x01UL
#define DBG_CFG1_SCA_HOST_NUM_BIT_SHIFT         0x1EUL
#define DBG_CFG1_SCA_HOST_NUM_BIT_SIZE          0x02UL

/* Top status: dubhe_conf2 */
#define DBH_CFG2_REG_OFFSET 0x08UL
#define DBG_CFG2_OTP_SEC_WORD_SIZE_BIT_SHIFT    0x00UL
#define DBG_CFG2_OTP_SEC_WORD_SIZE_BIT_SIZE     0x0BUL
#define DBG_CFG2_OTP_NSEC_WORD_SIZE_BIT_SHIFT   0x10UL
#define DBG_CFG2_OTP_NSEC_WORD_SIZE_BIT_SIZE    0x0BUL

/* Top status: dubhe_conf3 */
#define DBH_CFG3_REG_OFFSET 0x0CUL
#define DBG_CFG3_INTF_APB3_PAR_IS_ODD_BIT_SHIFT 0x00UL
#define DBG_CFG3_INTF_APB3_PAR_IS_ODD_BIT_SIZE  0x01UL
#define DBG_CFG3_INTF_APB4_PAR_IS_ODD_BIT_SHIFT 0x01UL
#define DBG_CFG3_INTF_APB4_PAR_IS_ODD_BIT_SIZE  0x01UL
#define DBG_CFG3_INTF_AHB_PAR_IS_ODD_BIT_SHIFT  0x02UL
#define DBG_CFG3_INTF_AHB_PAR_IS_ODD_BIT_SIZE   0x01UL
#define DBG_CFG3_INTF_APB3_PAR_ENA_BIT_SHIFT    0x03UL
#define DBG_CFG3_INTF_APB3_PAR_ENA_BIT_SIZE     0x01UL
#define DBG_CFG3_INTF_APB4_PAR_ENA_BIT_SHIFT    0x04UL
#define DBG_CFG3_INTF_APB4_PAR_ENA_BIT_SIZE     0x01UL
#define DBG_CFG3_INTF_AHB_PAR_ENA_BIT_SHIFT     0x05UL
#define DBG_CFG3_INTF_AHB_PAR_ENA_BIT_SIZE      0x01UL

/* Top status: dubhe_intr_stat_host0 register */
#define DBH_TOP_INTR_STAT_HOST0_REG_OFFSET  0x10UL
#define DBH_TOP_INTR_STAT_ACA0_BIT_SHIFT         0x00UL
#define DBH_TOP_INTR_STAT_ACA0_BIT_SIZE          0x01UL
#define DBH_TOP_INTR_STAT_HASH0_BIT_SHIFT        0x01UL
#define DBH_TOP_INTR_STAT_HASH0_BIT_SIZE         0x01UL
#define DBH_TOP_INTR_STAT_SCA0_BIT_SHIFT         0x02UL
#define DBH_TOP_INTR_STAT_SCA0_BIT_SIZE          0x01UL
#define DBH_TOP_INTR_STAT_RNPOOL0_BIT_SHIFT      0x03UL
#define DBH_TOP_INTR_STAT_RNPOOL0_BIT_SIZE       0x01UL
#define DBH_TOP_INTR_STAT_TRNG_BIT_SHIFT         0x04UL
#define DBH_TOP_INTR_STAT_TRNG_BIT_SIZE          0x01UL
#define DBH_TOP_RESERVE_BIT_SHIFT                0x05UL
#define DBH_TOP_RESERVE_BIT_SIZE                 0x03UL

/* Top status: dubhe_intr_stat_host1 register */
#define DBH_TOP_INTR_STAT_HOST1_REG_OFFSET  0x14UL
#define DBH_TOP_INTR_STAT_ACA1_BIT_SHIFT         0x00UL
#define DBH_TOP_INTR_STAT_ACA1_BIT_SIZE          0x01UL
#define DBH_TOP_INTR_STAT_HASH1_BIT_SHIFT        0x01UL
#define DBH_TOP_INTR_STAT_HASH1_BIT_SIZE         0x01UL
#define DBH_TOP_INTR_STAT_SCA1_BIT_SHIFT         0x02UL
#define DBH_TOP_INTR_STAT_SCA1_BIT_SIZE          0x01UL
#define DBH_TOP_INTR_STAT_RNPOOL1_BIT_SHIFT      0x03UL
#define DBH_TOP_INTR_STAT_RNPOOL1_BIT_SIZE       0x01UL

/* Debug control: debug_ctrl register */
#define DBH_DBG_CTRL_REG_OFFSET 0x00UL
#define DBH_DBG_CTRL_VALUE_BIT_SHIFT   0x00UL
#define DBH_DBG_CTRL_VALUE_BIT_SIZE    0x20UL

/* Debug control: debug_ctrl_lock register */
#define DBH_DBG_CTRL_LOCK_REG_OFFSET   0x04UL
#define DBH_DBG_CTRL_LOCK_VALUE_BIT_SHIFT   0x00UL
#define DBH_DBG_CTRL_LOCK_VALUE_BIT_SIZE    0x20UL

/* Debug control: debug_intr_mask register */
#define DBH_DBG_CTRL_INTR_MASK_REG_OFFSET 0x0CUL

/* SCA: sca_quequ register */
#define DBH_SCA_QUEUE_REG_OFFSET    0x00UL
#define DBH_SCA_QUEUE_ENTRY_BIT_SHIFT   0x00UL
#define DBH_SCA_QUEUE_ENTRY_BIT_SIZE    0x20UL

/* SCA: sca_ctrl register */
#define DBH_SCA_CTRL_REG_OFFSET 0x04UL
#define DBH_SCA_RUN_BIT_SHIFT           0x00UL
#define DBH_SCA_RUN_BIT_SIZE            0x01UL
#define DBH_SCA_QUEUE_WM_BIT_SHIFT      0x01UL
#define DBH_SCA_QUEUE_WM_BIT_SIZE       0x07UL
#define DBH_SCA_RESET_BIT_SHIFT         0x08UL
#define DBH_SCA_RESET_BIT_SIZE          0X01UL

/* SCA: sca_stat register */
#define DBH_SCA_STAT_REG_OFFSET 0x08UL
#define DBH_SCA_ENGINE_STAT_BIT_SHIFT       0x00UL
#define DBH_SCA_ENGINE_STAT_BIT_SIZE        0x02UL
#define DBH_SCA_QUEUE_AVAIL_SLOTS_BIT_SHIFT 0x02UL
#define DBH_SCA_QUEUE_AVAIL_SLOTS_BIT_SIZE  0x08UL

/* SCA: sca_intr_stat register */
#define DBH_SCA_INTR_STAT_REG_OFFSET    0x0CUL
#define DBH_SCA_CMD_INTR_BIT_SHIFT              0x00UL
#define DBH_SCA_CMD_INTR_BIT_SIZE               0x01UL
#define DBH_SCA_INVALID_CMD_BIT_SHIFT           0x01UL
#define DBH_SCA_INVALID_CMD_BIT_SIZE            0x01UL
#define DBH_SCA_INVALID_KEY_BIT_SHIFT           0x02UL
#define DBH_SCA_INVALID_KEY_BIT_SIZE            0x01UL
#define DBH_SCA_BUS_ERR_BIT_SHIFT               0x03UL
#define DBH_SCA_BUS_ERR_BIT_SIZE                0x01UL
#define DBH_SCA_WM_INTR_BIT_SHIFT               0x04UL
#define DBH_SCA_WM_INTR_BIT_SIZE                0x01UL

/* SCA: sca_intr_mask register */
#define DBH_SCA_INTR_MASK_REG_OFFSET    0x10UL
#define DBH_SCA_INTR_MASK_CMD_INTR_BIT_SHIFT            0x00UL
#define DBH_SCA_INTR_MASK_CMD_INTR_BIT_SIZE             0x01UL
#define DBH_SCA_INTR_MASK_INVALID_CMD_BIT_SHIFT         0x01UL
#define DBH_SCA_INTR_MASK_INVALID_CMD_BIT_SIZE          0x01UL
#define DBH_SCA_INTR_MASK_INVALID_KEY_BIT_SHIFT         0x02UL
#define DBH_SCA_INTR_MASK_INVALID_KEY_BIT_SIZE          0x01UL
#define DBH_SCA_INTR_MASK_BUS_ERR_BIT_SHIFT             0x03UL
#define DBH_SCA_INTR_MASK_BUS_ERR_BIT_SIZE              0x01UL
#define DBH_SCA_INTR_MASK_WM_INTR_BIT_SHIFT             0x04UL
#define DBH_SCA_INTR_MASK_WM_INTR_BIT_SIZE              0x01UL

/* SCA: sca_intr_mask register */
#define DBH_SCA_SUSPEND_MASK_REG_OFFSET                 0x14UL
#define DBH_SCA_SUSPEND_MASK_CMD_INTR_BIT_SHIFT         0x00UL
#define DBH_SCA_SUSPEND_MASK_CMD_INTR_BIT_SIZE          0x01UL
#define DBH_SCA_SUSPEND_MASK_INVALID_CMD_BIT_SHIFT      0x01UL
#define DBH_SCA_SUSPEND_MASK_INVALID_CMD_BIT_SIZE       0x01UL
#define DBH_SCA_SUSPEND_MASK_INVALID_KEY_BIT_SHIFT      0x02UL
#define DBH_SCA_SUSPEND_MASK_INVALID_KEY_BIT_SIZE       0x01UL
#define DBH_SCA_SUSPEND_MASK_BUS_ERR_BIT_SHIFT          0x03UL
#define DBH_SCA_SUSPEND_MASK_BUS_ERR_BIT_SIZE           0x01UL
#define DBH_SCA_SUSPEND_MASK_WM_INTR_BIT_SHIFT          0x04UL
#define DBH_SCA_SUSPEND_MASK_WM_INTR_BIT_SIZE           0x01UL

/* OTP manager: otp_setting register */
#define DBH_OTP_SET_REG_OFFSET  0x00UL
#define DBH_OTP_SET_INIT_DONE_BIT_SHIFT                 0x00UL
#define DBH_OTP_SET_INIT_DONE_BIT_SIZE                  0x01UL
#define DBH_OTP_SET_LCS_LOAD_FAIL_BIT_SHIFT             0x01UL
#define DBH_OTP_SET_LCS_LOAD_FAIL_BIT_SIZE              0x01UL
#define DBH_OTP_SET_MID_LOAD_FAIL_BIT_SHIFT             0x02UL
#define DBH_OTP_SET_MID_LOAD_FAIL_BIT_SIZE              0x01UL
#define DBH_OTP_SET_DEVID_LOAD_FAIL_BIT_SHIFT           0x03UL
#define DBH_OTP_SET_DEVID_LOAD_FAIL_BIT_SIZE            0x01UL
#define DBH_OTP_SET_KEY_LOAD_FAIL_BIT_SHIFT             0x04UL
#define DBH_OTP_SET_KEY_LOAD_FAIL_BIT_SIZE              0x01UL
#define DBH_OTP_SET_LOCK_LOAD_FAIL_BIT_SHIFT            0x05UL
#define DBH_OTP_SET_LOCK_LOAD_FAIL_BIT_SIZE             0x01UL
#define DBH_OTP_SET_DATA_INV_FAIL_BIT_SHIFT             0x06UL
#define DBH_OTP_SET_DATA_INV_FAIL_BIT_SIZE              0x01UL
#define DBH_OTP_SET_LCS_ERR_BIT_SHIFT                   0x07UL
#define DBH_OTP_SET_LCS_ERR_BIT_SIZE                    0x01UL
#define DBH_OTP_SET_DIRECT_RD_BIT_SHIFT                 0x08UL
#define DBH_OTP_SET_DIRECT_RD_BIT_SIZE                  0x01UL
#define DBH_OTP_SET_GENERAL_CTRL_BIT_SHIFT              0x09UL
#define DBH_OTP_SET_GENERAL_CTRL_BIT_SIZE               0x08UL
#define DBH_OTP_SET_GENERAL_CTRL_PTA_BIT_SHIFT          0x09UL
#define DBH_OTP_SET_GENERAL_CTRL_PTA_BIT_SIZE           0x02UL
#define DBH_OTP_SET_GENERAL_CTRL_PAPUF_BIT_SHIFT        0x0BUL
#define DBH_OTP_SET_GENERAL_CTRL_PAPUF_BIT_SIZE         0x01UL
#define DBH_OTP_SET_GENERAL_CTRL_PRESETN_BIT_SHIFT      0x0CUL
#define DBH_OTP_SET_GENERAL_CTRL_PRESETN_BIT_SIZE       0x01UL
#define DBH_OTP_SET_GENERAL_CTRL_PDSTB_BIT_SHIFT        0x0DUL
#define DBH_OTP_SET_GENERAL_CTRL_PDSTB_BIT_SIZE         0x01UL
#define DBH_OTP_SET_GENERAL_CTRL_PENVDD2_BIT_SHIFT      0x0EUL
#define DBH_OTP_SET_GENERAL_CTRL_PENVDD2_BIT_SIZE       0x01UL

/* OTP manager: otp_wr_addr register */
#define DBH_OTP_WR_ADDR_REG_OFFSET  0x04UL
#define DBH_OTP_WR_ADDR_VAL_BIT_SHIFT   0x02UL
#define DBH_OTP_WR_ADDR_VAL_BIT_SIZE    0x0BUL

/* OTP manager: otp_wr_data register */
#define DBH_OTP_WR_DATA_REG_OFFSET  0x08UL
#define DBH_OTP_WR_DATA_VAL_BIT_SHIFT   0x00UL
#define DBH_OTP_WR_DATA_VAL_BIT_SIZE    0x20UL

/* OTP manager: otp_wr register */
#define DBH_OTP_WR_REG_OFFSET   0x0CUL
#define DBH_OTP_WR_TRIG_BIT_SHIFT       0x00UL
#define DBH_OTP_WR_TRIG_BIT_SIZE        0x01UL

/* OTP manager: otp_update_stat register */
#define DBH_OTP_UPDATE_STAT_REG_OFFSET          0x10UL
#define DBH_OTP_UPDATE_STAT_BUSY_BIT_SHIFT      0x00UL
#define DBH_OTP_UPDATE_STAT_BUSY_BIT_SIZE       0x01UL
#define DBH_OTP_UPDATE_STAT_FAIL_BIT_SHIFT      0x01UL
#define DBH_OTP_UPDATE_STAT_FAIL_BIT_SIZE       0x01UL

/* OTP manager: otp_intr_mask register */
#define DBH_OTP_INTR_MASK_REG_OFFSET    0x18UL

/* HASH: hash_queue register */
#define DBH_HASH_QUEUE_REG_OFFSET 0x00UL
#define DBH_HASH_QUEUE_ENTRY_BIT_SHIFT  0x00UL
#define DBH_HASH_QUEUE_ENTRY_BIT_SIZE   0x20UL

/* HASH: hash_ctrl register */
#define DBH_HASH_CTRL_REG_OFFSET    0x04UL
#define DBH_HASH_RUN_BIT_SHIFT          0x00UL
#define DBH_HASH_RUN_BIT_SIZE           0x01UL
#define DBH_HASH_QUEUE_WM_BIT_SHIFT     0x01UL
#define DBH_HASH_QUEUE_WM_BIT_SIZE      0x07UL

/* HASH: hash_stat register */
#define DBH_HASH_STAT_REG_OFFSET    0x08UL
#define DBH_HASH_ENGINE_STAT_BIT_SHIFT              0x00UL
#define DBH_HASH_ENGINE_STAT_BIT_SIZE               0x02UL
#define DBH_HASH_ENGINE_QUEUE_AVAIL_SLOTS_BIT_SHIFT 0x02UL
#define DBH_HASH_ENGINE_QUEUE_AVAIL_SLOTS_BIT_SIZE  0x07UL

/* HASH: hash_intr_stat register */
#define DBH_HASH_INTR_STAT_REG_OFFSET   0x0CUL
#define DBH_HASH_INTR_STAT_CMD_INTR_BIT_SHIFT       0x00UL
#define DBH_HASH_INTR_STAT_CMD_INTR_BIT_SIZE        0x01UL
#define DBH_HASH_INTR_STAT_IVLIAD_CMD_BIT_SHIFT     0x01UL
#define DBH_HASH_INTR_STAT_IVLIAD_CMD_BIT_SHIFT     0x01UL
#define DBH_HASH_INTR_STAT_PADDING_ERR_BIT_SHIFT    0x02UL
#define DBH_HASH_INTR_STAT_PADDING_ERR_BIT_SIZE     0x01UL
#define DBH_HASH_INTR_STAT_BUS_ERR_BIT_SHIFT        0x03UL
#define DBH_HASH_INTR_STAT_BUS_ERR_BIT_SIZE         0x01UL
#define DBH_HASH_INTR_STAT_WM_INTR_BIT_SHIFT        0x04UL
#define DBH_HASH_INTR_STAT_WM_INTR_BIT_SIZE         0x01UL

/* HASH: hash_intr_mask register */
#define DBH_HASH_INTR_MASK_REG_OFFSET   0x10UL
#define DBH_HASH_INTR_MASK_CMD_INTR_BIT_SHIFT       0x00UL
#define DBH_HASH_INTR_MASK_CMD_INTR_BIT_SIZE        0x01UL
#define DBH_HASH_INTR_MASK_INVALID_CMD_BIT_SHIFT    0x01UL
#define DBH_HASH_INTR_MASK_INVALID_CMD_BIT_SIZE     0x01UL
#define DBH_HASH_INTR_MASK_PADDING_ERR_BIT_SHIFT    0x02UL
#define DBH_HASH_INTR_MASK_PADDING_ERR_BIT_SIZE     0x01UL
#define DBH_HASH_INTR_MASK_BUS_ERR_BIT_SHIFT        0x03UL
#define DBH_HASH_INTR_MASK_BUS_ERR_BIT_SIZE         0x01UL
#define DBH_HASH_INTR_MASK_WM_INTR_BIT_SHIFT        0x04UL
#define DBH_HASH_INTR_MASK_WM_INTR_BIT_SIZE         0x01UL

/* HASH: hash_suspend_mask register */
#define DBH_HASH_SUSPEND_MASK_REG_OFFSET    0x14UL
#define DBH_HASH_SUSPEND_MASK_CMD_INTR_BIT_SHIFT    0x00UL
#define DBH_HASH_SUSPEND_MASK_CMD_INTR_BIT_SIZE     0x01UL
#define DBH_HASH_SUSPEND_MASK_INVALID_CMD_BIT_SHIFT 0x01UL
#define DBH_HASH_SUSPEND_MASK_INVALID_CMD_BIT_SIZE  0x01UL
#define DBH_HASH_SUSPEND_MASK_PADDING_ERR_BIT_SHIFT 0x02UL
#define DBH_HASH_SUSPEND_MASK_PADDING_ERR_BIT_SIZE  0x01UL
#define DBH_HASH_SUSPEND_MASK_BUS_ERR_BIT_SHIFT     0x03UL
#define DBH_HASH_SUSPEND_MASK_BUS_ERR_BIT_SIZE      0x01UL
#define DBH_HASH_SUSPEND_MASK_WM_INTR_BIT_SHIFT     0x04UL
#define DBH_HASH_SUSPEND_MASK_WM_INTR_BIT_SIZE      0x01UL

/* TRNG CTRL: entropy_src register */
#define DBH_TRNG_ENTROPY_SRC_REG_OFFSET         0x04UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN0_EN_BIT_SHIFT        0x00UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN0_EN_BIT_SIZE         0x01UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN1_EN_BIT_SHIFT        0x01UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN1_EN_BIT_SIZE         0x01UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN2_EN_BIT_SHIFT        0x02UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN2_EN_BIT_SIZE         0x01UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN3_EN_BIT_SHIFT        0x03UL
#define DBH_TRNG_ENTROPY_SRC_CHAIN3_EN_BIT_SIZE         0x01UL
#define DBH_TRNG_ENTROPY_SRC_SELECT_BIT_SHIFT           0x04UL
#define DBH_TRNG_ENTROPY_SRC_SELECT_BIT_SIZE            0x01UL

/* TRNG CTRL: entropy_sample register */
#define DBH_TRNG_ENTROPY_SAMPLE_REG_OFFSET      0x08UL
#define DBH_TRNG_ENTROPY_SAMPLE_RATE_DIV_BIT_SHIFT      0x00UL
#define DBH_TRNG_ENTROPY_SAMPLE_RATE_DIV_BIT_SIZE       0x10UL
#define DBH_TRNG_ENTROPY_SAMPLE_DELAY_BIT_SHIFT         0x10UL
#define DBH_TRNG_ENTROPY_SAMPLE_DELAY_BIT_SIZE          0x08UL

/* TRNG CTRL: ro_setting register */
#define DBH_TRNG_RO_SETTING_REG_OFFSET          0x0CUL
#define DBH_TRNG_RO_SETTING_GROUP0_RO_TAPS_EN_BIT_SHIFT 0x00UL
#define DBH_TRNG_RO_SETTING_GROUP0_RO_TAPS_EN_BIT_SIZE  0X03UL
#define DBH_TRNG_RO_SETTING_GROUP1_RO_TAPS_EN_BIT_SHIFT 0x04UL
#define DBH_TRNG_RO_SETTING_GROUP1_RO_TAPS_EN_BIT_SIZE  0X03UL
#define DBH_TRNG_RO_SETTING_GROUP2_RO_TAPS_EN_BIT_SHIFT 0x08UL
#define DBH_TRNG_RO_SETTING_GROUP2_RO_TAPS_EN_BIT_SIZE  0X03UL
#define DBH_TRNG_RO_SETTING_GROUP3_RO_TAPS_EN_BIT_SHIFT 0x0CUL
#define DBH_TRNG_RO_SETTING_GROUP3_RO_TAPS_EN_BIT_SIZE  0X03UL

/* TRNG CTRL: post_process register */
#define DBH_TRNG_POST_PROCESS_REG_OFFSET        0x10UL
#define DBH_TRNG_POST_PROCESS_PRNG_BYPASS_BIT_SHIFT     0x00UL
#define DBH_TRNG_POST_PROCESS_PRNG_BYPASS_BIT_SIZE      0x01UL
#define DBH_TRNG_POST_PROCESS_VN_BYPASS_BIT_SHIFT       0x01UL
#define DBH_TRNG_POST_PROCESS_VN_BYPASS_BIT_SIZE        0x01UL
#define DBH_TRNG_POST_PROCESS_CRNG_BYPASS_BIT_SHIFT     0x02UL
#define DBH_TRNG_POST_PROCESS_CRNG_BYPASS_BIT_SIZE      0x01UL

/* TRNG CTRL: evaluation_setting register */
#define DBH_TRNG_EVAL_REG_OFFSET                0x14UL
#define DBH_TRNG_EVAL_ADP_TEST_TH_BIT_SHIFT             0x00UL
#define DBH_TRNG_EVAL_ADP_TEST_TH_BIT_SIZE              0x0AUL
#define DBH_TRNG_EVAL_REP_TEST_TH_BIT_SHIFT             0x0AUL
#define DBH_TRNG_EVAL_REP_TEST_TH_BIT_SIZE              0x06UL
#define DBH_TRNG_EVAL_ADP_TEST_EN_BIT_SHIFT             0x10UL
#define DBH_TRNG_EVAL_ADP_TEST_EN_BIT_SIZE              0x01UL
#define DBH_TRNG_EVAL_REP_TEST_EN_BIT_SHIFT             0x11UL
#define DBH_TRNG_EVAL_REP_TEST_EN_BIT_SIZE              0x01UL
#define DBH_TRNG_EVAL_AUTO_CORR_TEST_EN_BIT_SHIFT       0x12UL
#define DBH_TRNG_EVAL_AUTO_CORR_TEST_EN_BIT_SIZE        0x01UL
#define DBH_TRNG_EVAL_AUTO_CORR_TEST_TH_BIT_SHIFT       0x13UL
#define DBH_TRNG_EVAL_AUTO_CORR_TEST_TH_BIT_SIZE        0x04UL

/* TRNG CTRL: trng_intr_stat register */
#define DBH_TRNG_INTR_REG_OFFSET                0x1CUL
#define DBH_TRNG_INTR_VN_ERR_BIT_SHIFT                  0x00UL
#define DBH_TRNG_INTR_VN_ERR_BIT_SIZE                   0x01UL
#define DBH_TRNG_INTR_CRNG_ERR_BIT_SHIFT                0x01UL
#define DBH_TRNG_INTR_CRNG_ERR_BIT_SIZE                 0x01UL
#define DBH_TRNG_INTR_REP_TEST_ERR_BIT_SHIFT            0x02UL
#define DBH_TRNG_INTR_REP_TEST_ERR_BIT_SIZE             0x01UL
#define DBH_TRNG_INTR_ADP_TEST_ERR_BIT_SHIFT            0x03UL
#define DBH_TRNG_INTR_ADP_TEST_ERR_BIT_SIZE             0x01UL

/* TRNG CTRL: trng_intr_mask register */
#define DBH_TRNG_INTR_MASK_REG_OFFSET           0x20UL
#define DBH_TRNG_INTR_MASK_VN_ERR_BIT_SHIFT             0x00UL
#define DBH_TRNG_INTR_MASK_VN_ERR_BIT_SIZE              0x01UL
#define DBH_TRNG_INTR_MASK_CRNG_ERR_BIT_SHIFT           0x01UL
#define DBH_TRNG_INTR_MASK_CRNG_ERR_BIT_SIZE            0x01UL
#define DBH_TRNG_INTR_MASK_REP_TEST_ERR_BIT_SHIFT       0x02UL
#define DBH_TRNG_INTR_MASK_REP_TEST_ERR_BIT_SIZE        0x01UL
#define DBH_TRNG_INTR_MASK_ADP_TEST_ERR_BIT_SHIFT       0x03UL
#define DBH_TRNG_INTR_MASK_ADP_TEST_ERR_BIT_SIZE        0x01UL

/* TRNG CTRL: err_cnt_threshold register */
#define DBH_TRNG_ERR_CNT_TH_REG_OFFSET          0x24UL
#define DBH_TRNG_ERR_CNT_TH_VN_ERR_BIT_SHIFT            0x00UL
#define DBH_TRNG_ERR_CNT_TH_VN_ERR_BIT_SIZE             0x04UL
#define DBH_TRNG_ERR_CNT_TH_CRNG_ERR_BIT_SHIFT          0x08UL
#define DBH_TRNG_ERR_CNT_TH_CRNG_ERR_BIT_SIZE           0x08UL
#define DBH_TRNG_ERR_CNT_TH_REP_ERR_BIT_SHIFT           0x10UL
#define DBH_TRNG_ERR_CNT_TH_REP_ERR_BIT_SIZE            0x08UL
#define DBH_TRNG_ERR_CNT_TH_ADAP_ERR_BIT_SHIFT          0x18UL
#define DBH_TRNG_ERR_CNT_TH_ADAP_ERR_BIT_SIZE           0x08UL

/* TRNG CTRL: xxx_test_total_cnt register */
#define DBH_TRNG_VN_TEST_TOTAL_CNT_REG_OFFSET   0x28UL
#define DBH_TRNG_CRNG_TEST_TOTAL_CNT_REG_OFFSET 0x2CUL
#define DBH_TRNG_REP_TEST_TOTAL_CNT_REG_OFFSET  0x30UL
#define DBH_TRNG_ADAP_TEST_TOTAL_CNT_REG_OFFSET 0x34UL

/* TRNG Pool: rn_pool_ctrl register */
#define DBH_TRNG_POOL_CTRL_REG_OFFSET           0x00UL
#define DBH_TRNG_POOL_CTRL_FILL_REQ_BIT_SHIFT           0x00UL
#define DBH_TRNG_POOL_CTRL_FILL_REQ_BIT_SIZE            0x01UL
#define DBH_TRNG_POOL_CTRL_CLOCK_ENABLE_BIT_SHIFT       0x01UL
#define DBH_TRNG_POOL_CTRL_CLOCK_ENABLE_BIT_SIZE        0x01UL

/* TRNG Pool: rn_pool_stat register */
#define DBH_TRNG_POOL_STAT_REG_OFFSET           0x04UL
#define DBH_TRNG_POOL_STAT_AUTO_CORR_TEST_ERR_BIT_SHIFT 0x00UL
#define DBH_TRNG_POOL_STAT_AUTO_CORR_TEST_ERR_BIT_SIZE  0x01UL
#define DBH_TRNG_POOL_STAT_TRNG_CORE_STAT_BIT_SHIFT     0x01UL
#define DBH_TRNG_POOL_STAT_TRNG_CORE_STAT_BIT_SIZE      0x02UL

/* TRNG Pool: rn_pool_intr register */
#define DBH_TRNG_POOL_INTR_REG_OFFSET           0x08UL
#define DBH_TRNG_POOL_INTR_FILL_DONE_BIT_SHIFT          0x00UL
#define DBH_TRNG_POOL_INTR_FILL_DONE_BIT_SIZE           0x01UL

/* TRNG Pool: rn_pool_intr_mask register */
#define DBH_TRNG_POOL_INTR_MASK_REG_OFFSET      0x0CUL
#define DBH_TRNG_POOL_INTR_MASK_FILL_DONE_BIT_SHIFT    0x00UL
#define DBH_TRNG_POOL_INTR_MASK_FILL_DONE_BIT_SIZE     0x01UL

/* TRNG Pool: rn_pool_data_wordx register */
#define DBH_TRNG_POOL_DATA_WORD_REG_OFFSET      0x10UL
#define DBH_TRNG_POOL_DATA_WORD_REG_BYTE_SIZE   0x04UL

/* ACA: grx_sram_addr register */
#define DBH_SRAM_MAP0_REG_OFFSET        0x000UL
#define DBH_SRAM_MAP0_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP0_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP1_REG_OFFSET        0x004UL
#define DBH_SRAM_MAP1_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP1_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP2_REG_OFFSET        0x008UL
#define DBH_SRAM_MAP2_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP2_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP3_REG_OFFSET        0x00CUL
#define DBH_SRAM_MAP3_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP3_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP4_REG_OFFSET        0x010UL
#define DBH_SRAM_MAP4_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP4_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP5_REG_OFFSET        0x014UL
#define DBH_SRAM_MAP5_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP5_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP6_REG_OFFSET        0x018UL
#define DBH_SRAM_MAP6_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP6_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP7_REG_OFFSET        0x01CUL
#define DBH_SRAM_MAP7_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP7_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP8_REG_OFFSET        0x020UL
#define DBH_SRAM_MAP8_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP8_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP9_REG_OFFSET        0x024UL
#define DBH_SRAM_MAP9_VALUE_BIT_SHIFT   0x0UL
#define DBH_SRAM_MAP9_VALUE_BIT_SIZE    0xCUL
#define DBH_SRAM_MAP10_REG_OFFSET       0x028UL
#define DBH_SRAM_MAP10_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP10_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP11_REG_OFFSET       0x02CUL
#define DBH_SRAM_MAP11_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP11_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP12_REG_OFFSET       0x030UL
#define DBH_SRAM_MAP12_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP12_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP13_REG_OFFSET       0x034UL
#define DBH_SRAM_MAP13_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP13_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP14_REG_OFFSET       0x038UL
#define DBH_SRAM_MAP14_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP14_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP15_REG_OFFSET       0x03CUL
#define DBH_SRAM_MAP15_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP15_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP16_REG_OFFSET       0x040UL
#define DBH_SRAM_MAP16_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP16_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP17_REG_OFFSET       0x044UL
#define DBH_SRAM_MAP17_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP17_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP18_REG_OFFSET       0x048UL
#define DBH_SRAM_MAP18_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP18_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP19_REG_OFFSET       0x04CUL
#define DBH_SRAM_MAP19_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP19_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP20_REG_OFFSET       0x050UL
#define DBH_SRAM_MAP20_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP20_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP21_REG_OFFSET       0x054UL
#define DBH_SRAM_MAP21_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP21_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP22_REG_OFFSET       0x058UL
#define DBH_SRAM_MAP22_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP22_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP23_REG_OFFSET       0x05CUL
#define DBH_SRAM_MAP23_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP23_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP24_REG_OFFSET       0x060UL
#define DBH_SRAM_MAP24_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP24_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP25_REG_OFFSET       0x064UL
#define DBH_SRAM_MAP25_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP25_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP26_REG_OFFSET       0x068UL
#define DBH_SRAM_MAP26_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP26_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP27_REG_OFFSET       0x06CUL
#define DBH_SRAM_MAP27_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP27_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP28_REG_OFFSET       0x070UL
#define DBH_SRAM_MAP28_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP28_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP29_REG_OFFSET       0x074UL
#define DBH_SRAM_MAP29_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP29_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP30_REG_OFFSET       0x078UL
#define DBH_SRAM_MAP30_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP30_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_MAP31_REG_OFFSET       0x07CUL
#define DBH_SRAM_MAP31_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_MAP31_VALUE_BIT_SIZE   0xCUL

/* ACA: gr_len_typex register */
#define DBH_LEN_TYPE0_REG_OFFSET        0x100UL
#define DBH_LEN_TYPE0_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE0_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE1_REG_OFFSET        0x104UL
#define DBH_LEN_TYPE1_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE1_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE2_REG_OFFSET        0x108UL
#define DBH_LEN_TYPE2_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE2_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE3_REG_OFFSET        0x10CUL
#define DBH_LEN_TYPE3_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE3_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE4_REG_OFFSET        0x1100UL
#define DBH_LEN_TYPE4_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE4_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE5_REG_OFFSET        0x114UL
#define DBH_LEN_TYPE5_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE5_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE6_REG_OFFSET        0x118UL
#define DBH_LEN_TYPE6_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE6_VALUE_BIT_SIZE    0xDUL
#define DBH_LEN_TYPE7_REG_OFFSET        0x11CUL
#define DBH_LEN_TYPE7_VALUE_BIT_SHIFT   0x0UL
#define DBH_LEN_TYPE7_VALUE_BIT_SIZE    0xDUL

/* ACA: n_p_t0_t1_use_grid register */
#define DBH_N_NP_T0_T1_ADDR_REG_OFFSET  0x120UL
#define DBH_N_NP_T0_T1_ADDR_N_VIRTUAL_ADDR_BIT_SHIFT    0x0UL
#define DBH_N_NP_T0_T1_ADDR_N_VIRTUAL_ADDR_BIT_SIZE     0x5UL
#define DBH_N_NP_T0_T1_ADDR_NP_VIRTUAL_ADDR_BIT_SHIFT   0x5UL
#define DBH_N_NP_T0_T1_ADDR_NP_VIRTUAL_ADDR_BIT_SIZE    0x5UL
#define DBH_N_NP_T0_T1_ADDR_T0_VIRTUAL_ADDR_BIT_SHIFT   0xAUL
#define DBH_N_NP_T0_T1_ADDR_T0_VIRTUAL_ADDR_BIT_SIZE    0x5UL
#define DBH_N_NP_T0_T1_ADDR_T1_VIRTUAL_ADDR_BIT_SHIFT   0xFUL
#define DBH_N_NP_T0_T1_ADDR_T1_VIRTUAL_ADDR_BIT_SIZE    0x5UL

/* ACA: op_cfg register */
#define DBH_OP_CFG_REG_OFFSET  0x124UL
#define DBH_OP_CFG_RUN_BIT_SHIFT        0x0UL
#define DBH_OP_CFG_RUN_BIT_SIZE         0x1UL
#define DBH_OP_CFG_RESERVE_BIT_SHIFT    0x1UL
#define DBH_OP_CFG_RESERVE_BIT_SIZE     0x3UL
#define DBH_OP_CFG_FIFO_WM_BIT_SHIFT    0x4UL
#define DBH_OP_CFG_FIFO_WM_BIT_SIZE     0x4UL
#define DBH_OP_CFG_FIFO_RST_BIT_SHIFT   0x8UL
#define DBH_OP_CFG_FIFO_RST_BIT_SIZE    0x1UL

/* ACA: entry register */
#define DBH_OPCODE_ENTRY_REG_OFFSET     0x128UL
#define DBH_OPCODE_ENTRY_NEED_INTR_BIT_SHIFT    0x0UL
#define DBH_OPCODE_ENTRY_NEED_INTR_BIT_SIZE     0x1UL
#define DBH_OPCODE_ENTRY_REG_C_BIT_SHIFT        0x1UL
#define DBH_OPCODE_ENTRY_REG_C_BIT_SIZE         0x5UL
#define DBH_OPCODE_ENTRY_R_DISCARD_BIT_SHIFT    0x6UL
#define DBH_OPCODE_ENTRY_R_DISCARD_BIT_SIZE     0x1UL
#define DBH_OPCODE_ENTRY_REG_R_BIT_SHIFT        0x7UL
#define DBH_OPCODE_ENTRY_REG_R_BIT_SIZE         0x5UL
#define DBH_OPCODE_ENTRY_REG_B_BIT_SHIFT        0xCUL
#define DBH_OPCODE_ENTRY_REG_B_BIT_SIZE         0x5UL
#define DBH_OPCODE_ENTRY_REG_B_IM_BIT_SHIFT     0x11UL
#define DBH_OPCODE_ENTRY_REG_B_IM_BIT_SIZE      0x1UL
#define DBH_OPCODE_ENTRY_REG_A_BIT_SHIFT        0x12UL
#define DBH_OPCODE_ENTRY_REG_A_BIT_SIZE         0x5UL
#define DBH_OPCODE_ENTRY_RESERVE_BIT_SHIFT      0x17UL
#define DBH_OPCODE_ENTRY_RESERVE_BIT_SIZE       0x1UL
#define DBH_OPCODE_ENTRY_LEN_BIT_SHIFT          0x18UL
#define DBH_OPCODE_ENTRY_LEN_BIT_SIZE           0x3UL
#define DBH_OPCODE_ENTRY_OPCODE_BIT_SHIFT       0x1BUL
#define DBH_OPCODE_ENTRY_OPCODE_BIT_SIZE        0x5UL

/* ACA: aca_status register */
#define DBH_ACA_STAT_REG_OFFSET         0x12CUL
#define DBH_ACA_STAT_FIFO_FREE_NUM_BIT_SHIFT    0x0UL
#define DBH_ACA_STAT_FIFO_FREE_NUM_BIT_SIZE     0x4UL
#define DBH_ACA_STAT_FIFO_EMPTY_BIT_SHIFT       0x4UL
#define DBH_ACA_STAT_FIFO_EMPTY_BIT_SIZE        0x1UL
#define DBH_ACA_STAT_FIFO_FULL_BIT_SHIFT        0x5UL
#define DBH_ACA_STAT_FIFO_FULL_BIT_SIZE         0x1UL
#define DBH_ACA_STAT_ADD_RESULT_ZERO_BIT_SHIFT  0x6UL
#define DBH_ACA_STAT_ADD_RESULT_ZERO_BIT_SIZE   0x1UL
#define DBH_ACA_STAT_AND_RESULT_ZERO_BIT_SHIFT  0x7UL
#define DBH_ACA_STAT_AND_RESULT_ZERO_BIT_SIZE   0x1UL
#define DBH_ACA_STAT_XOR_RESULT_ZERO_BIT_SHIFT  0x8UL
#define DBH_ACA_STAT_XOR_RESULT_ZERO_BIT_SIZE   0x1UL
#define DBH_ACA_STAT_ALU_CARRY_BIT_SHIFT        0x9UL
#define DBH_ACA_STAT_ALU_CARRY_BIT_SIZE         0x1UL
/*The times of subtraction as do modular reduction.
  As > 32, it will keep as 31. */
#define DBH_ACA_STAT_REDUCTION_TIMES_BIT_SHIFT  0xAUL
#define DBH_ACA_STAT_REDUCTION_TIMES_BIT_SIZE   0x6UL
/*
ACA engine status
0x0->idle
0x1-> busy run op command by other host
0x2-> busy run op command by this host self
0x3-> reserve
*/
#define DBH_ACA_STAT_ENGIN_STAT_BIT_SHIFT       0x10UL
#define DBH_ACA_STAT_ENGIN_STAT_BIT_SIZE        0x2UL
/*
0x0: indicate 4 k bytes SRAM.
0x1: indicate 8 k bytes SRAM.
0x2: indicate 16 k bytes SRAM.
0x3: reserved.
*/
#define DBH_ACA_STAT_SRAM_SIZE_MODE_BIT_SHIFT   0x12UL
#define DBH_ACA_STAT_SRAM_SIZE_MODE_BIT_SIZE    0x2UL




/*currently, Duhbe HW can support max sram size 16KBytes,
 *so 12bits is enough for sram address
*/
/* ACA: sram_xxx register */
#define DBH_SRAM_WADDR_REG_OFFSET       0x130UL
#define DBH_SRAM_WADDR_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_WADDR_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_WDATA_REG_OFFSET       0x134UL
#define DBH_SRAM_WDATA_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_WDATA_VALUE_BIT_SIZE   0x20UL
#define DBH_SRAM_RADDR_REG_OFFSET       0x138UL
#define DBH_SRAM_RADDR_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_RADDR_VALUE_BIT_SIZE   0xCUL
#define DBH_SRAM_RDATA_REG_OFFSET       0x13CUL
#define DBH_SRAM_RDATA_VALUE_BIT_SHIFT  0x0UL
#define DBH_SRAM_RDATA_VALUE_BIT_SIZE   0x20UL

/* ACA: aca_intr_stat register */
#define DBH_ACA_INTR_STAT_REG_OFFSET    0x140UL
#define DBH_ACA_INTR_STAT_CMD_FINISH_BIT_SHIFT      0x0UL
#define DBH_ACA_INTR_STAT_CMD_FINISH_BIT_SIZE       0x1UL
#define DBH_ACA_INTR_STAT_FIFO_UNDER_WM_BIT_SHIFT   0x1UL
#define DBH_ACA_INTR_STAT_FIFO_UNDER_WM_BIT_SIZE    0x1UL
#define DBH_ACA_INTR_STAT_FIFO_OVERFLOW_BIT_SHIFT   0x2UL
#define DBH_ACA_INTR_STAT_FIFO_OVERFLOW_BIT_SIZE    0x1UL
#define DBH_ACA_INTR_STAT_DIV_ZERO_BIT_SHIFT        0x3UL
#define DBH_ACA_INTR_STAT_DIV_ZERO_BIT_SIZE         0x1UL
#define DBH_ACA_INTR_STAT_MODINV_ZERO_BIT_SHIFT     0x4UL
#define DBH_ACA_INTR_STAT_MODINV_ZERO_BIT_SIZE      0x1UL
#define DBH_ACA_INTR_STAT_ACA_DONE_BIT_SHIFT        0x5UL
#define DBH_ACA_INTR_STAT_ACA_DONE_BIT_SIZE         0x1UL
#define DBH_ACA_INTR_STAT_MULT_RED_ERR_BIT_SHIFT    0x6UL
#define DBH_ACA_INTR_STAT_MULT_RED_ERR_BIT_SIZE     0x1UL

/* ACA: aca_intr_mask register */
#define DBH_ACA_INTR_MASK_REG_OFFSET    0x144UL
#define DBH_ACA_INTR_MASK_CMD_FINISH_BIT_SHIFT      0x0UL
#define DBH_ACA_INTR_MASK_CMD_FINISH_BIT_SIZE       0x1UL
#define DBH_ACA_INTR_MASK_FIFO_UNDER_WM_BIT_SHIFT   0x1UL
#define DBH_ACA_INTR_MASK_FIFO_UNDER_WM_BIT_SIZE    0x1UL
#define DBH_ACA_INTR_MASK_FIFO_OVERFLOW_BIT_SHIFT   0x2UL
#define DBH_ACA_INTR_MASK_FIFO_OVERFLOW_BIT_SIZE    0x1UL
#define DBH_ACA_INTR_MASK_DIV_ZERO_BIT_SHIFT        0x3UL
#define DBH_ACA_INTR_MASK_DIV_ZERO_BIT_SIZE         0x1UL
#define DBH_ACA_INTR_MASK_MODINV_ZERO_BIT_SHIFT     0x4UL
#define DBH_ACA_INTR_MASK_MODINV_ZERO_BIT_SIZE      0x1UL
#define DBH_ACA_INTR_MASK_ACA_DONE_BIT_SHIFT        0x5UL
#define DBH_ACA_INTR_MASK_ACA_DONE_BIT_SIZE         0x1UL
#define DBH_ACA_INTR_MASK_MULT_RED_ERR_BIT_SHIFT    0x6UL
#define DBH_ACA_INTR_MASK_MULT_RED_ERR_BIT_SIZE     0x1UL

/* ACA: aca_suspend_mask register */
#define DBH_ACA_SUSPEND_MASK_REG_OFFSET 0x148UL
#define DBH_ACA_SUSPEND_MASK_CMD_FINISH_BIT_SHIFT    0x0UL
#define DBH_ACA_SUSPEND_MASK_CMD_FINISH_BIT_SIZE     0x1UL
#define DBH_ACA_SUSPEND_MASK_RESERVE_BIT_SHIFT       0x1UL
#define DBH_ACA_SUSPEND_MASK_RESERVE_BIT_SIZE        0x1UL
#define DBH_ACA_SUSPEND_MASK_FIFO_OVERFLOW_BIT_SHIFT 0x2UL
#define DBH_ACA_SUSPEND_MASK_FIFO_OVERFLOW_BIT_SIZE  0x1UL
#define DBH_ACA_SUSPEND_MASK_DIV_ZERO_BIT_SHIFT      0x3UL
#define DBH_ACA_SUSPEND_MASK_DIV_ZERO_BIT_SIZE       0x1UL
#define DBH_ACA_SUSPEND_MASK_MODINV_ZERO_BIT_SHIFT   0x4UL
#define DBH_ACA_SUSPEND_MASK_MODINV_ZERO_BIT_SIZE    0x1UL

/*Bit fields operations macros.*/
#define BITMASK(mask_size) (((mask_size) < 32) ?        \
                            ((1UL << (mask_size)) - 1) : 0xFFFFFFFFUL)
#define BITMASK_AT(mask_size, mask_offset) (BITMASK(mask_size) << (mask_offset))

#define BITFIELD_GET(word, bit_offset, bit_size) \
    (((word) >> (bit_offset)) & BITMASK(bit_size))
#define BITFIELD_SET(word, bit_offset, bit_size, new_val)   do {    \
        word = ((word) & ~BITMASK_AT(bit_size, bit_offset)) |       \
               (((new_val) & BITMASK(bit_size)) << (bit_offset));  \
} while (0)

/*! Bit fields get */
#define DBH_REG_FLD_GET(reg_name, fld_name, reg_val)        \
    (DBH_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20 ?              \
     reg_val /*!< \internal Optimization for 32b fields */ :               \
     BITFIELD_GET(reg_val, DBH_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT, \
                  DBH_ ## reg_name ## _ ## fld_name ## _BIT_SIZE))


/*! Bit fields set */
#define DBH_REG_FLD_SET(reg_name, fld_name, reg_shadow_var, new_fld_val)      \
    do {                                                                     \
        if (DBH_ ## reg_name ## _ ## fld_name ## _BIT_SIZE == 0x20) {       \
            reg_shadow_var = new_fld_val;     /*!< \internal Optimization for
                                               * 32b fields */ \
        } \
        else{                                                             \
            BITFIELD_SET(reg_shadow_var,                             \
                         DBH_ ## reg_name ## _ ## fld_name ## _BIT_SHIFT,  \
                         DBH_ ## reg_name ## _ ## fld_name ## _BIT_SIZE,   \
                         new_fld_val);}                                    \
    } while (0)

/* Usage example:
   uint32_t reg_shadow = DBH_READ_REGISTER(DBH_REG_OFFSET(ACA,OP_CFG));
   DBH_REG_FLD_SET(OP_CFG,RUN,reg_shadow, 1);
   DBH_REG_FLD_SET(OP_CFG,FIFO_WM,reg_shadow, 5);
   DBH_WRITE_REGISTER(DBH_REG_OFFSET(ACA,OP_CFG), reg_shadow);
 */



/*the baseaddress will defined in HAL layer*/
extern uint32_t    _g_Dubhe_RegBase;

#ifdef REG_DEBUG
extern const char* reg_array[10][10];
extern const char* aca_other[20];

#define PRINTF_REG_INFO(unit, reg, val) \
    do { \
        uint32_t reg_offset = unit + reg; \
        switch ( unit ) \
        { \
        case DBH_BASE_TOP_CTRL: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x  val: 0x%08x\n", \
                   reg_array[0][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_TOP_STAT: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x  val: 0x%08x\n", \
                   reg_array[1][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_DBG_CTRL: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x  val: 0x%08x\n", \
                   reg_array[2][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_TRNG_CTRL: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   reg_array[3][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_TRNG_POOL: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   reg_array[4][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_OTP_MGR: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   reg_array[5][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_SECURE_HASH: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   reg_array[6][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_SECURE_SCA: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   reg_array[7][reg / 4], (uint32_t)reg_offset, (uint32_t)val); \
            break; \
        case DBH_BASE_ACA: \
            PAL_LOG_DEBUG("###WR %-30s addr: 0x%08x, val: 0x%08x\n", \
                   aca_other[(reg - 0x120) / 4], \
                   (uint32_t)reg_offset, \
                   (uint32_t)val); \
            break; \
        default: \
            break; \
        } \
    } while ( 0 )

#if 0

#endif

#define PRINTF_ACA_SRAM_REG(reg_offset, val) \
    do { \
        uint32_t reg_val = ((reg_offset) - DBH_BASE_ACA); \
        if (reg_val < 0x100) { \
            PAL_LOG_DEBUG("###WR (DBH_SRAM_MAP%u_REG) addr: 0x%08x, val: 0x%08x\n", \
                   (unsigned int)reg_val / 4, \
                   (unsigned int)reg_offset, \
                   (unsigned int)val);} \
        else{ \
            PAL_LOG_DEBUG("###WR (DBH_LEN_TYPE%u_REG) addr: 0x%08x, val: 0x%08x\n", \
                   (unsigned int)(reg_val - 0x100) / 4, \
                   (unsigned int)reg_offset, \
                   (unsigned int)val);} \
    } while (0)
#else
#define PRINTF_ACA_SRAM_REG(reg_offset, val) do {} while (0)
#define PRINTF_REG_INFO(unit, reg, val) do {} while (0)
#endif

/* Register Offset macro */
#define DBH_REG_OFFSET(unit_name, reg_name)               \
    (DBH_BASE_ ## unit_name + DBH_ ## reg_name ## _REG_OFFSET)

#define DBH_READ_REGISTER2(regOffset) \
    (*((volatile uint32_t*)(_g_Dubhe_RegBase + (regOffset))))

#define DBH_WRITE_REGISTER2(regOffset, val) \
    do { \
        PRINTF_ACA_SRAM_REG(regOffset, val); \
        (*((volatile uint32_t*)(_g_Dubhe_RegBase + (regOffset))) = (val)); \
    } while (0)


#define DBH_READ_REGISTER(unit_name, reg_name) \
    (*((volatile uint32_t*)(_g_Dubhe_RegBase + \
                            (DBH_REG_OFFSET(unit_name, reg_name)))))

#define DBH_WRITE_REGISTER(unit_name, reg_name, val) \
    do { \
        uint32_t unit = DBH_BASE_ ## unit_name;  \
        uint32_t reg  = DBH_ ## reg_name ## _REG_OFFSET; \
        PRINTF_REG_INFO(unit, reg, val); \
        (*((volatile uint32_t*)(_g_Dubhe_RegBase + (reg + unit))) = (val)); \
    } while (0)

#endif

/*************************** The End Of File*****************************/
