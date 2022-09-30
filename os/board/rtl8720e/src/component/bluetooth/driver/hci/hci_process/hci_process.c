/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "osif.h"
#include "hci_config.h"
#include "hci_common.h"
#include "hci_process.h"
#include "hci_platform.h"
#include "hci_dbg.h"

#define USE_HCI_H4 1
#ifdef USE_HCI_H4
    #define RESERVE_LEN 1
#elif USE_HCI_H5
    #define RESERVE_LEN 4
#else
    #define RESERVE_LEN 0
#endif

#ifdef hci_platform_START_IQK
static uint8_t hci_process_start_iqk(uint16_t opcode)
{
    /* OpCode: 0xfd4a, Data Len: Cmd(7), Event(6) */
    uint8_t buf_raw[RESERVE_LEN+7];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    if (HCI_SUCCESS == hci_platform_check_iqk())
        return HCI_IGNORE;

    for (uint8_t i = 0; i < HCI_START_IQK_TIMES; i++)
    {
        buf[0] = (uint8_t)(opcode >> 0);
        buf[1] = (uint8_t)(opcode >> 8);
        buf[2] = (uint8_t)(HCI_IQK_DATA_LEN);
        buf[3] = (uint8_t)(hci_iqk_data[i].offset);
        buf[4] = (uint8_t)(hci_iqk_data[i].value >> 0);
        buf[5] = (uint8_t)(hci_iqk_data[i].value >> 8);
        buf[6] = (uint8_t)(0);

        if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 7, HCI_SYNC))
            return HCI_FAIL;

        /* Wait Resp: Need, Check Resp: No Need */
    }

    if (HCI_SUCCESS != hci_platform_start_iqk())
        return HCI_FAIL;

    return HCI_SUCCESS;
}
#endif

static uint8_t hci_process_read_local_ver(uint16_t opcode)
{
    /* OpCode: 0x1001, Data Len: Cmd(3), Event(14) */
    uint8_t buf_raw[RESERVE_LEN+14];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = (uint8_t)(0);

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 3, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: OpCode and Status, LMP Subversion */
    if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00){
        return HCI_FAIL;
    }

    uint16_t lmp_sbuver = ((uint16_t)buf[12]) | (((uint16_t)buf[13]) << 8);
    if (HCI_DEFAULT_LMP_SUBVER != lmp_sbuver)
        return HCI_FAIL;

    return HCI_SUCCESS;
}

static uint8_t hci_process_read_rom_ver(uint16_t opcode)
{
    /* OpCode: 0xfc6d, Data Len: Cmd(3), Event(7) */
    uint8_t buf_raw[RESERVE_LEN+7];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = (uint8_t)(0);

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 3, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: OpCode and Status */
    if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
        return HCI_FAIL;

    /* Get Chip Id (Rom_Ver+1) and Find Patch */
    hci_platform_record_chipid(buf[6] + 1);

    return HCI_SUCCESS;
}

static uint8_t hci_process_update_baudrate(uint16_t opcode)
{
    /* OpCode: 0xfc17, Data Len: Cmd(7), Event(6) */
    uint8_t buf_raw[RESERVE_LEN+7];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = sizeof(uint32_t);
    hci_platform_get_baudrate(&buf[3], buf[2]);

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 7, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: OpCode and Status */
    if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
        return HCI_FAIL;

    /* Set Host-side baudrate */
    if (HCI_SUCCESS != hci_platform_set_baudrate())
        return HCI_FAIL;

    return HCI_SUCCESS;
}

static uint8_t hci_process_download_patch(uint16_t opcode)
{
    /* OpCode: 0xfc20, Data Len: Cmd(256), Event(7) */
    uint8_t ret = HCI_SUCCESS;
    uint8_t buf_raw[RESERVE_LEN+256];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    ret = hci_platform_dl_patch_init();
    if (HCI_SUCCESS != ret)
        goto dl_patch_done;

    while (1)
    {
        buf[0] = (uint8_t)(opcode >> 0);
        buf[1] = (uint8_t)(opcode >> 8);
        ret = hci_platform_get_patch_cmd_len(&buf[2]);
        if (HCI_SUCCESS != ret)
            goto dl_patch_done;

        ret = hci_platform_get_patch_cmd_buf(&buf[3], buf[2]);
        if (HCI_SUCCESS != ret)
            goto dl_patch_done;

        ret = hci_sa_send(H4_CMD, buf, buf[2] + 3, HCI_SYNC);
        if (HCI_SUCCESS != ret)
            goto dl_patch_done;

        /* Check Resp: OpCode and Status */
        if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
            goto dl_patch_done;

        if (buf[6] & 0x80)
            break;
    }

dl_patch_done:
    hci_platform_dl_patch_done();


    return ret;
}

