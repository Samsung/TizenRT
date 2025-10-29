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

#ifndef __DUBHE_OTP_H__
#define __DUBHE_OTP_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "dubhe_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARM_CE_FPGA_VERSION

/* OTP error information */
#define DBH_OTP_OVERFLOW_ERR -0x11000
#define DBH_OTP_LCS_ERR -0x11001
#define DBH_OTP_RD_IL_ERR -0x11002
#define DBH_OTP_WR_IL_ERR -0x11003
#define DBH_OTP_OP_INVALID_ERR -0x11004
#define DBH_OTP_WR_INVALID_ERR -0x11005
#define DBH_OTP_INFO_INVALID_ERR -0x11006
#define DBH_OTP_PARAM_INVALID_ERR -0x11007
#define DBH_OTP_LOCK_ERR -0x11008
#define DBH_OTP_PERMISSION_DENY -0x11009

#define DBH_OTP_LOCK_OK 0

#define DBH_OTP_UPDATE_SUCCESS 0
#define DBH_OTP_UPDATE_FAIL -0x1

#ifdef DBH_OTP_PUF_SUPPORT
#define DBH_OTP_QUALITY_CHECK_ERR -0x11007
#endif

#define DBH_OTP_ENROLL_SEED 0x12345678
#define DBH_OTP_WR_FAIL_INFO 0xFFFFFFFF

#define DBH_OTP_MODEL_ID_OFFSET 0x0000UL
#define DBH_OTP_MODEL_KEY_OFFSET 0x0004UL
#define DBH_OTP_DEV_ID_OFFSET 0x0014UL
#define DBH_OTP_DEV_ROOT_KEY_OFFSET 0x0018UL
#define DBH_OTP_SEC_BT_PK_HASH_OFFSET 0x0028UL
#define DBH_OTP_SEC_DBG_PK_HASH_OFFSET 0x0048UL
#define DBH_OTP_LCS_OFFSET 0x0068UL
#define DBH_OTP_RES_OFFSET 0x006CUL
#define DBH_OTP_LOCK_CTRL_OFFSET 0x007CUL
#define DBH_OTP_USR_NON_SEC_REGION_OFFSET 0x0080UL

#define DBH_OTP_MODEL_ID_SIZE                                                  \
    ( ( DBH_OTP_MODEL_KEY_OFFSET - DBH_OTP_MODEL_ID_OFFSET ) )
#define DBH_OTP_MODEL_KEY_SIZE                                                 \
    ( ( DBH_OTP_DEV_ID_OFFSET - DBH_OTP_MODEL_KEY_OFFSET ) )
#define DBH_OTP_DEV_ID_SIZE                                                    \
    ( ( DBH_OTP_DEV_ROOT_KEY_OFFSET - DBH_OTP_DEV_ID_OFFSET ) )
#define DBH_OTP_DEV_ROOT_KEY_SIZE                                              \
    ( ( DBH_OTP_SEC_BT_PK_HASH_OFFSET - DBH_OTP_DEV_ROOT_KEY_OFFSET ) )
#define DBH_OTP_SEC_BT_PK_HASH_SIZE                                            \
    ( ( DBH_OTP_SEC_DBG_PK_HASH_OFFSET - DBH_OTP_SEC_BT_PK_HASH_OFFSET ) )
#define DBH_OTP_SEC_DBG_PK_HASH_SIZE                                           \
    ( ( DBH_OTP_LCS_OFFSET - DBH_OTP_SEC_DBG_PK_HASH_OFFSET ) )
#define DBH_OTP_LCS_SIZE ( ( DBH_OTP_RES_OFFSET - DBH_OTP_LCS_OFFSET ) )
#define DBH_OTP_WORD_SIZE 4
#define DBH_OTP_LOCK_CTRL_SIZE ( 1 * DBH_OTP_WORD_SIZE )
#define DBH_OTP_TEST_REGION_SIZE ( 0 * DBH_OTP_WORD_SIZE )

/* User defined region 8 bytes default */
#define DBH_OTP_USR_NON_SEC_REGION_SIZE ( 64 * DBH_OTP_WORD_SIZE )
#define DBH_OTP_USR_SEC_REGION_SIZE ( 96 * DBH_OTP_WORD_SIZE )
#define DBH_OTP_USR_SEC_REGION_OFFSET                                          \
    ( ( DBH_OTP_USR_NON_SEC_REGION_OFFSET )                                    \
      + ( DBH_OTP_USR_NON_SEC_REGION_SIZE ) )
