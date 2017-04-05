/*!
 *  @file       mb_cmd_dh.h
 *  @brief      Headerfile : mailox command for dh
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */


#ifndef MB_CMD_DH_H_
#define MB_CMD_DH_H_


// ======================================
// Structure
// ======================================
//! @struct sDH_PARAM
//! @brief struct of DH parameter
struct sDH_PARAM
{
	unsigned int object_id;
	unsigned int modules_p_byte_len;
	unsigned char *modules_p;
	unsigned int generator_g_byte_len;
	unsigned char *generator_g;
	unsigned int order_q_byte_len;
	unsigned char *order_q;
	unsigned int publickey_byte_len;	
	unsigned char *publickey;
};

#endif /* MB_CMD_DH_H_ */

