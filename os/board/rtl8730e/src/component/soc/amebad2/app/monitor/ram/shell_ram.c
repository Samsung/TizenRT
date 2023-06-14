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
#include "rtw_wifi_constants.h"
#include <stdarg.h>
//#include "strproc.h"

#if defined ( __ICCARM__ )
#pragma section=".cmd.table.data"

SECTION(".data") u8 *__cmd_table_start__ = 0;
SECTION(".data") u8 *__cmd_table_end__ = 0;
#endif

#define OpenShellRx		2

extern volatile UART_LOG_CTL		shell_ctl;
extern UART_LOG_BUF				shell_buf;

extern COMMAND_TABLE    shell_cmd_table[];

xSemaphoreHandle	shell_sema = NULL;

#ifdef SUPPORT_LOG_SERVICE
extern char log_buf[LOG_SERVICE_BUFLEN];
extern _sema log_rx_interrupt_sema;
extern void log_service_init(void);
#endif

static monitor_cmd_handler shell_get_cmd(char *argv)
{
	PCOMMAND_TABLE  pCmdTbl = shell_ctl.pCmdTbl;
	u32 CmdCnt = 0;
	u32 CmdNum = shell_ctl.CmdTblSz;

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
static void shell_cmd_exec_up_sema(u8  argc, u8  **argv)
{
	/* To avoid gcc warnings */
	(void) argc;
	(void) argv;

#ifdef SUPPORT_LOG_SERVICE
	rtw_up_sema((_sema *)&log_rx_interrupt_sema);
#endif
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
			return TRUE;
		}
	}

	return FALSE;
}

VOID shell_loguratRx_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	(void) Data;
	(void) IrqStatus;
	(void) ChanNum;

#if defined (ARM_CORE_CM4)
#ifndef CONFIG_LINUX_FW_EN
	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_LP_TO_NP, IPC_L2N_LOGUART_RX_SWITCH);
#else
	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_AP_TO_NP, IPC_A2N_LOGUART_RX_SWITCH);
#endif // CONFIG_LINUX_FW_EN
#else
	PIPC_MSG_STRUCT ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_LP_TO_AP, IPC_L2A_LOGUART_RX_SWITCH);
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

#ifndef MP_CMD_BUFLEN
#define MP_CMD_BUFLEN 64
#endif
#ifndef STA_WLAN_INDEX
#define STA_WLAN_INDEX		0
#endif
char mp_buf[MP_CMD_BUFLEN];
char log_buf_temp[UART_LOG_CMD_BUFLEN];

int mp_commnad_handler_shell(char *cmd)
{
#if defined (ARM_CORE_CM4)
	char *token = NULL;

	token = strtok(cmd, " ");

	if (token && (strcmp(cmd, "iwpriv") == 0)) {
		token = strtok(NULL, "");
#ifdef CONFIG_MP_INCLUDED
		wext_private_command(STA_WLAN_INDEX, token, 1, NULL);
#endif
		return TRUE;
	}
	return FALSE;
#else
	/* To avoid gcc warnings */
	(void) cmd;
	return FALSE;
#endif
}

#ifdef ARM_CORE_CM0
UART_LOG_BUF				tmp_log_buf;

VOID shell_loguartRx_dispatch(VOID)
{
	u32 i, CpuId = 0;
	IPC_MSG_STRUCT ipc_msg_temp;
	u32 ipc_dir = 0;
	u32 ipc_ch = 0;

#ifdef CONFIG_AS_INIC_KM4_NP_CA32_AP
	for (i = 0; i < UART_LOG_CMD_BUFLEN; i++) {
		if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] != ' ') {
			if ((shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '~')) {
				CpuId = NP_CPU_ID;	/* CMD should processed by KM4 */
				ipc_dir = IPC_LP_TO_NP;
				ipc_ch = IPC_L2N_LOGUART_RX_SWITCH;
			} else if ((shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '@') || (shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '\0')) {
				CpuId = LP_CPU_ID;	/* CMD should processed by KM0 itself */
			} else {
				CpuId = AP_CPU_ID;
				ipc_dir = IPC_LP_TO_AP;
				ipc_ch = IPC_L2A_LOGUART_RX_SWITCH;
			}
			break;
		}
	}
