/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "amebahp_secure_boot.h"
#include "bootloader_hp.h"
#include "boot_ota_hp.h"

static Certificate_TypeDef Cert[2]; //Certificate of SlotA & SlotB
static Manifest_TypeDef Manifest[2]; //Manifest of SlotA & SlotB

static SubImgInfo_TypeDef SubImgInfo[10]; //store sub image addr and length

static uint32_t ImagePattern[2] = {
	0x35393138,
	0x31313738,
};

static u8 ValidIMGNum[3] = {0};

/*
* @brif	OTA start address. Because KM0 & KM4 IMG2 are combined, users only need to set the start address
*	of KM0 IMG2.
*  In each entry, the first address is OTA1 address(SlotA address), the second address is OTA2 address (SlotB address)
*/
u32 OTA_Region[3][2];
extern FlashLayoutInfo_TypeDef Flash_Layout_Nor[];
extern FlashLayoutInfo_TypeDef Flash_Layout_Nand[];

/**
  * @brief  Get Version Number from OTP
  * @param  none
  * @retval bootloader version in OTP
  */
BOOT_RAM_TEXT_SECTION
static u32 SYSCFG_OTP_GetRollbackVer(void)
{
	u16 VerNum = 0;
	u32 VN; //32-bit full version in OTP

	u16 tmp = HAL_READ16(OTPC_REG_BASE, SEC_BOOT_IMGVER0);

	for (int i = 0; i < 16; i++) {
		if ((tmp & BIT(i)) == 0) {
			VerNum++;
		}
	}

	VN = VerNum << 16;
	return VN;

}

#define SYSCFG_OTP_GetRollbackVerIMG SYSCFG_OTP_GetRollbackVer

/**
  * @brief  check manifest pattern and version for IMG2, IMG3 and AP
  * @param  IMGType: IMG_IMG2/IMG_IMG3/IMG_APIMG
  * @retval 0:Slot A  1:Slot B
  */
BOOT_RAM_TEXT_SECTION
u8 BOOT_OTA_IMGSlotSelect(u32 IMGType)
{
	u16 MajorVer[2] = {0}; //16-bit major
	u16 MinorVer[2] = {0}; //16-bit minor
	u32 Vertemp;
	s64 Ver[2] = {0};  //32-bit full version
	s64 VN_IMG; //32-bit full version in OTP for IMG2
	u8 ImgIndex, i;
	/* ------------1 load IMG(Slot A & Slot B) from flash to SRAM, check pattern------------ */
	for (i = 0; i < 2; i++) {
		BOOT_ImgCopy((void *)&Manifest[i], (void *)OTA_Region[IMGType][i], sizeof(Manifest_TypeDef));
		if (_memcmp(Manifest[i].Pattern, ImagePattern, sizeof(ImagePattern)) == 0) {
			MajorVer[i] = (u16)Manifest[i].MajorImgVer;
			MinorVer[i] = (u16)Manifest[i].MinorImgVer;
			Vertemp = MajorVer[i] << 16 | MinorVer[i];
			Ver[i] = (s64)Vertemp; // get 32-bit full version number
		} else {
			Ver[i] = -1;
		}
	}
	/* ---------------------------------2 Get bootldr version in OTP if anti-rollback --------------------------------- */
	if (SYSCFG_OTP_AntiRollback()) {
		VN_IMG = (s64)SYSCFG_OTP_GetRollbackVerIMG();
	} else {
		VN_IMG = 0;
	}

	/* ---------------------------------3 Check full KeyVerion in manifest--------------------------------- */
	if ((VN_IMG > Ver[0]) && (VN_IMG > Ver[1])) {
		ValidIMGNum[IMGType] = NONEVALIDIMG;
	} else if ((VN_IMG <= Ver[0]) && (VN_IMG <= Ver[1])) {
		ValidIMGNum[IMGType] = TWOVALIDIMG;
	} else {
		ValidIMGNum[IMGType] = ONEVALIDIMG;
	}

	ImgIndex = (Ver[0] >= Ver[1]) ? BOOT_FROM_OTA1 : BOOT_FROM_OTA2;
	return ImgIndex;
}

/**
  * @brief  check Certificate pattern and version
  * @param none
  * @retval 0:Slot A  1:Slot B  2:V1 = V2
  */
