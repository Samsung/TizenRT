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
#include <debug.h>
#include <queue.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <tinyara/preference.h>

struct key_cb_list_s {
	struct key_cb_list_s *flink;
	char *key;
	int type;
	sq_queue_t cb_list; // node type : key_cb_node_t
};
typedef struct key_cb_list_s key_cb_list_t;

struct key_cb_node_s {
	struct key_cb_node_s *flink;
	int pid;
	preference_changed_cb cb_func;
	void *cb_data;
};
typedef struct key_cb_node_s key_cb_node_t;

static sq_queue_t g_key_cb_list; // node type : key_cb_list_t

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static key_cb_list_t *preference_get_key_cb_list(int type, const char *key)
{
	key_cb_list_t *ptr;

	ptr = (key_cb_list_t *)sq_peek(&g_key_cb_list);
	while (ptr != NULL) {
		if (ptr->type == type && !strncmp(ptr->key, key, strlen(ptr->key) + 1)) {
			return ptr;
		}
		ptr = (key_cb_list_t *)sq_next(ptr);
	}

	return ptr;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_SIGNAL)
void preference_send_cb_msg(int type, const char *key)
{
	int ret;
	mqd_t send_mq;
	struct mq_attr attr;
	char q_name[PREFERENCE_CBMQ_LEN];
	key_cb_node_t *node;
	key_cb_list_t *list;
	preference_callback_t data;

	attr.mq_msgsize = sizeof(preference_callback_t);
	attr.mq_maxmsg = PREFERNENCE_CBMSG_MAX;
	attr.mq_flags = 0;

	list = preference_get_key_cb_list(type, key);
	if (list != NULL) {
		node = (key_cb_node_t *)sq_peek(&list->cb_list);
		while (node) {
			/* Create message queue to receive notification messages */
			data.key = key;
			data.cb_func = node->cb_func;
			data.cb_data = node->cb_data;
			snprintf(q_name, PREFERENCE_CBMQ_LEN, "%s%d", PREFERENCE_CBMSG_MQ, node->pid);
			send_mq = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
			if (send_mq == (mqd_t)ERROR) {
				prefdbg("Failed to open mq '%s', errno %d\n", q_name, errno);
				return;
			}
			
			/* Send callback message */
			ret = mq_send(send_mq, (char *)&data, sizeof(preference_callback_t), 50);
			mq_close(send_mq);
			if (ret == OK) {
				ret = kill(node->pid, SIG_PREFERENCE);
				if (ret != OK) {
					prefdbg("Failed to send signal, pid %d errno %d\n", node->pid, errno);
					goto errout_with_unlink;
				}
			} else {
				bmdbg("Failed to send mq %s\n", q_name);
				goto errout_with_unlink;
			}
			node = (key_cb_node_t *)sq_next(node);
		}
	}

errout_with_unlink:
	mq_unlink(q_name);
}
#endif

int preference_register_callback(preference_callback_t *data)
{
	int ret;
	bool result;
	key_cb_list_t *list;
	key_cb_node_t *node;

	if (data == NULL) {
		return PREFERENCE_INVALID_PARAMETER;
	}

	ret = preference_check_key(data->type, data->key, &result);
	if (ret < 0) {
		prefdbg("Failed to check key, ret %d\n", ret);
		return ret;
	} else if (result == false) {
		prefdbg("Key %s is not existing\n", data->key);
		return PREFERENCE_KEY_NOT_EXIST;
	}

	list = preference_get_key_cb_list(data->type, data->key);
	if (list == NULL) {
		/* Make a new callback list of key */
		list = (key_cb_list_t *)PREFERENCE_ALLOC(sizeof(key_cb_list_t));
		if (list == NULL) {
			return PREFERENCE_OUT_OF_MEMORY;
		}
		sq_init(&list->cb_list);
		list->flink = NULL;
		list->key = strdup(data->key);
		list->type = data->type;
		sq_addlast((FAR sq_entry_t *)list, &g_key_cb_list);
	}

	node = (key_cb_node_t *)PREFERENCE_ALLOC(sizeof(key_cb_node_t));
	if (node == NULL) {
		prefdbg("Failed to allocate cb node\n");
		return PREFERENCE_OUT_OF_MEMORY;
	}
	node->pid = getpid();
	node->cb_func = data->cb_func;
	node->cb_data = data->cb_data;

	sq_addlast((FAR sq_entry_t *)node, &list->cb_list);

	return OK;
}

int preference_unregister_callback(const char *key, int type)
{
	int ret;
	int pid;
	bool result;
	key_cb_list_t *list;
	key_cb_node_t *node;

	ret = preference_check_key(type, key, &result);
	if (ret < 0) {
		prefdbg("Failed to check key, ret %d\n", ret);
		return ret;
	} else if (result == false) {
		prefdbg("Key %s is not existing\n", key);
		return PREFERENCE_KEY_NOT_EXIST;
	}

	list = preference_get_key_cb_list(type, key);
	if (list == NULL) {
		prefdbg("Callback is not registered for key %s\n", key);
		return PREFERENCE_NOT_REGISTERED;
	}

	/* Search the registered callback in key callback list */
	pid = getpid();
	node = (key_cb_node_t *)sq_peek(&list->cb_list);
	while (node != NULL) {
		if (node->pid == pid) {
			sq_rem((FAR sq_entry_t *)node, &list->cb_list);
			PREFERENCE_FREE(node);
			break;
		}
		node = (key_cb_node_t *)sq_next(node);
	}

	/* Free if a list of callback is empty */
	if (sq_empty(&list->cb_list)) {
		sq_rem((FAR sq_entry_t *)list, &g_key_cb_list);
		PREFERENCE_FREE(list->key);
		PREFERENCE_FREE(list);
	}

	return OK;
}

void preference_clear_callbacks(pid_t pid)
{
	key_cb_list_t *list;
	key_cb_node_t *node;

	list = (key_cb_list_t *)sq_peek(&g_key_cb_list);

	while (list) {
		node = (key_cb_node_t *)sq_peek(&list->cb_list);
		while (node) {
			if (node->pid == pid) {
				sq_rem((FAR sq_entry_t *)node, &list->cb_list);
				/* Free if a list of callback is empty */
				if (sq_empty(&list->cb_list)) {
					sq_rem((FAR sq_entry_t *)list, &g_key_cb_list);
					PREFERENCE_FREE(list->key);
					PREFERENCE_FREE(list);
					list = NULL;
				}
				PREFERENCE_FREE(node);
				break;
			}
			node = (key_cb_node_t *)sq_next(node);
		}
		if (list == NULL) {
			list = (key_cb_list_t *)sq_peek(&g_key_cb_list);
		} else {
			list = (key_cb_list_t *)sq_next(list);
		}
	}
}
