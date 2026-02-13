// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


/************************************************************************************
 * Included Files
 ************************************************************************************/
#include <common/bk_include.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <tinyara/config.h>
#include "bk_cli.h"
#include "cli.h"
#include "stdarg.h"
#include "cli_config.h"
#include "uart_driver.h"
#include "uart_hal.h"
#include <driver/uart.h>
#include "bk_uart.h"


#define TAG "cli"


static struct cli_st *pCli = NULL;
static struct cli_st s_Cli;
beken_semaphore_t log_rx_interrupt_sema = NULL;
static uint16_t s_running_command_index = MAX_COMMANDS;
#if CFG_CLI_DEBUG
static uint8_t s_running_status = 0;
#endif

extern int cli_putstr(const char *msg);
extern int cli_hexstr2bin(const char *hex, u8 *buf, size_t len);

void help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if defined(CONFIG_BKREG)
#define BKREG_MAGIC_WORD0                 (0x01)
#define BKREG_MAGIC_WORD1                 (0xE0)
#define BKREG_MAGIC_WORD2                 (0xFC)
#define BKREG_MIN_LEN                     3
#endif
/* Find the command 'name' in the cli commands table.
* If len is 0 then full match will be performed else upto len bytes.
* Returns: a pointer to the corresponding cli_command struct or NULL.
*/
const struct cli_command *lookup_command(char *name, int len)
{
	int i = 0;
	int n = 0;

	while (i < MAX_COMMANDS && n < pCli->num_commands) {
		if (pCli->commands[i]->name == NULL) {
			i++;
			continue;
		}

		/* See if partial or full match is expected */
		if (len != 0) {
			if (!os_strncmp(pCli->commands[i]->name, name, len)) {
				s_running_command_index = i;
				return pCli->commands[i];
			}
		} else {
			if (!os_strcmp(pCli->commands[i]->name, name)) {
				s_running_command_index = i;
				return pCli->commands[i];
			}
		}

		i++;
		n++;
	}

	return NULL;
}

int lookup_cmd_table(const struct cli_command *cmd_table, int table_items, char *name, int len)
{
	int i;

	for (i = 0; i < table_items; i++)
	{
		if (cmd_table[i].name == NULL)
		{
			continue;
		}

		/* See if partial or full match is expected */
		if (len != 0)
		{
			if (!os_strncmp(cmd_table[i].name, name, len))
			{
				return i;
			}
		}
		else
		{
			if (!os_strcmp(cmd_table[i].name, name))
			{
				return i;
			}
		}
	}

	return -1;
}
#if defined(CONFIG_ATE_TEST)
static beken_semaphore_t ate_test_semaphore = NULL;
static void ate_uart_rx_isr(uart_id_t id, void *param)
{
	int ret;

	ret = rtos_set_semaphore(&ate_test_semaphore);
	if(kNoErr !=ret)
		CLI_LOGD("ate_uart_rx_isr: ATE set sema failed\r\n");
}

static void ate_uart_tx_isr(uart_id_t id, void *param)
{
	bk_uart_disable_tx_interrupt(bk_get_printf_port());
}

static void cli_ate_main(uint32_t data)
{

	char *msg = NULL;
	int ret = -1;
	int cnt = 0;
	uint8_t rx_data;

	if(NULL == ate_test_semaphore)
	{
		ret = rtos_init_semaphore(&ate_test_semaphore, 1);
		if (kNoErr != ret)
			CLI_LOGD("cli_ate_main: ATE create background sema failed\r\n");
	}

	bk_uart_disable_sw_fifo(bk_get_printf_port());
	bk_uart_register_rx_isr(bk_get_printf_port(), (uart_isr_t)ate_uart_rx_isr, NULL);
	bk_uart_enable_rx_interrupt(bk_get_printf_port());

	bk_uart_register_tx_isr(bk_get_printf_port(), (uart_isr_t)ate_uart_tx_isr, NULL);
	bk_uart_enable_tx_interrupt(bk_get_printf_port());

	get_device_id();
	send_chip_id();
	ate_test_multiple_cpus_init();

	while (1) {

		ret = rtos_get_semaphore(&ate_test_semaphore, BEKEN_WAIT_FOREVER);
		if(kNoErr == ret) {
			while(1)  /* read all data from rx-FIFO. */
			{
				ret = uart_read_byte_ex(bk_get_printf_port(), &rx_data);
				if (ret == -1)
					break;

				pCli->inbuf[cnt] = rx_data;
				cnt++;

				if(cnt >= INBUF_SIZE)
					break;
			}

			bkreg_run_command1(pCli->inbuf, cnt);

			if (cnt > 0) {
				for (int i = 0;i < cnt; i++)
					pCli->inbuf[i] = 0;
				cnt = 0;
			}
		}

		msg = (char *)pCli->inbuf;
		if (os_strcmp(msg, EXIT_MSG) == 0)
				break;
	}

	CLI_LOGD("CLI exited\r\n");
	os_free(pCli);
	pCli = NULL;

	rtos_delete_thread(NULL);
}

