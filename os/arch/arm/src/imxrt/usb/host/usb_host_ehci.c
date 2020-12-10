/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host_hci.h"
#include "usb_host_devices.h"
#include "usb_host_ehci.h"
#include "usb_phy.h"

#ifdef USB_TIZEN_RT
#include <errno.h>
#include <tinyara/usb/usb.h>
#include <tinyara/usb/usbdev.h>
#include <tinyara/usb/usbhost.h>
#include <tinyara/usb/usbhost.h>
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define USB_HOST_EHCI_BANDWIDTH_DELAY (3500U)
#define USB_HOST_EHCI_BANDWIDTH_HUB_LS_SETUP (333U)
#define USB_HOST_EHCI_BANDWIDTH_FRAME_TOTOAL_TIME (900U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief compute data bandwidth time.
 *
 * @param speed       data speed.
 * @param pipeType    data type.
 * @param direction   data direction.
 * @param dataLength  data length.
 *
 *@return time value.
 */
static uint32_t USB_HostBandwidthComputeTime(uint8_t speed, uint8_t pipeType, uint8_t direction, uint32_t dataLength);

/*!
 * @brief compute current allocated bandwidth when ehci work as full-speed or low-speed host.
 *
 * @param frameIndex      frame index.
 * @param frameBandwidths return frame bandwidth data.
 */
static void USB_HostBandwidthFslsHostComputeCurrent(uint16_t frameIndex, uint16_t *frameBandwidth);

/*!
 * @brief compute current hub's allocated FS/LS bandwidth when ehci work as hi-speed host.
 *
 * @param hubNumber       hub address.
 * @param frameIndex      frame index.
 * @param frameBandwidths return frame bandwidth data.
 */
static void USB_HostBandwidthHsHostComputeCurrentFsls(uint32_t hubNumber, uint16_t frameIndex, uint8_t frameBandwidths[8]);

/*!
 * @brief compute current allocated HS bandwidth when ehci work as hi-speed host.
 *
 * @param frameIndex      frame index.
 * @param frameBandwidths return frame bandwidth data.
 */
static void USB_HostBandwidthHsHostComputeCurrentHsAll(uint16_t frameIndex, uint8_t frameBandwidths[8]);

/*!
 * @brief allocate HS bandwidth when host work as high-speed host.
 *
 * @param uframeInterval  micro-frame interval.
 * @param timeData        time for allocating.
 * @param uframe_index_out return start uframe index.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostBandwidthHsHostAllocateHsCommon(uint16_t uframeInterval, uint16_t timeData, uint16_t *uframeIndexOut);

/*!
 * @brief allocate HS interrupt bandwidth when host work as high-speed host.
 *
 * @param ehciPipePointer    ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostBandwidthHsHostAllocateInterrupt(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief allocate bandwidth when host work as full-speed or low-speed host.
 *
 * @param ehciPipePointer    ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostBandwidthFslsHostAllocate(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief get the 2 power value of uint8_t.
 *
 * @param value     input uint8_t value.
 */
static uint8_t USB_HostEhciGet2PowerValue(uint8_t value);

/*!
 * @brief memory zero.
 *
 * @param buffer     buffer pointer.
 * @param length     buffer length.
 */
static void USB_HostEhciZeroMem(uint32_t *buffer, uint32_t length);

/*!
 * @brief host ehci delay.
 *
 * @param ms          millisecond.
 */
static void USB_HostEhciDelay(uint32_t ms);

/*!
 * @brief host ehci start async schedule.
 *
 */
static void USB_HostEhciStartAsync();

/*!
 * @brief host ehci stop async schedule.
 *
 */
static void USB_HostEhciStopAsync();

/*!
 * @brief host ehci start periodic schedule.
 *
 */
static void USB_HostEhciStartPeriodic();

/*!
 * @brief host ehci stop periodic schedule.
 *
 */
static void USB_HostEhciStopPeriodic();

/*!
 * @brief initialize the qtd for one transfer.
 *
 * @param ehciPipePointer ehci pipe pointer.
 * @param transfer      transfer information.
 *
 *@return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciQhQtdListInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer);

/*!
 * @brief release the qtd list.
 *
 * @param ehciQtdStart   qtd list start pointer.
 * @param ehciQtdEnd     qtd list end pointer.
 *
 *@return the transfer's length.
 */
static uint32_t USB_HostEhciQtdListRelease(usb_host_ehci_qtd_t *ehciQtdStart, usb_host_ehci_qtd_t *ehciQtdEnd);

/*!
 * @brief de-initialize qh's linking qtd list.
 * 1. remove qtd from qh; 2. remove transfer from qh; 3. release qtd; 4. transfer callback.
 *
  * @param ehciPipePointer ehci pipe.
 *
 *@return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciQhQtdListDeinit(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief de-initialize transfer's linking qtd list.
 * 1. stop this qh schedule; 2. remove qtd from qh; 3. remove transfer from qh; 4. release qtd; 5. transfer callback; 6.
 *start this qh schedule.
 *
 * @param ehciPipePointer ehci pipe pointer.
 * @param transfer        transfer information.
 *
 *@return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciTransferQtdListDeinit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer);

/*!
 * @brief initialize QH when opening one control, bulk or interrupt pipe.
 *
 * @param ehciPipePointer    ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciQhInit(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief de-initialize QH when closing one control, bulk or interrupt pipe.
 *
 * @param ehciPipePointer    ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciQhDeinit(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief add qh to one frame entry.
 *
 * @param entryPointerValue entry pointer value.
 * @param framePos          frame index.
 * @param uframeInterval    micro-frame interval.
 */
static void USB_HostEhciAddQhToFrame(uint32_t entryPointerValue, uint16_t framePos, uint16_t uframeInterval);

/*!
 * @brief remove entry from frame list.
 *
 * @param entryPointerValue entry pointer value.
 * @param framePos          frame index.
 */
static void USB_HostEhciRemoveFromFrame(uint32_t entryPointerValue, uint16_t framePos);

#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
/*!
 * @brief add sitd array to the frame list.
 *
 * @param entryPointerValue entry pointer value.
 * @param startEntryPointer sitd entry pointer.
 */
static void USB_HostEhciLinkSitd(usb_host_ehci_pipe_t *ehciPipePointer, void *startEntryPointer);

/*!
 * @brief initialize sitd array for one transfer.
 *
 * @param ehciPipePointer   ehci pipe pointer.
 * @param transfer          transfer information.
 */
static usb_status_t USB_HostEhciSitdArrayInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer);

/*!
 * @brief release sitd list.
 *
 * @param startSitdPointer start sitd pointer.
 * @param endSitdPointer   end sitd pointer.
 *
 * @return transfer's result length.
 */
static uint32_t USB_HostEhciSitdArrayRelease(usb_host_ehci_sitd_t *startSitdPointer, usb_host_ehci_sitd_t *endSitdPointer);

/*!
 * @brief de-initialize sitd list.
 * 1. remove transfer; 2. remove sitd from frame list and release sitd; 3. transfer callback
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciSitdArrayDeinit(usb_host_ehci_pipe_t *ehciPipePointer);
#endif							/* USB_HOST_CONFIG_EHCI_MAX_SITD */

#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
/*!
 * @brief compute the frame index when inserting itd.
 *
 * @param lastLinkUframe  last inserted micro-frame.
 * @param startUframe     start micro-frame.
 * @param uframeInterval  micro-frame interval.
 *
 * @return frame index
 */
static uint32_t USB_HostEhciGetItdLinkFrame(uint32_t lastLinkUframe, uint16_t startUframe, uint16_t uframeInterval);

/*!
 * @brief initialize itd list for one transfer.
 * 1. initialize itd list; 2. insert itd to frame list.
 *
 * @param ehciPipePointer ehci pipe pointer.
 * @param transfer        transfer information.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciItdArrayInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer);

/*!
 * @brief release itd list.
 *
 * @param startItdPointer start itd pointer.
 * @param endItdPointer   end itd pointer.
 *
 * @return transfer's result length.
 */
static uint32_t USB_HostEhciItdArrayRelease(usb_host_ehci_itd_t *startItdPointer, usb_host_ehci_itd_t *endItdPointer);

/*!
 * @brief de-initialize itd list.
 * 1. remove transfer; 2. remove itd from frame list and release itd; 3. transfer callback
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciItdArrayDeinit(usb_host_ehci_pipe_t *ehciPipePointer);
#endif							/* USB_HOST_CONFIG_EHCI_MAX_ITD */

/*!
 * @brief open control or bulk pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciOpenControlBulk(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief close control or bulk pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciCloseControlBulk(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief open interrupt pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciOpenInterrupt(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief close interrupt pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciCloseInterrupt(usb_host_ehci_pipe_t *ehciPipePointer);

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
/*!
 * @brief open iso pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciOpenIso(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief close iso pipe.
 *
 * @param ehciPipePointer ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciCloseIso(usb_host_ehci_pipe_t *ehciPipePointer);

/*!
 * @brief allocate HS iso bandwidth when host work as high-speed host.
 *
 * @param ehciPipePointer    ehci pipe pointer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostBandwidthHsHostAllocateIso(usb_host_ehci_pipe_t *ehciPipePointer);

#endif

/*!
 * @brief reset ehci ip.
 *
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciResetIP();

/*!
 * @brief start ehci ip.
 *
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciStartIP();

/*!
 * @brief cancel pipe's transfers.
 *
 * @param ehciPipePointer ehci pipe pointer.
 * @param transfer        the canceling transfer.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciCancelPipe(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer);

/*!
 * @brief control ehci bus.
 *
 * @param bus_control   control code.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostEhciControlBus(uint8_t busControl);

/*!
 * @brief ehci transaction done process function.
 *
 */
void USB_HostEhciTransactionDone();

/*!
 * @brief ehci port change interrupt process function.
 *
 */
static void USB_HostEhciPortChange();

/*!
 * @brief ehci timer0 interrupt process function.
 * cancel control/bulk transfer that time out.
 *
 */
static void USB_HostEhciTimer0();

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
/*!
 * @brief ehci timer1 interrupt process function.
 * cancel control/bulk transfer that time out.
 *
 */
static void USB_HostEhciTimer1();
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

#define USB_HOST_EHCI_MAX_MICRFRAME_VALUE ((USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE << 3U) - 1)

static uint8_t s_SlotMaxBandwidth[8] = { 125, 125, 125, 125, 125, 125, 50, 0 };
static uint8_t s_SlotMaxBandwidthHs[8] = { 100, 100, 100, 100, 100, 100, 100, 100 };

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t USB_HostBandwidthComputeTime(uint8_t speed, uint8_t pipeType, uint8_t direction, uint32_t dataLength)
{
	uint32_t result = (3167 + ((1000 * dataLength) * 7U * 8U / 6U)) / 1000;

	if (pipeType == USB_ENDPOINT_ISOCHRONOUS) {	/* iso */
		if (speed == IMXRT_USB_SPEED_HIGH) {
			result = 38 * 8 * 2083 + 2083 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
		} else if (speed == IMXRT_USB_SPEED_FULL) {
			if (direction == USB_IN) {
				result = 7268000 + 83540 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
			} else {
				result = 6265000 + 83540 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
			}
		} else {
		}
	} else {					/* interrupt */

		if (speed == IMXRT_USB_SPEED_HIGH) {
			result = 55 * 8 * 2083 + 2083 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
		} else if (speed == IMXRT_USB_SPEED_FULL) {
			result = 9107000 + 83540 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
		} else if (speed == IMXRT_USB_SPEED_LOW) {
			if (direction == USB_IN) {
				result = 64060000 + 2000 * USB_HOST_EHCI_BANDWIDTH_HUB_LS_SETUP + 676670 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
			} else {
				result = 6265000 + 83540 * result + USB_HOST_EHCI_BANDWIDTH_DELAY;
			}
		} else {
		}
	}

	result /= 1000000;
	if (result == 0) {
		result = 1;
	}

	return result;
}

static void USB_HostBandwidthFslsHostComputeCurrent(uint16_t frameIndex, uint16_t *frameBandwidth)
{
	usb_host_ehci_pipe_t *ehciPipePointer;

	/* clear the bandwidth */
	*frameBandwidth = 0;

	ehciPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;
	while (ehciPipePointer != NULL) {
		/* only compute iso and interrupt pipe */
		if ((ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) || (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT)) {
			/* does pipe allocate bandwidth in frameIndex frame? note: interval is power of 2. */
			if ((frameIndex >= ehciPipePointer->startFrame) && (!((uint32_t)(frameIndex - ehciPipePointer->startFrame) & (uint32_t)(ehciPipePointer->pipeCommon.interval - 1)))) {
				*frameBandwidth += ehciPipePointer->dataTime;
			}
		}
		ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}
}

static void USB_HostBandwidthHsHostComputeCurrentFsls(uint32_t hubNumber, uint16_t frameIndex, uint8_t frameBandwidths[8])
{
	usb_host_ehci_pipe_t *ehciPipePointer;
	uint8_t index;

	for (index = 0; index < 8; ++index) {
		frameBandwidths[index] = 0;
	}

	ehciPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;
	while (ehciPipePointer != NULL) {
		/* only compute iso and interrupt pipe */
		if ((ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) || (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT)) {
			/* compute FS/LS bandwidth that blong to same high-speed hub, because FS/LS bandwidth is allocated from
			 * first parent high-speed hub */

			//JS_REMOVE USB_HostHelperGetPeripheralInformation(ehciPipePointer->pipeCommon.deviceHandle,kUSB_HostGetDeviceHSHubNumber, &deviceInfo);
			if (ehciPipePointer->pipeCommon.hsHubNumber != hubNumber) {
				ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
				continue;
			}

			if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
				ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
				continue;
			}

			/* does pipe allocate bandwidth in frameIndex frame? note: interval is power of 2. */
			if ((frameIndex >= ehciPipePointer->startFrame) && (!((uint32_t)(frameIndex - ehciPipePointer->startFrame) & (uint32_t)(ehciPipePointer->pipeCommon.interval - 1)))) {
				if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) {	/* iso bandwidth is allocated once */
					frameBandwidths[ehciPipePointer->startUframe + 1] += ehciPipePointer->dataTime;
				} else {		/* iso bandwidth is allocated three times */

					frameBandwidths[ehciPipePointer->startUframe + 1] += ehciPipePointer->dataTime;
					frameBandwidths[ehciPipePointer->startUframe + 2] += ehciPipePointer->dataTime;
					frameBandwidths[ehciPipePointer->startUframe + 3] += ehciPipePointer->dataTime;
				}
			}
		}
		ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}

	for (index = 0; index < 7; ++index) {	/*  */
		if (frameBandwidths[index] > s_SlotMaxBandwidth[index]) {
			frameBandwidths[index + 1] += (frameBandwidths[index] - s_SlotMaxBandwidth[index]);
			frameBandwidths[index] = s_SlotMaxBandwidth[index];
		}
	}
}

static void USB_HostBandwidthHsHostComputeCurrentHsAll(uint16_t frameIndex, uint8_t frameBandwidths[8])
{
	usb_host_ehci_pipe_t *ehciPipePointer;
	uint8_t index;
	uint32_t deviceInfo;
	uint16_t frameInterval;

	for (index = 0; index < 8; ++index) {
		frameBandwidths[index] = 0;
	}

	ehciPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;
	while (ehciPipePointer != NULL) {
		/* only compute iso and interrupt pipe */
		if ((ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) || (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT)) {
			frameInterval = ehciPipePointer->pipeCommon.interval;
			if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {	/* high-speed data bandwidth */
				/* frameInterval means micro-frame here */
				if (frameIndex >= ehciPipePointer->startFrame) {
					if ((frameInterval > 8) && (frameIndex * 8 - ehciPipePointer->startFrame * 8 >= ehciPipePointer->startUframe)) {
						if (!((uint32_t)(frameIndex * 8 - ehciPipePointer->startFrame * 8 - ehciPipePointer->startUframe) & (uint32_t)(frameInterval - 1))) {
							frameBandwidths[ehciPipePointer->startUframe] += ehciPipePointer->dataTime;
						}
					} else {
						for (index = ehciPipePointer->startUframe; index < 8; index += frameInterval) {
							frameBandwidths[index] += ehciPipePointer->dataTime;
						}
					}
				}
			} else {			/* full-speed split bandwidth */

				if ((frameIndex >= ehciPipePointer->startFrame) && (!((uint32_t)(frameIndex - ehciPipePointer->startFrame) & (uint32_t)(frameInterval - 1)))) {
					for (index = 0; index < 8; ++index) {
						if ((uint32_t)(ehciPipePointer->uframeSmask) & (uint32_t)(0x01 << index)) {	/* start-split micro-frames */
							frameBandwidths[index] += ehciPipePointer->startSplitTime;
						}
						if ((uint32_t)(ehciPipePointer->uframeCmask) & (uint32_t)(0x01 << index)) {	/* complete-split micro-frames */
							frameBandwidths[index] += ehciPipePointer->completeSplitTime;
						}
					}
				}
			}
		}
		ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}

#if 0
	for (index = 0; index < 7; ++index) {	/*  */
		if (frameBandwidths[index] > s_SlotMaxBandwidthHs[index]) {
			frameBandwidths[index + 1] += (frameBandwidths[index] - s_SlotMaxBandwidthHs[index]);
			frameBandwidths[index] = s_SlotMaxBandwidthHs[index];
		}
	}
#endif
}

/*!
 * @brief allocate HS bandwidth when host work as high-speed host.
 *
 * @param uframeInterval  micro-frame interval.
 * @param timeData        time for allocating.
 * @param uframeIndexOut return start uframe index.
 *
 * @return kStatus_USB_Success or error codes.
 */
static usb_status_t USB_HostBandwidthHsHostAllocateHsCommon(uint16_t uframeInterval, uint16_t timeData, uint16_t *uframeIndexOut)
{
	uint16_t uframeIntervalIndex;
	uint16_t uframeIndex;
	uint16_t frameIndex;
	uint8_t frameTimes[8];

	frameIndex = 0;
	USB_HostBandwidthHsHostComputeCurrentHsAll(frameIndex, frameTimes);	/* compute the allocated bandwidths in the frameIndex frame */
	for (uframeIntervalIndex = 0; (uframeIntervalIndex < uframeInterval); ++uframeIntervalIndex) {	/* start micro-frame */
		/* for all the micro-frame in interval uframeInterval */
		for (uframeIndex = uframeIntervalIndex; uframeIndex < (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE * 8); uframeIndex += uframeInterval) {
			if (frameIndex != (uframeIndex >> 3)) {
				frameIndex = (uframeIndex >> 3);
				USB_HostBandwidthHsHostComputeCurrentHsAll(frameIndex, frameTimes);	/* compute the allocated bandwidths in the new frameIndex frame */
			}
			if (frameTimes[uframeIndex & 0x0007] + timeData > s_SlotMaxBandwidthHs[(uframeIndex & 0x0007)]) {	/* micro-frame has enough idle bandwidth? */
				break;			/* fail */
			}
		}
		if (uframeIndex >= (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE * 8)) {	/* success? */
			break;
		}
	}

	if (uframeIntervalIndex < uframeInterval) {
		*uframeIndexOut = (uframeIntervalIndex);
		return kStatus_USB_Success;
	} else {
		return kStatus_USB_Error;
	}
}

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))

