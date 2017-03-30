/*****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 * WLAN result codes
 *
 ****************************************************************************/

#ifndef WL_RESULT_H__
#define WL_RESULT_H__

#define CsrResult u16

#define CSR_RESULT_SUCCESS ((CsrResult)0x0000)
#define CSR_RESULT_FAILURE ((CsrResult)0xffff)

#define CSR_WIFI_HIP_RESULT_INVALID_VALUE    ((CsrResult)1)        /* Invalid argument value */
#define CSR_WIFI_HIP_RESULT_NO_DEVICE        ((CsrResult)2)        /* The specified device is no longer present */
#define CSR_WIFI_HIP_RESULT_NO_SPACE         ((CsrResult)3)        /* A queue or buffer is full */
#define CSR_WIFI_HIP_RESULT_NO_MEMORY        ((CsrResult)4)        /* Fatal error, no memory */
#define CSR_WIFI_HIP_RESULT_RANGE            ((CsrResult)5)        /* Request exceeds the range of a file or a buffer */
#define CSR_WIFI_HIP_RESULT_NOT_FOUND        ((CsrResult)6)        /* A file (typically a f/w patch) is not found */
#define CSR_WIFI_PS_RESULT_FULL              ((CsrResult)7)        /* Signal successfully queued, queue now full. */
#define CSR_WIFI_PS_QSIG_NONE                ((CsrResult)8)        /* no qsig found, queue empty */
#define CSR_WIFI_PS_QSIG_NO_SPACE            ((CsrResult)9)        /* no room for outstanding qsigs */
#define CSR_WIFI_PS_QS_PAUSED_UNTIL_BOT_UPDATE     ((CsrResult)10) /* BOT has disabled the qset and waiting for updates from the firmware */

#endif
