/*****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __SLSI_BA_H__
#define __SLSI_BA_H__

#include "dev.h"

void slsi_handle_blockack(struct netif *dev, struct slsi_peer *peer, u16 vif, u8 *peer_qsta_address, u16 parameter_set, u16 sequence_number, u16 reason_code, u16 direction);

int slsi_ba_process_frame(struct netif *dev, struct slsi_peer *peer, struct max_buff *mbuf, u16 sequence_number, u16 tid);

void slsi_ba_process_complete(struct netif *dev);

bool slsi_ba_check(struct slsi_peer *peer, u16 tid);

void slsi_rx_ba_stop_all(struct netif *dev, struct slsi_peer *peer);

void slsi_rx_ba_init(struct slsi_dev *sdev);
#endif
