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

#include <common/bk_include.h>
#include <components/log.h>
#include "bk_drv_model.h"
#include "bk_uart.h"
#include "bk_wifi_private.h"
#include "bk_sys_ctrl.h"
#include <driver/efuse.h>
#include <driver/otp.h>
#include <driver/otp_types.h>
#include <os/mem.h>
#include "bk_phy.h"
#include <components/system.h>
#include <components/log.h>
#include "bk_wifi_types.h"
#include "bk_wifi.h"
#if (defined(CONFIG_RANDOM_MAC_ADDR))
#include <driver/trng.h>
#endif
#include "driver/flash_partition.h"
#if (defined(CONFIG_RF_FIRMWARE_DYNAMIC_PARTITION))
#include "driver/flash.h"
#include "driver/flash_types.h"
#endif

#define TAG "mac"


#define BASE_MAC_LEN  (6)
#define DEFAULT_MAC_ADDR "\xC8\x47\x8C\x00\x00\x18"
static uint8_t s_base_mac[] = DEFAULT_MAC_ADDR;
static bool s_mac_inited = false;

#if (defined(CONFIG_BASE_MAC_FROM_OTP2))
/**
 * to find an empty mac addr item
 */
int get_otp2_mac_addr_item(UINT32 *item)
{
    UINT8 i = OTP_MAC_ADDRESS1;
    int ret = BK_FAIL;
    uint8_t mac_r[] = DEFAULT_MAC_ADDR;

    for(i = OTP_MAC_ADDRESS1; i <= OTP_MAC_ADDRESS4; i++)
    {
        *item = i;
        ret = bk_otp_ahb_read(i,(UINT8 *)&mac_r,BASE_MAC_LEN);
        for(UINT8 j = 0; j < BASE_MAC_LEN; j++)
        {
            if(mac_r[j] == 0x00)
            {
               ret += 1;
            }
        }
        if(ret == BASE_MAC_LEN)
        {
            return BK_OK;
        }
        else if(i == OTP_MAC_ADDRESS4 && ret != BASE_MAC_LEN)
        {
            return BK_FAIL;
        }
    }
    return ret;

}

static int write_base_mac_to_otp2(const uint8_t *mac)
{
#if defined(CONFIG_OTP)
    int ret = BK_FAIL;
    uint8_t buf[6];
    UINT32 item = OTP_MAC_ADDRESS1;

    ret = get_otp2_mac_addr_item(&item);
    if((item > OTP_MAC_ADDRESS1 && item <= OTP_MAC_ADDRESS4) && ret == BK_OK)
    {
        item -= 1;
    }

    ret = bk_otp_ahb_read(item, buf, BASE_MAC_LEN);

    if(((buf[0] != mac[0]) || (buf[1] != mac[1]) || (buf[2] != mac[2]) || (buf[3] != mac[3])
        || (buf[4] != mac[4]) || (buf[5] != mac[5])) && ret == BK_OK)
    {
        ret = get_otp2_mac_addr_item(&item);
        if(ret == BK_OK)
        {
           ret = bk_otp_ahb_update(item, (uint8_t *)mac, BASE_MAC_LEN);
           if(ret == BK_OK)
               return BK_OK;
           else
               return BK_FAIL;
        }
        else
            return BK_FAIL;
    }
    else
        return BK_OK;
#endif
}

static int read_base_mac_to_otp2(uint8_t *mac)
{
#if defined(CONFIG_OTP)
    int ret = BK_FAIL;
    uint8_t buf[6];
    UINT32 item = OTP_MAC_ADDRESS1;

    ret = get_otp2_mac_addr_item(&item);
    if((item > OTP_MAC_ADDRESS1 && item <= OTP_MAC_ADDRESS4) && ret == BK_OK)
    {
        item -= 1;
    }

    ret = bk_otp_ahb_read(item, buf, BASE_MAC_LEN);
    if ((ret == BK_OK) && ((buf[0] != 0) || (buf[1] != 0) || (buf[2] != 0) || (buf[3] != 0) || (buf[4] != 0) || (buf[5] != 0)))
    {
        os_memcpy(mac, buf, sizeof(buf));
        return ret;
    }
    else if(ret == BK_OK)
    {
        ret = bk_otp_apb_read(OTP_MAC_ADDRESS, buf, BASE_MAC_LEN);
        if ((ret == BK_OK) && ((buf[0] != 0) || (buf[1] != 0) || (buf[2] != 0) || (buf[3] != 0) || (buf[4] != 0) || (buf[5] != 0)))
        {
            os_memcpy(mac, buf, sizeof(buf));
            return ret;
        }
        else
            return BK_FAIL;
    }
    else
        return BK_FAIL;
#endif
//TODO: enable build CONFIG_OTP failed,temp block
    // if (manual_cal_get_macaddr_from_flash((uint8_t *)mac))
    //     return BK_OK;
    // else
    //     return BK_FAIL;
	return BK_OK;
}
#endif

