#ifndef __WIFIFW_HIOE_RAM_H__
#define __WIFIFW_HIOE_RAM_H__
/*--------------------Define --------------------------------------------*/

#define HIOE_BACKUP		1
#define HIOE_RESTORE	0
#define HIOECmdFileEndFlg	(0x00000000)
#define HIOECmdFileRsValEndFlg	(0xFF)
#define HIOE_ACCESS_BYTE_MODE	(0x00000000)
#define HIOE_ACCESS_DWORD_MODE	(0x00000001)
#define HIOE_DELAY_MODE	(0x00000003)
#define HIOE_MODE_MASK	(0x00000003)
#define HIOE_BYTE_SEL_MASK	(0x0000000F)
#define HIOE_BYTE_SEL_MASK_SHT	(24)
#define HIOE_REG_ADDR_MASK	(0x003FFFFF)
#define HIOE_REG_ADDR_MASK_SHT	(2)
#define HIOE_DATA_BYTE0_MASK	(0xFF000000) //bit 24~31
#define HIOE_DATA_BYTE123_MASK	(0x00FFFFFF) //bit 32~55
#define HIOECMD_SIZE_WM		(8)
#define HIOECMD_SIZE_BM		(4)
#define HIOECMD_SIZE_DM		(4)
#define HIOECMD_ARfcRegBaseAddr		(0x17000)
#define HIOECMD_DRfcRegBaseAddr		(0x1E000)

#define HIOE_DM_INITVALTAB_TYPE(x) 	((x)|BIT(4))
#define HIOE_CMDFILE_SIZE(x)		((sizeof(x) - 4)*2)

#define HIOECMD_WM0(reg_addr)		\
	(HIOE_ACCESS_DWORD_MODE | ((((u32)(reg_addr)) & HIOE_REG_ADDR_MASK)<<HIOE_REG_ADDR_MASK_SHT))

#define HIOECMD_WM1(byte_sel)		(((u32)(byte_sel) & HIOE_BYTE_SEL_MASK)<<HIOE_BYTE_SEL_MASK_SHT)


#define HIOECMD_BM(reg_addr)		\
	(HIOE_ACCESS_BYTE_MODE | ((((u32)(reg_addr)) & HIOE_REG_ADDR_MASK)<<HIOE_REG_ADDR_MASK_SHT))

#define HIOECMD_DM(delay_time)		\
	(HIOE_DELAY_MODE | ((((u32)(delay_time)) & HIOE_REG_ADDR_MASK)<<HIOE_REG_ADDR_MASK_SHT))
#define HIOECMD_GetMode(x)	((x)&HIOE_MODE_MASK)

typedef struct HIOECmdFileRSValue {
	/*bytemak 0: byte cmd mode, else value mean the real dword cmd mode byte mask*/
	u8 bytemask;
	u32 RSValue;
} HIOECmdFileRSVal;
/*--------------------Function declaration---------------------------------*/
extern u32 PG_BackupRestoreREGTable[];
extern void HIOESetSingleCmdValWM_8720E(u32 CmdAddrIn8byte, u32 value);
extern void HIOESetSingleCmdValBM_8720E(u32 cmdaddrIn4byte, u8 value);
extern BOOLEAN HIOECmdInitValTypeChk_8720E(u8 HIOECmdType, u8 InitValByteMask);
extern BOOLEAN HIOEChkChanelIdle_8720E(u32 chn_ctrl_reg, u32 backup_req_bit, u32 restore_reg_bit);
extern void HIOEInitCmdFileToTXFF_8720E(u32 *HIOECmdFileTab, HIOECmdFileRSVal *HIOECmdFileRsValTab, u32 TXFF_Offset);
extern void HIOESetHIOEConfigFile_8720E(void);
extern void HIOEBackupRestore_8720E(bool mode);
extern void HIOECHKHigh8kTXFFPGUseOverlap_8720E(void);
#endif