#else //CONFIG_ATE_TEST

/* Parse input line and locate arguments (if any), keeping count of the number
* of arguments and their locations.  Look up and call the corresponding cli
* function if one is found and pass it the argv array.
*
* Returns: 0 on success: the input line contained at least a function name and
*          that function exists and was called.
*          1 on lookup failure: there is no corresponding function for the
*          input line.
*          2 on invalid syntax: the arguments list couldn't be parsed
*/
static int handle_input(char *inbuf)
{
	struct {
		unsigned inArg: 1;
		unsigned inQuote: 1;
		unsigned done: 1;
		unsigned limQ : 1;
		unsigned isD : 2;
	} stat;
	char *argv[16];
	int argc = 0;
	int i = 0;
	const struct cli_command *command = NULL;
	const char *p;

	os_memset((void *)&argv, 0, sizeof(argv));
	os_memset(&stat, 0, sizeof(stat));

	if (inbuf[i] == '\0')
		return 0;

	do {
		switch (inbuf[i]) {
		case '\0':
			if (((argc == 0)||(stat.isD == 1))||(stat.limQ)||(stat.inQuote))
				return 2;
			stat.done = 1;
			break;

		case '"':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg)
				break;
			if (stat.inQuote && !stat.inArg)
				return 2;

			if (!stat.inQuote && !stat.inArg) {
				stat.inArg = 1;
				stat.inQuote = 1;
				argc++;
				argv[argc - 1] = &inbuf[i + 1];
			} else if (stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				stat.inQuote = 0;
				inbuf[i] = '\0';
			}
			break;

		case ' ':
			if (i > 0 && inbuf[i - 1] == '\\' && stat.inArg) {
				os_memcpy(&inbuf[i - 1], &inbuf[i],
						  os_strlen(&inbuf[i]) + 1);
				--i;
				break;
			}
			if (!stat.inQuote && stat.inArg) {
				stat.inArg = 0;
				inbuf[i] = '\0';
			}
			break;

        case '=':
            if(argc == 1) {
                inbuf[i] = '\0';
                stat.inArg = 0;
                stat.isD = 1;
            }
            else if(argc == 0){
                CLI_LOGD("The data does not conform to the regulations %d\r\n",__LINE__);
                return 2;
            }
            break;

        case ',':
            if((stat.isD == 1)&&(argc == 1))  ///=,
            {
                CLI_LOGD("The data does not conform to the regulations %d\r\n",__LINE__);
                return 2;
            }
            if(!stat.inQuote && stat.inArg) {
                stat.inArg = 0;
                inbuf[i] = '\0';
                stat.limQ = 1;
            }
            break;

		default:
			if (!stat.inArg) {
				stat.inArg = 1;
				argc++;
				argv[argc - 1] = &inbuf[i];
                stat.limQ = 0;
                if(stat.isD == 1) {
                    stat.isD = 2;
                }
			}
			break;
		}
	} while (!stat.done && ++i < INBUF_SIZE);

	if (stat.inQuote)
		return 2;

	if (argc < 1)
		return 0;

	if (!pCli->echo_disabled)
		CLI_LOGD("\r\n");

	/*
	* Some comamands can allow extensions like foo.a, foo.b and hence
	* compare commands before first dot.
	*/
	i = ((p = os_strchr(argv[0], '.')) == NULL) ? 0 :
		(p - argv[0]);
	command = lookup_command(argv[0], i);
	if (command == NULL)
		return 1;

	os_memset(pCli->outbuf, 0, OUTBUF_SIZE);
#if !defined(CONFIG_NIST_TEST) || (CONFIG_NIST_TEST == 0)
	cli_putstr("\r\n");
#endif

#if defined(CONFIG_STA_PS)
	/*if cmd,exit dtim ps*/
	if (os_strncmp(command->name, "ps", 2)) {
	}
