/**
 * @if copyright_display
 *      Copyright (C), 2024-2025, Arm Technology (China) Co., Ltd.
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

#pragma once

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	AES_OPERATION_DECRYPT = 0,
	AES_OPERATION_ENCRYPT = 1,
}key_ladder_mode_t;

typedef struct{
	uint32_t          key_id;
	key_ladder_mode_t mode;
	unsigned char     iv[16];
}key_ladder_context_t;

int dubhe_key_ladder_crypt_aes_cbc( key_ladder_context_t *ex_ctx, size_t length,
                         const unsigned char *input,
                         unsigned char *output );

int dubhe_key_ladder_crypt_aes_ecb( key_ladder_context_t *ex_ctx, size_t length,
                         const unsigned char *input,
                         unsigned char *output );

#ifdef __cplusplus
}
#endif