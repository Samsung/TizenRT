/*!
 *  @file       mb_cmd_secure_storage_cert.h
 *  @brief      Headerfile : mailbox command for secure storage cert
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_SECURE_STORAGE_CERT_H_
#define MB_CMD_SECURE_STORAGE_CERT_H_

#define SSTORAGE_CERT_SLOT_INDEX_MAX		(8)
#define SSTORAGE_CERT_DATA_MAX				(0x1000)	// 4096
#define SSTORAGE_CERT_SLOT_MAX				(0x1100)	// 4096+256

// ======================================
// Function
// ======================================
// Secure Storage for Cert
int mb_storage_set_cert_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int index);
int mb_storage_set_cert_update(unsigned char *out_block, unsigned char *in_block);
int mb_storage_set_cert_final(unsigned char *out_block, unsigned char *in_block, unsigned int in_block_byte_len);

int mb_storage_get_cert_init(unsigned char *iv, unsigned int msg_byte_len, unsigned int index);
int mb_storage_get_cert_update(unsigned char *out_block, unsigned char *in_block);
int mb_storage_get_cert_final(unsigned char *out_block, unsigned char *in_block, unsigned int block_byte_len);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_SSTORAGE_CERT_ENC_INIT			(0x0001A102)
#define FUNC_SSTORAGE_CERT_ENC_UPDATE		(0x0002A102)
#define FUNC_SSTORAGE_CERT_ENC_FINAL		(0x0003A102)

#define FUNC_SSTORAGE_CERT_DEC_INIT			(0x0011A102)
#define FUNC_SSTORAGE_CERT_DEC_UPDATE		(0x0012A102)
#define FUNC_SSTORAGE_CERT_DEC_FINAL		(0x0013A102)

#endif							/* MB_CMD_SECURE_STORAGE_CERT_H_ */
