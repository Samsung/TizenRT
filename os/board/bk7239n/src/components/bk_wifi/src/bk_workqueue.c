// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bk_list.h"
#include "bk_workqueue.h"
#include "os/mem.h"


static LIST_HEAD_DEFINE(g_workqueue);
static struct bk_workqueue *g_wq_default;

static void bk_worker_task(void *arg)
{
	struct bk_work *work = NULL;
	struct bk_workqueue *queue = (struct bk_workqueue *)arg;
	GLOBAL_INT_DECLARATION();


	while (1) {
		/* wait for work */
		rtos_get_semaphore(&queue->sem, BEKEN_WAIT_FOREVER);

		/* have work to do. */
		GLOBAL_INT_DISABLE();
		work = list_entry(queue->work_list.next, struct bk_work, work_node);
		list_del(&work->work_node);
		queue->work_current = work;
		work->exist = false;
		GLOBAL_INT_RESTORE();

		/* do work */
		work->handle(work->arg);

		/* clean current work */
		GLOBAL_INT_DISABLE();
		queue->work_current = NULL;
		GLOBAL_INT_RESTORE();
	}
}

bk_err_t _bk_workqueue_create(struct bk_workqueue *workqueue, const char *name,
				uint8_t priority, size_t stack_size)
{
	bk_err_t ret;
	GLOBAL_INT_DECLARATION();

	INIT_LIST_HEAD(&workqueue->node);
	INIT_LIST_HEAD(&workqueue->work_list);
	workqueue->work_current = NULL;
	workqueue->name = name;

	ret = rtos_init_semaphore(&workqueue->sem, 10);
	if (ret != BK_OK)
		return ret;

	GLOBAL_INT_DISABLE();
	list_add_tail(&workqueue->node, &g_workqueue);
	GLOBAL_INT_RESTORE();

	ret = rtos_create_thread(&workqueue->worker, priority, name,
			bk_worker_task, stack_size, (void *)workqueue);

	if (ret != BK_OK) {
		GLOBAL_INT_DISABLE();
		list_del(&workqueue->node);
		GLOBAL_INT_RESTORE();
		rtos_deinit_semaphore(&(workqueue->sem));
		return ret;
	}

	return BK_OK;
}

struct bk_workqueue *bk_workqueue_create(const char *name, uint8_t priority, size_t stack_size)
{
	struct bk_workqueue *wq;
	bk_err_t ret;

	wq = os_zalloc(sizeof(struct bk_workqueue));
	if (!wq)
		return NULL;

	ret = _bk_workqueue_create(wq, name, priority, stack_size);
	if (ret != BK_OK) {
		os_free(wq);
		return NULL;
	}

	return wq;
}

bk_err_t bk_work_init(struct bk_work *work, bk_work_handle_t handle, void *arg)
{
	if (!work || !handle)
		return BK_ERR_NULL_PARAM;

	os_memset(work, 0, sizeof(struct bk_work));
	INIT_LIST_HEAD(&(work->work_node));
	work->handle = handle;
	work->arg = arg;

	return BK_OK;
}

bk_err_t bk_work_run(struct bk_workqueue *workqueue, struct bk_work *work)
{
	GLOBAL_INT_DECLARATION();
	bk_err_t ret;

	if (!workqueue)
		return BK_ERR_NULL_PARAM;

	GLOBAL_INT_DISABLE();

	if (workqueue->work_current == work) {
		GLOBAL_INT_RESTORE();
		return BK_WORKQUEUE_WORK_RUNNING;
	}

	if (work->exist) {
		GLOBAL_INT_RESTORE();
		return BK_WORKQUEUE_WORK_EXIST;
	}

	/* NOTE: the work MUST be initialized firstly */
	list_del(&work->work_node);
	list_add_tail(&work->work_node, &workqueue->work_list);

	work->wq = workqueue;
	work->exist = true;

	GLOBAL_INT_RESTORE();
	ret = rtos_set_semaphore(&workqueue->sem);
	if (ret != BK_OK)
		return ret;

	return BK_OK;
}

bk_err_t bk_work_sched(struct bk_work *work)
{
	return bk_work_run(g_wq_default, work);
}

bk_err_t bk_work_cancel(struct bk_work *work)
{
	struct bk_workqueue *wq;
	GLOBAL_INT_DECLARATION();

	if (!work)
		return BK_ERR_NULL_PARAM;

	wq = work->wq;
	if (!wq)
		return BK_OK;

	GLOBAL_INT_DISABLE();
	if (wq->work_current == work) {
		GLOBAL_INT_RESTORE();
		return BK_WORKQUEUE_WORK_RUNNING;
	}

	if (work->exist) {
		GLOBAL_INT_RESTORE();
		return BK_WORKQUEUE_WORK_EXIST;
	}

	list_del(&work->work_node);
	work->wq = NULL;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

void bk_workqueue_init(void)
{
	if (!g_wq_default)
		g_wq_default = bk_workqueue_create("syswq", CONFIG_WIFI_KMSG_TASK_PRIO, 1024);
}

