/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2018  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/
#include <drv_types.h>
#include <hal_data.h>
#include "rtl8721d.h"
#include "rtl8721d_bt_calibration.h"

extern Rltk_wlan_t	rltk_wlan_info[NET_IF_NUM];

#define BTIQK_DELAY_TIME_8721D 10

#define MAX_TOLERANCE 5

#define BTIQK_BB_REG_NUM_8721D 16
#define BTIQK_RF_REG_NUM_8721D 6
#define BTIQK_MAC_REG_NUM_8721D 2
#define DBG_BT_CALIBRATION				(0) /* Debug log, default close */

#define BT_IQK_TOLERANCE				(2) /* Suggested by BT Jeffery */
#define BT_DCK_TOLERANCE				(1) /* Suggested by RDC BiChing */
#define BT_DCK_BIT_LEN					(9)
#define BT_IQK_BIT_LEN					(10)
#define BT_LOK_BIT_LEN					(6)
#define BT_IQK_CANDIDATE_NUM        (3)
#define BT_IQK_MAX_RETRY		    (2)

#define _ENABLE_DACDCK_BEFORE_IQK_
void
_btiqk_backup_bb_registers_8721d(struct dm_struct *dm,
				 u32 *adda_reg,
				 u32 *adda_backup,
				 u32 register_num)
{
	u32 i;

	for (i = 0; i < register_num; i++)
		adda_backup[i] = odm_get_bb_reg(dm, adda_reg[i], MASKDWORD);
}

void
_btiqk_backup_mac_registers_8721d(struct dm_struct *dm,
				  u32 *mac_reg,
				  u32 *mac_backup,
				  u32 register_num)
{
	u32 i;

	for (i = 0; i < register_num; i++)
		mac_backup[i] = odm_read_4byte(dm, mac_reg[i]);
}

void
_btiqk_backup_rf_registers_8721d(struct dm_struct *dm,
				 u32 *rf_reg,
				 u32 rf_reg_backup[][1],
				 u32 rf_reg_num)
{
	u32 i;

	for (i = 0; i < rf_reg_num; i++)
		rf_reg_backup[i][RF_PATH_A] = odm_get_rf_reg(dm, RF_PATH_A,
							     rf_reg[i],
							     RFREG_MASK);
}

void
_btiqk_reload_bb_registers_8721d(struct dm_struct *dm,
				 u32 *adda_reg,
				 u32 *adda_backup,
				 u32 regiester_num)
{
	u32 i;

	for (i = 0; i < regiester_num; i++)
		odm_set_bb_reg(dm, adda_reg[i], MASKDWORD, adda_backup[i]);
}

void
_btiqk_reload_mac_registers_8721d(struct dm_struct *dm,
				  u32 *mac_reg,
				  u32 *mac_backup,
				  u32 register_num)
{
	u32 i;

	RF_DBG(dm, DBG_RF_IQK, "[BTIQK] Reload MAC parameters !\n");
	for (i = 0; i < register_num; i++)
		odm_write_4byte(dm, mac_reg[i], mac_backup[i]);
}

void _btiqk_reload_rf_registers_8721d(struct dm_struct *dm,
				      u32 *rf_reg,
				      u32 rf_reg_backup[][1],
				      u32 rf_reg_num)
{
	u32 i;

	for (i = 0; i < rf_reg_num; i++) {
		odm_set_rf_reg(dm, RF_PATH_A, rf_reg[i], RFREG_MASK,
			       rf_reg_backup[i][RF_PATH_A]);
		RF_DBG(dm, DBG_RF_IQK, "[BTIQK] Reload RF_A 0x%x = 0x%x\n",
		       rf_reg[i], rf_reg_backup[i][RF_PATH_A]);
	}
}

int16_t signed_value_conversion(uint16_t val, uint8_t bit_len)
{
	int16_t sign_bit = 1 << (bit_len-1);
	int16_t value_mask = (1 << (bit_len-1)) - 1;
	if (val & sign_bit) { /* val < 0 */
		int16_t twos_complement_val = ((~val) & value_mask) + 1;
		return -(twos_complement_val);
	}
	else
		return val;
}

