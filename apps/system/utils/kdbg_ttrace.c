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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <debug.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <tinyara/config.h>
#include <tinyara/ttrace.h>
#include <tinyara/clock.h>

#define MAX_TAG_NAMESIZE 4

struct tag_list {
	const char *name;
	const char *longname;
	const int tags;
};

static const struct tag_list ttrace_tags[] = {
	{"none",    "None",          TTRACE_TAG_OFF},
	{"apps",    "Applications",  TTRACE_TAG_APPS},
	{"libs",    "Libraries",     TTRACE_TAG_LIBS},
	{"lock",    "Lock",          TTRACE_TAG_LOCK},
	{"task",    "TASK",          TTRACE_TAG_TASK},
	{"ipc",     "IPC",           TTRACE_TAG_IPC},
};

int param = 0;
int selected_tags = 0;
int is_overwritable = 0;

static void show_help(void);
void wait_ttrace_dump(void);

static int print_uid_packet(struct trace_packet *packet)
{
	int8_t uid = packet->codelen & ~TTRACE_CODE_UNIQUE;
	printf("[%06d:%06d] %03d: %c|%u\r\n",
		   packet->ts.tv_sec, packet->ts.tv_usec,
		   packet->pid,
		   packet->event_type, uid);
	return sizeof(struct trace_packet) - TTRACE_MSG_BYTES;
}

static int print_message_packet(struct trace_packet *packet)
{
	printf("[%06d:%06d] %03d: %c|%s\r\n",
		   packet->ts.tv_sec, packet->ts.tv_usec,
		   packet->pid,
		   packet->event_type,
		   packet->msg.message);
	return sizeof(struct trace_packet);
}

static int print_sched_packet(struct trace_packet *packet)
{
	printf("[%06d:%06d] %03d: %c|prev_comm=%s prev_pid=%u prev_prio=%u prev_state=%u ==> next_comm=%s next_pid=%u next_prio=%u\r\n",
		   packet->ts.tv_sec, packet->ts.tv_usec,
		   packet->pid,
		   (char)packet->event_type,
		   packet->msg.sched_msg.prev_comm,
		   packet->msg.sched_msg.prev_pid,
		   packet->msg.sched_msg.prev_prio,
		   packet->msg.sched_msg.prev_state,
		   packet->msg.sched_msg.next_comm,
		   packet->msg.sched_msg.next_pid,
		   packet->msg.sched_msg.next_prio);
	return sizeof(struct trace_packet);
}

static int print_packet(struct trace_packet *packet)
{
	int isSched = (packet->event_type == 's') ? 1 : 0;
	int isUnique = packet->codelen & TTRACE_CODE_UNIQUE;

	if (isSched) {
		return print_sched_packet(packet);
	} else if (isUnique) {
		return print_uid_packet(packet);
	} else {
		return print_message_packet(packet);
	}
}

static void show_help()
{
	printf("usage: ttrace [opions] [tags...]\r\n");
	printf("options include:\r\n");
	printf("    -s     Start tracing, You should specify tags at tail\r\n");
	printf("    -o     Enable overwrite buffer, This should be used with -s\r\n");
	printf("    -f     Finish tracing and print result\r\n");
	printf("    -i     Show information(state, available/selected/TP used tags, bufsize)\r\n");
	printf("    -d     Dump trace buffer, It should be run after finish\r\n");
	printf("    -p     Print trace buffer, It should be run after finish\r\n");
}

static int assign_tag(char *name)
{
	int i = 0;
	int tags = 0;
	int len_tags = sizeof(ttrace_tags) / sizeof(struct tag_list);
	for (i = 0; i < len_tags; i++)
		if (strncmp(name, ttrace_tags[i].name, MAX_TAG_NAMESIZE) == 0) {
			tags = ttrace_tags[i].tags;
		}
	return tags;
}

static int parse_args(int argc, char **args)
{
	int cmd = 0;
	int ret = 0;
	int i = 0;
	is_overwritable = 0;

	/* options:
	 * -s : TTRACE_START, start tracing
	 * -o : TTRACE_OVERWRITE, enable overwrite buffer
	 * -f : TTRACE_FINISH, finish tracing
	 * -i : TTRACE_INFO, print information(state, bufsize, available tags)
	 * -b : TTRACE_BUFFER, set buf size with argument
	 * -t : TTRACE_SELECTED_TAG, select tags(hidden to user)
	 * -g : TTRACE_FUNC_TAG, TP's tag(hidden to user)
	 * -d : TTRACE_DUMP, dump mode(hang), It should be run after finish.
	 * -p : TTRACE_PRINT, print traces, It should be run after finish.
	 */
	while (1) {
		optarg = NULL;
		ret = getopt(argc, args, "sofidpb:");
		if (ret == '?') {
			show_help();
			return TTRACE_INVALID;
		}

		if (ret < 0) {
			break;
		}

		if (ret == 'o') {
			is_overwritable = 1;
			continue;
		}

		cmd = ret;
		printf("cmd: %d, %c, optarg: %d, %c, %s\r\n", cmd, cmd, optarg, optarg, optarg);

		if (optarg != NULL) {
			param = atoi(optarg);
		}
	}
	for (i = optind; i < argc; i++) {
		printf("args[%d], %s\r\n", i, args[i]);
		// Add args[i] to tag list
		selected_tags |= assign_tag(args[i]);
	}
	printf("selected tags: %d\r\n", selected_tags);
	return cmd;
}

