#include "ipc_message_queue.h"


const IMQ_INIT_TABLE imq_rsvd_queue_config[] = {
	//QUEUE_ID					DIR		 		QUEUE_BUFFER_SIZE	QUEUE_TYPE
	{QUEUE_KM4_TO_KR4, 		IPC_KM4_TO_KR4, 	2048, 				MESSAGE_QUEUE}, // ringbuffer for KM4 -> KR4
	{QUEUE_KR4_TO_KM4, 		IPC_KR4_TO_KM4, 	2048,				MESSAGE_QUEUE}, // ringbuffer for KR4 -> KM4
	{QUEUE_KM4_TO_DSP, 		IPC_KM4_TO_DSP, 	2048,				MESSAGE_QUEUE}, // ringbuffer for KM4 -> DSP
	{QUEUE_DSP_TO_KM4, 		IPC_DSP_TO_KM4, 	2048,				MESSAGE_QUEUE}, // ringbuffer for DSP -> KM4
	{QUEUE_KR4_TO_DSP, 		IPC_KR4_TO_DSP, 	20480,				MESSAGE_QUEUE}, // ringbuffer for KR4 -> DSP
	{QUEUE_DSP_TO_KR4, 		IPC_DSP_TO_KR4,		20480,				MESSAGE_QUEUE}, // ringbuffer for DSP -> KR4

	{0xFFFFFFFF,			OFF,				OFF, 				OFF},	/* Table end */
};


#if defined(CONFIG_AS_INIC_AP)
#define IMQ_INFO_HEAP_SIZE		0x2000
#define IMQ_ENTRY_HEAP_SIZE		0x20000

u8 imq_info_heap[IMQ_INFO_HEAP_SIZE]__attribute__((aligned(128)));
u8 imq_queue_entry_heap[IMQ_ENTRY_HEAP_SIZE]__attribute__((aligned(128)));
#endif
/* For queue info entry */
IPC_HeapRegion_t imq_info_region = {
#if defined(CONFIG_AS_INIC_AP)
	.xStartAddress = (u32)imq_info_heap,
	.xSizeInBytes = sizeof(imq_info_heap),
#else
	.xStartAddress = 0,
	.xSizeInBytes = 0,
#endif
};

/* For MESSAGE_QUEUE and SOCKET_QUEUE */
IPC_HeapRegion_t imq_heap_region = {
#if defined(CONFIG_AS_INIC_AP)
	.xStartAddress = (u32)imq_queue_entry_heap,
	.xSizeInBytes = sizeof(imq_queue_entry_heap),
#else
	.xStartAddress = 0,
	.xSizeInBytes = 0,
#endif
};



void ipc_app_init_thread(void *param)
{
	/* To avoid gcc warnings */
	(void) param;

#if defined(CONFIG_AS_INIC_AP)
	/* init heap address and size in retention ram */
	RRAM_TypeDef *rram = RRAM_DEV;

	memset(imq_info_region.xStartAddress, 0, imq_info_region.xSizeInBytes);
	memset(imq_heap_region.xStartAddress, 0, imq_heap_region.xSizeInBytes);

	imq_printf(_INFO_, "IMQ_INIT_DONE: %lu\n", rram->IMQ_INIT_DONE);

	rram->IMQ_HEAP_ADDR[IMQ_INFO_HEAP_IDX] = imq_info_region.xStartAddress;
	rram->IMQ_HEAP_SIZE[IMQ_INFO_HEAP_IDX] = imq_info_region.xSizeInBytes;
	rram->IMQ_INIT_DONE = 1;

	imq_printf(_INFO_, "IMQ_INIT_DONE: %lu\n", rram->IMQ_INIT_DONE);
#else
	/* Wait until AP init done */
	RRAM_TypeDef *rram = RRAM_DEV;

	imq_printf(_INFO_, "IMQ_INIT_DONE: %lu\n", rram->IMQ_INIT_DONE);

	while (rram->IMQ_INIT_DONE == 0) {
		rtw_msleep_os(2);
	}
	/* get heap address and size from retention ram */
	imq_info_region.xStartAddress = rram->IMQ_HEAP_ADDR[IMQ_INFO_HEAP_IDX];
	imq_info_region.xSizeInBytes = rram->IMQ_HEAP_SIZE[IMQ_INFO_HEAP_IDX];
#endif
	IPC_Message_Queue_Init();
exit:
	rtw_thread_exit();
}

u8 g_ipc_init = 0;
void ipc_app_init(void)
{
	struct task_struct task;
	/* Can only call once! */
	if (g_ipc_init != 0) {
		imq_printf(_ERR_, "%s can only call once!\n", __func__);
		return;
	}

	g_ipc_init = 1;

	if (rtw_create_task(&task, ((const char *)"ipc_init"), 2048, tskIDLE_PRIORITY + 2, ipc_app_init_thread, NULL) != pdPASS) {
		imq_printf(_ERR_, "\n\r%s rtw_create_task(ipc_init) failed", __FUNCTION__);
	}
}
