/**
 * @file api_val_asset.h
 *
 * @brief VAL API - Asset Management services
 */

/*****************************************************************************
* Copyright (c) 2014-2019 INSIDE Secure B.V. All Rights Reserved.
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

#ifndef INCLUDE_GUARD_API_VAL_ASSET_H
#define INCLUDE_GUARD_API_VAL_ASSET_H

/*----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */
#include "basic_defs.h"             // uint8_t, uint16_t, uint32_t, uint64_t

#include "api_val_result.h"         // ValStatus_t
#include "api_val_buffers.h"        // ValOctetsIn_t, ValOctetsOut_t, ValSize_t


/*----------------------------------------------------------------------------
 * Definitions and macros
 */
/** Switch to enable Asset Lifetime Management
  * The setting of this define depends on the system/firmware configuration.
  * Uncomment this define only if Asset Lifetime functionality is available.
  */
//#define API_VAL_ASSET_LIFETIME_MANAGEMENT

/** Asset Identifier, refers to an Asset in the Asset Store.\n
 *  The Asset Identifier (AssetId) value is implementation specific and its
 *  value should not be used directly, except for comparing against
 *  VAL_ASSETID_INVALID. */
typedef uint32_t ValAssetId_t;
/** Optional Asset Identifier. Refers to an Asset in the Asset Store or has
 *  the value VAL_ASSETID_INVALID. */
typedef ValAssetId_t ValAssetId_Optional_t;

/** Guaranteed unused AssetId value */
#define VAL_ASSETID_INVALID  0


/** Asset Number, refers to a non-volatile Asset number.\n
 *  The AssetNumber is system specific and is used for identifying a Static
 *  Asset or (Static) Public Data object. */
typedef uint16_t ValAssetNumber_t;


/** The maximum value for the AssetSize parameter used in this API.\n
 *  Note: DH/DSA Domain parameters are taken as maximum reference */
#define VAL_ASSET_SIZE_MAX   (((3 * 32) + 3072 + 3072 + 256) / 8)


/** The maximum value for the AssetNumber parameter used in this API. */
#define VAL_ASSET_NUMBER_MAX   40

/** The maximum value for the PolicyNumber parameter used in this API. */
#define VAL_POLICY_NUMBER_MAX   30


/** The minimum length of the label used in the Key Derivation Function (KDF). */
#define VAL_KDF_LABEL_MIN_SIZE       53

/** The maximum length of the label used in the Key Derivation Function (KDF). */
#define VAL_KDF_LABEL_MAX_SIZE       (224 - 20)


/** The minimum length of the label used in the AES-SIV keyblob export/import. */
#define VAL_KEYBLOB_AAD_MIN_SIZE     33

/** The maximum length of the label used in the AES-SIV keyblob export/import. */
#define VAL_KEYBLOB_AAD_MAX_SIZE     224


/** The expected size of an AES-SIV keyblob.\n
 *  Note: AssetSize is the size of the Asset in octects (bytes). */
#ifdef API_VAL_ASSET_LIFETIME_MANAGEMENT
#define VAL_KEYBLOB_SIZE(AssetSize) ((128/8) + AssetSize + (32/8))
#else
#define VAL_KEYBLOB_SIZE(AssetSize) ((128/8) + AssetSize)
#endif
#define VAL_OTP_KEYBLOB_SIZE(AssetSize) ((128/8) + AssetSize)

typedef enum
{
    VAL_ASSET_LIFETIME_INFINITE = 0,        /* Not used */
#ifdef API_VAL_ASSET_LIFETIME_MANAGEMENT
    VAL_ASSET_LIFETIME_SECONDS,
    VAL_ASSET_LIFETIME_MILLISECONDS,
    VAL_ASSET_LIFETIME_COUNTER,
#endif
} ValAssetLifetimeUse_t;

/** Asset Policy is a bitmask that defines the type and use of an Asset.\n
 *  The VAL_POLICY_* defines should be used to construct the Asset Policy
 *  bitmask.\n
 *  Note: The Asset Policy limitations depend on the implementation. So, please
 *        check the documentation. */
typedef uint64_t ValPolicyMask_t;

