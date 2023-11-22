/**
  ******************************************************************************
  * @file    ameba_ota.h
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

#ifndef _AMEBA_OTA_H_
#define _AMEBA_OTA_H_

#if defined(CONFIG_MBED_TLS_ENABLED) && (CONFIG_MBED_TLS_ENABLED == 1)
#include <mbedtls/config.h>
#include <mbedtls/platform.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>

#define HTTPS_OTA_UPDATE
#endif

#define HTTP_OTA_UPDATE

/** @addtogroup Ameba_Platform
  * @{
  */
#define OTA_CLEAR_PATTERN	0

#define BUF_SIZE			2048								/*the size of the buffer used for receiving firmware data from server*/
#define SD_OTA_BUF_SIZE		512
#define HEADER_BAK_LEN		32
#define HEADER_LEN			8
#define SUB_HEADER_LEN		24


/* Exported constants --------------------------------------------------------*/

/** @defgroup OTA_Exported_Constants OTA Exported Constants
  * @{
  */
/** @defgroup OTA_system_parameter_definitions
  * @{
  */
#define MAX_IMG_NUM			2

#define OTA_IMGID_BOOT		0
#define OTA_IMGID_APP		1
#define OTA_IMGID_APIMG		2
#define OTA_IMGID_MAX		3

#define OTA_IMAG			0								/*identify the OTA image*/

#define OTA_LOCAL			0
#define OTA_HTTP			1
#define OTA_HTTPS			2
#define OTA_SDCARD			3

/**
  * @}
  */

/**
  * @}
  */


/** @defgroup OTA
  * @brief OTA driver modules
  * @{
  */

/* Exported Types --------------------------------------------------------*/
/** @defgroup OTA_Exported_Types OTA Exported Types
  * @{
  */

/**
  * @brief  OTA http response result structure definition
  */
typedef struct {
	u32	status_code;
	u32	header_len;
	u8	*body;
	u32	body_len;
	u8	*header_bak;
	u32	parse_status;
} http_response_result_t;


/**
  * @brief  OTA firmware file header structure definition
  */
typedef struct {
	u32	FwVer;	/*!< Specifies the OTA firmware verision.
	                         This parameter is in first Dword in the firmware file. */
	u32	HdrNum;/*!< Specifies the OTA firmware header number.
	                         This parameter indicates how many headers in firmware file. */
} update_file_hdr;

/**
  * @brief  OTA firmware file image header structure definition
  */
typedef struct {
	u8	Signature[4];	/*!< Specifies the OTA image signature, the value is "OTA".
	                         	This parameter is used to identify the OTA header needed. */
	u32	ImgHdrLen;	/*!< Specifies the OTA image header length.
	                         	This parameter indicates the Image Header Length. */
	u32	Checksum;	/*!< Specifies the OTA image checksum.
	                         	This parameter is used to judge whether the image received is correct. */
	u32  ImgLen;		/*!< Specifies the OTA image length. */
	u32  Offset;		/*!< Specifies the the location in the total firmware file. */
	u32  ImgID;    /*!< Specifies the image ID of the corresponding image. */
} update_file_img_hdr;

/**
  * @brief  OTA firmware file download information structure definition
  */
typedef struct {
	u32 ImgId;		/*!< Specifies the Image ID.*/

	u32	FlashAddr;	/*!< Specifies the Flash Address.
	                         	This parameter is used to write the Image to the flash. */
	u32	ImgOffset;	/*!< Specifies the Image location in Firmware header.
	                         	This parameter indicates the Image location in firmware file. */
	u32	ImageLen;	/*!< Specifies the OTA image length. */
} update_dw_info;

/**
  * @brief  OTA target image manifest structure definition
  */
typedef struct {
	u32 Pattern[2];
	u8 Rsvd1[8];
	u8 Ver;
	u8 ImgID;
	u8 AuthAlg;
	u8 HashAlg;
	u16 MajorImgVer;
	u16 MinorImgVer;
} update_manifest_info;


/**
  * @brief  OTA target image header structure definition
  */