#ifdef CONFIG_HCI_FT_MODE
static uint8_t hci_process_read_thermal(uint16_t opcode)
{
    /* OpCode: 0xfc40, Data Len: Cmd(3), Event(7) */
    uint8_t buf_raw[RESERVE_LEN+7];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    if (HCI_SUCCESS != hci_platform_check_mp())
        return HCI_IGNORE;

    for (uint8_t i = 0; i < HCI_READ_THER_TIMES; i++)
    {
        buf[0] = (uint8_t)(opcode >> 0);
        buf[1] = (uint8_t)(opcode >> 8);
        buf[2] = (uint8_t)(0);

        if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 3, HCI_SYNC))
            return HCI_FAIL;

        /* Check Resp: OpCode and Status, Get Thermal */
        if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
            return HCI_FAIL;

        hci_platform_get_thermal(buf[5]);
    }

    if (HCI_SUCCESS != hci_platform_set_thermal())
        return HCI_FAIL;

    return HCI_SUCCESS;
}
#endif

#ifdef hci_platform_WRITE_IQK
static uint8_t hci_process_write_iqk(uint16_t opcode)
{
    /* OpCode: 0xfd91, h4 buf_len: Cmd(15), Event(6) */
    uint8_t buf_raw[RESERVE_LEN+3+HCI_WRITE_IQK_DATA_LEN];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = (uint8_t)(HCI_WRITE_IQK_DATA_LEN);
    if (HCI_SUCCESS != hci_platform_get_iqk_data(&buf[3], buf[2]))
        return HCI_FAIL;

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 3+HCI_WRITE_IQK_DATA_LEN, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: No Need */

    return HCI_SUCCESS;
}
#endif

#ifdef hci_platform_SET_CUT_VER
static uint8_t hci_process_set_cut_ver(uint16_t opcode)
{
    /* OpCode: 0x0c03, Data Len: Cmd(3), Event(6) */
    uint8_t buf_raw[RESERVE_LEN+6];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = (uint8_t)(1);
	buf[3] = (uint8_t)(hal_sys_get_rom_ver()); // 0: tes chip, 1: A cut, 2: B cut, 3: C cut
	//buf[3] = (uint8_t)(3);

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 4, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: OpCode and Status, LMP Subversion */
    if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
        return HCI_FAIL;

    return HCI_SUCCESS;
}
#endif

#ifdef CONFIG_HCI_RESET
static uint8_t hci_process_hci_reset(uint16_t opcode)
{
    /* OpCode: 0x0c03, Data Len: Cmd(3), Event(6) */
    uint8_t buf_raw[RESERVE_LEN+6];
    uint8_t* buf = buf_raw+RESERVE_LEN;

    buf[0] = (uint8_t)(opcode >> 0);
    buf[1] = (uint8_t)(opcode >> 8);
    buf[2] = (uint8_t)(0);

    if (HCI_SUCCESS != hci_sa_send(H4_CMD, buf, 3, HCI_SYNC))
        return HCI_FAIL;

    /* Check Resp: OpCode and Status, LMP Subversion */
    if (!(*(uint16_t *)(&buf[3]) == opcode) || buf[5] != 0x00)
        return HCI_FAIL;

    return HCI_SUCCESS;
}
#endif

#ifdef CONFIG_HCI_READ_BDADDR
static uint8_t hci_process_read_bd_addr(uint16_t opcode)
{
	(void)opcode;
	/* TODO */
	return HCI_SUCCESS;
}
#endif

static struct {
    uint16_t opcode;
    uint8_t (*func)(uint16_t opcode);
} hci_process_table[] = {
#ifdef hci_platform_START_IQK
    {0xFD4A, hci_process_start_iqk},
#endif
    {0x1001, hci_process_read_local_ver},
    {0xFC6D, hci_process_read_rom_ver},
    {0xFC17, hci_process_update_baudrate},
    {0xFC20, hci_process_download_patch},
#ifdef CONFIG_HCI_FT_MODE
    {0xFC40, hci_process_read_thermal},
#endif
#ifdef hci_platform_WRITE_IQK
    {0xFD91, hci_process_write_iqk},
#endif
#ifdef hci_platform_SET_CUT_VER
	{0xFDAB, hci_process_set_cut_ver},
#endif
#ifdef CONFIG_HCI_RESET
    {0x0C03, hci_process_hci_reset},
#endif
#ifdef CONFIG_HCI_READ_BDADDR
	{0x1009, hci_process_read_bd_addr},
#endif
    {0, NULL}
};

uint8_t hci_process(void)
{
    uint8_t step = 0;

    while (hci_process_table[step].func)
    {
        if (HCI_FAIL == hci_process_table[step].func(hci_process_table[step].opcode))
            return HCI_FAIL;
        step++;
    }

    return HCI_SUCCESS;
}
