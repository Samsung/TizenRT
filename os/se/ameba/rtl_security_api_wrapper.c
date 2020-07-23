/**************************************************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied. See the License for the specific
* language governing permissions and limitations under the License.
*
***************************************************************************************************/

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <tinyara/kmalloc.h>

#include "flash_api.h"
#include "device_lock.h"

/* Function return Local Enum define */
#define RTL_HAL_SUCCESS 0
#define RTL_HAL_NOT_SUPPORTED 15
#define RTL_HAL_FAIL 18

typedef enum {
	RTL_OPERATION_NONE = -1,
	RTL_DECRYPT = 0,
	RTL_ENCRYPT,
} rtl_operation_t;

/* AES */
static const unsigned char aes_iv[16] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

/* Secure efuse key location */
#define SAMSUNG_KEY_ADDR 0x150
#define SAMSUNG_KEY_SIZE 32

/* Factory Cert Key */
#define FACTORY_CERT_ADDR CONFIG_SE_FACTORY_CERT_ADDRESS
#define FACTORY_KEY_ADDR CONFIG_SE_FACTORY_KEY_ADDRESS

/* Secure Storage Variable */
#define SEC_STORE_OFFSET CONFIG_SE_SSTORAGE_ADDRESS
#define TAG_LENGTH 16

static const unsigned char aes_test_iv_gcm[12] = {
	0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
	0xde, 0xca, 0xf8, 0x88
};

static const unsigned char aes_test_aad_gcm[20] = {
	0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
	0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
	0xab, 0xad, 0xda, 0xd2
};

/* Hal Data struct define */
typedef struct _hal_data {
	void *data;
	uint32_t data_len;
	void *priv;
	uint32_t priv_len;
} hal_data;

static uint8_t samsung_key[32] = { 0 };
static uint8_t crypto_inited = 0;

/**
 * Read secure efuse
 */
static void rtl_ReadEfuse(uint32_t addr, uint8_t size, uint8_t *key)
{
	uint32_t Idx = 0;
	uint32_t CtrlSetting = HAL_READ32(SYSTEM_CTRL_BASE, REG_HS_EFUSE_CTRL1_S);
	
	for (Idx = 0; Idx < size; Idx++) {
		EFUSERead8(CtrlSetting, addr + Idx, &key[Idx], L25EOUTVOLTAGE);
	}
}

/**
 * Hardware Crpyto Wrapper
 */
uint32_t rtl_cryptoEngine_init_wrapper(void)
{
	uint32_t ret = RTL_HAL_SUCCESS;

	if (!crypto_inited) {
		device_mutex_lock(RT_DEV_LOCK_CRYPTO);
		ret = rtl_cryptoEngine_init();	/* init crypto engine */
		device_mutex_unlock(RT_DEV_LOCK_CRYPTO);

		if (ret == RTL_HAL_SUCCESS) {
			crypto_inited = 1;
		} else {
			return RTL_HAL_FAIL;
		}
	}

	/* Read Samsung Key for Decyption */
	device_mutex_lock(RT_DEV_LOCK_EFUSE);
	rtl_ReadEfuse(SAMSUNG_KEY_ADDR, SAMSUNG_KEY_SIZE, samsung_key);
	device_mutex_unlock(RT_DEV_LOCK_EFUSE);

	return ret;
}

/**
 * AES Crpyto Wrapper
 */
uint32_t rtl_cryptoAES_ecb_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode)
{
	uint32_t ret = RTL_HAL_SUCCESS;

	device_mutex_lock(RT_DEV_LOCK_CRYPTO);
	ret = rtl_crypto_aes_ecb_init(key, keylen);

	/* Init Success */
	if (ret == RTL_HAL_SUCCESS) {
		if (mode == RTL_ENCRYPT) {
			/* Encryption operation */
			ret = rtl_crypto_aes_ecb_encrypt(message, msglen, NULL, 0, pResult);
		} else if (mode == RTL_DECRYPT) {
			/* Decryption operation */
			ret = rtl_crypto_aes_ecb_decrypt(message, msglen, NULL, 0, pResult);
		} else {
			ret = RTL_HAL_NOT_SUPPORTED;
		}
	} else {
		ret = RTL_HAL_FAIL;
	}
	device_mutex_unlock(RT_DEV_LOCK_CRYPTO);

	return ret;
}

