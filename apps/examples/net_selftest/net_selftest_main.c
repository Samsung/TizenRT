/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ST_PORT        7100
#define ST_LOOPBACK    "127.0.0.1"
#define ST_CHUNK       1024
#define ST_NCHUNKS     32
#define ST_UDP_LEN     512
#define ST_UDP_COUNT   10
#define ST_MAX_SOCKETS 64

#define CHECK(cond) \
	do { \
		if (cond) { \
			g_pass++; \
		} else { \
			g_fail++; \
			printf("  FAIL %s:%d (%s) errno %d\n", __func__, __LINE__, #cond, errno); \
		} \
	} while (0)

#define RUN(test) \
	do { \
		int fail = g_fail; \
		printf("[net_selftest] %s\n", #test); \
		test(); \
		printf("[net_selftest] %s: %s\n", #test, fail == g_fail ? "PASS" : "FAIL"); \
	} while (0)

/****************************************************************************
 * Private Data
 ****************************************************************************/

static int g_pass;
static int g_fail;
static uint8_t g_txbuf[4096];
static uint8_t g_rxbuf[4096];
static sem_t g_childsem;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int elapsed_ms(const struct timespec *start)
{
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return (int)((now.tv_sec - start->tv_sec) * 1000 + (now.tv_nsec - start->tv_nsec) / 1000000);
}

static void fill_addr(struct sockaddr_in *addr, const char *ip, int port)
{
	memset(addr, 0, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip);
}

static int tcp_listener(int port, bool reuse)
{
	struct sockaddr_in addr;
	int on = 1;
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0) {
		return -1;
	}

	if (reuse && setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		close(fd);
		return -1;
	}

	fill_addr(&addr, ST_LOOPBACK, port);
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 || listen(fd, 2) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

static int tcp_connect(const char *ip, int port)
{
	struct sockaddr_in addr;
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0) {
		return -1;
	}

	fill_addr(&addr, ip, port);
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

/* A connected pair of TCP sockets over the loopback interface */

static int tcp_pair(int *a, int *b)
{
	int lfd = tcp_listener(ST_PORT, true);

	*a = *b = -1;
	if (lfd < 0) {
		return -1;
	}

	*a = tcp_connect(ST_LOOPBACK, ST_PORT);
	if (*a >= 0) {
		*b = accept(lfd, NULL, NULL);
	}

	close(lfd);
	if (*a < 0 || *b < 0) {
		if (*a >= 0) {
			close(*a);
		}
		*a = *b = -1;
		return -1;
	}

	return 0;
}

static int recv_all(int fd, uint8_t *buf, int len)
{
	int done = 0;

	while (done < len) {
		int ret = recv(fd, buf + done, len - done, 0);
		if (ret <= 0) {
			return ret < 0 ? ret : done;
		}
		done += ret;
	}

	return done;
}

static int send_all(int fd, const uint8_t *buf, int len)
{
	int done = 0;

	while (done < len) {
		int ret = send(fd, buf + done, len - done, 0);
		if (ret <= 0) {
			return -1;
		}
		done += ret;
	}

	return done;
}

static void fill_pattern(uint8_t *buf, int len, int seed)
{
	int i;

	for (i = 0; i < len; i++) {
		buf[i] = (uint8_t)(seed * 31 + i * 7 + (i >> 8));
	}
}

/* Send data in chunks of different sizes and check what the peer echoes */

static void echo_exchange(int fd, int nchunks, int maxlen)
{
	int total = 0;
	int bad = 0;
	int i;

	for (i = 0; i < nchunks; i++) {
		int len = 1 + (i * 977) % maxlen;

		if (i == nchunks - 1) {
			len = maxlen;
		}

		fill_pattern(g_txbuf, len, i);
		if (send_all(fd, g_txbuf, len) != len || recv_all(fd, g_rxbuf, len) != len ||
			memcmp(g_txbuf, g_rxbuf, len) != 0) {
			bad++;
			break;
		}
		total += len;
	}

	CHECK(bad == 0);
	printf("  echoed %d bytes in %d chunks\n", total, i);
}

static void *echo_thread(void *arg)
{
	static uint8_t buf[2048];
	int fd = (int)(intptr_t)arg;
	int ret;

	while ((ret = recv(fd, buf, sizeof(buf), 0)) > 0) {
		if (send_all(fd, buf, ret) != ret) {
			break;
		}
	}

	close(fd);
	return NULL;
}

/****************************************************************************
 * Tests over the loopback interface
 ****************************************************************************/

static void test_tcp_echo(void)
{
	pthread_t tid;
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	CHECK(pthread_create(&tid, NULL, echo_thread, (void *)(intptr_t)b) == 0);
	echo_exchange(a, ST_NCHUNKS, (int)sizeof(g_txbuf));
	CHECK(close(a) == 0);
	CHECK(pthread_join(tid, NULL) == 0);
}

static void test_udp_echo(void)
{
	struct sockaddr_in addr_a;
	struct sockaddr_in addr_b;
	struct sockaddr_in from;
	socklen_t fromlen;
	int a = socket(AF_INET, SOCK_DGRAM, 0);
	int b = socket(AF_INET, SOCK_DGRAM, 0);
	int i;

	CHECK(a >= 0 && b >= 0);
	fill_addr(&addr_a, ST_LOOPBACK, ST_PORT + 1);
	fill_addr(&addr_b, ST_LOOPBACK, ST_PORT + 2);
	CHECK(bind(a, (struct sockaddr *)&addr_a, sizeof(addr_a)) == 0);
	CHECK(bind(b, (struct sockaddr *)&addr_b, sizeof(addr_b)) == 0);

	for (i = 0; i < ST_UDP_COUNT; i++) {
		fill_pattern(g_txbuf, ST_UDP_LEN, i);
		CHECK(sendto(a, g_txbuf, ST_UDP_LEN, 0, (struct sockaddr *)&addr_b, sizeof(addr_b)) == ST_UDP_LEN);

		fromlen = sizeof(from);
		memset(&from, 0, sizeof(from));
		CHECK(recvfrom(b, g_rxbuf, sizeof(g_rxbuf), 0, (struct sockaddr *)&from, &fromlen) == ST_UDP_LEN);
		CHECK(memcmp(g_txbuf, g_rxbuf, ST_UDP_LEN) == 0);
		CHECK(from.sin_port == htons(ST_PORT + 1) && from.sin_addr.s_addr == addr_a.sin_addr.s_addr);

		/* Back to the sender */

		CHECK(sendto(b, g_rxbuf, ST_UDP_LEN, 0, (struct sockaddr *)&from, fromlen) == ST_UDP_LEN);
		CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == ST_UDP_LEN);
	}

	CHECK(close(a) == 0);
	CHECK(close(b) == 0);
}

static void test_names(void)
{
	struct sockaddr_in addr;
	socklen_t len;
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	len = sizeof(addr);
	CHECK(getpeername(a, (struct sockaddr *)&addr, &len) == 0);
	CHECK(addr.sin_port == htons(ST_PORT));
	len = sizeof(addr);
	CHECK(getsockname(b, (struct sockaddr *)&addr, &len) == 0);
	CHECK(addr.sin_port == htons(ST_PORT) && addr.sin_addr.s_addr == inet_addr(ST_LOOPBACK));

	close(a);
	close(b);
}

static void test_read_write(void)
{
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	/* The descriptors of the sockets are descriptors of the VFS */

	CHECK(write(a, "hello", 5) == 5);
	CHECK(read(b, g_rxbuf, sizeof(g_rxbuf)) == 5);
	CHECK(memcmp(g_rxbuf, "hello", 5) == 0);

	close(a);
	close(b);
}

static void test_select(void)
{
	struct timespec start;
	struct timeval tv;
	fd_set rfds;
	fd_set wfds;
	int ms;
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	/* Nothing to read: select() times out */

	FD_ZERO(&rfds);
	FD_SET(a, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 200000;
	clock_gettime(CLOCK_MONOTONIC, &start);
	CHECK(select(a + 1, &rfds, NULL, NULL, &tv) == 0);
	ms = elapsed_ms(&start);
	CHECK(ms >= 150 && ms < 2000);
	CHECK(!FD_ISSET(a, &rfds));

	/* Data makes the socket readable */

	CHECK(send(b, "x", 1, 0) == 1);
	FD_ZERO(&rfds);
	FD_SET(a, &rfds);
	FD_SET(b, &rfds);
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	CHECK(select((a > b ? a : b) + 1, &rfds, NULL, NULL, &tv) == 1);
	CHECK(FD_ISSET(a, &rfds) && !FD_ISSET(b, &rfds));

	/* A connected socket is writable */

	FD_ZERO(&wfds);
	FD_SET(a, &wfds);
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	CHECK(select(a + 1, NULL, &wfds, NULL, &tv) == 1);
	CHECK(FD_ISSET(a, &wfds));

	close(a);
	close(b);
}

static void *delayed_send_thread(void *arg)
{
	int fd = (int)(intptr_t)arg;

	usleep(300 * 1000);
	send(fd, "wake", 4, 0);
	return NULL;
}

static void test_poll(void)
{
	struct timespec start;
	struct pollfd pfd;
	pthread_t tid;
	int lfd;
	int ms;
	int a;
	int b;

	/* A connection makes the listener readable */

	lfd = tcp_listener(ST_PORT, true);
	CHECK(lfd >= 0);
	if (lfd < 0) {
		return;
	}

	pfd.fd = lfd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	CHECK(poll(&pfd, 1, 100) == 0);

	a = tcp_connect(ST_LOOPBACK, ST_PORT);
	CHECK(a >= 0);
	pfd.revents = 0;
	CHECK(poll(&pfd, 1, 2000) == 1);
	CHECK((pfd.revents & POLLIN) != 0);
	b = accept(lfd, NULL, NULL);
	CHECK(b >= 0);
	close(lfd);
	if (a < 0 || b < 0) {
		return;
	}

	/* Nothing to read, but the socket is writable */

	pfd.fd = a;
	pfd.events = POLLIN;
	pfd.revents = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	CHECK(poll(&pfd, 1, 200) == 0);
	ms = elapsed_ms(&start);
	CHECK(ms >= 150 && ms < 2000);

	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	CHECK(poll(&pfd, 1, 2000) == 1);
	CHECK((pfd.revents & (POLLIN | POLLOUT)) == POLLOUT);

	/* Another thread sends while this one waits in poll() */

	CHECK(pthread_create(&tid, NULL, delayed_send_thread, (void *)(intptr_t)b) == 0);
	pfd.events = POLLIN;
	pfd.revents = 0;
	clock_gettime(CLOCK_MONOTONIC, &start);
	CHECK(poll(&pfd, 1, 5000) == 1);
	ms = elapsed_ms(&start);
	CHECK(ms >= 200 && ms < 3000);
	CHECK((pfd.revents & POLLIN) != 0);
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == 4);
	pthread_join(tid, NULL);

	/* The end of the stream is readable */

	close(b);
	pfd.revents = 0;
	CHECK(poll(&pfd, 1, 2000) == 1);
	CHECK((pfd.revents & (POLLIN | POLLHUP)) != 0);
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == 0);

	close(a);
}

static void test_nonblock(void)
{
	struct timespec start;
	struct timeval tv;
	int avail = -1;
	int flags;
	int on;
	int ms;
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	/* fcntl(O_NONBLOCK) */

	flags = fcntl(a, F_GETFL, 0);
	CHECK(flags >= 0 && (flags & O_NONBLOCK) == 0);
	CHECK(fcntl(a, F_SETFL, flags | O_NONBLOCK) == 0);
	CHECK((fcntl(a, F_GETFL, 0) & O_NONBLOCK) != 0);
	errno = 0;
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == -1);
	CHECK(errno == EAGAIN || errno == EWOULDBLOCK);
	CHECK(fcntl(a, F_SETFL, flags) == 0);

	/* ioctl(FIONBIO) */

	on = 1;
	CHECK(ioctl(a, FIONBIO, (unsigned long)&on) == 0);
	errno = 0;
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == -1);
	CHECK(errno == EAGAIN || errno == EWOULDBLOCK);
	on = 0;
	CHECK(ioctl(a, FIONBIO, (unsigned long)&on) == 0);

	/* MSG_DONTWAIT */

	errno = 0;
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), MSG_DONTWAIT) == -1);
	CHECK(errno == EAGAIN || errno == EWOULDBLOCK);

	/* SO_RCVTIMEO */

	tv.tv_sec = 0;
	tv.tv_usec = 200000;
	CHECK(setsockopt(a, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);
	clock_gettime(CLOCK_MONOTONIC, &start);
	errno = 0;
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == -1);
	ms = elapsed_ms(&start);
	CHECK(errno == EAGAIN || errno == EWOULDBLOCK);
	CHECK(ms >= 150 && ms < 2000);

	/* FIONREAD */

	CHECK(send(b, "12345", 5, 0) == 5);
	usleep(100 * 1000);
	CHECK(ioctl(a, FIONREAD, (unsigned long)&avail) == 0);
	CHECK(avail == 5);
	CHECK(recv(a, g_rxbuf, sizeof(g_rxbuf), 0) == 5);

	close(a);
	close(b);
}