static usb_status_t USB_HostBandwidthHsHostAllocateIso(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_status_t status;
	uint32_t deviceInfo;
	uint32_t hubNumber;
	uint16_t uframeIntervalIndex = 0;
	uint16_t frameIntervalIndex = 0;
	uint16_t frameIndex;
	uint16_t timeCompleteSplit;
	uint16_t timeStartSplit;
	uint32_t timeData;
	uint8_t SsCsNumber = 0;
	uint16_t frameInterval;
	uint8_t frameTimes[8];
	uint8_t allocateOk = 1;
	uint8_t index;

	timeData = USB_HostBandwidthComputeTime(ehciPipePointer->pipeCommon.speed, USB_ENDPOINT_ISOCHRONOUS, ehciPipePointer->pipeCommon.direction, ehciPipePointer->pipeCommon.maxPacketSize * ehciPipePointer->pipeCommon.numberPerUframe);
	/* pipe is high-speed */
	if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
		uframeIntervalIndex = 0;
		status = USB_HostBandwidthHsHostAllocateHsCommon(ehciPipePointer->uframeInterval, timeData, &uframeIntervalIndex);
		if (status == kStatus_USB_Success) {
			ehciPipePointer->startFrame = (uframeIntervalIndex / 8);
			ehciPipePointer->startUframe = (uframeIntervalIndex & 0x0007);
			ehciPipePointer->dataTime = timeData;

			return kStatus_USB_Success;
		}
	} else {					/* pipe is full-speed or low-speed */

		/* deviceInfo variable means hub think time */
		deviceInfo = 0;			// TODO : temporary HostGetHubThinkTime result /* deviceInfo variable means hub think time */
		//JS_REMOVE USB_HostHelperGetPeripheralInformation(ehciPipePointer->pipeCommon.deviceHandle, kUSB_HostGetHubThinkTime,&deviceInfo); /* deviceInfo variable means hub think time */
		timeData += (deviceInfo * 7 / (6 * 12));

		hubNumber = ehciPipePointer->pipeCommon.hsHubNumber;
		frameInterval = ehciPipePointer->pipeCommon.interval;

		/* compute start-split and complete-split bandwidth */
		if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
			timeStartSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_ISOCHRONOUS, USB_OUT, ehciPipePointer->pipeCommon.maxPacketSize);
			timeCompleteSplit = 0;
		} else {
			timeStartSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_ISOCHRONOUS, USB_IN, 1);
			timeCompleteSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_ISOCHRONOUS, USB_IN, ehciPipePointer->pipeCommon.maxPacketSize);
		}
		/* note: bandwidth must put in one frame */
		for (uframeIntervalIndex = 0; uframeIntervalIndex <= 5; ++uframeIntervalIndex) {	/* uframe interval */
			for (frameIntervalIndex = 0; frameIntervalIndex < frameInterval; ++frameIntervalIndex) {	/* frame interval */
				allocateOk = 1;
				for (frameIndex = frameIntervalIndex; frameIndex < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; frameIndex += frameInterval) {	/* check all the frames */
					/* compute start-split and complete-split number */
					SsCsNumber = (ehciPipePointer->pipeCommon.maxPacketSize + 187) / 188;	/* ss number for iso out; cs number for iso in */
					if (ehciPipePointer->pipeCommon.direction == USB_OUT) {	/* ISO OUT */
						if (uframeIntervalIndex + SsCsNumber > 8) {
							allocateOk = 0;
						}
					} else {
						if (uframeIntervalIndex + 2 + SsCsNumber > 8) {	/* ISO IN: there are two micro-frame interval between start-split and complete-split */
							allocateOk = 0;
						}
					}
					if (allocateOk) {
						/* allocate start-split and complete-split bandwidth */
						USB_HostBandwidthHsHostComputeCurrentHsAll(frameIndex, frameTimes);
						if (ehciPipePointer->pipeCommon.direction == USB_OUT) {	/* ISO OUT */
							index = uframeIntervalIndex;
							for (; index < (uframeIntervalIndex + SsCsNumber); ++index) {
								if (frameTimes[index] + timeStartSplit > s_SlotMaxBandwidthHs[index]) {
									allocateOk = 0;
									break;
								}
							}
						} else {	/* ISO IN */

							index = uframeIntervalIndex;
							if (frameTimes[index] + timeStartSplit > s_SlotMaxBandwidthHs[index]) {
								allocateOk = 0;
							}
							if (allocateOk) {
								index = uframeIntervalIndex + 2;	/* there are two micro-frames interval between start-split and complete-split */
								for (; index < (uframeIntervalIndex + 2 + SsCsNumber); ++index) {
									if (frameTimes[index] + timeCompleteSplit > s_SlotMaxBandwidthHs[index]) {
										allocateOk = 0;
										break;
									}
								}
							}
						}
					}

					/* allocate data bandwidth */
					if (allocateOk) {
						USB_HostBandwidthHsHostComputeCurrentFsls(hubNumber, frameIndex, frameTimes);
						index = uframeIntervalIndex + 1;	/* timeData bandwidth start position */
						/* iso must occupy all the uframe bandwidth */
						{
							deviceInfo = timeData;	/* note: deviceInfo variable means bandwidth here */
							while ((index < 8) && (deviceInfo > s_SlotMaxBandwidth[index])) {
								if (frameTimes[index] > 0) {
									allocateOk = 0;
									break;
								} else {
									deviceInfo -= s_SlotMaxBandwidth[index];
								}
								++index;
							}
						}
					}
					if (allocateOk) {
						/* data bandwidth can be put in the frame? */
						index = uframeIntervalIndex + 1;	/* timeData bandwidth start position */
						frameTimes[index] += timeData;
						for (; index < 7; ++index) {
							if (frameTimes[index] > s_SlotMaxBandwidth[index]) {
								frameTimes[index + 1] += (frameTimes[index] - s_SlotMaxBandwidth[index]);
								frameTimes[index] = s_SlotMaxBandwidth[index];
							} else {
								break;
							}
						}
						if (frameTimes[index] > s_SlotMaxBandwidth[index]) {
							allocateOk = 0;
						}
					}

					if (allocateOk) {
						break;
					}
				}
				if (allocateOk) {
					break;
				}
			}
			if (allocateOk) {
				break;
			}
		}

		if (allocateOk) {
			ehciPipePointer->startFrame = frameIntervalIndex;
			ehciPipePointer->startUframe = uframeIntervalIndex;
			ehciPipePointer->dataTime = timeData;
			ehciPipePointer->startSplitTime = timeStartSplit;
			ehciPipePointer->completeSplitTime = timeCompleteSplit;
			if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
				index = uframeIntervalIndex;
				for (; index < (uframeIntervalIndex + SsCsNumber); ++index) {
					ehciPipePointer->uframeSmask = (uint32_t) ehciPipePointer->uframeSmask | (uint32_t)(0x01 << index);
				}
			} else {
				index = uframeIntervalIndex;
				ehciPipePointer->uframeSmask = (uint32_t) ehciPipePointer->uframeSmask | (uint32_t)(0x01 << index);
				index = uframeIntervalIndex + 2;
				for (; index < (uframeIntervalIndex + 2 + SsCsNumber); ++index) {
					ehciPipePointer->uframeCmask = (uint32_t) ehciPipePointer->uframeCmask | (uint32_t)(0x01 << index);
				}
			}

			return kStatus_USB_Success;
		}
	}

	return kStatus_USB_Error;
}

#endif

static usb_status_t USB_HostBandwidthHsHostAllocateInterrupt(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_status_t status;
	uint32_t deviceInfo;
	uint32_t hubNumber;
	uint16_t uframeIntervalIndex = 0;
	uint16_t frameIntervalIndex = 0;
	uint16_t frameIndex;
	uint16_t timeCompleteSplit;
	uint16_t timeStartSplit;
	uint32_t timeData;
	uint8_t SsCsNumber;
	uint16_t frameInterval;
	uint8_t frameTimes[8];
	uint8_t allocateOk = 1;
	uint8_t index;

	timeData = USB_HostBandwidthComputeTime(ehciPipePointer->pipeCommon.speed, USB_ENDPOINT_INTERRUPT, ehciPipePointer->pipeCommon.direction, ehciPipePointer->pipeCommon.maxPacketSize * ehciPipePointer->pipeCommon.numberPerUframe);
	/* pipe is high-speed */
	if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
		uframeIntervalIndex = 0;
		status = USB_HostBandwidthHsHostAllocateHsCommon(ehciPipePointer->uframeInterval, timeData, &uframeIntervalIndex);
		if (status == kStatus_USB_Success) {
			ehciPipePointer->startFrame = (uframeIntervalIndex / 8);
			ehciPipePointer->startUframe = (uframeIntervalIndex & 0x0007);
			/* for HS interrupt start transaction position */
			if (ehciPipePointer->uframeInterval >= 8) {
				ehciPipePointer->uframeSmask = (0x01 << ehciPipePointer->startUframe);
			} else {
				ehciPipePointer->uframeSmask = 0x00u;
				for (index = ehciPipePointer->startUframe; index < 8; index += ehciPipePointer->uframeInterval) {
					ehciPipePointer->uframeSmask |= (0x01U << index);
				}
			}
			ehciPipePointer->dataTime = timeData;

			return kStatus_USB_Success;
		}
	} else {					/* pipe is full-speed or low-speed */

		deviceInfo = 0;			// TODO : temporary HostGetHubThinkTime result /* deviceInfo variable means hub think time */
		timeData += (deviceInfo * 7 / (6 * 12));

		hubNumber = ehciPipePointer->pipeCommon.hsHubNumber;
		frameInterval = ehciPipePointer->pipeCommon.interval;
		SsCsNumber = 3;			/* complete split number */

		/* compute start-split and complete-split bandwidth */
		if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
			timeStartSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_OUT, ehciPipePointer->pipeCommon.maxPacketSize) + USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_OUT, 1);
			timeCompleteSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_OUT, 0);
		} else {
			timeStartSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_IN, 1);
			timeCompleteSplit = USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_IN, ehciPipePointer->pipeCommon.maxPacketSize) + USB_HostBandwidthComputeTime(IMXRT_USB_SPEED_HIGH, USB_ENDPOINT_INTERRUPT, USB_IN, 0);
		}
		/* note: bandwidth must put in one frame */
		for (uframeIntervalIndex = 0; uframeIntervalIndex <= 4; ++uframeIntervalIndex) {	/* uframe interval */
			for (frameIntervalIndex = 0; frameIntervalIndex < frameInterval; ++frameIntervalIndex) {	/* frame interval */
				allocateOk = 1;
				for (frameIndex = frameIntervalIndex; frameIndex < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; frameIndex += frameInterval) {	/* check all the frames */
					/* allocate data bandwidth */
					USB_HostBandwidthHsHostComputeCurrentFsls(hubNumber, frameIndex, frameTimes);
					index = uframeIntervalIndex + 1;
					for (; index <= (uframeIntervalIndex + 3); ++index) {
						/* data bandwidth number is 3.
																				   uframeIntervalIndex don't exceed 4, so
																				   index cannot exceed 7 */
						if (frameTimes[index] + timeData > s_SlotMaxBandwidth[index]) {
							allocateOk = 0;
							break;
						}
					}

					if (allocateOk) {
						USB_HostBandwidthHsHostComputeCurrentHsAll(frameIndex, frameTimes);
						/* allocate start_split bandwidth */
						if (frameTimes[uframeIntervalIndex] + timeStartSplit > s_SlotMaxBandwidthHs[uframeIntervalIndex]) {
							allocateOk = 0;
						}
						if (allocateOk) {
							/* allocate complete_split bandwidth */
							index = uframeIntervalIndex + 2;
							/* complete-split number is normal 3. When uframeIntervalIndex is 4, complete-split number
							 * is 2. */
							for (; (index <= (uframeIntervalIndex + 1 + SsCsNumber)) && (index < 8); ++index) {
								if (frameTimes[index] + timeCompleteSplit > s_SlotMaxBandwidthHs[index]) {
									allocateOk = 0;
									break;
								}
							}
						}
					}

					if (!allocateOk) {
						break;	/* allocate fail */
					}
				}
				if (allocateOk) {
					break;
				}
			}
			if (allocateOk) {
				break;
			}
		}

		if (allocateOk) {
			ehciPipePointer->startFrame = frameIntervalIndex;
			ehciPipePointer->startUframe = uframeIntervalIndex;
			ehciPipePointer->uframeSmask = (0x01 << ehciPipePointer->startUframe);
			ehciPipePointer->uframeCmask = 0;
			index = uframeIntervalIndex + 2;
			for (; (index <= (uframeIntervalIndex + 1 + SsCsNumber)) && (index < 8); ++index) {
				ehciPipePointer->uframeCmask = (uint32_t) ehciPipePointer->uframeCmask | (uint32_t)(0x01 << index);
			}
			ehciPipePointer->dataTime = timeData;
			ehciPipePointer->startSplitTime = timeStartSplit;
			ehciPipePointer->completeSplitTime = timeCompleteSplit;

			return kStatus_USB_Success;
		}
	}

	return kStatus_USB_BandwidthFail;
}

static usb_status_t USB_HostBandwidthFslsHostAllocate(usb_host_ehci_pipe_t *ehciPipePointer)
{
	uint32_t FslsTime = 0;
	uint16_t uframeIntervalIndex;
	uint16_t frameIndex;
	uint16_t frameInterval;
	uint16_t frameTime;

	FslsTime = 0;				// TODO : temporary HostGetHubThinkTime result /* FslsTime variable means hub think time */

	FslsTime += (FslsTime * 7 / (6 * 12));
	FslsTime = FslsTime + USB_HostBandwidthComputeTime(ehciPipePointer->pipeCommon.speed, ehciPipePointer->pipeCommon.pipeType, ehciPipePointer->pipeCommon.direction, ehciPipePointer->pipeCommon.maxPacketSize);

	frameInterval = ehciPipePointer->pipeCommon.interval;
	for (uframeIntervalIndex = 0; uframeIntervalIndex < ehciPipePointer->uframeInterval; ++uframeIntervalIndex) {	/* uframeIntervalIndex can exceed 8 */
		for (frameIndex = (uframeIntervalIndex >> 3); frameIndex < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; frameIndex += frameInterval) {
			USB_HostBandwidthFslsHostComputeCurrent(frameIndex, &frameTime);
			if (frameTime + FslsTime > USB_HOST_EHCI_BANDWIDTH_FRAME_TOTOAL_TIME) {
				break;
			}
		}
		if (frameIndex >= USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE) {
			break;
		}
	}
	if (uframeIntervalIndex < ehciPipePointer->uframeInterval) {
		ehciPipePointer->startFrame = (uframeIntervalIndex >> 3);
		ehciPipePointer->startUframe = (uframeIntervalIndex & 0x0007);
		ehciPipePointer->uframeSmask = 0;	/* useless */
		ehciPipePointer->uframeCmask = 0;
		ehciPipePointer->dataTime = FslsTime;

		return kStatus_USB_Success;
	}

	return kStatus_USB_BandwidthFail;
}

static uint8_t USB_HostEhciGet2PowerValue(uint8_t value)
{
	if ((value == 0) || (value == 1)) {
		return value;
	}
	if (value & 0xf0) {
		if (value & 0x80) {
			return 128;
		} else if (value & 0x40) {
			return 64;
		} else if (value & 0x20) {
			return 32;
		} else {
			return 16;
		}
	} else {
		if (value & 0x08) {
			return 8;
		} else if (value & 0x04) {
			return 4;
		} else if (value & 0x02) {
			return 2;
		} else {
			return 1;
		}
	}
}

static void USB_HostEhciZeroMem(uint32_t *buffer, uint32_t length)
{
	/* note: the zero unit is uint32_t */
	while (length--) {
		*buffer = 0;
		buffer++;
	}
}

static void USB_HostEhciDelay(uint32_t ms)
{
	/* note: the max delay time cannot exceed half of max value (0x4000) */
	int32_t sofStart;
	int32_t SofEnd;
	uint32_t distance;

	sofStart = (int32_t)(gIMXRT1020_USB_HOST.usb->FRINDEX & EHCI_MAX_UFRAME_VALUE);

	do {
		SofEnd = (int32_t)(gIMXRT1020_USB_HOST.usb->FRINDEX & EHCI_MAX_UFRAME_VALUE);
		distance = (uint32_t)(SofEnd - sofStart + EHCI_MAX_UFRAME_VALUE + 1);
	} while ((distance & EHCI_MAX_UFRAME_VALUE) < (ms * 8));	/* compute the distance between sofStart and SofEnd */
}

static void USB_HostEhciStartAsync()
{
	uint32_t stateSync;

	if (!(gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK)) {
		/* the status must be same when change USBCMD->ASE */
		do {
			stateSync = ((gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK) | (gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_ASE_MASK));
		} while ((stateSync == USBHS_USBSTS_AS_MASK) || (stateSync == USBHS_USBCMD_ASE_MASK));

		gIMXRT1020_USB_HOST.usb->ASYNCLISTADDR = (uint32_t)(gIMXRT1020_USB_HOST.shedFirstQh);
		gIMXRT1020_USB_HOST.usb->USBCMD |= USBHS_USBCMD_ASE_MASK;
		while (!(gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK)) {
		}
	}
}

static void USB_HostEhciStopAsync()
{
	uint32_t stateSync;

	/* the status must be same when change USBCMD->ASE */
	do {
		stateSync = ((gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK) | (gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_ASE_MASK));
	} while ((stateSync == USBHS_USBSTS_AS_MASK) || (stateSync == USBHS_USBCMD_ASE_MASK));

	gIMXRT1020_USB_HOST.usb->USBCMD &= (uint32_t)(~(uint32_t) USBHS_USBCMD_ASE_MASK);	/* disable async schedule */
	while (gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK) {
	}
}

static void USB_HostEhciStartPeriodic()
{
	uint32_t stateSync;

	if (!(gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_PS_MASK)) {
		/* the status must be same when change USBCMD->PSE */
		do {
			stateSync = ((gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_PS_MASK) | (gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_PSE_MASK));
		} while ((stateSync == USBHS_USBSTS_PS_MASK) || (stateSync == USBHS_USBCMD_PSE_MASK));
		gIMXRT1020_USB_HOST.usb->PERIODICLISTBASE = (uint32_t)(gIMXRT1020_USB_HOST.ehciFrameList);
		if (!(gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_PSE_MASK)) {
			gIMXRT1020_USB_HOST.usb->USBCMD |= USBHS_USBCMD_PSE_MASK;	/* start periodic schedule */
		}
		while (!(gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_PS_MASK)) {
		}
	}
	return;
}

static void USB_HostEhciStopPeriodic()
{
	uint32_t stateSync;

	/* the status must be same when change USBCMD->PSE */
	do {
		stateSync = ((gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_PS_MASK) | (gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_PSE_MASK));
	} while ((stateSync == USBHS_USBSTS_PS_MASK) || (stateSync == USBHS_USBCMD_PSE_MASK));

	gIMXRT1020_USB_HOST.usb->USBCMD &= (~USBHS_USBCMD_PSE_MASK);	/* stop periodic schedule */
	while (gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_PS_MASK) {
	}
}