BOOT_RAM_TEXT_SECTION
u8 BOOT_OTA_CertSlotSelect(void)
{
	u16 MajorVer[2] = {0}; //16-bit major
	u16 MinorVer[2] = {0}; //16-bit minor
	u32 Vertemp;
	s64 Ver[2] = {0};  //32-bit full version
	s64 VN_CERT; //32-bit full version in OTP
	u8 ImgIndex, i;

	/* ----1 load certificate(Slot A & Slot B) from flash to SRAM, get full version and check validation---- */
	for (i = 0; i < 2; i++) {
		BOOT_ImgCopy((void *)&Cert[i], (void *)OTA_Region[IMG_CERT][i], sizeof(Certificate_TypeDef));

		if (_memcmp(Cert[i].Pattern, ImagePattern, sizeof(ImagePattern)) == 0) {
			MajorVer[i] = (u16)Cert[i].MajorKeyVer;
			MinorVer[i] = (u16)Cert[i].MinorKeyVer;
			Vertemp = (MajorVer[i] << 16) | MinorVer[i];
			Ver[i] = (s64)Vertemp; // get 32-bit full version number
		} else {
			Ver[i] = -1;
		}
	}

	/* ---------------------------------2 Get bootldr version in OTP if anti-rollback --------------------------------- */
	if (SYSCFG_OTP_AntiRollback()) {
		VN_CERT = (s64)SYSCFG_OTP_GetRollbackVer();
	} else {
		VN_CERT = 0;
	}


	/* ---------------------------------3 Check full KeyVerion in certificate--------------------------------- */
	if ((VN_CERT > Ver[0]) && (VN_CERT > Ver[1])) {
		ValidIMGNum[IMG_CERT] = NONEVALIDIMG;
	} else if ((VN_CERT <= Ver[0]) && (VN_CERT <= Ver[1])) {
		ValidIMGNum[IMG_CERT] = TWOVALIDIMG;

		if (Ver[0] == Ver[1]) {
			return SAMEVERSION;
		}
	} else {
		ValidIMGNum[IMG_CERT] = ONEVALIDIMG;
	}

	ImgIndex = (Ver[0] >= Ver[1]) ? BOOT_FROM_OTA1 : BOOT_FROM_OTA2;
	return ImgIndex;

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
		if (!IS_FLASH_ADDR(DstAddr)) {
			BOOT_ImgCopy((void *)DstAddr, (void *)StartAddr, Len);
			DCache_CleanInvalidate(DstAddr, Len);
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
/* Load km0/km4 img and check pattern if secure boot enabled*/
u8 BOOT_OTA_LoadIMG2(u8 ImgIndex)
{
	u32 LogAddr, PhyAddr, ImgAddr, TotalLen = 0;
	u8 Cnt;
	u8 i;
	u32 ret;
	u32 Index = 0;

	char *Km0Label[] = {"KM0 XIP IMG", "KM0 SRAM", "KM0 DRAM"};
	char *Km4Label[] = {"KM4 XIP IMG", "KM4 SRAM", "KM4 DRAM"};


	PhyAddr = OTA_Region[IMG_IMG2][ImgIndex];

	/* set IMG2 IV */
	BOOT_RSIPIvSet(&Manifest[ImgIndex], 1);

	/* remap KM0 XIP image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	LogAddr = (u32)__km0_flash_text_start__ - IMAGE_HEADER_LEN;

	RSIP_MMU_Config(0, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(0, ENABLE);

	/*KM0 IMG2 RSIP configurations*/
	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 1);

	/* KM0 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km0Label) / sizeof(char *);
	ImgAddr = SYSCFG_BootFromNor() ? LogAddr : PhyAddr;

	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Km0Label, _TRUE) == FALSE) {
		return FALSE;
	}
	SubImgInfo[Index].Addr = SYSCFG_BootFromNor() ? LogAddr : PhyAddr;

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

	RSIP_MMU_Config(1, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(1, ENABLE);

	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 2);

	/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km4Label) / sizeof(char *);
	ImgAddr = SYSCFG_BootFromNor() ? LogAddr : PhyAddr;
	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Km4Label, _TRUE) == FALSE) {
		return FALSE;
	}
	SubImgInfo[Index].Addr = SYSCFG_BootFromNor() ? LogAddr : PhyAddr;
	Index += Cnt;

	/* check if RDP enable */
	if (SYSCFG_OTP_RDPEn() == TRUE) {
		/* Load from OTA and ECC check for IMG3 */
		DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "RDP EN\n");
		Cnt = BOOT_OTA_RDP(SubImgInfo, Index, ImgIndex);
		Index += Cnt;
	}

	assert_param(Index < sizeof(SubImgInfo) / sizeof(SubImgInfo_TypeDef));

	/* IMG2 ECC verify if need */
	ret = BOOT_SignatureCheck(&Manifest[ImgIndex], SubImgInfo, Index, &Cert[ImgIndex], KEYID_NSPE);

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG2(OTA%d) %s, ret: %d\n", ImgIndex + 1, ret > 0 ? "VALID" : "INVALID", ret);


	if (ret == FALSE) {
		return FALSE;
	}

	return TRUE;
}

