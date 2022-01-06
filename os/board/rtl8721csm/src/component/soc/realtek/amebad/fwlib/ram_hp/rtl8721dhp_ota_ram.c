/**
  ******************************************************************************
  * @file    rtl8721dhp_ota_ram.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the OTA functions.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"
#include "strproc.h"

/**
  * @brief  Set OTA image index.
  * @param  OTAIdx: This parameter can be  one of the following values:
  *            @arg OTA_INDEX_1: select OTA1(low flash address) as image2
  *            @arg OTA_INDEX_2: select OTA2(high flash address) as image2
  * @retval status value:
  *          - 1: set ok
  *          - 0: set fail
  */

#define OTA1_ADDR 0x6000
#define OTA2_ADDR 0X106000

IMAGE2_RAM_TEXT_SECTION
u32 OTA_Change(u32 OTAIdx)
{
	u32 BitIdx = 0;
	u32 ota2_sig[2];

	ota2_sig[0] = HAL_READ32(SPI_FLASH_BASE, OTA2_ADDR);
	ota2_sig[1] = HAL_READ32(SPI_FLASH_BASE, OTA2_ADDR+4);

	for(int i = 0; i < 2; i++)
		DBG_8195A("%x",ota2_sig[i]);

	if(0x35393138 == ota2_sig[0] && 0x31313738 == ota2_sig[1])
		BitIdx = 2;
	else
		BitIdx = 1;

	ota2_sig[0] = 0x35393138;
	ota2_sig[1] = 0x31313738;

	/* even bits 0 currrent is OTA1 */
	if (BitIdx == 1) {
		if (OTAIdx == OTA_INDEX_1) {
			DBG_8195A("currrent is OTA1, select OTA1 \n");
		} else {
			DBG_8195A("currrent is OTA1, select OTA2 \n");
			FLASH_EreaseDwordsXIP(OTA1_ADDR, 2);
			FLASH_EreaseDwordsXIP(OTA2_ADDR, 2);
			FLASH_TxData12BXIP(OTA2_ADDR, 8,(u8*)ota2_sig);
		}
	} else { /* odd bits 0 currrent is OTA2 */
		if (OTAIdx == OTA_INDEX_1) {
			DBG_8195A("currrent is OTA2, select OTA1 \n");
			FLASH_EreaseDwordsXIP(OTA1_ADDR, 2);
			FLASH_EreaseDwordsXIP(OTA2_ADDR, 2);
			FLASH_TxData12BXIP(OTA1_ADDR, 8, (u8*)ota2_sig);
		} else {
			DBG_8195A("currrent is OTA2, select OTA2 \n");
		}	
	}
	
	return _TRUE;
}

#define TIZENRT_KERNEL_HEADER_LEN 0x10
IMAGE2_RAM_TEXT_SECTION
static u8 OTA_AddrChk(u8 idx, uint32_t pAddr, uint32_t *size, uint32_t *SBAddr)
{
	IMAGE_HEADER *Img2Hdr, *Img2DataHdr, *PsramHdr;
	uint32_t ImgSize, SBheader;

	/* Verify signature */
	if(idx == 0) {
		Img2Hdr = (IMAGE_HEADER *)(pAddr + TIZENRT_KERNEL_HEADER_LEN);
	} else {
		Img2Hdr = (IMAGE_HEADER *)pAddr;
	}
	if((Img2Hdr->signature[0] != 0x35393138) || (Img2Hdr->signature[1] != 0x31313738) || ((Img2Hdr->image_size) & 0x3)) {
		return _FALSE;
	}

	/* Get image size, which should be 4KB-aligned */
	if(idx == 0) {
		Img2DataHdr = (IMAGE_HEADER *)(pAddr + TIZENRT_KERNEL_HEADER_LEN + IMAGE_HEADER_LEN + Img2Hdr->image_size);
		ImgSize = Img2Hdr->image_size + Img2DataHdr->image_size + IMAGE_HEADER_LEN * 2 + TIZENRT_KERNEL_HEADER_LEN;
	} else {
		Img2DataHdr = (IMAGE_HEADER *)(pAddr + IMAGE_HEADER_LEN + Img2Hdr->image_size);
		if((Img2DataHdr->signature[0] != 0x35393138) || (Img2DataHdr->signature[1] != 0x31313738) || ((Img2DataHdr->image_size) & 0x3)) {
			return _FALSE;
		}
		ImgSize = Img2Hdr->image_size + Img2DataHdr->image_size + IMAGE_HEADER_LEN * 2;
	}

	/* KM4 image contains PSRAM code+data part */
	if(idx == 1) {
		PsramHdr = (IMAGE_HEADER *)(pAddr + ImgSize);
		if((PsramHdr->signature[0] != 0x35393138) || (PsramHdr->signature[1] != 0x31313738) || ((PsramHdr->image_size) & 0x3)) {
			return _FALSE;
		}
		ImgSize += (IMAGE_HEADER_LEN + PsramHdr->image_size);

		/*check if KM4 image contains secure image2 signature*/
		SBheader = Img2Hdr->sb_header;
		if (SBheader != 0xFFFFFFFF) {
			ImgSize += 0x1000;
			if(SBAddr != NULL)
				*SBAddr = SBheader;
		}
	}

	ImgSize = (((ImgSize -1) >> 12) + 1) << 12;  /* 4KB aligned */
	if(size != NULL)
		*size = ImgSize;

	return _TRUE;
}

