/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ameba_soc.h"
#include "osdep_service.h"

#include "ipc.h"


#define IMQ_BUF_SIZE1 			512
#define IMQ_BUF_SIZE2 			512
#define IMQ_TEST_COUNT1			1000
#define IMQ_TEST_COUNT2			1000
#ifdef ARM_CORE_CM4
#ifdef CONFIG_IPC_MSG_MASTER
#define QUEUE_NP_TO_AP			0
#else
#define QUEUE_NP_TO_AP			1
#endif
#else
#ifdef CONFIG_IPC_MSG_MASTER
#define QUEUE_NP_TO_AP			1
#else
#define QUEUE_NP_TO_AP			0
#endif
#endif


extern int rtw_get_random_bytes(void *dst, uint32_t size);

void RPCServerTask(void *param)
{
	(void)param;
	printf("###########%s Enter.\n", __FUNCTION__);

	u8 buf[IMQ_BUF_SIZE1];
	uint32_t size;
	int count = 0;
	int total_size = 0;

	printf("%s, rx task start!\n", __FUNCTION__);

	while (count < IMQ_TEST_COUNT1) {

		memset(buf, 0x00, IMQ_BUF_SIZE1);
		size = IMQ_BUF_SIZE1;
		int32_t ret = IPC2_Message_Queue_Recv(QUEUE_NP_TO_AP, buf, &size);

		count++;
		total_size += size;

		//if (count % 20 == 0)
		{
			printf("%x recv %d packets, %d bytes, ret=%d\n", QUEUE_NP_TO_AP, count, total_size, ret);
		}
	}
	vTaskDelay(200);

	vTaskDelete(NULL);
}

void RPCClientTask(void *param)
{
	(void)param;
	printf("***************%s Enter.\n", __FUNCTION__);
	u8 buf[IMQ_BUF_SIZE2];
	int count = 0;
	int total_size = 0;

	printf("%s, tx task start!\n", __FUNCTION__);

	while (count < IMQ_TEST_COUNT2) {
		memset(buf, 0x00, IMQ_BUF_SIZE2);
		rtw_get_random_bytes(buf, IMQ_BUF_SIZE2);

		count++;
		total_size += IMQ_BUF_SIZE2;
		int32_t ret = IPC2_Message_Queue_Send(QUEUE_NP_TO_AP, buf, IMQ_BUF_SIZE2, WAIT_FOREVER);
		//if (count % 11 == 0)
		{
			printf("%x send %d packets, %d bytes, ret=%d\n", QUEUE_NP_TO_AP, count, total_size, ret);
		}
	}
	vTaskDelay(200);
	vTaskDelete(NULL);
}

void TestTask(void *param)
{
	(void)param;
	printf("TestTask\n");
	IPC2_Message_Queue_Init();
	//vTaskDelay(2000);
#if defined(CONFIG_IPC_MSG_MASTER)
	printf("start RPCServerTask\n");
	if (xTaskCreate(RPCServerTask, ((const char *)"RPCServerTask"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(RPCServerTask) failed", __FUNCTION__);
	}
#else
	printf("start RPCClientTask\n");

	if (xTaskCreate(RPCClientTask, ((const char *)"RPCClientTask"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(RPCClientTask) failed", __FUNCTION__);
	}
#endif
	vTaskDelete(NULL);
}

void test_ipc_message(void)
{
	if (xTaskCreate(TestTask, ((const char *)"TestTask"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(TestTask) failed", __FUNCTION__);
	}
}

CMD_TABLE_DATA_SECTION
const COMMAND_TABLE ipc_message_test_cmd_table[] = {
	{
		(const u8 *)"test_ipc_message",  1, test_ipc_message, (const u8 *)"\t test_ipc_message \n"
	},
};