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
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <tinyara/health_monitor.h>

/* These are example limits, not limits of the public ioctl API. */
#define HM_EXAMPLE_MAX_MS 60000
#define HM_EXAMPLE_MAX_COUNT 100000

/* The worker used by start/kick/stop is a separate task so it outlives the shell command. */
#define HM_WORKER_NAME "hm_worker"
#define HM_WORKER_MQ "/hm_example"
#define HM_WORKER_PRIORITY 100
#define HM_WORKER_STACKSIZE 4096

/* Scenario 1: two threads pass a token and kick only on their own turn. */
#define HM_SCN1_TIMEOUT_MS 1000
#define HM_SCN1_WORK_MS 100
#define HM_SCN1_ROUNDS 20

#if HM_SCN1_TIMEOUT_MS <= 3 * HM_SCN1_WORK_MS
#error "HM_SCN1_TIMEOUT_MS must exceed a full token round trip"
#endif

enum hm_cmd_e {
	HM_CMD_START,
	HM_CMD_KICK,
	HM_CMD_STOP
};

struct hm_msg_s {
	uint8_t cmd;
	uint32_t timeout;
};

struct hm_pingpong_s {
	int fd;
	sem_t ready;
	sem_t turn[2];
	volatile bool abort;
	int result[2];
	pid_t pid[2];
};

struct hm_peer_s {
	struct hm_pingpong_s *shared;
	int id;
};

struct hm_scenario_s {
	uint32_t id;
	const char *description;
	int (*run)(int fd);
};

static int hm_number(const char *text, uint32_t maximum, uint32_t *value)
{
	char *end;
	unsigned long parsed;

	if (*text < '0' || *text > '9') {
		return -1;
	}
	set_errno(0);
	parsed = strtoul(text, &end, 10);
	if (errno || *end || parsed > maximum) {
		return -1;
	}
	*value = (uint32_t)parsed;
	return 0;
}

static int hm_delay(uint32_t milliseconds)
{
	struct timespec delay;

	delay.tv_sec = milliseconds / 1000;
	delay.tv_nsec = (milliseconds % 1000) * 1000000L;
	while (nanosleep(&delay, &delay) < 0) {
		if (errno != EINTR) {
			return -1;
		}
	}
	return 0;
}

static int hm_ioctl(int fd, const char *who, const char *name, int request, unsigned long arg)
{
	int ret = ioctl(fd, request, arg);

	if (ret < 0) {
		printf("health_monitor: %s %s failed: errno=%d\n", who, name, errno);
	} else {
		printf("health_monitor: %s %s ok\n", who, name);
	}
	return ret;
}

static int hm_open(void)
{
	int fd = open(HEALTH_MONITOR_DEVPATH, O_RDWR);

	if (fd < 0) {
		printf("health_monitor: open failed: %d\n", errno);
	}
	return fd;
}

/****************************************************************************
 * start / kick / stop: one ioctl per command, issued by a persistent worker
 ****************************************************************************/

static int hm_worker_main(int argc, char *argv[])
{
	struct hm_msg_s msg;
	mqd_t mq;
	char who[24];
	int fd;
	bool done = false;
	ssize_t len;

	snprintf(who, sizeof(who), "worker(pid=%d)", (int)getpid());
	mq = mq_open(HM_WORKER_MQ, O_RDONLY);
	if (mq == (mqd_t)-1) {
		printf("health_monitor: %s mq_open failed: %d\n", who, errno);
		return EXIT_FAILURE;
	}
	fd = hm_open();
	while (fd >= 0 && !done) {
		len = mq_receive(mq, (char *)&msg, sizeof(msg), NULL);
		if (len < 0) {
			if (errno == EINTR) {
				continue;
			}
			break;
		}
		switch (msg.cmd) {
		case HM_CMD_START:
			/* A failed START leaves the worker unregistered, so it can simply end. */
			done = hm_ioctl(fd, who, "START", HMIOC_START, (unsigned long)msg.timeout) < 0;
			break;
		case HM_CMD_KICK:
			(void)hm_ioctl(fd, who, "KICK", HMIOC_KICK, 0UL);
			break;
		case HM_CMD_STOP:
			/* The worker is unregistered after STOP, so ending is safe. */
			(void)hm_ioctl(fd, who, "STOP", HMIOC_STOP, 0UL);
			done = true;
			break;
		default:
			break;
		}
	}
	if (fd >= 0) {
		close(fd);
	}
	mq_close(mq);
	mq_unlink(HM_WORKER_MQ);
	printf("health_monitor: %s exit\n", who);
	return EXIT_SUCCESS;
}

