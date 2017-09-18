/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ss_mutex.h"
#include "ss_aes.h"
#include "ss_sha2.h"
#include "ss_api.h"
#include "ss_device.h"
#include "ss_defines.h"
#include "wbaes_cmac.h"

#include "things_malloc.h"
#include "things_logger.h"

#define TAG "SECURE_STORAGE"

static secure_storage_ctx_s ss_ctx;

static ss_mutex_t *ss_mutex = NULL;

static void ss_log_buffer(const char *msg, unsigned char *buf, size_t buf_size)
{
	if (msg && buf && buf_size > 0) {
		char *octet_str = things_malloc(buf_size * 3 + 1);
		char *temp = octet_str;
		for (int i = 0; i < buf_size; i++) {
			if ((i + 1) % 20 == 0 || (i + 1) == buf_size) {
				sprintf(temp, "%02X\n", buf[i]);
			} else {
				sprintf(temp, "%02X ", buf[i]);
			}
			temp += 3;
		}
		THINGS_LOG_D(THINGS_DEBUG, TAG, "\n[%s]\n%s", msg, octet_str);
		things_free(octet_str);
	}
}

/**
 * @brief    encrypt with key which is saved in seucre storage
 *
 * @param    ctx                [in] context of secure sotrage
 * @param    p_input            [in] plain data
 * @param    input_length    [in] length of plain data
 * @param    p_output            [out] cipher data
 * @param    p_output_length    [out] length of cipher data
 *
 * @return    SS_SUCCESS        if no error is occured.
 */
static int ss_encrypt(secure_storage_ctx_s *ctx, unsigned char *p_input, unsigned int input_length, unsigned char *p_output, unsigned int *p_output_length)
{
	uint32_t padding_len = 0;

	if (NULL == ctx || NULL == p_input || 0 == input_length || NULL == p_output || NULL == p_output_length) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid param");
		return SS_INVALID_ARGUMENT;
	}

	ss_log_buffer("MASTER SECRECT for Secure Storage", ctx->ssMasterKey, ctx->ssMasterKeyLen);
	ss_log_buffer("INITIAL VECTOR for Secure Storage", ctx->ssIV, ctx->ssIVLen);

	//padding
	padding_len = SS_AES_BLK_SIZE - (input_length % SS_AES_BLK_SIZE);
	memset(p_output + input_length, padding_len, padding_len);

	//encrypt with AES CBC
	ss_aes128_cbc_encrypt_buffer(p_output, p_input, input_length + padding_len, ctx->ssMasterKey, ctx->ssIV);
	*p_output_length = input_length + padding_len;

	return SS_SUCCESS;
}

/**
 * @brief    decrypt with key which is saved in secure storage
 *
 * @param    ctx                [in] context of secure storage
 * @param    p_input            [in] cipher data
 * @param    pInputLength    [in] length of cipher data
 * @param    p_output            [out] plain data
 * @param    p_output_length    [out] length of plain data
 *
 * @return    SS_SUCCESS        if no error is occured.
 */
static int ss_decrypt(secure_storage_ctx_s *ctx, unsigned char *p_input, unsigned int input_length, unsigned char *p_output, unsigned int *p_output_length)
{
	uint32_t depadding_len = 0;
	if (NULL == ctx || NULL == p_input || 0 == input_length || NULL == p_output || NULL == p_output_length) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid param");
		return SS_INVALID_ARGUMENT;
	}

	*p_output_length = 0;

	//decrypt with AES CBC
	ss_aes128_cbc_decrypt_buffer(p_output, p_input, input_length, ctx->ssMasterKey, ctx->ssIV);
	//de-padding
	depadding_len = p_output[input_length - 1];
	*p_output_length = input_length - depadding_len;

	return SS_SUCCESS;
}

/**
 * @brief   API to init secure storage context
 *
 * @param   pBuf            [in] Secure storage buffer
 * @param   nBufLen         [in] SECURE_STORAGE_FORMAT or Byte-length of secure storage buffer
 * @param   nMaxBufLen      [in] max length of buffer
 *
 * @return  SS_SUCCESS                  if no error is occured.
			SS_INVALID_ARGUMENT
			SS_MEMORY_OVERFLOW
			SS_DATA_VALIDATION_FAIL
			SS_MEMORY_ALLOCATION_FAIL
 */
