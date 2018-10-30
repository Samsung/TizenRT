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

#include "mxman.h"
#include "scsc_mx_impl.h"
#include "mifintrbit.h"
#include "miframman.h"
#include "mifmboxman.h"
#include "srvman.h"
#include "servman_messages.h"
#include "mxmgmt_transport.h"
#include "debug_scsc.h"
#include <pthread.h>
#include <errno.h>
#include <tinyara/kmalloc.h>

/* Timeout Service Manager start/stop (ms) - default 1000. 0 = infinite */
static ulong sm_completion_timeout_ms = 1000;
#define SCSC_MX_SERVICE_RECOVERY_TIMEOUT 20000	/* 20 seconds */

struct scsc_service {
	struct slsi_dlist_head list;
	struct scsc_mx *mx;
	enum scsc_service_id id;
	struct scsc_service_client *client;
	struct completion sm_msg_start_completion;
	struct completion sm_msg_stop_completion;
	struct work_s sm_msg_start_cfm_work;
};

void srvman_init(struct srvman *srvman, struct scsc_mx *mx)
{
	SLSI_INFO_NODEV("\n");
	srvman->mx = mx;
	SLSI_INIT_DLIST_HEAD(&srvman->service_list);
	pthread_mutex_init(&srvman->service_list_mutex, NULL);
	pthread_mutex_init(&srvman->api_access_mutex, NULL);
}

void srvman_deinit(struct srvman *srvman)
{
	struct scsc_service *service, *next;

	SLSI_INFO_NODEV("\n");
	slsi_dlist_for_each_entry_safe(service, next, &srvman->service_list, list) {
		slsi_dlist_del(&service->list);
		kmm_free(service);
	}
	pthread_mutex_destroy(&srvman->api_access_mutex);
	pthread_mutex_destroy(&srvman->service_list_mutex);
}

void srvman_set_error(struct srvman *srvman)
{
	struct scsc_service *service;

	SLSI_INFO_NODEV("\n");
	srvman->error = true;
	pthread_mutex_lock(&srvman->service_list_mutex);
	slsi_dlist_for_each_entry(service, &srvman->service_list, list) {
		complete(&service->sm_msg_start_completion);
		complete(&service->sm_msg_stop_completion);
	}
	pthread_mutex_unlock(&srvman->service_list_mutex);
}

static int wait_for_sm_msg_start_cfm(struct scsc_service *service)
{
	int r;

	if (0 == sm_completion_timeout_ms) {
		/* Zero implies infinite wait, for development use only.
		 * r = -ERESTARTSYS if interrupted (e.g. Ctrl-C), 0 if completed
		 */
		r = wait_for_completion_interruptible(&service->sm_msg_start_completion);
		if (r == -ERESTARTSYS) {
			/* Paranoid sink of any pending event skipped by the interrupted wait */
			r = wait_for_completion_timeout(&service->sm_msg_start_completion, 500);
			if (r == 0) {
				SLSI_ERR_NODEV("timed out\n");
				return -ETIMEDOUT;
			}
		}
		return r;
	}

	r = wait_for_completion_timeout(&service->sm_msg_start_completion, sm_completion_timeout_ms);
	if (r == 0) {
		SLSI_ERR_NODEV("timeout\n");
		return -ETIMEDOUT;
	}
	return 0;
}

static int wait_for_sm_msg_stop_cfm(struct scsc_service *service)
{
	int r;

	if (0 == sm_completion_timeout_ms) {
		/* Zero implies infinite wait, for development use only.
		 * r = -ERESTARTSYS if interrupted (e.g. Ctrl-C), 0 if completed
		 */
		r = wait_for_completion_interruptible(&service->sm_msg_stop_completion);
		if (r == -ERESTARTSYS) {
			/* Paranoid sink of any pending event skipped by the interrupted wait */
			r = wait_for_completion_timeout(&service->sm_msg_stop_completion, 500);
			if (r == 0) {
				SLSI_ERR_NODEV(" timed out\n");
				return -ETIMEDOUT;
			}
		}
		return r;
	}
	r = wait_for_completion_timeout(&service->sm_msg_stop_completion, sm_completion_timeout_ms);
	if (r == 0) {
		SLSI_ERR_NODEV("timeout\n");
		return -ETIMEDOUT;
	}

	return 0;
}