typedef struct {
	update_file_hdr	FileHdr;			/*!< Specifies the firmware file header. */
	update_file_img_hdr	FileImgHdr[MAX_IMG_NUM];	/*!< Specifies the target OTA image firmware file header. */
	update_manifest_info Manifest[MAX_IMG_NUM];			/*!< Specifies the manifest of target image. */
	u8 ValidImgCnt;						/*!< Specifies valid image number in file. */
} update_ota_target_hdr;

/**
  * @brief  OTA target image download control information structure definition
  */
typedef struct {
	u8 NextImgFg;			/*!< Specifies the Flag that Buffer belong to next image. */
	u8 NextImgBuf[BUF_SIZE];/*!< Specifies the Buffer belong to next image. */
	int NextImgLen;			/*!< Specifies the Buffer Length belong to next image. */
	int ReadBytes;		/*!< Specifies the Bytes already received. */
	int RemainBytes;		/*!< Specifies the Remain Bytes of OTA firmware. */
	u32 ImgId;		/*!< Specifies the Image ID.*/
	u32 ImgOffset;		/*!< Specifies the Offset of Manifest in firmware. */
	u32	ImageLen;	/*!< Specifies the OTA image length. */
	u32 FlashAddr;			/*!< Specifies the Flash Address.
									This parameter is used to write the Image to the flash. */
	int SigCnt;		/*!< Specifies the Manifest received length. */
	u8 SigFg;		/*!< Specifies the Flag that Manifest received finished. */
	u8 SkipBootOTAFg;	/*!< Specifies the Flag that skip update the bootloader. */
	u8 FirstBufFg;		/*!< Specifies the Flag that exist a buffer before downloading. */
	u8 IsGetHdr;		/*!< Specifies the Flag that get ota target header. */
	u8 targetIdx;		/*!< Specifies the ota target index. */
	int index;			/*!< Specifies the current image index. */
} update_ota_ctrl_info;

/**
  * @brief  OTA ssl structure definition
  */
typedef struct {
#ifdef HTTPS_OTA_UPDATE
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
#else
	void *ssl;
	void *conf;
#endif
} update_tls;

/**
  * @brief  OTA sdcard structure definition
  */
typedef struct {
#ifdef SDCARD_OTA_UPDATE
	FATFS fs;
	FIL file;
#endif
	int offset;			/* file read offset is used for f_lseek  */
	int drv_num;
	char logical_drv[4]; /* root diretor */
} update_sdcard;

/**
  * @brief  OTA http redirect connection structure definition
  */
typedef struct {
	char *url;
	int len;
	u16 port;
	char *host;
	char *resource;
} update_redirect_conn;

/**
  * @brief  OTA context structure definition
  */
typedef struct {
	char *host;
	int port;
	char *resource;
	int fd;
	u8 type;
	int buflen;
	update_tls *tls;
	update_sdcard *sdcard;
	update_ota_ctrl_info *otactrl;
	update_redirect_conn *redirect;
	update_ota_target_hdr *otaTargetHdr;
} ota_context;


/* Exported functions --------------------------------------------------------*/
/** @defgroup OTA_Exported_Functions OTA Exported Functions
  * @{
  */
u32 ota_get_cur_index(u32 idx);
u32 verify_ota_checksum(update_ota_target_hdr *pOtaTgtHdr, u8 targetIdx, int index);
u32 ota_update_manifest(update_ota_target_hdr *pOtaTgtHdr, u32 ota_target_index, int index);
int ota_update_init(ota_context *ctx, char *host, int port, char *resource, u8 type);
void ota_update_deinit(ota_context *ctx);
int ota_update_start(ota_context *ctx);

#define OTA_GET_FWVERSION(address) \
	(HAL_READ16(SPI_FLASH_BASE, address + 22) << 16) | HAL_READ16(SPI_FLASH_BASE, address + 20)

#define _OTA_INFO_	3
#define _OTA_WARN_	2
#define _OTA_ERR_	1

#define ota_printf(level, fmt, arg...)     \
do {\
	if(level == _OTA_INFO_) \
		printf("\r\n[OTA INFO]: " fmt, ##arg);\
	else if(level == _OTA_WARN_) \
		printf("\r\n[OTA WRAN]: " fmt, ##arg);\
	else if(level == _OTA_ERR_) \
		printf("\r\n[OTA ERROR]: " fmt, ##arg);\
}while(0)

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif //_AMEBA_OTA_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
