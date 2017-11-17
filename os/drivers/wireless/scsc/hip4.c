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

#include <scsc/scsc_mx.h>
#include <scsc/scsc_mifram.h>
#include "hip4.h"
#include "mbulk.h"
#include "dev.h"
#include "utils_scsc.h"
#include "debug_scsc.h"

#ifdef CONFIG_SCSC_PLATFORM
#define SCSC_SCOREBOARD_VER  (1)
#else
#define SCSC_SCOREBOARD_VER  (0)
#endif

enum rw {
	widx,
	ridx,
};

/* Q mapping V3 - V4 */
/*offset of F/W owned indices */
#define FW_OWN_OFS      (64)
/**
 * HIP queue indices layout in the scoreboard (SC-505612-DD). v3
 *
 *             3        2        1        0
 *         +-----------------------------------+
 *     +0  |  Q3R   |   Q2R  |  Q1W   |  Q0W   |  Owned by the host
 *         +-----------------------------------+
 *     +4  |        |        |  Q5W   |  Q4R   |  Owned by the host
 *         +-----------------------------------+
 *
 *         +-----------------------------------+
 *    +64  |  Q3W   |   Q2W  |  Q1R   |  Q0R   |  Owned by the F/W
 *         +-----------------------------------+
 *    +68  |        |        |  Q5R   |  Q4W   |  Owned by the F/W
 *         +-----------------------------------+
 *
 * The queue indcies which owned by the host are only writable by the host.
 * F/W can only read them. And vice versa.
 */
static int q_idx_layout[6][2] = {
	{0, FW_OWN_OFS + 0},		/* mif_q_fh_ctl : 0 */
	{1, FW_OWN_OFS + 1},		/* mif_q_fh_dat : 1 */
	{FW_OWN_OFS + 2, 2},		/* mif_q_fh_rfb : 2 */
	{FW_OWN_OFS + 3, 3},		/* mif_q_th_ctl : 3 */
	{FW_OWN_OFS + 4, 4},		/* mif_q_th_dat : 4 */
	{5, FW_OWN_OFS + 5}			/* mif_q_th_rfb : 5 */
};

/*offset of F/W owned VIF Status */
#define FW_OWN_VIF      (96)
/**
 * HIP Pause state VIF. v4. 2 bits per PEER
 *
 *         +-----------------------------------+
 *    +96  |        VIF[0] Peers [15-1]        |  Owned by the F/W
 *         +-----------------------------------+
 *    +100 |        VIF[0] Peers [31-16]       |  Owned by the F/W
 *         +-----------------------------------+
 *    +104 |        VIF[1] Peers [15-1]        |  Owned by the F/W
 *         +-----------------------------------+
 *    +108 |        VIF[1] Peers [31-16]       |  Owned by the F/W
 *         +-----------------------------------+
 *    +112 |        VIF[2] Peers [15-1]        |  Owned by the F/W
 *         +-----------------------------------+
 *    +116 |        VIF[2] Peers [31-16]       |  Owned by the F/W
 *         +-----------------------------------+
 *    +120 |        VIF[3] Peers [15-1]        |  Owned by the F/W
 *         +-----------------------------------+
 *    +124 |        VIF[3] Peers [31-16]       |  Owned by the F/W
 *         +-----------------------------------+
 *
 */
/* Update scoreboard index */
/* Function can be called from BH context */
static void hip4_update_index(struct slsi_hip4 *hip, u32 q, enum rw r_w, u8 value)
{
	struct hip4_priv *hip_priv = hip->hip_priv;

	SLSI_MUTEX_LOCK(hip_priv->rw_scoreboard);
	if (hip->hip_priv->version == 3 || hip->hip_priv->version == 4) {
		*((u8 *)(hip->hip_priv->scbrd_base + q_idx_layout[q][r_w])) = value;
	} else {
		SLSI_ERR_NODEV("Incorrect version\n");
	}
	SLSI_MUTEX_UNLOCK(hip_priv->rw_scoreboard);
}

/* Read scoreboard index */
/* Function can be called from BH context */
static u8 hip4_read_index(struct slsi_hip4 *hip, u32 q, enum rw r_w)
{
	struct hip4_priv *hip_priv = hip->hip_priv;
	u32 value = 0;

	SLSI_MUTEX_LOCK(hip_priv->rw_scoreboard);
	if (hip->hip_priv->version == 3 || hip->hip_priv->version == 4) {
		value = *((u8 *)(hip->hip_priv->scbrd_base + q_idx_layout[q][r_w]));
	} else {
		SLSI_ERR_NODEV("Incorrect version\n");
	}
	SLSI_MUTEX_UNLOCK(hip_priv->rw_scoreboard);

	return value;
}

/* Transform mbuf to mbulk (fapi_signal + payload) */
static struct mbulk *hip4_mbuf_to_mbulk(struct hip4_priv *hip, struct max_buff *mbuf, bool ctrl_packet)
{
	struct mbulk *m = NULL;
	void *sig = NULL, *b_data = NULL;
	size_t payload = 0;
	u8 pool_id = ctrl_packet ? MBULK_CLASS_FROM_HOST_CTL : MBULK_CLASS_FROM_HOST_DAT;
	u8 headroom = 0, tailroom = 0;
	enum mbulk_class clas = ctrl_packet ? MBULK_CLASS_FROM_HOST_CTL : MBULK_CLASS_FROM_HOST_DAT;

