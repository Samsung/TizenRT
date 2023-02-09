#ifndef __WIFIFW_PHYDM_OFLOAD_RAM_H__
#define __WIFIFW_PHYDM_OFLOAD_RAM_H__

#define	DIG_MAX_LPS			0x3e
#define	DIG_MIN_LPS			0x20

#define	RSSI_TABLE_SIZE			4
#define	RSSI_DIFF_THRESHOLD			30

#define RFREG_MASK		0xfffff
#define INVALID_RF_DATA 0xffffffff

/*--------------------Define Enum---------------------------------------*/
enum rtw_dv_sel {
	DAV,
	DDV,
};

enum rf_path {
	RF_PATH_A = 0,
	RF_PATH_B = 1,
	RF_PATH_C = 2,
	RF_PATH_D = 3,
	RF_PATH_AB,
	RF_PATH_AC,
	RF_PATH_AD,
	RF_PATH_BC,
	RF_PATH_BD,
	RF_PATH_CD,
	RF_PATH_ABC,
	RF_PATH_ABD,
	RF_PATH_ACD,
	RF_PATH_BCD,
	RF_PATH_ABCD,
};

typedef struct _RSSI_PARM_ {
	u8		abs_rssi_temp[RSSI_TABLE_SIZE];
	u8		abs_rssi_average;
	u16		abs_rssi_sum;
} RSSI_PARM;

extern void DIGByRSSILps_8720E(u16 phystsofst);
extern u32 halbb_fw_read_rf_reg_8720e(enum rf_path path, u32 reg_addr, u32 bit_mask);
extern bool halbb_fw_write_rf_reg_8720e(enum rf_path path, u32 reg_addr, u32 bit_mask, u32 data);
extern u32 halbb_fw_get_bb_reg_8720e(u32 addr, u32 mask);
extern void halbb_fw_set_bb_reg_8720e(u32 addr, u32 mask, u32 val);

#endif
