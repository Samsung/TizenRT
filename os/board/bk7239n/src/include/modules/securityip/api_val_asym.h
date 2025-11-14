/**
 * @file api_val_asym.h
 *
 * @brief VAL API - Asymmetric Cryptography services
 */

/*****************************************************************************
* Copyright (c) 2014-2018 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef INCLUDE_GUARD_API_VAL_ASYM_H
#define INCLUDE_GUARD_API_VAL_ASYM_H

/*-----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValSize_t
#include "api_val_asset.h"          // ValAssetId_t
#include "api_val_sym.h"            // ValSymContextPtr_Optional_t, ValSymAlgo_t


/*-----------------------------------------------------------------------------
 * Definitions and macros
 */
/** General size conversion defines based on a bits size: */
/** - Converts a size in bits to a size in bytes */
#define VAL_ASYM_DATA_SIZE_B2B(x)   ((x+7)/8)
/** - Converts a size in bits to a size in bytes aligned on a word boundary */
#define VAL_ASYM_DATA_SIZE_B2WB(x)  (4*((x+31)/32))
/** - Converts a size in bits to a size in words */
#define VAL_ASYM_DATA_SIZE_B2W(x)   ((x+31)/32)

/** Various asymmetric crypto related limits/size defines: */
/** for ECP operations for the minimum allowed modulus length in bits */
#define VAL_ASYM_ECP_MIN_BITS          192

/** for ECP operations for the maximum allowed modulus length in bits */
#define VAL_ASYM_ECP_MAX_BITS          521

/** for RSA operations for the minimum allowed modulus length in bits */
#define VAL_ASYM_RSA_MIN_BITS          1024

/** for RSA operations for the maximum allowed modulus length in bits */
#define VAL_ASYM_RSA_MAX_BITS          3072

/** for RSA operations for the minimum allowed exponent length in bits */
#define VAL_ASYM_RSA_EXPO_MIN_BITS     17

/** for RSA operations for the maximum allowed exponent length in bits */
#define VAL_ASYM_RSA_EXPO_MAX_BITS     255

/** for DH and DSA operations for the minimum allowed prime (modulus) length
 *  in bits */
#define VAL_ASYM_DH_DSA_MIN_BITS       1024

/** for DH and DSA operations for the maximum allowed prime (modulus) length
 *  in bits */
#define VAL_ASYM_DH_DSA_MAX_BITS       3072

/** for DH and DSA operations for the minimum allowed divisor (order or
 *  sub-prime) length in bits */
#define VAL_ASYM_DH_DSA_DIVISOR_MIN_BITS 160

/** for DH and DSA operations for the maximum allowed divisor (order or
 *  sub-prime) length in bits */
#define VAL_ASYM_DH_DSA_DIVISOR_MAX_BITS 256

/** for ECP operations for the maximum size of buffer in bytes */
#define VAL_ASYM_ECP_BYTES          VAL_ASYM_DATA_SIZE_B2B(VAL_ASYM_ECP_MAX_BITS)

/** for ECP operations for the maximum size of buffer in words */
#define VAL_ASYM_ECP_WORDS          VAL_ASYM_DATA_SIZE_B2W(VAL_ASYM_ECP_MAX_BITS)

/** for ECDH operations for the maximum size of buffer in bytes */
#define VAL_ASYM_ECDH_BYTES         VAL_ASYM_DATA_SIZE_B2B(VAL_ASYM_ECP_MAX_BITS)

/** for ECDH operations for the maximum size of buffer in words (same as the
 *  ECP maximum) */
#define VAL_ASYM_ECDH_WORDS         VAL_ASYM_DATA_SIZE_B2W(VAL_ASYM_ECP_MAX_BITS)

/** for RSA operations for the maximum size of buffer in bytes */
#define VAL_ASYM_RSA_BYTES          VAL_ASYM_DATA_SIZE_B2B(VAL_ASYM_RSA_MAX_BITS)

/** for RSA operations for the maximum size of buffer in words */
#define VAL_ASYM_RSA_WORDS          VAL_ASYM_DATA_SIZE_B2W(VAL_ASYM_RSA_MAX_BITS)

/** for DH and DSA operations for the maximum size of buffer in bytes */
#define VAL_ASYM_DSA_BYTES          VAL_ASYM_DATA_SIZE_B2B(VAL_ASYM_DH_DSA_MAX_BITS)

/** for DH and DSA operations for the maximum size of buffer in words */
#define VAL_ASYM_DSA_WORDS          VAL_ASYM_DATA_SIZE_B2W(VAL_ASYM_DH_DSA_MAX_BITS)

/** for DH and DSA operations for the maximum size of buffer in bytes */
#define VAL_ASYM_DSA_DIVISOR_BYTES VAL_ASYM_DATA_SIZE_B2B(VAL_ASYM_DH_DSA_DIVISOR_MAX_BITS)

/** for DH and DSA operations for the maximum size of buffer in words */
#define VAL_ASYM_DSA_DIVISOR_WORDS VAL_ASYM_DATA_SIZE_B2W(VAL_ASYM_DH_DSA_DIVISOR_MAX_BITS)

/** counter size in bytes utilized in PSS operation */
#define VAL_ASYM_PSS_CTR_SIZE               4

/** size in bytes used for tag and padding in PKCS #1 padding operation */
#define VAL_ASYM_PKCS1_FIX_PAD              11

/** value of tag used in PKCS #1 padding when done for sign/verification */
#define VAL_ASYM_PKCS1_SIGN_VERIFY_TAG      1

/** value of tag used in PKCS #1 padding when done for encryption/decryption */
#define VAL_ASYM_PKCS1_ENCRYPT_DECRYPT_TAG  2


/** ValAsymBigInt_t - Big number structure */
typedef struct
{
    uint8_t * Data_p;                   // Data part
    uint32_t ByteDataSize;              // Data size in bytes
} ValAsymBigInt_t;

/** ValAsymECCPoint_t - Elliptic Curve Cryptography point structure  */
typedef struct
{
    ValAsymBigInt_t x;                  // Represents the x coordinate
    ValAsymBigInt_t y;                  // Represents the y coordinate
} ValAsymECCPoint_t;

/** ValAsymECDomainParam_t - Elliptic Curve domain parameters */
typedef struct
{
    ValSize_t ModulusSizeBits;          // Modulus size in bits
    ValAsymBigInt_t Modulus;            // Curve modulus (p)
    ValAsymBigInt_t a;                  // Curve constant a
    ValAsymBigInt_t b;                  // Curve constant b
    ValAsymBigInt_t Order;              // Curve order (n)
    ValAsymECCPoint_t BasePoint;        // Curve base point (G)
    uint8_t CoFactor;                   // Curve cofactor (h)
                                        // - Preferably 1, 0 == 1
} ValAsymECDomainParam_t;

