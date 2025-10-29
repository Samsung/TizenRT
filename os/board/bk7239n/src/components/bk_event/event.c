#include <common/bk_include.h>
#include <components/event.h>
#include "event.h"
#include <components/log.h>
#include "bk_list.h"
#include <os/mem.h>
#include <modules/wifi_types.h>

static beken_queue_t s_event_queue = NULL;
static beken_thread_t s_event_task = NULL;
static struct list_head s_event_module_list;
static bool s_event_inited = false;

static bool event_is_inited(void)
{
	return s_event_inited;
}

static bool event_is_invalid(event_module_t event_module, int event_id)
{
	if ((event_module >= EVENT_MOD_COUNT) || (event_module < 0))
		return true;

	if ((event_id != EVENT_ID_ALL) && (event_id < 0))
		return true;

	return false;
}

static int register_cb_to_cb_list(struct list_head *cb_list, event_register_info_t *reg_info)
{
	struct list_head *pos, *next;
	event_cb_node_t *cb_node;
	event_cb_node_t *new_cb_node = NULL;

	EVENT_LOGD("register cb to cb list(%p)\n", cb_list);

	list_for_each_safe(pos, next, cb_list) {
		cb_node = list_entry(pos, event_cb_node_t, next);
		if (cb_node->event_cb == reg_info->event_cb) {
			EVENT_LOGD("event <%d %d> cb exist\n", reg_info->event_module_id,
					   reg_info->event_id);
			return BK_ERR_EVENT_CB_EXIST;
		}
	}

	new_cb_node = (event_cb_node_t *) os_zalloc(sizeof(event_cb_node_t));
	if (!new_cb_node) {
		EVENT_LOGE("failed to alloc new cb node\n");
		return BK_ERR_NO_MEM;
	}

	EVENT_LOGD("new cb node(%p)\n", new_cb_node);
	new_cb_node->event_cb = reg_info->event_cb;
	new_cb_node->event_cb_arg = reg_info->event_cb_arg;
	list_add_tail(&new_cb_node->next, cb_list);

	return BK_OK;
}

static int register_cb_to_event_node_list(struct list_head *event_node_list,
		event_register_info_t *reg_info)
{
	struct list_head *pos, *next;
	event_node_t *event_node = NULL;
	event_node_t *new_event_node = NULL;
	int ret;

	EVENT_LOGD("register cb to event node(%p)\n", event_node_list);
	list_for_each_safe(pos, next, event_node_list) {
		event_node = list_entry(pos, event_node_t, next);
		if (event_node->event_id == reg_info->event_id) {
			return register_cb_to_cb_list(&event_node->cb_list,
										  reg_info);
		}
	}

	new_event_node = (event_node_t *)os_zalloc(sizeof(event_node_t));
	if (!new_event_node) {
		EVENT_LOGE("failed to alloc event node\n");
		return BK_ERR_NO_MEM;
	}

	EVENT_LOGD("new event node(%p)\n", new_event_node);
	INIT_LIST_HEAD(&new_event_node->next);
	INIT_LIST_HEAD(&new_event_node->cb_list);
	new_event_node->event_id = reg_info->event_id;

	ret = register_cb_to_cb_list(&new_event_node->cb_list, reg_info);
	if (ret != BK_OK) {
		os_free(new_event_node);
		return ret;
	}

	list_add_tail(&new_event_node->next, event_node_list);
	return BK_OK;
}

static int register_cb_to_module_node(event_module_node_t *module_node,
									  event_register_info_t *reg_info)
{
	EVENT_LOGD("register cb to module node(%p)\n", module_node);
	if (reg_info->event_id == EVENT_ID_ALL)
		return register_cb_to_cb_list(&module_node->cb_list, reg_info);
	else
		return register_cb_to_event_node_list(&module_node->event_node_list, reg_info);
}

