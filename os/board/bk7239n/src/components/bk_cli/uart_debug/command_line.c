#include <common/bk_include.h>
#include <components/system.h>
#include "command_line.h"
#include "command_table.h"
#include <os/str.h>
#include "bk_uart.h"
#include <os/mem.h>
#include <driver/flash_partition.h>

#include <driver/flash.h>
#include "sys_ctrl.h"

#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include <components/system.h>

#include <components/ate.h>

#include "bk_phy.h"

#include <driver/adc.h>
#include <driver/efuse.h>
#include <driver/aud.h>
#include <driver/aud_types.h>

#if defined(CONFIG_AUDIO)
//#include "aud_driver.h"
#endif
#include "sys_driver.h"
#include "bk_rf_internal.h"

extern bk_err_t uart_write_byte(uart_id_t id, uint8_t data);
extern void manual_cal_show_txpwr_tab_simple(void);
extern UINT32 manual_cal_fitting_txpwr_tab(void);
extern uint8_t if_ble_sleep(void);
extern uint8_t is_xvr_reg_backup(uint32_t addr);
extern uint8_t is_btdm_reg_backup(uint32_t addr);

int bkreg_tx_get_uart_port(void)
{
	int port = CONFIG_UART_CLI_BKREG_PORT;

	return port;
}

