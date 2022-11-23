/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "osdep_service.h"
#include <stdarg.h>
//#include "strproc.h"

#if defined ( __ICCARM__ )
#pragma section=".cmd.table.data"

SECTION(".data") u8 *__cmd_table_start__ = 0;
SECTION(".data") u8 *__cmd_table_end__ = 0;
#endif

extern volatile UART_LOG_CTL		shell_ctl;
extern UART_LOG_BUF				shell_buf;

extern COMMAND_TABLE    shell_cmd_table[];

#ifdef ARM_CORE_CM4

UART_LOG_BUF				tmp_np_log_buf;
UART_LOG_BUF				tmp_ap_log_buf;
#endif

xSemaphoreHandle	shell_sema = NULL;

#if defined(CONFIG_WIFI_NORMAL)
#if SUPPORT_LOG_SERVICE
extern char log_buf[LOG_SERVICE_BUFLEN];
extern _sema log_rx_interrupt_sema;
extern void log_service_init(void);
#endif
#endif

static monitor_cmd_handler shell_get_cmd(char *argv)
{
	PCOMMAND_TABLE  pCmdTbl = shell_ctl.pCmdTbl;
	u32 CmdCnt = 0;
	u32 CmdNum = shell_ctl.CmdTblSz;
#ifndef ARM_CORE_CM4
	/*remove CA7 flag ~*/
	argv += 1;
#endif

	monitor_cmd_handler cmd_handler = NULL;

	for (CmdCnt = 0; CmdCnt < CmdNum; CmdCnt++) {
		if ((_stricmp(argv, (const char *)pCmdTbl[CmdCnt].cmd)) == 0) {
			cmd_handler = pCmdTbl[CmdCnt].func;
			break;
		}
	}

	return cmd_handler;
}

static void shell_give_sema(void)
{
	if (shell_ctl.shell_task_rdy) {
		portBASE_TYPE taskWoken = pdFALSE;
		xSemaphoreGiveFromISR(shell_sema, &taskWoken);
		portEND_SWITCHING_ISR(taskWoken);
	}
}
//======================================================
u32 shell_cmd_exec(u8  argc, u8  **argv)
{
	/* To avoid gcc warnings */
	(void) argc;
	(void) *argv;

#if defined(CONFIG_WIFI_NORMAL)
	if (argc == 0) {
		return FALSE;
	}

#if SUPPORT_LOG_SERVICE
	rtw_up_sema((_sema *)&log_rx_interrupt_sema);
#endif
	shell_array_init(argv[0], sizeof(argv[0]), 0);

#endif
	return TRUE;
}

//======================================================
static u32 shell_cmd_exec_ram(u8  argc, u8  **argv)
{
	monitor_cmd_handler cmd_handler = NULL;

	if (argc > 0) {
		cmd_handler = shell_get_cmd((char *)argv[0]);

		if (cmd_handler != NULL) {
			_strupr(argv[0]);
			cmd_handler((argc - 1), (argv + 1));
			/* target buffer will be completely cleared later in the function shell_task_ram */
			//shell_array_init(argv[0], sizeof(argv[0]),0);
			return TRUE;
		}
	}

	//(*pUartLogBuf).BufCount = 0;
	//shell_array_init(&(*pUartLogBuf).UARTLogBuf[0], UART_LOG_CMD_BUFLEN, '\0');

	return FALSE;
}

VOID shell_loguratRx_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

#if defined (RSICV_CORE_KR4)
	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_KM4_TO_KR4, IPC_M2R_LOGUART_RX_SWITCH);
#else
	PIPC_MSG_STRUCT ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_KM4_TO_DSP, IPC_M2D_LOGUART_RX_SWITCH);
