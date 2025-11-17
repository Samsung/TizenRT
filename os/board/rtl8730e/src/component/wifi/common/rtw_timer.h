/**
  ******************************************************************************
  * @file    rtw_timer.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __RTW_TIMER_H_
#define __RTW_TIMER_H_

#include "dlist.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void	            *_timerHandle;
typedef void (*TIMER_FUN)(void *context);

struct timer_list {
	struct list_head list;
	_timerHandle 	timer_hdl;
	unsigned long	data;
	void (*function)(void *);
};

typedef struct timer_list	_timer;
#define rtw_timer_list timer_list

#define AP_CSA_TO	(102)
#define REAUTH_TO	(800) //(50)
#define REASSOC_TO	(300) //(50)
#define REAUTH_LIMIT	(4)
#define REASSOC_LIMIT	(4)

void init_timer(struct timer_list *timer, const char *name);
void mod_timer(struct timer_list *timer, uint32_t delay_time_ms);
void  cancel_timer_ex(struct timer_list *timer);
void del_timer_sync(struct timer_list *timer);
void init_timer_wrapper(void);
void deinit_timer_wrapper(void);
/* void *adapter useless in rtw_init_timer, may clear later. */
void rtw_init_timer(_timer *ptimer, void *adapter, TIMER_FUN pfunc, void *cntx, const char *name);
void rtw_set_timer(_timer *ptimer, uint32_t delay_time);
uint8_t  rtw_cancel_timer(_timer *ptimer);
void rtw_del_timer(_timer *ptimer);
void init_timer_pool(void);
void deinit_timer_pool(void);
#ifdef __cplusplus
}
#endif

#endif //__RTW_TIMER_H_
