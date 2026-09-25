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

#include <tinyara/config.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

#define PERF_SS_SLOT              5
#define PERF_SS_DATA_SIZE         4000
#define PERF_SS_MAX_DATA_SIZE     8192

enum perf_ss_mode {
	PERF_SS_READ,
	PERF_SS_WRITE,
	PERF_SS_ERASE,
	PERF_SS_COMPOSITE,
};

struct perf_ss_stats {
	unsigned int count;
	unsigned long long total_us;
	unsigned long long min_us;
	unsigned long long max_us;
};

static unsigned char g_write_data[PERF_SS_DATA_SIZE];

static void perf_ss_print_usage(const char *name)
{
	printf("Usage: %s <read|write|erase|composite|all> <count>\n", name);
	printf("  read       measure sl_read_storage\n");
	printf("  write      measure sl_write_storage\n");
	printf("  erase      measure sl_delete_storage\n");
	printf("  composite  measure write -> read -> erase\n");
	printf("  all        run all four tests\n");
}

static int perf_ss_parse_count(const char *text, unsigned int *count)
{
	char *end = NULL;
	unsigned long value;

	if (!text || !count || text[0] == '\0') {
		return -1;
	}

	value = strtoul(text, &end, 10);
	if (errno == ERANGE || end == text || *end != '\0' || value == 0 || value > UINT_MAX) {
		return -1;
	}

	*count = (unsigned int)value;
	return 0;
}

static unsigned long long perf_ss_elapsed_us(const struct timespec *start, const struct timespec *end)
{
	time_t sec = end->tv_sec - start->tv_sec;
	long nsec = end->tv_nsec - start->tv_nsec;

	if (nsec < 0) {
		sec--;
		nsec += 1000000000L;
	}

	return (unsigned long long)sec * 1000000ULL + (unsigned long long)nsec / 1000ULL;
}

static void perf_ss_stats_init(struct perf_ss_stats *stats)
{
	memset(stats, 0, sizeof(*stats));
}

static void perf_ss_stats_add(struct perf_ss_stats *stats, unsigned long long elapsed_us)
{
	if (stats->count == 0 || elapsed_us < stats->min_us) {
		stats->min_us = elapsed_us;
	}
	if (stats->count == 0 || elapsed_us > stats->max_us) {
		stats->max_us = elapsed_us;
	}
	stats->count++;
	stats->total_us += elapsed_us;
}

static void perf_ss_print_stats(const char *name, const struct perf_ss_stats *stats)
{
	unsigned long long average_us = 0;
	unsigned long long average_milli_us = 0;

	if (stats->count > 0) {
		average_us = stats->total_us / stats->count;
		average_milli_us = (stats->total_us % stats->count) * 1000ULL / stats->count;
	}

	printf("[perf_ss] %-9s count=%u total=%llu us average=%llu.%03llu us " "min=%llu us max=%llu us\n", name, stats->count, stats->total_us, average_us, average_milli_us, stats->min_us, stats->max_us);
}

static void perf_ss_init_data(void)
{
	unsigned int i;

	for (i = 0; i < PERF_SS_DATA_SIZE; i++) {
		g_write_data[i] = (unsigned char)(0xA5U ^ (i & 0xFFU));
	}
}

static int perf_ss_delete_slot(sl_ctx handle)
{
	int ret = sl_delete_storage(handle, PERF_SS_SLOT);

	if (ret == SECLINK_OK || ret == SECLINK_EMPTY_SLOT) {
		return 0;
	}

	printf("[perf_ss] delete slot %d failed: %d (%s)\n", PERF_SS_SLOT, ret, sl_strerror(ret));
	return -1;
}

static int perf_ss_write_slot(sl_ctx handle)
{
	hal_data input = HAL_DATA_INITIALIZER;

	input.data = g_write_data;
	input.data_len = PERF_SS_DATA_SIZE;
	return sl_write_storage(handle, PERF_SS_SLOT, &input);
}

static int perf_ss_prepare_read_buffer(hal_data *output)
{
	output->data = (unsigned char *)malloc(PERF_SS_MAX_DATA_SIZE);
	if (!output->data) {
		printf("[perf_ss] output buffer allocation failed\n");
		return -1;
	}

	output->data_len = PERF_SS_MAX_DATA_SIZE;
	output->priv = NULL;
	output->priv_len = 0;
	return 0;
}

static void perf_ss_free_read_buffer(hal_data *output)
{
	if (output->data) {
		free(output->data);
		output->data = NULL;
	}
	output->data_len = 0;
}