struct blocked_recv_s {
	int fd;
	volatile int done;
	int ret;
	int err;
};

static void *blocked_recv_thread(void *arg)
{
	struct blocked_recv_s *br = (struct blocked_recv_s *)arg;
	char c;

	br->ret = recv(br->fd, &c, 1, 0);
	br->err = errno;
	br->done = 1;
	return NULL;
}

static void test_close_during_recv(void)
{
	static struct blocked_recv_s br;
	pthread_t tid;
	int i;
	int a;
	int b;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	br.fd = a;
	br.done = 0;
	CHECK(pthread_create(&tid, NULL, blocked_recv_thread, &br) == 0);
	usleep(300 * 1000);
	CHECK(br.done == 0);

	/* close() of another thread releases the descriptor.  The socket stays
	 * until the blocking recv() returns.
	 */

	CHECK(close(a) == 0);
	usleep(300 * 1000);
	CHECK(br.done == 0);
	errno = 0;
	CHECK(send(a, "x", 1, 0) == -1);
	CHECK(errno == EBADF);

	CHECK(send(b, "z", 1, 0) == 1);
	for (i = 0; i < 30 && !br.done; i++) {
		usleep(100 * 1000);
	}

	CHECK(br.done == 1);
	if (br.done) {
		CHECK(br.ret == 1);
		pthread_join(tid, NULL);

		/* Now the socket is closed */

		CHECK(recv(b, g_rxbuf, sizeof(g_rxbuf), 0) == 0);
	} else {
		pthread_detach(tid);
	}

	close(b);
}

