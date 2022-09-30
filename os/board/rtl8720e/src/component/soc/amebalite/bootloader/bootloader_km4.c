/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "ameba_secure_boot.h"
#include "bootloader_km4.h"

#define DSP_BOOT_EN		0
#define VAD_NS_EN		0

typedef struct {
	u32 NVICbackup_HP[6];
	u32 SCBVTORbackup_HP;
	u8 NVICIPbackup_HP[MAX_PERIPHERAL_IRQ_NUM];
} CPU_S_BackUp_TypeDef;

CPU_S_BackUp_TypeDef PMC_S_BK;

#if defined ( __ICCARM__ )
#pragma section=".ram.bss"
#pragma section=".rom.bss"
#pragma section=".ram.start.table"
#pragma section=".ram_image1.bss"
#pragma section=".ram_image2.entry"

BOOT_RAM_RODATA_SECTION u8 *__image2_entry_func__ = 0;
BOOT_RAM_RODATA_SECTION u8 *__image1_bss_start__ = 0;
BOOT_RAM_RODATA_SECTION u8 *__image1_bss_end__ = 0;
#endif

BOOT_RAM_TEXT_SECTION
PRAM_START_FUNCTION BOOT_SectionInit(void)
{
#if defined ( __ICCARM__ )
	// only need __bss_start__, __bss_end__
	__image2_entry_func__		= (u8 *)__section_begin(".ram_image2.entry");
	__image1_bss_start__		= (u8 *)__section_begin(".ram_image1.bss");
	__image1_bss_end__			= (u8 *)__section_end(".ram_image1.bss");
#endif
	return (PRAM_START_FUNCTION)__image2_entry_func__;
}

BOOT_RAM_TEXT_SECTION
__attribute__((noinline)) void BOOT_NsStart(u32 Addr)
{
	/* clear stack */
	u32 stack_top = MSPLIM_RAM_HP;
	u32 stack_end = MSP_RAM_HP + 4;
	u32 stack_mid = (stack_top + stack_end) / 2;

	__set_MSP(stack_mid);

	_memset((void *)stack_mid, 0, stack_end - stack_mid);
	DCache_CleanInvalidate(stack_mid, stack_end - stack_mid);

	__set_MSP(stack_end);

	_memset((void *)stack_top, 0, stack_mid - stack_top);
	DCache_CleanInvalidate(stack_top, stack_mid - stack_top);

	/* jump to ns world */
	nsfunc *fp = cmse_nsfptr_create(Addr);
	fp();
}

/* open some always on functions in this function */
BOOT_RAM_TEXT_SECTION
void BOOT_RccConfig(void)
{
	u32 idx = 0;
	u32 TempVal = 0;

	u32 FenReg[4] = {REG_LSYS_FEN_GRP0, REG_LSYS_FEN_GRP1, NULL, REG_AON_FEN};
	u32 FenSet[4] = {0};
	u32 FuncRegIndx = 0;

	u32 CenReg[4] = {REG_LSYS_CKE_GRP0, REG_LSYS_CKE_GRP1, NULL, REG_AON_CLK};
	u32 CenSet[4] = {0};
	u32 ClkRegIndx = 0;

	for (idx = 0; ; idx++) {
		/*  Check if search to end */
		if (RCC_Config[idx].func == 0xFFFFFFFF) {
			break;
		}

		if (RCC_Config[idx].state != ENABLE) {
			continue;
		}

		ClkRegIndx = (RCC_Config[idx].clk >> 30) & 0x03;
		FuncRegIndx = (RCC_Config[idx].func >> 30) & 0x03;

		CenSet[ClkRegIndx] |= RCC_Config[idx].clk;
		FenSet[FuncRegIndx] |= RCC_Config[idx].func;
	}

	for (idx = 0; idx < 4; idx++) {
		/*  Clock configure */
		TempVal = HAL_READ32(SYSTEM_CTRL_BASE, CenReg[idx]);
		TempVal |= CenSet[idx] & (~(BIT(31) | BIT(30)));
		HAL_WRITE32(SYSTEM_CTRL_BASE, CenReg[idx], TempVal);

		/*  Function configure */
		TempVal = HAL_READ32(SYSTEM_CTRL_BASE, FenReg[idx]);
		TempVal |= FenSet[idx] & (~(BIT(31) | BIT(30)));
		HAL_WRITE32(SYSTEM_CTRL_BASE, FenReg[idx], TempVal);
	}

	DelayUs(10);

	/*SDM32K*/
	SDM32K_Enable();
}