#endif

#if CFG_CLI_DEBUG
	s_running_status |= CLI_COMMAND_IS_RUNNING;
	command->function((char *)pCli->outbuf, OUTBUF_SIZE, argc, argv);
	s_running_status &= ~CLI_COMMAND_IS_RUNNING;
#else
	command->function(pCli->outbuf, OUTBUF_SIZE, argc, argv);
#endif
	cli_putstr((char *)pCli->outbuf);
	return 0;
}

/* Perform basic tab-completion on the input buffer by string-matching the
* current input line against the cli functions table.  The current input line
* is assumed to be NULL-terminated. */
static void tab_complete(char *inbuf, unsigned int *bp)
{
	int i, n, m;
	const char *fm = NULL;

	CLI_LOGD("\r\n");

	/* show matching commands */
	for (i = 0, n = 0, m = 0; i < MAX_COMMANDS && n < pCli->num_commands;
		 i++) {
		if (pCli->commands[i]->name != NULL) {
			if (!os_strncmp(inbuf, pCli->commands[i]->name, *bp)) {
				m++;
				if (m == 1)
					fm = pCli->commands[i]->name;
				else if (m == 2)
					CLI_LOGD("%s %s ", fm,
							  pCli->commands[i]->name);
				else
					CLI_LOGD("%s ",
							  pCli->commands[i]->name);
			}
			n++;
		}
	}

	/* there's only one match, so complete the line */
	if (m == 1 && fm) {
		n = os_strlen(fm) - *bp;
		if (*bp + n < INBUF_SIZE) {
			os_memcpy(inbuf + *bp, fm + *bp, n);
			*bp += n;
			inbuf[(*bp)++] = ' ';
			inbuf[*bp] = '\0';
		}
	}

	/* just redraw input line */
#if !defined(CONFIG_NIST_TEST) || (CONFIG_NIST_TEST == 0)
	CLI_LOGD("%s%s", PROMPT, inbuf);
#endif
}
/* Get an input line.
*
* Returns: 1 if there is input, 0 if the line should be ignored. */
static int get_input(char *inbuf, unsigned int *bp)
{
	if (inbuf == NULL) {
		CLI_LOGD("inbuf_null\r\n");
		return 0;
	}

	while (cli_getchar(&inbuf[*bp]) == 1) {
#if defined(CONFIG_BKREG)
		if ((0x01U == (UINT8)inbuf[*bp]) && (*bp == 0)) {
			(*bp)++;
			continue;
		} else if ((0xe0U == (UINT8)inbuf[*bp]) && (*bp == 1)) {
			(*bp)++;
			continue;
		} else if ((0xfcU == (UINT8)inbuf[*bp]) && (*bp == 2)) {
			(*bp)++;
			continue;
		} else {
			if ((0x01U == (UINT8)inbuf[0])
				&& (0xe0U == (UINT8)inbuf[1])
				&& (0xfcU == (UINT8)inbuf[2])
				&& (*bp == 3)) {
				uint8_t ch = inbuf[*bp];
				uint8_t left = ch, len = 4 + (uint8_t)ch;
				inbuf[*bp] = ch;
				(*bp)++;

				if (ch >= INBUF_SIZE) {
					CLI_LOGE("Error: input buffer overflow\r\n");
					CLI_LOGD(PROMPT);
					*bp = 0;
					return 0;
				}

				while (left--) {
					if (0 == cli_getchar((char *)&ch))
						break;

					inbuf[*bp] = ch;
					(*bp)++;
				}

				bkreg_run_command(inbuf, len);
				os_memset(inbuf, 0, len);
				*bp = 0;
				continue;
			}
		}
#endif
		/* find first invalid input data, discard input data more than 0x7f */
		if ((uint8_t)inbuf[0] > 0x7f) {
			continue;
		}
		if (inbuf[*bp] == RET_CHAR)
			continue;
		if (inbuf[*bp] == END_CHAR) {   /* end of input line */
			inbuf[*bp] = '\0';
			*bp = 0;
			return 1;
		}

		if ((inbuf[*bp] == 0x08) || /* backspace */
			(inbuf[*bp] == 0x7f)) { /* DEL */
			if (*bp > 0) {
				(*bp)--;
				if (!pCli->echo_disabled)
					CLI_LOGD("%c %c", 0x08, 0x08);
			}
			continue;
		}

		if (inbuf[*bp] == '\t') {
			inbuf[*bp] = '\0';
			tab_complete(inbuf, bp);
			continue;
		}

		if (!pCli->echo_disabled)
			CLI_LOGD("%c", inbuf[*bp]);

		(*bp)++;
		if (*bp >= INBUF_SIZE) {
			CLI_LOGE("Error: input buffer overflow\r\n");
			CLI_LOGD(PROMPT);
			*bp = 0;
			return 0;
		}
	}

	return 0;
}

