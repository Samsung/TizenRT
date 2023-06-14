/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __ATCMD_BT_VENDOR_H__
#define __ATCMD_BT_VENDOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

int atcmd_bt_tx_power_gain(int argc, char* argv[]);

int atcmd_bt_hci_debug_enable(int argc, char* argv[]);

int atcmd_bt_sleep_mode(int argc, char *argv[]);

int atcmd_bt_ant(int argc, char *argv[]);

int atcmd_bt_read_reg(int argc, char *argv[]);

int atcmd_bt_sof_eof_ind(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* __ATCMD_BT_VENDOR_H__ */