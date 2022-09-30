/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

/**
  * @brief  Autorun simulation trigger simulation to dump backup2 and backup3 registers.
  * @param  addr: write address to backup2. Addr is 0xFxxxxxxx, the high 4 bits are used as flag for simulation dump.
  * @param  data: write data to backup3.
  * @retval None
  */
VOID app_simulation_dump(u32 addr, u32 data)
{
	if ((SYSCFG_CHIPType_Get() != CHIP_TYPE_RTLSIM) && \
		(SYSCFG_CHIPType_Get() != CHIP_TYPE_PALADIUM)) {
		return;
	}

	BKUP_Clear(BKUP_REG1, BIT(30));
	/*Write the dump data to BKUP2 and BKUP3*/
	BKUP_Write(BKUP_REG2, (addr | 0xF0000000));
	BKUP_Write(BKUP_REG3, data);
	/*Trigger DV env to dump  BKUP2 and BKUP3*/
	BKUP_Set(BKUP_REG1, BIT(30));
}

/**
  * @brief  Autorun simulation trigger wakeup event.
  * @param  event: Simulation trigger event. The high 4bits of backup2 register indicate simulation trigger event.
  *  0x01 means comparator trigger event.
  * @retval None
  */
VOID app_simulation_event_trigger(u8 event)
{
	if ((SYSCFG_CHIPType_Get() != CHIP_TYPE_RTLSIM) && \
		(SYSCFG_CHIPType_Get() != CHIP_TYPE_PALADIUM)) {
		return;
	}

	BKUP_Write(BKUP_REG2, ((u32)(((event) & 0x0000000F) << 28)));
}

VOID app_simulation_task(VOID *Data)
{
	/* To avoid gcc warnings */
	(void) Data;

	//u8  *argv[6];
	static u32 test_done = 0;
	simulation_stage_set(SIMULATION_KM4_CPUID, BIT_KM4_RUN_INTO_FLASH);

	do {
		/* this is an example, you can change to yourself code if needed */
		if (test_done == 0) {
			simulation_stage_set(SIMULATION_KM4_CPUID, BIT_KM4_SIMULATION_START);
#if 0
			//add simulation code here
			argv[0] = "1";
			argv[1] = "2";
			argv[2] = "0";
			argv[3] = "0";
			argv[4] = "1";
			argv[5] = "1";
			CmdDDRTest(6, argv);
#endif
			test_done = 1;
			simulation_stage_set(SIMULATION_KM4_CPUID, BIT_KM4_SIMULATION_END);
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
