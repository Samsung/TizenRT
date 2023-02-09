/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include <osdep_service.h>
#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP
#include "tcm_heap.h"
#endif

#define OSDEP_DBG(x, ...) do {} while(0)

extern struct osdep_service_ops osdep_service;

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

int max_mutex_num = MAX_MUTEX_BUF_NUM;
struct mutex_buf {
	struct list_head list;
	StaticSemaphore_t mutex;
};

struct mutex_buf mutex_pool[MAX_MUTEX_BUF_NUM];
static struct list_head wrapper_mutexbuf_list;
int mutexbuf_used_num, mutex_dynamic_num;
int max_mutexbuf_used_num;
int mutexpool_flag = 0;

static __inline__ void *get_mutex_from_poll(struct list_head *phead, int *count)
{
	StaticSemaphore_t *mutex;
	struct list_head *plist;

	if (mutexpool_flag == 0) {
		return NULL;
	}

	save_and_cli();
	if (list_empty(phead)) {
		restore_flags();
		return NULL;
	}

	plist = phead->next;
	list_del_init(plist);

	mutex = (StaticSemaphore_t *)((unsigned int)plist + sizeof(struct list_head));

	*count = *count + 1;

	restore_flags();

	return (unsigned char *)mutex;
}

static void release_mutex_to_poll(unsigned char *buf, struct list_head *phead, int *count)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)buf) - sizeof(struct list_head));
	list_add_tail(plist, phead);

	*count = *count - 1;
}

static u8 rtw_get_mutex_dynamic_flag(_sema sema)
{
	StaticSemaphore_t *pqueue = (StaticSemaphore_t *)sema;
	return pqueue->ucDummy6;
}

#endif

void init_mutex_pool(void)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	int i;

	memset(mutex_pool, '\0', max_mutex_num * sizeof(struct mutex_buf));
	INIT_LIST_HEAD(&wrapper_mutexbuf_list);

	for (i = 0; i < max_mutex_num; i++) {
		INIT_LIST_HEAD(&mutex_pool[i].list);
		list_add_tail(&mutex_pool[i].list, &wrapper_mutexbuf_list);
	}

	mutexbuf_used_num = 0;
	max_mutexbuf_used_num = 0;
	mutex_dynamic_num = 0;

	mutexpool_flag = 1;
#endif
}

u16
_htons(u16 n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

u16
_ntohs(u16 n)
{
	return _htons(n);
}

u32
_htonl(u32 n)
{
	return ((n & 0xff) << 24) |
		   ((n & 0xff00) << 8) |
		   ((n & 0xff0000UL) >> 8) |
		   ((n & 0xff000000UL) >> 24);
}

u32
_ntohl(u32 n)
{
	return _htonl(n);
}

/*
* Translate the OS dependent @param error_code to OS independent RTW_STATUS_CODE
* @return: one of RTW_STATUS_CODE
*/
int RTW_STATUS_CODE(int error_code)
{
	if (error_code >= 0) {
		return _SUCCESS;
	}

	return _FAIL;
}

u32 rtw_atoi(u8 *s)
{
	int num = 0, flag = 0;
	u32 i;

	for (i = 0; i <= strlen((char *)s); i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			num = num * 10 + s[i] - '0';
		} else if (s[0] == '-' && i == 0) {
			flag = 1;
		} else {
			break;
		}
	}

	if (flag == 1) {
		num = num * -1;
	}

	return (num);
}
#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP
void *tcm_heap_malloc(int size);
void *tcm_heap_calloc(int size);
#endif
void *_rtw_vmalloc(u32 sz)
{
	void *pbuf = NULL;
#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP
	pbuf = tcm_heap_malloc(sz);
#endif
	if (pbuf == NULL) {
		if (osdep_service.rtw_vmalloc) {
			pbuf = osdep_service.rtw_vmalloc(sz);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_vmalloc");
		}
	}
	return pbuf;
}

void *_rtw_zvmalloc(u32 sz)
{
	void *pbuf = NULL;
#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP
	pbuf = tcm_heap_calloc(sz);
#endif
	if (pbuf == NULL) {
		if (osdep_service.rtw_zvmalloc) {
			pbuf = osdep_service.rtw_zvmalloc(sz);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_zvmalloc");
		}
	}
	return pbuf;
}

void _rtw_vmfree(u8 *pbuf, u32 sz)
{

#if defined(CONFIG_USE_TCM_HEAP) && CONFIG_USE_TCM_HEAP
	if ((u32)pbuf > 0x1FFF0000 && (u32)pbuf < 0x20000000) {
		tcm_heap_free(pbuf);
	} else
#endif
	{
		if (osdep_service.rtw_vmfree) {
			osdep_service.rtw_vmfree(pbuf, sz);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_vmfree");
		}
	}
}