static int perf_ss_measure_read(sl_ctx handle, unsigned int repeat, struct perf_ss_stats *stats)
{
	hal_data output = HAL_DATA_INITIALIZER;
	unsigned int i;
	int ret;

	if (perf_ss_delete_slot(handle) < 0 || perf_ss_write_slot(handle) != SECLINK_OK) {
		printf("[perf_ss] read benchmark setup failed\n");
		perf_ss_delete_slot(handle);
		return -1;
	}
	if (perf_ss_prepare_read_buffer(&output) < 0) {
		perf_ss_delete_slot(handle);
		return -1;
	}

	for (i = 0; i < repeat; i++) {
		struct timespec start;
		struct timespec end;

		if (clock_gettime(CLOCK_REALTIME, &start) < 0) {
			printf("[perf_ss] read start time failed: %d\n", errno);
			ret = -1;
			goto out;
		}
		output.data_len = PERF_SS_MAX_DATA_SIZE;
		ret = sl_read_storage(handle, PERF_SS_SLOT, &output);
		if (clock_gettime(CLOCK_REALTIME, &end) < 0) {
			printf("[perf_ss] read end time failed: %d\n", errno);
			ret = -1;
			goto out;
		}
		if (ret != SECLINK_OK) {
			printf("[perf_ss] read iteration %u failed: %d (%s)\n", i + 1, ret, sl_strerror(ret));
			ret = -1;
			goto out;
		}
		if (output.data_len != PERF_SS_DATA_SIZE ||
			memcmp(output.data, g_write_data, PERF_SS_DATA_SIZE) != 0) {
			printf("[perf_ss] read iteration %u data mismatch\n", i + 1);
			ret = -1;
			goto out;
		}
		perf_ss_stats_add(stats, perf_ss_elapsed_us(&start, &end));
	}

	ret = 0;
out:
	perf_ss_free_read_buffer(&output);
	if (perf_ss_delete_slot(handle) < 0) {
		ret = -1;
	}
	return ret;
}

static int perf_ss_measure_write(sl_ctx handle, unsigned int repeat, struct perf_ss_stats *stats)
{
	hal_data input = HAL_DATA_INITIALIZER;
	unsigned int i;
	int ret = 0;

	input.data = g_write_data;
	input.data_len = PERF_SS_DATA_SIZE;
	for (i = 0; i < repeat; i++) {
		struct timespec start;
		struct timespec end;

		if (perf_ss_delete_slot(handle) < 0) {
			ret = -1;
			break;
		}
		if (clock_gettime(CLOCK_REALTIME, &start) < 0) {
			printf("[perf_ss] write start time failed: %d\n", errno);
			ret = -1;
			break;
		}
		ret = sl_write_storage(handle, PERF_SS_SLOT, &input);
		if (clock_gettime(CLOCK_REALTIME, &end) < 0) {
			printf("[perf_ss] write end time failed: %d\n", errno);
			ret = -1;
			break;
		}
		if (ret != SECLINK_OK) {
			printf("[perf_ss] write iteration %u failed: %d (%s)\n", i + 1, ret, sl_strerror(ret));
			ret = -1;
			break;
		}
		perf_ss_stats_add(stats, perf_ss_elapsed_us(&start, &end));
	}

	if (perf_ss_delete_slot(handle) < 0) {
		ret = -1;
	}
	return ret == 0 ? 0 : -1;
}

static int perf_ss_measure_erase(sl_ctx handle, unsigned int repeat, struct perf_ss_stats *stats)
{
	unsigned int i;
	int ret = 0;

	for (i = 0; i < repeat; i++) {
		struct timespec start;
		struct timespec end;

		if (perf_ss_delete_slot(handle) < 0 || perf_ss_write_slot(handle) != SECLINK_OK) {
			printf("[perf_ss] erase iteration %u setup failed\n", i + 1);
			ret = -1;
			break;
		}
		if (clock_gettime(CLOCK_REALTIME, &start) < 0) {
			printf("[perf_ss] erase start time failed: %d\n", errno);
			ret = -1;
			break;
		}
		ret = sl_delete_storage(handle, PERF_SS_SLOT);
		if (clock_gettime(CLOCK_REALTIME, &end) < 0) {
			printf("[perf_ss] erase end time failed: %d\n", errno);
			ret = -1;
			break;
		}
		if (ret != SECLINK_OK) {
			printf("[perf_ss] erase iteration %u failed: %d (%s)\n", i + 1, ret, sl_strerror(ret));
			ret = -1;
			break;
		}
		perf_ss_stats_add(stats, perf_ss_elapsed_us(&start, &end));
	}

	if (perf_ss_delete_slot(handle) < 0) {
		ret = -1;
	}
	return ret == 0 ? 0 : -1;
}

