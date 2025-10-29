// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_CRYPTO_RSA_H__
#define __HAL_CRYPTO_RSA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
#include "hal_crypto.h"

HAL_API hal_ret_t hal_crypto_rsa_verify_digest(hal_crypto_sch_t sch,
                                               hal_crypto_key_t *key,
                                               uint8_t *dgst,
                                               size_t dgst_size,
                                               uint8_t *sig,
                                               size_t sig_size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_CRYPTO_RSA_H__ */