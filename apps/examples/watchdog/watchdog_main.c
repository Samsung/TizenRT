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
 * apps/examples/watchdog/watchdog_main.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <tinyara/watchdog.h>

#ifdef CONFIG_WATCHDOG
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WATCHDOG_DEFAULT_TIMEOUT_MS 30001
#define WATCHDOG_DEFAULT_STATUS_PERIOD_MS 0
#define WATCHDOG_PAUSE_DEFAULT_MS 1000
#define WATCHDOG_RESUME_DEFAULT_MS 200
#define WATCHDOG_PAUSE_TIMEOUT_MULTIPLIER 10
#define WATCHDOG_MAX_SLEEP_MS (UINT32_MAX / 1000)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void watchdog_format_flags(uint32_t flags, char *buffer, size_t buflen)
{
	int len;
	uint32_t unknown_flags;

	if (buflen == 0) {
		return;
	}

	len = snprintf(buffer, buflen, "%s%s%s%s", (flags & WDFLAGS_ACTIVE) ? "ACTIVE|" : "", (flags & WDFLAGS_RESET) ? "RESET|" : "", (flags & WDFLAGS_CAPTURE) ? "CAPTURE|" : "", (flags & WDFLAGS_WAIT) ? "WAIT|" : "");

	if (len > 0 && buffer[len - 1] == '|') {
		buffer[len - 1] = '\0';
	}

	unknown_flags = flags & ~(WDFLAGS_ACTIVE | WDFLAGS_RESET | WDFLAGS_CAPTURE | WDFLAGS_WAIT);

	if (unknown_flags != 0) {
		len = strlen(buffer);
		snprintf(buffer + len, buflen - len, "%sUNKNOWN(0x%08x)", len > 0 ? "|" : "", unknown_flags);
	} else if (buffer[0] == '\0') {
		snprintf(buffer, buflen, "NONE");
	}
}

static void watchdog_print_status(const char *tag, const struct watchdog_status_s *status)
{
	char flag_names[96];

	watchdog_format_flags(status->flags, flag_names, sizeof(flag_names));
	printf("%s: flags=0x%08x (%s) timeout=%u timeleft=%u ms\n", tag, status->flags, flag_names, status->timeout, status->timeleft);
}

static void watchdog_help(void)
{
	printf("Usage:\n");
	printf("  watchdog <command> [arguments]\n\n");
	printf("Commands:\n");
	printf("  help\n");
	printf("      Show this help message\n\n");
	printf("  expire [timeout_ms] [status_period_ms]\n");
	printf("      Start watchdog expiration test\n");
	printf("      timeout_ms        Watchdog timeout in milliseconds\n");
	printf("                        Default: %u ms\n", (uint32_t)WATCHDOG_DEFAULT_TIMEOUT_MS);
	printf("      status_period_ms  Status print interval in milliseconds\n");
	printf("                        0 disables periodic status printing\n");
	printf("                        Default: %u ms\n\n", (uint32_t)WATCHDOG_DEFAULT_STATUS_PERIOD_MS);
	printf("  keepalive\n");
	printf("      Send keepalive signal to watchdog\n\n");
	printf("  status\n");
	printf("      Show current watchdog status\n\n");
	printf("  stop\n");
	printf("      Stop watchdog\n\n");
	printf("  pause\n");
	printf("      Pause watchdog\n\n");
	printf("  resume\n");
	printf("      Resume watchdog\n\n");
	printf("  pause_resume [pause_ms] [resume_ms]\n");
	printf("      Run one watchdog pause/resume test sequence\n");
	printf("      pause_ms          Pause duration in milliseconds\n");
	printf("                        Default: %u ms\n", (uint32_t)WATCHDOG_PAUSE_DEFAULT_MS);
	printf("      resume_ms         Resume duration in milliseconds\n");
	printf("                        Default: %u ms\n\n", (uint32_t)WATCHDOG_RESUME_DEFAULT_MS);
	printf("Examples:\n");
	printf("  watchdog expire\n");
	printf("  watchdog expire 30001 1000\n");
	printf("  watchdog pause_resume\n");
	printf("  watchdog pause_resume 1000 200\n\n");
	printf("Notes:\n");
	printf("  pause_resume timeout = pause_ms * %u\n", (uint32_t)WATCHDOG_PAUSE_TIMEOUT_MULTIPLIER);
}

static int watchdog_validate_sleep_ms(uint32_t value_ms, const char *name, bool allow_zero)
{
	if (value_ms == 0 && !allow_zero) {
		printf("watchdog: %s must be greater than 0\n", name);
		return ERROR;
	}

	if (value_ms > WATCHDOG_MAX_SLEEP_MS) {
		printf("watchdog: %s is too large: %u\n", name, value_ms);
		return ERROR;
	}

	return OK;
}