static int hm_send(mqd_t mq, enum hm_cmd_e cmd, uint32_t timeout)
{
	struct hm_msg_s msg;

	msg.cmd = (uint8_t)cmd;
	msg.timeout = timeout;
	return mq_send(mq, (const char *)&msg, sizeof(msg), 0);
}

static int hm_cmd_start(uint32_t timeout)
{
	struct mq_attr attr;
	mqd_t mq;
	int ret;

	attr.mq_maxmsg = 4;
	attr.mq_msgsize = sizeof(struct hm_msg_s);
	attr.mq_flags = 0;
	/* O_EXCL makes the existence check and creation one step, so only one start can create the worker. */
	mq = mq_open(HM_WORKER_MQ, O_WRONLY | O_CREAT | O_EXCL, 0666, &attr);
	if (mq == (mqd_t)-1) {
		if (errno == EEXIST) {
			printf("health_monitor: worker already running; use kick or stop\n");
		} else {
			printf("health_monitor: mq_open failed: %d\n", errno);
		}
		return -1;
	}
	if (hm_send(mq, HM_CMD_START, timeout) < 0) {
		printf("health_monitor: mq_send failed: %d\n", errno);
		ret = -1;
	} else if (task_create(HM_WORKER_NAME, HM_WORKER_PRIORITY, HM_WORKER_STACKSIZE, hm_worker_main, NULL) < 0) {
		printf("health_monitor: worker creation failed: %d\n", errno);
		ret = -1;
	} else {
		printf("health_monitor: worker created; kick within %" PRIu32 " ms or expect PANIC\n", timeout);
		ret = 0;
	}
	mq_close(mq);
	if (ret < 0) {
		mq_unlink(HM_WORKER_MQ);
	}
	return ret;
}

static int hm_cmd_forward(enum hm_cmd_e cmd)
{
	mqd_t mq;
	int ret;

	mq = mq_open(HM_WORKER_MQ, O_WRONLY);
	if (mq == (mqd_t)-1) {
		printf("health_monitor: worker not running; run start first\n");
		return -1;
	}
	ret = hm_send(mq, cmd, 0);
	if (ret < 0) {
		printf("health_monitor: mq_send failed: %d\n", errno);
	}
	mq_close(mq);
	return ret;
}

/****************************************************************************
 * run: START, periodic KICK, STOP in the calling thread
 ****************************************************************************/

static int hm_run(int fd, uint32_t timeout, uint32_t period, uint32_t count)
{
	uint32_t i;
	int ret = 0;

	if (ioctl(fd, HMIOC_START, (unsigned long)timeout) < 0) {
		return -1;
	}
	for (i = 0; i < count; i++) {
		if (hm_delay(period) < 0 || ioctl(fd, HMIOC_KICK, 0UL) < 0) {
			ret = -1;
			break;
		}
	}
	if (ioctl(fd, HMIOC_STOP, 0UL) < 0) {
		ret = -1;
	}
	return ret;
}

/****************************************************************************
 * scenario 1: two threads alternate turns and kick on their own turn
 ****************************************************************************/

static void *hm_peer(void *arg)
{
	struct hm_peer_s *peer = arg;
	struct hm_pingpong_s *shared = peer->shared;
	const char name = 'A' + peer->id;
	bool started;
	uint32_t round;
	int ret;

	shared->pid[peer->id] = getpid();
	started = ioctl(shared->fd, HMIOC_START, (unsigned long)HM_SCN1_TIMEOUT_MS) == 0;
	shared->result[peer->id] = started ? 0 : -1;
	sem_post(&shared->ready);
	if (!started) {
		return NULL;
	}
	for (round = 0; round < HM_SCN1_ROUNDS; round++) {
		do {
			ret = sem_wait(&shared->turn[peer->id]);
		} while (ret < 0 && errno == EINTR);
		if (ret < 0 || shared->abort || ioctl(shared->fd, HMIOC_KICK, 0UL) < 0) {
			/* Tell the partner to stop waiting for a turn that will not come. */
			shared->result[peer->id] = -1;
			shared->abort = true;
			sem_post(&shared->turn[1 - peer->id]);
			break;
		}
		printf("health_monitor: round=%" PRIu32 " thread=%c kicked\n", round + 1, name);
		hm_delay(HM_SCN1_WORK_MS);
		sem_post(&shared->turn[1 - peer->id]);
	}
	/* The thread must end monitoring itself before it exits. */
	if (ioctl(shared->fd, HMIOC_STOP, 0UL) < 0) {
		shared->result[peer->id] = -1;
	}
	return NULL;
}