boolean
_btiqk_simularity_compare_8721d(struct dm_struct *dm,
				s32 result[][2],
				u8 c1,
				u8 c2)
{
	u32 i, j, diff, simularity_bit_map, bound = 2;
	boolean is_result = true;
	s32 tmp1 = 0, tmp2 = 0;

	RF_DBG(dm, DBG_RF_IQK,
	       "===> BTIQK:phy simularity compare 8721d c1 %d c2 %d!!!\n",
	       c1, c2);
	simularity_bit_map = 0;
	for (i = 0; i < bound; i++) {
		if (i == 1 || i == 3 || i == 5 || i == 7) {
			if ((result[c1][i] & 0x00000200) != 0)
				tmp1 = result[c1][i] | 0xFFFFFC00;
			else
				tmp1 = result[c1][i];

			if ((result[c2][i] & 0x00000200) != 0)
				tmp2 = result[c2][i] | 0xFFFFFC00;
			else
				tmp2 = result[c2][i];
		} else {
			tmp1 = result[c1][i];
			tmp2 = result[c2][i];
		}

		diff = (tmp1 > tmp2) ? (tmp1 - tmp2) : (tmp2 - tmp1);

		if (diff > MAX_TOLERANCE) {
			RF_DBG(dm, DBG_RF_IQK,
			       "BTIQK:differnece overflow %d index %d compare1 0x%x compare2 0x%x!!!\n",
			       diff, i, result[c1][i], result[c2][i]);
			simularity_bit_map = simularity_bit_map | (1 << i);
		}
	}

	RF_DBG(dm, DBG_RF_IQK,
	       "BTIQK:phy simularity compare 8721d simularity bit_map %x !!!\n",
	       simularity_bit_map);

	if (simularity_bit_map == 0)
		return is_result;

	if (!(simularity_bit_map & 0x03)) { /*path A TX OK*/
		for (i = 0; i < 2; i++)
			result[3][i] = result[c1][i];
	}

	return false;
}
boolean _btiqk_lok_8721d(void *dm_void, u32 *u32bt_iqk_x, u32 *u32bt_iqk_y, u32 *u32bt_dac_i, u32 *u32bt_dac_q)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 btiqk_x, btiqk_y, r_tx_x, r_tx_y;
	u32 lok_result, btiqk_success = 0;
	u32 reg_eac, iqk_fail_bit, mac_reg4;
	u8 t = 0;

	RF_DBG(dm, DBG_RF_IQK, "[BTIQK] BTIQK:Start!!!");

	/*11_AD_TXIQK_LOK_20181024_SingleTone_polling*/
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xCCF400C0);
	odm_set_bb_reg(dm, R_0xc80, MASKDWORD, 0x40000100);
	odm_set_bb_reg(dm, R_0xe6c, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x03a05601);
	odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000800e4);
	odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);
	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x000000cb);/*AFE = BT*/
	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00800000);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdf, 0x00040, 0x0);

	odm_set_bb_reg(dm, R_0xe30, MASKDWORD, 0x1400cc08);
	odm_set_bb_reg(dm, R_0xe34, MASKDWORD, 0x3800cc1b);
	odm_set_bb_reg(dm, R_0xe38, MASKDWORD, 0x821403ff);
	odm_set_bb_reg(dm, R_0xe3c, MASKDWORD, 0x28160000);
	odm_set_bb_reg(dm, R_0xe40, MASKDWORD, 0x01007c00);
	odm_set_bb_reg(dm, R_0xe44, MASKDWORD, 0x01004800);
	odm_set_bb_reg(dm, R_0xe48, MASKDWORD, 0xf8000800);
	odm_set_bb_reg(dm, R_0xe4c, MASKDWORD, 0x00402911);
	odm_set_bb_reg(dm, R_0xe4c, 0x00180000, 0x0);
	odm_set_bb_reg(dm, R_0xe50, MASKDWORD, 0x1000dc10);
	odm_set_bb_reg(dm, R_0xe54, MASKDWORD, 0x1000dc10);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xee, 0x40000, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdd, 0x00004, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, 0x06, 0x03000, 0x3);/*DAC Gain*/
	odm_set_rf_reg(dm, RF_PATH_A, 0x06, 0x00e00, 0x2);/*IPA Gain*/
	odm_set_rf_reg(dm, RF_PATH_A, 0x06, 0x001e0, 0x3);/*PAD Gain*/
	odm_set_rf_reg(dm, RF_PATH_A, 0x06, 0x0001f, 0x3);/*TXBB Gain*/
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x18, 0x30000, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, 0x01, 0x00004, 0x1);/*RF GNT_BT = 1*/

	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x80800000);

	mac_reg4 = odm_read_4byte(dm, 0x4);/*Shut down DAI*/
	odm_write_4byte(dm, 0x4, mac_reg4 & ~(BIT(31)));
	odm_set_bb_reg(dm, R_0xe48, MASKDWORD, 0xfa000800);
	odm_set_bb_reg(dm, R_0xe48, MASKDWORD, 0xf8000800);
	while ((odm_get_bb_reg(dm, R_0xeac, BIT(26)) != 1) && (t < 20)) {
		ODM_delay_ms(1);
		t++;
	}
	odm_write_4byte(dm, 0x4, mac_reg4);/*Enable DAI*/
	/*12_Read_Tx_X_Tx_Y*/
	btiqk_x = odm_get_bb_reg(dm, R_0xe94, 0x03ff0000);
	btiqk_y = odm_get_bb_reg(dm, R_0xe9c, 0x03ff0000);
	RF_DBG(dm, DBG_RF_IQK,
	       "[BTIQK] btiqk_x = 0x%x, btiqk_y = 0x%x, iqk end = 0x%x/n",
	       btiqk_x, btiqk_y, odm_get_bb_reg(dm, R_0xeac, BIT(26)));

    *u32bt_iqk_x = btiqk_x;
    *u32bt_iqk_y = btiqk_y;

	/* Check failed */
	reg_eac = odm_get_bb_reg(dm, R_0xeac, MASKDWORD);
	iqk_fail_bit = odm_get_bb_reg(dm, R_0xeac, 0x10000000);
	RF_DBG(dm, DBG_RF_IQK, "[BTIQK] 0xeac = 0x%x,eacBIT28=0x%x\n", reg_eac,
	       iqk_fail_bit);
	/*monitor image power before & after IQK*/
	RF_DBG(dm, DBG_RF_IQK,
	       "[BTIQK] 0xe90(before IQK)= 0x%x, 0xe98(afer IQK) = 0x%x\n",
	       odm_get_bb_reg(dm, R_0xe90, MASKDWORD),
	       odm_get_bb_reg(dm, R_0xe98, MASKDWORD));
