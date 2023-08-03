/**
  ******************************************************************************
  * @file    ameba_ota.c
  * @author
  * @version V1.0.0
  * @date    2023-07-13
  * 
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


#include "rtl_se_crypto_function.h"
#include "bootloader_hp.h"
#define TIZENRT_KERNEL_HEADER_LEN 0x10
Certificate_TypeDef Cert;
SubImgInfo_TypeDef SubImgInfo[12];
static int SBOOT_Validate_Algorithm_OTA(u8 *AuthAlg, u8 *HashAlg, u8 ManiAuth, u8 ManiHash)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER algo_info;
	(&algo_info)->AuthAlg = AuthAlg;
	(&algo_info)->HashAlg = HashAlg;
	(&algo_info)->ManiAuth = ManiAuth;
	(&algo_info)->ManiHash = ManiHash;
	ret = ameba_SBOOT_Validate_Algorithm(&algo_info);
	return ret;
}
static int SBOOT_Validate_Signature_OTA(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER sig_info;
	(&sig_info)->AuthAlg = AuthAlg;
	(&sig_info)->HashAlg = HashAlg;
	(&sig_info)->Pk = Pk;
	(&sig_info)->Msg = Msg;
	(&sig_info)->Len = Len;
	(&sig_info)->Sig = Sig;
	ret = ameba_SBOOT_Validate_Signature(&sig_info);
	
	return ret;
}

static int SBOOT_Validate_ImgHash_OTA(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
{
	int ret;
	OTA_SIG_CHECK_ADAPTER hash_info;
	
	(&hash_info)->HashAlg = HashAlg;
	(&hash_info)->ImgHash = ImgHash;
	(&hash_info)->SubImgInfo = SubImgInfo;
	(&hash_info)->Num = Num;
	ret = ameba_SBOOT_Validate_ImgHash(&hash_info);
	
	return ret;
}

static int SBOOT_Validate_PubKey_OTA(u8 AuthAlg, u8 *Pk, u8 *Hash)
{

	int ret;
	OTA_SIG_CHECK_ADAPTER pkey_info;
	(&pkey_info)->AuthAlg = AuthAlg;
	(&pkey_info)->Pk = Pk;
	(&pkey_info)->Hash = Hash;
	ret = ameba_SBOOT_Validate_PubKey(&pkey_info);
	
	return ret;

}

static void BOOT_ImgCopy_OTA(void *__restrict dst0, const void *__restrict src0, size_t len0)
{
	_memcpy(dst0, src0, len0);
}


/* User image verification*/
static int User_SignatureCheck_OTA(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum)
{
	int ret;
	u8 AuthAlg, HashAlg;
	
	/*1. verify signature*/
	/*1.1 Initialize hash engine*/
	
	
	/* 1.2 Check algorithm from manifest against OTP configuration if need. */

	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Manifest->AuthAlg, Manifest->HashAlg);
	
	if (ret != 0) {
		goto SBOOT_FAIL;
	}
	
	
	/* 1.3 validate signature */

	ret = SBOOT_Validate_Signature_OTA(AuthAlg, HashAlg, Manifest->SBPubKey, (u8 *)Manifest, sizeof(Manifest_TypeDef) - SIGN_MAX_LEN, (u8 *)Manifest->Signature);

	if (ret != 0) {
		printf ("%s User image validate Signature failed\n", __func__);
		goto SBOOT_FAIL;
	}

	/* 1.4 calculate and validate image hash */

	ret = SBOOT_Validate_ImgHash_OTA(HashAlg, (u8 *)Manifest->ImgHash, SubImgInfo, SubImgNum);

	if (ret != 0) {
		printf ("%s User image validate ImgHash failed\n",__func__);
		goto SBOOT_FAIL;
	}
	printf("USER IMAGE VERIFY PASS\n");
	return TRUE;
	
SBOOT_FAIL:
	printf("USER IMAGE VERIFY FAIL, ret = %d\n", ret);
	return FALSE;	

}

