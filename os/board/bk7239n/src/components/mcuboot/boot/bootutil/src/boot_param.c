/*
 * Copyright (c) 2018-2021 Arm Limited
 * Copyright (c) 2020 Linaro Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "mcuboot_config/mcuboot_config.h"
#include "flash_partition.h"
#include "flash_driver.h"
#include "boot_param.h"
#include "components/log.h"

#define TAG "boot_param"
#define BP_LOGD BK_LOGD
#define BP_LOGI BK_LOGI
#define BP_LOGE BK_LOGE

/* Ensure UINT32_MAX is defined */
#ifndef UINT32_MAX
#define UINT32_MAX 0xFFFFFFFFU
#endif
/* Data for Boot parameters */
#define BP_SEEK_OFFSET(index)   (index == 0 ? 0 : BOOT_PARAM_PARTSIZE / 2)
#define SIGN_HEAD_OFFSET        (16)
#define IMAGE_DEFAULT_VAL       (0xFFFFFFFF)

static const char s_app_name[] = "app1";
/************************************************************************************************
 * Private Data
 ************************************************************************************************/

static const uint32_t crc32_tab[] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/************************************************************************************************
 * Public Functions
 ************************************************************************************************/
/************************************************************************************************
 * Name: crc32part
 *
 * Description:
 *   Continue CRC calculation on a part of the buffer.
 *
 ************************************************************************************************/

 static uint32_t boot_crc32part(const uint8_t *src, size_t len, uint32_t crc32val)
{
    size_t i;
    uint32_t crc_val;

    /* Validate input parameters */
    if (src == NULL || len == 0)
    {
        BP_LOGE(TAG, "Invalid CRC input: src=%p, len=%zu\n", (void*)src, len);
        return 0xFFFFFFFF;  /* Return invalid CRC value */
    }

    /* Additional size validation to prevent integer overflow */
    if (len > BOOT_PARAM_SIZE)
    {
        BP_LOGE(TAG, "CRC length %zu exceeds maximum %d\n", len, BOOT_PARAM_SIZE);
        return 0xFFFFFFFF;
    }

    crc_val = ~crc32val;

    for (i = 0; i < len; i++) 
    {
        crc_val = crc32_tab[(crc_val ^ src[i]) & 0xff] ^ (crc_val >> 8);
    }
    return ~crc_val;
}

/************************************************************************************************
 * Name: crc32
 *
 * Description:
 *   Return a 32-bit CRC of the contents of the 'src' buffer, length 'len'
 *
 ************************************************************************************************/

 static uint32_t boot_crc32(const uint8_t *src, size_t len)
{
    /* Validate parameters before calling */
    if (src == NULL || len == 0)
    {
        BP_LOGE(TAG, "Invalid CRC input: src=%p, len=%zu\n", (void*)src, len);
        return 0xFFFFFFFF;
    }

    return boot_crc32part(src, len, 0);
}

static bool is_valid_bootparam(boot_bpdata_t *bp_data)
{
    uint8_t bootparam[BOOT_PARAM_SIZE] = {0};

    if (!bp_data)
    {
        BP_LOGE(TAG, "Boot param is NULL\n");
        return false;
    }

    /* Check structure size to prevent buffer overflow */
    if (sizeof(boot_bpdata_t) > BOOT_PARAM_SIZE)
    {
        BP_LOGE(TAG, "Boot param structure size (%zu) exceeds buffer size (%d)\n", 
                sizeof(boot_bpdata_t), BOOT_PARAM_SIZE);
        return false;
    }
    
    if (bp_data->format_ver == 0 || bp_data->active_slot >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG,"Invalid data. ver: %u, active index: %u, addresses: %x, %x\n", bp_data->version, bp_data->active_slot, bp_data->address[0], bp_data->address[1]);
        return false;
    }

    memset(bootparam, 0xFF, BOOT_PARAM_SIZE);
    memcpy(bootparam, bp_data, sizeof(boot_bpdata_t));
    if (bp_data->crc_hash != boot_crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOT_PARAM_SIZE - CHECKSUM_SIZE))
    {
        BP_LOGE(TAG,"Invalid crc32 value, crc32 %u, ver: %u, active index: %u, addresses: %x, %x\n", bp_data->crc_hash, bp_data->version, bp_data->active_slot, bp_data->address[0], bp_data->address[1]);
        return false;
    }

    return true;
}

