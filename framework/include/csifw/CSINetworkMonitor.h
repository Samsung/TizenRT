#ifndef __CSI_NETWORK_MONITOR_H__
#define __CSI_NETWORK_MONITOR_H__
#include "csifw/csifw.h"

CSIFW_RES network_monitor_init(network_status_listener network_status_callback);
CSIFW_RES network_monitor_deinit();
#endif /* __CSI_NETWORK_MONITOR_H__ */