static int perf_ss_measure_composite(sl_ctx handle, unsigned int repeat, struct perf_ss_stats *stats)
{
	hal_data output = HAL_DATA_INITIALIZER;
	unsigned int i;
	int ret = 0;

	if (perf_ss_prepare_read_buffer(&output) < 0) {
		return -1;
	}

	for (i = 0; i < repeat; i++) {
		struct timespec start;
		struct timespec end;
		hal_data input = HAL_DATA_INITIALIZER;

		input.data = g_write_data;
		input.data_len = PERF_SS_DATA_SIZE;
		if (perf_ss_delete_slot(handle) < 0) {
			ret = -1;
			break;
		}
		if (clock_gettime(CLOCK_REALTIME, &start) < 0) {
			printf("[perf_ss] composite start time failed: %d\n", errno);
			ret = -1;
			break;
		}
		ret = sl_write_storage(handle, PERF_SS_SLOT, &input);
		if (ret == SECLINK_OK) {
			output.data_len = PERF_SS_MAX_DATA_SIZE;
			ret = sl_read_storage(handle, PERF_SS_SLOT, &output);
		}
		if (ret == SECLINK_OK) {
			ret = sl_delete_storage(handle, PERF_SS_SLOT);
		}
		if (clock_gettime(CLOCK_REALTIME, &end) < 0) {
			printf("[perf_ss] composite end time failed: %d\n", errno);
			ret = -1;
			break;
		}
		if (ret != SECLINK_OK) {
			printf("[perf_ss] composite iteration %u failed: %d (%s)\n", i + 1, ret, sl_strerror(ret));
			ret = -1;
			break;
		}
		if (output.data_len != PERF_SS_DATA_SIZE ||
			memcmp(output.data, g_write_data, PERF_SS_DATA_SIZE) != 0) {
			printf("[perf_ss] composite iteration %u data mismatch\n", i + 1);
			ret = -1;
			break;
		}
		perf_ss_stats_add(stats, perf_ss_elapsed_us(&start, &end));
	}

	perf_ss_free_read_buffer(&output);
	if (perf_ss_delete_slot(handle) < 0) {
		ret = -1;
	}
	return ret == 0 ? 0 : -1;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ss_performance_test(int argc, char *argv[])
#endif
{
	sl_ctx handle = NULL;
	enum perf_ss_mode modes[4];
	unsigned int repeat;
	unsigned int mode_count = 0;
	unsigned int i;
	const char *name;
	int ret = 0;

	if (argc != 3 || perf_ss_parse_count(argv[2], &repeat) < 0) {
		perf_ss_print_usage(argv[0]);
		return -1;
	}

	if (sl_init(&handle) != SECLINK_OK) {
		printf("[perf_ss] sl_init failed\n");
		return -1;
	}

	printf("[perf_ss] slot %d will be reset during the benchmark\n", PERF_SS_SLOT);
	perf_ss_init_data();
	if (strcmp(argv[1], "read") == 0) {
		modes[mode_count++] = PERF_SS_READ;
	} else if (strcmp(argv[1], "write") == 0) {
		modes[mode_count++] = PERF_SS_WRITE;
	} else if (strcmp(argv[1], "erase") == 0) {
		modes[mode_count++] = PERF_SS_ERASE;
	} else if (strcmp(argv[1], "composite") == 0) {
		modes[mode_count++] = PERF_SS_COMPOSITE;
	} else if (strcmp(argv[1], "all") == 0) {
		modes[mode_count++] = PERF_SS_READ;
		modes[mode_count++] = PERF_SS_WRITE;
		modes[mode_count++] = PERF_SS_ERASE;
		modes[mode_count++] = PERF_SS_COMPOSITE;
	} else {
		perf_ss_print_usage(argv[0]);
		ret = -1;
	}

	for (i = 0; i < mode_count && ret == 0; i++) {
		struct perf_ss_stats stats;

		perf_ss_stats_init(&stats);
		switch (modes[i]) {
		case PERF_SS_READ:
			name = "read";
			ret = perf_ss_measure_read(handle, repeat, &stats);
			break;
		case PERF_SS_WRITE:
			name = "write";
			ret = perf_ss_measure_write(handle, repeat, &stats);
			break;
		case PERF_SS_ERASE:
			name = "erase";
			ret = perf_ss_measure_erase(handle, repeat, &stats);
			break;
		case PERF_SS_COMPOSITE:
			name = "composite";
			ret = perf_ss_measure_composite(handle, repeat, &stats);
			break;
		default:
			ret = -1;
			continue;
		}

		perf_ss_print_stats(name, &stats);
	}

	if (sl_deinit(handle) != SECLINK_OK) {
		printf("[perf_ss] sl_deinit failed\n");
		ret = -1;
	}
	return ret;
}