/** ValAsymDHDSADomainParam_t - Diffie-Hellman or Digital Signature Algorithm
 *                              domain parameters */
typedef struct
{
    ValSize_t PrimeSizeBits;            // Prime (modulus) size in bits
    ValSize_t DivisorSizeBits;          // Divisor (order or sub-prime) size in bits
    ValAsymBigInt_t Prime;              // Prime p
    ValAsymBigInt_t Divisor;            // Divisor (order or sub-prime) q
    ValAsymBigInt_t Base;               // Base g
} ValAsymDHDSADomainParam_t;

/** ValAsymRSADomainParam_t - RSA domain parameters for private key */
typedef struct
{
    ValAsymBigInt_t Modulus;            // Modulus m
    ValAsymBigInt_t primeP;             // Prime P
    ValAsymBigInt_t primeQ;             // Prime Q
    ValAsymBigInt_t dmodP;              // d % P
    ValAsymBigInt_t dmodQ;              // d % Q
    ValAsymBigInt_t cofQinv;            // cofQinv * p_Q mod m = 1
} ValAsymRSADomainParam_t;


/** ValAsymKey_t - Asymmetric crypto key structure used for all public-key
 *                 algorithms to pass the public and private key part in the
 *                 specified algorithm. */
typedef struct
{
    ValAssetId_t KeyAssetId;            // AssetId of the private/public key
    ValAssetId_t DomainAssetId;         // AssetId of the domain parameters

    ValSize_t ModulusSizeBits;          // (Prime) Modulus size in bits
    ValSymAlgo_t HashAlgorithm;         // Hash Algorithm to use
} ValAsymKey_t;


/** ValAsymSign_t - Digital Signature  */
typedef struct
{
    ValAsymBigInt_t r;                  // Signature component r
    ValAsymBigInt_t s;                  // Signature component s
} ValAsymSign_t;


/** ValAsymPkaOperation_t - PKA operations without Asset use */
typedef enum
{
    VAL_ASYM_PKA_OP_MODEXPE = 0,
    VAL_ASYM_PKA_OP_MODEXPD,
    VAL_ASYM_PKA_OP_MODEXPCRT,
    VAL_ASYM_PKA_OP_ECMONTMUL,
    VAL_ASYM_PKA_OP_ECCMUL,
    VAL_ASYM_PKA_OP_ECCADD,
    VAL_ASYM_PKA_OP_DSASIGN,
    VAL_ASYM_PKA_OP_DSAVERIFY,
    VAL_ASYM_PKA_OP_ECDSASIGN,
    VAL_ASYM_PKA_OP_ECDSAVERIFY,
} ValAsymPkaOperation_t;


/*-----------------------------------------------------------------------------
 * val_AsymEccAllocDomainAsset
 */
/**
 * This function allocates an ECC Domain parameters Asset.
 * Note: Can be used for ECDH, ECDSA and ECC El-Gamal.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccAllocDomainAsset(
        const ValSize_t ModulusSizeBits,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEcdhAllocPrivateKeyAsset
 */
/**
 * This function allocates an ECDH private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdhAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEcdhAllocPublicKeyAsset
 */
/**
 * This function allocates an ECDH public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdhAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEcdsaAllocPrivateKeyAsset
 */
/**
 * This function allocates an ECDSA private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdsaAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEcdsaAllocPublicKeyAsset
 */
/**
 * This function allocates an ECDSA public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdsaAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEddsaAllocKeyAsset
 */
/**
 * This function allocates an EDDSA private or public key Asset.
 *
 * @param [in] fPublicKey
 *     Indication to allocate a public key.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaAllocKeyAsset(
        const bool fPublicKey,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/** Please look at val_AsymEddsaAllocKeyAsset() for details */
#define val_AsymEddsaAllocPrivateKeyAsset(a,b,c) val_AsymEddsaAllocKeyAsset(false,a,b,c)
/** Please look at val_AsymEddsaAllocKeyAsset() for details */
#define val_AsymEddsaAllocPublicKeyAsset(a,b,c)  val_AsymEddsaAllocKeyAsset(true,a,b,c)

/*-----------------------------------------------------------------------------
 * val_AsymEccElGamalAllocPrivateKeyAsset
 */
/**
 * This function allocates an ECC El-Gamal private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccElGamalAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymEccElGamalAllocPublicKeyAsset
 */
/**
 * This function allocates an ECC El-Gamal public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccElGamalAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymCurve25519AllocPrivateKeyAsset
 */
/**
 * This function allocates a Curve25519 private key Asset.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519AllocPrivateKeyAsset(
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymCurve25519AllocPublicKeyAsset
 */
/**
 * This function allocates a Curve25519 public key Asset.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519AllocPublicKeyAsset(
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhDsaAllocDomainAsset
 */
/**
 * This function allocates a DH or DSA Domain parameters Asset.
 *
 * @param [in] PrimeSizeBits
 *     Size of the prime modulus in bits.
 *
 * @param [in] DivisorSizeBits
 *     Size of the divisor (order or sub-prime) in bits.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhDsaAllocDomainAsset(
        const ValSize_t PrimeSizeBits,
        const ValSize_t DivisorSizeBits,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymDsaAllocPrivateKeyAsset
 */
/**
 * This function allocates a DSA private key Asset.
 *
 * @param [in] DivisorSizeBits
 *     Size of the divisor (order or sub-prime) in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDsaAllocPrivateKeyAsset(
        const ValSize_t DivisorSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymDsaAllocPublicKeyAsset
 */
/**
 * This function allocates a DSA public key Asset.
 *
 * @param [in] PrimeSizeBits
 *     Size of the prime modulus in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDsaAllocPublicKeyAsset(
        const ValSize_t PrimeSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhAllocPrivateKeyAsset
 */
/**
 * This function allocates a DH private key Asset.
 *
 * @param [in] DivisorSizeBits
 *     Size of the divisor (order or sub-prime) in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhAllocPrivateKeyAsset(
        const ValSize_t DivisorSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhAllocPublicKeyAsset
 */
/**
 * This function allocates a DH public key Asset.
 *
 * @param [in] PrimeSizeBits
 *     Size of the prime modulus in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhAllocPublicKeyAsset(
        const ValSize_t PrimeSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15AllocPrivateKeyAsset
 */
/**
 * This function allocates a RSA PKCS#1v1.5 private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15AllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15AllocPublicKeyAsset
 */
/**
 * This function allocates a RSA PKCS#1v1.5 public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15AllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPssAllocPrivateKeyAsset
 */
/**
 * This function allocates a RSA PSS private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPssAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPssAllocPublicKeyAsset
 */
/**
 * This function allocates a RSA PSS public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPssAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);


/*-----------------------------------------------------------------------------
 * val_AsymEccLoadDomainAssetPlaintext
 */