static int send_sm_msg_start_blocking(struct scsc_service *service, scsc_mifram_ref ref)
{
	struct scsc_mx *mx = service->mx;
	struct mxmgmt_transport *mxmgmt_transport = scsc_mx_get_mxmgmt_transport(mx);
	int r;
	struct sm_msg_packet message = {.service_id = service->id,
		.msg = SM_MSG_START_REQ,
		 .optional_data = ref
	};

	reinit_completion(&service->sm_msg_start_completion);

	/* Send to FW in MM stream */
	mxmgmt_transport_send(mxmgmt_transport, MMTRANS_CHAN_ID_SERVICE_MANAGEMENT, &message, sizeof(message));
	r = wait_for_sm_msg_start_cfm(service);
	if (r) {
		SLSI_ERR_NODEV("wait_for_sm_msg_start_cfm() failed: r=%d\n", r);
	}
	return r;
}

static int send_sm_msg_stop_blocking(struct scsc_service *service)
{
	struct scsc_mx *mx = service->mx;
	struct mxman *mxman = scsc_mx_get_mxman(mx);
	struct mxmgmt_transport *mxmgmt_transport = scsc_mx_get_mxmgmt_transport(mx);
	int r;
	struct sm_msg_packet message = {.service_id = service->id,
		.msg = SM_MSG_STOP_REQ,
		 .optional_data = 0
	};
	if (mxman->mxman_state == MXMAN_STATE_FAILED) {
		return 0;
	}

	reinit_completion(&service->sm_msg_stop_completion);

	/* Send to FW in MM stream */
	mxmgmt_transport_send(mxmgmt_transport, MMTRANS_CHAN_ID_SERVICE_MANAGEMENT, &message, sizeof(message));
	r = wait_for_sm_msg_stop_cfm(service);
	if (r) {
		SLSI_ERR_NODEV("wait_for_sm_msg_stop_cfm() for service=%p service->id=%d failed: r=%d\n", service, service->id, r);
	}
	return r;
}

void sm_start_cfm_complete(struct scsc_service *service)
{
	complete(&service->sm_msg_start_completion);
}

/*
 * Receive handler for messages from the FW along the maxwell management transport
 */
static void srv_message_handler(const void *message, void *data)
{
	struct srvman *srvman = (struct srvman *)data;
	struct scsc_service *service;
	const struct sm_msg_packet *msg = message;
	bool found = false;

	pthread_mutex_lock(&srvman->service_list_mutex);
	slsi_dlist_for_each_entry(service, &srvman->service_list, list) {
		if (service->id == msg->service_id) {
			found = true;
			break;
		}
	}
	pthread_mutex_unlock(&srvman->service_list_mutex);
	if (!found) {
		SLSI_ERR_NODEV("No service for msg->service_id=%d", msg->service_id);
		return;
	}
	/* Forward the message to the applicable service to deal with */
	switch (msg->msg) {
	case SM_MSG_START_CFM:
		SLSI_INFO_NODEV("Received SM_MSG_START_CFM message service=%p with service_id=%d from the firmware\n", service, msg->service_id);

		work_queue(LPWORK, &service->sm_msg_start_cfm_work, (worker_t)sm_start_cfm_complete, service, 0);
		break;
	case SM_MSG_STOP_CFM:
		SLSI_INFO_NODEV("Received SM_MSG_STOP_CFM message for service=%p with service_id=%d from the firmware\n", service, msg->service_id);
		complete(&service->sm_msg_stop_completion);
		break;
	default:
		/* HERE: Unknown message, raise fault */
		pr_warn("%s: Received unknown message for service=%p with service_id=%d from the firmware: msg->msg=%d\n", __func__, service, msg->msg, msg->service_id);
		break;
	}
}

