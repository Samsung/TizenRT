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

/**
  * @brief  Set OTA image index.
  * @param  OTAIdx: This parameter can be  one of the following values:
  *            @arg OTA_INDEX_1: select OTA1(low flash address) as image2
  *            @arg OTA_INDEX_2: select OTA2(high flash address) as image2
  * @retval status value:
  *          - 1: set ok
  *          - 0: set fail
  */
u32 OTA_Change(u32 OTAIdx)
{
	u32 BitIdx = 0;
	u32 ota_sig[2];
	int ver[2] = {0};
	int i = 0;
	int ImgID = 0;
	u32 Address;
	u32 ota_addr[2];

	for (ImgID = 1; ImgID < 2; ImgID++) {

		for (i = 0; i < 2; i++) {
			Address = IMG_ADDR[ImgID][i] - SPI_FLASH_BASE;
			if (ImgID == OTA_IMGID_IMG2) {
				Address = Address + 0x1000; /* skip certificate */
			}
			ota_sig[0] = HAL_READ32(SPI_FLASH_BASE, Address);
			ota_sig[1] = HAL_READ32(SPI_FLASH_BASE, Address + 4);
			if (0x35393138 == ota_sig[0] && 0x31313738 == ota_sig[1]) {
				ver[i] = OTA_GET_FWVERSION(Address);
			} else {
				ver[i] = -1;
			}
		}

		BitIdx = (ver[0] > ver[1]) ? 0 : 1;

		ota_sig[0] = 0x35393138;
		ota_sig[1] = 0x31313738;

		/* 0 currrent is OTA1 */
		/* 1 currrent is OTA2 */
		if (BitIdx == OTAIdx) {
			DBG_8195A("[%s] IMGID: %d, currrent is OTA %d, select OTA %d \n", __func__, ImgID, BitIdx + 1, OTAIdx + 1);
		} else {
			if (ver[OTAIdx] < 0) {
				DBG_8195A("[%s] IMGID: %d, currrent is OTA %d, select OTA %d is invalid\n", __func__, ImgID, BitIdx + 1, OTAIdx + 1);
				return _FALSE;
			}
			DBG_8195A("[%s] IMGID: %d, currrent is OTA %d, select OTA %d \n", __func__, ImgID, BitIdx + 1, OTAIdx + 1);
			/* exchange ota1 version and ota2 version */
			ota_addr[OTA_INDEX_1] = IMG_ADDR[ImgID][OTA_INDEX_1] - SPI_FLASH_BASE;
			ota_addr[OTA_INDEX_2] = IMG_ADDR[ImgID][OTA_INDEX_2] - SPI_FLASH_BASE;

			if (ImgID == OTA_IMGID_IMG2) {
				ota_addr[OTA_INDEX_1] = ota_addr[OTA_INDEX_1] + 0x1000; /* skip certificate */
				ota_addr[OTA_INDEX_2] = ota_addr[OTA_INDEX_2] + 0x1000; /* skip certificate */
			}

			FLASH_EreaseDwordsXIP(ota_addr[OTA_INDEX_1] + 20, 1);
			FLASH_EreaseDwordsXIP(ota_addr[OTA_INDEX_2] + 20, 1);
			FLASH_TxData12BXIP(ota_addr[OTA_INDEX_1] + 20, 4, (u8 *)&ver[OTA_INDEX_2]);
			FLASH_TxData12BXIP(ota_addr[OTA_INDEX_2] + 20, 4, (u8 *)&ver[OTA_INDEX_1]);
		}
	}

	return TRUE;
}

#include "ameba_soc.h"
#include "ameba_secure_boot.h"
#include "bootloader_km4.h"

extern u8 RMA_PK_HASH[32];
extern u32 Cert_PKHash_OTP_ADDR;
u32 Img3Addr= 0;

u32 Cert_PKHash_OTP_ADDR = SEC_PKKEY_PK1_0;

#define TIZENRT_KERNEL_HEADER_LEN 0x10

typedef struct {
	u8 *AuthAlg;
	u8 *HashAlg;
	u8 ManiAuth;
	u8 ManiHash;
	u8 *Pk;
	u8 *Msg;
	u32 Len;
	u8 *Sig;
	u8 *Hash;
	u8 *ImgHash;
	SubImgInfo_TypeDef *SubImgInfo;
	u8 Num;
} OTA_SIG_CHECK_ADAPTER,*POTA_SIG_CHECK_ADAPTER;

void BOOT_ImgCopy_OTA(void *__restrict dst0, const void *__restrict src0, size_t len0)
{
	_memcpy(dst0, src0, len0);
}