static int BOOT_LoadSubImage_OTA(SubImgInfo_TypeDef *SubImgInfo, u32 StartAddr, u8 Num, char **ImgName, u8 ErrLog)
{
	IMAGE_HEADER ImgHdr;
	u32 DstAddr, Len;
	u32 i;

	for (i = 0; i < Num; i++) {
		BOOT_ImgCopy_OTA((void *)&ImgHdr, (void *)StartAddr, IMAGE_HEADER_LEN);

		if ((ImgHdr.signature[0] != 0x35393138) || (ImgHdr.signature[1] != 0x31313738)) {
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
		}

		StartAddr += Len;
	}

	return TRUE;
}




/* Kernel image verification*/

static int Kernel_SignatureCheck_OTA(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum, Certificate_TypeDef *Cert, u32 KeyID)
{	
	int ret;
	u8 i;
	char *Name[] = {"IMG1", "IMG2", "IMG3", "AP BL1&FIP"};
	u8 AuthAlg, HashAlg;
	
	if (SYSCFG_OTP_SBootEn() == FALSE) {
	
		return FALSE;
	}


	/* 2.2 Check algorithm from manifest against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Manifest->AuthAlg, Manifest->HashAlg);

	if (ret != 0) {
		
		printf ("%s Kernel image validate Algorithm failed\n",__func__);
		goto SBOOT_FAIL;
	}

	/* 2.3 validate pubkey hash */
	for (i = 0; i < Cert->EntryNum; i++) {
		if (Cert->PKInfo[i].KeyID == KeyID) {
			break;
		}
	}
	ret = SBOOT_Validate_PubKey_OTA(AuthAlg, Manifest->SBPubKey, Cert->PKInfo[i].Hash);

	if (ret != 0) {
		printf ("%s Kernel image validate PubKey failed\n",__func__);
		goto SBOOT_FAIL;
	}

	/* 2.4 validate signature */

	
	ret = SBOOT_Validate_Signature_OTA(AuthAlg, HashAlg, Manifest->SBPubKey, (u8 *)Manifest, sizeof(Manifest_TypeDef) - SIGN_MAX_LEN, Manifest->Signature);

	if (ret != 0) {
		printf ("%s Kernel image validate Signature failed\n",__func__);
		goto SBOOT_FAIL;
	}

	/* 2.5 calculate and validate image hash */
		
	ret = SBOOT_Validate_ImgHash_OTA(HashAlg, Manifest->ImgHash, SubImgInfo, SubImgNum);

	if (ret != 0) {
		printf ("%s Kernel image validate ImgHash failed\n",__func__);
		goto SBOOT_FAIL;
	}

	printf("%s VERIFY PASS\n", Name[Manifest->ImgID]);
	return TRUE;

SBOOT_FAIL:
	printf("%s VERIFY FAIL, ret = %d\n", Name[Manifest->ImgID], ret);
	for (int i = 0; i < 32; i ++){
		printf("%02x",*(Manifest->SBPubKey + i));
	}
	return FALSE;
}

static int Kernel_CertificateCheck_OTA(Certificate_TypeDef *Cert, u32 Addr)
{
	u8 Signature[SIGN_MAX_LEN];
	u8 PubKeyHash[32];
	u8 i;
	int ret = 0;
	u8 AuthAlg, HashAlg;

	/* 1. check if secure boot enable. */
	if (SYSCFG_OTP_SBootEn() == FALSE) {
	
		return FALSE;
	}

	BOOT_ImgCopy_OTA(Signature, (void *)(Addr + Cert->TableSize), SIGN_MAX_LEN);

	/* 2. read public key hash from OTP.*/
	
	for (i = 0; i < 32; i++) {
	
		PubKeyHash[i] = HAL_READ8(OTPC_REG_BASE, Cert_PKHash_OTP_ADDR + i);
	}

	/* 3. verify signature */


	/* 3.2 Check algorithm from flash against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Cert->AuthAlg, Cert->HashAlg);
	if (ret != 0) {
		printf ("%s Kernel image validate Algorithm failed\n",__func__);
		goto SBOOT_FAIL;
	}

	/* 3.3 validate pubkey hash */
	ret = SBOOT_Validate_PubKey_OTA(AuthAlg, Cert->SBPubKey, PubKeyHash);

	if (ret != 0) {
		printf ("%s Kernel image validate PubKey failed\n",__func__);
		goto SBOOT_FAIL;
	}

	/* 3.4 validate signature */
	ret = SBOOT_Validate_Signature_OTA(AuthAlg, HashAlg, Cert->SBPubKey, (u8 *)Cert, Cert->TableSize, Signature);
	if (ret != 0) {
		printf ("%s Kernel image validate Signature failed\n",__func__);
		goto SBOOT_FAIL;
	}

	return TRUE;

SBOOT_FAIL:
	printf("Certificate VERIFY FAIL, ret = %d\n", ret);
	
	
	return FALSE;
}