uint32_t rtl_cryptoAES_cbc_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode)
{
	uint32_t ret = RTL_HAL_SUCCESS;
	unsigned char iv[16];

	memcpy(iv, aes_iv, sizeof(aes_iv));
	device_mutex_lock(RT_DEV_LOCK_CRYPTO);
	ret = rtl_crypto_aes_cbc_init(key, keylen);

	if (ret == RTL_HAL_SUCCESS) {
		if (mode == RTL_ENCRYPT) {
			/* Encryption operation */
			ret = rtl_crypto_aes_cbc_encrypt(message, msglen, iv, sizeof(iv), pResult);
		} else if (mode == RTL_DECRYPT) {
			/* Decryption operation */
			ret = rtl_crypto_aes_ecb_decrypt(message, msglen, iv, sizeof(iv), pResult);
		} else {
			ret = RTL_HAL_NOT_SUPPORTED;
		}
	} else {
		ret = RTL_HAL_FAIL;
	}
	device_mutex_unlock(RT_DEV_LOCK_CRYPTO);

	return ret;
}

uint32_t rtl_cryptoAES_ctr_wrapper(uint8_t *key, uint32_t keylen, unsigned char *message, uint32_t msglen, unsigned char *pResult, uint8_t mode)
{
	uint32_t ret = RTL_HAL_SUCCESS;
	unsigned char iv[16];

	memcpy(iv, aes_iv, sizeof(aes_iv));
	device_mutex_lock(RT_DEV_LOCK_CRYPTO);
	ret = rtl_crypto_aes_ctr_init(key, keylen);

	if (ret == RTL_HAL_SUCCESS) {
		if (mode == RTL_ENCRYPT) {
			/* Encryption operation */
			ret = rtl_crypto_aes_ctr_encrypt(message, msglen, iv, sizeof(iv), pResult);
		} else if (mode == RTL_DECRYPT) {
			/* Decryption operation */
			ret = rtl_crypto_aes_ctr_decrypt(message, msglen, iv, sizeof(iv), pResult);
		} else {
			ret = RTL_HAL_NOT_SUPPORTED;
		}
	} else {
		ret = RTL_HAL_FAIL;
	}
	device_mutex_unlock(RT_DEV_LOCK_CRYPTO);

	return ret;
}

/**
 * Factory Key and Cert
 */
uint32_t rtl_read_factory_cert_wrapper(hal_data *data)
{
	uint32_t ret;
	uint32_t data_len;
	uint8_t buf_length[4];
	uint8_t *buf_flash = NULL;
	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, FACTORY_CERT_ADDR, 4, buf_length);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	data_len = atoi(buf_length);	/* Cert Length */

	buf_flash = (uint8_t *)kmm_malloc(data_len + 4);	/* Allocate buff */

	if (buf_flash) {
		memset(buf_flash, 0, data_len + 4);

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_stream_read(&flash, FACTORY_CERT_ADDR, data_len + 4, buf_flash);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		memcpy(data->data, buf_flash + 4, data_len);	/* Copy Cert into output buff */
		data->data_len = data_len;

		ret = RTL_HAL_SUCCESS;
	} else {
		ret = RTL_HAL_FAIL;
	}

	if (buf_flash != NULL) {
		kmm_free(buf_flash);
	}

	return ret;
}

