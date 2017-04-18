/*!
 *  @file       mb_cmd_aes_securekey.h
 *  @brief      Headerfile : mailbox api for aes using securekey
 *  @author     jinsu.hyun
 *  @version    v0.10 : 2016.7.29 Init.
 */

#ifndef MB_CMD_AES_SECUREKEY_H_
#define MB_CMD_AES_SECUREKEY_H_

#include "mb_cmd_aes.h"

// ======================================
// Function
// ======================================
int mb_aes_ecb_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_ecb_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_ecb_final_securekey(struct sAES_PARAM *aes_param);

int mb_aes_cbc_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_cbc_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_cbc_final_securekey(struct sAES_PARAM *aes_param);

int mb_aes_ctr_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_ctr_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_ctr_final_securekey(struct sAES_PARAM *aes_param);

int mb_aes_xts_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_xts_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_xts_final_securekey(struct sAES_PARAM *aes_param);

int mb_aes_ccm_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_ccm_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_ccm_final_securekey(struct sAES_PARAM *aes_param, unsigned int enc);

int mb_aes_gcm_init_securekey(struct sAES_PARAM *aes_param, unsigned int enc);
int mb_aes_gcm_update_securekey(struct sAES_PARAM *aes_param);
int mb_aes_gcm_final_securekey(struct sAES_PARAM *aes_param, unsigned int enc);

int mb_aes_generate_key_securekey(unsigned int key_byte_len, unsigned int key_index);

// ======================================
// Mailbox Command
// ======================================
#define FUNC_AES_ECB_INIT_SECUREKEY		(0x00043302)
#define FUNC_AES_ECB_UPDATE_SECUREKEY	(0x00053302)
#define FUNC_AES_ECB_FINAL_SECUREKEY	(0x00063302)

#define FUNC_AES_CBC_INIT_SECUREKEY		(0x00143302)
#define FUNC_AES_CBC_UPDATE_SECUREKEY	(0x00153302)
#define FUNC_AES_CBC_FINAL_SECUREKEY	(0x00163302)

#define FUNC_AES_CTR_INIT_SECUREKEY		(0x00243302)
#define FUNC_AES_CTR_UPDATE_SECUREKEY	(0x00253302)
#define FUNC_AES_CTR_FINAL_SECUREKEY	(0x00263302)

#define FUNC_AES_XTS_INIT_SECUREKEY		(0x00343302)
#define FUNC_AES_XTS_UPDATE_SECUREKEY	(0x00353302)
#define FUNC_AES_XTS_FINAL_SECUREKEY	(0x00363302)

#define FUNC_AES_CCM_INIT_SECUREKEY		(0x00443302)
#define FUNC_AES_CCM_UPDATE_SECUREKEY	(0x00453302)
#define FUNC_AES_CCM_FINAL_SECUREKEY	(0x00463302)

#define FUNC_AES_GCM_INIT_SECUREKEY		(0x00543302)
#define FUNC_AES_GCM_UPDATE_SECUREKEY	(0x00553302)
#define FUNC_AES_GCM_FINAL_SECUREKEY	(0x00563302)

#define FUNC_AES_KEYGEN_SECUREKEY		(0x00A13302)

#endif							/* MB_CMD_AES_SECUREKEY_H_ */