#if 1
	/*13_Read_IQDAC, re read/write rf08 means enable writing_table*/
	r_tx_x = odm_get_rf_reg(dm, RF_PATH_A, RF_0x08, 0xfc000);
	r_tx_y = odm_get_rf_reg(dm, RF_PATH_A, RF_0x08, 0x003f0);
	lok_result = (r_tx_x << 6) | (r_tx_y);

    *u32bt_dac_i = r_tx_x;
    *u32bt_dac_q = r_tx_y;
#endif
	/*14_restore_BTRF_setting_20181024*/
	odm_set_bb_reg(dm, R_0xe28, MASKDWORD, 0x00000000);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xdd, 0x00004, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xee, 0x40000, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, 0x01, 0x00004, 0x0);

	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x00000003);

	if (!(reg_eac & BIT(28)) &&
	    (((btiqk_x & 0x03FF0000) >> 16) != 0x142) &&
	    (((btiqk_y & 0x03FF0000) >> 16) != 0x42))
	{
		btiqk_success |= 0x01;
#if DBG_BT_CALIBRATION 
        printf("[RTK_BT] the IQK and LOK procedure is done\n");	/* success != workable */
#endif
	}
	else
	{
#if DBG_BT_CALIBRATION 
        printf("[RTK_BT] the IQK and LOK procedure is fail\n");
#endif
		RF_DBG(dm, DBG_RF_IQK, "[BTIQK] pathA TX IQK is fail!\n");
	}

	return btiqk_success;
}

void sort_iqk(u32* iqk_array, int num)
{
	u32 temp;
	int i, j;
	short int val_i, val_j;

	for (i = 0; i < (num - 1); i++) {
		val_i = signed_value_conversion(iqk_array[i], BT_IQK_BIT_LEN);
		for (j = (i + 1); j < num; j++) {
			val_j = signed_value_conversion(iqk_array[j], BT_IQK_BIT_LEN);
			if (val_i > val_j) {
				temp = iqk_array[i];
				iqk_array[i] = iqk_array[j];
				iqk_array[j] = temp;
				val_i = signed_value_conversion(iqk_array[i], BT_IQK_BIT_LEN);
			}
		}
	}
}

void sort_lok(u16* lok_array, int num)
{
	u16 temp;
	int i, j;

	for (i = 0; i < (num - 1); i++) {
		for (j = (i + 1); j < num; j++) {
			if (lok_array[i] > lok_array[j]) {
				temp = lok_array[i];
				lok_array[i] = lok_array[j];
				lok_array[j] = temp;
			}
		}
	}
}

bool bt_calibration_similar(u16 a, u16 b, u8 bit_len, u8 tolerance, bool signed_val)
{
	short int val_a = (signed_val)? signed_value_conversion(a, bit_len): a;
	short int val_b = (signed_val)? signed_value_conversion(b, bit_len): b;

	short int diff = (val_a > val_b) ? (val_a - val_b) : (val_b - val_a);

	if (diff <= tolerance) {
		return true;
	}
	else {
		return false;
	}
}

bool select_iqk(u32* iqk_array, int num, u32* selected)
{
	u8 median_idx = (num/2);
	sort_iqk(iqk_array, num);

	if (bt_calibration_similar(iqk_array[0], iqk_array[num-1], BT_IQK_BIT_LEN, BT_IQK_TOLERANCE, true))
	{
		*selected = iqk_array[median_idx];
		return true;
	}
	else {
		return false;
	}
}



bool select_lok(u16* lok_array, int num, u32* selected)
{
	uint8_t median_idx = (num/2);

	sort_lok(lok_array, num);

	if (bt_calibration_similar(lok_array[0], lok_array[num-1], BT_LOK_BIT_LEN, BT_IQK_TOLERANCE, false))
	{
		*selected = lok_array[median_idx];
		return true;
	}
	else {
		return false;
	}

}