u8 BOOT_LoadSubImage_OTA(SubImgInfo_TypeDef *SubImgInfo, u32 StartAddr, u8 Num, char **ImgName, u8 ErrLog)
{
	IMAGE_HEADER ImgHdr;
	u32 DstAddr, Len;
	u32 i;

	for (i = 0; i < Num; i++) {
		BOOT_ImgCopy_OTA((void *)&ImgHdr, (void *)StartAddr, IMAGE_HEADER_LEN);

		if ((ImgHdr.signature[0] != 0x35393138) || (ImgHdr.signature[1] != 0x31313738)) {
			if (ErrLog == TRUE) {
				DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s Invalid\n", ImgName[i]);
			}
			return FALSE;
		}

		DstAddr = StartAddr;
		Len = ImgHdr.image_size + IMAGE_HEADER_LEN;

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

	return TRUE;
}

int BOOT_LoadRDPImg_OTA(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo)
{
	IMAGE_HEADER *Hdr = NULL;
	u8 InBuf[256], Outbuf[16];
	u8 Key[32], TmpIV[16];
	u32 DestAddr, TotalLen;
	u32 SrcAddr = (u32)Img3Addr;
	u32 i = 0, sz;

	char *Km4Img3Label[2] = {"KM4 IMG3", "KM4 NSC"};

	/* check if RDP enable */
	if (SYSCFG_OTP_RDPEn() == FALSE) {
		//return FALSE;
	}

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "RDP EN\n");
	/* load img3 manifest to SRAM */
	BOOT_ImgCopy_OTA((void *)Manifest, (void *)SrcAddr, sizeof(Manifest_TypeDef));
	SrcAddr += MANIFEST_SIZE_4K_ALIGN;

	/* check if RDP image should decrypt. If not, copy to secure RAM directly */
	if (BOOT_LoadSubImage_OTA(SubImgInfo, SrcAddr, 2, Km4Img3Label, FALSE) == TRUE) {
		return TRUE;
	}

	/* Initialize AES engine */
	CRYPTO_Init(NULL);

	/* Load RDP key to IPSEC */
	CRYPTO_OTPKey_Init(0, ENABLE);

	/* Key array not used, pass in for API restrict */
	rtl_crypto_aes_cbc_init(Key, 32);

	_memcpy(TmpIV, Manifest->RsipIV, 16);

	for (i = 0; i < 2; i++) {

		/* decrypt header to get image addr and size */
		BOOT_ImgCopy_OTA((void *)InBuf, (void *)SrcAddr, 16);
		rtl_crypto_aes_cbc_decrypt(InBuf, 16, TmpIV, 16, Outbuf);

		Hdr = (IMAGE_HEADER *)Outbuf;
		if ((Hdr->signature[0] != 0x35393138) || (Hdr->signature[1] != 0x31313738)) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s Invalid\n", Km4Img3Label[i]);
			goto RDP_FAIL;
		}

		DestAddr = Hdr->image_addr - IMAGE_HEADER_LEN;
		TotalLen = Hdr->image_size + IMAGE_HEADER_LEN;
		DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s[%08x:%x]\n", Km4Img3Label[i], DestAddr, TotalLen);

		if (SubImgInfo != NULL) {
			SubImgInfo[i].Addr = DestAddr;
			SubImgInfo[i].Len = TotalLen;
		}

		/* decrypt image by segment */
		while (TotalLen > 0) {
			sz = TotalLen > 256 ? 256 : TotalLen;

			BOOT_ImgCopy_OTA((void *)InBuf, (void *)SrcAddr, sz);
			rtl_crypto_aes_cbc_decrypt(InBuf, sz, TmpIV, 16, (u8 *)DestAddr);

			/* store IV for next block */
			_memcpy(TmpIV, InBuf + sz - 16, 16);

			TotalLen -= sz;
			DestAddr += sz;
			SrcAddr += sz;
		}
	}

	CRYPTO_OTPKey_Init(0, DISABLE);
	return TRUE;

RDP_FAIL:
	DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "Fail to load RDP image!\n");
	return FALSE;
}

int BOOT_Validate_PubKey_OTA(u8 *AuthAlg, u8 *Pk, u8 *Hash)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER pkey_info;
	up_allocate_secure_context(4096);
	(&pkey_info)->AuthAlg = AuthAlg;
	(&pkey_info)->Pk = Pk;
	(&pkey_info)->Hash = Hash;
	ret = SBOOT_Validate_PubKey_NSC(&pkey_info);
	up_free_secure_context();
	return ret;
}