static bool is_valid_address(boot_bpdata_t *bp_data, uint8_t index)
{
    if (!bp_data)
    {
        BP_LOGE(TAG,"Boot param is NULL\n");
        return false;
    }

    /* Validate index bounds */
    if(index >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG,"Invalid index: %u\n", index);
        return false;
    }

    if(index == 0) //kernel_addr1
    {
        if (bp_data->address[0] != CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            return false;
        }
    }
    else//kernel_addr2
    { 
        if (bp_data->address[1] != CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            return false;
        }
    }
    BP_LOGD(TAG,"line[%d],is_valid_address, active_slot = %d\n", __LINE__, index);
    return true;
}

static kernel_image_ver_e boot_get_highest_kernel_image_version(void)
{
    uint32_t a_image_version = IMAGE_DEFAULT_VAL;
    uint32_t b_image_version = IMAGE_DEFAULT_VAL;
    kernel_image_ver_e image_ver_val;
    int ret;

    /* Initialize to default values in case flash read fails */
    ret = bk_flash_read_bytes((CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET + 6), &a_image_version, sizeof(uint32_t));
    if (ret < 0)
    {
        BP_LOGE(TAG, "Failed to read primary image version, errno %d\n", ret);
        a_image_version = IMAGE_DEFAULT_VAL;
    }
    
    ret = bk_flash_read_bytes((CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET + 6), &b_image_version, sizeof(uint32_t));
    if (ret < 0)
    {
        BP_LOGE(TAG, "Failed to read secondary image version, errno %d\n", ret);
        b_image_version = IMAGE_DEFAULT_VAL;
    }
    BP_LOGD(TAG,"line[%d], a_version :0x%x, b_version :0x%x\r\n",__LINE__, a_image_version, b_image_version);

    if(a_image_version < b_image_version)
    {
        if(b_image_version != IMAGE_DEFAULT_VAL) //b image version is high
        {
            image_ver_val = KERNEL_B_IMGAE_VERSION_IS_HIGN; 
        }
        else //a image version is high
        {
            image_ver_val = KERNEL_A_IMGAE_VERSION_IS_HIGN;
        }
    }
    else if(a_image_version == b_image_version)
    {
        image_ver_val = KERNEL_IMGAE_VERSION_IS_SAME;
    }
    else //(a_image_version > b_image_version)
    {
        if(a_image_version != IMAGE_DEFAULT_VAL) //a image version is high
        {
            image_ver_val = KERNEL_A_IMGAE_VERSION_IS_HIGN; 
        }
        else //b image version is high
        {
            image_ver_val = KERNEL_B_IMGAE_VERSION_IS_HIGN;
        }

    }
    return image_ver_val;
}

int boot_get_the_highest_bp_version(boot_bpinfo_t *bp_info)
{
    if(!bp_info)
    {
        return -1;
    }

    return bp_info->inuse_bp;
}

int boot_update_the_highest_bp_version(boot_bpinfo_t *bp_info)
{
    if(!bp_info)
    {
        return -1;
    }

    return (bp_info->inuse_bp ^= 1);
}

int boot_get_the_active_slot(boot_bpinfo_t *bp_info)
{
    if(!bp_info)
    {
        return -1;
    }

    /* Validate inuse_bp index */
    if(bp_info->inuse_bp < 0 || bp_info->inuse_bp >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG, "Invalid inuse_bp index: %d\n", bp_info->inuse_bp);
        return -1;
    }

    if(bp_info->inuse_bp == 0)
    {
        return bp_info->bp_data[0].active_slot;
    }
    else
    {
        return bp_info->bp_data[1].active_slot;
    }
}

int boot_update_the_active_slot(boot_bpinfo_t *bp_info)
{
    if(!bp_info)
    {
        return -1;
    }

    /* Validate inuse_bp index */
    if(bp_info->inuse_bp < 0 || bp_info->inuse_bp >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG, "Invalid inuse_bp index: %d\n", bp_info->inuse_bp);
        return -1;
    }

    if(bp_info->inuse_bp == 0)
    {
        return (bp_info->bp_data[0].active_slot ^=1);
    }
    else
    {
        return (bp_info->bp_data[1].active_slot ^=1);
    }
}

