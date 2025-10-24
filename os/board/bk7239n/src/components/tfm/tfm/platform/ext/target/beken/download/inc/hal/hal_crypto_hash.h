// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_CRYPTO_HASH_H__
#define __HAL_CRYPTO_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
#include "hal_crypto.h"

typedef void *hal_hash_ctx_t;

/**
 * @ingroup	hal_hash
 * @brief	hal_crypto_hash function, this function excute a hash
 * operation
 * @param[out] dgst  HASH digest value
 * @param[in] dgst_sch  HASH scheme see enume hal_hash_sch_t
 * @param[in] data  the data which used to calculate the digest
 * @param[in] data_size  size of the data which used to calculate the digest
 * @return	HAL_OK on success. error codes otherwise
 */
HAL_API hal_ret_t hal_crypto_hash(hal_hash_sch_t dgst_sch,
                                  uint8_t *data,
                                  size_t data_size,
                                  uint8_t *dgst,
                                  size_t *dgst_size);

HAL_API hal_ret_t hal_crypto_hash_init(hal_hash_sch_t dgst_sch,
                                       hal_hash_ctx_t *ctx);
HAL_API hal_ret_t hal_crypto_hash_update(hal_hash_ctx_t ctx,
                                         uint8_t *data,
                                         size_t data_size);
HAL_API hal_ret_t hal_crypto_hash_final(hal_hash_ctx_t ctx,
                                        uint8_t *dgst,
                                        size_t *dgst_size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_CRYPTO_HASH_H__ */