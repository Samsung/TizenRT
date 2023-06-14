#include "ipc_message_queue.h"


static const u32 xQueueHeapHeaderSize	= (sizeof(imq_queue_heap_t) + ((u32)(IMQ_BYTE_ALIGNMENT - 1))) & ~((u32) IMQ_BYTE_ALIGNMENT_MASK);
_sema g_ipc_wake_sema[IMQ_RSVD_QUEUE_MAX];

imq_error_t IPC_Message_Queue_Buffer_Init(u32 ID, u32 size, u32 *address, u32 *asize)
{
	imq_queue_heap_t *pxHeapHeader;
	u32 xAlignSize = 0;
	u32 xAddress;

	pxHeapHeader = (imq_queue_heap_t *)imq_heap_region.xStartAddress;

	if (pxHeapHeader->init != 1) {
		pxHeapHeader->remainFreeBytes = imq_heap_region.xSizeInBytes - xQueueHeapHeaderSize;
		pxHeapHeader->startBlock = imq_heap_region.xStartAddress + xQueueHeapHeaderSize;
		pxHeapHeader->init = 1;
	}

	if (pxHeapHeader->remainFreeBytes < (u32)size) {
		imq_printf(_ERR_, "%s, ID: %lu, HEAP OVERFLOW! FREE: %lu, REQUEST: %lu\n", __func__, ID, pxHeapHeader->remainFreeBytes, size);
		return IMQ_NO_MEMORY;
	}

	/* Get send queue ring buffer start address */
	xAddress = pxHeapHeader->startBlock;

	/* Ensure the heap region starts on a correctly aligned boundary. */

	/* Size need aligment with 128bytes */
	xAlignSize = size;
	IPC_BYTE_ALIGNMENT(xAlignSize);

	/* Update heap header */
	pxHeapHeader->remainFreeBytes -= xAlignSize;
	pxHeapHeader->startBlock += xAlignSize;

	*address = xAddress;
	*asize = xAlignSize;

	return IMQ_OK;
}

imq_error_t IPC_Message_Queue_Init(void)
{
	int i = 0;

	for (i = 0; i < IMQ_RSVD_QUEUE_MAX; i++) {
		rtw_init_sema(&g_ipc_wake_sema[i], 0);
	}

#if defined(CONFIG_AS_IMQ_AP)
	int ret = IMQ_OK;
	imq_queue_info_t *queue_info;
	u32 address = 0;
	u32 size = 0;
	u32 ID;

	for (i = 0;;) {

		/*  Check if search to end */
		if (imq_rsvd_queue_config[i].ID == 0xFFFFFFFF) {
			break;
		}

		if (imq_rsvd_queue_config[i].ID > IMQ_RSVD_QUEUE_MAX) {
			continue;
		}

		/* Initialize queue info */
		ID = imq_rsvd_queue_config[i].ID;
		queue_info = (imq_queue_info_t *)(imq_info_region.xStartAddress + ID * sizeof(imq_queue_info_t));

		ret = IPC_Message_Queue_Buffer_Init(ID, imq_rsvd_queue_config[i].RING_BUFFER_SIZE, &address, &size);
		if (ret != IMQ_OK) {
			imq_printf(_ERR_, "%s, master init fail ret: %d\n", __func__, ret);
		} else {
			queue_info->dir = imq_rsvd_queue_config[i].DIR;
			queue_info->addr = address;
			queue_info->len = size;
			queue_info->type = imq_rsvd_queue_config[i].TYPE;
			queue_info->wptr = address;
			queue_info->rptr = address;
			queue_info->init = 1;
			DCache_Clean((u32)queue_info, sizeof(imq_queue_info_t));

			imq_printf(_ALWAYS_, "%s, queue id: %lu, addr: 0x%08x, size: %lu\n", __func__, ID, (unsigned int)address, size);
		}
		i++;

	}
#endif
	imq_printf(_ALWAYS_, "%s, init done\n", __func__);
	return IMQ_OK;
}

