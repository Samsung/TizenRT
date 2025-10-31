/**
 * \file sm4.h
 *
 * \brief This file contains ARM Dubhe engine SCA SM4 definitions and functions.
 *
 */
#ifndef ARM_CE_DUBHE_SM4_H
#define ARM_CE_DUBHE_SM4_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(ARM_CE_DUBHE_SCA)

#include <stddef.h>
#include <stdint.h>

#include "dubhe_sca.h"
/* padlock.c and aesni.c rely on these values! */
#define MBEDTLS_SM4_ENCRYPT     1 /**< SM4 encryption. */
#define MBEDTLS_SM4_DECRYPT     0 /**< SM4 decryption. */

#define MBEDTLS_SM4_DERIVED_MODEL_KEY       0
#define MBEDTLS_SM4_DERIVED_DEVICE_ROOT_KEY 1

/* Error codes in range 0x0020-0x0022 */
#define MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH                -0x0020  /**< Invalid key length. */
#define MBEDTLS_ERR_SM4_INVALID_INPUT_LENGTH              -0x0022  /**< Invalid data input length. */

/* Error codes in range 0x0023-0x0025 */
#define MBEDTLS_ERR_SM4_FEATURE_UNAVAILABLE               -0x0023  /**< Feature not available. For example, an unsupported SM4 key size. */
#define MBEDTLS_ERR_SM4_HW_ACCEL_FAILED                   -0x0025  /**< SM4 hardware accelerator failed. */

#define MBEDTLS_ERR_SM4_INVALID_PARAM   -0x002F
#define MBEDTLS_ERR_SM4_GENERIC   -0x0030

#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	arm_ce_sca_context_t *sca_ctx;
} mbedtls_sm4_context;


/**
 * \brief          This function initializes the specified SM4 context.
 *
 *                 It must be the first API called before using
 *                 the context.
 *
 * \param ctx      The SM4 context to initialize.
 */
void mbedtls_sm4_init( mbedtls_sm4_context *ctx );

/**
 * \brief          This function releases and clears the specified SM4 context.
 *
 * \param ctx      The SM4 context to clear.
 */
void mbedtls_sm4_free( mbedtls_sm4_context *ctx );

/**
 * \brief          This function sets the encryption key.
 *
 * \param ctx      The SM4 context to which the key should be bound.
 * \param key      The encryption key.
 * \param keybits  The size of data passed in bits. Only support 128 bits.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH / MBEDTLS_ERR_SM4_GENERIC / MBEDTLS_ERR_SM4_INVALID_PARAM on failure.
 */
int mbedtls_sm4_setkey_enc( mbedtls_sm4_context *ctx, const unsigned char *key,
					unsigned int keybits );

/**
 * \brief          This function sets the decryption key.
 *
 * \param ctx      The SM4 context to which the key should be bound.
 * \param key      The decryption key.
 * \param keybits  The size of data passed. Only support 128 bits.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH / MBEDTLS_ERR_SM4_GENERIC / MBEDTLS_ERR_SM4_INVALID_PARAM on failure.
 */
int mbedtls_sm4_setkey_dec( mbedtls_sm4_context *ctx, const unsigned char *key,
					unsigned int keybits );

/**
 * \brief          This function sets the derived cryption key.
 *
 * \param ctx      The SM4 context to which the key should be bound.
 * \param key_type The derived key type.
 * \param mode	   The encryption/decrypiton mode.
 * \param ek1      The encryption ek1.
 * \param ek2      The encryption ek2.
 * \param ek3      The encryption ek3.
 * \param ek1bits  The size of derived key in bits. Only support 128 bits.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_SM4_INVALID_KEY_LENGTH / MBEDTLS_ERR_SM4_GENERIC / MBEDTLS_ERR_SM4_INVALID_PARAM on failure.
 */
int mbedtls_sm4_set_derived_key( mbedtls_sm4_context *ctx,
							 int key_type,
							 int mode,
							 const unsigned char *ek1,
							 const unsigned char *ek2,
							 const unsigned char *ek3,
							 unsigned int ek1bits );

