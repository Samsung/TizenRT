#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <common/bk_kernel_err.h>
#include <common/bk_generic.h>
#include <os/mem.h>
// #include "bk_uart.h"
#include <components/system.h>
#include "cmsis_gcc.h"


#include <sched.h>
#include <semaphore.h>

#include "pthread.h"
#include <sys/types.h>
#include <threads.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>
#include <tinyara/sched.h>
#include <arch/irq.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/config.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/mm.h>
#include <tinyara/time.h>
#include <tinyara/wdog.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../kernel/mqueue/mqueue.h"

/******************************************************
 *                    Constants
 ******************************************************/
typedef struct {
    mqd_t mqdes;
    struct task_group_s *creator_group;
} beken_queue_internal_t;

 static inline uint32_t check_local_irq_disabled(void)
{
#ifdef CONFIG_ARMV8M_USEBASEPRI
    /* In BASEPRI mode, non-zero value means IRQs are disabled */
    return (__get_BASEPRI() != 0);
#else
    /* In PRIMASK mode, bit 0 being set means IRQs are disabled */
    return ((__get_PRIMASK() & 0x01) != 0);
#endif
}

/* some kernel APIs have blocking mechanisms that can only be 
used in task context and not in interrupt context, otherwise
the system status will be abnormal */
#define RTOS_ASSERT_TASK_CONTEXT() do {                    \
	BK_ASSERT(0 == rtos_is_in_interrupt_context());        \
} while(0)

/* when the scheduler is turned on,APIs that can trigger task 
switching(such as creating a new task or delete a task) cannot
be called in the critical section,because this will cause the system
to enter a deadlock or abnormal status */
#define RTOS_ASSERT_INT_ENABLED_WITH_SCHEDULER() do {    \
		BK_ASSERT(0 == check_local_irq_disabled());      \
	                                                     \
} while(0)

/* Turning off interrupts is equivalent to entering a critical section. At 
  this time,you cannot call any API that may cause system scheduling, otherwise
  it may cause a system exception */
#define RTOS_ASSERT_INT_ENABLED() do {                 \
	BK_ASSERT(0 == check_local_irq_disabled());        \
} while(0)

#define  BEKEN_RTOS_DEFAULT_PRIORITY_MIN 180

 /******************************************************
 *               Function Definitions
 ******************************************************/
bool rtos_is_in_interrupt_context(void)
{
   return   up_interrupt_context();  
}

bool rtos_local_irq_disabled(void)
{
    return check_local_irq_disabled();
}

static int wrapper_thread(int argc, char *argv[])
{
	int func_addr, ctx_addr;
	beken_thread_function_t func;
	void *thctx;
	if (argc != 3) {
		return -1;
	}
	/* Arguments : [0] task name [1] func addr, [2] ctx addr */
	func_addr = strtoul(argv[1], NULL, 16);
	ctx_addr = strtoul(argv[2], NULL, 16);
	func = (beken_thread_function_t)func_addr;
	thctx = (void *)ctx_addr;
	func(thctx);
	return OK;
}

bk_err_t rtos_create_thread( beken_thread_t* thread, uint8_t priority, const char* name, 
                        beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg )
{
    int func_addr, ctx_addr;
    pid_t pid;
	char str_func_addr[9];
	char str_ctx_addr[9];
	char *task_info[3];
    priority = BEKEN_RTOS_DEFAULT_PRIORITY_MIN + (9 - priority);
	priority = (priority > SCHED_PRIORITY_MAX || priority < SCHED_PRIORITY_MIN) ? SCHED_PRIORITY_DEFAULT:priority;

	func_addr = (int)function;
	ctx_addr = (int)arg;

    snprintf(str_func_addr, sizeof(str_func_addr), "%x", func_addr);
    task_info[0] = str_func_addr;
    snprintf(str_ctx_addr, sizeof(str_ctx_addr), "%x", ctx_addr);
    task_info[1] = str_ctx_addr;
	task_info[2] = NULL;

    pid = kernel_thread(name, priority, stack_size, wrapper_thread, (char * const *)task_info);
	if (pid == ERROR) {
		return kGeneralErr;
	}

    *thread = (beken_thread_t)(intptr_t)pid;
    return kNoErr;
}