static usb_status_t USB_HostEhciQhQtdListInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer)
{
	volatile usb_host_ehci_qh_t *vltQhPointer;
	usb_host_ehci_qtd_t *qtdPointer = NULL;
	usb_host_ehci_qtd_t *BaseQtdPointer = NULL;
	volatile uint32_t *entryPointer;
	uint32_t qtdNumber;
	uint32_t dataLength;
	uint32_t dataAddress;
	uint32_t endAddress;
	uint8_t index;
	USB_SR_ALLOC();				//JS_TEST

	/* compute the qtd number */
	if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_CONTROL) {
		/* assume setup data don't exceed one qtd data size, one qtd can transfer least 16k data */
		if (transfer->transferLength == 0) {
			qtdNumber = 2;
		} else {
			qtdNumber = 3;
		}
	} else {
		qtdNumber = (((transfer->transferLength) & 0xFFFFC000U) >> 14) + (((transfer->transferLength) & 0x00003FFF) ? 1 : 0);
	}

	vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;
	/* get qtd list */
	USB_HostEhciLock();
	if (qtdNumber <= gIMXRT1020_USB_HOST.ehciQtdNumber) {
		gIMXRT1020_USB_HOST.ehciQtdNumber -= qtdNumber;
		BaseQtdPointer = gIMXRT1020_USB_HOST.ehciQtdHead;
		qtdPointer = NULL;
		do {
			if (qtdPointer != NULL) {
				qtdPointer->nextQtdPointer = (uint32_t) gIMXRT1020_USB_HOST.ehciQtdHead;
			}
			qtdPointer = gIMXRT1020_USB_HOST.ehciQtdHead;
			gIMXRT1020_USB_HOST.ehciQtdHead = (usb_host_ehci_qtd_t *) qtdPointer->nextQtdPointer;
			qtdPointer->nextQtdPointer = 0;
		} while (--qtdNumber);
		if (gIMXRT1020_USB_HOST.ehciQtdNumber == 0) {
			gIMXRT1020_USB_HOST.ehciQtdTail = NULL;
		}
	} else {
		USB_HostEhciUnlock();
		return kStatus_USB_Error;
	}
	USB_HostEhciUnlock();

	/* int qTD list */
	if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_CONTROL) {
		/* setup transaction qtd */
		qtdPointer = BaseQtdPointer;
		qtdPointer->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
		/* dt: need set; ioc: 0; C_Page: 0; PID Code: SETUP; Status: Active */
		qtdPointer->transferResults[0] = qtdPointer->transferResults[1] = 0;
		qtdPointer->transferResults[0] = ((0x00000000 << EHCI_HOST_QTD_DT_SHIFT) | (8 << EHCI_HOST_QTD_TOTAL_BYTES_SHIFT) | (EHCI_HOST_PID_SETUP << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
		dataAddress = ((uint32_t) transfer->setupPacket);
		qtdPointer->transferResults[1] = dataAddress;	/* current offset is set too */
		/* set buffer pointer no matter data length */
		for (index = 0; index < 4; ++index) {
			qtdPointer->bufferPointers[index] = ((dataAddress + (index + 1) * 4 * 1024) & 0xFFFFF000U);
		}

		/* data transaction qtd */
		dataLength = transfer->transferLength;
		if (dataLength != 0) {
			qtdPointer = (usb_host_ehci_qtd_t *)(qtdPointer->nextQtdPointer);

			qtdPointer->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
			/* dt: need set; ioc: 0; C_Page: 0; PID Code: IN/OUT; Status: Active */
			qtdPointer->transferResults[0] = qtdPointer->transferResults[1] = 0;
			if (transfer->direction == USB_OUT) {
				qtdPointer->transferResults[0] = ((0x00000001U << EHCI_HOST_QTD_DT_SHIFT) | (dataLength << EHCI_HOST_QTD_TOTAL_BYTES_SHIFT) | (EHCI_HOST_PID_OUT << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
			} else {
				qtdPointer->transferResults[0] = ((0x00000001U << EHCI_HOST_QTD_DT_SHIFT) | (dataLength << EHCI_HOST_QTD_TOTAL_BYTES_SHIFT) | (EHCI_HOST_PID_IN << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
			}

			dataAddress = (uint32_t) transfer->transferBuffer;
			qtdPointer->transferResults[1] = dataAddress;	/* current offset is set too */
			/* set buffer pointer no matter data length */
			for (index = 0; index < 4; ++index) {
				qtdPointer->bufferPointers[index] = ((dataAddress + (index + 1) * 4 * 1024) & 0xFFFFF000U);
			}
		}

		/* status transaction qtd */
		qtdPointer = (usb_host_ehci_qtd_t *)(qtdPointer->nextQtdPointer);
		qtdPointer->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
		/* dt: dont care; ioc: 1; C_Page: 0; PID Code: IN/OUT; Status: Active */
		qtdPointer->transferResults[0] = qtdPointer->transferResults[1] = 0;
		if ((dataLength == 0) || (transfer->direction == USB_OUT)) {
			qtdPointer->transferResults[0] = ((0x00000001U << EHCI_HOST_QTD_DT_SHIFT) | (EHCI_HOST_PID_IN << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_IOC_MASK) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
		} else {
			qtdPointer->transferResults[0] = ((0x00000001U << EHCI_HOST_QTD_DT_SHIFT) | (EHCI_HOST_PID_OUT << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_IOC_MASK) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
		}
		qtdPointer->nextQtdPointer |= EHCI_HOST_T_INVALID_VALUE;
	} else {
		dataLength = transfer->transferLength;
		dataAddress = (uint32_t) transfer->transferBuffer;
		qtdPointer = BaseQtdPointer;
		while (1) {
			endAddress = dataAddress + (16 * 1024);
			if (endAddress > (uint32_t)(transfer->transferBuffer + transfer->transferLength)) {
				endAddress = (uint32_t)(transfer->transferBuffer + transfer->transferLength);
			}

			qtdPointer->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
			/* dt: set; ioc: 0; C_Page: 0; PID Code: IN/OUT; Status: Active */
			qtdPointer->transferResults[0] = qtdPointer->transferResults[1] = 0;
			if (transfer->direction == USB_OUT) {
				qtdPointer->transferResults[0] = (((endAddress - dataAddress) << EHCI_HOST_QTD_TOTAL_BYTES_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.nextdata01 << EHCI_HOST_QTD_DT_SHIFT) | (EHCI_HOST_QTD_CERR_MAX_VALUE << EHCI_HOST_QTD_CERR_SHIFT) | (EHCI_HOST_PID_OUT << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
			} else {
				qtdPointer->transferResults[0] = (((endAddress - dataAddress) << EHCI_HOST_QTD_TOTAL_BYTES_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.nextdata01 << EHCI_HOST_QTD_DT_SHIFT) | (EHCI_HOST_QTD_CERR_MAX_VALUE << EHCI_HOST_QTD_CERR_SHIFT) | (EHCI_HOST_PID_IN << EHCI_HOST_QTD_PID_CODE_SHIFT) | (EHCI_HOST_QTD_STATUS_ACTIVE_MASK));
			}
			qtdPointer->transferResults[1] = dataAddress;	/* current offset is set too */
			/* set buffer pointer no matter data length */
			for (index = 0; index < 4; ++index) {
				qtdPointer->bufferPointers[index] = ((dataAddress + (index + 1) * 4 * 1024) & 0xFFFFF000U);
			}
			dataAddress = endAddress;	/* for next qtd */

			if (qtdPointer->nextQtdPointer == 0) {
				break;
			}
			qtdPointer = (usb_host_ehci_qtd_t *)(qtdPointer->nextQtdPointer);
		}

		qtdPointer->nextQtdPointer |= EHCI_HOST_T_INVALID_VALUE;
		qtdPointer->transferResults[0] |= EHCI_HOST_QTD_IOC_MASK;	/* last one set IOC */
	}

	/* save qtd to transfer */
	transfer->union1.unitHead = (uint32_t) BaseQtdPointer;
	transfer->union2.unitTail = (uint32_t) qtdPointer;
	/* link transfer to qh */
	transfer->next = NULL;
	if (vltQhPointer->ehciTransferHead == NULL) {
		transfer->next = NULL;
		vltQhPointer->ehciTransferHead = vltQhPointer->ehciTransferTail = transfer;
	} else {
		transfer->next = NULL;
		vltQhPointer->ehciTransferTail->next = transfer;
		vltQhPointer->ehciTransferTail = transfer;
	}

	USB_HostEhciLock();
	/* link qtd to qh (link to end) */
	entryPointer = &(vltQhPointer->nextQtdPointer);
	dataAddress = *entryPointer;	/* dataAddress variable means entry value here */
	while ((dataAddress) && (!(dataAddress & EHCI_HOST_T_INVALID_VALUE))) {
		entryPointer = (volatile uint32_t *)dataAddress;
		dataAddress = *entryPointer;
	}
	*entryPointer = (uint32_t) BaseQtdPointer;
	USB_HostEhciUnlock();
	USB_HostEhciStartAsync();

	return kStatus_USB_Success;
}

static uint32_t USB_HostEhciQtdListRelease(usb_host_ehci_qtd_t *ehciQtdStart, usb_host_ehci_qtd_t *ehciQtdEnd)
{
	uint32_t length = 0;
	usb_host_ehci_qtd_t *qtdPointer;
	USB_SR_ALLOC();				//JS_TEST

	ehciQtdEnd->nextQtdPointer = 0;

	/* compute remaining length */
	qtdPointer = ehciQtdStart;
	while (qtdPointer != ehciQtdEnd) {
		length += ((qtdPointer->transferResults[0] & EHCI_HOST_QTD_TOTAL_BYTES_MASK) >> EHCI_HOST_QTD_TOTAL_BYTES_SHIFT);
		qtdPointer = (usb_host_ehci_qtd_t *) qtdPointer->nextQtdPointer;
	}
	qtdPointer = ehciQtdEnd;
	length += ((qtdPointer->transferResults[0] & EHCI_HOST_QTD_TOTAL_BYTES_MASK) >> EHCI_HOST_QTD_TOTAL_BYTES_SHIFT);

	/* put releasing qtd to idle qtd list */
	USB_HostEhciLock();
	if (gIMXRT1020_USB_HOST.ehciQtdNumber == 0) {
		gIMXRT1020_USB_HOST.ehciQtdHead = ehciQtdStart;
		gIMXRT1020_USB_HOST.ehciQtdTail = ehciQtdEnd;
	} else {
		gIMXRT1020_USB_HOST.ehciQtdTail->nextQtdPointer = (uint32_t) ehciQtdStart;
		gIMXRT1020_USB_HOST.ehciQtdTail = ehciQtdEnd;
	}

	while (ehciQtdStart != ehciQtdEnd) {
		gIMXRT1020_USB_HOST.ehciQtdNumber++;
		ehciQtdStart = (usb_host_ehci_qtd_t *) ehciQtdStart->nextQtdPointer;
	}
	gIMXRT1020_USB_HOST.ehciQtdNumber++;
	USB_HostEhciUnlock();

	return length;
}

static usb_status_t USB_HostEhciQhQtdListDeinit(usb_host_ehci_pipe_t *ehciPipePointer)
{
	volatile usb_host_ehci_qh_t *vltQhPointer;
	usb_host_transfer_t *transfer;
	usb_host_transfer_t *nextTransfer;
	uint8_t needStop = 0;
	USB_SR_ALLOC();				//JS_TEST

	vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;

	USB_HostEhciLock();			/* this API is called from APP, the host task may occupy to access the same resource */
	/* remove qtd from qh */
	if ((!((uint32_t) vltQhPointer->nextQtdPointer & EHCI_HOST_T_INVALID_VALUE)) || (!((uint32_t) vltQhPointer->currentQtdPointer & EHCI_HOST_T_INVALID_VALUE))) {
		/* need stop async schedule */
		if ((!(vltQhPointer->horizontalLinkPointer & EHCI_HOST_T_INVALID_VALUE)) && (ehciPipePointer->pipeCommon.pipeType != USB_ENDPOINT_INTERRUPT)) {
			needStop = 1;
		}
		if (needStop) {
			USB_HostEhciStopAsync();
		}
		vltQhPointer->currentQtdPointer = EHCI_HOST_T_INVALID_VALUE;	/* invalid current qtd */
		vltQhPointer->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;	/* invalid next qtd */
		vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
		if (needStop) {
			USB_HostEhciStartAsync();
		}
	}

	/* remove transfer from the QH transfer list */
	transfer = vltQhPointer->ehciTransferHead;
	vltQhPointer->ehciTransferHead = vltQhPointer->ehciTransferTail = NULL;
	USB_HostEhciUnlock();

	/* release qtd  and transfer callback */
	while (transfer != NULL) {
		nextTransfer = transfer->next;	/* the transfer is released when call back */
		transfer->transferSofar = USB_HostEhciQtdListRelease((usb_host_ehci_qtd_t *)(transfer->union1.unitHead), (usb_host_ehci_qtd_t *)(transfer->union2.unitTail));
		transfer->transferSofar = (transfer->transferLength < transfer->transferSofar) ? 0 : (transfer->transferLength - transfer->transferSofar);
		/* callback function is different from the current condition */
		transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferCancel);
		transfer = nextTransfer;
	}

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciTransferQtdListDeinit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer)
{
	volatile usb_host_ehci_qh_t *vltQhPointer;
	usb_host_transfer_t *preSearchTransfer;
	uint32_t qhNextQtdValue;
	uint32_t qtdPointerEntry;
	uint32_t *searchQtdEntryPointer;
	USB_SR_ALLOC();				//JS_TEST

	vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;

	USB_HostEhciLock();			/* this API is called from APP, the host task may occupy to access the same resource */
	/* remove qtd from qh */
	qhNextQtdValue = (uint32_t) vltQhPointer->currentQtdPointer;
	qtdPointerEntry = *((uint32_t *) qhNextQtdValue + 2);	/* note: qtdPointerEntry means qtd status */
	if ((qhNextQtdValue & EHCI_HOST_T_INVALID_VALUE) || (!(qtdPointerEntry & EHCI_HOST_QTD_STATUS_ACTIVE_MASK))) {
		qhNextQtdValue = (uint32_t) vltQhPointer->nextQtdPointer;
	}
	if (!(qhNextQtdValue & EHCI_HOST_T_INVALID_VALUE)) {	/* there is pending qtd in the qh */
		/* this qh don't schedule temporarily */
		if (ehciPipePointer->pipeCommon.pipeType != USB_ENDPOINT_INTERRUPT) {
			USB_HostEhciStopAsync();
		}
		vltQhPointer->currentQtdPointer |= EHCI_HOST_T_INVALID_VALUE;	/* invalid current qtd */
		vltQhPointer->nextQtdPointer |= EHCI_HOST_T_INVALID_VALUE;	/* invalid next qtd */
		if (ehciPipePointer->pipeCommon.pipeType != USB_ENDPOINT_INTERRUPT) {
			USB_HostEhciStartAsync();
		}

		/* remove qtd from qh one by one */
		qtdPointerEntry = transfer->union1.unitHead;
		while (1) {
			/* search qh's qtd list for qtdPointerEntry */
			searchQtdEntryPointer = &qhNextQtdValue;
			while (!((*searchQtdEntryPointer) & EHCI_HOST_T_INVALID_VALUE)) {
				if ((*searchQtdEntryPointer) == qtdPointerEntry) {
					*searchQtdEntryPointer = *((uint32_t *) qtdPointerEntry);	/* remove the qtd from qh */
					break;
				} else {
					searchQtdEntryPointer = (uint32_t *)(*searchQtdEntryPointer);
				}
			}
			if (qtdPointerEntry == transfer->union2.unitTail) {
				break;
			}
			qtdPointerEntry = *((uint32_t *) qtdPointerEntry);
		}
	}

	/* remove transfer from the QH transfer list */
	preSearchTransfer = vltQhPointer->ehciTransferHead;
	if (preSearchTransfer == transfer) {
		vltQhPointer->ehciTransferHead = preSearchTransfer->next;
	} else {
		while (preSearchTransfer != NULL) {
			if (preSearchTransfer->next == transfer) {
				preSearchTransfer->next = transfer->next;
				break;
			} else {
				preSearchTransfer = preSearchTransfer->next;
			}
		}
	}
	USB_HostEhciUnlock();

	/* release qtd and callback */
	transfer->transferSofar = USB_HostEhciQtdListRelease((usb_host_ehci_qtd_t *)(transfer->union1.unitHead), (usb_host_ehci_qtd_t *)(transfer->union2.unitTail));
	transfer->transferSofar = (transfer->transferLength < transfer->transferSofar) ? 0 : (transfer->transferLength - transfer->transferSofar);
	/* callback function is different from the current condition */
	transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferCancel);

	/* start this qh schedule */
	vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
	if ((qhNextQtdValue != 0) && (!(qhNextQtdValue & EHCI_HOST_T_INVALID_VALUE))) {
		vltQhPointer->nextQtdPointer = qhNextQtdValue;
	}

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciQhInit(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_qh_t *qhPointer = NULL;
	uint32_t portNumber, hubNumber;
	uint32_t controlBits1 = 0;
	uint32_t controlBits2 = 0;
	USB_SR_ALLOC();				//JS_TEST

	/* get qh */
	USB_HostEhciLock();
	if (gIMXRT1020_USB_HOST.ehciQhList != NULL) {
		qhPointer = (usb_host_ehci_qh_t *) gIMXRT1020_USB_HOST.ehciQhList;
		gIMXRT1020_USB_HOST.ehciQhList = (usb_host_ehci_qh_t *)(gIMXRT1020_USB_HOST.ehciQhList->horizontalLinkPointer & EHCI_HOST_POINTER_ADDRESS_MASK);
	}
	USB_HostEhciUnlock();
	if (qhPointer == NULL) {
		udbg("get qh error\r\n");
		return kStatus_USB_Error;
	}
	ehciPipePointer->ehciQh = (void *)qhPointer;

	/* initialize qh */
	USB_HostEhciZeroMem((uint32_t *) qhPointer, sizeof(usb_host_ehci_qh_t) / 4);
	qhPointer->horizontalLinkPointer = EHCI_HOST_T_INVALID_VALUE;
	qhPointer->currentQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	qhPointer->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	qhPointer->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	qhPointer->ehciPipePointer = ehciPipePointer;
	qhPointer->timeOutLabel = 0;
	qhPointer->timeOutValue = USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE;
	/* initialize staticEndpointStates[0] */
	if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT) {
		/* Software should set the RL field to zero if the queue head is an interrupt endpoint. */
		controlBits1 |= ((0U << EHCI_HOST_QH_RL_SHIFT) & EHCI_HOST_QH_RL_MASK);
	} else {
		if (ehciPipePointer->pipeCommon.nakCount >= 16) {
			controlBits1 |= ((15U << EHCI_HOST_QH_RL_SHIFT) & EHCI_HOST_QH_RL_MASK);
		} else {
			controlBits1 |= (((uint32_t) ehciPipePointer->pipeCommon.nakCount << EHCI_HOST_QH_RL_SHIFT) & EHCI_HOST_QH_RL_MASK);
		}
	}
	if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_CONTROL) {
		if (ehciPipePointer->pipeCommon.speed != IMXRT_USB_SPEED_HIGH) {
			controlBits1 |= (1 << EHCI_HOST_QH_C_SHIFT);
		}
		controlBits1 |= (1 << EHCI_HOST_QH_DTC_SHIFT);
	}
	controlBits1 |= ((uint32_t) ehciPipePointer->pipeCommon.maxPacketSize << EHCI_HOST_QH_MAX_PACKET_LENGTH_SHIFT);
	controlBits1 |= (ehciPipePointer->pipeCommon.speed << EHCI_HOST_QH_EPS_SHIFT);
	controlBits1 |= ((uint32_t) ehciPipePointer->pipeCommon.endpointAddress << EHCI_HOST_QH_ENDPT_SHIFT);
	controlBits1 |= (ehciPipePointer->pipeCommon.deviceAddress << EHCI_HOST_QH_DEVICE_ADDRESS_SHIFT);
	qhPointer->staticEndpointStates[0] = controlBits1;
	if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
		controlBits2 |= ((uint32_t) ehciPipePointer->pipeCommon.numberPerUframe << EHCI_HOST_QH_MULT_SHIFT);
	} else {
		controlBits2 |= (0x00000001U << EHCI_HOST_QH_MULT_SHIFT);
	}
	/*initialize staticEndpointStates[1] */
	if (ehciPipePointer->pipeCommon.speed != IMXRT_USB_SPEED_HIGH) {
		hubNumber = ehciPipePointer->pipeCommon.hsHubNumber;
		portNumber = ehciPipePointer->pipeCommon.hsHubPort;
	} else {
		hubNumber = ehciPipePointer->pipeCommon.hubNumber;
		portNumber = ehciPipePointer->pipeCommon.portNumber;
	}
	controlBits2 |= (portNumber << EHCI_HOST_QH_PORT_NUMBER_SHIFT);
	controlBits2 |= (hubNumber << EHCI_HOST_QH_HUB_ADDR_SHIFT);
	controlBits2 |= ((uint32_t) ehciPipePointer->uframeCmask << EHCI_HOST_QH_UFRAME_CMASK_SHIFT);
	controlBits2 |= ((uint32_t) ehciPipePointer->uframeSmask << EHCI_HOST_QH_UFRAME_SMASK_SHIFT);
	qhPointer->staticEndpointStates[1] = controlBits2;

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciQhDeinit(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_qh_t *qhPointer;
	USB_SR_ALLOC();				//JS_TEST

	qhPointer = (usb_host_ehci_qh_t *) ehciPipePointer->ehciQh;
	/* de-initialize qtd from qh */
	USB_HostEhciQhQtdListDeinit(ehciPipePointer);

	/* release QH */
	USB_HostEhciLock();
	qhPointer->horizontalLinkPointer = (uint32_t) gIMXRT1020_USB_HOST.ehciQhList;
	gIMXRT1020_USB_HOST.ehciQhList = qhPointer;
	USB_HostEhciUnlock();

	return kStatus_USB_Success;
}

static void USB_HostEhciAddQhToFrame(uint32_t entryPointerValue, uint16_t framePos, uint16_t uframeInterval)
{
	volatile uint32_t *frameEntryPointer;
	uint32_t frameEntryValue;

	/* search for the inserting point by interval */
	frameEntryPointer = (volatile uint32_t *)(&((uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList)[framePos]);
	while (frameEntryPointer) {
		frameEntryValue = *frameEntryPointer;
		if (frameEntryValue & EHCI_HOST_T_INVALID_VALUE) {
			/* insert into the end */
			*((uint32_t *) entryPointerValue) = EHCI_HOST_T_INVALID_VALUE;
			*frameEntryPointer = (entryPointerValue | EHCI_HOST_POINTER_TYPE_QH);
			break;
		}

		if ((frameEntryValue & EHCI_HOST_POINTER_ADDRESS_MASK) == entryPointerValue) {
			return;				/* has inserted */
		}
		if (((frameEntryValue & EHCI_HOST_POINTER_TYPE_MASK) == EHCI_HOST_POINTER_TYPE_QH) && (((usb_host_ehci_qh_t *)(frameEntryValue & EHCI_HOST_POINTER_ADDRESS_MASK))->ehciPipePointer->uframeInterval <= uframeInterval)) {
			/* insert into this point */
			*((uint32_t *) entryPointerValue) = frameEntryValue;
			*frameEntryPointer = (entryPointerValue | EHCI_HOST_POINTER_TYPE_QH);
			return;
		} else {
			frameEntryPointer = (volatile uint32_t *)(frameEntryValue & EHCI_HOST_POINTER_ADDRESS_MASK);
		}
	}
}

static void USB_HostEhciRemoveFromFrame(uint32_t entryPointerValue, uint16_t framePos)
{
	volatile uint32_t *frameEntryPointer;
	uint32_t frameEntryValue;

	/* search for the qh/itd/sitd entry */
	frameEntryPointer = (volatile uint32_t *)(&((uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList)[framePos]);

	while (frameEntryPointer) {
		frameEntryValue = *frameEntryPointer;
		if (frameEntryValue & EHCI_HOST_T_INVALID_VALUE) {
			return;
		}

		if ((frameEntryValue & EHCI_HOST_POINTER_ADDRESS_MASK) == entryPointerValue) {
			/* remove the entry */
			*frameEntryPointer = *((uint32_t *) entryPointerValue);
			break;
		} else {
			frameEntryPointer = (volatile uint32_t *)(frameEntryValue & EHCI_HOST_POINTER_ADDRESS_MASK);
		}
	}
}

#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
static void USB_HostEhciLinkSitd(usb_host_ehci_pipe_t *ehciPipePointer, void *startEntryPointer)
{
	usb_host_ehci_iso_t *isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;
	usb_host_ehci_sitd_t *sitdPointer;
	uint32_t distance;
	uint32_t frameInterval;
	int32_t shouldLinkFrame;
	int32_t currentFrame;

	frameInterval = (ehciPipePointer->uframeInterval >> 3);

	if (isoPointer->lastLinkFrame == 0xFFFF) {	/* first link */
		currentFrame = ((gIMXRT1020_USB_HOST.usb->FRINDEX & USB_HOST_EHCI_MAX_MICRFRAME_VALUE) >> 3);
		currentFrame = ((uint32_t)(currentFrame + USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER) & (USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3));	/* add USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER */
		/* frame should align with interval */
		currentFrame -= ehciPipePointer->startFrame;
		currentFrame = ((uint32_t)(currentFrame + frameInterval - 1) & (~(frameInterval - 1)));	/* frameInterval is power of 2 */
		currentFrame += ehciPipePointer->startFrame;
	} else {
		shouldLinkFrame = isoPointer->lastLinkFrame + frameInterval;	/* continuous next should link frame */
		if (shouldLinkFrame > (int32_t)(USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3)) {
			shouldLinkFrame = shouldLinkFrame - ((USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3) + 1);
		}
		currentFrame = ((gIMXRT1020_USB_HOST.usb->FRINDEX & USB_HOST_EHCI_MAX_MICRFRAME_VALUE) >> 3);
		distance = ((shouldLinkFrame - currentFrame + (USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3) + 1) & (USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3));	/* get the distance from shouldLinkFrame to currentFrame */
		/* shouldLinkFrame has add frameInterval, think about the align with interval, so here add (frameInterval * 2) */
		if ((distance <= (USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER + frameInterval * 2)) && (distance > 0)) {
			currentFrame = shouldLinkFrame;
		} else {				/* re-link */

			currentFrame = ((uint32_t)(currentFrame + USB_HOST_EHCI_ISO_BOUNCE_FRAME_NUMBER) & (USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3));
			if (currentFrame > (int32_t)(USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3)) {
				currentFrame = currentFrame - ((USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3) + 1);
			}
			/* frame should align with interval */
			currentFrame -= ehciPipePointer->startFrame;
			currentFrame = ((uint32_t)(currentFrame + frameInterval - 1) & (~(frameInterval - 1)));
			currentFrame += ehciPipePointer->startFrame;
		}
	}
	if (currentFrame >= (int32_t) USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE) {	/* frame turn around */
		shouldLinkFrame = (currentFrame - USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE);	/* shouldLinkFrame means inserted frame position */
	} else {
		shouldLinkFrame = currentFrame;	/* shouldLinkFrame means inserted frame position */
	}

	sitdPointer = (usb_host_ehci_sitd_t *) startEntryPointer;
	while (sitdPointer) {
		sitdPointer->frameEntryIndex = shouldLinkFrame;
		/* add to frame list head */
		sitdPointer->nextLinkPointer = ((uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList)[shouldLinkFrame];
		((uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList)[shouldLinkFrame] = ((uint32_t) sitdPointer | EHCI_HOST_POINTER_TYPE_SITD);
		if (sitdPointer->nextSitdIndex == 0xFF) {	/* 0xFF is invalid value */
			break;
		}
		sitdPointer = &(gIMXRT1020_USB_HOST.ehciSitdIndexBase[sitdPointer->nextSitdIndex]);	/* next sitd */

		shouldLinkFrame += frameInterval;
		currentFrame += frameInterval;
		if (shouldLinkFrame >= (int32_t) USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE) {
			shouldLinkFrame = (shouldLinkFrame - USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE);
		}
	}

	if (currentFrame > (int32_t)(USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3)) {
		currentFrame = currentFrame - ((USB_HOST_EHCI_MAX_MICRFRAME_VALUE >> 3) + 1);
	}
	isoPointer->lastLinkFrame = currentFrame;	/* save the last link frame value */
}

static usb_status_t USB_HostEhciSitdArrayInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer)
{
	usb_host_ehci_iso_t *isoPointer;
	uint32_t sitdNumber = 0;
	usb_host_ehci_sitd_t *sitdPointer;
	uint32_t dataLength = 0;
	uint32_t sitdLength = 0;
	uint32_t dataBufferValue;
	uint32_t hubNumber;
	uint32_t portNumber;
	uint32_t tmp;
	uint8_t index;
	USB_SR_ALLOC();				//JS_TEST

	sitdNumber = ((transfer->transferLength - 1 + (ehciPipePointer->pipeCommon.maxPacketSize)) / (ehciPipePointer->pipeCommon.maxPacketSize));
	/* get sitd array */
	tmp = ehciPipePointer - gIMXRT1020_USB_HOST.ehciPipeIndexBase;	/* pipe index */
	USB_HostEhciLock();
	if (gIMXRT1020_USB_HOST.ehciSitdNumber >= sitdNumber) {
		sitdPointer = gIMXRT1020_USB_HOST.ehciSitdList;
		transfer->union1.unitHead = (uint32_t) sitdPointer;
		for (index = 1; index < sitdNumber; ++index) {
			sitdPointer->nextSitdIndex = (((usb_host_ehci_sitd_t *) sitdPointer->nextLinkPointer) - gIMXRT1020_USB_HOST.ehciSitdIndexBase);
			sitdPointer = (usb_host_ehci_sitd_t *) sitdPointer->nextLinkPointer;
		}
		sitdPointer->nextSitdIndex = 0xFF;
		gIMXRT1020_USB_HOST.ehciSitdList = (usb_host_ehci_sitd_t *) sitdPointer->nextLinkPointer;
		gIMXRT1020_USB_HOST.ehciSitdNumber -= sitdNumber;
	} else {
		USB_HostEhciUnlock();
		return kStatus_USB_Error;
	}
	USB_HostEhciUnlock();
	transfer->union2.unitTail = (uint32_t) sitdPointer;
	/* initialize sitd array */
	portNumber = ehciPipePointer->pipeCommon.portNumber;
	hubNumber = ehciPipePointer->pipeCommon.hubNumber;
	sitdPointer = (usb_host_ehci_sitd_t *) transfer->union1.unitHead;
	dataLength = transfer->transferLength;
	while (sitdNumber--) {
		USB_HostEhciZeroMem((uint32_t *) sitdPointer, 7);
		sitdLength = dataLength;
		if (sitdLength > ehciPipePointer->pipeCommon.maxPacketSize) {
			sitdLength = ehciPipePointer->pipeCommon.maxPacketSize;
		}
		dataBufferValue = (uint32_t)(transfer->transferBuffer + (transfer->transferLength - dataLength));
		dataLength -= sitdLength;	/* update left data length */
		sitdPointer->transferResults[1] = dataBufferValue;
		sitdPointer->transferResults[2] = ((dataBufferValue + 4 * 1024) & 0xFFFFF000U);
		sitdPointer->endpointStates[0] = (((uint32_t) ehciPipePointer->pipeCommon.direction << EHCI_HOST_SITD_DIRECTION_SHIFT) | (portNumber << EHCI_HOST_SITD_PORT_NUMBER_SHIFT) | (hubNumber << EHCI_HOST_SITD_HUB_ADDR_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.endpointAddress << EHCI_HOST_SITD_ENDPT_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.deviceAddress << EHCI_HOST_SITD_DEVICE_ADDRESS_SHIFT));
		sitdPointer->transferResults[0] = ((sitdLength << EHCI_HOST_SITD_TOTAL_BYTES_SHIFT) | (EHCI_HOST_SITD_STATUS_ACTIVE_MASK));

		if (gIMXRT1020_USB_HOST.firstDeviceSpeed == IMXRT_USB_SPEED_HIGH) {
			sitdPointer->endpointStates[1] = (((uint32_t) ehciPipePointer->uframeCmask << EHCI_HOST_SITD_CMASK_SHIFT) | ((uint32_t) ehciPipePointer->uframeSmask << EHCI_HOST_SITD_SMASK_SHIFT));

			tmp = (sitdLength + 187) / 188;
			if (tmp > 1) {
				sitdPointer->transferResults[2] |= (0x01 << EHCI_HOST_SITD_TP_SHIFT);	/* for iso split */
			} else {
				sitdPointer->transferResults[2] |= (0x00 << EHCI_HOST_SITD_TP_SHIFT);	/* for iso split */
			}
			sitdPointer->transferResults[2] |= (tmp << EHCI_HOST_SITD_TCOUNT_SHIFT);	/* for iso split */
		}

		sitdPointer->backPointer = EHCI_HOST_T_INVALID_VALUE;

		sitdPointer = (gIMXRT1020_USB_HOST.ehciSitdIndexBase + sitdPointer->nextSitdIndex);
	}
	sitdPointer = (usb_host_ehci_sitd_t *) transfer->union2.unitTail;
	sitdPointer->transferResults[0] |= (1U << EHCI_HOST_SITD_IOC_SHIFT);	/* last set IOC */

	/* link transfer to usb_host_ehci_iso_t transfer list */
	isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;
	USB_HostEhciLock();
	if (isoPointer->ehciTransferHead == NULL) {
		transfer->next = NULL;
		isoPointer->ehciTransferHead = isoPointer->ehciTransferTail = transfer;
	} else {
		transfer->next = NULL;
		isoPointer->ehciTransferTail->next = transfer;
		isoPointer->ehciTransferTail = transfer;
	}
	USB_HostEhciUnlock();

	/* link itd to frame list (note: initialize frameEntryIndex) */
	USB_HostEhciLinkSitd(ehciPipePointer, (void *)transfer->union1.unitHead);

	return kStatus_USB_Success;
}

static uint32_t USB_HostEhciSitdArrayRelease(usb_host_ehci_sitd_t *startSitdPointer, usb_host_ehci_sitd_t *endSitdPointer)
{
	usb_host_ehci_sitd_t *sitdPointer = startSitdPointer;
	uint32_t leftLength = 0;
	USB_SR_ALLOC();
	/* remove itd from frame list */
	while (1) {
		/* record the transfer's result length */
		leftLength += ((sitdPointer->transferResults[0] & EHCI_HOST_SITD_TOTAL_BYTES_MASK) >> EHCI_HOST_SITD_TOTAL_BYTES_SHIFT);
		USB_HostEhciRemoveFromFrame((uint32_t) sitdPointer, sitdPointer->frameEntryIndex);	/* remove from the inserted frame list */

		/* release itd */
		USB_HostEhciLock();
		sitdPointer->nextLinkPointer = (uint32_t) gIMXRT1020_USB_HOST.ehciSitdList;
		gIMXRT1020_USB_HOST.ehciSitdList = sitdPointer;
		gIMXRT1020_USB_HOST.ehciSitdNumber++;
		USB_HostEhciUnlock();

		if (sitdPointer == endSitdPointer) {
			break;
		}

		sitdPointer = &(gIMXRT1020_USB_HOST.ehciSitdIndexBase[sitdPointer->nextSitdIndex]);
	}

	return leftLength;
}

static usb_status_t USB_HostEhciSitdArrayDeinit(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_iso_t *isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;
	usb_host_transfer_t *transfer;
	usb_host_transfer_t *nextTransfer;
	USB_SR_ALLOC();				//JS_TEST

	/* firstly remove the transfer (because host task may occupy to access the resource) */
	USB_HostEhciLock();
	transfer = isoPointer->ehciTransferHead;
	isoPointer->ehciTransferHead = isoPointer->ehciTransferTail = NULL;
	USB_HostEhciUnlock();

	while (transfer != NULL) {
		nextTransfer = transfer->next;
		/* remove sitd from frame list and release itd */
		transfer->transferSofar = transfer->transferLength - USB_HostEhciSitdArrayRelease((usb_host_ehci_sitd_t *) transfer->union1.unitHead, (usb_host_ehci_sitd_t *) transfer->union2.unitTail);
		/* callback function is different from the current condition */
		transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferCancel);
		/* next transfer */
		transfer = nextTransfer;
	}

	return kStatus_USB_Success;
}
#endif							/* USB_HOST_CONFIG_EHCI_MAX_SITD */

#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
static uint32_t USB_HostEhciGetItdLinkFrame(uint32_t lastLinkUframe, uint16_t startUframe, uint16_t uframeInterval)
{
	int32_t shouldLinkUframe;
	int32_t currentUframe;
	int32_t distance;

	if (lastLinkUframe != 0xFFFF) {
		shouldLinkUframe = lastLinkUframe + uframeInterval;
		if (shouldLinkUframe > (int32_t) USB_HOST_EHCI_MAX_MICRFRAME_VALUE) {
			shouldLinkUframe = shouldLinkUframe - (USB_HOST_EHCI_MAX_MICRFRAME_VALUE + 1);
		}
		currentUframe = (gIMXRT1020_USB_HOST.usb->FRINDEX & USB_HOST_EHCI_MAX_MICRFRAME_VALUE);
		distance = ((shouldLinkUframe - currentUframe + USB_HOST_EHCI_MAX_MICRFRAME_VALUE + 1) & USB_HOST_EHCI_MAX_MICRFRAME_VALUE);	/* get the distance */
		/* shouldLinkUframe has add uframeInterval, think about the align with interval, so here add (uframeInterval
		 * * 2) */
		if ((distance <= (int32_t)(USB_HOST_EHCI_ISO_BOUNCE_UFRAME_NUMBER + (uframeInterval * 8))) && (distance > 2)) {
			currentUframe = shouldLinkUframe;
		} else {				/* re-link */

			currentUframe = ((uint32_t)(currentUframe + USB_HOST_EHCI_ISO_BOUNCE_UFRAME_NUMBER) & USB_HOST_EHCI_MAX_MICRFRAME_VALUE);
			if (currentUframe > (int32_t) USB_HOST_EHCI_MAX_MICRFRAME_VALUE) {
				currentUframe = currentUframe - (USB_HOST_EHCI_MAX_MICRFRAME_VALUE + 1);
			}
			/* uframe should align with interval */
			currentUframe -= startUframe;
			currentUframe = ((uint32_t)(currentUframe + uframeInterval) & (~((uint32_t) uframeInterval - 1)));	/* uframeInterval is power of 2 */
			currentUframe += startUframe;
		}
	} else {
		currentUframe = (gIMXRT1020_USB_HOST.usb->FRINDEX & USB_HOST_EHCI_MAX_MICRFRAME_VALUE);
		currentUframe = ((uint32_t)(currentUframe + USB_HOST_EHCI_ISO_BOUNCE_UFRAME_NUMBER) & USB_HOST_EHCI_MAX_MICRFRAME_VALUE);
		/* uframe should align with interval */
		currentUframe -= startUframe;
		currentUframe = ((uint32_t)(currentUframe + uframeInterval) & (~((uint32_t) uframeInterval - 1)));	/* uframeInterval is power of 2 */
		currentUframe += startUframe;
	}

	return currentUframe;
}

static usb_status_t USB_HostEhciItdArrayInit(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer)
{
	usb_host_ehci_iso_t *isoPointer;
	usb_host_ehci_itd_t *itdPointer = NULL;
	usb_host_ehci_itd_t *itdHead = NULL;
	usb_host_ehci_itd_t *tmpItdPointer;
	uint32_t dataLength;		/* the remaining data for sending */
	uint32_t transactionLength;	/* the initializing transaction descriptor data length */
	uint32_t itdBufferValue;
	uint32_t itdBufferBaseValue;	/* for calculating PG value */
	uint32_t lastShouldLinkUframe;
	uint32_t linkUframe;
	uint32_t minDataPerItd = ehciPipePointer->pipeCommon.numberPerUframe * ehciPipePointer->pipeCommon.maxPacketSize;
	uint8_t maxItdNumber;
	uint8_t index = 0;
	USB_SR_ALLOC();

	isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;

	/* max needed itd number, the actual needed number may be less because micro-frame interval may be less than 8 */
	maxItdNumber = ((transfer->transferLength - 1 + minDataPerItd) / minDataPerItd);
	if (ehciPipePointer->uframeInterval < 8) {
		maxItdNumber = ((maxItdNumber * ehciPipePointer->uframeInterval + 7) / 8) + 1;
	}

	if (maxItdNumber > gIMXRT1020_USB_HOST.ehciItdNumber) {
		return kStatus_USB_Error;
	}

	/* link transfer to usb_host_ehci_iso_t transfer list */
	transfer->next = NULL;
	USB_HostEhciLock();
	if (isoPointer->ehciTransferHead == NULL) {
		isoPointer->ehciTransferHead = isoPointer->ehciTransferTail = transfer;
	} else {
		isoPointer->ehciTransferTail->next = transfer;
		isoPointer->ehciTransferTail = transfer;
	}
	USB_HostEhciUnlock();

	dataLength = transfer->transferLength;
	transfer->union1.unitHead = (uint32_t) NULL;
	/* get the link micro-frame */
	lastShouldLinkUframe = USB_HostEhciGetItdLinkFrame(isoPointer->lastLinkFrame, (uint16_t)((ehciPipePointer->startFrame << 3) + ehciPipePointer->startUframe), ehciPipePointer->uframeInterval);
	if (lastShouldLinkUframe > USB_HOST_EHCI_MAX_MICRFRAME_VALUE) {
		linkUframe = lastShouldLinkUframe - (USB_HOST_EHCI_MAX_MICRFRAME_VALUE + 1);
	} else {
		linkUframe = lastShouldLinkUframe;
	}
	itdHead = gIMXRT1020_USB_HOST.ehciItdList;
	while (dataLength) {
		/* get one idle itd */
		tmpItdPointer = gIMXRT1020_USB_HOST.ehciItdList;
		gIMXRT1020_USB_HOST.ehciItdList = (usb_host_ehci_itd_t *) tmpItdPointer->nextLinkPointer;
		gIMXRT1020_USB_HOST.ehciItdNumber -= 1;
		if (tmpItdPointer == NULL) {
			return kStatus_USB_Error;	/* this should not reach */
		}
		tmpItdPointer->nextItdPointer = NULL;

		/* use the itd */
		if (transfer->union1.unitHead == (uint32_t) NULL) {	/* first itd */
			transfer->union1.unitHead = (uint32_t) tmpItdPointer;
		} else {				/* link itd list */

			itdPointer->nextItdPointer = tmpItdPointer;
		}
		itdPointer = tmpItdPointer;

		/* itd has been set to all zero when releasing */
		itdBufferBaseValue = itdBufferValue = (uint32_t)(transfer->transferBuffer + (transfer->transferLength - dataLength));
		for (index = 0; index < 7; ++index) {
			itdPointer->bufferPointers[index] = ((itdBufferBaseValue + (index * 4 * 1024)) & 0xFFFFF000U);
		}

		/* initialize iTD common fields */
		itdPointer->bufferPointers[0] |= (((uint32_t) ehciPipePointer->pipeCommon.endpointAddress << EHCI_HOST_ITD_ENDPT_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.deviceAddress << EHCI_HOST_ITD_DEVICE_ADDRESS_SHIFT));
		itdPointer->bufferPointers[1] |= (((uint32_t) ehciPipePointer->pipeCommon.direction << EHCI_HOST_ITD_DIRECTION_SHIFT) | ((uint32_t) ehciPipePointer->pipeCommon.maxPacketSize << EHCI_HOST_ITD_MAX_PACKET_SIZE_SHIFT));
		itdPointer->bufferPointers[2] |= (ehciPipePointer->pipeCommon.numberPerUframe);
		/* initialize transaction descriptors */
		for (index = (linkUframe & 0x0007); index < 8; index += ehciPipePointer->uframeInterval) {
			transactionLength = ((dataLength > minDataPerItd) ? minDataPerItd : dataLength);
			/* initialize the uframeIndex's transaction descriptor in itd */
			itdPointer->transactions[index] = ((EHCI_HOST_ITD_STATUS_ACTIVE_MASK) | (transactionLength << EHCI_HOST_ITD_TRANSACTION_LEN_SHIFT) | ((((itdBufferValue & 0xFFFFF000U) - (itdBufferBaseValue & 0xFFFFF000U)) >> EHCI_HOST_ITD_BUFFER_POINTER_SHIFT)
											   << EHCI_HOST_ITD_PG_SHIFT) | (itdBufferValue & EHCI_HOST_ITD_TRANSACTION_OFFSET_MASK));
			dataLength -= transactionLength;
			itdBufferValue += transactionLength;
			if (dataLength <= 0) {
				break;
			}
		}
	}

	transfer->union2.unitTail = (uint32_t) itdPointer;
	itdPointer->transactions[index] |= (1 << EHCI_HOST_ITD_IOC_SHIFT);	/* last set IOC */

	itdPointer = itdHead;
	/* link itd to frame list (note: initialize frameEntryIndex) */
	while (itdPointer) {
		uint32_t *linkPointer = &((uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList)[linkUframe >> 3];
		uint32_t linkValue = *linkPointer;
		itdPointer->frameEntryIndex = linkUframe >> 3;
		while ((!(linkValue & EHCI_HOST_T_INVALID_VALUE)) && ((linkValue & EHCI_HOST_POINTER_TYPE_MASK) == EHCI_HOST_POINTER_TYPE_ITD)) {
			linkPointer = (uint32_t *)(linkValue & EHCI_HOST_POINTER_ADDRESS_MASK);
			linkValue = *linkPointer;
		}
		itdPointer->nextLinkPointer = *linkPointer;
		*linkPointer = ((uint32_t) itdPointer | EHCI_HOST_POINTER_TYPE_ITD);
		itdPointer = itdPointer->nextItdPointer;
		if (itdPointer == NULL) {
			break;
		}

		linkUframe += ehciPipePointer->uframeInterval;
		lastShouldLinkUframe += ehciPipePointer->uframeInterval;
		if (linkUframe >= (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE << 3)) {
			linkUframe = (linkUframe - (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE << 3));
		}
	}

	if (lastShouldLinkUframe > USB_HOST_EHCI_MAX_MICRFRAME_VALUE) {
		lastShouldLinkUframe = lastShouldLinkUframe - (USB_HOST_EHCI_MAX_MICRFRAME_VALUE + 1);
	}
	isoPointer->lastLinkFrame = lastShouldLinkUframe;

	return kStatus_USB_Success;
}

static uint32_t USB_HostEhciItdArrayRelease(usb_host_ehci_itd_t *startItdPointer, usb_host_ehci_itd_t *endItdPointer)
{
	usb_host_ehci_itd_t *itdPointer = startItdPointer;
	uint8_t index;
	uint32_t doneLength = 0;
	USB_SR_ALLOC();
	/* remove itd from frame list */
	while (1) {
		/* record the transfer's result length */
		for (index = 0; index < 8; ++index) {
			doneLength += ((itdPointer->transactions[index] & EHCI_HOST_ITD_TRANSACTION_LEN_MASK) >> EHCI_HOST_ITD_TRANSACTION_LEN_SHIFT);
		}

		USB_HostEhciRemoveFromFrame((uint32_t) itdPointer, itdPointer->frameEntryIndex);	/* remove from the inserted frame list */

		/* release itd */
		USB_HostEhciLock();
		USB_HostEhciZeroMem((uint32_t *) itdPointer, sizeof(usb_host_ehci_itd_t) >> 2);
		itdPointer->nextLinkPointer = (uint32_t) gIMXRT1020_USB_HOST.ehciItdList;
		gIMXRT1020_USB_HOST.ehciItdList = itdPointer;
		gIMXRT1020_USB_HOST.ehciItdNumber++;
		USB_HostEhciUnlock();

		if (itdPointer == endItdPointer) {
			break;
		}
		itdPointer = itdPointer->nextItdPointer;
	}

	return doneLength;
}

static usb_status_t USB_HostEhciItdArrayDeinit(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_iso_t *isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;
	usb_host_transfer_t *transfer;
	usb_host_transfer_t *nextTransfer;
	uint32_t doneLength = 0;
	USB_SR_ALLOC();				//JS_TEST

	/* firstly remove the transfer (because host task may occupy to access the resource) */
	USB_HostEhciLock();
	transfer = isoPointer->ehciTransferHead;
	isoPointer->ehciTransferHead = isoPointer->ehciTransferTail = NULL;
	USB_HostEhciUnlock();

	while (transfer != NULL) {
		nextTransfer = transfer->next;
		doneLength = 0;
		/* remove itd from frame list and release itd */
		doneLength = USB_HostEhciItdArrayRelease((usb_host_ehci_itd_t *) transfer->union1.unitHead, (usb_host_ehci_itd_t *) transfer->union2.unitTail);

		/* transfer callback */
		if (ehciPipePointer->pipeCommon.direction == USB_OUT) {
			doneLength = transfer->transferLength;
		}
		transfer->transferSofar = doneLength;
		/* callback function is different from the current condition */
		transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferCancel);

		/* next transfer */
		transfer = nextTransfer;
	}

	return kStatus_USB_Success;
}
#endif							/* USB_HOST_CONFIG_EHCI_MAX_ITD */

static usb_status_t USB_HostEhciOpenControlBulk(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_qh_t *qhPointer;

	if (USB_HostEhciQhInit(ehciPipePointer) != kStatus_USB_Success) {	/* initialize control/bulk qh */
		return kStatus_USB_Error;
	}

	qhPointer = (usb_host_ehci_qh_t *) ehciPipePointer->ehciQh;

	/* add qh to async */
	qhPointer->horizontalLinkPointer = gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer;
	gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer = ((uint32_t) qhPointer | EHCI_HOST_POINTER_TYPE_QH);

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciCloseControlBulk(usb_host_ehci_pipe_t *ehciPipePointer)
{
	volatile usb_host_ehci_qh_t *vltPrevQhPointer;
	uint32_t horizontalLinkValue;

	/* remove qh from async schedule */
	if ((gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer & EHCI_HOST_POINTER_ADDRESS_MASK) == (uint32_t) ehciPipePointer->ehciQh) {	/* the removing qh is the first qh in the async list */
		USB_HostEhciStopAsync();
		gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer = ((usb_host_ehci_qh_t *) ehciPipePointer->ehciQh)->horizontalLinkPointer;
		USB_HostEhciStartAsync();
	} else {
		/* search for the removing qh from the async list */
		vltPrevQhPointer = gIMXRT1020_USB_HOST.shedFirstQh;
		while (vltPrevQhPointer != NULL) {
			horizontalLinkValue = vltPrevQhPointer->horizontalLinkPointer;
			if ((horizontalLinkValue & EHCI_HOST_T_INVALID_VALUE) || ((horizontalLinkValue & EHCI_HOST_POINTER_ADDRESS_MASK) == (uint32_t) ehciPipePointer->ehciQh) || ((horizontalLinkValue & EHCI_HOST_POINTER_ADDRESS_MASK) == (uint32_t) gIMXRT1020_USB_HOST.shedFirstQh)) {
				break;
			}

			vltPrevQhPointer = (volatile usb_host_ehci_qh_t *)(horizontalLinkValue & EHCI_HOST_POINTER_ADDRESS_MASK);
		}

		/* remove the qh from async list */
		if ((vltPrevQhPointer != NULL) && (!(horizontalLinkValue & EHCI_HOST_T_INVALID_VALUE)) && ((horizontalLinkValue & EHCI_HOST_POINTER_ADDRESS_MASK) == (uint32_t) ehciPipePointer->ehciQh)) {
			USB_HostEhciStopAsync();
			vltPrevQhPointer->horizontalLinkPointer = ((usb_host_ehci_qh_t *) ehciPipePointer->ehciQh)->horizontalLinkPointer;
			USB_HostEhciStartAsync();
		}
	}
	((usb_host_ehci_qh_t *) ehciPipePointer->ehciQh)->horizontalLinkPointer = EHCI_HOST_T_INVALID_VALUE;	/* invalid next qh link */
	return USB_HostEhciQhDeinit(ehciPipePointer);	/* de-initialize qh and release qh */
}

static usb_status_t USB_HostEhciOpenInterrupt(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_status_t status = kStatus_USB_Success;
	uint32_t frameIndex;

	/* allocate bandwidth */
	if (gIMXRT1020_USB_HOST.firstDeviceSpeed == IMXRT_USB_SPEED_HIGH) {
		status = USB_HostBandwidthHsHostAllocateInterrupt(ehciPipePointer);	/* host works as high-speed */
	} else {
		status = USB_HostBandwidthFslsHostAllocate(ehciPipePointer);	/* host works as full-speed or low-speed */
	}

	if (status != kStatus_USB_Success) {
		return status;
	}
	if (USB_HostEhciQhInit(ehciPipePointer) != kStatus_USB_Success) {
		return kStatus_USB_Error;
	}

	/* insert QH to frame list */
	for (frameIndex = ehciPipePointer->startFrame; frameIndex < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; frameIndex += ((ehciPipePointer->uframeInterval + 7) / 8)) {
		USB_HostEhciAddQhToFrame((uint32_t) ehciPipePointer->ehciQh, frameIndex, ehciPipePointer->uframeInterval);
	}

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciCloseInterrupt(usb_host_ehci_pipe_t *ehciPipePointer)
{
	uint32_t frameIndex;

	/* remove from frame list */
	for (frameIndex = ehciPipePointer->startFrame; frameIndex < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; frameIndex += ((ehciPipePointer->uframeInterval + 7) / 8)) {
		USB_HostEhciRemoveFromFrame((uint32_t) ehciPipePointer->ehciQh, frameIndex);
	}
	((usb_host_ehci_qh_t *) ehciPipePointer->ehciQh)->horizontalLinkPointer |= EHCI_HOST_T_INVALID_VALUE;	/* invalid next qh link */

	return USB_HostEhciQhDeinit(ehciPipePointer);	/* de-initilaze qh and release qh */
}

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))

static usb_status_t USB_HostEhciOpenIso(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_iso_t *isoPointer;
	usb_status_t status = kStatus_USB_Success;
	USB_SR_ALLOC();				//JS_TEST

	if (gIMXRT1020_USB_HOST.firstDeviceSpeed == IMXRT_USB_SPEED_HIGH) {
		status = USB_HostBandwidthHsHostAllocateIso(ehciPipePointer);	/* allocate iso bandwidth when host works as high-speed */
	} else {
		status = USB_HostBandwidthFslsHostAllocate(ehciPipePointer);	/* allocate iso bandwidth when host works as full-speed or low-speed */
	}

	if (status != kStatus_USB_Success) {
		return status;
	}

	/* get usb_host_ehci_iso_t */
	if (gIMXRT1020_USB_HOST.ehciIsoList == NULL) {
		return kStatus_USB_Error;
	}
	USB_HostEhciLock();
	isoPointer = gIMXRT1020_USB_HOST.ehciIsoList;
	gIMXRT1020_USB_HOST.ehciIsoList = gIMXRT1020_USB_HOST.ehciIsoList->next;
	USB_HostEhciUnlock();
	isoPointer->lastLinkFrame = 0xFFFF;
	ehciPipePointer->ehciQh = isoPointer;

	return status;
}

static usb_status_t USB_HostEhciCloseIso(usb_host_ehci_pipe_t *ehciPipePointer)
{
	usb_host_ehci_iso_t *isoPointer;
	USB_SR_ALLOC();				//JS_TEST

	isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;

	if (isoPointer->ehciTransferHead != NULL) {
		if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
			USB_HostEhciItdArrayDeinit(ehciPipePointer);	/* de-initialize itd list and free them */
#endif
		} else {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
			USB_HostEhciSitdArrayDeinit(ehciPipePointer);	/* de-initialize sitd list and free them */
#endif
		}
	}

	/* release usb_host_ehci_iso_t */
	USB_HostEhciLock();
	isoPointer->next = gIMXRT1020_USB_HOST.ehciIsoList;
	gIMXRT1020_USB_HOST.ehciIsoList = isoPointer;
	USB_HostEhciUnlock();
	return kStatus_USB_Success;
}

#endif

static usb_status_t USB_HostEhciResetIP()
{
	uint32_t tmp;
	/* reset controller */

	gIMXRT1020_USB_HOST.usb->USBCMD = USBHS_USBCMD_RST_MASK;
	while (gIMXRT1020_USB_HOST.usb->USBCMD & USBHS_USBCMD_RST_MASK) {
	}

	// setup interrupt interval
	tmp = gIMXRT1020_USB_HOST.usb->USBCMD;
	tmp &= (~USBHS_USBCMD_ITC_MASK);
	gIMXRT1020_USB_HOST.usb->USBCMD = tmp | USB_USBCMD_ITC(USB_HOST_INTERRUPT_THRESHOLD);

	/* set host mode */
	gIMXRT1020_USB_HOST.usb->USBMODE |= 0x03;

	/* check frame list size */
	if (!(gIMXRT1020_USB_HOST.usb->HCCPARAMS & USBHS_HCCPARAMS_PFL_MASK)) {
#if ((USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE < 8) || (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE > 1024))
		return kStatus_USB_Error;
#endif
#if (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE & (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE - 1))
		return kStatus_USB_Error;	/* frame size must be 1024/512/256/128/64/32/16/8 */
#endif
	}
	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciStartIP()
{
	uint32_t tmp;
	if (gIMXRT1020_USB_HOST.usb->HCSPARAMS & USBHS_HCSPARAMS_PPC_MASK) {	/* Ports have power port switches */
		/* only has one port */
		tmp = gIMXRT1020_USB_HOST.usb->PORTSC1;
		tmp &= (~EHCI_PORTSC1_W1_BITS);
		gIMXRT1020_USB_HOST.usb->PORTSC1 = (tmp | USBHS_PORTSC1_PP_MASK);	/* turn on port power */
	}

	/* set frame list size */
	if (gIMXRT1020_USB_HOST.usb->HCCPARAMS & USBHS_HCCPARAMS_PFL_MASK) {
		// clear FS_2 & FS_1 from USBCMD register
		tmp = gIMXRT1020_USB_HOST.usb->USBCMD;
		tmp &= ~(USBHS_USBCMD_FS2_MASK | USB_USBCMD_FS_1_MASK);
		gIMXRT1020_USB_HOST.usb->USBCMD = tmp;

#if (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE <= 64)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (USBHS_USBCMD_FS2_MASK);
#if (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 64)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x00 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 32)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x01 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 16)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x02 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 8)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x03 << USBHS_USBCMD_FS_SHIFT);
#endif
#else
#if (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 1024)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x00 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 512)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x01 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 256)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x02 << USBHS_USBCMD_FS_SHIFT);
#elif (USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE == 128)
		gIMXRT1020_USB_HOST.usb->USBCMD |= (0x03 << USBHS_USBCMD_FS_SHIFT);
#endif
#endif
	}

	/* start the controller */
	gIMXRT1020_USB_HOST.usb->USBCMD |= USBHS_USBCMD_RS_MASK;

	/* set timer0 */
	gIMXRT1020_USB_HOST.usb->GPTIMER0LD = (100 * 1000 - 1);	/* 100ms */

	/* enable interrupt (USB interrupt enable + USB error interrupt enable + port change detect enable + system error
	 * enable + interrupt on async advance enable) + general purpos Timer 0 Interrupt enable */
	gIMXRT1020_USB_HOST.usb->USBINTR |= (0x1000037);

	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciCancelPipe(usb_host_ehci_pipe_t *ehciPipePointer, usb_host_transfer_t *transfer)
{
	usb_host_ehci_qh_t *qhPointer;
#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	usb_host_ehci_iso_t *isoPointer;
#endif
	uint8_t cancelPipe = 0;

	switch (ehciPipePointer->pipeCommon.pipeType) {
	case USB_ENDPOINT_BULK:
	case USB_ENDPOINT_CONTROL:
	case USB_ENDPOINT_INTERRUPT:
		qhPointer = (usb_host_ehci_qh_t *) ehciPipePointer->ehciQh;
		if (qhPointer->ehciTransferHead == NULL) {	/* there is no transfer to cancel */
			return kStatus_USB_Success;
		}
		if (transfer != NULL) {
			if ((qhPointer->ehciTransferHead == transfer) && (qhPointer->ehciTransferHead == qhPointer->ehciTransferTail)) {	/* only has this one transfer */
				cancelPipe = 1;
			} else {
				cancelPipe = 0;
			}
		} else {
			cancelPipe = 1;
		}
		if (cancelPipe == 1) {	/* cancel all pipe */
			USB_HostEhciQhQtdListDeinit(ehciPipePointer);	/* release all the qtd */
		} else {				/* cancel one transfer */

			USB_HostEhciTransferQtdListDeinit(ehciPipePointer, transfer);
		}
		break;

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	 ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	case USB_ENDPOINT_ISOCHRONOUS:
		isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;
		if (isoPointer->ehciTransferHead == NULL) {	/* there is no transfer to cancel */
			return kStatus_USB_Success;
		}
		/* cancel all pipe, don't implement canceling transfer for iso */
		if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
			USB_HostEhciItdArrayDeinit(ehciPipePointer);	/* de-initialize itd */
#endif
		} else {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
			USB_HostEhciSitdArrayDeinit(ehciPipePointer);	/* de-initialize sitd */
#endif
		}
		break;
#endif

	default:
		break;
	}
	return kStatus_USB_Success;
}

static usb_status_t USB_HostEhciControlBus(uint8_t busControl)
{
	usb_status_t status = kStatus_USB_Success;
	uint32_t portScRegister;

	switch (busControl) {
	case kUSB_HostBusReset:
		/* reset port */
		portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
		portScRegister &= (~EHCI_PORTSC1_W1_BITS);
		gIMXRT1020_USB_HOST.usb->PORTSC1 = (portScRegister | USBHS_PORTSC1_PR_MASK);
		while (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_PR_MASK) {
		}
		break;

	case kUSB_HostBusRestart:
		gIMXRT1020_USB_HOST.deviceAttached = kEHCIDeviceDetached;
		gIMXRT1020_USB_HOST.usb->USBINTR |= (USBHS_USBINTR_PCE_MASK);	/* enable ehci port change interrupt */
		break;

	case kUSB_HostBusEnableAttach:	/* enable device attach */
		if (gIMXRT1020_USB_HOST.deviceAttached == kEHCIDeviceDetached) {
			gIMXRT1020_USB_HOST.usb->USBINTR |= (USBHS_USBINTR_PCE_MASK);	/* enable ehci port change interrupt */
		}
		break;

	case kUSB_HostBusDisableAttach:	/* disable device attach */
		gIMXRT1020_USB_HOST.usb->USBINTR &= (~USBHS_USBINTR_PCE_MASK);	/* disable ehci port change interrupt */
		break;
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
	case kUSB_HostBusSuspend:
		if (gIMXRT1020_USB_HOST.usb->PORTSC1 && USBHS_PORTSC1_CCS_MASK) {
			/* set timer1 */
			gIMXRT1020_USB_HOST.usb->GPTIMER1LD = (1 * 1000);	/* 1ms */
			gIMXRT1020_USB_HOST.usb->GPTIMER1CTL |= (USBHS_GPTIMER0CTL_RUN_MASK | USBHS_GPTIMER0CTL_MODE_MASK | USBHS_GPTIMER0CTL_RST_MASK);

			USB_HostEhciStopAsync();
			USB_HostEhciStopPeriodic();
			while (gIMXRT1020_USB_HOST.usb->USBSTS & (USBHS_USBSTS_PS_MASK | USBHS_USBSTS_AS_MASK)) {
				__asm("nop");
			}
			gIMXRT1020_USB_HOST.usb->PORTSC1 &= ~USBHS_PORTSC1_WKCN_MASK;
			gIMXRT1020_USB_HOST.usb->PORTSC1 |= USBHS_PORTSC1_WKDS_MASK;
			gIMXRT1020_USB_HOST.usb->PORTSC1 |= (USBHS_PORTSC1_SUSP_MASK);	/* Suspend the device */
			gIMXRT1020_USB_HOST.matchTick = 0U;
			gIMXRT1020_USB_HOST.usb->USBINTR |= (USBHS_USBINTR_TIE1_MASK);
			gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciStartSuspend;
		} else {
			status = kStatus_USB_Error;
		}
		break;
	case kUSB_HostBusResume:
		gIMXRT1020_USB_HOST.usb->PORTSC1 &= ~(USBHS_PORTSC1_SUSP_MASK);	/* Clear Suspend bit */
		gIMXRT1020_USB_HOST.usb->PORTSC1 &= ~USBHS_PORTSC1_PHCD_MASK;
		if (gIMXRT1020_USB_HOST.deviceAttached != kEHCIDeviceDetached) {
			gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciStartResume;
			gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_CTRL &= ~USBNC_USB_OTGn_CTRL_WIE_MASK;

			gIMXRT1020_USB_HOST.usb->USBCMD |= (USBHS_USBCMD_RS_MASK);
			gIMXRT1020_USB_HOST.usb->PORTSC1 |= (USBHS_PORTSC1_FPR_MASK);	/* Resume the device */
		} else {
			status = kStatus_USB_Error;
		}
		break;
#endif
	default:
		status = kStatus_USB_Error;
		break;
	}
	return status;
}

void USB_HostEhciTransactionDone()
{
	/* process async QH */
	usb_host_ehci_pipe_t *ehciPipePointer;
	usb_host_ehci_pipe_t *ehciClearPipePointer = NULL;
	volatile usb_host_ehci_qh_t *vltQhPointer;
	volatile usb_host_ehci_qtd_t *vltQtdPointer;
	usb_host_transfer_t *transfer;
	usb_host_transfer_t *nextTransfer;
	uint32_t qtdStatus = 0;
#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
	volatile usb_host_ehci_itd_t *vltItdPointer;
	uint8_t index = 0;
#endif
#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
	volatile usb_host_ehci_sitd_t *vltSitdPointer;
#endif
#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	 ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	usb_host_ehci_iso_t *isoPointer;
	uint32_t dataLength;
#endif

	ehciPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;	/* check all the running pipes */
	while (ehciPipePointer != NULL) {
		switch (ehciPipePointer->pipeCommon.pipeType) {
		case USB_ENDPOINT_BULK:
		case USB_ENDPOINT_INTERRUPT:
		case USB_ENDPOINT_CONTROL:
			vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;	/* pipe's qh */
			transfer = vltQhPointer->ehciTransferHead;	/* qh's transfer */
			while (transfer != NULL) {
				nextTransfer = transfer->next;
				/* normal case */
				vltQtdPointer = (volatile usb_host_ehci_qtd_t *)transfer->union2.unitTail;
				if ((vltQtdPointer->transferResults[0] & (EHCI_HOST_QTD_IOC_MASK)) && (!(vltQtdPointer->transferResults[0] & EHCI_HOST_QTD_STATUS_ACTIVE_MASK))) {	/* transfer is done */
					qtdStatus = (vltQtdPointer->transferResults[0] & EHCI_HOST_QTD_STATUS_ERROR_MASK);
					transfer->transferSofar = USB_HostEhciQtdListRelease((usb_host_ehci_qtd_t *)(transfer->union1.unitHead), (usb_host_ehci_qtd_t *)(transfer->union2.unitTail));
					transfer->transferSofar = (transfer->transferLength < transfer->transferSofar) ? 0 : (transfer->transferLength - transfer->transferSofar);

					vltQhPointer->ehciTransferHead = transfer->next;
					vltQhPointer->timeOutLabel = 0;
					vltQhPointer->timeOutValue = USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE;
					if (qtdStatus) {	/* has errors */
						if (!(vltQhPointer->transferOverlayResults[0] & EHCI_HOST_QTD_STATUS_ACTIVE_MASK)) {
							vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
						}
						if (qtdStatus & EHCI_HOST_QH_STATUS_NOSTALL_ERROR_MASK) {
							/* callback function is different from the current condition */
							transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferFailed);	/* transfer fail */
						} else {
							/* callback function is different from the current condition */
							transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferStall);
						}
					} else {
						if ((ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_CONTROL) && (transfer->setupPacket->bRequest == USB_REQUEST_STANDARD_CLEAR_FEATURE) && (transfer->setupPacket->bmRequestType == USB_REQUEST_TYPE_RECIPIENT_ENDPOINT) && ((USB_SHORT_FROM_LITTLE_ENDIAN(transfer->setupPacket->wValue) & 0x00FFu) == USB_REQUEST_STANDARD_FEATURE_SELECTOR_ENDPOINT_HALT)) {
							ehciClearPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;
							while (ehciClearPipePointer != NULL) {
								/* only compute bulk and interrupt pipe */
								if (((ehciClearPipePointer->pipeCommon.endpointAddress | (ehciClearPipePointer->pipeCommon.direction << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)) == (uint8_t)(USB_SHORT_FROM_LITTLE_ENDIAN(transfer->setupPacket->wIndex))) && (ehciClearPipePointer->pipeCommon.hubNumber == ehciPipePointer->pipeCommon.hubNumber) &&	// To check same device JS_CHECK
									(ehciClearPipePointer->pipeCommon.portNumber == ehciPipePointer->pipeCommon.portNumber) &&	// To check same device JS_CHECK
									(ehciClearPipePointer->pipeCommon.hsHubNumber == ehciPipePointer->pipeCommon.hsHubNumber) &&	// To check same device JS_CHECK
									(ehciClearPipePointer->pipeCommon.hsHubPort == ehciPipePointer->pipeCommon.hsHubPort) &&	// To check same device JS_CHECK
									(ehciClearPipePointer->pipeCommon.level == ehciPipePointer->pipeCommon.level) &&	// To check same device JS_CHECK
									(ehciClearPipePointer->pipeCommon.deviceAddress == ehciPipePointer->pipeCommon.deviceAddress)	// To check same device JS_CHECK
								   ) {
									break;
								}
								ehciClearPipePointer = (usb_host_ehci_pipe_t *) ehciClearPipePointer->pipeCommon.next;
							}

							if ((ehciClearPipePointer != NULL) && ((ehciClearPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT) || (ehciClearPipePointer->pipeCommon.pipeType == USB_ENDPOINT_BULK))) {
								((volatile usb_host_ehci_qh_t *)(ehciClearPipePointer->ehciQh))->transferOverlayResults[0] &= (~EHCI_HOST_QTD_DT_MASK);
							}
						}
						/* callback function is different from the current condition */
						transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_Success);	/* transfer success */
					}
				} else if ((!(vltQhPointer->transferOverlayResults[0] & EHCI_HOST_QTD_STATUS_ACTIVE_MASK)) && (vltQhPointer->transferOverlayResults[0] & EHCI_HOST_QH_STATUS_ERROR_MASK)) {	/* there is error and transfer is done */
					qtdStatus = (vltQhPointer->transferOverlayResults[0] & EHCI_HOST_QH_STATUS_ERROR_MASK);
					vltQtdPointer = (volatile usb_host_ehci_qtd_t *)(vltQhPointer->currentQtdPointer);

					if (((uint32_t) vltQtdPointer & EHCI_HOST_T_INVALID_VALUE) || (vltQtdPointer == NULL)) {	/* the error status is unreasonable */
						vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
					} else {
						/* remove qtd from qh */
						while ((vltQtdPointer != NULL) && (!(vltQtdPointer->transferResults[0] & EHCI_HOST_QTD_IOC_MASK))) {	/* find the IOC qtd */
							vltQtdPointer = (volatile usb_host_ehci_qtd_t *)vltQtdPointer->nextQtdPointer;
						}

						vltQhPointer->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
						vltQhPointer->currentQtdPointer = EHCI_HOST_T_INVALID_VALUE;
						vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
						if (vltQtdPointer != NULL) {
							vltQhPointer->nextQtdPointer = vltQtdPointer->nextQtdPointer;
						}

						transfer->transferSofar = USB_HostEhciQtdListRelease((usb_host_ehci_qtd_t *)(transfer->union1.unitHead), (usb_host_ehci_qtd_t *)(transfer->union2.unitTail));
						transfer->transferSofar = (transfer->transferLength < transfer->transferSofar) ? 0 : (transfer->transferLength - transfer->transferSofar);
						vltQhPointer->ehciTransferHead = transfer->next;
						vltQhPointer->timeOutLabel = 0;
						vltQhPointer->timeOutValue = USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE;
						if (qtdStatus & EHCI_HOST_QH_STATUS_NOSTALL_ERROR_MASK) {
							/* callback function is different from the current condition */
							transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferFailed);	/* transfer fail */
						} else {
							/* callback function is different from the current condition */
							transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferStall);	/* transfer stall */
						}
					}
				} else {
					break;
				}
				transfer = nextTransfer;
			}
			break;
#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
		case USB_ENDPOINT_ISOCHRONOUS:
			qtdStatus = 0;		/* qtdStatus means break here, because there is only one break in while for misra */
			isoPointer = (usb_host_ehci_iso_t *) ehciPipePointer->ehciQh;	/* pipe's usb_host_ehci_iso_t */
			transfer = isoPointer->ehciTransferHead;	/* usb_host_ehci_iso_t's transfer */
			while (transfer != NULL) {
				nextTransfer = transfer->next;
				if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
					vltItdPointer = (volatile usb_host_ehci_itd_t *)(transfer->union2.unitTail);	/* transfer's last itd */
					for (index = 0; index < 8; ++index) {
						if (vltItdPointer->transactions[index] & EHCI_HOST_ITD_STATUS_ACTIVE_MASK) {
							break;
						}
					}
					if (index == 8) {	/* transfer is done */
						/* remove itd from frame list and release itd */
						dataLength = USB_HostEhciItdArrayRelease((usb_host_ehci_itd_t *) transfer->union1.unitHead, (usb_host_ehci_itd_t *) transfer->union2.unitTail);
						transfer->transferSofar = dataLength;
						isoPointer->ehciTransferHead = transfer->next;
						/* callback function is different from the current condition */
						transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_Success);	/* transfer callback success */
						/* TODO: iso callback error */
					} else {
						qtdStatus = 1;	/* break */
					}
#endif
				} else {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
					vltSitdPointer = (volatile usb_host_ehci_sitd_t *)(transfer->union2.unitTail);	/* transfer's last sitd */
					if (!(vltSitdPointer->transferResults[0] & EHCI_HOST_SITD_STATUS_ACTIVE_MASK)) {	/* transfer is done */
						/* remove sitd from frame list and release itd */
						dataLength = USB_HostEhciSitdArrayRelease((usb_host_ehci_sitd_t *) transfer->union1.unitHead, (usb_host_ehci_sitd_t *) transfer->union2.unitTail);
						transfer->transferSofar = transfer->transferLength - dataLength;
						isoPointer->ehciTransferHead = transfer->next;
						/* callback function is different from the current condition */
						transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_Success);	/* transfer callback success */
						/* TODO: iso callback error */
					} else {
						qtdStatus = 1;	/* break */
					}
#endif
				}
				if (qtdStatus == 1) {
					break;
				}
				transfer = nextTransfer;
			}
			break;
#endif

		default:
			break;
		}
		ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}
}