void
_btiqk_retry_8721d(struct dm_struct *dm,
		   u32 result[])
{
	u32 i;
	u8 path_aok = 0;

#if MP_DRIVER
	const u32 retry_count = 3;
#else
	const u32 retry_count = 2;
#endif

    u32 btiqk_x, btiqk_y, btlok_i, btlok_q;

	int candidate_num = BT_IQK_CANDIDATE_NUM;
	BT_Cali_TypeDef defaul_value = {0x100,0x00,0x20,0x20}; /* default value */
	BT_Cali_TypeDef selected = defaul_value;
	u32 iqk_array[2][BT_IQK_CANDIDATE_NUM]; /* IQK_XX, IQK_YY */
	u16 lok_array[2][BT_IQK_CANDIDATE_NUM]; /* QDAC, IDAC */
	bool calibration_valid[4] = {true, true, true, true};			/* IQK_XX, IQK_YY, QDAC, IDAC */
	bool calibration_status[4] = {false, false, false, false};	/* IQK_XX, IQK_YY, QDAC, IDAC */
	bool calibration_all_vaid = false;
	const char *name[]  = {"IQK_xx","IQK_yy","QDAC","IDAC"};
	int iqk_cnt = 0;

	while ( (!calibration_all_vaid) && (iqk_cnt < BT_IQK_MAX_RETRY) ) {
		/* Do calibration */
		iqk_cnt ++;
		for(i=0; i < BT_IQK_CANDIDATE_NUM; i++) {
			path_aok = _btiqk_lok_8721d(dm, &btiqk_x, &btiqk_y, &btlok_i, &btlok_q);

			iqk_array[0][i] = btiqk_x;
			iqk_array[1][i] = btiqk_y;
			lok_array[0][i] = btlok_i;
			lok_array[1][i] = btlok_q;
#if DBG_BT_CALIBRATION
			DBG_8195A("\n\r[BT IQK loop][%d] IQK_xx = 0x%x, IQK_yy = 0x%x, IDAC = 0x%x, QDAC = 0x%x\n",
				   i, iqk_array[0][i], iqk_array[1][i], lok_array[0][i], lok_array[1][i]);
#endif

		}

		/* Check IQK candidate range <= BT_IQK_TOLERANCE indivdually */
		for(i = 0; i < 2; i++) {
			if (calibration_status[i] == false) {
				u32* iqk_ptr = (i==0)? (&selected.IQK_xx) : (&selected.IQK_yy);
				calibration_status[i] = select_iqk(iqk_array[i], BT_IQK_CANDIDATE_NUM, iqk_ptr);
			}
		}

		/* Check LOK candidate range <= BT_IQK_TOLERANCE indivdually */
		for(i = 0; i < 2; i++) {

			if (calibration_status[i+2] == false)
            {
				u32* lok_ptr = (i==0)? (&selected.IDAC) : (&selected.QDAC);
				calibration_status[i+2] = select_lok(lok_array[i], candidate_num, lok_ptr);
			}

		}

#if DBG_BT_CALIBRATION
        DBG_8195A("\n\r %x, %x, %x, %x\n", calibration_status[0], calibration_status[1], calibration_status[2], calibration_status[3]);
        DBG_8195A("\n\r %x, %x, %x, %x\n", calibration_valid[0], calibration_valid[1], calibration_valid[2], calibration_valid[3]);
#endif

		if (!memcmp(calibration_status, calibration_valid, sizeof(calibration_status))) {
			/* All calibration done, exit retry loop */
			calibration_all_vaid = true;
			break;
		}
		else {

#if DBG_BT_CALIBRATION
			DBG_8195A("\n\r***********************[IQK FAIL]***********************");
			/* Debug MSG */
			for(i=0; i<4; i++) {
				if (!calibration_status[i]) {
					DBG_8195A("\n\r[BT IQK][%d] %s fail\n", i, name[i]);
				}
			}
#endif

		}

	}
#if 1
	if (!calibration_all_vaid) {
#if DBG_BT_CALIBRATION
		DBG_8195A("\n\r***********************[_IQK_]***********************");
#endif
		for(i=0; i<4; i++) {
			if (!calibration_status[i]) {
				DBG_8195A("\n\r[BT IQK][Warning] %s fail, use default value", name[i]);
			}
		}
	}

#if DBG_BT_CALIBRATION
	DBG_8195A("\n\r[BT IQK][Selected] IQK_xx = 0x%x, IQK_yy = 0x%x, IDAC = 0x%x, QDAC = 0x%x\n",
		   selected.IQK_xx, selected.IQK_yy, selected.IDAC, selected.QDAC);
#endif

    result[0] = selected.IQK_xx;
    result[1] = selected.IQK_yy;
    result[2] = selected.IDAC;
    result[3] = selected.QDAC;

#endif
	RF_DBG(dm, DBG_RF_IQK, "%s <==\n", __func__);
}

u32
_btiqk_return_result_8721d(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
    u32 result[4];

    _btiqk_retry_8721d(dm, result);
    return (result[0]<<22)|(result[1]<<12)|(result[2]<<6)|(result[3]<<0);
}

void phy_bt_lok_write_8721d(struct dm_struct *dm,
			    u32 idac,
			    u32 qdac)
{
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xee, BIT18, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x1, BIT2, 0x1);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x08, 0xfc000, idac);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x08, 0x003f0, qdac);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0x1, BIT2, 0x0);
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xee, BIT18, 0x0);
}

