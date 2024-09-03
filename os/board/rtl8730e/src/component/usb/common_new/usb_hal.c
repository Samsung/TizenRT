/**
  ******************************************************************************
  * @file    usb_hal.c
  * @author  Realsil WLAN5 Team
  * @brief   This file provides the common USB HAL API
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "usb_hal.h"

/* Private defines -----------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

USB_DATA_SECTION
static const char *const TAG = "USB";

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Reset the USB Core (needed after USB clock settings change)
  * @retval Status
  */
USB_TEXT_SECTION
static u8 usb_hal_core_reset(void)
{
	u32 count = 0U;
	u32 reg;
	u32 gSNPSiD = USB_GLOBAL->GSNPSID;

	RTK_LOGI(TAG, "GSNPSID = 0x%08X\n", gSNPSiD);

	/* Wait for AHB master IDLE state. */
	do {
		usb_os_delay_us(10U);
		if (++count > 100000U) {
			RTK_LOGE(TAG, "usb_hal_core_reset: TO1 0x%08X\n", USB_GLOBAL->GRSTCTL);
			return HAL_TIMEOUT;
		}
	} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);

	/* Core Soft Reset */
	count = 0U;
	USB_GLOBAL->GRSTCTL |= USB_OTG_GRSTCTL_CSRST;

	if (gSNPSiD < USB_OTG_CORE_ID_420A) {
		do {
			if (++count > 10000U) {
				RTK_LOGE(TAG, "usb_hal_core_reset: TO2 0x%08X\n", USB_GLOBAL->GRSTCTL);
				return HAL_TIMEOUT;
			}
			usb_os_delay_us(1U);
		} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_CSRST) == USB_OTG_GRSTCTL_CSRST);
	} else {
		do {
			if (++count > 10000U) {
				RTK_LOGE(TAG, "usb_hal_core_reset: TO2 0x%08X\n", USB_GLOBAL->GRSTCTL);
				return HAL_TIMEOUT;
			}
			usb_os_delay_us(1U);
		} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_CSRSTDONE) == 0U);

		reg = USB_GLOBAL->GRSTCTL;
		reg &= ~USB_OTG_GRSTCTL_CSRST;
		reg |= USB_OTG_GRSTCTL_CSRSTDONE;
		USB_GLOBAL->GRSTCTL = reg;
	}

	/* Wait for AHB master IDLE state. */
	do {
		usb_os_delay_us(10U);
		if (++count > 100000U) {
			RTK_LOGE(TAG, "[USB] Core reset: TO3 0x%08x\n", USB_GLOBAL->GRSTCTL);
			return HAL_TIMEOUT;
		}
	} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_AHBIDL) == 0U);

	return HAL_OK;
}

#if CONFIG_USB_PHY

/**
  * @brief  Load PHY vendor control registers
  * @param  addr: PHY register address
  * @retval Status
  */
USB_TEXT_SECTION
static u8 usb_hal_load_phy_vendor_control_register(u8 addr)
{
	u8 ret = HAL_OK;
	u32 count = 0U;
	u32 reg = 0x0A300000U;

	reg |= (((u32)(USB_OTG_PHY_LOW_ADDR(addr))) << USB_OTG_GPVNDCTL_VCTRL_Pos);
	USB_GLOBAL->GPVNDCTL = reg;

	do {
		/* 1us timeout expected, 1ms for safe */
		DelayUs(1);
		if (++count > 1000U) {
			RTK_LOGE(TAG, "Fail to load phy vendor control register\n");
			ret = HAL_TIMEOUT;
			break;
		}
	} while ((USB_GLOBAL->GPVNDCTL & USB_OTG_GPVNDCTL_VSTSDONE) == 0U);

	return ret;
}

/**
  * @brief  Select USB PHY register page
  * @param  None
  * @retval Status
  */
