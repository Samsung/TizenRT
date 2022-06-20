#ifndef __WIFIFW_PHYDM_OFLOAD_RAM_H__
#define __WIFIFW_PHYDM_OFLOAD_RAM_H__

#define	DIG_MAX_LPS			0x3e
#define	DIG_MIN_LPS			0x20

#define	RSSI_TABLE_SIZE			4
#define	RSSI_DIFF_THRESHOLD			30

typedef struct _RSSI_PARM_ {
	u8		abs_rssi_temp[RSSI_TABLE_SIZE];
	u8		abs_rssi_average;
	u16		abs_rssi_sum;
} RSSI_PARM;

extern void DIGByRSSILps_8720E(u16 phystsofst);

#endif