#if 1
void phy_bt_adda_dck_8721d(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
    u32 t = 0, dck_cnt = 0;
    u8 dck_ok = 0;
	u16 adci, adcq, dai_adci, daq_adcq;

	RF_DBG(dm, DBG_RF_IQK, "[BT DCK]ADC offset mark off!!");
	while (!dck_ok) {
	/*01_DAC_DCK_ADC_offset_mark_off_20181024*/
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xccf000c0);
	odm_set_bb_reg(dm, R_0xc80, MASKDWORD, 0x40000100);
	odm_set_bb_reg(dm, R_0xe6c, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x03a05601);
	odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000800e4);
	odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);
	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x0000008b);

	odm_set_bb_reg(dm, R_0x950, 0x01ff01ff, 0x00);

	odm_set_bb_reg(dm, R_0x818, 0x00400000, 0x0);

	odm_set_bb_reg(dm, R_0x990, 0x40000000, 0x1);


	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x0);	/* RX BB to AD off */
		ODM_delay_ms(25);  /* without this delay, BT LOK may fail (still don't know why) (Debugged with RDC BiChing) */

	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00008000, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x0);

	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x1);

	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x0);


	odm_set_bb_reg(dm, R_0xe28, 0x00000008, 0x1);

	odm_set_bb_reg(dm, R_0xe48, 0x00f00000, 0x1);

	odm_set_bb_reg(dm, R_0xe48, 0x00000002, 0x0);

	odm_set_bb_reg(dm, R_0x908, 0x00000fff, 0x300);


	odm_set_bb_reg(dm, R_0x990, 0x80000000, 0x0);
	ODM_delay_us(1);

	odm_set_bb_reg(dm, R_0x990, 0x80000000, 0x1);


	/* read 0xdf4[9:1]    ADCI offset */
	adci = odm_get_bb_reg(dm, R_0xdf4, 0x000003FE);
	/* read 0xdf4[31:23]  ADCQ offset */
	adcq = odm_get_bb_reg(dm, R_0xdf4, 0xFF800000);
	RF_DBG(dm, DBG_RF_IQK, "[BT DCK]BT DAC mark off!!");

	/*04_DAC_DCK_mark_off_20181024*/
		odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xccf000c0);

		odm_set_bb_reg(dm, R_0xc80, MASKDWORD, 0x40000100);

		odm_set_bb_reg(dm, R_0xe6c, MASKDWORD, 0x03c00016);

		odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x03c00016);

		odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x03a05601);

		odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000800e4);

		odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);

	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x0000008b);

	odm_set_bb_reg(dm, R_0x818, 0x00400000, 0x0);

	odm_set_bb_reg(dm, R_0x990, 0x40000000, 0x1);


	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x0);

	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x0);

	odm_set_bb_reg(dm, R_0x880, 0x00008000, 0x0);

	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x1);

	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x1);


		ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x988, MASKDWORD, 0x0003FFF0);

	odm_set_bb_reg(dm, R_0x98c, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0x994, MASKDWORD, 0x0003FFF0);

	odm_set_bb_reg(dm, R_0x998, MASKDWORD, 0x00000000);


	odm_set_bb_reg(dm, R_0x988, 0x80000000, 0x1);

	odm_set_bb_reg(dm, R_0x994, 0x80000000, 0x1);

	odm_set_bb_reg(dm, R_0x988, 0x40000000, 0x0);

	odm_set_bb_reg(dm, R_0x994, 0x40000000, 0x0);

	ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x988, 0x00000001, 0x1);	/* pow_da_I_CR */

	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x1);	/* pow_da_Q_CR */

	ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x988, 0x00000002, 0x1);	/* cal_os_I_CR */

	odm_set_bb_reg(dm, R_0x994, 0x00000002, 0x1);	/* cal_os_Q_CR */


		ODM_delay_us(350);
		while ((odm_get_bb_reg(dm, 0xfec, BIT31) != 1) && (t < 1000)) {
			ODM_delay_us(5);
			t += 5;
		}
	odm_set_bb_reg(dm, R_0x988, 0x00000001, 0x0);	/* pow_da_I_CR */
		while ((odm_get_bb_reg(dm, 0xff8, BIT31) != 1) && (t < 1000)) {
			ODM_delay_us(5);
			t += 5;
		}
	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x0);	/* pow_da_Q_CR */
		/* read 0xdf4[9:1]    ADCI offset */
		dai_adci = odm_get_bb_reg(dm, R_0xdf4, 0x000003FE);
		/* read 0xdf4[31:23]  ADCQ offset */
		daq_adcq = odm_get_bb_reg(dm, R_0xdf4, 0xFF800000);
	/*return to normal setting*/
	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x0);

	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x0);

	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x0);


	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x00000003);

	dck_cnt++;

		if ((bt_calibration_similar(adci, dai_adci, BT_DCK_BIT_LEN, BT_DCK_TOLERANCE, true))
			 && (bt_calibration_similar(adcq, daq_adcq, BT_DCK_BIT_LEN, BT_DCK_TOLERANCE, true)))
		{
#if DBG_BT_CALIBRATION
			DBG_8195A("\n\r[BT DCK][%d] DCK OK: ", dck_cnt);
			DBG_8195A("ADCI = %x, ADCQ = %x, DAI_ACDI = %x, DAQ_ADCQ = %x",
				   adci, adcq, dai_adci, daq_adcq);
#endif
			dck_ok = 1;
		}
		else {
#if DBG_BT_CALIBRATION
			DBG_8195A("\n\r***********************[DCK Error]***********************");
			DBG_8195A("\n\r[BT DCK][%d] DCK fail: ", dck_cnt);
			DBG_8195A("ADCI = %x, ADCQ = %x, DAI_ACDI = %x, DAQ_ADCQ = %x",
				   adci, adcq, dai_adci, daq_adcq);
#endif
			dck_ok = 0;
			if (dck_cnt == 100) {
				DBG_8195A("\n\r[BT DCK] DCK Error 100 times. Please reboot.");
				while (1);
			}
		}

	}
#if DBG_BT_CALIBRATION
    DBG_8195A("\n\r[BT DCK] DCK run %d time(s) %s", dck_cnt, dck_ok? "OK":"FAIL");
#endif

u32 reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3;

reg_tmp0 = 0xFE000000&(odm_get_bb_reg(dm, 0xFE4, MASKDWORD));
reg_tmp1 = 0x0000001F&(odm_get_bb_reg(dm, 0xFE8, MASKDWORD));
reg_tmp2 = 0xFE000000&(odm_get_bb_reg(dm, 0xFF0, MASKDWORD));
reg_tmp3 = 0x0000001F&(odm_get_bb_reg(dm, 0xFF4, MASKDWORD));

