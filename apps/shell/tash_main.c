/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/// @file   tash_main.c
/// @brief  Main functions of TinyAra SHell (TASH)

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#if !defined(CONFIG_DISABLE_POLL)
#include <sys/select.h>
#endif
#include <tinyara/ascii.h>
#include "tash_internal.h"

#if defined(CONFIG_TASH_PASSWORD)
#include <tls/sha256.h>
#endif

enum tash_input_state_e {
	IN_VOID,
	IN_WORD,
	IN_QUOTE
};

/* Following defines are fixed to avoid many configuration variables for TASH */
#define TASH_TOKEN_MAX        (32)
#ifdef CONFIG_TASH
#define TASH_LINEBUFLEN       (128)
#define TASH_TRY_MAXCNT       (5)
#if !defined(CONFIG_DISABLE_POLL)
#define SELECT_TIMEOUT_SECS   (6)
#define SELECT_TIMEOUT_USECS  (0)
#endif
#define TASH_TASK_STACKSIZE   (4096)
#define TASH_TASK_PRIORITY    (125)

#ifndef CONFIG_TASH_QSIZE
#define CONFIG_TASH_QSIZE     (5)
#endif

const char tash_prompt[] = "TASH>>";
#endif							/* CONFIG_TASH */

int tash_running = FALSE;

static void tash_remove_char(char *char_pos)
{
	int remaining;

	/* Get a length of string remained */

	remaining = strlen(char_pos + 1);

	/* Add one for NULL */

	remaining++;

	/* remove character from buffer */

	memmove(char_pos, char_pos + 1, remaining);
}

#ifdef CONFIG_TASH
/** @brief Read the input command
 *  @ingroup tash
 */

#if CONFIG_TASH_QSIZE > 0
struct cmd_buffer {
	char cmd_buf[TASH_LINEBUFLEN];
	int cmd_len;
};

static struct cmd_buffer *cmdq[CONFIG_TASH_QSIZE];
static int cmdq_widx;
static int retries;
char blank[TASH_LINEBUFLEN];

static struct cmd_buffer *tash_de_queue(int offset)
{
	int index;

	if (offset > CONFIG_TASH_QSIZE) {
		offset %= CONFIG_TASH_QSIZE;
	}

	index = cmdq_widx - offset + CONFIG_TASH_QSIZE;
	index %= CONFIG_TASH_QSIZE;

	return cmdq[index];
}

static int tash_arr_queue(char *cmd, int cmdlen)
{
	int i;
	struct cmd_buffer *prev = NULL;

	for (i = 1; i <= CONFIG_TASH_QSIZE; i++) {
		prev = tash_de_queue(i);
		if(prev->cmd_len != cmdlen) {
			continue;
		}
		if (!memcmp(cmd, prev->cmd_buf, prev->cmd_len)) {
			break;
		}
	}

	/* New command */
	if (i == 1) {
		return 1;
	} else if (i == CONFIG_TASH_QSIZE + 1) {
		return 0;
	}

	/* Rearrangement command */
	int index = (cmdq_widx - i + CONFIG_TASH_QSIZE) % CONFIG_TASH_QSIZE;

	while (i-- > 1)
	{
		cmdq[index] = tash_de_queue(i);
		index = cmdq_widx - i + CONFIG_TASH_QSIZE;
		index %= CONFIG_TASH_QSIZE;
	}

	cmdq[index] = prev;

	return 1;
}

static void tash_en_queue(char *buf, int len)
{
	if (buf == NULL || len <= 0 || len > TASH_LINEBUFLEN) {
		return;
	}

	/* Check whether input command is new or not */
	if (!tash_arr_queue(buf, len)) {
		memcpy(cmdq[cmdq_widx]->cmd_buf, buf, len);
		cmdq[cmdq_widx++]->cmd_len = len;

		if (cmdq_widx == CONFIG_TASH_QSIZE) {
			cmdq_widx = 0;
		}
	}
}

/* Handle a Control Sequence Introducer (CSI) */
static int tash_handle_csi(int operation, int fd, char *buffer)
{
	struct cmd_buffer *cmd;

	switch (operation) {
	case ASCII_A:
		cmd = tash_de_queue(++retries);
		break;
	case ASCII_B:
		cmd = tash_de_queue(--retries);
		break;
	default:
		return 0;
	}

	if (cmd == NULL) {
		return 0;
	}

	write(fd, blank, TASH_LINEBUFLEN);
	write(fd, tash_prompt, sizeof(tash_prompt));

	memcpy(buffer, cmd->cmd_buf, cmd->cmd_len);

	if (write(fd, &buffer[0], cmd->cmd_len) <= 0) {
		shdbg("TASH: echo failed (errno = %d)\n", get_errno());
	}
	return cmd->cmd_len;
}
#endif