imq_error_t IPC_Message_Queue_Send(u32 ID, u8 *buf, u32 size, int timeout)
{
	imq_queue_info_t *queue_info;
	u32 segmentLen = 0;
	u32 remainFreeBytes = 0;
	u32 endAddr;

	if (ID > IMQ_RSVD_QUEUE_MAX) {
		imq_assert_param(IMQ_INVALID_CH);
		return IMQ_INVALID_CH;
	}

	if (!size || !buf) {
		imq_printf(_ERR_, "%s, id: %lu, invalid size or buf\n", __func__, ID);
		imq_assert_param(IMQ_INVALID_SIZE);
		return IMQ_INVALID_SIZE;
	}

	queue_info = (imq_queue_info_t *)(imq_info_region.xStartAddress + ID * sizeof(imq_queue_info_t));
	DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));

	if (queue_info->init != 1) {
		imq_assert_param(IMQ_NOT_INIT);
		return IMQ_NOT_INIT;
	}

	/* Check free bytes
	 * Block incase the reader haven't recevied the message */
	while (1) {
		u32 rptr;

		rptr = queue_info->rptr;

		if (rptr > queue_info->wptr) {
			remainFreeBytes = rptr - queue_info->wptr - 1;
		} else {
			remainFreeBytes = rptr + queue_info->len - queue_info->wptr - 1;
		}

		if (remainFreeBytes > size) {
			break;
		}

		if (timeout == 0) {
			return IMQ_TX_TIMEOUT;
		}

		rtw_msleep_os(1);

		if (timeout != IMQ_WAIT_FOREVER) {
			timeout--;
			if (timeout == 0) {
				imq_assert_param(IMQ_TX_TIMEOUT);
				return IMQ_TX_TIMEOUT;
			}
		}

		DCache_Invalidate((u32)&queue_info->rptr, IMQ_BYTE_ALIGNMENT);
	}

	/* Check if the wanted size overflow the ring buffer queue
	 * If so, packets need segment, switch to the start of ring buffer queue
	 * Copy buf into queue ring buffer and Update write pointer */
	endAddr = queue_info->addr + queue_info->len;
	if (queue_info->wptr + size > endAddr) {
		segmentLen = endAddr - queue_info->wptr;
		rtw_memcpy((void *)queue_info->wptr, (void *)buf, segmentLen);
		rtw_memcpy((void *)queue_info->addr, (void *)(buf + segmentLen), size - segmentLen);
		DCache_Clean(queue_info->wptr, segmentLen);
		DCache_Clean(queue_info->addr, size - segmentLen);
		queue_info->wptr = queue_info->addr + (size - segmentLen);
	} else {
		rtw_memcpy((void *)queue_info->wptr, (void *)buf, size);
		DCache_Clean(queue_info->wptr, size);
		queue_info->wptr += size;
	}

	/* Update write pointer */
	DCache_Clean((u32) & (queue_info->wptr), IMQ_BYTE_ALIGNMENT);
	if (queue_info->type == MESSAGE_QUEUE) {
		/* Ipc message send */
		IPC_MSG_STRUCT ipc_message = {0};
		ipc_message.msg = ID;
		ipc_message.msg_type = IPC_USER_POINT;
		ipc_send_message(queue_info->dir, IPC_IMQ_RX_TRAN_0, &ipc_message);
	}
	return IMQ_OK;
}

imq_error_t IPC_Message_Queue_Recv(u32 ID, u8 *buf, u32 *size)
{
	imq_queue_info_t *queue_info;
	u32 endAddr;
	u32 segmentLen = 0;
	u32 readableBytes = 0;
	u32 wptr;

	if (!buf || *size == 0) {
		imq_printf(_ERR_, "%s, id: %lu, invalid buf or size : %lu\n", __func__, ID, *size);
		imq_assert_param(IMQ_INVALID_SIZE);
		return IMQ_INVALID_SIZE;
	}

	if (ID > IMQ_RSVD_QUEUE_MAX) {
		imq_assert_param(IMQ_INVALID_CH);
		return IMQ_INVALID_CH;
	}

	queue_info = (imq_queue_info_t *)(imq_info_region.xStartAddress + ID * sizeof(imq_queue_info_t));
	DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));

	if (queue_info->init != 1) {
		imq_assert_param(IMQ_NOT_INIT);
		return IMQ_NOT_INIT;
	}

	if (queue_info->type == MESSAGE_QUEUE) {
		/* Block receive */
		rtw_down_sema(&g_ipc_wake_sema[ID]);
		DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));
	}

	wptr = queue_info->wptr;

	if (queue_info->rptr > wptr) {
		readableBytes = wptr + queue_info->len - queue_info->rptr;
	} else {
		readableBytes = wptr - queue_info->rptr;
	}

	if (readableBytes < *size) {
		*size = readableBytes;
		imq_assert_param(IMQ_NO_ENOUGH_BUF);
		return IMQ_NO_ENOUGH_BUF;
	}

	/* Check if the wanted size overflow the queue ring buffer
	 * If so, packets need segment, switch to the start of queue ring buffer
	 * Copy buf from queue ring buffer and Update read pointer */
	endAddr = queue_info->addr + queue_info->len;
	if (queue_info->rptr + *size > endAddr) {
		segmentLen = endAddr - queue_info->rptr;
		DCache_Invalidate(queue_info->rptr, segmentLen);
		DCache_Invalidate(queue_info->addr, *size - segmentLen);
		rtw_memcpy((void *)buf, (void *)queue_info->rptr, segmentLen);
		rtw_memcpy((void *)(buf + segmentLen), (void *)queue_info->addr, *size - segmentLen);
		queue_info->rptr = queue_info->addr + (*size - segmentLen);
	} else {
		DCache_Invalidate(queue_info->rptr, *size);
		rtw_memcpy((void *)buf, (void *)queue_info->rptr, *size);
		queue_info->rptr += *size;
	}

	/* Update read pointer */
	DCache_Clean((u32)&queue_info->rptr, IMQ_BYTE_ALIGNMENT);
	return IMQ_OK;
}


void IPC_Message_Queue_ipc_int_hdl_0(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	/* up the semaphore according to the id */
	PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_IMQ_MSG_RX_0, IPC_IMQ_RX_TRAN_0);
	DCache_Invalidate((u32)p_ipc_recv_msg, sizeof(IPC_MSG_STRUCT));

	rtw_up_sema_from_isr(&g_ipc_wake_sema[p_ipc_recv_msg->msg]);
}

/* ---------------------------- Global Variables ---------------------------- */
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_message_queue_table[] = {
	{IPC_USER_POINT,	IPC_Message_Queue_ipc_int_hdl_0,	(VOID *) NULL, IPC_DIR_IMQ_MSG_RX_0, IPC_IMQ_RX_TRAN_0, IPC_RX_FULL},
};