#endif

	u32 addr = ipc_msg_temp->msg;
	DCache_Invalidate(addr, sizeof(UART_LOG_BUF));
	_memcpy(shell_ctl.pTmpLogBuf, (u32 *)addr, sizeof(UART_LOG_BUF));

	PUART_LOG_BUF addr_temp = (PUART_LOG_BUF)addr;
	addr_temp->BufCount = 0;
	shell_array_init((u8 *)addr, sizeof(UART_LOG_BUF), '\0');
	DCache_CleanInvalidate(addr, sizeof(UART_LOG_BUF));

	shell_ctl.ExecuteCmd = _TRUE;
	if (shell_ctl.shell_task_rdy) {
		shell_ctl.GiveSema();
	}
}

static VOID shell_task_ram(VOID *Data)
{
	/* To avoid gcc warnings */
	(void) Data;

	u32 ret = TRUE;
	int i = 0;

	//4 Set this for UartLog check cmd history
	shell_ctl.shell_task_rdy = 1;
	shell_ctl.BootRdy = 1;

	portALLOCATE_SECURE_CONTEXT(configMINIMAL_SECURE_STACK_SIZE);

	do {
		xSemaphoreTake(shell_sema, RTW_MAX_DELAY);

#if defined (ARM_CORE_CM4)
		int flag_KM0 = 0;

		for (i = 0; i < UART_LOG_CMD_BUFLEN; i++) {
			if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] != ' ') {
				if ((shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '~')) {
					flag_KM0 = 2;	/* CMD should processed by KM0 itself */
					i = i + 1;
					break;
				} else if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '@') {
					flag_KM0 = 0;	/* CMD should processed by KM0 itself */
					i = i + 1;
					break;
				} else {
					flag_KM0 = 1;
					break;
				}
			}
		}
		if (flag_KM0 == 0) {		/* CMD should processed by KR4, inform KR4 thru IPC */
			IPC_MSG_STRUCT ipc_msg_temp;
			ipc_msg_temp.msg_type = IPC_USER_POINT;

			memcpy((u32 *)&tmp_np_log_buf, (u32 *)shell_ctl.pTmpLogBuf, sizeof(UART_LOG_BUF));

			DCache_CleanInvalidate((u32)&tmp_np_log_buf, sizeof(UART_LOG_BUF));

			shell_ctl.pTmpLogBuf->BufCount = 0;
			shell_array_init((u8 *)(&shell_ctl.pTmpLogBuf[0]), UART_LOG_CMD_BUFLEN, '\0');

			ipc_msg_temp.msg = (u32)&tmp_np_log_buf;

			ipc_msg_temp.msg_len = 1;
			ipc_msg_temp.rsvd = 0;
			ipc_send_message(IPC_KM4_TO_KR4, IPC_M2R_LOGUART_RX_SWITCH, &ipc_msg_temp);
			shell_ctl.ExecuteCmd = _FALSE;
		} else if (flag_KM0 == 2) {		/* CMD should processed by CA7, inform CA7 thru IPC */
			IPC_MSG_STRUCT ipc_msg_temp;
			ipc_msg_temp.msg_type = IPC_USER_POINT;
			memcpy((u32 *)&tmp_ap_log_buf, (u32 *)shell_ctl.pTmpLogBuf, sizeof(UART_LOG_BUF));

			DCache_CleanInvalidate((u32)&tmp_ap_log_buf, sizeof(UART_LOG_BUF));

			shell_ctl.pTmpLogBuf->BufCount = 0;
			shell_array_init((u8 *)(&shell_ctl.pTmpLogBuf[0]), UART_LOG_CMD_BUFLEN, '\0');

			ipc_msg_temp.msg = (u32)&tmp_ap_log_buf;

			ipc_msg_temp.msg_len = 1;
			ipc_msg_temp.rsvd = 0;
			ipc_send_message(IPC_KM4_TO_DSP, IPC_M2D_LOGUART_RX_SWITCH, &ipc_msg_temp);
			shell_ctl.ExecuteCmd = _FALSE;
		} else {
#endif
			if (shell_ctl.ExecuteCmd) {
				u8  argc = 0;
				u8  **argv;
				PUART_LOG_BUF   pUartLogBuf = shell_ctl.pTmpLogBuf;

#if defined(CONFIG_WIFI_NORMAL)
#if SUPPORT_LOG_SERVICE
#if defined(RSICV_CORE_KR4)
				strncpy(log_buf, (const char *)(&(*pUartLogBuf).UARTLogBuf[i] + 1), LOG_SERVICE_BUFLEN - 1);
#else
				strncpy(log_buf, (const char *) & (*pUartLogBuf).UARTLogBuf[i], LOG_SERVICE_BUFLEN - 1);
#endif
#endif
#endif
#if defined (RSICV_CORE_KR4)
				/* avoid useless space */
				for (i = 0; i < UART_LOG_CMD_BUFLEN; i++) {
					if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] != ' ') {
						break;
					}
				}