/* Print out a bad command string, including a hex
* representation of non-printable characters.
* Non-printable characters show as "\0xXX".
*/
static void print_bad_command(char *cmd_string)
{
	if (cmd_string != NULL) {
		char *c = cmd_string;
		CLI_LOGD("command '");
		while (*c != '\0') {
			if (is_print(*c))
				CLI_LOGD("%c", *c);
			else
				CLI_LOGD("\\0x%x", *c);
			++c;
		}
		CLI_LOGD("' not found\r\n");
	}
}

int bk_get_cli_uart_port(void) {
    return CONFIG_UART_CLI_BKREG_PORT;
}


bk_err_t bk_cli_uart_init(void)
{
	int ret;

	const uart_config_t config = 
	{
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_NONE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_FLOWCTRL_DISABLE,
		.src_clk = UART_SCLK_XTAL_26M
	};

    ret = bk_uart_init(bk_get_cli_uart_port(), &config);
	return ret;
}

/* Main CLI processing thread
*
* Waits to receive a command buffer pointer from an input collector, and
* then processes.  Note that it must cleanup the buffer when done with it.
*
* Input collectors handle their own lexical analysis and must pass complete
* command lines to CLI.
*/

static beken_semaphore_t s_bkcli_sem = NULL;
static void bk_cli_uart_rx_isr(uart_id_t id, void *param)
{
	int ret;
	bk_uart_disable_rx_interrupt(bk_get_cli_uart_port());
	ret = rtos_set_semaphore(&s_bkcli_sem);
	if(kNoErr !=ret)
		CLI_LOGD("cli_uart_rx_isr: ATE set sema failed\r\n");
}

static void bk_cli_uart_tx_isr(uart_id_t id, void *param)
{
	bk_uart_disable_tx_interrupt(bk_get_cli_uart_port());
}

static void cli_main(uint32_t data)
{
	char *msg = NULL;
	int ret = -1;
	// int cnt = 0;  // Unused variable
	// uint8_t rx_data;  // Unused variable
	char prompt[5] = {0};

	CLI_LOGI("cli_main: enter.\r\n");

	if (ate_is_enabled())
		strcpy(prompt,"\r\n# ");
	else
		strcpy(prompt,"\r\n$ ");

	bk_cli_uart_init();

	if(NULL == s_bkcli_sem)
	{
		ret = rtos_init_semaphore(&s_bkcli_sem, 1);
		if (kNoErr != ret)
			CLI_LOGD("cli_main: BKREG create sema failed\r\n");
	}

	bk_uart_disable_sw_fifo(bk_get_cli_uart_port());
	bk_uart_register_rx_isr(bk_get_cli_uart_port(), (uart_isr_t)bk_cli_uart_rx_isr, NULL);
	bk_uart_enable_rx_interrupt(bk_get_cli_uart_port());

	bk_uart_register_tx_isr(bk_get_cli_uart_port(), (uart_isr_t)bk_cli_uart_tx_isr, NULL);
	bk_uart_enable_tx_interrupt(bk_get_cli_uart_port());

	while (1) {

		ret = rtos_get_semaphore(&s_bkcli_sem, BEKEN_WAIT_FOREVER);
		if(kNoErr == ret) {
			if (get_input((char *)pCli->inbuf, &pCli->bp)) {
				msg = (char *)pCli->inbuf;

				if (os_strcmp(msg, EXIT_MSG) == 0)
					break;

				ret = handle_input(msg);
				if (ret == 1)
					print_bad_command(msg);
				else if (ret == 2)
					CLI_LOGD("syntax error\r\n");

			#if !defined(CONFIG_NIST_TEST) || (CONFIG_NIST_TEST == 0)
				CLI_LOGD(prompt);
			#endif
			}
		}
		bk_uart_enable_rx_interrupt(bk_get_cli_uart_port());
	}

	CLI_LOGI("CLI exited\r\n");
	// os_free(pCli);
	pCli = NULL;
	rtos_delete_thread(NULL);
}

