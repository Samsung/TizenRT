/**
  ******************************************************************************
  * @file    ameba_ota.c
  * @author
  * @version V1.0.0
  * @date    2023-07-13
  * 
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "rtl_se_crypto_function.h"
#include "ameba_ota.h"

/* OTA image signature verification
 *
 * The verification logic runs entirely in the secure world (BL32), where it
 * has direct access to the SBOOT_Validate_* primitives. Only the image address
 * is passed across the SMC boundary, so the signature checking code and the
 * certificate/manifest parsing it depends on are no longer present in the
 * non-secure image.
 */
int OTA_UserImageSignatureCheck(uint32_t input_addr)
{
	return ameba_OTA_UserImageSignatureCheck(input_addr);
}

int OTA_KernelImageSignatureCheck(uint32_t input_addr)
{
	return ameba_OTA_KernelImageSignatureCheck(input_addr);
}


/**
* @brief Key Derivation Function (KDF) for deriving cryptographic keys from a password and salt.
*
* This function performs key derivation using the input password and salt, and produces a derived key
* of the specified length using a specified number of iterations. The derived key is stored in `dstbuf`.
*
* @param password 		The input password from which the key will be derived.
* @param plen 			The length of the password.
* @param salt 			A random value known as the salt, used to make the key derivation process more secure.
* @param slen 			The length of the salt.
* @param iteration_count 	The number of iterations to apply in the key derivation process.
* @param dstbuf 		A buffer to store the derived key.
* @param dlen 			The desired length of the derived key to be produced.
*
* @return 			0: on success
*				-1: on iteration_count > 0xFFFFFFFF
*				others non-zero value: refer to mbedtls error code
*/
int Ameba_KeyDeriveFunc(const unsigned char *password, size_t plen,
			const unsigned char *salt, size_t slen,
			unsigned int iteration_count,
			unsigned char *dstbuf, size_t dlen)
{
	int ret;
	secure_kdf_struc KDF_input;

	if (dstbuf == NULL) /* output buff is empty */
		return FALSE;

	KDF_input.password = (unsigned char *)password;
	KDF_input.plen = plen;
	KDF_input.salt = (unsigned char *)salt;
	KDF_input.slen = slen;
	KDF_input.iteration_count = iteration_count;
	KDF_input.dstbuf = dstbuf;
	KDF_input.dlen = dlen;

	ret = ameba_Secure_KeyDeriveFunc(&KDF_input);	/* PBKDF2 API */

	return ret;
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor *****END OF FILE****/
