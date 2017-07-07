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

#include "platform_mif_module.h"
#include "platform_mif.h"
#include <errno.h>
#include <tinyara/kmalloc.h>

/* Implements */
#include "scsc_mif_abs.h"
#include "utils_misc.h"

/* Variables */
struct mif_abs_node {
	struct slsi_dlist_head list;
	struct scsc_mif_abs *mif_abs;
};

struct mif_driver_node {
	struct slsi_dlist_head list;
	struct scsc_mif_abs_driver *driver;
};

struct mif_mmap_node {
	struct slsi_dlist_head list;
	struct scsc_mif_mmap_driver *driver;
};

static struct platform_mif_module {
	struct slsi_dlist_head mif_abs_list;
	struct slsi_dlist_head mif_driver_list;
	struct slsi_dlist_head mif_mmap_list;
} mif_module = {
	.mif_abs_list = SLSI_DLIST_HEAD_INIT(mif_module.mif_abs_list), .mif_driver_list = SLSI_DLIST_HEAD_INIT(mif_module.mif_driver_list), .mif_mmap_list = SLSI_DLIST_HEAD_INIT(mif_module.mif_mmap_list),
};

/* Private Functions */

static void platform_mif_module_probe_registered_clients(struct scsc_mif_abs *mif_abs)
{
	struct mif_driver_node *mif_driver_node, *next;
	bool driver_registered = false;

	/* Traverse Linked List for each mif_driver node */
	slsi_dlist_for_each_entry_safe(mif_driver_node, next, &mif_module.mif_driver_list, list) {
		mif_driver_node->driver->probe(mif_driver_node->driver, mif_abs);
		driver_registered = true;
	}
}

//static int platform_mif_module_probe(struct platform_device *pdev)
int platform_mif_module_probe(void)
{
	struct mif_abs_node *mif_node;
	struct scsc_mif_abs *mif_abs;

	/* ADD EARLY BOARD INITIALIZATIONS IF REQUIRED */
	/* platform_mif_init(); */

	mif_node = kmm_zalloc(sizeof(*mif_node));
	if (!mif_node) {
		return -ENODEV;
	}

	mif_abs = platform_mif_create();
	if (!mif_abs) {
		pr_err("%s: Error creating platform interface\n", __func__);
		kmm_free(mif_node);
		return -ENODEV;
	}
	/* Add node */
	mif_node->mif_abs = mif_abs;

	slsi_dlist_add_tail(&mif_node->list, &mif_module.mif_abs_list);
	platform_mif_module_probe_registered_clients(mif_abs);

	return 0;
}

static int platform_mif_module_remove(/*struct platform_device *pdev */ void)
{
	return 0;
}

/* Public Functions */
void scsc_mif_abs_register(struct scsc_mif_abs_driver *driver)
{
	struct mif_driver_node *mif_driver_node;
	struct mif_abs_node *mif_node;

	/* Add node in driver linked list */
	mif_driver_node = kmm_zalloc(sizeof(*mif_driver_node));
	if (!mif_driver_node) {
		return;
	}

	mif_driver_node->driver = driver;
	slsi_dlist_add_tail(&mif_driver_node->list, &mif_module.mif_driver_list);

	/* Traverse Linked List for each mif_abs node */
	slsi_dlist_for_each_entry(mif_node, &mif_module.mif_abs_list, list) {
		driver->probe(driver, mif_node->mif_abs);
	}
}

void scsc_mif_abs_unregister(struct scsc_mif_abs_driver *driver)
{
	struct mif_driver_node *mif_driver_node, *next;

	/* Traverse Linked List for each mif_driver node */
	slsi_dlist_for_each_entry_safe(mif_driver_node, next, &mif_module.mif_driver_list, list) {
		if (mif_driver_node->driver == driver) {
			slsi_dlist_del(&mif_driver_node->list);
			kmm_free(mif_driver_node);
		}
	}
}

/* Register a mmap - debug driver - for this specific transport*/
void scsc_mif_mmap_register(struct scsc_mif_mmap_driver *mmap_driver)
{
	struct mif_mmap_node *mif_mmap_node;
	struct mif_abs_node *mif_node;

	/* Add node in driver linked list */
	mif_mmap_node = kmm_zalloc(sizeof(*mif_mmap_node));
	if (!mif_mmap_node) {
		return;
	}

	mif_mmap_node->driver = mmap_driver;
	slsi_dlist_add_tail(&mif_mmap_node->list, &mif_module.mif_mmap_list);

	/* Traverse Linked List for each mif_abs node */
	slsi_dlist_for_each_entry(mif_node, &mif_module.mif_abs_list, list) {
		mmap_driver->probe(mmap_driver, mif_node->mif_abs);
	}
}

/* Unregister a mmap - debug driver - for this specific transport*/
void scsc_mif_mmap_unregister(struct scsc_mif_mmap_driver *mmap_driver)
{
	struct mif_mmap_node *mif_mmap_node, *next;

	/* Traverse Linked List for each mif_driver node */
	slsi_dlist_for_each_entry_safe(mif_mmap_node, next, &mif_module.mif_mmap_list, list) {
		if (mif_mmap_node->driver == mmap_driver) {
			slsi_dlist_del(&mif_mmap_node->list);
			kmm_free(mif_mmap_node);
		}
	}
}