#define DBH_OTP_TEST_REGION_OFFSET                                             \
    ( ( DBH_OTP_USR_NON_SEC_REGION_OFFSET )                                    \
      + ( DBH_OTP_USR_NON_SEC_REGION_SIZE )                                \
      + ( DBH_OTP_USR_SEC_REGION_SIZE ) )

#define OTP_INFO_MODEL_ID_LOCK_BIT_OFFSET 0x0
#define OTP_INFO_MODEL_KEY_LOCK_BIT_OFFSET 0x1
#define OTP_INFO_DEV_ID_LOCK_BIT_OFFSET 0x2
#define OTP_INFO_DEV_ROOT_KEY_LOCK_BIT_OFFSET 0x3
#define OTP_INFO_SEC_BT_PK_HASH_LOCK_BIT_OFFSET 0x4
#define OTP_INFO_SEC_DBG_PK_HASH_LOCK_BIT_OFFSET 0x5
#define OTP_INFO_USR_NON_SEC_LOCK_BIT_OFFSET 0x8
#define OTP_INFO_USR_SEC_LOCK_BIT_OFFSET 0x10

#define USR_NON_SEC_MAX_LOCK_SIZE 0xC0
#define USR_SEC_MAX_LOCK_SIZE 0x80
#define USR_NON_SEC_MAX_LOCK_OFFSET                                            \
    ( ( DBH_OTP_USR_NON_SEC_REGION_OFFSET ) + ( USR_NON_SEC_MAX_LOCK_SIZE ) )
#define USR_SEC_MAX_LOCK_OFFSET                                                \
    ( ( DBH_OTP_USR_SEC_REGION_OFFSET ) + ( USR_SEC_MAX_LOCK_SIZE ) )


#define ALIGN_UP(x, align)                                                \
    ((((unsigned long)(x)) + ((align)-1UL)) & (~((align)-1UL)))

#define ALIGN_DOWN(x, align)        ((unsigned long)(x) & (~((align)-1)))

#define OR_BLOCK(_des, _src, _ofs, _size) do {          \
    for (size_t _i = 0; _i < _size; _i++)               \
    {                                                   \
        (_des)[(_ofs)+_i] |= (_src)[_i];                \
    }                                                   \
}while(0)

#define CFG_DBH_OTP_WR_ONE

#define DBH_CHECK_OTP_LOCK( type, ret )                                        \
    do {                                                                       \
        volatile uint32_t value;                                               \
        _DBH_READ_OTP_INFO(DBH_OTP_LOCK_CTRL_OFFSET,                           \
                           sizeof(value), (uint32_t *)&value);                 \
        value &= ( 0x1 << ( OTP_INFO_##type##_LOCK_BIT_OFFSET ) );             \
        if ( value == 0 )                                                      \
            *ret = DBH_OTP_LOCK_OK;                                            \
        else                                                                   \
            *ret = value;                                                      \
    } while ( 0 )


#define DBH_OTP_SET_DIRECT_READ() do{                                         \
                            volatile uint32_t _value = 0;                     \
                            _value = DBH_READ_REGISTER( OTP_MGR, OTP_SET );   \
                            DBH_REG_FLD_SET( OTP_SET, DIRECT_RD,              \
                                             _value, 0x1 );                   \
                            DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, _value );   \
                        }while (0)

#define DBH_OTP_SET_SHADOW_READ() do{                                         \
                            volatile uint32_t _value = 0;                     \
                            _value = DBH_READ_REGISTER( OTP_MGR, OTP_SET );   \
                            DBH_REG_FLD_SET( OTP_SET, DIRECT_RD,              \
                                             _value, 0x0 );                   \
                            DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, _value );   \
                        }while (0)


typedef enum {
    DBH_DEV_LCS_CM      = 0,
    DBH_DEV_LCS_DM      = 1,
    DBH_DEV_LCS_DD      = 3,
    DBH_DEV_LCS_DR      = 7,
    DBH_DEV_LCS_INVALID = 0xFF,
} arm_ce_dev_lcs_t;

