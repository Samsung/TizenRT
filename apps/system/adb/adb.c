/* Copyright 2018 Byoungtae Cho <bt.cho@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <tinyara/config.h>
#include <tinyara/version.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#include "net.h"
#include "adb.h"
#include "file.h"
#include "arbiter.h"

#define PROPERTY_VALUE_MAX 92
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static aconnection connection;
static atransport *net;
#ifdef ADB_DEBUG
static sem_t dbgsem;
#endif
#ifdef ADB_USE_POLLWORKER
static mqd_t mq;				/* Message queue for receiving messages */
#endif

static bool run;

#ifdef ADB_DEBUG
void dbgmsg(const char *func, int line, char *fmt, ...)
{
	va_list ap;

	sem_wait(&dbgsem);

	va_start(ap, fmt);
	fprintf(stderr, "[%s:%d] ", func, line);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	sem_post(&dbgsem);
}
#endif

aconnection *getconn(void)
{
	return &connection;
}

static int adb_initnet(void)
{
	int fd;
	atransport *t;

	fd = net_init();

	t = (atransport *)malloc(sizeof(atransport));
	if (t == NULL) {
		D("failed to allocate an transport");
		return -ENOMEM;
	}

	memset(t, 0, sizeof(atransport));

	t->fd = fd;
	t->read = net_read;
	t->write = net_write;
	t->type = kTransportNet;

	net = t;

	return OK;
}

/*
 * Adds a new ADB connection.
 */
static aconnection *setconn(const char *name, int remote)
{
	/* Allocate a new ADB connection object */

	aconnection *conn = getconn();

	strcpy(conn->name, name);

	conn->local++;
	conn->remote = remote;
	conn->status = ADB_OPEN;
	conn->execute = NULL;

	return conn;
}

static apacket *get_apacket(void)
{
	apacket *p = (apacket *)(malloc(sizeof(apacket)));

	if (p == NULL) {
		D("failed to allocate an apacket");
		return NULL;
	}

	memset(p, 0, sizeof(apacket) - MAX_PAYLOAD);

	return p;
}

static void put_apacket(apacket *p)
{
	free(p);
}

static int check_header(apacket *p)
{
	if (p->msg.magic != (p->msg.command ^ 0xffffffff)) {
		D("check_header(): invalid magic\n");
		return -1;
	}

	if (p->msg.data_length > MAX_PAYLOAD) {
		D("check_header(): %d > MAX_PAYLOAD\n", p->msg.data_length);
		return -1;
	}

	return 0;
}

static int check_data(apacket *p)
{
	unsigned count, sum;
	unsigned char *x;

	count = p->msg.data_length;
	x = p->data;
	sum = 0;

	while (count-- > 0) {
		sum += *x++;
	}

	if (sum != p->msg.data_check) {
		return -1;
	} else {
		return 0;
	}
}

static void dump_packet(const char *func, apacket *p)
{
#ifdef ADB_DEBUG
	unsigned command = p->msg.command;
	int len = p->msg.data_length;
	char cmd[9];
	char arg0[12], arg1[12];
	int n;

	for (n = 0; n < 4; n++) {
		int b = (command >> (n*8)) & 255;

		if (b < 32 || b >= 127)
			break;
		cmd[n] = (char)b;
	}
	if (n == 4) {
		cmd[4] = 0;
	} else {
		/* There is some non-ASCII name in the command, so dump
		 * the hexadecimal value instead
		 */
		snprintf(cmd, sizeof(cmd), "%08x", command);
	}

	if (p->msg.arg0 < 256U)
		snprintf(arg0, sizeof(arg0), "%d", p->msg.arg0);
	else
		snprintf(arg0, sizeof(arg0), "0x%x", p->msg.arg0);

	if (p->msg.arg1 < 256U)
		snprintf(arg1, sizeof(arg1), "%d", p->msg.arg1);
	else
		snprintf(arg1, sizeof(arg1), "0x%x", p->msg.arg1);
	D("%s: [%s] arg0=%s arg1=%s (len=%d)\n",
		func, cmd, arg0, arg1, len);
#endif
}