int scsc_mx_service_start(struct scsc_service *service, scsc_mifram_ref ref)
{
	struct scsc_mx *mx = service->mx;
	struct srvman *srvman = scsc_mx_get_srvman(mx);
	int r;

	pthread_mutex_lock(&srvman->api_access_mutex);
	if (srvman->error) {
		SLSI_ERR_NODEV("error\n");
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return -EIO;
	}

	r = send_sm_msg_start_blocking(service, ref);
	if (r) {
		SLSI_ERR_NODEV("send_sm_msg_start_blocking() failed: r=%d\n", r);
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return r;
	}
	pthread_mutex_unlock(&srvman->api_access_mutex);
	return 0;
}

int scsc_mx_service_stop(struct scsc_service *service)
{
	struct scsc_mx *mx = service->mx;
	struct srvman *srvman = scsc_mx_get_srvman(mx);
	int r;

	SLSI_INFO_NODEV("\n");
	pthread_mutex_lock(&srvman->api_access_mutex);
	if (srvman->error) {
		SLSI_ERR_NODEV("\n");
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return -EIO;
	}

	r = send_sm_msg_stop_blocking(service);
	if (r) {
		SLSI_ERR_NODEV("send_sm_msg_stop_blocking() failed: r=%d\n", r);
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return -EIO;
	}
	pthread_mutex_unlock(&srvman->api_access_mutex);
	return 0;
}

void srvman_freeze_services(struct srvman *srvman)
{
	struct scsc_service *service;
	struct mxman *mxman = scsc_mx_get_mxman(srvman->mx);

	SLSI_INFO_NODEV("\n");
	pthread_mutex_lock(&srvman->service_list_mutex);
	slsi_dlist_for_each_entry(service, &srvman->service_list, list) {
		service->client->stop_on_failure(service->client);
	}
	init_completion(&mxman->recovery_completion);
	pthread_mutex_unlock(&srvman->service_list_mutex);
	SLSI_INFO_NODEV("OK\n");
}

void srvman_clear_error(struct srvman *srvman)
{
	SLSI_INFO_NODEV("\n");
	srvman->error = false;
}

void srvman_unfreeze_services(struct srvman *srvman, u16 scsc_panic_code)
{
	struct scsc_service *service;

	SLSI_INFO_NODEV("\n");
	pthread_mutex_lock(&srvman->service_list_mutex);
	slsi_dlist_for_each_entry(service, &srvman->service_list, list) {
		service->client->failure_reset(service->client, scsc_panic_code);
	}
	pthread_mutex_unlock(&srvman->service_list_mutex);
	SLSI_INFO_NODEV("OK\n");
}

/** Signal a failure detected by the Client. This will trigger the systemwide
 * failure handling procedure: _All_ Clients will be called back via
 * their stop_on_failure() handler as a side-effect.
 */
void scsc_mx_service_service_failed(struct scsc_service *service)
{
	struct scsc_mx *mx = service->mx;
	struct srvman *srvman = scsc_mx_get_srvman(mx);

	SLSI_INFO_NODEV("\n");
	srvman_set_error(srvman);
	mxman_fail(scsc_mx_get_mxman(mx));
}

void scsc_mx_service_close(struct scsc_service *service)
{
	struct mxman *mxman = scsc_mx_get_mxman(service->mx);
	struct scsc_mx *mx = service->mx;
	struct srvman *srvman = scsc_mx_get_srvman(mx);
	bool empty;

	SLSI_INFO_NODEV("\n");
	pthread_mutex_lock(&srvman->api_access_mutex);
	if (srvman->error) {
		SLSI_ERR_NODEV("error\n");
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return;
	}
	/* remove the service from the list and deallocate the service memory */
	pthread_mutex_lock(&srvman->service_list_mutex);
	slsi_dlist_del(&service->list);
	empty = slsi_dlist_empty(&srvman->service_list);
	pthread_mutex_unlock(&srvman->service_list_mutex);
	if (empty)
		/* unregister channel handler */
	{
		mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mx), MMTRANS_CHAN_ID_SERVICE_MANAGEMENT, NULL, NULL);
	}

	kmm_free(service);
	mxman_close(mxman);
	pthread_mutex_unlock(&srvman->api_access_mutex);
}

