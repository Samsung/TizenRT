#ifndef _IMXRT_USB_UTIL_H_INCLUDED_
#define _IMXRT_USB_UTIL_H_INCLUDED_

#include <stdint.h>

#define roundup(x, y)	((((x) + ((y) - 1)) / (y)) * (y))

/*! @brief Define USB OSA mutex handle */
#ifdef USB_TIZEN_RT
#include <semaphore.h>
#include <sys/types.h>
#include <tinyara/kthread.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wqueue.h>
#include <debug.h>
#include "cache.h"
typedef sem_t *usb_osa_mutex_handle;
typedef sem_t *usb_osa_event_handle;

typedef enum _USB_RESULT_ {
	USB_OK = 0x00U,	// Success
	USB_ERROR,					// Failed
	USB_TIMEOUT,				// Timeout occurs
} USB_RESULT;

#define CONFIG_USBHOST_EVENT_TASK_DEFPRIO		(120)
#define CONFIG_USBHOST_STATUS_MONITOR_TASK_DEFPRIO	(CONFIG_USBHOST_EVENT_TASK_DEFPRIO - 1)
#define CONFIG_USBHOST_STACKSIZE			(2048L / sizeof(uint32_t))

#define usb_aligned_malloc(size, aligned)		kmm_memalign((aligned), roundup((size), (aligned)))
#define usb_aligned_free(p)				kmm_free((p))

typedef pid_t usb_osa_task_handle;

#define SUPPRESS_FALL_THROUGH_WARNING()


#if !defined(MAX)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#else
#include "fsl_common.h"			// for SDK_Malloc
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

typedef TaskHandle_t usb_osa_task_handle;
typedef osa_event_handle_t usb_osa_event_handle;
typedef uint32_t uintptr_t;
typedef void *usbhost_ep_t;
typedef int32_t ssize_t;

#define FAR
#define CONFIG_USBHOST_EVENT_TASK_DEFPRIO		(6)
#define CONFIG_USBHOST_STATUS_MONITOR_TASK_DEFPRIO	(CONFIG_USBHOST_EVENT_TASK_DEFPRIO - 1)
#define CONFIG_USBHOST_STACKSIZE			(2048L / sizeof(uint32_t))

#define usb_aligned_malloc(size, aligned)		SDK_Malloc(roundup((size), (aligned)), (aligned))
#define usb_aligned_free(p)				SDK_Free((p))

#define arch_clean_invalidate_dcache_by_range_to_poc(start, end) DCACHE_CleanInvalidateByRange(((uint32_t)(start)), (((uint32_t)(end)) - ((uint32_t)(start)) + 1))
#define arch_clean_dcache_by_range_to_poc(start, end)	DCACHE_CleanByRange(((uint32_t)(start)), (((uint32_t)(end)) - ((uint32_t)(start)) + 1))
#endif	/* 
 */


/*! @brief The event flags are cleared automatically or manually.*/
typedef enum _USB_EVENT_MODE {
	USB_EVENT_MANUAL_CLEAR = 0U, /*!< The flags of the event is cleared manually. */
	USB_EVENT_AUTO_CLEAR = 1U, /*!< The flags of the event is cleared automatically. */
} USB_EVENT_MODE;

#define USB_EVENT_FIFO_SIZE	(256)
typedef struct _USB_EVENT {

	uint8_t msg_buf[USB_EVENT_FIFO_SIZE];

	volatile uint32_t in;

	volatile uint32_t out;

	volatile uint32_t full;
} USB_EVENT;

#define USB_SR_ALLOC()				uint8_t usbCurrentSr = 0;
#define USB_ENTER_CRITICAL()		_USB_OsaEnterCritical(&usbCurrentSr)
#define USB_EXIT_CRITICAL()			_USB_OsaExitCritical(usbCurrentSr)

USB_RESULT USB_EVENT_FIFO_CREATE(USB_EVENT *pEvent);
USB_RESULT USB_EVENT_FIFO_DESTROY(USB_EVENT *pEvent);
USB_RESULT USB_EVENT_FIFO_PUSH(USB_EVENT *pEvent, uint8_t msg);
USB_RESULT USB_EVENT_FIFO_POP(USB_EVENT *pEvent, uint8_t *msg);
uint32_t USB_EVENT_FIFO_available(USB_EVENT *pEvent);
uint32_t USB_EVENT_FIFO_is_empty(USB_EVENT *pEvent);
uint32_t USB_EVENT_FIFO_is_full(USB_EVENT *pEvent);


void _USB_OsaEnterCritical(uint8_t *sr);
void _USB_OsaExitCritical(uint8_t sr);

extern USB_RESULT USB_EHCI_EVENT_TRIGGER_CREATE(usb_osa_event_handle *h);
extern USB_RESULT USB_EHCI_EVENT_TRIGGER_SEND(usb_osa_event_handle h);
extern USB_RESULT USB_EHCI_EVENT_TRIGGER_RECV(usb_osa_event_handle h);
extern USB_RESULT USB_EHCI_EVENT_TRIGGER_DESTROY(usb_osa_event_handle *h);

#endif	/* 
 */