#endif // CONFIG_ATE_TEST
static void cli_cmd_rsp(char *buf, u8 cmd_state)
{
	sprintf(buf, "CMDRsp:%s\r\n", cmd_state ? "OK" : "Fail");
}

void help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
void cli_sort_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if defined(CONFIG_BKREG)
#define BKCMD_RXSENS_R      'r'
#define BKCMD_RXSENS_X      'x'
#define BKCMD_RXSENS_s      's'

#define BKCMD_TXEVM_T       't'
#define BKCMD_TXEVM_X       'x'
#define BKCMD_TXEVM_E       'e'

int bkreg_cmd_handle_input(char *inbuf, int len)
{
	if (((char)BKREG_MAGIC_WORD0 == inbuf[0])
		&& ((char)BKREG_MAGIC_WORD1 == inbuf[1])
		&& ((char)BKREG_MAGIC_WORD2 == inbuf[2])) {
		if (cli_getchars(inbuf, len)) {
			bkreg_run_command(inbuf, len);
			os_memset(inbuf, 0, len);
			//BKREG cmd has been Handled
			return 1;
		}
	} else if ((((char)BKCMD_RXSENS_R == inbuf[0])
				&& ((char)BKCMD_RXSENS_X == inbuf[1])
				&& ((char)BKCMD_RXSENS_s == inbuf[2]))
			   || (((char)BKCMD_TXEVM_T == inbuf[0])
				   && ((char)BKCMD_TXEVM_X == inbuf[1])
				   && ((char)BKCMD_TXEVM_E == inbuf[2]))) {
		if (cli_getchars(inbuf, len)) {
			return 0;
		}
	}

	return 0;
}

int bkreg_command_handle(int argc, char **argv)
{
	int 		i, j;
	int 		buf_len = 0;
	int  		str_len = 0;

	for(i = 1; i < argc; i++)
	{
		str_len = strlen(argv[i]);
		for(j = 0; j < str_len; j++)
		{
			if(argv[i][j] == ' ')
			{
				break;
			}
		}

		if(j < str_len) /* contains ' ' in string. */
		{
			argv[0][buf_len++] = '"';
			for(j = 0; j < str_len; j++)
			{
				if(argv[i][j] == '"')
				{
					argv[0][buf_len++] = '\\';
				}
				argv[0][buf_len++] = argv[i][j];
			}
			argv[0][buf_len++] = '"';
		}
		else
		{
			memcpy(&argv[0][buf_len], argv[i], str_len);
			buf_len += str_len;
			argv[0][buf_len++] = ' ';
		}
	}

	argv[0][buf_len++] = '\r';
	argv[0][buf_len++] = '\n';

	return bkreg_cmd_handle_input(argv[0], buf_len);
}

#endif

static const struct cli_command built_ins[] = {
	{"help", NULL, help_command},
};

static int _cli_name_cmp(const void *a, const void *b)
{
	struct cli_command *cli0, *cli1;

	cli0 = *(struct cli_command **)a;
	cli1 = *(struct cli_command **)b;

	if ((NULL == a) || (NULL == b))
		return 0;

	return os_strcmp(cli0->name, cli1->name);
}

#if 1 //defined(CONFIG_WPA_SUPPLICANT)
extern int hexstr2bin(const char *hex, u8 *buf, size_t len);

int cli_hexstr2bin(const char *hex, u8 *buf, size_t len)
{
        return hexstr2bin(hex, buf, len);
}
#else
static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

int hexstr2bin(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

/**
 * hexstr2bin - Convert ASCII hex string into binary data
 * @hex: ASCII hex string (e.g., "01ab")
 * @buf: Buffer for the binary data
 * @len: Length of the text to convert in bytes (of buf); hex will be double
 * this size
 * Returns: 0 on success, -1 on failure (invalid hex string)
 */
int cli_hexstr2bin(const char *hex, u8 *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	u8 *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}
#endif // CONFIG_WPA_SUPPLICANT

void cli_sort_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t build_in_count;
	GLOBAL_INT_DECLARATION();

	build_in_count = sizeof(built_ins) / sizeof(struct cli_command);

	//CLI_LOGD("cmd_count:%d, built_in_count:%d\r\n", pCli->num_commands, build_in_count);

	GLOBAL_INT_DISABLE();
	qsort(&pCli->commands[build_in_count], pCli->num_commands - build_in_count, sizeof(struct cli_command *), _cli_name_cmp);
	GLOBAL_INT_RESTORE();
}

