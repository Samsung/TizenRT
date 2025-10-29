/**
* Copyright (C), 2018-2023, Arm Technology (China) Co., Ltd.
* All rights reserved
*
* The content of this file or document is CONFIDENTIAL and PROPRIETARY
* to Arm Technology (China) Co., Ltd. It is subject to the terms of a
* License Agreement between Licensee and Arm Technology (China) Co., Ltd
* restricting among other things, the use, reproduction, distribution
* and transfer.  Each of the embodiments, including this information and,,
* any derivative work shall retain this copyright notice.
*/
 

#ifndef __SECUREDEBUG_INTERNAL_H__
#define __SECUREDEBUG_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "pal.h"
#include "securedebug_defs.h"
#include "securedebug_protocol.h"
#include "secure_debug.h"

typedef struct _sec_debug_buff_alloc_ctx_t {
    bool init_flag;
    hal_chan_t channel;
} sec_debug_buff_alloc_ctx_t;

#define _SEC_DEBUG_GET_DGST_SIZE(sch)                                          \
    ((sch == SEC_DEBUG_DGST_SCH_SHA1)                                          \
         ? SEC_DEBUG_SHA1_DIGEST_SIZE                                          \
         : ((sch == SEC_DEBUG_DGST_SCH_SHA256)                                 \
                ? SEC_DEBUG_SHA256_DIGEST_SIZE                                 \
                : ((sch == SEC_DEBUG_DGST_SCH_SHA224)                          \
                       ? SEC_DEBUG_SHA224_DIGEST_SIZE                          \
                       : 0)))
#define _GET_DGST_SCH_FROM_SIG_SCH(sig_sch)                                    \
    (((sig_sch == SEC_DEBUG_SIG_SCH_RSA_1024_SHA1) ||                          \
      (sig_sch == SEC_DEBUG_SIG_SCH_RSA_2048_SHA1) ||                          \
      (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1) ||                         \
      (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA1))                           \
         ? (HAL_HASH_SCH_SHA1)                                                 \
         : (((sig_sch == SEC_DEBUG_SIG_SCH_RSA_1024_SHA256) ||                 \
             (sig_sch == SEC_DEBUG_SIG_SCH_RSA_2048_SHA256) ||                 \
             (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA256) ||                \
             (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256))                  \
                ? (HAL_HASH_SCH_SHA256)                                        \
                : (HAL_HASH_SCH_INVALID)))

#define _GET_ASYM_KEY_TYPE_FROM_SCH(sig_sch)                                   \
    (((sig_sch == SEC_DEBUG_SIG_SCH_RSA_1024_SHA1) ||                          \
      (sig_sch == SEC_DEBUG_SIG_SCH_RSA_1024_SHA256))                          \
         ? (HAL_CRYPTO_KEY_RSA_1024)                                           \
         : (((sig_sch == SEC_DEBUG_SIG_SCH_RSA_2048_SHA1) ||                   \
             (sig_sch == SEC_DEBUG_SIG_SCH_RSA_2048_SHA256))                   \
                ? (HAL_CRYPTO_KEY_RSA_2048)                                    \
                : (((sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1) ||           \
                    (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA256))           \
                       ? (HAL_CRYPTO_KEY_ECP_NIST_256)                         \
                       : (((sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA1) ||    \
                           (sig_sch == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256))    \
                              ? (HAL_CRYPTO_KEY_ECP_NIST_521)                  \
                              : (HAL_CRYPTO_KEY_INVALID)))))

#define ECDSA_R_S_MAX_SIZE 100
#define SWITCH_ECDSA_SIG_SCH(__sig_sch__)                                      \
    ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1) ||                   \
      ((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA1))                     \
         ? (HAL_CRYPTO_SCH_ECDSA_SHA1)                                         \
         : ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA256) ||          \
             ((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256))            \
                ? (HAL_CRYPTO_SCH_ECDSA_SHA256)                                \
                : (HAL_CRYPTO_SCH_INVALID)))

#define _GET_ECDSA_SIG_SIZE_FROM_SIG_SCH(__sig_sch__)                          \
    ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1) ||                   \
      ((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_256_SHA256))                   \
         ? (32)                                                                \
         : ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA1) ||            \
             ((__sig_sch__) == SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256))            \
                ? (66)                                                         \
                : (0)))

#define SWITCH_HASH_SCH(__dgst_sch__)                                          \
    (((__dgst_sch__) == SEC_DEBUG_DGST_SCH_SHA1)                               \
         ? (HAL_HASH_SCH_SHA1)                                                 \
         : (((__dgst_sch__) == SEC_DEBUG_DGST_SCH_SHA224)                      \
                ? (HAL_HASH_SCH_SHA224)                                        \
                : (((__dgst_sch__) == SEC_DEBUG_DGST_SCH_SHA256)               \
                       ? (HAL_HASH_SCH_SHA256)                                 \
                       : (HAL_HASH_SCH_INVALID))))

#define _GET_LCS(__cur_otp_lcs__)                                              \
    (((__cur_otp_lcs__) == 0)                                                  \
         ? SEC_DEBUG_LCS_CM                                                    \
         : (((__cur_otp_lcs__) == 1)                                           \
                ? SEC_DEBUG_LCS_DM                                             \
                : (((__cur_otp_lcs__) == 3)                                    \
                       ? SEC_DEBUG_LCS_DD                                      \
                       : (((__cur_otp_lcs__) == 7) ? SEC_DEBUG_LCS_DR          \
                                                   : SEC_DEBUG_LCS_MAX))))
#define SEC_DEBUG_PKCS_V15

#ifdef SEC_DEBUG_PKCS_V15
#define SWITCH_RSA_SIG_SCH(__sig_sch__)                                        \
    ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_1024_SHA1) ||                    \
      ((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_2048_SHA1))                      \
         ? (HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA1)                               \
         : ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_1024_SHA256) ||           \
             ((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_2048_SHA256))             \
                ? (HAL_CRYPTO_SCH_RSASSA_PKCS_V15_SHA256)                      \
                : (HAL_CRYPTO_SCH_INVALID)))
#else
#error "currently only support RSA_PKCS_V15,other PKCS not support"
#endif

#define _GET_RSA_SIG_SIZE_FROM_SCH(__sig_sch__)                                \
    ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_1024_SHA1) ||                    \
      ((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_1024_SHA256))                    \
         ? (128)                                                               \
         : ((((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_2048_SHA1) ||             \
             ((__sig_sch__) == SEC_DEBUG_SIG_SCH_RSA_2048_SHA256))             \
                ? (256)                                                        \
                : (0)))

extern sec_debug_buff_alloc_ctx_t g_sec_dbg_ctx;

hal_ret_t sec_debug_calc_data_size(uint8_t *data,
                                   size_t *data_size,
                                   size_t *ength_field_size);
hal_ret_t sec_debug_send_pkg(uint8_t *data, size_t data_size);
hal_ret_t sec_debug_recv_pkg(uint8_t *data, size_t *data_size);
#ifdef __cplusplus
}
#endif

#endif  

 


/*************************** The End Of File*****************************/

