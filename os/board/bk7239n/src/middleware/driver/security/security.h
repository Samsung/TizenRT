#ifndef _SEC_H_
#define _SEC_H_

#if (defined(CONFIG_SOC_BK7251))

typedef enum AES_RETURN_E {
	AES_OK              = 0,
	AES_BUSY,
	AES_KEYLEN_ERR,
} AES_RETURN;

typedef void (*sec_done_callback)(void *param);
struct sec_done_des {
	sec_done_callback callback;
	void  *param;
};

int security_aes_busy(void);
int security_aes_start(unsigned int mode);
int security_aes_init(sec_done_callback callback, void *param);
int security_aes_set_key(const unsigned char *key, unsigned int keybits);
int security_aes_set_block_data(const unsigned char *block_data);
int security_aes_get_result_data(unsigned char *pul_data);
#endif //(CONFIG_SOC_BK7251)

#endif

