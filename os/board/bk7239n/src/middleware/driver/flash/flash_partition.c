// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdlib.h>
#include <string.h>
#include <os/mem.h>
#include <driver/flash.h>
#include <driver/flash_partition.h>
#include "flash_driver.h"
#include "flash_hal.h"

#if (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
//#include "partitions_gen_ns.h"
#endif

#include <ctype.h>
#if (defined(CONFIG_PSA_MBEDTLS)) || (defined(CONFIG_MBEDTLS_ACCELERATOR))
#include "mbedtls/aes.h"
#endif

#if defined(CONFIG_TFM_READ_FLASH_NSC)
#include "tfm_flash_nsc.h"

#if defined(CONFIG_NVS_ENCRYPTION)
#include "nvs_internal.h"
#include "nvs_flash.h"
#include "nvs_sec_provider.h"
#endif

const bk_logic_partition_t *bk_partition_find_first(esp_partition_type_t type,
        esp_partition_subtype_t subtype, const char *label);

uint32_t flash_partition_get_index(const bk_logic_partition_t *partition_ptr);

#endif

#define NVS_KEY_SIZE        32 // AES-256
#define DATAUNIT_SIZE       32
#define TAG "partition"
#define PARTITION_AMOUNT 50

#define SOC_FLASH_BASE_ADDR 0x02000000

enum {
	PARTITION_PRIMARY_ALL = 0,
	PARTITION_SECONDARY_ALL,
	PARTITION_OTA,
	PARTITION_PARTITION,
	PARTITION_SPE,
	PARTITION_TFM_NS,
	PARTITION_NSPE,
	PARTITION_OTP_NV,
	PARTITION_PS,
	PARTITION_ITS,
	PARTITION_CNT,
};

const char *s_partition_name[PARTITION_CNT] = {
	// "sys_rf",
	// "sys_net",
	// "easyflash",
	// "primary_tfm_s",
	// "primary_cpu0_app",
	// "ota",
	// "user_config"
		"primary_all",
	"secondary_all",
	"ota",
	"partition",
	"primary_tfm_s",
	"primary_tfm_ns",
	"primary_cpu0_app",
	"sys_otp_nv",
	"sys_ps",
	"sys_its"
};
#if (defined(CONFIG_TFM_READ_FLASH_NSC))
	#define PARTITION_PARTITION_PHY_OFFSET   CONFIG_PARTITION_PHY_PARTITION_OFFSET
	#define PARTITION_PPC_OFFSET             0x400
	#define PARTITION_NAME_LEN               20
	#define PARTITION_ENTRY_LEN              32
	#define PARTITION_OFFSET_OFFSET          22
	#define PARTITION_SIZE_OFFSET            26
	#define PARTITION_FLAGS_OFFSET           30

	typedef struct {
		uint32_t phy_offset;
		uint32_t phy_size;
		uint32_t phy_flags;
	} partition_config_t;
	static partition_config_t partition_config[PARTITION_CNT] = {0};


const partition_map_t partition_map[] = {
    {BK_PARTITION_BOOTLOADER, "bl2", CONFIG_BL2_PHY_PARTITION_OFFSET, CONFIG_BL2_PHY_PARTITION_SIZE},
    {BK_PARTITION_APPLICATION, "primary_cpu0_app", CONFIG_PRIMARY_CPU0_APP_PHY_PARTITION_OFFSET, CONFIG_PRIMARY_CPU0_APP_PHY_PARTITION_SIZE},
    {BK_PARTITION_OTA, "ota", 0x0, 0x0},
    {BK_PARTITION_APPLICATION1, "primary_cpu0_app1", 0x0, 0x0},
    {BK_PARTITION_MATTER_FLASH, "matter", 0x0, 0x0},
    {BK_PARTITION_RF_FIRMWARE, "sys_rf", CONFIG_SYS_RF_PHY_PARTITION_OFFSET, CONFIG_SYS_RF_PHY_PARTITION_SIZE},
    {BK_PARTITION_NET_PARAM, "sys_net", CONFIG_SYS_NET_PHY_PARTITION_OFFSET, CONFIG_SYS_NET_PHY_PARTITION_SIZE},
    {BK_PARTITION_USR_CONFIG, "user_config", CONFIG_USER_CONFIG_PHY_PARTITION_OFFSET, CONFIG_USER_CONFIG_PHY_PARTITION_SIZE},
    {BK_PARTITION_OTA_FINA_EXECUTIVE, "ota2", 0x0, 0x0},
    {BK_PARTITION_APPLICATION2, "primary_cpu0_app2", 0x0, 0x0},
    {BK_PARTITION_EASYFLASH, "easyflash", CONFIG_EASYFLASH_PHY_PARTITION_OFFSET, CONFIG_EASYFLASH_PHY_PARTITION_SIZE},
    {BK_PARTITION_NVS, "nvs", CONFIG_NVS_PHY_PARTITION_OFFSET, CONFIG_NVS_PHY_PARTITION_SIZE},
    {BK_PARTITION_NVS_KEY, "nvs_key", CONFIG_NVS_KEY_PHY_PARTITION_OFFSET, CONFIG_NVS_KEY_PHY_PARTITION_SIZE},
};
#endif


