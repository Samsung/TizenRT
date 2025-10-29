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
 

#ifndef __SECUREDEBUG_PROTOCOL_H__
#define __SECUREDEBUG_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "securedebug_defs.h"

#define SWAP_ENDIAN(word)                                                      \
    (((word) >> 24) | (((word)&0x00FF0000) >> 8) |                             \
     (((word)&0x0000FF00) << 8) | (((word)&0x000000FF) << 24))

#ifdef BIG__ENDIAN
#define SWAP_TO_LE(word) SWAP_ENDIAN(word)
#define SWAP_TO_BE(word) word
#else
#define SWAP_TO_LE(word) word
#define SWAP_TO_BE(word) SWAP_ENDIAN(word)
#endif

#define SEC_DEBUG_PROTOCOL_MAGIC_VALUE (0x53454442)  

#define SEC_DEBUG_HAND_SHAKE_MAGIC_VALUE 0x5a6b7c8d
#define SEC_DEBUG_PROTOCOL_MAX_DATA_SIZE 1024
#define SEC_DEBUG_PROTOCOL_STATUS_SIZE 0x01
#define SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE 0x20

 
enum {
    SEC_DEBUG_PROTOCOL_PUBKEY_VERIFY_SUCCESS = 0x66,
    SEC_DEBUG_PROTOCOL_SIG_AND_CMD_SUCCESS   = 0x88,
    SEC_DEBUG_PROTOCOL_PUBKEY_VERIFY_FAILED  = 0xF1,
    SEC_DEBUG_PROTOCOL_SIG_VERIFY_FAILED     = 0xF2,
    SEC_DEBUG_PROTOCOL_EXEC_CMD_FAILED       = 0xF3,
    SEC_DEBUG_PROTOCOL_FAILED_INTERNAL       = 0xF4,
};
#define SEC_DEBUG_MAX_PUBKEY_SIZE 256

typedef uint8_t
    sec_debug_protocol_challenge_t[SEC_DEBUG_PROTOCOL_CHALLENGE_SIZE];

typedef enum _sec_debug_protocol_tag_t {
    SEC_DEBUG_CREATE_HANDSHAKE_REQUEST           = 0,
    SEC_DEBUG_CREATE_HANDSHAKE_RESPONSE          = 1,
    SEC_DEBUG_PUBKEY_VERIFY_REQUEST              = 2,
    SEC_DEBUG_PUBKEY_VERIFY_RESPONSE             = 3,
    SEC_DEBUG_SIG_VERIFY_AND_EXCUTE_CMD_REQUEST  = 4,
    SEC_DEBUG_SIG_VERIFY_AND_EXCUTE_CMD_RESPONSE = 5,
    SEC_DEBUG_DEVICE_TERMINATE_REQUEST           = 0xA5,

    SEC_DEBUG_INVALID = 0xFF,
} sec_debug_protocol_tag_t;

typedef enum _sec_debug_protocol_status_t {
    SEC_DEBUG_PROTOCOL_INITIAL                     = 0,
    SEC_DEBUG_PROTOCOL_HANDSHAKE_PROCESS_SUCCESS   = 1,
    SEC_DEBUG_PROTOCOL_PUBKEY_PROCESS_SUCCESS      = 2,
    SEC_DEBUG_PROTOCOL_SIG_AND_CMD_PROCESS_SUCCESS = 3,

    SEC_DEBUG_PROTOCOL_FAILED = 0xFF,
} sec_debug_protocol_status_t;

typedef struct _sec_debug_pubkey_info_t {
    sec_debug_dgst_sch_t pubkey_dgst_sch;
    uint32_t pubkey_size;
    uint8_t pubkey[SEC_DEBUG_MAX_PUBKEY_SIZE];
} sec_debug_pubkey_info_t;

typedef struct _sec_debug_protocol_info_t {
    sec_debug_pubkey_info_t pubkey_info;
    uint32_t model_id;
    uint32_t device_id;
    sec_debug_protocol_challenge_t challenge_data;
} debug_protocol_info_t;

typedef struct _sec_debug_sig_info_t {
    sec_debug_sig_sch_t sig_sch;
    uint32_t sig_size;
    uint8_t *sig;
} sec_debug_sig_info_t;

hal_ret_t sec_debug_protocol_process(void);

static inline uint32_t _check_sum(uint8_t *data, size_t size)
{
    size_t i     = 0;
    uint32_t sum = 0;
    for (i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum;
}

#define _PROTOCOL_MAGIC(__u32_ptr__) (((uint32_t *)(__u32_ptr__))[0])
#define _PROTOCOL_CHECKSUM(__u32_ptr__) (((uint32_t *)(__u32_ptr__))[1])
#define _PROTOCOL_LEN(__u32_ptr__) (((uint32_t *)(__u32_ptr__))[2])
#define _PROTOCOL_CMD(__u32_ptr__) (((uint32_t *)(__u32_ptr__))[3])
#define _PROTOCOL_PAYLOAD_ADDR(__u32_ptr__) ((uint32_t *)(__u32_ptr__) + 4)
#define _PROTOCOL_PAYLOAD_SIZE(__u32_ptr__)                                    \
    (_PROTOCOL_LEN(__u32_ptr__) - (sizeof(uint32_t) * 4))

static inline void _protocol_init_header(uint32_t *protocol_data,
                                         uint32_t body_len,
                                         uint32_t cmd_tag)
{
     
    _PROTOCOL_MAGIC(protocol_data) = SEC_DEBUG_PROTOCOL_MAGIC_VALUE;
     
    _PROTOCOL_LEN(protocol_data) = body_len + sizeof(uint32_t) * 4;
     
    _PROTOCOL_CMD(protocol_data) = cmd_tag;
     
    _PROTOCOL_CHECKSUM(protocol_data) =
        _check_sum((uint8_t *)(&(_PROTOCOL_LEN(protocol_data))),
                   _PROTOCOL_LEN(protocol_data) - sizeof(uint32_t) * 2);
}

#ifdef __cplusplus
}
#endif

#endif  

 


/*************************** The End Of File*****************************/