/* Built-in "help" command: prints all registered commands and their help
* text string, if any. */
void help_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i, n;
	uint32_t build_in_count = sizeof(built_ins) / sizeof(struct cli_command);

#define cmd_ind_printf		CLI_LOGI

	cmd_ind_printf("====Build-in Commands====\r\n");
	for (i = 0, n = 0; i < MAX_COMMANDS && n < pCli->num_commands; i++) {
		if (pCli->commands[i]->name) {
			if (pCli->commands[i]->help)
				cmd_ind_printf("%s: %s\r\n", pCli->commands[i]->name,
						  pCli->commands[i]->help ?
						  pCli->commands[i]->help : "");
			else
				cmd_ind_printf("%s\r\n", pCli->commands[i]->name);

			n++;
			if (n == build_in_count)
				cmd_ind_printf("\r\n====User Commands====\r\n");
		}
	}

	//cli_cmd_rsp(&pcWriteBuffer[0], 1);

}


int cli_register_command(const struct cli_command *command)
{
	int i;
	if (!command->name || !command->function)
		return 0;

	if (pCli->num_commands < MAX_COMMANDS) {
		/* Check if the command has already been registered.
		* Return 0, if it has been registered.
		*/
		for (i = 0; i < pCli->num_commands; i++) {
			if (pCli->commands[i] == command)
				return 0;
		}
		pCli->commands[pCli->num_commands++] = command;
		return 0;
	}

	return 1;
}

int cli_unregister_command(const struct cli_command *command)
{
	int i;
	if (!command->name || !command->function)
		return 1;

	for (i = 0; i < pCli->num_commands; i++) {
		if (pCli->commands[i] == command) {
			pCli->num_commands--;
			int remaining_cmds = pCli->num_commands - i;
			if (remaining_cmds > 0) {
				os_memmove(&pCli->commands[i], &pCli->commands[i + 1],
						   (remaining_cmds *
							sizeof(struct cli_command *)));
			}
			pCli->commands[pCli->num_commands] = NULL;
			return 0;
		}
	}

	return 1;
}

int cli_register_commands(const struct cli_command *commands, int num_commands)
{
	int i;
	for (i = 0; i < num_commands; i++)
		if (cli_register_command(commands++))
			return 1;
	return 0;
}

int cli_unregister_commands(const struct cli_command *commands,
							int num_commands)
{
	int i;
	for (i = 0; i < num_commands; i++)
		if (cli_unregister_command(commands++))
			return 1;

	return 0;
}

/* ========= CLI input&output APIs ============ */
int cli_printf(const char *msg, ...)
{
	va_list ap;
	char *pos, message[256];
	int sz;
	int nMessageLen = 0;

	os_memset(message, 0, 256);
	pos = message;

	sz = 0;
	va_start(ap, msg);
	nMessageLen = vsnprintf(pos, 256 - sz, msg, ap);
	va_end(ap);

	if (nMessageLen <= 0) return 0;

	cli_putstr((const char *)message);
	return 0;
}

int cli_putstr(const char *msg)
{
	if (msg[0] != 0)
		uart_write_string(bk_get_cli_uart_port(), msg);
	return 0;
}

int cli_getchar(char *inbuf)
{
	if (bk_uart_read_bytes(bk_get_cli_uart_port(), inbuf, 1, CLI_GETCHAR_TIMEOUT) > 0)
		return 1;
	else
		return 0;
}

int cli_getchars(char *inbuf, int len)
{
	if (bk_uart_read_bytes(bk_get_cli_uart_port(), inbuf, len, CLI_GETCHAR_TIMEOUT) > 0)
		return 1;
	else
		return 0;
}

int cli_getchars_prefetch(char *inbuf, int len)
{
	return 0;
}

int cli_get_all_chars_len(void)
{
	return uart_get_length_in_buffer(bk_get_cli_uart_port());
}

#if defined(CONFIG_CLI)
static const struct cli_command user_clis[] = {
};
#endif