#define ADDR_PHY_2_VIRT(addr)  ((((addr + 33) / 34) * 32))
#define PARTITION_IRAM         __attribute__((section(".iram")))

/// TODO: use bk_flash_partitions name for all, every soc can define self config at:
///             middleware/boards/bk7256/vnd_flash/vnd_flash.c
/// Custom can override bk_flash_partitions in project, For example:
///             projects/customization/bk7256_config2/main/vendor_flash.c
/* Logic partition on flash devices */


bk_logic_partition_t bk_flash_partitions[] = {
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "bootloader",
		.partition_start_addr      = 0xFFFFFFFF,
		.partition_length          = 0xFFFFFFFF,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "rf_firmware",
		.partition_start_addr      = 0xFFFFFFFF,
		.partition_length          = 0xFFFFFFFF,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
	{
		.partition_owner           = BK_FLASH_EMBEDDED,
		.partition_description     = "net_param",
		.partition_start_addr      = 0xFFFFFFFF,
		.partition_length          = 0xFFFFFFFF,
		.partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
	},
    {
        .partition_owner = BK_FLASH_EMBEDDED,
        .partition_description     = "easyflash",
        .partition_start_addr      = 0xFFFFFFFF,
        .partition_length          = 0xFFFFFFFF,
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
};


#if defined(CONFIG_TFM_READ_FLASH_NSC)
static void get_partition_name(bk_partition_t partition, char *name, size_t name_len) {
    for (size_t i = 0; i < sizeof(partition_map) / sizeof(partition_map[0]); ++i) {
        if (partition_map[i].partition == partition) {
            strncpy(name, partition_map[i].name, name_len - 1);
            name[name_len - 1] = '\0';
            return;
        }
    }
    strncpy(name, "unknown", name_len - 1);
    name[name_len - 1] = '\0';
}

static bk_logic_partition_t * get_partition_info(bk_partition_t partition) {
    for (size_t i = 0; i < partition_map_size; ++i) {
        static bk_logic_partition_t pt;
        if (partition_map[i].partition == partition) {
            pt.partition_start_addr =  partition_map[i].offset;
            pt.partition_length =  partition_map[i].size;
            return &pt;
        }
    }
    return NULL;
}

const size_t partition_map_size = sizeof(partition_map) / sizeof(partition_map[0]);
#endif

static bool flash_partition_is_valid(bk_partition_t partition)
{
	if (partition >= BK_PARTITION_BOOTLOADER) {
		return true;
	} else {
		return false;
	}
}

static int is_alpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

static uint32_t piece_address(uint8_t *array,uint32_t index)
{
	return ((uint32_t)(array[index]) << 24 | (uint32_t)(array[index+1])  << 16 | (uint32_t)(array[index+2])  << 8 | (uint32_t)((array[index+3])));
}

static uint16_t short_address(uint8_t *array,uint16_t index)
{
	return ((uint16_t)(array[index]) << 8 | (uint16_t)(array[index+1]));
}

#if defined(CONFIG_TFM_READ_FLASH_NSC)
static bool partitionIsEncrypt(bk_partition_t partition)
{
	bool bRet = false;
	if (flash_partition_is_valid(partition)) {
		uint8_t* buf = (uint8_t*)malloc(PARTITION_ENTRY_LEN * sizeof(uint8_t));
		if(buf == NULL){
			BK_LOGE(TAG, "memory malloc fails.\r\n");
			return bRet;
		}
		uint32_t i;
		char name[PARTITION_NAME_LEN + 1];
        get_partition_name(partition, name, sizeof(name));

		uint32_t partition_start = PARTITION_PARTITION_PHY_OFFSET + PARTITION_PPC_OFFSET;

		for(i=0;i<PARTITION_AMOUNT;++i){
		#if (defined(CONFIG_TFM_FWU))
			psa_flash_read_bytes(partition_start + PARTITION_ENTRY_LEN*i, buf, PARTITION_ENTRY_LEN);
		#else
			bk_flash_read_bytes(partition_start + PARTITION_ENTRY_LEN*i, buf, PARTITION_ENTRY_LEN);
		#endif
			if(is_alpha(buf[0]) == 0){
				break;
			}

			int j;
			char name_in_flash[PARTITION_NAME_LEN + 1];

			for(j=0; j<PARTITION_NAME_LEN; ++j){
				if(buf[j] == 0xFF){
					break;
				}
				name_in_flash[j] = buf[j];
			}

			name_in_flash[j] = '\0';
			if(strcmp(name,name_in_flash) == 0){
				partition_config[i].phy_flags = short_address(buf, PARTITION_FLAGS_OFFSET);
				if (partition_config[i].phy_flags & 1)
				{
					BK_LOGE(TAG, "en/decrypt mode.\r\n");
					free(buf);
					return true;
				}
			}
		}
		free(buf);
	}
	return bRet;
}
#endif

static int bk_get_partition_info_from_config(const char *part_name, uint32_t *address, uint32_t *size)
{
    if (part_name == NULL || address == NULL || size == NULL) {
        return -1;
    }

    // Use const pointers to the macro strings for parsing (we only read them, not modify)
    const char *size_str = CONFIG_FLASH_PART_SIZE;
    const char *name_str = CONFIG_FLASH_PART_NAME;
    
    const char *size_ptr = size_str;
    const char *name_ptr = name_str;
    
    uint32_t current_address = 0;
    int found = 0;
    
    // Traverse through partitions
    while (*size_ptr != '\0' && *name_ptr != '\0') {
        char size_buf[12] = {0};
        char name_buf[16] = {0};
        
        // Extract size token
        const char *size_comma = strchr(size_ptr, ',');
        if (size_comma != NULL) {
            int size_len = size_comma - size_ptr;
            if (size_len > 0 && size_len < sizeof(size_buf)) {
                memcpy(size_buf, size_ptr, size_len);
                size_buf[size_len] = '\0';
            }
            size_ptr = size_comma + 1;
        } else {
            // Last token (no comma)
            int size_len = strlen(size_ptr);
            if (size_len > 0 && size_len < sizeof(size_buf)) {
                memcpy(size_buf, size_ptr, size_len);
                size_buf[size_len] = '\0';
            }
            size_ptr += size_len; // Move to end
        }
        
        // Extract name token
        const char *name_comma = strchr(name_ptr, ',');
        if (name_comma != NULL) {
            int name_len = name_comma - name_ptr;
            if (name_len > 0 && name_len < sizeof(name_buf)) {
                memcpy(name_buf, name_ptr, name_len);
                name_buf[name_len] = '\0';
            }
            name_ptr = name_comma + 1;
        } else {
            // Last token (no comma)
            int name_len = strlen(name_ptr);
            if (name_len > 0 && name_len < sizeof(name_buf)) {
                memcpy(name_buf, name_ptr, name_len);
                name_buf[name_len] = '\0';
            }
            name_ptr += name_len; // Move to end
        }
        
        // Check if we have valid tokens
        if (strlen(size_buf) == 0 || strlen(name_buf) == 0) {
            break;
        }
        
        // Convert size from KB to bytes
        uint32_t part_size_kb = (uint32_t)atoi(size_buf);
        uint32_t part_size = part_size_kb * 1024;
        
        // Check if this is the partition we're looking for
        if (strcmp(name_buf, part_name) == 0) {
            *address = current_address;
            *size = part_size;
            found = 1;
            break;
        }
        
        // Accumulate address for next partition (in bytes)
        current_address += part_size;
    }
    
    return found ? 0 : -1;
}

void init_bk_flash_partitions(void)
{
	uint32_t address = 0;
	uint32_t size = 0;

	if (bk_get_partition_info_from_config("bl1", &address, &size) == 0) {
		bk_flash_partitions[0].partition_start_addr = address;
		bk_flash_partitions[0].partition_length = size;
	}

	if (bk_get_partition_info_from_config("rf", &address, &size) == 0) {
		bk_flash_partitions[1].partition_start_addr = address;
		bk_flash_partitions[1].partition_length = size;
	}

	if (bk_get_partition_info_from_config("net", &address, &size) == 0) {
		bk_flash_partitions[2].partition_start_addr = address;
		bk_flash_partitions[2].partition_length = size;
	}

	if (bk_get_partition_info_from_config("easyflash", &address, &size) == 0) {
		bk_flash_partitions[3].partition_start_addr = address;
		bk_flash_partitions[3].partition_length = size;
	}
}

bk_logic_partition_t *bk_flash_partition_get_info(bk_partition_t partition)
{
	bk_logic_partition_t *pt = NULL;

	if (bk_flash_partitions[0].partition_start_addr == 0xFFFFFFFF) {
		init_bk_flash_partitions();
	}

	BK_ASSERT(BK_PARTITION_BOOTLOADER < BK_PARTITION_MAX);

	if (flash_partition_is_valid(partition)) {
#if defined(CONFIG_TFM_READ_FLASH_NSC)
		pt = get_partition_info(partition);
#else
		if (partition == BK_PARTITION_BOOTLOADER) {
			pt = (bk_logic_partition_t *)&bk_flash_partitions[0];
		} else if (partition == BK_PARTITION_RF_FIRMWARE) {
			pt = (bk_logic_partition_t *)&bk_flash_partitions[1];
		} else if (partition == BK_PARTITION_NET_PARAM) {
			pt = (bk_logic_partition_t *)&bk_flash_partitions[2];
		} else if (partition == BK_PARTITION_EASYFLASH) {
			pt = (bk_logic_partition_t *)&bk_flash_partitions[3];
		}
#endif
	}
	return pt;
}

bk_err_t bk_flash_partition_erase(bk_partition_t partition, uint32_t offset, uint32_t size)
{
	uint32_t erase_addr = 0;
	uint32_t start_sector, end_sector = 0;
	bk_logic_partition_t *partition_info = NULL;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	start_sector = offset >> FLASH_SECTOR_SIZE_OFFSET; /* offset / FLASH_SECTOR_SIZE */
	end_sector = (offset + size - 1) >> FLASH_SECTOR_SIZE_OFFSET;

	flash_protect_type_t  partition_type = bk_flash_get_protect_type();

	for (uint32_t i = start_sector; i <= end_sector; i ++) {

		erase_addr = partition_info->partition_start_addr + (i << FLASH_SECTOR_SIZE_OFFSET);

		GLOBAL_INT_DISABLE();

			bk_flash_set_protect_type(FLASH_PROTECT_NONE);
		bk_flash_erase_sector(erase_addr);
			bk_flash_set_protect_type(partition_type);

		GLOBAL_INT_RESTORE();
	}

	return BK_OK;
}

int toHex(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	} else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	} else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	} else {
		return 0;
	}
}

