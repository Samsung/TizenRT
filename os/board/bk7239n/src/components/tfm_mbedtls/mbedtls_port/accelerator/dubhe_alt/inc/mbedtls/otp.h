/**
* @if copyright_display
 *      Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
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
/**
 * \file otp.h
 *
 * \brief This file contains ARM Dubhe engine OTP definitions and functions.
 *
 */
#ifndef ARM_CE_DUBHE_OTP_H
#define ARM_CE_DUBHE_OTP_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(ARM_CE_DUBHE_OTP)
#include <stddef.h>
#include <stdint.h>
#include "dubhe_otp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OTP error numbers */
#define MBEDTLS_OTP_OVERFLOW_ERR -0x11000
#define MBEDTLS_OTP_LCS_ERR -0x11001
#define MBEDTLS_OTP_RD_IL_ERR -0x11002
#define MBEDTLS_OTP_WR_IL_ERR -0x11003
#define MBEDTLS_OTP_OP_INVALID_ERR -0x11004
#define MBEDTLS_OTP_WR_INVALID_ERR -0x11005
#define MBEDTLS_OTP_INFO_INVALID_ERR -0x11006
#define MBEDTLS_OTP_PARAM_INVLID_ERR -0x11007
#define MBEDTLS_OTP_LOCK_ERR -0x11008
#define MBEDTLS_OTP_PERMISSION_DENY -0x11009

typedef enum
{
    MODEL_ID,
    MODEL_KEY,
    DEV_ID,
    DEV_ROOT_KEY,
    SEC_BT_PK_HASH,
    SEC_DBG_PK_HASH,
    LIFE_CYCLE,
    LOCK_CTRL,
    USR_NON_SEC_REGION,
    USR_SEC_REGION,
    TEST_REGION,
} mbedtls_otp_info_t;

typedef enum {
    LCS_CM      = 0,
    LCS_DM      = 1,
    LCS_DD      = 3,
    LCS_DR      = 7,
    LCS_INVALID = 0xFF,
} mbedtls_otp_lcs_t;

typedef arm_ce_otp_lock_t mbedtls_otp_lock_t;

/**
 * \brief          This function get the otp information, and writes
 *                 the result to the output buffer. word stream base.
 * \param type     The mbedtls_otp_info_t type.
 * \param output   The otp information result.
 * \param size     output buffer size.
 * \param offset   The internal read word offset.
 *
 * \return         \c 0 on success.
 */
int mbedtls_get_otp_info( mbedtls_otp_info_t type, uint32_t *output,
                          size_t size, uint32_t offset );

/**
 * \brief          This function get the otp information, and writes
 *                 the result to the output buffer. bytes streams base.
 * \param type     The mbedtls_otp_info_t type.
 * \param output   The otp information result.
 * \param size     output buffer size.
 * \param offset   offset bytes from area's beginning
 *
 * \return         \c 0 on success.
 */
int mbedtls_get_otp_info_ex( mbedtls_otp_info_t type, void *output,
                          size_t size, size_t offset );

/**
 * \brief          This function set the otp information. word stream base.
 * \param type     The mbedtls_otp_info_t type.
 * \param input    The information to be written.
 * \param ilen     The input information size.
 * \param offset   The internal write word offset.
 *
 * \return         \c 0 on success.
 */

int mbedtls_set_otp_info( mbedtls_otp_info_t type, const uint32_t *input,
                          size_t ilen, uint32_t offset );

/**
 * \brief          This function set the otp information. bytes streams base.
 * \param type     The mbedtls_otp_info_t type.
 * \param input    The information to be written.
 * \param ilen     The input information size.
 * \param offset   offset bytes from area's beginning
 *
 * \return         \c 0 on success.
 */

int mbedtls_set_otp_info_ex( mbedtls_otp_info_t type, const void *input,
                          size_t ilen, size_t offset );

#define mbedtls_otp_set_lock arm_ce_otp_set_lock

#define mbedtls_otp_get_lock arm_ce_otp_get_lock

#if defined(DBH_OTP_PUF_SUPPORT)

/**
 * \brief          puf is ready or not.
 * \return         \c true on ready, false on not ready.
 */

bool mbedtls_otp_puf_ready(void);

/**
 * \brief          puf running in deep standby mode.
 *
 */

int mbedtls_otp_puf_deep_standby(void);

/**
 * \brief          wake up form deep standby mode.
 *
 */

int mbedtls_otp_puf_wakeup( void );

/**
 * \brief          initial margin read.
 * \param area     otp region
 * \param output   buffer holding the data of otp region
 * \param size     size of the output buffer,
 *                 after called rewrite with actual size of reading
 * \param offset   offset bytes form erea's beginning
 *
 * \return         \c 0 on success.
 */

int mbedtls_otp_puf_initial_margin_read( mbedtls_otp_info_t area,
                                        void *output, size_t *size
                                        , size_t offset );

/**
 * \brief          program margin read.
 * \param area     otp region
 * \param output   buffer holding the data of otp region
 * \param size     size of the output buffer,
 *                 after called rewrite with actual size of reading
 * \param offset   offset bytes form erea's beginning
 *
 * \return         \c 0 on success.
 */

int mbedtls_otp_puf_pgm_margin_read( mbedtls_otp_info_t area,
                                     void *output, size_t *size
                                     , size_t offset);

/**
 * \brief          enrollment.
 *
 * \return         \c 0 on success.
 */
int mbedtls_otp_puf_enrollment(void);

/**
 * \brief          check puf codes's quality.
 *
 * \return         \c 0 on success.
 */
int mbedtls_otp_puf_quality_check(void);



/**
 * \brief          revokation.
 *
 * \return         \c 0 on success.
 */
// int mbedtls_otp_puf_revoke(void);


#endif



#ifdef __cplusplus
}
#endif
#endif /* ARM_CE_DUBHE_OTP */
#endif /* ARM_CE_DUBHE_OTP_H */
