/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_SAE_H_
#define __RTW_SAE_H_

#include "rtw_opt_crypto_ssl.h"

#if (CONFIG_SAE_DH_SUPPORT == 1)
#include "dh_groups.h"
#endif
/*********************************************************************************************

                     						Micro Defination

*********************************************************************************************/
#define SAE_KCK_LEN 		32
#define SAE_PMK_LEN 		32
#define SAE_PMKID_LEN 		16
#define SAE_GRP_SIZE 		10			/* SAE group size: 6 ECC group + 4 FFC group */
#define SAE_PWE_MAX_CNT  	40
#define SAE_MAX_PRIME_LEN	512
#define SAE_MAX_ECC_PRIME_LEN 66

#define SAE_SEQ_COMMIT 		1
#define SAE_SEQ_CONFIRM 	2

#define dot11RSNASAESync    5

/* Special value returned by sae_parse_commit() */
#define SAE_SILENTLY_DISCARD 65535


#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif
/*********************************************************************************************

                     						Data Structure

*********************************************************************************************/

enum sae_state {
	SAE_NOTHING,
	SAE_COMMITTED,
	SAE_CONFIRMED,
	SAE_ACCEPTED
};

enum sae_type {
	_SAE_TYPE_COMMIT_,
	_SAE_TYPE_CONFIRM_
};

enum sae_group_config {
	_GRP_KEEP_CURRENT_,		/* keep current group id */
	_GRP_AUTO_SELECT_, 		/* auto select a group id in sae_groups[] */
	_GRP_USER_CONFIG_		/* manually config the group id through user cmd */
};

/*
**  event defination(32bits):
**
**  [ 31:16 ]	[ ~ ]	   [ 9 ]	   [ 8 ]	   [ 7 ]	 [ 6 ]		[ 5 ]	 [ 4 ]	  [ 3 ]   [ 2 ]  [ 1 ]	[ 0 ]
**   status  | reserve | moregroups | lowmac | highmac | badauth | diffgroup | badgroup | badid  | con	| com  | init
*/
enum sae_event {
	_SAE_INIT_ 		 = BIT(0),
	_SAE_COM_  		 = BIT(1),
	_SAE_CON_  		 = BIT(2),
	_SAE_BADID_ 	 = BIT(3),
	_SAE_BADGROUP_ 	 = BIT(4),
	_SAE_DIFFGROUP_  = BIT(5),
	_SAE_BADAUTH_	 = BIT(6),
	_SAE_HIGMAC_	 = BIT(7),
	_SAE_LOWMAC_	 = BIT(8),
	_SAE_MOREGROUPS_ = BIT(9),
};

struct sae_octect_buf {
	u8 *pbuf;
	u8  length;
};

struct sae_data {
	_adapter	*padapter;	/* pointer to adapter */
	enum sae_state state;
	u16 sc;					/* own send-confirm counter */
	u16 rc;					/* peer send-confirm counter */
	u32 sync;	 			/* protocol instance variable: Sync */
	u16 group;				/* FCC(Finite Cyclic Cryptography) group ID */
	u32 sae_grp_idx;		/* index to the current group in the sae_groups	*/
	u16 last_peer_cmit_seq_num;	/* seq number of commit frame in wlan header */
	u16 last_peer_cfrm_seq_num;	/* seq number of confirm frame in wlan header */
	u16 last_status;		/* status code */
	u8 h2e;					/* hash to element support */
	u8 pmk[SAE_PMK_LEN];
	u8 pmkid[SAE_PMKID_LEN];
	u8 kck[SAE_KCK_LEN];

	sae_crypto_bignum order;
	u32 order_len;
	sae_crypto_bignum prime;
	u32 prime_len;

	sae_crypto_bignum rand;
	sae_crypto_bignum mask;

	sae_crypto_bignum own_commit_scalar;
	sae_crypto_bignum peer_commit_scalar;

	sae_ecc_crypto *ecc;
	sae_ecc_point pwe_ecc;
	sae_ecc_point own_commit_element_ecc;
	sae_ecc_point peer_commit_element_ecc;
	sae_ecc_point pt_ecc;

