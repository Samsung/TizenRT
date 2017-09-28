/*!
 *  @file       mb_cmd_rsa_securekey.h
 *  @brief      Headerfile : mailbox command for rsa securekey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_RSA_SECUREKEY_H_
#define MB_CMD_RSA_SECUREKEY_H_

#include "mb_cmd_rsa.h"

// ======================================
// Functions
// ======================================
int mb_rsa_encrypt_securekey_step1(unsigned int object_id, unsigned char *input, unsigned int input_byte_len);
int mb_rsa_encrypt_securekey_step2(unsigned int object_id);
int mb_rsa_encrypt_securekey_step3(unsigned int object_id, unsigned char *output, unsigned int *output_byte_len);

int mb_rsa_decrypt_securekey_step1(unsigned int object_id, unsigned char *input, unsigned int input_byte_len);
int mb_rsa_decrypt_securekey_step2(unsigned int object_id);
int mb_rsa_decrypt_securekey_step3(unsigned int object_id, unsigned char *output, unsigned int *output_byte_len);

int mb_rsa_sign_securekey_step1(unsigned int object_id, unsigned char *hash);
int mb_rsa_sign_securekey_step2(unsigned int object_id);
int mb_rsa_sign_securekey_step3(unsigned int object_id, struct sRSA_SIGN *rsa_sign);

int mb_rsa_verify_securekey_step1(unsigned int object_id, unsigned char *hash);
int mb_rsa_verify_securekey_step2(unsigned int object_id);
int mb_rsa_verify_securekey_step3(unsigned int object_id, struct sRSA_SIGN *rsa_sign);
int mb_rsa_verify_securekey_step4(unsigned int object_id, struct sRSA_SIGN *rsa_sign);

int mb_rsa_generate_key_secretkey_step1(unsigned int object_id, unsigned int pukey);

int mb_rsa_get_pubkey_secretkey_step1(unsigned int object_id, struct sRSA_KEY *rsa_key);
int mb_rsa_get_pubkey_secretkey_step2(unsigned int object_id, struct sRSA_KEY *rsa_key);

int mb_load_prime(void);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_RSA_ENCRYPT_SECUREKEY1     (0x00015102)
#define FUNC_RSA_ENCRYPT_SECUREKEY2     (0x01025102)
#define FUNC_RSA_ENCRYPT_SECUREKEY3     (0x00035102)

#define FUNC_RSA_DECRYPT_SECUREKEY1     (0x00015202)
#define FUNC_RSA_DECRYPT_SECUREKEY2     (0x01025202)
//skip 03rd when securekey
#define FUNC_RSA_DECRYPT_SECUREKEY3     (0x00045202)

#define FUNC_RSA_SIGN_SECUREKEY1        (0x00011502)
#define FUNC_RSA_SIGN_SECUREKEY2        (0x01021502)
//skip 03rd when secure key
#define FUNC_RSA_SIGN_SECUREKEY3        (0x00041502)
#define FUNC_RSA_SIGN_SECUREKEY3_PKCS   (0x01041502)
#define FUNC_RSA_SIGN_SECUREKEY3_PSS    (0x00041502)

#define FUNC_RSA_VERIFY_SECUREKEY1      (0x00011602)
#define FUNC_RSA_VERIFY_SECUREKEY2      (0x01021602)
#define FUNC_RSA_VERIFY_SECUREKEY3      (0x00031602)
#define FUNC_RSA_VERIFY_SECUREKEY4      (0x00041602)

#define FUNC_RSA_GENKEY_SECUREKEY1_RNDPK  (0x01011702)
#define FUNC_RSA_GENKEY_SECUREKEY1_USERPK (0x03011702)

#define FUNC_RSA_GET_PUBKEY_SECUREKEY1  (0x01021702)
#define FUNC_RSA_GET_PUBKEY_SECUREKEY2  (0x01031702)

#endif							/* MB_CMD_RSA_SECUREKEY_H_ */