static char *tash_read_input_line(int fd)
{
	int bufsize = TASH_LINEBUFLEN;
	int pos = 0;
	int nbytes = 0;
	int char_idx = 0;
#if !defined(CONFIG_DISABLE_POLL)
	fd_set tfd;
	struct timeval stimeout;
	stimeout.tv_sec = SELECT_TIMEOUT_SECS;
	stimeout.tv_usec = SELECT_TIMEOUT_USECS;
#endif
	char *buffer = tash_alloc(sizeof(char) * bufsize);

	if (!buffer) {
		shdbg("TASH: input buffer alloc failed\n");
		return NULL;
	}

	memset(buffer, 0x0, bufsize);

	do {
#if !defined(CONFIG_DISABLE_POLL)
		FD_ZERO(&tfd);
		FD_SET(fd, &tfd);

		if ((select(fd + 1, &tfd, NULL, NULL, &stimeout)) && FD_ISSET(fd, &tfd)) {
#endif
			/* read characters */
			nbytes = read(fd, &buffer[pos], (bufsize - pos));
			if (nbytes < 0) {
				shdbg("TASH: can not read uart\n");
				return buffer;
			}

			for (char_idx = 0; char_idx < nbytes; char_idx++) {
				/* treat backspace and delete */
				if ((buffer[pos] == ASCII_BS) || (buffer[pos] == ASCII_DEL)) {
					int valid_char_pos = pos + 1;
					if (pos > 0) {
						pos--;
						/* Update screen */
						if (write(fd, "\b \b", 3) <= 0) {
							shdbg("TASH: echo failed (errno = %d)\n", get_errno());
						}
					}

					if ((buffer[valid_char_pos] != 0x0) && (valid_char_pos < TASH_LINEBUFLEN)) {
						memmove(&buffer[pos], &buffer[valid_char_pos], (bufsize - valid_char_pos));
					}
				}
#if CONFIG_TASH_QSIZE > 0
				/* Handle a Control Sequence Introducer (CSI : start with '^[') */
				else if (nbytes == 3 && (buffer[pos] == ASCII_ESC) && (buffer[pos + 1] == ASCII_LBRACKET)) {
					pos = tash_handle_csi(buffer[pos + 2], fd, buffer);
					break;
				}
#endif
				else {
					if (buffer[pos] == ASCII_CR) {
						buffer[pos] = ASCII_LF;
					}

#if defined(CONFIG_TASH_PASSWORD)
					if (!tash_running) {
						if (buffer[pos] == ASCII_LF) {
							write(fd, &buffer[pos], 1);
						} else {
							if (write(fd, "*", 1) <= 0) {
								shdbg("TASH: echo failed (errno = %d)\n", get_errno());
							}
						}
					} else
#endif
					{
						/* echo */
						if (write(fd, &buffer[pos], 1) <= 0) {
							shdbg("TASH: echo failed (errno = %d)\n", get_errno());
						}
					}


					pos++;
					if (pos >= TASH_LINEBUFLEN) {
						printf("\nTASH: length of input character is too long, maximum length is %d\n", TASH_LINEBUFLEN);
						buffer[0] = ASCII_NUL;
						return buffer;
					}

					if (buffer[pos - 1] == ASCII_LF) {
						buffer[pos - 1] = ASCII_NUL;
#if CONFIG_TASH_QSIZE > 0
						retries = 0;
						tash_en_queue(buffer, pos - 1);
#endif
						return buffer;
					}
				}
			}
#if !defined(CONFIG_DISABLE_POLL)
		}
#endif
	} while (buffer[pos - 1] != ASCII_LF);

	buffer[pos - 1] = ASCII_NUL;
	return buffer;
}

static int tash_open_console(void)
{
	int cnt = 0;
	int ret;

	do {
		ret = open("/dev/console", O_RDWR);
		if (ret > 0) {
			break;
		}

		usleep(20);
		if (cnt == TASH_TRY_MAXCNT) {
			shdbg("TASH: can not open uart, tried (%d) times\n", cnt);
			ret = ERROR;
		}
	} while (cnt++ < TASH_TRY_MAXCNT);

	return ret;
}

/** @brief TASH loop
 *  @ingroup tash
 */
