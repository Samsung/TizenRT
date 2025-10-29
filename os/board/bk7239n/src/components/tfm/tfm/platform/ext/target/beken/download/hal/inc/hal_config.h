/*
 * Copyright (c) 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved.
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __HAL_CONFIG_H__
#define __HAL_CONFIG_H__

/**
 * This file contains the secure boot configurations used for the secure boot
 * core.
 */

/**
 * \def     CAL_ENABLE_SHA256
 * \brief   Enable or disable SHA256 related algorithms used in:
 *          1. Calculating public key hash.
 *          2. Calculating image hash.
 *          3. RSASSA_PKCS_V15_SHA256 of verifying manifest's signature.
 *          4. RSASSA_PKCS_V21_SHA256 of verifying manifest's signature.
 *          5. ECDSA_SHA256 of verifying manifest's signature.
 */
#define CAL_ENABLE_SHA256

/**
 * \def     CAL_ENABLE_SHA224
 * \brief   Enable or disable SHA224 related algorithms used in:
 *          1. Calculating public key hash.
 *          2. Calculating image hash.
 */
#define CAL_ENABLE_SHA224

/**
 * \def     CAL_ENABLE_SHA1
 * \brief   Enable or disbale SHA1 related algorithms used in:
 *          1. Calculating public key hash.
 *          2. Calculating image hash.
 *          3. RSASSA_PKCS_V15_SHA1 of verifying manifest's signature.
 *          4. RSASSA_PKCS_V21_SHA1 of verifying manifest's signature.
 *          5. ECDSA_SHA1 of verifying manifest's signature.
 */
#define CAL_ENABLE_SHA1

/**
 * \def     CAL_ENABLE_RSA
 * \brief   Enable or disable RSA related algorithms in signature verification
 *          including:
 *          1. RSASSA_PKCS_V15_SHA256
 *          2. RSASSA_PKCS_V15_SHA1
 *          3. RSASSA_PKCS_V21_SHA256
 *          4. RSASSA_PKCS_V21_SHA1
 */
#define CAL_ENABLE_RSA

/**
 * \def     CAL_ENABLE_ECDSA
 * \brief   Enable or disable ECDSA related algorithms in signature verification
 *          including:
 *          1. ECDSA_SHA256
 *          2. ECDSA_SHA1
 */
#define CAL_ENABLE_ECDSA

/**
 * \def     CAL_ENABLE_CIPHER_MODE_ECB
 * \brief   Enable or disable cipher mode of ECB including:
 *          1. AES-ECB
 */
#define CAL_ENABLE_CIPHER_MODE_ECB

/**
 * \def     CAL_ENABLE_CIPHER_MODE_CTR
 * \brief   Enable or disable cipher mode of CTR including:
 *          1. AES-CTR
 */
#define CAL_ENABLE_CIPHER_MODE_CTR

#if (defined(CAL_ENABLE_CIPHER_MODE_ECB) ||                   \
     defined(CAL_ENABLE_CIPHER_MODE_CTR))
#define CAL_ENABLE_AES
#endif

#include "hal_check_config.h"

#endif /* __HAL_CONFIG_H__ */