	payload = mbuf->data_len - mbuf->fapi.sig_length;

	/* Get headroom/tailroom */
	headroom = hip->unidat_req_headroom;
	tailroom = hip->unidat_req_tailroom;

	/* Allocate mbulk */
	if (payload) {
		/* If signal include payload, add headroom and tailroom */
		m = mbulk_with_signal_alloc_by_pool(pool_id, mbuf->colour, clas, mbuf->fapi.sig_length + 4, payload + headroom + tailroom);
		if (!m) {
			return NULL;
		}
		if (!mbulk_reserve_head(m, headroom)) {
			return NULL;
		}
	} else {
		/* If it is only a signal do not add headroom */
		m = mbulk_with_signal_alloc_by_pool(pool_id, mbuf->colour, clas, mbuf->fapi.sig_length + 4, 0);
		if (!m) {
			return NULL;
		}
	}

	/* Get signal handler */
	sig = mbulk_get_signal(m);
	if (!sig) {
		mbulk_free_virt_host(m);
		return NULL;
	}

	/* Copy signal */
	/* 4Bytes offset is required for FW fapi header */
	memcpy(sig + 4, slsi_mbuf_get_data(mbuf), mbuf->fapi.sig_length);

	/* Copy payload */
	/* If the signal has payload memcpy the data */
	if (payload > 0) {
		/* Get head pointer */
		b_data = mbulk_dat_rw(m);
		if (!b_data) {
			mbulk_free_virt_host(m);
			return NULL;
		}
		/* Copy payload skipping the signal data */
		memcpy(b_data, slsi_mbuf_get_data(mbuf) + mbuf->fapi.sig_length, payload);
		mbulk_append_tail(m, payload);
	}
	m->flag |= MBULK_F_OBOUND;

	return m;
}

/* Transform mbulk to mbuf (fapi_signal + payload) */
static struct max_buff *hip4_mbulk_to_mbuf(struct scsc_service *service, struct mbulk *m, scsc_mifram_ref *to_free, bool cfm)
{
	struct mbulk *next_mbulk[MBULK_MAX_CHAIN];
	struct max_buff *mbuf = NULL;
	scsc_mifram_ref ref;
	scsc_mifram_ref m_chain_next;
	u8 free = 0;
	u8 i = 0, j = 0;
	u8 *p;
	size_t bytes_to_alloc = 0;

	/* Get the mif ref pointer, check for incorrect mbulk */
	if (scsc_mx_service_mif_ptr_to_addr(service, m, &ref)) {
		return NULL;
	}

	/* Track mbulk that should be freed */
	to_free[free++] = ref;

	bytes_to_alloc += m->sig_bufsz - 4;
	bytes_to_alloc += m->len;

	/* Detect Chained mbulk to start building the chain */
	if ((MBULK_SEG_IS_CHAIN_HEAD(m)) && (MBULK_SEG_IS_CHAINED(m))) {
		m_chain_next = mbulk_chain_next(m);
		if (!m_chain_next) {
			pr_err("%s: Mbulk is set MBULK_F_CHAIN_HEAD and MBULK_F_CHAIN but m_chain_next is NULL\n", __func__);
			goto cont;
		}
		while (1) {
			/* increase number mbulks in chain */
			i++;
			/* Get next_mbulk kernel address space pointer  */
			next_mbulk[i - 1] = scsc_mx_service_mif_addr_to_ptr(service, m_chain_next);
			if (!next_mbulk[i - 1]) {
				pr_err("%s: Mbulk is set as MBULK_F_CHAIN but next_mbulk is NULL\n", __func__);
				return NULL;
			}
			/* Track mbulk to be freed */
			to_free[free++] = m_chain_next;
			bytes_to_alloc += next_mbulk[i - 1]->len;
			if (MBULK_SEG_IS_CHAINED(next_mbulk[i - 1])) {
				/* continue traversing the chain */
				m_chain_next = mbulk_chain_next(next_mbulk[i - 1]);
				if (!m_chain_next) {
					break;
				}

				if (i >= MBULK_MAX_CHAIN) {
					pr_err("%s: Max number of chained MBULK reached\n", __func__);
					return NULL;
				}
			} else {
				break;
			}
		}
	}

cont:
	p = mbulk_get_signal(m);
	if (!p) {
		return NULL;
	}

	mbuf = mbuf_alloc(bytes_to_alloc);
	if (!mbuf) {
		return NULL;
	}

	mbuf->fapi.sig_length = m->sig_bufsz - 4;
	/* fapi_data_append adds to the data_length */
	mbuf->fapi.data_length = mbuf->fapi.sig_length;

	/* Remove 4Bytes offset coming from FW */
	p += 4;

	/* Don't need to copy the 4Bytes header coming from the FW */
	memcpy(mbuf_put(mbuf, mbuf->fapi.sig_length), p, mbuf->fapi.sig_length);

	if (m->len) {
		fapi_append_data(mbuf, mbulk_dat_r(m), m->len);
	}
	for (j = 0; j < i; j++) {
		fapi_append_data(mbuf, mbulk_dat_r(next_mbulk[j]), next_mbulk[j]->len);
	}

	return mbuf;
}