static void USB_HostEhciPortChange()
{
	/* note: only has one port */
	uint32_t portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
	int32_t sofStart = 0;
	int32_t sofCount = 0;
	uint32_t index;
	uint32_t event_triggered = 0;
	int ret;

	if (portScRegister & USBHS_PORTSC1_CSC_MASK) {	/* connection status change */
		sofStart = (int32_t)(gIMXRT1020_USB_HOST.usb->FRINDEX & EHCI_MAX_UFRAME_VALUE);

		/* process CSC bit */
		while (1) {
			portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
			if (portScRegister & USBHS_PORTSC1_CSC_MASK) {
				/* clear csc bit */
				portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
				portScRegister &= (~EHCI_PORTSC1_W1_BITS);
				gIMXRT1020_USB_HOST.usb->PORTSC1 = (portScRegister | USBHS_PORTSC1_CSC_MASK);
			}
			sofCount = (int32_t)(gIMXRT1020_USB_HOST.usb->FRINDEX & EHCI_MAX_UFRAME_VALUE);
			if (((sofCount - sofStart + EHCI_MAX_UFRAME_VALUE + 1) & EHCI_MAX_UFRAME_VALUE) > (1 * 8)) {	/* delay 1ms to clear CSC */
				break;
			}
		}
	}

	/* process CCS bit */
	portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
	if (portScRegister & USBHS_PORTSC1_CCS_MASK) {	/* process attach */
		if ((gIMXRT1020_USB_HOST.deviceAttached == kEHCIDevicePhyAttached) || (gIMXRT1020_USB_HOST.deviceAttached == kEHCIDeviceAttached)) {
			return;
		}
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
		gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
		gIMXRT1020_USB_HOST.usb->USBINTR &= ~(USBHS_USBINTR_TIE1_MASK);
#endif
		for (index = 0; index < USB_HOST_EHCI_PORT_CONNECT_DEBOUNCE_DELAY; ++index) {
			USB_HostEhciDelay(1);
			if (!(gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_CCS_MASK)) {
				break;
			}
		}
		if (index < USB_HOST_EHCI_PORT_CONNECT_DEBOUNCE_DELAY) {	/* CCS is cleared */
			gIMXRT1020_USB_HOST.deviceAttached = kEHCIDeviceDetached;
			return;
		}

		/* reset port */
		portScRegister = gIMXRT1020_USB_HOST.usb->PORTSC1;
		portScRegister &= (~EHCI_PORTSC1_W1_BITS);
		gIMXRT1020_USB_HOST.usb->PORTSC1 = (portScRegister | USBHS_PORTSC1_PR_MASK);
		while (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_PR_MASK) {
		}

		gIMXRT1020_USB_HOST.firstDeviceSpeed = ((gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_PSPD_MASK) >> USBHS_PORTSC1_PSPD_SHIFT);
		/* enable ehci phy disconnection */
		if (gIMXRT1020_USB_HOST.firstDeviceSpeed == IMXRT_USB_SPEED_HIGH) {
			USB_EhcihostPhyDisconnectDetectCmd(1);
		}

		/* wait for reset */
		USB_HostEhciDelay(USB_HOST_EHCI_PORT_RESET_DELAY);

		/* process attach */
		USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_DEVICE_ATTACH);
		event_triggered++;

		/* gpt timer start */
		gIMXRT1020_USB_HOST.usb->GPTIMER0CTL |= (USBHS_GPTIMER0CTL_RUN_MASK | USBHS_GPTIMER0CTL_MODE_MASK | USBHS_GPTIMER0CTL_RST_MASK);
		gIMXRT1020_USB_HOST.deviceAttached = kEHCIDevicePhyAttached;
	} else {
		if ((gIMXRT1020_USB_HOST.deviceAttached == kEHCIDevicePhyAttached) || (gIMXRT1020_USB_HOST.deviceAttached == kEHCIDeviceAttached)) {
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
			gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
			gIMXRT1020_USB_HOST.usb->USBINTR &= ~(USBHS_USBINTR_TIE1_MASK);
#endif
			/* disable ehci phy disconnection */
			USB_EhcihostPhyDisconnectDetectCmd(0);
			/* disable async and periodic */
			USB_HostEhciStopAsync();
			USB_HostEhciStopPeriodic();

			USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_DEVICE_DETACH);
			event_triggered++;
		}
	}

	if (event_triggered) {
#ifdef USB_TIZEN_RT
		if (work_available(&(gIMXRT1020_USB_HOST.ehciEventTrigger))) {
			if (work_queue(HPWORK, &(gIMXRT1020_USB_HOST.ehciEventTrigger), USB_HostEhciTaskFunction, (void *)NULL, 0) != 0) {
				udbg("ERROR: gIMXRT1020_USB_HOST.ehciEventTrigger failed to queue work\n");
			}
		}
#else
		USB_EHCI_EVENT_TRIGGER_SEND(gIMXRT1020_USB_HOST.ehciEventTrigger);
#endif
	}
}

