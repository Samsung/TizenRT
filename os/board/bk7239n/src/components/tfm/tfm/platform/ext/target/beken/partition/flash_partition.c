#include "flash_map/flash_map.h"
#include "target.h"
#include "Driver_Flash.h"
#include "flash_partition.h"
#include <components/log.h>
#include <ctype.h>
#define TAG "partition"
#if CONFIG_BL2_UPDATE_WITH_PC
#include "partitions_gen.h"
#endif
#include "string.h"

typedef struct {
	uint32_t phy_offset;
	uint32_t phy_size;
    uint32_t phy_flags;
} partition_config_t;
static partition_config_t s_partition_config[PARTITION_CNT] = {0};

const char *s_partition_name[PARTITION_CNT] = {
	"primary_all",
	"secondary_all",
	"ota",
	"partition",
	"primary_tfm_s",
	"primary_tfm_ns",
	"primary_cpu0_app",
	"sys_otp_nv",
	"sys_ps",
	"sys_its",
	"primary_manifest",
	"secondary_manifest",
	"bl2",
	"bl2_B",
	"partition_B"
};

static int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

uint32_t piece_address(uint8_t *array,uint32_t index)
{
    return ((uint32_t)(array[index]) << 24 | (uint32_t)(array[index+1])  << 16 | (uint32_t)(array[index+2])  << 8 | (uint32_t)((array[index+3])));
}

uint16_t short_address(uint8_t *array,uint16_t index)
{
    return ((uint16_t)(array[index]) << 8 | (uint16_t)(array[index+1]));
}

uint32_t partition_get_phy_offset(uint32_t id)
{
	if (id >= PARTITION_CNT) {
		return 0;
	}
	return s_partition_config[id].phy_offset;
}

uint32_t partition_get_phy_size(uint32_t id)
{
	if (id >= PARTITION_CNT) {
		return 0;
	}
	return s_partition_config[id].phy_size;
}
#if CONFIG_BL2_UPDATE_WITH_PC
static uint32_t s_partition_valid = 0;
int partition_check_partition_address_valid(partition_id_e partition_id)
{
	//partition_id_e partition_id = PARTITION_PRIMARY_ALL;
	switch(partition_id)
	{
		case PARTITION_PRIMARY_ALL:
			if((s_partition_config[partition_id].phy_offset != CONFIG_PRIMARY_ALL_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_PRIMARY_ALL_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_SECONDARY_ALL:
			if((s_partition_config[partition_id].phy_offset != CONFIG_SECONDARY_ALL_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_SECONDARY_ALL_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_PS:
			break;
		case PARTITION_ITS:
			break;
		case PARTITION_PRIMARY_MANIFEST:
			if((s_partition_config[partition_id].phy_offset != CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_SECONDARY_MANIFEST:
			if((s_partition_config[partition_id].phy_offset != CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_PRIMARY_BL2:
			if((s_partition_config[partition_id].phy_offset != CONFIG_BL2_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_BL2_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_SECONDARY_BL2:
			if((s_partition_config[partition_id].phy_offset != CONFIG_BL2_B_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_BL2_B_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_PRIMARY_PARTITION:
			if((s_partition_config[partition_id].phy_offset != CONFIG_PARTITION_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_PARTITION_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_SECONDARY_PARTITION:
			if((s_partition_config[partition_id].phy_offset != CONFIG_PARTITION_B_PHY_PARTITION_OFFSET)
				&&(s_partition_config[partition_id].phy_size != CONFIG_PARTITION_B_PHY_PARTITION_SIZE))
			{
				s_partition_valid |= 0x1 << partition_id;
			}
			else
			{
				s_partition_valid &= ~(0x1 << partition_id);
			}
			break;
		case PARTITION_OTA:
		case PARTITION_PARTITION:
		case PARTITION_SPE:
		case PARTITION_TFM_NS:
		case PARTITION_NSPE:
		case PARTITION_OTP_NV:
		case PARTITION_CNT:
			break;
	}
	return 0;
}
#endif
extern bk_err_t bk_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size);
extern void *os_malloc(size_t size);
extern void os_free(void *ptr);
int partition_init(void)
{
	uint8_t* buf = (uint8_t*)os_malloc(PARTITION_ENTRY_LEN * sizeof(uint8_t));
	if(buf == NULL){
		BK_LOGE(TAG, "memory malloc fails.\r\n");
		return -1;
	}

	uint32_t i;
	char name[PARTITION_NAME_LEN + 1];

	uint32_t partition_start = PARTITION_PARTITION_PHY_OFFSET + PARTITION_PPC_OFFSET;
	// uint32_t partition_start = FLASH_PHY2VIRTUAL(CEIL_ALIGN_34(PARTITION_PARTITION_PHY_OFFSET + PARTITION_PPC_OFFSET));
	for(i=0;i<PARTITION_AMOUNT;++i){
		bk_flash_read_bytes(partition_start + PARTITION_ENTRY_LEN*i, buf, PARTITION_ENTRY_LEN);
		if(is_alpha(buf[0]) == 0){
			break;
		} 

		int j;
		for(j=0; j<PARTITION_NAME_LEN; ++j){
			if(buf[j] == 0xFF){
				break;
			}
			name[j] = buf[j];
		}
		name[j] = '\0';

		for (int k = 0; k < PARTITION_CNT; k++) 
		{
			if(strcmp(s_partition_name[k],name) == 0)
			{
				s_partition_config[k].phy_offset = piece_address(buf, PARTITION_OFFSET_OFFSET);
				s_partition_config[k].phy_size = piece_address(buf, PARTITION_SIZE_OFFSET);
				s_partition_config[k].phy_flags = short_address(buf, PARTITION_FLAGS_OFFSET);
				#if CONFIG_BL2_UPDATE_WITH_PC
				partition_check_partition_address_valid(k);
				#endif
			}
		}
	}

	os_free(buf);
	return 0;
}

void dump_partition(void)
{
	for (int k = 0; k < PARTITION_CNT; k++) {
		BK_LOGE(TAG, "%s offset=%x size=%x flags=%x\r\n", s_partition_name[k], \
            s_partition_config[k].phy_offset, s_partition_config[k].phy_size, s_partition_config[k].phy_flags);
	}
	#if CONFIG_BL2_UPDATE_WITH_PC
	printf("Partition check:0x%x\r\n",s_partition_valid);
	#endif
}