static int tash_main(int argc, char *argv[])
{
	char *line_buff;
	int fd;
	int nbytes;
	int ret = OK;

	ret = tash_init();
	if (ret == ERROR) {
		exit(EXIT_FAILURE);
	}

#if CONFIG_TASH_QSIZE > 0
	int i;
	struct cmd_buffer *tmp = malloc(CONFIG_TASH_QSIZE * sizeof(struct cmd_buffer));

	if (tmp == NULL) {
		exit(EXIT_FAILURE);
	} else {
		memset(tmp, 0, CONFIG_TASH_QSIZE * sizeof(struct cmd_buffer));
		for (i = 0; i < CONFIG_TASH_QSIZE; i++)
		{
			cmdq[i] = tmp + i;
		}
	}
	memset(blank, ' ', TASH_LINEBUFLEN);
	blank[0] = '\r';
	blank[TASH_LINEBUFLEN - 1] = '\r';
#endif

	fd = tash_open_console();
	if (fd < 0) {
		exit(EXIT_FAILURE);
	}

#if defined(CONFIG_TASH_PASSWORD)
	int i;
	int len = 0;
	char chr[1];
	unsigned char temp[2];
	unsigned char sha256[64];
	unsigned char result[64];
	unsigned char sha256_hex[64];
	const char passwd_prompt[] = "PASSWORD>>";

	do {
		nbytes = write(fd, passwd_prompt, sizeof(passwd_prompt));
		if (nbytes <= 0) {
			usleep(20);
			continue;
		}
		line_buff = tash_read_input_line(fd);
		len = strlen(line_buff);

		sprintf((char *)sha256, "%s", CONFIG_TASH_PASSWORD_SHA256);

		for (i = 0; i < 64;) {
			strncpy(chr, (char *)&sha256[i], 1);
			temp[i % 2] = strtoul(chr, NULL, 16);

			if (i % 2) {
				sha256_hex[i / 2] = (temp[0] << 4) | temp[1];
			}

			i++;
		}

		mbedtls_sha256((unsigned char *)line_buff, len, result, 0);

		tash_free(line_buff);

		if (!strncmp((char *)sha256_hex, (char *)result, 32)) {
			break;
		}
	} while (1);
#endif

	tash_running = TRUE;

	do {
		nbytes = write(fd, tash_prompt, sizeof(tash_prompt));
		if (nbytes <= 0) {
			shdbg("TASH: prompt is not displayed (errno = %d)\n", get_errno());
			usleep(20);
			continue;
		}
		line_buff = tash_read_input_line(fd);
		shvdbg("TASH: input string (%s)\n", line_buff);

		ret = tash_execute_cmdline(line_buff);

		tash_free(line_buff);
	} while (tash_running);

#if CONFIG_TASH_QSIZE > 0
	free(tmp);
#endif
	(void)close(fd);
	return 0;					/* TBD: For now, it always returns success */
}

int tash_start(void)
{
	int pid;
	int errcode;

	pid = task_create("tash", TASH_TASK_PRIORITY, TASH_TASK_STACKSIZE, tash_main, (FAR char *const *)NULL);
	if (pid < 0) {
		errcode = errno;
		DEBUGASSERT(errcode > 0);
		return -errcode;
	}

	return pid;
}
#endif							/* CONFIG_TASH */

int tash_execute_cmdline(char *buff)
{
	int argc;
	char *argv[TASH_TOKEN_MAX];
	enum tash_input_state_e state;
	bool is_nextcmd = false;
	int ret = OK;

	do {
		for (argc = 0, argv[argc] = NULL, is_nextcmd = false, state = IN_VOID; *buff && argc < TASH_TOKEN_MAX - 1 && is_nextcmd == false; buff++) {
			switch (state) {

			case IN_VOID:
				switch (*buff) {

				case ASCII_SPACE:
					/* ignore, do nothing */

					break;

				case ASCII_HASH:
					/* following string is a comment, let's quit parsing */

					is_nextcmd = false;
					*buff = ASCII_NUL;
					*(buff + 1) = ASCII_NUL;
					break;

				case ASCII_QUOTE:
					/* Argument is started with opening double quatation mark */

					state = IN_QUOTE;
					argv[argc++] = buff + 1;
					break;

				case ASCII_LF:
				case ASCII_SEMICOLON:
					/* Command is finished, excute it */

					is_nextcmd = true;
					*buff = ASCII_NUL;
					break;

				default:
					state = IN_WORD;
					argv[argc++] = buff;
					break;
				}
				break;

			case IN_QUOTE:
				if (*buff == '"') {
					if (*(buff - 1) == ASCII_BACKSLASH) {
						/* # character, need to remove a backslash */

						tash_remove_char(buff - 1);
					} else {
						/* closing double quatation mark, need to remove quatation mark */

						tash_remove_char(buff);

						state = IN_WORD;
					}
					buff--;
				}
				break;

			case IN_WORD:
				switch (*buff) {
				case ASCII_SPACE:
					/* end of argument */

					state = IN_VOID;
					*buff = '\0';
				break;

				case ASCII_QUOTE:
					if (*(buff - 1) == ASCII_BACKSLASH) {
						/* # character, need to remove a backslash */

						tash_remove_char(buff - 1);
					} else {
						/* opening double quatation mark, need to remove quatation mark */

						tash_remove_char(buff);
						state = IN_QUOTE;
					}
					buff--;
					break;

				case ASCII_LF:
				case ASCII_SEMICOLON:
					/* Command is finished, excute it */

					is_nextcmd = true;
					*buff = ASCII_NUL;
					break;

				default:
					/* do nothing */

					break;
				}
				break;
			}
		}

		/* unclosed quotation */

		if (state == IN_QUOTE) {
			shdbg("TASH: unclosed double quotation mark\n");
			argc = 0;
		}

		/* make a null at end of argv */

		argv[argc] = NULL;

		/* excute a command if it is valid */

		if (argc > 0) {
			ret = tash_execute_cmd(argv, argc);
		}
	} while (is_nextcmd == true);

	return ret;
}
