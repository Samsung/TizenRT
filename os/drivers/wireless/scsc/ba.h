/****************************************************************************
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