uint8_t toHexByte(const char* c) {
	return 16 * toHex(c[0]) + toHex(c[1]);
}

void toHexStream(const char* src, uint8_t* dest, uint32_t* dest_len) {
	uint32_t cnt = 0;
	const char* p = src;
	while (*p != '\0' && *(p + 1) != '\0') {
		dest[cnt++] = toHexByte(p);
		p += 2;
	}
	*dest_len = cnt;
}

bk_err_t bk_flash_partition_write(bk_partition_t partition, const uint8_t *buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(buffer);

	uint32_t start_addr;
	bk_logic_partition_t *partition_info;

	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partition not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}

	start_addr = partition_info->partition_start_addr + offset;
	if((offset + buffer_len) > partition_info->partition_length) {
		FLASH_LOGE("partition overlap. offset(%d),len(%d)\r\n", offset, buffer_len);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	uint8_t *dest_hex = os_malloc(buffer_len);
	if (dest_hex == NULL) {
		FLASH_LOGW("%s malloc failed\r\n", __func__);
		return BK_ERR_NO_MEM;
	}

	os_memcpy(dest_hex, buffer, buffer_len);
#if (defined(CONFIG_TFM_READ_FLASH_NSC))
	if (partitionIsEncrypt(partition))
	{
		uint8_t eky_hex[2 * NVS_KEY_SIZE];
		uint8_t ptxt_hex[32], ctxt_hex[32], TweakValue[16];

		mbedtls_aes_xts_context ectx[1];

		mbedtls_aes_xts_init(ectx);
		
		nvs_sec_cfg_t cfg = {{0x72, 0x36},{0x20, 0x24}};

        #if defined(CONFIG_NVS_ENCRYPTION)
        bk_err_t ret = BK_FAIL;
        
        ret = nvs_flash_read_cfg_by_partId(BK_PARTITION_NVS_KEY, &cfg);
        if (ret == ESP_ERR_NVS_KEYS_NOT_INITIALIZED) {
            BK_LOGI(TAG, "NVS key partition empty, generating keys");
            const bk_logic_partition_t *key_part = bk_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS, NVS_KEY_PARTITION_LABEL);
            if (key_part == NULL) {
                BK_LOGE(TAG, "CONFIG_NVS_ENCRYPTION is enabled, but no partition with subtype nvs_keys found in the partition table.");
                return ret;
            }
            
            ret = nvs_flash_generate_keys(key_part, &cfg);
            if (ret != BK_OK) {
                BK_LOGE(TAG, "Failed to generate keys: [0x%02X]", ret);
                return ret;
            }
        } else if (ret != BK_OK) {
            BK_LOGE(TAG, "Failed to read NVS security cfg: [0x%02X]", ret);
            return ret;
        }
        #endif // CONFIG_NVS_ENCRYPTION
		
		os_memcpy(eky_hex,cfg.eky,NVS_KEY_SIZE);
		os_memcpy(eky_hex + NVS_KEY_SIZE,cfg.tky,NVS_KEY_SIZE);

		mbedtls_aes_xts_setkey_enc(ectx, eky_hex, 2 * NVS_KEY_SIZE * 8);

		uint32_t unit_num = 0;
		for (size_t i = 0; i < buffer_len;  i += DATAUNIT_SIZE)
		{
			os_memset(TweakValue, 0, 16);
			os_memset(ptxt_hex, 0xff, 32);

			for (int k = 0; k < 16 && k < DATAUNIT_SIZE; k++) {
				if (8 * k >= 32)continue;
				TweakValue[15 - k] = (unit_num  >> (8 * k)) & 0xFF;
			}

			const uint8_t* tab_addr = &buffer[i];

			if ((i + DATAUNIT_SIZE) < buffer_len)
				os_memcpy(ptxt_hex, tab_addr, DATAUNIT_SIZE);
			else
			{
				// last cpy
				u32 len = buffer_len - i;
				os_memcpy(ptxt_hex, tab_addr, len);
			}

			ret = mbedtls_aes_crypt_xts(ectx, MBEDTLS_AES_ENCRYPT, 32, TweakValue,ptxt_hex, ctxt_hex);
			mbedtls_aes_xts_free(ectx);
			if (ret != BK_OK)
			{
				BK_LOGE(TAG, "Failed to mbedtls_aes_crypt_xts_encrypt: [0x%02X]", ret);
            	return ret;
			}

			if ((i + DATAUNIT_SIZE) < buffer_len)
				os_memcpy(&dest_hex[i], ctxt_hex, DATAUNIT_SIZE);
			else
			{
				// last cpy
				u32 len = buffer_len - i;
				os_memcpy(&dest_hex[i], ctxt_hex, len);
			}
			unit_num++;
		}
	}
