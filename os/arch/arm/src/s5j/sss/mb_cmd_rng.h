/*!
 *  @file		mb_cmd_rng.h
 *  @brief		Headerfile : mailbox command for rng
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_RNG_H_
#define MB_CMD_RNG_H_

// ======================================
// Function
// ======================================
int mb_generate_random_number(unsigned int *random_number, unsigned int random_wlen);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_RNG  (0x00016102)

#endif							/* MB_CMD_RNG_H_ */