#define DUMPMAX 32
void print_packet(const char *label, apacket *p)
{
#ifdef ADB_DEBUG
	char *tag;
	char *x;
	unsigned count;

	switch (p->msg.command) {
	case A_SYNC:
		tag = "SYNC";
		break;
	case A_CNXN:
		tag = "CNXN";
		break;
	case A_OPEN:
		tag = "OPEN";
		break;
	case A_OKAY:
		tag = "OKAY";
		break;
	case A_CLSE:
		tag = "CLSE";
		break;
	case A_WRTE:
		tag = "WRTE";
		break;
	case A_AUTH:
		tag = "AUTH";
		break;
	default:
		tag = "????";
		break;
	}

	fprintf(stderr, "%s: %s %08x %08x %04x \"",
			label, tag, p->msg.arg0, p->msg.arg1, p->msg.data_length);
	count = p->msg.data_length;
	x = (char *) p->data;
	if (count > DUMPMAX) {
		count = DUMPMAX;
		tag = "\n";
	} else {
		tag = "\"\n";
	}
	while (count-- > 0) {
		if ((*x >= ' ') && (*x < 127)) {
			fputc(*x, stderr);
		} else {
			fputc('.', stderr);
		}
		x++;
	}
	fputs(tag, stderr);
#endif
}

/* qual_overwrite is used to overwrite a qualifier string. dst is a
 * pointer to a char pointer. It is assumed that if *dst is non-NULL, it
 * was malloc'ed and needs to freed. *dst will be set to a dup of src.
 */
static void qual_overwrite(char **dst, const char *src)
{
	if (!dst)
		return;

	free(*dst);
	*dst = NULL;

	if (!src || !*src)
		return;

	*dst = strdup(src);
}

int property_get(const char *key, char *value, const char *default_value)
{
	static const char *cnxn_props[] = {
		"ro.product.name",
		"ro.product.model",
		"ro.product.device",
	};

	if (strcmp(key, cnxn_props[0]) == 0) {
		strcpy(value, "ARTIK");
	}
	if (strcmp(key, cnxn_props[1]) == 0) {
		strcpy(value, CONFIG_VERSION_BOARD);
	}
	if (strcmp(key, cnxn_props[2]) == 0) {
		strcpy(value, CONFIG_VERSION_STRING);
	}
	return 0;
}

static size_t fill_connect_data(char *buf, size_t bufsize)
{
	static const char *cnxn_props[] = {
		"ro.product.name",
		"ro.product.model",
		"ro.product.device",
	};
	const char *adb_device_banner = "device";
	static const int num_cnxn_props = ARRAY_SIZE(cnxn_props);
	int i;
	size_t remaining = bufsize;
	size_t len;

	len = snprintf(buf, remaining, "%s::", adb_device_banner);
	remaining -= len;
	buf += len;
	for (i = 0; i < num_cnxn_props; i++) {
		char value[PROPERTY_VALUE_MAX];

		property_get(cnxn_props[i], value, "");
		len = snprintf(buf, remaining, "%s=%s;", cnxn_props[i], value);
		remaining -= len;
		buf += len;
	}

	return bufsize - remaining + 1;
}

static int adb_read(apacket *p, atransport *t)
{
	if (t->read(t->fd, sizeof(amessage), (uint8_t *)&p->msg) < 0) {
		D("read terminated (message)\n");
		return -1;
	}

	if (check_header(p)) {
		D("check_header failed\n");
		return -1;
	}

	if (p->msg.data_length) {
		if (t->read(t->fd, p->msg.data_length, p->data) < 0) {
			D("terminated (data)\n");
			return -1;
		}
	}

	if (check_data(p)) {
		D("check_data failed\n");
		return -1;
	}

	return 0;
}

