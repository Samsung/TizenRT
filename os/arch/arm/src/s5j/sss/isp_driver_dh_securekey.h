/*!
 *  @file		isp_driver_dh_securekey.h
 *  @brief		Headerfile : isp driver for dh securekey
 *  @author		jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_DH_SECUREKEY_H_
#define ISP_DRIVER_DH_SECUREKEY_H_

#include "mb_cmd_dh_securekey.h"

int isp_dh_generate_param_securekey(struct sDH_PARAM *o_dh_param, unsigned int dh_param_index);
/*
 * input fixed dh_param p
 * output random generator g & key pair
 */

int isp_dh_generate_keypair_secureparam(struct sDH_PARAM *o_dh_param, unsigned int key_index);
/*
 * output dh_param p, g, pukey
 */

//added
int isp_dh_generate_keypair_userparam_securestorage(struct sDH_PARAM *i_dh_param, unsigned int dh_param_index);
/*
 * input dh_param p, g & output dh_param pukey
 */

int isp_dh_get_pubkey_securestorage(struct sDH_PARAM *o_dh_param, unsigned int key_index);
/*
 * get public key from existing key pair from secure storage
 */

int isp_dh_compute_shared_secret_securekey(unsigned char *shared_secret, unsigned int *shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned int key_index);

#endif							/* ISP_DRIVER_DH_SECUREKEY_H_ */
