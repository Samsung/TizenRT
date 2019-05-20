
#include <osdep_service.h>
#include <stdio.h>
#include <../arch/arm/src/imxrt/imxrt_config.h>
//#include <tcm_heap.h>
#include <drv_types.h>
/********************* os depended utilities ********************/

#ifndef USE_MUTEX_FOR_SPINLOCK
#define USE_MUTEX_FOR_SPINLOCK 1
#endif

//----- ------------------------------------------------------------------
// Misc Function
//----- ------------------------------------------------------------------

extern _list timer_table;

void timer_wrapper(_timerHandle timer_hdl)
{
	_list *plist;
	struct timer_entry *timer_entry = NULL;

	DBG_TRACE("%s is called", __FUNCTION__);
	//printf("\r\ntimer_wrapper is called \r\n");
	save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE)	
	{		
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		if(timer_entry->timer->timer_hdl == timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

	restore_flags();

	if(plist == &timer_table)
		DBG_ERR("Fail to find the timer_entry in timer table");
	else{
#if TIMEOUT_LEAK_DBG
		timer_entry->timeout = 0;
		DBG_TRACE("[%d]: TTTTimeout timer=%p\n", rtw_get_current_time(), timer_entry->timer);
#endif	
		if(timer_entry->timer->data){
			struct _ADAPTER *_adapter = (struct _ADAPTER *)(timer_entry->timer->data);
			if(_adapter->RxStop){
				DBG_871X("[timer_wrapper]:RxStop = %d\n",_adapter->RxStop);
				return;
			}
		}else{
			DBG_871X("[timer_wrapper]:Adapter = NULL\n");
			return;
		}
		
		timer_entry->timer->timevalue -= 1000;
		
		if(timer_entry->timer->timevalue > 0){
			if(rtw_timerChangePeriod(timer_entry->timer, rtw_ms_to_systime(1000), TIMER_MAX_DELAY) == _FAIL)
				DBG_ERR("Fail to set timer period");
		}else if(timer_entry->timer->function){
			//ndbg("\r\n==============>>timer->function = %x\r\n",timer_entry->timer->function);
			timer_entry->timer->timevalue = 0;
			timer_entry->timer->function((void *) timer_entry->timer->data);
		}
		
	}
}


irqstate_t flags;
void save_and_cli()
{
    flags = 0;
    flags = irqsave();
}


void restore_flags()
{
    irqrestore(flags);
}

void cli()
{

}

/* Not needed on 64bit architectures */
static unsigned int __div64_32(u64 *n, unsigned int base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	unsigned int high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high * base) << 32;
	}

	while ((u64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

/********************* os depended service ********************/

u8* _tizenrt_malloc(u32 sz)
{
	return kmm_zalloc(sz);
}
u8* _tizenrt_zmalloc(u32 sz)
{
	u8 *pbuf = _tizenrt_malloc(sz);

	if (pbuf != NULL)
		memset(pbuf, 0, sz);

	return pbuf;	
}


void _tizenrt_mfree(u8 *pbuf, u32 sz)
{
	kmm_free(pbuf);

}


static void _tizenrt_memcpy(void* dst, void* src, u32 sz)
{
	memcpy(dst, src, sz);
}


static int _tizenrt_memcmp(void *dst, void *src, u32 sz)
{
//under Linux/GNU/GLibc, the return value of memcmp for two same mem. chunk is 0
	if (!(memcmp(dst, src, sz)))
		return 1;

	return 0;
}


static void _tizenrt_memset(void *pbuf, int c, u32 sz)

{
	memset(pbuf, c, sz);
}


static void _tizenrt_init_sema(_sema *sema, int init_val)
{
    *sema=_tizenrt_zmalloc(sizeof(sem_t));
	sem_init(*sema,0, init_val);
}


static void _tizenrt_free_sema(_sema *sema)

{     
		sem_destroy(*sema);
		free(*sema);
}


static void _tizenrt_up_sema(_sema *sema)
{
		sem_post(*sema);
}


static void _tizenrt_up_sema_from_isr(_sema *sema)
{

}


static u32 _tizenrt_down_sema(_sema *sema, u32 timeout)
{
	 struct timespec ts;
	 clock_gettime(CLOCK_REALTIME, &ts);
	 ts.tv_sec+=timeout/1000;
	if (sem_timedwait(*sema,&ts)<0)
			return _FAIL;
	else
			return _SUCCESS;
}


static void _tizenrt_mutex_init(_mutex *pmutex)
{
      
       *pmutex=_tizenrt_zmalloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(*pmutex,NULL);

}


static void _tizenrt_mutex_free(_mutex *pmutex)
{
		pthread_mutex_destroy(*pmutex);
		free(*pmutex);
}


static void _tizenrt_mutex_get(_lock *plock)
{   int temp;	   
	temp=pthread_mutex_lock(*plock);
	if(temp!=0) DBG_ERR("_tizenrt_mutex_get %s failed!\n"); 

}


static int _tizenrt_mutex_get_timeout(_lock *plock, u32 timeout_ms)
{
	int temp;	   
	temp=pthread_mutex_lock(*plock);
	if(temp!=0) DBG_ERR("_tizenrt_mutex_get_timeout %s failed!\n"); 
	return temp;
}


static void _tizenrt_mutex_put(_lock *plock)
{

		
		pthread_mutex_unlock(*plock);
}



static void _tizenrt_enter_critical(_lock *plock, _irqL *pirqL)
{
    flags = 0;
    flags = irqsave();

}

static void _tizenrt_exit_critical(_lock *plock, _irqL *pirqL)
{
    irqrestore(flags);
}



static void _tizenrt_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{

}


static void _tizenrt_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{

}


static int _tizenrt_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{

	    int temp;	   
	temp=pthread_mutex_lock(*pmutex);
	if(temp!=0) DBG_ERR("_tizenrt_enter_critical_mutex %s failed!\n"); 
		return temp;

}


static void _tizenrt_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)

{

	pthread_mutex_unlock(*pmutex);

}


#if defined(CONFIG_PLATFORM_8195BHP)
#include "timer_api.h"
static gtimer_t tmp_timer_obj;
#endif
static void _tizenrt_cpu_lock(void)
{
#if defined(CONFIG_PLATFORM_8195BHP)
	__disable_irq();
	icache_disable();
	dcache_disable();
	
	gtimer_init(&tmp_timer_obj, 0xff);
	gtimer_reload(&tmp_timer_obj, 400*1000 );	// 4s
	gtimer_start(&tmp_timer_obj);
#endif
}

static void _tizenrt_cpu_unlock(void)
{
#if defined(CONFIG_PLATFORM_8195BHP)	
	int duration = (int)gtimer_read_us(&tmp_timer_obj)/1000;
	gtimer_deinit(&tmp_timer_obj);
	// compensate rtos tick
	vTaskIncTick(duration);
	dcache_enable();
	icache_enable();	
	icache_invalidate();
	__enable_irq();
#endif
}

static void _tizenrt_spinlock_init(_lock *plock)
{

    *plock=_tizenrt_zmalloc(sizeof(pthread_mutex_t));
	if(*plock==NULL) {printf("\r\n _tizenrt_spinlock_init failed \n");return;}
	pthread_mutex_init(*plock,NULL);


}

static void _tizenrt_spinlock_free(_lock *plock)
{
	pthread_mutex_destroy(*plock);
	free(*plock);


}


static void _tizenrt_spinlock(_lock *plock)

{

	int temp;	   
	temp=pthread_mutex_lock(*plock);
	if(temp!=0) DBG_ERR("_tizenrt_spinlock failed!\n");

}


static void _tizenrt_spinunlock(_lock *plock)
{

	pthread_mutex_unlock(*plock);

}


static void _tizenrt_spinlock_irqsave(_lock *plock, _irqL *irqL)

{

       int temp;
	    __enable_irq();
	temp=pthread_mutex_lock(*plock);
	if(temp!=0) DBG_ERR("_tizenrt_spinlock failed!\n");
       
		

}


static void _tizenrt_spinunlock_irqsave(_lock *plock, _irqL *irqL)

{

    
	pthread_mutex_unlock(*plock);
    __disable_irq();

}


static int _tizenrt_init_xqueue( _xqueue* queue, const char* name, u32 message_size, u32 number_of_messages )

{
    
    return 0;
}


static int _tizenrt_push_to_xqueue( _xqueue* queue, void* message, u32 timeout_ms )
{
	

    return 0;
}


static int _tizenrt_pop_from_xqueue( _xqueue* queue, void* message, u32 timeout_ms )

{
	

    return 0;
}


static int _tizenrt_deinit_xqueue( _xqueue* queue )

{
    
    return 0;
}


static u32 _tizenrt_get_current_time(void)

{

		return clock();

}


static u32 _tizenrt_systime_to_ms(u32 systime)
{

		return (systime*1000L/TICK_PER_SEC);

}


static u32 _tizenrt_systime_to_sec(u32 systime)
{

	return (systime/TICK_PER_SEC);

}

static u32 _tizenrt_ms_to_systime(u32 ms)
{

		return (ms*TICK_PER_SEC/1000L);

}


static u32 _tizenrt_sec_to_systime(u32 sec)
{

	return (sec*TICK_PER_SEC);

}


static void _tizenrt_msleep_os(int ms)
{

	usleep((unsigned int)ms*1000);

}


static void _tizenrt_usleep_os(int us)
{

	usleep((unsigned int)us);

}


static void _tizenrt_mdelay_os(int ms)
{

	up_mdelay((unsigned long)ms); 

}


static void _tizenrt_udelay_os(int us)
{

		up_udelay((unsigned long)us);

}


static void _tizenrt_yield_os(void)
{

	sched_yield();


}


static void _tizenrt_ATOMIC_SET(ATOMIC_T *v, int i)
{

	atomic_set(v,i);
}

static int _tizenrt_ATOMIC_READ(ATOMIC_T *v)
{
	return atomic_read(v);
}

static void _tizenrt_ATOMIC_ADD(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter += i;
	restore_flags();
}

static void _tizenrt_ATOMIC_SUB(ATOMIC_T *v, int i)
{
	save_and_cli();
	v->counter -= i;
	restore_flags();
}

static void _tizenrt_ATOMIC_INC(ATOMIC_T *v)
{
	_tizenrt_ATOMIC_ADD(v, 1);
}

static void _tizenrt_ATOMIC_DEC(ATOMIC_T *v)
{
	_tizenrt_ATOMIC_SUB(v, 1);
}

static int _tizenrt_ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _tizenrt_ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	int temp;

	save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags();

	return temp;
}