static int adb_write(apacket *ppacket, atransport *t)
{
	uint8_t *p = (uint8_t *) ppacket; /* we really write the packet address */
	int r, len;

	dump_packet("to remote", ppacket);

	len = sizeof(ppacket->msg) + ppacket->msg.data_length;
	D("%d send\n", len);
	while (len > 0) {
		r = t->write(t->fd, len, p);
		if (r > 0) {
			len -= r;
			p += r;
		} else {
			D("(fd=%d) error ret=%d errno=%d: %s\n",
				t->fd, r, errno, strerror(errno));
			if ((r < 0) && (errno == EINTR)) {
				continue;
			}
			return -1;
		}
	}
	return 0;
}

static int read_packet(apacket *p, atransport *t)
{
	int r;

	while (true) {
		r = adb_read(p, t);
		if (r == 0) {
			break;
		} else {
			D("read_packet (fd=%d), error ret=%d errno=%d: %s\n", t->fd, r, errno, strerror(errno));
			if ((r < 0) && (errno == EINTR)) {
				continue;
			}
			return -1;
		}
	}

	dump_packet("from remote", p);

	return 0;
}

void send_packet(apacket *p, atransport *t)
{
	unsigned char *x;
	unsigned sum;
	unsigned count;

	p->msg.magic = p->msg.command ^ 0xffffffff;

	count = p->msg.data_length;
	x = (unsigned char *) p->data;
	sum = 0;
	while (count-- > 0) {
		sum += *x++;
	}
	p->msg.data_check = sum;

	print_packet("send", p);

	if (adb_write(p, t)) {
		D("cannot enqueue packet on transport socket");
	}
}

static void send_auth_request(atransport *t)
{
	D("Calling send_auth_request\n");
}

static void send_connect(atransport *t)
{
	apacket *p = get_apacket();

	D("Calling send_connect\n");
	p->msg.command = A_CNXN;
	p->msg.arg0 = A_VERSION;
	p->msg.arg1 = MAX_PAYLOAD;
	p->msg.data_length = fill_connect_data((char *)p->data, sizeof(p->data));
	send_packet(p, t);

	put_apacket(p);
}

static void send_ready(aconnection *conn, atransport *t)
{
	apacket *p = get_apacket();

	D("Calling send_ready\n");
	p->msg.command = A_OKAY;
	p->msg.arg0 = conn->local;
	p->msg.arg1 = conn->remote;

	send_packet(p, t);

	put_apacket(p);
}

void send_close(aconnection *conn, atransport *t)
{
	apacket *p = get_apacket();

	D("Calling send_close\n");
	p->msg.command = A_CLSE;
	p->msg.arg0 = conn->local;
	p->msg.arg1 = conn->remote;

	send_packet(p, t);

	put_apacket(p);
}

void send_close2(int remote, atransport *t)
{
	apacket *p = get_apacket();

	D("Calling send_close2\n");
	p->msg.command = A_CLSE;
	p->msg.arg0 = 0;
	p->msg.arg1 = remote;

	send_packet(p, t);

	put_apacket(p);
}

void send_msg(aconnection *conn, atransport *t, uint8_t *data, uint32_t len)
{
	uint32_t count, sum = 0;
	uint8_t *x;

	D("Calling send_msg\n");

	// Calculate data checksum
	count = len;
	x = data;
	while (count-- > 0) {
		sum += *x++;
	}

	apacket *p = get_apacket();

	p->msg.command = A_WRTE;
	p->msg.arg0 = conn->local;
	p->msg.arg1 = conn->remote;
	p->msg.data_length = len;
	p->msg.data_check = sum;
	p->msg.magic = p->msg.command ^ 0xffffffff;
	memcpy(p->data, data, len);

	send_packet(p, t);

	put_apacket(p);
}

