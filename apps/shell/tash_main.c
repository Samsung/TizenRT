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
#ifdef CONFIG_SECURED_TASH
#include <mbedtls/sha256.h>
#endif
#include <tinyara/ascii.h>
#include "tash_internal.h"

enum tash_input_state_e {
	IN_VOID,
	IN_WORD,
	IN_QUOTE,
	IN_APOSTROPHE
};

/* Following defines are fixed to avoid many configuration variables for TASH */
#define TASH_TOKEN_MAX        (32)
#ifdef CONFIG_TASH
#define TASH_TRY_MAXCNT       (5)
#if !defined(CONFIG_DISABLE_POLL)
#define SELECT_TIMEOUT_SECS   (6)
#define SELECT_TIMEOUT_USECS  (0)
#endif
#define TASH_TASK_STACKSIZE   (4096)
#define TASH_TASK_PRIORITY    (125)

#define TASH_PROMPT           "TASH>>"
#endif							/* CONFIG_TASH */

static int tash_running = FALSE;

#if TASH_MAX_STORE > 0
static void tash_clear_line(int fd, int len)
{
	if (write(fd, (const void *)"\r", sizeof("\r")) <= 0) {
		shdbg("TASH: echo failed (errno = %d)\n", get_errno());
	}

	for (int i = 0; i < len; ++i) {
		if (write(fd, (const void *)" ", sizeof(" ")) <= 0) {
			shdbg("TASH: echo failed (errno = %d)\n", get_errno());
		}
	}
}
#endif

