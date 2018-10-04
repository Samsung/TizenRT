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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>

#define CODE_VARIABLE        0
#define CODE_UNIQUE         (1 << 7)

#define MSG_BYTES            32
#define COMM_BYTES           12
#define BYTE_ALIGN           4

#define INVALID -1
#define VALID 0

#define packed_struct __attribute__ ((packed))

int param = 0;

struct sched_message {
	short prev_pid;
	unsigned char prev_prio;
	unsigned char prev_state;
	char prev_comm[COMM_BYTES];
	short next_pid;
	unsigned char next_prio;
	char pad;
	char next_comm[COMM_BYTES];
};

union trace_message {
	char message[MSG_BYTES];	// 24B, message(192b)
	struct sched_message sched_msg;
};

struct trace_packet {			// total 40 byte(message), 16byte(uid)
	int tv_sec;
	int tv_nsec;
	short pid;					// 2B, int16_t(16b)
	char event_type;			// 1B, char(8b)
	unsigned char codelen;		// 1B, code(1b) + variable length(7b) or uid(7b)
	int pad;
	union trace_message msg;
};

static int print_uid_packet(struct trace_packet *packet)
{
	unsigned char uid = packet->codelen & (unsigned char)(~CODE_UNIQUE);
	printf("[%06d:%06d] %03u: %c|%u\r\n", packet->tv_sec, packet->tv_nsec / 1000, (unsigned int)packet->pid, (unsigned int)packet->event_type, uid);
	return sizeof(struct trace_packet) - MSG_BYTES;
}

static int print_message_packet(struct trace_packet *packet)
{
	printf("[%06d:%06d] %03u: %c|%s\r\n", packet->tv_sec, packet->tv_nsec / 1000, (unsigned int)packet->pid, packet->event_type, packet->msg.message);
	return sizeof(struct trace_packet);
}

static int print_sched_packet(struct trace_packet *packet)
{
	printf("[%06d:%06d] %03u: %c|prev_comm=%s prev_pid=%u prev_prio=%u prev_state=%u ==> next_comm=%s next_pid=%u next_prio=%u\r\n", packet->tv_sec, packet->tv_nsec / 1000, (unsigned int)packet->pid, packet->event_type, packet->msg.sched_msg.prev_comm, (unsigned int)packet->msg.sched_msg.prev_pid, (unsigned int)packet->msg.sched_msg.prev_prio, (unsigned int)packet->msg.sched_msg.prev_state, packet->msg.sched_msg.next_comm, (unsigned int)packet->msg.sched_msg.next_pid, (unsigned int)packet->msg.sched_msg.next_prio);
	return sizeof(struct trace_packet);
}

static int print_packet(struct trace_packet *packet)
{
	int isSched = (packet->event_type == 's') ? 1 : 0;
	int isUnique = (packet->codelen & CODE_UNIQUE);

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
	printf("usage: parse_dump [dumpfile]\r\n");
	printf("example: ./parse_dump ramdump\r\n");
}

static int check_args_validation(int argc, char **args)
{
	int i = 0;
	if (argc != 2) {
		show_help();
		return INVALID;
	}

	return VALID;
}

static int get_dumpsize(FILE *file)
{
	int size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

static FILE *open_dump(char *path)
{
	FILE *file = fopen(path, "r+");
	if (file == NULL) {
		printf("Failed to open : %s\r\n", path);
	}
	return file;
}

static char *alloc_tracebuffer(int size)
{
	char *bp = (char *)malloc(size * sizeof(char));
	if (bp == NULL) {
		printf("Failed to malloc buffer in ttrace\r\n");
	}
	return bp;
}

static void close_dump(FILE *file)
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
	int read_len = 0, offset = 0;

	buffer = alloc_tracebuffer(bufsize);
	if (buffer == NULL) {
		return INVALID;
	}

	read_len = fread(buffer, sizeof(char), bufsize, file);
	if (read_len < 0) {
		free_tracebuffer(buffer);
		return INVALID;
	}

	while (offset < read_len) {
		offset += print_packet((struct trace_packet *)(buffer + offset));
	}

	free_tracebuffer(buffer);
	return VALID;
}

int main(int argc, char **args)
{
	FILE *file;
	int filesize, ret;

	if (INVALID == check_args_validation(argc, args)) {
		return INVALID;
	}

	file = open_dump(args[1]);
	if (file == NULL) {
		return INVALID;
	}

	filesize = get_dumpsize(file);
	if (filesize <= 0) {
		close_dump(file);
		return INVALID;
	}

	ret = read_tracebuffer(file, filesize);

	close_dump(file);
	return ret;
}