/** Asset policies related to hash/HMAC and CMAC algorithms */
#define VAL_POLICY_SHA1                       0x0000000000000001ULL
#define VAL_POLICY_SHA224                     0x0000000000000002ULL
#define VAL_POLICY_SHA256                     0x0000000000000004ULL
#define VAL_POLICY_SHA384                     0x0000000000000008ULL
#define VAL_POLICY_SHA512                     0x0000000000000010ULL
#define VAL_POLICY_CMAC                       0x0000000000000020ULL
#define VAL_POLICY_POLY1305                   0x0000000000000040ULL

/** Asset policies related to symmetric cipher algorithms */
#define VAL_POLICY_ALGO_CIPHER_MASK           0x0000000000000300ULL
#define VAL_POLICY_ALGO_CIPHER_AES            0x0000000000000100ULL
#define VAL_POLICY_ALGO_CIPHER_TRIPLE_DES     0x0000000000000200ULL
#define VAL_POLICY_ALGO_CIPHER_CHACHA20       0x0000000000002000ULL
#define VAL_POLICY_ALGO_CIPHER_SM4            0x0000000000004000ULL
#define VAL_POLICY_ALGO_CIPHER_ARIA           0x0000000000008000ULL

/** Asset policies related to symmetric cipher modes */
#define VAL_POLICY_MODE1                      0x0000000000010000ULL
#define VAL_POLICY_MODE2                      0x0000000000020000ULL
#define VAL_POLICY_MODE3                      0x0000000000040000ULL
#define VAL_POLICY_MODE4                      0x0000000000080000ULL
#define VAL_POLICY_MODE5                      0x0000000000100000ULL
#define VAL_POLICY_MODE6                      0x0000000000200000ULL
#define VAL_POLICY_MODE7                      0x0000000000400000ULL
#define VAL_POLICY_MODE8                      0x0000000000800000ULL
#define VAL_POLICY_MODE9                      0x0000000001000000ULL
#define VAL_POLICY_MODE10                     0x0000000002000000ULL

/** Asset policies specialized per symmetric cipher algorithm */
#define VAL_POLICY_AES_MODE_ECB               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE1)
#define VAL_POLICY_AES_MODE_CBC               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE2)
#define VAL_POLICY_AES_MODE_CTR               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE4)
#define VAL_POLICY_AES_MODE_CTR32             (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE4)
#define VAL_POLICY_AES_MODE_ICM               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE5)
#define VAL_POLICY_AES_MODE_CCM               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE7|VAL_POLICY_CMAC)
#define VAL_POLICY_AES_MODE_F8                (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE8)
#define VAL_POLICY_AES_MODE_XTS               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE9)
#define VAL_POLICY_AES_MODE_GCM               (VAL_POLICY_ALGO_CIPHER_AES|VAL_POLICY_MODE10)

#define VAL_POLICY_3DES_MODE_ECB              (VAL_POLICY_ALGO_CIPHER_TRIPLE_DES|VAL_POLICY_MODE1)
#define VAL_POLICY_3DES_MODE_CBC              (VAL_POLICY_ALGO_CIPHER_TRIPLE_DES|VAL_POLICY_MODE2)

#define VAL_POLICY_CHACHA20_ENCRYPT           (VAL_POLICY_ALGO_CIPHER_CHACHA20)
#define VAL_POLICY_CHACHA20_AEAD              (VAL_POLICY_ALGO_CIPHER_CHACHA20|VAL_POLICY_POLY1305)

#define VAL_POLICY_SM4_MODE_ECB               (VAL_POLICY_ALGO_CIPHER_SM4|VAL_POLICY_MODE1)
#define VAL_POLICY_SM4_MODE_CBC               (VAL_POLICY_ALGO_CIPHER_SM4|VAL_POLICY_MODE2)
#define VAL_POLICY_SM4_MODE_CTR               (VAL_POLICY_ALGO_CIPHER_SM4|VAL_POLICY_MODE4)

#define VAL_POLICY_ARIA_MODE_ECB              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE1)
#define VAL_POLICY_ARIA_MODE_CBC              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE2)
#define VAL_POLICY_ARIA_MODE_CTR              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE4)
#define VAL_POLICY_ARIA_MODE_CTR32            (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE4)
#define VAL_POLICY_ARIA_MODE_ICM              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE5)
#define VAL_POLICY_ARIA_MODE_CCM              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE7|VAL_POLICY_CMAC)
#define VAL_POLICY_ARIA_MODE_GCM              (VAL_POLICY_ALGO_CIPHER_ARIA|VAL_POLICY_MODE10)

