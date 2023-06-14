/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>

#ifndef configNUM_CORES
#define configNUM_CORES			1
#define portGET_CORE_ID()			0
#endif

#if 1 //Justin: temporary solution for enter critical code for tizenRT
static irqstate_t initial_tizen_flags, up_tizen_flag;
static int flagcnt = 0;
#endif

#if 1 //Justin: temporary solution for enter critical code for tizenRT
void save_and_cli_temp(void);
void restore_flags_temp(void);
void save_and_cli_temp()
{
	if(flagcnt){
		up_tizen_flag = irqsave();
	}else{
		initial_tizen_flags = irqsave();
	}
	flagcnt++;
}

void restore_flags_temp()
{
	flagcnt--;
	if(flagcnt){
		irqrestore(up_tizen_flag);
	}else{
		irqrestore(initial_tizen_flags);
	}
}
#endif

void rtw_enter_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;
	(void) plock;

	if (rtw_in_interrupt()) {
		DBG_INFO("\n");
	} else {
#if 1 //temporary solution for enter critical code for tizenRT
		save_and_cli_temp();
#else 
	//printf("\n"); //suppress meaningless printout
#endif
	}
}

void rtw_exit_critical(_lock *plock, _irqL *pirqL)
{
	/* To avoid gcc warnings */
	(void) pirqL;
	(void) plock;

	if (rtw_in_interrupt()) {
		DBG_INFO("\n");
	} else {
#if 1 //temporary solution for enter critical code for tizenRT
		restore_flags_temp();
#else 
	//printf("\n"); //suppress meaningless printout
#endif
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
	int temp;
	temp = sem_wait(*pmutex);
	if (temp != 0) {
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
	if (temp != 0) {
		DBG_ERR("Failed!\n");
	}
}

void rtw_spin_unlock(_lock *plock)
{
	sem_post(*plock);
}

unsigned int save_and_cli(void)
{
	return irqsave();
}

void restore_flags(unsigned int flag)
{
	irqrestore(flag);
}

void cli()
{
}


