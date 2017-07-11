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
#include <errno.h>
#include <tinyara/kmalloc.h>
#include "scsc_mif_abs.h"
#include "scsc_mx_impl.h"
#include "utils_misc.h"
#include "debug_scsc.h"

#define SCSC_MX_CORE_MODDESC "mx140 Core Driver"

struct clients_node {
	struct slsi_dlist_head list;
	struct scsc_mx_module_client *module_client;
};

struct mx_node {
	struct slsi_dlist_head list;
	struct scsc_mx *mx;
};

static struct mx_module {
	struct slsi_dlist_head clients_list;
	struct slsi_dlist_head mx_list;
} mx_module = {
	.clients_list = SLSI_DLIST_HEAD_INIT(mx_module.clients_list), .mx_list = SLSI_DLIST_HEAD_INIT(mx_module.mx_list)
};

static void scsc_mx_module_probe_registered_clients(struct scsc_mx *new_mx)
{
	bool client_registered = false;
	struct clients_node *client_node, *client_next;

	/* Traverse Linked List for each mif_driver node */
	slsi_dlist_for_each_entry_safe(client_node, client_next, &mx_module.clients_list, list) {
		client_node->module_client->probe(client_node->module_client, new_mx, SCSC_MODULE_CLIENT_REASON_HW_PROBE);
		client_registered = true;
	}
	if (client_registered == false) {
		SLSI_INFO_NODEV("No clients registered\n");
	}
}

static void scsc_mx_module_probe(struct scsc_mif_abs_driver *abs_driver, struct scsc_mif_abs *mif_abs)
{
	struct scsc_mx *new_mx;
	struct mx_node *mx_node;

	/* Avoid unused parm error */
	(void)abs_driver;

	mx_node = kmm_zalloc(sizeof(*mx_node));
	if (!mx_node) {
		return;
	}

	/* Create new mx instance */
	new_mx = scsc_mx_create(mif_abs);
	if (!new_mx) {
		kmm_free(mx_node);
		pr_err("%s: Error allocating new_mx\n", __func__);
		return;
	}
	/* Add instance in mx_node linked list */
	mx_node->mx = new_mx;

	slsi_dlist_add_tail(&mx_node->list, &mx_module.mx_list);

	scsc_mx_module_probe_registered_clients(new_mx);
}

static void scsc_mx_module_remove(struct scsc_mif_abs *abs)
{
	bool match = false;
	struct mx_node *mx_node, *next;

	/* Traverse Linked List for each mx node */
	slsi_dlist_for_each_entry_safe(mx_node, next, &mx_module.mx_list, list) {
		/* If there is a match, call destroy  */
		if (scsc_mx_get_mif_abs(mx_node->mx) == abs) {
			match = true;
			scsc_mx_destroy(mx_node->mx);
			slsi_dlist_del(&mx_node->list);
			kmm_free(mx_node);
		}
	}
	if (match == false) {
		pr_err("%s: FATAL, no match for given scsc_mif_abs\n", __func__);
	}
}

static struct scsc_mif_abs_driver mx_module_mif_if = {
	.name = "mx140 driver",
	.probe = scsc_mx_module_probe,
	.remove = scsc_mx_module_remove,
};

int scsc_mx_module_init(void)
{
	SLSI_INFO_NODEV(SCSC_MX_CORE_MODDESC " loaded\n");

	scsc_mif_abs_register(&mx_module_mif_if);
	return 0;
}

static void scsc_mx_module_exit(void)
{
	struct mx_node *mx_node, *next_mx;

	/* Traverse Linked List for each mx node */
	slsi_dlist_for_each_entry_safe(mx_node, next_mx, &mx_module.mx_list, list) {
		scsc_mx_destroy(mx_node->mx);
		slsi_dlist_del(&mx_node->list);
		kmm_free(mx_node);
	}

	scsc_mif_abs_unregister(&mx_module_mif_if);
	SLSI_INFO_NODEV(SCSC_MX_CORE_MODDESC " unloaded\n");
}

/**
 * Reset all registered service drivers by first calling the remove callback and
 * then the probe callback. This function is used during recovery operations,
 * where the chip has been reset as part of the recovery and the service drivers
 * has to do the same.
 */
int scsc_mx_module_reset(void)
{
	struct clients_node *clients_node;
	struct mx_node *mx_node, *next_mx;

	/* Traverse Linked List and call registered removed callbacks */
	slsi_dlist_for_each_entry_safe(mx_node, next_mx, &mx_module.mx_list, list)
	slsi_dlist_for_each_entry(clients_node, &mx_module.clients_list, list)
	clients_node->module_client->remove(clients_node->module_client, mx_node->mx, SCSC_MODULE_CLIENT_REASON_RECOVERY);

	/* Traverse Linked List and call registered probed callbacks */
	slsi_dlist_for_each_entry_safe(mx_node, next_mx, &mx_module.mx_list, list)
	slsi_dlist_for_each_entry(clients_node, &mx_module.clients_list, list)
	clients_node->module_client->probe(clients_node->module_client, mx_node->mx, SCSC_MODULE_CLIENT_REASON_RECOVERY);

	return 0;
}

int scsc_mx_module_register_client_module(struct scsc_mx_module_client *module_client)
{
	struct clients_node *module_client_node;
	struct mx_node *mx_node;

	/* Add node in modules linked list */
	module_client_node = kmm_zalloc(sizeof(*module_client_node));
	if (!module_client_node) {
		return -ENOMEM;
	}

	module_client_node->module_client = module_client;
	slsi_dlist_add_tail(&module_client_node->list, &mx_module.clients_list);

	/* Traverse Linked List for each mx node */
	slsi_dlist_for_each_entry(mx_node, &mx_module.mx_list, list) {
		module_client->probe(module_client, mx_node->mx, SCSC_MODULE_CLIENT_REASON_HW_PROBE);
	}
	return 0;
}

void scsc_mx_module_unregister_client_module(struct scsc_mx_module_client *module_client)
{
	struct clients_node *client_node, *client_next;
	struct mx_node *mx_node, *next_mx;

	/* Traverse Linked List for each client_list  */
	slsi_dlist_for_each_entry_safe(client_node, client_next, &mx_module.clients_list, list) {
		if (client_node->module_client == module_client) {
			slsi_dlist_for_each_entry_safe(mx_node, next_mx, &mx_module.mx_list, list) {
				module_client->remove(module_client, mx_node->mx, SCSC_MODULE_CLIENT_REASON_RECOVERY);
			}
			slsi_dlist_del(&client_node->list);
			kmm_free(client_node);
		}
	}
}