static int _tizenrt_ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return _tizenrt_ATOMIC_ADD_RETURN(v, 1);
}

static int _tizenrt_ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return _tizenrt_ATOMIC_SUB_RETURN(v, 1);
}

static u64 _tizenrt_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;

	if (((n) >> 32) == 0) {
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	}
	else
		__rem = __div64_32(&(n), __base);
	
	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _tizenrt_arc4random(void)
{

	u32 res =  clock();

	static unsigned long seed = 0xDEADB00B;

#if CONFIG_PLATFORM_8711B || defined(CONFIG_PLATFORM_8721D)
	if(random_seed){
		seed = random_seed;
		random_seed = 0;
	}
#endif

	seed = ((seed & 0x007F00FF) << 7) ^
	    ((seed & 0x0F80FF00) >> 8) ^ // be sure to stir those low bits
	    (res << 13) ^ (res >> 9);    // using the clock too!
	return (int)seed;
}

static int _tizenrt_get_random_bytes(void *buf, size_t len)
{
#if 1 //becuase of 4-byte align, we use the follow code style.
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *) buf;

	for(i = 0; i < count; i ++) {
		lp[i] = _tizenrt_arc4random();  
		len -= sizeof(unsigned int);
	}

	if(len > 0) {
		ranbuf = _tizenrt_arc4random();
		_tizenrt_memcpy(&lp[i], &ranbuf, len);
	}
	return 0;