bk_err_t rtos_delete_thread( beken_thread_t* thread )
{
    pid_t pid;
	int status;

    if (thread == NULL) {
        FAR struct tcb_s *tcb = sched_self();
        if (tcb == NULL) {
            return kGeneralErr;
        }
        pid = tcb->pid;
    } 
    else {
        pid = (pid_t)(intptr_t) *thread;
    }

	status = task_delete(pid);
	if (status != OK) {
        return kGeneralErr;
	}

	return kNoErr;
}

bk_err_t rtos_thread_join(beken_thread_t* thread)
{
    return kGeneralErr;
}

bool rtos_is_current_thread( beken_thread_t* thread )
{
    FAR struct tcb_s *tcb = sched_self();
    if (*thread == tcb->pid) {
        return true;
    }
    return false;
}

beken_thread_t* rtos_get_current_thread( void )
{
    FAR struct tcb_s *tcb = sched_self();
    if (tcb == NULL) {
        return NULL;
    }
    return (beken_thread_t *)(intptr_t)tcb->pid;
}

bk_err_t rtos_thread_set_priority(beken_thread_t *thread, int priority)
{
    FAR struct tcb_s *rtcb = sched_gettcb(*(pid_t *)thread);

	if (rtcb == NULL) {
		return kGeneralErr;
	}

	sched_setpriority(rtcb, priority + SCHED_PRIORITY_DEFAULT);
	return kNoErr;
}


void rtos_thread_sleep(uint32_t seconds)
{
    rtos_delay_milliseconds(seconds * 1000);
}


void rtos_suspend_all_thread(void)
{
    sched_lock();
}

void rtos_resume_all_thread(void)
{
    sched_unlock();
}

bk_err_t rtos_init_semaphore( beken_semaphore_t* semaphore, int InitCount )
{
    (void)InitCount;
    
	sem_t *sem = (sem_t *)kmm_malloc(sizeof(*sem)); 
	if (!sem) {
		return kGeneralErr;
	}
	int status = sem_init(sem, 0, 0);
	if (status != OK) {
        kmm_free(sem);
		return kGeneralErr;
	}
    *semaphore = (void *)sem;
	return kNoErr;
}

bk_err_t rtos_init_semaphore_ex( beken_semaphore_t* semaphore, int maxCount, int init_count)
{
    BK_LOGE( NULL, "rtos_init_semaphore_ex not support!\n");
    return -1;
}

bk_err_t rtos_get_semaphore( beken_semaphore_t* semaphore, uint32_t timeout_ms )
{
    struct timespec ts;
    int ret;

    if (semaphore == NULL) {
		return kGeneralErr;
	}

    RTOS_ASSERT_TASK_CONTEXT();

    if(timeout_ms == BEKEN_WAIT_FOREVER)
    {
        ret = sem_wait((sem_t *)*semaphore);
        if (ret == OK) {
			return kNoErr;
		} else {
			return kGeneralErr;
		}
    }      
    else
    {    
        clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout_ms / 1000;		
        ret = sem_timedwait((sem_t *)*semaphore, &ts);
		if (ret == OK) {
			return kNoErr;
		} else {
			return kTimeoutErr;
		}
    }
}

int rtos_get_semaphore_count( beken_semaphore_t* semaphore )
{
    int ret;
    int sval;
	if (semaphore == NULL) {
		return kGeneralErr;
	}
    ret = sem_getvalue((sem_t *)*semaphore, &sval);
    if (ret == OK) {
        return sval;
    } else {
        return 0;
    }
}

int rtos_set_semaphore( beken_semaphore_t* semaphore )
{
    int ret;
	if (semaphore == NULL) {
		return kGeneralErr;
	}
	ret = sem_post((sem_t *)*semaphore);
	if (ret == OK) {
		return kNoErr;
	} else {
		return kGeneralErr;
	}
}