int boot_get_the_highest_bp_slot(boot_bpinfo_t *bp_info)
{
    int ret     = -1;
    int bp_slot = 0;

    /* Validate input parameter */
    if(!bp_info)
    {
        BP_LOGE(TAG, "Boot param info is NULL\n");
        return -1;
    }

    BP_LOGE(TAG,"line[%d], CONFIG_NUM_APPS  : %d\n", __LINE__,CONFIG_NUM_APPS);
    for(bp_slot = 0; bp_slot < BOOT_PARAM_COUNT; bp_slot++)
    {
        ret = bk_flash_read_bytes(CONFIG_BOOTPARAM_PHY_PARTITION_OFFSET + BP_SEEK_OFFSET(bp_slot), &bp_info->bp_data[bp_slot], sizeof(bp_info->bp_data[bp_slot]));
        BP_LOGD(TAG,"line[%d],bp_idx[%d], size is %d , version :0x%x, \r\n",__LINE__,bp_slot, sizeof(bp_info->bp_data[bp_slot]), bp_info->bp_data[bp_slot].version );
        if(ret < 0)
        {
            BP_LOGE(TAG,"line[%d],Fail to read BP%d, errno %d\n", __LINE__,bp_slot, ret);
            continue;
        }
    }

    if(ret < 0){
        return -1;
    }

    if(bp_info->bp_data[0].version > bp_info->bp_data[1].version)
    {
        if(bp_info->bp_data[0].version != IMAGE_DEFAULT_VAL)
        {
            bp_info->inuse_bp = INUSE_BP_IDX0;    //BP0
        }
        else
        {
            bp_info->inuse_bp = INUSE_BP_IDX1;    //BP1
        }
    }
    else if(bp_info->bp_data[0].version == bp_info->bp_data[1].version)
    {
        bp_info->inuse_bp = INUSE_BP_IDX0;    //BP0;
        BP_LOGD(TAG,"inuse_idx = 0 \r\n");
    }
    else  //bp_info->bp_data[0].version < bp_info->bp_data[1].version
    {
        if(bp_info->bp_data[1].version != IMAGE_DEFAULT_VAL)
        {
            bp_info->inuse_bp = INUSE_BP_IDX1;    //BP1
        }
        else
        {
            bp_info->inuse_bp = INUSE_BP_IDX0;    //BP0
        }
    }
 
    BP_LOGD(TAG,"line[%d],inuse_idx = 0x%x \r\n",__LINE__,bp_info->inuse_bp);

    return 0;
}

