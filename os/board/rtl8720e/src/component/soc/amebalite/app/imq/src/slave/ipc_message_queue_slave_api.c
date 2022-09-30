#include "ipc_message_queue.h"

static const uint32_t xShareHeapHeaderSize	= (sizeof(imq_share_heap_t) + ((uint32_t)(IMQ_BYTE_ALIGNMENT - 1))) & ~((uint32_t) IMQ_BYTE_ALIGNMENT_MASK);
static const uint32_t xShareBlockHeaderSize	= (sizeof(imq_share_buffer_t) + ((uint32_t)(IMQ_BYTE_ALIGNMENT - 1))) & ~((uint32_t) IMQ_BYTE_ALIGNMENT_MASK);

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( uint32_t ) ( xShareBlockHeaderSize << 1 ) )


_sema g_ipc_slave_wake_sema[IMQ_QUEUE_MAX];

imq_error_t IPC_Message_Queue_Slave_Init(u8 ID)
{
	imq_queue_info_t *queue_info;
	uint32_t queue_addr;

	/* Get queue info */

	queue_addr = IMQ_QUEUE_INFO_BASE_ADDR + ID * sizeof(imq_queue_info_t);
	queue_info = (imq_queue_info_t *)queue_addr;

	/* Wait until master initialized */

	while (queue_info->m_init != 1) {
		vTaskDelay(10);
		DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));
	}

	/* Initialize the semaphore */

	g_ipc_slave_wake_sema[ID] = xSemaphoreCreateCounting(0xFFFFFFFF, 0);

	queue_info->s_init = 1;

	DCache_Clean(queue_addr, sizeof(imq_queue_info_t));

	printf("%s, queue id: %x, addr: 0x%08x, size: %x, init done\n", __func__, ID, queue_info->s_addr, queue_info->s_len);

	return IMQ_OK;
}

uint32_t IPC_checkRemainFreeBytes(imq_queue_info_t *queue_info)
{
	uint32_t remainFreeBytes = 0;
	uint32_t startAddr, endAddr;

	startAddr = queue_info->s_addr;
	endAddr = queue_info->s_addr + queue_info->s_len;

	if (queue_info->m_rptr > queue_info->s_wptr) {
		remainFreeBytes = queue_info->m_rptr - queue_info->s_wptr;
	} else {
		remainFreeBytes = (queue_info->m_rptr - startAddr) + (endAddr - queue_info->s_wptr);
	}
	return remainFreeBytes;
}

imq_error_t IPC_Message_Queue_Send(u8 ID, u8 *buf, int size)
{
	imq_queue_info_t *queue_info;
	uint32_t queue_addr;
	struct imq_message tx_message = {0};
	uint32_t offset = 0;
	uint32_t alignSize = 0;
	uint32_t hdrLen = sizeof(struct imq_message);
	uint8_t segmentNeed = 0;
	uint32_t segmentLen = 0;
	uint32_t remainFreeBytes = 0;
	uint32_t startAddr, endAddr;

	if (ID < 0 || ID > IMQ_QUEUE_MAX) {
		return IMQ_INVALID_CH;
	}

	if (size <= 0) {
		return IMQ_INVALID_SIZE;
	}

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_22, 1);
#endif

	queue_addr = IMQ_QUEUE_INFO_BASE_ADDR + ID * sizeof(imq_queue_info_t);
	queue_info = (imq_queue_info_t *)queue_addr;
	DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));

	if (!queue_info->m_init || !queue_info->s_init) {
		return IMQ_NOT_INIT;
	}

	startAddr = queue_info->s_addr;
	endAddr = queue_info->s_addr + queue_info->s_len;

	/* Check free bytes */
	remainFreeBytes = IPC_checkRemainFreeBytes(queue_info);

	/* Size need aligment with 32bytes */
	alignSize = size + hdrLen;
	IPC_BYTE_ALIGNMENT(alignSize);

	if (alignSize > queue_info->s_len) {
		return IMQ_NO_MEMORY;
	}

	/* Block incase the master haven't recevied the message */
	while (remainFreeBytes < alignSize) {
		vTaskDelay(1);
		DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));
		remainFreeBytes = IPC_checkRemainFreeBytes(queue_info);
	}

	/* Check if the wanted size overflow the queue ring buffer
	   If so, packets need segment, switch to the start of queue ring buffer */
	if (queue_info->s_wptr + alignSize > endAddr) {
		segmentNeed = 1;
		segmentLen = endAddr - queue_info->s_wptr;
		/* If the remain bytes less than header length,
		   directly switch to the start of queue ring buffer */
		if (segmentLen < hdrLen) {
			segmentNeed = 0;
			queue_info->s_wptr = queue_info->s_addr;
		}
	}

	tx_message.msg_len = size;
