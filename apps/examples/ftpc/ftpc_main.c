/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/ftpc/ftpc_main.c
 *
 *   Copyright (C) 2011-2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "ftpc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FTPC_MAX_ARGUMENTS 4

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct cmdmap_s {
	const char *cmd;	/* Name of the command */
	cmd_t handler;		/* Function that handles the command */
	uint8_t minargs;	/* Minimum number of arguments */
	uint8_t maxargs;	/* Maximum number of arguments */
	const char *usage;	/* Usage instructions for 'help' command */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char g_delim[] = " \t\n";

static int cmd_lhelp(SESSION handle, int argc, char **argv);
static int cmd_lunrecognized(SESSION handle, int argc, char **argv);

static pthread_t pth = -1;
static mqd_t g_send_mqfd;
static mqd_t g_recv_mqfd;

static const struct cmdmap_s g_cmdmap[] = {
	{ "cd",       cmd_rchdir,  2, 2, "<directory>" },
	{ "chmod",    cmd_rchmod,  3, 3, "<permissions> <path>" },
	{ "get",      cmd_rget,    2, 4, "[-a|b] <rname> [<lname>]" },
	{ "help",     cmd_lhelp,   1, 1, "" },
	{ "idle",     cmd_ridle,   1, 2, "[<idletime>]" },
	{ "login",    cmd_rlogin,  2, 3, "<uname> [<password>]" },
	{ "ls",       cmd_rls,     1, 2, "[<dirpath>]" },
	{ "mkdir",    cmd_rmkdir,  2, 2, "<directory>" },
	{ "noop",     cmd_rnoop,   1, 1, "" },
	{ "put",      cmd_rput,    2, 4, "[-a|b] <lname> [<rname>]" },
	{ "pwd",      cmd_rpwd,    1, 1, "" },
	{ "quit",     cmd_rquit,   1, 1, "" },
	{ "rename",   cmd_rrename, 3, 3, "<oldname> <newname>" },
	{ "rhelp",    cmd_rhelp,   1, 2, "[<command>]" },
	{ "rm",       cmd_runlink, 2, 2, "<filepath>" },
	{ "rmdir",    cmd_rrmdir,  2, 2, "<directory>" },
	{ "size",     cmd_rsize,   2, 2, "<filepath>" },
	{ "time",     cmd_rtime,   2, 2, "<filepath>" },
	{ "up",       cmd_rcdup,   1, 1, "" },
	{ NULL,       NULL,        1, 1, NULL }
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_lhelp
 ****************************************************************************/

static int cmd_lhelp(SESSION handle, int argc, char **argv)
{
	const struct cmdmap_s *ptr;

	printf("Local FTPC commands:\n");
	for (ptr = g_cmdmap; ptr->cmd; ptr++) {
		if (ptr->usage) {
			printf("  %s %s\n", ptr->cmd, ptr->usage);
		} else {
			printf("  %s\n", ptr->cmd);
		}
	}
	return OK;
}

/****************************************************************************
 * Name: cmd_lunrecognized
 ****************************************************************************/

static int cmd_lunrecognized(SESSION handle, int argc, char **argv)
{
	printf("Command %s unrecognized\n", argv[0]);
	return ERROR;
}

/****************************************************************************
 * Name: ftpc_argument
 ****************************************************************************/

char *ftpc_argument(char **saveptr)
{
	char *pbegin = *saveptr;
	char *pend   = NULL;
	const char *term;

	/* Find the beginning of the next token */

	for (; *pbegin && strchr(g_delim, *pbegin) != NULL; pbegin++);

	/* If we are at the end of the string with nothing
	* but delimiters found, then return NULL.
	*/

	if (!*pbegin) {
		return NULL;
	} else if (*pbegin == '#') {
		/* Does the token begin with '#' -- comment */
		/* Return NULL meaning that we are at the end of the line */

		*saveptr = pbegin;
		pbegin   = NULL;
	} else {
		/* Otherwise, we are going to have to parse to find the end of
		* the token.  Does the token begin with '"'?
		*/

		if (*pbegin == '"') {
			/* Yes.. then only another '"' can terminate the string */

			pbegin++;
			term = "\"";
		} else {
			/* No, then any of the usual terminators will terminate the argument */

			term = g_delim;
		}

		/* Find the end of the string */

		for (pend = pbegin + 1; *pend && strchr(term, *pend) == NULL; pend++);

		/* pend either points to the end of the string or to
		* the first delimiter after the string.
		*/

		if (*pend) {
			/* Turn the delimiter into a null terminator */

			*pend++ = '\0';
		}

		/* Save the pointer where we left off */

		*saveptr = pend;

	}

	/* Return the beginning of the token. */

	return pbegin;
}

/****************************************************************************
 * Name: ftpc_execute
 ****************************************************************************/

static int ftpc_execute(SESSION handle, int argc, char *argv[])
{
	const struct cmdmap_s *cmdmap;
	const char            *cmd;
	cmd_t                  handler = cmd_lunrecognized;
	int                    ret;

	/* The form of argv is:
	*
	* argv[0]:      The command name.  This is argv[0] when the arguments
	*               are, finally, received by the command handler
	* argv[1]:      The beginning of argument (up to FTPC_MAX_ARGUMENTS)
	* argv[argc]:   NULL terminating pointer
	*/

	cmd = argv[0];

	/* See if the command is one that we understand */

	for (cmdmap = g_cmdmap; cmdmap->cmd; cmdmap++) {
		if (strcmp(cmdmap->cmd, cmd) == 0) {
			/* Check if a valid number of arguments was provided.  We
			* do this simple, imperfect checking here so that it does
			* not have to be performed in each command.
			*/

			if (argc < cmdmap->minargs) {
				/* Fewer than the minimum number were provided */

				printf("Too few arguments for '%s'\n", cmd);
				return ERROR;
			} else if (argc > cmdmap->maxargs) {
				/* More than the maximum number were provided */

				printf("Too many arguments for '%s'\n", cmd);
				return ERROR;
			} else {
				/* A valid number of arguments were provided (this does
				* not mean they are right).
				*/

				handler = cmdmap->handler;
				break;
			}
		}
	}

	ret = handler(handle, argc, argv);
	if (ret < 0) {
		printf("%s failed: %d\n", cmd, errno);
	} else {
		printf("Command %s successed\n", cmd);
	}
	return ret;
}

/****************************************************************************
 * Name: ftpc_parse
 ****************************************************************************/

int ftpc_parse(SESSION handle, char *cmdline)
{
	FAR char *argv[FTPC_MAX_ARGUMENTS+1];
	FAR char *saveptr;
	FAR char *cmd;
	int       argc;
	int       ret;

	/* Initialize parser state */

	memset(argv, 0, FTPC_MAX_ARGUMENTS*sizeof(FAR char *));

	/* Parse out the command at the beginning of the line */

	saveptr = cmdline;
	cmd = ftpc_argument(&saveptr);

	/* Check if any command was provided -OR- if command processing is
	* currently disabled.
	*/

	if (!cmd) {
		/* An empty line is not an error */

		return OK;
	}

	/* Parse all of the arguments following the command name. */

	argv[0] = cmd;
	for (argc = 1; argc < FTPC_MAX_ARGUMENTS; argc++) {
		argv[argc] = ftpc_argument(&saveptr);
		if (!argv[argc]) {
			break;
		}
	}
	argv[argc] = NULL;

	/* Check if the maximum number of arguments was exceeded */

	if (argc > FTPC_MAX_ARGUMENTS) {
		printf("Too many arguments\n");
		ret = -EINVAL;
	} else {
	/* Then execute the command */

	ret = ftpc_execute(handle, argc, argv);
	}

	return ret;
}

void* ftpc_thread(void* argv)
{
	struct ftpc_connect_s connect = { { 0 }, 0 };
	SESSION g_handle = NULL;
	FAR char *ptr = NULL;

	/* Check if the argument includes a port number */
	ptr = strchr((char*)argv, ':');
	if (ptr) {
		*ptr = '\0';
		connect.port = atoi(ptr + 1);
	}

	/* In any event, we can now extract the IP address from the comman-line */

	connect.addr.s_addr = inet_addr((char*)argv);

	/* Connect to the FTP server */

	g_handle = ftpc_connect(&connect);
	if (!g_handle) {
		printf("Failed to connect to the server: %d\n", errno);
		return NULL;
	} else {
		printf("Connected successful\n");
	}

	char msg_buffer[FTPC_LINELEN];
	struct mq_attr attr;
	int nbytes;

	/* Fill in attributes for message queue */

	attr.mq_maxmsg = 20;
	attr.mq_msgsize = FTPC_LINELEN;
	attr.mq_flags = 0;

	/* Set the flags for the open of the queue.
	* Make it a blocking open on the queue, meaning it will block if
	* this task tries to read from the queue when the queue is empty
	*
	*   O_CREAT - the queue will get created if it does not already exist.
	*   O_RDONLY - we are only planning to read from the queue.
	*
	* Open the queue, and create it if the sending process hasn't
	* already created it.
	*/

	g_recv_mqfd = mq_open("mqueue", O_RDONLY | O_CREAT, 0666, &attr);
	if (g_recv_mqfd == (mqd_t)ERROR) {
		printf("tc_mqueue_mq_open FAIL\n");
		pthread_exit((pthread_addr_t)1);
	}

	while (1) {
		memset(msg_buffer, 0x00, FTPC_LINELEN);
		nbytes = mq_receive(g_recv_mqfd, msg_buffer, FTPC_LINELEN, 0);
		if (nbytes > 0) {
			(void)ftpc_parse(g_handle, msg_buffer);
			if (strncmp(msg_buffer, "quit", sizeof("quit") + 1) == 0) {  //exit thread
				printf("Disconnected\n");
				pth = -1;
				return NULL;
			}
		}
	}
	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int ftpc_main(int argc, char **argv, char **envp)
{
	const struct cmdmap_s *cmdmap;
	const char *cmd;
	int i;

	if (argc < 2) {
		printf("Usage:\n");
		printf("   %s open xx.xx.xx.xx[:pp]\n", argv[0]);
		printf("Where\n");
		printf("  xx.xx.xx.xx is the IP address of the FTP server\n");
		printf("  pp is option port to use with the FTP server\n\n");
		printf("*****************************************************************\n");
		printf("* FTP client sample application                                 *\n");
		printf("*****************************************************************\n");
		printf("Available commands:\n");
		printf("   cd\n");
		printf("   cdmod\n");
		printf("   get\n");
		printf("   help\n");
		printf("   login\n");
		printf("   ls\n");
		printf("   mkdir\n");
		printf("   noop\n");
		printf("   open\n");
		printf("   put\n");
		printf("   quit\n");
		printf("   rename\n");
		printf("   rhelp\n");
		printf("   rm\n");
		printf("   rmdir\n");
		printf("   size\n");
		printf("   time\n");
		printf("   up\n");
		printf("use the command help to get extended help about arguments for the\n");
		printf("different commands\n");
		printf("*****************************************************************\n");
		return 0;
	}

	cmd = argv[1];

	if (strncmp("open", cmd, sizeof("open") + 1) == 0) {
		int ret = pthread_create(&pth, NULL, ftpc_thread, (void*)argv[2]);
		if (ret) {
			printf("Thread creation Failed %d\n", errno);
			return 0;
		}
		pthread_join(pth, NULL);
	} else {
		if (pth == -1) {
			printf("Not connected\n");
			return 0;
		}
		for (cmdmap = g_cmdmap; cmdmap->cmd; cmdmap++) {
			if (strncmp(cmdmap->cmd, cmd, strlen(cmdmap->cmd) + 1) == 0) {
				if (argc < cmdmap->minargs + 1)	{
					/* Fewer than the minimum number were provided */
					printf("Too few arguments for '%s'\n", cmd);
					return ERROR;
				} else if (argc > cmdmap->maxargs + 1) {
					/* More than the maximum number were provided */
					printf("Too many arguments for '%s'\n", cmd);
					return ERROR;
				} else {
					/* A valid number of arguments were provided (this does
					* not mean they are right).
					*/
					char msg_buffer[FTPC_LINELEN];
					struct mq_attr attr;
					int status = 0;

					/* Fill in attributes for message queue */

					attr.mq_maxmsg = 20;
					attr.mq_msgsize = FTPC_LINELEN;
					attr.mq_flags = 0;

					/* Set the flags for the open of the queue.
					* Make it a blocking open on the queue, meaning it will block if
					* this process tries to send to the queue and the queue is full.
					*
					*   O_CREAT - the queue will get created if it does not already exist.
					*   O_WRONLY - we are only planning to write to the queue.
					*
					* Open the queue, and create it if the receiving process hasn't
					* already created it.
					*/

					g_send_mqfd = mq_open("mqueue", O_WRONLY | O_CREAT, 0666, &attr);
					if (g_send_mqfd == (mqd_t)-1) {
						printf("tc_mqueue_mq_open FAIL\n");
					}

					msg_buffer[0] = '\0';
					strcat(msg_buffer, argv[1]);
					for (i = 2; i < argc; i++) {
						strcat(msg_buffer, " ");
						strcat(msg_buffer, argv[i]);
					}

					status = mq_send(g_send_mqfd, msg_buffer, strlen(msg_buffer), 0);
					if (status < 0) {
						printf("tc_mqueue_mq_send FAIL : failure=%d\n", status);
					}

					/* Close the queue and return success */

					if (mq_close(g_send_mqfd) < 0) {
						printf("tc_mqueue_mq_close FAIL\n");
					} else {
						g_send_mqfd = NULL;
					}

					return 0;
				}
			}
		}
	}

	return 0;
}