typedef enum {
    OTP_INFO_MODEL_ID,
    OTP_INFO_MODEL_KEY,
    OTP_INFO_DEV_ID,
    OTP_INFO_DEV_ROOT_KEY,
    OTP_INFO_SEC_BT_PK_HASH,
    OTP_INFO_SEC_DBG_PK_HASH,
    OTP_INFO_LIFE_CYCLE,
    OTP_INFO_LOCK_CTRL,
    OTP_INFO_USR_NON_SEC_REGION,
    OTP_INFO_USR_SEC_REGION,
    OTP_INFO_TEST_REGION,
} arm_ce_otp_info_t;

typedef union _arm_ce_otp_lock_t {
    uint32_t value;
    struct {
        uint32_t model_id : 1;
        uint32_t model_key : 1;
        uint32_t device_id : 1;
        uint32_t device_root_key : 1;
        uint32_t secure_boot_pk_hash : 1;
        uint32_t secure_dbg_pk_hash : 1;
        uint32_t reserve : 2;
        uint32_t usr_nonsec_region : 8;
        uint32_t usr_sec_region : 16;
    } locks;
} arm_ce_otp_lock_t;

#if defined( DBH_OTP_PUF_SUPPORT )
/**
 * \brief          This function do the puf initial margin read for test.
 * \param area     witch otp regions
 * \param output   The otp test output result.
 * \param size     size of output, will be overwrited after returning back .
 * \param offset   offset from area beginning .
 *
 * \return         \c 0 on success.
 */

int arm_ce_otp_puf_initial_margin_read( arm_ce_otp_info_t area,
                                void *output,
                                size_t *size,
                                size_t offset );

/**
 * \brief          This function do the puf program margin read for test.
 * \param area     witch otp regions
 * \param output   The otp test output result.
 * \param size     size of output, will be rewrited after returning back .
 * \param offset   offset from area beginning .
 *
 * \return         \c 0 on success.
 */

int arm_ce_otp_puf_pgm_margin_read( arm_ce_otp_info_t area,
                                void *output,
                                size_t *size,
                                size_t offset );

/**
 * \brief          This function do the puf quality check.
 */

int arm_ce_do_puf_quality_check( void );

/**
 * \brief          This function do the enrollment.
 * \return         \c 0 on success.
 */

int arm_ce_enroll_puf( void );

/**
 * \brief          revoke puf and wipe out root key & model key.
 */

// int arm_ce_revoke_puf( void );

/**
 * \brief          This function setup the otp module into deep standby status.
 * \return         \c 0 on success.
 */

int arm_ce_otp_deep_standby( void );

/**
 * \brief         wakeup from deep standby status.
 * \return         \c 0 on success.
 */
int arm_ce_otp_wakeup( void );

/**
 * \brief         check otp PUF is ready or not.
 * \return         \c true - ready, false - not ready.
 */
bool arm_ce_otp_puf_ready( void );
#endif

/**
 * \brief          This function get the otp information, and writes
 *                 the result to the output buffer.
 * \param type     The arm_ce_otp_info_t type.
 * \param output   The otp information result.
 * \param size     output buffer size.
 * \param offset   The internal read offset.
 *
 * \return         \c 0 on success.
 */

int arm_ce_get_otp_info( arm_ce_otp_info_t type,
                         void *output,
                         size_t size,
                         size_t offset );

/**
 * \brief          This function set the otp information.
 * \param type     The arm_ce_otp_info_t type.
 * \param input    The information to be written.
 * \param ilen     The input information size.
 * \param offset   The internal write offset.
 *
 * \return         \c 0 on success.
 */

int arm_ce_set_otp_info( arm_ce_otp_info_t type,
                         const void *input,
                         size_t ilen,
                         size_t offset );

/**
 * \brief     init otp driver
 */
int arm_ce_otp_driver_init( void );

/**
 * \brief          This function set lock region of otp. each locks writes 1
 * valid, writes 0 ignored \param type     The arm_ce_otp_info_t type. \param
 * lock     The information to be written. \return         \c 0 on success.
 */
int arm_ce_otp_set_lock( arm_ce_otp_lock_t *lock );
/**
 * \brief          This function get otp lock info.
 * \param type     The arm_ce_otp_info_t type.
 * \param lock     store lock info read back from otp.
 * \return         \c 0 on success.
 */
int arm_ce_otp_get_lock( arm_ce_otp_lock_t *lock );

#ifdef __cplusplus
}
#endif

#endif
/*************************** The End Of File*****************************/