#else
	for (i = 0; i < UART_LOG_CMD_BUFLEN; i++) {
		if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] != ' ') {
			if ((shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '~')) {
				CpuId = AP_CPU_ID;	/* CMD should processed by CA32 */
				ipc_dir = IPC_LP_TO_AP;
				ipc_ch = IPC_L2A_LOGUART_RX_SWITCH;
			} else if ((shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '@') || (shell_ctl.pTmpLogBuf->UARTLogBuf[i] == '\0')) {
				CpuId = LP_CPU_ID;	/* CMD should processed by KM0 itself */
			} else {
				CpuId = NP_CPU_ID;
				ipc_dir = IPC_LP_TO_NP;
				ipc_ch = IPC_L2N_LOGUART_RX_SWITCH;
			}
			break;
		}
	}
#endif
	if ((CpuId == NP_CPU_ID) || (CpuId == AP_CPU_ID)) {		/* CMD should processed by KM4, inform KM4 thru IPC */
		memcpy((u32 *)&tmp_log_buf, (u32 *)shell_ctl.pTmpLogBuf, sizeof(UART_LOG_BUF));
		DCache_CleanInvalidate((u32)&tmp_log_buf, sizeof(UART_LOG_BUF));

		ipc_msg_temp.msg_type = IPC_USER_POINT;
		ipc_msg_temp.msg = (u32)&tmp_log_buf;
		ipc_msg_temp.msg_len = 1;
		ipc_msg_temp.rsvd = 0;		/* for coverity init issue */
		ipc_send_message(ipc_dir, ipc_ch, &ipc_msg_temp);

		shell_ctl.pTmpLogBuf->BufCount = 0;
		shell_array_init((u8 *)(&shell_ctl.pTmpLogBuf->UARTLogBuf[0]), UART_LOG_CMD_BUFLEN, '\0');
		shell_ctl.ExecuteCmd = _FALSE;
	}
}
#else
#define shell_loguartRx_dispatch()
#endif

static VOID shell_task_ram(VOID *Data)
{
	/* To avoid gcc warnings */
	(void) Data;

	u32 ret = TRUE;
	int i = 0;
	u8  argc = 0;
	u8  **argv;
	PUART_LOG_BUF pUartLogBuf = shell_ctl.pTmpLogBuf;

	//4 Set this for UartLog check cmd history
	shell_ctl.shell_task_rdy = 1;
	shell_ctl.BootRdy = 1;

	portALLOCATE_SECURE_CONTEXT(configMINIMAL_SECURE_STACK_SIZE);

	do {
		xSemaphoreTake(shell_sema, RTW_MAX_DELAY);

		shell_loguartRx_dispatch();

		if (shell_ctl.ExecuteCmd) {
			/* avoid useless space */
			for (i = 0; i < UART_LOG_CMD_BUFLEN; i++) {
				if (shell_ctl.pTmpLogBuf->UARTLogBuf[i] != ' ') {
					break;
				}
			}

#ifdef CONFIG_AS_INIC_KM4_NP_CA32_AP
			if (SYS_CPUID() != AP_CPU_ID) {
				i = i + 1;	/* remove flag like @ other than CA32 */
			}
#else
			if (SYS_CPUID() != NP_CPU_ID) {
				i = i + 1;	/* remove flag like @ other than KM4 */
			}
#endif

#ifdef SUPPORT_LOG_SERVICE
			strncpy(log_buf, (const char *)&pUartLogBuf->UARTLogBuf[i], LOG_SERVICE_BUFLEN - 1);
#endif // SUPPORT_LOG_SERVICE

			memset(mp_buf, 0, MP_CMD_BUFLEN);
			memset(log_buf_temp, 0, UART_LOG_CMD_BUFLEN);
#if defined(ARM_CORE_CA32) && defined(ARM_CORE_CM4)
			strncpy(mp_buf, (const char *)&pUartLogBuf->UARTLogBuf[i], MP_CMD_BUFLEN - 1);
#endif
			memcpy(log_buf_temp, (const char *)&pUartLogBuf->UARTLogBuf[0], UART_LOG_CMD_BUFLEN - 1);

			argc = shell_get_argc((const u8 *)&pUartLogBuf->UARTLogBuf[i]);
			argv = shell_get_argv((const u8 *)&pUartLogBuf->UARTLogBuf[i]); /* UARTLogBuf will be changed */

			if (argc > 0) {
				/* step 1. for monitor cmd */
				ret = shell_cmd_exec_ram(argc, argv);

				/* step 2. for mp cmd */
				if (ret == FALSE) {
					ret = mp_commnad_handler_shell(mp_buf);

					if (ret == TRUE) {
						DiagPrintf("\n\r[MEM] After do cmd, available heap %d\n\r", xPortGetFreeHeapSize());
						DiagPrintf("\r\n\n#\r\n"); //"#" is needed for mp tool
					}
				}

				if (ret == FALSE) {
#ifndef SUPPORT_LOG_SERVICE
					DiagPrintf("\r\nunknown command '%s'", log_buf_temp);
					DiagPrintf("\n\r[MEM] After do cmd, available heap %d\n\r", xPortGetFreeHeapSize());
					DiagPrintf("\r\n\n#\r\n"); //"#" is needed for mp tool
#endif
				}

				/* step 3. normal for LOG service */
				if (ret == FALSE) {
					/* To avoid gcc warnings */
					shell_cmd_exec_up_sema(argc, argv);
				}

				pUartLogBuf->BufCount = 0;
				shell_array_init(&pUartLogBuf->UARTLogBuf[0], UART_LOG_CMD_BUFLEN, '\0');
			} else {
				/*In some exception case, even if argc parsed is 0(when the first character value in log buffer is '\0'),
				log buffer may not be empty and log buffer counter may not be zero. If not clean log buffer and counter
				, some error will happen. Therefore, clean log buffer and initialize buffer counter when it occurs.*/
				if (pUartLogBuf->BufCount != 0) {
					pUartLogBuf->BufCount = 0;
					shell_array_init(&pUartLogBuf->UARTLogBuf[0], UART_LOG_CMD_BUFLEN, '\0');
				}
				CONSOLE_AMEBA();
			}
			shell_ctl.ExecuteCmd = _FALSE;

			//pmu_set_sysactive_time(10000);
		}
	} while (1);
}


