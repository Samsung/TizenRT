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

extern u8 RMA_PK_HASH[32];
extern u32 Cert_PKHash_OTP_ADDR;

u8 BOOT_LoadSubImage(SubImgInfo_TypeDef *SubImgInfo, u32 StartAddr, u8 Num, char **ImgName, u8 ErrLog);

BOOT_RAM_TEXT_SECTION
void BOOT_OTFMaskConfig(void)
{
	u32 BaseAddr, EndAddr, IVnum, Keysize, Mode;
	u8 i = 0;

	for (i = 0;; i++) {
		BaseAddr = RSIP_OTF_Config[i].BaseAddr;
		EndAddr = RSIP_OTF_Config[i].EndAddr;
		IVnum = RSIP_OTF_Config[i].IVNum;
		Keysize = RSIP_OTF_Config[i].KeySize;
		Mode = RSIP_OTF_Config[i].Type;

		UNUSED(Keysize);

		if (BaseAddr != 0xFFFFFFFF) {
			if (i > 7) {
				DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "There can be at most 8 RSIP mask entries!\n");
				break;
			}

			if ((BaseAddr >= SPI_FLASH_BASE) && (BaseAddr <= 0x0FFFFFFF)) {
				RSIP_OTF_Enable(i, BaseAddr, EndAddr, ENABLE, IVnum, Mode);
			} else {
				DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "RSIP entry addr %x invalid\n", BaseAddr);
			}
		} else {
			break;
		}
	}
}

BOOT_RAM_TEXT_SECTION
u8 BOOT_LoadRDPImg(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u32 SrcAddr)
{
	IMAGE_HEADER *Hdr = NULL;
	u8 InBuf[256], Outbuf[16];
	u8 Key[32], TmpIV[16];
	u32 DestAddr, TotalLen;
	u32 i = 0, sz;
	u8 Cnt = 0;

	char *Km4Img3Label[2] = {"KM4 IMG3", "KM4 NSC"};

	/* load img3 manifest to SRAM */
	BOOT_ImgCopy((void *)Manifest, (void *)SrcAddr, sizeof(Manifest_TypeDef));
	SrcAddr += MANIFEST_SIZE_4K_ALIGN;

	Cnt = sizeof(Km4Img3Label) / sizeof(char *);

	/* check if RDP image should decrypt. If not, copy to secure RAM directly */
	if (BOOT_LoadSubImage(SubImgInfo, SrcAddr, 2, Km4Img3Label, _FALSE) == _TRUE) {
		return Cnt;
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
		BOOT_ImgCopy((void *)InBuf, (void *)SrcAddr, 16);
		rtl_crypto_aes_cbc_decrypt(InBuf, 16, TmpIV, 16, Outbuf);

		Hdr = (IMAGE_HEADER *)Outbuf;
		if ((Hdr->signature[0] != 0x35393138) || (Hdr->signature[1] != 0x31313738)) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "%s Invalid\n", Km4Img3Label[i]);
			return FALSE;
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

			BOOT_ImgCopy((void *)InBuf, (void *)SrcAddr, sz);
			rtl_crypto_aes_cbc_decrypt(InBuf, sz, TmpIV, 16, (u8 *)DestAddr);

			/* store IV for next block */
			_memcpy(TmpIV, InBuf + sz - 16, 16);

			TotalLen -= sz;
			DestAddr += sz;
			SrcAddr += sz;
		}
	}

	CRYPTO_OTPKey_Init(0, DISABLE);
	return Cnt;
}

