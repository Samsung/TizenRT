#ifndef _WIFIFW_LPS_OFFLOAD_RAM_H_
#define _WIFIFW_LPS_OFFLOAD_RAM_H_

/*--------------------Define --------------------------------------------*/
#define CmdFileType_Delay	0
#define CmdFileType_RealOnOff	1
#define LPSOfld_KeepCurrentVal	(0xFF)
#define LPSOfld_WaitFCSToms	(10)
#define LPSOfld_HIOEOnClrIntCmdNum	(2) //unit 8 bytes
#define LPSOfld_BCNTO	(10000)//us
#define LPSOfld_TSFT_DIFF_THRES	(1000)//us
#define KeyCAMHIOEEntrySize (16)

/*------------------------------Function declaration--------------------------*/
extern void LPSOfld_HIOECmdFileInit_8720E(void);
extern void LPSOfld_RxbcnModeCmdFileValInit_8720E(void);
extern void LPSOfld_ExitRxbcnModeCmdFileValInit_8720E(void);
extern void LPSOfld_SwtHIOECmdFile_8720E(BOOLEAN OnHIOECmdFileType, BOOLEAN OffHIOECmdFileType);
extern void LPSOfld_IMRSetting_8720E(BOOLEAN HWCtrlTRXOnOff);
extern void LPSOfld_SetBcnErlyandTimeout_8720E(u8 bcnearlyitvlms, u8 bcnearlyitvl128us, u8 bcnearlyitvl32us, u32 bcntimeout);
extern void LPSOfld_FuncInit_8720E(void);
extern void LPSOfld_PGResume_8720E(void);
extern void LPSOfld_HWCtrlTRXOnOff_8720E(BOOLEAN HWCtrlTRXOnOff);
extern void LPSOfld_PSRxBcnProcess_8720E(void);

#endif
