/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

//IROM (rx) : 			ORIGIN = 0x10100000, LENGTH = 0x10000	/* ROM: 64k */
//IROM_NS (rx) : 			ORIGIN = 0x10110000, LENGTH = 0x38000	/* ROM: 224k */
//DROM (rx) : 			ORIGIN = 0x101C0000, LENGTH = 0x8000	/* ROM: 32k */
//DROM_NS (rx) : 			ORIGIN = 0x101C8000, LENGTH = 0x10000	/* ROM: 64k */
//ROMBSS_RAM (rw)  : 		ORIGIN = 0x10000000, LENGTH = 0x2000	/* ROM BSS RAM: 8K */
//BOOTLOADER_RAM (rwx)  : 	ORIGIN = 0x10002000, LENGTH = 0x3000	/* BOOT Loader RAM: 12K */
//BD_RAM (rwx)  : 		ORIGIN = 0x10005000, LENGTH = 0x78000	/* MAIN RAM: 480 */
//PRINT_RAM (wx)  : 		ORIGIN = 0x1007D000, LENGTH = 0x1000	/* PRINTF BUFFER RAM: 4k */
//MSP_RAM (wx)  : 		ORIGIN = 0x1007E000, LENGTH = 0x2000	/* MSP & MSP_S RAM: 8k */

/*
// <e>Setup behaviour of Sleep and Exception Handling
*/
#define SCB_CSR_AIRCR_INIT  1

/*
//   <o> Deep Sleep can be enabled by
//     <0=>Secure and Non-Secure state
//     <1=>Secure state only
//   <i> Value for SCB->CSR register bit DEEPSLEEPS
*/
#define SCB_CSR_DEEPSLEEPS_VAL  1

/*
//   <o>System reset request accessible from
//     <0=> Secure and Non-Secure state
//     <1=> Secure state only
//   <i> Value for SCB->AIRCR register bit SYSRESETREQS
*/
#define SCB_AIRCR_SYSRESETREQS_VAL  1

/*
//   <o>Priority of Non-Secure exceptions is
//     <0=> Not altered
//     <1=> Lowered to 0x80-0xFF
//   <i> Value for SCB->AIRCR register bit PRIS
*/
#define SCB_AIRCR_PRIS_VAL      1

/*
//   <o>BusFault, HardFault, and NMI target
//     <0=> Secure state
//     <1=> Non-Secure state
//   <i> Value for SCB->AIRCR register bit BFHFNMINS
//	Notice: HardFault only behaves as a banked exception if AIRCR.BFHFNMINS is 1, otherwise it behaves as an
//	unbanked exception targeting Secure state.
*/
#define SCB_AIRCR_BFHFNMINS_VAL 1

/* this table is configured by usrcfg */
extern const TZ_CFG_TypeDef sau_config[];
extern const TZ_CFG_TypeDef mpc1_config[];
extern const TZ_CFG_TypeDef mpc2_config[];
extern const TZ_CFG_TypeDef mpc3_config[];

extern const AF_CFG_TypeDef af0_config[];
extern const AF_CFG_TypeDef af1_config[];
extern const AF_CFG_TypeDef af2_config[];
extern const AF_CFG_TypeDef af3_config[];
extern const AF_CFG_TypeDef af4_config[];
extern const AF_CFG_TypeDef af5_config[];

/**
  *  @brief Config all SAU & IDAU based on tz_config table.
  *  @retval None
  */