BOOT_RAM_TEXT_SECTION
void BOOT_OTA_Region_Init(void)
{
	u32 i = 0;
	u32 temp;
	FlashLayoutInfo_TypeDef *pLayout;

	if (SYSCFG_BootFromNor()) {
		pLayout = Flash_Layout_Nor;
	} else {
		pLayout = Flash_Layout_Nand;
	}

	while (pLayout[i].region_type != 0xFF) {
		temp = pLayout[i].region_type;
		if (temp == IMG_APP_OTA1) {
			OTA_Region[IMG_CERT][0] = pLayout[i].start_addr;
		} else if (temp == IMG_APP_OTA2) {
			OTA_Region[IMG_CERT][1] = pLayout[i].start_addr;
		} else if (temp == IMG_APIMG_OTA1) {
			OTA_Region[IMG_APIMG][0] = pLayout[i].start_addr;
		} else if (temp == IMG_APIMG_OTA2) {
			OTA_Region[IMG_APIMG][1] = pLayout[i].start_addr;
		}

		i++;
	}

	OTA_Region[IMG_IMG2][0] = OTA_Region[IMG_CERT][0] + 0x1000;
	OTA_Region[IMG_IMG2][1] = OTA_Region[IMG_CERT][1] + 0x1000;
}


BOOT_RAM_TEXT_SECTION
u8 BOOT_OTA_IMG2(void)
{
	u8 CertImgIndex, CertValidNumber, IMG2ImgIndex, ImgIndex = 0;
	u32 PhyAddr;
	u8 IMG_ID;
	u8 ret, i;

	BOOT_OTA_Region_Init();

	/*-----------------------1. OTA Slot Selecte according to Cert & IMG2 version--------------------- */
	if (SYSCFG_OTP_SBootEn() == FALSE) {

		/* Skip Certificate, Select Slot according to IMG2 Version */
		IMG2ImgIndex = BOOT_OTA_IMGSlotSelect(IMG_IMG2);

		/* no valid IMG2, boot fail */
		if (ValidIMGNum[IMG_IMG2] == NONEVALIDIMG) {
			goto Fail;
		}

		IMG_ID = IMG_IMG2;
		ImgIndex = IMG2ImgIndex;

	} else {

		/* Select Slot according to Cert Version */
		CertImgIndex = BOOT_OTA_CertSlotSelect();
		CertValidNumber = ValidIMGNum[IMG_CERT];

		switch (CertValidNumber) {
		case NONEVALIDIMG:
			goto Fail;

		case ONEVALIDIMG:

			/* use the valid Cert & use IMG2 of the Same slot */
			IMG_ID = IMG_CERT;
			ImgIndex = CertImgIndex;
			BOOT_ImgCopy((void *)&Manifest[ImgIndex], (void *)OTA_Region[IMG_IMG2][ImgIndex], sizeof(Manifest_TypeDef));
			break;

		case TWOVALIDIMG:

			/* check if two Cert Version are the Same */
			if (CertImgIndex == SAMEVERSION) {

				/* Select Slot according to IMG2 Version */
				IMG2ImgIndex = BOOT_OTA_IMGSlotSelect(IMG_IMG2);

				/* no valid IMG2, boot fail */
				if (ValidIMGNum[IMG_IMG2] == NONEVALIDIMG) {
					goto Fail;
				}

				/* use the bigger version of IMG2 & use Cert of the Same slot */
				IMG_ID = IMG_IMG2;
				ImgIndex = IMG2ImgIndex;
			} else {

				/* use the bigger version of Cert & use IMG2 of the Same slot */
				IMG_ID = IMG_CERT;
				ImgIndex = CertImgIndex;
				BOOT_ImgCopy((void *)&Manifest[ImgIndex], (void *)OTA_Region[IMG_IMG2][ImgIndex], sizeof(Manifest_TypeDef));
			}

			break;
		}
	}

	/*-----------------------2. Boot from Selected Slot: image load and signature check--------------------- */
	for (i = 0; i < ValidIMGNum[IMG_ID]; i++) {

		PhyAddr = OTA_Region[IMG_CERT][ImgIndex]; //get physical address
		ret = BOOT_CertificateCheck(&Cert[ImgIndex], PhyAddr); //Cert ECC check

		if (ret != TRUE) {
			DBG_8195A("Cert check fail\n");
			ImgIndex = (ImgIndex + 1) % 2;
			continue;
		}

		ret = BOOT_OTA_LoadIMG2(ImgIndex); // // IMG2 load & ECC check

		if (ret != TRUE) {
			ImgIndex = (ImgIndex + 1) % 2;
			continue;
		} else {
			break;
		}
	}

	if (i == ValidIMGNum[IMG_ID]) {
		goto Fail;
	}

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG2 BOOT from OTA %d\n", ImgIndex + 1);

	return ImgIndex; //verified slot index


Fail:
	DBG_8195A("OTA Certificate & IMG2 invalid, BOOT FAIL!!\n");

	/* clear stack */
	__set_MSP(MSP_RAM_HP);
	_memset((void *)MSPLIM_RAM_HP, 0, MSP_RAM_HP - 128 - MSPLIM_RAM_HP);
	DCache_CleanInvalidate(MSPLIM_RAM_HP, MSP_RAM_HP - 128 - MSPLIM_RAM_HP);

	while (1) {
		DelayMs(1000);
	}

	return 0;
}


