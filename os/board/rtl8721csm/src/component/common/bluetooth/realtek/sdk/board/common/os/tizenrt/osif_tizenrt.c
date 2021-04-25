/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#include <fcntl.h>
#include <osif.h>
#include <osdep_service.h>
#include "../kernel/mqueue/mqueue.h"

/****************************************************************************/
/* Check if in task context (true), or isr context (false)                  */
/****************************************************************************/
static inline bool osif_task_context_check(void)
{
	return !up_interrupt_context();
}

/****************************************************************************/
/* Delay current task in a given milliseconds                               */
/****************************************************************************/
void osif_delay(uint32_t ms)
{
	usleep((unsigned int) ms * 1000);
}

/****************************************************************************/
/* Get system time in milliseconds                                          */
/****************************************************************************/
uint32_t osif_sys_time_get(void)
{
	uint32_t sys_time = clock();
	return (sys_time * 1000L / TICK_PER_SEC);
}

/****************************************************************************/
/* Start os kernel scheduler                                                */
/****************************************************************************/
bool osif_sched_start(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Stop os kernel scheduler                                                 */
/****************************************************************************/
bool osif_sched_stop(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Suspend os kernel scheduler                                              */
/****************************************************************************/
bool osif_sched_suspend(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Resume os kernel scheduler                                               */
/****************************************************************************/
bool osif_sched_resume(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

static int osif_wrapper_thread(int argc, char *argv[])
{
	void (*p_routine)(void *);
	void *p_param;

	if (argc != 3) {
		printf("[osif_task_create] %s osif_wrapper_thread error\r\n", argv[0]);
		return -1;
	}

	p_routine = (void *) strtoul(argv[1], NULL, 16);
	p_param = (void *) strtoul(argv[2], NULL, 16);

	p_routine(p_param);

	return 0;
}

/****************************************************************************/
/* Create os level task routine                                             */
/****************************************************************************/
bool osif_task_create(void **pp_handle, const char *p_name, void (*p_routine)(void *),
                      void *p_param, uint16_t stack_size, uint16_t priority)
{
	pid_t pid;
	char routine_addr[9], param_addr[9];
	char *task_info[3];

	if (!pp_handle) {
		printf("[osif_task_create] pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (*pp_handle) {
		printf("[osif_task_create] task already init\r\n");
		return _FAIL;
	}

	task_info[0] = itoa((int) p_routine, routine_addr, 16);
	task_info[1] = itoa((int) p_param, param_addr, 16);
	task_info[2] = NULL;
	stack_size = stack_size * sizeof(uint32_t);
	priority = (priority + SCHED_PRIORITY_DEFAULT > SCHED_PRIORITY_MAX || priority + SCHED_PRIORITY_DEFAULT < SCHED_PRIORITY_MIN)? SCHED_PRIORITY_DEFAULT : priority + SCHED_PRIORITY_DEFAULT;

	pid = kernel_thread(p_name, priority, stack_size, osif_wrapper_thread, (char *const *) task_info);
	if (pid == ERROR) {
		printf("[osif_task_create] %s create fail\r\n", p_name);
		return _FAIL;
	}

	*pp_handle = (void *) ((uint32_t) pid);

	return _SUCCESS;
}

/****************************************************************************/
/* Delete os level task routine                                             */
/****************************************************************************/
bool osif_task_delete(void *p_handle)
{
	pid_t pid = (pid_t) ((uint32_t) p_handle);

	if (!p_handle) {
		printf("[osif_task_delete] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (task_delete(pid) != OK) {
		printf("[osif_task_delete] delete task 0x%x fail\r\n", p_handle);
		return _FAIL;
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Suspend os level task routine                                            */
/****************************************************************************/
bool osif_task_suspend(void *p_handle)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Resume os level task routine                                             */
/****************************************************************************/
bool osif_task_resume(void *p_handle)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Yield current os level task routine                                      */
/****************************************************************************/
bool osif_task_yield(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Get current os level task routine handle                                 */
/****************************************************************************/
bool osif_task_handle_get(void **pp_handle)
{
	if (!pp_handle) {
		printf("[osif_task_handle_get] pp_handle is NULL\r\n");
		return _FAIL;
	}

	*pp_handle = (void *) ((uint32_t) getpid());
	return _SUCCESS;
}

/****************************************************************************/
/* Get os level task routine priority                                       */
/****************************************************************************/
bool osif_task_priority_get(void *p_handle, uint16_t *p_priority)
{
	struct tcb_s *p_tcb;
	pid_t pid = (pid_t) ((uint32_t) p_handle);

	if (!p_handle) {
		printf("[osif_task_priority_get] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (p_priority == NULL) {
		printf("[osif_task_priority_get] p_priority is NULL\r\n");
		return _FAIL;
	}

	p_tcb = sched_gettcb(pid);

	*p_priority = p_tcb->sched_priority - SCHED_PRIORITY_DEFAULT;

	return _SUCCESS;
}

/****************************************************************************/
/* Set os level task routine priority                                       */
/****************************************************************************/
bool osif_task_priority_set(void *p_handle, uint16_t priority)
{
	struct tcb_s *p_tcb;
	pid_t pid = (pid_t) ((uint32_t) p_handle);

	if (!p_handle) {
		printf("[osif_task_priority_set] p_handle is NULL\r\n");
		return _FAIL;
	}

	p_tcb = sched_gettcb(pid);

	priority = (priority + SCHED_PRIORITY_DEFAULT > SCHED_PRIORITY_MAX || priority + SCHED_PRIORITY_DEFAULT < SCHED_PRIORITY_MIN)? SCHED_PRIORITY_DEFAULT : priority + SCHED_PRIORITY_DEFAULT;

	if (sched_setpriority(p_tcb, priority) != OK) {
		printf("[osif_task_priority_set] sched setpriority fail\r\n");
		return _FAIL;
	}

	return _SUCCESS;
}

static void *osif_sig_handle = NULL;

/****************************************************************************/
/* Init signal                                                              */
/****************************************************************************/
bool osif_signal_init(void)
{
	return osif_sem_create(&osif_sig_handle, 0, 1);
}

/****************************************************************************/
/* Deinit signal                                                            */
/****************************************************************************/
void osif_signal_deinit(void)
{
	osif_sem_delete(osif_sig_handle);
	osif_sig_handle = NULL;
}

/****************************************************************************/
/* Send signal to target task                                               */
/****************************************************************************/
bool osif_task_signal_send(void *p_handle, uint32_t signal)
{
	return osif_sem_give(osif_sig_handle);
}

/****************************************************************************/
/* Receive signal in target task                                            */
/****************************************************************************/
bool osif_task_signal_recv(uint32_t *p_handle, uint32_t wait_ms)
{
	return osif_sem_take(osif_sig_handle, wait_ms);
}

/****************************************************************************/
/* Clear signal in target task                                              */
/****************************************************************************/
bool osif_task_signal_clear(void *p_handle)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Lock critical section                                                    */
/****************************************************************************/
uint32_t osif_lock(void)
{
	return irqsave();
}

/****************************************************************************/
/* Unlock critical section                                                  */
/****************************************************************************/
void osif_unlock(uint32_t flags)
{
	irqrestore((irqstate_t) flags);
}

/****************************************************************************/
/* Create counting semaphore                                                */
/****************************************************************************/
bool osif_sem_create(void **pp_handle, uint32_t init_count, uint32_t max_count)
{
	sem_t *sem;

	if (!pp_handle) {
		printf("[osif_sem_create] pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (*pp_handle) {
		printf("[osif_sem_create] sem already init\r\n");
		return _FAIL;
	}

	sem = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(sem_t));
	if (sem == NULL) {
		printf("[osif_sem_create] alloc sem_t fail\r\n");
		return _FAIL;
	}
	memset(sem, 0, sizeof(sem_t));

	if (sem_init(sem, 0, init_count) != OK) {
		osif_mem_free(sem);
		printf("[osif_sem_create] sem init fail\r\n");
		return _FAIL;
	}

	*pp_handle = sem;

	return _SUCCESS;
}

/****************************************************************************/
/* Delete counting semaphore                                                */
/****************************************************************************/
bool osif_sem_delete(void *p_handle)
{
	if (!p_handle) {
		printf("[osif_sem_delete] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (sem_destroy((sem_t *) p_handle) != OK) {
		printf("[osif_sem_delete] sema destroy fail\r\n");
		return _FAIL;
	}

	osif_mem_free((sem_t *) p_handle);
	p_handle = NULL;

	return _SUCCESS;
}

/****************************************************************************/
/* Take counting semaphore                                                  */
/****************************************************************************/
bool osif_sem_take(void *p_handle, uint32_t wait_ms)
{
	if (!p_handle) {
		printf("[osif_sem_take] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (sem_timedwait((sem_t *) p_handle, &ts) != OK) {
			printf("[osif_sem_take] sema wait 0x%x ms fail\r\n", wait_ms);
			return _FAIL;
		}
	} else {
		if (sem_wait((sem_t *) p_handle) != OK) {
			printf("[osif_sem_take] sema wait fail\r\n");
			return _FAIL;
		}
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Give counting semaphore                                                  */
/****************************************************************************/
bool osif_sem_give(void *p_handle)
{
	if (!p_handle) {
		printf("[osif_sem_give] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (sem_post((sem_t *) p_handle) != OK) {
		printf("[osif_sem_give] sema post fail\r\n");
		return _FAIL;
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Create recursive mutex                                                   */
/****************************************************************************/
bool osif_mutex_create(void **pp_handle)
{
	return osif_sem_create(pp_handle, 0, 1);
}

/****************************************************************************/
/* Delete recursive mutex                                                   */
/****************************************************************************/
bool osif_mutex_delete(void *p_handle)
{
	return osif_sem_delete(p_handle);
}

/****************************************************************************/
/* Take recursive mutex                                                     */
/****************************************************************************/
bool osif_mutex_take(void *p_handle, uint32_t wait_ms)
{
	return osif_sem_take(p_handle, wait_ms);
}

/****************************************************************************/
/* Give recursive mutex                                                     */
/****************************************************************************/
bool osif_mutex_give(void *p_handle)
{
	return osif_sem_give(p_handle);
}

/****************************************************************************/
/* Create inter-thread message queue                                        */
/****************************************************************************/
bool osif_msg_queue_create(void **pp_handle, uint32_t msg_num, uint32_t msg_size)
{
	mqd_t pmqd;
	char mq_name[9];
	struct mq_attr attr;

	if (!pp_handle) {
		printf("[osif_msg_queue_create] pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (*pp_handle) {
		printf("[osif_msg_queue_create] msg queue already init\r\n");
		return _FAIL;
	}

	attr.mq_maxmsg = msg_num;
	attr.mq_msgsize = msg_size > MQ_MAX_BYTES? MQ_MAX_BYTES : msg_size;

	itoa((int) pp_handle, mq_name, 16);

	pmqd = mq_open((const char *) mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (pmqd == (mqd_t) ERROR) {
		printf("[osif_msg_queue_create] mq open fail\r\n");
		return _FAIL;
	}

	*pp_handle = pmqd;

	return _SUCCESS;
}

/****************************************************************************/
/* Delete inter-thread message queue                                        */
/****************************************************************************/
bool osif_msg_queue_delete(void *p_handle)
{
	if (!p_handle) {
		printf("[osif_msg_queue_delete] p_handle is NULL\r\n");
		return _FAIL;
	}

	if(mq_close((mqd_t) p_handle) != OK) {
		printf("[osif_msg_queue_delete] mq 0x%x close fail\r\n", p_handle);
		return _FAIL;
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Peek inter-thread message queue's pending but not received msg number    */
/****************************************************************************/
bool osif_msg_queue_peek(void *p_handle, uint32_t *p_msg_num)
{
	if (!p_handle) {
		printf("[osif_msg_queue_peek] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (p_msg_num == NULL) {
		printf("[osif_msg_queue_peek] p_msg_num is NULL\r\n");
		return _FAIL;
	}

	*p_msg_num = ((mqd_t) p_handle)->msgq->nmsgs;

	return _SUCCESS;
}

/****************************************************************************/
/* Send inter-thread message                                                */
/****************************************************************************/
bool osif_msg_send(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	int prio = MQ_PRIO_MAX;

	if (!p_handle) {
		printf("[osif_msg_send] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (up_interrupt_context() == false && wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (mq_timedsend((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio, &ts) != OK) {
			printf("[osif_msg_send] mq time send fail\r\n");
			return _FAIL;
		}
	} else {
		if (mq_send((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio) != OK) {
			printf("[osif_msg_send] mq send fail\r\n");
			return _FAIL;
		}
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Receive inter-thread message                                             */
/****************************************************************************/
bool osif_msg_recv(void *p_handle, void *p_msg, uint32_t wait_ms)
{
	int prio;

	if (!p_handle) {
		printf("[osif_msg_recv] p_handle is NULL\r\n");
		return _FAIL;
	}

	if (wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (mq_timedreceive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio, &ts) == ERROR) {
			if(ETIMEDOUT != get_errno())
			{
				printf("[osif_msg_recv] mq time receive fail errno:%d\r\n", get_errno());
			}
			return _FAIL;
		}
	} else {
		if (mq_receive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio) == ERROR) {
			printf("[osif_msg_recv] mq receive fail\r\n");
			return _FAIL;
		}
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Peek inter-thread message                                                */
/****************************************************************************/
bool osif_msg_peek(void *p_handle, void *p_msg, uint32_t wait_ms)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

/****************************************************************************/
/* Allocate memory                                                          */
/****************************************************************************/
void *osif_mem_alloc(RAM_TYPE ram_type, size_t size)
{
	return kmm_malloc(size);
}

/****************************************************************************/
/* Allocate aligned memory                                                  */
/****************************************************************************/
void *osif_mem_aligned_alloc(RAM_TYPE ram_type, size_t size, uint8_t alignment)
{
	return kmm_memalign(alignment, size);
}

/****************************************************************************/
/* Free memory                                                              */
/****************************************************************************/
void osif_mem_free(void *p_block)
{
	kmm_free(p_block);
}

/****************************************************************************/
/* Free aligned memory                                                      */
/****************************************************************************/
void osif_mem_aligned_free(void *p_block)
{
	kmm_free(p_block);
}

/****************************************************************************/
/* Peek unused (available) memory size                                    */
/****************************************************************************/
size_t osif_mem_peek(RAM_TYPE ram_type)
{
    /* Trace log uses the API, may ignore it if difficult to implement */
    /* TIZEN_RT_NOT_REQUIRED */
    return 0;
}

struct osif_timer_list {
	struct osif_timer_list *prev, *next;
	struct work_s *work_hdl;
	unsigned char timer_name[16];
	uint32_t timer_id;
	uint32_t timeout;
	uint32_t reload;
	uint32_t live;
	void (*function)(void *);
	void *data;
};

struct osif_timer_entry {
	struct list_head list;
	struct osif_timer_list *timer;
};

_list osif_timer_table;
bool osif_timer_table_init = 0;

void osif_timer_wrapper(void *timer)
{
	uint32_t lock;
	_list *plist;
	struct osif_timer_entry *timer_entry = NULL;

	lock = osif_lock();

	plist = get_next(&osif_timer_table);
	while ((rtw_end_of_queue_search(&osif_timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct osif_timer_entry, list);
		if (timer_entry->timer == timer) {
			break;
		}
		plist = get_next(plist);
	}

	osif_unlock(lock);

	if (plist == &osif_timer_table) {
		printf("[osif_timer_wrapper] find timer_entry fail\r\n");
		return;
	}

	if (timer_entry->timer->reload) {
		if (work_queue(LPWORK, timer_entry->timer->work_hdl, osif_timer_wrapper, (void *) (timer_entry->timer), (timer_entry->timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
			printf("[osif_timer_wrapper] work queue fail\r\n");
			timer_entry->timer->live = 0;
		}
	} else {
		timer_entry->timer->live = 0;
	}

	if (timer_entry->timer->function) {
		timer_entry->timer->function((void *) timer_entry->timer->data);
	}
}

/****************************************************************************/
/* Get software timer ID                                                    */
/****************************************************************************/
bool osif_timer_id_get(void **pp_handle, uint32_t *p_timer_id)
{
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		printf("[osif_timer_id_get] pp_handle || *pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (p_timer_id == NULL) {
		printf("[osif_timer_id_get] p_timer_id is NULL\r\n");
		return _FAIL;
	}

	timer = *pp_handle;
	*p_timer_id = timer->timer_id;

	return _SUCCESS;
}

/****************************************************************************/
/* Create software timer                                                    */
/****************************************************************************/
bool osif_timer_create(void **pp_handle, const char *p_timer_name, uint32_t timer_id,
                       uint32_t interval_ms, bool reload, void (*p_timer_callback)(void *))
{
	uint32_t lock;
	struct osif_timer_list *timer;
	struct osif_timer_entry *timer_entry;

	if (!pp_handle) {
		printf("[osif_timer_create] pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (*pp_handle) {
		printf("[osif_timer_create] timer already init\r\n");
		return _FAIL;
	}

	timer = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct osif_timer_list));
	if (timer == NULL) {
		printf("[osif_timer_create] alloc osif_timer_list fail\r\n");
		return _FAIL;
	}
	memset(timer, 0, sizeof(struct osif_timer_list));

	timer->work_hdl = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct work_s));
	if (timer->work_hdl == NULL) {
		printf("[osif_timer_create] alloc work_s fail\r\n");
		osif_mem_free(timer);
		return _FAIL;
	}
	memset(timer->work_hdl, 0, sizeof(struct work_s));

	memcpy(timer->timer_name, p_timer_name, 16);
	timer->timer_id = timer_id;
	timer->timeout = interval_ms;
	timer->reload = reload;
	timer->live = 0;
	timer->function = p_timer_callback;
	timer->data = timer;

	*pp_handle = timer;

	if (!osif_timer_table_init) {
		rtw_init_listhead(&osif_timer_table);
		osif_timer_table_init = 1;
	}

	timer_entry = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct osif_timer_entry));
	if (timer_entry == NULL) {
		printf("[osif_timer_create] alloc osif_timer_entry fail\r\n");
		osif_mem_free(timer->work_hdl);
		osif_mem_free(timer);
		return _FAIL;
	}
	memset(timer_entry, 0, sizeof(struct osif_timer_entry));

	timer_entry->timer = timer;

	lock = osif_lock();
	rtw_list_insert_head(&(timer_entry->list), &osif_timer_table);
	osif_unlock(lock);

	return _SUCCESS;
}

/****************************************************************************/
/* Start software timer                                                     */
/****************************************************************************/
bool osif_timer_start(void **pp_handle)
{
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		printf("[osif_timer_start] pp_handle || *pp_handle is NULL\r\n");
		return _FAIL;
	}

	timer = *pp_handle;
	if (work_queue(LPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
		printf("[osif_timer_start] work queue fail\r\n");
		return _FAIL;
	}

	timer->live = 1;

	return _SUCCESS;
}

/****************************************************************************/
/* Restart software timer                                                   */
/****************************************************************************/
bool osif_timer_restart(void **pp_handle, uint32_t interval_ms)
{
	int ret;
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		printf("[osif_timer_restart] pp_handle || *pp_handle is NULL\r\n");
		return _FAIL;
	}

	timer = *pp_handle;
	ret = work_queue(LPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (interval_ms * TICK_PER_SEC / 1000L));
	if (ret == -EALREADY) {
		if (work_cancel(LPWORK, timer->work_hdl) != OK) {
			printf("[osif_timer_restart] work cancel fail\r\n");
			return _FAIL;
		}

		if (work_queue(LPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (interval_ms * TICK_PER_SEC / 1000L)) != OK) {
			printf("[osif_timer_restart] work queue fail\r\n");
			return _FAIL;
		}
	} else if (ret != OK) {
		printf("[osif_timer_restart] work queue fail\r\n");
		return _FAIL;
	}

	timer->timeout = interval_ms;
	timer->live = 1;

	return _SUCCESS;
}

/****************************************************************************/
/* Stop software timer                                                      */
/****************************************************************************/
bool osif_timer_stop(void **pp_handle)
{
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		printf("[osif_timer_stop] pp_handle || *pp_handle is NULL\r\n");
		return _FAIL;
	}

	timer = *pp_handle;
	if (work_cancel(LPWORK, timer->work_hdl) != OK) {
		printf("[osif_timer_stop] work cancel fail\r\n");
		return _FAIL;
	}

	timer->live = 0;

	return _SUCCESS;
}

/****************************************************************************/
/* Delete software timer                                                    */
/****************************************************************************/
bool osif_timer_delete(void **pp_handle)
{
	int ret;
	uint32_t lock;
	_list *plist;
	struct osif_timer_list *timer;
	struct osif_timer_entry *timer_entry;

	if (pp_handle == NULL || *pp_handle == NULL) {
		return _FAIL;
	}

	timer = *pp_handle;
	ret = work_cancel(LPWORK, timer->work_hdl);
	if (ret != OK && ret != -ENOENT) {
		printf("[osif_timer_delete] work cancel fail\r\n");
		return _FAIL;
	}

	lock = osif_lock();

	plist = get_next(&osif_timer_table);
	while ((rtw_end_of_queue_search(&osif_timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct osif_timer_entry, list);
		if (timer_entry->timer == timer) {
			rtw_list_delete(plist);
			osif_mem_free(timer_entry);
			break;
		}
		plist = get_next(plist);
	}

	osif_unlock(lock);

	if (plist == &osif_timer_table) {
		printf("[osif_timer_delete] find timer_entry fail\r\n");
		return _FAIL;
	}

	timer->data = NULL;
	timer->function = NULL;
	timer->live = 0;
	timer->reload = 0;
	timer->timeout = 0;
	timer->timer_id = 0;
	memset(timer->timer_name, 0, 16);
	osif_mem_free(timer->work_hdl);
	timer->work_hdl = NULL;
	osif_mem_free(timer);
	timer = NULL;
	*pp_handle = NULL;

	return _SUCCESS;
}

/****************************************************************************/
/* Get timer state                                                          */
/****************************************************************************/
bool osif_timer_state_get(void **pp_handle, uint32_t *p_timer_state)
{
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		printf("[osif_timer_state_get] pp_handle || *pp_handle is NULL\r\n");
		return _FAIL;
	}

	if (p_timer_state == NULL) {
		printf("[osif_timer_state_get] p_timer_state is NULL\r\n");
		return _FAIL;
	}

	timer = *pp_handle;
	*p_timer_state = timer->live;

	return _SUCCESS;
}

/****************************************************************************/
/* Dump software timer                                                      */
/****************************************************************************/
bool osif_timer_dump(void)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return true;
}

void osif_create_secure_context(uint32_t size)
{
    /* TIZEN_RT_NOT_REQUIRED */
    return;
}
