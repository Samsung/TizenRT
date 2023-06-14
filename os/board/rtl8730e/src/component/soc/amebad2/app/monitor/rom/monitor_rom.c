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

static u32 cmd_rom_help(u16 argc, u8  *argv[]);

SHELL_ROM_TEXT_SECTION
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

	MONITOR_LOG("\n\rRebooting ...\n\r");
	NVIC_SystemReset();

	return _TRUE;
}

SHELL_ROM_DATA_SECTION
static COMMAND_TABLE   shell_cmd_table_rom[] = {
	{
		(const u8 *)"?",			0, cmd_rom_help,	(const u8 *)"\tHELP (?) \n"
		"\t\t Print this help messag\n"
	},
	{
		(const u8 *)"REBOOT",	4, cmd_reboot,	(const u8 *)"\tREBOOT \n"
		"\t\t <item, string> : \n"
		"\t\t item: uartburn or N/A \n"
		"\t\t \n"
	},
};

SHELL_ROM_TEXT_SECTION
u32 cmd_rom_table(void **PTable)
{
	*PTable = (void *)&shell_cmd_table_rom;

	return (sizeof(shell_cmd_table_rom) / sizeof(COMMAND_TABLE));
}

SHELL_ROM_TEXT_SECTION
static u32 cmd_rom_help(u16 argc, u8  *argv[])
{
	u32	LoopINdex ;

	MONITOR_LOG("----------------- COMMAND MODE HELP ------------------\n");
	for (LoopINdex = 0  ; LoopINdex < (sizeof(shell_cmd_table_rom) / sizeof(COMMAND_TABLE)) ; LoopINdex++) {
		if (shell_cmd_table_rom[LoopINdex].msg) {
			MONITOR_LOG("%s\n", shell_cmd_table_rom[LoopINdex].msg);
		}
	}
	MONITOR_LOG("----------------- COMMAND MODE END  ------------------\n");

	return _TRUE ;
}
#if defined  (__GNUC__)
#pragma GCC diagnostic pop
#endif