static int register_cb(event_register_info_t *reg_info)
{
	event_module_node_t *new_module_node = NULL;
	event_module_node_t *module_node = NULL;
	struct list_head *pos, *next;
	int ret = 0;

	EVENT_LOGD("handle event register, event<%d %d %p %p>\n",
			   reg_info->event_module_id, reg_info->event_id, reg_info->event_cb,
			   reg_info->event_cb_arg);

	list_for_each_safe(pos, next, &s_event_module_list) {
		module_node = list_entry(pos, event_module_node_t, next);
		if (reg_info->event_module_id == module_node->event_module_id)
			return register_cb_to_module_node(module_node, reg_info);
	}

	new_module_node = (event_module_node_t *)os_zalloc(sizeof(event_module_node_t));
	if (!new_module_node)
		return BK_ERR_NO_MEM;

	EVENT_LOGD("new event module=%p\n", new_module_node);
	new_module_node->event_module_id = reg_info->event_module_id;
	INIT_LIST_HEAD(&new_module_node->next);
	INIT_LIST_HEAD(&new_module_node->event_node_list);
	INIT_LIST_HEAD(&new_module_node->cb_list);
	ret = register_cb_to_module_node(new_module_node, reg_info);
	if (ret != BK_OK) {
		os_free(new_module_node);
		return ret;
	}

	list_add_tail(&new_module_node->next, &s_event_module_list);
	return BK_OK;
}

static int unregister_cb_from_cb_list(struct list_head *cb_list,
									  event_register_info_t *unreg_info)
{
	struct list_head *pos, *next;
	event_cb_node_t *cb_node;

	EVENT_LOGD("unregister cb from cb list(%p)\n", cb_list);

	list_for_each_safe(pos, next, cb_list) {
		cb_node = list_entry(pos, event_cb_node_t, next);
		if (cb_node->event_cb == unreg_info->event_cb) {
			EVENT_LOGD("free cb_node(%p)\n", cb_node);
			list_del(pos);
			os_free(cb_node);
			return BK_OK;
		}
	}

	EVENT_LOGD("event <%d %d %p> doesn't exist\n", unreg_info->event_module_id,
			   unreg_info->event_id, unreg_info->event_cb);
	return BK_ERR_EVENT_NO_CB;
}

static int unregister_cb_from_event_node_list(struct list_head *event_node_list,
		event_register_info_t *unreg_info)
{
	struct list_head *pos, *next;
	event_node_t *event_node = NULL;
	int ret;

	EVENT_LOGD("unregister cb from event node(%p)\n", event_node_list);
	list_for_each_safe(pos, next, event_node_list) {
		event_node = list_entry(pos, event_node_t, next);
		if (event_node->event_id == unreg_info->event_id) {
			ret = unregister_cb_from_cb_list(&event_node->cb_list,
											 unreg_info);

			if (list_empty(&event_node->cb_list)) {
				EVENT_LOGD("free event node(%p)\n", event_node);
				list_del(pos);
				os_free(event_node);
			}

			return ret;
		}
	}

	EVENT_LOGD("event <%d %d %p> doesn't exist\n", unreg_info->event_module_id,
			   unreg_info->event_id, unreg_info->event_cb);
	return BK_ERR_EVENT_NO_CB;
}

static int unregister_cb_from_module_node(event_module_node_t *module_node,
		event_register_info_t *unreg_info)
{
	EVENT_LOGD("unregister cb from module node(%p)\n", module_node);
	if (unreg_info->event_id == EVENT_ID_ALL)
		return unregister_cb_from_cb_list(&module_node->cb_list, unreg_info);
	else
		return unregister_cb_from_event_node_list(&module_node->event_node_list, unreg_info);

	return BK_ERR_EVENT_NO_CB;
}

static int unregister_cb(event_register_info_t *unreg_info)
{
	event_module_node_t *module_node = NULL;
	struct list_head *pos, *next;
	int ret;

	EVENT_LOGD("handle event unregister, event<%d %d %p>\n",
			   unreg_info->event_module_id, unreg_info->event_id, unreg_info->event_cb);

	list_for_each_safe(pos, next, &s_event_module_list) {
		module_node = list_entry(pos, event_module_node_t, next);
		if (unreg_info->event_module_id == module_node->event_module_id) {
			ret = unregister_cb_from_module_node(module_node, unreg_info);
			if (list_empty(&module_node->cb_list) &&
				list_empty(&module_node->event_node_list)) {
				EVENT_LOGD("free module node(%p)\n", module_node);
				list_del(pos);
				os_free(module_node);
				return ret;
			}

			return ret;
		}
	}

	EVENT_LOGD("event <%d %d %p> doesn't exist\n", unreg_info->event_module_id,
			   unreg_info->event_id, unreg_info->event_cb);
	return BK_ERR_EVENT_NO_CB;
}

static void event_task_init(void)
{
	INIT_LIST_HEAD(&s_event_module_list);
}

static void event_deinit_cb_list(struct list_head *cb_list)
{
	struct list_head *pos, *next;
	event_cb_node_t *cb_node;

	list_for_each_safe(pos, next, cb_list) {
		cb_node = list_entry(pos, event_cb_node_t, next);
		list_del(pos);
		os_free(cb_node);
		EVENT_LOGD("free cb(%p)\n", cb_node);
	}
}