int OTA_UserImageSignatureCheck(uint32_t input_addr)
{
	
	SubImgInfo_TypeDef SubImgInfo;

	Manifest_TypeDef Manifest;

	u32 PhyAddr;
	u8 Index;
	
	PhyAddr = (u32)input_addr;
	
	/*load manifest to SRAM*/
	BOOT_ImgCopy_OTA((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));
	
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	
	SubImgInfo.Addr = PhyAddr;
	
	SubImgInfo.Len = Manifest.ImgSize;
	
	Index = 1;
	
	if (User_SignatureCheck_OTA(&Manifest, &SubImgInfo, Index) == FALSE)
		return FALSE;
		
	return TRUE;

}

static int BOOT_OTA_AP (u32 addr)
{
	u8 Cnt;
	u8 ret;
	Manifest_TypeDef Manifest;
	
	char *APLabel[] = {"AP BL1 SRAM", "AP BL1 DRAM", "AP FIP"};
	u32 PhyAddr = (u32) addr;
	BOOT_ImgCopy_OTA((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));
	Cnt = sizeof(APLabel) / sizeof(char *);
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	
	if (BOOT_LoadSubImage_OTA(&SubImgInfo, PhyAddr, Cnt, APLabel, TRUE) == TRUE) {
				/* AP ECC verify if need */
		if (Kernel_SignatureCheck_OTA(&Manifest, SubImgInfo, Cnt, &Cert, KEYID_AP) == FALSE){
			return FALSE;
		}
	} else {
			printf("%s Load OTA_AP image fail\r\n",__func__);
			return FALSE;
		}



}

int OTA_KernelImageSignatureCheck(uint32_t input_addr)
{
	u32 ImgAddr = 0;
	u32 TotalLen = 0; 
	u32 TotalLen_KM4 = 0;
	
	Manifest_TypeDef Manifest;
	u8 Cnt;
	u8 i;
	u32 Index = 0;
	
	char *Km0Label[] = {"KM0 XIP IMG", "KM0 SRAM", "KM0 DRAM"};
	char *Km4Label[] = {"KM4 XIP IMG", "KM4 SRAM", "KM4 DRAM"};

	u32 PhyAddr = input_addr + TIZENRT_KERNEL_HEADER_LEN;
	
	BOOT_ImgCopy_OTA((void *)&Cert, (void *)PhyAddr, sizeof(Certificate_TypeDef));
	
	/*ECC check certificate*/
	if (Kernel_CertificateCheck_OTA(&Cert, PhyAddr) == FALSE) {
		return FALSE;
	}
	PhyAddr += CERT_SIZE_4K_ALIGN - TIZENRT_KERNEL_HEADER_LEN;
	BOOT_ImgCopy_OTA((void *)&Manifest, (void *)PhyAddr, sizeof(Manifest_TypeDef));



	/* remap KM0 XIP image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;



	/* KM0 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km0Label) / sizeof(char *);
	ImgAddr = PhyAddr;

	if (BOOT_LoadSubImage_OTA(&SubImgInfo[Index], ImgAddr, Cnt, Km0Label, TRUE) == FALSE) {
		return FALSE;
	}
	SubImgInfo[Index].Addr = PhyAddr;

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

	/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km4Label) / sizeof(char *);
	ImgAddr = PhyAddr;

	if (BOOT_LoadSubImage_OTA(&SubImgInfo[Index], ImgAddr, Cnt, Km4Label, TRUE) == FALSE) {
		return FALSE;
	}
	SubImgInfo[Index].Addr = PhyAddr;

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
	
	
	/* IMG2 ECC verify if need */
	if (Kernel_SignatureCheck_OTA(&Manifest, SubImgInfo, Index, &Cert, KEYID_NSPE) == FALSE) {
	
		return FALSE;
	}
	BOOT_OTA_AP(PhyAddr);
	
	return TRUE;
}

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor *****END OF FILE****/