uint32_t rtl_write_factory_cert_wrapper(hal_data *data)
{
	uint32_t ret;
	uint8_t buf_length[4];
	flash_t flash;
	uint32_t data_len = data->data_len;

	uint8_t	*buf_flash = (uint8_t *)kmm_malloc(data_len + 4);

	if (buf_flash) {
		itoa(data_len, buf_length, 10);
		memcpy(buf_flash, buf_length, 4);
		memcpy(buf_flash + 4, data->data, data->data_len);

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_erase_sector(&flash, FACTORY_CERT_ADDR);
		flash_stream_write(&flash, FACTORY_CERT_ADDR, data_len + 4, buf_flash);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		ret = RTL_HAL_SUCCESS;
		kmm_free(buf_flash);
	} else {
		ret = RTL_HAL_FAIL;
	}

	return ret;
}

uint32_t rtl_read_factory_key_wrapper(hal_data *data)
{
	uint32_t ret;
	uint32_t data_len;
	uint8_t buf_length[4];
	uint8_t *buf_plain = NULL;
	uint8_t *buf_crypt = NULL;
	uint8_t *buf_flash = NULL;
	flash_t flash;
	unsigned char aes_key[32] __attribute__((aligned(4)));

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, FACTORY_KEY_ADDR, 4, buf_length);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	data_len = atoi(buf_length);	/* Key Length */

	/* Allocate buff */
	buf_flash = (uint8_t *)kmm_malloc(data_len + 4);
	buf_crypt = (uint8_t *)kmm_malloc(data_len);
	buf_plain = (uint8_t *)kmm_malloc(data_len);

	if (buf_plain && buf_crypt && buf_flash) {
		memset(buf_flash, 0, data_len + 4);
		memcpy(aes_key, samsung_key, sizeof(samsung_key));

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_stream_read(&flash, FACTORY_KEY_ADDR, data_len + 4, buf_flash);	/* Read data */
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		memcpy(buf_crypt, buf_flash + 4, data_len);

		/* Decrypt data */
		ret = rtl_cryptoAES_ecb_wrapper(aes_key, sizeof(aes_key), buf_crypt, data_len, buf_plain, RTL_DECRYPT);

		if (ret == RTL_HAL_SUCCESS) {
			memcpy(data->data, buf_plain, data_len);	/* Copy Key into output buff */
			data->data_len = data_len;
		}
	} else {
		ret = RTL_HAL_FAIL;
	}

	if (buf_flash != NULL) {
		kmm_free(buf_flash);
	}
	if (buf_plain != NULL) {
		kmm_free(buf_plain);
	}
	if (buf_crypt != NULL) {
		kmm_free(buf_crypt);
	}

	return ret;
}

uint32_t rtl_write_factory_key_wrapper(hal_data *data)
{
	uint32_t ret;
	uint8_t buf_length[4];
	uint8_t	*buf_crypt = NULL;
	uint8_t	*buf_flash = NULL;
	uint8_t	*buf_plain = NULL;
	unsigned char aes_key[32] __attribute__((aligned(4)));
	flash_t flash;
	uint32_t data_len = (data->data_len + 15) / 16 * 16;	/* AES blocks */

	buf_crypt = (uint8_t *)kmm_malloc(data_len);
	buf_flash = (uint8_t *)kmm_malloc(data_len + 4);
	buf_plain = (uint8_t *)kmm_malloc(data_len);

	if (buf_crypt && buf_flash && buf_plain) {
		memset(buf_crypt, 0, data_len);
		memset(buf_flash, 0, data_len + 4);
		memset(buf_plain, 0, data_len);
		memcpy(buf_plain, data->data, data->data_len);

		memset(aes_key, 0, SAMSUNG_KEY_SIZE);
		memcpy(aes_key, samsung_key, SAMSUNG_KEY_SIZE);

		ret = rtl_cryptoAES_ecb_wrapper(aes_key, SAMSUNG_KEY_SIZE, buf_crypt, data_len, buf_plain, RTL_ENCRYPT);

		if (ret != 0) {
			goto exit;
		}
		itoa(data_len, buf_length, 10);
		memcpy(buf_flash, buf_length, 4);
		memcpy(buf_flash + 4, buf_crypt, data_len);

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_erase_sector(&flash, FACTORY_KEY_ADDR);
		flash_stream_write(&flash, FACTORY_KEY_ADDR, data_len + 4, buf_flash);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		ret = RTL_HAL_SUCCESS;
	} else {
		ret = RTL_HAL_FAIL;
	}

exit:
	if (buf_crypt != NULL) {
		kmm_free(buf_crypt);
	}
	if (buf_flash != NULL) {
		kmm_free(buf_flash);
	}
	if (buf_plain != NULL) {
		kmm_free(buf_plain);
	}

	return ret;
}

