/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
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

#include "dubhe_regs.h"
#include "dubhe_trng.h"
#include "dubhe_driver.h"
#include "pal_string.h"
#include "pal_log.h"
#include "pal_time.h"
#include "dubhe_event.h"

void arm_ce_dbg_set(bool debug_en)
{
	if (debug_en) {
		DBH_WRITE_REGISTER( DBG_CTRL, DBG_CTRL, 0xFFFFFFFF );
	} else {
		DBH_WRITE_REGISTER( DBG_CTRL, DBG_CTRL, 0 );
		DBH_WRITE_REGISTER( DBG_CTRL, DBG_CTRL_LOCK, 0xFFFFFFFF );
	}
}
