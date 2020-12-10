#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#ifdef USB_TIZEN_RT
#include <tinyara/config.h>
#include <errno.h>
#include <debug.h>
#include <semaphore.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wqueue.h>
#include <tinyara/signal.h>
#include <tinyara/semaphore.h>
#else
#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#endif

#include "usb_host_config.h"
#include "usb_misc.h"

extern volatile uint32_t ISR_is_executed;
extern volatile uint32_t USB_HostEhciTaskFunction_executing;

#define NVIC_SYSH_MAX_ISR_PRIORITY	(0x20)	// max interrupt priority (2) << 4 bit
static uint32_t critical_nesting_level = 0;

inline __attribute__((always_inline))
static uint32_t get_ipsr(void)
{
	uint32_t ret;
	__asm volatile("mrs %0, ipsr":"=r"(ret));
	return (ret);
}

inline __attribute__((always_inline))
static uint32_t get_and_raise_basepri(void)
{
	uint32_t original_basepri, new_basepri;

	__asm volatile
	(
		"	mrs %0, basepri	\n" \
		"	mov %1, %2		\n"	\
		"	msr basepri, %1	\n" \
		"	isb				\n" \
		"	dsb				\n" \
		:"=r" (original_basepri), "=r" (new_basepri) : "i" ( NVIC_SYSH_MAX_ISR_PRIORITY ) : "memory"
	);

	/* This return will not be reached but is necessary to prevent compiler warnings. */
	return original_basepri;
}

inline __attribute__((always_inline))
void raise_basepri(void)
{
	uint32_t new_basepri;

	__asm volatile
	(
		"	mov %0, %1		\n"	\
		"	msr basepri, %0	\n" \
		"	isb				\n" \
		"	dsb				\n" \
		:"=r" (new_basepri) : "i" ( NVIC_SYSH_MAX_ISR_PRIORITY ) : "memory"
	);
}

inline __attribute__((always_inline))
static void set_basepri(uint32_t new_priority)
{
	__asm volatile
	(
		"	msr basepri, %0	" :: "r" ( new_priority ) : "memory"
	);
}

void _USB_OsaEnterCritical(uint8_t *sr)
{
	if (get_ipsr()) {
		*sr = get_and_raise_basepri();
	} else {
		raise_basepri();
		critical_nesting_level++;
	}
}

void _USB_OsaExitCritical(uint8_t sr)
{
	if (get_ipsr()) {
		set_basepri(sr);
	} else {
		if (critical_nesting_level > 0) {
			critical_nesting_level--;
			if (critical_nesting_level == 0) {
				set_basepri(0);
			}
		}
	}
}

USB_RESULT USB_EVENT_FIFO_CREATE(USB_EVENT *pEvent)
{
	USB_SR_ALLOC();

	if (!pEvent) {
		return USB_ERROR;
	}

	USB_ENTER_CRITICAL();

	pEvent->in = 0;
	pEvent->out = 0;
	pEvent->full = false;

	USB_EXIT_CRITICAL();
	return USB_OK;
}

USB_RESULT USB_EVENT_FIFO_DESTROY(USB_EVENT *pEvent)
{
	USB_SR_ALLOC();

	if (pEvent) {
		USB_ENTER_CRITICAL();

		USB_EXIT_CRITICAL();
		return USB_OK;
	}
	return USB_ERROR;
}

USB_RESULT USB_EVENT_FIFO_PUSH(USB_EVENT *pEvent, uint8_t msg)
{
	USB_SR_ALLOC();

	if (pEvent) {
		USB_ENTER_CRITICAL();
		if (!(pEvent->full)) {
			pEvent->msg_buf[pEvent->in] = msg;

			//////////////////////////////////////////////////////////////
			// moving pointer
			//////////////////////////////////////////////////////////////

			pEvent->in++;
			if (pEvent->in == USB_EVENT_FIFO_SIZE) {
				pEvent->in = 0;
			}

			pEvent->full = (pEvent->in == pEvent->out);
			USB_EXIT_CRITICAL();
			return USB_OK;
		}
		USB_EXIT_CRITICAL();
		return USB_OK;
	}
	udbg("USB_EVENT_FIFO_PUSH push failed\n");
	return USB_ERROR;
}

