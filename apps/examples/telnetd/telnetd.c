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
/****************************************************************************
 * examples/telnetd/telnetd.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * This is a leverage of similar logic from uIP:
 *
 *   Author: Adam Dunkels <adam@sics.se>
 *   Copyright (c) 2003, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apps/shell/tash.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <tinyara/config.h>
#include <protocols/telnetd.h>
#include <netutils/netlib.h>
#include "telnetd.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifdef CONFIG_TASH_COMMAND_INTERFACE
#define MAX_TELNET_CMDS CONFIG_TASH_MAX_COMMANDS
#else
#define MAX_TELNET_CMDS (4)
#endif
#define TELNET_TOKENLEN (32)
#define MAX_NUMBER_OF_TOKENS (5)
/****************************************************************************
 * Private Types
 ****************************************************************************/
struct ptentry_s {
	FAR char commandstr[TELNET_TOKENLEN];
	void (*pfunc)(int argc, char **argv);
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void telnetd_parse(FAR char *line, int len);
static void telnetd_help(int argc, char **argv);
static void telnetd_quit(int argc, char **argv);
static void telnetd_install_commands(void);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct ptentry_s g_parsetab[MAX_TELNET_CMDS];

/****************************************************************************
 * Name: telnetd_help
****************************************************************************/
static void telnetd_help(int argc, char **argv)
{
	struct ptentry_s *entry;
	printf("Available commands:\n");
	for (entry = g_parsetab; entry->pfunc != NULL; entry++) {
		printf("\t%s\n", entry->commandstr);
	}
}

/****************************************************************************
 * Name: telnetd_quit
 ****************************************************************************/
static void telnetd_quit(int argc, char **argv)
{
	printf("Bye!\n");
	exit(0);
}

/****************************************************************************
 * Name: telnetd_parse
 ****************************************************************************/

static void telnetd_parse(FAR char *line, int len)
{
	struct ptentry_s *entry;
	FAR char *cmd = NULL;
	FAR char *bkp = NULL;
	FAR char *saveptr;
	char *tokens[MAX_NUMBER_OF_TOKENS];
	int i = 0;
	int hit = 0;

	/* Get the command from the beginning the line */

	cmd = strtok_r(line, " \t\n\r\f\v", &saveptr);
	bkp = cmd;

	while (bkp) {
		tokens[i++] = bkp;
		bkp = strtok_r(NULL, " \t\n\r\f\v", &saveptr);
	}

	if (cmd) {
		/* Now find the matching command in the command table */

		for (entry = g_parsetab; entry->pfunc != NULL; entry++) {
			if (strncmp(entry->commandstr, cmd, strlen(entry->commandstr)) == 0) {
				hit = 1;
				break;
			}
		}
		if (hit) {
			entry->pfunc(i, &tokens[0]);
		} else {
			printf("Command not found\n");
		}
	}
}

/****************************************************************************
 * Name: telnetd_session
 ****************************************************************************/
static void telnetd_install_commands(void)
{
#ifdef CONFIG_TASH_COMMAND_INTERFACE
	int i = 0;
	int count = 0;
	count = tash_get_cmdscount();

	printf("Tash cmd count is %d\n", count);

	for (i = 0; i < count; i++) {
		tash_get_cmdpair(g_parsetab[i].commandstr, (TASH_CMD_CALLBACK *)&g_parsetab[i].pfunc, i);
		if (strncmp(g_parsetab[i].commandstr, "help", 4) == 0) {
			g_parsetab[i].pfunc = &telnetd_help;
		} else if (strncmp(g_parsetab[i].commandstr, "exit", 4) == 0) {
			g_parsetab[i].pfunc = &telnetd_quit;
		}
	}
#else
	struct ptentry_s *entry = g_parsetab;
	const char str1[] = "help";
	const char str2[] = "exit";
	int len = 4;				//strlen of help & exit
	strncpy(entry->commandstr, str1, len);
	g_parsetab[0].pfunc = &telnetd_help;
	entry++;
	strncpy(entry->commandstr, str2, len);
	g_parsetab[1].pfunc = &telnetd_quit;
#endif
}

/****************************************************************************
 * Name: telnetd_session
 ****************************************************************************/

int telnetd_session(int argc, char *argv[])
{
	char line[128];
	memset(line, 0, 128);

	for (;;) {
		printf(SHELL_PROMPT);
		fflush(stdout);

		if (fgets(line, 128, stdin) == NULL) {
			break;
		}

		telnetd_parse(line, 128);
	}

	return 0;
}

/****************************************************************************
 * Name: telnetd_netinit
 ****************************************************************************/

static void telnetd_netinit(void)
{
#if 0
	struct in_addr addr;
#endif
#ifdef CONFIG_EXAMPLES_TELNETD_NOMAC
	uint8_t mac[IFHWADDRLEN];
#endif

	/* Many embedded network interfaces must have a software assigned MAC */

#ifdef CONFIG_EXAMPLES_TELNETD_NOMAC
	mac[0] = 0x00;
	mac[1] = 0xe0;
	mac[2] = 0xde;
	mac[3] = 0xad;
	mac[4] = 0xbe;
	mac[5] = 0xef;
	netlib_setmacaddr("eth0", mac);
#endif

#if 0							// Block this code. Network conf is setup by DHCPC
	/* Set up our host address */

	addr.s_addr = HTONL(CONFIG_EXAMPLES_TELNETD_IPADDR);
	netlib_set_ipv4addr("eth0", &addr);

	/* Set up the default router address */

	addr.s_addr = HTONL(CONFIG_EXAMPLES_TELNETD_DRIPADDR);
	netlib_set_dripv4addr("eth0", &addr);

	/* Setup the subnet mask */

	addr.s_addr = HTONL(CONFIG_EXAMPLES_TELNETD_NETMASK);
	netlib_set_ipv4netmask("eth0", &addr);
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int telnetd_main(int argc, char *argv[])
#endif
{
	struct telnetd_config_s config;
	int ret;

	/* Configure the network */

	printf("telnetd_main: Initializing the network\n");
	telnetd_netinit();

	/* Configure the telnet daemon */

	config.d_port = HTONS(23);
	config.d_priority = CONFIG_EXAMPLES_TELNETD_DAEMONPRIO;
	config.d_stacksize = CONFIG_EXAMPLES_TELNETD_DAEMONSTACKSIZE;
	config.t_priority = CONFIG_EXAMPLES_TELNETD_CLIENTPRIO;
	config.t_stacksize = CONFIG_EXAMPLES_TELNETD_CLIENTSTACKSIZE;
	config.t_entry = telnetd_session;

	/* Install telnet commands */
	telnetd_install_commands();

	/* Start the telnet daemon */

	printf("telnetd_main: Starting the Telnet daemon\n");
	ret = telnetd_start(&config);
	if (ret < 0) {
		printf("Failed to tart the Telnet daemon\n");
	}

	printf("telnetd_main: Exiting\n");
	return 0;
}
