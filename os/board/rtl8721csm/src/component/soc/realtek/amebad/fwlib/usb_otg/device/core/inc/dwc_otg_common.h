/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef DWC_OTG_COMMON_H
#define DWC_OTG_COMMON_H

#include <stdarg.h>

#include "basic_types.h"
#include "diag.h"
#include "dwc_os.h"

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr)-(size_t)(&((type *)0)->member)))
#endif

#define TIMER_MAX_DELAY    0xFFFFFFFF

#define GFP_KERNEL         1
#define GFP_ATOMIC         1

extern void DWC_SPINLOCK_RAM(dwc_spinlock_t *lock);
extern void DWC_SPINLOCK_FREE_RAM(dwc_spinlock_t *lock);
extern void DWC_SPINUNLOCK_RAM(dwc_spinlock_t *lock);
extern void DWC_SPINLOCK_IRQSAVE_RAM(dwc_spinlock_t *lock, dwc_irqflags_t *flags);
extern dwc_spinlock_t *DWC_SPINLOCK_ALLOC_RAM(void);

extern void DWC_SPINUNLOCK_IRQRESTORE_RAM(dwc_spinlock_t *lock, dwc_irqflags_t flags);

extern dwc_timer_t *DWC_TIMER_ALLOC_RAM(char *name, dwc_timer_callback_t cb, void *data);
extern void DWC_TIMER_FREE_RAM(dwc_timer_t *timer);

extern void DWC_TIMER_SCHEDULE_RAM(dwc_timer_t *timer, uint32_t time_ms);
extern void DWC_TIMER_CANCEL_RAM(dwc_timer_t *timer);

void DWC_UDELAY_RAM(uint32_t usecs);

#endif // DWC_OTG_COMMON_H