static int ss_init_secure_storage_context(secure_storage_ctx_s *ctx)
{
	unsigned char dev_id[SS_DEVICE_ID_MAX_SIZE] = { 0 };
	unsigned char hashed_iv[SS_DEVICE_ID_MAX_SIZE] = { 0 };
	unsigned int dev_id_len = 16;
	ss_sha256_ctx sha_ctx1;
	ss_sha256_ctx sha_ctx2;

	memset(ctx, 0, sizeof(secure_storage_ctx_s));
	ctx->ssBuffer = (unsigned char *)things_malloc(SS_BUFFER_BLOCK_SIZE);
	if (!ctx->ssBuffer) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation");
		return SS_MEMORY_ALLOCATION_FAIL;
	}

	int ss_ret = ssGetDeviceID(dev_id, SS_DEVICE_ID_MAX_SIZE, &dev_id_len);
	if (SS_SUCCESS != ss_ret) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Can not read device ID for secure storage. Please make sure the callback function is registered.");
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Please register callback using ssSetGetDeviceUniqueIDCallback().");
		return ss_ret;
	}
	ss_log_buffer("Device ID", dev_id, dev_id_len);

	//Master key generation : SHA256 with Device Unique Value
	ss_sha256_init(&sha_ctx1);
	ss_sha256_update(&sha_ctx1, dev_id, dev_id_len);
	ss_sha256_final(&sha_ctx1, dev_id);
	WBAES_CMAC(dev_id, dev_id_len, ctx->ssMasterKey, &(ctx->ssMasterKeyLen));

	ss_sha256_init(&sha_ctx2);
	ss_sha256_update(&sha_ctx2, dev_id, dev_id_len);
	ss_sha256_final(&sha_ctx2, hashed_iv);
	memcpy(ctx->ssIV, hashed_iv, SS_CRYPTO_IV_SIZE);
	ctx->ssIVLen = SS_CRYPTO_IV_SIZE;

	ctx->isOpend = 0;

	return SS_SUCCESS;
}

/**
 * @brief   finalize process for secure storage context
 *
 * @param   pBuf            [out]data in secure storage
 * @param   pBufLen         [out]length of data
 *
 * @return  SS_SUCCESS      if no error is occured.
 */
static int ss_final_secure_storage_context(secure_storage_ctx_s *ctx)
{
	if (ctx == NULL) {
		return SS_INVALID_ARGUMENT;
	}

	memset(ctx->ssMasterKey, 0x00, ctx->ssMasterKeyLen);
	ctx->ssMasterKeyLen = 0;
	memset(ctx->ssIV, 0x00, ctx->ssIVLen);
	ctx->ssIVLen = 0;
	memset(ctx->ssBuffer, 0x00, ctx->ssBufferSize);
	things_free(ctx->ssBuffer);
	ctx->isOpend = 0;
	ctx->curPos = 0;
	ctx->ssBufferSize = 0;

	return SS_SUCCESS;
}

int init_secure_storage()
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "In %s", __func__);

	if (NULL == ss_mutex) {
		ss_mutex = ss_mutex_new();
		if (NULL == ss_mutex) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "ss_mutex_new error!");
			return SS_ERROR;
		}
	}

	return SS_SUCCESS;
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Out %s", __func__);
}

int deinit_secure_storage()
{
	THINGS_LOG_D(THINGS_DEBUG, TAG, "In %s", __func__);

	if (!ss_mutex_free(ss_mutex)) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "ss_mutex_free error!");
		return SS_ERROR;
	}
	ss_mutex = NULL;
	return SS_SUCCESS;
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Out %s", __func__);
}

/** Persistent storage open handler.*/
FILE *secure_fopen(const char *path, const char *mode)
{
	int ss_ret = SS_SUCCESS;
	FILE *fp = NULL;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "secure_fopen :  %s : %s", path, mode);

	ss_mutex_lock(ss_mutex);
	memset(&ss_ctx, 0x00, sizeof(secure_storage_ctx_s));
	ss_ret = ss_init_secure_storage_context(&ss_ctx);
	if (SS_SUCCESS != ss_ret) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to secure storage initialization : %d", ss_ret);
		ss_mutex_unlock(ss_mutex);
		return NULL;
	}
	fp = fopen(path, mode);
	ss_mutex_unlock(ss_mutex);

	return fp;
}