/** Asset policies related to Algorithm/cipher/MAC operations */
#define VAL_POLICY_MAC_GENERATE               0x0000000004000000ULL
#define VAL_POLICY_MAC_VERIFY                 0x0000000008000000ULL
#define VAL_POLICY_ENCRYPT                    0x0000000010000000ULL
#define VAL_POLICY_DECRYPT                    0x0000000020000000ULL

/** Asset policies related to temporary values
 *  Note that the VAL_POLICY_TEMP_MAC should be used for intermediate
 *  hash digest as well. */
#define VAL_POLICY_TEMP_IV                    0x0001000000000000ULL
#define VAL_POLICY_TEMP_COUNTER               0x0002000000000000ULL
#define VAL_POLICY_TEMP_MAC                   0x0004000000000000ULL
#define VAL_POLICY_TEMP_AUTH_STATE            0x0010000000000000ULL

/** Asset policy related to monotonic counter */
#define VAL_POLICY_MONOTONIC                  0x0000000100000000ULL

/** Asset policies related to key derive functionality */
#define VAL_POLICY_TRUSTED_ROOT_KEY           0x0000000200000000ULL
#define VAL_POLICY_TRUSTED_KEY_DERIVE         0x0000000400000000ULL
#define VAL_POLICY_KEY_DERIVE                 0x0000000800000000ULL

/** Asset policies related to AES key wrap functionality\n
 *  Note: Must be combined with operations bits */
#define VAL_POLICY_TRUSTED_WRAP               0x0000001000000000ULL
#define VAL_POLICY_AES_WRAP                   0x0000002000000000ULL

/** Asset policies related to PK operations */
#define VAL_POLICY_PUBLIC_KEY                 0x0000000080000000ULL
#define VAL_POLICY_PK_RSA_OAEP_WRAP           0x0000004000000000ULL
#define VAL_POLICY_PK_RSA_PKCS1_WRAP          0x0000010000000000ULL
#define VAL_POLICY_PK_RSA_PKCS1_SIGN          0x0000020000000000ULL
#define VAL_POLICY_PK_RSA_PSS_SIGN            0x0000040000000000ULL
#define VAL_POLICY_PK_DSA_SIGN                0x0000080000000000ULL
#define VAL_POLICY_PK_ECC_ECDSA_SIGN          0x0000100000000000ULL
#define VAL_POLICY_PK_DH_KEY                  0x0000200000000000ULL
#define VAL_POLICY_PK_ECDH_KEY                0x0000400000000000ULL
#define VAL_POLICY_PUBLIC_KEY_PARAM           0x0000800000000000ULL

#define VAL_POLICY_PK_ECC_ELGAMAL_ENC         (VAL_POLICY_PK_ECC_ECDSA_SIGN|VAL_POLICY_PK_ECDH_KEY)

/** Asset policies related to Authentication */
#define VAL_POLICY_EMMC_AUTH_KEY              0x0400000000000000ULL
#define VAL_POLICY_AUTH_KEY                   0x8000000000000000ULL

/** Asset policies related to the domain */
#define VAL_POLICY_SOURCE_NON_SECURE          0x0100000000000000ULL
#define VAL_POLICY_CROSS_DOMAIN               0x0200000000000000ULL

/** Asset policies related to general purpose data that can or must be used
 *  in an operation */
#define VAL_POLICY_PRIVATE_DATA               0x0800000000000000ULL
#define VAL_POLICY_PUBLIC_DATA                0x1000000000000000ULL

/** Asset policies related to export functionality */
#define VAL_POLICY_EXPORT                     0x2000000000000000ULL
#define VAL_POLICY_TRUSTED_EXPORT             0x4000000000000000ULL


/*-----------------------------------------------------------------------------
 * val_AssetAlloc
 */
