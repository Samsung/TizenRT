/*!
 *  @file		isp_driver_dh_encryptedkey.h
 *  @brief		Headerfile : isp driver for dh encryptedkey
 *  @author		jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_DH_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_DH_ENCRYPTEDKEY_H_

#include "mb_cmd_dh_securekey.h"

int isp_dh_generate_param_encryptedkey(struct sDH_PARAM *o_dh_param, unsigned char *encryptedkey);
int isp_dh_generate_keypair_userparam_encryptedkey(struct sDH_PARAM *i_dh_param, unsigned char *encryptedkey);
int isp_dh_generate_keypair_encryptedparam(struct sDH_PARAM *o_dh_param, unsigned char *encryptedkey);
int isp_dh_get_pubkey_encryptedparam(struct sDH_PARAM *o_dh_param, unsigned char *encryptedkey);
int isp_dh_compute_shared_secret_encryptedkey(unsigned char *shared_secret, unsigned int* shared_secret_byte_len, struct sDH_PARAM dh_publickey, unsigned char *encryptedkey);


#endif /* _ISP_DRIVER_DH_ENCRYPTEDKEY_H_ */