bk_err_t rtos_deinit_semaphore( beken_semaphore_t* semaphore )
{
    int ret;
    if (semaphore == NULL) {
		return kGeneralErr;
	}
	ret = sem_destroy((sem_t *)*semaphore);
    if (ret == OK) {
        kmm_free(*semaphore);
        return kNoErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_init_mutex( beken_mutex_t* mutex )
{
    pthread_mutexattr_t mattr;
	int status = 0;

	pthread_mutex_t *s_mutex = (pthread_mutex_t *)kmm_malloc(sizeof(pthread_mutex_t));
	if (s_mutex == NULL) {
		return kGeneralErr;
	}
	pthread_mutexattr_init(&mattr);
	status = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
	if (status != 0) {
		pthread_mutexattr_destroy(&mattr);
		kmm_free(s_mutex);
		return kGeneralErr;
	}
	status = pthread_mutex_init(s_mutex, &mattr);
	if (status) {
		pthread_mutexattr_destroy(&mattr);
		kmm_free(s_mutex);
		return kGeneralErr;
	}
	pthread_mutexattr_destroy(&mattr);
    *mutex = (void *)s_mutex;
	return kNoErr;
}

bk_err_t rtos_trylock_mutex(beken_mutex_t *mutex)
{
    if (mutex == NULL) {
		return kGeneralErr;
	}
	int ret = pthread_mutex_trylock((pthread_mutex_t *)*mutex);
	if (ret != OK) {
		return kGeneralErr;
	}
	return kNoErr;
}

bk_err_t rtos_lock_mutex( beken_mutex_t* mutex )
{
    if (mutex == NULL) {
		return kGeneralErr;
	}
	int ret = pthread_mutex_lock((pthread_mutex_t *)*mutex);
	if (ret != OK) {
		return kGeneralErr;
	}
	return kNoErr;
}

bk_err_t rtos_unlock_mutex( beken_mutex_t* mutex )
{

    if (mutex == NULL) {
		return kGeneralErr;
	}
	int ret = pthread_mutex_unlock((pthread_mutex_t *)*mutex);
	if (ret != OK) {
		return kGeneralErr;
	}
	return kNoErr;
}

bk_err_t rtos_deinit_mutex( beken_mutex_t* mutex )
{
   int ret;
   pthread_mutex_t *s_mutex;

   if (mutex == NULL) {
		return kGeneralErr;
	}
	s_mutex = (pthread_mutex_t *)*mutex;
	ret = pthread_mutex_destroy(s_mutex);
	if (ret == OK) {
		kmm_free(s_mutex);
		*mutex = NULL;
		return kNoErr;
	} else {
		return kGeneralErr;
	}
}

bk_err_t rtos_init_recursive_mutex( beken_mutex_t* mutex )
{
    pthread_mutexattr_t mattr;
	int status = 0;

	pthread_mutex_t *s_mutex = (pthread_mutex_t *)kmm_malloc(sizeof(pthread_mutex_t));
	if (s_mutex == NULL) {
		return kGeneralErr;
	}
	pthread_mutexattr_init(&mattr);
	status = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
	if (status != 0) {
		pthread_mutexattr_destroy(&mattr);
		kmm_free(s_mutex);
		return kGeneralErr;
	}
	status = pthread_mutex_init(s_mutex, &mattr);
	if (status) {
		pthread_mutexattr_destroy(&mattr);
		kmm_free(s_mutex);
		return kGeneralErr;
	}
	pthread_mutexattr_destroy(&mattr);
    *mutex = (void *)s_mutex;
	return kNoErr;
}

bk_err_t rtos_lock_recursive_mutex( beken_mutex_t* mutex )
{
	return rtos_lock_mutex(mutex);
}

bk_err_t rtos_unlock_recursive_mutex( beken_mutex_t* mutex )
{
    return rtos_unlock_mutex(mutex);
}

bk_err_t rtos_deinit_recursive_mutex( beken_mutex_t* mutex )
{
    return rtos_deinit_mutex(mutex);
}

bk_err_t rtos_init_queue( beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages )
{
    struct mq_attr attr;
    mqd_t pmqd;
    int ret;

    attr.mq_maxmsg = number_of_messages;
	attr.mq_msgsize = message_size > MQ_MAX_BYTES? MQ_MAX_BYTES : message_size;

	pmqd = mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
	if (pmqd == (mqd_t) ERROR) {
		ret = get_errno();
		return kGeneralErr;
	}

    beken_queue_internal_t *internal = (beken_queue_internal_t *)kmm_malloc(sizeof(beken_queue_internal_t));
    if (internal == NULL) {
		mq_close(pmqd);
		return kGeneralErr;
	}
    internal->mqdes = pmqd;
    internal->creator_group = ((FAR struct tcb_s *)sched_self())->group;

	*queue = (beken_queue_t)internal;
    return kNoErr;
}

bk_err_t rtos_push_to_queue( beken_queue_t* queue, void* message, uint32_t timeout_ms )
{
    int prio = MQ_PRIO_MAX;
	int ret;

    if (*queue == NULL) {
		return kGeneralErr;
	}
    
    mqd_t mqdes = ((beken_queue_internal_t *)*queue)->mqdes;

    if ((timeout_ms != BEKEN_WAIT_FOREVER) && (!rtos_is_in_interrupt_context())) {
        struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout_ms / 1000;
		ts.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedsend(mqdes, message, mqdes->msgq->maxmsgsize, prio, &ts) != OK) {
			ret = get_errno();
			return kGeneralErr;
		}
    } else {
		if (mq_send((mqd_t) mqdes, message, mqdes->msgq->maxmsgsize, prio) != OK) {
			ret = get_errno();
			return kGeneralErr;
		}
	}

    return kNoErr;
}


bk_err_t rtos_push_to_queue_front( beken_queue_t* queue, void* message, uint32_t timeout_ms )
{
    BK_LOGE( NULL, "rtos_push_to_queue_front not support!\n");
    return -1;
}

bk_err_t rtos_pop_from_queue( beken_queue_t* queue, void* message, uint32_t timeout_ms )
{
    int prio;
	int ret;

	if (*queue == NULL) {
		return kGeneralErr;
	}

    mqd_t mqdes = ((beken_queue_internal_t *)*queue)->mqdes;

	if (timeout_ms != BEKEN_WAIT_FOREVER) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout_ms / 1000;
		ts.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedreceive(mqdes, message, mqdes->msgq->maxmsgsize, &prio, &ts) == ERROR) {
			ret = get_errno();
			return kGeneralErr;
		}
	} else {
		if (mq_receive(mqdes, message, mqdes->msgq->maxmsgsize, &prio) == ERROR) {
			ret = get_errno();
			return kGeneralErr;
		}
	}
    return kNoErr;
}