/**
 * This function allocates an Asset entry in the (Dynamic) Asset Store,
 * allowing it to be filled with a cryptographic secret (typically a key).
 * The Asset Policy, Size and Ownership (Host and Identity) are set and cannot
 * be changed afterwards.
 *
 * Note: To avoid running out of space in the Asset Store, implementations can
 *       choose to discard assets from the Asset Store when an application has
 *       terminated.
 *
 * @param [in] AssetPolicy
 *      Bitmask (based on VAL_POLICY_*) that defines the asset policy.
 *
 * @param [in] AssetSize
 *      The exact size of the asset, in bytes.
 *
 * @param [in] fAllHosts
 *      Indication that all Hosts may use the Asset.
 *
 * @param [in] fRemoveSecure
 *      Set indication to remove Secure from the Asset Installer Domain when
 *      using the Asset for an operation.
 *
 * @param [in] LifetimeUse
 *      Asset lifetime use setting.
 *
 * @param [in] fLifetimeNoLoad
 *      Indication that the Asset lifetime should be loaded during load operation.
 *
 * @param [in] fLifetimeRelative
 *      Indication that the Asset lifetime is relative.
 *
 * @param [in] Lifetime
 *      The Asset lifetime. The actual value depends on its use.
 *
 * @param [out] AssetId_p
 *      Pointer to the memory location where the AssetId will be written when
 *      the entry was successfully allocated. The AssetId is needed for asset
 *      related operation with this Asset.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetAlloc(
        const ValPolicyMask_t AssetPolicy,
        const ValSize_t AssetSize,
        const bool fAllHosts,
        const bool fRemoveSecure,
        const ValAssetLifetimeUse_t LifetimeUse,
        const bool fLifetimeNoLoad,
        const bool fLifetimeRelative,
        const uint32_t Lifetime,
        ValAssetId_t * const AssetId_p);


/*-----------------------------------------------------------------------------
 * val_AssetFree
 */
/**
 * This function removes an Asset from the Asset Store, freeing up the space
 * it occupies and invalidating the corresponding AssetId.
 *
 * Note: To avoid running out of space in the Asset Store, implementations can
 *       choose to discard assets from the Asset Store when an application has
 *       terminated.
 *
 * @param [in] AssetId
 *      Reference to the Asset to remove from the Asset Store.
 *      This reference is provided by val_AssetAlloc().
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetFree(
        const ValAssetId_t AssetId);


/*-----------------------------------------------------------------------------
 * val_AssetSearch
 */
/**
 * This function can be used to get an AssetId for static Asset or static
 * Public Data object that is typically provisioned for example during the
 * chip production.
 *
 * @param [in] StaticAssetNumber
 *     The asset number to search for in the (static) Asset Store. Note that
 *     the supported values for StaticAssetNumber are defined by the device
 *     manufacturer and vendor and can differ from product to product.
 *
 * @param [out] AssetId_p
 *     Pointer to the memory location where the AssetId will be written when
 *     the Asset was found. The AssetId is needed to use the asset.
 *     It is not required to free the AssetId using val_asset_free().
 *
 * @param [out] AssetSize_p
 *     If not NULL the size of the asset, in bytes, is returned.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetSearch(
        const ValAssetNumber_t StaticAssetNumber,
        ValAssetId_t * const AssetId_p,
        ValSize_t * const AssetSize_p);


/*-----------------------------------------------------------------------------
 * val_AssetGetRootKey
 */
/**
 * This helper function returns the AssetId of the RootKey available to this
 * application. The RootKey can be used in Trusted Derive operations.
 * This function cannot fail. The AssetId for the RootKey is constant and
 * cached by the implementation.
 */
ValAssetId_t
val_AssetGetRootKey(void);


/*-----------------------------------------------------------------------------
 * For next functions, the definition of a Key Blob
 *
 * A Key Blob is a cryptographically protected block of data with an Asset
 * inside. It is generated and returned immediately after an asset was loaded
 * plaintext or randomly generated. For asymmetric keys (private and/or public
 * keys) the generation function can also generate a Key Blob. These are the
 * only possiblity to get a key blob as it cannot be generated on request.
 * The keyblob can be imported into the Asset Store at a later time, for
 * example after a power cycle.
 */

/*-----------------------------------------------------------------------------
 * val_AssetLoadImport
 */
