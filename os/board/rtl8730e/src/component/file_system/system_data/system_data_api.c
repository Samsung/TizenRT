#include <platform_opts.h>
#include <wifi_fast_connect.h>
#include "flash_api.h"
#include "device_lock.h"
#include "system_data_api.h"

#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
#include "snand_api.h"
#include "netif.h"
#include <lwip_netconf.h>
#include "ftl_nand_api.h"
#include "ftl_nor_api.h"
#include "ftl_common_api.h"
#else
#include "kv.h"
#endif

#if !defined(CONFIG_FTL_EN) || (!CONFIG_FTL_EN)
static int sys_write_wlan_data_to_kv(u8 *data, uint32_t len)
{
	// store to NOR flash
	int ret = 0;

	if (!data) {
		ret = -1;
		goto exit;
	}

	ret = rt_kv_set("wlan_data", data, len);
exit:
	return ret;
}

static int sys_write_bt_data_to_kv(u8 *data, uint32_t len)
{
	int ret = 0;

	if (!data) {
		ret = -1;
		goto exit;
	}

	ret = rt_kv_set("bt_data", data, len);

exit:
	return ret;
}

static int sys_read_wlan_data_from_kv(u8 *data, uint32_t len)
{
	int ret = 0;

	ret = rt_kv_get("wlan_data", data, len);

	return ret;
}

static int sys_read_bt_data_from_kv(u8 *data)
{
	int ret = 0;

	ret = rt_kv_get("bt_data", data, SYS_DATA_SGNT_LEN);

	return ret;
}

static int sys_erase_system_data_kv(void)
{
	rt_kv_delete("wlan_data");
	rt_kv_delete("bt_data");
	return 0;
}
#endif

#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
static int sys_write_wlan_data_to_flash_ftl(u8 *data, uint32_t len)
{
	int ret = 0;
	u8	read_signature[4] = {0} ;
	u8	read_bt_para_data[4];
	u8	*write_data = NULL;
	uint32_t write_data_len;
	int offset = 0;
	int address = FAST_RECONNECT_DATA;
	int type = 0;
	int page_size = 0;
	int block_size = 0;
	int block_cnt = 0;

	if (!data) {
		ret = -1;
		goto exit;
	}

	ftl_common_info(&type, &page_size, &block_size, &block_cnt);

	if (type == FTL_NAND_FLASH) {
		write_data_len = 2048;
	} else {
		write_data_len = TOTAL_PARTION_SIZE;
	}

	write_data = (u8 *)malloc(write_data_len);
	if (!write_data) {
		printf("\n\r[%s] malloc write data failed!", __func__);
		ret = -1;
		goto exit;
	}
	memset(write_data, 0xff, write_data_len);
	memset(read_bt_para_data, 0x0, SYS_DATA_SGNT_LEN);

	offset = 0;
	ret = ftl_common_read(address + offset, read_signature, SYS_DATA_SGNT_LEN);
	if (ret < 0) {
		printf("\n\r[%s] ftl read signature failed", __func__);
		goto exit;
	}

	if (memcmp(read_signature, SYS_DATA_SGNT, SYS_DATA_SGNT_LEN) != 0) {
		//printf("\n\r[%s] read_signature not same\n\r",__func__);
	} else {
		//Backup BT paramter
		offset = SYS_DATA_BT_DATA_ADDR;
		ret = ftl_common_read(address + offset, read_bt_para_data, SYS_DATA_SGNT_LEN);
		if (ret < 0) {
			printf("\n\r[%s] ftl read bt para data failed", __func__);
			goto exit;
		}
		memcpy(write_data + SYS_DATA_BT_DATA_ADDR, read_bt_para_data, SYS_DATA_SGNT_LEN);
	}

	//Read flash_data_backup
	if (type == FTL_NOR_FLASH) {
		offset = WIFI_FAST_CON_PARTION_SIZE;
		ret = ftl_common_read(address + offset, (write_data + WIFI_FAST_CON_PARTION_SIZE), (TOTAL_PARTION_SIZE - WIFI_FAST_CON_PARTION_SIZE));
		if (ret < 0) {
			printf("\n\r[%s] ftl read flash data backup failed", __func__);
			goto exit;
		}
	}

	memcpy(write_data, &SYS_DATA_SGNT, SYS_DATA_SGNT_LEN);
	memcpy(write_data + SYS_DATA_VERSION_ADDR, &SYS_DATA_VERSION, SYS_DATA_SGNT_LEN);
	memcpy(write_data + SYS_DATA_WIFI_DATA_ADDR, data, len);

	offset = 0;
	ret = ftl_common_write(address + offset, write_data, write_data_len);
	if (ret < 0) {
		printf("\n\r[%s] ftl write failed", __func__);
		goto exit;
	}

exit:
	if (write_data) {
		free(write_data);
	}

	return ret;
}

