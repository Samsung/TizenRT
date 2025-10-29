/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SVC_NUM_H__
#define __SVC_NUM_H__

/*
 * SVC numbers for FF-M compliant implementations.
 * 0x0 ~ 0x7F can be only called from Thread Mode, the rest from ISR only.
 */

/********************* SVC for Thread Mode ************************************/
/* PSA Client APIs */
#define TFM_SVC_PSA_FRAMEWORK_VERSION   (0x0)
#define TFM_SVC_PSA_VERSION             (0x1)
#define TFM_SVC_PSA_CONNECT             (0x2)
#define TFM_SVC_PSA_CALL                (0x3)
#define TFM_SVC_PSA_CLOSE               (0x4)
/* PSA Secure Partition APIs */
#define TFM_SVC_PSA_WAIT                (0x5)
#define TFM_SVC_PSA_GET                 (0x6)
#define TFM_SVC_PSA_SET_RHANDLE         (0x7)
#define TFM_SVC_PSA_READ                (0x8)
#define TFM_SVC_PSA_SKIP                (0x9)
#define TFM_SVC_PSA_WRITE               (0xA)
#define TFM_SVC_PSA_REPLY               (0xB)
#define TFM_SVC_PSA_NOTIFY              (0xC)
#define TFM_SVC_PSA_CLEAR               (0xD)
#define TFM_SVC_PSA_EOI                 (0xE)
#define TFM_SVC_PSA_PANIC               (0xF)
#define TFM_SVC_PSA_LIFECYCLE           (0x10)
#define TFM_SVC_PSA_IRQ_ENABLE          (0x11)
#define TFM_SVC_PSA_IRQ_DISABLE         (0x12)
#define TFM_SVC_PSA_RESET_SIGNAL        (0x13)
/* TF-M specific, starts from 0x40 */
#define TFM_SVC_GET_BOOT_DATA           (0x40)
#define TFM_SVC_SPM_INIT                (0x41)
#define TFM_SVC_FLIH_FUNC_RETURN        (0x42)
#define TFM_SVC_THREAD_NUMBER_END       (0x7F)
#if TFM_SP_LOG_RAW_ENABLED
#define TFM_SVC_OUTPUT_UNPRIV_STRING    (TFM_SVC_THREAD_NUMBER_END)
#endif

#define TFM_SVC_HANDLER_NUMBER_START    (0x80)

/********************* SVC for interrupt handling *****************************/
#define TFM_SVC_PREPARE_DEPRIV_FLIH     (0x80)

#endif /* __SVC_NUM_H__ */