#else
	unsigned long ranbuf, *lp;
	lp = (unsigned long *)buf;
	while (len > 0) {
		ranbuf = _tizenrt_arc4random();
		*lp++ = ranbuf; //this op need the pointer is 4Byte-align!
		len -= sizeof(ranbuf);
	}
	return 0;
#endif
}

static u32 _tizenrt_GetFreeHeapSize(void)

{
	return 0;
}
void *tcm_heap_malloc(int size);

typedef CODE int (*main_t)(int argc, char *argv[]);

static int _tizenrt_create_task(struct task_struct *ptask, const char *name,
	u32  stack_size, u32 priority, thread_func_t func, void *thctx)
{

	//printf("\r\n===============>Enter _tizenrt_create_task\r\n");
	//task_fun Task_fun=(task_fun)func;
	ptask->task_name = name;	
	ptask->task = priority;
	ptask->_rtw_task_info_s = task_create(name, priority, stack_size, (main_t)func, NULL);
	//ptask->_rtw_task_info_s=kernel_thread(name,priority,stack_size,(main_t)func,(char *const*)NULL);
	
	if(!ptask->_rtw_task_info_s)
	{
			 DBG_ERR("create the task %s failed!",name);
			 ptask->task = 0;
			 return _FAIL;
	 }
						
	return _SUCCESS;
}

	
static void _tizenrt_delete_task(struct task_struct *ptask)
{
	    int status = 0; 
        if (!ptask->_rtw_task_info_s){
			 DBG_ERR("_tizenrt_delete_task(): ptask is NULL!\n",ptask->task_name);
			 return;
		 }
		 status=task_delete(ptask->_rtw_task_info_s) ;//TODO
		 if(status != OK)
		 {
			DBG_ERR("delete the task failed!",ptask->task_name);
			return;
		 }
		 free(ptask);
		 return;
}