/* Add signal reference (offset in shared memory) in the selected queue */
/* This function should be called in atomic context. Callers should supply proper locking mechanism */
static int hip4_q_add_signal(struct slsi_hip4 *hip, enum hip4_hip_q_conf conf, scsc_mifram_ref phy_m, struct scsc_service *service)
{
	struct hip4_hip_control *ctrl = hip->hip_control;
	struct hip4_priv *hip_priv = hip->hip_priv;
	u8 idx_w;
	u8 idx_r;

	/* Read the current q write pointer */
	if (SCSC_SCOREBOARD_VER == 0) {
		idx_w = QW(hip_priv, conf);
	} else {
		idx_w = hip4_read_index(hip, conf, widx);
	}

	/* Read the current q read pointer */
	if (SCSC_SCOREBOARD_VER == 0) {
		idx_r = QR(hip_priv, conf);
	} else {
		idx_r = hip4_read_index(hip, conf, ridx);
	}

	/* Queueu is full */
	if (idx_r == ((idx_w + 1) & (MAX_NUM - 1))) {
		return -ENOSPC;
	}

	/* Update array */
	ctrl->q[conf].array[idx_w] = phy_m;

	/* Increase index */
	idx_w++;
	idx_w &= (MAX_NUM - 1);

	/* Update the scoreboard */
	if (SCSC_SCOREBOARD_VER == 0) {
		QW(hip_priv, conf) = idx_w;
	} else {
		hip4_update_index(hip, conf, widx, idx_w);
	}

	scsc_service_mifintrbit_bit_set(service, hip_priv->rx_intr_fromhost, SCSC_MIFINTR_TARGET_R4);

	return 0;
}

#ifdef CONFIG_SLSI_WLAN_STATS
void hip4_update_queue_stats(struct slsi_dev *sdev)
{
	struct slsi_hip4 *hip = &sdev->hip4_inst;
	struct hip4_priv *hip_priv = hip->hip_priv;

	if (hip_priv == NULL) {
		return;
	}

	if (SCSC_SCOREBOARD_VER == 0) {
		sdev->dp_stats.hip4_th_data_idx_r = QR(hip_priv, HIP4_MIF_Q_TH_DAT);
		sdev->dp_stats.hip4_th_data_idx_w = QW(hip_priv, HIP4_MIF_Q_TH_DAT);

		sdev->dp_stats.hip4_th_ctrl_idx_r = QR(hip_priv, HIP4_MIF_Q_TH_CTRL);
		sdev->dp_stats.hip4_th_ctrl_idx_w = QW(hip_priv, HIP4_MIF_Q_TH_CTRL);

		sdev->dp_stats.hip4_th_rfb_idx_r = QR(hip_priv, HIP4_MIF_Q_TH_RFB);
		sdev->dp_stats.hip4_th_rfb_idx_w = QW(hip_priv, HIP4_MIF_Q_TH_RFB);

		sdev->dp_stats.hip4_fh_data_idx_r = QR(hip_priv, HIP4_MIF_Q_FH_DAT);
		sdev->dp_stats.hip4_fh_data_idx_w = QW(hip_priv, HIP4_MIF_Q_FH_DAT);

		sdev->dp_stats.hip4_fh_ctrl_idx_r = QR(hip_priv, HIP4_MIF_Q_FH_CTRL);
		sdev->dp_stats.hip4_fh_ctrl_idx_w = QW(hip_priv, HIP4_MIF_Q_FH_CTRL);

		sdev->dp_stats.hip4_fh_rfb_idx_r = QR(hip_priv, HIP4_MIF_Q_FH_RFB);
		sdev->dp_stats.hip4_fh_rfb_idx_w = QW(hip_priv, HIP4_MIF_Q_FH_RFB);
	} else {
		sdev->dp_stats.hip4_th_data_idx_r = hip4_read_index(hip, HIP4_MIF_Q_TH_DAT, ridx);
		sdev->dp_stats.hip4_th_data_idx_w = hip4_read_index(hip, HIP4_MIF_Q_TH_DAT, widx);

		sdev->dp_stats.hip4_th_ctrl_idx_r = hip4_read_index(hip, HIP4_MIF_Q_TH_CTRL, ridx);
		sdev->dp_stats.hip4_th_ctrl_idx_w = hip4_read_index(hip, HIP4_MIF_Q_TH_CTRL, widx);

		sdev->dp_stats.hip4_th_rfb_idx_r = hip4_read_index(hip, HIP4_MIF_Q_TH_RFB, ridx);
		sdev->dp_stats.hip4_th_rfb_idx_w = hip4_read_index(hip, HIP4_MIF_Q_TH_RFB, widx);

		sdev->dp_stats.hip4_fh_data_idx_r = hip4_read_index(hip, HIP4_MIF_Q_FH_DAT, ridx);
		sdev->dp_stats.hip4_fh_data_idx_w = hip4_read_index(hip, HIP4_MIF_Q_FH_DAT, widx);

		sdev->dp_stats.hip4_fh_ctrl_idx_r = hip4_read_index(hip, HIP4_MIF_Q_FH_CTRL, ridx);
		sdev->dp_stats.hip4_fh_ctrl_idx_w = hip4_read_index(hip, HIP4_MIF_Q_FH_CTRL, widx);

		sdev->dp_stats.hip4_fh_rfb_idx_r = hip4_read_index(hip, HIP4_MIF_Q_FH_RFB, ridx);
		sdev->dp_stats.hip4_fh_rfb_idx_w = hip4_read_index(hip, HIP4_MIF_Q_FH_RFB, widx);
	}
}
#endif

