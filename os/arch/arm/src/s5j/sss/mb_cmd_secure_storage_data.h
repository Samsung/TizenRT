/*!
 *  @file       mb_cmd_secure_storage_data.h
 *  @brief      Headerfile : mailbox command for secure storage data
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_SECURE_STORAGE_DATA_H_
#define MB_CMD_SECURE_STORAGE_DATA_H_


#define SSTORAGE_DATA_SLOT_INDEX_MAX		(32)
#define SSTORAGE_DATA_DATA_MAX				(208)
#define SSTORAGE_DATA_SLOT_MAX				(256)


// ======================================
// Function
// ======================================
// Secure Storage for Data
int mb_storage_set_data_init(unsigned char * iv, unsigned int msg_byte_len, unsigned int index);
int mb_storage_set_data_update(unsigned char * out_block, unsigned char *in_block);
int mb_storage_set_data_final(unsigned char * out_block, unsigned char *in_block, unsigned int in_block_byte_len);

int mb_storage_get_data_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int index);
int mb_storage_get_data_update(unsigned char * out_block, unsigned char *in_block);
int mb_storage_get_data_final(unsigned char * out_block, unsigned char *in_block, unsigned int block_byte_len);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_SSTORAGE_DATA_ENC_INIT			(0x0001A202)
#define FUNC_SSTORAGE_DATA_ENC_UPDATE		(0x0002A202)
#define FUNC_SSTORAGE_DATA_ENC_FINAL		(0x0003A202)

#define FUNC_SSTORAGE_DATA_DEC_INIT			(0x0011A202)
#define FUNC_SSTORAGE_DATA_DEC_UPDATE		(0x0012A202)
#define FUNC_SSTORAGE_DATA_DEC_FINAL		(0x0013A202)


#endif /* MB_CMD_SECURE_STORAGE_DATA_H_ */