static void tash_print_cmd(int fd, char *cmd, int pos)
{
	if (write(fd, (const void *)"\r", sizeof("\r")) <= 0) {
		shdbg("TASH: echo failed (errno = %d)\n", get_errno());
	}

	if (write(fd, (const void *)TASH_PROMPT, sizeof(TASH_PROMPT)) <= 0) {
		shdbg("TASH: echo failed (errno = %d)\n", get_errno());
	}

	if (write(fd, (const void *)cmd, pos) <= 0) {
		shdbg("TASH: echo failed (errno = %d)\n", get_errno());
	}
}

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
char *tash_read_input_line(int fd)
{
	#define SKIP_NEXT_CHAR(n)    char_idx += (n)
	#define PREV_CHAR            buffer[pos - 1]
	#define CURR_CHAR            buffer[pos]
	#define NEXT_CHAR            buffer[pos + 1]
	#define NEXTNEXT_CHAR        buffer[pos + 2]

	int bufsize = TASH_LINEBUFLEN;
	int pos = 0;
	int nbytes;
	int char_idx;
	bool is_tab_pressed = false;
#if TASH_MAX_STORE > 0
	bool is_esc_pressed = false;
	bool prepare_direction_key = false;
	bool is_direction_pressed = false;
	char direction;
	int prev_cmd_len = 0;
#endif
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

				if ((CURR_CHAR == ASCII_BS) || (CURR_CHAR == ASCII_DEL)) {
					/* Back space or Del key - delete previous character */

					int valid_char_pos = pos + 1;
					if (pos > 0) {
						pos--;
						/* Update screen */
						if (write(fd, "\b \b", 3) <= 0) {
							shdbg("TASH: echo failed (errno = %d)\n", get_errno());
						}
					}

					if ((NEXT_CHAR != 0x0) && (valid_char_pos < TASH_LINEBUFLEN)) {
						memmove(&CURR_CHAR, &NEXT_CHAR, (bufsize - valid_char_pos));
					}
					is_tab_pressed = false;
					is_esc_pressed = false;
					prev_cmd_len = pos;
				} else if (CURR_CHAR == ASCII_TAB) {
					/* TAB key - Auto-complete the command functionality */

					if (pos > 0 && tash_do_autocomplete(buffer, &pos, is_tab_pressed) == true) {
						tash_print_cmd(fd, buffer, pos);
						prev_cmd_len = pos;
					}
					is_tab_pressed = true;
					is_esc_pressed = false;
				}
#if TASH_MAX_STORE > 0
				/* ASCII_ESC + ASCII_LBRACKET + ASCII_A is the UP key (direction)
				 * ASCII_ESC + ASCII_LBRACKET + ASCII_B is the DOWN key (direction)
				 */

				else if (CURR_CHAR == ASCII_ESC) {
					if (NEXT_CHAR == ASCII_LBRACKET) {
						if ((NEXTNEXT_CHAR == ASCII_A) || (NEXTNEXT_CHAR == ASCII_B)) {
							is_direction_pressed = true;
							direction = NEXTNEXT_CHAR;
							SKIP_NEXT_CHAR(2);
						} else {
							prepare_direction_key = true;
						}
						NEXT_CHAR = ASCII_NUL;

					} else {
						is_esc_pressed = true;
					}

					CURR_CHAR = ASCII_NUL;
					is_tab_pressed = false;

				} else if (is_esc_pressed) {
					if (CURR_CHAR == ASCII_LBRACKET) {
						if ((NEXT_CHAR == ASCII_A) || (NEXT_CHAR == ASCII_B)) {
							is_direction_pressed = true;
							direction = NEXT_CHAR;
							SKIP_NEXT_CHAR(1);
						} else {
							prepare_direction_key = true;
						}
						CURR_CHAR = ASCII_NUL;

					} else {
						shvdbg("TASH: Not support\n");
					}

					CURR_CHAR = ASCII_NUL;
					is_tab_pressed = false;
					is_esc_pressed = false;

				} else if (prepare_direction_key) {
					if ((CURR_CHAR == ASCII_A) || (CURR_CHAR == ASCII_B)) {
						is_direction_pressed = true;
						direction = CURR_CHAR;
					} else {
						shvdbg("TASH: Not support\n");
					}

					CURR_CHAR = ASCII_NUL;
					is_tab_pressed = false;
					is_esc_pressed = false;
					prepare_direction_key = false;
				}
#endif
				else {
					if (CURR_CHAR == ASCII_CR) {
						CURR_CHAR = ASCII_LF;
					}

#ifdef CONFIG_SECURED_TASH
					if (tash_running || CURR_CHAR == ASCII_LF) {
#endif
						/* echo */
						if (write(fd, &CURR_CHAR, 1) <= 0) {
							shdbg("TASH: echo failed (errno = %d)\n", get_errno());
						}
#ifdef CONFIG_SECURED_TASH
					} else if (write(fd, "*", 1) <= 0) {
						shdbg("TASH: echo failed (errno = %d)\n", get_errno());
					}
#endif

					pos++;
					if (pos >= TASH_LINEBUFLEN) {
						printf("\nTASH: length of input character is too long, maximum length is %d\n", TASH_LINEBUFLEN);
						buffer[0] = ASCII_NUL;
						return buffer;
					}
					is_tab_pressed = false;
					is_esc_pressed = false;
					prev_cmd_len = pos;
				}
			}

#if TASH_MAX_STORE > 0
			if (is_direction_pressed) {
				if (tash_search_cmd(buffer, &pos, direction) == true) {
					tash_clear_line(fd, sizeof(TASH_PROMPT) + prev_cmd_len);

					tash_print_cmd(fd, buffer, pos);
					prev_cmd_len = pos;
				}
				is_direction_pressed = false;
			}
#endif
#if !defined(CONFIG_DISABLE_POLL)
		}
#endif
	} while (PREV_CHAR != ASCII_LF);

	PREV_CHAR = ASCII_NUL;
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
#ifndef CONFIG_DISABLE_SIGNALS
		usleep(20);
#endif
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

	fd = tash_open_console();
	if (fd < 0) {
		exit(EXIT_FAILURE);
	}

#ifdef CONFIG_SECURED_TASH
	tash_check_security(fd);
#endif

	tash_running = TRUE;

	do {
		nbytes = write(fd, (const void *)TASH_PROMPT, sizeof(TASH_PROMPT));
		if (nbytes <= 0) {
			shdbg("TASH: prompt is not displayed (errno = %d)\n", get_errno());
#ifndef CONFIG_DISABLE_SIGNALS
			usleep(20);
#endif
			continue;
		}
		line_buff = tash_read_input_line(fd);
		shvdbg("TASH: input string (%s)\n", line_buff);

		tash_execute_cmdline(line_buff);

		tash_free(line_buff);
	} while (tash_running);

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