bk_err_t rtos_deinit_queue( beken_queue_t* queue )
{
    int ret;

	if (*queue == NULL) {
		return kGeneralErr;
	}

    beken_queue_internal_t *internal = (beken_queue_internal_t *)*queue;

    if (internal->mqdes == NULL || internal->creator_group == NULL) {
        kmm_free(internal);
        *queue = NULL;
        return kGeneralErr;
    }

    mqd_t mqdes = internal->mqdes;
    struct task_group_s *creator_group = internal->creator_group;

    sched_lock();
    ret = mq_close_group(mqdes, creator_group);
    sched_unlock();

    if (ret != OK) {
        kmm_free(internal);
        *queue = NULL;
        return kGeneralErr;
    }

    kmm_free(internal);
    *queue = NULL;

    return kNoErr;
}


bool rtos_is_queue_empty( beken_queue_t* queue )
{
    struct mq_attr mq_stat;
    int ret;

    if (*queue == NULL) {
		return kGeneralErr;
	}

    mqd_t mqdes = ((beken_queue_internal_t *)*queue)->mqdes;

    ret = mq_getattr(mqdes, &mq_stat);
    if (ret == OK) 
    {
        return mq_stat.mq_curmsgs == 0 ? true : false;
    }
    else 
    {
        return false;
    }   
}

bool rtos_is_queue_full( beken_queue_t* queue )
{
    struct mq_attr mq_stat;
    int ret;

    if (*queue == NULL) {
		return kGeneralErr;
	}

    mqd_t mqdes = ((beken_queue_internal_t *)*queue)->mqdes;

    ret = mq_getattr(mqdes, &mq_stat);
    if (ret == OK) 
    {
        return mq_stat.mq_curmsgs == mq_stat.mq_maxmsg ? true : false;
    }
    else 
    {
        return false;
    }  
}

