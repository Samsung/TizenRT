#ifndef __WIFIFW_CSIREPORT_RAM_H__
#define __WIFIFW_CSIREPORT_RAM_H__
/*--------------------Define MACRO--------------------------------------*/
#define RTY_LMT_SW_CSINULLDATA					3
#define RTY_LMT_HW_CSINULLDATA					0

/*------------------------------Function declaration--------------------------*/
extern void HALBB_FW_BBReset_8720E(void);
extern BOOLEAN CheckCSITxNullDone_8720E(void);
extern BOOLEAN CheckNoCSIClientEnable_8720E(void);
extern void CheckNextCSIClientTxnull_8720E(u8 clientnum, u8 powerbit);
extern void CheckCSIClientTxnullRty_8720E(u8 clientnum, u8 powerbit);
extern void H2CHDL_Set_CSI_TXNULL_8720E(u8 *pbuf);
extern void CSIEnableClientParameter_8720E(u8 *pbuf, u8 clientnum);
extern void CSIDisableClientParameter_8720E(u8 clientnum);
extern void SetPeriodTxNullAddrRate_8720E(u8 clientnum, u16 offset);
extern void IssueCSITXNull_8720E(u8 clientnum);

#endif  /* #ifndef   __WIFIFW_TXREPORT_RAM_H__ */
