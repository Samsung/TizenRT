#ifndef __PLATFORM_OPTS_BT_H__
#define __PLATFORM_OPTS_BT_H__
#include "platform_autoconf.h"

#define VERSION_2019		1
#define VERSION_2021		2
#define UPPER_STACK_VERSION	VERSION_2021

#if defined CONFIG_BT && CONFIG_BT
#if ((defined CONFIG_BT_NP && CONFIG_BT_NP) || (defined CONFIG_BT_SINGLE_CORE && CONFIG_BT_SINGLE_CORE))
#define CONFIG_FTL_ENABLED
#endif
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER) || \
    (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE)
#define CONFIG_BT_MESH    1
#endif
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER) && \
    (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE)
#error "CONFIG_BT_MESH_PROVISIONER & CONFIG_BT_MESH_DEVICE can not open at the same time"
#endif
#endif

#endif // __PLATFORM_OPTS_BT_H__

