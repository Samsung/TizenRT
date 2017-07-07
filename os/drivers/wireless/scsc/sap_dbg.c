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
#include "debug_scsc.h"
#include "dev.h"
#include "sap.h"
#include "sap_dbg.h"
#include "hip.h"

#define SUPPORTED_VERSION       13
#define SUPPORTED_OLD_VERSION   0

static int sap_dbg_version_supported(u16 version);
static int sap_dbg_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf);

static struct sap_api sap_dbg = {
	.sap_class = SAP_DBG,
	.sap_version_supported = sap_dbg_version_supported,
	.sap_handler = sap_dbg_rx_handler,
	.sap_versions = {SUPPORTED_VERSION, SUPPORTED_OLD_VERSION},
};

static int sap_dbg_version_supported(u16 version)
{
	unsigned int major = SAP_MAJOR(version);

#ifdef CONFIG_DEBUG
	unsigned int minor = SAP_MINOR(version);
#endif
	u8 i = 0;
	SLSI_INFO_NODEV("Reported version: %d.%d\n", major, minor);

	for (i = 0; i < SAP_MAX_VER; i++)
		if (sap_dbg.sap_versions[i] == major) {
			return 0;
		}

	pr_err("%s: Version %d.%d Not supported\n", __func__, major, minor);

	return -EINVAL;
}

static void slsi_rx_debug(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	u16 id = fapi_get_u16(mbuf, id);

	SLSI_UNUSED_PARAMETER(dev);

	switch (id) {
	case DEBUG_FAULT_IND:
		SLSI_WARN(sdev, "FAULT_IND: |cpu %s|id 0x%04X|arg 0x%08X|count %d|timestamp %10u|\n", ((fapi_get_u16(mbuf, u.debug_fault_ind.cpu) == 0x8000) ? "MAC" : (fapi_get_u16(mbuf, u.debug_fault_ind.cpu) == 0x4000) ? "PHY" : "???"), fapi_get_u16(mbuf, u.debug_fault_ind.faultid), fapi_get_u32(mbuf, u.debug_fault_ind.arg), fapi_get_u16(mbuf, u.debug_fault_ind.count), fapi_get_u32(mbuf, u.debug_fault_ind.timestamp));
		break;
	case DEBUG_WORD12IND:
		SLSI_DBG1(sdev, SLSI_FW_TEST, "FW DEBUG(id:%d, subid:%d, vif:%d, time:%u) %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X\n", fapi_get_u16(mbuf, u.debug_word12_ind.module_id), fapi_get_u16(mbuf, u.debug_word12_ind.module_sub_id), fapi_get_vif(mbuf), fapi_get_u32(mbuf, u.debug_word12_ind.timestamp), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[0]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[1]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[2]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[3]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[4]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[5]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[6]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[7]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[8]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[9]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[10]), fapi_get_u16(mbuf, u.debug_word12_ind.debug_words[11]));
		break;
	default:
		SLSI_DBG1(sdev, SLSI_MLME, "Unhandled Debug Ind: 0x%.4x\n", id);
		break;
	}
	slsi_kfree_mbuf(mbuf);
}

static int slsi_rx_dbg_sap(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	u16 id = fapi_get_u16(mbuf, id);

	switch (id) {
	case DEBUG_FAULT_IND:
	case DEBUG_WORD12IND:
	case DEBUG_GENERIC_IND:
		slsi_rx_debug(sdev, NULL, mbuf);
		break;
	default:
		slsi_kfree_mbuf(mbuf);
		SLSI_ERR(sdev, "Unhandled Ind: 0x%.4x\n", id);
		break;
	}

	return 0;
}

void slsi_rx_dbg_sap_work(FAR void *arg)
{
	FAR struct slsi_mbuf_work *w = (FAR struct slsi_mbuf_work *)arg;
	struct slsi_dev *sdev = w->sdev;
	struct max_buff *mbuf = slsi_mbuf_work_dequeue(w);

	while (mbuf) {
		slsi_rx_dbg_sap(sdev, mbuf);
		mbuf = slsi_mbuf_work_dequeue(w);
	}
}

static int sap_dbg_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	/* DEBUG SAP has a generic confirm. Theoretically, that
	 * can mean upper layer code can block on the confirm.
	 */
	if (slsi_rx_blocking_signals(sdev, mbuf) == 0) {
		return 0;
	}

	slsi_mbuf_work_enqueue(&sdev->rx_dbg_sap, mbuf, slsi_rx_dbg_sap_work);
	return 0;
}

int sap_dbg_init(void)
{
	SLSI_INFO_NODEV("Registering SAP\n");

	slsi_hip_sap_register(&sap_dbg);

	return 0;
}

int sap_dbg_deinit(void)
{
	SLSI_INFO_NODEV("Unregistering SAP\n");
	slsi_hip_sap_unregister(&sap_dbg);
	return 0;
}