u32 _tizenrt_wake_task( struct task_struct *task)
{    
    sem_post(task->wakeup_sema);
	return _SUCCESS;	
}


static void _tizenrt_thread_enter(char *name)
{
	DBG_INFO("\n\rRTKTHREAD %s\n", name);
}

static void _tizenrt_thread_exit(void)
{
	DBG_INFO("\n\rRTKTHREAD exit %s\n", __FUNCTION__); 
		task_delete(0);
	
}

_timerHandle _tizenrt_timerCreate( _timer *timer, const signed char *pcTimerName, 
							  osdepTickType xTimerPeriodInTicks, 
							  u32 uxAutoReload, 
							  void * pvTimerID 
							 )
{              

	timer->work_hdl = (struct work_s *)rtw_zmalloc(sizeof(struct work_s));

	if(timer->work_hdl == NULL) {
		DBG_ERR("Fail to alloc timer->work_hdl");
		rtw_timerDelete(timer, TIMER_MAX_DELAY);
		timer->timer_hdl = NULL;
		return;
	}
	
	return timer->work_hdl;
}

u32 _tizenrt_timerDelete( _timer *timer, 
							   osdepTickType xBlockTime )
{
#if 0
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock, (void *)&(timer->mutex));
	pthread_mutex_lock(&timer->mutex);
	timer->live=0;
	timer->timer_hdl = NULL;
	free(timer);			
	pthread_mutex_unlock(&timer->mutex);
	pthread_cleanup_pop( 1 );
#endif
	int ret;
	ret = work_cancel(LPWORK, timer->work_hdl);
	TC_ASSERT_EQ_CLEANUP("work_cancel", ret, OK, goto cleanup);
	free(timer->work_hdl);
	timer->timer_hdl = NULL;
	timer->timevalue = 0;
	return _SUCCESS;

cleanup:
	if(ret != -2){
		free(timer->work_hdl);
		DBG_ERR("_tizenrt_del_timer failed! ret = %d",ret);
		return _FAIL;
	}
	timer->timer_hdl = NULL;
	timer->timevalue = 0;
	DBG_ERR("_tizenrt_del_timer is Done! timer->work_hdl = %x",timer->work_hdl);
	return _SUCCESS;
}

u32 _tizenrt_timerIsTimerActive( _timer *timer)
{
		
	return timer->live;
          
}

u32  _tizenrt_timerStop( _timer *timer, 
							   osdepTickType xBlockTime )
{
	int ret;
	//timer->live=0;
	ret = work_cancel(LPWORK, timer->work_hdl);
	TC_ASSERT_EQ_CLEANUP("work_cancel", ret, OK, goto cleanup);
	timer->timevalue = 0;
	return _SUCCESS;

cleanup:
	if(ret != -2){
		free(timer->work_hdl);
		DBG_ERR("_tizenrt_stop_timer failed! ret = %d",ret);
		return _FAIL;
	}
	timer->timevalue = 0;
	DBG_ERR("_tizenrt_stop_timer is Done! timer->work_hdl = %x",timer->work_hdl);
	return _SUCCESS;

}

