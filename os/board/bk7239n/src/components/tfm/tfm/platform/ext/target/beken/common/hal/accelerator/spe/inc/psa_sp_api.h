/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __PSA_SP_API_H__
#define __PSA_SP_API_H__

#ifndef __ASSEMBLY__

#include <stdarg.h>
#include "psa_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Generic API */
void *psa_malloc(size_t size);
void *psa_realloc(void *buffer, size_t size);
void *psa_calloc(size_t nmemb, size_t size);
void psa_free(void *buffer);

void *psa_memmove(void *dst, const void *src, size_t size);
void *psa_memset(void *buffer, int c, size_t size);
int32_t psa_memcmp(const void *buffer1, const void *buffer2, size_t size);

int psa_printf(const char *format, ...);
int psa_vprintf(const char *format, va_list vargs);
int psa_snprintf(char *out, unsigned int count, const char *format, ...);
int psa_vsnprintf(char *out, unsigned int count, const char *format,
                  va_list vargs);

/* Time API */
void psa_udelay(uint32_t us);
void psa_mdelay(uint32_t msecs);
psa_status_t psa_msleep(uint32_t msecs);
ulong_t psa_get_tick(void);

/* Secure Storage API */
typedef enum _sst_open_flag_t {
    SST_CREATE      = 0x00000001,
    SST_READ        = 0x00000002,
    SST_WRITE       = 0x00000004,
    SST_SHARD_READ  = 0x00000008 + SST_READ,
    SST_SHARD_WRITE = 0x00000010 + SST_WRITE,
} sst_open_flag_t;

#define SST_SEEK_SET (0)
#define SST_SEEK_CUR (1)
#define SST_SEEK_END (2)

#define SST_ERR_SUCCESS 0
#define SST_ERR_ASSET_NOT_PREPARED -256
#define SST_ERR_ASSET_NOT_FOUND -257
#define SST_ERR_PARAM -258
#define SST_ERR_INVALID_HANDLE -259
#define SST_ERR_STORAGE_SYSTEM_FULL -260
#define SST_ERR_SYSTEM_ERROR -261
#define SST_ERR_NULL_POINTER -262
#define SST_ERR_OUT_OF_MEMORY -512
#define SST_ERR_GENERIC -513
#define SST_ERR_PERMISSION -514

int32_t psa_sst_open(uint16_t file_id, sst_open_flag_t flags,
                     uint32_t max_size);
int32_t psa_sst_close(int32_t fd);
int32_t psa_sst_read(int32_t fd, void *buf, size_t count);
int32_t psa_sst_write(int32_t fd, const void *buffer, size_t count);
uint32_t psa_sst_lseek(int32_t fd, int32_t off, int32_t whence);
int32_t psa_sst_unlink(int32_t fd);
int32_t psa_sst_ftruncate(int32_t fd, size_t length);
int32_t  psa_sst_alloc_enumerator(void);
uint16_t psa_sst_get_next(int32_t hd);
void psa_sst_free_enumerator(int32_t hd);

#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */

#endif /* __PSA_SP_API_H__ */