/* init psram if needed */
BOOT_RAM_TEXT_SECTION
void BOOT_PSRAM_Init(void)
{
	PCTL_InitTypeDef PCTL_InitStruct;
	PSPHY_InitTypeDef PSPHY_InitStruct;

	/* init psram phy */
	PSRAM_PHY_StructInit(&PSPHY_InitStruct);
	PSRAM_PHY_Init(&PSPHY_InitStruct);

	/* set pad driving */
	HAL_WRITE32(0x4100C9c0, 0, 0x134D49A6);
	HAL_WRITE8(0x4100C9c7, 0, 0x58);

	if (Boot_PSRAM_APM) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Init APM\r\n");

		PSRAM_CTRL_StructInit(&PCTL_InitStruct);
		PSRAM_CTRL_Init(&PCTL_InitStruct);
		/* init psram device */
		PSRAM_APM_DEVIC_Init();

	} else {
		DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Init WB\r\n");

		/* init psram controller */
		PSRAM_CTRL_WB_StructInit(&PCTL_InitStruct);
		PSRAM_CTRL_Init(&PCTL_InitStruct);
		/* init psram device */
		PSRAM_WB_DEVIC_Init();
	}

	/* psram calibration */
	PCAL_InitTypeDef PCAL_InitStruct;
	PSRAM_calibration(&PCAL_InitStruct);

	PSRAM_AutoGating(ENABLE, 1, 16);
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_SCBVTORBackup_HP(void)
{
	PMC_S_BK.SCBVTORbackup_HP = SCB->VTOR;
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_SCBVTORReFill_HP(void)
{
	SCB->VTOR = PMC_S_BK.SCBVTORbackup_HP;
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_NVICBackup_HP(void)
{
	u32 i = 0;

	PMC_S_BK.NVICbackup_HP[0] = NVIC->ISER[0];
	PMC_S_BK.NVICbackup_HP[1] = NVIC->ISER[1];
	PMC_S_BK.NVICbackup_HP[2] = NVIC->ISER[2];

	for (i = 0; i < MAX_PERIPHERAL_IRQ_NUM; i++) {
		PMC_S_BK.NVICIPbackup_HP[i] = NVIC->IPR[i];
	}

	DCache_CleanInvalidate((u32)&PMC_S_BK.NVICbackup_HP[0], sizeof(CPU_S_BackUp_TypeDef));
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_NVICReFill_HP(void)
{
	u32 i = 0;

	for (i = 0; i < MAX_PERIPHERAL_IRQ_NUM; i++) {
		NVIC->IPR[i] = PMC_S_BK.NVICIPbackup_HP[i];
	}

	/* mask log uart irq in secure world when wake */
	NVIC->ISER[0] = PMC_S_BK.NVICbackup_HP[0];
	NVIC->ISER[1] = (PMC_S_BK.NVICbackup_HP[1] & (~(BIT(UART_LOG_IRQ % 32))));
	NVIC->ISER[2] = PMC_S_BK.NVICbackup_HP[2];
}

BOOT_RAM_TEXT_SECTION
void BOOT_ImgCopy(void *__restrict dst0, const void *__restrict src0, size_t len0)
{
	_memcpy(dst0, src0, len0);
}

BOOT_RAM_TEXT_SECTION
u8 BOOT_LoadSubImage(SubImgInfo_TypeDef *SubImgInfo, u32 StartAddr, u8 Num, char **ImgName, u8 ErrLog)
{
	IMAGE_HEADER ImgHdr;
	u32 DstAddr, Len;
	u32 i;

	for (i = 0; i < Num; i++) {
		BOOT_ImgCopy((void *)&ImgHdr, (void *)StartAddr, IMAGE_HEADER_LEN);

		if ((ImgHdr.signature[0] != 0x35393138) || (ImgHdr.signature[1] != 0x31313738)) {
			if (ErrLog == _TRUE) {
				DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s Invalid\n", ImgName[i]);
			}
			return _FALSE;
		}

		DstAddr = ImgHdr.image_addr - IMAGE_HEADER_LEN;
		Len = ImgHdr.image_size + IMAGE_HEADER_LEN;

		/* If not XIP sub-image, then copy it to specific address (include the IMAGE_HEADER)*/
		if ((!IS_FLASH_ADDR(DstAddr)) && (Len > IMAGE_HEADER_LEN)) {
			BOOT_ImgCopy((void *)DstAddr, (void *)StartAddr, Len);
			DCache_CleanInvalidate(DstAddr, Len);
		}

		/* empty Image, Just put in flash, for image hash later */
		if (Len == IMAGE_HEADER_LEN) {
			DstAddr = StartAddr;
		}
		if (SubImgInfo != NULL) {
			SubImgInfo[i].Addr = DstAddr;
			SubImgInfo[i].Len = Len;

			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s[%08x:%x]\n", ImgName[i], DstAddr, Len);
		}

		StartAddr += Len;
	}

	return _TRUE;
}

BOOT_RAM_TEXT_SECTION
void BOOT_RSIPIvSet(Manifest_TypeDef *Manifest, u32 IV_index)
{
	u8 iv[8] = {0};

	/* 1. check if RSIP enable */
	if (SYSCFG_OTP_RSIPEn() == FALSE) {
		return;
	}

	/* get iv from manifest */
	_memcpy(iv, Manifest->RsipIV, 8);
	RSIP_IV_Set(IV_index, iv);

}

/* start addr --> logical addr */
BOOT_RAM_TEXT_SECTION
void BOOT_OTFCheck(u32 start_addr, u32 end_addr, u32 IV_index, u32 OTF_index)
{
	u32 mode;

	/* 1. check if RSIP enable */
	if (SYSCFG_OTP_RSIPEn() == FALSE) {
		return;
	}

	if (IS_FLASH_ADDR(start_addr) && IS_FLASH_ADDR(end_addr)) {
		DBG_8195A("IMG2 OTF EN\n");
		mode = SYSCFG_OTP_RSIPMode();

		switch (mode) {
		case RSIP_XTS_MODE:
			mode = OTF_XTS_MODE;
			break;
		case RSIP_CTR_MODE:
			mode = OTF_CTR_MODE;
			break;
		default:
			DBG_8195A("OTF Mode error\r\n");
			return;
		}

		RSIP_OTF_Enable(OTF_index, start_addr, end_addr, ENABLE, IV_index, mode);
		RSIP_OTF_Cmd(ENABLE);
	}

	return;
}

BOOT_RAM_TEXT_SECTION
int BOOT_LoadDSPImg(Certificate_TypeDef *Cert)
{
	SubImgInfo_TypeDef SubImgInfo[3];
	Manifest_TypeDef Manifest;

	char *DSPLabel[] = {"DSP XIP IMG", "DSP SRAM", "DSP PSRAM"};
	u32 PhyAddr, LogAddr;

	LogAddr = (u32)__dsp_flash_text_start__ - IMAGE_HEADER_LEN;
	PhyAddr = (u32)__dsp_phy_addr__;

	BOOT_ImgCopy((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));

	/* set dsp IV */
	BOOT_RSIPIvSet(&Manifest, 2);

	RSIP_MMU_Config(3, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(3, ENABLE);
	RSIP_MMU_Cache_Clean();
	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 2, 3);

	/* Load dsp image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	if (BOOT_LoadSubImage(SubImgInfo, PhyAddr, 3, DSPLabel, _TRUE) == FALSE) {
		return FALSE;
	}

	/* ECC verify if need */
	BOOT_SignatureCheck(&Manifest, SubImgInfo, 3, Cert, KEYID_DSP);
	return TRUE;
}

BOOT_RAM_TEXT_SECTION
u32 BOOT_LoadImages(void)
{
	SubImgInfo_TypeDef SubImgInfo[12];
	Certificate_TypeDef Cert;
	Manifest_TypeDef Manifest;
	u32 LogAddr, PhyAddr, ImgAddr, TotalLen = 0;
	u8 Index = 0, Cnt, i;

	char *Km0Label[] = {"KR4 XIP IMG", "KR4 PSRAM", "KR4 DATA"};
	char *Km4Label[] = {"KM4 XIP IMG", "KM4 PSRAM", "KM4 SRAM"};

	/* load certificate to SRAM */
	PhyAddr = (u32)__nspe_phy_addr__;
	BOOT_ImgCopy((void *)&Cert, (void *)PhyAddr, sizeof(Certificate_TypeDef));

	/* ECC check certificate */
	BOOT_CertificateCheck(&Cert, PhyAddr);

	/* load manifest to SRAM */
	PhyAddr += CERT_SIZE_4K_ALIGN;
	BOOT_ImgCopy((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));

	/* set IMG2 IV */
	BOOT_RSIPIvSet(&Manifest, 1);

	/* remap KM0 XIP image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	LogAddr = (u32)__kr4_flash_text_start__ - IMAGE_HEADER_LEN;

	RSIP_MMU_Config(1, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(1, ENABLE);
	RSIP_MMU_Cache_Clean();
	/*KM0 IMG2 RSIP configurations*/
	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 1);

	/* KM0 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km0Label) / sizeof(char *);
	ImgAddr = LogAddr;
	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Km0Label, _TRUE) == FALSE) {
		return FALSE;
	}
	for (i = 0; i < Cnt; i++) {
		TotalLen += SubImgInfo[i].Len;
	}
	Index += Cnt;

	/* add 4K-align padding info */
	SubImgInfo[Index].Addr = ImgAddr + TotalLen;
	SubImgInfo[Index].Len = ((((TotalLen - 1) >> 12) + 1) << 12) - TotalLen;
	TotalLen += SubImgInfo[Index].Len;
	Index ++;

	/* remap KM4 XIP image */
	PhyAddr += TotalLen;
	LogAddr = (u32)__km4_flash_text_start__ - IMAGE_HEADER_LEN;

	RSIP_MMU_Config(2, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(2, ENABLE);
	RSIP_MMU_Cache_Clean();

	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 2);

	/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km4Label) / sizeof(char *);
	ImgAddr = LogAddr;
	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Km4Label, _TRUE) == FALSE) {
		return FALSE;
	}

	TotalLen = 0;
	/* calculate dsp phy addr */
	for (i = 0; i < Cnt; i++) {
		TotalLen += SubImgInfo[Index + i].Len;

	}

	Index += Cnt;

