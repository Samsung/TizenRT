/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <osif.h>

#include <platform_opts_bt.h>
#include <log_service.h>
#include <bt_utils.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <atcmd_bt_impl.h>
#include <rtk_bt_gap.h>

static int atcmd_bt_gap_ecfc(int argc, char **argv)
{
#if RTK_BT_5_2_L2C_ECFC_SUPPORT
    char *op = argv[0];

    if(memcmp(op, "reg", 4) == 0) {
        uint16_t proto_id = 0;
        rtk_bt_gap_ecfc_register_t param = {
            .psm = RTK_BT_PSM_EATT,
            .is_le = true,
            .p_proto_id = &proto_id,
            .data_path = RTK_BT_GAP_ECFC_DATA_PATH_GATT
        };

        if(argc > 1)
            param.psm = str_to_int(argv[1]);
        if(argc > 2)
            param.is_le = str_to_int(argv[2]) ? true : false;
        if(argc > 3)
            param.data_path = str_to_int(argv[3]);

        if(rtk_bt_gap_ecfc_register(&param) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() register PSM(0x%x) OK, protocol id = %d\r\n", param.psm, proto_id);
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() register PSM(0x%x) fail\r\n", param.psm);
            return -1;
        }
    } else if(memcmp(op, "conn_req", 9) == 0) {
        rtk_bt_gap_ecfc_conn_req_t param;
        if(argc != 7) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }
        param.psm = str_to_int(argv[1]);
        param.proto_id = str_to_int(argv[2]);
        param.local_mtu = str_to_int(argv[3]);
        hexdata_str_to_bd_addr(argv[4], param.bd_addr, RTK_BD_ADDR_LEN);
        param.bd_type = str_to_int(argv[5]);
        param.cid_num = str_to_int(argv[6]);

        if(rtk_bt_gap_ecfc_send_conn_req(&param) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send connection req OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() send connection req fail\r\n");
            return -1;
        }
    } else if(memcmp(op, "conn_cfm", 9) == 0) {
        rtk_bt_gap_ecfc_conn_cfm_t param;
        int i;
        if(argc < 4) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }
        param.conn_handle = str_to_int(argv[1]);
        param.identity_id = str_to_int(argv[2]);
        param.cause = str_to_int(argv[3]);
        if(param.cause == 0 && (argc < 6 || argc != 6 + str_to_int(argv[5]) || str_to_int(argv[4]) == 0)) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }
        param.local_mtu = str_to_int(argv[4]);
        param.cid_num = str_to_int(argv[5]);
        for(i=0; i<param.cid_num; i++) {
            param.p_cid[i] = str_to_int(argv[6+i]);
        }

        if(rtk_bt_gap_ecfc_send_conn_cfm(&param) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send connection rsp OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() send connection rsp fail\r\n");
            return -1;
        }
    } else if(memcmp(op, "disconn", 8) == 0) {
        uint16_t conn_handle;
        uint16_t cid;
        if(argc != 3) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }
        conn_handle = str_to_int(argv[1]);
        cid = str_to_int(argv[2]);

        if(rtk_bt_gap_ecfc_send_disconn_req(conn_handle, cid) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() disconnect conn_handle(%d) cid(%d) OK\r\n", conn_handle, cid);
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() disconnect conn_handle(%d) cid(%d) fail\r\n", conn_handle, cid);
            return -1;
        }
    } else if(memcmp(op, "send_data", 10) == 0) {
        uint16_t conn_handle, cid, len;
        uint8_t *data;
        if(argc != 5) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send data wrong arg num\r\n");
            return -1;
        }
        conn_handle = str_to_int(argv[1]);
        cid = str_to_int(argv[2]);
        len = str_to_int(argv[3]);
        data = (uint8_t *)argv[4];

        if(rtk_bt_gap_ecfc_send_data(conn_handle, cid, data, len) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send data OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() send data fail\r\n");
            return -1;
        }
    } else if(memcmp(op, "sec_reg", 8) == 0) {
        rtk_bt_gap_ecfc_sec_reg_req_t param;
        if(argc != 7) {
            AT_PRINTK("atcmd_bt_gap_ecfc() sec_reg wrong arg num\r\n");
            return -1;
        }
        param.is_le = str_to_int(argv[1]) ? true : false;
        param.active = str_to_int(argv[2]) ? true : false;
        param.psm = str_to_int(argv[3]);
        param.uuid = str_to_int(argv[4]);
        param.requirement = str_to_int(argv[5]);
        param.key_size = str_to_int(argv[6]);

        if(rtk_bt_gap_ecfc_send_sec_reg_req(&param) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send security req OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() send security req fail\r\n");
            return -1;
        }
    } else if(memcmp(op, "reconf", 7) == 0) {
        rtk_bt_gap_ecfc_reconf_req_t param;
        int i;
        if(argc < 5 || argc != 5 + str_to_int(argv[4]) || str_to_int(argv[4]) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }

        param.conn_handle = str_to_int(argv[1]);
        param.local_mtu = str_to_int(argv[2]);
        param.local_mps = str_to_int(argv[3]);
        param.cid_num = str_to_int(argv[4]);
        for(i=0; i<param.cid_num; i++) {
            param.p_cid[i] = str_to_int(argv[5+i]);
        }

        if(rtk_bt_gap_ecfc_send_reconf_req(&param) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() send reconfigure req OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() send reconfigure req fail\r\n");
            return -1;
        }
    } else if(memcmp(op, "param", 6) == 0) {
        rtk_bt_gap_ecfc_param_t param_type;
        uint32_t value;

        if(argc != 3) {
            AT_PRINTK("atcmd_bt_gap_ecfc() wrong arg num\r\n");
            return -1;
        }

        param_type = str_to_int(argv[1]);
        value = str_to_int(argv[2]);

        if(rtk_bt_gap_ecfc_set_param(param_type, value) == 0) {
            AT_PRINTK("atcmd_bt_gap_ecfc() set parameter OK\r\n");
        } else {
            AT_PRINTK("atcmd_bt_gap_ecfc() set parameter fail\r\n");
            return -1;
        }
    } else {
        AT_PRINTK("atcmd_bt_gap_ecfc() invalid command\r\n");
        return -1;
    }

	return 0;
#else
    (void)argc;
    (void)argv;
    AT_PRINTK("This platform NOT support ECFC!\r\n");
    return -1;
#endif
}

static const cmd_table_t gap_cmd_table[] = {
	{"ecfc",       atcmd_bt_gap_ecfc,        2, 12},

	{NULL,},
};

int atcmd_bt_gap(int argc, char* argv[])
{
	atcmd_bt_excute(argc, argv, gap_cmd_table, "[ATBC][gap]");
	return 0;
}