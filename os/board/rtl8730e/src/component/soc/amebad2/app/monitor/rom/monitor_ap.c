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

static const char *TAG = "MONITOR";

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

	RTK_LOGI(TAG, "----------------- TEST COMMAND MODE HELP %d------------------\n", cmd_mum);
	for (index = 0  ; index < cmd_mum; index++) {
		if (cmd_table[index].msg) {
			DiagPrintf("%s\n", cmd_table[index].msg);
		}
	}
	RTK_LOGI(TAG, "----------------- TEST COMMAND MODE END  %x------------------\n", cmd_mum);

	return _TRUE ;
}

static u32
cmd_reboot(
	IN  u16 argc,
	IN  u8  *argv[]
)
{
	if (argc >= 1) {
		if (_strcmp((const char *)argv[0], "uartburn") == 0) {
			BKUP_Set(0, BKUP_BIT_UARTBURN_BOOT);
		} else if (_strcmp((const char *)argv[0], "uartburn_dbg") == 0) {
			BKUP_Set(0, BKUP_BIT_UARTBURN_BOOT | BKUP_BIT_UARTBURN_DEBUG);
		}
	}

	RTK_LOGI(TAG, "Rebooting ...\n\r");

	System_Reset();

	return _TRUE;
}

#ifndef CONFIG_PLATFORM_TIZENRT_OS
u32 cmd_efuse_protect(u16 argc, u8  *argv[])
{
	/* To avoid gcc warnings */
	(void) argc;

	u8 EfuseBuf[OTP_REAL_CONTENT_LEN];
	u32 index;
	u8 ret = 0;

	/* efuse wmap 0x0 2 2187 */
	/* efuse wmap 0x18 4 01020304 */
	if (_strcmp((const char *)argv[0], "wmap") == 0) {
		u32 Addr = _strtoul((const char *)(argv[1]), (char **)NULL, 16);
		u32 Len = _strtoul((const char *)(argv[2]), (char **)NULL, 16);
		char *DString = (char *)argv[3];
		u32 Cnt;

		Cnt = _strlen(DString);
		if (Cnt % 2) {
			RTK_LOGW(TAG, "string length(%d) should be odd \n", Cnt);
			return FALSE;
		}

		Cnt = Cnt / 2;
		if (Cnt != Len) {
			RTK_LOGW(TAG, "Oops: write lenth not match input string lentg, choose smaller one\n");
			Len = (Cnt < Len) ? Cnt : Len;
		}

		RTK_LOGI(TAG, "efuse wmap write len:%d, string len:%d\n", Len, Cnt << 1);

		for (index = 0; index < Len; index++) {
			EfuseBuf[index] = _2char2hex(DString[index * 2], DString[index * 2 + 1]);
		}

		OTP_LogicalMap_Write(Addr, Len, (u8 *)(EfuseBuf));
	}

	if (_strcmp((const char *)argv[0], "rmap") == 0) {
		RTK_LOGI(TAG, "efuse rmap \n");

		ret = OTP_LogicalMap_Read(EfuseBuf, 0, OTP_LMAP_LEN);
		if (ret == _FAIL) {
			RTK_LOGW(TAG, "OTP_LogicalMap_Read fail \n");
		}

		for (index = 0; index < 1024; index += 16) {
			DiagPrintf("EFUSE[%03x]: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", index,
					   EfuseBuf[index], EfuseBuf[index + 1], EfuseBuf[index + 2], EfuseBuf[index + 3],
					   EfuseBuf[index + 4], EfuseBuf[index + 5], EfuseBuf[index + 6], EfuseBuf[index + 7],
					   EfuseBuf[index + 8], EfuseBuf[index + 9], EfuseBuf[index + 10], EfuseBuf[index + 11],
					   EfuseBuf[index + 12], EfuseBuf[index + 13], EfuseBuf[index + 14], EfuseBuf[index + 15]);
		}
	}

	if (_strcmp((const char *)argv[0], "rraw") == 0) {
		RTK_LOGI(TAG, "efuse rraw\n");

		for (index = 0; index < OTP_USER_END; index++) {
			OTP_Read8(index, (EfuseBuf + index));
		}

		for (index = 0; index < OTP_USER_END; index += 16) {
			DiagPrintf("RawMap[%03x]: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", index,
					   EfuseBuf[index], EfuseBuf[index + 1], EfuseBuf[index + 2], EfuseBuf[index + 3],
					   EfuseBuf[index + 4], EfuseBuf[index + 5], EfuseBuf[index + 6], EfuseBuf[index + 7],
					   EfuseBuf[index + 8], EfuseBuf[index + 9], EfuseBuf[index + 10], EfuseBuf[index + 11],
					   EfuseBuf[index + 12], EfuseBuf[index + 13], EfuseBuf[index + 14], EfuseBuf[index + 15]);
		}
	}

	/* efuse wraw 0xA0 1 aa */
	/* efuse wraw 0xA0 2 aabb */
	/* efuse wraw 0xA0 4 aabbccdd */
	if (_strcmp((const char *)argv[0], "wraw") == 0) {
		u32 Addr = _strtoul((const char *)(argv[1]), (char **)NULL, 16);
		u32 Len = _strtoul((const char *)(argv[2]), (char **)NULL, 16);
		char *DString = (char *)argv[3];
		u32 Cnt;

		Cnt = _strlen(DString);
		if (Cnt % 2) {
			RTK_LOGW(TAG, "string length(%d) should be odd \n", Cnt);
			return FALSE;
		}

		Cnt = Cnt / 2;
		if (Cnt != Len) {
			RTK_LOGW(TAG, "Oops: write lenth not match input string lentg, choose smaller one\n");
			Len = (Cnt < Len) ? Cnt : Len;
		}

		for (index = 0; index < Len; index++) {
			EfuseBuf[index] = _2char2hex(DString[index * 2], DString[index * 2 + 1]);
		}

		RTK_LOGI(TAG, "efuse wraw write len:%d, string len:%d\n", Len, Cnt << 1);

		for (index = 0; index < Len; index++) {
			DiagPrintf("wraw: %x %x \n", Addr + index, EfuseBuf[index]);
			OTP_Write8((Addr + index), EfuseBuf[index]);
		}
	}

	if (_strcmp((const char *)argv[0], "getcrc") == 0) {
		u32 crc = OTPGetCRC();
		RTK_LOGI(TAG, "new crc value is 0x%x", crc);
	}

	return 0;
}
#endif

