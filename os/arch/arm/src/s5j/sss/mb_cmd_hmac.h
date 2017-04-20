/*!
 *  @file       mb_cmd_hmac.h
 *  @brief      Headerfile : mailbox command for hmac
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_HMAC_H_
#define MB_CMD_HMAC_H_

#define MAX_HMAC_BLOCK_BLEN 	(128)

//! @struct sHMAC_MSG
//! @brief struct of message for HMAC
struct sHMAC_MSG {
	unsigned int addr_low;
	unsigned int addr_high;
	unsigned int descriptor_byte_len;
	unsigned int msg_byte_len;
	unsigned int msg_type;
};

#endif							/* MB_CMD_HMAC_H_ */
