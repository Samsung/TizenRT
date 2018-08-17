/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <pthread.h>

struct pthread_arg {
	int argc;
	char **argv;
};

extern int aws_jobs_sample(int argc, char **argv);
extern int aws_shadow_sample(int argc, char **argv);
extern int aws_shadow_sample_console_echo(int argc, char **argv);
extern int aws_subscribe_publish_sample(int argc, char **argv);
extern int aws_subscribe_publish_library_sample(int argc, char **argv);

static void aws_show_usage(const char *program)
{
	printf("\nUSAGE:\n");
	printf(" %s jobs                      : jobs_sample\n", program);
	printf(" %s shadow [option]           : shadow_sample\n", program);
	printf(" %s shadowconsole [option]    : shadow_sample_console_echo\n", program);
	printf(" %s subscribe [option]        : subscribe_publish_sample\n", program);
	printf(" %s subscribelibrary [option] : subscribe_publish_library_sample\n", program);
}

int aws_examples_cb(void *args)
{
	int argc;
	char **argv;

	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;

	if (argc > 1) {
		if (strcmp(argv[1], "jobs") == 0) {
			aws_jobs_sample(argc - 1, (void *)(argv + 1));
		} else if (strcmp(argv[1], "shadow") == 0) {
			aws_shadow_sample(argc - 1, (void *)(argv + 1));
		} else if (strcmp(argv[1], "shadowconsole") == 0) {
			aws_shadow_sample_console_echo(argc - 1, (void *)(argv + 1));
		} else if (strcmp(argv[1], "subscribe") == 0) {
			aws_subscribe_publish_sample(argc - 1, (void *)(argv + 1));
		} else if (strcmp(argv[1], "subscribelibrary") == 0) {
			aws_subscribe_publish_library_sample(argc - 1, (void *)(argv + 1));
		} else {
			aws_show_usage(argv[0]);
		}
	} else {
		aws_show_usage(argv[0]);
	}

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int aws_main(int argc, FAR char *argv[])
#endif
{
	pthread_t tid;
	pthread_attr_t attr;
	int ret;
	struct pthread_arg args;

	args.argc = argc;
	args.argv = argv;

	/* Initialize the attribute variable */
	if ((ret = pthread_attr_init(&attr)) != 0) {
		printf("%s: pthread_attr_init failed, status=%d\n", __func__, ret);
	}

	/* set a stacksize */
	if ((ret = pthread_attr_setstacksize(&attr, 32768)) != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, ret);
	}

	/* create pthread with entry function */
	if ((ret = pthread_create(&tid, &attr, (pthread_startroutine_t) aws_examples_cb, (void *)&args)) != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, ret);
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}

