/**************************************************************************************************
*
* Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tinyara/security_hal.h>
 
#define AWRAP_TAG "[AMEBA_WRAPPER]"
#define AWRAP_ENTER                                     \
	do {                                                \
		sedbg(AWRAP_TAG"%s:%d\n", __FILE__, __LINE__);  \
	} while (0)

#define HAL_COPY_DATA(in, out, len)                     \
	do {                                                \
		if (in->data == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->data, out, len);                     \
		in->data_len = len;                             \
	} while (0)

#define HAL_COPY_PRIV_DATA(in, out, len)                \
	do {                                                \
		if (in->priv == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->priv, out, len);                     \
		in->priv_len = len;                             \
	} while (0)

/* Max Factory Key and usable Key Index (Only allocate 8 slot) */
#define FACTORY_KEY_INDEX_MAX 32
#define USABLE_FACTORY_KEY_INDEX 8

/* Max RAM Key and Total Key Index */
#define KEY_STORAGE_INDEX_MAX 32
#define TOTAL_KEY_STORAGE_INDEX FACTORY_KEY_INDEX_MAX + KEY_STORAGE_INDEX_MAX
#define HAL_MAX_RANDOM_SIZE 256

/* Secure Storage Base Address, After Bootloader before Kernel */
/* Fix Address, should not be changed, once change previous data will be lost */
#ifdef CONFIG_AMEBAD_TRUSTZONE
#define SS_BASE_ADDRESS 0xA000
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
#define SS_BASE_ADDRESS 0x14000
#endif
#ifdef CONFIG_AMEBASMART_TRUSTZONE
#define SS_BASE_ADDRESS 0x1F0000 /* ToDo: Temporary set need changes based flash addr */
#endif


/* 8 Slots for Cert, 8 Slots for Key, 1 Slot is 4KB */
#define SE_FACTORY_KEY_SIZE 0x8000
#define SE_FACTORY_CERT_SIZE 0x8000
#define SE_FACTORY_SIZE SE_FACTORY_KEY_SIZE + SE_FACTORY_CERT_SIZE

#define CONFIG_SE_FACTORY_CERT_ADDRESS SS_BASE_ADDRESS
#define CONFIG_SE_FACTORY_KEY_ADDRESS SS_BASE_ADDRESS + SE_FACTORY_CERT_SIZE

/* Secure Storage base address locate after Factory Key and Cert */
#define CONFIG_SE_SSTORAGE_ADDRESS SS_BASE_ADDRESS + SE_FACTORY_SIZE

/* Factory Cert Key */
#define FACTORY_CERT_ADDR CONFIG_SE_FACTORY_CERT_ADDRESS
#define FACTORY_KEY_ADDR CONFIG_SE_FACTORY_KEY_ADDRESS

/* Secure Storage Variable */
#define SEC_STORE_OFFSET CONFIG_SE_SSTORAGE_ADDRESS
#define SEC_STORE_MAX_SLOT 33

/* Flash Sector Size */
#define SECTOR_SIZE 4096

/* Secure efuse key location */
#ifdef CONFIG_AMEBAD_TRUSTZONE
#define SAMSUNG_KEY_ADDR 0x150
#endif
#ifdef CONFIG_AMEBALITE_TRUSTZONE
#define SAMSUNG_KEY_ADDR 0x390
#endif
#ifdef CONFIG_AMEBASMART_TRUSTZONE
#define SAMSUNG_KEY_ADDR 0x390
#endif

/* Non-Secure Data buff, 8K (2 Sector + Tag) */
#define NS_BUF_LEN ((SECTOR_SIZE * 2) + 32)

typedef struct {
	void *ns_func_s;
	uint8_t *buf;
	uint32_t buf_len;
} ns_passin_struc;

typedef struct {
	hal_data *input_data;
	hal_data *output_data;
} inout_struc;

typedef struct {
	uint32_t factory_cert_addr;
	uint32_t factory_key_addr;
	uint32_t sstorage_addr;
	uint32_t samsung_key_addr;
	hal_key_type factory_slot_key_type[USABLE_FACTORY_KEY_INDEX];
} factory_struc;

