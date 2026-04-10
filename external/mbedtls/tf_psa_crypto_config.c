/*
 *  TF-PSA-Crypto configuration checks
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

/*
 * TizenRT note: The upstream version of this file includes several
 * auto-generated headers (tf_psa_crypto_config_check_before.h, etc.)
 * produced by the CMake/Python build system.  TizenRT uses a custom
 * Make-based build, so those generated checks are omitted.  Configuration
 * is controlled entirely by external/include/mbedtls/mbedtls_config.h.
 */

#include <tf-psa-crypto/build_info.h>

/* Manually written consistency checks (not auto-generated) */
#include "tf_psa_crypto_check_config.h"

/* For MBEDTLS_STATIC_ASSERT */
#include "tf_psa_crypto_common.h"
/* For PSA_HASH_LENGTH */
#include <psa/crypto_sizes.h>

/* Additional domain-specific checks */
#if defined(MBEDTLS_PSA_CRYPTO_C)
#include "psa_crypto_random_impl.h"
#endif