USB_RESULT USB_EVENT_FIFO_POP(USB_EVENT *pEvent, uint8_t *msg)
{
	uint32_t bits;
	USB_SR_ALLOC();

	if (pEvent) {
		USB_ENTER_CRITICAL();
		if (pEvent->full || (pEvent->in != pEvent->out)) {
			*msg = pEvent->msg_buf[pEvent->out];
			//////////////////////////////////////////////////////////////
			// moving pointer
			//////////////////////////////////////////////////////////////
			pEvent->full = false;

			pEvent->out++;
			if (pEvent->out == USB_EVENT_FIFO_SIZE) {
				pEvent->out = 0;
			}
			USB_EXIT_CRITICAL();
			return USB_OK;
		}
		USB_EXIT_CRITICAL();
	}
	return USB_ERROR;
}

uint32_t USB_EVENT_FIFO_available(USB_EVENT *pEvent)
{
	USB_SR_ALLOC();
	uint32_t size = USB_EVENT_FIFO_SIZE;

	USB_ENTER_CRITICAL();

	if (!pEvent->full) {
		if (pEvent->in >= pEvent->out) {
			size = (pEvent->in - pEvent->out);
		} else {
			size = (USB_EVENT_FIFO_SIZE + pEvent->in - pEvent->out);
		}
	}

	USB_EXIT_CRITICAL();
	return size;
}

uint32_t USB_EVENT_FIFO_is_empty(USB_EVENT *pEvent)
{
	int ret;
	USB_SR_ALLOC();

	USB_ENTER_CRITICAL();
	ret = (!pEvent->full && (pEvent->in == pEvent->out));
	USB_EXIT_CRITICAL();

	return ret;
}

uint32_t USB_EVENT_FIFO_is_full(USB_EVENT *pEvent)
{
	int ret;
	USB_SR_ALLOC();

	USB_ENTER_CRITICAL();
	ret = pEvent->full;
	USB_EXIT_CRITICAL();
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// USB_EHCI_TASK_TRIGGER
//////////////////////////////////////////////////////////////////////////
USB_RESULT USB_EHCI_EVENT_TRIGGER_CREATE(usb_osa_event_handle *h)
{
	if (h) {
#ifdef USB_TIZEN_RT
		*h = (sem_t *) usb_aligned_malloc(sizeof(sem_t), USB_CACHE_LINESIZE);
		if (*h) {
			if (sem_init((*h), 1, 0) == OK) {
				sem_setprotocol(*h, SEM_PRIO_NONE);
				return USB_OK;
			} else {
				usb_aligned_free(*h);
				*h = NULL;
			}
		}
#else
		if (OSA_EventCreate(*h, USB_EVENT_AUTO_CLEAR) == USB_OK) {
			return USB_OK;
		}
#endif
	}
	return USB_ERROR;
}

USB_RESULT USB_EHCI_EVENT_TRIGGER_SEND(usb_osa_event_handle h)
{
	if (h) {
#ifdef USB_TIZEN_RT
		sem_post(h);
#else
#define EHCI_TASK_EVENT_READY	(0x01U)
		OSA_EventSet(h, EHCI_TASK_EVENT_READY);
#endif
		return USB_OK;
	}
	return USB_ERROR;
}

USB_RESULT USB_EHCI_EVENT_TRIGGER_RECV(usb_osa_event_handle h)
{
	if (h) {
#ifdef USB_TIZEN_RT
		int ret;

		do {
			// Take the semaphore (perhaps waiting)
			ret = sem_wait(h);
			// The only case that an error should occur here is if the wait was awakened by a signal.
			//DEBUGASSERT(ret == OK || ret == -EINTR);
		} while (ret == -EINTR);
		return USB_OK;
#else
		uint32_t bitSet;

		if (OSA_EventWait(h, osaEventFlagsAll_c, 0, osaWaitForever_c, &bitSet) == USB_OK) {	/* wait all event */
			return USB_OK;
		}
#endif
	}
	return USB_ERROR;
}

USB_RESULT USB_EHCI_EVENT_TRIGGER_DESTROY(usb_osa_event_handle *h)
{
	if (h) {
		if (*h) {
#ifdef USB_TIZEN_RT
			sem_destroy(*h);
			usb_aligned_free(*h);
			*h = NULL;
#else
			OSA_EventDestroy(*h);
#endif
			return USB_OK;
		}
	}
	return USB_ERROR;
}