/* Work handler for hip4. The function processes the mbulk and
 * unmasks the intr bit.
 */
static void hip4_wq(FAR void *arg)
{
	struct hip4_priv *hip_priv = (FAR struct hip4_priv *)arg;
	struct slsi_hip4 *hip = hip_priv->hip;
	struct hip4_hip_control *ctrl = hip->hip_control;
	scsc_mifram_ref ref;
	void *mem;
	struct mbulk *m;
	u8 idx_r;
	u8 idx_w;
	struct slsi_dev *sdev = container_of(hip, struct slsi_dev, hip4_inst);
	struct scsc_service *service;
	bool update = false;

	if (!sdev || !sdev->service) {
		return;
	}
	service = sdev->service;

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_hip_wq);

	if (SCSC_SCOREBOARD_VER == 0) {
		idx_r = QR(hip_priv, HIP4_MIF_Q_FH_RFB);
		idx_w = QW(hip_priv, HIP4_MIF_Q_FH_RFB);
	} else {
		idx_r = hip4_read_index(hip, HIP4_MIF_Q_FH_RFB, ridx);
		idx_w = hip4_read_index(hip, HIP4_MIF_Q_FH_RFB, widx);
		update = false;
	}

	while (idx_r != idx_w) {
		u16 colour;

		ref = ctrl->q[HIP4_MIF_Q_FH_RFB].array[idx_r];
		mem = scsc_mx_service_mif_addr_to_ptr(service, ref);
		m = (struct mbulk *)mem;
		if (m == NULL) {
			SLSI_ERR_NODEV("FB: Mbulk is NULL\n");
			goto consume_fb_mbulk;
		}
		/* colour is defined as: */
		/* u16 register bits:
		 * 0      - do not use
		 * [2:1]  - vif
		 * [7:3]  - peer_index
		 * [10:8] - ac queue
		 */
		colour = ((m->clas & 0xc0) << 2) | (m->pid & 0xfe);
		/* Account ONLY for data RFB */
		if ((m->pid & 0x1) == MBULK_CLASS_FROM_HOST_DAT) {
			slsi_hip_tx_done(sdev, colour);    /* Ignore return value */
		}
		mbulk_free_virt_host(m);
consume_fb_mbulk:
		/* Increase index */
		idx_r++;
		idx_r &= (MAX_NUM - 1);
		update = true;
	}
	/* Update the scoreboard */
	if (SCSC_SCOREBOARD_VER == 0) {
		QR(hip_priv, HIP4_MIF_Q_FH_RFB) = (u8)idx_r;
	} else if (update) {
		hip4_update_index(hip, HIP4_MIF_Q_FH_RFB, ridx, idx_r);
	}

	if (SCSC_SCOREBOARD_VER == 0) {
		idx_r = QR(hip_priv, HIP4_MIF_Q_TH_CTRL);
		idx_w = QW(hip_priv, HIP4_MIF_Q_TH_CTRL);
	} else {
		idx_r = hip4_read_index(hip, HIP4_MIF_Q_TH_CTRL, ridx);
		idx_w = hip4_read_index(hip, HIP4_MIF_Q_TH_CTRL, widx);
		update = false;
	}

	while (idx_r != idx_w) {
		struct max_buff *mbuf;
		/* Currently the max number to be freed is 2. In future
		 * implementations (i.e. AMPDU) this number may be bigger
		 * list of mbulks to be freedi
		 */
		scsc_mifram_ref to_free[MBULK_MAX_CHAIN + 1] = { 0 };
		u8 i = 0;

		/* Catch-up with idx_w */
		ref = ctrl->q[HIP4_MIF_Q_TH_CTRL].array[idx_r];
		mem = scsc_mx_service_mif_addr_to_ptr(service, ref);
		m = (struct mbulk *)(mem);
		/* Process Control Signal */

		mbuf = hip4_mbulk_to_mbuf(service, m, to_free, false);
		if (!mbuf) {
			pr_err("%s: Ctrl: Error parsing mbuf\n", __func__);
			goto consume_ctl_mbulk;
		}

		if (slsi_hip_rx(sdev, mbuf) < 0) {
			pr_err("%s: Ctrl: Error detected slsi_hip_rx\n", __func__);

			slsi_kfree_mbuf(mbuf);
		}
consume_ctl_mbulk:
		/* Increase index */
		idx_r++;
		idx_r &= (MAX_NUM - 1);

		/* Go through the list of references to free */
		while ((ref = to_free[i++]))
			/* return to the firmware */
			if (hip4_q_add_signal(hip, HIP4_MIF_Q_TH_RFB, ref, service)) {
				/* We need to know when this happens. We need to wait until space becomes free.
				 * We need to engineer the size of the ring buffers to ensure this is a rare event.
				 * After retrying once, we could panic or something.
				 */
				/* need to retry later */
			}
		update = true;
	}
	/* Update the scoreboard */
	if (SCSC_SCOREBOARD_VER == 0) {
		QR(hip_priv, HIP4_MIF_Q_TH_CTRL) = (u8)idx_r;
	} else if (update) {
		hip4_update_index(hip, HIP4_MIF_Q_TH_CTRL, ridx, idx_r);
	}

	if (SCSC_SCOREBOARD_VER == 0) {
		/* Continue Q[HIP4_MIF_Q_TH_DAT] (4) */
		idx_r = QR(hip_priv, HIP4_MIF_Q_TH_DAT);
		idx_w = QW(hip_priv, HIP4_MIF_Q_TH_DAT);
	} else {
		idx_r = hip4_read_index(hip, HIP4_MIF_Q_TH_DAT, ridx);
		idx_w = hip4_read_index(hip, HIP4_MIF_Q_TH_DAT, widx);
		update = false;
	}

	while (idx_r != idx_w) {
		struct max_buff *mbuf;
		/* Currently the max number to be freed is 2. In future
		 * implementations (i.e. AMPDU) this number may be bigger
		 */
		/* list of mbulks to be freed */
		scsc_mifram_ref to_free[MBULK_MAX_CHAIN + 1] = { 0 };
		u8 i = 0;

		/* Catch-up with idx_w */
		ref = ctrl->q[HIP4_MIF_Q_TH_DAT].array[idx_r];
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_hip_rx_data_pkts);
		mem = scsc_mx_service_mif_addr_to_ptr(service, ref);
		m = (struct mbulk *)(mem);
		mbuf = hip4_mbulk_to_mbuf(service, m, to_free, false);
		if (!mbuf) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_mem_alloc_failed);

			goto consume_dat_mbulk;
		}
		if (slsi_hip_rx(sdev, mbuf) < 0) {
			slsi_kfree_mbuf(mbuf);
		}
