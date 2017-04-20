/****************************************************************************
 *
 * Copyright (c) 2012 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __SLSI_FW_TEST_H__
#define __SLSI_FW_TEST_H__

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
