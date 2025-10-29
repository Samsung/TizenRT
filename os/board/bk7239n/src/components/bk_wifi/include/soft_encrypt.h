#ifndef _SOFT_ENCRYPT_H_
#define _SOFT_ENCRYPT_H_

#define AES_ENCRYPT                   1
#define AES_DECRYPT                   0

#define WEENY_AES_ROM_TABLES          0
#define XOR_KEY_COUNT                 8

#define BK_AES_IV_LEN                (16)
#define BK_AES_KEY_LEN               (32)
#define BK_TINY_AES_IV  "0123456789ABCEEF"
#define BK_TINY_AES_KEY "0123456789ABCEEF0123456789ABCDEF"

#define POW_LEN    (256)
#define LOG_LEN    (256)

/**
 * \brief          AES context structure
 */
typedef struct {
	int nr;			/*!<  number of rounds  */
	unsigned long *rk;	/*!<  AES round keys    */
	unsigned long buf[68];	/*!<  unaligned data    */
} weeny_aes_context;

#ifdef __cplusplus
extern "C" {
#endif

void xor_enc(void *in, void *out, int len);
void xor_dec(void *in, void *out, int len);

/**
 * \brief          AES key schedule (encryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      encryption key
 * \param keysize  must be 128, 192 or 256
 */
void weeny_aes_setkey_enc(weeny_aes_context *ctx, unsigned char *key, int keysize);

/**
 * \brief          AES key schedule (decryption)
 *
 * \param ctx      AES context to be initialized
 * \param key      decryption key
 * \param keysize  must be 128, 192 or 256
 */
void weeny_aes_setkey_dec(weeny_aes_context *ctx, unsigned char *key, int keysize);

/**
 * \brief          AES-ECB block encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 */
void weeny_aes_crypt_ecb(weeny_aes_context *ctx,
						 int mode,
						 unsigned char input[16], unsigned char output[16]);

/**
 * \brief          AES-CBC buffer encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void weeny_aes_crypt_cbc(weeny_aes_context *ctx,
						 int mode,
						 int length,
						 unsigned char iv[16],
						 unsigned char *input, unsigned char *output);

/**
 * \brief          AES-CFB128 buffer encryption/decryption
 *
 * \param ctx      AES context
 * \param mode     AES_ENCRYPT or AES_DECRYPT
 * \param length   length of the input data
 * \param iv_off   offset in IV (updated after use)
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
void weeny_aes_crypt_cfb128(weeny_aes_context *ctx,
							int mode,
							int length,
							int *iv_off,
							unsigned char iv[16],
							unsigned char *input, unsigned char *output);

#ifdef __cplusplus
}
#endif
#endif //_SOFT_ENCRYPT_H_
// eof

