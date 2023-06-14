/**
  ******************************************************************************
  * @file    ameba_ota.c
  * @author
  * @version V1.0.0
  * @date    2016-12-19
  * @brief   This file contains the code which is used to update firmware over the air(OTA) in local area network
  *
  *  @verbatim
  *
  *          ===================================================================
  *                                 How to use the local OTA upgrade code
  *          ===================================================================
  *          1. Firstly, read the OTA related documents to know about how the code to realize updating firmware
  *              over the air(OTA), and some protocol included in it.
  *
  *          2. Read the source code and APIs in this file.
  *
  *          3. Porting this code in this file to the specified cloud platform according to the upgrade flow and parameters of
  *              the specified cloud service providers
  *
  *          4. Test the code after porting on the specified cloud platform.
  *
  *          5. Generate the release verision that will run.
  *
  *          ===================================================================
  *                                 the basic flow of the local ota upgrade code
  *          ===================================================================
  *          (1) Connects to server. The IP address and port are needed.
  *
  *			 (2) Acquire the older firmware address to be upgraded according to MMU setting. If address is re-mapping to OTA1
  *				  space by MMU, the OTA2 address would be selected to upgrade. Otherwise OTA1 address would be selected.
  *
  *          (3) Receive newer firmware file header from server to get the target OTA image information, such as image number,
  *				  image length and image ID.
  *
  *          (4) Download new firmware for server.
  *
  *          (5) Erase flash space for new firmware and write it into flash except Manifest structure.
  *
  *          (6) Verify checksum. If checksum error, OTA fail.
  *
  *          (7) If checksum ok, write Manifest structure to the upgraded firmware region.
  *
  *	         (8) OTA upgrade successfully, restart device
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <sys.h>

#include <device_lock.h>
#include "ameba_soc.h"
#include "lwipconf.h"

#include "osdep_service.h"
#include "flash_api.h"

extern u32 IMG_ADDR[3][2];
extern void sys_reset(void);

/**
  * @brief  Read a stream of data from specified address in user mode
  * @param obj: Flash object define in application software.
  * @param  address: Specifies the starting address to read from.
  * @param  len: Specifies the length of the data to read.
  * @param  data: Specified the address to save the readback data.
  * @retval   status: Success:1 or Failure: Others.
  * @note SPIC user mode is used because this mode can bypass RSIP(include OTF and MMU).
  *           User mode can read original data from physical address without decrypting, which is
  *           useful when calculate checksum.
  */
SRAMDRAM_ONLY_TEXT_SECTION
int ota_readstream_user(u32 address, u32 len, u8 *data)
{
	assert_param(data != NULL);

	u32 offset_to_align;
	u32 i;
	u32 read_word;
	u8 *ptr;
	u8 *pbuf;

	FLASH_Write_Lock();

	offset_to_align = address & 0x03;
	pbuf = data;
	if (offset_to_align != 0) {
		/* the start address is not 4-bytes aligned */
		FLASH_RxData(0, address - offset_to_align, 4, (u8 *)&read_word);

		ptr = (u8 *)&read_word + offset_to_align;
		offset_to_align = 4 - offset_to_align;
		for (i = 0; i < offset_to_align; i++) {
			*pbuf = *(ptr + i);
			pbuf++;
			len--;
			if (len == 0) {
				break;
			}
		}
	}

	/* address = next 4-bytes aligned */
	address = (((address - 1) >> 2) + 1) << 2;

	ptr = (u8 *)&read_word;
	if ((u32)pbuf & 0x03) {
		while (len >= 4) {
			FLASH_RxData(0, address, 4, (u8 *)&read_word);

			for (i = 0; i < 4; i++) {
				*pbuf = *(ptr + i);
				pbuf++;
			}
			address += 4;
			len -= 4;
		}
	} else {
		while (len >= 4) {
			FLASH_RxData(0, address, 4, pbuf);

			pbuf += 4;
			address += 4;
			len -= 4;
		}
	}

	if (len > 0) {
		FLASH_RxData(0, address, 4, (u8 *)&read_word);

		for (i = 0; i < len; i++) {
			*pbuf = *(ptr + i);
			pbuf++;
		}
	}

	FLASH_Write_Unlock();

	return 1;
}

/**
  * @brief  get current image2 location
  * @param  image index
  * @retval  The retval can be one of the followings:
  *              OTA_INDEX_1: current images located in OTA1 address space
  *              OTA_INDEX_2: current images located in OTA2 address space
  */
u32 ota_get_cur_index(u32 idx)
{
	u32 PhyAddr;
	(void)idx;

	RSIP_REG_TypeDef *RSIP = ((RSIP_REG_TypeDef *) RSIP_REG_BASE);
	u32 AddrStart = RSIP->FLASH_MMU[0].RSIP_REMAPxOR;

	if (AddrStart & RSIP_BIT_REMAP_x_ENABLE) {
		PhyAddr = (AddrStart >> 9) << 5;

		/*add certificate and manifest*/
		PhyAddr = PhyAddr - 0x2000;

		if (PhyAddr == IMG_ADDR[OTA_IMGID_APP][OTA_INDEX_1]) {
			return OTA_INDEX_1;
		} else {
			return OTA_INDEX_2;
		}
	}

	return OTA_INDEX_1;
}

/**
  * @brief  check if bootloader OTA2 address is valid
  * @param  none
  * The retval can be one of the followings:
  *              0: OTA2 address is valid
  *              1: OTA2 address is invalid
  */
u8 ota_checkbootloader_ota2(void)
{
	u16 tmp;
	u32 ImgAddr;

	tmp = HAL_READ16(OTPC_REG_BASE, SEC_OTA_ADDR);

	/* Take default OTP value as invalid address */
	if (tmp != 0xFFFF) {
		ImgAddr = tmp << 12;
	} else {
		ImgAddr = 0;
	}

	/* check if OTA2 addr is valid. The OTA2 addr in OTP should be a multiples of 4KB. */
	return ((ImgAddr == IMG_ADDR[OTA_IMGID_BOOT][OTA_INDEX_2]) ? 0 : 1);
}

/**
  * @brief	  parse firmware file header and get the desired OTA header
  * @param   buf: point to buffer for receiving OTA header of new firmware
  * @param   len: data length to be received from server
  * @param   pOtaTgtHdr:point to target image OTA  header
  * @retval 0: receive fail, 1: receive ok
  */
