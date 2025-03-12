/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
/* This file is renamed from osdep_service_misc.c, purpose is for functions such as wrapper_IrqFun */
#include <os_wrapper.h>
#include <stdio.h>

/* Refer to ecos bsd tcpip codes */
static int _rtw_arc4random(void)
{
	int value = (int)_rand();
	return value;
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

int rtw_get_random_bytes_f_rng(void *p_rng, unsigned char *output, size_t output_size)
{
	UNUSED(p_rng);
	return rtw_get_random_bytes(output, output_size);
}

u32 rtw_getFreeHeapSize(void)
{
	return 0;
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

/* CA32 has 96 Shared Peripheral Interrupts */
static IRQ_FUN TizenUserIrqFunTable[96];
static int wrapper_IrqFun(int irq, FAR void *context, FAR void *arg)
{
	if (irq < AMEBASMART_IRQ_FIRST) {
		DBG_INFO("INT %d should not come here\n", irq);
		return OK;
	}

	if (TizenUserIrqFunTable[irq - AMEBASMART_IRQ_FIRST] != NULL) {
		TizenUserIrqFunTable[irq - AMEBASMART_IRQ_FIRST]((VOID *)(arg));
	} else {
		DBG_INFO("INT_Entry Irq %d Fun Not Assign!!!!!\n", irq - AMEBASMART_IRQ_FIRST);
	}
	return OK;
}

BOOL irq_register(IRQ_FUN IrqFun, IRQn_Type IrqNum, u32 Data, u32 Priority)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return _TRUE;
	}

	if (IrqNum == WL_DMA_IRQ || IrqNum == WL_PROTOCOL_IRQ) {
		Priority = 4;
	}
	TizenUserIrqFunTable[IrqNum] = (IRQ_FUN)((u32) IrqFun | 0x1);
	irq_attach(IrqNum + AMEBASMART_IRQ_FIRST, (xcpt_t)wrapper_IrqFun, (void *)Data);
	up_prioritize_irq(IrqNum + AMEBASMART_IRQ_FIRST, Priority);	// Use the API in arm_gicv2.c
	return _TRUE;
}

BOOL irq_unregister(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return _TRUE;
	}
	irq_detach(IrqNum + AMEBASMART_IRQ_FIRST);
	TizenUserIrqFunTable[IrqNum] = NULL;
	return _TRUE;
}

void irq_enable(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return;
	}
	up_enable_irq(IrqNum + AMEBASMART_IRQ_FIRST);
}

void irq_disable(IRQn_Type IrqNum)
{
	if (IrqNum < 0) {
		DBG_INFO("INT %d should not come here\n", IrqNum);
		return;
	}
	up_disable_irq(IrqNum + AMEBASMART_IRQ_FIRST);
}

void irq_clear_pending(IRQn_Type IrqNum)
{
	up_clear_pending_irq(IrqNum + AMEBASMART_IRQ_FIRST);
}

void irq_set_pending(IRQn_Type IrqNum)
{
	/*cannot Set-pending bits for SGIs*/
	up_set_pending_irq(IrqNum + AMEBASMART_IRQ_FIRST);
}

uint32_t irq_get_pending(IRQn_Type IrqNum)
{
	return up_irq_is_pending(IrqNum + AMEBASMART_IRQ_FIRST);
}