BOOT_RAM_TEXT_SECTION
int _BOOT_Validate_ImgHash(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
{
	hw_sha_context Context;
	SHA2_TYPE MdType;
	u8 CalcHash[HASH_MAX_LEN];
	u8 HashLen;
	u8 IsHmac = 0;
	u8 buf[512];
	u32 addr, size, t_size;
	u8 i, j, error = 0;
	int ret = 0;

	memset(&Context, 0, sizeof(hw_sha_context));

	ret = SBOOT_GetMdType(HashAlg, &MdType, &IsHmac);
	if (ret != 0) {
		goto exit;
	}

	if (IsHmac) {
		/* SHA engine auto-load HMAC key */
		CRYPTO_OTPKey_SHA_Init(1, ENABLE);

		CRYPTO_CHK(rtl_crypto_hmac_sha2_init(MdType, NULL, 32, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
		for (i = 0; i < Num; i++) {
			/* SHA engine shall not access Flash directly*/
			if (IS_FLASH_ADDR(SubImgInfo[i].Addr)) {
				size = SubImgInfo[i].Len;
				addr = SubImgInfo[i].Addr;

				while (size > 0) {
					t_size = (size > 512) ? 512 : size;

					BOOT_ImgCopy((void *)buf, (void *)addr, t_size);
					DCache_CleanInvalidate((u32)buf, t_size);

					CRYPTO_CHK(rtl_crypto_hmac_sha2_update(buf, t_size, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
					size -= t_size;
					addr += t_size;
				}
			} else {
				CRYPTO_CHK(rtl_crypto_hmac_sha2_update((u8 *)SubImgInfo[i].Addr, SubImgInfo[i].Len, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
			}
		}
		CRYPTO_CHK(rtl_crypto_hmac_sha2_final(CalcHash, &Context), SBOOT_ERR_IMG_HASH_ENGINE);

		CRYPTO_OTPKey_SHA_Init(1, DISABLE);
	} else {
		CRYPTO_CHK(rtl_crypto_sha2_init(MdType, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
		for (i = 0; i < Num; i++) {
			/* SHA engine shall not access Flash directly*/
			if (IS_FLASH_ADDR(SubImgInfo[i].Addr)) {
				size = SubImgInfo[i].Len;
				addr = SubImgInfo[i].Addr;

				while (size > 0) {
					t_size = (size > 512) ? 512 : size;

					BOOT_ImgCopy((void *)buf, (void *)addr, t_size);
					DCache_CleanInvalidate((u32)buf, t_size);

					CRYPTO_CHK(rtl_crypto_sha2_update(buf, t_size, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
					size -= t_size;
					addr += t_size;
				}
			} else {
				CRYPTO_CHK(rtl_crypto_sha2_update((u8 *)SubImgInfo[i].Addr, SubImgInfo[i].Len, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
			}
		}
		CRYPTO_CHK(rtl_crypto_sha2_final(CalcHash, &Context), SBOOT_ERR_IMG_HASH_ENGINE);
	}

	HashLen = (u8)MdType;

	/* Compare */
	j = _rand() % HashLen;
	for (i = 0; i < HashLen; i ++, j = (j + 1) % HashLen) {
		error |= CalcHash[j] ^ ImgHash[j];
	}

	if (error || i != HashLen) {
		ret = SBOOT_ERR_IMG_HASH_INVALID;
	}

exit:
	return ret;
}

BOOT_RAM_TEXT_SECTION
int BOOT_Validate_PubKey(u8 AuthAlg, u8 *Pk, u8 *Hash)
{
	return SBOOT_Validate_PubKey(AuthAlg, Pk, Hash);
}

BOOT_RAM_TEXT_SECTION
int BOOT_Validate_Signature(u8 AuthAlg, u8 HashAlg, u8 *Pk, u8 *Msg, u32 Len, u8 *Sig)
{
	return SBOOT_Validate_Signature(AuthAlg, HashAlg, Pk, Msg, Len, Sig);
}

BOOT_RAM_TEXT_SECTION
int BOOT_Validate_ImgHash(u8 HashAlg, u8 *ImgHash, SubImgInfo_TypeDef *SubImgInfo, u8 Num)
{
	if (SYSCFG_BootFromNor()) {
		return SBOOT_Validate_ImgHash(HashAlg, ImgHash, SubImgInfo, Num);
	} else {
		return _BOOT_Validate_ImgHash(HashAlg, ImgHash, SubImgInfo, Num);
	}
}

BOOT_RAM_TEXT_SECTION
u8 BOOT_SignatureCheck(Manifest_TypeDef *Manifest, SubImgInfo_TypeDef *SubImgInfo, u8 SubImgNum, Certificate_TypeDef *Cert, u32 KeyID)
{
	int ret;
	u8 i;
	char *Name[] = {"IMG1", "IMG2", "IMG3", "AP BL1&FIP"};
	u8 AuthAlg, HashAlg;

	/* 1. check if secure boot enable. */
	if (SYSCFG_OTP_SBootEn() == FALSE) {
		return TRUE;
	}

	/* 2. verify signature */
	/* 2.1 Initialize hash engine */
	CRYPTO_SHA_Init(NULL);

	/* 2.2 Check algorithm from manifest against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm(&AuthAlg, &HashAlg, Manifest->AuthAlg, Manifest->HashAlg);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.3 validate pubkey hash */
	for (i = 0; i < Cert->EntryNum; i++) {
		if (Cert->PKInfo[i].KeyID == KeyID) {
			break;
		}
	}
	ret = BOOT_Validate_PubKey(AuthAlg, Manifest->SBPubKey, Cert->PKInfo[i].Hash);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.4 validate signature */
	ret = BOOT_Validate_Signature(AuthAlg, HashAlg, Manifest->SBPubKey, (u8 *)Manifest, sizeof(Manifest_TypeDef) - SIGN_MAX_LEN,
								  Manifest->Signature);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 2.5 calculate and validate image hash */
	ret = BOOT_Validate_ImgHash(HashAlg, Manifest->ImgHash, SubImgInfo, SubImgNum);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	DBG_8195A("%s VERIFY PASS\n", Name[Manifest->ImgID]);
	return TRUE;

SBOOT_FAIL:
	DBG_8195A("%s VERIFY FAIL, ret = %d\n", Name[Manifest->ImgID], ret);

	/* clear copied image */
	for (i = 0; i < SubImgNum; i++) {
		if (!IS_FLASH_ADDR(SubImgInfo[i].Addr)) {
			_memset((void *)SubImgInfo[i].Addr, 0, SubImgInfo[i].Len);
			DCache_CleanInvalidate(SubImgInfo[i].Addr, SubImgInfo[i].Len);
		}
	}

	/* if clear here then it won't go back to ota flow, so clear stack in ota flow */
	return FALSE;
}

BOOT_RAM_TEXT_SECTION
u8 BOOT_CertificateCheck(Certificate_TypeDef *Cert, u32 Addr)
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

	BOOT_ImgCopy(Signature, (void *)(Addr + Cert->TableSize), SIGN_MAX_LEN);

	/* 2. read public key hash from OTP. Start with a random index to avoid side channel attack. */
	j = _rand() % 32;
	for (i = 0; i < 32; i++, j = (j + 1) % 32) {
		PubKeyHash[j] = HAL_READ8(OTPC_REG_BASE, Cert_PKHash_OTP_ADDR + j);
	}

	/* 3. verify signature */
	/* 3.1 Initialize hash engine */
	CRYPTO_SHA_Init(NULL);

	/* 3.2 Check algorithm from flash against OTP configuration if need. */
	ret = SBOOT_Validate_Algorithm(&AuthAlg, &HashAlg, Cert->AuthAlg, Cert->HashAlg);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 3.3 validate pubkey hash */
	ret = BOOT_Validate_PubKey(AuthAlg, Cert->SBPubKey, PubKeyHash);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	/* 3.4 validate signature */
	ret = BOOT_Validate_Signature(AuthAlg, HashAlg, Cert->SBPubKey, (u8 *)Cert, Cert->TableSize, Signature);
	if (ret != 0) {
		goto SBOOT_FAIL;
	}

	return TRUE;

SBOOT_FAIL:
	DBG_8195A("Certificate VERIFY FAIL, ret = %d\n", ret);

	/* if clear here then it won't go back to ota flow, so clear stack in ota flow */
	return FALSE;
}


