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

/** Implements */
#include "gdb_transport.h"

/** Uses */
//#include <tinyara/module.h>
#include "mifintrbit.h"
#include <pthread.h>
#include <errno.h>
#include <tinyara/kmalloc.h>
#include "debug_scsc.h"

struct clients_node {
	struct slsi_dlist_head list;
	struct gdb_transport_client *gdb_client;
};

struct gdb_transport_node {
	struct slsi_dlist_head list;
	struct gdb_transport *gdb_transport;
};

static struct gdb_transport_module {
	struct slsi_dlist_head clients_list;
	struct slsi_dlist_head gdb_transport_list;
} gdb_transport_module = {
	.clients_list = SLSI_DLIST_HEAD_INIT(gdb_transport_module.clients_list), .gdb_transport_list = SLSI_DLIST_HEAD_INIT(gdb_transport_module.gdb_transport_list)
};

static void input_irq_handler(int irq, void *data)
{
	struct gdb_transport *gdb_transport = (struct gdb_transport *)data;
	struct scsc_mif_abs *mif_abs;
	u32 num_bytes;
	u32 alloc_bytes;
	char *buf;

	/* 1st length */
	/* Clear the interrupt first to ensure we can't possibly miss one */
	mif_abs = scsc_mx_get_mif_abs(gdb_transport->mx);
	mif_abs->irq_bit_clear(mif_abs, irq);
	while (mif_stream_read(&gdb_transport->mif_istream, &num_bytes, sizeof(uint32_t))) {
		if (num_bytes > 0 && num_bytes < (GDB_TRANSPORT_BUF_LENGTH - sizeof(uint32_t))) {
			alloc_bytes = sizeof(char) * num_bytes;
			buf = kmm_malloc(alloc_bytes);
			/* 2nd payload (msg) */
			mif_stream_read(&gdb_transport->mif_istream, buf, num_bytes);
			gdb_transport->channel_handler_fn(buf, num_bytes, gdb_transport->channel_handler_data);
			kmm_free(buf);
		} else {
			SLSI_ERR_NODEV("Incorrect num_bytes\n");
		}
	}
}

/** MIF Interrupt handler for acknowledging reads made by the AP */
static void output_irq_handler(int irq, void *data)
{
	struct scsc_mif_abs *mif_abs;
	struct gdb_transport *gdb_transport = (struct gdb_transport *)data;

	/* Clear the interrupt first to ensure we can't possibly miss one */
	/* The FW read some data from the output stream.
	 * Currently we do not care, so just clear the interrupt. */
	mif_abs = scsc_mx_get_mif_abs(gdb_transport->mx);
	mif_abs->irq_bit_clear(mif_abs, irq);
}

static void gdb_transport_probe_registered_clients(struct gdb_transport *gdb_transport)
{
	bool client_registered = false;
	struct clients_node *gdb_client_node, *gdb_client_next;
	struct scsc_mif_abs *mif_abs;
	char *dev_uid;

	/* Traverse Linked List for each mif_driver node */
	slsi_dlist_for_each_entry_safe(gdb_client_node, gdb_client_next, &gdb_transport_module.clients_list, list) {
		/* Get UID */
		mif_abs = scsc_mx_get_mif_abs(gdb_transport->mx);
		dev_uid = mif_abs->get_uid(mif_abs);
		gdb_client_node->gdb_client->probe(gdb_client_node->gdb_client, gdb_transport, dev_uid);
		client_registered = true;
	}
	if (client_registered == false) {
		SLSI_INFO_NODEV("No clients registered\n");
	}
}

void gdb_transport_release(struct gdb_transport *gdb_transport)
{
	struct clients_node *gdb_client_node, *gdb_client_next;
	struct gdb_transport_node *gdb_transport_node, *gdb_transport_node_next;
	bool match = false;

	slsi_dlist_for_each_entry_safe(gdb_transport_node, gdb_transport_node_next, &gdb_transport_module.gdb_transport_list, list) {
		if (gdb_transport_node->gdb_transport == gdb_transport) {
			match = true;
			/* Need to notify clients using the transport has been released */
			slsi_dlist_for_each_entry_safe(gdb_client_node, gdb_client_next, &gdb_transport_module.clients_list, list) {
				gdb_client_node->gdb_client->remove(gdb_client_node->gdb_client, gdb_transport);
			}
			slsi_dlist_del(&gdb_transport_node->list);
			kmm_free(gdb_transport_node);
		}
	}
	if (match == false) {
		SLSI_INFO_NODEV("No match for given scsc_mif_abs\n");
	}

	mif_stream_release(&gdb_transport->mif_istream);
	mif_stream_release(&gdb_transport->mif_ostream);
}

