#ifndef __WIFIFW_HIOE_RAM_H__
#define __WIFIFW_HIOE_RAM_H__
/*--------------------Define --------------------------------------------*/
#define SET_HIOE_BACKUP		1
#define SET_HIOE_RESTORE	1
#define HIOE_BACKUP		1
#define HIOE_RESTORE	0
#define HIOECmdFileEndFlg	0x00000000
#define HIOECmdFileRsValEndFlg	0xFF
#define HIOE_ACCESS_BYTE_MODE	0x00000000
#define HIOE_ACCESS_DWORD_MODE	0x00000001
#define HIOE_DELAY_MODE	0x00000003
#define HIOE_MODE_MASK	0x00000003
#define HIOE_BYTE_SEL_MASK	0x0000000F
#define HIOE_REG_ADDR_MASK	0x003FFFFF
#define HIOE_DATA_BYTE0_MASK	0xFF00000000
#define HIOE_DATA_BYTE123_MASK	0x00FFFFFF

#define HIOECMD_WM0(reg_addr)		\
	(HIOE_ACCESS_DWORD_MODE | ((((u32)(reg_addr)) & HIOE_REG_ADDR_MASK)<<2))

#define HIOECMD_WM1(byte_sel)		(((u32)(byte_sel) & HIOE_BYTE_SEL_MASK)<<24)


#define HIOECMD_BM(reg_addr)		\
	(HIOE_ACCESS_BYTE_MODE | ((((u32)(reg_addr)) & HIOE_REG_ADDR_MASK)<<2))

#define HIOECMD_DM(delay_time)		\
	(HIOE_DELAY_MODE | ((((u32)(delay_time)) & HIOE_REG_ADDR_MASK)<<2))
#define HIOECMD_GetMode(x)	((x)&HIOE_MODE_MASK)

typedef struct HIOECmdFileRSValue {
	/*bytemak 0: byte cmd mode, else value mean the real dword cmd mode byte mask*/
	u8 bytemask;
	u32 RSValue;
} HIOECmdFileRSVal;
/*--------------------Function declaration---------------------------------*/
extern u32 PG_BackupRestoreREGTable[];
extern void SetHIOEConfigFile_8720E(void);
extern void HIOEBackupRestore_8720E(bool mode);
extern void CHKHigh8kTXFFPGUseOverlap_8720E(void);
#endif
