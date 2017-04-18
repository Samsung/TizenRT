/*!
 *  @file       mb_cmd_dh_securekey.h
 *  @brief      Headerfile : mailbox command for dh securekey
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef MB_CMD_DH_SECUREKEY_H_
#define MB_CMD_DH_SECUREKEY_H_

#include "mb_cmd_dh.h"

// ======================================
// Function
// ======================================
//1. int isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index);
int mb_dh_generate_key_userkey_step1(struct sDH_PARAM *i_dh_param, unsigned int object_id);	//p
int mb_dh_generate_key_userkey_step2(struct sDH_PARAM *i_dh_param, unsigned int object_id);	//g
int mb_dh_generate_key_userkey_step3(struct sDH_PARAM *i_dh_param, unsigned int object_id);	//q
int mb_dh_generate_key_userkey_step4(unsigned int object_id);	//execution
int mb_dh_generate_key_securekey_step5(unsigned int object_id);	//before secure storage
//store secure storage
int mb_dh_generate_key_userkey_step6(struct sDH_PARAM *o_dh_param, unsigned int object_id);	//pukey

//2. int isp_dh_generate_keypair_secureparam(struct sDH_PARAM *o_dh_param, unsigned int key_index);
//get key from secure storage
int mb_dh_generate_key_securekey_step1(unsigned int object_id);
//set key store secure storage
int mb_dh_generate_param_userkey_step7(struct sDH_PARAM *o_dh_param, unsigned int object_id);	//output p
int mb_dh_generate_param_userkey_step8(struct sDH_PARAM *o_dh_param, unsigned int object_id);	//output g

//3. int isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, struct sDH_KEY* dh_publickey, unsigned int key_index);
int mb_dh_compute_shared_secret_securekey_step1(struct sDH_PARAM i_dh_pukey, unsigned int object_id);	//other pukey
//get key
int mb_dh_compute_shared_secret_securekey_step2(unsigned int object_id);	//load param from secure storage
int mb_dh_compute_shared_secret_securekey_step3(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, unsigned int object_id);

//4. int isp_dh_generate_param_securekey(struct sDH_PARAM *o_dh_param, unsigned int dh_param_index);
int mb_dh_generate_param_securekey_step1(unsigned int object_id);

//5. get public key from secure storage
int mb_dh_get_pubkey_securekey_step1(unsigned int object_id);
int mb_dh_get_pubkey_securekey_step2(unsigned int object_id);	//execution

//6. generate parameter except prime
int mb_dh_generate_param_generator_step1(unsigned int object_id);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_DH_GEN_KEY_USERKEY1						(0x00012402)
#define FUNC_DH_GEN_KEY_USERKEY2						(0x00022402)
#define FUNC_DH_GEN_KEY_USERKEY3						(0x00032402)
#define FUNC_DH_GEN_KEY_USERKEY4						(0x00042402)
#define FUNC_DH_GEN_KEY_SECUREKEY5						(0x01052402)
#define FUNC_DH_GEN_KEY_USERKEY6						(0x00062402)

#define FUNC_DH_GEN_KEY_SECUREKEY1						(0x03012402)
#define FUNC_DH_GEN_PARAM_USERKEY7						(0x00022302)
#define FUNC_DH_GEN_PARAM_USERKEY8						(0x00032302)

#define FUNC_DH_GET_PUKEY_SECUREKEY1					(0x01012402)
#define FUNC_DH_GET_PUKEY_SECUREKEY2					(0x01042402)

#define FUNC_DH_COMPUTE_SHARED_SECRET_SECUREKEY1		(0x00012502)
#define FUNC_DH_COMPUTE_SHARED_SECRET_SECUREKEY2		(0x01022502)
#define FUNC_DH_COMPUTE_SHARED_SECRET_SECUREKEY3		(0x00052502)

#define FUNC_DH_GEN_PARAM_SECUREKEY1					(0x00012302)
#define FUNC_DH_GEN_GENERATOR_SECUREKEY1				(0x01012302)

#endif							/* MB_CMD_DH_SECUREKEY_H_ */
