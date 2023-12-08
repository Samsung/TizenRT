#ifndef __PLATFORM_OPTS_BT_H__
#define __PLATFORM_OPTS_BT_H__
#include "platform_autoconf.h"

#define VERSION_2019		1
#define VERSION_2021		2
#define UPPER_STACK_VERSION	VERSION_2021

#if defined CONFIG_BT && CONFIG_BT
#ifndef CONFIG_FTL_ENABLED
#define CONFIG_FTL_ENABLED
#endif
#endif

#endif // __PLATFORM_OPTS_BT_H__