#if DSP_BOOT_EN
	if ((u32)__dsp_phy_addr__ > 0x08200000) {

		/* dsp.bin not in img2 */
		BOOT_LoadDSPImg(&Cert);

	} else {
		char *DSPLabel[] = {"DSP XIP IMG", "DSP SRAM", "DSP PSRAM"};
		u32 dsp_only = 0;

		LogAddr = (u32)__dsp_flash_text_start__ - IMAGE_HEADER_LEN;

		/* remap DSP image2 */
		PhyAddr += TotalLen;

		RSIP_MMU_Config(3, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
		RSIP_MMU_Cmd(3, ENABLE);
		RSIP_MMU_Cache_Clean();
		BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 3);

		/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
		Cnt = sizeof(DSPLabel) / sizeof(char *);
		ImgAddr = LogAddr;
		if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, DSPLabel, _TRUE) == FALSE) {
			return FALSE;
		}
		Index += Cnt;

	}

#endif

	/* IMG2 ECC verify if need */
	BOOT_SignatureCheck(&Manifest, SubImgInfo, Index, &Cert, KEYID_NSPE);

	/* Load RDP image if need */
	if (BOOT_LoadRDPImg(&Manifest, SubImgInfo) == TRUE) {
		/* IMG3 ECC verify if need */
		BOOT_SignatureCheck(&Manifest, SubImgInfo, 2, &Cert, KEYID_SPE);
	}

	return _TRUE;
}