static void USB_HostEhciTimer0()
{
	volatile usb_host_ehci_qh_t *vltQhPointer;
	volatile usb_host_ehci_qtd_t *vltQtdPointer;
	usb_host_transfer_t *transfer;
	uint32_t backValue;
	volatile uint32_t *totalBytesAddress = NULL;
	usb_host_ehci_pipe_t *ehciPipePointer = gIMXRT1020_USB_HOST.ehciRunningPipeList;
	uint8_t timeoutLabel;

	while (ehciPipePointer != NULL) {
		switch (ehciPipePointer->pipeCommon.pipeType) {
		case USB_ENDPOINT_BULK:
		case USB_ENDPOINT_CONTROL:
			vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;	/* pipe's qh */
			transfer = vltQhPointer->ehciTransferHead;	/* qh's transfer */
			if ((transfer != NULL)) {	/* there is transferring data */
				timeoutLabel = 0;
				if (gIMXRT1020_USB_HOST.deviceAttached != kEHCIDeviceAttached) {
					vltQtdPointer = (volatile usb_host_ehci_qtd_t *)transfer->union2.unitTail;

					vltQhPointer->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;	/* invalid next qtd */
					vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
					timeoutLabel = 1;
				} else {
					if (vltQhPointer->transferOverlayResults[0] & EHCI_HOST_QTD_STATUS_ACTIVE_MASK) {
						vltQtdPointer = (volatile usb_host_ehci_qtd_t *)vltQhPointer->currentQtdPointer;
						totalBytesAddress = &(vltQhPointer->transferOverlayResults[0]);
					} else {
						vltQtdPointer = (volatile usb_host_ehci_qtd_t *)transfer->union2.unitTail;
						totalBytesAddress = ((uint32_t *) vltQtdPointer + 2);
					}

					backValue = (((*totalBytesAddress) & EHCI_HOST_QTD_TOTAL_BYTES_MASK) >> EHCI_HOST_QTD_TOTAL_BYTES_SHIFT);	/* backValue is used for total bytes to transfer */
					if (vltQhPointer->timeOutLabel != backValue) {	/* use total bytes to reflect the time out */
						vltQhPointer->timeOutValue = USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE;
						vltQhPointer->timeOutLabel = backValue;
					} else {
						/* time out when the total bytes don't change for the duration
						 * USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE
						 */
						(vltQhPointer->timeOutValue)--;
						if (vltQhPointer->timeOutValue == 0) {
							/* stop the qh schedule */
							USB_HostEhciStopAsync();
							if (backValue != (((*totalBytesAddress) & EHCI_HOST_QTD_TOTAL_BYTES_MASK) >> EHCI_HOST_QTD_TOTAL_BYTES_SHIFT)) {
								USB_HostEhciStartAsync();
							} else {
								vltQhPointer->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;	/* invalid next qtd */
								vltQhPointer->transferOverlayResults[0] &= (~EHCI_HOST_QTD_STATUS_MASK);	/* clear error status */
								USB_HostEhciStartAsync();
								timeoutLabel = 1;
							}
						}
					}
				}

				if (timeoutLabel == 1) {
					/* remove qtd from qh */
					while ((vltQtdPointer != NULL) && (!(vltQtdPointer->transferResults[0] & EHCI_HOST_QTD_IOC_MASK)) && (vltQtdPointer != (usb_host_ehci_qtd_t *) vltQhPointer->ehciTransferTail)) {
						vltQtdPointer = (volatile usb_host_ehci_qtd_t *)vltQtdPointer->nextQtdPointer;
					}
					if ((vltQtdPointer != NULL) && (!(vltQtdPointer->nextQtdPointer & EHCI_HOST_T_INVALID_VALUE))) {
						vltQhPointer->nextQtdPointer = vltQtdPointer->nextQtdPointer;	/* start qh if there are other qtd that don't belong to the transfer */
					}
					transfer->transferSofar = USB_HostEhciQtdListRelease((usb_host_ehci_qtd_t *)(transfer->union1.unitHead), (usb_host_ehci_qtd_t *)(transfer->union2.unitTail));
					transfer->transferSofar = (transfer->transferLength < transfer->transferSofar) ? 0 : (transfer->transferLength - transfer->transferSofar);

					vltQhPointer->ehciTransferHead = transfer->next;
					vltQhPointer->timeOutValue = USB_HOST_EHCI_CONTROL_BULK_TIME_OUT_VALUE;
					/* callback function is different from the current condition */
					transfer->callbackFn(transfer->callbackParam, transfer, kStatus_USB_TransferFailed);
				}
			}
			break;
		default:
			break;
		}
		ehciPipePointer = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}
}

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
static void USB_HostEhciTimer1()
{
	if (gIMXRT1020_USB_HOST.deviceAttached != kEHCIDeviceDetached) {
		if (kBus_EhciStartSuspend == gIMXRT1020_USB_HOST.busSuspendStatus) {
			if (0 == gIMXRT1020_USB_HOST.matchTick) {
				gIMXRT1020_USB_HOST.matchTick = gIMXRT1020_USB_HOST.hwTick;
			} else {
				if ((gIMXRT1020_USB_HOST.hwTick - gIMXRT1020_USB_HOST.matchTick) >= 5) {
					gIMXRT1020_USB_HOST.usb->USBCMD &= ~USBHS_USBCMD_RS_MASK;
					gIMXRT1020_USB_HOST.usb->USBSTS |= USBHS_USBSTS_SRI_MASK;
#if 0
					gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_ENVBUSCHG_WKUP_MASK | USBPHY_CTRL_ENIDCHG_WKUP_MASK | USBPHY_CTRL_ENDPDMCHG_WKUP_MASK | USBPHY_CTRL_ENIRQRESUMEDETECT_MASK;
#endif

					gIMXRT1020_USB_HOST.usb->PORTSC1 |= USBHS_PORTSC1_PHCD_MASK;

					gIMXRT1020_USB_HOST.usb_phy->PWD = 0xFFFFFFFFU;

					while (gIMXRT1020_USB_HOST.usb_phy->CTRL & (USBPHY_CTRL_UTMI_SUSPENDM_MASK)) {
						__asm("nop");
					}

					gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_CTRL |= USBNC_USB_OTGn_CTRL_WKUP_ID_EN_MASK | USBNC_USB_OTGn_CTRL_WKUP_VBUS_EN_MASK | USBNC_USB_OTGn_CTRL_WKUP_DPDM_EN_MASK;
					gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_CTRL |= USBNC_USB_OTGn_CTRL_WIE_MASK;

					gIMXRT1020_USB_HOST.usb_phy->CTRL |= USBPHY_CTRL_CLKGATE_MASK;
					gIMXRT1020_USB_HOST.deviceCallback(gIMXRT1020_USB_HOST.suspendedDevice, NULL, kUSB_HostEventSuspended);	/* call host callback function */
					gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciSuspended;
				}
			}
		} else if (kBus_EhciStartResume == gIMXRT1020_USB_HOST.busSuspendStatus) {
			if (!(gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_FPR_MASK)) {
				gIMXRT1020_USB_HOST.usb->PORTSC1 &= ~USBHS_PORTSC1_WKDS_MASK;
				if (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_CCS_MASK) {
					USB_HostEhciStartAsync();
					USB_HostEhciStartPeriodic();
				}
				gIMXRT1020_USB_HOST.deviceCallback(gIMXRT1020_USB_HOST.suspendedDevice, NULL, kUSB_HostEventResumed);	/* call host callback function */
				gIMXRT1020_USB_HOST.suspendedDevice = NULL;
				gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
				gIMXRT1020_USB_HOST.usb->USBINTR &= ~(USBHS_USBINTR_TIE1_MASK);
			}
		} else {
		}
	} else {
		gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
		gIMXRT1020_USB_HOST.usb->USBINTR &= ~(USBHS_USBINTR_TIE1_MASK);
	}
}
#endif