/**
 * This function converts the plaintext ECC domain parameters to the EIP-13x
 * HW format and initializes the Asset for it in the Asset Store.
 * Note: Can be used for ECDH, ECDSA and ECC El-Gamal.
 *
 * @param [in] Domain_p
 *     Pointer to the buffer that holds the ECC domain parameter according
 *     ValAsymECDomainParam_t.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the ECC domain
 *     parameters.
 *     This reference is provided by val_AsymEccAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccLoadDomainAssetPlaintext(
        const ValAsymECDomainParam_t * const Domain_p,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymEccLoadPrivateKeyAssetPlaintext
 */
/**
 * This function converts the ECC private key to the EIP-13x HW format and
 * initializes the Asset for it in the Asset Store.
 * Note: Can be used for ECDH, ECDSA and ECC El-Gamal.
 *
 * @param [in] Key_p
 *     Pointer to the buffer that holds the ECC private key.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the ECC domain
 *     parameters.
 *     This reference is provided by val_AsymEccAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccLoadPrivateKeyAssetPlaintext(
        const ValAsymBigInt_t * const Key_p,
        const ValSize_t ModulusSizeBits,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymEccLoadPublicKeyAssetPlaintext
 */
/**
 * This function converts the ECC public key to the EIP-13x HW format and
 * initializes the Asset for it in the Asset Store.
 * Note: Can be used for ECDH, ECDSA and ECC El-Gamal.
 *
 * @param [in] KeyPoint_p
 *     Pointer to the buffer that holds the ECC public key (an ECC point).
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the ECC domain
 *     parameters.
 *     This reference is provided by val_AsymEccAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccLoadPublicKeyAssetPlaintext(
        const ValAsymECCPoint_t * const KeyPoint_p,
        const ValSize_t ModulusSizeBits,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymEddsaLoadKeyAssetPlaintext
 */
/**
 * This function converts the specified private or public key to the EIP-13x
 * HW format and initializes the Asset for it in the Asset Store.
 *
 * @param [in] KeyValue_p
 *     Pointer to the buffer that holds the key value.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the ECC domain
 *     parameters.
 *     This reference is provided by val_AsymEccAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaLoadKeyAssetPlaintext(
        const ValAsymBigInt_t * const KeyValue_p,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymCurve25519LoadKeyAssetPlaintext
 */
/**
 * This function converts the specified private or public key to the EIP-13x
 * HW format and initializes the Asset for it in the Asset Store.
 *
 * @param [in] KeyValue_p
 *     Pointer to the buffer that holds the key value.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the ECC domain
 *     parameters.
 *     This reference is provided by val_AsymEccAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519LoadKeyAssetPlaintext(
        const ValAsymBigInt_t * const KeyValue_p,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhDsaLoadDomainAssetPlaintext
 */
/**
 * This function converts the plaintext DH or DSA domain parameters to the
 * EIP-13x HW format and initializes the Asset for it in the Asset Store.
 *
 * @param [in] Domain_p
 *     Pointer to the buffer that holds the DSA domain parameter according
 *     ValAsymDHDSADomainParam_t.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with the DH or DSA
 *     domain parameters.
 *     This reference is provided by val_AsymDhDsaAllocDomainAsset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhDsaLoadDomainAssetPlaintext(
        const ValAsymDHDSADomainParam_t * const Domain_p,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhDsaLoadKeyAssetPlaintext
 */
/**
 * This function converts the DSA private or public key data to the EIP-13x HW
 * format and initializes the Asset for it in the Asset Store.
 *
 * @param [in] Data_p
 *     Pointer to the buffer that holds the data (a big integer).
 *
 * @param [in] DataSizeBits
 *     Size of the data in bits.
 *
 * @param [in] TargetAssetId
 *     Reference to the key Asset that needs to be loaded.
 *     This reference is provided by val_AsymDsaAlloc*Asset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhDsaLoadKeyAssetPlaintext(
        const ValAsymBigInt_t * const Data_p,
        const ValSize_t DataSizeBits,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaLoadKeyAssetPlaintext
 */
/**
 * This function converts the RSA key data to the EIP-13x HW format and
 * initializes the Asset for it in the Asset Store.
 * Note: Can be used for all RSA key types.
 *
 * @param [in] Modulus_p
 *     Pointer to the buffer that holds the modulus (a big integer).
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] Exponent_p
 *     Pointer to the buffer that holds the exponent (a big integer).
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] TargetAssetId
 *     Reference to the key Asset that needs to be loaded.
 *     This reference is provided by val_AsymRsa*Alloc*Asset() or
 *     val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaLoadKeyAssetPlaintext(
        const ValAsymBigInt_t * const Modulus_p,
        const ValSize_t ModulusSizeBits,
        const ValAsymBigInt_t * const Exponent_p,
        const ValSize_t ExponentSizeBits,
        const ValAssetId_t TargetAssetId,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_Optional_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);


/*-----------------------------------------------------------------------------
 * val_AsymInitKey
 */
/**
 * This function initializes an asymmetric crypto key.
 * Note: This function must be called when the key part are loaded via a
 *       key blob into the Asset Store.
 *
 * @param [in] KeyAssetId
 *     Optional reference to the private/public key Asset. If not provided,
 *     the DomainAssetId must be provided.
 *     This reference is provided by val_AssetAlloc().
 *
 * @param [in] DomainAssetId
 *     Optional reference to the domain parameters Asset.
 *     This reference is provided by val_AssetAlloc().
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [out] KeyContext_p
 *     Pointer to the asymmetric crypto key structure that is initialized.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymInitKey(
        const ValAssetId_Optional_t KeyAssetId,
        const ValAssetId_Optional_t DomainAssetId,
        const ValSize_t ModulusSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        ValAsymKey_t * const KeyContext_p);


/*-----------------------------------------------------------------------------
 * val_AsymEcdsaSign
 */
/**
 * This function is used for generating the ECDSA signature of the supplied
 * message.  The signature is generated using the ECDSA algorithm as defined
 * in standard FIPS PUB 186-4 and IEEE-P1363.
 * The supported ECDSA curves vary from 192 bits to 521 bits.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's private
 *     key for ECDSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer in which the generated Signature must be returned.
 *     Note: The size of the r and s buffers must be each the curve modulus in
 *           bytes.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be generated.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *     Pointer to the symmetric crypto context from the already performed hash
 *     update operation(s) or NULL if HashMessage_p refers to the complete
 *     message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdsaSign(
        ValAsymKey_t * const KeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);

/*-----------------------------------------------------------------------------
 * val_AsymEcdsaVerify
 */
/**
 * This function is used to verify the ECDSA signature of the supplied message.
 * The signature is verified using the ECDSA algorithm as defined in standard
 * FIPS PUB 186-4 and IEEE-P1363.
 * The supported ECDSA curves vary from 192 bits to 521 bits.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer with Signature that must be verification.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be verified.
 *
 * @param [in] HashMessageSize
 *     The size od the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdsaVerify(
        ValAsymKey_t * const KeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);


/*-----------------------------------------------------------------------------
 * val_AsymEddsaSign
 */
