/*
 * BK_HCI_protocol.c
 *
 *  Created on: 2017-5-8
 *      Author: gang.cheng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bl_uart.h"
#include "system.h"
#include "bootloader.h"
//#include "crc32.h"
//#include "bl_config.h"
#include "flash_partition.h"
#include "partitions_gen.h"
#include "boot_update.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

const PARTITION_STRUCT g_partitions_map[] = PARTITION_MAP;
extern uint32_t flash_max_size;
extern void Reset_Handler  (void);
#if CONFIG_BL2_UPDATE_WITH_PC
u32 g_forbid_dl_partition[] = {0, 1};  //mainfest can be operate when do bootlaoder update
#else
u32 g_forbid_dl_partition[] = {0, 1, 3, 4};
#endif
volatile static  uint8_t s_times = 0;

#define OPERATE_BOOT_FLAG_LEN    (32)
#if 0
bool flash_partition_is_invalid(uint32_t offset, uint32_t size)
{
	PARTITION_STRUCT *p;

	for (int i = 0; i < ARRAY_SIZE(g_forbid_dl_partition); i++) 
	{
		p = &g_partitions_map[g_forbid_dl_partition[i]];
		if (!((p->partition_offset >= (offset + size)) || ((p->partition_offset + p->partition_size) <= offset)))
		{
			printf("invalid partition, o=%x, s=%x overlapped with p%d o=%x s=%x\n", offset, size, i, p->partition_offset, p->partition_size);
			return true;
		}
	}
	if ((offset > flash_max_size) || ((offset + size) > flash_max_size))
	{
		printf("o=%x s=%x exceeds flash size %x\n", offset, size, flash_max_size);
		return true;
	}

	return false;
}
#endif

uint8_t bl_get_current_boot_execute_partition(void)
{
    uint32 reset_flash_addr= 0;
    uint8_t current_exexute_val =0;

    reset_flash_addr =(uint32)(((void*)Reset_Handler) - FLASH_DEVICE_BASE_OFFSET);

    if((reset_flash_addr > CONFIG_BL2_VIRTUAL_CODE_START)&&(reset_flash_addr < (CONFIG_BL2_VIRTUAL_CODE_START + CONFIG_BL2_VIRTUAL_CODE_SIZE)))
    {
        current_exexute_val = EXEC_BOOT_A_PARTITION;   //primary
    }
    else
    {
        current_exexute_val = EXEC_BOOT_B_PARTITION;   //secondary
    }
    if(s_times == 0)
    {
        //printf("current_exexute_val :0x%x \r\n", current_exexute_val);
        ++s_times;
    }

    return current_exexute_val;

}

#if CONFIG_BL2_UPDATE_WITH_PC
bool bl_double_check_the_written_value(uint8_t *boot_flag_new_val)
{
	uint8_t written_val[OPERATE_BOOT_FLAG_LEN] ={0};

	if(boot_flag_new_val == NULL)
	{
		printf("input boot_flag val fail \r\n");
		return false;
	}

 	flash_read_data(&written_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(written_val));

	if(!((written_val[0] == 0x63)&&(written_val[1] == 0x54)&&(written_val[2] == 0x72)&&(written_val[3] == 0x4C)&&((written_val[4] == 0x1)||(written_val[4] == 0x2))))
	{
		printf(" boot_flag_written_val = 0x%x--0x%x\r\n", written_val[0],written_val[4]);
		if((written_val[0] != 0xFF)||(written_val[1] != 0xFF)||(written_val[2] != 0xFF)||(written_val[3] != 0xFF)||(written_val[4] != 0xFF))
		{
			flash_erase_cmd(CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, FLASH_OPCODE_SE);	
		}

		flash_write_data(&boot_flag_new_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, 32);
		
		memset(&written_val[0], 0 ,32);

		flash_read_data(&written_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(written_val));
		printf(" written_val = 0x%x--0x%x---0x%x--0x%x--0x%x !!!\r\n", written_val[0],written_val[1],written_val[2],written_val[3],written_val[4]);

	}

	return true;
}

uint8_t bl_get_boot_flag_value(void)
{
	uint8_t  boot_flag_val[OPERATE_BOOT_FLAG_LEN] = {0};
	uint8_t  current_boot_part_val = 0;

	flash_read_data(&boot_flag_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(boot_flag_val));
	
    current_boot_part_val = bl_get_current_boot_execute_partition();
	if((boot_flag_val[0] == 0x63) &&(boot_flag_val[1] == 0x54)&&(boot_flag_val[2] == 0x72)&&(boot_flag_val[3] == 0x4C))
	{
		if(current_boot_part_val == EXEC_BOOT_A_PARTITION) //operate primary_boot
		{
			if(boot_flag_val[4]== BOOT_FLAG_PRIMARY)
			{
				//printf("run primary boot correctly. \r\n");
			}
			else
			{
				printf("get primary boot fail. \r\n");
				flash_erase_cmd(CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, FLASH_OPCODE_SE);

				boot_flag_val[4] = BOOT_FLAG_PRIMARY;
				memset(&boot_flag_val[28], 0 , 4);
				flash_write_data(&boot_flag_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(boot_flag_val));
				bl_double_check_the_written_value(&boot_flag_val[0]);
			}
		}
		else if(current_boot_part_val == EXEC_BOOT_B_PARTITION) //operate secondary_boot
		{
			if(boot_flag_val[4] == BOOT_FLAG_SECONDARY)
			{
				//printf("run secondary boot correctly. \r\n");
			}
			else
			{
				printf("get secondary boot fail. \r\n");
				flash_erase_cmd(CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, FLASH_OPCODE_SE);	
				
				boot_flag_val[4] = BOOT_FLAG_SECONDARY;
				memset(&boot_flag_val[28], 0 , 4);
				flash_write_data(&boot_flag_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(boot_flag_val));
				bl_double_check_the_written_value(&boot_flag_val[0]);
			}
		}
		else
		{
			printf("do nothing. \r\n");
		}
	}
	else
	{
		printf("magic abnormal.... \r\n");

		boot_flag_val[0] = 0x63; 
		boot_flag_val[1] = 0x54;
		boot_flag_val[2] = 0x72;
		boot_flag_val[3] = 0x4C;
		if (current_boot_part_val == EXEC_BOOT_B_PARTITION)
		{
			boot_flag_val[4] = BOOT_FLAG_SECONDARY;
		}
		else
		{
			boot_flag_val[4] = BOOT_FLAG_PRIMARY;
		}
		memset(&boot_flag_val[28], 0 , 4);
	
		flash_write_data(&boot_flag_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(boot_flag_val));
		bl_double_check_the_written_value(&boot_flag_val[0]);
	}
	printf("run boot_flag :%d \r\n", boot_flag_val[4]);

	return boot_flag_val[4];
}

 
uint8_t bl_set_boot_flag_value(void)
{
	uint8_t status = 0;
	uint8_t boot_flag_new_val[OPERATE_BOOT_FLAG_LEN] = {0x63, 0x54, 0x72, 0x4C};

	memset(&boot_flag_new_val[28], 0 , 4); //for bootrom log print

	flash_set_protect_type(FLASH_PROTECT_NONE);
	if(bl_get_boot_flag_value() == 1)
	{
		printf(" run on A, and refresh boot_flag \r\n");
		boot_flag_new_val[4] = BOOT_FLAG_SECONDARY ;
	}
	else if(bl_get_boot_flag_value() == 2)
	{
		printf(" run on B, and refresh boot_flag \r\n");
		boot_flag_new_val[4] = BOOT_FLAG_PRIMARY;
	}
	else
	{
		printf(" default A and refresh boot_flag \r\n");
		boot_flag_new_val[4] = BOOT_FLAG_PRIMARY;
	}

	flash_erase_cmd(CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, FLASH_OPCODE_SE);
	flash_write_data(&boot_flag_new_val[0], CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, sizeof(boot_flag_new_val));

	bl_double_check_the_written_value(boot_flag_new_val);

	flash_set_protect_type(FLASH_PROTECT_ALL);

	return status;
}

uint8_t bl_set_aon_pmu_reg03_for_deepsleep(void)
{
	AON_PMU_REG03 |= (0x1 << 1);  //set the bit1;when do bootloader update

 	if((AON_PMU_REG03 & 0x1) == 0)    //jump primary_boot
	{
		AON_PMU_REG03_SET_BF_PRIMARY;  //bit0 for deepsleep need double check
	}
	else
	{
		AON_PMU_REG03_CLR_BF_PRIMARY;
	}

	return 0;
}
#endif


bool bl_forbid_operate_boot_partition(uint32_t addr, uint32_t len)
{

#if CONFIG_BL2_UPDATE_WITH_PC
	uint8_t  current_boot_part_val = 0;

	current_boot_part_val =  bl_get_current_boot_execute_partition();
	if(s_times == 1)
	{
	   	 printf("addr :0x%x !!! current_boot_part_val :0x%x \r\n",addr,current_boot_part_val);
		 ++s_times;
	}

	if(addr >= flash_max_size)
	{
		printf("operate flash is override!!! \r\n");
		return false;
	}

    if(((addr >= CONFIG_PARTITION_B_PHY_PARTITION_OFFSET)&&((addr < CONFIG_PARTITION_B_PHY_PARTITION_OFFSET+CONFIG_PARTITION_B_PHY_PARTITION_SIZE)))||
    ((addr < CONFIG_PARTITION_B_PHY_PARTITION_OFFSET)&&((addr+len) > CONFIG_PARTITION_B_PHY_PARTITION_OFFSET+CONFIG_PARTITION_B_PHY_PARTITION_SIZE)))
    {
		printf("operate bl2 version is forbid!!! \r\n");
		return false;
    }

	if(current_boot_part_val == EXEC_BOOT_A_PARTITION)		//forbid operate primary_boot
	{
		if((addr < CONFIG_PARTITION_PHY_PARTITION_OFFSET)||
		((addr >= CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET)&&(addr  < (CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET + CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE)))||
		((addr < CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET)&&((addr + len) > (CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET )))||
		((addr >= CONFIG_BL2_PHY_PARTITION_OFFSET)&&(addr < (CONFIG_BL2_PHY_PARTITION_OFFSET + CONFIG_BL2_PHY_PARTITION_SIZE)))||
		((addr < CONFIG_BL2_PHY_PARTITION_OFFSET)&&((addr + len) > (CONFIG_BL2_PHY_PARTITION_OFFSET))))
		{
			printf("run on primary currently,so forbid operating primary \r\n");

			return false;
		}
	}
	else if(current_boot_part_val == EXEC_BOOT_B_PARTITION)	//forbid operate secondary_boot
	{
		if((addr < CONFIG_PARTITION_PHY_PARTITION_OFFSET)||
		((addr >= CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET)&&(addr < (CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET + CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_SIZE)))||
		((addr < CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET)&&((addr + len) >(CONFIG_SECONDARY_MANIFEST_PHY_PARTITION_OFFSET )))||
		((addr >= CONFIG_BL2_B_PHY_PARTITION_OFFSET)&&(addr  <(CONFIG_BL2_B_PHY_PARTITION_OFFSET + CONFIG_BL2_B_PHY_PARTITION_SIZE)))||
		((addr < CONFIG_BL2_B_PHY_PARTITION_OFFSET)&&((addr + len) >(CONFIG_BL2_B_PHY_PARTITION_OFFSET ))))
		{
			printf("run on secondary currently,so forbid operating secondary \r\n");

			return false;
		}
	}
	else
	{
			//printf("normal test partition \r\n ");
			return true;
	}
#endif
	return true;
}

bool bl_forbid_erase_boot_partition(uint32_t addr, u8   size_cmd)
{
	if(size_cmd == CMD_4KB_ERASE) //0x20
	{
		if (bl_forbid_operate_boot_partition(addr, ERASE_4KB_LENGTH) != true  )
		{
			return false;
		}
	}
	else if(size_cmd == CMD_64KB_ERASE)  //0xd8
	{
		if (bl_forbid_operate_boot_partition(addr, ERASE_64KB_LENGTH) != true  )
		{
			return false;
		}
	}
	else if(size_cmd == CMD_32KB_ERASE)  //0x52
	{
		if (bl_forbid_operate_boot_partition(addr, ERASE_32KB_LENGTH) != true  )
		{
			return false;
		}
	}
	else
	{
		printf("erase cmd error! \r\n");
		return false;
	}

	return true;

}


#include "bl2_version_str.h"
static const char s_bl2_version[] = BL2_VERSION;

static int bl_update_bootloader_version(uint8_t curr_bl2_idx,uint32_t write_pos, uint8_t*flash_save_ver_buf)
{
    uint8_t *p_buf = flash_save_ver_buf;

    if(p_buf == NULL)
    {
        return -1;
    }

	if(write_pos < (CONFIG_BL2_B_PHY_PARTITION_OFFSET + CONFIG_BL2_B_PHY_PARTITION_SIZE))
	{
		return -2;
	}
	if(write_pos != CONFIG_PARTITION_B_PHY_PARTITION_OFFSET)
	{
		return -3;
	}

    flash_set_protect_type(FLASH_PROTECT_NONE);
    flash_erase_cmd(write_pos, FLASH_OPCODE_SE);
    if (curr_bl2_idx == EXEC_BOOT_A_PARTITION)  //curr exec boot_A.
    {
        flash_write_data((uint8_t*)s_bl2_version, write_pos, sizeof(s_bl2_version)); //update boot_A version.
        flash_write_data(&p_buf[ALLOCATED_VERSION_LEN], (write_pos + ALLOCATED_VERSION_LEN), ALLOCATED_VERSION_LEN); // maintain boot_B version.
    }
    else  //curr exec boot_B.
    {
        flash_write_data(&p_buf[0], write_pos, ALLOCATED_VERSION_LEN); // maintain boot_A version.
        flash_write_data((uint8_t*)s_bl2_version, (write_pos + ALLOCATED_VERSION_LEN), sizeof(s_bl2_version));//update boot_B version.
    }

    flash_set_protect_type(FLASH_PROTECT_ALL);

    return 0;
}

static int bl_check_bootloader_version(uint8_t curr_bl2_idx,uint32_t write_pos)
{
    int ret = 0;
    int bl2_ver_compare = -1;
    uint8_t flash_save_ver_buf[ALLOCATED_VERSION_LEN*2] = {0};

    flash_read_data(&flash_save_ver_buf[0], write_pos, sizeof(flash_save_ver_buf));
    printf("S_ver:%s, A_ver :%s, B_ver:%s \r\n",s_bl2_version, flash_save_ver_buf, &flash_save_ver_buf[32]);
    printf("bl2_version :%s \r\n", BOOT_RECORD_VERSION);

    if (curr_bl2_idx == EXEC_BOOT_A_PARTITION)  //curr exec boot_A.
    {
        bl2_ver_compare = strcmp(s_bl2_version, (char*)&flash_save_ver_buf[0]);
    }
    else //curr exec boot_B.
    {
        bl2_ver_compare = strcmp(s_bl2_version, (char*)&flash_save_ver_buf[ALLOCATED_VERSION_LEN]);
    }

    if(bl2_ver_compare == 0)
    {
        printf("bl2_ver = flash_save_ver\r\n");
    }
	else
	{
		ret = bl_update_bootloader_version(curr_bl2_idx, write_pos, &flash_save_ver_buf[0]);
		printf("bl2_ver != flash_save_ver ret :%d \r\n", ret);
	}
    return ret;
}

int bl_operate_bootloader_version(void)
{
    uint8_t curr_bl2_idx = 0;
    uint32_t wr_flash_pos = CONFIG_PARTITION_B_PHY_PARTITION_OFFSET;

    curr_bl2_idx = bl_get_current_boot_execute_partition();

    bl_check_bootloader_version(curr_bl2_idx, wr_flash_pos);

    return 0;
}
