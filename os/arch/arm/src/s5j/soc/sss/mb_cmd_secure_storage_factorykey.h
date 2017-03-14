/*!
 *  @file       mb_cmd_secure_storage_factorykey.h
 *  @brief      Headerfile : mailbox command for secure storage for factorykey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_SECURE_STORAGE_FACTORYKEY_H_
#define MB_CMD_SECURE_STORAGE_FACTORYKEY_H_

// ======================================
// Function
// ======================================
int mb_storage_set_factorykey_init(unsigned int key_byte_len);
int mb_storage_set_factorykey_update(unsigned char *in_block);
int mb_storage_set_factorykey_final(unsigned char *in_block, unsigned int block_byte_len);

int mb_storage_get_factorykey_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int type);
int mb_storage_get_factorykey_update(unsigned char *in_block);
int mb_storage_get_factorykey_final(unsigned char *in_block, unsigned int block_byte_len);

int mb_storage_set_factorykey_data_init(unsigned char * iv, unsigned int msg_byte_len, unsigned int key_id);
int mb_storage_set_factorykey_data_update(unsigned char *out_block, unsigned char *in_block);
int mb_storage_set_factorykey_data_final(unsigned char *out_block, unsigned char *in_block, unsigned int in_block_byte_len);

int mb_storage_get_factorykey_data_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int key_id);
int mb_storage_get_factorykey_data_update(unsigned char *out_block, unsigned char *in_block);
int mb_storage_get_factorykey_data_final(unsigned char *out_block, unsigned char *in_block, unsigned int block_byte_len);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_SSTORAGE_FACTORYKEY_ENC_INIT				(0x0001A402)
#define FUNC_SSTORAGE_FACTORYKEY_ENC_UPDATE				(0x0002A402)
#define FUNC_SSTORAGE_FACTORYKEY_ENC_FINAL				(0x0003A402)

#define FUNC_SSTORAGE_FACTORYKEY_DATA_ENC_INIT			(0x0021A402)
#define FUNC_SSTORAGE_FACTORYKEY_DATA_ENC_UPDATE		(0x0022A402)
#define FUNC_SSTORAGE_FACTORYKEY_DATA_ENC_FINAL			(0x0023A402)

#define FUNC_SSTORAGE_FACTORYKEY_DATA_DEC_INIT			(0x0031A402)
#define FUNC_SSTORAGE_FACTORYKEY_DATA_DEC_UPDATE		(0x0032A402)
#define FUNC_SSTORAGE_FACTORYKEY_DATA_DEC_FINAL			(0x0033A402)


#endif /* MB_CMD_SECURE_STORAGE_FACTORYKEY_H_ */