USB_TEXT_SECTION
static u8 usb_hal_select_phy_register_page(u8 page)
{
	u8 ret;
	u8 reg;

	ret = usb_hal_read_phy_register(USB_OTG_PHY_REG_F4, &reg);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Fail to R PHY F4: %d\n", ret);
		return ret;
	}

	reg &= (~USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK);
	reg |= (page << USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_POS) & USB_OTG_PHY_REG_F4_BIT_PAGE_SEL_MASK;
	ret = usb_hal_write_phy_register(USB_OTG_PHY_REG_F4, reg);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Fail to W PHY F4: %d\n", ret);
	}

	return ret;
}

#endif

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initializes the USB Core
  * @param  dma_enable DMA enable flag
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_core_init(u8 dma_enable)
{
	u8 ret = HAL_OK;
	u32 reg;

	reg = USB_GLOBAL->GUSBCFG;
	reg &= ~USB_OTG_GUSBCFG_ULPIUTMISEL;
	reg |= USB_OTG_GUSBCFG_PHYIF;
	USB_GLOBAL->GUSBCFG = reg;

	ret = usb_hal_core_reset();
	if (ret != HAL_OK) {
		return ret;
	}

	if (dma_enable == 1U) {
		USB_GLOBAL->GAHBCFG |= (USB_OTG_GAHBCFG_HBSTLEN_4 | USB_OTG_GAHBCFG_DMAEN);
	}

	USB_GLOBAL->GOTGCTL &= ~USB_OTG_GOTGCTL_OTGVER;

	return ret;
}

/**
  * @brief  Enable USB interrupt
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_enable_interrupt(void)
{
	InterruptEn(USB_IRQ, USB_IRQ_PRI);
}

/**
  * @brief  Disable USB interrupt
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_disable_interrupt(void)
{
	InterruptDis(USB_IRQ);
}

/**
  * @brief  Register USB IRQ handler
  * @param  handler: IRQ handler
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_register_irq_handler(usb_irq_fun_t handler)
{
	if (handler != NULL) {
		InterruptRegister(handler, USB_IRQ, (u32)NULL, USB_IRQ_PRI);
	}
}

/**
  * @brief  Unregister USB IRQ handler
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_unregister_irq_handler(void)
{
	InterruptUnRegister(USB_IRQ);
}

/**
  * @brief  Enables the controller's Global Int in the AHB Config reg
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_enable_global_interrupt(void)
{
	USB_GLOBAL->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
}

/**
  * @brief  Disable the controller's Global Int in the AHB Config reg
  * @retval void
*/
USB_TEXT_SECTION
void usb_hal_disable_global_interrupt(void)
{
	USB_GLOBAL->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;
}

/**
  * @brief  Return the global USB interrupt status
  * @retval Global USB interrupt status
  */
USB_TEXT_SECTION
u32 usb_hal_read_interrupts(void)
{
	u32 reg;

	reg = USB_GLOBAL->GINTSTS;
	reg &= USB_GLOBAL->GINTMSK;

	return reg;
}

/**
  * @brief  Clear a USB interrupt
  * @param  interrupt  interrupt flag
  * @retval void
  */
USB_TEXT_SECTION
void usb_hal_clear_interrupts(u32 interrupt)
{
	USB_GLOBAL->GINTSTS |= interrupt;
}

/**
  * @brief  Set functional mode
  * @param  mode OTG mode
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_set_otg_mode(usb_otg_mode_t mode)
{
	u32 reg;
	u32 count = 0U;

	reg = USB_GLOBAL->GUSBCFG;
	reg &= ~(USB_OTG_GUSBCFG_FHMOD | USB_OTG_GUSBCFG_FDMOD);

	if (mode == USB_OTG_MODE_DEVICE) {
		reg |= USB_OTG_GUSBCFG_FDMOD;
		USB_GLOBAL->GUSBCFG = reg;
	} else if (mode == USB_OTG_MODE_HOST) {
#if (CONFIG_USB_OTG == 0)
		reg |= USB_OTG_GUSBCFG_FHMOD;
#endif
		USB_GLOBAL->GUSBCFG = reg;
	} else {
		return HAL_ERR_PARA;
	}

	/* Wait for mode switch ready */
