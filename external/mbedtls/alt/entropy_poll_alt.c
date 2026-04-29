/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/**
 * Hardware entropy source using the Samsung SE (SecLink RNG).
 * Enabled when CONFIG_HW_RNG=y.
 */

#include <tinyara/config.h>
#include <string.h>
#include <tinyara/security_hal.h>
#include <tinyara/seclink.h>

#include "mbedtls/build_info.h"
#include "mbedtls/platform.h"
#include "psa/crypto_driver_random.h"

#if defined(MBEDTLS_PSA_DRIVER_GET_ENTROPY)

/**
 * mbedtls_platform_get_entropy – hardware entropy callback for PSA.
 *
 * @param flags        Driver flags (currently unused; pass PSA_DRIVER_GET_ENTROPY_FLAGS_NONE).
 * @param estimate_bits On success, set to (8 * output_size) indicating full-entropy output.
 * @param output       Buffer to receive random bytes.
 * @param output_size  Number of random bytes requested.
 *
 * @return 0 on success, non-zero on failure.
 */
int mbedtls_platform_get_entropy(psa_driver_get_entropy_flags_t flags,
                                  size_t *estimate_bits,
                                  unsigned char *output, size_t output_size)
{
    (void)flags;

    sl_ctx shnd;
    hal_data random = {output, output_size, NULL, 0};

    if (sl_init(&shnd) != SECLINK_OK) {
        return -1;
    }

    if (sl_generate_random(shnd, output_size, &random) != SECLINK_OK) {
        sl_deinit(shnd);
        return -1;
    }

    sl_deinit(shnd);

    /* Full-entropy output: each output byte carries 8 bits of entropy */
    *estimate_bits = 8 * output_size;
    return 0;
}

#endif /* MBEDTLS_PSA_DRIVER_GET_ENTROPY */