#if DBG_BT_CALIBRATION
DBG_8195A("DCK result 0x%08x, 0x%08x, 0x%08x, 0x%08x\n", reg_tmp0, reg_tmp1, reg_tmp2, reg_tmp3);
#endif

}
#else
void phy_bt_adc_dck_8721d(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &dm->rf_calibrate_info;

	RF_DBG(dm, DBG_RF_IQK, "[BT DCK]ADC offset mark off!!");

	/*01_DAC_DCK_ADC_offset_mark_off_20181024*/
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xccf000c0);
	odm_set_bb_reg(dm, R_0xc80, MASKDWORD, 0x40000100);
	odm_set_bb_reg(dm, R_0xe6c, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x03a05601);
	odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000800e4);
	odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);
	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x0000008b);

	odm_set_bb_reg(dm, R_0x950, 0x01ff01ff, 0x00);
	odm_set_bb_reg(dm, R_0x818, 0x00400000, 0x0);
	odm_set_bb_reg(dm, R_0x990, 0x40000000, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x1);
	odm_set_bb_reg(dm, R_0x880, 0x00008000, 0x1);
	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x1);
	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x0);

	odm_set_bb_reg(dm, R_0xe28, 0x00000008, 0x1);
	odm_set_bb_reg(dm, R_0xe48, 0x00f00000, 0x1);
	odm_set_bb_reg(dm, R_0xe48, 0x00000002, 0x0);
	odm_set_bb_reg(dm, R_0x908, 0x00000fff, 0x300);

	odm_set_bb_reg(dm, R_0x990, 0x80000000, 0x0);
	ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x990, 0x80000000, 0x1);
}

void phy_bt_dac_dck_8721d(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct dm_rf_calibration_struct *cali_info = &dm->rf_calibrate_info;

	RF_DBG(dm, DBG_RF_IQK, "[BT DCK]BT DAC mark off!!");

	/*04_DAC_DCK_mark_off_20181024*/
	odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0xccf000c0);
	odm_set_bb_reg(dm, R_0xc80, MASKDWORD, 0x40000100);
	odm_set_bb_reg(dm, R_0xe6c, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xe70, MASKDWORD, 0x03c00016);
	odm_set_bb_reg(dm, R_0xc04, MASKDWORD, 0x03a05601);
	odm_set_bb_reg(dm, R_0xc08, MASKDWORD, 0x000800e4);
	odm_set_bb_reg(dm, R_0x874, MASKDWORD, 0x25204000);
	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x0000008b);

	odm_set_bb_reg(dm, R_0x818, 0x00400000, 0x0);
	odm_set_bb_reg(dm, R_0x990, 0x40000000, 0x1);

	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00008000, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x1);
	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x1);
	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x1);

	odm_set_bb_reg(dm, R_0x988, MASKDWORD, 0x0003FFF0);
	odm_set_bb_reg(dm, R_0x98c, MASKDWORD, 0x00000000);
	odm_set_bb_reg(dm, R_0x994, MASKDWORD, 0x0003FFF0);
	odm_set_bb_reg(dm, R_0x998, MASKDWORD, 0x00000000);

	odm_set_bb_reg(dm, R_0x988, 0x80000000, 0x1);
	odm_set_bb_reg(dm, R_0x994, 0x80000000, 0x1);
	odm_set_bb_reg(dm, R_0x988, 0x40000000, 0x0);
	odm_set_bb_reg(dm, R_0x994, 0x40000000, 0x0);

	ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x988, 0x00000001, 0x1);	/* pow_da_I_CR */
	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x1);	/* pow_da_Q_CR */
	ODM_delay_us(1);
	odm_set_bb_reg(dm, R_0x988, 0x00000002, 0x1);	/* cal_os_I_CR */
	odm_set_bb_reg(dm, R_0x994, 0x00000002, 0x1);	/* cal_os_Q_CR */

	odm_set_bb_reg(dm, R_0x988, 0x00000001, 0x0);	/* pow_da_I_CR */
	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x0);	/* pow_da_Q_CR */
	/*return to normal setting*/
	odm_set_bb_reg(dm, R_0x880, 0x00000002, 0x1);
	odm_set_bb_reg(dm, R_0x880, 0x00000010, 0x1);
	odm_set_bb_reg(dm, R_0x880, 0x00010000, 0x0);
	odm_set_bb_reg(dm, R_0x880, 0x00020000, 0x0);
	odm_set_bb_reg(dm, R_0x888, 0x00000020, 0x0);

	odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0x00000003);
}

#endif

void phy_bt_dck_write_8721d(struct dm_struct *dm,
			    u8 q_dck,
			    u8 i_dck)
{
	odm_set_bb_reg(dm, R_0x998, 0x00000001, 0x1);
	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x1);

	odm_set_bb_reg(dm, R_0x98c, 0x01000000, 1);
	odm_set_bb_reg(dm, R_0x998, 0x01000000, 1);
	odm_set_bb_reg(dm, R_0x98c, 0x00200000, 1);
	odm_set_bb_reg(dm, R_0x998, 0x00200000, 1);
	odm_set_bb_reg(dm, R_0x98c, 0x02000000, 1);
	odm_set_bb_reg(dm, R_0x998, 0x02000000, 1);

	odm_set_bb_reg(dm, R_0x98c, 0x00007c00, 0x1f);
	odm_set_bb_reg(dm, R_0x98c, 0x00000200, 0x1);

	odm_set_bb_reg(dm, R_0x98c, 0x001F8000, i_dck);

	odm_set_bb_reg(dm, R_0x998, 0x00007c00, 0x1f);
	odm_set_bb_reg(dm, R_0x998, 0x00000200, 0x1);

	odm_set_bb_reg(dm, R_0x998, 0x001F8000, q_dck);

	odm_set_bb_reg(dm, R_0x998, 0x00000001, 0x0);
	odm_set_bb_reg(dm, R_0x994, 0x00000001, 0x0);
}