static void event_deinit_module_node(event_module_node_t *event_module)
{
	struct list_head *pos, *next;
	event_node_t *event_node = NULL;

	event_deinit_cb_list(&event_module->cb_list);
	list_for_each_safe(pos, next, &event_module->event_node_list) {
		event_node = list_entry(pos, event_node_t, next);
		list_del(pos);
		event_deinit_cb_list(&event_node->cb_list);
		os_free(event_node);
		EVENT_LOGD("free event node(%p)\n", event_node);
	}
	os_free(event_module);
	EVENT_LOGD("free module(%p)\n", event_module);
}

static void event_deinit(void)
{
	event_module_node_t *module_node = NULL;
	struct list_head *pos, *next;

	EVENT_LOGD("event deinit\n");
	list_for_each_safe(pos, next, &s_event_module_list) {
		module_node = list_entry(pos, event_module_node_t, next);
		list_del(pos);
		event_deinit_module_node(module_node);
	}

	INIT_LIST_HEAD(&s_event_module_list);
}

static void event_task_deinit(void)
{
	EVENT_LOGI("event task deinit\n");

	event_deinit();
	if (s_event_queue) {
		rtos_deinit_queue(&s_event_queue);
		s_event_queue = NULL;
	}

	if (s_event_task) {
		rtos_delete_thread(&s_event_task);
		s_event_task = NULL;
	}
}

static int event_post_to_cb_list(struct list_head *cb_list, event_info_t *event_info)
{
	struct list_head *pos, *next;
	event_cb_node_t *cb_node;

	list_for_each_safe(pos, next, cb_list) {
		cb_node = list_entry(pos, event_cb_node_t, next);
		cb_node->event_cb(cb_node->event_cb_arg, event_info->event_module_id,
						  event_info->event_id, event_info->event_data);
	}

	return BK_OK;
}

static int event_post_to_module(event_module_node_t *module_node,
								event_info_t *event_info)
{
	struct list_head *pos, *next;
	event_node_t *event_node;

	event_post_to_cb_list(&module_node->cb_list, event_info);

	list_for_each_safe(pos, next, &module_node->event_node_list) {
		event_node = list_entry(pos, event_node_t, next);
		if (event_node->event_id == event_info->event_id) {
			event_post_to_cb_list(&event_node->cb_list, event_info);
			break;
		}
	}

	return BK_OK;
}

static int event_post(event_info_t *event_info)
{
	event_module_node_t *module_node;
	struct list_head *pos, *next;
	int ret;

	if (event_is_invalid(event_info->event_module_id, event_info->event_id))
		return BK_ERR_EVENT_MOD_OR_ID;

	list_for_each_safe(pos, next, &s_event_module_list) {
		module_node = list_entry(pos, event_module_node_t, next);
		if (module_node->event_module_id == event_info->event_module_id) {
			ret = event_post_to_module(module_node, event_info);
			if (event_info->event_data)
				os_free(event_info->event_data);
			return ret;
		}
	}

	if (event_info->event_data)
		os_free(event_info->event_data);

	EVENT_LOGW("event <%d %d> has no cb\n", event_info->event_module_id, event_info->event_id);
	return BK_OK;
}

#if EVENT_DEBUG
static void event_dump_cb_list(event_module_t module_id, int event_id, struct list_head *cb_list)
{
	struct list_head *pos, *next;
	event_cb_node_t *cb_node;

	list_for_each_safe(pos, next, cb_list) {
		cb_node = list_entry(pos, event_cb_node_t, next);
		BK_LOG_RAW("%9x   %8x   %-p    %-p\n", module_id, event_id,
				   cb_node->event_cb, cb_node->event_cb_arg);
	}
}

static void event_dump_module_node(event_module_node_t *module_node)
{
	struct list_head *pos, *next;
	event_node_t *event_node;

	event_dump_cb_list(module_node->event_module_id, EVENT_ID_ALL, &module_node->cb_list);
	list_for_each_safe(pos, next, &module_node->event_node_list) {
		event_node = list_entry(pos, event_node_t, next);
		event_dump_cb_list(module_node->event_module_id, event_node->event_id,
						   &event_node->cb_list);
	}
}

