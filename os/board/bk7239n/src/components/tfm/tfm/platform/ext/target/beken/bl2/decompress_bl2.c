#include <inttypes.h>
#include <driver/flash.h>
#include "partitions_gen.h"
#include <stdlib.h>
#include <string.h>

#define TAG "decompress"
#define BL2_DECOMPRESS_LOGD BK_LOGD
#define BL2_DECOMPRESS_LOGI BK_LOGI
#define BL2_DECOMPRESS_LOGW BK_LOGW
#define BL2_DECOMPRESS_LOGE BK_LOGE
#define BL2_DECOMPRESS_DEBUG 0

#define TOVIRTURE(addr) ((addr)%34+(addr)/34*32)
#define TOPHY(addr) ((addr)%32+(addr)/32*34)
#define ALIGN_34(addr) (((addr) + 34 - 1) / 34 * 34)
#define ALIGN_4096(addr) (((addr) + 4096 - 1) / 4096 * 4096)
#define COMPRESS_BLOCK_SIZE (32*1024)

extern uint8_t *decompress_in_memory();
extern uint32_t get_flash_map_offset(uint32_t index);
extern uint32_t get_flash_map_size(uint32_t index);

int
boot_copy_region(struct boot_loader_state *state,
				 const struct flash_area *fap_src,
				 const struct flash_area *fap_dst,
				 uint32_t off_src, uint32_t off_dst, uint32_t sz)
{
	uint32_t bytes_copied = 0;
	uint32_t primary_all_vir_size = get_flash_map_size(0);
	uint32_t primary_all_phy_offset = get_flash_map_offset(0);

	uint32_t block_num = (primary_all_vir_size) >> 15;
	uint16_t block_list[block_num + 2];
	uint8_t* buf = (uint8_t*)malloc(COMPRESS_BLOCK_SIZE);
	uint8_t* decode_buf;

	if(buf == NULL){
		BL2_DECOMPRESS_LOGE(TAG, "memory malloc fail\r\n");
		return -1;
	}
	flash_protect_type_t protect = bk_flash_get_protect_type();
	bk_flash_set_protect_type(FLASH_PROTECT_NONE);

	bytes_copied = BL2_HEADER_SIZE;
	flash_area_read_dbus(fap_src, off_src + bytes_copied, block_list, 2 * (block_num + 2));

	bytes_copied += 2 * (block_num + 2);
	uint32_t erase_size = ALIGN_4096(TOPHY(primary_all_vir_size));
	flash_area_erase_fast(primary_all_phy_offset,erase_size);

	int rate_process = block_num / 5;
	uint32_t vir_primary_all_start_address = TOVIRTURE(ALIGN_34(CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET));

	uint16_t i = 0;
	for(;i < block_num; i++){
		memset(buf,0,sizeof(buf));
		flash_area_read(fap_src, off_src + bytes_copied, buf, block_list[i]);

		decode_buf = decompress_in_memory(buf,COMPRESS_BLOCK_SIZE,0);
		bk_flash_write_cbus(vir_primary_all_start_address+COMPRESS_BLOCK_SIZE*i,(decode_buf),COMPRESS_BLOCK_SIZE);

		free(decode_buf);

		bytes_copied += block_list[i];
		if((i + 1) % rate_process == 0){
			BL2_DECOMPRESS_LOGI(TAG, "OTA %d%%\r\n",(i / rate_process + 1) * 20);
		}

	}

	uint16_t last_block_before_size = block_list[i+1];
	uint16_t last_block_after_size  = block_list[i];
	memset(buf,0,sizeof(buf));
	flash_area_read(fap_src, off_src + bytes_copied, buf, last_block_after_size);

	decode_buf = decompress_in_memory(buf,last_block_before_size,0);

	uint16_t write_size = (last_block_before_size + 31) / 32;

	bk_flash_write_cbus(vir_primary_all_start_address+COMPRESS_BLOCK_SIZE*i,(decode_buf),write_size);

	free(decode_buf);
	decode_buf = NULL;
	free(buf);
	buf = NULL;

	uint32_t ota_phy_size = get_flash_map_size(1);
	uint32_t ota_phy_offset = get_flash_map_offset(1);
	flash_area_erase_fast(ota_phy_offset,ota_phy_size);

	bk_flash_set_protect_type(protect);

	return 0;
}
