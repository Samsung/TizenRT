/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifndef configNUM_CORES
#define configNUM_CORES			1
/* Link to TizenRT code */
#define portGET_CORE_ID()			up_cpu_index()
#endif
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
void rtw_enter_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;
	(void) plock;

	/* In TizenRT kernel, enter_critical_section will handle entering critical zone 
	In single core cases: enter_critical_section behaves as irqsave
	In multi core cases: enter_critical_section adds spinlock mechanism among cores
	*/
}

void rtw_exit_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;
	(void) plock;

	/* In TizenRT kernel, leave_critical_section will handle exiting critical zone 
	In single core cases: leave_critical_section behaves as irqsave
	In multi core cases: leave_critical_section adds spinlock mechanism among cores
	*/
}
#endif

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
	int temp;
	temp = sem_wait(*pmutex);
	if (temp != OK) {
		DBG_ERR("Failed!\n");
	}
	return temp;
}

void rtw_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL)
{
	sem_post(*pmutex);
}

void rtw_cpu_lock(void)
{
	printf("Not implement osdep service: rtw_cpu_lock");
}

void rtw_cpu_unlock(void)
{
	printf("Not implement osdep service: rtw_cpu_unlock");
}


void rtw_spinlock_init(_lock *plock)
{
	if (*plock == NULL) {
		*plock = kmm_zalloc(sizeof(sem_t));
		if (*plock == NULL) {
			DBG_ERR("Failed\n");
			return;
		}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
		DEBUG_SET_CALLER_ADDR(*plock);
#endif
	}
	sem_init(*plock, 0, 1);
	sem_setprotocol(*plock, SEM_PRIO_NONE);
}


void rtw_spinlock_free(_lock *plock)
{
	int i;
	if (*plock != NULL) {
		i = sem_destroy(*plock);
		if (i == 0) {
			kmm_free(*plock);
		} else {
			DBG_ERR("Fail!!!\n");
		}
	}
	*plock = NULL;
}


void rtw_spin_lock(_lock *plock)
{
	int temp;
	temp = sem_wait(*plock);
	if (temp != OK) {
		DBG_ERR("Failed!\n");
	}
}

void rtw_spin_unlock(_lock *plock)
{
	sem_post(*plock);
}

unsigned int save_and_cli(void)
{
	return enter_critical_section();
}

void restore_flags(unsigned int flag)
{
	leave_critical_section(flag);
}

/* For definition in rtw_skbuff.h->ASSERT(0) */
void cli()
{
}
