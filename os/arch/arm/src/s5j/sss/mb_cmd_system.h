/*!
 *  @file       mb_cmd_system.h
 *  @brief      Headerfile : mailbox command for system function
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_SYSTEM_H_
#define MB_CMD_SYSTEM_H_

// ======================================
// Function
// ======================================
int mb_system_clear(unsigned int type);
int mb_system_get_info(unsigned int *version);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_SYSTEM_GET_INFO    (0x00000102)
#define FUNC_SYSTEM_CLEAR       (0x00000202)

#endif							/* MB_CMD_SYSTEM_H_ */