BOOT_RAM_TEXT_SECTION
u8 BOOT_OTA_RDP(SubImgInfo_TypeDef *SubImgInfo, u8 Index, u8 ImgIndex)
{
	u8 i;
	u8 Cnt = 0;
	Manifest_TypeDef ManifestIMG;
	u32 PhyAddr;
	u32 Img2Addr = OTA_Region[IMG_IMG2][ImgIndex];

	/* calculate rdp phy addr */
	PhyAddr = Img2Addr;
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	for (i = 0; i < Index; i++) {
		PhyAddr += SubImgInfo[i].Len;
	}

	BOOT_ImgCopy((void *)&ManifestIMG, (void *)PhyAddr, sizeof(Manifest_TypeDef));

	if (_memcmp(ManifestIMG.Pattern, ImagePattern, sizeof(ImagePattern)) != 0) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "RDP IMG Invalid\n");
		goto Fail;
	}

	// Check sub-image pattern and load RDP sub-image
	if ((Cnt = BOOT_LoadRDPImg(&ManifestIMG, &SubImgInfo[Index], PhyAddr)) == FALSE) {
		goto Fail;
	}

	BOOT_SignatureCheck(&ManifestIMG, &SubImgInfo[Index], 2, &Cert[ImgIndex], KEYID_SPE); // IMG3 ECC verify if need
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG3 BOOT from OTA %d\n", ImgIndex + 1);

	return Cnt;

Fail:
	DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "Fail to load RDP image!\n");
	while (1) {
		DelayMs(1000);
	}
}

BOOT_RAM_TEXT_SECTION
u8 BOOT_OTA_AP(u8 CertImgIndex)
{
	u32 PhyAddr;
	u8 Cnt, i;
	u8 ret;
	u8 APImgIndex;
	Manifest_TypeDef Manifest;

	char *APLabel[] = {"AP BL1 SRAM", "AP BL1 DRAM", "AP FIP"};

	APImgIndex = BOOT_OTA_IMGSlotSelect(IMG_APIMG);

	/* no valid AP IMG, boot fail */
	if (ValidIMGNum[IMG_APIMG] == NONEVALIDIMG) {
		goto Fail;
	}

	for (i = 0; i < ValidIMGNum[IMG_APIMG]; i++) {

		PhyAddr = OTA_Region[IMG_APIMG][APImgIndex];
		BOOT_ImgCopy((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));

		Cnt = sizeof(APLabel) / sizeof(char *);
		PhyAddr += MANIFEST_SIZE_4K_ALIGN;

		ret = BOOT_LoadSubImage(SubImgInfo, PhyAddr, Cnt, APLabel, _TRUE); // Check sub-image pattern and load AP sub-image

		if (ret != TRUE) {
			APImgIndex = (APImgIndex + 1) % 2;
			continue;
		}

		ret = BOOT_SignatureCheck(&Manifest, SubImgInfo, Cnt, &Cert[CertImgIndex], KEYID_AP); //BL1 ECC verify if need

		if (ret != TRUE) {
			APImgIndex = (APImgIndex + 1) % 2;
			continue;
		} else {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "AP BOOT from OTA %d\n", APImgIndex + 1);
			break;
		}

	}

	if (i == ValidIMGNum[IMG_APIMG]) {
		goto Fail;
	}

	return 0;

Fail:
	DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "Fail to load AP image!\n");
	while (1) {
		DelayMs(1000);
	}
}



