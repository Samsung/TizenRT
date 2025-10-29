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
 

#ifndef __SECUREDEBUG_DEFS_H__
#define __SECUREDEBUG_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "pal.h"
#include "secure_debug.h"

 

#define SEC_DEBUG_UNUSED_PARAM(prm) ((void)prm)

#define SEC_DEBUG_MAX_UINT32_VAL (0xFFFFFFFF)

#define SEC_DEBUG_SHA1_DIGEST_SIZE 20
#define SEC_DEBUG_SHA224_DIGEST_SIZE 28
#define SEC_DEBUG_SHA256_DIGEST_SIZE 32

#define SEC_DEBUG_AES128_KEY_LEN_INBITS 128
#define SEC_DEBUG_AES256_KEY_LEN_INBITS 256
#define SEC_DEBUG_AES128_KEY_LEN 16
#define SEC_DEBUG_AES256_KEY_LEN 32

#define SEC_DEBUG_AES_IV_SIZE 16
#define SEC_DEBUG_AES_BLOCK_SIZE 16

#define SEC_DEBUG_RSA1024_SIG_LEN 128
#define SEC_DEBUG_RSA2048_SIG_LEN 256
#define SEC_DEBUG_RSA1024_PUBKEY_SIZE 128
#define SEC_DEBUG_RSA2048_PUBKEY_SIZE 256

 
 
#define SEC_DEBUG_ECDSA_MAX_SIG_LEN (2 * (1 + 1 + 1 + (521 + 7) / 8) + 1 + 2)

#define SEC_DEBUG_ECDSA256_PUBKEY_LEN 65
#define SEC_DEBUG_ECDSA521_PUBKEY_LEN 133

#define SEC_DEBUG_ECDSA256_SIG_LEN 64
#define SEC_DEBUG_ECDSA521_SIG_LEN 132

#define IS_RSA_SIG_SCH(sig_sch)                                                \
    (((sig_sch >= SEC_DEBUG_SIG_SCH_RSA_1024_SHA1) &&                          \
      (sig_sch <= SEC_DEBUG_SIG_SCH_RSA_2048_SHA256))                          \
         ? true                                                                \
         : false)

#define IS_ECDSA_SIG_SCH(sig_sch)                                              \
    (((sig_sch >= SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1) &&                         \
      (sig_sch <= SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256))                         \
         ? true                                                                \
         : false)

typedef uint8_t sec_debug_dgst_result_t[SEC_DEBUG_SHA256_DIGEST_SIZE];

 

 
typedef enum _sec_debug_dgst_sch_t {
    SEC_DEBUG_DGST_SCH_INVALID = 0,
    SEC_DEBUG_DGST_SCH_SHA1    = 1,
    SEC_DEBUG_DGST_SCH_SHA224  = 2,
    SEC_DEBUG_DGST_SCH_SHA256  = 3,
} sec_debug_dgst_sch_t;

 
typedef enum _sec_debug_sig_sch_t {
    SEC_DEBUG_SIG_SCH_INVALID         = 0,
    SEC_DEBUG_SIG_SCH_RSA_1024_SHA1   = 1,
    SEC_DEBUG_SIG_SCH_RSA_1024_SHA256 = 2,
    SEC_DEBUG_SIG_SCH_RSA_2048_SHA1   = 3,
    SEC_DEBUG_SIG_SCH_RSA_2048_SHA256 = 4,
    SEC_DEBUG_SIG_SCH_ECDSA_256_SHA1  = 5,   
    SEC_DEBUG_SIG_SCH_ECDSA_256_SHA256 = 6,  
    SEC_DEBUG_SIG_SCH_ECDSA_521_SHA1 = 7,    
    SEC_DEBUG_SIG_SCH_ECDSA_521_SHA256 = 8,  
} sec_debug_sig_sch_t;

#ifdef __cplusplus
}
#endif

#endif  

 


/*************************** The End Of File*****************************/