/**
 * This function is used for generating the EdDSA signature of the supplied
 * message. The signature is generated using the EdDSA algorithm as defined in:
 * - 'High-speed high-security signatures'
 *   https://ed25519.cr.yp.to/ed25519-20110926.pdf
 * - 'Edwards-curve Digital Signature Algorithm (EdDSA)'
 *   draft-irtf-cfrg-eddsa-08
 * The supported EdDSA curve is Ed25519 (255 bits).
 *
 * @param [in] PrvKeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's private
 *     key for EdDSA.
 *
 * @param [in] PubKeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's public
 *     key for EdDSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer in which the generated Signature must be returned.
 *     Note: The size of the r and s buffers must be each the curve modulus in
 *           bytes.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be generated.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaSign(
        ValAsymKey_t * const PrvKeyContext_p,
        ValAsymKey_t * const PubKeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize);

/*-----------------------------------------------------------------------------
 * val_AsymEddsaVerify
 */
/**
 * This function is used to verify the EdDSA signature of the supplied message.
 * The signature is verified using the EdDSA algorithm as defined in:
 * - 'High-speed high-security signatures'
 *   https://ed25519.cr.yp.to/ed25519-20110926.pdf
 * - 'Edwards-curve Digital Signature Algorithm (EdDSA)'
 *   draft-irtf-cfrg-eddsa-08
 * The supported EdDSA curve is Ed25519 (255 bits).
 *
 * @param [in] PubKeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     EdDSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer with Signature that must be verification.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be verified.
 *
 * @param [in] HashMessageSize
 *     The size od the message that needs to be hashed in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaVerify(
        ValAsymKey_t * const PubKeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize);


/*-----------------------------------------------------------------------------
 * val_AsymDsaSign
 */
/**
 * This function is used for generating the DSA signature of the supplied
 * message. The signature is generated using the DSA algorithm as defined in
 * standard FIPS PUB 186-4.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's private
 *     key for DSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer in which the generated Signature must be returned.
 *     Note: The size of the r and s buffers must be each the curve modulus in
 *           bytes.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be generated.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDsaSign(
        ValAsymKey_t * const KeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);

/*-----------------------------------------------------------------------------
 * val_AsymDsaVerify
 */
/**
 * This function is used to verify the DSA signature of the supplied message.
 * The signature is verified using the DSA algorithm as defined in standard
 * FIPS PUB 186-4.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     DSA.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer with Signature that must be verification.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be verified.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDsaVerify(
        ValAsymKey_t * const KeyContext_p,
        ValAsymSign_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);


/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15Sign
 */
/**
 * This function is used for generating the signature using a RSA-PKCS#1v1.5
 * algorithm on the supplied message. The signature is generated as defined in
 * the standard PKCS#1.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's private
 *     key for RSA-PKCS#1 v1.5.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer in which the generated Signature must be returned.
 *     Note: The size of the r and s buffers must be each the curve modulus in
 *           bytes.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be generated.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15Sign(
        ValAsymKey_t * const KeyContext_p,
        ValAsymBigInt_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPssSign
 */
/**
 * This function is used for generating the signature using a RSA-PSS
 * algorithm on the supplied message. The signature is generated as defined in
 * the standard PKCS#1.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the signer's private
 *     key for RSA-PSS.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer in which the generated Signature must be returned.
 *     Note: The size of the r and s buffers must be each the curve modulus in
 *           bytes.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be generated.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @param [in] SaltSize
 *     The size of the salt in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPssSign(
        ValAsymKey_t * const KeyContext_p,
        ValAsymBigInt_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p,
        const ValSize_t SaltSize);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15Verify
 */
/**
 * This function is used to verify the signature using the RSA-PKCS#1v1.5
 * algorithm on the supplied message. The signature is verified as defined
 * in the standard PKCS#1.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-PKCS#1 v1.5.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer with Signature that must be verification for
 *     RSA-PKCS#1 v1.5.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be verified.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15Verify(
        ValAsymKey_t * const KeyContext_p,
        ValAsymBigInt_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPssVerify
 */
/**
 * This function is used to verify the signature using the RSA-PSS algorithm
 * on the supplied message. The signature is verified as defined in the
 * standard PKCS#1.
 * Note that also the last part of the message (no empty/zero-length message
 * allowed) may be provided but in that case the symmetric crypto context of
 * the already hashed message must be provided as well.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-PSS.
 *
 * @param [in] Signature_p
 *     Pointer to the buffer with Signature that must be verification.
 *
 * @param [in] HashMessage_p
 *     Pointer to the message that needs to be hashed before the signature can
 *     be verified.
 *
 * @param [in] HashMessageSize
 *     The size of the message that needs to be hashed in bytes.
 *
 * @param [in] HashContext_p
 *      Pointer to the symmetric crypto context from the already performed hash
 *      update operation(s) or NULL if HashMessage_p refers to the complete
 *      message to hash.
 *
 * @param [in] SaltSize
 *     The size of the salt in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPssVerify(
        ValAsymKey_t * const KeyContext_p,
        ValAsymBigInt_t * const Signature_p,
        ValOctetsIn_t * const HashMessage_p,
        const ValSize_t HashMessageSize,
        ValSymContextPtr_Optional_t const HashContext_p,
        const ValSize_t SaltSize);


/*-----------------------------------------------------------------------------
 * val_AsymEccGenKeyPair
 */
/**
 * Generate an ECDH, ECDSA or ECC El-Gamal key pair as defined in standard FIPS
 * PUB 186-4 and IEEE-P1363.
 *
 * The key pair is generated as follows:
 *
 * An entity 'A's public and private key pair is associated with a particular
 * set of elliptic curve domain parameters, T = (p; a; b; G; n; h).
 *
 * To generate a key pair, entity 'A' does the following:
 *
 * 1. A random or pseudo-random integer d is selected in the interval [1, n-1].
 * 2. Q = dG is computed.
 * 3. A's public key is Q; A's private key is d.
 *
 * The maximum modulus value of PublicKey can be 521.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] PubKeyPoint_p
 *     Pointer to the buffer in which the public key (an ECC point) must be
 *     returned.
 *
 * @param [out] PrivKeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] PrivKeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccGenKeyPair(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValAsymECCPoint_t * const PubKeyPoint_p,
        ValOctetsOut_Optional_t * const PrivKeyBlob_p,
        ValSize_t * const PrivKeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhGenKeyPair
 */