#if CONFIG_USB_OTG
	usb_os_sleep_ms(100U);
#else
	do {
		/* the max timeout is 100ms
		 * the value of 25ms is got from the amebaD branch
		 * the linux code suggests that maybe sleep 2ms (each time) is enough
		 * if the boot time is very important, the sleep time can be decreased
		 */
		usb_os_sleep_ms(25U);

		if ((USB_GLOBAL->GINTSTS & USB_OTG_GINTSTS_CMOD) == (u32)mode) {
			break;
		}

		if (++count > 3U) {
			RTK_LOGD(TAG, "[USB] Set OTG mode %d TO\n", mode);
			return HAL_TIMEOUT;
		}
	} while (1);
#endif

	return HAL_OK;
}

/**
  * @brief  Returns USB core mode
  * @retval return core mode : Host or Device
  *         This parameter can be one of these values:
  *         0 : Host
  *         1 : Device
  */
USB_TEXT_SECTION
usb_otg_mode_t usb_hal_get_otg_mode(void)
{
	usb_otg_mode_t mode = USB_OTG_MODE_DEVICE;
	if (((USB_GLOBAL->GINTSTS) & USB_OTG_GINTSTS_CMOD) != 0U) {
		mode = USB_OTG_MODE_HOST;
	}

	return mode;
}

/**
  * @brief  Set Rx FIFO
  * @param  size: Rx fifo size
  * @retval HAL status
  */
USB_TEXT_SECTION
int usb_hal_set_rx_fifo(u16 size)
{
	USB_GLOBAL->GRXFSIZ = size;

	return HAL_OK;
}

/**
  * @brief  Set Non-periodic Tx FIFO
  * @param  pcd: PCD handle
  * @param  fifo: Tx fifo number
  * @param  size: Fifo size
  * @retval HAL status
  */
USB_TEXT_SECTION
int usb_hal_set_np_tx_fifo(u16 size)
{
	USB_GLOBAL->GNPTXFSIZ = ((u32)size << USB_OTG_GNPTXFSIZ_NPTXFDEP_Pos) | USB_GLOBAL->GRXFSIZ;

	return HAL_OK;
}

/**
  * @brief  Set Periodic Tx FIFO
  * @param  fifo: ptx fifo number
  * @param  size: Fifo size
  * @retval HAL status
  */
USB_TEXT_SECTION
int usb_hal_set_ptx_fifo(u8 fifo, u16 size)
{
	u8 i;
	u32 tx_offset;

	tx_offset = USB_GLOBAL->GRXFSIZ;
	tx_offset += (USB_GLOBAL->GNPTXFSIZ) >> USB_OTG_GNPTXFSIZ_NPTXFDEP_Pos;

	if (USB_OTG_MODE_DEVICE == usb_hal_get_otg_mode()) {  /* device */
		for (i = 0U; i < (fifo - 1U); i++) {
			tx_offset += (USB_GLOBAL->DPTXFSIZ_DIEPTXF[i] >> USB_OTG_DPTXFSIZ_DIEPTXF_TXFD_Pos);
		}

		/* Multiply Tx_Size by 2 to get higher performance */
		USB_GLOBAL->DPTXFSIZ_DIEPTXF[fifo - 1U] = ((u32)size << USB_OTG_DPTXFSIZ_DIEPTXF_TXFD_Pos) | tx_offset;
	} else { /* host */
		USB_GLOBAL->HPTXFSIZ = (u32)(((size << USB_OTG_HPTXFSIZ_PTXFD_Pos) & USB_OTG_HPTXFSIZ_PTXFD) | (tx_offset));
	}

	return HAL_OK;
}