consume_dat_mbulk:
		/* Increase index */
		idx_r++;
		idx_r &= (MAX_NUM - 1);

		/* Go through the list of references to free */
		while ((ref = to_free[i++]))
			/* return to the firmware */
			if (hip4_q_add_signal(hip, HIP4_MIF_Q_TH_RFB, ref, service)) {
				/* We need to know when this happens. We need to wait until space becomes free.
				 * We need to engineer the size of the ring buffers to ensure this is a rare event.
				 * After retrying once, we could panic or something.
				 */
			}
		update = true;
	}
	/* Update the scoreboard */
	if (SCSC_SCOREBOARD_VER == 0) {
		QR(hip_priv, HIP4_MIF_Q_TH_DAT) = (u8)idx_r;
	} else if (update) {
		hip4_update_index(hip, HIP4_MIF_Q_TH_DAT, ridx, idx_r);
	}

	if (!(hip->hip_priv->closing)) {
		scsc_service_mifintrbit_bit_unmask(service, hip->hip_priv->rx_intr_tohost);
	}
}

/* IRQ handler for hip4. The function runs in Interrupt context, so all the
 * asssumptions related to interrupt should be applied (sleep, fast,...)
 */
static void hip4_irq_handler(int irq, void *data)
{
	struct slsi_hip4 *hip = (struct slsi_hip4 *)data;
	struct slsi_dev *sdev;
	struct scsc_service *service;

	(void)irq;					/* unused */

	if (hip == NULL || hip->hip_priv == NULL) {
		return;
	}

	sdev = container_of(hip, struct slsi_dev, hip4_inst);
	if (sdev->service == NULL) {
		return;
	}
	service = sdev->service;

	/* Mask interrupt to avoid interrupt storm and let BH run */
	scsc_service_mifintrbit_bit_mask(service, hip->hip_priv->rx_intr_tohost);

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_interrupts);

	work_queue(SLSI_HIP_WORK_QID, &hip->hip_priv->intr_wq, hip4_wq, (FAR void *)hip->hip_priv, 0);

	/* Clear interrupt */
	scsc_service_mifintrbit_bit_clear(service, hip->hip_priv->rx_intr_tohost);
}

/**** OFFSET SHOULD BE 4096 BYTES ALIGNED ***/
#define IMG_MGR_SEC_WLAN_CONFIG_SIZE    0x2000	/* 8 kB */
#define IMG_MGR_SEC_WLAN_MIB_SIZE       0x01000	/* 4 kB */

#ifdef CONFIG_SCSC_WLANLITE
#define IMG_MGR_SEC_WLAN_TX_DAT_SIZE    0x02000	/* 8 kB */
#define IMG_MGR_SEC_WLAN_TX_CTL_SIZE    0x00800	/* 2 kB */
#define IMG_MGR_SEC_WLAN_RX_SIZE        0x48000	/* 288 kB */
#else
#define IMG_MGR_SEC_WLAN_TX_DAT_SIZE    0x1C000	/* 112 kB */
#define IMG_MGR_SEC_WLAN_TX_CTL_SIZE    0x03800	/* 14 kB */
#define IMG_MGR_SEC_WLAN_RX_SIZE        0x2C000	/* 176 kB */
#endif

#define IMG_MGR_SEC_WLAN_TX_SIZE        (IMG_MGR_SEC_WLAN_TX_DAT_SIZE + IMG_MGR_SEC_WLAN_TX_CTL_SIZE)

char *hip4_get_from_host_ctrl_pool_addr(char *fh_pool_addr)
{
	char *fh_ctrl_pool_addr;

	fh_ctrl_pool_addr = (fh_pool_addr + IMG_MGR_SEC_WLAN_TX_DAT_SIZE);

	return fh_ctrl_pool_addr;
}

