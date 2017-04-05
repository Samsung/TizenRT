/*!
 *  @file       mb_cmd_secure_storage_key.h
 *  @brief      Headerfile : mailbox command for secure storage key
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_SECURE_STORAGE_KEY_H_
#define MB_CMD_SECURE_STORAGE_KEY_H_


// Storage Map
#define MB_SSTORAGE_BLOCK_BYTELEN_MAX (240)

// ======================================
// Function
// ======================================
// Secure Storage for Key
int mb_storage_set_securekey_init(unsigned char * iv, unsigned int msg_byte_len, unsigned int type, unsigned int index);
int mb_storage_set_securekey_update(unsigned char * out_block, unsigned char *in_block, unsigned int type);
int mb_storage_set_securekey_final(unsigned char *out_block, unsigned char *in_block, unsigned int block_byte_len, unsigned int type);

int mb_storage_get_securekey_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int type, unsigned int index);
int mb_storage_get_securekey_update(unsigned char *in_block);
int mb_storage_get_securekey_final(unsigned int *object_id, unsigned char *in_block, unsigned int block_byte_len);

int mb_storage_set_genkey_securekey_init(unsigned char * iv, unsigned int *msg_byte_len, unsigned int type, unsigned int index);
int mb_storage_set_genkey_securekey_update(unsigned char * out_block, unsigned int type);
int mb_storage_set_genkey_securekey_final(unsigned char *out_block, unsigned int type);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_SSTORAGE_KEY_ENC_INIT				(0x0001A302)
#define FUNC_SSTORAGE_KEY_ENC_UPDATE			(0x0002A302)
#define FUNC_SSTORAGE_KEY_ENC_FINAL				(0x0003A302)

#define FUNC_SSTORAGE_KEY_DEC_INIT				(0x0011A302)
#define FUNC_SSTORAGE_KEY_DEC_UPDATE			(0x0012A302)
#define FUNC_SSTORAGE_KEY_DEC_FINAL				(0x0013A302)

#define FUNC_SSTORAGE_KEY_ENC_KEYGEN_INIT		(0x0021A302)
#define FUNC_SSTORAGE_KEY_ENC_KEYGEN_UPDATE		(0x0022A302)
#define FUNC_SSTORAGE_KEY_ENC_KEYGEN_FINAL		(0x0023A302)

#endif /* MB_CMD_SECURE_STORAGE_KEY_H_ */