#if defined(CONFIG_UART_DEBUG)
/* find command table entry for a command */
cmd_tbl_t *cmd_find_tbl(const char *cmd, cmd_tbl_t *table, int table_len)
{
	cmd_tbl_t *cmdtp;
	cmd_tbl_t *cmdtp_temp = table;	/* Init value */
	const char *p;
	int len;
	int n_found = 0;

	if (!cmd)
		return NULL;
	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	len = ((p = os_strchr(cmd, '.')) == NULL) ? os_strlen(cmd) : (p - cmd);

	for (cmdtp = table; cmdtp != table + table_len; cmdtp++) {
		if (os_strncmp(cmd, cmdtp->name, len) == 0) {
			if (len == os_strlen(cmdtp->name))
				return cmdtp;	/* full match */

			cmdtp_temp = cmdtp;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1)  			/* exactly one match */
		return cmdtp_temp;

	return NULL;	/* not found or ambiguous command */
}

cmd_tbl_t *cmd_find(const char *cmd)
{
	cmd_tbl_t *start = entry_get_start();
	const int len = entry_get_count();

	return cmd_find_tbl(cmd, start, len);
}

int cmd_usage(const cmd_tbl_t *cmdtp)
{
	CLI_DEBUG_LOGI("%s\r\n", cmdtp->usage);

#ifdef	CONFIG_SYS_LONGHELP
	if (!cmdtp->help) {
		CLI_DEBUG_LOGI("- No additional help available.\n");
		return 1;
	}

	CLI_DEBUG_LOGI(cmdtp->help);
	CLI_DEBUG_LOGI("\r\n");
#endif	/* CONFIG_SYS_LONGHELP */

	return 0;
}

/**
 * Call a command function. This should be the only route in U-Boot to call
 * a command, so that we can track whether we are waiting for input or
 * executing a command.
 *
 * @param cmdtp		Pointer to the command to execute
 * @param flag		Some flags normally 0 (see CMD_FLAG_.. above)
 * @param argc		Number of arguments (arg 0 must be the command text)
 * @param argv		Arguments
 * @return 0 if command succeeded, else non-zero (CMD_RET_...)
 */
static int cmd_call(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int result;

	result = (cmdtp->cmd)(cmdtp, flag, argc, argv);
	if (result)
		CLI_DEBUG_LOGI("Command failed, result=%d\n", result);

	return result;
}

int cmd_process(int flag, int argc, char *const argv[],
				int *repeatable, unsigned int *ticks)
{
	int rc = CMD_RET_SUCCESS;
	cmd_tbl_t *cmdtp;

	/* Look up command in command table */
	cmdtp = cmd_find(argv[0]);
	if (cmdtp == NULL) {
		CLI_DEBUG_LOGI("Unknown command '%s' - try 'help'\n", argv[0]);
		return CMD_RET_FAILURE;
	}

	if (DEBUG_PARSER)
		CLI_DEBUG_LOGI("    cmd_find\r\n");

	CLI_DEBUG_LOGI("\r\n    -------------------------------------------------------------------\r\n");

	/* found - check max args */
	if (argc > cmdtp->maxargs)
		rc = CMD_RET_USAGE;

	/* If OK so far, then do the command */
	if (!rc) {
		rc = cmd_call(cmdtp, flag, argc, argv);

		*repeatable &= cmdtp->repeatable;
	}
	if (rc == CMD_RET_USAGE)
		rc = cmd_usage(cmdtp);

	CLI_DEBUG_LOGI("\r\n    -------------------------------------------------------------------\r\n");
	return rc;
}

int cmd_parse_line(char *line, char *argv[])
{
	int nargs = 0;

	while (nargs < CONFIG_SYS_MAXARGS) {
		/* skip any white space */
		while (isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && !isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	CLI_DEBUG_LOGI("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

	return nargs;
}

void cmd_process_macros(const char *input, char *output)
{
	char c, prev;
	int inputcnt = os_strlen(input);
	int outputcnt = CONFIG_SYS_CBSIZE;
	int state = 0;		/* 0 = waiting for '$'  */

	/* 1 = waiting for '(' or '{' */
	/* 2 = waiting for ')' or '}' */
	/* 3 = waiting for '''  */

	prev = '\0';		/* previous character   */

	while (inputcnt && outputcnt) {
		c = *input++;
		inputcnt--;

		if (state != 3) {
			/* remove one level of escape characters */
			if ((c == '\\') && (prev != '\\')) {
				if (inputcnt-- == 0)
					break;
				prev = c;
				c = *input++;
			}
		}

		switch (state) {
		case 0:	/* Waiting for (unescaped) $    */
			if ((c == '\'') && (prev != '\\')) {
				state = 3;
				break;
			}
			if ((c == '$') && (prev != '\\'))
				state++;
			else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		case 1:	/* Waiting for (        */
			if (c == '(' || c == '{')
				state++;
			else {
				state = 0;
				*(output++) = '$';
				outputcnt--;

				if (outputcnt) {
					*(output++) = c;
					outputcnt--;
				}
			}
			break;
		case 2:	/* Waiting for )        */
			if (c == ')' || c == '}') {
				char *envval;
				/* Varname # of chars */

				/* Get the varname */

				/* Get its value */
				envval = 0; // getenv(envname);

				/* Copy into the line if it exists */
				if (envval != NULL)
					while ((*envval) && outputcnt) {
						*(output++) = *(envval++);
						outputcnt--;
					}
				/* Look for another '$' */
				state = 0;
			}
			break;
		case 3:	/* Waiting for '        */
			if ((c == '\'') && (prev != '\\'))
				state = 0;
			else {
				*(output++) = c;
				outputcnt--;
			}
			break;
		}
		prev = c;
	}

	if (outputcnt)
		*output = 0;
	else
		*(output - 1) = 0;
}


/*
 * Run a command using the selected parser.
 *
 * @param cmd	Command to run
 * @param flag	Execution flags (CMD_FLAG_...)
 * @return 0 on success, or != 0 on error.
 */
int run_command(const char *cmd, int flag)
{
	char cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CONFIG_SYS_CBSIZE];
	char *str = cmdbuf;
	char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

	if (DEBUG_PARSER
		&& (cmd[0])) {
		/* use CLI_DEBUG_LOGI - string may be loooong */
		CLI_DEBUG_LOGI("run_command: ");
		CLI_DEBUG_LOGI(cmd ? cmd : "NULL");
	}

	/* forget any previous Control C */
	if (!cmd || !*cmd)
		return -1;	/* empty command */

	if (os_strlen(cmd) >= CONFIG_SYS_CBSIZE) {
		CLI_DEBUG_LOGI("## Command too long!\n");
		return -1;
	}

	os_strcpy(cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

	while (*str) {
		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep == '\'') &&
				(*(sep - 1) != '\\'))
				inquotes = !inquotes;

			if (!inquotes &&
				(*sep == ';') &&	/* separator		*/
				(sep != str) &&	/* past string start	*/
				(*(sep - 1) != '\\'))	/* and NOT escaped */
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		} else {
			str = sep;	/* no more commands for next pass */
		}

		/* find macros in this token and replace them */
		cmd_process_macros(token, finaltoken);

		/* Extract arguments */
		argc = cmd_parse_line(finaltoken, argv);
		if (argc == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		if (cmd_process(flag, argc, argv, &repeatable, NULL))
			rc = -1;

		/* Did the user stop this? */
	}

	return rc ? rc : repeatable;
}

#else
void bkreg_tx(HCI_EVENT_PACKET *pHCItxBuf)
{
#ifndef KEIL_SIMULATOR
	char *tmp;
#if (!defined(CONFIG_SHELL_ASYNCLOG))
	unsigned int i, port = 1;
#endif
	unsigned int tx_len = HCI_EVENT_HEAD_LENGTH + pHCItxBuf->total;

#if (!defined(CONFIG_SHELL_ASYNCLOG))
	port = bkreg_tx_get_uart_port();
#endif

	pHCItxBuf->code  = TRA_HCIT_EVENT;
	pHCItxBuf->event = HCI_COMMAND_COMPLETE_EVENT;

	tmp = (char *)pHCItxBuf;

#if defined(CONFIG_SHELL_ASYNCLOG)
	shell_log_raw_data((uint8_t *)tmp, tx_len);
#else
	for (i = 0; i < tx_len; i ++) {
		uart_write_byte(port, tmp[i]); //BK_UART_2
	}
#endif //#if CONFIG_SHELL_ASYNCLOG
#endif //#ifndef KEIL_SIMULATOR
}

cmd_tbl_t *cmd_find_tbl(const char *cmd, cmd_tbl_t *table, int table_len)
{
	return 0;
}

int cmd_usage(const cmd_tbl_t *cmdtp)
{
	return 0;
}
int run_command(const char *cmd, int flag)
{
	return 0;
}

#if defined(CONFIG_BKREG)
#define BKREG_COMMAND_OP_CHIP_ID       0x800000
#define BKREG_COMMAND_OP_DEVICE_ID     0x800004

static int bkreg_run_command_implement(const char *content, int cnt)
{
	char tx_buf[BKREG_TX_FIFO_THRD];
	uint32_t found = 1;
	UINT32 uart_rx_index;
	REGISTER_PARAM *rx_param;
	REGISTER_PARAM *tx_param;
	HCI_EVENT_PACKET   *pHCItxBuf = (HCI_EVENT_PACKET *)tx_buf;
	HCI_COMMAND_PACKET *pHCIrxBuf = (HCI_COMMAND_PACKET *)content;

	uart_rx_index = cnt;
	pHCItxBuf->total = 1;
	pHCItxBuf->param[0] = pHCIrxBuf->cmd;
	switch (pHCIrxBuf->cmd) {
	case BEKEN_UART_REGISTER_WRITE_CMD:
		rx_param        = (REGISTER_PARAM *)pHCIrxBuf->param;
#if (defined(CONFIG_SOC_BK7271))
		if (rx_param->addr >= SCTRL_ANALOG_CTRL0
			&& rx_param->addr <= SCTRL_ANALOG_CTRL10)
			sctrl_analog_set(rx_param->addr, rx_param->value);
		else
#endif
		{
#if (defined(CONFIG_SOC_BK7256XX) || defined(CONFIG_SOC_BK7236XX) || defined(CONFIG_SOC_BK7239XX) || defined(CONFIG_SOC_BK7286XX))
			if(ate_is_enabled() &&
			  ((rx_param->addr == 0x0191004c) ||
			  (rx_param->addr == 0x910040) ||
			  (rx_param->addr == 0x91004c) ||
			  (rx_param->addr == 0x910048) ||
			  (rx_param->addr == 0x910094) ||
			  (rx_param->addr == 0x9100f4) ||
			  (rx_param->addr == 0x910090) ||
			  (rx_param->addr == 0x806e40) ||
			  (rx_param->addr == 0x801440) ||
			  (rx_param->addr == 0xf00040))){
				break;
			}
#endif
			if(((rx_param->addr &0xe0)== 0xe0)||((rx_param->addr & 0x01) == 0x01)) //ignore cmd : 01 e0 fc xx xx (used for pc dl).
			{
				break;
			}

			if(rx_param->addr)
				REG_WRITE(rx_param->addr, rx_param->value);
			else
				CLI_DEBUG_LOGI("ERR add=0x%x,value=%d\n", rx_param->addr, rx_param->value);

		}

		pHCItxBuf->total = uart_rx_index - 1;
		os_memcpy(pHCItxBuf->param, pHCIrxBuf, HCI_EVENT_HEAD_LENGTH);
		pHCItxBuf->param[3] = pHCIrxBuf->cmd;

		tx_param = (REGISTER_PARAM *)&pHCItxBuf->param[HCI_COMMAND_HEAD_LENGTH];
		tx_param->addr      = rx_param->addr;
		tx_param->value     = rx_param->value;

#if defined(CONFIG_CALI)
		{
            extern void rwnx_cal_save_reg_val_with_addr(UINT32 addr, UINT32 val);
            // when write trx and rc beken regs, updata registers save.
            if( (rx_param->addr & REG_RC_BASE_ADDR_MSK) == REG_RC_BASE_ADDR)
                rwnx_cal_save_reg_val_with_addr(rx_param->addr, rx_param->value);
        }
#endif

#if CONFIG_BLE_LV_SUPPORT
		{
			// when write xvr and btdm beken regs, updata registers save.
			extern void write_xvr_reg_backup(uint32_t addr, uint32_t val);
			if(is_xvr_reg_backup(rx_param->addr))
			{
				write_xvr_reg_backup(rx_param->addr, rx_param->value);
			}

			extern void write_btdm_reg_backup(uint32_t addr, uint32_t val);
			if(is_btdm_reg_backup(rx_param->addr))
			{
				write_btdm_reg_backup(rx_param->addr, rx_param->value);
			}
		}
#endif
		break;

	case BEKEN_UART_REGISTER_READ_CMD:
		rx_param        = (REGISTER_PARAM *)pHCIrxBuf->param;
		pHCItxBuf->total         = HCI_EVENT_HEAD_LENGTH + uart_rx_index; /*may err, porting from bk3260*/
		os_memcpy(pHCItxBuf->param, pHCIrxBuf, HCI_EVENT_HEAD_LENGTH);
		pHCItxBuf->param[3]      = pHCIrxBuf->cmd;

		tx_param = (REGISTER_PARAM *)&pHCItxBuf->param[HCI_COMMAND_HEAD_LENGTH];
		tx_param->addr  = rx_param->addr;

#if (defined(CONFIG_SOC_BK7271))
		if (rx_param->addr >= SCTRL_ANALOG_CTRL0
			&& rx_param->addr <= SCTRL_ANALOG_CTRL10)
			tx_param->value = sctrl_analog_get(rx_param->addr);
		else
#endif
			if (rx_param->addr == SCTRL_REG_RESV5) {
#if defined(CONFIG_ATE)
				if (ate_is_enabled())
					tx_param->value = 0x1B190104;   // testmode flag[31:28] | bk7231U:B [27:24] | date
				else
					tx_param->value = 0x0B190104;
#endif
			} else {
				if (ate_is_enabled() && (tx_param->addr == BKREG_COMMAND_OP_CHIP_ID)) {
					/* 0x800000 as a universal CHIP ID instruction */
					tx_param->value = sys_drv_get_chip_id();
				} else if (ate_is_enabled() && (tx_param->addr == BKREG_COMMAND_OP_DEVICE_ID)) {
					/* 0x800004 as a universal DEVICE ID instruction */
					tx_param->value = sys_drv_get_device_id();
				}
#if (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
				else if(ate_is_enabled() &&
						 ((tx_param->addr == 0x01910040) ||
						  (tx_param->addr == 0x910040) ||
						  (tx_param->addr == 0x91004c) ||
						  (tx_param->addr == 0x910048) ||
						  (tx_param->addr == 0x910094) ||
						  (tx_param->addr == 0x9100f4) ||
						  (tx_param->addr == 0x910090) ||
						  (tx_param->addr == 0x806e40) ||
						  (tx_param->addr == 0x801440) ||
						  (tx_param->addr == 0xf00040))){
						break;
				}
#endif
#if (defined(CONFIG_CALI)) && (defined(CONFIG_SOC_BK7236XX) || defined(CONFIG_SOC_BK7239XX) || defined(CONFIG_SOC_BK7286XX))
				else if (!ate_is_enabled() && ((rx_param->addr & REG_RC_BASE_ADDR_MSK) == REG_RC_BASE_ADDR))
				{
					extern UINT32 rwnx_cal_load_reg_val_with_addr(UINT32 addr);
					// when read trx and rc beken regs, read ram since RF sleep.
					tx_param->value = rwnx_cal_load_reg_val_with_addr(rx_param->addr);
				}
#endif
#if CONFIG_BLE_LV_SUPPORT
				// when read xvr and btdm beken regs, read ram since BLE sleep.
				else if (if_ble_sleep() && is_xvr_reg_backup(rx_param->addr))
				{
					extern uint32_t read_xvr_reg_backup(uint32_t addr);
					tx_param->value = read_xvr_reg_backup(rx_param->addr);
				}
				else if (if_ble_sleep() && is_btdm_reg_backup(rx_param->addr))
				{
					extern uint32_t read_btdm_reg_backup(uint32_t addr);
					tx_param->value = read_btdm_reg_backup(rx_param->addr);
				}
#endif
				else
					tx_param->value = REG_READ(rx_param->addr);
			}
		break;

	case BEKEN_WRITE_OTP_CMD: {
		UINT8 *flag_ptr = NULL;
		//UINT8 *write_buf = NULL;
		UINT8 flag = 0;
		rx_param        = (REGISTER_PARAM *)pHCIrxBuf->param;
		//write_buf       = (UINT8 *)(rx_param + 1);
//TODO: remove code for bringup
//		flag = manual_cal_wirte_otp_flash(rx_param->addr, rx_param->value, write_buf);

		pHCItxBuf->total = OTP_CMD_RET_LEN + 1;
		os_memcpy(pHCItxBuf->param, pHCIrxBuf, HCI_EVENT_HEAD_LENGTH);
		pHCItxBuf->param[3] = pHCIrxBuf->cmd;

		tx_param = (REGISTER_PARAM *)&pHCItxBuf->param[HCI_COMMAND_HEAD_LENGTH];
		tx_param->addr      = rx_param->addr;
		tx_param->value     = rx_param->value;

		flag_ptr = (UINT8 *)&pHCItxBuf->param[OTP_CMD_RET_LEN];
		*flag_ptr = flag;
	}
	break;

	case BEKEN_READ_OTP_CMD: {
		UINT32 len, len_left = 0, addr;
		//UINT8 *read_buf = (UINT8 *)&pHCItxBuf->param[OTP_CMD_RET_LEN];

		rx_param = (REGISTER_PARAM *)pHCIrxBuf->param;
#if CONFIG_RF_FIRMWARE_DYNAMIC_PARTITION
		addr = rx_param->addr - (bk_flash_get_capacity_bytes() - FLASH_RF_FIRMWARE_OFFSET);
#else
		bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_RF_FIRMWARE);

		len_left = rx_param->value;
		addr = rx_param->addr - pt->partition_start_addr;//0xFA000;
#endif
		len_left = rx_param->value;
		while (len_left) {
			len = (len_left > OTP_READ_MAX_LEN) ? OTP_READ_MAX_LEN : len_left;
			//TODO: For BringUp remove code here
			//len = manual_cal_read_otp_flash(addr, len, read_buf);

			if (len == 0)
				return 0;

			pHCItxBuf->total = OTP_CMD_RET_LEN + len;
			os_memcpy(pHCItxBuf->param, pHCIrxBuf, HCI_EVENT_HEAD_LENGTH);
			pHCItxBuf->param[3] = pHCIrxBuf->cmd;

			tx_param = (REGISTER_PARAM *)&pHCItxBuf->param[HCI_COMMAND_HEAD_LENGTH];
			tx_param->addr      = rx_param->addr + addr;
			tx_param->value     = len;
			bkreg_tx(pHCItxBuf);
			len_left -= len;
			addr += len;
		}
		return 0;
	}
	break;

	case BEKEN_TEMP_CMD: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_dac_pause();
		//bk_audio_intf_adc_pause();
#endif
	}
	break;

	case BEKEN_TEMP_TCP: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_dac_play();
		//bk_audio_intf_adc_play();
#endif
	}
	break;

	case BEKEN_TEST_UDP: {
#if defined(CONFIG_BKREG_SHOW_CALI)
		manual_cal_show_txpwr_tab_simple();
#endif
	}
	break;

	case BEKEN_SD_CLOSE: {
#if defined(CONFIG_MANUAL_CALI)
		manual_cal_fitting_txpwr_tab();
#endif
	}
	break;

	case LOOP_MODE_CMD: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_uninit();
#endif
	}
	break;

	case BEKEN_DUMP_ENV_CMD: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_init();