BOOT_RAM_TEXT_SECTION
void BOOT_RAM_TZCfg(void)
{
	int idex = 0;
	int mpcIdx = 0;
	MPC_TypeDef *MPCArray[3] = {KM4_MPC1, KM4_MPC2, KM4_MPC3};
	const TZ_CFG_TypeDef *CfgArray[3] = {mpc1_config, mpc2_config, mpc3_config};
	MPC_TypeDef *MPC;
	const TZ_CFG_TypeDef *Config;

	/* Configure MPC */
	for (mpcIdx = 0; mpcIdx < 3; mpcIdx++) {
		MPC = MPCArray[mpcIdx];
		Config = CfgArray[mpcIdx];

		for (idex = 0; idex < MPC_ENTRYS_NUM; idex++) {
			/*  Check if search to end */
			if (Config[idex].Start == 0xFFFFFFFF) {
				break;
			}

			/* set MPC */
			MPC->ENTRY[idex].IDAU_BARx = Config[idex].Start;
			MPC->ENTRY[idex].IDAU_LARx = Config[idex].End;
			MPC->IDAU_CTRL |= BIT(idex);
		}
		MPC->IDAU_LOCK = 1;

		//DBG_8195A("MPC: %08x, %08x\n", (u32)MPC, MPC->IDAU_CTRL);
		for (idex = 0; idex < MPC_ENTRYS_NUM; idex++) {
			//DBG_8195A("IDAU_BAR%d:[%08x:%08x]\n", idex, MPC->ENTRY[idex].IDAU_BARx, MPC->ENTRY[idex].IDAU_LARx);
		}
	}

	/* Configure PPC, set all peripherals to Non-secure, except RXI300_hs */
	PPC_TypeDef *PPC = KM4_PPC; //km0, modify later
	PPC_TypeDef *PPC1 = KM4_PPC2;

	PPC->PPC_REG0 = 0xFFFFFFFF;
	PPC1->PPC_REG0 = 0xFFFFFFFF & ~BIT(6);

	/* Configure SAU */
	for (idex = 0; idex < SAU_ENTRYS_NUM; idex++) {
		/*  Check if search to end */
		if (sau_config[idex].Start == 0xFFFFFFFF) {
			break;
		}

		/* set SAU */
		SAU->RNR  = (idex & SAU_RNR_REGION_Msk);
		SAU->RBAR = (sau_config[idex].Start & SAU_RBAR_BADDR_Msk);
		SAU->RLAR = (sau_config[idex].End & SAU_RLAR_LADDR_Msk) | \
					((sau_config[idex].NSC << SAU_RLAR_NSC_Pos)  & SAU_RLAR_NSC_Msk)   | \
					ENABLE << SAU_RLAR_ENABLE_Pos;
	}

	for (idex = 0; idex < SAU_ENTRYS_NUM; idex++) {
		//SAU->RNR = idex;
		//DBG_8195A("SAU_BAR%d:[%08x:%08x]\n", idex, SAU->RBAR, SAU->RLAR);
	}

	SAU->CTRL = ((SAU_INIT_CTRL_ENABLE << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk) |
				((SAU_INIT_CTRL_ALLNS  << SAU_CTRL_ALLNS_Pos)  & SAU_CTRL_ALLNS_Msk)   ;

	if ((HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG) & LSYS_BIT_BOOT_WAKE_FROM_PS_HS) == 0) {
		/* Clean and invalidate DCache before SAU configurations, because secure cache line hit
			by non-secure transaction would cause CPU hang. */
		SCB_CleanInvalidateDCache();
	}

	//DBG_8195A("SAU_CTRL:%x\n", SAU->CTRL);

	SCB->SCR   = (SCB->SCR   & ~(SCB_SCR_SLEEPDEEPS_Msk)) |
				 ((SCB_CSR_DEEPSLEEPS_VAL     << SCB_SCR_SLEEPDEEPS_Pos)     & SCB_SCR_SLEEPDEEPS_Msk);

	SCB->AIRCR = (SCB->AIRCR &
				  ~(SCB_AIRCR_VECTKEY_Msk   | SCB_AIRCR_SYSRESETREQS_Msk | SCB_AIRCR_BFHFNMINS_Msk |  SCB_AIRCR_PRIS_Msk)) |
				 ((0x05FAU                    << SCB_AIRCR_VECTKEY_Pos)      & SCB_AIRCR_VECTKEY_Msk)      |
				 //((SCB_AIRCR_SYSRESETREQS_VAL << SCB_AIRCR_SYSRESETREQS_Pos) & SCB_AIRCR_SYSRESETREQS_Msk) | /* reset both secure and non-secure */
				 ((SCB_AIRCR_PRIS_VAL         << SCB_AIRCR_PRIS_Pos)         & SCB_AIRCR_PRIS_Msk)         |
				 ((SCB_AIRCR_BFHFNMINS_VAL    << SCB_AIRCR_BFHFNMINS_Pos)    & SCB_AIRCR_BFHFNMINS_Msk);

	/* <0=> Secure state <1=> Non-Secure state */
	NVIC->ITNS[0] = 0xFFFFFFFF; /* IRQ 0~31: Non-Secure state */
	NVIC->ITNS[1] = 0xFFFFFFFF; /* IRQ 32~63: Non-Secure state */
	NVIC->ITNS[2] = 0xFFFFFFFF; /* IRQ 64~95: Non-Secure state */

}

/**
  *  @brief Config all address filter based on af_config table.
  *  @retval None
  */
BOOT_RAM_TEXT_SECTION
void BOOT_RAM_AFCfg(void)
{
	int addrfilterIdx = 0;
	int idex = 0;

	ADDRFILTER_TypeDef *AFArray[6] = {KM4_AF0, KM4_AF1, KM4_AF2, KM4_AF3, KM4_AF4, KM4_AF5};
	const AF_CFG_TypeDef *AFCfgArray[6] = {af0_config, af1_config, af2_config, af3_config, af4_config, af5_config};
	ADDRFILTER_TypeDef *ADDRFILTER;
	const AF_CFG_TypeDef *AFConfig;

	/* Configure address filter */
	for (addrfilterIdx = 0; addrfilterIdx < 6; addrfilterIdx++) {
		ADDRFILTER = AFArray[addrfilterIdx];
		AFConfig = AFCfgArray[addrfilterIdx];

		for (idex = 0; idex < AF_ENTRYS_NUM; idex++) {
			/*  Check if search to end */
			if (AFConfig[idex].Start == 0xFFFFFFFF) {
				break;
			}

			/* set Address Filter */
			ADDRFILTER->ENTRY[idex].IDAU_BARx = AFConfig[idex].Start;
			ADDRFILTER->ENTRY[idex].IDAU_LARx = AFConfig[idex].End;
			ADDRFILTER->IDAU_CTRL |= AFConfig[idex].AccessC << (idex * 2);
		}
		ADDRFILTER->IDAU_LOCK = 1;

		//DBG_8195A("AF: %08x, %08x, %08x\n", (u32)ADDRFILTER, ADDRFILTER->IDAU_CTRL, ADDRFILTER->IDAU_LOCK);
		//for (idex = 0; idex < MPC_ENTRYS_NUM; idex++) {
		//DBG_8195A("IDAU_BAR%d:[%08x:%08x]\n", idex, ADDRFILTER->ENTRY[idex].IDAU_BARx, ADDRFILTER->ENTRY[idex].IDAU_LARx);
		//}
	}

}
