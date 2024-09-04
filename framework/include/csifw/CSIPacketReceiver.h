#ifndef __CSI_PACKET_RECEIVER_H__
#define __CSI_PACKET_RECEIVER_H__

#include "csifw/csifw.h"

CSIFW_RES csi_packet_receiver_init(CSIDataListener CSIDataCallback, get_empty_data_buffer get_buf_cb);
CSIFW_RES csi_packet_receiver_get_mac_addr(csifw_mac_info *mac_info);
CSIFW_RES csi_packet_receiver_start_collect(rtw_csi_action_parm_t *config);
CSIFW_RES csi_packet_receiver_pause_collect(rtw_csi_action_parm_t *config);
CSIFW_RES csi_packet_receiver_stop_collect(CSIFW_REASON reason, rtw_csi_action_parm_t *config);
CSIFW_RES csi_packet_receiver_deinit();

#endif /* __CSI_PACKET_RECEIVER_H__ */