static int sys_write_bt_data_to_flash_ftl(u8 *data, uint32_t len)
{
	int ret = 0;
	u8	read_signature[4] = {0};
	u8	*write_data = NULL;
	uint32_t write_data_len;
	int offset = 0;
	int address = FAST_RECONNECT_DATA;
	int type = 0;
	int page_size = 0;
	int block_size = 0;
	int block_cnt = 0;

	if (!data) {
		ret = -1;
		goto exit;
	}

	ftl_common_info(&type, &page_size, &block_size, &block_cnt);

	if (type == FTL_NAND_FLASH) {
		write_data_len = 2048;
	} else {
		write_data_len = TOTAL_PARTION_SIZE;
	}

	write_data = (u8 *)malloc(write_data_len);
	if (!write_data) {
		printf("\n\r[%s] malloc write data failed!", __func__);
		ret = -1;
		goto exit;
	}
	memset(write_data, 0xff, write_data_len);

	offset = 0;
	ret = ftl_common_read(address + offset, read_signature, SYS_DATA_SGNT_LEN);
	if (ret < 0) {
		printf("\n\r[%s] ftl read signature failed", __func__);
		goto exit;
	}

	if (memcmp(read_signature, SYS_DATA_SGNT, SYS_DATA_SGNT_LEN) != 0) {
		//printf("\n\r__sys_write_bt_data_to_flash__ read_signature not same\n\r");
	} else {
		//Backup wlan reconnect data and 3k flash_data_backup
		offset = SYS_DATA_WIFI_DATA_ADDR;
		ret = ftl_common_read(address + offset, (write_data + SYS_DATA_WIFI_DATA_ADDR), (write_data_len - SYS_DATA_WIFI_DATA_ADDR));
		if (ret < 0) {
			printf("\n\r[%s] ftl read wifi data failed", __func__);
			goto exit;
		}
	}

	memcpy(write_data, &SYS_DATA_SGNT, SYS_DATA_SGNT_LEN);
	memcpy(write_data + SYS_DATA_VERSION_ADDR, &SYS_DATA_VERSION, SYS_DATA_SGNT_LEN);
	memcpy(write_data + SYS_DATA_BT_DATA_ADDR, data, len);

	offset = 0;
	ret = ftl_common_write(address + offset, write_data, write_data_len);
	if (ret < 0) {
		printf("\n\r[%s] ftl write failed", __func__);
		goto exit;
	}

exit:
	if (write_data) {
		free(write_data);
	}

	return ret;
}

static int sys_read_wlan_data_from_flash_ftl(u8 *data, uint32_t len)
{
	u8	read_signature[4] = {0};
	int offset;
	int address = FAST_RECONNECT_DATA;
	int ret = 0;

	offset = 0;
	ret = ftl_common_read(address + offset, read_signature, SYS_DATA_SGNT_LEN);
	if (ret < 0) {
		printf("\n\r[%s] ftl read signature failed", __func__);
		goto exit;
	}

	if (memcmp(read_signature, SYS_DATA_SGNT, SYS_DATA_SGNT_LEN) != 0) {
		//printf("\n\r[%s] read_signature not same\n\r",__func__);
		return -1;
	}

	offset = SYS_DATA_WIFI_DATA_ADDR;
	ret = ftl_common_read(address + offset, data, len);
	if (ret < 0) {
		printf("\n\r[%s] ftl read wlan data failed", __func__);
		goto exit;
	}

exit:
	return ret;
}

static int sys_read_bt_data_from_flash_ftl(u8 *data)
{
	int ret = 0;
	u8	read_signature[4] = {0} ;
	int offset;
	int address = FAST_RECONNECT_DATA;

	offset = 0;
	ret = ftl_common_read(address + offset, read_signature, SYS_DATA_SGNT_LEN);
	if (ret < 0) {
		printf("\n\r[%s] ftl read signature failed", __func__);
		goto exit;
	}

	if (memcmp(read_signature, SYS_DATA_SGNT, SYS_DATA_SGNT_LEN) != 0) {
		//printf("\n\r[%s] read_signature not same\n\r",__func__);
		return -1;
	}

	offset = SYS_DATA_BT_DATA_ADDR;
	ret = ftl_common_read(address + offset, data, SYS_DATA_SGNT_LEN);
	if (ret < 0) {
		printf("\n\r[%s] ftl read bt para data failed", __func__);
		goto exit;
	}

exit:
	return ret;
}

