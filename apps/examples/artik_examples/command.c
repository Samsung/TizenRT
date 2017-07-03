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

void usage(const char *command_base, const struct command *commands)
{
	const struct command *cmd = commands;

	fprintf(stderr, "usage:\n");

	while (cmd->fn) {
		fprintf(stderr, "\t%s %s - %s\n", command_base, cmd->name, cmd->usage);
		cmd++;
	}
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
				char tname[32];

				snprintf(tname, 32, "%s_command", cmd->name);
				task_create(tname, SCHED_PRIORITY_DEFAULT, 16384, cmd->fn, argv);
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