u32 phy_btiq_calibrate_8721d(struct dm_struct *dm)
{
	u32 result;
	u32 bt_rf_backup[BTIQK_RF_REG_NUM_8721D][1];
	u32 bt_bb_backup[BTIQK_BB_REG_NUM_8721D];
	u32 bt_mac_backup[BTIQK_MAC_REG_NUM_8721D];

	u32 BTIQK_BB_REG_8721D[] = { R_0xc80, R_0x88c, R_0xe6c, R_0xe70,
		R_0xc04, R_0xc08, R_0x874, R_0x888, R_0xe28, R_0xe30, R_0xe34,
		R_0xe38, R_0xe3c, R_0xe40, R_0xe44,R_0xb00 };
	u32 BTIQK_RF_REG_8721D[][1] = { RF_0x0, RF_0x1, RF_0x18, RF_0xdf,
		RF_0xdd, RF_0xee };
	u8 i = 0;
	u8 pta_poll = 100;
	RF_DBG(dm, DBG_RF_IQK, "[BTIQK]==========IQK strat!!!!!==========\n");

	_btiqk_backup_bb_registers_8721d(dm, BTIQK_BB_REG_8721D, bt_bb_backup,
					 BTIQK_BB_REG_NUM_8721D);
	_btiqk_backup_rf_registers_8721d(dm, (uint32_t *)BTIQK_RF_REG_8721D,
					 bt_rf_backup, BTIQK_RF_REG_NUM_8721D);

	/*Switch to wifi temporary, will move to wifi driver later*/
	/*Set gnt bt to 0 to force to wifi*/
	while(!btc_set_gnt_bt_atmoic_8721d(1, 0) && pta_poll){
		pta_poll--;
		vTaskDelay(10);
	}

	/*WIFI DPK&&IQK OFF*/
	odm_set_bb_reg(dm, R_0xb00, 0x80, 0);
	
	ODM_delay_ms(BTIQK_DELAY_TIME_8721D);

#if DBG_BT_CALIBRATION
u32 reg_tmp;

reg_tmp = odm_get_bb_reg(dm, 0x88C, MASKDWORD);
DBG_8195A("original 0x88C = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xC80, MASKDWORD);
DBG_8195A("original 0xC80 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe6c, MASKDWORD);
DBG_8195A("original 0xe6c = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0x670, MASKDWORD);
DBG_8195A("original 0x670 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xc04, MASKDWORD);
DBG_8195A("original 0xc04 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xc08, MASKDWORD);
DBG_8195A("original 0xc08 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0x874, MASKDWORD);
DBG_8195A("original 0x874 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0x888, MASKDWORD);
DBG_8195A("original 0x888 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe28, MASKDWORD);
DBG_8195A("original 0xe28 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, RF_0xdf, MASKDWORD);
DBG_8195A("original RF_0xdf = 0x%08x\n", reg_tmp);


reg_tmp = odm_get_bb_reg(dm, 0xe30, MASKDWORD);
DBG_8195A("original 0xe30 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe34, MASKDWORD);
DBG_8195A("original 0xe34 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe38, MASKDWORD);
DBG_8195A("original 0xe38 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe3C, MASKDWORD);
DBG_8195A("original 0xe3C = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe40, MASKDWORD);
DBG_8195A("original 0xe40 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe44, MASKDWORD);
DBG_8195A("original 0xe44 = 0x%08x\n", reg_tmp);


reg_tmp = odm_get_bb_reg(dm, 0xe48, MASKDWORD);
DBG_8195A("original 0xe48 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe4c, MASKDWORD);
DBG_8195A("original 0xe4c = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe50, MASKDWORD);
DBG_8195A("original 0xe50 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_bb_reg(dm, 0xe54, MASKDWORD);
DBG_8195A("original 0xe54 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, RF_0xee, MASKDWORD);
DBG_8195A("original RF_0xee = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, RF_0xdd, MASKDWORD);
DBG_8195A("original RF_0xdd = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, 0x06, MASKDWORD);
DBG_8195A("original rf 0x06 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, RF_0x18, MASKDWORD);
DBG_8195A("original RF_0x18 = 0x%08x\n", reg_tmp);

reg_tmp = odm_get_rf_reg(dm, RF_PATH_A, 0x01, MASKDWORD);
DBG_8195A("original rf 0x01 = 0x%08x\n", reg_tmp);
#endif

	result = _btiqk_return_result_8721d(dm);
	_btiqk_reload_rf_registers_8721d(dm, (uint32_t *)BTIQK_RF_REG_8721D,
					 bt_rf_backup, BTIQK_RF_REG_NUM_8721D);
	_btiqk_reload_bb_registers_8721d(dm, BTIQK_BB_REG_8721D, bt_bb_backup,
					 BTIQK_BB_REG_NUM_8721D);
	
	/*Atmoic operation done*/
	btc_set_gnt_bt_atmoic_8721d(0, 0);
	
	RF_DBG(dm, DBG_RF_IQK, "[BTIQK]delay 10ms!!!\n");
	ODM_delay_ms(BTIQK_DELAY_TIME_8721D);

	RF_DBG(dm, DBG_RF_IQK, "[BTIQK]==========IQK end!!!!!==========\n");
	return result;
}

void phy_bt_flatk_8721d(struct dm_struct *dm, u16 txgain_flatk)
{
	u8 txgain_flak_use;
	u8 txgaink_flag ;
	u8 temp = 0;

	txgaink_flag = (signed_value_conversion((txgain_flatk>>0&0x0f), 4) <= 2) &&
		(signed_value_conversion((txgain_flatk>>4&0x0f), 4) <= 2) &&
		(signed_value_conversion((txgain_flatk>>8&0x0f), 4) <= 2) &&
		(signed_value_conversion((txgain_flatk>>12&0x0f), 4) <= 2);

	odm_set_rf_reg(dm, RF_PATH_A, RF_0xed, 0x40, 0x1);
	for(int i = 0; i<4; i++) {
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, BIT(12)|BIT(13), i);
		txgain_flak_use = (txgain_flatk>>(i*4)) & 0x0f ;
		if(txgaink_flag)
			txgain_flak_use +=2;
		if(txgain_flak_use == 0x07)
			txgain_flak_use =0x06;
		temp = (((txgain_flak_use>>1) + (txgain_flak_use&0x01)) & 0x07);
		if(temp == 0x07)
			odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, BIT2|BIT1|BIT0, 0x05);
		else if(temp == 0x05)
			odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, BIT2|BIT1|BIT0, 0x07);
		else if(temp == 0x04)
			odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, BIT2|BIT1|BIT0, 0x07);
		else
			odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, BIT2|BIT1|BIT0, temp);
	}
	odm_set_rf_reg(dm, RF_PATH_A, RF_0xed, 0x40, 0x0);
}



u32 bt_iqk_8721d(BT_Cali_TypeDef *cal_data, BOOLEAN store)
{
	struct net_device *dev;
	_adapter *padapter;
	PHAL_DATA_TYPE pHalData;
	u32 result;
	u32 ret = _SUCCESS;

	dev = rltk_wlan_info[0].dev;
	if(dev) {
		padapter = (_adapter *)rtw_netdev_priv(dev);
		pHalData = GET_HAL_DATA(padapter);
		LeaveAllPowerSaveMode(padapter);

		result = phy_btiq_calibrate_8721d(&(pHalData->odmpriv));

		/* get calibration data */
		if(cal_data) {
			cal_data->IQK_xx = (result >> 22) & 0x3FF;
			cal_data->IQK_yy = (result >> 12) & 0x3FF;
			cal_data->IDAC = (result >> 6) & 0x3F;
			cal_data->QDAC = (result >> 0) & 0x3F;
		}

	} else {
		ret =_FAIL;
		DBG_ERR("netif is DOWN");
	}
return ret;
}

u32 bt_adda_dck_8721d(void)
{
	struct net_device *dev;
	_adapter *padapter;
	PHAL_DATA_TYPE pHalData;
	u32 ret = _SUCCESS;

	dev = rltk_wlan_info[0].dev;
	if(dev) {
		padapter = (_adapter *)rtw_netdev_priv(dev);
		if (Hal_BT_Is_Supported(padapter) == _FALSE)
			HAL_READ8(0,0);	/* creat a hardfault to limit some comusters use bt */
		pHalData = GET_HAL_DATA(padapter);
		LeaveAllPowerSaveMode(padapter);

		phy_bt_adda_dck_8721d(&(pHalData->odmpriv));

	} else {
		ret = _FAIL;
		DBG_ERR("netif is DOWN");
	}

	return ret;
}

u32 bt_lok_write(u32 idac, u32 qdac)
{
	struct net_device *dev;
	_adapter *padapter;
	PHAL_DATA_TYPE pHalData;
	u32 ret = _SUCCESS;

	dev = rltk_wlan_info[0].dev;
	if(dev) {
		padapter = (_adapter *)rtw_netdev_priv(dev);
		if (Hal_BT_Is_Supported(padapter) == _FALSE)
			HAL_READ8(0,0);	/* creat a hardfault to limit some comusters use bt */
		pHalData = GET_HAL_DATA(padapter);
		LeaveAllPowerSaveMode(padapter);

		phy_bt_lok_write_8721d(&(pHalData->odmpriv),idac,qdac);
	} else {
		ret = _FAIL;
		DBG_ERR("netif is DOWN");
	}

	return ret;
}

u32 bt_dck_write(u8 q_dck, u8 i_dck)
{
	struct net_device *dev;
	_adapter *padapter;
	PHAL_DATA_TYPE pHalData;
	u32 ret = _SUCCESS;
		
	dev = rltk_wlan_info[0].dev;
	if(dev) {
		padapter = (_adapter *)rtw_netdev_priv(dev);
		if (Hal_BT_Is_Supported(padapter) == _FALSE)
			HAL_READ8(0,0);	/* creat a hardfault to limit some comusters use bt */
		pHalData = GET_HAL_DATA(padapter);
		LeaveAllPowerSaveMode(padapter);

		phy_bt_dck_write_8721d(&(pHalData->odmpriv),q_dck,i_dck);
	} else {
		ret = _FAIL;
		DBG_ERR("netif is DOWN");
	}

	return ret;
}

u32 bt_flatk_8721d(u16 txgain_flatk)
{
	struct net_device *dev;
	_adapter *padapter;
	PHAL_DATA_TYPE pHalData;
	u32 ret = _SUCCESS;

	dev = rltk_wlan_info[0].dev;
	if(dev) {
		padapter = (_adapter *)rtw_netdev_priv(dev);
		if (Hal_BT_Is_Supported(padapter) == _FALSE)
			HAL_READ8(0,0);	/* creat a hardfault to limit some comusters use bt */
		pHalData = GET_HAL_DATA(padapter);
		LeaveAllPowerSaveMode(padapter);

		phy_bt_flatk_8721d(&(pHalData->odmpriv), txgain_flatk);

	} else {
		ret = _FAIL;
		DBG_ERR("netif is DOWN");
	}

	return ret;
}