VOID shell_init_ram(VOID)
{
#ifdef SUPPORT_LOG_SERVICE
	log_service_init();
#endif

#ifdef AMEBAD_TODO
	LOGUART_SetBaud_FromFlash();
#endif

#if defined ( __ICCARM__ )
	__cmd_table_start__ = (u8 *)__section_begin(".cmd.table.data");
	__cmd_table_end__ = (u8 *)__section_end(".cmd.table.data");
#endif

	shell_ctl.pCmdTbl = (PCOMMAND_TABLE)__cmd_table_start__;
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
#ifndef CONFIG_LINUX_FW_EN
	{IPC_USER_DATA,	shell_loguratRx_ipc_int,	(VOID *) NULL,	IPC_LP_TO_NP,	IPC_L2N_LOGUART_RX_SWITCH,	IPC_RX_FULL},
#else
	{IPC_USER_DATA,	shell_loguratRx_ipc_int,	(VOID *) NULL,	IPC_AP_TO_NP,	IPC_A2N_LOGUART_RX_SWITCH,	IPC_RX_FULL},
#endif // CONFIG_LINUX_FW_EN
	{IPC_USER_DATA, 	shell_loguratRx_ipc_int,	(VOID *) NULL,	IPC_LP_TO_AP,	IPC_L2A_LOGUART_RX_SWITCH,	IPC_RX_FULL},
};


#ifdef ARM_CORE_CM0

/* for uart bridge to close shell loguart rx */
void shell_uartbridge_ipc_int(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg = (PIPC_MSG_STRUCT)ipc_get_message(IPC_NP_TO_LP, IPC_N2L_UARTBRIDGE);
	if (ipc_msg->msg == OpenShellRx) {
		InterruptRegister((IRQ_FUN) shell_uart_irq_rom, UART_LOG_IRQ, (u32)NULL, 3);
		InterruptEn(UART_LOG_IRQ, 3);
	} else {
		InterruptDis(UART_LOG_IRQ);
	}
}

void shell_uartbridge_ipc_int_A2L(VOID *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg = (PIPC_MSG_STRUCT)ipc_get_message(IPC_AP_TO_LP, IPC_A2L_UARTBRIDGE);
	if (ipc_msg->msg == OpenShellRx) {
		InterruptRegister((IRQ_FUN) shell_uart_irq_rom, UART_LOG_IRQ, (u32)NULL, 3);
		InterruptEn(UART_LOG_IRQ, 3);
	} else {
		InterruptDis(UART_LOG_IRQ);
	}
}

IPC_TABLE_DATA_SECTION
const IPC_INIT_TABLE   ipc_uartbridge_table[] = {
	{IPC_USER_DATA, 	shell_uartbridge_ipc_int,	(VOID *) NULL,	IPC_NP_TO_LP,	IPC_N2L_UARTBRIDGE,	IPC_RX_FULL},
	{IPC_USER_DATA, 	shell_uartbridge_ipc_int_A2L,	(VOID *) NULL,	IPC_AP_TO_LP,	IPC_A2L_UARTBRIDGE,	IPC_RX_FULL},
};
#endif
