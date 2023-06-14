/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

int max_mutex_num = MAX_MUTEX_BUF_NUM;
struct mutex_buf {
	struct list_head list;
	StaticSemaphore_t mutex;
};

struct mutex_buf mutex_pool[MAX_MUTEX_BUF_NUM];
struct list_head wrapper_mutexbuf_list;
int mutexbuf_used_num, mutex_dynamic_num;
int max_mutexbuf_used_num;
int mutexpool_flag = 0;

void *get_mutex_from_poll(struct list_head *phead, int *count)
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

void release_mutex_to_poll(unsigned char *buf, struct list_head *phead, int *count)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)buf) - sizeof(struct list_head));
	list_add_tail(plist, phead);

	*count = *count - 1;
}

u8 rtw_get_mutex_dynamic_flag(_mutex sema)
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
		*pmutex = xSemaphoreCreateMutexStatic(mutex);
	}

	if (mutexbuf_used_num > max_mutexbuf_used_num) {
		max_mutexbuf_used_num = mutexbuf_used_num;
	}

	return;
exit:
#endif

	if (*pmutex == NULL) {
		*pmutex = kmm_zalloc(sizeof(sem_t));
		if (*pmutex == NULL) {
			DBG_ERR("Failed\n");
			return;
		}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(*pmutex);
#endif
	}
	sem_init(*pmutex, 0, 1);
	sem_setprotocol(*pmutex, SEM_PRIO_NONE);
}

void rtw_mutex_free(_mutex *pmutex)
{
	int i;
	if (*pmutex != NULL) {
		i = sem_destroy(*pmutex);
		if (i == 0) {
			kmm_free(*pmutex);
		} else {
			DBG_ERR("Fail!!!\n");
		}
	}
	*pmutex = NULL;
}

void rtw_mutex_put(_mutex *pmutex)
{
	sem_post(*pmutex);
}

void rtw_mutex_get(_mutex *pmutex)
{
	int temp;
	temp = sem_wait(*pmutex);
	if (temp != 0) {
		DBG_ERR("Failed!\n");
	}
}

int rtw_mutex_get_timeout(_mutex *pmutex, u32 timeout_ms)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += timeout_ms / 1000;
	if (sem_timedwait(*pmutex, &ts) < 0) {
		return _FAIL;
	} else {
		return _SUCCESS;
	}
}