int boot_get_the_valid_active_slot(boot_bpinfo_t *bp_info)
{
    uint8_t active_slot = 0;
    uint32_t a_sign_magic = IMAGE_DEFAULT_VAL, b_sign_magic = IMAGE_DEFAULT_VAL; //default is 0x3DB8F396
    int ret_a, ret_b;

    if(!bp_info)
    {
        return -1;
    }

    if(bp_info->inuse_bp == INUSE_BP_IDX0)  //BP0 is highest and pre inuse
    {
        BP_LOGD(TAG,"line[%d],BP0 is inuse \r\n",__LINE__);
        if(is_valid_bootparam(&bp_info->bp_data[0]) == true)                //crc verify BP0 ok
        {
            BP_LOGE(TAG,"line[%d],BP0 crc verify ok \r\n",__LINE__);
            active_slot = bp_info->bp_data[0].active_slot;                  // 0: kernel_addr1 ; 1:kernel_addr2
        }
        else if(is_valid_bootparam(&bp_info->bp_data[1]) == true)           //crc verify BP1 ok
        {
            BP_LOGE(TAG,"line[%d],BP0 crc verify fail and BP1 crc verify ok \r\n",__LINE__);
            active_slot = bp_info->bp_data[1].active_slot;
            boot_update_the_highest_bp_version(bp_info);
        }
        else
        {
            BP_LOGE(TAG,"line[%d],BP0 crc and BP1 crc verify all fail \r\n",__LINE__);

            ret_a = bk_flash_read_bytes(CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET + SIGN_HEAD_OFFSET, &a_sign_magic, 4);
            ret_b = bk_flash_read_bytes(CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET + SIGN_HEAD_OFFSET, &b_sign_magic, 4);
            
            if (ret_a < 0)
            {
                BP_LOGE(TAG, "Failed to read primary sign magic, errno %d\n", ret_a);
            }
            if (ret_b < 0)
            {
                BP_LOGE(TAG, "Failed to read secondary sign magic, errno %d\n", ret_b);
            }

            if((a_sign_magic == IMAGE_DEFAULT_VAL) && (b_sign_magic == IMAGE_DEFAULT_VAL))
            {
                  return NEED_DO_REBOOT_RETRY;//need reboot
            }

            return NEED_DO_RECORVERY;   //NEED TODO recover BP0 and BP1
        }
    }
    else    //BP1 is highest
    {
        BP_LOGD(TAG,"line[%d],BP1 is inuse \r\n",__LINE__);
        if(is_valid_bootparam(&bp_info->bp_data[1]) == true)                //crc verify BP1 ok
        {
            BP_LOGE(TAG,"line[%d],BP1 crc verify ok \r\n",__LINE__);
            active_slot = bp_info->bp_data[1].active_slot;                  // 0: kernel_addr1 ; 1:kernel_addr2
        }
        else if(is_valid_bootparam(&bp_info->bp_data[0]) == true)           //crc verify BP0 ok
        {
            BP_LOGE(TAG,"line[%d],BP1 crc verify fail and BP0 crc verify ok \r\n",__LINE__);
            active_slot = bp_info->bp_data[0].active_slot;
            boot_update_the_highest_bp_version(bp_info);
        }
        else
        {
            BP_LOGE(TAG,"line[%d],BP1 crc and BP0 crc verify all fail \r\n",__LINE__);

            ret_a = bk_flash_read_bytes(CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET + SIGN_HEAD_OFFSET, &a_sign_magic, 4);
            ret_b = bk_flash_read_bytes(CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET + SIGN_HEAD_OFFSET, &b_sign_magic, 4);
            
            if (ret_a < 0)
            {
                BP_LOGE(TAG, "Failed to read primary sign magic, errno %d\n", ret_a);
            }
            if (ret_b < 0)
            {
                BP_LOGE(TAG, "Failed to read secondary sign magic, errno %d\n", ret_b);
            }
            
            if((a_sign_magic == IMAGE_DEFAULT_VAL) && (b_sign_magic == IMAGE_DEFAULT_VAL))
            {
                  return NEED_DO_REBOOT_RETRY;
            }

            return NEED_DO_RECORVERY;
        }
    }
    BP_LOGD(TAG,"line[%d],active_slot = %d \r\n",__LINE__, active_slot);
    return active_slot;
}

int boot_write_bp_param_partition(int inuse_bp, uint8_t *bootparam)
{
    int ret ;
    uint32_t type;

    if(!bootparam)
    {
        BP_LOGE(TAG, "Boot param buffer is NULL\n");
        return -1;
    }

    /* Validate inuse_bp parameter */
    if(inuse_bp < 0 || inuse_bp >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG, "Invalid inuse_bp: %d (must be 0-%d)\n", 
                inuse_bp, BOOT_PARAM_COUNT-1);
        return -1;
    }

    flash_op_enable_ctrl(1,1);
    type = bk_flash_get_protect_type();
    bk_flash_set_protect_type(FLASH_PROTECT_NONE);
    bk_flash_erase_sector(CONFIG_BOOTPARAM_PHY_PARTITION_OFFSET + BP_SEEK_OFFSET(inuse_bp));
    ret = bk_flash_write_bytes(CONFIG_BOOTPARAM_PHY_PARTITION_OFFSET + BP_SEEK_OFFSET(inuse_bp), (uint8_t *)(bootparam) , BOOT_PARAM_SIZE);
    flash_op_enable_ctrl(1,0);
    bk_flash_set_protect_type(type);

    return ret;
}