/**
 * This function imports an Asset previously exported in a Keyblob.
 * The following parameters must be identical to when the Keyblob was
 * generated, otherwise it will not be imported:
 *   - Asset Ownership
 *   - Asset Policy
 *   - Asset Size
 *   - KEK used
 *   - Additional Data
 * The first three were set when val_AssetAlloc() was invoked. The other
 * two paremeters must be provide via this function.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be imported with the keyblob.
 *     This reference is provided by val_AssetAlloc().
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same Associated Data must be provided
 *     when generating and importing a keyblob.
 *
 * @param [in] KeyBlob_p
 * @param [in] KeyBlobSize
 *     Pointer to and size of the keyblob to import.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadImport(
        const ValAssetId_t TargetAssetId,
        const ValAssetId_t KekAssetId,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsIn_t * const KeyBlob_p,
        const ValSize_t KeyBlobSize);


/*-----------------------------------------------------------------------------
 * val_AssetLoadDerive
 */
/**
 * This function allows one Asset to be derived from another Asset using the
 * built-in Key Derivation Function (KDF). The target Asset must have been
 * allocated, with ownership, policy and length defined, but not yet filled
 * with data.
 *
 * Note: The Key Derivation method use is defined by fCounter and fRFC5869.
 *       fCounter  fRFC5869
 *       false     false        NIST SP800-108 KDF in Feedback Mode
 *       true      false        NIST SP800-108 KDF in Counter Mode
 *       false     true         RFC5869 KDF
 *       true      true         Invalid
 *
 * It is not possible nor required to get a keyblob since the derive operation
 * is repeatable: it produces in the same key after every power cycle.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be derived.
 *     This reference is provided by val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KdkAssetId
 *     Reference to the KDK Asset to be used when deriving an Asset.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer-to and size-in-bytes of the Associated Data to use in the KDF.
 *     The Associated Data, together with other properties of the asset, helps
 *     ensuring the derived asset is unique.
 *
 * @param [in] fCounter
 *     When set, the key derivation is performed with the KDF in counter mode
 *     as defined in NIST SP800-108. See above-mentioned note.
 *
 * @param [in] fRFC5869
 *     When set, the key derivation is performed as defined in RFC 5869.
 *     See above-mentioned note.
 *
 * @param [in] Salt_p
 * @param [in] SaltSize
 *     Pointer-to and size-in-bytes of the Salt to use in the KDF Randomness
 *     Extraction step.
 *     Note: Salt_p maybe NULL, in that case only the KDF Key Expansion step
 *           is performed.
 *
 * @param [in] IV_p
 * @param [in] IVSize
 *     Pointer-to and size-in-bytes of the IV to use in the KDF Key Expansion
 *     step.
 *     Note: IV_p maybe NULL.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadDerive(
        const ValAssetId_t TargetAssetId,
        const ValAssetId_t KdkAssetId,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        const bool fCounter,
        const bool fRFC5869,
        ValOctetsIn_Optional_t * const Salt_p,
        const ValSize_t SaltSize,
        ValOctetsIn_Optional_t * const IV_p,
        const ValSize_t IVSize);


/*-----------------------------------------------------------------------------
 * val_AssetLoadPlaintext
 */
/**
 * This function can be used to load a plaintext secret into the Asset Store.
 * The Asset, with ownership, policy and size, must have been allocated using
 * val_AssetAlloc().
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded.
 *     This reference is provided by val_AssetAlloc().
 *
 * @param [in] Data_p
 *     Pointer to the plaintext data to be loaded into the Asset.
 *
 * @param [in] DataSize
 *     Number of data bytes pointed to by Data_p.
 *     This value must match exactly the size of the asset as provided to
 *     val_AssetAlloc().
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadPlaintext(
        const ValAssetId_t TargetAssetId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize);


/*-----------------------------------------------------------------------------
 * val_AssetLoadPlaintextExport
 */
/**
 * This function is a variant of val_AssetLoadPlaintext - see the description
 * elsewhere in this file. Apart from performing that function, it also
 * generates and exports a Keyblob.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded.
 *     This reference is provided by val_AssetAlloc().
 *
 * @param [in] Data_p
 *     Pointer to the plaintext data to be loaded into the Asset.
 *
 * @param [in] DataSize
 *     Number of data bytes pointed to by Data_p.
 *     This value must match exactly the size of the asset as provided to
 *     val_AssetAlloc().
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same Associated Data must be provided when
 *     generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer.
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadPlaintextExport(
        const ValAssetId_t TargetAssetId,
        ValOctetsIn_t * const Data_p,
        const ValSize_t DataSize,
        const ValAssetId_t KekAssetId,
        ValOctetsIn_t * const  AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);


/*-----------------------------------------------------------------------------
 * val_AssetLoadRandom
 */