#if IMQ_CHKSUM_DEBUG
	/* Calculate checksum */
	tx_message.chksum = 0;
	for (int i = 0; i < size; i++) {
		tx_message.chksum += buf[i];
	}
#endif

	if (segmentNeed) {
		/* Copy tx_message into queue ring buffer */
		memcpy(queue_info->s_wptr, &tx_message, hdrLen);
		offset = segmentLen - hdrLen;
		/* Copy buf into queue ring buffer */
		memcpy(queue_info->s_wptr + hdrLen, buf, offset);
		DCache_Clean(queue_info->s_wptr, segmentLen);

		/* Write pointer switch to the start of queue ring buffer */
		queue_info->s_wptr = queue_info->s_addr;
		memcpy(queue_info->s_wptr, buf + offset, size - offset);
		DCache_Clean(queue_info->s_wptr, alignSize - segmentLen);

		queue_info->s_wptr += (alignSize - segmentLen);
	} else {
		/* Copy tx_message into queue ring buffer */
		memcpy(queue_info->s_wptr, &tx_message, hdrLen);

		/* Copy buf into queue ring buffer */

		memcpy(queue_info->s_wptr + hdrLen, buf, size);
		DCache_Clean(queue_info->s_wptr, alignSize);

		/* Update master write pointer and slave read pointer */
		queue_info->s_wptr += alignSize;
	}

	DCache_Clean((u32)queue_info, sizeof(imq_queue_info_t));


	/* Ipc message send */

	IPC_MSG_STRUCT ipc_message = {0};
	ipc_message.msg = ID;
	ipc_message.msg_type = IPC_USER_POINT;

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_31, 1);
#endif

	ipc_send_message(IPC_DIR_MSG_TX, IPC_S2M_IMQ_TRX_TRAN, &ipc_message);

	return IMQ_OK;
}

imq_error_t IPC_Message_Queue_Send_Timeout(u8 ID, u8 *buf, int size, int timeout)
{
	imq_queue_info_t *queue_info;
	uint32_t queue_addr;
	struct imq_message tx_message = {0};
	uint32_t offset = 0;
	uint32_t alignSize = 0;
	uint32_t hdrLen = sizeof(struct imq_message);
	uint8_t segmentNeed = 0;
	uint32_t segmentLen = 0;
	uint32_t remainFreeBytes = 0;
	uint32_t startAddr, endAddr;
	uint32_t cnt = 0;

	if (ID < 0 || ID > IMQ_QUEUE_MAX) {
		return IMQ_INVALID_CH;
	}

	if (size <= 0) {
		return IMQ_INVALID_SIZE;
	}

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_22, 1);
#endif

	queue_addr = IMQ_QUEUE_INFO_BASE_ADDR + ID * sizeof(imq_queue_info_t);
	queue_info = (imq_queue_info_t *)queue_addr;
	DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));

	if (!queue_info->m_init || !queue_info->s_init) {
		return IMQ_NOT_INIT;
	}

	startAddr = queue_info->s_addr;
	endAddr = queue_info->s_addr + queue_info->s_len;

	/* Check free bytes */
	remainFreeBytes = IPC_checkRemainFreeBytes(queue_info);

	/* Size need aligment with 32bytes */
	alignSize = size + hdrLen;
	IPC_BYTE_ALIGNMENT(alignSize);

	if (alignSize > queue_info->s_len) {
		return IMQ_NO_MEMORY;
	}

	/* Wait timeout incase the master haven't recevied the message */
	while (remainFreeBytes < alignSize) {
		if (cnt >= timeout) {
			return IMQ_TX_TIMEOUT;
		}
		vTaskDelay(1);
		DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));
		remainFreeBytes = IPC_checkRemainFreeBytes(queue_info);
		cnt++;
	}

	/* Check if the wanted size overflow the queue ring buffer
	   If so, packets need segment, switch to the start of queue ring buffer */
	if (queue_info->s_wptr + alignSize > endAddr) {
		segmentNeed = 1;
		segmentLen = endAddr - queue_info->s_wptr;
		/* If the remain bytes less than header length,
		   directly switch to the start of queue ring buffer */
		if (segmentLen < hdrLen) {
			segmentNeed = 0;
			queue_info->s_wptr = queue_info->s_addr;
		}
	}

	tx_message.msg_len = size;