/**
 * Secure Storage
 */
uint32_t rtl_write_storage_wrapper(uint32_t ss_idx, hal_data *data)
{
	uint32_t ret;
	uint8_t tmpbuf[64];
	uint8_t buf_length[4];
	uint8_t *key = (uint8_t*)(((uint32_t)tmpbuf + 31) & (~31));	/* align to 32byte */
	flash_t flash;
	uint32_t data_len = (data->data_len + 15) / 16 * 16;	/* AES blocks */
	const uint8_t aes_gcm_iv[12];
	const uint8_t aes_gcm_aad[20];

	uint8_t	*buf = (uint8_t *)kmm_malloc(data_len + TAG_LENGTH);
	uint8_t	*buf_flash = (uint8_t *)kmm_malloc(data_len + TAG_LENGTH + 4);
	uint8_t	*data_plain = (uint8_t *)kmm_malloc(data_len);
	uint8_t	*tag = buf + data_len;

	memcpy(aes_gcm_iv, aes_test_iv_gcm, sizeof(aes_test_iv_gcm));
	memcpy(aes_gcm_aad, aes_test_aad_gcm, sizeof(aes_test_aad_gcm));

	if (buf && buf_flash && data_plain) {
		memset(buf, 0, data_len + TAG_LENGTH);
		memset(buf_flash, 0, data_len + TAG_LENGTH + 4);
		memset(data_plain, 0, data_len);
		memcpy(data_plain, data->data, data->data_len);

		if (!crypto_inited) {
			device_mutex_lock(RT_DEV_LOCK_CRYPTO);
			ret = rtl_cryptoEngine_init();
			device_mutex_unlock(RT_DEV_LOCK_CRYPTO);
			if (ret == RTL_HAL_SUCCESS) {
				crypto_inited = 1;
			} else {
				sedbg("\n\r ERROR: rtl_cryptoEngine_init \n\r");
				ret = RTL_HAL_FAIL;
				goto exit;
			}
		}
		memset(key, 0, sizeof(key));
		memcpy(key, samsung_key, SAMSUNG_KEY_SIZE);

		device_mutex_lock(RT_DEV_LOCK_CRYPTO);
		ret = rtl_crypto_aes_gcm_init(key, 32);
		device_mutex_unlock(RT_DEV_LOCK_CRYPTO);
		if (ret != 0) {
			sedbg("\n\r ERROR: crypto_aes_gcm_init \n\r");
			goto exit;
		}
		device_mutex_lock(RT_DEV_LOCK_CRYPTO);
		ret = rtl_crypto_aes_gcm_encrypt(data_plain, data_len, aes_gcm_iv, aes_gcm_aad, sizeof(aes_gcm_aad), buf, tag);
		device_mutex_unlock(RT_DEV_LOCK_CRYPTO);

		if (ret != 0) {
			sedbg("\n\r ERROR: crypto_aes_gcm_encrypt \n\r");
			goto exit;
		}
		itoa(data_len, buf_length, 10);
		memcpy(buf_flash, buf_length, 4);
		memcpy(buf_flash + 4, buf, data_len + TAG_LENGTH);

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_erase_sector(&flash, SEC_STORE_OFFSET + ss_idx * 4096);
		flash_stream_write(&flash, SEC_STORE_OFFSET + ss_idx * 4096, data_len + TAG_LENGTH + 4, buf_flash);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		ret = RTL_HAL_SUCCESS;
	} else {
		ret = RTL_HAL_FAIL;
	}

exit:
	if (buf != NULL) {
		kmm_free(buf);
	}
	if (buf_flash != NULL) {
		kmm_free(buf_flash);
	}
	if (data_plain != NULL) {
		kmm_free(data_plain);
	}

	return ret;
}

