/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_1_CRYPTO_KEY_DEFS_H
#define BL1_1_CRYPTO_KEY_DEFS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HUK_SIZE
#define HUK_SIZE (32)
#endif /* HUK_SIZE */

#ifndef BL2_ENCRYPTION_KEY_SIZE
#define BL2_ENCRYPTION_KEY_SIZE (32)
#endif /* BL2_ENCRYPTION_KEY_SIZE */

 enum tfm_bl1_key_id_t {
    TFM_BL1_KEY_HUK,
    TFM_BL1_KEY_BL2_ENCRYPTION,
#ifdef TEST_BL1_1
    TFM_BL1_KEY_TEST_1,
    TFM_BL1_KEY_TEST_2,
#endif /* TEST_BL1_1 */

  tfm_bl1_key_max,
};

#ifdef __cplusplus
}
#endif

#endif /* BL1_1_CRYPTO_KEY_DEFS_H */