#if (defined(CONFIG_RANDOM_MAC_ADDR))
static void random_mac_address(u8 *mac)
{
	int i = 0;

	mac[3] = bk_rand() & 0xff;
	mac[4] = bk_rand() & 0xff;
	mac[5] = bk_rand() & 0xff;

	os_printf("mac:");
	for (i = 0; i < 6; i++)
	    os_printf("%02X ", mac[i]);
	os_printf("\n");
}

static int bk_check_mac_address(u8 *mac)
{
	if (mac[0] != 0xc8 || mac[1] != 0x47 || mac[2] != 0x8c) {
		BK_LOGE(TAG, "not a beken mac addr\n");
		return 1;
	} else
		return 0;
}
#endif

#if (defined(CONFIG_NEW_MAC_POLICY))
/*
1. BASE MAC stored at flash partison: BK_PARTITION_NET_PARAM as WIFI_MAC_ITEM (0x3ff000: first 6 bytes)
2. BASE MAC maybe missed while board power down at the time point:
	erase flash partition between BK_PARTITION_NET_PARAM update other netinfo process
3. Use BK_PARTITION_RF_FIRMWARE last free 512 bytes to backup the BASE MAC
4. Every backup record of BASE MAC is 10 bytes, just write, no erase. MAX backup 51 times
*/

#define BASE_MAC_MAGIC_TAG          (0x4d41)
#define BASE_MAC_RECORD_COUNT_MAX   (51)
#define BASE_MAC_RECORD_SIZE        (512)
#define BASE_MAC_RECORD_OFFSET      (4096 - BASE_MAC_RECORD_SIZE)
#define BASE_MAC_RECORD_HDR_LEN     (3)
#define BASE_MAC_RECORD_OPT_READ    (0)
#define BASE_MAC_RECORD_OPT_WRITE   (1)

typedef struct base_mac_record {
	// magic tag of record: 0x4d41
	uint16_t magic_tag;
	// crc of mac[0~5]
	uint8_t  crc8_data;
	// crc of magic tag and crc8_data
	uint8_t  crc8_hdr;
	// base mac data
	uint8_t  mac[BASE_MAC_LEN];
} base_mac_record_t;