IMAGE2_RAM_TEXT_SECTION
int OTA_Image2SignatureCheck(uint32_t input_addr)
{
	int ret, index = 0;
	uint32_t image2_addr, SBAddr, xip_AddrSrt, size;
	unsigned char image_key[32], cmp_key[32];

	rtw_printf("Image Start Addrs: 0x%x \n", input_addr);
	ret = OTA_AddrChk(0, input_addr, &size, NULL);	/* KM0 Image2 Check */
	if(ret == _FALSE) {
		dbg("KM0 Image Header Check Fail \n");
		return -1;
	}
	rtw_printf("OTA_AddrChk: 0, size: 0x%x \n", size);

	image2_addr = (input_addr + size);	/* KM4 image2 addr = KM0 Image2 + KM0 Image2 Size */
	rtw_printf("image2_addr: 0x%x \n", image2_addr);

	ret = OTA_AddrChk(1, (input_addr + size), &size, &SBAddr);	/* KM4 Image2 Check */
	if(ret == _FALSE) {
		dbg("KM4 Image Header Check Fail \n");
		return -1;
	}
	rtw_printf("OTA_AddrChk: 1, size: 0x%x SBAddr: 0x%x \n", size, SBAddr);

	if(SBAddr == 0xFFFFFFFF) {	/*If Secure Boot Header empty */
		dbg("Secure Header Empty SBAddr: 0x%x \n", SBAddr);
		return -1;
	}

	ROM_SECURE_CALL_NS_ENTRY *prom_sec_call_ns_entry = (ROM_SECURE_CALL_NS_ENTRY *)__rom_entry_ns_start__;

	/* Get XIP Address Offset */
	RSIP_REG_TypeDef* RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);
	xip_AddrSrt = RSIP->FLASH_MMU[1].MMU_ENTRYx_STRADDR;

	/* Sign script added XIP start address into singature header */
	/* Secure Boot signature header = Address from Image header - xip_AddrSrt + Image header start address */
	/* xip_AddrSrt is because refering from XIP, so need to deduct it */
	SB_HEADER *SbHeader = (SB_HEADER *)(SBAddr - xip_AddrSrt + image2_addr);
	rtw_printf("SbHeader Address: 0x%x \n", SbHeader);
	if(((uint32_t)SbHeader < image2_addr) || ((uint32_t)SbHeader > (image2_addr + size)))	/*If Signature Header out of range */ {
		dbg("Secure Header out of range SbHeader: 0x%x \n", SbHeader);
		return -1;
	}

	/* get public key*/
	for (index = 0; index < 32; index++) {
		EFUSERead8(0, (SBOOT_PK_ADDR+index), &image_key[index], L25EOUTVOLTAGE);
	}
	memset(cmp_key, 0xff, sizeof(image_key));
	if (0 == memcmp(cmp_key, image_key, sizeof(image_key))) {
		dbg("Public Key is Empty\n");
		return -1;
	}

	size = SBAddr - xip_AddrSrt - IMAGE_HEADER_LEN; /* Image length: Secure Boot Header address - xip offset - Image Header length */
	rtw_printf("SBAddr - 0x%x - 0x%x:size = 0x%x \n", xip_AddrSrt, IMAGE_HEADER_LEN, size);
	rtw_printf("SbHeader->sb_sig (Signature Address): 0x%x \n", SbHeader->sb_sig);
	rtw_printf("%x %x %x %x %x %x %x %x \n", *(SbHeader->sb_sig), *(SbHeader->sb_sig+1), *(SbHeader->sb_sig+2), *(SbHeader->sb_sig+3), *(SbHeader->sb_sig+4), *(SbHeader->sb_sig+5), *(SbHeader->sb_sig+6), *(SbHeader->sb_sig+7));

	if (prom_sec_call_ns_entry->ed25519_verify_signature(SbHeader->sb_sig, (const unsigned char *)(image2_addr + IMAGE_HEADER_LEN), size, image_key) == 0) {
		rtw_printf("IMG2 Verify OK\n");
		return 0;
	} else {
		dbg("IMG2 Verify FAIL\n");
		return -1;
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