/**
 * This function allows an Asset to be filled with random data. Since the
 * Random Number Generator is directly accessible to the Asset Store, this
 * operation allows generating a truly secret key that is only known inside
 * the Asset Store.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with random data.
 *     This reference is provided by val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadRandom(
        const ValAssetId_t TargetAssetId);


/*-----------------------------------------------------------------------------
 * val_AssetLoadRandomExport
 */
/**
 * This function is a variant of val_AssetLoadRandom - see the description
 * elsewhere in this file. Apart from performing that function, it also
 * generates and exports a Keyblob.
 *
 * @param [in] TargetAssetId
 *     Reference to the Asset that needs to be loaded with random data.
 *     This reference is provided by val_AssetAlloc().
 *     Note: The Asset contains the desired length of the Asset.
 *
 * @param [in] KekAssetId
 *     Reference to the KEK Asset to be used when generating the keyblob.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication. The exact same Associated Data must be provided when
 *     generating and importing a keyblob.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the keyblob buffer
 *
 * @param [in,out] KeyBlobSize_p
 *     Size of the buffer intended for the keyblob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadRandomExport(
        const ValAssetId_t TargetAssetId,
        const ValAssetId_t KekAssetId,
        ValOctetsIn_t * const AssociatedData_p,
        ValSize_t AssociatedDataSize,
        ValOctetsOut_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);


/*-----------------------------------------------------------------------------
 * val_AssetLoadAesunwrap
 */
/**
 * This function imports an AES (key) wrapped asset.
 *
 * @param [in] TargetAssetId
 *     The asset to import the keyblob into.
 *     This number must have been returned by val_AssetAlloc().
 *
 * @param [in] KekAssetId
 *     The Asset ID of the Key Encryption Key that must be used
 *
 * @param [in] KeyBlob_p
 *     The KeyBlob which contains the an AES wrapped Asset
 *
 * @param [in] KeyBlobSize
 *     The size of the KeyBlob.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_AssetLoadAesunwrap(
        const ValAssetId_t TargetAssetId,
        const ValAssetId_t KekAssetId,
        ValOctetsIn_t * const KeyBlob_p,
        const ValSize_t KeyBlobSize);


/*-----------------------------------------------------------------------------
 * val_PublicDataRead
 */
/**
 * This function reads the Public Data.
 *
 * @param [in] AssetId
 *     The Asset Id of the Public Data to read.
 *
 * @param [out] Data_p
 *     The pointer to the data buffer in which the Public Data must be
 *     returned.
 *
 * @param [in] DataSize
 *     The size of the data buffer.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_PublicDataRead(
        const ValAssetId_t AssetId,
        ValOctetsOut_t * const Data_p,
        const ValSize_t DataSize);


/*-----------------------------------------------------------------------------
 * val_MonotonicCounterRead
 */
/**
 * This function reads the Monotonic Counter value.
 *
 * @param [in] AssetId
 *     The Asset Id of the Monotonic Counter to read.
 *
 * @param [out] Data_p
 *     The pointer to the data buffer in which the Monotonic Counter value
 *     must be returned.
 *
 * @param [in] DataSize
 *     The size of the data buffer.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_MonotonicCounterRead(
        const ValAssetId_t AssetId,
        ValOctetsOut_t * const Data_p,
        const ValSize_t DataSize);


/*-----------------------------------------------------------------------------
 * val_MonotonicCounterIncrement
 */
/**
 * This function increments a Monotonic Counter.
 *
 * @param [in] AssetId
 *     The Asset Id of the Monotonic Counter to increment.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_MonotonicCounterIncrement(
        const ValAssetId_t AssetId);


/*-----------------------------------------------------------------------------
 * val_OTPDataWrite
 */
