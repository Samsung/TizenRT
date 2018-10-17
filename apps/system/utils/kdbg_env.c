/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
/// @file   kdbg_env.c
/// @brief  functions to set/unset a environment value
#include <tinyara/config.h>

#if defined(CONFIG_ENABLE_ENV_GET_CMD) || defined(CONFIG_ENABLE_ENV_SET_CMD) || defined(CONFIG_ENABLE_ENV_UNSET_CMD)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#endif

#ifdef CONFIG_ENABLE_ENV_SET_CMD
int kdbg_env_set(int argc, char **args)
{
	int ret;

	if ((argc != 3) || !strncmp(args[1], "--help", strlen("--help") + 1)) {
		printf("\nUsage : setenv NAME VALUE\n");
		printf("Set each NAME to VALUE in the environment");
		return ERROR;
	}

	ret = setenv(args[1], args[2], true);
	if (ret < 0) {
		printf("SET is failed, err (%d)\n", get_errno());
	} else {
		printf("env \"%s=%s\" is SET\n", args[1], args[2]);
	}

	return ret;
}
#endif

#ifdef CONFIG_ENABLE_ENV_UNSET_CMD
int kdbg_env_unset(int argc, char **args)
{
	int ret;

	if ((argc != 2) || !strncmp(args[1], "--help", strlen("--help") + 1)) {
		printf("\nUsage : unsetenv NAME\n");
		printf("Remove the variable, NAME from the environment\n");
		return ERROR;
	}

	ret = unsetenv(args[1]);
	if (ret < 0) {
		printf("UNSET is failed, err (%d)\n", get_errno());
	} else {
		printf("env \"%s\" is UNSET\n", args[1]);
	}

	return ret;
}
#endif

#ifdef CONFIG_ENABLE_ENV_GET_CMD
int kdbg_env_get(int argc, char **args)
{
	char *env_ptr;
#ifndef CONFIG_BUILD_PROTECTED
	size_t env_size = 0;
	char *env_end;
#endif
	int ret = OK;

#ifndef CONFIG_BUILD_PROTECTED
	/* get_environ_ptr is not supported in protected build */
	if (argc == 1) {
		env_ptr = get_environ_ptr(&env_size);
		if (env_ptr != NULL) {
			env_end = &env_ptr[env_size];
			for (; env_ptr < env_end; env_ptr += (strlen(env_ptr) + 1)) {
				printf("%s\n", env_ptr);
			}
		} else {
			printf("There is no env in this task group\n");
		}
	} else
#endif
	{
		if (argc != 2 || !strncmp(args[1], "--help", strlen("--help"))) {
#ifndef CONFIG_BUILD_PROTECTED
			printf("\nUsage : getenv [NAME]\n");
			printf("Display the value of a variable, NAME or all environment variables\n");
			printf("If NAME is not specified, 'name=value' pairs of all environment variables will be displayed\n");
#else
			printf("\nUsage : getenv NAME\n");
			printf("Display the value of environment variable, NAME\n");
#endif
			return ERROR;
		}

		env_ptr = getenv(args[1]);
		if (env_ptr != NULL) {
			printf("%s=%s\n", args[1], env_ptr);
		} else {
			printf("GET is failed, err (%d)\n", get_errno());
			ret = ERROR;
		}
	}

	return ret;
}
#endif
