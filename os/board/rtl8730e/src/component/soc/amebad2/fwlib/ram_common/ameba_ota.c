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
static int SBOOT_Validate_Signature_OTA(u8 *AuthAlg, u8 *HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig)
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

static int SBOOT_Validate_ImgHash_OTA(u8 *HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
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



static void BOOT_ImgCopy_OTA(void *__restrict dst0, const void *__restrict src0, size_t len0)
{
	_memcpy(dst0, src0, len0);
}



static int User_SignatureCheck_OTA(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum)
{
	int ret;
	u8 i;
	u8 AuthAlg, HashAlg;
	
	/*1. verify signature*/
	/*1.1 Initialize hash engine*/
	
	
	/* 1.2 Check algorithm from manifest against OTP configuration if need. */

	ret = SBOOT_Validate_Algorithm_OTA(&AuthAlg, &HashAlg, Manifest->AuthAlg, Manifest->HashAlg);
	
	if (ret != 0) {
		goto SBOOT_FAIL;
	}
	
	
	/* 1.3 validate signature */

	ret = SBOOT_Validate_Signature_OTA(AuthAlg, HashAlg, Manifest->SBPubKey, (u8 *)Manifest, sizeof(Manifest_TypeDef) - SIGN_MAX_LEN, Manifest->Signature);

	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 1.4 calculate and validate image hash */

	ret = SBOOT_Validate_ImgHash_OTA(HashAlg, Manifest->ImgHash, SubImgInfo, SubImgNum);

	if (ret != 0) {
		goto SBOOT_FAIL;
	}
	printf("USER IMAGE VERIFY PASS\n");
	return TRUE;
	
SBOOT_FAIL:
	printf("USER IMAGE VERIFY FAIL, ret = %d\n", ret);
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




/******************* (C) COPYRIGHT 2023 Realtek Semiconductor *****END OF FILE****/
