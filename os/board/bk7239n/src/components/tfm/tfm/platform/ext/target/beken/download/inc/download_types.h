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

#ifndef __DOWNLOAD_TYPES_H__
#define __DOWNLOAD_TYPES_H__

//#ifndef __ASSEMBLY__

#ifdef __cplusplus
extern "C" {
}
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>  /* for off_t */

#ifndef __ULONG_T_DEFINED__
typedef unsigned long ulong_t;
#define __ULONG_T_DEFINED__
#endif

#ifndef __ERR_T_DEFINED__
typedef int32_t err_t;
#define __ERR_T_DEFINED__
#endif

#ifndef __ADDR_T_DEFINED__
typedef unsigned long addr_t;
#define __ADDR_T_DEFINED__
#endif

//#ifdef CONFIG_BK_BOOT
typedef unsigned char  UINT8;
typedef signed   char  INT8;
typedef unsigned short UINT16;
typedef signed   short INT16;
typedef unsigned int   UINT32;
typedef signed   int   INT32;

typedef unsigned char  uint8;
typedef signed   char  int8;
typedef unsigned short uint16;
typedef signed   short int16;
typedef unsigned int   uint32;
typedef signed   int   int32;

typedef unsigned char  uint8_t;
typedef signed   char  int8_t;
typedef unsigned short uint16_t;
typedef signed   short int16_t;
//typedef unsigned int   uint32_t;
//typedef signed   int   int32_t;

typedef unsigned char       BYTE;
typedef signed   char       int8;
typedef signed   short      int16;

typedef signed   long long  int64;
typedef unsigned char       uint8;
typedef unsigned char       u_int8;
typedef unsigned short      uint16;
typedef unsigned short      u_int16;

typedef unsigned long long  uint64;
typedef float               fp32;
typedef double              fp64;

typedef signed   char       s8;
typedef signed   short      s16;
typedef signed   long       s32;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef uint64              u64;
typedef unsigned long       u_int32;

typedef unsigned char  		UINT8;
typedef signed   char  		INT8;
typedef unsigned short 		UINT16;
typedef signed   short 		INT16;
typedef unsigned int   		UINT32;
typedef signed   int   		INT32;
typedef unsigned long long  UINT64;
typedef signed   long long  INT64;
typedef float         		FP32;
typedef double         		FP64;

typedef unsigned char       BOOLEAN;

//#endif

#ifdef __cplusplus
}
#endif

//#endif /* !__ASSEMBLY__ */

#endif /* __DOWNLOAD_TYPES_H__ */
