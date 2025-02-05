/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#ifndef __CSI_PACKET_RECEIVER_H__
#define __CSI_PACKET_RECEIVER_H__

#include "csifw.h"

CSIFW_RES csi_packet_receiver_init(csi_config_type_t config_type, unsigned int interval_ms, CSIDataListener CSIDataCallback);
CSIFW_RES csi_packet_receiver_get_mac_addr(csifw_mac_info *mac_info);
CSIFW_RES csi_packet_receiver_start_collect(void);
CSIFW_RES csi_packet_receiver_stop_collect(CSIFW_REASON reason);
CSIFW_RES csi_packet_change_interval(unsigned int interval);
CSIFW_RES csi_packet_receiver_deinit(void);

#endif /* __CSI_PACKET_RECEIVER_H__ */

