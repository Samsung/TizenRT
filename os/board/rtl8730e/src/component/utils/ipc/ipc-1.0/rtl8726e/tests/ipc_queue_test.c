#include "ameba_soc.h"
#include "osdep_service.h"
#include "ipc_message_queue.h"

#define IMQ_BUF_SIZE 			512
#define IMQ_TEST_COUNT			10000
#define TEST_ID					1

static void IPCServerTask(void *param)
{
	vTaskDelay(1000);
	(void)param;
	printf("###########%s Enter.\n", __FUNCTION__);
	u8 buf[IMQ_BUF_SIZE];
	uint32_t size;
	int count = 0;
	int total_size = 0;
	printf("%s, rx task start!\n", __FUNCTION__);
	while (count < IMQ_TEST_COUNT) {
		rtw_memset(buf, 0x00, IMQ_BUF_SIZE);
		size = IMQ_BUF_SIZE;
		imq_error_t ret = IPC_Message_Queue_Recv(TEST_ID, buf, &size);
		count++;
		total_size += size;
		//if (count % 10 == 0)
		{
			printf("%x recv %d packets, %d bytes\n", TEST_ID, count, total_size);
		}
	}
	printf("%x recv %d packets, %d bytes\n", TEST_ID, count, total_size);
	vTaskDelete(NULL);
}

static void IPCClientTask(void *param)
{
	vTaskDelay(5000);
	(void)param;
	printf("***************%s Enter.\n", __FUNCTION__);
	u8 buf[IMQ_BUF_SIZE];
	int count = 0;
	int total_size = 0;
	printf("%s, tx task start!\n", __FUNCTION__);
	while (count < IMQ_TEST_COUNT) {
		rtw_memset(buf, 0x00, IMQ_BUF_SIZE);
		rtw_get_random_bytes(buf, IMQ_BUF_SIZE);
		count++;
		total_size += IMQ_BUF_SIZE;
		imq_error_t ret = IPC_Message_Queue_Send(TEST_ID, buf, IMQ_BUF_SIZE, IMQ_WAIT_FOREVER);
		rtw_msleep_os(1);
		//if (count % 10 == 0)
		{
			printf("%x send %d packets, %d bytes\n", TEST_ID, count, total_size);
		}
		//printf("%s, IPC_Message_Queue_Send = %d\n", __FUNCTION__, ret);
	}
	vTaskDelete(NULL);
}

static void TestTask(void *param)
{
#ifdef ARM_CORE_CM4
	if (xTaskCreate(IPCServerTask, ((const char *)"IPCServerTask"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(IPCServerTask) failed", __FUNCTION__);
	}
#else
	if (xTaskCreate(IPCClientTask, ((const char *)"IPCClientTask"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(IPCClientTask) failed", __FUNCTION__);
	}
#endif
	vTaskDelete(NULL);
}

static void ipc_queue_test(void)
{
	if (xTaskCreate(TestTask, ((const char *)"TestTask"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(TestTask) failed", __FUNCTION__);
	}
}

CMD_TABLE_DATA_SECTION
const COMMAND_TABLE ipc_test_cmd_table[] = {
	{
		(const u8 *)"ipc_queue_test",  1, ipc_queue_test, (const u8 *)"\t ipc_queue_test \n"
	},
};