static int check_args_validation(int argc, char **args)
{
	ttdbg("argc: %d, args[1]: %s\r\n", argc, args[1]);
	if (argc == 1 || (strncmp(args[1], "--help", strlen("--help") + 1) == 0)) {
		show_help();
		return TTRACE_INVALID;
	}
	return TTRACE_VALID;
}

static int run_cmd(FILE *fp, int cmd, int arg)
{
	int ret = ioctl(fp->fs_fd, cmd, (unsigned long)arg);
	return ret;
}

static FILE *open_ttrace(char *path)
{
	FILE *file = fopen(path, "r+");
	if (file == NULL) {
		printf("Failed to open : %s\r\n", path);
	}
	return file;
}

static char *alloc_tracebuffer(int size)
{
	char *bp = (char *)zalloc(size * sizeof(char));
	if (bp == NULL) {
		printf("Failed to zalloc buffer in ttrace\r\n");
	}
	return bp;
}

static void close_ttrace(FILE *file)
{
	if (file == NULL) {
		printf("file pointer doesn't opened\r\n");
		return;
	}
	fclose(file);
	return;
}

static void free_tracebuffer(char *buffer)
{
	if (buffer == NULL) {
		printf("tracebuffer dosen't allocated\r\n");
		return;
	}
	free(buffer);
	return;
}

static int read_tracebuffer(FILE *file, int bufsize)
{
	char *buffer = NULL;
	int read_len = 0;
	int offset = 0;

	buffer = alloc_tracebuffer(bufsize);
	if (buffer == NULL) {
		return TTRACE_INVALID;
	}

	read_len = fread(buffer, sizeof(char), bufsize, file);
	if (read_len < 0) {
		return TTRACE_INVALID;
	}

	while (offset < read_len) {
		offset += print_packet((struct trace_packet *)(buffer + offset));
	}

	free_tracebuffer(buffer);
	return TTRACE_VALID;
}

void wait_ttrace_dump()
{
	int i = 0;
	/* Actually, gdb set breakpoints wait_ttrace_dump,
	 * Below block will not be excuted.
	 */
	while (i < 1) {
		usleep(1000000);
		printf(".");
	}
}

static int send_cmds(FILE *file, int cmd)
{
	int ret = 0;
	int bufsize = 0;

	if (cmd == TTRACE_START) {
		ret = run_cmd(file, TTRACE_SELECTED_TAG, selected_tags);
		ret = run_cmd(file, TTRACE_OVERWRITE, is_overwritable);
		ret = run_cmd(file, TTRACE_SET_BUFSIZE, (unsigned long)sizeof(struct trace_packet));
	} else if (cmd == TTRACE_FINISH) {
		ret = run_cmd(file, TTRACE_OVERWRITE, 0);
		bufsize = run_cmd(file, TTRACE_USED_BUFSIZE, param);
	} else if (cmd == TTRACE_PRINT) {
		bufsize = run_cmd(file, TTRACE_USED_BUFSIZE, param);
		if (bufsize <= 0) {
			return TTRACE_NODATA;
		}
		ret = read_tracebuffer(file, bufsize);
		return ret;
	}

	if (run_cmd(file, cmd, param) == TTRACE_INVALID) {
		return TTRACE_INVALID;
	}

	return ret;
}

int kdbg_ttrace(int argc, char **args)
{
	FILE *file = NULL;
	int cmd = 0;
	int ret = 0;

	if (TTRACE_INVALID == check_args_validation(argc, args)) {
		return TTRACE_INVALID;
	}

	selected_tags = 0;
	cmd = parse_args(argc, args);
	if (cmd <= TTRACE_INVALID) {
		return TTRACE_INVALID;
	}
	file = open_ttrace(CONFIG_TTRACE_DEVPATH);
	if (file == NULL) {
		return TTRACE_INVALID;
	}

	if (cmd == TTRACE_DUMP) {
		printf("Dump mode(target hanged),\r\n");
		printf("To dump ttrace logs,\r\n");
		printf("1.connect target\r\n");
		printf("2.run 'ttrace_ttraceDump.py' at host pc.\r\n");
		printf("3.start tracing at target($ ttrace -s <tags>)\r\n");
		printf("4.stop tracing and run dumpmode($ ttrace -f; ttrace -d)\r\n");
		wait_ttrace_dump();
	} else {
		ret = send_cmds(file, cmd);
		if (ret == TTRACE_NODATA) {
			printf("There are no data in buffer.\r\n");
		}
	}

	close_ttrace(file);

	return OK;
}