/**
  * @brief  Copy Boot reason to a common register and clear Boot Reason.
  * if Boot Reason is not cleared, it can't get correct boot reason next time.
  * @param  None
  * @retval None.
*/
BOOT_RAM_TEXT_SECTION
void BOOT_ReasonSet(void)
{
	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_AON_BOOT_REASON_HW);

	/*Clear the wake up reason*/
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_AON_BOOT_REASON_HW, temp);

	/*Backup it to system register,So the software can read from the register*/
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_REASON_SW, temp);

}

BOOT_RAM_TEXT_SECTION
void BOOT_Enable_KR4(void)
{
	/* Let KR4 run */
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_BOOT_CFG) | LSYS_BIT_BOOT_KR4_RUN);

	/*If KR4_L2MEM_EN is not set, kr4 cannot access rom*/
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL) | LSYS_BIT_KR4_L2MEM_EN);
	RCC_PeriphClockCmd(APBPeriph_KR4, APBPeriph_KR4_CLOCK, ENABLE);
}


BOOT_RAM_TEXT_SECTION
void BOOT_WakeFromPG(void)
{
	PRAM_START_FUNCTION Image2EntryFun = NULL;
	u32 *vector_table = NULL;
	//cmse_address_info_t cmse_address_info = cmse_TT((void *)DiagPrintf);

	//DBG_8195A("%s\n", __func__);
	Image2EntryFun = (PRAM_START_FUNCTION)__image2_entry_func__;

	/* TODO: Config Non-Security World Registers */
	BOOT_RAM_TZCfg();

#ifndef CONFIG_XIP_FLASH
	if (!LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1))) {
		set_psram_suspend_and_restore(ENABLE);
	}