void tash_stop(void)
{
	tash_running = FALSE;
}
#endif							/* CONFIG_TASH */

int tash_execute_cmdline(char *buff)
{
	int argc;
	char *argv[TASH_TOKEN_MAX];
	enum tash_input_state_e state;
	bool has_nextcmd = false;
	int ret = OK;

#if TASH_MAX_STORE > 0
	/* Find, verify the exclamation command and replace it to real command */

	if (check_exclam_cmd(buff) == ERROR) {
		*buff = ASCII_NUL;
		return ERROR;
	}

	/* Save the input command into history command buffer */

	tash_store_cmd(buff);
#endif

	do {
		for (argc = 0, argv[argc] = NULL, has_nextcmd = false, state = IN_VOID; *buff && argc < TASH_TOKEN_MAX - 1 && has_nextcmd == false; buff++) {
			switch (state) {

			case IN_VOID:
				switch (*buff) {

				case ASCII_SPACE:
					/* ignore, do nothing */

					break;

				case ASCII_HASH:
					/* following string is a comment, let's quit parsing */

					has_nextcmd = false;
					*buff = ASCII_NUL;
					*(buff + 1) = ASCII_NUL;
					break;

				case ASCII_APOSTROPHE:
					/* Argument is starting with opening single quotation mark */
					
					state = IN_APOSTROPHE;
                                        argv[argc++] = buff + 1;
                                        break;

				case ASCII_QUOTE:
					/* Argument is starting with opening double quotation mark */

					state = IN_QUOTE;
					argv[argc++] = buff + 1;
					break;

				case ASCII_SEMICOLON:
					if (argc) {
						/* Even semicolon comes in IN_VOID state, (argc != 0) means there is saved commands already.
						 * Let's set the has_nextcmd true to check continuous command at next loop
						 * and execute current as ASCII_LF.
						 */

						has_nextcmd = true;
					} else {
						/* Only semicolon comes without command.
						 * Let's feedback the failure.
						 */

						printf("syntax error near unexpected token ';'\n");
						return ERROR;
					}

				case ASCII_LF:
					/* Command is finished, excute it */

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
						/* closing double quotation mark, need to remove quotation mark */

						tash_remove_char(buff);

						state = IN_WORD;
					}
					buff--;
				}
				break;
			
			case IN_APOSTROPHE:
				if (*buff == (char)ASCII_APOSTROPHE) {
                                        if (*(buff - 1) == ASCII_BACKSLASH) {
                                               /* # character, need to remove a backslash */

                                                tash_remove_char(buff - 1);
                                        } else {
                                                /* closing single quotation mark, need to remove quotation mark */

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

				case ASCII_APOSTROPHE:
					if (*(buff - 1) == ASCII_BACKSLASH) {
                                                /* # character, need to remove a backslash */

                                                tash_remove_char(buff - 1);
                                        } else {
                                                /* opening single quotation mark, need to remove quotation mark */

                                                tash_remove_char(buff);
                                                state = IN_APOSTROPHE;
                                        }
                                        buff--;
                                        break;

				case ASCII_QUOTE:
					if (*(buff - 1) == ASCII_BACKSLASH) {
						/* # character, need to remove a backslash */

						tash_remove_char(buff - 1);
					} else {
						/* opening double quotation mark, need to remove quotation mark */

						tash_remove_char(buff);
						state = IN_QUOTE;
					}
					buff--;
					break;

				case ASCII_SEMICOLON:
					/* Set the has_nextcmd true to check contiuouse command and execute current as ASCII_LF */

					has_nextcmd = true;

				case ASCII_LF:
					/* Command is finished, excute it */

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

		if (state == IN_APOSTROPHE) {
                        shdbg("TASH: unclosed single quotation mark\n");
                        argc = 0;
                }

		/* make a null at end of argv */

		argv[argc] = NULL;

		/* excute a command if it is valid */

		if (argc > 0) {
			ret = tash_execute_cmd(argv, argc);
		}
	} while (has_nextcmd == true);

	return ret;
}
