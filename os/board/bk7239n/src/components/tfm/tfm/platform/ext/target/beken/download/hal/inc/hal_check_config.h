/*
 * Copyright (c) 2021, Arm Technology (China) Co., Ltd.
 * All rights reserved.
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __HAL_CHECK_CONFIG_H__
#define __HAL_CHECK_CONFIG_H__

#if !defined(CAL_ENABLE_SHA256) && !defined(CAL_ENABLE_SHA1)
#error "one of the algo (SHA256/SHA1) should be enabled for secure boot"
#endif

#if !defined(CAL_ENABLE_RSA) && !defined(CAL_ENABLE_ECDSA)
#error "one of the algo (RSA/ECDSA) should be enabled for secure boot"
#endif

#endif /* __HAL_CHECK_CONFIG_H__ */
