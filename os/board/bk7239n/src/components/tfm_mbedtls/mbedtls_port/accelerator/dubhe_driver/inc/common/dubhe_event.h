/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef __DUBHE_EVENT_H__
#define __DUBHE_EVENT_H__

#include "pal.h"

typedef enum _dubhe_event_type_t {
    DBH_EVENT_SCA_CMD_EXCUTED = 0,
    DBH_EVENT_HASH_CMD_EXCUTED,
    DBH_EVENT_ACA_DONE,
    DBH_EVENT_ACA_FIFO_READY,
} dubhe_event_type_t;

typedef enum _dubhe_mutex_type_t {
    DBH_SCA_MUTEX = 0,
    DBH_HASH_MUTEX,
    DBH_ACA_MUTEX,
    DBH_TRNG_MUTEX,
    DBH_OTP_MUTEX,
} dubhe_mutex_type_t;

int32_t dubhe_event_init( void );
void dubhe_event_cleanup( void );
#if defined(DUBHE_FOR_RUNTIME)
int32_t dubhe_wait_event( dubhe_event_type_t dubhe_event );
int32_t dubhe_release_event( dubhe_event_type_t dubhe_event );
void dubhe_event_set_sca_status(int32_t status);
void dubhe_event_set_hash_status(int32_t status);
#endif
int32_t dubhe_mutex_lock( dubhe_mutex_type_t dubhe_mutex );
int32_t dubhe_mutex_unlock( dubhe_mutex_type_t dubhe_mutex );

#endif
/*************************** The End Of File*****************************/