#endif

	vector_table = (u32 *)Image2EntryFun->VectorNS;
	vector_table[1] = (u32)Image2EntryFun->RamWakeupFun;
	SCB_NS->VTOR = (u32)vector_table;

	/* refill SCB_VTOR_S after wakeup */
	if (PMC_S_BK.SCBVTORbackup_HP) {
		BOOT_SCBVTORReFill_HP();
	}

	/* Enable all access to FPU */
	SCB->CPACR |= 0x00f00000;
	SCB_NS->CPACR |=  0x00f00000;
	SCB->NSACR |= BIT(10) | BIT(11); // enable non-secure to access VFP

	/* refill NVIC registers after wakeup */
	BOOT_NVICReFill_HP();

	/* Set non-secure main stack (MSP_NS) */
	__TZ_set_MSP_NS(MSP_RAM_HP_NS);

	/* Set PSPS Temp */
	__set_PSP(MSP_RAM_HP_NS - 2048);

	/* Set non-secure main stack (MSP_NS) */
	//DBG_8195A ("NonSecure Start @ 0x%x MSP_NS:%x VCTOR:%x\r\n", vector_table[1], vector_table[0], vector_table);

	/* Start non-secure state software application */
	//DBG_8195A("Start NonSecure @ 0x%x ...\r\n", NonSecure_ResetHandler);
	//DBG_8195A("Cache Enable:%x\r\n", DCache_IsEnabled());
	BOOT_NsStart(vector_table[1]);

	return;
}

BOOT_RAM_TEXT_SECTION
void BOOT_Enable_DSP(void)
{
	u32 hifi_en;
	u32 reset_vector_addr;
	u32 dsp_ram_start = (u32)(__dsp_ram_addr__);

	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_4, ENABLE);
	reset_vector_addr = HAL_READ32(dsp_ram_start, 0);

	RCC_PeriphClockCmd(APBPeriph_HIFI, APBPeriph_HIFI_CLOCK, ENABLE);

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL1, reset_vector_addr);//address of the reset vector
	hifi_en = LSYS_BIT_HIFI_BRESETN | LSYS_BIT_HIFI_DRESETN | LSYS_BIT_HIFI_PRESETN | LSYS_BIT_HIFI_STAT_VECTOR_SEL | LSYS_BIT_HIFI_PDEBUG_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL0, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_HIFI_CTRL0) | hifi_en);
}

BOOT_RAM_TEXT_SECTION
void BOOT_NonSecure_VAD(void)
{
	RCC_PeriphClockCmd(APBPeriph_AC, APBPeriph_AC_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_AC_AIP, NULL, ENABLE);
	RCC_PeriphClockSource_AUDIOCODEC(CKSL_AC_XTAL);
	VADBUF_TypeDef *VAD_BUF = ((VADBUF_TypeDef *) VAD_REG_BASE_S);
	VAD_BUF->VADBUF_VAD_SECURE_SEL &= ~VADBUF_BIT_VAD_SECURITY_SEL;
}


//3 Image 1
BOOT_RAM_TEXT_SECTION
void BOOT_Image1(void)
{
	u32 ret;
	u32 *vector_table = NULL;
	PRAM_START_FUNCTION Image2EntryFun = (PRAM_START_FUNCTION)__image2_entry_func__;
	STDLIB_ENTRY_TABLE *prom_stdlib_export_func = (STDLIB_ENTRY_TABLE *)__rom_stdlib_text_start__;

	RBSS_UDELAY_CLK = CPU_ClkGet() / 1000000;

	/* open analog LDO */
	LDO_PSRAM(ENABLE);

	_memset((void *) __image1_bss_start__, 0, (__image1_bss_end__ - __image1_bss_start__));

	BOOT_ReasonSet();
	/*check environment*/
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_PALADIUM) {
		Boot_PSRAM_En = TRUE;
	}
	/* need about 100-300us, need sync */
	if (Boot_PSRAM_En) {
		RCC_PeriphClockCmd(APBPeriph_PSRAM, APBPeriph_PSRAM_CLOCK, ENABLE);
	}

	/* open LP log */
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_2, ENABLE);

	if (Boot_Log_En && !SYSCFG_OTP_DisBootLog()) {
		LOG_MASK_MODULE(MODULE_BOOT, LEVEL_INFO, ENABLE);
	}
	LOG_MASK_MODULE(MODULE_BOOT, LEVEL_ERROR, ENABLE);

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG1 ENTER MSP:[%08x]\n", __get_MSP());

	cmse_address_info_t cmse_address_info = cmse_TT((void *)BOOT_Image1);
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG1 SECURE STATE: %d\n", cmse_address_info.flags.secure);

	/* init stdlib bss and load stdlib data */
	prom_stdlib_export_func->stdlib_init();

	BOOT_SectionInit();

	BOOT_RccConfig();

	flash_highspeed_setup();

	/* backup flash_init_para address for KR4 */
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_FLASH_PARA_ADDR, (u32)&flash_init_para);

