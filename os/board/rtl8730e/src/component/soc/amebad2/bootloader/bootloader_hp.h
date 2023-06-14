/**
  ******************************************************************************
  * @file    bootloader_hp.h
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file contains all the definations of platform.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _HAL_BOOT_
#define _HAL_BOOT_

#define CERT_SIZE_4K_ALIGN		0x1000
#define CERT_MAX_KEYINFO		5

#define KEYID_NSPE				0
#define KEYID_SPE				1
#define KEYID_AP				2

typedef struct {
	uint32_t KeyID;
	unsigned char Hash[32];

} CertEntry_TypeDef;

typedef struct {
	u8 Pattern[8];
	u8 Rsvd1[8];
	u8 Ver;
	u8 ImgID;
	u8 AuthAlg;
	u8 HashAlg;
	u16 MajorKeyVer;
	u16 MinorKeyVer;
	u32 TableSize;
	u32 EntryNum;
	u8 SBPubKey[PKEY_MAX_LEN];
	CertEntry_TypeDef PKInfo[CERT_MAX_KEYINFO];
	// Signature
} Certificate_TypeDef;

void BOOT_ImgCopy(void *__restrict dst0, const void *__restrict src0, size_t len0);
void BOOT_OTFMaskConfig(void);
u8 BOOT_LoadRDPImg(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u32 SrcAddr);
u8 BOOT_SignatureCheck(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum, Certificate_TypeDef *Cert, u32 KeyID);
u8 BOOT_CertificateCheck(Certificate_TypeDef *Cert, u32 addr);
#endif