static int watchdog_parse_u32(const char *arg, uint32_t *value)
{
	char *endptr;
	unsigned long parsed;

	set_errno(0);
	parsed = strtoul(arg, &endptr, 10);
	if (get_errno() != 0 || endptr == arg || *endptr != '\0' || parsed > UINT32_MAX) {
		return ERROR;
	}

	*value = (uint32_t)parsed;
	return OK;
}

static int watchdog_open_device(void)
{
	int fd;

	fd = open(CONFIG_WATCHDOG_DEVPATH, O_RDWR);
	if (fd < 0) {
		printf("watchdog: open %s failed: %d\n", CONFIG_WATCHDOG_DEVPATH, get_errno());
	}

	return fd;
}

static int watchdog_start(int fd, uint32_t timeout_ms)
{
	int ret;

	ret = ioctl(fd, WDIOC_SETTIMEOUT, (unsigned long)timeout_ms);
	if (ret < 0) {
		printf("watchdog: WDIOC_SETTIMEOUT(%u) failed: %d\n", timeout_ms, get_errno());
		return ERROR;
	}

	ret = ioctl(fd, WDIOC_START, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_START failed: %d\n", get_errno());
		return ERROR;
	}

	return OK;
}

static int watchdog_getstatus(int fd, struct watchdog_status_s *status, const char *where)
{
	int ret;

	ret = ioctl(fd, WDIOC_GETSTATUS, (unsigned long)status);
	if (ret < 0) {
		printf("watchdog: WDIOC_GETSTATUS failed in %s: %d\n", where, get_errno());
		return ERROR;
	}

	return OK;
}

static int watchdog_stop(int fd)
{
	int ret;

	ret = ioctl(fd, WDIOC_STOP, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_STOP failed: %d\n", get_errno());
		return ERROR;
	}

	return OK;
}

static int watchdog_wait_loop(int fd, uint32_t status_period_ms)
{
	struct watchdog_status_s status;

	memset(&status, 0, sizeof(status));

	if (status_period_ms == 0) {
		for (;;) {
			usleep(1000 * 1000);
		}
	}

	if (watchdog_validate_sleep_ms(status_period_ms, "status_period_ms", false) != OK) {
		return ERROR;
	}

	for (;;) {
		usleep(status_period_ms * 1000);

		if (watchdog_getstatus(fd, &status, "wait_loop") != OK) {
			return ERROR;
		}
		watchdog_print_status("status", &status);
	}
}

static int watchdog_run_expire(uint32_t timeout_ms, uint32_t status_period_ms)
{
	int fd;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	if (watchdog_start(fd, timeout_ms) != OK) {
		close(fd);
		return ERROR;
	}

	printf("watchdog will expire after %ums.\n", timeout_ms);
	if (status_period_ms > 0) {
		printf("watchdog status will be printed every %ums.\n", status_period_ms);
	} else {
		printf("No watchdog status will be printed while waiting.\n");
	}

	if (watchdog_wait_loop(fd, status_period_ms) != OK) {
		watchdog_stop(fd);
		close(fd);
		return ERROR;
	}

	watchdog_stop(fd);
	close(fd);
	return OK;
}

static int watchdog_run_status(void)
{
	struct watchdog_status_s status;
	int fd;

	memset(&status, 0, sizeof(status));

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	if (watchdog_getstatus(fd, &status, "status") != OK) {
		close(fd);
		return ERROR;
	}

	watchdog_print_status("current status", &status);

	close(fd);
	return OK;
}

