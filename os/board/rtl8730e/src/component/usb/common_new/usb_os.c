/**
  ******************************************************************************
  * @file    usb_ch9.h
  * @author  Realsil WLAN5 Team
  * @brief   This file provides general defines for USB SPEC CH9
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "usb_hal.h"
#ifndef CONFIG_FLOADER_USBD_EN
#include "osdep_service.h"
#endif

/* Private defines -----------------------------------------------------------*/

#ifndef CONFIG_FLOADER_USBD_EN
USB_DATA_SECTION
static const char *const TAG = "USB";
#endif

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/**
  * @brief  sleep ms, will release CPU
  * @param  ms: time in ms
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_sleep_ms(u32 ms)
{
#ifdef CONFIG_FLOADER_USBD_EN
	DelayMs(ms);
#else
	rtw_msleep_os(ms);
	//rtw_mdelay_os(ms);
	//vtaskdelay
#endif
}

/**
  * @brief  Delay us, will hold CPU
  * @param  us: time in us
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_delay_us(u32 us)
{
#ifdef CONFIG_FLOADER_USBD_EN
	DelayUs(us);
#else
	rtw_usleep_os(us);
	//rtw_udelay_os(us);
	//vtaskdelay
#endif
}

#ifndef CONFIG_FLOADER_USBD_EN

/**
  * @brief  malloc for size
  * @param  size: the request memory size
  * @retval void* ：the malloc address
  */
USB_TEXT_SECTION
void *usb_os_mem_alloc(u32 size)
{
	void *ret = NULL;
	if (size == 0) {
		return NULL;
	}
	ret = rtw_zmalloc(size);
	if (NULL == ret) {
		RTK_LOGE(TAG, "Fail to allocate %d memory\n", size);
		return NULL;
	}
	RTK_LOGD(TAG, "Allocate memory %d(0x%08x) success\n", size, (u32)ret);
	return ret;
}
/**
  * @brief  free memory
  * @param  handle: the memory handle to be freed
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_mem_free(void *handle)
{
	if (handle) {
		RTK_LOGD(TAG, "Free memory 0x%08x\n", (u32)handle);
		rtw_free(handle);
	}
}

/**
  * @brief  Alloc spinloc
  * @param  void
  * @retval Spinloc pointer
  */
USB_TEXT_SECTION
usb_spinlock_t *usb_os_spinlock_alloc(void)
{
	usb_spinlock_t *lock = NULL;
	lock = (usb_spinlock_t *)rtw_zmalloc(sizeof(usb_spinlock_t));
	if (lock != NULL) {
		rtw_spinlock_init((_lock *)lock);
	}
	return lock;
}

/**
  * @brief  Free spinloc
  * @param  lock: spinloc pointer
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_spinlock_free(usb_spinlock_t *lock)
{
	if (lock != NULL) {
		rtw_spinlock_free((_lock *)lock);
		rtw_free((void *)lock);
	}
}

/**
  * @brief  Get spinloc
  * @param  lock: spinloc pointer
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_spinlock(usb_spinlock_t *lock)
{
	rtw_spin_lock((_lock *)lock);
}

/**
  * @brief  Put spinloc
  * @param  lock: spinloc pointer
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_spinunlock(usb_spinlock_t *lock)
{
	rtw_spin_unlock((_lock *)lock);
}

/**
  * @brief  Disable USB interrupt and get spinloc
  * @param  lock: spinloc pointer
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_spinlock_safe(usb_spinlock_t *lock)
{
	usb_hal_disable_interrupt();
	usb_os_spinlock(lock);
}

/**
  * @brief  Put spinlock and enable USB interrupt
  * @param  lock: spinloc pointer
  * @retval void
  */
USB_TEXT_SECTION
void usb_os_spinunlock_safe(usb_spinlock_t *lock)
{
	usb_os_spinunlock(lock);
	usb_hal_enable_interrupt();
}

#endif