static int sys_erase_system_data_flash_ftl(void)
{
	int address = FAST_RECONNECT_DATA;
	int ret = 0;
	ret = ftl_erase_sector(address);

	return ret;
}
#endif

int sys_write_wlan_data_to_flash(u8 *data, uint32_t len)
{
#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
	int ret = 0;

	ret = sys_write_wlan_data_to_flash_ftl(data, len);

	return ret;
#else
	int ret = 0;

	ret = sys_write_wlan_data_to_kv(data, len);

	return ret;
#endif
}


int sys_write_bt_data_to_flash(u8 *data, uint32_t len)
{
#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
	int ret = 0;

	ret = sys_write_bt_data_to_flash_ftl(data, len);

	return ret;
#else
	int ret = 0;

	ret = sys_write_bt_data_to_kv(data, len);

	return ret;
#endif
}

int sys_read_wlan_data_from_flash(u8 *data, uint32_t len)
{
#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
	int ret = 0;

	ret = sys_read_wlan_data_from_flash_ftl(data, len);

	return ret;
#else
	int ret = 0;

	ret = sys_read_wlan_data_from_kv(data, len);

	return ret;
#endif
}

int sys_read_bt_data_from_flash(u8 *data)
{
#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
	int ret = 0;

	ret = sys_read_bt_data_from_flash_ftl(data);

	return ret;
#else
	int ret = 0;

	ret = sys_read_bt_data_from_kv(data);

	return ret;
#endif
}

int sys_erase_system_data(void)
{
#if defined(CONFIG_FTL_EN) && CONFIG_FTL_EN
	int ret = 0;

	ret = sys_erase_system_data_flash_ftl();

	return ret;
#else
	int ret = 0;

	ret = sys_erase_system_data_kv();

	return ret;
#endif
}

#if 0
//For debug example
#if defined(ENABLE_FAST_CONNECT_NAND) && ENABLE_FAST_CONNECT_NAND
int sys_read_system_data_test(void)
{
	uint8_t boot_sel = -1;

	boot_sel = hal_sys_get_boot_select();
	if (0 == boot_sel) {
		flash_t flash;
		u8	*read_data;
		uint32_t read_data_len = TOTAL_PARTION_SIZE;

		device_mutex_lock(RT_DEV_LOCK_FLASH);

		flash_stream_read(&flash, FAST_RECONNECT_DATA, sizeof(struct wlan_fast_reconnect), read_data);

		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		int j = 0;
		for (int i = 0; i < read_data_len ; i++) {
			j = j + 1;
			if (i < (sizeof(struct wlan_fast_reconnect) + SYS_DATA_WIFI_DATA_ADDR)) {
				printf("%c,", read_data[i]);
			} else {
				//printf("%x,",read_data[i]);
			}
			if (i < (sizeof(struct wlan_fast_reconnect) + SYS_DATA_WIFI_DATA_ADDR)) {
				if (j == 16) {
					printf("\n\r");
					j = 0;
				}
			}
		}
	} else if (1 == boot_sel) {
		snand_t flash;
		snand_init(&flash);
		uint8_t data[2112] __attribute__((aligned(32)));

		device_mutex_lock(RT_DEV_LOCK_FLASH);
		snand_page_read(&flash, 12 * 64, 2048 + 32, &data[0]);	// 1
		device_mutex_unlock(RT_DEV_LOCK_FLASH);

		int j = 0;
		printf("\n\r__Read_system_data_test__ data[2048]: %x\n\r", data[2048]);
		if (data[2048] == 0xff) {
			for (int i = 0; i < 2048 ; i++) {
				j = j + 1;
				if (i < (sizeof(struct wlan_fast_reconnect) + SYS_DATA_WIFI_DATA_ADDR)) {
					printf("%c,", data[i]);
				} else {
					printf("%x,", data[i]);
				}
				if (j == 16) {
					printf("\n\r");
					j = 0;
				}
			}
		}
	}
	return 0;
}
#endif
#endif