/*
 * Copyright (c) 2017-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SECURE_BOOT_GEN_DEFS_H
#define _SECURE_BOOT_GEN_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*! @file
@brief This file contains all of the definitions and structures that are used for the secure boot.
*/

#include "cc_pal_sb_plat.h"
#include "cc_sec_defs.h"


/* General definitions */
/***********************/

/*RSA definitions*/
#if (KEY_SIZE == 2048)
#define SB_RSA_MOD_SIZE_IN_WORDS		 64
#elif (KEY_SIZE == 3072)
#define SB_RSA_MOD_SIZE_IN_WORDS		96
#else
#error Unsupported CryptoCell key size requested
#endif

#define SB_RSA_HW_PKI_PKA_BARRETT_MOD_TAG_SIZE_IN_WORDS 5


/*! Public key data structure. */
typedef struct {
	uint32_t N[SB_RSA_MOD_SIZE_IN_WORDS];				/*!< N public key, big endian representation. */
	uint32_t Np[SB_RSA_HW_PKI_PKA_BARRETT_MOD_TAG_SIZE_IN_WORDS];	/*!< Np (Barrett n' value). */
} CCSbNParams_t;

/*! Signature structure. */
typedef struct {
	uint32_t sig[SB_RSA_MOD_SIZE_IN_WORDS];				/*!< RSA PSS signature. */
} CCSbSignature_t;


/********* Supported algorithms definitions ***********/

/*! RSA supported algorithms */
/* Note: this applies to either 2k or 3k based on CryptoCell SBROM library
 * version - it means 2k in version 1 and 3k in version 2 (yes, really).
 */
typedef enum {
	RSA_PSS                = 0x01,			/*!< RSA PSS after hash SHA 256 */
	RSA_PKCS15	       = 0x02,			/*!< RSA PKX15 */
	RSA_Last               = 0x7FFFFFFF
} CCSbRsaAlg_t;

#ifdef __cplusplus
}
#endif

#endif