void *_tizenrt_timerGetID( _timerHandle timer_hdl ){

	return (int)timer_hdl;
}

u32  _tizenrt_timerStart( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return 0;	
}

u32  _tizenrt_timerStartFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return 0;	
}

u32  _tizenrt_timerStopFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return 0;	
}

u32  _tizenrt_timerResetFromISR( _timerHandle xTimer, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	return 0;	
}

u32  _tizenrt_timerChangePeriod( _timer *timer, 
							   osdepTickType xNewPeriod, 
							   osdepTickType xBlockTime )
{
	int ret;

	//ndbg("\r\n%s():================>work_hdl = %x\r\n",__func__,timer->work_hdl);

	ret = work_queue(LPWORK, timer->work_hdl, timer_wrapper, (void *)(timer->timer_hdl), xNewPeriod);
	if(ret == -EALREADY){
		ret = work_cancel(LPWORK, timer->work_hdl);
		TC_ASSERT_EQ_CLEANUP("work_cancel", ret, OK, goto cleanup);
		ret = work_queue(LPWORK, timer->work_hdl, timer_wrapper, (void *)(timer->timer_hdl), xNewPeriod);
		TC_ASSERT_EQ_CLEANUP("work_queue", ret, OK, goto cleanup);
	}
	
	//timer->live = 1;

	return _SUCCESS;

cleanup:

	free(timer->work_hdl);
	timer->timer_hdl = NULL;
	DBG_ERR("_tizenrt_set_timer failed!");
	return _FAIL;
	
}

u32  _tizenrt_timerChangePeriodFromISR( _timerHandle xTimer, 
							   osdepTickType xNewPeriod, 
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken )
{
	
	return 0;
}

u32  _tizenrt_timerReset( _timerHandle xTimer, 
							   osdepTickType xBlockTime )
{
	return 0;	
}

void _tizenrt_acquire_wakelock()
{

}

void _tizenrt_release_wakelock()
{


}

void _tizenrt_wakelock_timeout(uint32_t timeout)
{

}

u8 _tizenrt_get_scheduler_state(void)
{
	
	return 0;
}


