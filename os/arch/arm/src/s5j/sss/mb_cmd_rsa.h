/*!
 *  @file       mb_cmd_rsa.h
 *  @brief      Headerfile : mailbox command for rsa
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_RSA_H_
#define MB_CMD_RSA_H_

// ======================================
// Structure
// ======================================
//! @struct sRSA_KEY
//! @brief struct of RSA Key parameter
struct sRSA_KEY {
	unsigned int object_id;
	unsigned char *modules;
	unsigned int modules_byte_len;
	unsigned char *publickey;
	unsigned int publickey_byte_len;
	unsigned char *privatekey;
};

//! @struct sRSA_SIGN
//! @brief struct of RSA Signature
struct sRSA_SIGN {
	unsigned int signature_byte_len;
	unsigned char *signature;
	unsigned int salt_byte_len;
	unsigned int alg_type;
};

#endif							/* MB_CMD_RSA_H_ */
