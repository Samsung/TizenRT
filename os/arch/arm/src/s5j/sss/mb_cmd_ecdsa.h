/*!
 *  @file       mb_cmd_ecdsa.h
 *  @brief      Headerfile : mailbox command for ecdsa
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_ECDSA_H_
#define MB_CMD_ECDSA_H_

#define MAX_ECDSA_BLOCK_BLEN 	(128)
#define MAX_ECDSA_KEY_BLEN 		(244)

// ======================================
// Structure
// ======================================
//! @struct sECC_KEY
//! @brief struct of ECDSA Key parameter
struct sECC_KEY {
	unsigned int curve;
	unsigned char *privatekey;
	unsigned char *publickey_x;
	unsigned char x_byte_len;
	unsigned char *publickey_y;
	unsigned char y_byte_len;
};

//! @struct sECC_SIGN
//! @brief struct of ECDSA Signature
struct sECC_SIGN {
	unsigned int sign_type;
	unsigned char *r;
	unsigned char r_byte_len;
	unsigned char *s;
	unsigned char s_byte_len;
};

#endif							/* MB_CMD_ECDSA_H_ */
