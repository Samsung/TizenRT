/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <osif.h>
#include <log_service.h>

#include <rtk_bt_def.h>
#include <rtk_bt_gatts.h>
#include <atcmd_bt_impl.h>
#include <bt_utils.h>

static int atcmd_bt_gatts_notify(int argc, char **argv)
{
    uint16_t ret = 0;
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};

    if (argc != 5 && argc != 6) {
        AT_PRINTK("[ATBC] GATTS notify failed, wrong args num!");
        return -1;
    }
    ntf_param.conn_handle = str_to_int(argv[0]);
    ntf_param.app_id = str_to_int(argv[1]);
    ntf_param.index = str_to_int(argv[2]);
    ntf_param.len = str_to_int(argv[3]);

    if (ntf_param.len != strlen(argv[4])/2) {
        AT_PRINTK("[ATBC] GATTS notify failed, notify data len is wrong!");
        return -1;
    }
    ntf_param.data = (void *)osif_mem_alloc(RAM_TYPE_DATA_ON, ntf_param.len);
    hexdata_str_to_array(argv[4], (uint8_t *)ntf_param.data, ntf_param.len);

    if (argc == 6)
        ntf_param.cid = atoi(argv[5]);

    ret = rtk_bt_gatts_notify(&ntf_param);
    if (RTK_BT_OK != ret) {
        osif_mem_free((void *)ntf_param.data);
        AT_PRINTK("[ATBC] GATTS notify failed! err: 0x%x", ret);
        return -1;
    }

    osif_mem_free((void *)ntf_param.data);
    AT_PRINTK("[ATBC] GATTS notify sending ...");
    return 0;
}

static int atcmd_bt_gatts_indicate(int argc, char **argv)
{
    uint16_t ret = 0;
    rtk_bt_gatts_ntf_and_ind_param_t ntf_param = {0};

    if (argc != 5 && argc != 6) {
        AT_PRINTK("[ATBC] GATTS notify failed, wrong args num!");
        return -1;
    }
    ntf_param.conn_handle = str_to_int(argv[0]);
    ntf_param.app_id = str_to_int(argv[1]);
    ntf_param.index = str_to_int(argv[2]);
    ntf_param.len = str_to_int(argv[3]);

    if (ntf_param.len != strlen(argv[4])/2) {
        AT_PRINTK("[ATBC] GATTS indicate failed: indicate data len is wrong!");
        return -1;
    }
    ntf_param.data = (void *)osif_mem_alloc(RAM_TYPE_DATA_ON, ntf_param.len);
    hexdata_str_to_array(argv[4], (uint8_t *)ntf_param.data, ntf_param.len);

    if (argc == 6)
        ntf_param.cid = atoi(argv[5]);

    ret = rtk_bt_gatts_indicate(&ntf_param);
    if (RTK_BT_OK != ret) {
        osif_mem_free((void *)ntf_param.data);
        AT_PRINTK("[ATBC] GATTS indicate failed! err: 0x%x", ret);
        return -1;
    }

    osif_mem_free((void *)ntf_param.data);
    AT_PRINTK("[ATBC] GATTS indicate sending ...");
    return 0;
}

static const cmd_table_t gatts_cmd_table[] = {
    {"notify",      atcmd_bt_gatts_notify,      6, 7},
    {"indicate",    atcmd_bt_gatts_indicate,    6, 7},
    {NULL,},
};

int atcmd_bt_gatts(int argc, char* argv[])
{
    atcmd_bt_excute(argc, argv, gatts_cmd_table, "[ATBC][gatts]");
    return 0;
}