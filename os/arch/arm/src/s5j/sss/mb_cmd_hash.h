/*!
 *  @file		mb_cmd_hash.h
 *  @brief		Headerfile : mailbox command for hash
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_HASH_H_
#define MB_CMD_HASH_H_

// ======================================
// Structure
// ======================================
//! @struct sHASH_MSG
//! @brief struct of message for Hash
struct sHASH_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

#define MAX_HASH_BLOCK_BLEN 		(144)
#define GET_HASH_SIZE(OID)			((OID>> 8)&0x07)

// ======================================
// Function
// ======================================
int mb_hash_init(unsigned int object_id, unsigned int msg_byte_len);
int mb_hash_update(unsigned int block_byte_len, unsigned char *msg_block);
int mb_hash_final(unsigned char *hash, unsigned int block_byte_len, unsigned char *msg_block);

int mb_hash_dma(unsigned char *hash, unsigned int object_id, unsigned int msg_byte_len, unsigned int msg_addr_low, unsigned int msg_addr_high);
int mb_hash_descriptor(unsigned char *hash, unsigned int object_id, unsigned int msg_byte_len, unsigned int descriptor_byte_len, unsigned int descriptor_addr_low, unsigned int descriptor_addr_high);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_HASH_INIT					(0x00103102)
#define FUNC_HASH_UPDATE				(0x00203102)
#define FUNC_HASH_FINAL					(0x00303102)
#define FUNC_HASH_DMA					(0x01013102)
#define FUNC_HASH_DESCRIPTOR			(0x02013102)

#endif							/* MB_CMD_HASH_H_ */
