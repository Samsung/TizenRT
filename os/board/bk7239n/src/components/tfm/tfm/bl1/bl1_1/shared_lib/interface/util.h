/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef BL1_UTIL_H
#define BL1_UTIL_H

#include <stddef.h>

#include "fih.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief                  Compares the given regions of memory for equality.
 *
 * \param[in]  ptr1        Pointer to the first memory region.
 * \param[in]  ptr2        Pointer to the second memory region.
 * \param[in]  size        Size of the two memory regions.
 *
 * \note                   This function is hardened against both fault
 *                         injection and differential power analysis, and is
 *                         constant time (except for time taken for TRNG
 *                         invocations).
 *
 * \note                   The ordering of comparisons in this function is
 *                         randomised. This is done by comparing in the forward
 *                         direction for a uniform random number of elements
 *                         between 1 and 8 inclusive, and then comparing in the
 *                         reverse direction for a uniform random number of
 *                         elements between 1 and 8 inclusive. This is repeated
 *                         until the comparison is done.
 *
 * \note                   This function only checks equality, and does not
 *                         return any information about the elements which
 *                         differ, so is semantically different to memcmp.
 *
 * \retval FIH_SUCCESS     The two given memory regions are identical.
 * \retval FIH_FAILURE     The two given memory regions are not identical, or a
 *                         failure has occurred and they cannot be compared.
 */
fih_int bl_secure_memeql(const void *ptr1, const void *ptr2, size_t num);

/**
 * \brief                  Copies the values in memory at source to the memory
 *                         at destination, for a given size.
 *
 * \param[in]  source      Source memory to copy from.
 * \param[in]  num         Number of bytes to be copied.
 * \param[out] destination Destination memory to copy into.
 *
 * \note                   This function is hardened against both fault
 *                         injection and differential power analysis.
 *
 * \note                   The ordering of copying in this function is
 *                         randomised. This is done by comparing in the forward
 *                         direction for a uniform random number of elements
 *                         between 1 and 8 inclusive, and then copying in the
 *                         reverse direction for a uniform random number of
 *                         elements between 1 and 8 inclusive. This is repeated
 *                         until the copying is done.
 *
 * \retval FIH_SUCCESS     The copy completed successfully.
 * \retval FIH_FAILURE     A failure has occurred and the copy has not been
 *                         completed.
 */
fih_int bl_secure_memcpy(void *destination, const void *source, size_t num);

#ifdef __cplusplus
}
#endif

#endif /* BL1_UTIL_H */