/* CRC-8: the poly is 0x31 (x^8 + x^5 + x^4 + 1) */
static uint8_t utils_crc8(uint8_t *buf, uint16_t length)
{
    uint8_t crc = 0x00;
    uint8_t i;

    while (length--) {
        crc ^= *buf++;
        for (i = 8; i > 0; i--) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

static int is_free_mac_record(base_mac_record_t *mac_rec_p) {
	uint16_t *p = (uint16_t *)mac_rec_p;
	uint16_t *end = p + sizeof(base_mac_record_t)/sizeof(uint16_t);
	while (p < end) {
		if(*p++ != 0xffff) {
			return 0;
		}
	}
	return 1;
}

static int get_first_free_mac_record(base_mac_record_t *mac_rec_p, int count) {
	int left = 0, right = count - 1;

	while (left < right) {
		if(is_free_mac_record(&mac_rec_p[left])) {
			return left;
		}
		left++;
	}

	return -1;
}


static int is_valid_mac_record(base_mac_record_t *mac_rec_p) {
	if (BASE_MAC_MAGIC_TAG == mac_rec_p->magic_tag) {
		uint8_t crc_hdr = utils_crc8((uint8_t *)mac_rec_p, BASE_MAC_RECORD_HDR_LEN);
		if(crc_hdr == mac_rec_p->crc8_hdr) {
			uint8_t crc_data = utils_crc8(mac_rec_p->mac, BASE_MAC_LEN);
			if(crc_data == mac_rec_p->crc8_data) {
				return 1;
			}
		}
	}
	return 0;
}

static int get_latest_valid_mac_record(base_mac_record_t *mac_rec_p, int count, int free_index) {
	int valid_index = 0;

	if (free_index == 0) {
		return -1;
	}

	if(0 < free_index && free_index < BASE_MAC_RECORD_COUNT_MAX) {
		valid_index = free_index - 1;
	} else {
		valid_index = BASE_MAC_RECORD_COUNT_MAX - 1;
	}

	while (valid_index >= 0) {
		if(is_valid_mac_record(&mac_rec_p[valid_index])) {
			// BK_LOGI(TAG, "get_latest_valid_mac_record: free index(%d), valid index(%d).\n",
			// 	free_index, valid_index);
			return valid_index;
		}
		valid_index--;
	}

	return -1;
}


static void build_base_mac_record(base_mac_record_t *new_mac_p, uint8_t *mac) {
	new_mac_p->magic_tag = BASE_MAC_MAGIC_TAG;
	os_memcpy(new_mac_p->mac, mac, BASE_MAC_LEN);
	new_mac_p->crc8_data = utils_crc8(new_mac_p->mac, BASE_MAC_LEN);
	new_mac_p->crc8_hdr = utils_crc8((uint8_t *)new_mac_p, BASE_MAC_RECORD_HDR_LEN);
}

/*
sync base mac
mode: 0 read from rf flash, 1 backup to rf flash
*/
static int sync_base_mac_record(uint8_t *mac, int mode)
{
	int ret = BK_FAIL;
	int valid_index = 0;
	int free_index = 0;
	const int record_len = sizeof(base_mac_record_t);

	// internal call, need not valid parameter
	// if(NULL == mac) {
	// 	BK_LOGE(TAG, "get_base_mac_record, para NULL.\n");
	// 	return BK_FAIL;
	// }

	if (BASE_MAC_RECORD_OPT_WRITE == mode) {
		BK_LOGI(TAG, "sync_base_mac_record, mac:"BK_MAC_FORMAT"\n", BK_MAC_STR(mac));
		BK_LOGI(TAG, "sync_base_mac_record, mode: %d. \n", mode);
	}

	base_mac_record_t *mac_rec_p = os_zalloc(BASE_MAC_RECORD_SIZE);

	if (NULL == mac_rec_p) {
		BK_LOGE(TAG, "get_base_mac_record, memory oom.\n");
		return BK_FAIL;
	}

	do {
#if (defined(CONFIG_RF_FIRMWARE_DYNAMIC_PARTITION))
		uint32_t mac_addr = bk_flash_get_capacity_bytes() - FLASH_RF_FIRMWARE_OFFSET + BASE_MAC_RECORD_OFFSET;
		ret = bk_flash_read_bytes(mac_addr, (uint8_t *)mac_rec_p, BASE_MAC_RECORD_SIZE);
#else
		ret = bk_flash_partition_read(BK_PARTITION_RF_FIRMWARE,
					(uint8_t *)mac_rec_p,
					BASE_MAC_RECORD_OFFSET,
					BASE_MAC_RECORD_SIZE);
#endif
		if(BK_OK != ret) {
			BK_LOGE(TAG, "get_base_mac_record, read flash failed.\n");
			break;
		}

		free_index = get_first_free_mac_record(mac_rec_p, BASE_MAC_RECORD_COUNT_MAX);
		if(free_index < 0 || free_index >= BASE_MAC_RECORD_COUNT_MAX) {
			BK_LOGE(TAG, "sync_base_mac_record, record is full, free space not found.\n");
			if (BASE_MAC_RECORD_OPT_READ != mode) {
				ret = BK_FAIL;
				break;
			}
		} else if(free_index > 1) {
			BK_LOGW(TAG, "sync_base_mac_record, saved records is more than 1, free index(%d).\n", free_index);
		}

		if (BASE_MAC_RECORD_OPT_READ == mode) {
			valid_index = get_latest_valid_mac_record(mac_rec_p, BASE_MAC_RECORD_COUNT_MAX, free_index);
			if(valid_index < 0 || valid_index >= BASE_MAC_RECORD_COUNT_MAX) {
				BK_LOGE(TAG, "sync_base_mac_record, valid record not found.\n");
				ret = BK_FAIL;
				break;
			}
			os_memcpy(mac, mac_rec_p[valid_index].mac, BASE_MAC_LEN);
		} else if (BASE_MAC_RECORD_OPT_WRITE == mode) {
			build_base_mac_record(&mac_rec_p[free_index], mac);
			bk_flash_partition_write(BK_PARTITION_RF_FIRMWARE,
					(uint8_t *)&mac_rec_p[free_index],
					BASE_MAC_RECORD_OFFSET + record_len*free_index,
					record_len);
		}

		ret = BK_OK;

	} while (0);

	if (NULL != mac_rec_p) {
		os_free(mac_rec_p);
		mac_rec_p = NULL;
	}

	return ret;
}

static int is_valid_mac_addr(uint8_t *mac) {
	if(mac[0] == 0xff && mac[1] == 0xff && mac[2] == 0xff
	&& mac[3] == 0xff && mac[4] == 0xff && mac[5] == 0xff) {
		return 0;
	} else if(mac[0] == 0x0 && mac[1] == 0x0 && mac[2] == 0x0
	       && mac[3] == 0x0 && mac[4] == 0x0 && mac[5] == 0x0) {
		return 0;
	}

	return 1;
}

static int sync_mac_record(void) {
	int ret = BK_OK;
	uint8_t base_mac[BASE_MAC_LEN] = {0};

	BK_LOGI(TAG, "sync_mac_record, base mac:"BK_MAC_FORMAT"\n", BK_MAC_STR(s_base_mac));

	ret = sync_base_mac_record(base_mac, BASE_MAC_RECORD_OPT_READ);

	if (BK_OK == ret) {
		BK_LOGI(TAG, "sync_mac_record, saved base mac:"BK_MAC_FORMAT"\n", BK_MAC_STR(base_mac));
	}

	if (!is_valid_mac_addr(s_base_mac)) {
		if(BK_OK == ret) {
			// Use saved latest valid mac in last 512 bytes of RF partition
			// Sync the s_base_mac and mac in netinfo partition
			os_memcpy(s_base_mac, base_mac, BASE_MAC_LEN);
			save_net_info(WIFI_MAC_ITEM, s_base_mac, NULL, NULL);
		} else {
			// There isn't valid mac info in Both netinfo partition and RF partition
			BK_LOGE(TAG, "sync_mac_record, NO valid mac be founded.\n");
			return BK_FAIL;
		}
	} else {
		if(BK_OK == ret) {
			if(0 != os_memcmp(s_base_mac, base_mac, BASE_MAC_LEN)) {
				sync_base_mac_record(s_base_mac, BASE_MAC_RECORD_OPT_WRITE);
			}
		} else {
			sync_base_mac_record(s_base_mac, BASE_MAC_RECORD_OPT_WRITE);
		}
	}

	return BK_OK;
}
#endif //#if (CONFIG_NEW_MAC_POLICY)


static int mac_init(void)
{
    int ret = BK_FAIL;

#if (defined(CONFIG_NEW_MAC_POLICY))
        ret = get_net_info(WIFI_MAC_ITEM, s_base_mac, NULL, NULL);
        //ret = sync_mac_record();
#elif (defined(CONFIG_BASE_MAC_FROM_OTP2))
        ret = read_base_mac_to_otp2(s_base_mac);
#endif

#if (defined(CONFIG_RANDOM_MAC_ADDR))
    if ((BK_OK != ret) || BK_IS_GROUP_MAC(s_base_mac)
#if defined(CONFIG_BK_MAC_ADDR_CHECK)
        || bk_check_mac_address(s_base_mac)
#endif
    ) {
        os_memcpy(s_base_mac, DEFAULT_MAC_ADDR, BK_MAC_ADDR_LEN);
        random_mac_address(s_base_mac);
#if (defined(CONFIG_NEW_MAC_POLICY))
        ret = save_net_info(WIFI_MAC_ITEM, s_base_mac, NULL, NULL);
        //ret = sync_mac_record();
#endif
        BK_LOGI(TAG, "use random mac "BK_MAC_FORMAT" as base mac\n", BK_MAC_STR(s_base_mac));
    }
#else //#if (CONFIG_RANDOM_MAC_ADDR)
    if (BK_OK != ret)
    {
        os_memcpy(s_base_mac, DEFAULT_MAC_ADDR, BK_MAC_ADDR_LEN);
        if (BK_IS_GROUP_MAC(s_base_mac))
        {
            BK_LOGE(TAG, "base mac is group mac"BK_MAC_FORMAT"\n", BK_MAC_STR(s_base_mac));
            return BK_ERR_GROUP_MAC;
        }
        else
        {
            BK_LOGI(TAG, "use default mac "BK_MAC_FORMAT" as base mac\n", BK_MAC_STR(s_base_mac));
        }
    }
    else
    {
        BK_LOGI(TAG, "base mac "BK_MAC_FORMAT"\n", BK_MAC_STR(s_base_mac));
    }
#endif //#if (CONFIG_RANDOM_MAC_ADDR)

    return BK_OK;
}


bk_err_t bk_get_mac(uint8_t *mac, mac_type_t type)
{
#if !defined(CONFIG_CUS_MAC_MASK)
	uint8_t mac_mask = (0xff & (2/*NX_VIRT_DEV_MAX*/ - 1));
	uint8_t mac_low;
#endif

	if (s_mac_inited == false) {
		mac_init();
		s_mac_inited = true;
	}

	switch (type) {
	case MAC_TYPE_BASE:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		break;

	case MAC_TYPE_AP:
#if defined(CONFIG_CUS_MAC_MASK)
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		for (int i = 0; i < BK_MAC_ADDR_LEN; i++) {
			mac[BK_MAC_ADDR_LEN-i-1] ^= (CONFIG_CUS_MAC_MASK >> (i*8)) & 0xff;
		}
#else
		mac_mask = (0xff & (2/*NX_VIRT_DEV_MAX*/ - 1));

		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		mac_low = mac[5];

		// if  NX_VIRT_DEV_MAX == 4.
		// if support AP+STA, mac addr should be equal with each other in byte0-4 & byte5[7:2],
		// byte5[1:0] can be different
		// ie: mac[5]= 0xf7,  so mac[5] can be 0xf4, f5, f6. here wre chose 0xf4
		mac[5] &= ~mac_mask;
		mac_low = ((mac_low & mac_mask) ^ mac_mask);
		mac[5] |= mac_low;
#endif
		break;

	case MAC_TYPE_STA:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		break;

	case MAC_TYPE_BLUETOOTH:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		mac[5] += 1; // add 1, diff from wifi's mac
		break;

#ifdef CONFIG_ETH
	case MAC_TYPE_ETH:
		os_memcpy(mac, s_base_mac, BK_MAC_ADDR_LEN);
		mac[5] += 2 /* NX_VIRT_DEV_MAX */  + 1 /* BLUETOOTH */;
		break;
#endif

	default:
		return BK_ERR_INVALID_MAC_TYPE;
	}

	return BK_OK;
}

bk_err_t bk_set_base_mac(const uint8_t *mac)
{
	int ret = BK_FAIL;

	if (!mac)
		return BK_ERR_NULL_PARAM;

	if (BK_IS_GROUP_MAC(mac)) {
		BK_LOGE(TAG, "set failed, cann't be a bc/mc address\r\n");
		return BK_ERR_GROUP_MAC;
	}

	os_memcpy(s_base_mac, mac, BK_MAC_ADDR_LEN);

#if (defined(CONFIG_NEW_MAC_POLICY))
	ret = save_net_info(WIFI_MAC_ITEM, s_base_mac, NULL, NULL);
	//ret = sync_mac_record();
#elif (defined(CONFIG_BASE_MAC_FROM_OTP2))
	ret = write_base_mac_to_otp2(s_base_mac);
#endif

	if (ret != BK_OK)
		BK_LOGE(TAG, "failed to write base mac, ret(%x)\n", ret);

	return ret;
}