static int event_dump(void)
{
	struct list_head *pos, *next;
	event_module_node_t *module_node;

	BK_LOG_RAW("\nevent dump result:\n");
	BK_LOG_RAW("%9s   %8s   %8s   %8s\n", "module_id", "event_id", "cb", "arg");
	BK_LOG_RAW("---------   --------   --------   --------\n");
	list_for_each_safe(pos, next, &s_event_module_list) {
		module_node = list_entry(pos, event_module_node_t, next);
		event_dump_module_node(module_node);
	}
	return BK_OK;
}
#endif

static int event_task_handle_msg(event_msg_t *msg)
{
	if (!msg) {
		EVENT_LOGE("null event msg\n");
		return BK_ERR_EVENT_NULL_MSG;
	}

	switch (msg->msg_type) {
	case EVENT_MSG_REGISTER:
		return register_cb(&msg->msg.register_info);
		break;
	case EVENT_MSG_UNREGISTER:
		return unregister_cb(&msg->msg.register_info);
	case EVENT_MSG_POST:
		return event_post(&msg->msg.event_info);
#if EVENT_DEBUG
	case EVENT_MSG_DUMP:
		return event_dump();
#endif
	default:
		EVENT_LOGE("invalid event msg\n");
		return BK_ERR_EVENT_UNKNOWN_MSG;
	}

	return BK_OK;
}

static void event_task(beken_thread_arg_t arg)
{
	event_msg_t *msg = NULL;
	int event_ret;
	event_task_init();

	while (1) {
		rtos_pop_from_queue(&s_event_queue, &msg, BEKEN_WAIT_FOREVER);

		if (msg && msg->msg_type == EVENT_MSG_DEINIT)
			break;

		event_ret = event_task_handle_msg(msg);

		if (msg->is_sync_msg) {
			msg->sync_msg_ret = event_ret;
			rtos_set_semaphore(&msg->sync_msg_sem);
		} else {
			os_free(msg);
			msg = NULL;
		}
	}

	event_task_deinit();
}

static int event_send_msg_to_event_task(event_msg_t *pmsg, uint32_t timeout)
{
	int rtos_ret;
	bool is_sync_msg = pmsg->is_sync_msg;

	if (is_sync_msg) {
		rtos_ret = rtos_init_semaphore(&pmsg->sync_msg_sem, 1);
		if (rtos_ret != kNoErr)
			return BK_ERR_EVENT_INIT_SEM;
	}

	rtos_ret = rtos_push_to_queue(&s_event_queue, &pmsg, timeout);
	if (rtos_ret != kNoErr) {
		if (is_sync_msg)
			rtos_deinit_semaphore(&pmsg->sync_msg_sem);

		return BK_ERR_EVENT_POST_QUEUE;
	}

	if (is_sync_msg) {
		rtos_get_semaphore(&pmsg->sync_msg_sem, BEKEN_NEVER_TIMEOUT);
		rtos_deinit_semaphore(&pmsg->sync_msg_sem);
		return pmsg->sync_msg_ret;
	} else
		return BK_OK;
}

bk_err_t bk_event_init(void)
{
	int ret;

	//Not protect. We assume caller will NOT call bk_event_init/deinit concurrently.
	if (event_is_inited()) {
		EVENT_LOGD("event already init, ignore request");
		return BK_OK;
	}

	ret = rtos_init_queue(&s_event_queue, "eventq", sizeof(event_msg_t *), EVENT_QUEUE_SIZE);
	if (kNoErr != ret) {
		EVENT_LOGE("failed to create event queue\n");
		return BK_ERR_EVENT_CREATE_QUEUE;
	}

	ret = rtos_create_thread(&s_event_task, EVENT_TASK_PRIORITY, "event",
							 (beken_thread_function_t)event_task, EVENT_TASK_STACK_SIZE, 0);
	if (kNoErr != ret) {
		rtos_deinit_queue(&s_event_queue);
		s_event_queue = NULL;
		EVENT_LOGE("failed to create event task\n");
		return BK_ERR_EVENT_CREATE_TASK;
	}

	s_event_inited = true;
	EVENT_LOGD("inited\n");
	return BK_OK;
}

/* Currently we don't have scenario that need to deinit the event module,
 * so we haven't make bk_event_deinit() thread-safe yet. We can make it
 * thread-safe once we have real customer requirements about event deinit.
 */
bk_err_t bk_event_deinit(void)
{
    event_msg_t *pmsg;

    if (!event_is_inited())
        return BK_OK;

    s_event_inited = false;

    pmsg = os_zalloc(sizeof(event_msg_t));
    if (!pmsg)
        return BK_ERR_NO_MEM;

    pmsg->msg_type = EVENT_MSG_DEINIT;
    pmsg->is_sync_msg = false;

    if (BK_OK != event_send_msg_to_event_task(pmsg, BEKEN_WAIT_FOREVER)) {
        os_free(pmsg);
        return BK_FAIL;
    }

    return BK_OK;
}

