// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __PAL_HEAP_H__
#define __PAL_HEAP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pal_common.h"
PAL_API void *pal_calloc(size_t nmemb, size_t size);
PAL_API void *pal_malloc(size_t size);
PAL_API void pal_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __PAL_HEAP_H__ */