bool rtos_reset_queue( beken_queue_t* queue )
{
    return false;
}

static void timer_callback2( beken2_timer_t  *timer )
{
    if (timer->handle == NULL) 
    {      
        return ;
    }

	if(BEKEN_MAGIC_WORD != timer->beken_magic)
	{
		return;
	}
    if ( timer->function )
    {
        timer->function( timer->left_arg, timer->right_arg );
    }
}

static void timer_callback1( beken_timer_t * timer)
{
    if (timer->handle == NULL) 
    {      
        return ;
    }

    if ( timer->function )
    {
        timer->function( timer->arg);
    }
}

static void  period_timer_callback(int argc, uint32_t arg, ...)
{
	beken_timer_t *timer = (beken_timer_t *) arg;
	work_cancel(HPWORK, (FAR struct work_s *)timer->timer_next);
	work_queue(HPWORK, (FAR struct work_s *)timer->timer_next, (worker_t)timer_callback1, timer, 0);
    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID)timer->handle, delay, period_timer_callback, 1, timer);
}

static void once_timer_callback(int argc, uint32_t arg, ...)
{
	beken2_timer_t *timer = (beken2_timer_t *) arg;
	work_cancel(HPWORK, (FAR struct work_s *)timer->timer_next);
	work_queue(HPWORK, (FAR struct work_s *)timer->timer_next, (worker_t)timer_callback2, timer, 0);
}

bk_err_t rtos_start_oneshot_timer( beken2_timer_t* timer )
{
    if (timer->handle == NULL) 
    {       
        return kGeneralErr;
    }

    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID) timer->handle, delay, once_timer_callback, 1, timer);

    return kNoErr;
}

bk_err_t rtos_deinit_oneshot_timer( beken2_timer_t* timer )
{
    if (timer->handle == NULL) 
    {
        return kGeneralErr;       
    }

    wd_delete((WDOG_ID) timer->handle);
    kmm_free(timer->timer_next);
    timer->timer_next = NULL;
    timer->handle     = NULL;
    timer->left_arg   = NULL;
    timer->right_arg  = NULL;
    timer->function   = NULL;
    timer->beken_magic = 0;
    timer->time_ms    = 0;
    return kNoErr;
}

bk_err_t rtos_stop_oneshot_timer( beken2_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return kGeneralErr;
       
    }
    wd_cancel((WDOG_ID) timer->handle);
    return kNoErr;
}

bool rtos_is_oneshot_timer_init( beken2_timer_t* timer )
{
    return timer->handle ? true : false;
}

bool rtos_is_oneshot_timer_running( beken2_timer_t* timer )
{
    return WDOG_ISACTIVE((WDOG_ID)timer->handle);
}

bk_err_t rtos_oneshot_reload_timer( beken2_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return kGeneralErr;       
    }
    wd_cancel((WDOG_ID) timer->handle);

    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID) timer->handle, delay, once_timer_callback, 1, timer);  
    return kNoErr;
}

bk_err_t rtos_oneshot_reload_timer_ex(beken2_timer_t *timer,
										uint32_t time_ms,
										timer_2handler_t function,
										void *larg,
										void *rarg)
{
    bk_err_t ret;

	if (rtos_is_oneshot_timer_running(timer)) {
		ret = rtos_stop_oneshot_timer(timer);
		if (ret != kNoErr) {
			return ret;
		}
	}

	timer->function = function;
	timer->left_arg = larg;
	timer->right_arg = rarg;
    timer->time_ms = time_ms;
	ret = rtos_oneshot_reload_timer(timer);
	return ret;
}