void *_rtw_malloc(u32 sz)
{
	if (osdep_service.rtw_malloc) {
		void *pbuf = osdep_service.rtw_malloc(sz);
		return pbuf;
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_malloc");
	}

	return NULL;
}

void *_rtw_zmalloc(u32 sz)
{
	if (osdep_service.rtw_zmalloc) {
		void *pbuf = osdep_service.rtw_zmalloc(sz);
		return pbuf;
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_zmalloc");
	}

	return NULL;
}

void *_rtw_calloc(u32 nelements, u32 elementSize)
{
	u32 sz = nelements * elementSize;
	void *pbuf = _rtw_zmalloc(sz);
	return pbuf;
}

void _rtw_mfree(u8 *pbuf, u32 sz)
{
	if (osdep_service.rtw_mfree) {
		osdep_service.rtw_mfree(pbuf, sz);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mfree");
	}
}

void *rtw_vmalloc(u32 sz)
{
	void *pbuf = _rtw_vmalloc(sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void *rtw_zvmalloc(u32 sz)
{
	void *pbuf = _rtw_zvmalloc(sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void rtw_vmfree(u8 *pbuf, u32 sz)
{
	_rtw_vmfree(pbuf, sz);
}

void *rtw_malloc(u32 sz)
{
	void *pbuf = _rtw_malloc(sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void *rtw_zmalloc(u32 sz)
{
	void *pbuf = _rtw_zmalloc(sz);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void *rtw_calloc(u32 nelements, u32 elementSize)
{
	void *pbuf = _rtw_calloc(nelements, elementSize);
#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void rtw_mfree(u8 *pbuf, u32 sz)
{
	_rtw_mfree(pbuf, sz);
}

void *rtw_malloc2d(int h, int w, int size)
{
	int j;

	void **a = (void **) rtw_zmalloc(h * sizeof(void *) + h * w * size);
	if (a == NULL) {
		OSDEP_DBG("%s: alloc memory fail!\n", __FUNCTION__);
		return NULL;
	}

	for (j = 0; j < h; j++) {
		a[j] = ((char *)(a + h)) + j * w * size;
	}

	return a;
}

void rtw_mfree2d(void *pbuf, int h, int w, int size)
{
	rtw_mfree((u8 *)pbuf, h * sizeof(void *) + w * h * size);
}

void rtw_memcpy(void *dst, void *src, u32 sz)
{
	if (osdep_service.rtw_memcpy) {
		osdep_service.rtw_memcpy(dst, src, sz);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_memcpy");
	}
}

int rtw_memcmp(void *dst, void *src, u32 sz)
{
	if (osdep_service.rtw_memcmp) {
		return osdep_service.rtw_memcmp(dst, src, sz);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_memcmp");
	}

	return _FALSE;
}

void rtw_memset(void *pbuf, int c, u32 sz)
{
	if (osdep_service.rtw_memset) {
		osdep_service.rtw_memset(pbuf, c, sz);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_memset");
	}
}

void rtw_init_listhead(_list *list)
{
	INIT_LIST_HEAD(list);
}

/*
For the following list_xxx operations,
caller must guarantee the atomic context.
Otherwise, there will be racing condition.
*/
u32 rtw_is_list_empty(_list *phead)
{
	if (list_empty(phead)) {
		return _TRUE;
	}

	return _FALSE;
}

void rtw_list_insert_head(_list *plist, _list *phead)
{
	list_add(plist, phead);
}

void rtw_list_insert_tail(_list *plist, _list *phead)
{
	list_add_tail(plist, phead);
}

/*

Caller must check if the list is empty before calling rtw_list_delete

*/
void rtw_list_delete(_list *plist)
{
	list_del_init(plist);
}



void rtw_init_sema(_sema *sema, int init_val)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	StaticSemaphore_t *mutex;

	mutex = (StaticSemaphore_t *)get_mutex_from_poll(&wrapper_mutexbuf_list, &mutexbuf_used_num);

	if (mutex == NULL) {
		if (mutexpool_flag) {
			mutex_dynamic_num++;
			printf("static mutex is not avaliable. mutexbuf_used_num: %d\n", mutexbuf_used_num);
		}
		goto exit;
	} else {
		memset(mutex, '\0', sizeof(*mutex));
		if (osdep_service.rtw_init_sema_static) {
			osdep_service.rtw_init_sema_static(sema, init_val, mutex);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_init_sema_static");
		}
	}

	if (mutexbuf_used_num > max_mutexbuf_used_num) {
		max_mutexbuf_used_num = mutexbuf_used_num;
	}

	return;
exit:
#endif

	if (osdep_service.rtw_init_sema) {
		osdep_service.rtw_init_sema(sema, init_val);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_init_sema");
	}
}

void rtw_free_sema(_sema *sema)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	if ((*sema) == NULL) {
		return;
	}
	if (rtw_get_mutex_dynamic_flag(*sema)) {
		save_and_cli();
		release_mutex_to_poll((unsigned char *)*sema, &wrapper_mutexbuf_list, &mutexbuf_used_num);
		restore_flags();
	} else {
		if (mutexpool_flag) {
			mutex_dynamic_num--;
		}
	}
#endif

	if (osdep_service.rtw_free_sema) {
		osdep_service.rtw_free_sema(sema);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_free_sema");
	}
}


//#if defined(__GNUC__)
#include <cmsis.h>
//#endif
#if defined(__GNUC__) && defined(CONFIG_PLATFORM_8735B)
#include <cmsis_gcc.h>	// for __get_xPSR()
#endif

int rtw_in_interrupt(void)
{
#ifdef ARM_CORE_CA32
	return (__get_mode() != CPSR_M_USR) && (__get_mode() != CPSR_M_SYS);
#else
#if defined(__ICCARM__)
	return (__get_PSR() & 0x1FF) != 0;
#elif defined(__GNUC__)

#ifdef ARM_CORE_CM4
	return (__get_xPSR() & 0x1FF) != 0;
#elif defined(RSICV_CORE_KR4)
	return plic_get_active_irq_id_ram() != 0;
#else
	return __get_IPSR() != 0;
#endif

#endif
#endif
}

void rtw_up_sema(_sema *sema)
{
	if (rtw_in_interrupt()) {
		if (osdep_service.rtw_up_sema_from_isr) {
			osdep_service.rtw_up_sema_from_isr(sema);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_up_sema_from_isr");
		}
	} else {
		if (osdep_service.rtw_up_sema) {
			osdep_service.rtw_up_sema(sema);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_up_sema");
		}
	}
}

void rtw_up_sema_from_isr(_sema *sema)
{
	if (osdep_service.rtw_up_sema_from_isr) {
		osdep_service.rtw_up_sema_from_isr(sema);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_up_sema_from_isr");
	}
}

u32	rtw_down_timeout_sema(_sema *sema, u32 timeout)
{
	if (osdep_service.rtw_down_timeout_sema) {
		return osdep_service.rtw_down_timeout_sema(sema, timeout);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_down_timeout_sema");
	}

	return _FAIL;
}

u32 rtw_down_sema(_sema *sema)
{
	while (rtw_down_timeout_sema(sema, RTW_MAX_DELAY) != _TRUE)
//		rom_e_rtw_msg_871X_LEVEL(DOWN_SEMA_1, _drv_always_, "%s(%p) failed, retry\n",  __FUNCTION__, sema);
		//OSDEP_DBG_LEVEL(_drv_always_, "%s(%p) failed, retry\n",  __FUNCTION__, sema);
	{
		OSDEP_DBG("%s(%p) failed, retry\n",  __FUNCTION__, sema);
	}
	return _TRUE;
}

void rtw_mutex_init(_mutex *pmutex)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	StaticSemaphore_t *mutex;

	mutex = (StaticSemaphore_t *)get_mutex_from_poll(&wrapper_mutexbuf_list, &mutexbuf_used_num);

	if (mutex == NULL) {
		if (mutexpool_flag) {
			mutex_dynamic_num++;
			printf("static mutex is not avaliable. mutexbuf_used_num: %d\n", mutexbuf_used_num);
		}
		goto exit;
	} else {
		memset(mutex, '\0', sizeof(*mutex));
		if (osdep_service.rtw_mutex_init_static) {
			osdep_service.rtw_mutex_init_static(pmutex, mutex);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_mutex_init");
		}
	}

	if (mutexbuf_used_num > max_mutexbuf_used_num) {
		max_mutexbuf_used_num = mutexbuf_used_num;
	}

	return;
exit:
#endif

	if (osdep_service.rtw_mutex_init) {
		osdep_service.rtw_mutex_init(pmutex);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mutex_init");
	}
}

void rtw_mutex_free(_mutex *pmutex)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	if ((*pmutex) == NULL) {
		return;
	}
	if (rtw_get_mutex_dynamic_flag(*pmutex)) {
		save_and_cli();
		release_mutex_to_poll((unsigned char *)*pmutex, &wrapper_mutexbuf_list, &mutexbuf_used_num);
		restore_flags();
	} else {
		if (mutexpool_flag) {
			mutex_dynamic_num--;
		}
	}
#endif

	if (osdep_service.rtw_mutex_free) {
		osdep_service.rtw_mutex_free(pmutex);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mutex_free");
	}
}

void rtw_mutex_put(_mutex *pmutex)
{
	if (osdep_service.rtw_mutex_put) {
		osdep_service.rtw_mutex_put(pmutex);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mutex_put");
	}
}

void rtw_mutex_get(_mutex *pmutex)
{
	if (osdep_service.rtw_mutex_get) {
		osdep_service.rtw_mutex_get(pmutex);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mutex_get");
	}
}

int rtw_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
	if (osdep_service.rtw_mutex_get_timeout) {
		return osdep_service.rtw_mutex_get_timeout(pmutex, timeout_ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mutex_get_timeout");
		return -1;
	}
}

void rtw_enter_critical(_lock *plock, _irqL *pirqL)
{
	if (rtw_in_interrupt()) {
		if (osdep_service.rtw_enter_critical_from_isr) {
			osdep_service.rtw_enter_critical_from_isr(plock, pirqL);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_enter_critical_from_isr");
		}
	} else {
		if (osdep_service.rtw_enter_critical) {
			osdep_service.rtw_enter_critical(plock, pirqL);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_enter_critical");
		}
	}
}

void rtw_exit_critical(_lock *plock, _irqL *pirqL)
{
	if (rtw_in_interrupt()) {
		if (osdep_service.rtw_exit_critical_from_isr) {
			osdep_service.rtw_exit_critical_from_isr(plock, pirqL);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_exit_critical_from_isr");
		}
	} else {
		if (osdep_service.rtw_exit_critical) {
			osdep_service.rtw_exit_critical(plock, pirqL);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_exit_critical");
		}
	}
}

void rtw_enter_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	if (osdep_service.rtw_enter_critical) {
		osdep_service.rtw_enter_critical(plock, pirqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_enter_critical_from_isr");
	}
}

void rtw_exit_critical_from_isr(_lock *plock, _irqL *pirqL)
{
	if (osdep_service.rtw_exit_critical) {
		osdep_service.rtw_exit_critical(plock, pirqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_exit_critical_from_isr");
	}
}

void rtw_enter_critical_bh(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;
	rtw_spin_lock(plock);
}

void rtw_exit_critical_bh(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;

	rtw_spin_unlock(plock);
}

int rtw_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	if (osdep_service.rtw_enter_critical_mutex) {
		return osdep_service.rtw_enter_critical_mutex(pmutex, pirqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_enter_critical_mutex");
	}

	return 0;
}

void rtw_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	if (osdep_service.rtw_exit_critical_mutex) {
		osdep_service.rtw_exit_critical_mutex(pmutex, pirqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_exit_critical_mutex");
	}
}

void rtw_cpu_lock(void)
{
	if (osdep_service.rtw_cpu_lock) {
		osdep_service.rtw_cpu_lock();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_cpu_lock");
	}
}

void rtw_cpu_unlock(void)
{
	if (osdep_service.rtw_cpu_unlock) {
		osdep_service.rtw_cpu_unlock();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_cpu_unlock");
	}
}

void	rtw_init_queue(_queue	*pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_spinlock_init(&(pqueue->lock));
}

u32	  rtw_queue_empty(_queue	*pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}


u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

#if 1
void rtw_spinlock_init(_lock *plock)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	StaticSemaphore_t *mutex;

	mutex = (StaticSemaphore_t *)get_mutex_from_poll(&wrapper_mutexbuf_list, &mutexbuf_used_num);

	if (mutex == NULL) {
		if (mutexpool_flag) {
			mutex_dynamic_num++;
			printf("static mutex is not avaliable. mutexbuf_used_num: %d\n", mutexbuf_used_num);
		}
		goto exit;
	} else {
		memset(mutex, '\0', sizeof(*mutex));
		if (osdep_service.rtw_spinlock_init_static) {
			osdep_service.rtw_spinlock_init_static(plock, mutex);
		} else {
			OSDEP_DBG("Not implement osdep service: rtw_mutex_init");
		}
	}

	if (mutexbuf_used_num > max_mutexbuf_used_num) {
		max_mutexbuf_used_num = mutexbuf_used_num;
	}

	return;
exit:
#endif

	if (osdep_service.rtw_spinlock_init) {
		osdep_service.rtw_spinlock_init(plock);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_init");
	}
}


void rtw_spinlock_free(_lock *plock)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	if ((*plock) == NULL) {
		return;
	}
	if (rtw_get_mutex_dynamic_flag(*plock)) {
		save_and_cli();
		release_mutex_to_poll((unsigned char *)*plock, &wrapper_mutexbuf_list, &mutexbuf_used_num);
		restore_flags();
	} else {
		if (mutexpool_flag) {
			mutex_dynamic_num--;
		}
	}
#endif

	if (osdep_service.rtw_spinlock_free) {
		osdep_service.rtw_spinlock_free(plock);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_free");
	}
}

void rtw_spin_lock(_lock *plock)
{
	if (osdep_service.rtw_spin_lock) {
		osdep_service.rtw_spin_lock(plock);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spin_lock");
	}
}

void rtw_spin_unlock(_lock *plock)
{
	if (osdep_service.rtw_spin_unlock) {
		osdep_service.rtw_spin_unlock(plock);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spin_unlock");
	}
}

void rtw_spinlock_irqsave(_lock *plock, _irqL *irqL)
{
	if (osdep_service.rtw_spinlock_irqsave) {
		osdep_service.rtw_spinlock_irqsave(plock, irqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spinlock_irqsave");
	}
}

void rtw_spinunlock_irqsave(_lock *plock, _irqL *irqL)
{
	if (osdep_service.rtw_spinunlock_irqsave) {
		osdep_service.rtw_spinunlock_irqsave(plock, irqL);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_spinunlock_irqsave");
	}
}
#endif

int rtw_init_xqueue(_xqueue *queue, const char *name, u32 message_size, u32 number_of_messages)
{
	if (osdep_service.rtw_init_xqueue) {
		return (int)osdep_service.rtw_init_xqueue(queue, name, message_size, number_of_messages);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_init_xqueue");
	}

	return FAIL;
}

int rtw_push_to_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	if (osdep_service.rtw_push_to_xqueue) {
		return (int)osdep_service.rtw_push_to_xqueue(queue, message, timeout_ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_push_to_xqueue");
	}

	return FAIL;
}

int rtw_pop_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	if (osdep_service.rtw_pop_from_xqueue) {
		return (int)osdep_service.rtw_pop_from_xqueue(queue, message, timeout_ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_pop_from_xqueue");
	}

	return FAIL;
}

int rtw_peek_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	if (osdep_service.rtw_peek_from_xqueue) {
		return (int)osdep_service.rtw_peek_from_xqueue(queue, message, timeout_ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_peek_from_xqueue");
	}

	return FAIL;
}

int rtw_deinit_xqueue(_xqueue *queue)
{
	if (osdep_service.rtw_deinit_xqueue) {
		return (int)osdep_service.rtw_deinit_xqueue(queue);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_deinit_xqueue");
	}

	return FAIL;
}

#if 0
void rtw_init_queue(_queue *pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_mutex_init(&(pqueue->lock));
}

void	rtw_deinit_queue(_queue *pqueue)
{
	rtw_mutex_free(&(pqueue->lock));
}

u32 rtw_is_queue_empty(_queue *pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}

u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist) {
		return _TRUE;
	}

	return _FALSE;
}

_list	*rtw_get_queue_head(_queue	*queue)
{
	return (&(queue->queue));
}
#endif

uint32_t rtw_get_current_time(void)
{
	if (osdep_service.rtw_get_current_time) {
		return osdep_service.rtw_get_current_time();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_get_current_time");
	}

	return 0;
}

u32 rtw_systime_to_ms(u32 systimer)
{
	if (osdep_service.rtw_systime_to_ms) {
		return osdep_service.rtw_systime_to_ms(systimer);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_systime_to_ms");
	}

	return 0;
}

u32 rtw_systime_to_sec(u32 systimer)
{
	if (osdep_service.rtw_systime_to_sec) {
		return osdep_service.rtw_systime_to_sec(systimer);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_systime_to_sec");
	}

	return 0;
}

u32 rtw_ms_to_systime(u32 ms)
{
	if (osdep_service.rtw_ms_to_systime) {
		return osdep_service.rtw_ms_to_systime(ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_ms_to_systime");
	}

	return 0;
}

u32 rtw_sec_to_systime(u32 sec)
{
	if (osdep_service.rtw_sec_to_systime) {
		return osdep_service.rtw_sec_to_systime(sec);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_sec_to_systime");
	}

	return 0;
}

// the input parameter start use the same unit as returned by rtw_get_current_time
s32 rtw_get_passing_time_ms(u32 start)
{
	return rtw_systime_to_ms(rtw_get_current_time() - start);
}

s32 rtw_get_time_interval_ms(u32 start, u32 end)
{
	return rtw_systime_to_ms(end - start);
}

void rtw_msleep_os(int ms)
{
	if (osdep_service.rtw_msleep_os) {
		osdep_service.rtw_msleep_os(ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_msleep_os");
	}
}

void rtw_usleep_os(int us)
{
	if (osdep_service.rtw_usleep_os) {
		osdep_service.rtw_usleep_os(us);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_usleep_os");
	}
}

void rtw_mdelay_os(int ms)
{
	if (osdep_service.rtw_mdelay_os) {
		osdep_service.rtw_mdelay_os(ms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_mdelay_os");
	}
}

void rtw_udelay_os(int us)
{
	if (osdep_service.rtw_udelay_os) {
		osdep_service.rtw_udelay_os(us);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_udelay_os");
	}
}

void rtw_yield_os(void)
{
	if (osdep_service.rtw_yield_os) {
		osdep_service.rtw_yield_os();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_yield_os");
	}
}

void ATOMIC_SET(ATOMIC_T *v, int i)
{
	if (osdep_service.ATOMIC_SET) {
		osdep_service.ATOMIC_SET(v, i);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_SET");
	}
}

int ATOMIC_READ(ATOMIC_T *v)
{
	if (osdep_service.ATOMIC_READ) {
		return osdep_service.ATOMIC_READ(v);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_READ");
	}

	return 0;
}

void ATOMIC_ADD(ATOMIC_T *v, int i)
{
	if (osdep_service.ATOMIC_ADD) {
		osdep_service.ATOMIC_ADD(v, i);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_ADD");
	}
}

void ATOMIC_SUB(ATOMIC_T *v, int i)
{
	if (osdep_service.ATOMIC_SUB) {
		osdep_service.ATOMIC_SUB(v, i);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_SUB");
	}
}

void ATOMIC_INC(ATOMIC_T *v)
{
	if (osdep_service.ATOMIC_INC) {
		osdep_service.ATOMIC_INC(v);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_INC");
	}
}

void ATOMIC_DEC(ATOMIC_T *v)
{
	if (osdep_service.ATOMIC_DEC) {
		osdep_service.ATOMIC_DEC(v);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_DEC");
	}
}

int ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
	if (osdep_service.ATOMIC_ADD_RETURN) {
		return osdep_service.ATOMIC_ADD_RETURN(v, i);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_ADD_RETURN");
	}

	return 0;
}

int ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
	if (osdep_service.ATOMIC_SUB_RETURN) {
		return osdep_service.ATOMIC_SUB_RETURN(v, i);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_SUB_RETURN");
	}

	return 0;
}

int ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	if (osdep_service.ATOMIC_INC_RETURN) {
		return osdep_service.ATOMIC_INC_RETURN(v);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_INC_RETURN");
	}

	return 0;
}

int ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	if (osdep_service.ATOMIC_DEC_RETURN) {
		return osdep_service.ATOMIC_DEC_RETURN(v);
	} else {
		OSDEP_DBG("Not implement osdep service: ATOMIC_DEC_RETURN");
	}

	return 0;
}

int ATOMIC_DEC_AND_TEST(ATOMIC_T *v)
{
	return ATOMIC_DEC_RETURN(v) == 0;
}

u64 rtw_modular64(u64 x, u64 y)
{
	if (osdep_service.rtw_modular64) {
		return osdep_service.rtw_modular64(x, y);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_modular64");
	}

	return 0;
}

int rtw_get_random_bytes(void *dst, u32 size)
{
	if (osdep_service.rtw_get_random_bytes) {
		return osdep_service.rtw_get_random_bytes(dst, size);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_get_random_bytes");
	}

	return 0;
}

u32 rtw_getFreeHeapSize(void)
{
	if (osdep_service.rtw_getFreeHeapSize) {
		return osdep_service.rtw_getFreeHeapSize();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_getFreeHeapSize");
	}

	return 0;
}

int rtw_netif_queue_stopped(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;

	return 0;
}

void rtw_netif_wake_queue(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;
}

void rtw_netif_start_queue(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;
}

void rtw_netif_stop_queue(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;
}

void flush_signals_thread(void)
{
}

void rtw_acquire_wakelock(void)
{
	if (osdep_service.rtw_acquire_wakelock) {
		osdep_service.rtw_acquire_wakelock();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_acquire_wakelock");
	}
}

void rtw_release_wakelock(void)
{
	if (osdep_service.rtw_release_wakelock) {
		osdep_service.rtw_release_wakelock();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_release_wakelock");
	}
}

void rtw_wakelock_timeout(u32 timeoutms)
{
	if (osdep_service.rtw_wakelock_timeout) {
		osdep_service.rtw_wakelock_timeout(timeoutms);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_wakelock_timeout");
	}
}

int rtw_create_task(struct task_struct *task, const char *name,
					u32 stack_size, u32 priority, thread_func_t func, void *thctx)
{
	if (osdep_service.rtw_create_task) {
		return osdep_service.rtw_create_task(task, name, stack_size, priority, func, thctx);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_create_task");
	}
	return 1;
}
void rtw_delete_task(struct task_struct *task)
{
	if (osdep_service.rtw_delete_task) {
		osdep_service.rtw_delete_task(task);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_delete_task");
	}

	return;
}
void rtw_wakeup_task(struct task_struct *task)
{
	if (osdep_service.rtw_wakeup_task) {
		osdep_service.rtw_wakeup_task(task);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_wakeup_task");
	}

	return;
}

void rtw_set_priority_task(void *task, u32 NewPriority)
{
	if (osdep_service.rtw_set_priority_task) {
		osdep_service.rtw_set_priority_task(task, NewPriority);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_set_priority_task");
	}

	return;
}

int rtw_get_priority_task(void *task)
{
	if (osdep_service.rtw_get_priority_task) {
		return osdep_service.rtw_get_priority_task(task);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_get_priority_task");
	}

	return -1;
}

void rtw_suspend_task(void *task)
{
	if (osdep_service.rtw_suspend_task) {
		osdep_service.rtw_suspend_task(task);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_suspend_task");
	}

	return;
}

void rtw_resume_task(void *task)
{
	if (osdep_service.rtw_resume_task) {
		osdep_service.rtw_resume_task(task);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_resume_task");
	}

	return;
}

void rtw_suspend_task_all(void)
{
	if (osdep_service.rtw_suspend_task_all) {
		osdep_service.rtw_suspend_task_all();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_suspend_task_all");
	}

	return;
}

void rtw_resume_task_all(void)
{
	if (osdep_service.rtw_resume_task_all) {
		osdep_service.rtw_resume_task_all();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_resume_task_all");
	}

	return;
}

_timerHandle rtw_timerCreate(const signed char *pcTimerName,
							 osdepTickType xTimerPeriodInTicks,
							 u32 uxAutoReload,
							 void *pvTimerID,
							 TIMER_FUN pxCallbackFunction)
{
	if (osdep_service.rtw_timerCreate){
		return osdep_service.rtw_timerCreate(pcTimerName, xTimerPeriodInTicks, uxAutoReload,
											 pvTimerID, pxCallbackFunction);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerCreate");
	}

	return 0;
}

u32 rtw_timerDelete(_timerHandle xTimer,
					osdepTickType xBlockTime)
{
	if (osdep_service.rtw_timerDelete) {
		return osdep_service.rtw_timerDelete(xTimer, xBlockTime);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerDelete");
	}

	return 0;
}

u32 rtw_timerIsTimerActive(_timerHandle xTimer)
{
	if (osdep_service.rtw_timerIsTimerActive) {
		return osdep_service.rtw_timerIsTimerActive(xTimer);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerIsTimerActive");
	}

	return 0;
}

u32  rtw_timerStop(_timerHandle xTimer,
				   osdepTickType xBlockTime)
{
	if (osdep_service.rtw_timerStop) {
		return osdep_service.rtw_timerStop(xTimer, xBlockTime);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerStop");
	}

	return 0;
}

u32  rtw_timerChangePeriod(_timerHandle xTimer,
						   osdepTickType xNewPeriod,
						   osdepTickType xBlockTime)
{
	if (osdep_service.rtw_timerChangePeriod) {
		return osdep_service.rtw_timerChangePeriod(xTimer, xNewPeriod, xBlockTime);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerChangePeriod");
	}

	return 0;
}

void *rtw_timerGetID(_timerHandle xTimer)
{
	if (osdep_service.rtw_timerGetID) {
		return osdep_service.rtw_timerGetID(xTimer);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerGetID");
	}

	return NULL;
}

u32  rtw_timerStart(_timerHandle xTimer, osdepTickType xBlockTime)
{
	if (osdep_service.rtw_timerStart) {
		return osdep_service.rtw_timerStart(xTimer, xBlockTime);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerStart");
	}

	return 0;
}

u32  rtw_timerStartFromISR(_timerHandle xTimer,
						   osdepBASE_TYPE *pxHigherPriorityTaskWoken)
{
	if (osdep_service.rtw_timerStartFromISR) {
		return osdep_service.rtw_timerStartFromISR(xTimer, pxHigherPriorityTaskWoken);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerStartFromISR");
	}

	return 0;
}

u32  rtw_timerStopFromISR(_timerHandle xTimer,
						  osdepBASE_TYPE *pxHigherPriorityTaskWoken)
{
	if (osdep_service.rtw_timerStopFromISR) {
		return osdep_service.rtw_timerStopFromISR(xTimer, pxHigherPriorityTaskWoken);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerStopFromISR");
	}

	return 0;
}

u32  rtw_timerResetFromISR(_timerHandle xTimer,
						   osdepBASE_TYPE *pxHigherPriorityTaskWoken)
{
	if (osdep_service.rtw_timerResetFromISR) {
		return osdep_service.rtw_timerResetFromISR(xTimer, pxHigherPriorityTaskWoken);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerResetFromISR");
	}

	return 0;
}

u32  rtw_timerChangePeriodFromISR(_timerHandle xTimer,
								  osdepTickType xNewPeriod,
								  osdepBASE_TYPE *pxHigherPriorityTaskWoken)
{
	if (osdep_service.rtw_timerChangePeriodFromISR) {
		return osdep_service.rtw_timerChangePeriodFromISR(xTimer, xNewPeriod, pxHigherPriorityTaskWoken);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerChangePeriodFromISR");
	}

	return 0;
}

u32  rtw_timerReset(_timerHandle xTimer,
					osdepTickType xBlockTime)
{
	if (osdep_service.rtw_timerReset) {
		return osdep_service.rtw_timerReset(xTimer, xBlockTime);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_timerReset");
	}

	return 0;
}


#if 0 //TODO
void rtw_init_delayed_work(struct delayed_work *dwork, work_func_t func, const char *name)
{
	if (osdep_service.rtw_init_delayed_work) {
		osdep_service.rtw_init_delayed_work(dwork, func, name);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_init_delayed_work");
	}

	return;
}

void rtw_deinit_delayed_work(struct delayed_work *dwork)
{
	if (osdep_service.rtw_deinit_delayed_work) {
		osdep_service.rtw_deinit_delayed_work(dwork);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_deinit_delayed_work");
	}

	return;
}

int rtw_queue_delayed_work(struct workqueue_struct *wq,
						   struct delayed_work *dwork, u32 delay, void *context)
{
	if (osdep_service.rtw_queue_delayed_work) {
		osdep_service.rtw_queue_delayed_work(wq, dwork, delay, context);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_queue_delayed_work");
	}

	return;
}

BOOLEAN rtw_cancel_delayed_work(struct delayed_work *dwork)
{
	if (osdep_service.rtw_cancel_delayed_work) {
		osdep_service.rtw_cancel_delayed_work(dwork);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_cancel_delayed_work");
	}

	return;
}
#endif
void rtw_thread_enter(char *name)
{
	if (osdep_service.rtw_thread_enter) {
		osdep_service.rtw_thread_enter(name);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_thread_enter");
	}
}

void rtw_thread_exit()
{
	if (osdep_service.rtw_thread_exit) {
		osdep_service.rtw_thread_exit();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_thread_exit");
	}
}

u8 rtw_get_scheduler_state()
{
	// OS_SCHEDULER_NOT_STARTED	0
	// OS_SCHEDULER_RUNNING		1
	// OS_SCHEDULER_SUSPENDED	2
	// OS_SCHEDULER_UNREACHABLE	3
	if (osdep_service.rtw_get_scheduler_state) {
		return osdep_service.rtw_get_scheduler_state();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_get_scheduler_state");
		return 3;
	}
}

void rtw_create_secure_context(u32 secure_stack_size)
{
	if (osdep_service.rtw_create_secure_context) {
		osdep_service.rtw_create_secure_context(secure_stack_size);
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_create_secure_context");
	}
}

void *rtw_get_current_TaskHandle(void)
{
	if (osdep_service.rtw_get_current_TaskHandle) {
		return osdep_service.rtw_get_current_TaskHandle();
	} else {
		OSDEP_DBG("Not implement osdep service: rtw_get_current_TaskHandle");
	}

	return NULL;
}