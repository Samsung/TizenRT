/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_H__
#define __PLATFORM_OPTS_H__

#include "platform_autoconf.h"

#if defined CONFIG_BT && CONFIG_BT
#include "platform_opts_bt.h"
#endif

#ifndef CONFIG_AS_INIC_NP
#include "platform_opts_ap.h"
#endif

#endif //__PLATFORM_OPTS_H__

