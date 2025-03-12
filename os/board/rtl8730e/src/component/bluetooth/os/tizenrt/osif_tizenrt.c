/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#include <fcntl.h>
#include <osif.h>
#include <os_wrapper.h>
#include "../kernel/mqueue/mqueue.h"

#define OSIF_ALIGN 64
#define OSIF_ALIGN_MASK 0x003f

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
		dbg("%s osif_wrapper_thread error\n", argv[0]);
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
	/* The following character array of size 9 is used to store pointer address in hex represented as ascii.
	   As address has 32-bit integer value, the maximum string length in hex is 8 with 1 null character.*/
	char routine_addr[9], param_addr[9];
	char *task_info[3];

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (*pp_handle) {
		dbg("%s %p\n", p_name, *pp_handle);
		dbg("task already init\n");
		return _FAIL;
	}

	task_info[0] = itoa((int) p_routine, routine_addr, 16);
	task_info[1] = itoa((int) p_param, param_addr, 16);
	task_info[2] = NULL;
	stack_size = stack_size * sizeof(uint32_t);
	priority = priority + SCHED_PRIORITY_DEFAULT;
	if (priority > SCHED_PRIORITY_MAX)
		priority = SCHED_PRIORITY_DEFAULT;

	pid = kernel_thread(p_name, priority, stack_size, osif_wrapper_thread, (char *const *) task_info);
	if (pid == ERROR) {
		dbg("%s create fail\n", p_name);
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

	if (task_delete(pid) != OK) {
		dbg("delete task 0x%x fail\n", p_handle);
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
		dbg("pp_handle is NULL\n");
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
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if (p_priority == NULL) {
		dbg("p_priority is NULL\n");
		return _FAIL;
	}

	p_tcb = sched_gettcb(pid);

	if (!p_tcb) {
		dbg("p_tcb is NULL\n");
		return _FAIL;
	}

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
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	p_tcb = sched_gettcb(pid);
	if (!p_tcb) {
		dbg("pid %d tcb is NULL\n", pid);
		return _FAIL;
	}

	priority = priority + SCHED_PRIORITY_DEFAULT;
	if (priority > SCHED_PRIORITY_MAX)
		priority = SCHED_PRIORITY_DEFAULT;

	if (sched_setpriority(p_tcb, priority) != OK) {
		dbg("sched setpriority fail\n");
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
	uint32_t flags = 0U;
	if (osif_task_context_check() == true)
	{
		flags = tizenrt_critical_enter();
	}
	return flags;
}

/****************************************************************************/
/* Unlock critical section                                                  */
/****************************************************************************/
void osif_unlock(uint32_t flags)
{
	if (osif_task_context_check() == true)
	{
		tizenrt_critical_exit(flags);
	}
}

/****************************************************************************/
/* Create counting semaphore                                                */
/****************************************************************************/
bool osif_sem_create(void **pp_handle, uint32_t init_count, uint32_t max_count)
{
	sem_t *sem;

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (*pp_handle) {
		dbg("sem already init\n");
		return _FAIL;
	}

	sem = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(sem_t));
	if (sem == NULL) {
		dbg("alloc sem_t fail\n");
		return _FAIL;
	}
	memset(sem, 0, sizeof(sem_t));

	if (sem_init(sem, 0, init_count) != OK) {
		osif_mem_free(sem);
		dbg("sem init fail\n");
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
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if (sem_destroy((sem_t *) p_handle) != OK) {
		dbg("sema destroy fail\n");
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
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (sem_timedwait((sem_t *) p_handle, &ts) != OK) {
			dbg("sema wait 0x%x ms fail\n", wait_ms);
			return _FAIL;
		}
	} else {
		if (sem_wait((sem_t *) p_handle) != OK) {
			dbg("sema wait fail\n");
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
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (sem_post((sem_t *) p_handle) != OK) {
		dbg("sema post fail\n");
		return _FAIL;
	}

	return _SUCCESS;
}

/****************************************************************************/
/* Create recursive mutex                                                   */
/****************************************************************************/
bool osif_mutex_create(void **pp_handle)
{
	return osif_sem_create(pp_handle, 1, 1);
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
	/* The following character array of size 9 is used to store pointer address in hex represented as ascii.
	   As address has 32-bit integer value, the maximum string length in hex is 8 with 1 null character.*/
	char mq_name[9];
	struct mq_attr attr;
	int ret;

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (*pp_handle) {
		dbg("msg queue already init\n");
		return _FAIL;
	}

	attr.mq_maxmsg = msg_num;
	attr.mq_msgsize = msg_size > MQ_MAX_BYTES? MQ_MAX_BYTES : msg_size;

	itoa((int) pp_handle, mq_name, 16);

	pmqd = mq_open((const char *) mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (pmqd == (mqd_t) ERROR) {
		ret = get_errno();
		dbg("mq open fail: %d\n", ret);
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
	int ret;

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if(mq_close((mqd_t) p_handle) != OK) {
		ret = get_errno();
		dbg("mq 0x%x close fail: %d\n", p_handle, ret);
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
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if (p_msg_num == NULL) {
		dbg("p_msg_num is NULL\n");
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
	int ret;

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if (wait_ms != 0xFFFFFFFF && osif_task_context_check() == true) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedsend((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio, &ts) != OK) {
			ret = get_errno();
			dbg("mq time send fail: %d\n", ret);
			return _FAIL;
		}
	} else {
		if (mq_send((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio) != OK) {
			ret = get_errno();
			dbg("mq send fail: %d\n", ret);
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
	int ret;

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return _FAIL;
	}

	if (wait_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedreceive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio, &ts) == ERROR) {
			ret = get_errno();
			if (ETIMEDOUT != ret)
			{
				dbg("mq time receive fail errno: %d\n", ret);
			}
			return _FAIL;
		}
	} else {
		if (mq_receive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio) == ERROR) {
			ret = get_errno();
			dbg("mq receive fail: %d\n", ret);
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
	u8 *pbuf = kmm_malloc(size);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	if (pbuf)
	{
		DEBUG_SET_CALLER_ADDR(pbuf);
	}
#endif
	return pbuf;
}

/****************************************************************************/
/* Allocate aligned memory                                                  */
/****************************************************************************/
void *osif_mem_aligned_alloc(RAM_TYPE ram_type, size_t size, uint8_t alignment)
{
	if ((size & OSIF_ALIGN_MASK) != 0x00) {
		size += (OSIF_ALIGN - (size & OSIF_ALIGN_MASK));
	}
	u8 *pbuf = kmm_memalign(alignment, size);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	if (pbuf)
	{
		memset(pbuf, 0, size);
		DEBUG_SET_CALLER_ADDR(pbuf);
	}
#endif
	return pbuf;
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

/****************************************************************************/
/* To be only used in BLE related functions                                 */
/****************************************************************************/
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
		dbg("find timer_entry fail\n");
		return;
	}

	if (timer_entry->timer->reload) {
		if (work_queue(HPWORK, timer_entry->timer->work_hdl, osif_timer_wrapper, (void *) (timer_entry->timer), (timer_entry->timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
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
		dbg("pp_handle || *pp_handle is NULL\n");
		return _FAIL;
	}

	if (p_timer_id == NULL) {
		dbg("p_timer_id is NULL\n");
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
		dbg("pp_handle is NULL\n");
		return _FAIL;
	}

	if (*pp_handle) {
		dbg("timer already init\n");
		return _FAIL;
	}

	timer = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct osif_timer_list));
	if (timer == NULL) {
		dbg("alloc osif_timer_list fail\n");
		return _FAIL;
	}
	memset(timer, 0, sizeof(struct osif_timer_list));

	timer->work_hdl = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct work_s));
	if (timer->work_hdl == NULL) {
		dbg("alloc work_s fail\n");
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
		dbg("alloc osif_timer_entry fail\n");
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
/* Start software timer, to be only used in BLE related functions           */
/****************************************************************************/
bool osif_timer_start(void **pp_handle)
{
	int ret;
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		dbg("pp_handle || *pp_handle is NULL\n");
		return _FAIL;
	}

	timer = *pp_handle;
	ret = work_queue(HPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (timer->timeout * TICK_PER_SEC / 1000L));
	if (ret == -EALREADY) {
		if (work_cancel(HPWORK, timer->work_hdl) != OK) {
			dbg("work cancel fail\n");
			return _FAIL;
		}

		if (work_queue(HPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
			return _FAIL;
		}
	} else if (ret != OK) {
		dbg("work queue fail\n");
		return _FAIL;
	}

	timer->live = 1;

	return _SUCCESS;
}

/****************************************************************************/
/* Restart software timer, to be only used in BLE related functions         */
/****************************************************************************/
bool osif_timer_restart(void **pp_handle,uint32_t interval_ms)
{
	int ret;
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		dbg("pp_handle || *pp_handle is NULL\n");
		return _FAIL;
	}

	timer = *pp_handle;
	ret = work_queue(HPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (interval_ms * TICK_PER_SEC / 1000L));
	if (ret == -EALREADY) {
		if (work_cancel(HPWORK, timer->work_hdl) != OK) {
			dbg("work cancel fail\n");
			return _FAIL;
		}

		if (work_queue(HPWORK, timer->work_hdl, osif_timer_wrapper, (void *) (timer), (interval_ms * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
			return _FAIL;
		}
	} else if (ret != OK) {
		dbg("work queue fail\n");
		return _FAIL;
	}

	timer->timeout = interval_ms;
	timer->live = 1;

	return _SUCCESS;
}

/****************************************************************************/
/* Stop software timer, to be only used in BLE related functions            */
/****************************************************************************/
bool osif_timer_stop(void **pp_handle)
{
	struct osif_timer_list *timer;

	if (pp_handle == NULL || *pp_handle == NULL) {
		dbg("pp_handle || *pp_handle is NULL\n");
		return _FAIL;
	}

	timer = *pp_handle;
	if (work_cancel(HPWORK, timer->work_hdl) != OK) {
		dbg("work cancel fail\n");
		return _FAIL;
	}

	timer->live = 0;

	return _SUCCESS;
}

/****************************************************************************/
/* Delete software timer, to be only used in BLE related functions          */
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
	ret = work_cancel(HPWORK, timer->work_hdl);
	if (ret != OK && ret != -ENOENT) {
		dbg("work cancel fail\n");
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
		dbg("find timer_entry fail\n");
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
		dbg("pp_handle || *pp_handle is NULL\n");
		return _FAIL;
	}

	if (p_timer_state == NULL) {
		dbg("p_timer_state is NULL\n");
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