static void test_dup(void)
{
	int a;
	int b;
	int d;
	int e;

	CHECK(tcp_pair(&a, &b) == 0);
	if (a < 0) {
		return;
	}

	/* The duplicate refers to the same socket and keeps it open */

	d = dup(a);
	CHECK(d >= 0 && d != a);
	CHECK(close(a) == 0);
	CHECK(send(d, "dup", 3, 0) == 3);
	CHECK(recv(b, g_rxbuf, sizeof(g_rxbuf), 0) == 3);

	e = dup2(d, d + 5);
	CHECK(e == d + 5);
	CHECK(close(d) == 0);
	CHECK(send(b, "dup2", 4, 0) == 4);
	CHECK(recv(e, g_rxbuf, sizeof(g_rxbuf), 0) == 4);

	/* The last close() closes the connection */

	CHECK(close(e) == 0);
	CHECK(recv(b, g_rxbuf, sizeof(g_rxbuf), 0) == 0);
	close(b);

	errno = 0;
	CHECK(send(e, "x", 1, 0) == -1);
	CHECK(errno == EBADF);
}

static int open_until_full(int *fds, int max)
{
	int n;

	for (n = 0; n < max; n++) {
		fds[n] = socket(AF_INET, SOCK_DGRAM, 0);
		if (fds[n] < 0) {
			break;
		}
	}

	return n;
}