#ifndef USB_TIZEN_RT
static void USB_HostEhciEventTask(void *param)
{
	while (1) {
		if (USB_EHCI_EVENT_TRIGGER_RECV(gIMXRT1020_USB_HOST.ehciEventTrigger) == USB_OK) {
			USB_HostEhciTaskFunction(NULL);
		}
	}
}
#endif

uint8_t GetDes8_req[8] = {
	0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00
};

USB_RAM_ADDRESS_ALIGNMENT(USB_CACHE_LINESIZE)
uint8_t usb_descriptor_buffer[sizeof(usb_descriptor_device_t)];

#ifdef USB_TIZEN_RT
static int ehci_waiter(int argc, char *argv[])
#else
static void ehci_waiter(void *param)
#endif
{
	uint32_t index = 0;
	usb_status_t ret;
	uint8_t msg;
	pid_t pid;
	USB_SR_ALLOC();

	uvdbg("ehci_waiter:  Running\n");
	for (;;) {
		if (USB_EHCI_EVENT_TRIGGER_RECV(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger) == USB_OK) {
			while (USB_EVENT_FIFO_POP(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo), &msg) == USB_OK) {
				switch (msg) {
				case USB_HUB_EVENT_ATTACHED:
					uvdbg("usb hub enumerate - called!!!\n");
					FAR struct usbhost_hubport_s *hport;
					USB_HostEhciLock();
					hport = g_USBHOST_HUBPORT;
					g_USBHOST_HUBPORT = NULL;
					USB_HostEhciUnlock();
					if (usbhost_enumerate(hport, &(hport->devclass)) < 0) {
						uvdbg("usb hub enumerate failed!!!\n");
						hport->connected = false;
					}
					break;
				case USB_HOST_EVENT_ATTACHED:
					// control pipe update
					g_USBHOST_ROOT_HUB_PORT.hport.ep0 = gIMXRT1020_USB_HOST.controlPipe;
					g_USBHOST_ROOT_HUB_PORT.hport.connected = true;

					switch (gIMXRT1020_USB_HOST.firstDeviceSpeed) {
					case IMXRT_USB_SPEED_LOW:
						uvdbg("gIMXRT1020_USB_HOST.firstDeviceSpeed = LOW!!!\n");
						g_USBHOST_ROOT_HUB_PORT.hport.speed = USB_SPEED_LOW;
						break;
					case IMXRT_USB_SPEED_FULL:
						uvdbg("gIMXRT1020_USB_HOST.firstDeviceSpeed = FULL!!!\n");
						g_USBHOST_ROOT_HUB_PORT.hport.speed = USB_SPEED_FULL;
						break;
					case IMXRT_USB_SPEED_HIGH:
						g_USBHOST_ROOT_HUB_PORT.hport.speed = USB_SPEED_HIGH;
						uvdbg("gIMXRT1020_USB_HOST.firstDeviceSpeed = HIGH!!!\n");
						break;
					default:
						uvdbg("Unknown gIMXRT1020_USB_HOST.firstDeviceSpeed!!!\n");
						break;
					}

					uvdbg("usbhost_enumerate - called!!!\n");
					if (usbhost_enumerate(&(g_USBHOST_ROOT_HUB_PORT.hport), &(g_USBHOST_ROOT_HUB_PORT.hport.devclass)) < 0) {
						uvdbg("usbhost_enumerate failed!!!\n");
						g_USBHOST_ROOT_HUB_PORT.hport.connected = false;
					}
					break;
				case USB_HOST_EVENT_DETACHED:
					uvdbg("ehci_waiter() => got USB_HOST_EVENT_DETACHED\n");
					g_USBHOST_ROOT_HUB_PORT.hport.connected = false;
					if (g_USBHOST_ROOT_HUB_PORT.hport.devclass) {
						/* Yes.. Disconnect the class */
						CLASS_DISCONNECTED(g_USBHOST_ROOT_HUB_PORT.hport.devclass);
						g_USBHOST_ROOT_HUB_PORT.hport.devclass = NULL;
					}

					break;
				case USB_HOST_EVENT_ENUM_FAIL:
					uvdbg("ehci_waiter() => enumeration failed\r\n");
					g_USBHOST_ROOT_HUB_PORT.hport.connected = false;
					break;
				}
			}
		}
	}
	/* Keep the compiler from complaining */
