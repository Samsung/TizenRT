#include "Test1RPC_System.h"
#include "Test1RPC_Agent.h"

#include "ameba_soc.h"
#include "osdep_service.h"
#include "rpc_api.h"
#include "rpc_struct.h"


#ifdef ARM_CORE_CM4
#define TEST1_RPC_CHANNEL (BLOCK_MODE | (RPC_NP_AP << 1) | (RPC_AP_NP << 4))
#else
#define TEST1_RPC_CHANNEL (BLOCK_MODE | (RPC_AP_NP << 1) | (RPC_NP_AP << 4))
#endif


int RTTEST1_AFE_Init(int rate, int frame_length)
{
	int ret_value = -1;
	TEST1_RPC_AFE_INIT_CONFIG config;
	config.rate = rate;
	config.frame_length = frame_length;
	printf("TEST1_RPC_ToAgent_AFE_Init_0 Start\n");
	CLNT_STRUCT clnt = RPC_PrepareCLNT(TEST1_RPC_CHANNEL, TEST1_SYSTEM, 0);
	HRESULT *res = TEST1_RPC_ToAgent_AFE_Init_0(&config, &clnt);
	if (res) {
		ret_value = (int)(*res);
		free(res);
	}
	printf("TEST1_RPC_ToAgent_AFE_Init_0 End\n");
	return ret_value;
}

int RTTEST1_AFE_Process(long mic1, long mic2, long ref, long out)
{
	int ret_value = -1;
	TEST1_RPC_AFE_PROCESS_BUFS bufs;
	bufs.mic_1 = mic1;
	bufs.mic_2 = mic2;
	bufs.ref = ref;
	bufs.mic_out = out;
	printf("TEST1_RPC_ToAgent_AFE_Process_0 Start\n");
	CLNT_STRUCT clnt = RPC_PrepareCLNT(TEST1_RPC_CHANNEL, TEST1_SYSTEM, 0);
	HRESULT *res = TEST1_RPC_ToAgent_AFE_Process_0(&bufs, &clnt);
	printf("TEST1_RPC_ToAgent_AFE_Process_0 End\n");
	if (res) {
		ret_value = (int)(*res);
		free(res);
	}
	return ret_value;
}

void RTTEST1_AFE_Destroy(void)
{
	printf("TEST1_RPC_ToAgent_AFE_Destroy_0 Start\n");
	TEST1_VOID v = 0;
	CLNT_STRUCT clnt = RPC_PrepareCLNT(TEST1_RPC_CHANNEL, TEST1_SYSTEM, 0);
	TEST1_VOID *res = TEST1_RPC_ToAgent_AFE_Destroy_0(&v, &clnt);
	if (res) {
		free(res);
	}
	printf("TEST1_RPC_ToAgent_AFE_Destroy_0 End\n");
}

void TestTask1(void *param)
{
	(void)param;
	printf("TestTask1 Start\n");
	vTaskDelay(3000);
	int count = 1000;
	RTTEST1_AFE_Init(0, 0);
	while (count > 0) {
		RTTEST1_AFE_Process(0, 0, 0, 0);
		//count--;
		vTaskDelay(2);
	}
	RTTEST1_AFE_Destroy();
	vTaskDelete(NULL);
}

void test_rpc1(void)
{
	printf("test_rpc1 Start\n");
	if (xTaskCreate(TestTask1, ((const char *)"TestTask1"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(TestTask1) failed", __FUNCTION__);
	}
}