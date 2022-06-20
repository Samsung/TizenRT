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
#define KEYID_DSP				2

/* This is used as a signature to validate the blob header */
#define TOC_HEADER_NAME	0xAA640001

/* Length of a node address (an IEEE 802 address). */
#define	_UUID_NODE_LEN		6

#define _FIP_MAX_SIZE		0x00100000

struct uuid {
	uint8_t		time_low[4];
	uint8_t		time_mid[2];
	uint8_t		time_hi_and_version[2];
	uint8_t		clock_seq_hi_and_reserved;
	uint8_t		clock_seq_low;
	uint8_t		node[_UUID_NODE_LEN];
};

/* XXX namespace pollution? */
typedef struct uuid uuid_t;

typedef struct fip_toc_header {
	uint32_t	name;
	uint32_t	serial_number;
	uint64_t	flags;
} fip_toc_header_t;

typedef struct fip_toc_entry {
	uuid_t		uuid;
	uint64_t	offset_address;
	uint64_t	size;
	uint64_t	flags;
} fip_toc_entry_t;

typedef struct {
	unsigned int KeyID;
	unsigned char Hash[32];

} CertEntry_TypeDef;

typedef struct {
	unsigned char Pattern[8];
	unsigned char Rsvd1[8];
	unsigned char Ver;
	unsigned char ImgID;
	unsigned char AuthAlg;
	unsigned char HashAlg;
	unsigned int KeyVer;
	unsigned int TableSize;
	unsigned int EntryNum;
	unsigned char SBPubKey[PKEY_MAX_LEN];
	CertEntry_TypeDef PKInfo[CERT_MAX_KEYINFO];
	// Signature
} Certificate_TypeDef;

void BOOT_ImgCopy(void *__restrict dst0, const void *__restrict src0, size_t len0);
void BOOT_OTFMaskConfig(void);
int BOOT_LoadRDPImg(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo);
void BOOT_SignatureCheck(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum, Certificate_TypeDef *Cert, u32 KeyID);
void BOOT_CertificateCheck(Certificate_TypeDef *Cert, u32 addr);
#endif