/**
  * @brief  Flush a Tx FIFO
  * @param  num  FIFO number
  *         This parameter can be a value from 1 to 15
            15 means Flush all Tx FIFOs
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_flush_tx_fifo(u32 num)
{
	u32 count = 0U;

	USB_GLOBAL->GRSTCTL = (USB_OTG_GRSTCTL_TXFFLSH | (num << USB_OTG_GRSTCTL_TXFNUM_Pos));

	do {
		if (++count > 200000U) {
			return HAL_TIMEOUT;
		}
		usb_os_delay_us(1U);
	} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH) == USB_OTG_GRSTCTL_TXFFLSH);
	usb_os_delay_us(1U);

	return HAL_OK;
}

/**
  * @brief  Flush Rx FIFO
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_flush_rx_fifo(void)
{
	u32 count = 0;

	USB_GLOBAL->GRSTCTL = USB_OTG_GRSTCTL_RXFFLSH;

	do {
		if (++count > 200000U) {
			return HAL_TIMEOUT;
		}
		usb_os_delay_us(1U);
	} while ((USB_GLOBAL->GRSTCTL & USB_OTG_GRSTCTL_RXFFLSH) == USB_OTG_GRSTCTL_RXFFLSH);
	usb_os_delay_us(1U);

	return HAL_OK;
}

/**
  * @brief  Writes a packet into the Tx FIFO associated with the EP/channel
  * @param  src   pointer to source buffer
  * @param  ep_ch_num  endpoint number
  * @param  len  Number of bytes to write
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_write_packet(u8 *src, u8 ep_ch_num, u16 len)
{
	usb_unaligned_u32_t *p = (usb_unaligned_u32_t *)src;
	u32 count32b, i;
	u32 idx = ep_ch_num;

	RTK_LOGD(TAG, "usb_hal_write_packet EP%d len=%d\n", ep_ch_num, len);

	count32b = ((u32)len + 3U) / 4U;
	for (i = 0U; i < count32b; i++) {
		USB_DFIFO(idx) = p->data;
		p++;
	}

	return HAL_OK;
}

/**
  * @brief  Read a packet from the Tx FIFO associated with the EP/channel
  * @param  dest  source pointer
  * @param  ep_ch_num  endpoint number
  * @param  len  Number of bytes to read
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_read_packet(u8 *dest, u8 ep_ch_num, u16 len)
{
	usb_unaligned_u32_t *p = (usb_unaligned_u32_t *)dest;
	u32 i;
	u32 count32b = ((u32)len + 3U) / 4U;
	u32 idx = ep_ch_num;

	for (i = 0U; i < count32b; i++) {
		p->data = USB_DFIFO(idx);
		p++;
	}

	RTK_LOGD(TAG, "usb_hal_read_packet len=%d\n", len);

	return HAL_OK;
}

/**
  * @brief  Dump USB registers
  * @retval None
  */