bk_err_t bk_event_register_cb(event_module_t event_module_id, int event_id,
							  event_cb_t event_cb, void *event_cb_arg)
{
	event_msg_t msg = {0};
	event_msg_t *pmsg = &msg;

	if (!event_is_inited()) {
		EVENT_LOGE("event not init\n");
		return BK_ERR_EVENT_NOT_INIT;
	}

	if (event_is_invalid(event_module_id, event_id))
		return BK_ERR_EVENT_MOD_OR_ID;

	EVENT_LOGD("register event <%d %d %p %p>\n", event_module_id, event_id,
			event_cb, event_cb_arg);
	pmsg->msg_type = EVENT_MSG_REGISTER;
	pmsg->is_sync_msg = true;
	pmsg->sync_msg_ret = BK_OK;
	pmsg->msg.register_info.event_module_id = event_module_id;
	pmsg->msg.register_info.event_id = event_id;
	pmsg->msg.register_info.event_cb = event_cb;
	pmsg->msg.register_info.event_cb_arg = event_cb_arg;

	return event_send_msg_to_event_task(pmsg, BEKEN_WAIT_FOREVER);
}

bk_err_t bk_event_unregister_cb(event_module_t event_module_id, int event_id,
								event_cb_t event_cb)
{
	event_msg_t msg = {0};
	event_msg_t *pmsg = &msg;

	if (!event_is_inited()) {
		EVENT_LOGE("unregister fail, event not init\n");
		return BK_ERR_EVENT_NOT_INIT;
	}

	if (event_is_invalid(event_module_id, event_id))
		return BK_ERR_EVENT_MOD_OR_ID;

	pmsg->msg_type = EVENT_MSG_UNREGISTER;
	pmsg->msg.register_info.event_module_id = event_module_id;
	pmsg->msg.register_info.event_id = event_id;
	pmsg->msg.register_info.event_cb = event_cb;
	pmsg->is_sync_msg = true;
	pmsg->sync_msg_ret = BK_OK;

	EVENT_LOGD("unregister event <%d, %d, %p>\n", event_module_id, event_id, event_cb);
	return event_send_msg_to_event_task(pmsg, BEKEN_WAIT_FOREVER);
}

bk_err_t bk_event_post(event_module_t event_module_id, int event_id,
					   void *event_data, size_t event_data_size, uint32_t timeout)
{
	void *event_data_copy = NULL;
	event_msg_t *pmsg;
	bk_err_t ret = BK_OK;

	if (!event_is_inited())
		return BK_ERR_EVENT_NOT_INIT;

	if (event_is_invalid(event_module_id, event_id))
		return BK_ERR_EVENT_MOD_OR_ID;

	pmsg = os_zalloc(sizeof(event_msg_t));
	if (!pmsg)
		return BK_ERR_NO_MEM;

	if (event_data_size > 0) {
		event_data_copy = os_zalloc(event_data_size);
		if (!event_data_copy) {
			os_free(pmsg);
			return BK_ERR_NO_MEM;
		}

		os_memcpy(event_data_copy, event_data, event_data_size);
	}

	pmsg->msg_type = EVENT_MSG_POST;
	pmsg->msg.event_info.event_module_id = event_module_id;
	pmsg->msg.event_info.event_id = event_id;
	pmsg->msg.event_info.event_data = event_data_copy;
	pmsg->is_sync_msg = false;

	EVENT_LOGD("post event <%d %d %p %u>\n", event_module_id, event_id, event_data, timeout);
	ret = event_send_msg_to_event_task(pmsg, timeout);
	if (BK_OK != ret) {
		os_free(event_data_copy);
		os_free(pmsg);
	}

	return ret;
}

bk_err_t bk_event_dump(void)
{
#if EVENT_DEBUG
    event_msg_t *pmsg;

    if (!event_is_inited())
        return BK_OK;

    pmsg = os_zalloc(sizeof(event_msg_t));
    if (!pmsg)
        return BK_ERR_NO_MEM;

    pmsg->msg_type = EVENT_MSG_DUMP;
    pmsg->is_sync_msg = false;
    EVENT_LOGD("dump event\n");

    if (BK_OK != event_send_msg_to_event_task(pmsg, 0)) {
         os_free(pmsg);
         return BK_FAIL;
    }
#endif
	return BK_OK;
}
