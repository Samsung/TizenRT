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

/**
 * @file command.c
 */


#include "command.h"

#include <stdio.h>

struct launch_command_params {
	const struct command *cmd;
	int argc;
	char **argv;
};

void usage(const char *command_base, const struct command *commands)
{
	const struct command *cmd = commands;

	fprintf(stderr, "usage:\n");

	while (cmd->fn) {
		if (strlen(cmd->usage))
			fprintf(stderr, "\t%s %s - %s\n", command_base, cmd->name,
					cmd->usage);
		else
			fprintf(stderr, "\t%s %s\n", command_base, cmd->name);

		cmd++;
	}
}

static pthread_addr_t launch_command(void *arg)
{
	struct launch_command_params *params = (struct launch_command_params *)arg;
	int i = 0;
	int argc = params->argc + 1;
	char **argv = (char **)malloc(argc * sizeof(char *));

	if (!argv)
		return NULL;

	argv[0] = (char *)params->cmd->name;
	for (i = 1; i < argc; i++)
		argv[i] = params->argv[i - 1];

	params->cmd->fn(argc, argv);

	free(argv);

	return NULL;
}

int commands_parser(int argc, char **argv, const struct command *commands)
{
	const struct command *cmd = commands;

	if (argc < 2) {
		usage(argv[0], commands);
		return -1;
	}

	while (cmd->fn) {
		if (!strcmp(argv[1], cmd->name)) {
			if (cmd->fn) {
				pthread_t tid;
				pthread_attr_t attr;
				struct sched_param sparam;
				struct launch_command_params tparams;

				tparams.cmd = cmd;
				tparams.argc = argc;
				tparams.argv = argv;

				pthread_attr_init(&attr);
				sparam.sched_priority = SCHED_PRIORITY_DEFAULT;
				pthread_attr_setschedparam(&attr, &sparam);
				pthread_attr_setschedpolicy(&attr, SCHED_RR);
				pthread_attr_setstacksize(&attr, 16384);
				pthread_create(&tid, &attr, launch_command, (void *)&tparams);
				pthread_attr_destroy(&attr);
				pthread_setname_np(tid, "launch-command");
				pthread_join(tid, NULL);
			}
			break;
		}

		cmd++;
	}

	if (!cmd->fn) {
		fprintf(stderr, "Unknown command\n");
		usage(argv[0], commands);
		return -1;
	}

	return 0;
}
