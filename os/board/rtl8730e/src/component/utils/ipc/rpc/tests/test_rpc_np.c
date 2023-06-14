#include "Test1RPC_Agent.h"
#include "Test2RPC_Agent.h"

#include "ameba_soc.h"
#include "rpc_api.h"


extern void test_rpc1(void);
extern void test_rpc2(void);

void mcu_voice_task(void *param)
{
	(void) param;
	test_rpc1();
	//test_rpc2();

	vTaskDelete(NULL);
}

void test_rpc0(void)
{
	if (xTaskCreate(mcu_voice_task, ((const char *)"mcu_voice_task"), 1024, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate(mcu_voice_task) failed", __FUNCTION__);
	}
}