int hip4_init(struct slsi_hip4 *hip)
{
	struct hip4_hip_control *hip_control;
	struct scsc_service *service;
	struct slsi_dev *sdev = container_of(hip, struct slsi_dev, hip4_inst);
	scsc_mifram_ref ref;
	scsc_mifram_ref ref_scoreboard;
	int i;
	int ret;
	char *from_host_pool_addr;
	char *from_host_ctrl_pool_addr;
	char *to_host_pool_addr;

	if (!sdev || !sdev->service) {
		return -EINVAL;
	}

	hip->hip_priv = kmm_zalloc(sizeof(*hip->hip_priv));
	if (!hip->hip_priv) {
		return -ENOMEM;
	}

	/* Used in the workqueue */
	hip->hip_priv->hip = hip;

	service = sdev->service;

	hip->hip_priv->host_pool_id_dat = MBULK_CLASS_FROM_HOST_DAT;
	hip->hip_priv->host_pool_id_ctl = MBULK_CLASS_FROM_HOST_CTL;

	/* Queue memories */
	if (scsc_mx_service_mifram_alloc(sdev->service, IMG_MGR_SEC_WLAN_TX_SIZE, &hip->hip_priv->from_host_pool, 4096)) {
		pr_err("%s: Not enough memory to allocate from_host_pool\n", __func__);
		return -ENOMEM;
	}
	if (scsc_mx_service_mifram_alloc(sdev->service, IMG_MGR_SEC_WLAN_RX_SIZE, &hip->hip_priv->to_host_pool, 4096)) {
		pr_err("%s: Not enough memory to allocate to_host_pool\n", __func__);
		return -ENOMEM;
	}

	/**
	 * This gets sliced up into two regions, the data and control regions based on IMG_MGR_SEC_WLAN_TX_DAT_SIZE
	 * (at the front) and IMG_MGR_SEC_WLAN_TX_CTL_SIZE (the rest)
	 */
	from_host_pool_addr = scsc_mx_service_mif_addr_to_ptr(sdev->service, hip->hip_priv->from_host_pool);
	memset(from_host_pool_addr, 0, IMG_MGR_SEC_WLAN_TX_SIZE);

	to_host_pool_addr = scsc_mx_service_mif_addr_to_ptr(sdev->service, hip->hip_priv->to_host_pool);
	memset(to_host_pool_addr, 0, IMG_MGR_SEC_WLAN_RX_SIZE);

	/* Configure mbulk allocator - Data QUEUES */
	ret = mbulk_pool_add(MBULK_CLASS_FROM_HOST_DAT, from_host_pool_addr, from_host_pool_addr + IMG_MGR_SEC_WLAN_TX_DAT_SIZE, (IMG_MGR_SEC_WLAN_TX_DAT_SIZE / HIP4_DAT_SLOTS) - sizeof(struct mbulk), 0);
	if (ret) {
		return ret;
	}

	from_host_ctrl_pool_addr = hip4_get_from_host_ctrl_pool_addr(from_host_pool_addr);
	/* Configure mbulk allocator - Control QUEUES */
	ret = mbulk_pool_add(MBULK_CLASS_FROM_HOST_CTL, from_host_ctrl_pool_addr, from_host_ctrl_pool_addr + IMG_MGR_SEC_WLAN_TX_CTL_SIZE, (IMG_MGR_SEC_WLAN_TX_CTL_SIZE / HIP4_CTL_SLOTS) - sizeof(struct mbulk), 0);
	if (ret) {
		return ret;
	}

	/* TOHOST Handler allocator */
	hip->hip_priv->rx_intr_tohost = scsc_service_mifintrbit_register_tohost(service, hip4_irq_handler, hip);

	/* Mask the interrupt to prevent intr been kicked during start */
	scsc_service_mifintrbit_bit_mask(service, hip->hip_priv->rx_intr_tohost);

	/* FROMHOST Handler allocator */
	hip->hip_priv->rx_intr_fromhost = scsc_service_mifintrbit_alloc_fromhost(service, SCSC_MIFINTR_TARGET_R4);

	/**
	 * Get hip_control pointer on shared memory - this needs to be aligned on a cache line of the
	 * wifi subsystem
	 */
	if (scsc_mx_service_mifram_alloc(sdev->service, sizeof(struct hip4_hip_control), &hip->hip_priv->hip_control, 32)) {
		return -ENOMEM;
	}

	hip_control = scsc_mx_service_mif_addr_to_ptr(sdev->service, hip->hip_priv->hip_control);

	/* Reset hip_control table */
	memset(hip_control, 0, sizeof(struct hip4_hip_control));

	/* Initialize scoreboard */
	if (scsc_mx_service_mif_ptr_to_addr(service, &hip_control->scoreboard, &ref_scoreboard)) {
		return -EFAULT;
	}

	/* Initialize hip_control table for version 4 */
	/***** VERSION 4 *******/
	hip_control->config_v4.magic_number = 0xcaba0401;
	hip_control->config_v4.hip_config_ver = 4;
	hip_control->config_v4.config_len = sizeof(struct hip4_hip_config_version_4);
	hip_control->config_v4.host_cache_line = 64;
	hip_control->config_v4.host_buf_loc = hip->hip_priv->from_host_pool;
	hip_control->config_v4.fw_buf_loc = hip->hip_priv->to_host_pool;
	hip_control->config_v4.host_buf_sz = IMG_MGR_SEC_WLAN_TX_SIZE;
	hip_control->config_v4.fw_buf_sz = IMG_MGR_SEC_WLAN_RX_SIZE;
	hip_control->config_v4.mib_loc = 0;
	hip_control->config_v4.mib_sz = 0;
	hip_control->config_v4.log_config_loc = 0;
	hip_control->config_v4.mif_fh_int_n = hip->hip_priv->rx_intr_fromhost;
	hip_control->config_v4.mif_th_int_n = hip->hip_priv->rx_intr_tohost;
	hip_control->config_v4.scbrd_loc = (u32)ref_scoreboard;
	hip_control->config_v4.q_num = 6;
	hip_control->config_v4.q_len = 256;
	hip_control->config_v4.q_idx_sz = 1;
	/* Initialize q relative positions */
	for (i = 0; i < MIF_HIP_CFG_Q_NUM; i++) {
		if (scsc_mx_service_mif_ptr_to_addr(service, &hip_control->q[i].array, &ref)) {
			return -EFAULT;
		}
		hip_control->config_v4.q_loc[i] = (u32)ref;
	}
	/***** END VERSION 4 *******/

	/* Initialize hip_control table for version 3 */
	/***** VERSION 3 *******/
	hip_control->config_v3.magic_number = 0xcaba0401;
	hip_control->config_v3.hip_config_ver = 3;
	hip_control->config_v3.config_len = sizeof(struct hip4_hip_config_version_3);
	hip_control->config_v3.host_cache_line = 64;
	hip_control->config_v3.host_buf_loc = hip->hip_priv->from_host_pool;
	hip_control->config_v3.fw_buf_loc = hip->hip_priv->to_host_pool;
	hip_control->config_v3.host_buf_sz = IMG_MGR_SEC_WLAN_TX_SIZE;
	hip_control->config_v3.fw_buf_sz = IMG_MGR_SEC_WLAN_RX_SIZE;
	hip_control->config_v3.mib_loc = 0;
	hip_control->config_v3.mib_sz = 0;
	hip_control->config_v3.log_config_loc = 0;
	hip_control->config_v3.mif_fh_int_n = hip->hip_priv->rx_intr_fromhost;
	hip_control->config_v3.mif_th_int_n = hip->hip_priv->rx_intr_tohost;
	hip_control->config_v3.q_num = 6;
	hip_control->config_v3.q_len = 256;
	hip_control->config_v3.q_idx_sz = 1;
	hip_control->config_v3.scbrd_loc = (u32)ref_scoreboard;	/* scoreboard location */

	/* Initialize q relative positions */
	for (i = 0; i < MIF_HIP_CFG_Q_NUM; i++) {
		if (scsc_mx_service_mif_ptr_to_addr(service, &hip_control->q[i].array, &ref)) {
			return -EFAULT;
		}
		hip_control->config_v3.q_loc[i] = (u32)ref;
	}
	/***** END VERSION 3 *******/

	/* Initialzie hip_init configuration */
	hip_control->init.magic_number = 0xcaaa0400;
	if (scsc_mx_service_mif_ptr_to_addr(service, &hip_control->config_v4, &ref)) {
		return -EFAULT;
	}
	hip_control->init.version_a_ref = ref;

	if (scsc_mx_service_mif_ptr_to_addr(service, &hip_control->config_v3, &ref)) {
		return -EFAULT;
	}
	hip_control->init.version_b_ref = ref;
	/* End hip_init configuration */

	hip->hip_control = hip_control;
	hip->hip_priv->q_indice = (struct hip_q_indice *)(&hip_control->q[0].array);
	hip->hip_priv->scbrd_base = &hip_control->scoreboard;

	pthread_mutex_init(&hip->hip_priv->rx_lock, NULL);

	SLSI_MUTEX_INIT(hip->hip_priv->rw_scoreboard);

	return 0;
}