u32 get_ota_tartget_header(ota_context *ctx, u8 *buf, u32 len)
{
	update_file_img_hdr *ImgHdr;
	update_file_hdr *pOtaFileHdr;
	update_ota_target_hdr *pOtaTgtHdr = ctx->otaTargetHdr;
	u8 *pTempAddr;
	u32 i = 0, j = 0;
	int index = -1;

	/*check if buf and len is valid or not*/
	if ((len < HEADER_LEN) || (!buf)) {
		goto error;
	}

	pOtaFileHdr = (update_file_hdr *)buf;
	pTempAddr = buf + HEADER_LEN;

	if (len < (pOtaTgtHdr->FileHdr.HdrNum * SUB_HEADER_LEN + HEADER_LEN)) {
		goto error;
	}

	/*get the target OTA header from the new firmware file header*/
	for (i = 0; i < pOtaFileHdr->HdrNum; i++) {
		index = -1;
		pTempAddr = buf + HEADER_LEN + SUB_HEADER_LEN * i;
		ImgHdr = (update_file_img_hdr *)(pTempAddr);

		if (strncmp("OTA", (const char *)ImgHdr->Signature, 3) == 0) {
			index = 0;
		} else {
			goto error;
		}

		if (index >= 0) {
			_memcpy((void *)(u8 *)(&pOtaTgtHdr->FileImgHdr[j]), (void *)pTempAddr, sizeof(update_file_img_hdr));
			j++;
		}
	}

	pOtaTgtHdr->ValidImgCnt = j;

	if (j == 0) {
		ota_printf(_OTA_INFO_, "[%s] no valid image\n", __FUNCTION__);
		goto error;
	}

	return 1;
error:
	return 0;
}

/**
  * @brief	  verify new firmware checksum.
  * @param  addr: new image address
  * @param  len: new image length
  * @param  signature: point to signature strings
  * @param  pOtaTgtHdr: point to target image OTA  header
  * @retval 0: verify fail, 1: verify ok
  */
u32 verify_ota_checksum(update_ota_target_hdr *pOtaTgtHdr, u8 targetIdx, int index)
{
	u32 i;
	u32 flash_checksum = 0;
	u32 addr, len;
	update_manifest_info *manifest = NULL;
	u8 *pTempbuf;
	int k;
	int rlen;
	u8 res = _TRUE;

	pTempbuf = rtw_malloc(BUF_SIZE);

	flash_checksum = 0;

	addr = IMG_ADDR[pOtaTgtHdr->FileImgHdr[index].ImgID][targetIdx];
	len = pOtaTgtHdr->FileImgHdr[index].ImgLen - sizeof(update_manifest_info);
	manifest = &pOtaTgtHdr->Manifest[index];

	if (pOtaTgtHdr->FileImgHdr[index].ImgID == OTA_IMGID_APP) {
		/* read certificate first */
		/* read flash data back and calculate checksum */
		for (i = 0; i < 0x1000; i += BUF_SIZE) {
			rlen = (0x1000 - i) > BUF_SIZE ? BUF_SIZE : (0x1000 - i);
			ota_readstream_user(addr - SPI_FLASH_BASE + i, rlen, pTempbuf);

			for (k = 0; k < rlen; k++) {
				flash_checksum += pTempbuf[k];
			}
		}

		/* read from Manifest */
		len = len - 0x1000;
		addr = addr + 0x1000;
	}

	/*add signature's checksum*/
	for (i = 0; i < sizeof(update_manifest_info); i++) {
		flash_checksum += *((u8 *)manifest + i);
	}

	/* read flash data back and calculate checksum */
	for (i = 0; i < len; i += BUF_SIZE) {
		rlen = (len - i) > BUF_SIZE ? BUF_SIZE : (len - i);
		ota_readstream_user(addr - SPI_FLASH_BASE + i + sizeof(update_manifest_info), rlen, pTempbuf);
		for (k = 0; k < rlen; k++) {
			flash_checksum += pTempbuf[k];
		}
	}

	if (flash_checksum != pOtaTgtHdr->FileImgHdr[index].Checksum) {
		ota_printf(_OTA_ERR_, "OTA image(%08x) checksum error!!!\nCalculated checksum 0x%8x, host checksum 0x%8x\n", (unsigned int)addr, \
				   (unsigned int)flash_checksum, (unsigned int)pOtaTgtHdr->FileImgHdr[index].Checksum);
		res = _FALSE;

		goto EXIT;
	} else {
		ota_printf(_OTA_INFO_, "OTA image(%08x) checksum ok!!!\n", (unsigned int)addr);
	}
EXIT:
	rtw_free(pTempbuf);
	return res;
}

/**
  * @brief	  update manifest.
  * @param  addr: new image address
  * @retval 0: update fail, 1: update ok
  */
u32 ota_update_manifest(update_ota_target_hdr *pOtaTgtHdr, u32 ota_target_index, int index)
{
	u32 addr;
	update_manifest_info *manifest = NULL;
	flash_t flash;

#if OTA_CLEAR_PATTERN
	u8 ota_old_index = ota_target_index ^ 1;
	u8 empty_sig[8] = {0x0};
#endif

	device_mutex_lock(RT_DEV_LOCK_FLASH);

	addr = IMG_ADDR[pOtaTgtHdr->FileImgHdr[index].ImgID][ota_target_index];
	manifest = &pOtaTgtHdr->Manifest[index];

	ota_printf(_OTA_INFO_, "update addr: 0x%08x\n", (unsigned int)addr);
	ota_printf(_OTA_INFO_, "update version major: %d, minor: %d\n", manifest->MajorImgVer, manifest->MinorImgVer);

	if (pOtaTgtHdr->FileImgHdr[index].ImgID == OTA_IMGID_APP) {
		/*skip certificate for image2*/
		addr = addr + 0x1000;
	}

	/*write the manifest finally*/
	flash_stream_write(&flash, addr - SPI_FLASH_BASE, sizeof(update_manifest_info), (u8 *)manifest);

#if OTA_CLEAR_PATTERN
	if (strncmp("OTA", (const char *)pOtaTgtHdr->FileImgHdr[index].Signature, 3) == 0) {
		addr = IMG_ADDR[pOtaTgtHdr->FileImgHdr[index].ImgID][ota_old_index];
	} else {
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		return 0;
	}

	if (pOtaTgtHdr->FileImgHdr[index].ImgID == OTA_IMGID_APP) {
		/*skip the certificate for image2*/
		addr = addr + 0x1000;
	}

	/*clear the old FW pattern to 0 finally*/
	ota_printf(_OTA_INFO_, "ImgID: %lu, clear the old FW pattern, addr: 0x%08x\n", pOtaTgtHdr->FileImgHdr[index].ImgID, (unsigned int)addr);
	flash_stream_write(&flash, addr - SPI_FLASH_BASE, 8, empty_sig);
#endif
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	ota_printf(_OTA_INFO_, "[%s] Update OTA success!\n", __FUNCTION__);

	return 1;
}

int parser_url(char *url, char *host, u16 *port, char *resource, int len)
{
	if (url) {
		char *http = NULL, *pos = NULL;

		http = strstr(url, "http://");
		if (http) { // remove http
			url += strlen("http://");
		}
		memset(host, 0, len);

		pos = strstr(url, ":");	// get port
		if (pos) {
			memcpy(host, url, (pos - url));
			pos += 1;
			*port = atoi(pos);
		} else {
			pos = strstr(url, "/");
			if (pos) {
				memcpy(host, url, (pos - url));
				url = pos;
			}
			*port = 80;
		}
		ota_printf(_OTA_INFO_, "server: %s\n\r", host);
		ota_printf(_OTA_INFO_, "port: %d\n\r", *port);

		memset(resource, 0, len);
		pos = strstr(url, "/");
		if (pos) {
			memcpy(resource, pos + 1, strlen(pos + 1));
		}
		ota_printf(_OTA_INFO_, "resource: %s\n\r", resource);

		return 0;
	}
	return -1;
}

