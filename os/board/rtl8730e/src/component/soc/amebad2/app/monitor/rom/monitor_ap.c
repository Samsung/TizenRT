/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

#if defined  (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

u32
CmdRamHelp(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	/* To avoid gcc warnings */
	(void) argc;
	(void) *argv;

	COMMAND_TABLE *cmd_table = (COMMAND_TABLE *)__cmd_table_start__;
	u32 cmd_mum = ((__cmd_table_end__ - __cmd_table_start__) / sizeof(COMMAND_TABLE));
	u32	index ;

	DiagPrintfD("----------------- TEST COMMAND MODE HELP %d------------------\n", cmd_mum);
	for (index = 0  ; index < cmd_mum; index++) {
		if (cmd_table[index].msg) {
			DiagPrintfD("%s\n", cmd_table[index].msg);
		}
	}
	DiagPrintfD("----------------- TEST COMMAND MODE END  %x------------------\n", cmd_mum);

	return _TRUE ;
}

static u32
cmd_reboot(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	if (_strcmp((const char *)argv[0], "uartburn") == 0) {
		BKUP_Set(0, BKUP_BIT_UARTBURN_BOOT);
	} else if (_strcmp((const char *)argv[0], "uartburn_dbg") == 0) {
		BKUP_Set(0, BKUP_BIT_UARTBURN_BOOT | BKUP_BIT_UARTBURN_DEBUG);
	}

	DBG_8195A("\n\rRebooting ...\n\r");
#ifdef AMEBAD2_TODO
	NVIC_SystemReset();
#endif
	return _TRUE;
}

u32 cmd_dump_word(u16 argc, u8  *argv[])
{
	u32 Src;
	//u32 OTF_Enable = SYSCFG_OTP_RSIPEn();

	if (argc < 1) {
		MONITOR_LOG("Wrong argument number!\r\n");
		return _FALSE;
	}

	if (argv[0]) {
		Src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
	} else {
		MONITOR_LOG("Wrong argument number!\r\n");
		return _FALSE;
	}

	Src &= ~(0x03);

	if (IS_FLASH_ADDR(Src)) {
		MONITOR_LOG("Can't Read Flash!\r\n");
		return TRUE;
	}


	/* read encrypt image for FW protection */
	//if ((Src & 0x08000000) != 0) {
	//	if (OTF_Enable != 0) {
	//		RSIP_OTF_Cmd(DISABLE);
	//	}
	//}

	MONITOR_LOG("%08X: %08X \n", Src, *(u32 *)(Src));

	//if ((Src & 0x08000000) != 0) {
	//	if (OTF_Enable != 0) {
	//		RSIP_OTF_Cmd(ENABLE);
	//		Cache_Flush();
	//	}
	//}

	return _TRUE;

}

u32 cmd_write_word(u16 argc, u8  *argv[])
{
	UNUSED(argc);

	u32 Src;
	u32 Value;

	Src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);

	Src &= ~(0x03);

	Value = _strtoul((const char *)(argv[1]), (char **)NULL, 16);
	DBG_8195A("%08X: %08X \n", Src, Value);

	*(volatile u32 *)(Src) = Value;

	DCache_Clean(Src, sizeof(u32));

	return 0;
}

/*
u32
CmdTickPS(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	if (_strcmp((const char *)argv[0], "r") == 0) { // release
		if (argc >= 2 && _strcmp((const char *)argv[1], "debug") == 0) {
			pmu_tickless_debug(ENABLE);
		} else {
			pmu_tickless_debug(DISABLE);
		}

		pmu_release_wakelock(PMU_OS);
	}

	if (_strcmp((const char *)argv[0], "a") == 0) { // acquire
		pmu_acquire_wakelock(PMU_OS);
	}

	if (_strcmp((const char *)argv[0], "cpuhp") == 0) { // secondary core power on/off
		if (argc >= 2 && _strcmp((const char *)argv[1], "off") == 0) {
			vPortSecondaryOff();
		} else if (argc >= 2 && _strcmp((const char *)argv[1], "on") == 0) {
			smp_init();
		}
	}

	if (_strcmp((const char *)argv[0], "type") == 0) { // PG or CG
		if (argc >= 2 && _strcmp((const char *)argv[1], "pg") == 0) {
			pmu_set_sleep_type(SLEEP_PG);
		} else if (argc >= 2  && _strcmp((const char *)argv[1], "cg") == 0) {
			pmu_set_sleep_type(SLEEP_CG);
		} else {
			pmu_set_sleep_type(SLEEP_CG);
		}
	}

	if (_strcmp((const char *)argv[0], "get") == 0) { // get sleep & wake time
		DBG_8195A("lockbit:%x \n", pmu_get_wakelock_status());
		DBG_8195A("dslp_lockbit:%x\n", pmu_get_deepwakelock_status());
	}
	return _TRUE;
}
*/
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
static TaskHandle_t task_status_log_task_Handler = NULL;
static char InfoBuffer[1024];
static char InfoBuffer2[1024 * 2];
static char time_delay;