	const struct dh_group *dh;
	sae_crypto_bignum pwe_ffc;
	sae_crypto_bignum own_commit_element_ffc;
	sae_crypto_bignum peer_commit_element_ffc;
	sae_crypto_bignum pt_ffc;

	/*
	** password to generate PWE,get from wpa passphase
	** or password identifier from auth frame
	*/
	struct sae_octect_buf sae_password;
	struct sae_octect_buf anti_clogging_token;

};

/*********************************************************************************************

                     						Debug Info

*********************************************************************************************/

extern int sae_debug_level;

//#define SAE_DEBUG_ENABLE
#define _SAE_MSG_INFO_	2
#define _SAE_MSG_ERR_	1

#ifdef SAE_DEBUG_ENABLE
#define sae_printf(level, fmt, arg...)     \
do {\
	if(level <= sae_debug_level)	\
		printf("\r\nSAE: " fmt, ##arg);\
}while(0)
#else
#define sae_printf(level, fmt, arg...)
#endif


/*********************************************************************************************

                     						SAE Function

*********************************************************************************************/


/*********************************************************************************************
Function:  compare s1 and s2

Parameter: @s1: input 1
           @s2: input 2
           @n: length of input string

return:    0: if equal
           >0: if s1 > s2
           <0: if s1 < s2
*********************************************************************************************/
int sae_memcmp(void *s1, void *s2, u8 n);


/*********************************************************************************************
Function:  shift buffer

Parameter: @buf: pointer of buffer to shift
           @len: the length of buffer
           @bits: bit counter of buffer

return:    void
*********************************************************************************************/
void sae_buf_shift_right(u8 *buf, size_t len, size_t bits);


/*********************************************************************************************
Function:  check if sync is greater than dot11RSNASAESync

Parameter: @sae: pointer to sae data

result:    1: yes; 0: no
*********************************************************************************************/
int rtw_sae_check_big_sync(struct sae_data *sae);


/*********************************************************************************************
Function:  switch sae state

Parameter: @sae: 	   pointer to sae data
           @peer_addr: peer mac address
           @sate:  	   enum sae_state{}
           @reason:    a string for mark

result:    void
*********************************************************************************************/
void rtw_sae_set_state(struct sae_data *sae, u8 *peer_addr, enum sae_state state, const char *reason);


/*********************************************************************************************
Function:  ecc initialization of group id

Parameter: @group_id: group definition of spec, the default value is 19/20/21/25/26

return:    sae_ecc_crypto *: pointer of cryto ecc
*********************************************************************************************/
sae_ecc_crypto *sae_crypto_ecc_init(unsigned char group_id);


/*********************************************************************************************
Function:  ecc clear

Parameter: @pointer of ecc group

return:    void
*********************************************************************************************/
void sae_crypto_ecc_free(sae_ecc_crypto *ecc);


/*********************************************************************************************
Function:  generate SAE commit msg
		   1.)select a finite cyclic group
		   2.)generate pwe
		   3.)generate rand/mask
		   4.)generate commit-scalar
		   5.)generate commit-element

Parameter: @sae:       pointer to sae_data
           @own_addr:  pointer to own mac address
           @peer_add:  pointer to peer mac address
           @group_config: _GRP_KEEP_CURRENT_, _GRP_AUTO_SELECT_, _GRP_USER_CONFIG_

result:    0:success; -1: fail
*********************************************************************************************/
int rtw_sae_generate_commit_msg(struct sae_data *sae, u8 *own_addr, u8 *peer_addr, enum sae_group_config group_config);


/*********************************************************************************************
Function:  prepare SAE commit frame

Parameter: @pframe:		   pointer to next position of left frame buffer
           @sae:		   pointer to sae data
           @available_len: available size of buffer
           @frame_len:     current size of frame
           @status: 	   status code response to peer STA

result:    pointer to position of next available memeroy
*********************************************************************************************/
u8 *rtw_sae_prepare_commit(u8 *pframe,	struct sae_data *sae, u32 available_len,	u32 *frame_len, u16 status);


/*********************************************************************************************
Function:  prepare SAE confirm frame

Parameter: @pframe:		   pointer to next position of left frame buffer
           @sae:		   pointer to sae data
           @available_len: available size of buffer
           @frame_len: 	   current size of frame

result:    pointer to position of next available memeroy
*********************************************************************************************/
u8 *rtw_sae_prepare_confirm(u8 *pframe, struct sae_data *sae, u32 available_len,	u32 *frame_len);


/*********************************************************************************************
Function:  derive PWE

Parameter: @sae_priv: pointer of sae_data
           @addr1: array of local mac address
           @addr2: array of peer mac address
           @password: array of password denote as ASCII
           @password_len: password size

return:    0 if successful
           -1 if failed
*********************************************************************************************/
int sae_derive_pwe_ecc(struct sae_data *sae_priv, unsigned char *addr1, unsigned char *addr2,
					   unsigned char *password, unsigned int password_len);


/*********************************************************************************************
Function:  derive PWE for ffc

Parameter: @sae_priv: pointer of sae_data
           @addr1: array of local mac address
           @addr2: array of peer mac address
           @password: array of password denote as ASCII
           @password_len: password size

return:    0 if successful
           -1 if failed
*********************************************************************************************/
int sae_derive_pwe_ffc(struct sae_data *sae_priv, unsigned char *addr1, unsigned char *addr2,
					   unsigned char *password, unsigned int password_len);


/*********************************************************************************************
Function:  derive k
		   K = scalar-op(rand, (elem-op(scalar-op(peer-commit-scalar, PWE),PEER-COMMIT-ELEMENT)))
		   If K is identity element (point-at-infinity), reject
		   k = F(K) (= x coordinate)

Parameter: @sae_priv: pointer of sae_data
           @k: output key

return:    0 if successful
           -1 if failed
*********************************************************************************************/
int sae_derive_k_ecc(struct sae_data *sae_priv, unsigned char *k);


/*********************************************************************************************
Function:  derive k ffc
		   K = scalar-op(rand, (elem-op(scalar-op(peer-commit-scalar, PWE),PEER-COMMIT-ELEMENT)))
		   If K is identity element (point-at-infinity), reject
		   k = F(K) (= x coordinate)

Parameter: @sae_priv: pointer of sae_data
           @k: output key

return:    0 if successful
           -1 if failed
*********************************************************************************************/
int sae_derive_k_ffc(struct sae_data *sae_priv, unsigned char *k);


/*********************************************************************************************
Function:  derive pmk,kck and pmkid
		   keyseed = H(<0>32, k)
	  	   KCK || PMK = KDF-512(keyseed, "SAE KCK and PMK",(commit-scalar + peer-commit-scalar) modulo r)
	  	   PMKID = L((commit-scalar + peer-commit-scalar) modulo r, 0, 128)

Parameter: @sae_priv: pointer of sae_data
           @k: output key

return:    0 if successful
           -1 if failed
*********************************************************************************************/
int sae_derive_pmk_and_kck(struct sae_data *sae_priv, unsigned char *k, unsigned int k_len);


/*********************************************************************************************
Function:  handle the sae auth and trigger the state machine

Parameter: @padater:     pointer to device
		   @psta:        pointer to sta_info
		   @sae: 	     pointer to sae data
		   @own_addr:    local mac address
		   @peer_addr:   peer mac address
		   @sae_msg:     pointer to sae message(the part after status code in the auth frame)
		   @sae_msg_len: sae msg length
		   @seq_num:  	 auth seq num in the received auth frame
		   @status:		 statue code in the received auth frame

result:    0: no error; -1; some error happened
*********************************************************************************************/
int rtw_sae_handle_auth(_adapter *padapter, struct sta_info *psta, struct sae_data *sae,
						u8 *own_addr, u8 *peer_addr, u8 *sae_msg, u32 sae_msg_len, u16 seq_num, u16 status);


/*********************************************************************************************
Function:  allocate and start a sae instance

Parameter: @padater: pointer to device

result:    pointer to new sae data
*********************************************************************************************/
struct sae_data *rtw_sae_allocate_instance(_adapter *padapter);


/*********************************************************************************************
Function:  remove sae instance and free memory

Parameter: @sae: pointer to sae data

result:    void
*********************************************************************************************/
void rtw_sae_free_instance(_adapter *padapter, const char *reason);

#endif	//__RTW_SAE_H_