/** Persistent storage read handler.*/
size_t secure_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	uint8_t *encSVRDB = NULL;
	unsigned int encSVRDBLen = 0;
	int readLen = 0;
	int ss_ret = SS_SUCCESS;
	static unsigned int ssBufSize = SS_BUFFER_BLOCK_SIZE;

	ss_mutex_lock(ss_mutex);

	encSVRDB = (uint8_t *) things_malloc(ssBufSize + 1);
	if (!encSVRDB) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
		goto exit;
	}

	if (!ss_ctx.isOpend) {
		unsigned int curBufPos = 0;
		while (1) {
			encSVRDBLen = fread(encSVRDB + curBufPos, size, SS_BUFFER_BLOCK_SIZE, stream);
			if (encSVRDBLen + curBufPos == 0) {
				THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to open secure storage");
				goto exit;
			}
			if (encSVRDBLen + curBufPos < 32 + 16) {
				THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Secure storage too small");
				goto exit;
			}
			//If secure storage buffer is not enough, try reallocation
			if (encSVRDBLen == ssBufSize - curBufPos) {
				uint8_t *tmpSVRDB = (uint8_t *) things_malloc(curBufPos + encSVRDBLen + 1);
				if (!tmpSVRDB) {
					THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
					goto exit;
				}
				memcpy(tmpSVRDB, encSVRDB, curBufPos + encSVRDBLen);
				things_free(encSVRDB);
				encSVRDB = (uint8_t *) things_malloc((ssBufSize + SS_BUFFER_BLOCK_SIZE));
				if (!encSVRDB) {
					THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
					things_free(tmpSVRDB);
					goto exit;
				}
				memcpy(encSVRDB, tmpSVRDB, curBufPos + encSVRDBLen);
				things_free(tmpSVRDB);

				//reallocate secure storage buffer of ss context.
				things_free(ss_ctx.ssBuffer);
				ss_ctx.ssBuffer = (unsigned char *)things_malloc((ssBufSize + SS_BUFFER_BLOCK_SIZE));
				if (!ss_ctx.ssBuffer) {
					THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
					goto exit;
				}
				curBufPos += encSVRDBLen;
				ssBufSize = ssBufSize + SS_BUFFER_BLOCK_SIZE;
			} else {
				break;
			}
		}
		encSVRDBLen += curBufPos;

		ss_ret = ss_decrypt(&ss_ctx, (unsigned char *)encSVRDB, encSVRDBLen, ss_ctx.ssBuffer, &ss_ctx.ssBufferSize);
		if (SS_SUCCESS != ss_ret) {
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to decrypt SVR DB : %d", ss_ret);
			goto exit;
		}

		ss_ctx.isOpend = 1;
		ss_ctx.curPos = 0;
	}

	if (ss_ctx.curPos >= ss_ctx.ssBufferSize) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "End of Secure Storage");
		readLen = 0;
	} else if (ss_ctx.curPos == 0 && (nmemb * size) >= ss_ctx.ssBufferSize) {
		memcpy((uint8_t *) ptr, ss_ctx.ssBuffer, ss_ctx.ssBufferSize);
		ss_ctx.curPos = ss_ctx.ssBufferSize;
		readLen = ss_ctx.ssBufferSize;
	} else if (ss_ctx.curPos != 0 && (nmemb * size) >= (ss_ctx.ssBufferSize - ss_ctx.curPos)) {
		memcpy((uint8_t *) ptr, ss_ctx.ssBuffer + ss_ctx.curPos, ss_ctx.ssBufferSize - ss_ctx.curPos);
		readLen = ss_ctx.ssBufferSize - ss_ctx.curPos;
		ss_ctx.curPos = ss_ctx.ssBufferSize;
	} else {
		readLen = (nmemb * size);
		memcpy((uint8_t *) ptr, ss_ctx.ssBuffer + ss_ctx.curPos, readLen);
		ss_ctx.curPos += readLen;
	}

exit:
	if (encSVRDB) {
		things_free(encSVRDB);
	}
	ss_mutex_unlock(ss_mutex);

	return readLen;
}

/** Persistent storage write handler.*/
size_t secure_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	//encrypt ptr using secure storage
	uint8_t *encSVRDB = NULL;
	unsigned int encSVRDBLen = 0;
	int writtenLen = 0;
	int ss_ret = SS_SUCCESS;

	ss_mutex_lock(ss_mutex);
	encSVRDB = (uint8_t *) things_malloc(CBC_MAC_SIZE + (nmemb * size) + 16);	//+16 to prevent memory overflow in case of padding
	if (!encSVRDB) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to memory allocation!!");
		goto exit;
	}
	memset(encSVRDB, 0x00, CBC_MAC_SIZE + (nmemb * size) + 16);

	ss_ret = ss_encrypt(&ss_ctx, (unsigned char *)ptr, nmemb * size, encSVRDB, &encSVRDBLen);
	if (SS_SUCCESS != ss_ret) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Failed to encrypt SVR DB : %d", ss_ret);
		goto exit;
	}

	writtenLen = fwrite(encSVRDB, 1, encSVRDBLen, stream);
	if (writtenLen < nmemb * size) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "Failed to write encrypted SVR DB");
		goto exit;
	}

	writtenLen = (nmemb * size);

exit:
	if (encSVRDB) {
		things_free(encSVRDB);
	}
	ss_mutex_unlock(ss_mutex);

	return writtenLen;
}

/** Persistent storage close handler.*/
int secure_fclose(FILE *fp)
{
	int ret = 0;

	ss_mutex_lock(ss_mutex);
	ss_final_secure_storage_context(&ss_ctx);
	ret = fclose(fp);
	ss_mutex_unlock(ss_mutex);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "secure_fclose : %d", ret);

	return ret;
}