USB_TEXT_SECTION
void usb_hal_dump_registers(void)
{
	u32 i;

	RTK_LOGA(TAG, "\n*** Core Global CSRs ***\n");
	RTK_LOGA(TAG, "GOTGCTL   = 0x%08X\n", USB_GLOBAL->GOTGCTL);
	RTK_LOGA(TAG, "GOTGINT   = 0x%08X\n", USB_GLOBAL->GOTGINT);
	RTK_LOGA(TAG, "GAHBCFG   = 0x%08X\n", USB_GLOBAL->GAHBCFG);
	RTK_LOGA(TAG, "GUSBCFG   = 0x%08X\n", USB_GLOBAL->GUSBCFG);
	RTK_LOGA(TAG, "GRSTCTL   = 0x%08X\n", USB_GLOBAL->GRSTCTL);
	RTK_LOGA(TAG, "GINTSTS   = 0x%08X\n", USB_GLOBAL->GINTSTS);
	RTK_LOGA(TAG, "GINTMSK   = 0x%08X\n", USB_GLOBAL->GINTMSK);
	RTK_LOGA(TAG, "GRXSTSR   = 0x%08X\n", USB_GLOBAL->GRXSTSR);
	RTK_LOGA(TAG, "GRXSTSP   = 0x%08X\n", USB_GLOBAL->GRXSTSP);
	RTK_LOGA(TAG, "GRXFSIZ   = 0x%08X\n", USB_GLOBAL->GRXFSIZ);
	RTK_LOGA(TAG, "GNPTXFSIZ = 0x%08X\n", USB_GLOBAL->GNPTXFSIZ);
	RTK_LOGA(TAG, "GNPTXSTS  = 0x%08X\n", USB_GLOBAL->GNPTXSTS);
	RTK_LOGA(TAG, "GPVNDCTL  = 0x%08X\n", USB_GLOBAL->GPVNDCTL);
	RTK_LOGA(TAG, "GUID      = 0x%08X\n", USB_GLOBAL->GUID);
	RTK_LOGA(TAG, "GSNPSID   = 0x%08X\n", USB_GLOBAL->GSNPSID);
	RTK_LOGA(TAG, "GHWCFG1   = 0x%08X\n", USB_GLOBAL->GHWCFG1);
	RTK_LOGA(TAG, "GHWCFG2   = 0x%08X\n", USB_GLOBAL->GHWCFG2);
	RTK_LOGA(TAG, "GHWCFG3   = 0x%08X\n", USB_GLOBAL->GHWCFG3);
	RTK_LOGA(TAG, "GHWCFG4   = 0x%08X\n", USB_GLOBAL->GHWCFG4);
	RTK_LOGA(TAG, "GLPMCFG   = 0x%08X\n", USB_GLOBAL->GLPMCFG);
	RTK_LOGA(TAG, "GPWRDN    = 0x%08X\n", USB_GLOBAL->GPWRDN);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[0] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[0]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[1] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[1]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[2] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[2]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[3] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[3]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[4] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[4]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[5] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[5]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[6] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[6]);
	RTK_LOGA(TAG, "DPTXFSIZ_DIEPTXF[7] = 0x%08X\n", USB_GLOBAL->DPTXFSIZ_DIEPTXF[7]);
	if (USB_OTG_MODE_DEVICE == usb_hal_get_otg_mode()) {
		RTK_LOGA(TAG, "*** Device Mode CSRs ***\n");
		RTK_LOGA(TAG, "DCFG       = 0x%08X\n", USB_DEVICE->DCFG);
		RTK_LOGA(TAG, "DCTL       = 0x%08X\n", USB_DEVICE->DCTL);
		RTK_LOGA(TAG, "DSTS       = 0x%08X\n", USB_DEVICE->DSTS);
		RTK_LOGA(TAG, "DIEPMSK    = 0x%08X\n", USB_DEVICE->DIEPMSK);
		RTK_LOGA(TAG, "DOEPMSK    = 0x%08X\n", USB_DEVICE->DOEPMSK);
		RTK_LOGA(TAG, "DAINT      = 0x%08X\n", USB_DEVICE->DAINT);
		RTK_LOGA(TAG, "DAINTMSK   = 0x%08X\n", USB_DEVICE->DAINTMSK);
		RTK_LOGA(TAG, "DTKNQR1    = 0x%08X\n", USB_DEVICE->DTKNQR1);
		RTK_LOGA(TAG, "DTKNQR2    = 0x%08X\n", USB_DEVICE->DTKNQR2);
		RTK_LOGA(TAG, "DVBUSDIS   = 0x%08X\n", USB_DEVICE->DVBUSDIS);
		RTK_LOGA(TAG, "DVBUSPULSE = 0x%08X\n", USB_DEVICE->DVBUSPULSE);
		RTK_LOGA(TAG, "DTKNQR3    = 0x%08X\n", USB_DEVICE->DTKNQR3);
		RTK_LOGA(TAG, "DTKNQR4/DIEPEMPMSK = 0x%08X\n\n", USB_DEVICE->DIEPEMPMSK);
		for (i = 0 ; i < USB_MAX_ENDPOINTS; i++) {
			RTK_LOGA(TAG, "DIEPCTL[%ld] = 0x%08X\n", i, USB_INEP(i)->DIEPCTL);
			RTK_LOGA(TAG, "DIEPINT[%ld] = 0x%08X\n", i, USB_INEP(i)->DIEPINT);
			RTK_LOGA(TAG, "DIEPTSIZ[%ld]= 0x%08X\n", i, USB_INEP(i)->DIEPTSIZ);
			RTK_LOGA(TAG, "DIEPDMA[%ld] = 0x%08X\n", i, USB_INEP(i)->DIEPDMA);
			RTK_LOGA(TAG, "DTXFSTS[%ld] = 0x%08X\n", i, USB_INEP(i)->DTXFSTS);
			RTK_LOGA(TAG, "DIEPDMAB[%ld]= 0x%08X\n", i, USB_INEP(i)->DIEPDMAB);
			RTK_LOGA(TAG, "DOEPCTL[%ld] = 0x%08X\n", i, USB_OUTEP(i)->DOEPCTL);
			RTK_LOGA(TAG, "DOEPINT[%ld] = 0x%08X\n", i, USB_OUTEP(i)->DOEPINT);
			RTK_LOGA(TAG, "DOEPTSIZ[%ld]= 0x%08X\n", i, USB_OUTEP(i)->DOEPTSIZ);
			RTK_LOGA(TAG, "DOEPDMA[%ld] = 0x%08X\n", i, USB_OUTEP(i)->DOEPDMA);
			RTK_LOGA(TAG, "DOEPDMAB[%ld]= 0x%08X\n\n", i, USB_OUTEP(i)->DOEPDMAB);
		}
	} else {
		RTK_LOGA(TAG, "*** Host Mode CSRs ***\n");
		RTK_LOGA(TAG, "HCFG       = 0x%08X\n", USB_HOST->HCFG);
		RTK_LOGA(TAG, "HFIR       = 0x%08X\n", USB_HOST->HFIR);
		RTK_LOGA(TAG, "HFNUM      = 0x%08X\n", USB_HOST->HFNUM);
		RTK_LOGA(TAG, "HPTXSTS    = 0x%08X\n", USB_HOST->HPTXSTS);
		RTK_LOGA(TAG, "HAINT      = 0x%08X\n", USB_HOST->HAINT);
		RTK_LOGA(TAG, "HAINTMSK   = 0x%08X\n", USB_HOST->HAINTMSK);
		RTK_LOGA(TAG, "USB_HPRT0  = 0x%08X\n", USB_HPRT0);
#if (USB_MAX_PIPES > 0)
		for (i = 0 ; i < USB_MAX_PIPES; i++) {
			RTK_LOGA(TAG, "HCCHAR[%ld]  = 0x%08X\n", i, USB_HC(i)->HCCHAR);
			RTK_LOGA(TAG, "HCSPLT[%ld]  = 0x%08X\n", i, USB_HC(i)->HCSPLT);
			RTK_LOGA(TAG, "HCINT[%ld]   = 0x%08X\n", i, USB_HC(i)->HCINT);
			RTK_LOGA(TAG, "HCINTMSK[%ld]= 0x%08X\n", i, USB_HC(i)->HCINTMSK);
			RTK_LOGA(TAG, "HCTSIZ[%ld]  = 0x%08X\n", i, USB_HC(i)->HCTSIZ);
			RTK_LOGA(TAG, "HCDMA[%ld]   = 0x%08X\n\n", i, USB_HC(i)->HCDMA);
		}
#endif
	}
}