/**
 * \brief          This function performs an SM4 single-block encryption or
 *                 decryption operation.
 *
 *                 It performs the operation defined in the \p mode parameter
 *                 (encrypt or decrypt), on the input data buffer defined in
 *                 the \p input parameter.
 *
 *                 mbedtls_sm4_init(), and either mbedtls_sm4_setkey_enc() or
 *                 mbedtls_sm4_setkey_dec() must be called before the first
 *                 call to this API with the same context(The derived key API
 *				   is also called in the same order).
 *
 * \param ctx      The SM4 context to use for encryption or decryption.
 * \param mode     The SM4 operation: #MBEDTLS_SM4_ENCRYPT or
 *                 #MBEDTLS_SM4_DECRYPT.
 * \param input    The 16-Byte buffer holding the input data.
 * \param output   The 16-Byte buffer holding the output data.

 * \return         \c 0 on success.
 */
int mbedtls_sm4_crypt_ecb( mbedtls_sm4_context *ctx,
					int mode,
					const unsigned char input[16],
					unsigned char output[16] );


/**
 * \brief  This function performs an SM4-CBC encryption or decryption operation
 *         on full blocks.
 *
 *         It performs the operation defined in the \p mode
 *         parameter (encrypt/decrypt), on the input data buffer defined in
 *         the \p input parameter.
 *
 *         It can be called as many times as needed, until all the input
 *         data is processed. mbedtls_sm4_init(), and either
 *         mbedtls_sm4_setkey_enc() or mbedtls_sm4_setkey_dec() must be called
 *         before the first call to this API with the same context.(The derived
 *         key API is also called in the same order).
 *
 * \note   This function operates on aligned blocks, that is, the input size
 *         must be a multiple of the SM4 block size of 16 Bytes.
 *
 * \note   Upon exit, the content of the IV is updated so that you can
 *         call the same function again on the next
 *         block(s) of data and get the same result as if it was
 *         encrypted in one call. This allows a "streaming" usage.
 *         If you need to retain the contents of the IV, you should
 *         either save it manually or use the cipher module instead.
 *
 *
 * \param ctx      The SM4 context to use for encryption or decryption.
 * \param mode     The SM4 operation: #MBEDTLS_SM4_ENCRYPT or
 *                 #MBEDTLS_SM4_DECRYPT.
 * \param length   The length of the input data in Bytes. This must be a
 *                 multiple of the block size (16 Bytes).
 * \param iv       Initialization vector (updated after use).
 * \param input    The buffer holding the input data.
 * \param output   The buffer holding the output data.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_SM4_INVALID_INPUT_LENGTH
 *                 on failure.
 */
int mbedtls_sm4_crypt_cbc( mbedtls_sm4_context *ctx,
					int mode,
					size_t length,
					unsigned char iv[16],
					const unsigned char *input,
					unsigned char *output );


/**
 * \brief      This function performs an SM4-CTR encryption or decryption
 *             operation.
 *
 *             This function performs the operation defined in the \p mode
 *             parameter (encrypt/decrypt), on the input data buffer
 *             defined in the \p input parameter.
 *
 *             Due to the nature of CTR, you must use the same key schedule
 *             for both encryption and decryption operations. Therefore, you
 *             must use the context initialized with mbedtls_aes_setkey_enc()
 *             for both #MBEDTLS_SM4_ENCRYPT and #MBEDTLS_SM4_DECRYPT.
 *
 * \warning    You must keep the maximum use of your counter in mind.
 *
 * \param ctx              The SM4 context to use for encryption or decryption.
 * \param length           The length of the input data.
 * \param nc_off           The offset in the current \p stream_block, for
 *                         resuming within the current cipher stream. The
 *                         offset pointer should be 0 at the start of a stream.
 * \param nonce_counter    The 128-bit nonce and counter.
 * \param stream_block     The saved stream block for resuming. This is
 *                         overwritten by the function.
 * \param input            The buffer holding the input data.
 * \param output           The buffer holding the output data.
 *
 * \return                 \c 0 on success.
 */
int mbedtls_sm4_crypt_ctr( mbedtls_sm4_context *ctx,
					   size_t length,
					   size_t *nc_off,
					   unsigned char nonce_counter[16],
					   unsigned char stream_block[16],
					   const unsigned char *input,
					   unsigned char *output );

#ifdef __cplusplus
}
#endif

#endif /* ARM_CE_DUBHE_SCA */
#endif /* ARM_CE_DUBHE_SM4_H */
