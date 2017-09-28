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
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/clock.h>
#include <tinyara/ttrace.h>
#include <tinyara/sched.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define TTRACE_EVENT_TYPE_BEGIN    'b'
#define TTRACE_EVENT_TYPE_END      'e'
#define TTRACE_EVENT_TYPE_SCHED    's'

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
static int is_fd_available(void)
{
	if (fd < 0) {
		fd = open("/dev/ttrace", O_WRONLY);
	}

	return fd;
}

static bool is_tag_available(int tag)
{
	int ret;

	ret = ioctl(fd, TTRACE_FUNC_TAG, tag);
	if (!(ret & tag)) {
		return false;
	}

	return true;
}

#ifdef CONFIG_DEBUG_TTRACE
static void show_packet(struct trace_packet *packet)
{
	int uid;
	int size;

	if (packet->event_type == TTRACE_EVENT_TYPE_SCHED) {
		ttdbg("[%06d:%06d] %03d: %c|prev_comm=%s prev_pid=%u prev_prio=%u prev_state=%u ==> next_comm=%s next_pid=%u next_prio=%u\r\n",
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
		size = sizeof(struct trace_packet);
	} else {
		ttdbg("time: %06d.%06d\r\n", packet->ts.tv_sec, packet->ts.tv_usec);
		ttdbg("event_type: %c, %d\r\n", packet->event_type, packet->event_type);
		ttdbg("pid: %d\r\n", packet->pid);
		ttdbg("codelen: %d\r\n", packet->codelen);

		uid = (packet->codelen & TTRACE_CODE_UNIQUE) >> 7;
		ttdbg("unique code? %d\r\n", uid);
		if (uid == TRUE) {
			ttdbg("uid: %d\r\n", (packet->codelen & ~TTRACE_CODE_UNIQUE));
			size = sizeof(struct trace_packet) - TTRACE_MSG_BYTES;
		} else {
			ttdbg("message: %s\r\n", packet->msg.message);
			size = sizeof(struct trace_packet);
		}
	}

	ttdbg("packet size: %d\r\n", size);
}
#endif

static int send_packet_sched(struct trace_packet *packet)
{
	int ret = 0;
	ret = write(fd, packet, sizeof(struct trace_packet));
	return ret;
}

static int create_packet_sched(struct trace_packet *packet, struct tcb_s *prev, struct tcb_s *next)
{
	int ret = TTRACE_VALID;
	int msg_len = sizeof(struct sched_message);

	gettimeofday(&(packet->ts), NULL);
	packet->event_type = TTRACE_EVENT_TYPE_SCHED;
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_VARIABLE | msg_len;

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

	packet->msg.sched_msg.prev_comm[TTRACE_COMM_BYTES - 1] = '\0';
	packet->msg.sched_msg.next_comm[TTRACE_COMM_BYTES - 1] = '\0';

	return ret;
}

static int send_packet(struct trace_packet *packet)
{
	int ret = 0;

	if (packet->codelen & TTRACE_CODE_UNIQUE) {
		ret = write(fd, packet, sizeof(struct trace_packet) - TTRACE_MSG_BYTES);
	} else {
		ret = write(fd, packet, sizeof(struct trace_packet));
	}

	return ret;
}

static int create_packet(struct trace_packet *packet, char type, char *str, va_list valist)
{
	int ret = TTRACE_VALID;
	int msg_len = strlen(str);
	int div = msg_len / TTRACE_BYTE_ALIGN;
	msg_len = div * TTRACE_BYTE_ALIGN + TTRACE_BYTE_ALIGN;
	if (msg_len > TTRACE_MSG_BYTES) {
		msg_len = TTRACE_MSG_BYTES;
	}

	gettimeofday(&(packet->ts), NULL);
	packet->event_type = (int8_t)type;
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_VARIABLE | msg_len;

	vsnprintf(packet->msg.message, msg_len, str, valist);

	return ret;
}

static int create_packet_uid(struct trace_packet *packet, char type, int8_t uniqueid)
{
	int ret = 0;
	gettimeofday(&(packet->ts), NULL);
	packet->event_type = type;
	packet->pid = getpid();
	packet->codelen = TTRACE_CODE_UNIQUE | uniqueid;

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

	if (is_fd_available() < 0 || !is_tag_available(tag)) {
		return TTRACE_INVALID;
	}

	ret = create_packet_sched(&packet, prev_tcb, next_tcb);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

#ifdef CONFIG_DEBUG_TTRACE
	show_packet(&packet);
#endif

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

	if (is_fd_available() < 0 || !is_tag_available(tag)) {
		return TTRACE_INVALID;
	}

	va_start(ap, str);
	ret = create_packet(&packet, TTRACE_EVENT_TYPE_BEGIN, str, ap);
	va_end(ap);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

#ifdef CONFIG_DEBUG_TTRACE
	show_packet(&packet);
#endif

	ret = send_packet(&packet);

	return ret;
}

int trace_begin_uid(int tag, int8_t uniqueid)
{
	int ret = TTRACE_VALID;
	struct trace_packet packet;

	if (is_fd_available() < 0 || !is_tag_available(tag)) {
		return TTRACE_INVALID;
	}

	ret = create_packet_uid(&packet, TTRACE_EVENT_TYPE_BEGIN, uniqueid);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

#ifdef CONFIG_DEBUG_TTRACE
	show_packet(&packet);
#endif

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

	if (is_fd_available() < 0 || !is_tag_available(tag)) {
		return TTRACE_INVALID;
	}

	ret = create_packet_uid(&packet, TTRACE_EVENT_TYPE_END, 0);
	if (ret == TTRACE_INVALID) {
		assert(0);
	}

#ifdef CONFIG_DEBUG_TTRACE
	show_packet(&packet);
#endif

	ret = send_packet(&packet);

	return ret;
}

int trace_end_uid(int tag)
{
	return trace_end(tag);
}