static int hm_scenario_pingpong(int fd)
{
	struct hm_pingpong_s *shared;
	struct hm_peer_s peers[2];
	pthread_t threads[2];
	pthread_attr_t attr;
	uint32_t created = 0;
	uint32_t i;
	int ret = 0;
	int err;
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS >= 2
	cpu_set_t cpuset;
#endif

	shared = calloc(1, sizeof(*shared));
	if (!shared) {
		return -1;
	}
	shared->fd = fd;
	sem_init(&shared->ready, 0, 0);
	sem_init(&shared->turn[0], 0, 0);
	sem_init(&shared->turn[1], 0, 0);
	for (i = 0; i < 2; i++) {
		peers[i].shared = shared;
		peers[i].id = (int)i;
		err = pthread_attr_init(&attr);
#if defined(CONFIG_SMP) && CONFIG_SMP_NCPUS >= 2
		if (err == 0) {
			/* Put the two peers on different CPUs. */
			CPU_ZERO(&cpuset);
			CPU_SET(i, &cpuset);
			err = pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);
		}
#endif
		if (err == 0) {
			err = pthread_create(&threads[i], &attr, hm_peer, &peers[i]);
		}
		pthread_attr_destroy(&attr);
		if (err != 0) {
			printf("health_monitor: pthread setup failed: %d\n", err);
			ret = -1;
			break;
		}
		created++;
	}
	/* Start the token only after every created thread has registered. */
	for (i = 0; i < created; i++) {
		while (sem_wait(&shared->ready) < 0 && errno == EINTR) {
		}
	}
	if (created < 2 || shared->result[0] < 0 || shared->result[1] < 0) {
		/* Release any thread that started so it can stop and exit. */
		shared->abort = true;
		ret = -1;
		sem_post(&shared->turn[0]);
		sem_post(&shared->turn[1]);
	} else {
		/* A single token makes the two threads strictly alternate, A first. */
		sem_post(&shared->turn[0]);
	}
	for (i = 0; i < created; i++) {
		err = pthread_join(threads[i], NULL);
		if (err != 0) {
			/* Retain shared storage and fd while a thread may still use them. */
			printf("health_monitor: join failed: %d; resources retained\n", err);
			return -2;
		}
		if (shared->result[i] < 0) {
			ret = -1;
		}
		printf("health_monitor: thread=%c pid=%d result=%d\n", 'A' + (int)i, (int)shared->pid[i], shared->result[i]);
	}
	sem_destroy(&shared->ready);
	sem_destroy(&shared->turn[0]);
	sem_destroy(&shared->turn[1]);
	free(shared);
	if (ret == 0) {
		/* Surviving the old deadlines checks STOP before the threads exited. */
		ret = hm_delay(HM_SCN1_TIMEOUT_MS + 100);
	}
	return ret;
}

static const struct hm_scenario_s g_hm_scenarios[] = {
	{1, "two threads alternate turns (ping-pong) and kick on their own turn", hm_scenario_pingpong},
};

/****************************************************************************
 * expire: deliberate failures
 ****************************************************************************/

static int hm_expire_hang(int fd, uint32_t timeout)
{
	sem_t blocked;
	int ret;

	if (sem_init(&blocked, 0, 0) < 0) {
		return -1;
	}
	printf("health_monitor: deliberate timeout; expect Health Monitor PANIC\n");
	fflush(stdout);
	ret = ioctl(fd, HMIOC_START, (unsigned long)timeout);
	if (ret == 0) {
		/* No thread posts this semaphore. Signals must not end the test. */
		do {
			ret = sem_wait(&blocked);
		} while (ret < 0 && errno == EINTR);
		ioctl(fd, HMIOC_STOP, 0UL);
	}
	sem_destroy(&blocked);
	return -1; /* Returning at all is not a successful timeout test. */
}

