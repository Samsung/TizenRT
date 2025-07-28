/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/log_dump/log_dump.h>
#include <tinyara/compression.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int decompress_log_data(unsigned char *comp_buf, int total_read, unsigned char **decomp_buf, size_t *decomp_size);

static void print_decompressed_data(unsigned char *decomp_buf, size_t decomp_size);

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define READ_BUFFER_SIZE	1024	/* user configurable read size */
#define COMP_DRVPATH		"/dev/compress"

/****************************************************************************
 * log_dump_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int log_dump_main(int argc, char *argv[])
#endif
{
	int ret = 1;
	int total_read = 0;
	char buf[READ_BUFFER_SIZE];
	int fd = OPEN_LOGDUMP();
	unsigned char *comp_buf = NULL;
	unsigned char *decomp_buf = NULL;
	size_t decomp_size = 0;

	if (fd < 0) {
		printf("Please ensure that procfs is mounted\n");
		return -1;
	}

	/* Log dump starts automatically from boot up.
	 * To test start, intentionally add to stop. */
	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("Failed to stop log dump, errno %d\n", get_errno());
		ret = ERROR;
		goto errout;
	}

	printf("This Log Should NOT be saved!!!\n");
	sleep(1);

	printf("\n*********************   LOG DUMP START  *********************\n");

	if (START_LOGDUMP_SAVE(fd) < 0)	{
		printf("Failed to start log dump, errno %d\n", get_errno());
		ret = ERROR;
		goto errout;
	}

	printf("Before sched lock!\n");
	sched_lock();
	for (int i = 0; i < 1000; i++) {
		printf("LOGS: %d, ", i);
	}
	sched_unlock();
	printf("After sched unlock\n");
	printf("This Log Should be saved!!!\n");
	sleep(1);

	if (STOP_LOGDUMP_SAVE(fd) < 0) {
		printf("Failed to stop log dump, errno %d\n", get_errno());
		ret = ERROR;
		goto errout;
	}

	while (ret > 0) {
		ret = READ_LOGDUMP(fd, buf, sizeof(buf));
		if (ret > 0) {
			unsigned char *new_buf = realloc(comp_buf, total_read + ret);
			if (!new_buf) {
				printf("Failed to realloc comp_buf\n");
				break;
			}
			comp_buf = new_buf;
			memcpy(comp_buf + total_read, buf, ret);
			total_read += ret;
			for (int i = 0; i < ret; i++) {
				printf("%02x ", buf[i]);
			}
		}
	}
	printf("log_dump_size = %d, and total_read = %d, Both should be equal\n", GET_LOGDUMP_SIZE(fd), total_read);

	printf("\n*********************   LOG DUMP END  ***********************\n");

	if (decompress_log_data(comp_buf, total_read, &decomp_buf, &decomp_size) < 0) {
		ret = ERROR;
		goto errout;
	}

	print_decompressed_data(decomp_buf, decomp_size);
	ret = OK;

errout:
	CLOSE_LOGDUMP(fd);
	if (comp_buf) {
		free(comp_buf);
	}
	if (decomp_buf) {
		free(decomp_buf);
	}
	return ret;
}

static int decompress_log_data(unsigned char *comp_buf, int total_read, unsigned char **decomp_buf, size_t *decomp_size)
{
	int ret_val = OK;
	int block_len;
	char len_str[5];
	size_t remaining = total_read;
	unsigned char *compressed_data = comp_buf;
	unsigned char *new_decomp = NULL;
	struct compress_header *decomp_data = NULL;

	int comp_fd = open(COMP_DRVPATH, O_RDWR);
	if (comp_fd < 0) {
		printf("Failed to open compression driver %d\n", errno);
		return ERROR;
	}

	while (remaining >= 4) {
		memset(len_str, 0, sizeof(len_str));
		memcpy(len_str, compressed_data, 4);
		block_len = atoi(len_str); // Parse 4-byte length prefix
		compressed_data += 4;
		remaining -= 4;

		if (block_len <= 0 || block_len > remaining) {
			printf("Invalid block length: %d\n", block_len);
			ret_val = ERROR;
			break;
		}

		decomp_data = (struct compress_header *)malloc(sizeof(struct compress_header));
		if (!decomp_data) {
			printf("Failed to allocate decompression header\n");
			ret_val = ERROR;
			break;
		}

		decomp_data->input_size = block_len;
		decomp_data->input_buffer = compressed_data;
		decomp_data->output_size = block_len * 10; /* Max 90% compression ratio assumption */
		decomp_data->output_buffer = (unsigned char *)malloc(decomp_data->output_size);
		if (!decomp_data->output_buffer) {
			printf("Failed to allocate output buffer\n");
			free(decomp_data);
			ret_val = ERROR;
			break;
		}

		ret_val = ioctl(comp_fd, COMPIOC_DECOMPRESS, decomp_data);
		if (ret_val < 0) {
			printf("Decompression failed: %d\n", errno);
			free(decomp_data->output_buffer);
			free(decomp_data);
			break;
		}

		new_decomp = realloc(*decomp_buf, *decomp_size + decomp_data->output_size);
		if (!new_decomp) {
			free(decomp_data->output_buffer);
			free(decomp_data);
			ret_val = ERROR;
			break;
		}
		*decomp_buf = new_decomp;
		memcpy(*decomp_buf + *decomp_size, decomp_data->output_buffer, decomp_data->output_size);
		*decomp_size += decomp_data->output_size;

		free(decomp_data->output_buffer);
		free(decomp_data);
		compressed_data += block_len;
		remaining -= block_len;
	}

	close(comp_fd);
	return ret_val;
}

static void print_decompressed_data(unsigned char *decomp_buf, size_t decomp_size)
{
	if (decomp_buf) {
		printf("\n==================Decompressed Log Data:==================\n");
		for (size_t i = 0; i < decomp_size; i++) {
			printf("%c", decomp_buf[i]);
		}
		printf("\n==========================================================\n");
	}
}