#if IMQ_CHKSUM_DEBUG
	/* Calculate checksum */
	tx_message.chksum = 0;
	for (int i = 0; i < size; i++) {
		tx_message.chksum += buf[i];
	}
#endif

	if (segmentNeed) {
		/* Copy tx_message into queue ring buffer */
		memcpy(queue_info->s_wptr, &tx_message, hdrLen);
		offset = segmentLen - hdrLen;
		/* Copy buf into queue ring buffer */
		memcpy(queue_info->s_wptr + hdrLen, buf, offset);
		DCache_Clean(queue_info->s_wptr, segmentLen);

		/* Write pointer switch to the start of queue ring buffer */
		queue_info->s_wptr = queue_info->s_addr;
		memcpy(queue_info->s_wptr, buf + offset, size - offset);
		DCache_Clean(queue_info->s_wptr, alignSize - segmentLen);

		queue_info->s_wptr += (alignSize - segmentLen);
	} else {
		/* Copy tx_message into queue ring buffer */
		memcpy(queue_info->s_wptr, &tx_message, hdrLen);

		/* Copy buf into queue ring buffer */

		memcpy(queue_info->s_wptr + hdrLen, buf, size);
		DCache_Clean(queue_info->s_wptr, alignSize);

		/* Update master write pointer and slave read pointer */
		queue_info->s_wptr += alignSize;
	}

	DCache_Clean((u32)queue_info, sizeof(imq_queue_info_t));


	/* Ipc message send */

	IPC_MSG_STRUCT ipc_message = {0};
	ipc_message.msg = ID;
	ipc_message.msg_type = IPC_USER_POINT;

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_31, 1);
#endif

	ipc_send_message(IPC_DIR_MSG_TX, IPC_S2M_IMQ_TRX_TRAN, &ipc_message);

	return IMQ_OK;
}

