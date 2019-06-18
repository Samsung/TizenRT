/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <stdlib.h>
#include <queue.h>
#include <errno.h>
#include <mqueue.h>
#ifdef CONFIG_FS_NAMED_SEMAPHORES
#include <semaphore.h>
#endif
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0
#include <unistd.h>
#include <sys/mount.h>
#endif
#include <sys/types.h>

#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/init.h>

#include "sched/sched.h"
#include "binary_manager.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define RES_PREPATH_LEN  32
#define RES_NAME_LEN     (RES_PREPATH_LEN + CONFIG_NAME_MAX)

struct resource_node_s {
	struct resource_node_s *flink;
	int type;
	char name[RES_NAME_LEN];
};
typedef struct resource_node_s resource_node_t;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_clear_bin_inode
 *
 * Description:
 *   This function will clear all resource nodes registered in the binary.
 *
 * Input parameters:
 *   bin_idx   -   The index of the binary in binary table
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_clear_bin_resource(int bin_idx)
{
	int ret;
	resource_node_t *resource_node;
	resource_node_t *node_ptr;

	node_ptr = (resource_node_t *)sq_peek(&BIN_RESOURCELIST(bin_idx));
	while (node_ptr != NULL) {
		sq_rem((FAR sq_entry_t *)node_ptr, &BIN_RESOURCELIST(bin_idx));
		resource_node = node_ptr;
		switch (resource_node->type) {
		case BIN_RESOURCE_MQ:
			ret = mq_unlink(resource_node->name);
			bmvdbg("Unlink MQ %s, ret %d\n", resource_node->name, ret);
			break;
#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0
		case BIN_RESOURCE_FILE:
			ret = unlink(resource_node->name);
			bmvdbg("Unlink FILE %s, ret %d %d\n", resource_node->name, ret, errno);
			break;
		case BIN_RESOURCE_MOUNTPT:
			if (!sq_empty(&BIN_RESOURCELIST(bin_idx))) {
				/* mountpoint should be unmounted last */
				sq_addlast((FAR sq_entry_t *)resource_node, &BIN_RESOURCELIST(bin_idx));
				node_ptr = (resource_node_t *)sq_next(node_ptr);
				continue;
			}
			ret = umount(resource_node->name);
			bmvdbg("Umount MNT %s, ret %d\n", resource_node->name, ret);
			break;
#endif
#ifdef CONFIG_FS_NAMED_SEMAPHORES
		case BIN_RESOURCE_NAMEDSEM:
			ret = sem_unlink(resource_node->name);
			bmvdbg("Unlink NAMED SEM %s, ret %d\n", resource_node->name, ret);
			break;
#endif
		default:
			bmdbg("Invalid resource type %d, %s\n", resource_node->type, resource_node->name);
		}
		node_ptr = (resource_node_t *)sq_next(node_ptr);
		kmm_free(resource_node);
	}
}

/****************************************************************************
 * Name: binary_manager_register_resource
 *
 * Description:
 *   This function registers resource info to a resource list of binary.
 *
 * Input parameters:
 *   type   -   The type of the resource
 *              It will be one of binmgr_resource_type defined in <tinyara/binary_manager.h>
 *   name   -   The name of the resource
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_register_resource(int type, const char *name)
{
	int bin_idx;
	struct tcb_s *tcb;
	resource_node_t *resource_node;

	tcb = sched_self();
	if (tcb == NULL || tcb->group == NULL) {
		bmdbg("Failed to get tcb self\n");
		return;
	}

	/* Get binary id of fault task and check it is registered in binary manager */
	bin_idx = binary_manager_get_index_with_binid(tcb->group->tg_loadtask);
	if (bin_idx > 0) {
		resource_node = (resource_node_t *)kmm_malloc(sizeof(resource_node_t));
		if (resource_node == NULL) {
			bmdbg("Failed to allocate node for inode\n");
			return;
		}
		resource_node->type = type;
		strncpy(resource_node->name, name, CONFIG_NAME_MAX);
		sq_addlast((FAR sq_entry_t *)resource_node, &BIN_RESOURCELIST(bin_idx));
		bmvdbg("Inode '%s' is registered in '%s'\n", name, BIN_NAME(bin_idx));
	}
}

/****************************************************************************
 * Name: binary_manager_unregister_resource
 *
 * Description:
 *   This function unregisters resource info from a resource list of binary.
 *
 * Input parameters:
 *   type   -   The type of the resource
 *              It will be one of binmgr_resource_type defined in <tinyara/binary_manager.h>
 *   name   -   The name of the resource
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void binary_manager_unregister_resource(int type, const char *name)
{
	int count;
	int bin_idx;
	resource_node_t *resource_node;

	count = binary_manager_get_binary_count();

	for (bin_idx = 1; bin_idx < count + 1; bin_idx++) {
		if (!sq_empty(&BIN_RESOURCELIST(bin_idx))) {
			resource_node = (resource_node_t *)sq_peek(&BIN_RESOURCELIST(bin_idx));
			if (resource_node->type == type && !strncmp(resource_node->name, name, strlen(name) + 1)) {
				sq_rem((FAR sq_entry_t *)resource_node, &BIN_RESOURCELIST(bin_idx));
				bmvdbg("Inode '%s' is unregistered\n", name);
				kmm_free(resource_node);
				break;
			}
			resource_node = (resource_node_t *)sq_next(resource_node);
		}
	}
}