/**
 * Generate a DH key pair as defined in standard NIST SP800-56A.
 *
 * An entity 'A's key pair is associated with a particular set of domain
 * parameters, T = (p; q; g).
 *
 * The DH key pair is generated as follows :
 *
 * 1. x = a randomly or pseudo randomly generated integer with 0 < x < q
 * 2. y = g^x mod p
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     DH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     DH.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key (an big integer) must be
 *     returned.
 *
 * @param [out] PrivKeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] PrivKeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhGenKeyPair(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValAsymBigInt_t * const PubKeyValue_p,
        ValOctetsOut_Optional_t * const PrivKeyBlob_p,
        ValSize_t * const PrivKeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymEddsaGenKeyPair
 */
/**
 * Generate a EdDSA key pair as defined in:
 * - 'Edwards-curve Digital Signature Algorithm (EdDSA)'
 *   draft-irtf-cfrg-eddsa-08
 *
 * The key pair is generated as follows:
 *
 * The public and private key pair is associated with a particular set of
 * elliptic curve domain parameters, T = (p; d(=a); const(=b); G; n; h).
 *
 * To generate a key pair the following steps are performed:
 * - Private key (x) is a random value in the range [1..2^255].
 * - Public key (y) is calculated according following steps:
 *   1. Hash private key -> H = SHA-512(x)
 *   2. Derive k (prune hash) -> k = (H & (2^(256) -1) & ~(2^255 | 7)) | 2^254
 *   3. Perform a fixed-base scalar multiplication -> Y = [k]G
 *   4. Encode point to get the public key -> y = (Y.y % p) | ((Y.x & 1) << 254)
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     Curve25519.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     Curve25519.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key must be returned.
 *
 * @param [out] PrivKeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] PrivKeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaGenKeyPair(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValAsymBigInt_t * const PubKeyValue_p,
        ValOctetsOut_Optional_t * const PrivKeyBlob_p,
        ValSize_t * const PrivKeyBlobSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymCurve25519GenKeyPair
 */
/**
 * Generate a Curve25519 key pair as defined in RFC 7748 "Elliptic Curves for
 * Security" and Curve25519 "New Diffie-Hellman speed records"
 *
 * The key pair is generated as follows:
 *
 * An entity 'A's public and private key pair is associated with a particular
 * set of elliptic curve domain parameters, T = (p; a; b; G; n; h).
 *
 * To generate a key pair, entity 'A' does the following:
 *
 * 1. A random or pseudo-random integer u is selected in [0..2^254].
 * 2. x = ((u & (2^254-1) & ~7) | 2^254
 * 3. Q = xG is computed.
 * 3. A's public key is Q; A's private key is x.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     Curve25519.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     Curve25519.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same authentication data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key must be returned.
 *
 * @param [out] PrivKeyBlob_p
 *     Pointer to the keyblob buffer.
 *     Note: If KeyBlob_p == NULL and KeyBlobSize_p != NULL the required
 *           size will be returned with as return value VAL_BUFFER_TOO_SMALL.
 *
 * @param [in,out] PrivKeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519GenKeyPair(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const ValAssetId_Optional_t KekAssetId,
        ValOctetsIn_Optional_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValAsymBigInt_t * const PubKeyValue_p,
        ValOctetsOut_Optional_t * const PrivKeyBlob_p,
        ValSize_t * const PrivKeyBlobSize_p);


/*-----------------------------------------------------------------------------
 * val_AsymEccGenPublicKey
 */
/**
 * Generate an ECDH, ECDSA or ECC El-Gamal public key.
 *
 * An entity 'A's public key pair is associated with a particular set of
 * elliptic curve domain parameters, T = (p; a; b; G; n; h) and the entity
 * 'A's private key (d).
 *
 * To generate a public key, entity 'A' does the following:
 *
 * 1. Q = dG is computed.
 *
 * The maximum modulus value of PublicKey can be 521.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @param [out] PubKeyPoint_p
 *     Pointer to the buffer in which the public key (an ECC point) must be
 *     returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccGenPublicKey(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        ValAsymECCPoint_t * PubKeyPoint_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhGenPublicKey
 */
/**
 * Generate a DH (Key Exchange) public key as defined in the standard
 * NIST SP800-56A.
 *
 * An entity 'A's public key pair is associated with a particular set of
 * domain parameters, T = (p; q; g) and the entity 'A's private key (x).
 *
 * To generate a public key, entity 'A' does the following:
 *
 * 1. y = g^x mod p.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     DSA.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     DSA.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key (an big integer) must be
 *     returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhGenPublicKey(
    ValAsymKey_t * const PubKey_p,
    ValAsymKey_t * const PrivKey_p,
    ValAsymBigInt_t * PubKeyValue_p);

/*-----------------------------------------------------------------------------
 * val_AsymEddsaGenPublicKey
 */
/**
 * Generate a EdDSA public key as defined in:
 * - 'Edwards-curve Digital Signature Algorithm (EdDSA)'
 *   draft-irtf-cfrg-eddsa-08
 *
 * The public key is generated as follows:
 *
 * The public and private key pair is associated with a particular set of
 * elliptic curve domain parameters, T = (p; d(=a); const(=b); G; n; h).
 *
 * The Public key (y) is calculated according following steps:
 *   1. Hash private key -> H = SHA-512(private key)
 *   2. Derive k (prune hash) -> k = (H & (2^(256) -1) & ~(2^255 | 7)) | 2^254
 *   3. Perform a fixed-base scalar multiplication -> Y = [k]G
 *   4. Encode point to get the public key -> y = (Y.y % p) | ((Y.x & 1) << 254)
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     EdDSA.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     EdDSA.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key must be returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEddsaGenPublicKey(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        ValAsymBigInt_t * const PubKeyValue_p);

/*-----------------------------------------------------------------------------
 * val_AsymCurve25519GenPublicKey
 */
/**
 * Generate a Curve25519 public key.
 *
 * An entity 'A's public key pair is associated with a particular set of
 * elliptic curve domain parameters, T = (p; a; b; G; n; h) and the entity
 * 'A's private key (x).
 *
 * To generate a public key, entity 'A' does the following:
 *
 * 1. Q = xG is computed.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     Curve25519.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     Curve25519.
 *
 * @param [out] PubKeyValue_p
 *     Pointer to the buffer in which the public key must be returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519GenPublicKey(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        ValAsymBigInt_t * const PubKeyValue_p);


/*-----------------------------------------------------------------------------
 * val_AsymEccKeyCheck
 */
/**
 * This function checks the provided keys are a pair or if only the public
 * key is given a point on curve check is performed only.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH, ECDSA or ECC El-Gamal.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccKeyCheck(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p);

/*-----------------------------------------------------------------------------
 * val_AsymDhDsaKeyCheck
 */
/**
 * This function checks the provided keys are a pair or if only one
 * key (private or public) is given, the basic checks are performed.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for DH
 *     or DSA.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for DH
 *     or DSA.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhDsaKeyCheck(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p);


/*-----------------------------------------------------------------------------
 * val_AsymEcdhGenSharedSecretSingle
 */
