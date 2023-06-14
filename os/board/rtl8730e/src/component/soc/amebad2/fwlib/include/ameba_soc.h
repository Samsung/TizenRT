/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _AMEBA_SOC_H_
#define _AMEBA_SOC_H_

/* rom headers */
#include "ameba.h"
#include "ameba_simulation.h"
//#include "strproc.h"
#include "ameba_captouch.h"

#ifndef CONFIG_BUILD_ROM
/* ram headers */
#include "platform_opts.h"
#include "ameba_ota.h"

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include "ameba_freertos_pmu.h"
#elif defined(PLATFORM_FREERTOS)
#include "ameba_freertos_pmu.h"
#include "freertos_pmu.h"
#include "FreeRTOS.h"
#include "task.h"

#if (tskKERNEL_VERSION_MAJOR>=10) && (tskKERNEL_VERSION_MINOR>=2)
#include "freertos_backtrace_ext.h"
#include "freertos_heap5_config.h"
#endif

#include "semphr.h"
#endif
#endif

//#define CONFIG_LINUX_FW_EN
#endif //_AMEBA_SOC_H_