/* Consider move to a public scsc_mx interface */
struct scsc_service *scsc_mx_service_open(struct scsc_mx *mx, enum scsc_service_id id, struct scsc_service_client *client)
{
	int ret;
	struct scsc_service *service;
	struct srvman *srvman = scsc_mx_get_srvman(mx);
	struct mxman *mxman = scsc_mx_get_mxman(mx);
	bool empty;

	pthread_mutex_lock(&srvman->api_access_mutex);

	if (srvman->error) {
		SLSI_ERR_NODEV("error\n");
		pthread_mutex_unlock(&srvman->api_access_mutex);
		return NULL;
	}

	if (mxman->mxman_state == MXMAN_STATE_FAILED) {
		int r;

		pthread_mutex_unlock(&srvman->api_access_mutex);
		r = wait_for_completion_timeout(&mxman->recovery_completion, SCSC_MX_SERVICE_RECOVERY_TIMEOUT);
		if (r == 0) {
			SLSI_ERR_NODEV("Recovery timeout\n");
			return NULL;
		}

		pthread_mutex_lock(&srvman->api_access_mutex);
	}
	service = kmm_malloc(sizeof(struct scsc_service));
	if (service) {
		/* MaxwellManager Should allocate Mem and download FW */
		ret = mxman_open(mxman);
		if (ret) {
			kmm_free(service);
			pthread_mutex_unlock(&srvman->api_access_mutex);
			return NULL;
		}
		/* Initialise service struct here */
		service->mx = mx;
		service->id = id;
		service->client = client;
		service->list.lprev = NULL;
		service->list.lnext = NULL;

		init_completion(&service->sm_msg_start_completion);
		init_completion(&service->sm_msg_stop_completion);
		pthread_mutex_lock(&srvman->service_list_mutex);
		empty = slsi_dlist_empty(&srvman->service_list);
		pthread_mutex_unlock(&srvman->service_list_mutex);
		if (empty) {
			mxmgmt_transport_register_channel_handler(scsc_mx_get_mxmgmt_transport(mx), MMTRANS_CHAN_ID_SERVICE_MANAGEMENT, &srv_message_handler, srvman);
		}

		pthread_mutex_lock(&srvman->service_list_mutex);
		slsi_dlist_add_tail(&service->list, &srvman->service_list);
		pthread_mutex_unlock(&srvman->service_list_mutex);
	}

	pthread_mutex_unlock(&srvman->api_access_mutex);

	return service;
}

/** Allocate a contiguous block of SDRAM accessible to Client Driver */
int scsc_mx_service_mifram_alloc(struct scsc_service *service, size_t nbytes, scsc_mifram_ref *ref, u32 align)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;
	void *mem;
	int ret;

	mif_abs = scsc_mx_get_mif_abs(mx);

	mem = miframman_alloc(scsc_mx_get_ramman(mx), nbytes, align);

	if (!mem) {
		SLSI_ERR_NODEV("miframman_alloc() failed\n");
		return -ENOMEM;
	}

	/* Transform native pointer and get mifram_ref type */
	ret = scsc_mx_service_mif_ptr_to_addr(service, mem, ref);
	if (ret) {
		SLSI_ERR_NODEV("scsc_mx_service_mif_ptr_to_addr() failed: ret=%d", ret);
		miframman_free(scsc_mx_get_ramman(mx), mem);
	}

	return ret;
}

/** Free a contiguous block of SDRAM */
void scsc_mx_service_mifram_free(struct scsc_service *service, scsc_mifram_ref ref)
{
	struct scsc_mx *mx = service->mx;
	void *mem;

	mem = scsc_mx_service_mif_addr_to_ptr(service, ref);

	miframman_free(scsc_mx_get_ramman(mx), mem);
}