#if CONFIG_USB_PHY

/**
  * @brief  Write USB PHY register
  * @param  addr: USB PHY register address
  * @param  data: USB PHY register value
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_write_phy_register(u8 addr, u8 value)
{
	u8 ret;
	u32 reg;

	reg = HAL_READ32(USB_REG_BASE, USB_ADDON_REG_VND_STS_OUT);
	reg &= (~USB_ADDON_REG_VND_STS_OUT_DATA_MASK);
	reg |= (value << USB_ADDON_REG_VND_STS_OUT_DATA_POS);
	HAL_WRITE32(USB_REG_BASE, USB_ADDON_REG_VND_STS_OUT, reg);

	/* Load low addr */
	ret = usb_hal_load_phy_vendor_control_register(USB_OTG_PHY_LOW_ADDR(addr));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = usb_hal_load_phy_vendor_control_register(USB_OTG_PHY_HIGH_ADDR(addr));
	}

	return ret;
}

/**
  * @brief  Read USB PHY register
  * @param  addr: USB PHY register address
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_read_phy_register(u8 addr, u8 *value)
{
	u8 ret;
	u32 reg;
	u8 addr_read;

	if (addr >= 0xE0) {
		addr_read = addr - 0x20;
	} else {
		addr_read = addr;
	}

	/* Load low addr */
	ret = usb_hal_load_phy_vendor_control_register(USB_OTG_PHY_LOW_ADDR(addr_read));
	if (ret == HAL_OK) {
		/* Load high addr */
		ret = usb_hal_load_phy_vendor_control_register(USB_OTG_PHY_HIGH_ADDR(addr_read));
		if (ret == HAL_OK) {
			reg = USB_GLOBAL->GPVNDCTL;
			*value = ((reg & USB_OTG_GPVNDCTL_REGDATA_Msk) >> USB_OTG_GPVNDCTL_REGDATA_Pos) & 0xFF;
		}
	}

	return ret;
}