bk_err_t rtos_init_oneshot_timer( beken2_timer_t *timer, 
									uint32_t time_ms, 
									timer_2handler_t function,
									void* larg, 
									void* rarg )
{
    bk_err_t ret = kNoErr;
	  
    timer->handle = (WDOG_ID) wd_create();

    if ( timer->handle == NULL )
    {
        return kGeneralErr;
    }
    timer->timer_next = (beken2_timer_t *)kmm_zalloc(sizeof(struct work_s));
    if (timer->timer_next == NULL)
    {
        wd_delete((WDOG_ID) timer->handle);
        timer->handle = NULL;
        return kGeneralErr;
    }
    timer->function = function;
    timer->left_arg = larg;
    timer->right_arg = rarg;	
	timer->beken_magic = BEKEN_MAGIC_WORD;
    timer->time_ms = time_ms;

    return ret;
}

bk_err_t rtos_init_timer( beken_timer_t *timer, 
                            uint32_t time_ms, 
                            timer_handler_t function, 
                            void* arg )
{
    bk_err_t ret = kNoErr;
   
    timer->handle = (WDOG_ID) wd_create();

    if ( timer->handle == NULL )
    {
        return kGeneralErr;
    }

    timer->timer_next = (beken_timer_t *)kmm_zalloc(sizeof(struct work_s));
    if (timer->timer_next == NULL)
    {
        wd_delete((WDOG_ID) timer->handle);
        timer->handle = NULL;
        return kGeneralErr;
    }

    timer->function = function;
    timer->arg      = arg;
    timer->time_ms  = time_ms;

    return ret;
}

bk_err_t rtos_start_timer( beken_timer_t* timer )
{
    if (timer->handle == NULL) 
    {
        return kGeneralErr;       
    }

    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID) timer->handle, delay, period_timer_callback, 1, timer);

    return kNoErr;
}

bk_err_t rtos_stop_timer( beken_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return kGeneralErr;       
    }
    wd_cancel((WDOG_ID) timer->handle);
    return kNoErr;
}

bk_err_t rtos_reload_timer( beken_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return kGeneralErr;       
    }
    wd_cancel((WDOG_ID) timer->handle);

    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID) timer->handle, delay, period_timer_callback, 1, timer);

    return kNoErr;
}

bk_err_t rtos_change_period( beken_timer_t* timer, uint32_t time_ms)
{
    if (timer->handle == NULL)
    {
        return kGeneralErr;      
    }
    wd_cancel((WDOG_ID) timer->handle);

    timer->time_ms = time_ms;

    int delay = MSEC2TICK(timer->time_ms);
    wd_start((WDOG_ID) timer->handle, delay, period_timer_callback, 1, timer);
    
    return kNoErr;
}

bk_err_t rtos_deinit_timer( beken_timer_t* timer )
{
    if (timer->handle == NULL) 
    {
        return kGeneralErr;       
    }

    wd_delete((WDOG_ID) timer->handle);
    kmm_free(timer->timer_next);
    timer->timer_next = NULL;
    timer->handle     = NULL;
    timer->arg        = NULL;
    timer->function   = NULL;
    return kNoErr;
}

uint32_t rtos_get_timer_expiry_time( beken_timer_t* timer )
{
    if (timer->handle == NULL) 
    {
        return kGeneralErr;      
    }
    return  wd_gettime(timer->handle);
}

bool rtos_is_timer_init( beken_timer_t* timer )
{
    return timer->handle ? true : false;
}

bool rtos_is_timer_running( beken_timer_t* timer )
{
    return WDOG_ISACTIVE((WDOG_ID)timer->handle);
}

bk_err_t rtos_init_event_flags( beken_event_t* event_flags )
{
    return -1;
}

beken_event_flags_t rtos_wait_for_event_flags( beken_event_t* event_flags, 
                                    uint32_t flags_to_wait_for, 
                                    beken_bool_t clear_set_flags, 
                                    beken_event_flags_wait_option_t wait_option, 
                                    uint32_t timeout_ms )
{
    return 1;
}

void rtos_set_event_flags( beken_event_t* event_flags, uint32_t flags_to_set )
{
   
}

bk_err_t rtos_deinit_event_flags( beken_event_t* event_flags )
{
    return -1;
}

uint32_t bk_get_tick(void)
{
    return (uint32_t)clock();
}

uint32_t bk_get_ms_per_tick(void)
{
    return ( 1000L / TICK_PER_SEC);
}

bk_err_t beken_time_get_time(beken_time_t* time_ptr)
{
    return -1;
}