/* MIF ALLOCATIONS */
bool scsc_mx_service_alloc_mboxes(struct scsc_service *service, int n, int *first_mbox_index)
{
	struct scsc_mx *mx = service->mx;

	return mifmboxman_alloc_mboxes(scsc_mx_get_mboxman(mx), n, first_mbox_index);
}

void scsc_service_free_mboxes(struct scsc_service *service, int n, int first_mbox_index)
{
	struct scsc_mx *mx = service->mx;

	return mifmboxman_free_mboxes(scsc_mx_get_mboxman(mx), first_mbox_index, n);
}

u32 *scsc_mx_service_get_mbox_ptr(struct scsc_service *service, int mbox_index)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mifmboxman_get_mbox_ptr(scsc_mx_get_mboxman(mx), mif_abs, mbox_index);
}

int scsc_service_mifintrbit_bit_mask_status_get(struct scsc_service *service)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_bit_mask_status_get(mif_abs);
}

int scsc_service_mifintrbit_get(struct scsc_service *service)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_get(mif_abs);
}

void scsc_service_mifintrbit_bit_set(struct scsc_service *service, int which_bit, enum scsc_mifintr_target dir)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_bit_set(mif_abs, which_bit, dir);
}

void scsc_service_mifintrbit_bit_clear(struct scsc_service *service, int which_bit)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_bit_clear(mif_abs, which_bit);
}

void scsc_service_mifintrbit_bit_mask(struct scsc_service *service, int which_bit)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_bit_mask(mif_abs, which_bit);
}

void scsc_service_mifintrbit_bit_unmask(struct scsc_service *service, int which_bit)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->irq_bit_unmask(mif_abs, which_bit);
}

int scsc_service_mifintrbit_alloc_fromhost(struct scsc_service *service, enum scsc_mifintr_target dir)
{
	struct scsc_mx *mx = service->mx;

	return mifintrbit_alloc_fromhost(scsc_mx_get_intrbit(mx), dir);
}

int scsc_service_mifintrbit_free_fromhost(struct scsc_service *service, int which_bit, enum scsc_mifintr_target dir)
{
	struct scsc_mx *mx = service->mx;

	return mifintrbit_free_fromhost(scsc_mx_get_intrbit(mx), which_bit, dir);
}

int scsc_service_mifintrbit_register_tohost(struct scsc_service *service, void (*handler)(int irq, void *data), void *data)
{
	struct scsc_mx *mx = service->mx;

	return mifintrbit_alloc_tohost(scsc_mx_get_intrbit(mx), handler, data);
}

int scsc_service_mifintrbit_unregister_tohost(struct scsc_service *service, int which_bit)
{
	struct scsc_mx *mx = service->mx;

	return mifintrbit_free_tohost(scsc_mx_get_intrbit(mx), which_bit);
}

void *scsc_mx_service_mif_addr_to_ptr(struct scsc_service *service, scsc_mifram_ref ref)
{
	struct scsc_mx *mx = service->mx;

	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	return mif_abs->get_mifram_ptr(mif_abs, ref);
}

int scsc_mx_service_mif_ptr_to_addr(struct scsc_service *service, void *mem_ptr, scsc_mifram_ref *ref)
{
	struct scsc_mx *mx = service->mx;
	struct scsc_mif_abs *mif_abs;

	mif_abs = scsc_mx_get_mif_abs(mx);

	/* Transform native pointer and get mifram_ref type */
	if (mif_abs->get_mifram_ref(mif_abs, mem_ptr, ref)) {
		SLSI_ERR_NODEV("ooops somethig went wrong");
		return -ENOMEM;
	}

	return 0;
}

int scsc_service_force_panic(struct scsc_service *service)
{
	struct mxman *mxman = scsc_mx_get_mxman(service->mx);

	return mxman_force_panic(mxman);
}