int boot_recover_bp_param_partition(boot_bpinfo_t *bp_info, int inuse_bp)
{
    int ret = 0;
    boot_bpdata_t recover_bp_data;
    uint8_t bootparam[BOOT_PARAM_SIZE];
    uint32_t active_slot =0;

    if (!bp_info)
    {
        BP_LOGE(TAG, "line[%d], Boot param is NULL\n", __LINE__);
        return -1;
    }

    /* Validate inuse_bp index */
    if(inuse_bp < 0 || inuse_bp >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG, "line[%d], Invalid inuse_bp index: %d\n", __LINE__, inuse_bp);
        return -1;
    }

    memset(&bootparam[0], 0xFF, BOOT_PARAM_SIZE);
    active_slot = bp_info->bp_data[inuse_bp].active_slot;

    if(active_slot < 0 || active_slot >= BOOT_PARAM_COUNT)
    {
        BP_LOGE(TAG, "line[%d], Invalid active_slot: %u\n", __LINE__, active_slot);
        return -1;
    }

    BP_LOGD(TAG,"line[%d], recover 2 times  inuse_bp =0x%x, retry active_slot = 0x%x \n",__LINE__,inuse_bp, (active_slot^1));

    if(active_slot == ACTIVE_KERNEL_A) //active_slot is kernel_a
    {
        if(bp_info->bp_data[inuse_bp].address[(active_slot ^ 1)] == CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
        else if(bp_info->bp_data[inuse_bp ^ 1].address[(active_slot ^ 1)] == CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            inuse_bp ^=1;
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
        else
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            bp_info->bp_data[inuse_bp].address[(active_slot ^ 1)] = CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET;
            bp_info->bp_data[inuse_bp ^ 1].address[(active_slot ^ 1)] = CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET;
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            inuse_bp = INUSE_BP_IDX1; //need update to bp0 .
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
    }
    else //active_slot is kernel_b
    {
        if(bp_info->bp_data[inuse_bp].address[(active_slot ^ 1)] == CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
        else if(bp_info->bp_data[inuse_bp ^ 1].address[(active_slot ^ 1)] == CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET)
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            inuse_bp ^=1;
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
        else
        {
            BP_LOGD(TAG,"line[%d] \n",__LINE__);
            bp_info->bp_data[inuse_bp].address[(active_slot ^ 1)] = CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET;
            bp_info->bp_data[inuse_bp ^ 1].address[(active_slot ^ 1)] = CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET;
            memcpy(&recover_bp_data, &bp_info->bp_data[inuse_bp], sizeof(boot_bpdata_t));
            inuse_bp = INUSE_BP_IDX1; //update to bp0 .
            recover_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        }
    }

    BP_LOGD(TAG,"line[%d],copy from BP[%d],  retry active_slot[%d] \n",__LINE__,inuse_bp,(active_slot^1));
    recover_bp_data.active_slot = (active_slot ^ 1);
    memcpy(bootparam, &recover_bp_data, sizeof(boot_bpdata_t));
    ((boot_bpdata_t *)bootparam)->crc_hash = boot_crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOT_PARAM_SIZE- CHECKSUM_SIZE);
    inuse_bp ^=1;
    ret = boot_write_bp_param_partition(inuse_bp, bootparam);
    if(ret < 0)
    {
        BP_LOGE(TAG,"line[%d],Fail to write BP%d, errno %d\n", __LINE__, bp_info->inuse_bp, ret);
        return -1;
    }

    return 0;
}

int boot_update_bp_param_partition(boot_bpinfo_t *bp_info)
{
    int ret = 0;
	int inuse_bp;
    boot_bpdata_t update_bp_data;
    uint8_t bootparam[BOOT_PARAM_SIZE];

    memset(&bootparam[0], 0xFF, BOOT_PARAM_SIZE);
    inuse_bp = boot_get_the_highest_bp_version(bp_info);  //get the curr bp version
    if(inuse_bp < 0)
    {
        BP_LOGE(TAG,"line[%d],Fail to get the highest bp version, errno %d\n", __LINE__, inuse_bp);
        return -1;
    }

    BP_LOGD(TAG,"line[%d],curr inuse_bp = %d \n",__LINE__, inuse_bp);

    if(bp_info->recover_flag == BP_DO_RECOVERY_FAIL){

        ret = boot_recover_bp_param_partition(bp_info, inuse_bp);
        return ret;
    }
    else
    {
        if(inuse_bp == INUSE_BP_IDX0) //curr is BP0, update to BP1
        {
            BP_LOGI(TAG,"line[%d],curr is BP0, update to BP1 \n",__LINE__);
            memcpy(&update_bp_data, &bp_info->bp_data[0], sizeof(boot_bpdata_t));
        }
        else //curr is BP1, update to BP0
        {
            BP_LOGI(TAG,"line[%d],curr is BP1, update to BP0 \n",__LINE__);
            memcpy(&update_bp_data, &bp_info->bp_data[1], sizeof(boot_bpdata_t));
        }
    }

    BP_LOGD(TAG,"line[%d],origin version = 0x%x active_slot:0x%x  \n", __LINE__,update_bp_data.version, update_bp_data.active_slot);
    if(bp_info->recover_flag == BP_DO_RECOVERY_SUCCESS)
    {
        update_bp_data.version = (inuse_bp == INUSE_BP_IDX0) ? 2 : 1;
        update_bp_data.active_slot = bp_info->bp_data[inuse_bp].active_slot;

        /* Validate active_slot before using as array index */
        if(update_bp_data.active_slot >= BOOT_PARAM_COUNT)
        {
            BP_LOGE(TAG, "line[%d], Invalid active_slot: %u\n", __LINE__, update_bp_data.active_slot);
            return -1;
        }

        if(update_bp_data.active_slot == ACTIVE_KERNEL_A)
        {
            if(bp_info->bp_data[inuse_bp].address[update_bp_data.active_slot] != CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET)
            {    
                BP_LOGE(TAG,"line[%d],addr:0x%x  \n", __LINE__,bp_info->bp_data[inuse_bp].address[update_bp_data.active_slot]);
                update_bp_data.address[update_bp_data.active_slot] = CONFIG_PRIMARY_TFM_S_PHY_PARTITION_OFFSET;
            }
        }
        else
        {
            if(bp_info->bp_data[inuse_bp].address[update_bp_data.active_slot] != CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET)
            {
                BP_LOGE(TAG,"line[%d],addr:0x%x  \n", __LINE__,bp_info->bp_data[inuse_bp].address[update_bp_data.active_slot]);
                update_bp_data.address[update_bp_data.active_slot] = CONFIG_SECONDARY_TFM_S_PHY_PARTITION_OFFSET;
            }
        }
        //need check
        if(strncmp(bp_info->bp_data[inuse_bp].app_data[0].name, s_app_name, strlen(s_app_name) + 1) != 0)
        {
            strncpy(update_bp_data.app_data[0].name, s_app_name, (APP_NAME_LEN - 1));
            update_bp_data.app_data[0].name[APP_NAME_LEN - 1] = '\0';
            update_bp_data.app_data[0].useidx = 0;
            update_bp_data.app_count = CONFIG_NUM_APPS;
        }

        inuse_bp ^= 1;
        BP_LOGD(TAG,"line[%d],recover active_slot = 0x%x \n",__LINE__, update_bp_data.active_slot);
    }
    else
    {
        /* Prevent integer overflow - wrap around to 1 if at max value */
        if (update_bp_data.version == UINT32_MAX)
        {
            BP_LOGE(TAG, "line[%d], version overflow detected, resetting to 1\n", __LINE__);
            update_bp_data.version = 1;
        }
        else
        {
            update_bp_data.version++;
        }
        update_bp_data.active_slot ^= 1;
        inuse_bp ^= 1;   //Normal opt OK.
    }
    BP_LOGD(TAG,"line[%d],new .version = 0x%x active_slot:0x%x  \n",__LINE__, update_bp_data.version, update_bp_data.active_slot);
    memcpy(bootparam, &update_bp_data, sizeof(boot_bpdata_t));
    ((boot_bpdata_t *)bootparam)->crc_hash = boot_crc32((uint8_t *)bootparam + CHECKSUM_SIZE, BOOT_PARAM_SIZE- CHECKSUM_SIZE);
    BP_LOGD(TAG,"line[%d],new inuse_bp = %d \n", __LINE__, inuse_bp);

    ret = boot_write_bp_param_partition(inuse_bp, bootparam);
    if(ret < 0)
    {
        BP_LOGE(TAG,"line[%d],Fail to write BP%d, errno %d\n", __LINE__, bp_info->inuse_bp, ret);
        return -1;
    }

    return 0;
}

int boot_get_recovery_active_slot(boot_bpinfo_t *bp_info)
{
    int bp_idx = 0 , slot_idx = 0;
    kernel_image_ver_e  highest_kernel_image;

    /* Validate input parameter */
    if(!bp_info)
    {
        BP_LOGE(TAG, "Boot param info is NULL\n");
        return -1;
    }

    bp_info->recover_flag = BP_DO_RECOVERY;

    highest_kernel_image = boot_get_highest_kernel_image_version();
    if(highest_kernel_image == KERNEL_A_IMGAE_VERSION_IS_HIGN)
    {
        BP_LOGD(TAG,"line[%d],kernel_a version is high bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        for(bp_idx = 0; bp_idx < BOOT_PARAM_COUNT; bp_idx++)  // iterate through all boot params
        {
            if(is_valid_address(&bp_info->bp_data[bp_idx], slot_idx) == true)
            {
                BP_LOGD(TAG,"line[%d],bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
                bp_info->inuse_bp = bp_idx;
                bp_info->bp_data[bp_idx].active_slot = slot_idx;
                return slot_idx;
            }
            else
            {
                continue;
            }
        }

        BP_LOGD(TAG,"line[%d],slot0&&slot1 kernel a addr is invlaid bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        goto all_slot_addr_is_invalid;

    }
    else if(highest_kernel_image == KERNEL_B_IMGAE_VERSION_IS_HIGN)
    {
        slot_idx = 1;
        BP_LOGD(TAG,"line[%d],kernel_b version is high bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        for(bp_idx = 0; bp_idx < BOOT_PARAM_COUNT; bp_idx++)  // iterate through all boot params
        {
            if(is_valid_address(&bp_info->bp_data[bp_idx], slot_idx) == true)
            {
                BP_LOGD(TAG,"line[%d],bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
                bp_info->inuse_bp = bp_idx;
                bp_info->bp_data[bp_idx].active_slot = slot_idx;
                return slot_idx;
            }
            else
            {
                continue;
            }
        }
        BP_LOGE(TAG,"line[%d],slot0&&slot1 kernel b addr is invlaid bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        goto all_slot_addr_is_invalid;
    }
    else//highest_kernel_image == KERNEL_NONE_IMGAE_VERSION_IS_HIGN
    {
        for(slot_idx = 0; slot_idx < BOOT_PARAM_COUNT; slot_idx++)   // iterate through all slots
        {
            for(bp_idx = 0; bp_idx < BOOT_PARAM_COUNT; bp_idx++)  // iterate through all boot params
            {
                if(is_valid_address(&bp_info->bp_data[bp_idx], slot_idx) == true)
                {
                    BP_LOGD(TAG,"line[%d],bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
                    bp_info->inuse_bp = bp_idx;
                    bp_info->bp_data[bp_idx].active_slot = slot_idx;
                    return slot_idx;
                }
                else
                {
                    continue;
                }
            }
        }
        BP_LOGE(TAG,"line[%d],slot0&&slot1 kernel b addr is invlaid bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        goto all_slot_addr_is_invalid;
    }

all_slot_addr_is_invalid:
        BP_LOGE(TAG,"line[%d],do execute all_slot_addr_is_invalid  bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        slot_idx = ACTIVE_KERNEL_A;
        bp_idx = INUSE_BP_IDX0;
        bp_info->inuse_bp = INUSE_BP_IDX0;
        bp_info->bp_data[bp_idx].active_slot = ACTIVE_KERNEL_A;

        BP_LOGD(TAG,"line[%d], bp_idx = %d, slot_idx = %d \n", __LINE__, bp_idx, slot_idx);
        return slot_idx;
}
