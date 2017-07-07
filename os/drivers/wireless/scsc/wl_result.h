/*****************************************************************************
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

#ifndef WL_RESULT_H__
#define WL_RESULT_H__

#define CsrResult u16

#define CSR_RESULT_SUCCESS ((CsrResult)0x0000)
#define CSR_RESULT_FAILURE ((CsrResult)0xffff)

#define CSR_WIFI_HIP_RESULT_INVALID_VALUE    ((CsrResult)1)	/* Invalid argument value */
#define CSR_WIFI_HIP_RESULT_NO_DEVICE        ((CsrResult)2)	/* The specified device is no longer present */
#define CSR_WIFI_HIP_RESULT_NO_SPACE         ((CsrResult)3)	/* A queue or buffer is full */
#define CSR_WIFI_HIP_RESULT_NO_MEMORY        ((CsrResult)4)	/* Fatal error, no memory */
#define CSR_WIFI_HIP_RESULT_RANGE            ((CsrResult)5)	/* Request exceeds the range of a file or a buffer */
#define CSR_WIFI_HIP_RESULT_NOT_FOUND        ((CsrResult)6)	/* A file (typically a f/w patch) is not found */
#define CSR_WIFI_PS_RESULT_FULL              ((CsrResult)7)	/* Signal successfully queued, queue now full. */
#define CSR_WIFI_PS_QSIG_NONE                ((CsrResult)8)	/* no qsig found, queue empty */
#define CSR_WIFI_PS_QSIG_NO_SPACE            ((CsrResult)9)	/* no room for outstanding qsigs */
#define CSR_WIFI_PS_QS_PAUSED_UNTIL_BOT_UPDATE     ((CsrResult)10)	/* BOT has disabled the qset and waiting for updates from the firmware */

#endif
