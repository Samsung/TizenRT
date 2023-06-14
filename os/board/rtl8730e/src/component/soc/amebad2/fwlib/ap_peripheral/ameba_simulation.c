/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

VOID app_simulation_task(VOID *Data)
{
	/* To avoid gcc warnings */
	(void) Data;

	//u32 argc = 4;
	//u8  *argv[5];
	static u32 test_done = 1;

	do {
		/* this is an example, you can change to yourself code if needed */
		if (test_done == 0) {
			//u32 temp = 0;
			//argv[0] = "dhrystone";
			//argv[1] = "3";
			//argv[2] = "0";
			//argv[3] = "single";
			//CmdCpuTest(argc, argv);

			test_done = 0;
		} else {
			vTaskDelay(1000);
		}
	} while (1);
}

VOID app_simulation_init(VOID)
{
	if (pdTRUE != xTaskCreate(app_simulation_task, "simulation_task", 1024,
							  NULL, tskIDLE_PRIORITY + 5, NULL)) {
		DiagPrintf("Create simulation_task Err!!\n");
	}

}

