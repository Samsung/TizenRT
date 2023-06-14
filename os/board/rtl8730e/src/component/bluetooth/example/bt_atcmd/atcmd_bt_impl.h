/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __ATCMD_BT_IMPL_H__
#define __ATCMD_BT_IMPL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_types.h>

typedef int (*cmd_func_t)(int argc, char *argv[]);

typedef struct {
	const char *cmd_name;
	cmd_func_t cmd_func;
	uint16_t argc_min;
	uint16_t argc_max;
} cmd_table_t;

typedef struct cmd_help_tbl {
	const char *name;
	const char *discriptor;
	struct cmd_help_tbl *sub_tbl;
} cmd_help_table_t;

void atcmd_bt_excute(int argc, char *argv[], const cmd_table_t *cmd_table, const char *tag);

/* bt example cmd implementation */
int atcmd_bt_demo(int argc, char *argv[]);

int atcmd_bt_audio_mp_test(int argc, char *argv[]);

int atcmd_bt_central(int argc, char *argv[]);

int atcmd_bt_peripheral(int argc, char *argv[]);

int atcmd_bt_scatternet(int argc, char *argv[]);

int atcmd_bt_mesh_device(int argc, char *argv[]);

int atcmd_bt_mesh_provisioner(int argc, char *argv[]);

int atcmd_bt_a2dp(int argc, char *argv[]);

int atcmd_bt_spp(int argc, char *argv[]);

int atcmd_bt_hfp(int argc, char *argv[]);

int atcmd_ble_iso(int argc, char *argv[]);

int atcmd_ble_audio(int argc, char *argv[]);

/* device cmd impl */
int atcmd_bt_device(int argc, char *argv[]);

/* le gap cmd impl */
int atcmd_bt_le_gap(int argc, char *argv[]);

/* mesh stack cmd impl */
int atcmd_bt_mesh_stack(int argc, char *argv[]);

/* mesh config client model cmd impl */
int atcmd_bt_mesh_config(int argc, char *argv[]);

/* mesh generic on off model cmd impl */
int atcmd_bt_mesh_generic_onoff(int argc, char *argv[]);

/* mesh datatrans model cmd impl */
int atcmd_bt_mesh_datatrans_model(int argc, char *argv[]);

/* mesh light lightness model cmd impl */
int atcmd_bt_mesh_light_lightness(int argc, char *argv[]);

/* mesh light ctl model cmd impl */
int atcmd_bt_mesh_light_ctl(int argc, char *argv[]);

/* mesh light hsl model cmd impl */
int atcmd_bt_mesh_light_hsl(int argc, char *argv[]);

/* mesh light xyl model cmd impl */
int atcmd_bt_mesh_light_xyl(int argc, char *argv[]);

/* mesh light lc model cmd impl */
int atcmd_bt_mesh_light_lc(int argc, char *argv[]);

/* br gap cmd impl */
int atcmd_bt_br_gap(int argc, char *argv[]);

/* common gap cmd impl */
int atcmd_bt_gap(int argc, char *argv[]);

/* gatts cmd impl */
int atcmd_bt_gatts(int argc, char *argv[]);

/* gattc cmd impl */
int atcmd_bt_gattc(int argc, char *argv[]);

/* a2dp cmd impl */
int atcmd_bt_a2dp_cmd(int argc, char *argv[]);

/* avrcp cmd impl */
int atcmd_bt_avrcp_cmd(int argc, char *argv[]);

/* spp cmd impl */
int atcmd_bt_spp_cmd(int argc, char *argv[]);
#ifdef __cplusplus
}
#endif

#endif /* __ATCMD_BT_IMPL_H__ */