/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <FreeRTOS.h>
#include <osdep_service.h>
#include <stdio.h>
#include <freertos_pmu.h>

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
		rem -= (u64)(high * base) << 32;
	}

	while ((u64) b > 0 && b < rem) {
		b = b + b;
		d = d + d;
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

u64 rtw_modular64(u64 n, u64 base)
{
	unsigned int __base = (base);
	unsigned int __rem;

	if (((n) >> 32) == 0) {
		__rem = (unsigned int)(n) % __base;
		(n) = (unsigned int)(n) / __base;
	} else {
		__rem = __div64_32(&(n), __base);
	}

	return __rem;
}

/* Refer to ecos bsd tcpip codes */
static int _rtw_arc4random(void)
{
#if defined(CONFIG_PLATFORM_AMEBALITE)
	int value = (int)_rand();
	return value;

#else

	u32 res = xTaskGetTickCount();
	static unsigned long seed = 0xDEADB00B;

#if defined(CONFIG_PLATFORM_8711B)
	if (random_seed) {
		seed = random_seed;
		random_seed = 0;
	}
#endif
	seed = ((seed & 0x007F00FF) << 7) ^ ((seed & 0x0F80FF00) >> 8) ^	// be sure to stir those low bits
		   (res << 13) ^ (res >> 9);	// using the clock too!
	return (int)seed;
#endif
}


int rtw_get_random_bytes(void *buf, u32 len)
{
	unsigned int ranbuf;
	unsigned int *lp;
	int i, count;
	count = len / sizeof(unsigned int);
	lp = (unsigned int *)buf;

	for (i = 0; i < count; i++) {
		lp[i] = _rtw_arc4random();
		len -= sizeof(unsigned int);
	}

	if (len > 0) {
		ranbuf = _rtw_arc4random();
		memcpy(&lp[i], &ranbuf, len);
	}
	return 0;
}

u32 rtw_getFreeHeapSize(void)
{
	return 0;
}

void rtw_netif_start_queue(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;
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

void rtw_netif_stop_queue(void *pnetdev)
{
	/* To avoid gcc warnings */
	(void) pnetdev;
}

void rtw_acquire_wakelock(void)
{
}

void rtw_release_wakelock(void)
{
}

void rtw_wakelock_timeout(u32 timeoutms)
{
}

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

int rtw_printf(const char *format,...)
{
	int ret = 0;
#ifdef CONFIG_DEBUG_LWNL80211_VENDOR_DRV_INFO
	va_list ap;
	va_start(ap, format);
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, format, ap);
#else
	ret = vsyslog(LOG_INFO, format, ap);
#endif
	va_end(ap);
#endif
	return ret;
}

int rtw_printf_info(const char *format,...)
{
	va_list ap;
	int ret = 0;
	va_start(ap, format);
#ifdef CONFIG_LOGM
	ret = logm_internal(LOGM_NORMAL, LOGM_IDX, LOGM_INF, format, ap);
#else
	ret = vsyslog(LOG_INFO, format, ap);
#endif
	va_end(ap);
	return ret;
}

static IRQ_FUN TizenUserIrqFunTable[MAX_PERIPHERAL_IRQ_NUM];
static int wrapper_IrqFun(int irq, FAR void *context, FAR void *arg)
{
	if (irq < AMEBALITE_IRQ_FIRST) {
		DBG_INFO("INT %d should not come here\n", irq);
		return OK;
	}
	__NVIC_ClearPendingIRQ(irq - AMEBALITE_IRQ_FIRST);
	if (TizenUserIrqFunTable[irq - AMEBALITE_IRQ_FIRST] != NULL) {
		TizenUserIrqFunTable[irq - AMEBALITE_IRQ_FIRST]((VOID *)(arg));
	} else {
		DBG_INFO("INT_Entry Irq %d Fun Not Assign!!!!!\n", irq - AMEBALITE_IRQ_FIRST);
	}
	return OK;
}

/* AmebaLite compile cannot find this define so hardcoded here */
#define __NVIC_PRIO_BITS 3	/**< Number of priority bits implemented in the NVIC */
BOOL irq_register_ram(IRQ_FUN IrqFun, IRQn_Type IrqNum, u32 Data, u32 Priority)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return _TRUE;
	}

	if (IrqNum == WL_DMA_IRQ || IrqNum == WL_PROTOCOL_IRQ) {
		Priority = 4;
	}
	TizenUserIrqFunTable[IrqNum] = (IRQ_FUN)((u32) IrqFun | 0x1);
	Priority = (Priority << (8 - __NVIC_PRIO_BITS));
	irq_attach(IrqNum + AMEBALITE_IRQ_FIRST, wrapper_IrqFun, (void *)Data);
	up_prioritize_irq(IrqNum + AMEBALITE_IRQ_FIRST, Priority);	//Need to fix, because it can't get the same result as __NVIC_SetPriority
	return _TRUE;
}

BOOL irq_unregister_ram(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return _TRUE;
	}
	irq_detach(IrqNum);
	TizenUserIrqFunTable[IrqNum] = NULL;
	return _TRUE;
}

void irq_enable_ram(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return;
	}
	up_enable_irq(IrqNum + AMEBALITE_IRQ_FIRST);
}

void irq_disable_ram(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return;
	}
	up_disable_irq(IrqNum + AMEBALITE_IRQ_FIRST);
}