bk_err_t beken_time_set_time(beken_time_t* time_ptr)
{
    return -1;
}

uint32_t beken_ms_per_tick(void)
{
    return bk_get_ms_per_tick();
}

uint32_t bk_get_ticks_per_second(void)
{
    return TICK_PER_SEC;
}

/**
 * Gets time in milliseconds since RTOS start
 *
 * @Note: since this is only 32 bits, it will roll over every 49 days, 17 hours.
 *
 * @returns Time in milliseconds since RTOS started.
 */
beken_time_t rtos_get_time( void )
{
    uint32_t sys_time = (uint32_t) clock();
    return (sys_time * 1000L  / TICK_PER_SEC);
}


uint32_t bk_get_second(void)
{
    uint32_t sys_time = (uint32_t) clock();
    return (sys_time / TICK_PER_SEC);
}

/**
 * Delay for a number of milliseconds
 *
 * Processing of this function depends on the minimum sleep
 * time resolution of the RTOS.
 * The current thread sleeps for the longest period possible which
 * is less than the delay required, then makes up the difference
 * with a tight loop
 *
 * @return bk_err_t : kNoErr if delay was successful
 *
 */
bk_err_t rtos_delay_milliseconds( uint32_t num_ms )
{
     usleep(num_ms * 1000);
     return kNoErr;
}

/*-----------------------------------------------------------*/

void rtos_start_scheduler(void)
{
//	s_is_started_scheduler = true;
}

bool rtos_is_scheduler_started(void)
{
	return 0;//s_is_started_scheduler;
}

char* rtos_get_name(void)
{
#define RTOS_NAME                     "Tizen"
	return RTOS_NAME;
}

char* rtos_get_version(void)
{
#define RTOS_VERSION                  "Unknown"
	return RTOS_VERSION;
}

size_t rtos_get_total_heap_size(void)
{
	return 0;
}

size_t rtos_get_free_heap_size(void)
{
	return 0;
}
size_t rtos_get_free_heap_size_by_index(int heap_index)
{
	struct mm_heap_s *heap;
	extern struct mm_heap_s g_kmmheap[];
	
	/* Check heap index validity */
#ifdef CONFIG_KMM_NHEAPS
	if (heap_index >= CONFIG_KMM_NHEAPS) {
		return 0;
	}
#endif
	
	/* Use g_kmmheap directly instead of kmm_get_heap_with_index to avoid BASE_HEAP issues */
	heap = &g_kmmheap[heap_index];
	
	/* Check if heap is initialized */
	if (heap->mm_heapsize == 0) {
		/* Fallback: if requested heap is not initialized, try heap 0 */
		if (heap_index != 0 && g_kmmheap[0].mm_heapsize > 0) {
			heap = &g_kmmheap[0];
		} else {
			return 0;
		}
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	return (heap->mm_heapsize - heap->total_alloc_size);
#else
	struct mallinfo info;
	if (mm_mallinfo(heap, &info) != OK) {
		return 0;
	}
	return info.fordblks;
#endif
}
size_t rtos_get_minimum_free_heap_size(void)
{
	return 0;
}

size_t rtos_get_psram_total_heap_size(void)
{
	return 0;
}

size_t rtos_get_psram_free_heap_size(void)
{
	return 0;
}

size_t rtos_get_psram_minimum_free_heap_size(void)
{
	return 0;
}

uint32_t rtos_enter_critical( void )
{
    return (uint32_t)enter_critical_section();  
}

void rtos_exit_critical(uint32_t int_level)
{
    leave_critical_section((irqstate_t)int_level);
}

// static volatile uint32_t ulCriticalNesting = 0xaaaaaaaaUL;
uint32_t rtos_disable_int(void)
{
	return irqsave();
}

void rtos_enable_int(uint32_t int_level)
{
	irqrestore(int_level);
}

void *beken_malloc(size_t xWantedSize)
{
	return (void *)kmm_malloc(xWantedSize);
}

void beken_free( void *pv )
{
	kmm_free(pv);
}

void *beken_realloc( void *pv, size_t xWantedSize )
{
	return os_realloc(pv, xWantedSize);
}