uint32_t rtl_read_storage_wrapper(uint32_t ss_idx, hal_data *data)
{
	uint32_t ret;
	uint32_t data_len;
	uint8_t dec_tag[16];
	uint8_t tmpbuf[64];
	uint8_t buf_length[4];
	uint8_t *key = (uint8_t*)(((uint32_t)tmpbuf + 31) & (~31));	/* align to 32byte */
	flash_t flash;
	const uint8_t aes_gcm_iv[12];
	const uint8_t aes_gcm_aad[20];

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, SEC_STORE_OFFSET + ss_idx * 4096, 4, buf_length);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	data_len = atoi(buf_length);

	uint8_t *buf = (uint8_t *)kmm_malloc(data_len + TAG_LENGTH);
	uint8_t *buf_flash = (uint8_t *)kmm_malloc(data_len + TAG_LENGTH + 4);
	uint8_t *data_plain = (uint8_t *)kmm_malloc(data_len);
	uint8_t *tag = buf + data_len;

	memcpy(aes_gcm_iv, aes_test_iv_gcm, sizeof(aes_test_iv_gcm));
	memcpy(aes_gcm_aad, aes_test_aad_gcm, sizeof(aes_test_aad_gcm));

	if (buf && buf_flash && data_plain) {
		memset(buf, 0, data_len + TAG_LENGTH);
		memset(buf_flash, 0, data_len + TAG_LENGTH + 4);
		memset(data_plain, 0, data_len);

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_stream_read(&flash, SEC_STORE_OFFSET + ss_idx * 4096, data_len + 16 + 4, buf_flash);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		if (!crypto_inited) {
			device_mutex_lock(RT_DEV_LOCK_CRYPTO);
			ret = rtl_cryptoEngine_init();
			device_mutex_unlock(RT_DEV_LOCK_CRYPTO);
			if (ret == 0) {
				crypto_inited = 1;
			} else {
				sedbg("\n\r ERROR: rtl_cryptoEngine_init \n\r");
				goto exit;
			}
		}
		memset(key, 0, sizeof(key));
		memcpy(key, samsung_key, SAMSUNG_KEY_SIZE);

		device_mutex_lock(RT_DEV_LOCK_CRYPTO);
		ret = rtl_crypto_aes_gcm_init(key, 32);
		device_mutex_unlock(RT_DEV_LOCK_CRYPTO);
		if (ret != 0) {
			sedbg("\n\r ERROR: crypto_aes_gcm_init \n\r");
			goto exit;
		}
		memcpy(buf, buf_flash + 4, data_len + 16);

		device_mutex_lock(RT_DEV_LOCK_CRYPTO);
		ret = rtl_crypto_aes_gcm_decrypt(buf, data_len, aes_gcm_iv, aes_gcm_aad, sizeof(aes_gcm_aad), data_plain, dec_tag);
		device_mutex_unlock(RT_DEV_LOCK_CRYPTO);
		if (ret != 0) {
			sedbg("\n\r ERROR: crypto_aes_gcm_decrypt \n\r");
			goto exit;
		}

		/* verify tag */
		if (memcmp(dec_tag, tag, 16) != 0) {
			sedbg("\n\r ERROR: memcmp tag \n\r");
			ret = RTL_HAL_FAIL;
			goto exit;
		}
		memcpy(data->data, data_plain, data_len);
		data->data_len = data_len;

		ret = RTL_HAL_SUCCESS;
	} else {
		ret = RTL_HAL_FAIL;
	}

exit:
	if (buf != NULL) {
		kmm_free(buf);
	}
	if (buf_flash != NULL) {
		kmm_free(buf_flash);
	}
	if (data_plain != NULL) {
		kmm_free(data_plain);
	}

	return ret;
}

uint32_t rtl_delete_storage_wrapper(uint32_t ss_idx)
{
	flash_t flash;
	flash_erase_sector(&flash, SEC_STORE_OFFSET + ss_idx * 1024);

	return 0;
}