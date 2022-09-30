#ifndef _RTW_PMKSA_CACHE_H_
#define _RTW_PMKSA_CACHE_H_

/*
** pmksa internal control flag
**
*/
#define rtw_pmk_cache_enable 1		/* 1:enable;  0:disable  */
/*
** pmksa internal encrypted bssid size
**
*/
#define PMK_AES_CRYPTO_BLOCKSIZE16	16

/*
** struct pmksa_cache_entry
**
*/
struct pmksa_cache_entry {
	struct pmksa_cache *ppmksa;
	struct pmksa_cache_entry *next;
	u8 pmkid[PMKID_LEN];
	u8 pmk[PMK_LEN];
	u8 addr1[ETH_ALEN];
	u32 akmp; 					/* WPA_KEY_MGMT_* */
	_timer expire_timer;
	u8 encrypted_addr1[PMK_AES_CRYPTO_BLOCKSIZE16]; /*the mac address is encrypt with password to make sure the same rounter has been connected*/
};


/*
** struct pmksa_cache
**
*/
struct pmksa_cache {
	_lock lock;
	struct pmksa_cache_entry *pmksa_entry; 		/* PMKSA cache entry list head */
	struct pmksa_cache_entry *pcurrent_pmksa;	/* current PMKSA */
	u32 pmksa_entry_count; 						/* number of entries in PMKSA cache */
	u32 use_cached_pmksa;
};


/*********************************************************************************************
Function:  init pmksa cache resource

Parameter: @ppmksa: pointer to pmksa_cache

result:    0:success; -1 fail
*********************************************************************************************/
int rtw_pmksa_cache_init(_adapter *padapter);


/*********************************************************************************************
Function:  free pmksa cache resource

Parameter: @ppmksa: pointer to pmksa_cache

result:    0:success; -1 fail
*********************************************************************************************/
int rtw_pmksa_cache_deinit(_adapter *padapter);


/*********************************************************************************************
Function:  free a pmksa cache entry

Parameter: @pentry: entry to be free

result:    none
*********************************************************************************************/
void rtw_pmksa_cache_entry_free(struct pmksa_cache_entry *pentry);


/*********************************************************************************************
Function:  used to encrypt the mac address by take password as key.

Parameter:	@passphase:	password
			@mac_addr:	mac address
			@out_addr:	the encrpyted mac_addr

result:    none
*********************************************************************************************/
void rtw_pmksa_cache_encrypt_macaddr(u8 *passphrase, u8 *mac_addr, u8 *out_addr);


/*********************************************************************************************
Function:  add a new pmksa cache entry to entry list

Parameter:	@ppmksa: pointer to pmksa_cache
			@pmk:	 PMK
			@pmkid:  PMKID
			@addr1:  peer STA mac addr
			@akmp:	 AKM suite OUI

result:    retuern pmksa_entry_count if success;
           rerturn -1 if fail
*********************************************************************************************/
int rtw_pmksa_cache_entry_add(struct pmksa_cache *ppmksa, u8 *pmk, u8 *pmkid, u8 *addr1, u32 akmp, u8 *encrypt_addr);


/*********************************************************************************************
Function:  remove a pmksa cache entry to entry list

Parameter: @ppmksa:  pointer to pmksa
		   @pdelete: pointer to the pmksa entry to be removed

result:    pointer to the head pmksa cache entry
*********************************************************************************************/
struct pmksa_cache_entry *rtw_pmksa_cache_entry_remove(struct pmksa_cache *ppmksa, struct pmksa_cache_entry *pdelete);


/*********************************************************************************************
Function:  search an entry in the cache list by peer mac addr and akmp

Parameter: @ppmksa: pointer to pmksa_cache
		   @addr1: peer STA mac addr
		   @akmp: expected akmp

result:    success: the entry searched
		   fail:    NULL
*********************************************************************************************/
struct pmksa_cache_entry *rtw_pmksa_cache_entry_search(struct pmksa_cache *ppmksa, u8 *addr1, u32 akmp);


/*********************************************************************************************
Function:  search an entry in the cache list by peer mac addr

Parameter: @ppmksa: pointer to pmksa_cache
		   @addr1: peer STA mac addr

result:    none
*********************************************************************************************/
void rtw_pmksa_cache_set_current_entry(struct pmksa_cache *ppmksa, struct pmksa_cache_entry *pentry);



/*********************************************************************************************
Function:  search an entry in the cache list by peer mac addr

Parameter: @ppmksa: pointer to pmksa_cache
		   @addr1: peer STA mac addr

result:    success: the current PMK entry
		   fail:    NULL
*********************************************************************************************/
struct pmksa_cache_entry *rtw_pmksa_cache_get_current_entry(struct pmksa_cache *ppmksa);

/*********************************************************************************************
Function:  append pmkid to RSN IE
			Check the RSN IE length
			If the RSN IE length <= 20, the RSN IE didn't include the PMKID information
			0-11th element in the array are the fixed IE
			12th element in the array is the RSNE ID
			13th element in the array is the RSNE length

Parameter: @padapter: pointer to pmksa_cache
		   @pmkid: pmkid cached
		   @ie: pointer to RSNIE
		   @ie_len: length of RSNIE

result:    ie length
*********************************************************************************************/
u32 rtw_pmksa_append_pmkid(_adapter *padapter, u8 *pmkid, u8 *ie, uint *ie_len);
//u32 rtw_pmksa_append_pmkid(_adapter *padapter,u8 *pmkid, u8 *ie, uint ie_len);

#endif