static int hm_expire_exit(int fd, uint32_t timeout)
{
	printf("health_monitor: returning without STOP; expect UNEXPECTED_EXIT PANIC\n");
	fflush(stdout);
	return ioctl(fd, HMIOC_START, (unsigned long)timeout);
}

static void hm_usage(void)
{
	uint32_t i;

	printf("health_monitor start <timeout_ms>   (create worker thread; it issues START)\n"
		   "health_monitor kick                 (worker issues KICK)\n"
		   "health_monitor stop                 (worker issues STOP and exits)\n"
		   "health_monitor run <timeout_ms> <period_ms> <count>\n"
		   "health_monitor scenario <id>\n");
	for (i = 0; i < sizeof(g_hm_scenarios) / sizeof(g_hm_scenarios[0]); i++) {
		printf("    scenario %" PRIu32 ": %s\n", g_hm_scenarios[i].id, g_hm_scenarios[i].description);
	}
	printf("health_monitor expire <timeout_ms> [hang|exit]  (deliberate PANIC/reset)\n"
		   "Example limits: 1..%d ms, period < timeout, count 1..%d\n", HM_EXAMPLE_MAX_MS, HM_EXAMPLE_MAX_COUNT);
}

static int hm_finish(int fd, const char *name, int ret)
{
	if (ret != -2) {
		close(fd);
	}
	printf("health_monitor: %s %s\n", name, ret == 0 ? "completed" : "FAILED");
	return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int health_monitor_main(int argc, char *argv[])
#endif
{
	const struct hm_scenario_s *scenario = NULL;
	uint32_t timeout;
	uint32_t period;
	uint32_t count;
	uint32_t id;
	uint32_t i;
	int fd;
	int ret;

	if (argc < 2) {
		hm_usage();
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "start") == 0 && argc == 3 && hm_number(argv[2], HM_EXAMPLE_MAX_MS, &timeout) == 0 && timeout > 0) {
		return hm_cmd_start(timeout) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	if (strcmp(argv[1], "kick") == 0 && argc == 2) {
		return hm_cmd_forward(HM_CMD_KICK) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
	}
	if (strcmp(argv[1], "stop") == 0 && argc == 2) {
		return hm_cmd_forward(HM_CMD_STOP) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
	}

	if (strcmp(argv[1], "run") == 0 && argc == 5 && hm_number(argv[2], HM_EXAMPLE_MAX_MS, &timeout) == 0 && timeout > 0 &&
		hm_number(argv[3], timeout - 1, &period) == 0 && period > 0 && hm_number(argv[4], HM_EXAMPLE_MAX_COUNT, &count) == 0 && count > 0) {
		fd = hm_open();
		if (fd < 0) {
			return EXIT_FAILURE;
		}
		return hm_finish(fd, "run", hm_run(fd, timeout, period, count));
	}

	if (strcmp(argv[1], "scenario") == 0 && argc == 3 && hm_number(argv[2], UINT32_MAX, &id) == 0) {
		for (i = 0; i < sizeof(g_hm_scenarios) / sizeof(g_hm_scenarios[0]); i++) {
			if (g_hm_scenarios[i].id == id) {
				scenario = &g_hm_scenarios[i];
			}
		}
		if (scenario) {
			fd = hm_open();
			if (fd < 0) {
				return EXIT_FAILURE;
			}
			printf("health_monitor: scenario %" PRIu32 ": %s\n", id, scenario->description);
			return hm_finish(fd, "scenario", scenario->run(fd));
		}
	}

	if (strcmp(argv[1], "expire") == 0 && (argc == 3 || argc == 4) && hm_number(argv[2], HM_EXAMPLE_MAX_MS, &timeout) == 0 && timeout > 0 &&
		(argc == 3 || strcmp(argv[3], "hang") == 0 || strcmp(argv[3], "exit") == 0)) {
		fd = hm_open();
		if (fd < 0) {
			return EXIT_FAILURE;
		}
		if (argc == 4 && strcmp(argv[3], "exit") == 0) {
			ret = hm_expire_exit(fd, timeout);
		} else {
			ret = hm_expire_hang(fd, timeout);
		}
		return hm_finish(fd, "expire", ret);
	}

	hm_usage();
	return EXIT_FAILURE;
}
