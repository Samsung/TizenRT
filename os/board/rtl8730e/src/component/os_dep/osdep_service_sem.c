/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>

void rtw_init_sema(_sema *sema, int init_val)
{
	if (*sema == NULL) {
		*sema = (_sema) kmm_zalloc(sizeof(sem_t));
		if (*sema == NULL) {
			DBG_ERR("Failed to kmm_zalloc\n");
			return;
		}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(*sema);
#endif
	} else {
		DBG_ERR("Already inited\n");
		return;
	}
	int ret = sem_init((sem_t *)(*sema), 0, init_val);
	if (ret != OK) {
		DBG_ERR("Failed to sem_init\n");
		kmm_free(*sema);
		return;
	}
}

void rtw_free_sema(_sema *sema)
{
	int i;
	if (*sema != NULL) {
		i = sem_destroy(*sema);
		if (i == 0) {
			kmm_free(*sema);
		} else {
			DBG_ERR("Fail!!!\n");
		}
	}
	*sema = NULL;
}

void rtw_up_sema(_sema *sema)
{
	if (rtw_in_interrupt()) {
		sem_post((sem_t *)(*sema));
	} else {
		sem_post((sem_t *)(*sema));
	}
}

void rtw_up_sema_from_isr(_sema *sema)
{
	sem_post((sem_t *)(*sema));
}

u32	rtw_down_timeout_sema(_sema *sema, u32 timeout)
{
	struct timespec ts;
	int ret;
	if (timeout == RTW_MAX_DELAY) {
		ret = sem_wait((sem_t *)(*sema));
	} else {
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout / 1000;
		ret = sem_timedwait((sem_t *)(*sema), &ts);
	}
	if (ret != OK) {
		return _FAIL;
	} else {
		return _SUCCESS;
	}
}
u32 rtw_down_sema(_sema *sema)
{
	while (rtw_down_timeout_sema(sema, RTW_MAX_DELAY) != _TRUE) {
		OSDEP_DBG("%s(%p) failed, retry\n",  __FUNCTION__, sema);
	}
	return _TRUE;
}