void gdb_transport_config_serialise(struct gdb_transport *gdb_transport, struct mxtransconf *trans_conf)
{
	mif_stream_config_serialise(&gdb_transport->mif_istream, &trans_conf->to_ap_stream_conf);
	mif_stream_config_serialise(&gdb_transport->mif_ostream, &trans_conf->from_ap_stream_conf);
}

/** Public functions */
int gdb_transport_init(struct gdb_transport *gdb_transport, struct scsc_mx *mx, enum gdb_transport_enum type)
{
	int r;
	uint32_t mem_length = GDB_TRANSPORT_BUF_LENGTH;
	uint32_t packet_size = 4;
	uint32_t num_packets;
	struct gdb_transport_node *gdb_transport_node;

	gdb_transport_node = kmm_zalloc(sizeof(*gdb_transport_node));
	if (!gdb_transport_node) {
		return -EIO;
	}

	memset(gdb_transport, 0, sizeof(struct gdb_transport));
	num_packets = mem_length / packet_size;
	pthread_mutex_init(&gdb_transport->channel_handler_mutex, NULL);
	gdb_transport->mx = mx;
	r = mif_stream_init(&gdb_transport->mif_istream, MIF_STREAM_DIRECTION_IN, num_packets, packet_size, mx, input_irq_handler, gdb_transport);
	if (r) {
		kmm_free(gdb_transport_node);
		return r;
	}
	r = mif_stream_init(&gdb_transport->mif_ostream, MIF_STREAM_DIRECTION_OUT, num_packets, packet_size, mx, output_irq_handler, gdb_transport);
	if (r) {
		mif_stream_release(&gdb_transport->mif_istream);
		kmm_free(gdb_transport_node);
		return r;
	}

	gdb_transport->channel_handler_fn = NULL;
	gdb_transport->channel_handler_data = NULL;

	gdb_transport_node->gdb_transport = gdb_transport;
	/* Add gdb_transport node */
	slsi_dlist_add_tail(&gdb_transport_node->list, &gdb_transport_module.gdb_transport_list);

	gdb_transport->type = type;
	gdb_transport_probe_registered_clients(gdb_transport);
	return 0;
}

void gdb_transport_send(struct gdb_transport *gdb_transport, void *message, uint32_t message_length)
{
	/*int i; */
	char msg[300];

	memcpy(msg, message, message_length);

	pthread_mutex_lock(&gdb_transport->channel_handler_mutex);
	/* 1st length */
	mif_stream_write(&gdb_transport->mif_ostream, &message_length, sizeof(uint32_t));
	/* 2nd payload (msg) */
	mif_stream_write(&gdb_transport->mif_ostream, message, message_length);
	pthread_mutex_unlock(&gdb_transport->channel_handler_mutex);
}

void gdb_transport_register_channel_handler(struct gdb_transport *gdb_transport, gdb_channel_handler handler, void *data)
{
	pthread_mutex_lock(&gdb_transport->channel_handler_mutex);
	gdb_transport->channel_handler_fn = handler;
	gdb_transport->channel_handler_data = (void *)data;
	pthread_mutex_unlock(&gdb_transport->channel_handler_mutex);
}

int gdb_transport_register_client(struct gdb_transport_client *gdb_client)
{
	struct clients_node *gdb_client_node;
	struct gdb_transport_node *gdb_transport_node;
	struct scsc_mif_abs *mif_abs;
	char *dev_uid;

	/* Add node in modules linked list */
	gdb_client_node = kmm_zalloc(sizeof(*gdb_client_node));
	if (!gdb_client_node) {
		return -ENOMEM;
	}

	gdb_client_node->gdb_client = gdb_client;
	slsi_dlist_add_tail(&gdb_client_node->list, &gdb_transport_module.clients_list);

	/* Traverse Linked List for transport registered */
	slsi_dlist_for_each_entry(gdb_transport_node, &gdb_transport_module.gdb_transport_list, list) {
		/* Get UID */
		mif_abs = scsc_mx_get_mif_abs(gdb_transport_node->gdb_transport->mx);
		dev_uid = mif_abs->get_uid(mif_abs);
		gdb_client->probe(gdb_client, gdb_transport_node->gdb_transport, dev_uid);
	}
	return 0;
}

void gdb_transport_unregister_client(struct gdb_transport_client *gdb_client)
{
	struct clients_node *gdb_client_node, *gdb_client_next;

	/* Traverse Linked List for each client_list  */
	slsi_dlist_for_each_entry_safe(gdb_client_node, gdb_client_next, &gdb_transport_module.clients_list, list) {
		if (gdb_client_node->gdb_client == gdb_client) {
			slsi_dlist_del(&gdb_client_node->list);
			kmm_free(gdb_client_node);
		}
	}
}
