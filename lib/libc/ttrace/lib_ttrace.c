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
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ttrace.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/clock.h>
#include <tinyara/ttrace_internal.h>
#include <tinyara/sched.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Constant Data
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
int fd = -1;

/****************************************************************************
 * Private Constant Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
int is_fd_available(void)
{
	if (fd < 0) {
		fd = open("/dev/ttrace", O_WRONLY);
	}

	return fd;
}

int is_tag_available(int tag)
{
	return ioctl(fd, TTRACE_FUNC_TAG, tag);
}

int send_packet_sched(struct trace_packet *packet)
{
	int ret = 0;
	ret = write(fd, packet, sizeof(struct trace_packet));
	return ret;
}

int create_packet_sched(struct trace_packet *packet, struct tcb_s *prev, struct tcb_s *next)
{
	int ret = TTRACE_VALID;
	int msg_len = sizeof(struct sched_message);

	gettimeofday(&(packet->ts));
	packet->event_type = (int8_t)'s';
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_VARIABLE | msg_len;
	packet->pad = -1;

	packet->msg.sched_msg.pad = -1;

	if (prev != NULL) {
		memcpy(packet->msg.sched_msg.prev_comm, prev->name, TTRACE_COMM_BYTES);
		packet->msg.sched_msg.prev_pid = prev->pid;
		packet->msg.sched_msg.prev_prio = prev->sched_priority;
		packet->msg.sched_msg.prev_state = prev->task_state;
	} else {
		memcpy(packet->msg.sched_msg.prev_comm, "Idle Task", TTRACE_COMM_BYTES);
		packet->msg.sched_msg.prev_pid = 0;
		packet->msg.sched_msg.prev_prio = 0;
		packet->msg.sched_msg.prev_state = 3;
	}

	if (next != NULL) {
		memcpy(packet->msg.sched_msg.next_comm, next->name, TTRACE_COMM_BYTES);
		packet->msg.sched_msg.next_pid = next->pid;
		packet->msg.sched_msg.next_prio = next->sched_priority;
	} else {
		memcpy(packet->msg.sched_msg.next_comm, "Idle Task", TTRACE_COMM_BYTES);
		packet->msg.sched_msg.next_pid = 0;
		packet->msg.sched_msg.next_prio = 0;
	}

	return ret;
}

int send_packet(struct trace_packet *packet)
{
	int ret = 0;

	if (packet->codelen & TTRACE_CODE_UNIQUE) {
		ret = write(fd, packet, sizeof(struct trace_packet) - TTRACE_MSG_BYTES);
	} else {
		ret = write(fd, packet, sizeof(struct trace_packet));
	}

	return ret;
}

int create_packet(struct trace_packet *packet, char type, char *str, va_list valist)
{
	int ret = TTRACE_VALID;
	int msg_len = strlen(str);
	int div = msg_len / TTRACE_BYTE_ALIGN;
	msg_len = div * TTRACE_BYTE_ALIGN + TTRACE_BYTE_ALIGN;
	if (msg_len > TTRACE_MSG_BYTES) {
		msg_len = TTRACE_MSG_BYTES;
	}

	gettimeofday(&(packet->ts));
	packet->event_type = (int8_t)type;
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_VARIABLE | msg_len;
	packet->pad = -1;

	vsnprintf(packet->msg.message, msg_len, str, valist);

	return ret;
}

int create_packet_u(struct trace_packet *packet, char type, int8_t uid)
{
	int ret = 0;
	gettimeofday(&(packet->ts));
	packet->event_type = type;
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_UNIQUE | uid;
	packet->pad = -1;

	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int trace_sched(struct tcb_s *prev_tcb, struct tcb_s *next_tcb)
{
	int ret = TTRACE_VALID;
	int tag = TTRACE_TAG_TASK;
	struct trace_packet packet;

	if (is_fd_available() < 0) {
		return TTRACE_INVALID;
	}

	if (is_tag_available(tag) == TTRACE_INVALID) {
		return TTRACE_INVALID;
	}

	ret = create_packet_sched(&packet, prev_tcb, next_tcb);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

	//show_sched_packet(&packet);

	ret = send_packet_sched(&packet);
	return ret;

}
/****************************************************************************
 * Name: trace_begin
 *
 * Description:
 *   The function trace_begin() performs the same task as trace(_begin() with
 *   the difference that it takes a set of arguments which have been obtained
 *   using the stdarg variable argument list macros.
 *
 ****************************************************************************/
int trace_begin(int tag, char *str, ...)
{
	int ret = TTRACE_VALID;
	struct trace_packet packet;
	va_list ap;

	if (is_fd_available() < 0) {
		return TTRACE_INVALID;
	}

	if (is_tag_available(tag) == TTRACE_INVALID) {
		return TTRACE_INVALID;
	}

	va_start(ap, str);
	ret = create_packet(&packet, TTRACE_EVENT_TYPE_BEGIN, str, ap);
	va_end(ap);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

	//show_packet(&packet);

	ret = send_packet(&packet);
	return ret;
}

int trace_begin_u(int tag, int8_t uid)
{
	int ret = TTRACE_VALID;
	struct trace_packet packet;

	if (is_fd_available() < 0) {
		return TTRACE_INVALID;
	}

	if (is_tag_available(tag) == TTRACE_INVALID) {
		return TTRACE_INVALID;
	}

	ret = create_packet_u(&packet, TTRACE_EVENT_TYPE_BEGIN, uid);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

	//show_packet(&packet);

	ret = send_packet(&packet);
	return ret;
}

/****************************************************************************
 * Name: trace_end
 *
 * Description:
 *   The function trace_end() performs the same task as trace(_begin() with
 *   the difference that it takes a set of arguments which have been obtained
 *   using the stdarg variable argument list macros.
 *
 ****************************************************************************/

int trace_end(int tag)
{
	int ret = TTRACE_VALID;
	struct trace_packet packet;

	if (is_fd_available() < 0) {
		return TTRACE_INVALID;
	}

	if (is_tag_available(tag) == TTRACE_INVALID) {
		return TTRACE_INVALID;
	}

	ret = create_packet_u(&packet, TTRACE_EVENT_TYPE_END, 0);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

	//show_packet(&packet);

	ret = send_packet(&packet);
	return ret;
}

int trace_end_u(int tag)
{
	return trace_end(tag);
}