/**
 * This function generates the shared secret based on a single key pair for the
 * Key Agreement Schemes as defined in NIST SP800-56A.
 * - ‘Ephemeral Unified Model’ = C(2e, 0s, ECC CDH)
 * - ‘One-Pass Diffie-Hellman’ = C(1e, 1s, ECC CDH)
 * - ‘Static Unified Model’ = C(0e, 2s, ECC CDH)
 *   Note that NonceU must be embedded in the ‘OtherInfo’ string (OtherInfo_p).
 *
 * The generated shared secret can either be used as key in the KDF as
 * specified in NIST SP800-56A section 5.8.1 (‘Single-step Key-Derivation
 * Function’) or saved to be used to perform a key derivation at a later stage.
 * The NIST SP800-56A KDF uses the SHA-256 hash algorithm and distributes the
 * generated data over each target Asset starting at a 4-Byte boundary. When
 * the shared secret is saved another key derivation scheme could be used.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH.
 *
 * @param [in] fSaveSharedSecret
 *     Indication that the shared secret must be saved in an Asset rather than
 *     used for the key derivation to initialize the provided target Assets.
 *     In case fSaveSharedSecret=true the number of target Assets shall be 1.
 *
 * @param [in] OtherInfo_p
 *     Pointer to the string (label) used by the Key Derivation Function.
 *
 * @param [in] OtherInfoSize
 *     Size of the string (label) used by the Key Derivation Function.
 *
 * @param [in] AssetIdList_p
 *     Pointer to the list of target Assets to fill.
 *
 * @param [in] NumberOfAssets
 *     The number of target Assets to fill.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdhGenSharedSecretSingle(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const bool fSaveSharedSecret,
        ValOctetsIn_Optional_t * const OtherInfo_p,
        const ValSize_t OtherInfoSize,
        ValAssetId_t * const AssetIdList_p,
        const ValSize_t NumberOfAssets);

/*-----------------------------------------------------------------------------
 * val_AsymEcdhGenSharedSecretDual
 */
/**
 * This function generates the shared secret based on a dual key pair for the
 * Key Agreement Schemes as defined in NIST SP800-56A.
 * - ‘Full Unified Model’ = C(2e, 2s, ECC CDH)
 * - ‘One-Pass Unified Model’ = C(1e, 2s, ECC CDH)
 *
 * The generated shared secret can either be used as key in the KDF as
 * specified in NIST SP800-56A section 5.8.1 (‘Single-step Key-Derivation
 * Function’) or saved to be used to perform a key derivation at a later stage.
 * The NIST SP800-56A KDF uses the SHA-256 hash algorithm and distributes the
 * generated data over each target Asset starting at a 4-Byte boundary. When
 * the shared secret is saved another key derivation scheme could be used.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH.
 *
 * @param [in] PubKey2_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECDH.
 *
 * @param [in] PrivKey2_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECDH.
 *
 * @param [in] fSaveSharedSecret
 *     Indication that the shared secret must be saved in an Asset rather than
 *     used for the key derivation to initialize the provided target Assets.
 *     In case fSaveSharedSecret=true the number of target Assets shall be 1.
 *
 * @param [in] OtherInfo_p
 *     Pointer to the string (label) used by the Key Derivation Function.
 *
 * @param [in] OtherInfoSize
 *     Size of the string (label) used by the Key Derivation Function.
 *
 * @param [in] AssetIdList_p
 *     Pointer to the list of target Assets to fill.
 *
 * @param [in] NumberOfAssets
 *     The number of target Assets to fill.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEcdhGenSharedSecretDual(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        ValAsymKey_t * const PubKey2_p,
        ValAsymKey_t * const PrivKey2_p,
        const bool fSaveSharedSecret,
        ValOctetsIn_Optional_t * const OtherInfo_p,
        const ValSize_t OtherInfoSize,
        ValAssetId_t * const AssetIdList_p,
        const ValSize_t NumberOfAssets);


/*-----------------------------------------------------------------------------
 * val_AsymDhGenSharedSecretSingle
 */
/**
 * This function generates the shared secret based on a single key pair for the
 * Key Agreement Schemes as defined in NIST SP800-56A.
 * - dhEphem   C(2e, 0s, FFC DH)
 * - dhOneFlow C(1e, 1s, FFC DH)
 * - dhStatic  C(0e, 2s, FFC DH)
 *   Note that NonceU must be embedded in the ‘OtherInfo’ string (OtherInfo_p).
 *
 * The generated shared secret can either be used as key in the KDF as
 * specified in NIST SP800-56A section 5.8.1 (‘Single-step Key-Derivation
 * Function’) or saved to be used to perform a key derivation at a later stage.
 * The NIST SP800-56A KDF uses the SHA-256 hash algorithm and distributes the
 * generated data over each target Asset starting at a 4-Byte boundary. When
 * the shared secret is saved another key derivation scheme could be used.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     DH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     DH.
 *
 * @param [in] fSaveSharedSecret
 *     Indication that the shared secret must be saved in an Asset rather than
 *     used for the key derivation to initialize the provided target Assets.
 *     In case fSaveSharedSecret=true the number of target Assets shall be 1.
 *
 * @param [in] OtherInfo_p
 *     Pointer to the string (label) used by the Key Derivation Function.
 *
 * @param [in] OtherInfoSize
 *     Size of the string (label) used by the Key Derivation Function.
 *
 * @param [in] AssetIdList_p
 *     Pointer to the list of target Assets to fill.
 *
 * @param [in] NumberOfAssets
 *     The number of target Assets to fill.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhGenSharedSecretSingle(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const bool fSaveSharedSecret,
        ValOctetsIn_Optional_t * const OtherInfo_p,
        const ValSize_t OtherInfoSize,
        ValAssetId_t * const AssetIdList_p,
        const ValSize_t NumberOfAssets);

/*-----------------------------------------------------------------------------
 * val_AsymDhGenSharedSecretDual
 */