int BOOT_Validate_Signature_OTA(u8 *AuthAlg, u8 *HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER sig_info;
	up_allocate_secure_context(4096);
	(&sig_info)->AuthAlg = AuthAlg;
	(&sig_info)->HashAlg = HashAlg;
	(&sig_info)->Pk = Pk;
	(&sig_info)->Msg = Msg;
	(&sig_info)->Len = Len;
	(&sig_info)->Sig = Sig;
	ret = SBOOT_Validate_Signature_NSC(&sig_info);
	up_free_secure_context();
	return ret;
}

int BOOT_Validate_ImgHash_OTA(u8 *HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER hash_info;
	up_allocate_secure_context(4096);
	(&hash_info)->HashAlg = HashAlg;
	(&hash_info)->ImgHash = ImgHash;
	(&hash_info)->SubImgInfo = SubImgInfo;
	(&hash_info)->Num = Num;
	ret = SBOOT_Validate_ImgHash_NSC(&hash_info);
	up_free_secure_context();
	return ret;
}

int SBOOT_Validate_Algorithm_OTA(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER algo_info;
	up_allocate_secure_context(4096);
	(&algo_info)->AuthAlg = AuthAlg;
	(&algo_info)->HashAlg = HashAlg;
	(&algo_info)->ManiAuth = ManiAuth;
	(&algo_info)->ManiHash = ManiHash;
	ret = SBOOT_Validate_Algorithm_NSC(&algo_info);
	up_free_secure_context();
	return ret;
}

int BOOT_SignatureCheck_OTA(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum, Certificate_TypeDef *Cert, u32 KeyID)
{
	int ret;
	u8 i;
	char *Name[] = {"IMG1", "IMG2", "IMG3", "AP BL1"};
	u8 AuthAlg, HashAlg;

	/* 1. check if secure boot enable. */
	if (SYSCFG_OTP_SBootEn() == FALSE) {
		return TRUE;
	}

	/* 2. verify signature */
	/* 2.1 Initialize hash engine */
	CRYPTO_SHA_Init(NULL);

	/* 2.2 Check algorithm from manifest against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Manifest->AuthAlg, Manifest->HashAlg);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.3 validate pubkey hash */
	for (i = 0; i < Cert->EntryNum; i++) {
		if (Cert->PKInfo[i].KeyID == KeyID) {
			break;
		}
	}
	ret = BOOT_Validate_PubKey_OTA(&AuthAlg, Manifest->SBPubKey, Cert->PKInfo[i].Hash);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.4 validate signature */
	ret = BOOT_Validate_Signature_OTA(&AuthAlg, &HashAlg, Manifest->SBPubKey, (u8 *)Manifest, sizeof(Manifest_TypeDef) - SIGN_MAX_LEN,
								  Manifest->Signature);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.5 calculate and validate image hash */
	ret = BOOT_Validate_ImgHash_OTA(&HashAlg, Manifest->ImgHash, SubImgInfo, SubImgNum);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	DBG_8195A("%s VERIFY PASS\n", Name[Manifest->ImgID]);
	return TRUE;

SBOOT_FAIL:
	DBG_8195A("%s VERIFY FAIL, ret = %d\n", Name[Manifest->ImgID], ret);
	return FALSE;
}

int BOOT_CertificateCheck_OTA(Certificate_TypeDef *Cert, u32 Addr)
{
	u8 Signature[SIGN_MAX_LEN];
	u8 PubKeyHash[32];
	u8 i, j;
	int ret;
	u8 AuthAlg, HashAlg;

	/* 1. check if secure boot enable. */
	if (SYSCFG_OTP_SBootEn() == FALSE) {
		return TRUE;
	}

	BOOT_ImgCopy_OTA(Signature, (void *)(Addr + Cert->TableSize), SIGN_MAX_LEN);

	/* 2. read public key hash from OTP. Start with a random index to avoid side channel attack. */
	j = _rand() % 32;
	for (i = 0; i < 32; i++, j = (j + 1) % 32) {
		PubKeyHash[j] = HAL_READ8(OTPC_REG_BASE, Cert_PKHash_OTP_ADDR + j);
	}

	/* 3. verify signature */
	/* 3.1 Initialize hash engine */
	CRYPTO_SHA_Init(NULL);

	/* 3.2 Check algorithm from flash against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Cert->AuthAlg, Cert->HashAlg);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 3.3 validate pubkey hash */
	ret = BOOT_Validate_PubKey_OTA(&AuthAlg, Cert->SBPubKey, PubKeyHash);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 3.4 validate signature */
	ret = BOOT_Validate_Signature_OTA(&AuthAlg, &HashAlg, Cert->SBPubKey, (u8 *)Cert, Cert->TableSize, Signature);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	return TRUE;

SBOOT_FAIL:
	DBG_8195A("Certificate VERIFY FAIL, ret = %d\n", ret);
	return FALSE;
}