#endif

	GLOBAL_INT_DISABLE();

	flash_protect_type_t  partition_type = bk_flash_get_protect_type();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	if((offset + buffer_len) <= partition_info->partition_length) {
		bk_flash_write_bytes(start_addr, dest_hex, buffer_len);
	}
	bk_flash_set_protect_type(partition_type);

	GLOBAL_INT_RESTORE();

	if (dest_hex) {
		os_free(dest_hex);
		dest_hex = NULL;
	}

	return BK_OK;
}

bk_err_t bk_flash_partition_read(bk_partition_t partition, uint8_t *out_buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(out_buffer);

	uint32_t start_addr;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();
	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partiion not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}
	start_addr = partition_info->partition_start_addr + offset;
	if((offset + buffer_len) > partition_info->partition_length) {
		FLASH_LOGE("partition overlap. offset(%d),len(%d)\r\n", offset, buffer_len);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	GLOBAL_INT_DISABLE();
	bk_flash_read_bytes(start_addr, out_buffer, buffer_len);
	GLOBAL_INT_RESTORE();

#if (defined(CONFIG_TFM_READ_FLASH_NSC))
	if (partitionIsEncrypt(partition))
	{
		uint8_t eky_hex[2 * NVS_KEY_SIZE];
		uint8_t ptxt_hex[32], ctxt_hex[32], TweakValue[16];
		mbedtls_aes_xts_context dctx[1];

		uint8_t* dest_hex = os_malloc(buffer_len);
		if (dest_hex == NULL)
		{
			FLASH_LOGW("%s malloc failed\r\n", __func__);
			return BK_ERR_NO_MEM;
		}

		os_memset(dest_hex,0xff,buffer_len);

		mbedtls_aes_xts_init(dctx);
		// uint32_t byteArrayLen = 0;
        nvs_sec_cfg_t cfg = {{0x72, 0x36},{0x20, 0x24}};

        #if defined(CONFIG_NVS_ENCRYPTION)
        bk_err_t ret = BK_FAIL;
        
        ret = nvs_flash_read_cfg_by_partId(BK_PARTITION_NVS_KEY, &cfg);
        if (ret == ESP_ERR_NVS_KEYS_NOT_INITIALIZED) {
            BK_LOGI(TAG, "NVS key partition empty, generating keys");
            const bk_logic_partition_t *key_part = bk_partition_find_first(
                ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS, NVS_KEY_PARTITION_LABEL);
            if (key_part == NULL) {
                BK_LOGE(TAG, "CONFIG_NVS_ENCRYPTION is enabled, but no partition with subtype nvs_keys found in the partition table.");
                return ret;
            }
            
            ret = nvs_flash_generate_keys(key_part, &cfg);
            if (ret != BK_OK) {
                BK_LOGE(TAG, "Failed to generate keys: [0x%02X]", ret);
                return ret;
            }
        } else if (ret != BK_OK) {
            BK_LOGE(TAG, "Failed to read NVS security cfg: [0x%02X]", ret);
            return ret;
        }
        #endif // CONFIG_NVS_ENCRYPTION

		os_memcpy(eky_hex,cfg.eky,NVS_KEY_SIZE);
		os_memcpy(eky_hex + NVS_KEY_SIZE,cfg.tky,NVS_KEY_SIZE);

		mbedtls_aes_xts_setkey_dec(dctx, eky_hex, 2 * NVS_KEY_SIZE * 8);

		uint32_t unit_num = 0;
		for (size_t i = 0; i < buffer_len;  i += DATAUNIT_SIZE)
		{
			os_memset(TweakValue, 0, 16);

			for (int k = 0; k < 16 && k < DATAUNIT_SIZE; k++) {
				if (8 * k >= 32)continue;
				TweakValue[15 - k] = (unit_num  >> (8 * k)) & 0xFF;
			}

			uint8_t* tab_addr = &out_buffer[i];

			if ((i + DATAUNIT_SIZE) < buffer_len)
				os_memcpy(ctxt_hex, tab_addr, DATAUNIT_SIZE);
			else
			{
				// last cpy
				u32 len = buffer_len - i;
				os_memcpy(ctxt_hex, tab_addr, len);
			}

			mbedtls_aes_crypt_xts(dctx, MBEDTLS_AES_DECRYPT, 32, TweakValue, ctxt_hex,ptxt_hex);
			mbedtls_aes_xts_free(dctx);
			if (ret != BK_OK)
			{
				BK_LOGE(TAG, "Failed to mbedtls_aes_crypt_xts_encrypt: [0x%02X]", ret);
            	return ret;
			}

			if ((i + DATAUNIT_SIZE) < buffer_len)
				os_memcpy(&dest_hex[i], ptxt_hex, DATAUNIT_SIZE);
			else
			{
				// last cpy
				u32 len = buffer_len - i;
				os_memcpy(&dest_hex[i], ptxt_hex, len);
			}
			unit_num++;
		}
		os_memset(out_buffer,0xff,buffer_len);
		os_memcpy(out_buffer,dest_hex,buffer_len);

		if (dest_hex) {
			os_free(dest_hex);
			dest_hex = NULL;
		}
	}
#endif

	return BK_OK;
}