imq_error_t IPC_Message_Queue_Slave_Recv(u8 ID, u8 *buf, int *size)
{
	imq_queue_info_t *queue_info;
	uint32_t queue_addr;
	struct imq_message *rx_message;
	uint32_t offset = 0;
	uint32_t hdrLen = sizeof(struct imq_message);
	uint32_t alignSize = 0;
	uint32_t startAddr, endAddr;
	uint8_t segmentNeed = 0, switchNeed = 0;
	uint32_t segmentLen = 0;


	queue_addr = IMQ_QUEUE_INFO_BASE_ADDR + ID * sizeof(imq_queue_info_t);
	queue_info = (imq_queue_info_t *)queue_addr;
	DCache_Invalidate((u32)queue_info, sizeof(imq_queue_info_t));

	startAddr = queue_info->m_addr;
	endAddr = queue_info->m_addr + queue_info->m_len;

	/* Recv message from master, get rx_message */

	if (endAddr - queue_info->s_rptr < hdrLen) {
		switchNeed = 1;
		/* Read pointer switch to the start of queue ring buffer */
		queue_info->s_rptr = queue_info->m_addr;
	}

	rx_message = (struct imq_message *)queue_info->s_rptr;
	DCache_Invalidate((u32)rx_message, sizeof(struct imq_message));

	if (rx_message->msg_len < 0 || rx_message->msg_len > queue_info->m_len) {
		printf("Slave %d recv error, msg_len: %d\n", ID, rx_message->msg_len);
		return IMQ_RX_ERROR;
	}

	/* Size need aligment with 32bytes */
	alignSize = rx_message->msg_len + hdrLen;
	IPC_BYTE_ALIGNMENT(alignSize);

	if (queue_info->s_rptr + alignSize > endAddr && !switchNeed) {
		segmentNeed = 1;
		segmentLen = endAddr - queue_info->s_rptr;
	}

	/* Get data buffer */

	if (segmentNeed) {
		DCache_Invalidate(queue_info->s_rptr, segmentLen);
		offset = segmentLen - hdrLen;
		/* Get buf from queue ring buffer */
		memcpy(buf, queue_info->s_rptr + hdrLen, offset);

		/* Read pointer switch to the start of queue ring buffer */
		queue_info->s_rptr = queue_info->m_addr;

		DCache_Invalidate(queue_info->s_rptr, alignSize - segmentLen);
		memcpy(buf + offset, queue_info->s_rptr, rx_message->msg_len - offset);
		/* Update slave read pointer */
		queue_info->s_rptr += (alignSize - segmentLen);
	} else {
		DCache_Invalidate(queue_info->s_rptr, alignSize);
		/* Get buf from queue ring buffer */
		memcpy(buf, queue_info->s_rptr + hdrLen, rx_message->msg_len);
		/* Update slave read pointer */
		queue_info->s_rptr += alignSize;
	}

	*size = rx_message->msg_len;

	DCache_Clean((u32)queue_info, sizeof(imq_queue_info_t));

	//printf("%s, id: %d, size: %d,r_ptr: 0x%08x, w_ptr: 0x%08x\n", __func__,
	//			ID, rx_message->msg_len, queue_info->s_rptr, queue_info->m_wptr);

#if IMQ_CHKSUM_DEBUG
	/* Verify checksum */
	int chksum = 0;
	for (int i = 0; i < rx_message->msg_len; i++) {
		chksum += buf[i];
	}
	if (chksum != rx_message->chksum) {
		printf("Slave recv checksum error, id: %d, checksum0: %d, checksum1: %d\n", ID, chksum, rx_message->chksum);
	}
#endif

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_22, 0);
#endif

	return IMQ_OK;
}


imq_error_t IPC_Message_Queue_Recv(u8 ID, u8 *buf, int *size)
{
	/* Block receive */
	xSemaphoreTake(g_ipc_slave_wake_sema[ID], 0xFFFFFFFF);
	return IPC_Message_Queue_Slave_Recv(ID, buf, size);
}

imq_error_t IPC_Message_Queue_Recv_Timeout(u8 ID, u8 *buf, int *size, int timeout)
{
	if (xSemaphoreTake(g_ipc_slave_wake_sema[ID], timeout) != pdTRUE) {
		return IMQ_RX_TIMEOUT;
	}
	return IPC_Message_Queue_Slave_Recv(ID, buf, size);
}
#if 0
imq_error_t share_memory_region_init()
{
	uint32_t xTotalLen = 0;
	uint32_t xAddress;
	struct imq_share_buffer *pStartBlock;
	struct imq_share_buffer *pEndBlock;
	imq_share_heap_t *pxHeapHeader;

	pxHeapHeader = (imq_share_heap_t *)IMQ_SHARE_MEM_BASE_ADDR;
	xTotalLen = IMQ_SHARE_MEM_MAX_LEN;

	pxHeapHeader->remainFreeBytes = IMQ_SHARE_MEM_MAX_LEN - xShareHeapHeaderSize; // not include pxHeapHeader
	pxHeapHeader->startBlock = IMQ_QUEUE_MASTER_BASE_ADDR + xShareHeapHeaderSize;
	pxHeapHeader->totalLen = IMQ_SHARE_MEM_MAX_LEN - xShareHeapHeaderSize; // not include pxHeapHeader
	pxHeapHeader->init = 1;

	/* initialize heap region */
	xTotalLen = pxHeapHeader->totalLen;
	xAddress = pxHeapHeader->startBlock;

	pEndBlock = (struct imq_share_buffer *)(xAddress + xTotalLen - xShareBlockHeaderSize);
	pEndBlock->xBlockSize = 0;
	pEndBlock->fd = 0xFFFFFFFF;
	pEndBlock->ref = 0;
	pEndBlock->pNextUsedBlock = NULL;
	pEndBlock->pNextFreeBlock = NULL;

	pStartBlock = (struct imq_share_buffer *)xAddress;
	pStartBlock->xBlockSize = xTotalLen - 2 * xShareBlockHeaderSize; // not include xShareBlockHeaderSize
	pStartBlock->fd = 0;
	pStartBlock->ref = 0;
	pStartBlock->pNextUsedBlock = pEndBlock;
	pStartBlock->pNextFreeBlock = pEndBlock;

	pxHeapHeader->endBlock = (u32)pEndBlock;

	DCache_Clean((u32)pxHeapHeader, xShareHeapHeaderSize);

	return IMQ_OK;
}

