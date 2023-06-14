#include "Test1RPC_Agent_data.h"
#include "Test1RPC_System.h"
#include "Test1RPC_Agent.h"

#include "ameba_soc.h"
#include "rpc_api.h"


#ifdef ARM_CORE_CM4
#define RPC_CHANNEL_AUDIO (RPC_AP_NP<<4) | (BLOCK_MODE | (RPC_NP_AP << 1))
#elif defined(RSICV_CORE_KR4)
#define RPC_CHANNEL_AUDIO (RPC_NP_AP<<4) | (BLOCK_MODE | (RPC_AP_NP << 1))
#endif
#define DEFAULT_STACK_SIZE 1024


static HRESULT *Test_Init(TEST1_RPC_AFE_INIT_CONFIG *pParam, RPC_STRUCT *pRpcStruct, HRESULT *pRes)
{
	(void)pParam;
	(void)pRpcStruct;
	(void)pRes;
	*pRes = 0;
	return pRes;
}
static HRESULT *Test_DataSync(TEST1_RPC_AFE_PROCESS_BUFS *pParam, RPC_STRUCT *pRpcStruct, HRESULT *pRes)
{
	(void)pParam;
	(void)pRpcStruct;
	(void)pRes;
	printf("Test_DataSync\n");
	//vTaskDelay(1);
	*pRes = 0;
	return pRes;
}
static TEST1_VOID *Test_Release(TEST1_VOID *pParam, RPC_STRUCT *pRpcStruct, TEST1_VOID *pRes)
{
	(void)pParam;
	(void)pRpcStruct;
	(void)pRes;
	*pRes = 0;
	return pRes;
}
void KR4TestTask(void *params)
{
	(void)params;
	printf("KR4TestTask\n");
	p_TEST1_RPC_ToAgent_AFE_Init_0_svc = Test_Init;
	p_TEST1_RPC_ToAgent_AFE_Process_0_svc = Test_DataSync;
	p_TEST1_RPC_ToAgent_AFE_Destroy_0_svc = Test_Release;
	vTaskDelete(NULL);
}

void example_rpc_test(void)
{
	printf("%s Enter.\n", __FUNCTION__);
	xTaskCreate(KR4TestTask, "KR4TestTask", DEFAULT_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
}

void test_rpc(void)
{
	printf("hello MCU world GO test_rpc\n");
	example_rpc_test();
}