PARTITION_IRAM void *__iram_memcpy(void *d, const void *s, size_t n)
{
	/* attempt word-sized copying only if buffers have identical alignment */

	unsigned char *d_byte = (unsigned char *)d;
	const unsigned char *s_byte = (const unsigned char *)s;
	const uint32_t mask = sizeof(uint32_t) - 1;

	if ((((uint32_t)d ^ (uint32_t)s_byte) & mask) == 0) {
		/* do byte-sized copying until word-aligned or finished */
		while (((uint32_t)d_byte) & mask) {
				if (n == 0) {
						return d;
				}
				*(d_byte++) = *(s_byte++);
				n--;
		};

		/* do word-sized copying as long as possible */
		uint32_t *d_word = (uint32_t *)d_byte;
		const uint32_t *s_word = (const uint32_t *)s_byte;

		while (n >= sizeof(uint32_t)) {
				*(d_word++) = *(s_word++);
				n -= sizeof(uint32_t);
		}

		d_byte = (unsigned char *)d_word;
		s_byte = (unsigned char *)s_word;
	}

	/* do byte-sized copying until finished */
	while (n > 0) {
		*(d_byte++) = *(s_byte++);
		n--;
	}

	return d;
}

#if defined(CONFIG_FLASH_CBUS_RW)
PARTITION_IRAM bk_err_t bk_flash_partition_read_enhanced(bk_partition_t partition, uint8_t *out_buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(out_buffer);

	uint32_t in_ptr;
	uint32_t start_addr, flash_base;
	bk_logic_partition_t *partition_info;
	GLOBAL_INT_DECLARATION();

	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partiion not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}

	flash_base = SOC_FLASH_DATA_BASE;
	start_addr = partition_info->partition_start_addr;
	in_ptr = flash_base + ADDR_PHY_2_VIRT(start_addr) + offset;

	GLOBAL_INT_DISABLE();
	__iram_memcpy(out_buffer, (void *)in_ptr, buffer_len);
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

