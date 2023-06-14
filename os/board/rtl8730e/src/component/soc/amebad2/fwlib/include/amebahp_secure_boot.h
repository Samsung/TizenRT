#ifndef _AMEBAHP_SECURE_BOOT_H_
#define _AMEBAHP_SECURE_BOOT_H_

#include "mbedtls/ecp.h"

#define SBOOT_ERR_AUTH_NOT_SUPPORT					-1
#define SBOOT_ERR_PK_HASH_INVALID					-2
#define SBOOT_ERR_SIGN_INVALID						-3
#define SBOOT_ERR_ECDSA_CURVE_NOT_SUPPORT			-4
#define SBOOT_ERR_HASH_NOT_SUPPORT					-6
#define SBOOT_ERR_IMG_HASH_INVALID					-7
#define SBOOT_ERR_PK_HASH_ENGINE_ERROR				-8
#define SBOOT_ERR_EDDSA_HASH_ENGINE					-9
#define SBOOT_ERR_ECDSA_HASH_ENGINE					-10
#define SBOOT_ERR_IMG_HASH_ENGINE					-11
#define SBOOT_ERR_AUTH_ALG_NOT_MATCH				-12
#define SBOOT_ERR_HASH_ALG_NOT_MATCH				-13

#define IMG_SIZE_INVALID							-0xFFFE
#define IMG_INVALID									-0xFFFF

/*ImgID_TypeDef*/
#define IMGID_BOOT	0
#define IMGID_NSPE	1
#define IMGID_SPE	2
#define IMGID_BL1	3

/*AuthAlg_TypeDef*/
#define AUTHID_EDDSA_25519				0
#define AUTHID_ECDSA_SECP192R1			1
#define AUTHID_ECDSA_SECP224R1			2
#define AUTHID_ECDSA_SECP256R1			3
#define AUTHID_ECDSA_BP256R1			6
#define AUTHID_ECDSA_CURVE25519			9
#define AUTHID_ECDSA_SECP192K1			10
#define AUTHID_ECDSA_SECP224K1			11
#define AUTHID_ECDSA_SECP256K1			12

#define ECP_MAX_BITS_NUM     				256   /**< Maximum bit size of groups */
#define ECP_MAX_BYTES_NUM    				( ( ECP_MAX_BITS_NUM + 7 ) / 8 )
#define ECP_MAX_PT_LEN_NUM   				( 2 * ECP_MAX_BYTES_NUM)

#define IS_EDDSA(x)				(x == AUTHID_EDDSA_25519)
#define IS_ECDSA(x)				(x == AUTHID_ECDSA_SECP192R1) || \
								(x == AUTHID_ECDSA_SECP224R1) || \
								(x == AUTHID_ECDSA_SECP256R1) || \
								(x == AUTHID_ECDSA_BP256R1) || \
								(x == AUTHID_ECDSA_CURVE25519) || \
								(x == AUTHID_ECDSA_SECP192K1) || \
								(x == AUTHID_ECDSA_SECP224K1) || \
								(x == AUTHID_ECDSA_SECP256K1)

/*HashAlg_TypeDef*/
#define HASHID_SHA256		0
#define HASHID_SHA384		1
#define HASHID_SHA512		2
#define HASHID_HMAC256		3
#define HASHID_HMAC384		4
#define HASHID_HMAC512		5

#define HASH_MAX_LEN		64
#define PKEY_MAX_LEN		((((2 * MBEDTLS_ECP_MAX_BYTES -1) >> 4) + 1) << 4) //144 byte (16-byte aligned, format type field not included)
#define SIGN_MAX_LEN		((((2 * MBEDTLS_ECP_MAX_BYTES -1) >> 4) + 1) << 4) //144 byte (16-byte aligned, raw data format, not ASN.1)

#define CRYPTO_CHK(f, errno) do { if( ( ret = f ) != 0 ) { ret = errno; goto exit; }} while( 0 )

#define MANIFEST_SIZE_4K_ALIGN		0x1000

typedef struct {
	u32 Pattern[2];
	u8 Rsvd1[8];
	u8 Ver;
	u8 ImgID;
	u8 AuthAlg;
	u8 HashAlg;
	u16 MajorImgVer;
	u16 MinorImgVer;
	u32 ImgSize;
	u32 SecEpoch;
	u8 RsipIV[16];
	u8 ImgHash[HASH_MAX_LEN];
	u8 SBPubKey[PKEY_MAX_LEN];
	u8 Signature[SIGN_MAX_LEN];

} Manifest_TypeDef;

typedef struct {
	uint32_t Addr;
	uint32_t Len;
} SubImgInfo_TypeDef;

int SBOOT_GetMdType(u8 HashId, SHA2_TYPE *MdType, u8 *IsHmac);
int SBOOT_Validate_Algorithm(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash);
int SBOOT_Validate_PubKey(u8 AuthAlg, u8 *Pk, u8 *Hash);
int SBOOT_Validate_Signature(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig);
int SBOOT_Validate_ImgHash(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num);
int SBOOT_SignatureVerify(u8 *PubKeyHash, Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgCnt);

#endif