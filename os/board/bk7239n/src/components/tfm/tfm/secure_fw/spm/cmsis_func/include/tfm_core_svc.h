/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CORE_SVC_H__
#define __TFM_CORE_SVC_H__

/* SVC numbers */
#define TFM_SVC_PSA_WAIT                (0x0)
#define TFM_SVC_PSA_EOI                 (0x1)
#define TFM_SVC_SFN_REQUEST             (0x2)
#define TFM_SVC_SFN_RETURN              (0x3)
#define TFM_SVC_SFN_COMPLETION          (0x4)
#define TFM_SVC_GET_BOOT_DATA           (0x5)
#define TFM_SVC_DEPRIV_REQ              (0x6)
#define TFM_SVC_DEPRIV_RET              (0x7)
#define TFM_SVC_ENABLE_IRQ              (0x8)
#define TFM_SVC_DISABLE_IRQ             (0x9)
#if TFM_SP_LOG_RAW_ENABLED
#define TFM_SVC_OUTPUT_UNPRIV_STRING    (0xA)
#endif

#endif /* __TFM_CORE_SVC_H__ */