/**
 * This function generates the shared secret based on a dual key pair for the
 * Key Agreement Schemes as defined in NIST SP800-56A.
 * - dhHybrid1       C(2e, 2s, FFC DH)
 * - dhHybridOneFlow C(1e, 2s, FFC DH)
 *
 * The generated shared secret can either be used as key in the KDF as
 * specified in NIST SP800-56A section 5.8.1 (‘Single-step Key-Derivation
 * Function’) or saved to be used to perform a key derivation at a later stage.
 * The NIST SP800-56A KDF uses the SHA-256 hash algorithm and distributes the
 * generated data over each target Asset starting at a 4-Byte boundary. When
 * the shared secret is saved another key derivation scheme could be used.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for DH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     DH.
 *
 * @param [in] PubKey2_p
 *     Pointer to the asymmetric crypto key structure of the public key for DH.
 *
 * @param [in] PrivKey2_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     DH.
 *
 * @param [in] fSaveSharedSecret
 *     Indication that the shared secret must be saved in an Asset rather than
 *     used for the key derivation to initialize the provided target Assets.
 *     In case fSaveSharedSecret=true the number of target Assets shall be 1.
 *
 * @param [in] OtherInfo_p
 *     Pointer to the string (label) used by the Key Derivation Function.
 *
 * @param [in] OtherInfoSize
 *     Size of the string (label) used by the Key Derivation Function.
 *
 * @param [in] AssetIdList_p
 *     Pointer to the list of target Assets to fill.
 *
 * @param [in] NumberOfAssets
 *     The number of target Assets to fill.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymDhGenSharedSecretDual(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        ValAsymKey_t * const PubKey2_p,
        ValAsymKey_t * const PrivKey2_p,
        const bool fSaveSharedSecret,
        ValOctetsIn_Optional_t * const OtherInfo_p,
        const ValSize_t OtherInfoSize,
        ValAssetId_t * const AssetIdList_p,
        const ValSize_t NumberOfAssets);


/*-----------------------------------------------------------------------------
 * val_AsymCurve25519GenSharedSecret
 */
/**
 * This function generates the shared secret as defined in RFC 7748 "Elliptic
 * Curves for Security" and Curve25519 "New Diffie-Hellman speed records".
 *
 * The generated shared secret can either be used as key in the KDF as
 * specified in NIST SP800-56A section 5.8.1 (‘Single-step Key-Derivation
 * Function’) or saved to be used to perform a key derivation at a later stage.
 * The NIST SP800-56A KDF uses the SHA-256 hash algorithm and distributes the
 * generated data over each target Asset starting at a 4-Byte boundary. When
 * the shared secret is saved another key derivation scheme could be used.
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     Curve25519.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     Curve25519.
 *
 * @param [in] fSaveSharedSecret
 *     Indication that the shared secret must be saved in an Asset rather than
 *     used for the key derivation to initialize the provided target Assets.
 *     In case fSaveSharedSecret=true the number of target Assets shall be 1.
 *
 * @param [in] OtherInfo_p
 *     Pointer to the the ‘OtherInfo’ string used by the Key Derivation
 *     Function.
 *
 * @param [in] OtherInfoSize
 *     The ‘OtherInfo’ string  length.
 *
 * @param [in] AssetIdList_p
 *     Pointer to a list of Asset Id's that need to be loaded.
 *
 * @param [in] NumberOfAssets
 *     The number of Assets in the list.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymCurve25519GenSharedSecret(
        ValAsymKey_t * const PubKey_p,
        ValAsymKey_t * const PrivKey_p,
        const bool fSaveSharedSecret,
        ValOctetsIn_Optional_t * const OtherInfo_p,
        const ValSize_t OtherInfoSize,
        ValAssetId_t * const AssetIdList_p,
        const ValSize_t NumberOfAssets);


/*-----------------------------------------------------------------------------
 * val_AsymEccElGamalEncrypt
 */
/**
 * This function is used for encrypting a message point. The encryption is
 * done according the ECC El-Gamal en/decryption scheme.
 * The supported ECC curves vary from 192 bits to 521 bits.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     ECC El-Gamal.
 *
 * @param [in] MessagePoint_p
 *     Pointer to the message point that must be encrypted.
 *
 * @param [out] EncryptedPoint1_p
 *     Pointer to the buffer in which the encrypted message point part 1 must
 *     be returned.
 *
 * @param [out] EncryptedPoint2_p
 *     Pointer to the buffer in which the encrypted message point part 2 must
 *     be returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccElGamalEncrypt(
        ValAsymKey_t * const KeyContext_p,
        ValAsymECCPoint_t * const MessagePoint_p,
        ValAsymECCPoint_t * const EncryptedPoint1_p,
        ValAsymECCPoint_t * const EncryptedPoint2_p);

/*-----------------------------------------------------------------------------
 * val_AsymEccElGamalDecrypt
 */
/**
 * This function is used for decrypting an encrypted message point. The
 * decryption is done according the ECC El-Gamal en/decryption scheme.
 * The supported ECC curves vary from 192 bits to 521 bits.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the private key for
 *     ECC El-Gamal.
 *
 * @param [in] EncryptedPoint1_p
 *     Pointer to the encrypted message point part 1 that must be decrypted.
 *
 * @param [in] EncryptedPoint2_p
 *     Pointer to the encrypted message point part 2 that must be decrypted.
 *
 * @param [out] MessagePoint_p
 *     Pointer to the buffer in which the decrypted message point must be
 *     returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymEccElGamalDecrypt(
        ValAsymKey_t * const KeyContext_p,
        ValAsymECCPoint_t * const EncryptedPoint1_p,
        ValAsymECCPoint_t * const EncryptedPoint2_p,
        ValAsymECCPoint_t * const MessagePoint_p);


/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepAllocPrivateKeyAsset
 */
/**
 * This function allocates a RSA OAEP private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepAllocPublicKeyAsset
 */
/**
 * This function allocates a RSA OAEP public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] HashAlgorithm
 *     The related hash algorithm to use.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const ValSymAlgo_t HashAlgorithm,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);


/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepWrapString
 */
/**
 * This function wraps an Asset using the RSA-OAEP method, as defined in NIST
 * SP800-56B. This function uses a string as Additional Input.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-OAEP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [in] AdditionalInput_p
 *     Pointer to the string to use as Additional Input.
 *
 * @param [in] AdditionalInputSize
 *     The size of the string to use as Additional Input.
 *
 * @param [out] WrappedData_p
 *     Pointer to the buffer in which the wrapped data needs to be written.
 *
 * @param [in,out] WrappedDataSize_p
 *     Pointer to the size of data buffer, upon return it is updated with
 *     the actual size of the wrapped data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepWrapString(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const AdditionalInput_p,
        const ValSize_t AdditionalInputSize,
        ValOctetsOut_t * const WrappedData_p,
        ValSize_t * const WrappedDataSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepWrapHash
 */
/**
 * This function wraps an Asset using the RSA-OAEP method, as defined in NIST
 * SP800-56B. This function uses a hash digest as Additional Input.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-OAEP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [in] AdditionalInput_p
 *     Pointer to the hash digest to use as Additional Input.
 *
 * @param [in] AdditionalInputSize
 *     The size of the hash digest to use as Additional Input.
 *
 * @param [out] WrappedData_p
 *     Pointer to the buffer in which the wrapped data needs to be written.
 *
 * @param [in,out] WrappedDataSize_p
 *     Pointer to the size of data buffer, upon return it is updated with
 *     the actual size of the wrapped data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepWrapHash(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const AdditionalInput_p,
        const ValSize_t AdditionalInputSize,
        ValOctetsOut_t * const WrappedData_p,
        ValSize_t * const WrappedDataSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepUnwrapString
 */