#endif
				argc = shell_get_argc((const u8 *) & ((*pUartLogBuf).UARTLogBuf[i]));
				argv = (u8 **)shell_get_argv((const u8 *) & ((*pUartLogBuf).UARTLogBuf[i])); /* UARTLogBuf will be changed */

				if (argc > 0) {
					/* FPGA Verification */
					ret = shell_cmd_exec_ram(argc, argv);

					/* normal for LOG service */
					if (ret == FALSE) {
						ret = shell_cmd_exec(argc, argv);
					}

					(*pUartLogBuf).BufCount = 0;
					shell_array_init(&(*pUartLogBuf).UARTLogBuf[0], UART_LOG_CMD_BUFLEN, '\0');
				} else {
					/*In some exception case, even if argc parsed is 0(when the first character value in log buffer is '\0'),
					log buffer may not be empty and log buffer counter may not be zero. If not clean log buffer and counter
					, some error will happen. Therefore, clean log buffer and initialize buffer counter when it occurs.*/
					if ((*pUartLogBuf).BufCount != 0) {
						(*pUartLogBuf).BufCount = 0;
						shell_array_init(&(*pUartLogBuf).UARTLogBuf[0], UART_LOG_CMD_BUFLEN, '\0');
					}
					CONSOLE_AMEBA();
				}
				shell_ctl.ExecuteCmd = _FALSE;

				//pmu_set_sysactive_time(10000);
			}
#if defined (ARM_CORE_CM4)
		}
#endif
	} while (1);
}

VOID shell_init_ram(VOID)
{
#if defined(CONFIG_WIFI_NORMAL)
#if SUPPORT_LOG_SERVICE
	log_service_init();
#endif
#endif

#ifdef AMEBAD_TODO
	LOGUART_SetBaud_FromFlash();
#endif

#if defined ( __ICCARM__ )
	__cmd_table_start__ = (u8 *)__section_begin(".cmd.table.data");
	__cmd_table_end__ = (u8 *)__section_end(".cmd.table.data");
#endif

	shell_ctl.pCmdTbl = (PCOMMAND_TABLE)(void *)__cmd_table_start__;
	shell_ctl.CmdTblSz = ((__cmd_table_end__ - __cmd_table_start__) / sizeof(COMMAND_TABLE));

	shell_ctl.ExecuteCmd = _FALSE;
	shell_ctl.ExecuteEsc = _TRUE; //don't check Esc anymore
	shell_ctl.GiveSema = shell_give_sema;


	/* Create a Semaphone */
	vSemaphoreCreateBinary(shell_sema);
	xSemaphoreTake(shell_sema, 1 / portTICK_RATE_MS);

	if (pdTRUE != xTaskCreate(shell_task_ram, "LOGUART_TASK", 1024 * 2,
							  NULL, tskIDLE_PRIORITY + 5, NULL)) {
		DiagPrintf("Create Log UART Task Err!!\n");
	}

	//CONSOLE_AMEBA();
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_shell_table[] = {
	{IPC_USER_DATA,		shell_loguratRx_ipc_int,	(VOID *) NULL,	IPC_KM4_TO_KR4,	IPC_M2R_LOGUART_RX_SWITCH,	IPC_RX_FULL},
	{IPC_USER_DATA, 	shell_loguratRx_ipc_int,	(VOID *) NULL,	IPC_KM4_TO_DSP,	IPC_M2D_LOGUART_RX_SWITCH,	IPC_RX_FULL},
};