/**
 * This function writes a Static Asset or Public data object to OTP.
 * Note that a Monotonic Counter can be initialized as well with this function.
 *
 * @param [in] StaticAssetNumber
 *     The asset number of the Static Asset or Public data object to write.
 *     Range [0-63].
 *
 * @param [in] AssetPolicyNumber
 *     The number of the policy with which the Static Asset or Public data
 *     object needs to be associated.
 *     For example possible AssetPolicyNumber are:
 *     0    Monotonic counter
 *     1    Trusted-Root-Key (HUK)
 *     2    AES Unwrap key, intended for Secure Boot
 *     3    Trusted-Key-Derive key, intended for Secure Boot
 *     4    Public Data object
 *     6    Authentication Key for eMMC
 *     24   AES key unwrap key, intended for Secure FirmWare Boot
 *     25   Trusted-Key-Derive key, intended for Secure FirmWare Boot
 *     26   SHA256 Public key digest, intended for Secure FirmWare Boot
 *     30   Encrypted Vector KDK
 *
 * @param [in] fAddCRC
 *     Indication that the entry must be garded with an CRC.
 *     Note: For a Monotonic Counter this must always be false.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the keyblob
 *     authentication.
 *
 * @param [in] KeyBlob_p
 * @param [in] KeyBlobSize
 *     Pointer to and size of the keyblob containing the Asset Data.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_OTPDataWrite(
        const ValAssetNumber_t StaticAssetNumber,
        const ValAssetNumber_t AssetPolicyNumber,
        const bool fAddCRC,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsIn_t * const KeyBlob_p,
        const ValSize_t KeyBlobSize);


/*-----------------------------------------------------------------------------
 * val_ProvisionRandomRootKey
 */
/**
 * This function writes a randomly generated Trusted Root Key (HUK) to OTP in
 * combination with the Crypto Officer ID (COID). This function can only be
 * used when the COID is not available and the OTP is still empty.
 *
 * It is only recommended to generate an OTP Key Blob when the HUK needs to be
 * used outside the EIP-13x hardware for verification purposes in a secure
 * environment of course. Note that this is only point that the HUK can be
 * retrieved securely.
 *
 * @param [in] Identity
 *     The Crypto Officer ID.
 *
 * @param [in] StaticAssetNumber
 *     The asset number of the Static Asset or Public data object to write.
 *     Range [0-30].
 *
 * @param [in] f128bit
 *      Indication that the Trusted Root Key data size is 128 bit instead of
 *      256 bit. Thus if not set, the Trusted Root Key data size is 256 bit.
 *
 * @param [in] fAddCRC
 *     Indication that the entry must be garded with an CRC.
 *
 * @param [in] AutoSeed
 *     Setting that defines the automatic reseed after 'AutoSeed' times
 *     64K Bytes of DRBG random generation.
 *
 * @param [in] SampleCycles
 *     Setting that controls the number of (XOR-ed) FRO samples XOR-ed
 *     together to generate a single 'noise' bit. This value must be
 *     set such that the total amount of entropy in 8 'noise' bits
 *     equals at least 1 bit.
 *
 * @param [in] SampleDiv
 *     Setting that controls the number of module clock cycles between
 *     samples taken from the FROs.
 *
 * @param [in] NoiseBlocks
 *     Setting that defines number of 512 bit 'noise' blocks to process
 *     through the SHA-256 Conditioning function to generate a single
 *     256 bits 'full entropy' result for (re-)seeding the DRBG.
 *
 * @param [in] AssociatedData_p
 * @param [in] AssociatedDataSize
 *     Pointer and number of bytes for the Associated Data used in the OTP
 *     key blob authentication.
 *
 * @param [out] KeyBlob_p
 *     Pointer to the OTP keyblob buffer
 *
 * @param [in,out] KeyBlobSize_p
 *     Pointer to the size of the buffer intended for the OTP keyblob.
 *     Note when KeyBlobSize_p is NULL or the size is zero, NO OTP key blob
 *     generation is assumed.
 *
 * @return One of the ValStatus_t values.
 */
ValStatus_t
val_ProvisionRandomRootKey(
        const uint32_t Identity,
        const ValAssetNumber_t StaticAssetNumber,
        const bool f128bit,
        const bool fAddCRC,
        const uint8_t  AutoSeed,
        const uint16_t SampleCycles,
        const uint8_t  SampleDiv,
        const uint8_t  NoiseBlocks,
        ValOctetsIn_t * const AssociatedData_p,
        const ValSize_t AssociatedDataSize,
        ValOctetsOut_t * const KeyBlob_p,
        ValSize_t * const KeyBlobSize_p);


#endif /* Include Guard */


/* end of file api_val_asset.h */
