/**
 * Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 * All rights reserved
 * ----------------------------------------------------------------------------
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer. Each of the embodiments, including this information and
 * any derivative work shall retain this copyright notice.
 * ----------------------------------------------------------------------------
 */

#ifndef __DBH_INTR_HANDLER_H__
#define __DBH_INTR_HANDLER_H__
#include "dubhe_event.h"

#define STATUS_INTR_UNEXPECT_OCCUR   2
#define STATUS_INTR_EXPECT_OCCUR     1
#define STATUS_INTR_UNHAPPEN         0

#if defined( DUBHE_FOR_RUNTIME )
int dubhe_intr_handler( void );
int dubhe_intr_sync_handler( dubhe_event_type_t dubhe_event );
#endif
#endif

/*************************** The End Of File*****************************/