static void test_descriptors(void)
{
	static int fds[ST_MAX_SOCKETS];
	struct sockaddr_in addr;
	int first;
	int second;
	int bad = 0;
	int i;

	/* Sockets that are closed do not leak */

	for (i = 0; i < 200; i++) {
		int t = socket(AF_INET, SOCK_STREAM, 0);
		int u = socket(AF_INET, SOCK_DGRAM, 0);

		fill_addr(&addr, ST_LOOPBACK, ST_PORT + 3);
		if (t < 0 || u < 0 || bind(u, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			bad++;
		}

		if (t >= 0 && close(t) < 0) {
			bad++;
		}

		if (u >= 0 && close(u) < 0) {
			bad++;
		}
	}

	CHECK(bad == 0);

	/* The number of sockets is limited, and all of them come back */

	first = open_until_full(fds, ST_MAX_SOCKETS);
	printf("  %d sockets, then errno %d\n", first, errno);
	CHECK(first > 0 && first < ST_MAX_SOCKETS);
	for (i = 0; i < first; i++) {
		close(fds[i]);
	}

	second = open_until_full(fds, ST_MAX_SOCKETS);
	CHECK(second == first);
	for (i = 0; i < second; i++) {
		close(fds[i]);
	}

	/* Not a socket and not a descriptor */

	errno = 0;
	CHECK(send(-1, "x", 1, 0) == -1);
	CHECK(errno == EBADF);
	errno = 0;
	CHECK(getsockname(1, (struct sockaddr *)&addr, &(socklen_t){sizeof(addr)}) == -1);
	CHECK(errno == ENOTSOCK || errno == EBADF);
}

static int leaking_child(int argc, char *argv[])
{
	struct sockaddr_in addr;
	int u = socket(AF_INET, SOCK_DGRAM, 0);
	int t = tcp_listener(ST_PORT + 4, false);

	/* Exit with a bound UDP socket and a TCP listener */

	fill_addr(&addr, ST_LOOPBACK, ST_PORT + 4);
	if (u < 0 || t < 0 || bind(u, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("  child: failed to open the sockets, errno %d\n", errno);
	}

	sem_post(&g_childsem);
	return 0;
}

static void *blocked_udp_thread(void *arg)
{
	struct sockaddr_in addr;
	int u = socket(AF_INET, SOCK_DGRAM, 0);
	char c;

	fill_addr(&addr, ST_LOOPBACK, ST_PORT + 5);
	if (u < 0 || bind(u, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("  child: failed to open the socket, errno %d\n", errno);
	}

	sem_post(&g_childsem);
	recv(u, &c, 1, 0);
	return NULL;
}

static int blocked_child(int argc, char *argv[])
{
	pthread_t tid;

	/* Exit while a thread is blocked on a socket */

	if (pthread_create(&tid, NULL, blocked_udp_thread, NULL) != 0) {
		printf("  child: failed to create the thread\n");
		sem_post(&g_childsem);
	}

	usleep(300 * 1000);
	return 0;
}

static int idle_child(int argc, char *argv[])
{
	/* The sockets of the parent are inherited, and released at exit */

	sem_post(&g_childsem);
	return 0;
}

static void test_task_exit(void)
{
	struct sockaddr_in addr;
	int pid;
	int a;
	int b;
	int u;
	int t;

	sem_init(&g_childsem, 0, 0);

	/* The sockets of a task are closed when the task exits */

	pid = task_create("st_leak", 100, 4096, leaking_child, NULL);
	CHECK(pid > 0);
	if (pid > 0) {
		sem_wait(&g_childsem);
		usleep(300 * 1000);
	}

	u = socket(AF_INET, SOCK_DGRAM, 0);
	fill_addr(&addr, ST_LOOPBACK, ST_PORT + 4);
	CHECK(u >= 0 && bind(u, (struct sockaddr *)&addr, sizeof(addr)) == 0);
	t = tcp_listener(ST_PORT + 4, false);
	CHECK(t >= 0);
	close(u);
	close(t);

	/* Also the socket that a thread of the task is blocked on */

	pid = task_create("st_block", 100, 4096, blocked_child, NULL);
	CHECK(pid > 0);
	if (pid > 0) {
		sem_wait(&g_childsem);
		usleep(800 * 1000);
	}

	u = socket(AF_INET, SOCK_DGRAM, 0);
	fill_addr(&addr, ST_LOOPBACK, ST_PORT + 5);
	CHECK(u >= 0 && bind(u, (struct sockaddr *)&addr, sizeof(addr)) == 0);
	close(u);

	/* A socket that a child task inherited stays open in the parent */

	CHECK(tcp_pair(&a, &b) == 0);
	if (a >= 0) {
		pid = task_create("st_idle", 100, 4096, idle_child, NULL);
		CHECK(pid > 0);
		if (pid > 0) {
			sem_wait(&g_childsem);
			usleep(300 * 1000);
		}

		CHECK(send(a, "alive", 5, 0) == 5);
		CHECK(recv(b, g_rxbuf, sizeof(g_rxbuf), 0) == 5);
		close(a);
		close(b);
	}

	sem_destroy(&g_childsem);
}

static int run_loopback(void)
{
	RUN(test_tcp_echo);
	RUN(test_udp_echo);
	RUN(test_names);
	RUN(test_read_write);
	RUN(test_select);
	RUN(test_poll);
	RUN(test_nonblock);
	RUN(test_close_during_recv);
	RUN(test_dup);
	RUN(test_descriptors);
	RUN(test_task_exit);
	return 0;
}

/****************************************************************************
 * Tests with a host
 ****************************************************************************/

static const char *g_host;
static int g_hostport;

/* TCP echo with an echo server of the host */

static void test_host_tcp(void)
{
	int fd = tcp_connect(g_host, g_hostport);

	CHECK(fd >= 0);
	if (fd < 0) {
		return;
	}

	echo_exchange(fd, 64, (int)sizeof(g_txbuf));
	CHECK(close(fd) == 0);
}

/* UDP echo with an echo server of the host */

static void test_host_udp(void)
{
	struct sockaddr_in addr;
	struct timeval tv;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	int good = 0;
	int i;

	CHECK(fd >= 0);
	if (fd < 0) {
		return;
	}

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	CHECK(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);
	fill_addr(&addr, g_host, g_hostport);

	for (i = 0; i < ST_UDP_COUNT; i++) {
		int len = 100 + i * 130;

		fill_pattern(g_txbuf, len, i);
		if (sendto(fd, g_txbuf, len, 0, (struct sockaddr *)&addr, sizeof(addr)) == len &&
			recv(fd, g_rxbuf, sizeof(g_rxbuf), 0) == len && memcmp(g_txbuf, g_rxbuf, len) == 0) {
			good++;
		}
	}

	printf("  %d of %d datagrams echoed\n", good, ST_UDP_COUNT);
	CHECK(good == ST_UDP_COUNT);
	CHECK(close(fd) == 0);
}

static int run_host(const char *host, int port)
{
	g_host = host;
	g_hostport = port;
	RUN(test_host_tcp);
	RUN(test_host_udp);
	return 0;
}

/* A TCP echo server for one connection of the host */

static int run_server(int port)
{
	struct sockaddr_in addr;
	int total = 0;
	int on = 1;
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	int fd;
	int ret;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (lfd < 0 || setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ||
		bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0 || listen(lfd, 1) < 0) {
		printf("[net_selftest] server: failed to listen on %d, errno %d\n", port, errno);
		if (lfd >= 0) {
			close(lfd);
		}
		g_fail++;
		return -1;
	}

	printf("[net_selftest] server: listening on %d\n", port);
	fd = accept(lfd, NULL, NULL);
	close(lfd);
	if (fd < 0) {
		printf("[net_selftest] server: accept failed, errno %d\n", errno);
		g_fail++;
		return -1;
	}

	while ((ret = recv(fd, g_rxbuf, sizeof(g_rxbuf), 0)) > 0) {
		if (send_all(fd, g_rxbuf, ret) != ret) {
			g_fail++;
			break;
		}
		total += ret;
	}

	close(fd);
	printf("[net_selftest] server: echoed %d bytes\n", total);
	g_pass++;
	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int net_selftest_main(int argc, char *argv[])
#endif
{
	g_pass = 0;
	g_fail = 0;

	if (argc == 1) {
		run_loopback();
	} else if (argc == 4 && strcmp(argv[1], "host") == 0) {
		run_host(argv[2], atoi(argv[3]));
	} else if (argc == 3 && strcmp(argv[1], "server") == 0) {
		run_server(atoi(argv[2]));
	} else {
		printf("Usage: net_selftest                    tests over the loopback interface\n");
		printf("       net_selftest host <ip> <port>   TCP and UDP echo with an echo server\n");
		printf("       net_selftest server <port>      TCP echo server for one connection\n");
		return -1;
	}

	printf("[net_selftest] RESULT: %s (checks passed %d, failed %d)\n", g_fail == 0 ? "PASS" : "FAIL", g_pass, g_fail);
	return g_fail == 0 ? 0 : 1;
}