static char *prvWriteNameToBuffer(char *pcBuffer, const char *pcTaskName)
{
	size_t x;

	/* Start by copying the entire string. */
	strcpy(pcBuffer, pcTaskName);

	/* Pad the end of the string with spaces to ensure columns line up when printed out. */
	for (x = strlen(pcBuffer); x < (size_t)(configMAX_TASK_NAME_LEN - 1); x++)	{
		pcBuffer[ x ] = ' ';
	}

	/* Terminate. */
	pcBuffer[ x ] = (char) 0x00;

	/* Return the new end of string. */
	return &(pcBuffer[ x ]);
}

void vTaskGetRunTimeStats_rtk(char *pcWriteBuffer)
{
	TaskStatus_t *pxTaskStatusArray_old;
	TaskStatus_t *pxTaskStatusArray_new;
	UBaseType_t uxArraySize, x1, x2;
	UBaseType_t uxArraySize_old;
	UBaseType_t uxArraySize_new;
	float ulTotalTime, ulStatsAsPercentage;
	uint32_t ulTotalTime_old;
	uint32_t ulTotalTime_new;

	/* Make sure the write buffer does not contain a string. */
	*pcWriteBuffer = (char) 0x00;

	/* Take a snapshot of the number of tasks in case it changes while this function is executing. */
	uxArraySize_old = uxTaskGetNumberOfTasks();

	/* If configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will equate to NULL. */
	pxTaskStatusArray_old = pvPortMalloc(uxArraySize_old * sizeof(TaskStatus_t));

	if (pxTaskStatusArray_old != NULL) {
		uxArraySize_old = uxTaskGetSystemState(pxTaskStatusArray_old, uxArraySize_old, &ulTotalTime_old);
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	vTaskDelay(time_delay * 1000);

	/* Take a snapshot of the number of tasks in case it changes while this function is executing. */
	uxArraySize_new = uxTaskGetNumberOfTasks();

	/* If configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will equate to NULL. */
	pxTaskStatusArray_new = pvPortMalloc(uxArraySize_new * sizeof(TaskStatus_t));

	if (pxTaskStatusArray_new != NULL) {
		uxArraySize_new = uxTaskGetSystemState(pxTaskStatusArray_new, uxArraySize_new, &ulTotalTime_new);
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	/* For percentage calculations. */
	ulTotalTime = (float)((ulTotalTime_new - ulTotalTime_old));
	/* Avoid divide by zero errors. */
	if (ulTotalTime > 0UL) {
		/* Create a human readable table from the binary data. */
		for (x1 = 0; x1 < uxArraySize_new; x1++) {
			for (x2 = 0; x2 < uxArraySize_old; x2++) {
				if (pxTaskStatusArray_new[x1].xTaskNumber == pxTaskStatusArray_old[x2].xTaskNumber) {
					ulStatsAsPercentage = (float)(pxTaskStatusArray_new[ x1 ].ulRunTimeCounter - pxTaskStatusArray_old[ x2 ].ulRunTimeCounter) / ulTotalTime * 100;
					pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray_new[ x1 ].pcTaskName);
					sprintf(pcWriteBuffer, "\t%u\t%-10u \t%8.2f%% \r\n", (unsigned int) pxTaskStatusArray_new[ x1 ].uxCurrentPriority,
							((unsigned int) pxTaskStatusArray_new[ x1 ].ulRunTimeCounter - pxTaskStatusArray_old[ x2 ].ulRunTimeCounter),  ulStatsAsPercentage);
					pcWriteBuffer += strlen(pcWriteBuffer);
					break;
				}
			}
		}
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	vPortFree(pxTaskStatusArray_old);
	vPortFree(pxTaskStatusArray_new);
}

void task_status_log_task(void)
{
	while (1) {
		vTaskGetRunTimeStats_rtk(InfoBuffer);
		DiagPrintf("[CA32] FreeRTOS FreeHeapSize: %d \n", xPortGetFreeHeapSize());
		vTaskList(InfoBuffer2);
		DiagPrintf("[CA32] Name              State  Priority  Stack  Num \n");
		DiagPrintf("%s \r\n", InfoBuffer2);
		DiagPrintf("[CA32] Name             Priority  DeltaRunTime(us)  Percentage(total:200%%)\r\n");
		DiagPrintf("%s\r\n", InfoBuffer);
	}
}

u32 task_status(u16 argc, u8 *argv[])
{
	if (argc < 1) {
		DiagPrintf("need more argument. e.g.  start, stop \n");
		return 1;
	}

	if (!strcmp((const char *)argv[0], "start")) {
		if (argc < 2) {
			DiagPrintf("need time argument. e.g. \"status start 1\" \n");
			return 1;
		} else {
			time_delay = strtoul((const char *)(argv[1]), (char **)NULL, 10);
		}
		if (!task_status_log_task_Handler) {
			xTaskCreate((TaskFunction_t)task_status_log_task,
						(const char *)"task_status_log_task",
						(uint16_t)1024,
						(void *)NULL,
						(UBaseType_t)2,
						(TaskHandle_t *)&task_status_log_task_Handler);
		} else {
			DiagPrintf("[CA32] task_status_log_task task is already running.\n");
		}
	} else if (!strcmp((const char *)argv[0], "stop")) {
		if (task_status_log_task_Handler) {
			vTaskDelete(task_status_log_task_Handler);
			task_status_log_task_Handler = NULL;
		} else {
			DiagPrintf("[CA32] task_status_log_task task is NOT running.\n");
		}
	} else if (argc) {
		DiagPrintf("invalid argument: %s \n", argv[0]);
		return 1;
	}
	return 0;
}
#endif
#endif

CMD_TABLE_DATA_SECTION
static COMMAND_TABLE   shell_cmd_table_rom[] = {
	{
		(const u8 *)"?",		0, CmdRamHelp,	(const u8 *)"\tHELP (~?) \n"
		"\t\t Print this help messag\n"
	},
	{
		(const u8 *)"DW",		2, cmd_dump_word,	(const u8 *)"\tDW <Address, Hex>\n"
		"\t\t Dump memory dword or Read Hw dword register; \n"
		"\t\t Can Dump only one dword at the same time \n"
		"\t\t Unit: 4Bytes \n"
	},
	{
		(const u8 *)"EW",		2, cmd_write_word,	(const u8 *)"\tEW <Address, Hex>\n"
		"\t\t Write memory dword or Write Hw dword register \n"
		"\t\t Can write only one dword at the same time \n"
		"\t\t Ex: EW Address Value \n"
	},
	{
		(const u8 *)"REBOOT",	4, cmd_reboot,	(const u8 *)"\tREBOOT \n"
		"\t\t <item, string> : \n"
		"\t\t item: uartburn or N/A \n"
		"\t\t \n"
	},
	/*
	{
		(const u8 *)"TICKPS",	4, CmdTickPS,	(const u8 *)"\tTICKPS \n"
		"\t\t r: release os wakelock \n"
		"\t\t a: acquire os wakelock \n"
	},
	*/
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
	{
		(const u8 *)"STATUS",  4, task_status, (const u8 *)"\tSTATUS \n"
		"\t\t start <period>: start tasks status log\n"
		"\t\t stop:  stop tasks status log \n"
	},
#endif
#endif
};

u32 cmd_rom_table(void **PTable)
{
	*PTable = (void *)&shell_cmd_table_rom;

	return (sizeof(shell_cmd_table_rom) / sizeof(COMMAND_TABLE));
}


#if defined  (__GNUC__)
#pragma GCC diagnostic pop
#endif