static void process_command(aconnection *conn, atransport *t, char *name)
{
	D("process command\n");
	bool mode;

	if (!strncmp(name, "shell:", 6)) {
		char *cmd = (name + 6);

		mode = (*cmd) ? true : false;
		if (mode) {
			conn->status = ADB_COMMAND;
		} else {
			conn->status = ADB_SHELL;
		}

		D("shell mode (%d), cmd=%s\n", mode, cmd);

		if (conn->arbiter == false) {
			conn->arbiter = true;

			run_arbiter(conn, t);
		}

		shell_wakeup();

		conn->execute = execute_command;

		conn->execute(cmd, strlen(cmd));

	} else if (!strncmp(name, "sync:", 5)) {
		conn->status = ADB_FILE;

		if (conn->arbiter == false) {
			conn->arbiter = true;

			run_arbiter(conn, t);
		}

		fsync_wakeup();

		conn->execute = execute_fsync;
	} else {
		D("Unknown command: %s\n", name);
	}
}

static void handle_packet(apacket *p, atransport *t)
{
	int local;
	int remote;
	aconnection *conn;

	D("handle_packet() %c%c%c%c\n", ((char *) (&(p->msg.command)))[0],
			((char *) (&(p->msg.command)))[1],
			((char *) (&(p->msg.command)))[2],
			((char *) (&(p->msg.command)))[3]);
	print_packet("recv", p);

	switch (p->msg.command) {
	case A_CNXN: /* CONNECT(version, maxdata, "system-id-string") */
		conn = &connection;
		if (conn->status != ADB_OFFLINE) {
			D("connection status (%d), this request will be ignored\n",
					conn->status);
			break;
		}

		if (!conn->auth) {
			send_connect(t);
		} else {
			send_auth_request(t);
		}

		conn->status = ADB_CLOSED;
		break;

	case A_OPEN: /* OPEN(local-id, 0, "destination") */
		conn = getconn();

		if (conn->status != ADB_CLOSED) {
			D("local(%d) <-> remote(%d) still alive!!\n",
					conn->local, conn->remote);
			remote = p->msg.arg0;

			D("(%s) be rejected!", (char *)p->data);

			send_close2(remote, t);
			break;
		}

		if (p->msg.arg0 != 0 && p->msg.arg1 == 0) {
			char *name = (char *) p->data;

			remote = p->msg.arg0;

			name[p->msg.data_length > 0 ? p->msg.data_length - 1 : 0] = 0;
			D("name: %s\n", name);

			conn = setconn(name, remote);

			D("local(%d) <-> remote(%d)\n", conn->local, conn->remote);

			send_ready(conn, t);

			process_command(conn, t, name);
		}
		break;

	case A_OKAY: /* READY(local-id, remote-id, "") */
		local = p->msg.arg1;
		remote = p->msg.arg0;

		D("local(%d) <-> remote(%d)\n", local, remote);

		conn = getconn();
		if (conn->local == local && conn->remote == remote) {
			send_ready(conn, t);
		} else {
			D("Invalid A_OKAY(%d,%d) on transport\n", local, remote);
		}
		break;

	case A_CLSE: /* CLOSE(local-id, remote-id, "") or CLOSE(0, remote-id, "") */
		local = p->msg.arg1;
		remote = p->msg.arg0;

		D("local(%d) <-> remote(%d)\n", local, remote);

		conn = getconn();
		if (conn->local == local && conn->remote == remote) {
			conn->arbiter = false;
			stop_arbiter();
			conn->status = ADB_CLOSED;
		} else {
			D("Invalid A_CLSE(%d,%d) on transport\n", local, remote);
		}
		break;

	case A_WRTE: /* WRITE(local-id, remote-id, <data>) */
		local = p->msg.arg1;
		remote = p->msg.arg0;

		D("local(%d) <-> remote(%d)\n", local, remote);

		conn = getconn();
		if (conn->local == local && conn->remote == remote) {
			conn->execute((char *)(p->data), p->msg.data_length);
			send_ready(conn, t);
		} else {
			D("Invalid A_WRTE(%d,%d) on transport\n", local, remote);
		}
		break;

	default:
		printf("handle_packet: what is %08x?!\n", p->msg.command);
	}

	put_apacket(p);
}

