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

#ifndef __SLSI_FW_TEST_H__
#define __SLSI_FW_TEST_H__

#include <sys/types.h>
#include "dev.h"

struct slsi_fw_test {
	struct slsi_dev *sdev;
	bool fw_test_enabled;
	struct slsi_mbuf_work fw_test_work;
	pthread_mutex_t fw_test_lock;
	struct max_buff *mlme_add_vif_req[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1];
	struct max_buff *mlme_connect_req[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1];
	struct max_buff *mlme_connect_cfm[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1];
	struct max_buff *mlme_procedure_started_ind[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1];
};

void slsi_fw_test_init(struct slsi_dev *sdev, struct slsi_fw_test *fwtest);
void slsi_fw_test_deinit(struct slsi_dev *sdev, struct slsi_fw_test *fwtest);
int slsi_fw_test_signal(struct slsi_dev *sdev, struct slsi_fw_test *fwtest, struct max_buff *mbuf);
int slsi_fw_test_signal_with_udi_header(struct slsi_dev *sdev, struct slsi_fw_test *fwtest, struct max_buff *mbuf);
void slsi_fw_test_work(FAR void *arg);

#endif /*__SLSI_FW_TEST_H__*/