const struct osdep_service_ops osdep_service = {
	_tizenrt_malloc,			//rtw_vmalloc
	_tizenrt_zmalloc,			//rtw_zvmalloc
	_tizenrt_mfree,			//rtw_vmfree
	_tizenrt_malloc,			//rtw_malloc
	_tizenrt_zmalloc,			//rtw_zmalloc
	_tizenrt_mfree,			//rtw_mfree
	_tizenrt_memcpy,			//rtw_memcpy
	_tizenrt_memcmp,			//rtw_memcmp
	_tizenrt_memset,			//rtw_memset
	_tizenrt_init_sema,		//rtw_init_sema
	_tizenrt_free_sema,		//rtw_free_sema
	_tizenrt_up_sema,			//rtw_up_sema
	_tizenrt_up_sema_from_isr,	//rtw_up_sema_from_isr
	_tizenrt_down_sema,		//rtw_down_sema
	_tizenrt_mutex_init,		//rtw_mutex_init
	_tizenrt_mutex_free,		//rtw_mutex_free
	_tizenrt_mutex_get,		//rtw_mutex_get
	_tizenrt_mutex_get_timeout,//rtw_mutex_get_timeout
	_tizenrt_mutex_put,		//rtw_mutex_put
	_tizenrt_enter_critical,	//rtw_enter_critical
	_tizenrt_exit_critical,	//rtw_exit_critical
	_tizenrt_enter_critical_from_isr,	//rtw_enter_critical_from_isr
	_tizenrt_exit_critical_from_isr,	//rtw_exit_critical_from_isr
	NULL,		//rtw_enter_critical_bh
	NULL,		//rtw_exit_critical_bh
	_tizenrt_enter_critical_mutex,	//rtw_enter_critical_mutex
	_tizenrt_exit_critical_mutex,	//rtw_exit_critical_mutex
	NULL,
	NULL,
	_tizenrt_spinlock_init,		//rtw_spinlock_init
	_tizenrt_spinlock_free,		//rtw_spinlock_free
	_tizenrt_spinlock,				//rtw_spin_lock
	_tizenrt_spinunlock,			//rtw_spin_unlock
	_tizenrt_spinlock_irqsave,		//rtw_spinlock_irqsave
	_tizenrt_spinunlock_irqsave,	//rtw_spinunlock_irqsave
	_tizenrt_init_xqueue,			//rtw_init_xqueue
	_tizenrt_push_to_xqueue,		//rtw_push_to_xqueue
	_tizenrt_pop_from_xqueue,		//rtw_pop_from_xqueue
	_tizenrt_deinit_xqueue,		//rtw_deinit_xqueue
	_tizenrt_get_current_time,		//rtw_get_current_time
	_tizenrt_systime_to_ms,		//rtw_systime_to_ms
	_tizenrt_systime_to_sec,		//rtw_systime_to_sec
	_tizenrt_ms_to_systime,		//rtw_ms_to_systime
	_tizenrt_sec_to_systime,		//rtw_sec_to_systime
	_tizenrt_msleep_os,	//rtw_msleep_os
	_tizenrt_usleep_os,	//rtw_usleep_os
	_tizenrt_mdelay_os,	//rtw_mdelay_os
	_tizenrt_udelay_os,	//rtw_udelay_os
	_tizenrt_yield_os,		//rtw_yield_os
	
	_tizenrt_ATOMIC_SET,	//ATOMIC_SET
	_tizenrt_ATOMIC_READ,	//ATOMIC_READ
	_tizenrt_ATOMIC_ADD,	//ATOMIC_ADD
	_tizenrt_ATOMIC_SUB,	//ATOMIC_SUB
	_tizenrt_ATOMIC_INC,	//ATOMIC_INC
	_tizenrt_ATOMIC_DEC,	//ATOMIC_DEC
	_tizenrt_ATOMIC_ADD_RETURN,	//ATOMIC_ADD_RETURN
	_tizenrt_ATOMIC_SUB_RETURN,	//ATOMIC_SUB_RETURN
	_tizenrt_ATOMIC_INC_RETURN,	//ATOMIC_INC_RETURN
	_tizenrt_ATOMIC_DEC_RETURN,	//ATOMIC_DEC_RETURN

	_tizenrt_modular64,			//rtw_modular64
	_tizenrt_get_random_bytes,		//rtw_get_random_bytes
	_tizenrt_GetFreeHeapSize,		//rtw_getFreeHeapSize

	_tizenrt_create_task,			//rtw_create_task
	_tizenrt_delete_task,			//rtw_delete_task
	_tizenrt_wake_task,				//rtw_wakeup_task

	_tizenrt_thread_enter,			//rtw_thread_enter
	_tizenrt_thread_exit,			//rtw_thread_exit

	_tizenrt_timerCreate,			//rtw_timerCreate,       
	_tizenrt_timerDelete,			//rtw_timerDelete,       
	_tizenrt_timerIsTimerActive,	//rtw_timerIsTimerActive,
	_tizenrt_timerStop,			//rtw_timerStop,         
	_tizenrt_timerChangePeriod,	//rtw_timerChangePeriod 
	_tizenrt_timerGetID,			//rtw_timerGetID
	_tizenrt_timerStart,			//rtw_timerStart
	_tizenrt_timerStartFromISR,	//rtw_timerStartFromISR
	_tizenrt_timerStopFromISR,		//rtw_timerStopFromISR
	_tizenrt_timerResetFromISR,	//rtw_timerResetFromISR
	_tizenrt_timerChangePeriodFromISR,	//rtw_timerChangePeriodFromISR
	_tizenrt_timerReset,			//rtw_timerReset

	_tizenrt_acquire_wakelock,		//rtw_acquire_wakelock
	_tizenrt_release_wakelock,		//rtw_release_wakelock
	_tizenrt_wakelock_timeout,		//rtw_wakelock_timeout
	_tizenrt_get_scheduler_state	//rtw_get_scheduler_state
};