#ifndef CONFIG_XIP_FLASH
	if (Boot_PSRAM_En) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Init PSRAM\r\n");
		/* open pad */
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1) | PAD_BIT_DDR_PWDPADN_2REGU);
		BOOT_PSRAM_Init();
	}
#endif

	ret = BOOT_LoadImages();
	if (ret == _FALSE) {
		goto INVALID_IMG2;
	}

	/* Enable SecureFault, UsageFault, MemManageFault, BusFault */
	SCB->SHCSR |= SCB_SHCSR_SECUREFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | \
				  SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;

	SCB_NS->SHCSR |=  SCB_SHCSR_USGFAULTENA_Msk | \
					  SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;

	/* it will be reset after PG, backup SCB_VTOR_S */
	BOOT_SCBVTORBackup_HP();

	/* enable GDMA in secure world */
	/* Enable GDMA in DmaCfgReg */
	GDMA_TypeDef *GDMA = ((GDMA_TypeDef *) GDMA_BASE);
	GDMA->DmaCfgReg = 1;

	/* Config Non-Security World Registers */
	BOOT_RAM_TZCfg();

	/* config address filter before enable KR4 and HIFI5 */
	BOOT_RAM_AFCfg();

#if defined(CONFIG_AS_AP)
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1) | (LSYS_BIT_KR4_IS_NP));
#elif defined(CONFIG_AS_NP)
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1, HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1) & (~LSYS_BIT_KR4_IS_NP));
#endif
	for (u32 rw_prot = 0; rw_prot < 0x5C; rw_prot += 4) {
		HAL_WRITE32(0x4100DE00, rw_prot, 0);
	}

#if VAD_NS_EN
	BOOT_NonSecure_VAD();
#endif

	/*KR4 shall wait MPC setting for non-secure access*/
	BOOT_Enable_KR4();

	/* DSP Power-on, DSP start run */
#if DSP_BOOT_EN
	BOOT_Enable_DSP();
#endif

	vector_table = (u32 *)Image2EntryFun->VectorNS;
	vector_table[1] = (u32)Image2EntryFun->RamStartFun;
	SCB_NS->VTOR = (u32)vector_table;

	/* Enable all access to FPU */
	SCB->CPACR |= 0x00f00000;
	SCB_NS->CPACR |=  0x00f00000;
	SCB->NSACR |= BIT(10) | BIT(11); // enable non-secure to access VFP

	/* backup NVIC_S */
	BOOT_NVICBackup_HP();

	/* Set non-secure main stack (MSP_NS) */
	__TZ_set_MSP_NS(MSP_RAM_HP_NS);

	/* Set PSPS Temp */
	__set_PSP(MSP_RAM_HP_NS - 2048);

	/* Start non-secure state software application */
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Start NonSecure @ 0x%x ...\r\n", vector_table[1]);

	BOOT_NsStart(vector_table[1]);

	return;

INVALID_IMG2:
	while (1) {
		DelayMs(1000);//each delay is 100us
	}
}

IMAGE1_VALID_PATTEN_SECTION
const u8 RAM_IMG1_VALID_PATTEN[] = {
	0x23, 0x79, 0x16, 0x88, 0xff, 0xff, 0xff, 0xff
};

IMAGE1_EXPORT_SYMB_SECTION
BOOT_EXPORT_SYMB_TABLE boot_export_symbol = {

};

IMAGE1_ENTRY_SECTION
RAM_FUNCTION_START_TABLE RamStartTable = {
	.RamStartFun = NULL,
	.RamWakeupFun = NULL,
	.RamPatchFun0 = BOOT_WakeFromPG,
	.RamPatchFun1 = NULL,
	.RamPatchFun2 = NULL,
	.FlashStartFun = BOOT_Image1,
	.ExportTable = &boot_export_symbol
};