#ifdef USB_TIZEN_RT
	return 0;
#endif
}

usb_status_t USB_HostEhciCreate()
{
	uint32_t index = 0;
	uint32_t *framePointer;

#ifdef USB_TIZEN_RT
	memset(&(gIMXRT1020_USB_HOST.ehciEventTrigger), 0x00, sizeof(struct work_s));
#endif

	gIMXRT1020_USB_HOST.deviceAttached = kEHCIDeviceDetached;
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
	gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
#endif

	if (USB_HostEhciResetIP() != kStatus_USB_Success) {	/* reset ehci ip */
		//JS modify later : USB_OsaMemoryFree(ehciInstance);
		return kStatus_USB_Error;
	}
	///////////////////////////////////////////////////////////////////////////////////////

	for (index = 1; index < USB_HOST_CONFIG_EHCI_MAX_QH; ++index) {
		gIMXRT1020_USB_HOST.ehciQhList[index - 1].horizontalLinkPointer = (uint32_t)(&gIMXRT1020_USB_HOST.ehciQhList[index]);
	}

	gIMXRT1020_USB_HOST.ehciQhList[USB_HOST_CONFIG_EHCI_MAX_QH - 1].horizontalLinkPointer = (uint32_t) NULL;
	for (index = 1; index < USB_HOST_CONFIG_EHCI_MAX_QTD; ++index) {
		gIMXRT1020_USB_HOST.ehciQtdHead[index - 1].nextQtdPointer = (uint32_t)(&gIMXRT1020_USB_HOST.ehciQtdHead[index]);
	}

	gIMXRT1020_USB_HOST.ehciQtdNumber = USB_HOST_CONFIG_EHCI_MAX_QTD;
	gIMXRT1020_USB_HOST.ehciQtdHead[USB_HOST_CONFIG_EHCI_MAX_QTD - 1].nextQtdPointer = (uint32_t) NULL;
	gIMXRT1020_USB_HOST.ehciQtdTail = &gIMXRT1020_USB_HOST.ehciQtdHead[USB_HOST_CONFIG_EHCI_MAX_QTD - 1];

#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
	for (index = 1; index < USB_HOST_CONFIG_EHCI_MAX_ITD; ++index) {
		gIMXRT1020_USB_HOST.ehciItdList[index - 1].nextLinkPointer = (uint32_t)(&gIMXRT1020_USB_HOST.ehciItdList[index]);
	}
	gIMXRT1020_USB_HOST.ehciItdNumber = USB_HOST_CONFIG_EHCI_MAX_ITD;
	gIMXRT1020_USB_HOST.ehciItdList[USB_HOST_CONFIG_EHCI_MAX_ITD - 1].nextLinkPointer = (uint32_t) NULL;
#endif							/* USB_HOST_CONFIG_EHCI_MAX_ITD */

#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
	for (index = 1; index < USB_HOST_CONFIG_EHCI_MAX_SITD; ++index) {
		gIMXRT1020_USB_HOST.ehciSitdList[index - 1].nextLinkPointer = (uint32_t)(&gIMXRT1020_USB_HOST.ehciSitdList[index]);
	}
	gIMXRT1020_USB_HOST.ehciSitdNumber = USB_HOST_CONFIG_EHCI_MAX_SITD;
	gIMXRT1020_USB_HOST.ehciSitdList[USB_HOST_CONFIG_EHCI_MAX_SITD - 1].nextLinkPointer = (uint32_t) NULL;
#endif							/* USB_HOST_CONFIG_EHCI_MAX_SITD */

#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
	for (index = 1; index < USB_HOST_EHCI_ISO_NUMBER; ++index) {
		gIMXRT1020_USB_HOST.ehciIsoList[index - 1].next = &gIMXRT1020_USB_HOST.ehciIsoList[index];
	}
	gIMXRT1020_USB_HOST.ehciIsoList[USB_HOST_EHCI_ISO_NUMBER - 1].next = NULL;
#endif

	/* initialize pipes */
	gIMXRT1020_USB_HOST.ehciPipeList = gIMXRT1020_USB_HOST.ehciPipeIndexBase;
	for (index = 1; index < USB_HOST_CONFIG_MAX_PIPES; ++index) {
		gIMXRT1020_USB_HOST.ehciPipeList[index - 1].pipeCommon.next = (usb_host_pipe_t *)&gIMXRT1020_USB_HOST.ehciPipeList[index];
	}

	if (USB_EVENT_FIFO_CREATE(&(gIMXRT1020_USB_HOST.ehciEventFifo)) != USB_OK) {
		return kStatus_USB_Error;
	}

	if (USB_EVENT_FIFO_CREATE(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo)) != USB_OK) {
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		//JS modify later : USB_OsaMemoryFree(ehciInstance);
		return kStatus_USB_Error;
	}
#ifndef USB_TIZEN_RT
	gIMXRT1020_USB_HOST.ehciEventTrigger = &(gIMXRT1020_USB_HOST.ehciEvent);

	/* initialize task trigger */
	if (USB_EHCI_EVENT_TRIGGER_CREATE(&(gIMXRT1020_USB_HOST.ehciEventTrigger)) != USB_OK) {
		uvdbg("ehci trigger init fail\r\n");
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));
		//JS modify later : USB_OsaMemoryFree(ehciInstance);
		return kStatus_USB_Error;
	}
#endif

#ifndef USB_TIZEN_RT
	gIMXRT1020_USB_HOST.ehciStatusChangeTrigger = &(gIMXRT1020_USB_HOST.ehciStatusChangeEvent);
#endif
	/* initialize task trigger */
	if (USB_EHCI_EVENT_TRIGGER_CREATE(&(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger)) != USB_OK) {
		uvdbg("ehci trigger init fail\r\n");
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));

#ifndef USB_TIZEN_RT
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventTrigger));
#endif
		//JS modify later : USB_OsaMemoryFree(ehciInstance);
		return kStatus_USB_Error;
	}

	/* initialize first qh */
	gIMXRT1020_USB_HOST.shedFirstQh = gIMXRT1020_USB_HOST.ehciQhList;
	gIMXRT1020_USB_HOST.ehciQhList = (usb_host_ehci_qh_t *)(gIMXRT1020_USB_HOST.ehciQhList->horizontalLinkPointer & EHCI_HOST_POINTER_ADDRESS_MASK);
	gIMXRT1020_USB_HOST.shedFirstQh->staticEndpointStates[0] |= (1 << EHCI_HOST_QH_H_SHIFT);	/* first qh */
	gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer = EHCI_HOST_T_INVALID_VALUE;
	gIMXRT1020_USB_HOST.shedFirstQh->currentQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	gIMXRT1020_USB_HOST.shedFirstQh->nextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	gIMXRT1020_USB_HOST.shedFirstQh->alternateNextQtdPointer = EHCI_HOST_T_INVALID_VALUE;
	gIMXRT1020_USB_HOST.shedFirstQh->horizontalLinkPointer = (uint32_t)((uint32_t)(gIMXRT1020_USB_HOST.shedFirstQh) | EHCI_HOST_POINTER_TYPE_QH);

	/* initialize periodic list */
	framePointer = (uint32_t *) gIMXRT1020_USB_HOST.ehciFrameList;
	for (index = 0; index < USB_HOST_CONFIG_EHCI_FRAME_LIST_SIZE; ++index) {
		framePointer[index] = EHCI_HOST_T_INVALID_VALUE;
	}

#ifdef USB_TIZEN_RT
	if (irq_attach(IMXRT_IRQ_USBOTG1, USB_HostEhciIsrFunction, NULL) != 0) {
		uvdbg("\nError : USB_HostEhciCreate() => irq_attach failed\n");
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger));
		return kStatus_USB_Error;
	}
#else
	if (xTaskCreate(USB_HostEhciEventTask, "usb ehci event task", 2000L / sizeof(portSTACK_TYPE), NULL, CONFIG_USBHOST_EVENT_TASK_DEFPRIO, &(gIMXRT1020_USB_HOST.ehciEventTask)) != pdPASS) {
		uvdbg("\nError : USB_HostEhciEventTask create error\n");
		/* The task was created.  Use the task's handle to delete the task. */
		gIMXRT1020_USB_HOST.ehciEventTask = NULL;
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventTrigger));
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger));
		return kStatus_USB_Error;
	}
#endif

#ifdef USB_TIZEN_RT
	gIMXRT1020_USB_HOST.ehciStatusMonitorTask = kernel_thread("usb status monitor", CONFIG_USBHOST_STATUS_MONITOR_TASK_DEFPRIO, 8192 /*CONFIG_USBHOST_STACKSIZE */ , (main_t) ehci_waiter, (FAR char *const *)NULL);
	if (gIMXRT1020_USB_HOST.ehciStatusMonitorTask < 0) {
		uvdbg("Error : ehci_waiter task create error\n");
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = -1;
#else
	if (xTaskCreate(ehci_waiter, "usb status monitor", 2000L / sizeof(portSTACK_TYPE), NULL, CONFIG_USBHOST_STATUS_MONITOR_TASK_DEFPRIO, &(gIMXRT1020_USB_HOST.ehciStatusMonitorTask)) != pdPASS) {
		uvdbg("Error : ehci_waiter task create error\n");
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = NULL;
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventTrigger));

		if (gIMXRT1020_USB_HOST.ehciStatusMonitorTask) {
			vTaskDelete(gIMXRT1020_USB_HOST.ehciStatusMonitorTask);
			gIMXRT1020_USB_HOST.ehciStatusMonitorTask = NULL;
		}
#endif
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));	//JS
		USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));
		USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger));
		return kStatus_USB_Error;
	}

	USB_HostEhciStartIP();		/* start ehci ip */
	return kStatus_USB_Success;
}

usb_status_t USB_HostEhciDestroy()
{
	/* disable all interrupts */
	gIMXRT1020_USB_HOST.usb->USBINTR = 0;
	/* stop the controller */
	gIMXRT1020_USB_HOST.usb->USBCMD = 0;

	/* free memory */
#ifdef USB_TIZEN_RT
	if (gIMXRT1020_USB_HOST.ehciStatusMonitorTask != -1) {
		task_delete(gIMXRT1020_USB_HOST.ehciStatusMonitorTask);
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = -1;
	}
#else
	if (gIMXRT1020_USB_HOST.ehciEventTask) {
		vTaskDelete(gIMXRT1020_USB_HOST.ehciEventTask);
		gIMXRT1020_USB_HOST.ehciEventTask = NULL;
	}

	if (gIMXRT1020_USB_HOST.ehciStatusMonitorTask) {
		vTaskDelete(gIMXRT1020_USB_HOST.ehciStatusMonitorTask);
		gIMXRT1020_USB_HOST.ehciStatusMonitorTask = NULL;
	}
	USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventTrigger));