static int watchdog_run_keepalive(void)
{
	int fd;
	int ret;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	ret = ioctl(fd, WDIOC_KEEPALIVE, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_KEEPALIVE failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	printf("watchdog keepalive signal sent.\n");
	close(fd);
	return OK;
}

static int watchdog_run_pause(void)
{
	int fd;
	int ret;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	ret = ioctl(fd, WDIOC_PAUSE, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_PAUSE failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	printf("watchdog pause command sent.\n");
	close(fd);
	return OK;
}

static int watchdog_run_stop(void)
{
	int fd;
	int ret;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	ret = ioctl(fd, WDIOC_STOP, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_STOP failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	printf("watchdog stop command sent.\n");
	close(fd);
	return OK;
}

static int watchdog_run_resume(void)
{
	int fd;
	int ret;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	ret = ioctl(fd, WDIOC_RESUME, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_RESUME failed: %d\n", get_errno());
		close(fd);
		return ERROR;
	}

	printf("watchdog resume command sent.\n");
	close(fd);
	return OK;
}

static int watchdog_run_pause_resume(uint32_t pause_ms, uint32_t resume_ms)
{
	struct watchdog_status_s before_status;
	struct watchdog_status_s after_status;
	uint32_t timeout_ms;
	int fd;
	int ret;

	if (watchdog_validate_sleep_ms(pause_ms, "pause_ms", false) != OK || watchdog_validate_sleep_ms(resume_ms, "resume_ms", false) != OK) {
		return ERROR;
	}

	if (pause_ms > UINT32_MAX / WATCHDOG_PAUSE_TIMEOUT_MULTIPLIER) {
		printf("watchdog: pause_ms is too large for timeout calculation: %u\n", pause_ms);
		return ERROR;
	}

	memset(&before_status, 0, sizeof(before_status));
	memset(&after_status, 0, sizeof(after_status));
	timeout_ms = pause_ms * WATCHDOG_PAUSE_TIMEOUT_MULTIPLIER;

	fd = watchdog_open_device();
	if (fd < 0) {
		return ERROR;
	}

	if (watchdog_start(fd, timeout_ms) != OK) {
		close(fd);
		return ERROR;
	}

	printf("Pause/resume test will run with a %ums timeout.\n", timeout_ms);
	printf("Sequence: start(%u * %u) -> sleep %u ms -> status -> pause -> sleep %u ms -> resume -> sleep %u ms -> status -> stop.\n", pause_ms, (uint32_t)WATCHDOG_PAUSE_TIMEOUT_MULTIPLIER, pause_ms, pause_ms, resume_ms);

	usleep(pause_ms * 1000);

	ret = watchdog_getstatus(fd, &before_status, "pause_resume before pause");
	if (ret < 0) {
		goto errout;
	}
	watchdog_print_status("before pause", &before_status);

	ret = ioctl(fd, WDIOC_PAUSE, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_PAUSE failed: %d\n", get_errno());
		goto errout;
	}

	usleep(pause_ms * 1000);

	ret = ioctl(fd, WDIOC_RESUME, 0);
	if (ret < 0) {
		printf("watchdog: WDIOC_RESUME failed: %d\n", get_errno());
		goto errout;
	}

	usleep(resume_ms * 1000);

	ret = watchdog_getstatus(fd, &after_status, "pause_resume after resume");
	if (ret < 0) {
		goto errout;
	}
	watchdog_print_status("after resume", &after_status);

errout:
	if (watchdog_stop(fd) != OK) {
		close(fd);
		return ERROR;
	}
	close(fd);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, char *argv[])
#else
int watchdog_main(int argc, char *argv[])
#endif
{
	uint32_t timeout_ms;
	uint32_t status_period_ms;
	uint32_t pause_ms;
	uint32_t resume_ms;

	if (argc == 1) {
		watchdog_help();
		return OK;
	}

	if (strncmp(argv[1], "-h", 3) == 0 || strncmp(argv[1], "--help", 7) == 0 || strncmp(argv[1], "help", 5) == 0) {
		watchdog_help();
		return OK;
	}

	if (strncmp(argv[1], "expire", 7) == 0) {
		timeout_ms = WATCHDOG_DEFAULT_TIMEOUT_MS;
		status_period_ms = WATCHDOG_DEFAULT_STATUS_PERIOD_MS;

		if (argc >= 3 && watchdog_parse_u32(argv[2], &timeout_ms) != OK) {
			goto errout_with_help;
		}

		if (argc >= 4 && watchdog_parse_u32(argv[3], &status_period_ms) != OK) {
			goto errout_with_help;
		}

		if (argc > 4) {
			goto errout_with_help;
		}

		return watchdog_run_expire(timeout_ms, status_period_ms);
	}

	if (strncmp(argv[1], "keepalive", 10) == 0) {
		if (argc != 2) {
			goto errout_with_help;
		}

		return watchdog_run_keepalive();
	}

	if (strncmp(argv[1], "status", 7) == 0) {
		if (argc != 2) {
			goto errout_with_help;
		}

		return watchdog_run_status();
	}

	if (strncmp(argv[1], "stop", 5) == 0) {
		if (argc != 2) {
			goto errout_with_help;
		}

		return watchdog_run_stop();
	}

	if (strncmp(argv[1], "pause", 6) == 0) {
		if (argc != 2) {
			goto errout_with_help;
		}

		return watchdog_run_pause();
	}

	if (strncmp(argv[1], "resume", 7) == 0) {
		if (argc != 2) {
			goto errout_with_help;
		}

		return watchdog_run_resume();
	}

	if (strncmp(argv[1], "pause_resume", 13) == 0) {
		pause_ms = WATCHDOG_PAUSE_DEFAULT_MS;
		resume_ms = WATCHDOG_RESUME_DEFAULT_MS;

		if (argc >= 3 && watchdog_parse_u32(argv[2], &pause_ms) != OK) {
			goto errout_with_help;
		}

		if (argc >= 4 && watchdog_parse_u32(argv[3], &resume_ms) != OK) {
			goto errout_with_help;
		}

		if (argc > 4) {
			goto errout_with_help;
		}

		return watchdog_run_pause_resume(pause_ms, resume_ms);
	}

errout_with_help:
	watchdog_help();
	return ERROR;
}

#else /* CONFIG_WATCHDOG */
#error "WATCHDOG device support is not enabled (CONFIG_WATCHDOG)"
#endif /* CONFIG_WATCHDOG */
