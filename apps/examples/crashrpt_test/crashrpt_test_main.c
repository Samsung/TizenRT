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
 * apps/examples/crashrpt_test/crashrpt_test_main.c
 *
 * Demonstration of the crash diagnostic capture driver, /dev/crashrpt.
 *
 * The example keeps a body of diagnostic data in memory - 500 nodes of 48
 * bytes and 50 nodes of 64 bytes, each carrying a message string of up to
 * 255 bytes - and registers a reporter thread that serialises all of it to
 * a file when the system is about to assert.
 *
 * The data is built once at start-up rather than at crash time.  This is
 * deliberate: allocating several hundred blocks while the system is on its
 * way down is exactly the kind of work that should not be attempted.  It
 * also mirrors how a real product would hold diagnostic state that it
 * accumulates during normal operation.
 *
 * The reporter writes to a temporary file and renames it only after the
 * write has completed and been flushed.  A reader therefore never observes
 * a partially written report - the final name either exists and is complete,
 * or does not exist at all.
 *
 * Usage from TASH:
 *
 *   crashrpt_test init      Build the data, start and register the reporter
 *   crashrpt_test status    Show what is registered and how large a report
 *                           would be
 *   crashrpt_test crash     Raise an assert to exercise the whole path
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <tinyara/irq.h>
#include <tinyara/crashrpt.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_EXAMPLES_CRASHRPT_TEST_DIR
#define CONFIG_EXAMPLES_CRASHRPT_TEST_DIR   "/tmp"
#endif

#ifndef CONFIG_EXAMPLES_CRASHRPT_TEST_PRIORITY
#define CONFIG_EXAMPLES_CRASHRPT_TEST_PRIORITY  (SCHED_PRIORITY_MAX - 1)
#endif

#ifndef CONFIG_EXAMPLES_CRASHRPT_TEST_STACKSIZE
#define CONFIG_EXAMPLES_CRASHRPT_TEST_STACKSIZE 8192
#endif

#define CRTEST_TMP_PATH     CONFIG_EXAMPLES_CRASHRPT_TEST_DIR "/crashrpt.tmp"
#define CRTEST_FINAL_FMT    CONFIG_EXAMPLES_CRASHRPT_TEST_DIR "/crashrpt_r%d.bin"

#define CRTEST_N48          500		/* Number of 48 byte nodes */
#define CRTEST_N64          50		/* Number of 64 byte nodes */

#define CRTEST_MSG_MAX      255		/* Longest message, excluding NUL */

#define CRTEST_MAGIC        0x54505243	/* "CRPT" little endian */
#define CRTEST_VERSION      1

/* Bytes written per record: the node itself, a one byte length, then the
 * message.  A one byte length is sufficient because the message is capped
 * at 255.
 */

#define CRTEST_REC48_MAX    (sizeof(struct crtest_node48_s) + 1 + CRTEST_MSG_MAX)
#define CRTEST_REC64_MAX    (sizeof(struct crtest_node64_s) + 1 + CRTEST_MSG_MAX)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Exactly 48 bytes on a 32 bit target.  The layout is checked below. */

struct crtest_node48_s {
	uint32_t id;				/*  0 ..  3 */
	uint32_t seq;				/*  4 ..  7 */
	uint32_t timestamp;			/*  8 .. 11 */
	uint32_t value;				/* 12 .. 15 */
	uint16_t type;				/* 16 .. 17 */
	uint16_t flags;				/* 18 .. 19 */
	uint32_t reserved[6];		/* 20 .. 43 */
	char *msg;					/* 44 .. 47 */
};

/* Exactly 64 bytes on a 32 bit target. */

struct crtest_node64_s {
	uint32_t id;				/*  0 ..  3 */
	uint32_t seq;				/*  4 ..  7 */
	uint64_t timestamp;			/*  8 .. 15 */
	uint32_t value;				/* 16 .. 19 */
	uint32_t status;			/* 20 .. 23 */
	uint16_t type;				/* 24 .. 25 */
	uint16_t flags;				/* 26 .. 27 */
	uint32_t reserved[8];		/* 28 .. 59 */
	char *msg;					/* 60 .. 63 */
};