#if CONFIG_USB_OTG

/**
  * @brief  Enable OTG only for test
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_enable_otg(void)
{
	u8 ret = HAL_OK;
	u8 reg;

	ret = usb_hal_select_phy_register_page(USB_OTG_PHY_REG_F4_BIT_PAGE1);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Fail to switch phy page 1: %d\n", ret);
		return ret;
	}

	ret = usb_hal_read_phy_register(USB_OTG_PHY_REG_E1, &reg);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Fail to read phy E1/P1: %d\n", ret);
		return ret;
	}

	reg |= USB_OTG_PHY_REG_E1_PAGE1_BIT_EN_OTG;

	ret = usb_hal_write_phy_register(USB_OTG_PHY_REG_E1, reg);
	if (ret != HAL_OK) {
		RTK_LOGE(TAG, "Fail to write phy E1/P1: %d\n", ret);
		return ret;
	}
}

#endif

/**
  * @brief  USB PHY calibration
  * @param  data: USB PHY calibration data
  * @retval Status
  */
USB_TEXT_SECTION
u8 usb_hal_calibrate(u8 mode)
{
	u8 ret = HAL_OK;
	usb_cal_data_t *data;
	u8 old_page = 0xFF;

	data = usb_chip_get_cal_data(mode);
	if (data != NULL) {
		while (data->page != 0xFF) {
			if (data->page != old_page) {
				ret = usb_hal_select_phy_register_page(data->page);
				if (ret != HAL_OK) {
					RTK_LOGE(TAG, "Fail to switch to page %d: %d\n", data->page, ret);
					break;
				}
				old_page = data->page;
			}
			ret = usb_hal_write_phy_register(data->addr, data->val);
			if (ret != HAL_OK) {
				RTK_LOGE(TAG, "Fail to write page %d register 0x%02X: %d\n", data->page, data->addr, ret);
				break;
			}
			data++;
		}
	}

	return ret;
}

#endif // CONFIG_USB_PHY

