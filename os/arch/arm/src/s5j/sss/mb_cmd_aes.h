/*!
 *  @file       mb_cmd_aes.h
 *  @brief      Headerfile : mailbox api for aes
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_AES_H_
#define MB_CMD_AES_H_

#define AES_ENCTYPT		(0x00)
#define AES_DECRYPT		(0x01)

// Key Size field
#define AESKEY_128		(0x10)
#define AESKEY_192		(0x18)
#define AESKEY_256		(0x20)

// Mode
#define AES_ECB_MODE		(0x0008)
#define AES_CBC_MODE		(0x0108)
#define AES_CTR_MODE		(0x0208)
#define AES_XTS_MODE		(0x0308)
#define AES_CCM_MODE		(0x1008)
#define AES_GCM_MODE		(0x1108)
#define AES_KW_MODE			(0x1208)

#define AES_128_ECB 	((AESKEY_128<<16)|(AES_ECB))	// 0x00001008
#define AES_192_ECB 	((AESKEY_192<<16)|(AES_ECB))	// 0x00001808
#define AES_256_ECB 	((AESKEY_256<<16)|(AES_ECB))	// 0x00002008
#define AES_128_CBC 	((AESKEY_128<<16)|(AES_CBC))	// 0x00101008
#define AES_192_CBC 	((AESKEY_192<<16)|(AES_CBC))	// 0x00101808
#define AES_256_CBC 	((AESKEY_256<<16)|(AES_CBC))	// 0x00102008
#define AES_128_CTR 	((AESKEY_128<<16)|(AES_CTR))	// 0x00201008
#define AES_192_CTR 	((AESKEY_192<<16)|(AES_CTR))	// 0x00201808
#define AES_256_CTR 	((AESKEY_256<<16)|(AES_CTR))	// 0x00202008
#define AES_128_XTS 	((AESKEY_128<<16)|(AES_XTS))	// 0x00301008
// No AES_192_XTS
#define AES_256_XTS 	((AESKEY_256<<16)|(AES_XTS))	// 0x00302008
#define AES_128_CCM 	((AESKEY_128<<16)|(AES_CCM))	// 0x01001008
#define AES_192_CCM 	((AESKEY_192<<16)|(AES_CCM))	// 0x01001808
#define AES_256_CCM 	((AESKEY_256<<16)|(AES_CCM))	// 0x01002008
#define AES_128_GCM 	((AESKEY_128<<16)|(AES_GCM))	// 0x01101008
#define AES_192_GCM 	((AESKEY_192<<16)|(AES_GCM))	// 0x01101808
#define AES_256_GCM 	((AESKEY_256<<16)|(AES_GCM))	// 0x01102008

#define MAX_MB_AES_BLOCK_BLEN 	(240)
#define MAX_AES_AAD_BLEN 		(128)

// ======================================
// Structure
// ======================================
//! @struct sAES_PARAM
//! @brief struct of AES parameter
struct sAES_PARAM {
	unsigned char *pu8Plaintext;
	unsigned int u32Plaintext_byte_len;
	unsigned char *pu8Ciphertext;
	unsigned int u32Ciphertext_byte_len;

	unsigned char *pu8iv;
	unsigned int u32iv_byte_len;

	unsigned char *pu8aad;
	unsigned int u32aad_byte_len;

	unsigned char *pu8Tag;
	unsigned int u32Tag_byte_len;

	unsigned int u32Mode;
};

#endif							/* MB_CMD_AES_H_ */
