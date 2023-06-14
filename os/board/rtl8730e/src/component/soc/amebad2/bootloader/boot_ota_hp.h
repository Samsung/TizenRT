/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

#define NONEVALIDIMG		0
#define ONEVALIDIMG		1
#define TWOVALIDIMG		2

#define SAMEVERSION		2

// IMG Type
typedef enum _IMG_TYPE_ {
	IMG_CERT   =   0x0,
	IMG_IMG2    =   0x1,
	IMG_APIMG    =   0x2,
} IMG_TYPE, *PIMG_TYPE;

_LONG_CALL_ u8 BOOT_OTA_LoadIMG2(u8 ImgIndex);
_LONG_CALL_ u8 BOOT_OTA_IMG2(void);
_LONG_CALL_ u8 BOOT_OTA_RDP(SubImgInfo_TypeDef *SubImgInfo, u8 Index, u8 ImgIndex);
_LONG_CALL_ u8 BOOT_OTA_AP(u8 CertImgIndex);