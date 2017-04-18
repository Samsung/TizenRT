/*!
 *  @file       mb_cmd_ecdsa_securekey.h
 *  @brief      Headerfile : mailbox command for ecdsa securekey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_ECDSA_SECUREKEY_H_
#define MB_CMD_ECDSA_SECUREKEY_H_

#include "mb_cmd_ecdsa.h"

// ======================================
// Function
// ======================================
int mb_ecdsa_sign_securekey_step1(unsigned int object_id, unsigned char *hash);
int mb_ecdsa_sign_securekey_step2(struct sECC_SIGN *signature, unsigned int object_id);

int mb_ecdsa_verify_securekey_step1(unsigned int object_id, unsigned char *hash);
int mb_ecdsa_verify_securekey_step2(unsigned int object_id, struct sECC_SIGN *signature);
int mb_ecdsa_verify_securekey_step3(unsigned int object_id);

int mb_ecdsa_generate_key_securekey_step1(unsigned int object_id);
int mb_ecdsa_generate_publickey_securekey_step1(unsigned int object_id);
int mb_ecdsa_generate_publickey_securekey_step2(struct sECC_KEY *ecc_publickey, unsigned int object_id);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_ECDSA_SIGN_SECUREKEY1				(0x01011102)
#define FUNC_ECDSA_SIGN_SECUREKEY2				(0x01021102)

#define FUNC_ECDSA_VERIFY_SECUREKEY1			(0x01011202)
#define FUNC_ECDSA_VERIFY_SECUREKEY2			(0x01021202)
#define FUNC_ECDSA_VERIFY_SECUREKEY3			(0x01031202)

#define FUNC_ECDSA_GEN_KEY_SECUREKEY1			(0x03011302)
#define FUNC_ECDSA_GEN_PUBLICKEY_SECUREKEY1		(0x01011302)
#define FUNC_ECDSA_GEN_PUBLICKEY_SECUREKEY2		(0x01021302)

#endif							/* MB_CMD_ECDSA_SECUREKEY_H_ */