static int ota_ssl_random(void *p_rng, u8 *output, size_t output_len)
{
	(void) p_rng;
	rtw_get_random_bytes(output, output_len);
	return 0;
}

static void *ota_ssl_calloc(size_t nelements, size_t elementSize)
{
	size_t size;
	void *ptr = NULL;

	size = nelements * elementSize;
	ptr = pvPortMalloc(size);

	if (ptr) {
		memset(ptr, 0, size);
	}

	return ptr;
}

static void ota_ssl_free(void *pv)
{
	vPortFree(pv);
}

int ota_update_tls_new(ota_context *ctx)
{
	int ret = -1;
	update_tls *tls = (update_tls *)ctx->tls;

	mbedtls_platform_set_calloc_free(ota_ssl_calloc, ota_ssl_free);

	mbedtls_ssl_context *ssl = &tls->ssl;
	mbedtls_ssl_config *conf = &tls->conf;

	ota_printf(_OTA_INFO_, "  . Setting up the SSL/TLS structure...");

	mbedtls_ssl_init(ssl);
	mbedtls_ssl_config_init(conf);

	if ((ret = mbedtls_ssl_config_defaults(conf,
										   MBEDTLS_SSL_IS_CLIENT,
										   MBEDTLS_SSL_TRANSPORT_STREAM,
										   MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {

		ota_printf(_OTA_ERR_, "ERROR: mbedtls_ssl_config_defaults ret(%d)\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_NONE);
	mbedtls_ssl_conf_rng(conf, ota_ssl_random, NULL);

	if ((ret = mbedtls_ssl_setup(ssl, conf)) != 0) {
		ota_printf(_OTA_ERR_, "ERROR: mbedtls_ssl_setup ret(%d)\n", ret);
		goto exit;
	}

	ota_printf(_OTA_INFO_, " ok\n");

	mbedtls_ssl_set_bio(ssl, &ctx->fd, mbedtls_net_send, mbedtls_net_recv, NULL);
	return ret;
exit:
	mbedtls_ssl_free(ssl);
	mbedtls_ssl_config_free(conf);
	return ret;
}

int ota_update_conn_write(ota_context *ctx, u8 *data, int data_len)
{
	if (ctx->type == OTA_HTTPS) {
		return mbedtls_ssl_write(&ctx->tls->ssl, data, data_len);
	}
	return write(ctx->fd, data, data_len);
}

int ota_update_conn_read(ota_context *ctx, u8 *data, int data_len)
{
	int bytes_rcvd = -1;

	if (ctx->type == OTA_SDCARD) {
		f_lseek(&ctx->sdcard->file, ctx->sdcard->offset);
		if (f_read(&ctx->sdcard->file, data, data_len, (u32 *)&bytes_rcvd)) {
			ota_printf(_OTA_ERR_, "[%s] Read data failed", __FUNCTION__);
			return -1;
		}
		ctx->sdcard->offset += bytes_rcvd;
		return bytes_rcvd;
	}

	if (ctx->fd < 0) {
		ota_printf(_OTA_INFO_, "[%s], socket is invalid\n", __FUNCTION__);
		return bytes_rcvd;
	}

	if (ctx->type == OTA_HTTPS) {
		bytes_rcvd = mbedtls_ssl_read(&ctx->tls->ssl, data, data_len);
	} else {
		bytes_rcvd = read(ctx->fd, data, data_len);
	}

	if (bytes_rcvd < 0) {
		ota_printf(_OTA_INFO_, "[%s], Close HTTP Socket[%d].\n", __FUNCTION__, ctx->fd);
		return -2;
	}
	return bytes_rcvd;
}

int ota_update_http_send_request(ota_context *ctx)
{
	u8 *request = NULL;
	int ret = -1;

	request = (u8 *)rtw_malloc(strlen("GET /") + strlen(ctx->resource) + strlen(" HTTP/1.1\r\nHost: ")
							   + strlen(ctx->host) + strlen("\r\n\r\n") + 1);
	sprintf((char *)request, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n", ctx->resource, ctx->host);
	ret = ota_update_conn_write(ctx, request, strlen((char *)request));
	rtw_free(request);
	return ret;
}

/**
  * @brief  parse http response.
  * @param  response: the http response got from server
  * @param  response_len: The length of http response
  * @param  result: The struct that store the useful info from the http response
  * @retval  1:only got status code;3:got status code and content length,but not get the full header;4: got all info;-1:failed
  */
int ota_update_http_parse_response(ota_context *ctx, u8 *response, u32 response_len, http_response_result_t *result)
{
	update_redirect_conn *redirect = ctx->redirect;
	char *ptr_body_end;

	//Get status code
	if (0 == result->parse_status) { //didn't get the http response
		char *crlf_ptr = strstr((char *)response, "\r\n");
		if (crlf_ptr == NULL) { //didn't get the status code
			return -1;
		}
		result->status_code = atoi((char const *)response + 9); //get the status code
		if (result->status_code == 200) {
			result->parse_status = 1;
		} else if (result->status_code == 302) {
			char *tmp = NULL;
			const char *location1 = "LOCATION";
			const char *location2 = "Location";
			ota_printf(_OTA_INFO_, "response 302:%s \r\n", response);

			if ((tmp = strstr((char *)response, location1)) || (tmp = strstr((char *)response, location2))) {
				redirect->len = strlen(tmp + 10);
				ota_printf(_OTA_INFO_, "Location len = %d\r\n", redirect->len);
				if (redirect->url == NULL) {
					redirect->url = (char *)rtw_malloc(redirect->len);
					if (redirect->url == NULL) {
						return -1;
					}
				}
				memset(redirect->url, 0, redirect->len);
				memcpy(redirect->url, tmp + 10, strlen(tmp + 10));
			}

			if (redirect->host == NULL) {
				redirect->host = (char *)rtw_malloc(redirect->len);
				if (redirect->host == NULL) {
					return -1;
				}
			}

			if (redirect->resource == NULL) {
				redirect->resource = (char *)rtw_malloc(redirect->len);
				if (redirect->resource == NULL) {
					return -1;
				}
			}

			memset(redirect->host, 0, redirect->len);
			memset(redirect->resource, 0, redirect->len);
			if (parser_url(redirect->url, redirect->host, &redirect->port, redirect->resource, redirect->len) < 0) {
				return -1;
			}
			return -1;
		} else {
			ota_printf(_OTA_INFO_, "[%s] The http response status code is %lu", __FUNCTION__, result->status_code);
			return -1;
		}
	}

	//if didn't receive the full http header
	if (3 == result->parse_status) { //didn't get the http response
		if (NULL != (ptr_body_end = strstr((char *)response, "\r\n\r\n"))) {
			result->parse_status = 4;
			result->header_len = (u32)ptr_body_end - (u32)response + 4;
			result->body = response + result->header_len;
		}

		if (3 == result->parse_status) {//Still didn't receive the full header
			result->header_bak = rtw_malloc(HEADER_BAK_LEN + 1);
			memset(result->header_bak, 0, strlen((const char *)result->header_bak));
			memcpy(result->header_bak, response + response_len - HEADER_BAK_LEN, HEADER_BAK_LEN);
		}
	}

	ota_printf(_OTA_INFO_, "response: %s\n", response);

	//Get Content-Length
	if (1 == result->parse_status) { //didn't get the content length
		char *tmp_ptr;

		/* parse response_content_len */
		if (NULL != (tmp_ptr = strstr((char *)response, "Content-Length")) ||
			NULL != (tmp_ptr = strstr((char *)response, "CONTENT-LENGTH"))) {
			result->body_len = atoi(tmp_ptr + strlen("Content-Length: "));
			result->parse_status = 2;
		}
		if (NULL != (ptr_body_end = strstr((char *)response, "\r\n\r\n"))) {
			if (result->parse_status == 2) { //get the full header and the content length
				result->parse_status = 4;
				result->header_len = (u32)ptr_body_end - (u32)response + 4;
				result->body = response + result->header_len;
			} else { //there are no content length in header
				ota_printf(_OTA_INFO_, "[%s] No Content-Length in header", __FUNCTION__);
				return -1;
			}
		}

		if (1 == result->parse_status) {//didn't get the content length and the full header
			result->header_bak = rtw_malloc(HEADER_BAK_LEN + 1);
			memset(result->header_bak, 0, strlen((char *)result->header_bak));
			memcpy(result->header_bak, response + response_len - HEADER_BAK_LEN, HEADER_BAK_LEN);
		} else if (2 == result->parse_status) { //didn't get the full header but get the content length
			result->parse_status = 3;
			result->header_bak = rtw_malloc(HEADER_BAK_LEN + 1);
			memset(result->header_bak, 0, strlen((char *)result->header_bak));
			memcpy(result->header_bak, response + response_len - HEADER_BAK_LEN, HEADER_BAK_LEN);
		}
	}

	return result->parse_status;
}

int ota_update_http_recv_response(ota_context *ctx, u8 *buf, int buf_size)
{
	int read_bytes = 0;
	int writelen = 0;
	int ret = -1;
	http_response_result_t rsp_result = {0};
	u32 idx = 0;

	while (3 >= rsp_result.parse_status) { //still read header
		if (0 == rsp_result.parse_status) { //didn't get the http response
			memset(buf, 0, buf_size);
			read_bytes = ota_update_conn_read(ctx, buf, buf_size);
			if (read_bytes <= 0) {
				ota_printf(_OTA_ERR_, "[%s] Read socket failed\n", __FUNCTION__);
				goto exit;
			}
			idx = read_bytes;
			memset(&rsp_result, 0, sizeof(rsp_result));
			if (ota_update_http_parse_response(ctx, buf, idx, &rsp_result) == -1) {
				goto exit;
			}
		} else if ((1 == rsp_result.parse_status) || (3 == rsp_result.parse_status)) { //just get the status code
			memset(buf, 0, buf_size);
			memcpy(buf, rsp_result.header_bak, HEADER_BAK_LEN);
			rtw_free(rsp_result.header_bak);
			rsp_result.header_bak = NULL;
			read_bytes = ota_update_conn_read(ctx, buf + HEADER_BAK_LEN, (buf_size - HEADER_BAK_LEN));
			if (read_bytes <= 0) {
				ota_printf(_OTA_ERR_, "[%s] Read socket failed\n", __FUNCTION__);
				goto exit;
			}
			idx = read_bytes + HEADER_BAK_LEN;
			if (ota_update_http_parse_response(ctx, buf, read_bytes + HEADER_BAK_LEN, &rsp_result) == -1) {
				goto exit;
			}
		}
	}

	if (0 == rsp_result.body_len) {
		ota_printf(_OTA_INFO_, "[%s] New firmware size = 0 !\n", __FUNCTION__);
		goto exit;
	} else {
		ota_printf(_OTA_INFO_, "[%s] Download new firmware begin, total size : %lu\n", __FUNCTION__, rsp_result.body_len);
	}

	ota_printf(_OTA_INFO_, "idx = %lu, rsp_result.header_len = %lu\n", idx, rsp_result.header_len);

	writelen = idx - rsp_result.header_len;
	/* remove http header_len from alloc*/
	if (writelen >= 0) {
		memset(buf, 0, rsp_result.header_len);
		_memcpy((void *)buf, (void *)(buf + rsp_result.header_len), writelen);
		memset(buf + writelen, 0, rsp_result.header_len); // move backup to the head of alloc
	}

	return writelen;
exit:
	return ret;
}

/**
  * @brief  receive file_info from server. This operation is patched for the compatibility with ameba.
  * @param  Recvbuf: point for receiving buffer
  * @param  len: length of file info
  * @param  socket: socket handle
  * @retval 0: receive fail, 1: receive ok
  */
u32 recv_file_info_from_server(ota_context *ctx, u8 *Recvbuf, u32 len)
{
	int read_bytes = 0;
	u32 TempLen;
	u8 *buf;

	/*read 4 Dwords from server, get image header number and header length*/
	buf = Recvbuf;
	TempLen = len;
	while (TempLen > 0) {
		read_bytes = ota_update_conn_read(ctx, buf, TempLen);
		if (read_bytes < 0) {
			ota_printf(_OTA_ERR_, "[%s] read socket failed\n", __FUNCTION__);
			goto error;
		}
		if (read_bytes == 0) {
			break;
		}
		TempLen -= read_bytes;
		buf += read_bytes;
	}

	return 1;
error:
	return 0;
}

/**
* @brief  receive OTA firmware file header from server
* @param  Recvbuf: pointer to buffer for receiving OTA header of firmware file
* @param  writelen:the length already read from server
* @param  len: data length to be received from server
* @param  pOtaTgtHdr: point to target image OTA  header
* @param  socket: socket handler
* @retval  0:failed;1:success
*/
u32 recv_ota_file_header(ota_context *ctx, u8 *Recvbuf, u32 writelen, u32 *len)
{
	int read_bytes = 0;
	u32 TempLen;
	u8 *buf;
	update_file_hdr *pOtaFileHdr;
	update_ota_target_hdr *pOtaTgtHdr = ctx->otaTargetHdr;

	buf = Recvbuf + writelen;
	/*receive the remaining OTA firmware file header info if needed*/
	if (writelen < HEADER_LEN) {
		TempLen = HEADER_LEN - writelen;
		while (TempLen) {
			read_bytes = ota_update_conn_read(ctx, buf, TempLen);
			if (read_bytes < 0) {
				ota_printf(_OTA_ERR_, "[%s] read socket failed\n", __FUNCTION__);
				goto error;
			}
			if (read_bytes == 0) {
				break;
			}
			TempLen -= read_bytes;
			buf += read_bytes;
			writelen += read_bytes;
		}
	}

	pOtaFileHdr = (update_file_hdr *)(Recvbuf);

	pOtaTgtHdr->FileHdr.FwVer = pOtaFileHdr->FwVer;
	pOtaTgtHdr->FileHdr.HdrNum = pOtaFileHdr->HdrNum;

	ota_printf(_OTA_INFO_, "ota header num: %lu\n", pOtaTgtHdr->FileHdr.HdrNum);

	if (pOtaTgtHdr->FileHdr.HdrNum > MAX_IMG_NUM) {
		ota_printf(_OTA_ERR_, "ota header num: %lu is invaild\n", pOtaTgtHdr->FileHdr.HdrNum);
		goto error;
	}

	/*read the remaining Header info*/
	if (writelen < (pOtaFileHdr->HdrNum * SUB_HEADER_LEN + HEADER_LEN)) {
		buf = Recvbuf + writelen;
		TempLen = pOtaFileHdr->HdrNum * SUB_HEADER_LEN + HEADER_LEN - writelen;

		while (TempLen) {
			read_bytes = ota_update_conn_read(ctx, buf, TempLen);
			TempLen -= read_bytes;
			buf += read_bytes;
		}
	}

	*len = (pOtaFileHdr->HdrNum * SUB_HEADER_LEN) + HEADER_LEN;
	return 1;
error:
	return 0;
}

void download_percentage(int cur_size, int total_size)
{
	static int percent_bak = -1;
	int percent = cur_size * 100 / total_size;
	if (percent_bak != percent && percent % 10 == 0) {
		percent_bak = percent;
		ota_printf(_OTA_INFO_, "ota download percentage: %d", percent);
	}
}

void download_parameter_init(ota_context *ctx)
{
	update_ota_ctrl_info *otaCtrl = ctx->otactrl;
	update_ota_target_hdr *pOtaTgtHdr = ctx->otaTargetHdr;

	otaCtrl->ImgId = pOtaTgtHdr->FileImgHdr[otaCtrl->index].ImgID;

	/* -----step4: check OTA index we should update according to image2-------*/
	if (ota_get_cur_index(otaCtrl->ImgId) == OTA_INDEX_1) {
		otaCtrl->targetIdx = OTA_INDEX_2;
	} else {
		otaCtrl->targetIdx = OTA_INDEX_1;
	}
	ota_printf(_OTA_INFO_, "[%s] OTA%d address space will be upgraded\n", __FUNCTION__, otaCtrl->targetIdx + 1);

	/*update flag status*/
	otaCtrl->SigFg = 0;
	otaCtrl->SigCnt = 0;
	otaCtrl->SkipBootOTAFg = 0;

	/*init download information buffer, get OTA image and Write New Image to flash, skip the manifest,
		not write manifest first for power down protection*/
	otaCtrl->FlashAddr = IMG_ADDR[otaCtrl->ImgId][otaCtrl->targetIdx] - SPI_FLASH_BASE;
	otaCtrl->ImageLen = pOtaTgtHdr->FileImgHdr[otaCtrl->index].ImgLen;
	otaCtrl->ImgOffset = pOtaTgtHdr->FileImgHdr[otaCtrl->index].Offset;

	ota_printf(_OTA_INFO_, "ImgId: %lu\n", otaCtrl->ImgId);
	ota_printf(_OTA_INFO_, "FlashAddr: 0x%08x\n", (unsigned int)otaCtrl->FlashAddr);
	ota_printf(_OTA_INFO_, "ImageLen: %lu\n", otaCtrl->ImageLen);
	ota_printf(_OTA_INFO_, "ImgOffset: %lu\n", otaCtrl->ImgOffset);

	/*initialize the reveiving counter*/
	otaCtrl->ImgOffset = 0;
	otaCtrl->ReadBytes = 0;

	if (otaCtrl->ImgId == OTA_IMGID_APP) {
		/*skip certificate*/
		otaCtrl->ImgOffset += 0x1000;
		otaCtrl->RemainBytes = otaCtrl->ImageLen - otaCtrl->ReadBytes;
	} else {
		otaCtrl->RemainBytes = otaCtrl->ImageLen - sizeof(update_manifest_info) - otaCtrl->ReadBytes;/*skip the manifest structure*/
		otaCtrl->FlashAddr = otaCtrl->FlashAddr + sizeof(update_manifest_info);/*skip the manifest structure*/
		/*check bootloader OTA2*/
		if (otaCtrl->ImgId == OTA_IMGID_BOOT && otaCtrl->targetIdx == OTA_INDEX_2) {
			otaCtrl->SkipBootOTAFg = ota_checkbootloader_ota2();
		}
	}
	ota_printf(_OTA_INFO_, "ReadBytes: %d, ImgOffset: %lu\n", otaCtrl->ReadBytes, otaCtrl->ImgOffset);
}

int download_packet_process(ota_context *ctx, u8 *buf, int len)
{
	update_ota_ctrl_info *otaCtrl = ctx->otactrl;
	update_ota_target_hdr *pOtaTgtHdr = ctx->otaTargetHdr;
	static update_manifest_info *manifest = NULL;
	static int manifest_size = sizeof(update_manifest_info);
	static u8 *empty_sig = NULL;
	static u32 write_sector = 0;
	static u32 next_erase_sector = 0;
	static int size = 0;
	static flash_t flash;
	int TempCnt = 0;

	if (otaCtrl->IsGetHdr == 0) {
		/*downloading parse the OTA and RDP image from the data stream sent by server*/
		download_parameter_init(ctx);
		otaCtrl->IsGetHdr = 1;
		manifest = &pOtaTgtHdr->Manifest[otaCtrl->index];
		empty_sig = (u8 *)rtw_malloc(manifest_size);
		memset(empty_sig, 0xFF, manifest_size);
		write_sector = 0;
		next_erase_sector = 0;
		size = 0;
	}

	otaCtrl->ReadBytes += len;

	if (otaCtrl->ReadBytes > (int)otaCtrl->ImgOffset) {
		if (!otaCtrl->SigFg) {
			/*reach the desired image, the first packet process*/
			otaCtrl->SigFg = 1;
			TempCnt = otaCtrl->ReadBytes - otaCtrl->ImgOffset;
			otaCtrl->SigCnt = TempCnt < manifest_size ? TempCnt : manifest_size;
			_memcpy((void *)manifest, (void *)(buf + (len - TempCnt)), otaCtrl->SigCnt);

			if (otaCtrl->SigCnt < manifest_size || TempCnt - manifest_size == 0) {
				return size;
			}

			if (otaCtrl->ImgId == OTA_IMGID_APP) {
				_memcpy((void *)(buf + (len - TempCnt)), (void *)empty_sig, otaCtrl->SigCnt);
			} else {
				buf = buf + (len - TempCnt + manifest_size);
			}


			if (otaCtrl->ImgId != OTA_IMGID_APP) {
				len = TempCnt - manifest_size;
			}
		} else {
			/*normal packet process*/
			if (otaCtrl->SigCnt < manifest_size) {
				if (len < (int)(manifest_size - otaCtrl->SigCnt)) {
					_memcpy((void *)(manifest + otaCtrl->SigCnt), (void *)buf, len);
					otaCtrl->SigCnt += len;
					return size;
				} else {
					_memcpy((void *)(manifest + otaCtrl->SigCnt), (void *)buf, manifest_size - otaCtrl->SigCnt);

					if (otaCtrl->ImgId == OTA_IMGID_APP) {
						_memcpy((void *)(buf + (manifest_size - otaCtrl->SigCnt)), (void *)empty_sig, (manifest_size - otaCtrl->SigCnt));
					} else {
						buf = buf + (manifest_size - otaCtrl->SigCnt);
						len -= (manifest_size - otaCtrl->SigCnt) ;
					}

					otaCtrl->SigCnt = manifest_size;

					if (!len) {
						return size;
					}
				}
			} else {
				if (empty_sig) {
					rtw_free(empty_sig);
					empty_sig = NULL;
				}
			}
		}
	}

	otaCtrl->RemainBytes -= len;
	if (otaCtrl->RemainBytes < 0) {
		otaCtrl->NextImgLen = -otaCtrl->RemainBytes;
		otaCtrl->NextImgFg = 1;
		len = len - (-otaCtrl->RemainBytes);
		/* next firmware content */
		_memcpy((void *)otaCtrl->NextImgBuf, (void *)(buf + len), otaCtrl->NextImgLen);
	}

	/* skip bootloader */
	if (otaCtrl->SkipBootOTAFg == 1) {
		size += len;
		return size;
	}

	/* erase flash */
	write_sector = (otaCtrl->ImageLen - otaCtrl->RemainBytes - 1 + manifest_size) / 4096;
	if (write_sector >= next_erase_sector) {
		device_mutex_lock(RT_DEV_LOCK_FLASH);
		flash_erase_sector(&flash, otaCtrl->FlashAddr + write_sector * 4096);
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		next_erase_sector++;
	}
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_write(&flash, otaCtrl->FlashAddr + size, len, buf);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
	size += len;
	return size;
}

int download_fw_program(ota_context *ctx, u8 *buf, u32 len)
{
	update_ota_ctrl_info *otaCtrl = ctx->otactrl;
	int size = 0;

	if (otaCtrl->NextImgFg == 1) {
		size = download_packet_process(ctx, otaCtrl->NextImgBuf, otaCtrl->NextImgLen);
		otaCtrl->NextImgFg = 0;
	}

	size = download_packet_process(ctx, buf, len);
	download_percentage(size, otaCtrl->ImageLen);

	if (otaCtrl->RemainBytes <= 0) {

		if (otaCtrl->ImgId != OTA_IMGID_APP) {
			size += sizeof(update_manifest_info);    //add the manifest length
		}
		download_percentage(size, otaCtrl->ImageLen);

		ota_printf(_OTA_INFO_, "Update file size: %d bytes, start addr:0x%08x\n", size, (unsigned int)(otaCtrl->FlashAddr + SPI_FLASH_BASE));
		if ((u32)(size) != otaCtrl->ImageLen) {
			ota_printf(_OTA_ERR_, "download new firmware failed\n");
			return -1;
		}

		if (otaCtrl->SkipBootOTAFg) {
			ota_printf(_OTA_WARN_, "Bootloader OTA2 address is invalid, skip Bootloader OTA2\n");
			return 0;
		}

		/*----------step4: verify checksum and update signature-----------------*/
		if (verify_ota_checksum(ctx->otaTargetHdr, otaCtrl->targetIdx, otaCtrl->index)) {
			if (!ota_update_manifest(ctx->otaTargetHdr, otaCtrl->targetIdx, otaCtrl->index)) {
				ota_printf(_OTA_ERR_, "Change signature failed\n");
				return -1;
			}
		}
		/*check if another image is needed to download*/
		if (otaCtrl->index < ctx->otaTargetHdr->ValidImgCnt - 1) {
			otaCtrl->index++;
			otaCtrl->IsGetHdr = 0;
			ota_printf(_OTA_INFO_, "[%s] download image index : %d", __FUNCTION__, otaCtrl->index);
		} else {
			ota_printf(_OTA_INFO_, "[%s] download image end, total image num: %d", __FUNCTION__, ctx->otaTargetHdr->ValidImgCnt);
			return 2; // download end
		}
	}

	return 0;
}

int ota_update_s2_download_fw(ota_context *ctx)
{
	u8 *alloc;
	u8 *buf;
	update_ota_ctrl_info *otaCtrl = ctx->otactrl;
	int read_bytes;
	int ret = -1;

	/*acllocate buffer for downloading image from server*/
	alloc = rtw_malloc(ctx->buflen);
	buf = alloc;

	otaCtrl->index = 0;
	otaCtrl->IsGetHdr = 0;
	ota_printf(_OTA_INFO_, "[%s] download image index : %d", __FUNCTION__, otaCtrl->index);

	while (1) {
		memset(buf, 0, ctx->buflen);
		read_bytes = ota_update_conn_read(ctx, buf, ctx->buflen);
		if (read_bytes == 0) {
			ret = 0;
			break; // Read end
		}
		if (read_bytes < 0) {
			ota_printf(_OTA_ERR_, "[%s] Read socket failed", __FUNCTION__);
			ret = -1;
			goto exit;
		}
		ret = download_fw_program(ctx, buf, read_bytes);
		if (ret != 0) {
			if (ret == 2) {
				ret = 0;
			}
			ota_printf(_OTA_INFO_, "[%s] end", __FUNCTION__);
			goto exit;
		}
	}

exit:
	rtw_free(alloc);
	return ret;

}

int ota_update_s1_prepare(ota_context *ctx, u8 *buf, int len)
{
	u32 RevHdrLen = 0;
	int writelen = 0;
	int ret = -1;
	u32 file_info[3];

	/*----------------step1: send request to server---------------------*/
	if (ctx->type == OTA_LOCAL) {
		/* Receive file_info[] from server. Add this for compatibility. This file_info includes the
		file_size and checksum information of the total firmware file.	Even though the file_info
		is received from server , it won't be used.*/
		memset(buf, 0, sizeof(file_info));
		if (!recv_file_info_from_server(ctx, buf, sizeof(file_info))) {
			ota_printf(_OTA_ERR_, "[%s] receive file_info failed", __FUNCTION__);
			return -1;
		}
		writelen = 0;
	} else if (ctx->type == OTA_HTTP || ctx->type == OTA_HTTPS) {
		ret = ota_update_http_send_request(ctx);
		if (!ret) {
			ota_printf(_OTA_ERR_, "[%s] Send HTTP request failed\n", __FUNCTION__);
			return -1;
		}

		writelen = ota_update_http_recv_response(ctx, buf, len);
		if (writelen < 0) {
			ota_printf(_OTA_ERR_, "[%s] Parse HTTP response failed\n", __FUNCTION__);
			return -1;
		}
	}

	/*----------------step2: receive firmware file header---------------------*/
	if (!recv_ota_file_header(ctx, buf, writelen, &RevHdrLen)) {
		ota_printf(_OTA_ERR_, "[%s] rev firmware header failed", __FUNCTION__);
		return -1;
	}

	if (writelen < (int)RevHdrLen) {
		writelen = RevHdrLen;
	}

	/* -------step3: parse firmware file header and get the target OTA image header-----*/
	if (!get_ota_tartget_header(ctx, buf, RevHdrLen)) {
		ota_printf(_OTA_ERR_, "get OTA header failed\n");
		return -1;
	}

	memset(ctx->otactrl->NextImgBuf, 0, BUF_SIZE);
	_memcpy((void *)ctx->otactrl->NextImgBuf, (void *)(buf + RevHdrLen), writelen - RevHdrLen);
	ctx->otactrl->NextImgFg = 1;
	ctx->otactrl->NextImgLen = writelen - RevHdrLen;

	return writelen;
}

/**
  * @brief  connect to the OTA http server.
  * @param  server_socket: the socket used
  * @param  host: host address of the OTA server
  * @param  port: port of the OTA server
  * @retval  -1 when connect fail, socket value when connect success
  */
int ota_update_s0_connect_server(ota_context *ctx)
{
	struct sockaddr_in server_addr;
	struct hostent *server;
	int fd = -1;
	int ret = -1;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		ota_printf(_OTA_ERR_, "[%s] Create socket failed", __FUNCTION__);
		return -1;
	}
	ctx->fd = fd;
	ota_printf(_OTA_INFO_, "[%s] Create socket: %d success!\n", __FUNCTION__, ctx->fd);

	server = gethostbyname(ctx->host);
	if (server == NULL) {
		ota_printf(_OTA_ERR_, "[ERROR] Get host ip failed\n");
		goto exit;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(ctx->port);
	memcpy((void *)&server_addr.sin_addr, (void *)server->h_addr, 4);

	if (connect(ctx->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		ota_printf(_OTA_ERR_, "[%s] Socket connect failed", __FUNCTION__);
		goto exit;
	}

	ota_printf(_OTA_INFO_, "[%s] Connect success!\n", __FUNCTION__);

	if (ctx->type == OTA_HTTPS) {
		ret = ota_update_tls_new(ctx);
		if (ret != 0) {
			ota_printf(_OTA_ERR_, "[%s] tls create failed", __FUNCTION__);
			goto exit;
		}

		ota_printf(_OTA_INFO_, "  . Performing the SSL/TLS handshake...");

		if ((ret = mbedtls_ssl_handshake(&ctx->tls->ssl)) != 0) {
			mbedtls_ssl_free(&ctx->tls->ssl);
			mbedtls_ssl_config_free(&ctx->tls->conf);
			ota_printf(_OTA_INFO_, "ERROR: mbedtls_ssl_handshake ret(-0x%x)", -ret);
			goto exit;
		}
		ota_printf(_OTA_INFO_, " ok\n");
		ota_printf(_OTA_INFO_, "  . Use ciphersuite %s\n", mbedtls_ssl_get_ciphersuite(&ctx->tls->ssl));
	}
	return 0;
exit:
	closesocket(ctx->fd);
	return -1;
}

int ota_update_sdcard_init(ota_context *ctx)
{
#if FATFS_DISK_SD == 0
	UNUSED(ctx);
	return 0;
#else
	int ret = -1;
	char path[64];
	FILINFO fno = {0};
	update_sdcard *sdcard = ctx->sdcard;

	/*----------------------step1:open OTA file through file system-------------------------*/
	sdcard->drv_num = FATFS_RegisterDiskDriver(&SD_disk_Driver);

	if (sdcard->drv_num < 0) {
		ota_printf(_OTA_ERR_, "Rigester disk driver to FATFS fail.\n");
		goto exit;
	} else {
		sdcard->logical_drv[0] = sdcard->drv_num + '0';
		sdcard->logical_drv[1] = ':';
		sdcard->logical_drv[2] = '/';
		sdcard->logical_drv[3] = 0;
	}

	ret = f_mount(&sdcard->fs, sdcard->logical_drv, 1);
	if (ret) {
		ota_printf(_OTA_ERR_, "FATFS mount logical drive fail, ret:%d\n", ret);
		goto exit;
	}

	strcpy(path, sdcard->logical_drv);
	sprintf(&path[strlen(path)], "%s", ctx->resource);

	ret = f_stat(path, &fno);
	switch (ret) {
	case FR_OK:
		ota_printf(_OTA_INFO_, "[%s] Size: %lu\n", __FUNCTION__, fno.fsize);
		ota_printf(_OTA_INFO_, "[%s] Timestamp: %d/%d/%d, %d:%d\n", __FUNCTION__,
				   (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
				   fno.ftime >> 11, fno.ftime >> 5 & 63);
		break;
	case FR_NO_FILE:
		ota_printf(_OTA_ERR_, "[%s] Target file %s is not exist.\n", __FUNCTION__, ctx->resource);
		goto exit;
	default:
		ota_printf(_OTA_ERR_, "[%s] An error occured. (%d)\n", __FUNCTION__, ret);
		goto exit;
	}

	ret = f_open(&sdcard->file, path, FA_OPEN_ALWAYS | FA_READ);
	if (ret) {
		ota_printf(_OTA_ERR_, "[%s] Open file (%s) fail.\n", __FUNCTION__, ctx->resource);
		goto exit;
	}
	sdcard->offset = 0;
exit:
	return ret;
#endif
}

int ota_update_sdcard_deinit(ota_context *ctx)
{
#if FATFS_DISK_SD == 0
	UNUSED(ctx);
	return 0;
#else
	int ret = -1;
	update_sdcard *sdcard = ctx->sdcard;

	ota_printf(_OTA_INFO_, "[%s] f_close: %s\n\r", __FUNCTION__, ctx->resource);

	ret = f_close(&sdcard->file);
	if (ret != FR_OK) {
		ota_printf(_OTA_INFO_, "FATFS close file fail.\n");
		goto exit;
	}

	ret = f_unmount(sdcard->logical_drv);
	if (ret != FR_OK) {
		ota_printf(_OTA_INFO_, "FATFS unmount logical drive fail.\n");
		goto exit;
	}

	SD_DeInit();

	ret = FATFS_UnRegisterDiskDriver(sdcard->drv_num);
	if (ret != 0) {
		ota_printf(_OTA_INFO_, "Unregister disk driver from FATFS fail.\n");
		goto exit;
	}
exit:
	return ret;
#endif
}

int ota_update_start(ota_context *ctx)
{
	u8 *alloc = NULL;
	int alloc_buf_size = ctx->buflen;
	int ret = -1;
	int writelen = 0;
	update_redirect_conn *redirect = ctx->redirect;

restart_ota:
	redirect->port = 0;

	alloc = (u8 *)rtw_malloc(alloc_buf_size);
	if (!alloc) {
		ota_printf(_OTA_ERR_, "[%s] Alloc buffer failed\n", __FUNCTION__);
		goto update_ota_exit;
	}
	memset(alloc, 0, alloc_buf_size);

	if (ctx->type == OTA_SDCARD) {
		/*----------------step1: open OTA file through file system--------*/
		ret = ota_update_sdcard_init(ctx);
	} else {
		/*----------------step1: connect to server---------------------*/
		ret = ota_update_s0_connect_server(ctx);
	}

	if (ret != 0) {
		goto update_ota_exit;
	}

	/*----------------step2: receive firmware file header---------------------*/
	writelen = ota_update_s1_prepare(ctx, alloc, alloc_buf_size);
	if (writelen < 0) {
		ret = -1;
		goto update_ota_exit;
	}

	/*----------------step3: download firmware---------------------*/
	ret = ota_update_s2_download_fw(ctx);

update_ota_exit:
	if (alloc) {
		rtw_free(alloc);
	}

	if (ctx->fd >= 0) {
		close(ctx->fd);
	}

	if (ctx->type == OTA_SDCARD) {
		ota_update_sdcard_deinit(ctx);
	} else if (ctx->type == OTA_HTTPS) {
		mbedtls_ssl_free(&ctx->tls->ssl);
		mbedtls_ssl_config_free(&ctx->tls->conf);
	}

	/* redirect.port != 0 means there is redirect URL can be downloaded*/
	if (redirect->port != 0) {
		ctx->host = redirect->host;
		ctx->resource = redirect->resource;
		ctx->port = redirect->port;
		ota_printf(_OTA_INFO_, "OTA redirect host: %s, port: %d, resource: %s\n\r", ctx->host, ctx->port, ctx->resource);
		goto restart_ota;
	}
	if (redirect->url) {
		rtw_free(redirect->url);
	}
	if (redirect->host) {
		rtw_free(redirect->host);
	}
	if (redirect->resource) {
		rtw_free(redirect->resource);
	}
	return ret;
}

int ota_update_init(ota_context *ctx, char *host, int port, char *resource, u8 type)
{
	update_tls *tls = NULL;
	update_sdcard *sdcard = NULL;
	update_ota_ctrl_info *otactrl = NULL;
	update_redirect_conn *redirect = NULL;
	update_ota_target_hdr *otaTargetHdr = NULL;

	if (host) {
		ctx->host = (char *)rtw_malloc(strlen(host) + 1);
		if (ctx->host) {
			strcpy(ctx->host, host);
		} else {
			ota_printf(_OTA_ERR_, "%s, host malloc failed", __FUNCTION__);
			goto exit;
		}
	}

	if (resource) {
		ctx->resource = (char *)rtw_malloc(strlen(resource) + 1);
		if (ctx->resource) {
			strcpy(ctx->resource, resource);
		} else {
			ota_printf(_OTA_ERR_, "%s, resource malloc failed", __FUNCTION__);
			goto exit;
		}
	}

	otactrl = (update_ota_ctrl_info *)rtw_malloc(sizeof(update_ota_ctrl_info));
	if (otactrl) {
		memset(otactrl, 0, sizeof(update_ota_ctrl_info));
		ctx->otactrl = otactrl;
	} else {
		ota_printf(_OTA_ERR_, "%s, otactrl malloc failed", __FUNCTION__);
		goto exit;
	}

	redirect = (update_redirect_conn *)rtw_malloc(sizeof(update_redirect_conn));
	if (redirect) {
		memset(redirect, 0, sizeof(update_redirect_conn));
		ctx->redirect = redirect;
	} else {
		ota_printf(_OTA_ERR_, "%s, redirect malloc failed", __FUNCTION__);
		goto exit;
	}

	otaTargetHdr = (update_ota_target_hdr *)rtw_malloc(sizeof(update_ota_target_hdr));
	if (otaTargetHdr) {
		memset(otaTargetHdr, 0, sizeof(update_ota_target_hdr));
		ctx->otaTargetHdr = otaTargetHdr;
	} else {
		ota_printf(_OTA_ERR_, "%s, otaTargetHdr malloc failed", __FUNCTION__);
		goto exit;
	}
	ctx->port = port;
	ctx->type = type;
	ctx->fd = -1;
	ctx->buflen = BUF_SIZE;

	if (ctx->type == OTA_HTTPS) {
		tls = (update_tls *)rtw_malloc(sizeof(update_tls));
		if (tls) {
			ctx->tls = tls;
		} else {
			ota_printf(_OTA_ERR_, "%s, tls malloc failed", __FUNCTION__);
			goto exit;
		}
	} else if (ctx->type == OTA_SDCARD) {
		sdcard = (update_sdcard *)rtw_malloc(sizeof(update_sdcard));
		if (sdcard) {
			ctx->sdcard = sdcard;
			ctx->buflen = SD_OTA_BUF_SIZE;
		} else {
			ota_printf(_OTA_ERR_, "%s, sdcard malloc failed", __FUNCTION__);
			goto exit;
		}
	}
	ota_printf(_OTA_INFO_, "host: %s(%d), resource: %s\n", host ? ctx->host : "null", ctx->port, resource ? ctx->resource : "null");
	return 0;
exit:
	ota_update_deinit(ctx);
	return -1;
}

void ota_update_deinit(ota_context *ctx)
{
	if (ctx == NULL) {
		return;
	}

	if (ctx->host) {
		rtw_free(ctx->host);
	}

	if (ctx->resource) {
		rtw_free(ctx->resource);
	}

	if (ctx->otactrl) {
		rtw_free(ctx->otactrl);
	}

	if (ctx->redirect) {
		rtw_free(ctx->redirect);
	}

	if (ctx->otaTargetHdr) {
		rtw_free(ctx->otaTargetHdr);
	}

	if (ctx->type == OTA_HTTPS) {
		if (ctx->tls) {
			rtw_free(ctx->tls);
		}
	} else if (ctx->type == OTA_SDCARD) {
		if (ctx->sdcard) {
			rtw_free(ctx->sdcard);
		}
	}
}

void cmd_update(int argc, char **argv)
{
	/* To avoid gcc warnings */
	(void) argc;
	(void) argv;

	ota_printf(_OTA_INFO_, "OTA demo doesn't support update command. "
			   "Please use example_ota, and set type to OTA_LOCAL\n");
	return;
}

