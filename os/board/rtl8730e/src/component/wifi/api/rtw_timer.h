/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_TIMER_H_
#define __RTW_TIMER_H_

#define AP_CSA_TO	(102)
#define REAUTH_TO	(800) //(50)
#define REASSOC_TO	(300) //(50)
#define REAUTH_LIMIT	(4)
#define REASSOC_LIMIT	(4)

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
#if defined (CONFIG_AS_INIC_NP) && CONFIG_AS_INIC_NP
#define MAX_TIMER_BUF_NUM		18
#elif defined (CONFIG_AS_INIC_AP) && CONFIG_AS_INIC_AP
#define MAX_TIMER_BUF_NUM		5
#else
#define MAX_TIMER_BUF_NUM		23
#endif
#endif

void init_timer(struct timer_list *timer);
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

#endif //__RTW_TIMER_H_
