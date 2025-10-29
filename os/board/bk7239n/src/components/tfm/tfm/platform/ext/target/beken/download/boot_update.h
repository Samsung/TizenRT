/*
 * BK_HCI_protocol.h
 *
 *  Created on: 2017-5-8
 *      Author: gang.cheng
 */

#ifndef BK_HCI_PROTOCOL_H_
#define BK_HCI_PROTOCOL_H_
#include <common/bk_err.h>
#include "bl_bk_reg.h"

#define FLASH_DEVICE_BASE_OFFSET       (0x02000000)
#define __PACKED_POST__  __attribute__((packed))

#define CMD_4KB_ERASE            (0x20)
#define CMD_32KB_ERASE          (0x52)
#define CMD_64KB_ERASE          (0xd8)
#define ERASE_4KB_LENGTH      (0x1000)
#define ERASE_32KB_LENGTH    (0x8000)
#define ERASE_64KB_LENGTH    (0x10000)
#define ALLOCATED_VERSION_LEN    (32)

typedef enum boot_flag_t 
{
    BOOT_FLAG_INVALID  	= 0,
    BOOT_FLAG_PRIMARY  	= 1,
    BOOT_FLAG_SECONDARY = 2,
}BOOT_FLAG;

typedef enum current_exec_part_t 
{
    EXEC_BOOT_A_PARTITION = 1,
    EXEC_BOOT_B_PARTITION = 2,
}CURRENT_EXEC_PART;

typedef enum
{
	FLASH_PROTECT_NONE = 0,     /**< flash protect type none */
	FLASH_PROTECT_ALL,          /**< flash protect type all */
	FLASH_PROTECT_HALF,         /**< flash protect type half */
	FLASH_UNPROTECT_LAST_BLOCK, /**< flash protect type unprotect last block */
} flash_protect_type_t;

typedef struct {
    char *partition_name;
    u32 partition_offset;
    u32 partition_size;
} __PACKED_POST__ PARTITION_STRUCT;

typedef enum {
    FLASH_OPCODE_WREN    = 1,
    FLASH_OPCODE_WRDI    = 2,
    FLASH_OPCODE_RDSR    = 3,
    FLASH_OPCODE_WRSR    = 4,
    FLASH_OPCODE_READ    = 5,
    FLASH_OPCODE_RDSR2   = 6,
    FLASH_OPCODE_WRSR2   = 7,
    FLASH_OPCODE_PP      = 12,
    FLASH_OPCODE_SE      = 13,
    FLASH_OPCODE_BE1     = 14,
    FLASH_OPCODE_BE2     = 15,
    FLASH_OPCODE_CE      = 16,
    FLASH_OPCODE_DP      = 17,
    FLASH_OPCODE_RFDP    = 18,
    FLASH_OPCODE_RDID    = 20,
    FLASH_OPCODE_HPM     = 21,
    FLASH_OPCODE_CRMR    = 22,
    FLASH_OPCODE_CRMR2   = 23
}FLASH_OPCODE;

extern bk_err_t bk_flash_read_bytes(uint32_t address, uint8_t *user_buf, uint32_t size);
extern bk_err_t bk_flash_write_bytes(uint32_t address, const uint8_t *user_buf, uint32_t size);
extern bk_err_t bk_flash_erase_cmd(uint32_t address, int type);
extern uint16_t bk_flash_read_sr(unsigned char byte);
extern bk_err_t bk_flash_write_sr(unsigned char bytes,  uint16_t status_reg_data);
extern bk_err_t bk_flash_set_protect_type(flash_protect_type_t type);

#define   flash_read_data(user_buf, address, size)    bk_flash_read_bytes(address,user_buf,size)
#define   flash_write_data(user_buf, address, size)   bk_flash_write_bytes(address,user_buf,size)
#define   flash_erase_cmd(address, cmd)                 bk_flash_erase_cmd(address,cmd)
#define   flash_read_sr(byte)                                   bk_flash_read_sr(byte)
#define   flash_write_sr(bytes,  status_reg_data)     bk_flash_write_sr(bytes,  status_reg_data)
#define   flash_set_protect_type(type)                    bk_flash_set_protect_type(type)

uint8_t bl_get_boot_flag_value(void);
uint8_t bl_set_boot_flag_value(void);
uint8_t bl_set_aon_pmu_bit3_for_deepsleep(void);
bool    bl_forbid_operate_boot_partition(uint32_t addr, uint32_t len);
bool    bl_forbid_erase_boot_partition(uint32_t addr, u8   size_cmd);
uint8_t bl_get_current_boot_execute_partition(void);

#endif /* BK_HCI_PROTOCOL_H_ */