#endif
	USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciEventFifo));
	USB_EVENT_FIFO_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeFifo));
	USB_EHCI_EVENT_TRIGGER_DESTROY(&(gIMXRT1020_USB_HOST.ehciStatusChangeTrigger));

	//JS => modify later USB_OsaMemoryFree(ehciInstance);
	return kStatus_USB_Success;
}

usb_status_t USB_HostEhciOpenPipe(usb_host_pipe_handle *pipeHandle, usb_host_pipe_init_t *pipeInit)
{
	usb_host_ehci_pipe_t *ehciPipePointer = NULL;
	usb_status_t status;
	uint32_t val32;
	USB_SR_ALLOC();				//JS_TEST

	/* get one pipe */
	USB_HostEhciLock();
	if (gIMXRT1020_USB_HOST.ehciPipeList != NULL) {
		ehciPipePointer = gIMXRT1020_USB_HOST.ehciPipeList;
		gIMXRT1020_USB_HOST.ehciPipeList = (usb_host_ehci_pipe_t *) ehciPipePointer->pipeCommon.next;
	}
	USB_HostEhciUnlock();
	if (ehciPipePointer == NULL) {
		uvdbg("ehci open pipe failed\r\n");
		return kStatus_USB_Busy;
	}

	uvdbg("endpointaddress : 0x%x direction : %d interval : %d maxPacketSize : %d pipeType  : %d numberPerUframe : %d\r\n", pipeInit->endpointAddress, pipeInit->direction, pipeInit->interval, pipeInit->maxPacketSize, pipeInit->pipeType, pipeInit->numberPerUframe);

	/* initialize pipe information */
	USB_HostEhciZeroMem((uint32_t *) ehciPipePointer, sizeof(usb_host_ehci_pipe_t) / 4);
	ehciPipePointer->pipeCommon.deviceAddress = pipeInit->deviceAddress;
	ehciPipePointer->pipeCommon.endpointAddress = pipeInit->endpointAddress;
	ehciPipePointer->pipeCommon.direction = pipeInit->direction;
	ehciPipePointer->pipeCommon.interval = pipeInit->interval;
	ehciPipePointer->pipeCommon.maxPacketSize = pipeInit->maxPacketSize;
	ehciPipePointer->pipeCommon.pipeType = pipeInit->pipeType;
	ehciPipePointer->pipeCommon.speed = pipeInit->speed;

	ehciPipePointer->pipeCommon.hubNumber = pipeInit->hubNumber;
	ehciPipePointer->pipeCommon.portNumber = pipeInit->portNumber;
	ehciPipePointer->pipeCommon.hsHubNumber = pipeInit->hsHubNumber;
	ehciPipePointer->pipeCommon.hsHubPort = pipeInit->hsHubPort;
	ehciPipePointer->pipeCommon.level = pipeInit->level;

	ehciPipePointer->pipeCommon.numberPerUframe = pipeInit->numberPerUframe + 1;
	if (ehciPipePointer->pipeCommon.numberPerUframe > 3) {
		ehciPipePointer->pipeCommon.numberPerUframe = 3;
	}

	ehciPipePointer->pipeCommon.nakCount = pipeInit->nakCount;
	ehciPipePointer->pipeCommon.nextdata01 = 0;
	ehciPipePointer->ehciQh = NULL;
	if ((ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) || (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_INTERRUPT)) {
		if (ehciPipePointer->pipeCommon.pipeType == USB_ENDPOINT_ISOCHRONOUS) {
			ehciPipePointer->pipeCommon.interval = (1 << (ehciPipePointer->pipeCommon.interval - 1));	/* iso interval is the power of 2 */
		} else {
			if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
				ehciPipePointer->pipeCommon.interval = (1 << (ehciPipePointer->pipeCommon.interval - 1));	/* HS interrupt interval is the power of 2 */
			} else {
				ehciPipePointer->pipeCommon.interval = USB_HostEhciGet2PowerValue(ehciPipePointer->pipeCommon.interval);	/* FS/LS interrupt interval should be the power of 2, it is used for ehci bandwidth */
			}
		}

		if ((ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) && (ehciPipePointer->pipeCommon.interval < 8)) {
			val32 = gIMXRT1020_USB_HOST.usb->USBCMD;
			val32 &= (~USBHS_USBCMD_ITC_MASK);
			val32 |= USBHS_USBCMD_ITC((ehciPipePointer->pipeCommon.interval));
			gIMXRT1020_USB_HOST.usb->USBCMD = val32;
		}
	}

	/* save the micro-frame interval, it is convenient for the interval process */
	if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
		ehciPipePointer->uframeInterval = ehciPipePointer->pipeCommon.interval;
	} else {
		ehciPipePointer->uframeInterval = 8 * ehciPipePointer->pipeCommon.interval;
	}

	/* open pipe */
	switch (ehciPipePointer->pipeCommon.pipeType) {
	case USB_ENDPOINT_CONTROL:
	case USB_ENDPOINT_BULK:
		status = USB_HostEhciOpenControlBulk(ehciPipePointer);
		break;

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
		((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	case USB_ENDPOINT_ISOCHRONOUS:
		status = USB_HostEhciOpenIso(ehciPipePointer);
		break;
#endif

	case USB_ENDPOINT_INTERRUPT:
		status = USB_HostEhciOpenInterrupt(ehciPipePointer);
		break;

	default:
		status = kStatus_USB_Error;
		break;
	}

	if (status != kStatus_USB_Success) {
		/* release pipe */
		USB_HostEhciLock();
		ehciPipePointer->pipeCommon.next = (usb_host_pipe_t *) gIMXRT1020_USB_HOST.ehciPipeList;
		gIMXRT1020_USB_HOST.ehciPipeList = ehciPipePointer;
		USB_HostEhciUnlock();
		return status;
	}

	/* add pipe to run pipe list */
	USB_HostEhciLock();
	ehciPipePointer->pipeCommon.next = (usb_host_pipe_t *) gIMXRT1020_USB_HOST.ehciRunningPipeList;
	gIMXRT1020_USB_HOST.ehciRunningPipeList = ehciPipePointer;
	USB_HostEhciUnlock();

	*pipeHandle = ehciPipePointer;
	return status;
}

usb_status_t USB_HostEhciClosePipe(usb_host_pipe_handle pipeHandle)
{
	usb_host_ehci_pipe_t *ehciPipePointer = (usb_host_ehci_pipe_t *) pipeHandle;
	usb_host_pipe_t *prevPointer = NULL;
	USB_SR_ALLOC();				//JS_TEST

	uvdbg("\nUSB_HostEhciClosePipe called!!\n");

	switch (ehciPipePointer->pipeCommon.pipeType) {
	case USB_ENDPOINT_BULK:
	case USB_ENDPOINT_CONTROL:
		USB_HostEhciCloseControlBulk(ehciPipePointer);
		break;

	case USB_ENDPOINT_INTERRUPT:
		USB_HostEhciCloseInterrupt(ehciPipePointer);
		break;

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	case USB_ENDPOINT_ISOCHRONOUS:
		USB_HostEhciCloseIso(ehciPipePointer);
		break;
#endif

	default:
		break;
	}

	/* delete pipe from run pipe list */
	USB_HostEhciLock();
	prevPointer = (usb_host_pipe_t *) gIMXRT1020_USB_HOST.ehciRunningPipeList;
	if (prevPointer == (usb_host_pipe_t *) ehciPipePointer) {
		gIMXRT1020_USB_HOST.ehciRunningPipeList = (usb_host_ehci_pipe_t *)(prevPointer->next);
	} else {
		while (prevPointer != NULL) {
			if (prevPointer->next == (usb_host_pipe_t *) ehciPipePointer) {
				prevPointer->next = ehciPipePointer->pipeCommon.next;
				break;
			} else {
				prevPointer = prevPointer->next;
			}
		}
	}
	USB_HostEhciUnlock();

	/* release pipe */
	USB_HostEhciLock();
	ehciPipePointer->pipeCommon.next = (usb_host_pipe_t *) gIMXRT1020_USB_HOST.ehciPipeList;
	gIMXRT1020_USB_HOST.ehciPipeList = ehciPipePointer;
	USB_HostEhciUnlock();
	return kStatus_USB_Success;
}

usb_status_t USB_HostEhciWritePipe(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	usb_host_ehci_pipe_t *ehciPipePointer = (usb_host_ehci_pipe_t *) pipeHandle;
	usb_status_t status = kStatus_USB_Success;
#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
	 ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
#endif

	switch (ehciPipePointer->pipeCommon.pipeType) {
	case USB_ENDPOINT_BULK:
	case USB_ENDPOINT_CONTROL:
	case USB_ENDPOINT_INTERRUPT:
		status = USB_HostEhciQhQtdListInit(ehciPipePointer, transfer);	/* initialize qtd for control/bulk transfer */
		break;

#if (((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD)) || \
((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD)))
	case USB_ENDPOINT_ISOCHRONOUS:
		if (ehciPipePointer->pipeCommon.speed == IMXRT_USB_SPEED_HIGH) {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_ITD) && (USB_HOST_CONFIG_EHCI_MAX_ITD))
			status = USB_HostEhciItdArrayInit(ehciPipePointer, transfer);	/* initialize itd for iso transfer */
#endif
		} else {
#if ((defined USB_HOST_CONFIG_EHCI_MAX_SITD) && (USB_HOST_CONFIG_EHCI_MAX_SITD))
			status = USB_HostEhciSitdArrayInit(ehciPipePointer, transfer);	/* initialize sitd for iso transfer */
#endif
		}
		break;
#endif

	default:
		break;
	}
	return status;
}

usb_status_t USB_HostEhciReadpipe(usb_host_pipe_handle pipeHandle, usb_host_transfer_t *transfer)
{
	return USB_HostEhciWritePipe(pipeHandle, transfer);	/* same as write */
}

usb_status_t USB_HostEhciIoctl(uint32_t ioctlEvent, void *ioctlParam)
{
	usb_status_t status = kStatus_USB_Success;
	usb_host_cancel_param_t *param;
	usb_host_ehci_pipe_t *ehciPipePointer;
	volatile usb_host_ehci_qh_t *vltQhPointer;
	uint32_t deviceAddress;
	USB_SR_ALLOC();				//JS_TEST

	switch (ioctlEvent) {
	case kUSB_HostCancelTransfer:	/* cancel pipe or one transfer */
		param = (usb_host_cancel_param_t *) ioctlParam;
		status = USB_HostEhciCancelPipe((usb_host_ehci_pipe_t *) param->pipeHandle, param->transfer);
		break;

	case kUSB_HostBusControl:	/* bus control */
		status = USB_HostEhciControlBus(*((uint8_t *) ioctlParam));
		break;

	case kUSB_HostGetFrameNumber:	/* get frame number */
		*((uint32_t *) ioctlParam) = ((gIMXRT1020_USB_HOST.usb->FRINDEX & EHCI_MAX_UFRAME_VALUE) >> 3);
		break;

	case kUSB_HostUpdateControlEndpointAddress:
		ehciPipePointer = (usb_host_ehci_pipe_t *) ioctlParam;
		vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;

		USB_HostEhciLock();
		/* update address */
		vltQhPointer->staticEndpointStates[0] = (((vltQhPointer->staticEndpointStates[0]) & (~EHCI_HOST_QH_DEVICE_ADDRESS_MASK)) | ((uint32_t) ehciPipePointer->pipeCommon.deviceAddress << EHCI_HOST_QH_DEVICE_ADDRESS_SHIFT));
		USB_HostEhciUnlock();

		USB_HostEhciDelay(2U);
		break;

	case kUSB_HostUpdateControlPacketSize:
		ehciPipePointer = (usb_host_ehci_pipe_t *) ioctlParam;
		vltQhPointer = (volatile usb_host_ehci_qh_t *)ehciPipePointer->ehciQh;
		USB_HostEhciLock();
		if (gIMXRT1020_USB_HOST.usb->USBSTS & USBHS_USBSTS_AS_MASK) {
			USB_HostEhciStopAsync();
			/* update max packet size */
			vltQhPointer->staticEndpointStates[0] = (((vltQhPointer->staticEndpointStates[0]) & (~EHCI_HOST_QH_MAX_PACKET_LENGTH_MASK)) | ((uint32_t) ehciPipePointer->pipeCommon.maxPacketSize << EHCI_HOST_QH_MAX_PACKET_LENGTH_SHIFT));
			USB_HostEhciStartAsync();
		} else {
			/* update max packet size */
			vltQhPointer->staticEndpointStates[0] = (((vltQhPointer->staticEndpointStates[0]) & (~EHCI_HOST_QH_MAX_PACKET_LENGTH_MASK)) | ((uint32_t) ehciPipePointer->pipeCommon.maxPacketSize << EHCI_HOST_QH_MAX_PACKET_LENGTH_SHIFT));
		}
		USB_HostEhciUnlock();
		break;
	default:
		status = kStatus_USB_NotSupported;
		break;
	}
	return status;
}

volatile uint32_t USB_HostEhciTaskFunction_executing = 0;
void USB_HostEhciTaskFunction(void *parameter)
{
	uint8_t msg;
	usb_device_handle deviceHandle;
	USB_HostEhciTaskFunction_executing = 1;

	while (USB_EVENT_FIFO_POP(&(gIMXRT1020_USB_HOST.ehciEventFifo), &msg) == USB_OK) {
		if (msg == EHCI_TASK_EVENT_PORT_CHANGE) {	/* port change */
			USB_HostEhciPortChange();
		}

		if (msg == EHCI_TASK_EVENT_TIMER0) {	/* timer0 */
			USB_HostEhciTimer0();
		}
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
		if (msg == EHCI_TASK_EVENT_TIMER1) {	/* timer1 */
			USB_HostEhciTimer1();
		}
#endif

		if (gIMXRT1020_USB_HOST.deviceAttached == kEHCIDeviceAttached) {
			if (msg == EHCI_TASK_EVENT_TRANSACTION_DONE) {	/* transaction done */
				USB_HostEhciTransactionDone();
			}

			if (msg == EHCI_TASK_EVENT_DEVICE_DETACH) {	/* device detach */
				gIMXRT1020_USB_HOST.usb->USBINTR &= (~USBHS_USBINTR_PCE_MASK);	/* disable attach, enable when the detach process is done */
				gIMXRT1020_USB_HOST.deviceAttached = kEHCIDeviceDetached;
				USB_HostDetachDevice(0, 0);
			}
		} else if (gIMXRT1020_USB_HOST.deviceAttached != kEHCIDeviceAttached) {
			if (msg == EHCI_TASK_EVENT_DEVICE_ATTACH) {	/* device is attached */
				USB_HostEhciStartAsync();
				USB_HostEhciStartPeriodic();
				USB_HostAttachDevice(gIMXRT1020_USB_HOST.firstDeviceSpeed, 0, 0, 1, &deviceHandle);
			}
		} else {
		}
	}
	USB_HostEhciTaskFunction_executing = 0;
}

volatile uint32_t timer_count = 0;
volatile uint32_t ISR_is_executed = 0;

#ifdef USB_TIZEN_RT
int USB_HostEhciIsrFunction(int irq, FAR void *context, FAR void *arg)
#else
void USB_HostEhciIsrFunction()
#endif
{
	uint32_t event_triggered = 0;
	static uint32_t interruptStatus = 0;
	int ret;

	ISR_is_executed = 1;
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))

	if (gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_CTRL & USBNC_USB_OTGn_CTRL_WIE_MASK) {
		gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_CTRL &= ~USBNC_USB_OTGn_CTRL_WIE_MASK;
		gIMXRT1020_USB_HOST.deviceCallback(gIMXRT1020_USB_HOST.suspendedDevice, NULL, kUSB_HostEventDetectResume);	/* call host callback function */

		while (!(gIMXRT1020_USB_HOST.usb_nc->USB_OTGn_PHY_CTRL_0 & USBNC_USB_OTGn_PHY_CTRL_0_UTMI_CLK_VLD_MASK)) {
		}

		if (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_CCS_MASK) {
			USB_HostEhciStartAsync();
			USB_HostEhciStartPeriodic();
		}

		gIMXRT1020_USB_HOST.usb->USBCMD |= (USBHS_USBCMD_RS_MASK);
		if ((kBus_EhciSuspended == gIMXRT1020_USB_HOST.busSuspendStatus)) {
			/* gIMXRT1020_USB_HOST.usb->PORTSC1 |= USBHS_PORTSC1_FPR_MASK; */
			gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciStartResume;
		} else {
		}
	} else {
	}

#endif							/* USB_HOST_CONFIG_LOW_POWER_MODE */

	interruptStatus = gIMXRT1020_USB_HOST.usb->USBSTS;
	interruptStatus &= gIMXRT1020_USB_HOST.usb->USBINTR;

	while (interruptStatus) {	/* there are usb interrupts */
		gIMXRT1020_USB_HOST.usb->USBSTS = interruptStatus;	/* clear interrupt */

		if (interruptStatus & USBHS_USBSTS_SRI_MASK) {	/* SOF interrupt */
		}

		if (interruptStatus & USBHS_USBSTS_SEI_MASK) {	/* system error interrupt */
		}

		if ((interruptStatus & USBHS_USBSTS_UI_MASK) || (interruptStatus & USBHS_USBSTS_UEI_MASK)) {	/* USB interrupt or USB error interrupt */
			USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_TRANSACTION_DONE);
			event_triggered++;
		}

		if (interruptStatus & USBHS_USBSTS_PCI_MASK) {	/* port change detect interrupt */
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
			if (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_FPR_MASK) {
				if (kBus_EhciStartSuspend == gIMXRT1020_USB_HOST.busSuspendStatus) {
					if (gIMXRT1020_USB_HOST.usb->PORTSC1 & USBHS_PORTSC1_CCS_MASK) {
						USB_HostEhciStartAsync();
						USB_HostEhciStartPeriodic();
					}

					gIMXRT1020_USB_HOST.deviceCallback(gIMXRT1020_USB_HOST.suspendedDevice, NULL, kUSB_HostEventNotSuspended);	/* call host callback function */
					gIMXRT1020_USB_HOST.suspendedDevice = NULL;
					gIMXRT1020_USB_HOST.busSuspendStatus = kBus_EhciIdle;
					gIMXRT1020_USB_HOST.usb->USBINTR &= ~(USBHS_USBINTR_TIE1_MASK);
				} else {
				}
			}
#endif
			USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_PORT_CHANGE);
			event_triggered++;
		}

		if (interruptStatus & USBHS_USBSTS_TI0_MASK) {	/* timer 0 interrupt */
			USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_TIMER0);
			event_triggered++;
		}
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
		if (interruptStatus & USBHS_USBSTS_TI1_MASK) {	/* timer 1 interrupt */
			USB_EVENT_FIFO_PUSH(&(gIMXRT1020_USB_HOST.ehciEventFifo), EHCI_TASK_EVENT_TIMER1);
			event_triggered++;
		}
#endif

		interruptStatus = gIMXRT1020_USB_HOST.usb->USBSTS;
		interruptStatus &= gIMXRT1020_USB_HOST.usb->USBINTR;
	}

	if (event_triggered) {
#ifdef USB_TIZEN_RT
		if (work_available(&(gIMXRT1020_USB_HOST.ehciEventTrigger))) {
			if (work_queue(HPWORK, &(gIMXRT1020_USB_HOST.ehciEventTrigger), USB_HostEhciTaskFunction, (void *)NULL, 0) != 0) {
				udbg("ERROR: gIMXRT1020_USB_HOST.ehciEventTrigger_wq failed to queue work\n");
			}
		}
#else
		USB_EHCI_EVENT_TRIGGER_SEND(gIMXRT1020_USB_HOST.ehciEventTrigger);
#endif
	}

	ISR_is_executed = 0;

#ifdef USB_TIZEN_RT
	return OK;
#endif
}
