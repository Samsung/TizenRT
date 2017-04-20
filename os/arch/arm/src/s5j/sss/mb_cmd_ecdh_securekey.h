/*!
 *  @file       mb_cmd_ecdh_securekey.h
 *  @brief      Headerfile : mailbox command for ecdh securekey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_ECDH_SECUREKEY_H_
#define MB_CMD_ECDH_SECUREKEY_H_

#include "mb_cmd_ecdsa.h"

// ======================================
// Function
// ======================================
int mb_compute_ecdh_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sECC_KEY ecc_key, unsigned int object_id);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_ECDH_COMPUTE_SECUREKEY	(0x01012102)

#endif							/* MB_CMD_ECDH_SECUREKEY_H_ */