#endif
	}
	break;

#if defined(CONFIG_TEMP_DETECT)
	case BEKEN_TEMP_DETECT_CONFIG_CMD: {	// 01 E0 FC 04 ec 02 01 05
		extern void temp_detect_change_configuration(UINT32 intval, UINT32 thre, UINT32 dist);

		int intval, thre, dist;

		intval = (int)pHCIrxBuf->param[0];
		thre   = (int)pHCIrxBuf->param[1];
		dist   = (int)pHCIrxBuf->param[2];

		temp_detect_change_configuration(intval, thre, dist);
	}
	break;
#endif

	case BEKEN_SHOW_BT_STATUS: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_dac_set_volume();
#endif
	}
	break;

	case BEKEN_SHOW_BT_DEBUG: {
#if defined(CONFIG_AUDIO)
		//bk_audio_intf_dac_set_sample_rate();
#endif
	}
	break;

	case BEKEN_DO_REBOOT: {
		if ((pHCIrxBuf->param[0] == 0x95)
			&& (pHCIrxBuf->param[1] == 0x27)
			&& (pHCIrxBuf->param[2] == 0x95)
			&& (pHCIrxBuf->param[3] == 0x27))
			bk_reboot();
	}
	break;

	case BEKEN_UART_LINK_CHECK: {
		pHCItxBuf->total = 1;
		pHCItxBuf->param[0] = pHCIrxBuf->cmd;
		found = 1;
	}
	break;

	default:
		pHCItxBuf->total = 1;
		pHCItxBuf->param[0] = pHCIrxBuf->cmd;
		found = 0;
		break;
	}
	if(found)
	{
	    bkreg_tx(pHCItxBuf);
	}

	return found;
}

int bkreg_run_command(const char *content, int cnt)
{
    int ret = 0;
	//ps_switch(PS_UNALLOW, PS_EVENT_BKREG, PM_RF_BIT);
	//rf_module_vote_ctrl(RF_OPEN,RF_BY_BKREG_BIT);

	ret = bkreg_run_command_implement(content, cnt);

	//ps_switch(PS_ALLOW, PS_EVENT_BKREG, PM_RF_BIT);
	//rf_module_vote_ctrl(RF_CLOSE,RF_BY_BKREG_BIT);

	return ret;
}
#endif // CONFIG_BKREG
#endif // CONFIG_UART_DEBUG

// eof
