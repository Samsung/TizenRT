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
 * @file command.h
 */

#ifndef __ARTIK_COMMAND_H__
#define __ARTIK_COMMAND_H__

typedef int (*command_fn)(int argc, char *argv[]);

struct command {
	const char name[16];
	const char usage[128];
	command_fn fn;
};

int commands_parser(int argc, char **argv, const struct command *commands);
void usage(const char *command_base, const struct command *commands);

#endif /* _ARTIK_COMMAND_H */
