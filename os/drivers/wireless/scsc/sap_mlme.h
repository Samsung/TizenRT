/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __SAP_MLME_H__
#define __SAP_MLME_H__

int sap_mlme_init(void);
int sap_mlme_deinit(void);

/* MLME signal handlers in rx.c */
void slsi_rx_scan_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_scan_done_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_connect_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_connected_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_received_frame_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_disconnect_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_disconnected_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_procedure_started_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_frame_transmission_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_mic_failure_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
void slsi_rx_blockack_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);

#endif