static void share_memory_free_block_insert(struct imq_share_buffer *pxBlockToInsert)
{
	imq_share_heap_t *pxHeapHeader;
	struct imq_share_buffer *pxIterator;
	struct imq_share_buffer *pxEndBlock;
	uint8_t *puc;

	pxHeapHeader = (imq_share_heap_t *)IMQ_SHARE_MEM_BASE_ADDR;
	pxIterator = (struct imq_share_buffer *)pxHeapHeader->startBlock;
	pxEndBlock = (struct imq_share_buffer *)pxHeapHeader->endBlock;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for (pxIterator; pxIterator < pxBlockToInsert; pxIterator = pxIterator->pNextFreeBlock) {
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = (uint8_t *) pxIterator;
	if ((puc + pxIterator->xBlockSize) == (uint8_t *) pxBlockToInsert) {
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = (uint8_t *) pxBlockToInsert;
	if ((puc + pxBlockToInsert->xBlockSize) == (uint8_t *) pxIterator->pNextFreeBlock) {
		if (pxIterator->pNextFreeBlock != pxEndBlock) {
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pNextFreeBlock->xBlockSize;
			pxBlockToInsert->pNextFreeBlock = pxIterator->pNextFreeBlock->pNextFreeBlock;
		} else {
			pxBlockToInsert->pNextFreeBlock = pxEndBlock;
		}
	} else {
		pxBlockToInsert->pNextFreeBlock = pxIterator->pNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if (pxIterator != pxBlockToInsert) {
		pxIterator->pNextFreeBlock = pxBlockToInsert;
	}
}

static void share_memory_used_block_insert(struct imq_share_buffer *pxBlockToInsert)
{
	imq_share_heap_t *pxHeapHeader;
	struct imq_share_buffer *pxIterator, *pxPreviousBlock;

	pxHeapHeader = (imq_share_heap_t *)IMQ_SHARE_MEM_BASE_ADDR;
	pxIterator = (struct imq_share_buffer *)pxHeapHeader->startBlock;

	/* Iterate to last position */
	while (pxIterator->pNextUsedBlock != NULL) {
		pxPreviousBlock = pxIterator;
		pxIterator = pxIterator->pNextUsedBlock;
	}

	pxBlockToInsert->pNextUsedBlock = pxPreviousBlock->pNextUsedBlock;
	pxPreviousBlock->pNextUsedBlock = pxBlockToInsert;
}

static void share_memory_used_block_delete(struct imq_share_buffer *pxBlockToDelete)
{
	imq_share_heap_t *pxHeapHeader;
	struct imq_share_buffer *pxIterator, *pxBlock;

	pxHeapHeader = (imq_share_heap_t *)IMQ_SHARE_MEM_BASE_ADDR;
	pxIterator = (struct imq_share_buffer *)pxHeapHeader->startBlock;

	/*  */
	while (pxIterator->pNextUsedBlock != NULL && pxIterator != pxBlockToDelete) {
		pxBlock = pxIterator;
		pxIterator = pxIterator->pNextUsedBlock;
	}

	//pxBlockToInsert->pNextUsedBlock = pxBlock->pNextUsedBlock;
	//pxBlock->pNextUsedBlock = pxBlockToInsert;
}


imq_error_t share_malloc(int size, struct imq_share_buffer_handle *handle)
{
	imq_share_heap_t *pxHeapHeader;
	struct imq_share_buffer *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
	struct imq_share_buffer *pxEndBlock;
	uint32_t xFreeBytes;


	pxHeapHeader = (imq_share_heap_t *)IMQ_SHARE_MEM_BASE_ADDR;

	/* check if other core locked */
	while (pxHeapHeader->lock != 0) {
		DCache_Invalidate((u32)pxHeapHeader, sizeof(imq_share_heap_t));
	}

	/* acquire the lock */
	pxHeapHeader->lock |= IMQ_CORE_KM4;

	pxEndBlock = (struct imq_share_buffer *)pxHeapHeader->endBlock;
	xFreeBytes = pxHeapHeader->remainFreeBytes;


	if ((size > 0) && (size <= xFreeBytes)) {
		pxBlock = (imq_share_heap_t *)(pxHeapHeader->startBlock);
		pxPreviousBlock = (imq_share_heap_t *)(pxHeapHeader->startBlock);
		/* find the target block */
		while ((pxBlock->xBlockSize < size) && (pxBlock->pNextFreeBlock != NULL)) {
			pxPreviousBlock = pxBlock;
			pxBlock = pxBlock->pNextFreeBlock;
		}

		/* pxBlock is not the end block */
		if (pxBlock != pxEndBlock) {
			handle->addr = (u32)((uint8_t *)pxBlock + xShareBlockHeaderSize);

			pxPreviousBlock->pNextFreeBlock = pxBlock->pNextFreeBlock;


			/* If the block is larger than required it can be split into
						two. */
			if ((pxBlock->xBlockSize - size) > heapMINIMUM_BLOCK_SIZE) {
				/* This block is to be split into two.  Create a new
				block following the number of bytes requested. The void
				cast is used to prevent byte alignment warnings from the
				compiler. */
				pxNewBlockLink = (void *)(((uint8_t *) pxBlock) + size);

				/* Calculate the sizes of two blocks split from the
				single block. */
				pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - size;
				pxBlock->xBlockSize = size;

				/* Insert the new block into the list of free blocks. */
				share_memory_free_block_insert(pxNewBlockLink);
			}

		}

		handle->size = pxBlock->xBlockSize;
		handle->fd = pxBlock->fd;

		pxHeapHeader->remainFreeBytes -= size;
		/* Insert pxBlock into the list of used blocks */
		share_memory_used_block_insert(pxBlock);
	} else {
		/* release the lock */
		pxHeapHeader->lock &= ~IMQ_CORE_KM4;

		DCache_Clean((u32)pxHeapHeader, sizeof(imq_share_heap_t));

		return IMQ_NO_MEMORY;
	}


	/* release the lock */
	pxHeapHeader->lock &= ~IMQ_CORE_KM4;

	DCache_Clean((u32)pxHeapHeader, sizeof(imq_share_heap_t));

	return IMQ_OK;
}

void share_free(int fd)
{
	/* Get memory address accroding to the fd */

	return;
}

imq_error_t share_memory_tx_sync(int fd)
{
	return IMQ_OK;
}

imq_error_t share_memory_rx_sync(int fd, struct imq_share_buffer_handle *handle)
{
	return IMQ_OK;
}

#endif
void share_memory_ipc_slave_int_hdl(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

	/* up the semaphore according to the id */
	PIPC_MSG_STRUCT p_ipc_recv_msg = ipc_get_message(IPC_DIR_MSG_RX, IPC_M2S_IMQ_TRX_TRAN);
	DCache_Invalidate((u32)p_ipc_recv_msg, sizeof(IPC_MSG_STRUCT));
	uint32_t ID = p_ipc_recv_msg->msg;
	portBASE_TYPE taskWoken = pdFALSE;

#if IMQ_GPIO_MEAS
	GPIO_WriteBit(PB_31, 0);
#endif

	xSemaphoreGiveFromISR(g_ipc_slave_wake_sema[ID], &taskWoken);
	portEND_SWITCHING_ISR(taskWoken);
}

/* ---------------------------- Global Variables ---------------------------- */
#if defined(CONFIG_PLATFORM_AMEBALITE) || defined(CONFIG_PLATFORM_AMEBAD2)
IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_message_slave_table[] = {
	{IPC_USER_POINT,	share_memory_ipc_slave_int_hdl,	(VOID *) NULL, IPC_DIR_MSG_RX, IPC_M2S_IMQ_TRX_TRAN, IPC_RX_FULL},
};
#endif