u32 cmd_dump_word(u16 argc, u8  *argv[])
{
	u32 Src = 0x0;
	u32 Len = 1; //default value
	u32 Is_Byte = _FALSE;
//	u32 OTF_Enable = SYSCFG_OTP_RSIPEn();

	/* get parameters */
	if (argc < 1 || argc > 3) {
		RTK_LOGE(TAG, "Wrong argument number!\r\n");
		return _FALSE;
	}
	if (argc >= 3) {
		Is_Byte = *((const char *)argv[2]) == 'B' || *((const char *)argv[2]) == 'b';
	}
	if (argc >= 2) {
		Len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
	}
	if (argc >= 1) {
		Src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
	}

	/* align */
	Src &= ~(0x03);

	if (IS_FLASH_ADDR(Src)) {
		RTK_LOGW(TAG, "Can't Read Flash!\r\n");
		return TRUE;
	}

	/* read encrypt image for FW protection */
	//if ((Src & 0x08000000) != 0) {
	//	if (OTF_Enable != 0) {
	//		RSIP_OTF_Cmd(DISABLE);
	//	}
	//}

	if (Is_Byte && Len >= 1) {
		DCache_CleanInvalidate(Src, sizeof(u8) * Len);
		rtk_log_memory_dump_byte((u8 *)Src, Len);
	} else if (Len >= 1) {
		DCache_CleanInvalidate(Src, sizeof(u32) * Len);
		rtk_log_memory_dump_word((u32 *)Src, Len);
	} else {
		RTK_LOGE(TAG, "Wrong cmd!\r\n");
		return _FALSE;
	}

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
	DiagPrintf("[%08X] %08X \n", Src, Value);

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

	if (_strcmp((const char *)argv[0], "dslp") == 0) { // release
		pmu_release_deepwakelock(PMU_OS);
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
		RTK_LOGI(TAG, "lockbit:%x \n", pmu_get_wakelock_status());
		RTK_LOGI(TAG, "dslp_lockbit:%x\n", pmu_get_deepwakelock_status());
	}
	return _TRUE;
}
*/
u32 cmd_log_set(u16 argc, u8  *argv[])
{
	if (argc != 2) {
		RTK_LOGE(TAG, "Wrong argument number!\r\n");
		return _FALSE;
	}

	rtk_log_level_t Value = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
	rtk_log_level_set((const char *)argv[0], Value);

	return _TRUE;
}
CMD_TABLE_DATA_SECTION
static COMMAND_TABLE   shell_cmd_table_rom[] = {
	{
		(const u8 *)"?",		0, CmdRamHelp,	(const u8 *)"\tHELP (~?) \n"
		"\t\t Print this help messag\n"
	},
	{
		(const u8 *)"DW",		4, cmd_dump_word,	(const u8 *)"\tDW <Address, Length>\n"
		"\t\t Dump memory word or Read Hw word register; \n"
		"\t\t DW <addr> 		 Dump only one word at the same time, unit: 4Bytes\n"
		"\t\t DW <addr> <len>	 Dump the specified length of the word, unit: 4Bytes\n"
		"\t\t DW <addr> <len> b  Dump the specified length of the byte, unit: 1Byte\n"
	},
	{
		(const u8 *)"EW",		2, cmd_write_word,	(const u8 *)"\tEW <Address, Hex>\n"
		"\t\t Write memory dword or Write Hw dword register \n"
		"\t\t Can write only one dword at the same time \n"
		"\t\t Ex: EW Address Value \n"
	},
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	{
		(const u8 *)"EFUSE",	8, cmd_efuse_protect,	(const u8 *)"\tEFUSE \n"
		"\t\t wmap addr len data\n"
		"\t\t rmap \n"
		"\t\t <wmap 0x00 2 8195> efuse[0]=0x81, efuse [1]=0x98\n"
		"\t\t <wmap 0xF0 4 11223344> [0xF0]=0x11, [0xF1]=0x22, [0xF2]=0x33, [0xF3]=0x44\n"
	},
#endif
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
	{
		(const u8 *)"LOG",	3, cmd_log_set, (const u8 *)"\tLOG <tag, level> \n"
		"\t\t Set the log display level of a module individually\n"
		"\t\t <tag>: module label, If the tag is *, this will reset all tag levels except those added to the array\n"
		"\t\t <level>:0, turn off log\n"
		"\t\t	   1, always (Resident)log\n"
		"\t\t	   2, error log\n"
		"\t\t	   3, warning log\n"
		"\t\t	   4, info log\n"
		"\t\t	   5, debug log\n"
	},

};

u32 cmd_rom_table(void **PTable)
{
	*PTable = (void *)&shell_cmd_table_rom;

	return (sizeof(shell_cmd_table_rom) / sizeof(COMMAND_TABLE));
}


#if defined  (__GNUC__)
#pragma GCC diagnostic pop
#endif