beken_thread_t cli_thread_handle = NULL;
int bk_cli_init(void)
{
	int ret;

#if defined(CONFIG_CLI)

	pCli = &s_Cli;//(struct cli_st *)os_malloc(sizeof(struct cli_st));
	if (pCli == NULL)
		return kNoMemoryErr;

	os_memset((void *)pCli, 0, sizeof(struct cli_st));

	if (cli_register_commands(&built_ins[0],
							  sizeof(built_ins) / sizeof(struct cli_command)))
		goto init_general_err;

	if (cli_register_commands(user_clis, sizeof(user_clis) / sizeof(struct cli_command)))
		goto init_general_err;

	extern int cli_uart_init(void);
	cli_uart_init();

	cli_flash_init();
	cli_flash_test_init();

#if (CLI_CFG_TRNG == 1)
	cli_trng_init();
#endif

#if defined(CONFIG_EASY_FLASH)
    cli_easyflash_init();
#endif

#if (CLI_CFG_PWR == 1)
	cli_pwr_init();
#endif

	cli_mem_init();

#if defined(CONFIG_NIST_TEST) && (CONFIG_NIST_TEST == 1)
	cli_nist_init();
#endif

/*-------------BT&MultMedia cli command init begin----------------*/
#if (CLI_CFG_BLE == 1)
	cli_ble_init();
#endif

#if 0
/*-----------------WIFI cli command init begin-------------------*/
#if (CLI_CFG_WIFI == 1)
	cli_wifi_init();
#endif

#if (CLI_CFG_NETIF == 1)
	cli_netif_init();
#endif
#endif

#if (CLI_CFG_PHY == 1)
	cli_phy_init();
#endif

#if (CONFIG_SECURE_TEST == 1)
extern int cli_cipher_aes_init(void);
extern int cli_secure_storage_init(void);
extern int cli_ecdsa_test_init(void);
extern int cli_signature_verify_init(void);

cli_ecdsa_test_init();
cli_secure_storage_init();
cli_cipher_aes_init();
cli_signature_verify_init();
#endif

#if 0
#if (CLI_CFG_IPERF == 1)
#if (defined(CONFIG_WIFI_CLI_ENABLE) || defined(CONFIG_ARMINO_BLE))
	cli_phy_init();
#endif
#endif

#if (CLI_CFG_IPERF == 1)
	cli_iperf_init();
#endif

#if (defined(CONFIG_AT_CMD))
    cli_at_init();
#endif
#if (CLI_CFG_EVENT == 1)
	cli_event_init();
#endif

#if defined(CONFIG_DEBUG_FIRMWARE)
#if (CLI_CFG_TEMP_DETECT == 1)
	cli_temp_detect_init();
#endif
#if (defined(CONFIG_VOLT_DETECT))
	cli_volt_detect_init();
#endif
#if (CLI_CFG_AIRKISS == 1)
	cli_airkiss_init();
#endif

#if defined(CONFIG_LWIP)
	cli_lwip_init();
#endif
#endif //CONFIG_DEBUG_FIRMWARE

/*----------------WIFI cli command init end----------------------*/
#if (CLI_CFG_SCR == 1)
	cli_scr_init();
#endif


/*----------------platform cli command init begin------------------*/
#if (CLI_CFG_MEM == 1)
	cli_mem_init();
#endif

#if (CLI_CFG_OS == 1)
	cli_os_init();
#endif

#if (CLI_CFG_MISC == 1)
	cli_misc_init();
#endif

#if (CLI_CFG_FLASH == 1)
	//cli_flash_init();
#endif

#if (CLI_CFG_FLASH == 1)
	cli_flash_test_init();
#endif

#if (CLI_CFG_DUMP_FLASH == 1)
	cli_dump2flash_init();
#endif

#if (CLI_CFG_TIMER == 1)
	cli_timer_init();
#endif

#if (CLI_CFG_WDT == 1)
	cli_wdt_init();
#endif

#if (CLI_CFG_TRNG == 1)
	cli_trng_init();
#endif

#if (CLI_CFG_EFUSE == 1)
	cli_efuse_init();
#endif

#if (CLI_CFG_DMA == 1)
	cli_dma_init();
#endif

#if (CLI_CFG_GPIO == 1)
	cli_gpio_init();
#endif

#if (CLI_CFG_SDIO_HOST == 1)
	cli_sdio_host_init();
#endif

#if (CLI_CFG_SDIO_SLAVE == 1)
	cli_sdio_slave_init();
#endif

#if (CLI_CFG_KEYVALUE == 1)
    cli_keyVaule_init();
#endif

#if (CLI_CFG_UART == 1)
	cli_uart_init();
#endif

#if (CLI_CFG_SPI == 1)
	cli_spi_init();
#endif

#if (CLI_CFG_QSPI == 1)
	cli_qspi_init();
#endif

#if (CONFIG_AON_RTC_TEST == 1)
	cli_aon_rtc_init();
#endif

#if (CLI_CFG_I2C == 1)
	cli_i2c_init();
#endif

#if (CLI_CFG_ADC == 1)
#if defined(CONFIG_SARADC_V1P2)
	bk_sadc_register_cli_test_feature();
#endif // CONFIG_SARADC_V1P2
	cli_adc_init();
#endif

#if (CLI_CFG_SD == 1)
	cli_sd_init();
#endif

#if (CLI_FATFS == 1)
	cli_fatfs_init();
#endif

#if (CLI_CFG_VFS == 1)
	cli_vfs_init();
#endif

#if (CLI_CFG_SECURITY == 1)
	cli_security_init();
#endif

#if (CLI_CFG_MICO == 1)
	cli_mico_init();
#endif

#if (CLI_CFG_REG == 1)
	cli_reg_init();
#endif

#if defined(CONFIG_USB)
	cli_usb_init();
#endif

#if (CLI_CFG_PSRAM == 1)
	cli_psram_init();
#endif

#if (CLI_CFG_PWM == 1)
	cli_pwm_init();
#endif

#if (CLI_CFG_EXCEPTION == 1)
	cli_exception_init();
#endif

#if (CLI_CFG_TOUCH == 1)
	cli_touch_init();
#endif

#if defined(CONFIG_VAULT_SUPPORT)
#if (CLI_CFG_SECURITYIP == 1)
	cli_securityip_init();
#endif
#endif

#if defined(CONFIG_MBEDTLS)
#if defined(CONFIG_MBEDTLS_TEST)
	cli_mbedtls_init();
#endif
#endif

#if (CLI_CFG_CALENDAR == 1)
	cli_calendar_init();
#endif


#if defined(CONFIG_EASY_FLASH)
    int cli_easyflash_init(void);
    cli_easyflash_init();
#endif

#if (CLI_CFG_I2S == 1)
	cli_i2s_init();
#endif

#endif //#if 0

/*--------------------platform cli command init end--------------------*/

	/* sort cmds after registered all cmds. */
	cli_sort_command(NULL, 0, 0, NULL);

#endif  // CONFIG_CLI

#if defined(CONFIG_BK_CLI_USE_SEPRATE_UART)
#if defined(CONFIG_ATE_TEST)
	ret = rtos_create_thread(&cli_thread_handle,
							 SHELL_TASK_PRIORITY,
							 "cli",
							 (beken_thread_function_t)cli_ate_main /*cli_main*/,
							 4096,
							 0);
#else
	ret = rtos_create_thread(&cli_thread_handle,
							 BEKEN_DEFAULT_WORKER_PRIORITY,
							 "cli",
							 (beken_thread_function_t)cli_main,
							 3072,
							 0);
#endif // #if CONFIG_ATE_TEST
	if (ret != kNoErr) {
		CLI_LOGE("Error: Failed to create cli thread: %d\r\n",
				  ret);
		goto init_general_err;
	}

#endif // #if CONFIG_BK_CLI_USE_SEPRATE_UART


#if defined(CONFIG_CLI)

	pCli->initialized = 1;
#if defined(CONFIG_NIST_TEST) && (CONFIG_NIST_TEST == 1)
	pCli->echo_disabled = 1;
#else
	pCli->echo_disabled = 0;
#endif


#endif  // CONFIG_CLI

	return kNoErr;

init_general_err:
    if (pCli) {
        //os_free(pCli);
        pCli = NULL;
    }

    return kGeneralErr;
}


/* Parse input line and locate arguments (if any), keeping count of the number
* of arguments and their locations.  Look up and call the corresponding cli
* function if one is found and pass it the argv array.
*
* Returns: 0 on success: the input line contained at least a function name and
*          that function exists and was called.
*          1 on lookup failure: there is no corresponding function for the
*          input line.
*          2 on invalid syntax: the arguments list couldn't be parsed
*/
int beken_commands_handle_input(int argc, char *argv[])
{
    const struct cli_command *command = NULL;

    if (argc < 1) {
        dbg("Usage: <command> [args...]\r\n");
        return -1;
    }

    command = lookup_command(argv[0], 0);
    if (command == NULL) {
        dbg("Command NOT found: %s\r\n", argv[0]);
        return 1;
    }

    char write_buf[128] = {0};
    command->function(write_buf, sizeof(write_buf), argc, &argv[0]);
    return 0;
}