/**
 * This function unwraps RSA-OAEP wrapped data and loads (initializes) an Asset
 * with it. The RSA-OAEP method as defined in NIST SP800-56B. This function
 * uses a string as Additional Input.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-OAEP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [in] AdditionalInput_p
 *     Pointer to the string to use as Additional Input.
 *
 * @param [in] AdditionalInputSize
 *     The size of the string to use as Additional Input.
 *
 * @param [in] WrappedData_p
 *     Pointer to the buffer with the wrapped data for the Asset.
 *
 * @param [in] WrappedDataSize
 *     The size of the wrapped data for the Asset.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepUnwrapString(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const AdditionalInput_p,
        const ValSize_t AdditionalInputSize,
        ValOctetsIn_t * const WrappedData_p,
        const ValSize_t WrappedDataSize);

/*-----------------------------------------------------------------------------
 * val_AsymRsaOaepUnwrapHash
 */
/**
 * This function unwraps RSA-OAEP wrapped data and loads (initializes) an Asset
 * with it. The RSA-OAEP method as defined in NIST SP800-56B. This function
 * uses a hash digest as Additional Input.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-OAEP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [in] AdditionalInput_p
 *     Pointer to the hash digest to use as Additional Input.
 *
 * @param [in] AdditionalInputSize
 *     The size of the hash digest to use as Additional Input.
 *
 * @param [in] WrappedData_p
 *     Pointer to the buffer with the wrapped data for the Asset.
 *
 * @param [in] WrappedDataSize
 *     The size of the wrapped data for the Asset.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaOaepUnwrapHash(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const AdditionalInput_p,
        const ValSize_t AdditionalInputSize,
        ValOctetsIn_t * const WrappedData_p,
        const ValSize_t WrappedDataSize);


/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15WrapAllocPrivateKeyAsset
 */
/**
 * This function allocates a RSAES-PKCS1-v1_5 private key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15WrapAllocPrivateKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);

/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15WrapAllocPublicKeyAsset
 */
/**
 * This function allocates a RSAES-PKCS1-v1_5 public key Asset.
 *
 * @param [in] ModulusSizeBits
 *     Size of the modulus in bits.
 *
 * @param [in] ExponentSizeBits
 *     Size of the exponent in bits.
 *
 * @param [in] fCrossDomain
 *     Indication to allow cross domain use.
 *
 * @param [in] fExportAllowed
 *     Indication to allow asset export.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written on
 *     success.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15WrapAllocPublicKeyAsset(
        const ValSize_t ModulusSizeBits,
        const ValSize_t ExponentSizeBits,
        const bool fCrossDomain,
        const bool fExportAllowed,
        ValAssetId_t * const AssetId_p);


/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15Wrap
 */
/**
 * This function wraps an Asset using the RSAES-PKCS1-v1_5 encryption scheme,
 * as defined in PKCS#1.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-PKCS1-V15-WRAP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [out] WrappedData_p
 *     Pointer to the buffer in which the wrapped data needs to be written.
 *
 * @param [in,out] WrappedDataSize_p
 *     Pointer to the size of data buffer, upon return it is updated with
 *     the actual size of the wrapped data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15Wrap(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsOut_t * const WrappedData_p,
        ValSize_t * const WrappedDataSize_p);


/*-----------------------------------------------------------------------------
 * val_AsymRsaPkcs1v15Unwrap
 */
/**
 * This function unwraps RSAES-PKCS1-v1_5 wrapped data and loads (initializes)
 * an Asset with it. The RSAES-PKCS1-v1_5 encryption scheme is defined in PKCS#1.
 *
 * @param [in] KeyContext_p
 *     Pointer to the asymmetric crypto key structure of the public key for
 *     RSA-PKCS1-V15-WRAP.
 *
 * @param [in] AssetId
 *     Reference to the Asset to wrap.
 *
 * @param [in] WrappedData_p
 *     Pointer to the buffer with the wrapped data for the Asset.
 *
 * @param [in] WrappedDataSize
 *     The size of the wrapped data for the Asset.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymRsaPkcs1v15Unwrap(
        ValAsymKey_t * const KeyContext_p,
        const ValAssetId_t AssetId,
        ValOctetsIn_t * const WrappedData_p,
        const ValSize_t WrappedDataSize);


/*-----------------------------------------------------------------------------
 * val_AsymPkaClaim
 */
/**
 * This function claims the PKA (PKCP) for a sequence of operations.
 *
 * @param [in] Nwords
 *     Number of 32-bit words, which specifies a default vector size.
 *
 * @param [in] Mwords
 *     Number of 32-bit words, which specifies an alternate vector size.
 *
 * @param [in] Mmask
 *     Bitmask specifying (with 1b’s) which of the first 8 vectors
 *     that are loaded via val_AsymPkaLoadVector() are Mwords.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymPkaClaim(
        const uint8_t Nwords,
        const uint8_t Mwords,
        const uint8_t Mmask);

/*-----------------------------------------------------------------------------
 * val_AsymPkaLoadVector
 */
/**
 * This function loads a PKA vector at the specified location in PKA memory.
 *
 * @param [in] Index
 *     The location index where the vector must be loaded [0..15].
 *
 * @param [in] Vector_p
 *     Pointer to the vector data.
 *
 * @param [in] VectorSize
 *     The size of the vector data in bytes.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymPkaLoadVector(
        const uint8_t Index,
        ValOctetsIn_t * const Vector_p,
        const ValSize_t VectorSize);

/*-----------------------------------------------------------------------------
 * val_AsymPkaExecuteOperation
 */
/**
 * This function claims the PKA (PKCP) for a sequence of operations.
 *
 * @param [in] Operation
 *     PKA operation to execute.
 *
 * @param [in] PublicExponent
 *     Public Exponent, only applicable for the ModExpE operation.
 *
 * @param [in] InData_p
 *     Pointer to the input data.
 *
 * @param [in] InDataSize
 *     The size of the input data in bytes.
 *
 * @param [out] OutData_p
 *     Pointer to the buffer, where the output data must be written
 *
 * @param [in,out] OutDataSize_p
 *     The size of the output data buffer in bytes. Note that the size will be
 *     updated.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymPkaExecuteOperation(
        const ValAsymPkaOperation_t Operation,
        const uint32_t PublicExponent,
        ValOctetsIn_t * const InData_p,
        const ValSize_t InDataSize,
        ValOctetsOut_t * const OutData_p,
        ValSize_t * const OutDataSize_p);

/*-----------------------------------------------------------------------------
 * val_AsymPkaClaimRelease
 */
/**
 * This function releases the PKA (PKCP) claim.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AsymPkaClaimRelease(void);


#endif /* Include Guard */


/* end of file api_val_asym.h */