//IMAGE2_RAM_TEXT_SECTION
int OTA_Image2SignatureCheck(uint32_t input_addr)
{
	SubImgInfo_TypeDef SubImgInfo[12];
	Certificate_TypeDef Cert;
	Manifest_TypeDef Manifest;
	u32 LogAddr, PhyAddr, ImgAddr, TotalLen = 0, TotalLen_KM4 = 0;
	u8 Index = 0, Cnt, i;
	int ret;

	char *Km0Label[] = {"KR4 XIP IMG", "KR4 PSRAM", "KR4 DATA"};
	char *Km4Label[] = {"KM4 XIP IMG", "KM4 PSRAM", "KM4 SRAM"};

	//DiagPrintf("OTA address: 0x%x\r\n", input_addr);
	PhyAddr = (u32)input_addr+0x10;

	BOOT_ImgCopy_OTA((void *)&Cert, (void *)PhyAddr, sizeof(Certificate_TypeDef));

	/* ECC check certificate */

	if (BOOT_CertificateCheck_OTA(&Cert, PhyAddr) == FALSE)
		return FALSE;

	/* load manifest to SRAM */
	PhyAddr += CERT_SIZE_4K_ALIGN - TIZENRT_KERNEL_HEADER_LEN;
	BOOT_ImgCopy_OTA((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));

	/* remap KM0 XIP image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	LogAddr = (u32)__kr4_flash_text_start__ - IMAGE_HEADER_LEN;

	/* KM0 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km0Label) / sizeof(char *);
	ImgAddr = PhyAddr;
	if (BOOT_LoadSubImage_OTA(&SubImgInfo[Index], ImgAddr, Cnt, Km0Label, TRUE) == FALSE) {
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

	/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km4Label) / sizeof(char *);
	ImgAddr = PhyAddr;
	if (BOOT_LoadSubImage_OTA(&SubImgInfo[Index], ImgAddr, Cnt, Km4Label, TRUE) == FALSE) {
		return FALSE;
	}

	for (i = 0; i < Cnt; i++) {
			TotalLen_KM4 += SubImgInfo[Index+i].Len;
		}

	Index += Cnt;
	/* add 4K-align padding info */
	SubImgInfo[Index].Addr = ImgAddr + TotalLen_KM4;
	SubImgInfo[Index].Len = ((((TotalLen_KM4 - 1) >> 12) + 1) << 12) - TotalLen_KM4;


	TotalLen_KM4 += SubImgInfo[Index].Len;
	Index++;
	/*remap image3 */
	PhyAddr += TotalLen_KM4;
	Img3Addr = PhyAddr;

	/* IMG2 ECC verify if need */
	if (BOOT_SignatureCheck_OTA(&Manifest, SubImgInfo, Index, &Cert, KEYID_NSPE) == FALSE)
		return FALSE;

	/* Load RDP image if need */
	ret = BOOT_LoadRDPImg_OTA(&Manifest, SubImgInfo);
	if (ret == TRUE) {
		/* IMG3 ECC verify if need */
		if (BOOT_SignatureCheck_OTA(&Manifest, SubImgInfo, 2, &Cert, KEYID_SPE) == FALSE)
			return FALSE;
	}else{
		DiagPrintf("Load RDP image fail\r\n");
		return FALSE;
	}

	return TRUE;
}

/* Secure Key Derive Function structure */
typedef struct {
	unsigned char *password;	/* Password */
	size_t plen;			/* Password length */
	unsigned char *salt;		/* Salt */
	size_t slen;			/* Salt length */
	unsigned int iteration_count;
	unsigned char *dstbuf;		/* derived key buffer */
	size_t dlen;			/* derived key buffer length */
} secure_kdf_struc;

int Ameba_KeyDeriveFunc(const unsigned char *password, size_t plen,
			const unsigned char *salt, size_t slen,
			unsigned int iteration_count,
			unsigned char *dstbuf, size_t dlen)
{
	int ret;
	secure_kdf_struc KDF_input;

	if (dstbuf == NULL) /* output buff is empty */
		return FALSE;

	KDF_input.password = (unsigned char *)password;
	KDF_input.plen = plen;
	KDF_input.salt = (unsigned char *)salt;
	KDF_input.slen = slen;
	KDF_input.iteration_count = iteration_count;
	KDF_input.dstbuf = dstbuf;
	KDF_input.dlen = dlen;

	up_allocate_secure_context(4096);
	ret = Secure_KeyDeriveFunc(&KDF_input);	/* PBKDF2 API */
	up_free_secure_context();

	return ret;
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