#ifdef ADB_USE_POLLWORKER
static pthread_addr_t adb_worker(void *param)
{
	int r;
	int prio;
	apacket *p;
	atransport *t = net;
	amsg msg;

	D("worker is created!\n");

	while (true) {
		D("wait msg!\n");
		r = mq_receive(mq, (char *)&msg, sizeof(msg), &prio);

		/* Handle the case when we return with no message */

		if (r < sizeof(msg)) {
			D("ERROR: Message too small: %d\n", r);
			continue;
		}

		D("MSGQ RECV id=%d, addr=%p\n", msg.msgId, msg.p);

		p = (apacket *)msg.p;

		handle_packet(p, t);
	}

	return 0;
}

static int adb_initworker(void)
{
	int r;
	pthread_t tid;
	pthread_attr_t attr;
	struct mq_attr mq_attr;
	char *name = "/dev/adb_mq";

	pthread_attr_init(&attr);

	mq_attr.mq_maxmsg = 32;
	mq_attr.mq_msgsize = sizeof(amsg);
	mq_attr.mq_curmsgs = 0;
	mq_attr.mq_flags = 0;

	mq = mq_open(name, O_RDWR | O_CREAT, 0644, &mq_attr);
	if (mq == NULL) {
		/* Error creating message queue! */

		D("Couldn't allocate message queue\n");
		return -ENOMEM;
	}

	pthread_attr_setstacksize(&attr, CONFIG_IDLETHREAD_STACKSIZE);

	r = pthread_create(&tid, &attr, adb_worker, NULL);
	if (r != OK) {
		D("pthread_create failed\n");
		mq_close(mq);
		mq_unlink(name);
		return -ECHILD;
	}

	pthread_setname_np(tid, "adb_worker");

	return 0;
}
#endif

static void adb_poll(void)
{
	apacket *p;
	atransport *t = net;
	int sockfd;
#ifdef ADB_USE_POLLWORKER
	int r;
	uint16_t idx = 0;
	amsg msg;
#endif
	aconnection *conn;

	sockfd = t->fd;

	D("server fd=%d\n", sockfd);

	while (true) {
		do {
			t->fd = net_wait(sockfd);
		} while (t->fd <= 0);

		D("client fd=%d\n", t->fd);

		while (true) {
			p = get_apacket();

			if (read_packet(p, t)) {
				put_apacket(p);
				D("failed to read packet from fd %d\n", t->fd);

				conn = getconn();
				conn->status = ADB_OFFLINE;

				break;
			} else {
#ifdef ADB_USE_POLLWORKER
				msg.msgId = idx++;
				msg.p = (void *)p;

				D("MSGQ SEND id=%d, addr=%p\n", msg.msgId, msg.p);

				r = mq_send(mq, (const char *)&msg, sizeof(msg), 1);
				if (r < 0) {
					D("ERROR: mq_send failed: %d\n", errno);
				}
#else
				handle_packet(p, t);
#endif
			}
		}
	}
}

/*
 * Initialises the ADB protocol. This function initialises the USB layer
 * underneath so no further setup is required.
 */
void adb_init(void)
{
	aconnection *conn = getconn();

	memset((void *)conn, 0, sizeof(aconnection));
	conn->local = 1;

#ifdef ADB_DEBUG
	sem_init(&dbgsem, 0, 1);
	sem_setprotocol(&dbgsem, SEM_PRIO_NONE);
#endif

	adb_initnet();
#ifdef ADB_USE_POLLWORKER
	adb_initworker();
#endif
	adb_initarbiter();
	adb_initshell();
	adb_initfile();
}

int adb_main(int argc, char **argv)
{
	if (run == false) {
		run = true;

		adb_init();
		adb_poll();
	}

	return 0;
}