/* On-disk header. */

struct crtest_hdr_s {
	uint32_t magic;
	uint32_t version;
	uint32_t reason;
	uint32_t n48;
	uint32_t n64;
	uint32_t sz48;
	uint32_t sz64;
	uint32_t reserved;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct crtest_node48_s *g_node48;
static struct crtest_node64_s *g_node64;
static bool g_data_ready;

static int g_crashrpt_fd = -1;
static pthread_t g_reporter;
static bool g_reporter_running;

/* Sequence number so repeated reports do not overwrite one another. */

static int g_report_seq;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: crtest_verify_layout
 *
 * Description:
 *   The sizes are part of the requirement, so check them rather than assume
 *   the compiler laid the structures out as intended.  A mismatch here would
 *   silently change the file format.
 *
 ****************************************************************************/

static int crtest_verify_layout(void)
{
	if (sizeof(struct crtest_node48_s) != 48) {
		printf("crashrpt_test: node48 is %d bytes, expected 48\n", (int)sizeof(struct crtest_node48_s));
		return ERROR;
	}

	if (sizeof(struct crtest_node64_s) != 64) {
		printf("crashrpt_test: node64 is %d bytes, expected 64\n", (int)sizeof(struct crtest_node64_s));
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: crtest_make_message
 *
 * Description:
 *   Build a message of a varying but bounded length.  Lengths cycle so that
 *   the short, typical and maximum cases are all exercised, and the content
 *   encodes the node identity so a corrupted report is recognisable.
 *
 ****************************************************************************/

static char *crtest_make_message(const char *kind, uint32_t id)
{
	char *msg;
	size_t want;
	int used;

	/* Cycle 32, 96, 160, 255 so the 255 byte boundary is hit regularly. */

	static const size_t lengths[] = { 32, 96, 160, CRTEST_MSG_MAX };
	want = lengths[id & 3];

	msg = (char *)malloc(want + 1);
	if (msg == NULL) {
		return NULL;
	}

	used = snprintf(msg, want + 1, "%s node %u: diagnostic record captured during normal operation ", kind, (unsigned int)id);
	if (used < 0) {
		free(msg);
		return NULL;
	}

	/* Pad out to the intended length so the length distribution is exact. */

	if ((size_t)used < want) {
		memset(msg + used, '.', want - used);
	}
	msg[want] = '\0';

	return msg;
}

/****************************************************************************
 * Name: crtest_build_data
 *
 * Description:
 *   Populate the node arrays.  Done once, at start-up, while the system is
 *   healthy.
 *
 ****************************************************************************/

static int crtest_build_data(void)
{
	uint32_t i;

	if (g_data_ready) {
		return OK;
	}

	g_node48 = (struct crtest_node48_s *)calloc(CRTEST_N48, sizeof(struct crtest_node48_s));
	if (g_node48 == NULL) {
		printf("crashrpt_test: out of memory for 48 byte nodes\n");
		return ERROR;
	}

	g_node64 = (struct crtest_node64_s *)calloc(CRTEST_N64, sizeof(struct crtest_node64_s));
	if (g_node64 == NULL) {
		printf("crashrpt_test: out of memory for 64 byte nodes\n");
		free(g_node48);
		g_node48 = NULL;
		return ERROR;
	}

	for (i = 0; i < CRTEST_N48; i++) {
		g_node48[i].id = i;
		g_node48[i].seq = i * 3u;
		g_node48[i].timestamp = (uint32_t)time(NULL) + i;
		g_node48[i].value = 0xa5a50000u | i;
		g_node48[i].type = 48;
		g_node48[i].flags = (uint16_t)(i & 0xffu);
		g_node48[i].msg = crtest_make_message("small", i);

		if (g_node48[i].msg == NULL) {
			printf("crashrpt_test: out of memory for message %u\n", (unsigned int)i);
			return ERROR;
		}
	}

	for (i = 0; i < CRTEST_N64; i++) {
		g_node64[i].id = i;
		g_node64[i].seq = i * 7u;
		g_node64[i].timestamp = (uint64_t)time(NULL) + i;
		g_node64[i].value = 0x5a5a0000u | i;
		g_node64[i].status = i;
		g_node64[i].type = 64;
		g_node64[i].flags = (uint16_t)(i & 0xffu);
		g_node64[i].msg = crtest_make_message("large", i);

		if (g_node64[i].msg == NULL) {
			printf("crashrpt_test: out of memory for message %u\n", (unsigned int)i);
			return ERROR;
		}
	}

	g_data_ready = true;

	printf("crashrpt_test: built %d nodes of %d bytes and %d nodes of %d bytes\n",
		   CRTEST_N48, (int)sizeof(struct crtest_node48_s), CRTEST_N64, (int)sizeof(struct crtest_node64_s));

	return OK;
}

/****************************************************************************
 * Name: crtest_write_all
 *
 * Description:
 *   write() may return short.  Loop until everything is out or an error is
 *   returned that is not a signal interruption.
 *
 ****************************************************************************/

static int crtest_write_all(struct crashrpt_file_s *file, const void *buf, size_t len)
{
	const uint8_t *p = (const uint8_t *)buf;
	struct crashrpt_file_write_s warg;
	ssize_t n;

	while (len > 0) {
		warg.file = file;
		warg.buf = p;
		warg.len = len;

		n = ioctl(g_crashrpt_fd, CRASHRPTIOC_FILE_WRITE, (unsigned long)&warg);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return ERROR;
		}

		p += n;
		len -= n;
	}

	return OK;
}

/****************************************************************************
 * Name: crtest_write_record
 *
 * Description:
 *   Serialise one node followed by its message.
 *
 *   The message pointer inside the node is meaningless outside this process,
 *   so it is zeroed in the copy that goes to the file.  The message itself
 *   follows the node, preceded by a single length byte.
 *
 ****************************************************************************/

static int crtest_write_record(struct crashrpt_file_s *file, const void *node, size_t node_sz, const char *msg, size_t msg_off)
{
	uint8_t buf[CRTEST_REC64_MAX];
	size_t msg_len;
	size_t total;

	memcpy(buf, node, node_sz);

	/* Blank the pointer field so the file is reproducible. */

	memset(buf + msg_off, 0, sizeof(char *));

	msg_len = (msg != NULL) ? strnlen(msg, CRTEST_MSG_MAX) : 0;

	buf[node_sz] = (uint8_t)msg_len;
	total = node_sz + 1;

	if (msg_len > 0) {
		memcpy(buf + total, msg, msg_len);
		total += msg_len;
	}

	return crtest_write_all(file, buf, total);
}

/****************************************************************************
 * Name: crtest_write_report
 *
 * Description:
 *   Write the whole report to a temporary file, flush it, close it, and only
 *   then move it to its final name.
 *
 *   This is the part that runs while the system is on its way down, so it
 *   does no allocation and touches nothing that has not already been set up.
 *
 ****************************************************************************/

static int crtest_write_report(int reason)
{
	struct crtest_hdr_s hdr;
	char final_path[64];
	struct crashrpt_file_open_s oarg;
	struct crashrpt_file_s *file = &oarg.file;
	uint32_t i;
	int ret = ERROR;
	bool file_opened = false;

	if (!g_data_ready) {
		printf("crashrpt_test: no data to report\n");
		return ERROR;
	}

	/* Open the file through the driver using the kernel-internal file_open()
	 * which detaches the struct file from the thread's file descriptor
	 * table.  This is essential because the reporter runs in the crash path
	 * where the standard open()/write() syscalls that go through
	 * fs_getfilep() -> sched_getfiles() may fail.
	 */

	memset(&oarg, 0, sizeof(oarg));
	oarg.path = CRTEST_TMP_PATH;
	oarg.oflags = O_WRONLY | O_CREAT | O_TRUNC;
	oarg.mode = 0644;

	printf("crashrpt_test: opening %s via CRASHRPTIOC_FILE_OPEN ioctl\n", CRTEST_TMP_PATH);

	ret = ioctl(g_crashrpt_fd, CRASHRPTIOC_FILE_OPEN, (unsigned long)&oarg);
	if (ret < 0) {
		printf("crashrpt_test: cannot open %s, errno %d\n", CRTEST_TMP_PATH, errno);
		return ERROR;
	}
	file_opened = true;
	printf("crashrpt_test: file opened via driver ioctl, writing report\n");

	hdr.magic = CRTEST_MAGIC;
	hdr.version = CRTEST_VERSION;
	hdr.reason = (uint32_t)reason;
	hdr.n48 = CRTEST_N48;
	hdr.n64 = CRTEST_N64;
	hdr.sz48 = sizeof(struct crtest_node48_s);
	hdr.sz64 = sizeof(struct crtest_node64_s);
	hdr.reserved = 0;

	if (crtest_write_all(file, &hdr, sizeof(hdr)) != OK) {
		printf("crashrpt_test: header write failed\n");
		goto errout;
	}

	for (i = 0; i < CRTEST_N48; i++) {
		if (crtest_write_record(file, &g_node48[i], sizeof(struct crtest_node48_s),
								g_node48[i].msg, offsetof(struct crtest_node48_s, msg)) != OK) {
			printf("crashrpt_test: 48 byte record %u failed\n", (unsigned int)i);
			goto errout;
		}
	}

	for (i = 0; i < CRTEST_N64; i++) {
		if (crtest_write_record(file, &g_node64[i], sizeof(struct crtest_node64_s),
								g_node64[i].msg, offsetof(struct crtest_node64_s, msg)) != OK) {
			printf("crashrpt_test: 64 byte record %u failed\n", (unsigned int)i);
			goto errout;
		}
	}

	/* Get it onto the medium before the name is published.
	 * fsync is best-effort: TMPFS does not support it (returns EPERM),
	 * but the data is already in RAM so there is nothing to flush.
	 */

	if (ioctl(g_crashrpt_fd, CRASHRPTIOC_FILE_FSYNC, (unsigned long)file) < 0) {
		printf("crashrpt_test: fsync returned errno %d (continuing)\n", errno);
	}

	if (ioctl(g_crashrpt_fd, CRASHRPTIOC_FILE_CLOSE, (unsigned long)file) < 0) {
		printf("crashrpt_test: close failed, errno %d\n", errno);
		goto errout;
	}
	file_opened = false;

	/* Publish.  Until this succeeds no reader can see a partial report. */

	snprintf(final_path, sizeof(final_path), CRTEST_FINAL_FMT, g_report_seq++);

	if (rename(CRTEST_TMP_PATH, final_path) < 0) {
		printf("crashrpt_test: rename to %s failed, errno %d\n", final_path, errno);
		unlink(CRTEST_TMP_PATH);
		return ERROR;
	}

	printf("crashrpt_test: report written to %s\n", final_path);
	return OK;

errout:
	if (file_opened) {
		ioctl(g_crashrpt_fd, CRASHRPTIOC_FILE_CLOSE, (unsigned long)file);
	}

	unlink(CRTEST_TMP_PATH);
	return ret;
}

/****************************************************************************
 * Name: crtest_reporter_thread
 *
 * Description:
 *   Registers itself with the driver and then parks in fin_wait().  It is
 *   woken from the top of up_assert(), writes the report, and tells the
 *   driver it is finished so that assert handling can continue.
 *
 ****************************************************************************/

static void *crtest_reporter_thread(void *arg)
{
	int reason;

	if (ioctl(g_crashrpt_fd, CRASHRPTIOC_REGISTER, 0) < 0) {
		printf("crashrpt_test: register failed, errno %d\n", errno);
		g_reporter_running = false;
		return NULL;
	}

	printf("crashrpt_test: reporter registered and waiting\n");

	for (;;) {
		/* Parks here until the kernel wakes us.  There is no timeout - the
		 * thread is meant to wait indefinitely for a crash that may never
		 * come.
		 */

		reason = fin_wait();

		printf("crashrpt_test: woken, reason %d\n", reason);

		(void)crtest_write_report(reason);

		/* Release the asserting thread whether or not the write succeeded.
		 * Holding it up buys nothing and only delays the reboot.
		 */

		if (ioctl(g_crashrpt_fd, CRASHRPTIOC_DONE, 0) < 0) {
			printf("crashrpt_test: done notification failed, errno %d\n", errno);
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: crtest_start_reporter
 ****************************************************************************/

static int crtest_start_reporter(void)
{
	pthread_attr_t attr;
	struct sched_param param;
	int ret;

	if (g_reporter_running) {
		printf("crashrpt_test: reporter already running\n");
		return OK;
	}

	g_crashrpt_fd = open(CRASHRPT_DEVPATH, O_RDWR);
	if (g_crashrpt_fd < 0) {
		printf("crashrpt_test: cannot open %s, errno %d\n", CRASHRPT_DEVPATH, errno);
		printf("crashrpt_test: is CONFIG_CRASH_REPORT enabled?\n");
		return ERROR;
	}

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_CRASHRPT_TEST_STACKSIZE);

	/* High priority is a requirement, not a preference.  The reporter has to
	 * outrank everything else runnable or the deadline expires before it is
	 * scheduled.
	 */

	param.sched_priority = CONFIG_EXAMPLES_CRASHRPT_TEST_PRIORITY;
	pthread_attr_setschedparam(&attr, &param);

	ret = pthread_create(&g_reporter, &attr, crtest_reporter_thread, NULL);
	if (ret != 0) {
		printf("crashrpt_test: pthread_create failed, %d\n", ret);
		close(g_crashrpt_fd);
		g_crashrpt_fd = -1;
		return ERROR;
	}

	pthread_setname_np(g_reporter, "crashrpt_reporter");
	pthread_detach(g_reporter);

	g_reporter_running = true;
	return OK;
}

/****************************************************************************
 * Name: crtest_status
 ****************************************************************************/

static void crtest_status(void)
{
	size_t est;

	printf("crashrpt_test status\n");
	printf("  node48 size      : %d bytes x %d\n", (int)sizeof(struct crtest_node48_s), CRTEST_N48);
	printf("  node64 size      : %d bytes x %d\n", (int)sizeof(struct crtest_node64_s), CRTEST_N64);
	printf("  message maximum  : %d bytes\n", CRTEST_MSG_MAX);
	printf("  data built       : %s\n", g_data_ready ? "yes" : "no");
	printf("  reporter running : %s\n", g_reporter_running ? "yes" : "no");
	printf("  temporary file   : %s\n", CRTEST_TMP_PATH);

	est = sizeof(struct crtest_hdr_s) + (CRTEST_N48 * CRTEST_REC48_MAX) + (CRTEST_N64 * CRTEST_REC64_MAX);
	printf("  report at most   : %d bytes\n", (int)est);
}

/****************************************************************************
 * Name: crtest_usage
 ****************************************************************************/

static void crtest_usage(void)
{
	printf("Usage: crashrpt_test <init|status|crash>\n");
	printf("  init    build the diagnostic data and register the reporter\n");
	printf("  status  show current state and the largest possible report\n");
	printf("  crash   raise an assert to exercise the capture path\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: crashrpt_test_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int crashrpt_test_main(int argc, char *argv[])
#endif
{
	if (argc < 2) {
		crtest_usage();
		return 0;
	}

	if (crtest_verify_layout() != OK) {
		return ERROR;
	}

	if (strcmp(argv[1], "init") == 0) {
		if (crtest_build_data() != OK) {
			return ERROR;
		}

		if (crtest_start_reporter() != OK) {
			return ERROR;
		}

		printf("crashrpt_test: ready, run 'crashrpt_test crash' to exercise it\n");

	} else if (strcmp(argv[1], "status") == 0) {
		crtest_status();

	} else if (strcmp(argv[1], "crash") == 0) {
		if (!g_reporter_running) {
			printf("crashrpt_test: run 'crashrpt_test init' first\n");
			return ERROR;
		}

		printf("crashrpt_test: asserting now, the reporter should run before reboot\n");

		/* The assert reaches up_assert() through the ordinary system call
		 * path, so the hook at the top of that function sees a healthy
		 * system and can hand control to the reporter.
		 */

		ASSERT(0);

	} else {
		crtest_usage();
	}

	return 0;
}
