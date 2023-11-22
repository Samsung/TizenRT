/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _AMEBA_BOOTCFG_H_
#define _AMEBA_BOOTCFG_H_

extern MMU_ConfDef Flash_MMU_Config[];
extern RCC_ConfDef RCC_Config[];
extern CACHETCM_TypeDef CACHETCM_Def[];
extern GRST_ConfDef GRST_Config[];
extern RSIP_OTFDef RSIP_OTF_Config[];
extern u8 Force_OTA1_GPIO;
extern u8 Boot_Log_En;
extern u8 Boot_PSRAM_APM;
extern FuncPtr FwCheckCallback;
extern FuncPtr OTASelectHook;
extern u32 Cert_PKHash_OTP_ADDR;
extern u32 Boot_TCM_Size;
extern CACHEWRR_TypeDef CACHEWRR_Def[];
extern u8 Boot_AP_Enbale;
extern u8 Boot_MemSwr_Only;
extern SocClk_Info_TypeDef SocClk_Info[];
extern u8 Boot_Agg_En;

#endif
