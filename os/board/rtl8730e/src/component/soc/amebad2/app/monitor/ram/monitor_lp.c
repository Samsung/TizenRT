/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#include "ameba_soc.h"
#include "freertos_pmu.h"


u32
CmdRamHelp(
	IN  u16 argc,
	IN  u8  *argv[]
);

u32
cmd_reboot(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	DBG_8195A("\n\rRebooting ...\n\r");

	if (argc >= 1 && _strcmp((const char *)argv[0], "uartburn") == 0) {
		BKUP_Set(0, BKUP_BIT_UARTBURN_BOOT);
	}

	System_Reset();

	return _TRUE;
}

u32
CmdTickPS(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	/* To avoid gcc warnings */
	UNUSED(argc);

	if (_strcmp((const char *)argv[0], "r") == 0) { // release
		if (_strcmp((const char *)argv[1], "debug") == 0) {
			pmu_tickless_debug(ENABLE);
		} else {
			pmu_tickless_debug(DISABLE);
		}
		pmu_release_wakelock(PMU_OS);
	}

	if (_strcmp((const char *)argv[0], "a") == 0) { // acquire
		pmu_acquire_wakelock(PMU_OS);
	}

	if (_strcmp((const char *)argv[0], "type") == 0) { // PG or CG
		if (_strcmp((const char *)argv[1], "pg") == 0) {
			pmu_set_sleep_type(SLEEP_PG);
		} else if (_strcmp((const char *)argv[1], "cg") == 0) {
			pmu_set_sleep_type(SLEEP_CG);
		} else {
			pmu_set_sleep_type(SLEEP_PG);
		}
	}

	if (_strcmp((const char *)argv[0], "dslp") == 0) {
		u32 duration_ms = _strtoul((const char *)(argv[1]), (char **)NULL, 10);

		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		SOC_AONTimerClearINT();
		SOCPS_AONTimer(duration_ms);
		SOCPS_AONTimerINT_EN(ENABLE);

		SOCPS_DeepSleep_RAM();

	}

	if (_strcmp((const char *)argv[0], "get") == 0) { // get sleep & wake time
		DBG_8195A("lockbit:%x \n", pmu_get_wakelock_status());
		DBG_8195A("dslp_lockbit:%x\n", pmu_get_deepwakelock_status());
	}

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
		MONITOR_LOG("Can't Read Flash in LP!\r\n");
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
	MONITOR_LOG("%08X: %08X \n", Src, Value);

	*(volatile u32 *)(Src) = Value;

	DCache_Clean(Src, sizeof(u32));

	return 0;
}

u32
CmdAGGCmd(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	UNUSED(argc);
	if (_strcmp((const char *)argv[0], "en") == 0) {
		/*enable agg function*/
		LOGUART_WaitTxComplete(); // wait until tx complete
		LOGUART_AGGPathAllCmd(LOGUART_DEV, DISABLE); // close all path
		LOGUART_AGGCmd(LOGUART_DEV, ENABLE); // enable agg function
		LOGUART_AGGPathAllCmd(LOGUART_DEV, ENABLE); // open all path
	} else if (_strcmp((const char *)argv[0], "dis") == 0) {
		/*disable agg function*/
		LOGUART_WaitTxComplete(); // wait until tx complete
		LOGUART_AGGPathAllCmd(LOGUART_DEV, DISABLE); // close all path
		LOGUART_AGGCmd(LOGUART_DEV, DISABLE); // disable agg function
		LOGUART_AGGPathAllCmd(LOGUART_DEV, ENABLE); // open all path
	}

	return 0;
}

CMD_TABLE_DATA_SECTION
const COMMAND_TABLE   shell_cmd_table[] = {
	{
		(const u8 *)"?",		0, CmdRamHelp,	(const u8 *)"\tHELP (@?) : \n"
		"\t\t Print help messag\n"
	},
	{
		(const u8 *)"DW",		2, cmd_dump_word,	(const u8 *)"\t@DW <Address, Hex>\n"
		"\t\t Dump memory dword or Read Hw dword register; \n"
		"\t\t Can Dump only one dword at the same time \n"
		"\t\t Unit: 4Bytes \n"
	},
	{
		(const u8 *)"EW",		2, cmd_write_word,	(const u8 *)"\t@EW <Address, Hex>\n"
		"\t\t Write memory dword or Write Hw dword register \n"
		"\t\t Can write only one dword at the same time \n"
		"\t\t Ex: EW Address Value \n"
	},
	{
		(const u8 *)"REBOOT",	4, cmd_reboot,	(const u8 *)"\t@REBOOT \n"
		"\t\t reboot \n"
		"\t\t reboot uartburn \n"
	},
	{
		(const u8 *)"TICKPS",	4, CmdTickPS,	(const u8 *)"\t@TICKPS \n"
		"\t\t r: release os wakelock \n"
		"\t\t a: acquire os wakelock \n"
	},
	{
		(const u8 *)"AGG",	0, CmdAGGCmd,	(const u8 *)"\t@AGGCmd \n"
		"\t\t enable and disable loguart agg function\n"
	},
};

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