/**
 * This function is in charge to transmit a frame through the HIP.
 * It does NOT take ownership of the MBUF unless it successfully transmit it;
 * as a consequence mbuf is NOT freed on error.
 * We return ENOSPC on queue related troubles in order to trigger upper
 * layers of kernel to requeue/retry.
 * We free ONLY locally-allocated stuff.
 */
int scsc_wifi_transmit_frame(struct slsi_hip4 *hip, bool ctrl_packet, struct max_buff *mbuf)
{
	struct scsc_service *service;
	scsc_mifram_ref offset;
	struct mbulk *m;
	struct slsi_dev *sdev;
	struct fapi_signal_header *fapi_header;
#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
	struct timespec sig_time;
#endif

	if (hip == NULL) {
		return ERR_VAL;
	}
	sdev = container_of(hip, struct slsi_dev, hip4_inst);
	if (sdev->service == NULL) {
		return ERR_VAL;
	}
	service = sdev->service;

	if (!hip || !sdev || !sdev->service || !mbuf || !hip->hip_priv) {
#ifdef CONFIG_SLSI_WLAN_STATS
		if (!ctrl_packet) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_hip4_invalid_param);
		}
#endif
		return ERR_VAL;
	}

	service = sdev->service;

	fapi_header = (struct fapi_signal_header *)slsi_mbuf_get_data(mbuf);

	m = hip4_mbuf_to_mbulk(hip->hip_priv, mbuf, ctrl_packet);
	if (m == NULL) {
#ifdef CONFIG_SLSI_WLAN_STATS
		if (!ctrl_packet) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_hip4_queue_full);
		}