PARTITION_IRAM bk_err_t bk_flash_partition_write_enhanced(bk_partition_t partition, const uint8_t *buffer, uint32_t offset, uint32_t buffer_len)
{
	BK_RETURN_ON_NULL(buffer);

	uint32_t wr_ptr;
	uint32_t start_addr, flash_base;
	bk_logic_partition_t *partition_info;

	GLOBAL_INT_DECLARATION();
	FLASH_LOGI("bk_flash_partition_write_enhanced:0x%x\r\n", buffer_len);
	partition_info = bk_flash_partition_get_info(partition);
	if (NULL == partition_info) {
		FLASH_LOGW("%s partition not found\r\n", __func__);
		return BK_ERR_FLASH_PARTITION_NOT_FOUND;
	}

	flash_base = SOC_FLASH_DATA_BASE;
	start_addr = partition_info->partition_start_addr;
	wr_ptr = flash_base + ADDR_PHY_2_VIRT(start_addr) + offset;
	if((offset + buffer_len) > partition_info->partition_length) {
		FLASH_LOGE("partition overlap. offset(%d),len(%d)\r\n", offset, buffer_len);
		return BK_ERR_FLASH_ADDR_OUT_OF_RANGE;
	}

	GLOBAL_INT_DISABLE();

	flash_protect_type_t  partition_type = bk_flash_get_protect_type();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);
	if((offset + buffer_len) <= partition_info->partition_length) {
		__iram_memcpy((void *)wr_ptr, buffer, buffer_len);
	}
	bk_flash_set_protect_type(partition_type);

	GLOBAL_INT_RESTORE();

	return BK_OK;
}
#endif

