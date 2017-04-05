/*!
 *  @file       mb_cmd_hmac_securekey.h
 *  @brief      Headerfile : mailbox command for hmac securekey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_HMAC_SECUREKEY_H_
#define MB_CMD_HMAC_SECUREKEY_H_

#include "mb_cmd_hmac.h"

// ======================================
// Function
// ======================================
int mb_hmac_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

int mb_hmac_init_securekey(unsigned int object_id, unsigned int msg_byte_len);
int mb_hmac_update_securekey(unsigned int block_byte_len, unsigned char * msg_block);
int mb_hmac_final_securekey(unsigned char *mac, unsigned int block_byte_len, unsigned char * msg_block);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_HMAC_KEYGEN_SECUREKEY		(0x00013202)

#define FUNC_HMAC_INIT_SECUREKEY		(0x00113202)
#define FUNC_HMAC_UPDATE_SECUREKEY		(0x00213202)
#define FUNC_HMAC_FINAL_SECUREKEY		(0x00313202)

#endif /* MB_CMD_HMAC_SECUREKEY_H_ */