#endif
		return ERR_BUF;
	}

	if (scsc_mx_service_mif_ptr_to_addr(service, m, &offset) < 0) {
#ifdef CONFIG_SLSI_WLAN_STATS
		if (!ctrl_packet) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_mifptr_to_addr_fail);
		}
#endif
		mbulk_free_virt_host(m);
		return ERR_VAL;
	}

	if (hip4_q_add_signal(hip, ctrl_packet ? HIP4_MIF_Q_FH_CTRL : HIP4_MIF_Q_FH_DAT, offset, service)) {
#ifdef CONFIG_SLSI_WLAN_STATS
		if (!ctrl_packet) {
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_drop_fail_to_add_in_hipq);
		}
#endif
		mbulk_free_virt_host(m);
		return ERR_BUF;
	}

	/* slsi_log_clients_log_signal_fast: mbuf is copied to all the log clients */
	slsi_log_clients_log_signal_fast(sdev, &sdev->log_clients, mbuf, SLSI_LOG_DIRECTION_FROM_HOST);

#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
	if ((fapi_get_sigid(mbuf) & FAPI_SAP_TYPE_MASK) != FAPI_SAP_TYPE_MA) {
		clock_gettime(CLOCK_REALTIME, &sig_time);
		slsi_wlan_fapi_log_add_record(fapi_get_sigid(mbuf), sig_time);
	}
#endif

#ifdef CONFIG_SLSI_WLAN_STATS
	if (!ctrl_packet) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_data_hip);
	}
#endif

	return ERR_OK;
}

/* HIP4 has been initialize, setup with values
 * provided by FW
 */
int hip4_setup(struct slsi_hip4 *hip)
{
	struct slsi_dev *sdev = container_of(hip, struct slsi_dev, hip4_inst);
	struct scsc_service *service;
	u32 conf_hip4_ver = 0;

	if (!sdev || !sdev->service) {
		return -EIO;
	}

	service = sdev->service;

	/* Get the Version reported by the FW */
	conf_hip4_ver = scsc_wifi_get_hip_config_version(&hip->hip_control->init);
	/* Check if the version is supported. And get the index */
	/* This is hardcoded and may change in future versions */
	if (conf_hip4_ver != 4 && conf_hip4_ver != 3) {
		pr_err("%s: FW Version %d not supported\n", __func__, conf_hip4_ver);
		return -EIO;
	}

	/* If version 4 is used */
	if (conf_hip4_ver == 4) {
		hip->hip_priv->unidat_req_headroom = scsc_wifi_get_hip_config_u8(&hip->hip_control, unidat_req_headroom, 4);
		hip->hip_priv->unidat_req_tailroom = scsc_wifi_get_hip_config_u8(&hip->hip_control, unidat_req_tailroom, 4);
		hip->hip_priv->version = 4;
	} else {
		/* version 3 */
		hip->hip_priv->unidat_req_headroom = scsc_wifi_get_hip_config_u8(&hip->hip_control, unidat_req_headroom, 3);
		hip->hip_priv->unidat_req_tailroom = scsc_wifi_get_hip_config_u8(&hip->hip_control, unidat_req_tailroom, 3);
		hip->hip_priv->version = 3;
	}
	/* Unmask interrupts - now host should handle them */
	hip->hip_priv->closing = 0;
	scsc_service_mifintrbit_bit_unmask(service, hip->hip_priv->rx_intr_tohost);

	return 0;
}

void hip4_freeze(struct slsi_hip4 *hip)
{
	struct slsi_dev *sdev = container_of(hip, struct slsi_dev, hip4_inst);
	struct scsc_service *service;

	if (!sdev || !sdev->service) {
		return;
	}

	service = sdev->service;
	hip->hip_priv->closing = 1;

	scsc_service_mifintrbit_bit_mask(service, hip->hip_priv->rx_intr_tohost);

	work_cancel(SLSI_HIP_WORK_QID, &hip->hip_priv->intr_wq);
}

void hip4_deinit(struct slsi_hip4 *hip)
{
	struct slsi_dev *sdev = container_of(hip, struct slsi_dev, hip4_inst);
	struct scsc_service *service;

	if (!sdev || !sdev->service) {
		return;
	}

	service = sdev->service;

	hip->hip_priv->closing = 1;
	scsc_service_mifintrbit_bit_mask(service, hip->hip_priv->rx_intr_tohost);

	work_cancel(SLSI_HIP_WORK_QID, &hip->hip_priv->intr_wq);
	scsc_service_mifintrbit_unregister_tohost(service, hip->hip_priv->rx_intr_tohost);
	scsc_service_mifintrbit_free_fromhost(service, hip->hip_priv->rx_intr_fromhost, SCSC_MIFINTR_TARGET_R4);
	scsc_mx_service_mifram_free(sdev->service, sdev->hip4_inst.hip_priv->from_host_pool);
	scsc_mx_service_mifram_free(sdev->service, sdev->hip4_inst.hip_priv->to_host_pool);
	scsc_mx_service_mifram_free(sdev->service, sdev->hip4_inst.hip_priv->hip_control);

	kmm_free(hip->hip_priv);
	hip->hip_priv = NULL;
}
