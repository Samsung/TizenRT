#if 0
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
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#include <fcntl.h>

#include <errno.h>
#include <arch/board/board.h>
#include <tinyara/spi/spi.h>
#include <tinyara/fs/mtd.h>
#include <time.h>
#include <tinyara/config.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/

#define EB	(128 * 1024)

#define BUF_SIZE 2048

#define FS_PATH_MAX 16


struct timespec start_time;
struct timespec end_time;

FAR struct mtd_geometry_s geo;

static int nand_flash_read(unsigned long addr, int size)
{
	int ret;
	int offset;
	int buf_idx;
	int read_size;
	int remaining;
	FAR uint8_t *buffer;
	FAR struct mtd_dev_s *dev_mtd = NULL;

	long long total_duration_ms = 0;

	uint32_t block = 0;


	printf("Read insideh *********!!\n");

	if (addr == 0 || size <= 0) {
		printf("Invalid arguments, offset 0x%x, read size %d\n", addr, size);
		return ERROR;
	}

		printf("Received Flash Read cmd for external flash offset 0x%x, read size %d\n", addr, size);
		struct spi_dev_s *spi = up_spiinitialize(1);

		dev_mtd = w25n_initialize(spi);
				if (dev_mtd == NULL) {
			lldbg("w25n Init failed\n");
			return ERROR;
		}
		printf("w25n_initialize: done for secondary flash \n");
	/* Get the geometry of the FLASH device */
	ret = dev_mtd->ioctl(dev_mtd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t) & geo));
	if (ret < 0) {
		fdbg("ERROR: mtd->ioctl failed: %d\n", ret);
	}

	printf("Flash Geometry:\n");
	printf("  blocksize:      %lu\n", (unsigned long)geo.blocksize);
	printf("  erasesize:      %lu\n", (unsigned long)geo.erasesize);
	printf("  neraseblocks:   %lu\n", (unsigned long)geo.neraseblocks);

	ftl_nand_initialize(99, dev_mtd);
	char fs_devname[FS_PATH_MAX];
	// abhi_mtd = mtd;

	//    snprintf(fs_devname, FS_PATH_MAX, "/dev/little%dp%d", partinfo->minor, partinfo->littlefs_partno);
	// 	ret = mount(fs_devname, "/lfs", "littlefs", 0, "autoformat");
	// 	if (ret != OK) {
	// 		lldbg("ERROR: mounting '%s' failed, errno %d\n", fs_devname, get_errno());
	// 	} else {
	// 		printf("%s is mounted successfully @ %s \n", fs_devname, "/lfs");
	// 	}


	snprintf(fs_devname, FS_PATH_MAX, "/dev/mtdblock%d", 99);

	printf("mount try:\n");
	ret = mount(fs_devname, "/rom", "romfs", 0, NULL);
	if (ret != OK) {
			printf("ERROR: mounting '%s'(littlefs) failed, errno %d\n", fs_devname, get_errno());
	} else {
			printf("%s is mounted successfully @ %s \n", fs_devname, "/lfs");
	}

	buffer = (uint8_t *)malloc(BUF_SIZE);
	if (!buffer) {
		printf("Failed to allocate buffer %d\n", BUF_SIZE);
		return ERROR;
	}

	offset = 0;
	remaining = size;

	printf("Read 0x%x: ", addr);

	while (remaining > 0) {
		clock_gettime(CLOCK_REALTIME, &start_time);
		read_size = BUF_SIZE < remaining ? BUF_SIZE : remaining;
		ret = MTD_READ(dev_mtd, block, read_size, buffer);
		if (ret <= 0) {
			printf("Read Failed : %d\n", ret);
			free(buffer);
			return ERROR;
		}
		clock_gettime(CLOCK_REALTIME, &end_time);

		total_duration_ms += ((end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec)) / 1000000;
		/* Print read buffer */
		for (buf_idx = 0; buf_idx < ret; buf_idx++) {
			if ((buf_idx % 16) == 0) {
				printf("\n");
			}
			printf("0x%02x ", buffer[buf_idx]);
		}
		offset += ret;
		remaining -= ret;
		block++;
	}
	printf("\n time taken for Flash Read %lld(ms)\n", total_duration_ms);
	printf("\n");
	free(buffer);

	return OK;
}


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Hello, World nand flash!!\n");
	//return 0;

	int read_fd;
	int write_fd;
	int ret;
	int total_size;
	int copy_size;
	int read_size;
	uint32_t crc_hash = 0;

	uint32_t addr = 0x2000000;
	uint32_t size = 0x8000000;


	nand_flash_read(addr, size);

	// printf("Hello, World nand flash *********!!\n");

	#if 0
	char * mydata = malloc(sizeof(char) * EB);

	if (mydata == NULL) {
		printf("alloc failed\n");
		return ERROR;
	}	

	read_fd = open("/mnt0/romfs", O_RDONLY);

	if (read_fd < 0) {
		printf("Failed to open %s: errno %d\n", "/mnt0/romfs", get_errno());
		return ERROR;
	}

	write_fd = open("/dev/mtdblock10", O_WRONLY);
	if (write_fd < 0) {
		printf("Failed to open %s: errno %d\n", "mtdblock9", get_errno());
		return ERROR;
	}
#if 0
	for (int i = 0; i < 2048; i++) mydata[i] = 0xA;

	ret = write(write_fd, mydata, 2048);
	if (ret != 2048) {
		printf("Failed to write buffer : %d\n", ret);
		return ERROR;
	} else {
		printf("write is done\n");
	}
#endif

	do {
		ret = read(read_fd, mydata, EB);

		if (ret > 0) {
			int ret2 = write(write_fd, mydata, ret);
			if (ret2 != ret) {
				printf("write failed\n", ret2);
			}
			printf("transfered %d bytes\n", ret);
		} else {
			printf("read failed ret : %d\n", ret);
		}

	} while (ret > 0);

	close(write_fd);
#endif
	return 0;
}
#endif

/****************************************************************************
 *
 * Copyright 2016, 2021 Samsung Electronics All Rights Reserved.
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
 * apps/system/flash_test/flash_test.c
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#define MAX_CMD_LEN 128
#define MAX_ARG_LEN 64

void show_help(void) {
    printf("\n LittleFS Shell - Available Commands:\n");
    printf("========================================\n");
    printf("  Directory Operations:\n");
    printf("    mkdir <dir>      - Create a directory\n");
    printf("    rmdir <dir>      - Remove an empty directory\n");
    printf("    list <dir>       - List files in a directory\n");
    printf("\n  File Operations:\n");
    printf("    write <file> <data> - Write data to a file\n");
    printf("    read <file>         - Read contents of a file\n");
    printf("    erase <file>        - Delete a file\n");
    printf("\n  Other Commands:\n");
    printf("    help            - Show this help message\n");
    printf("    exit            - Exit the shell\n");
    printf("========================================\n");
}

void write_file(const char *path, const char *data) {
    struct stat st;
    
    // Check if the path exists but is a directory
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Error: %s is a directory, cannot write a file here.\n", path);
        return;
    }

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Error: Path does not exist - %s\n", path);
        } else {
            printf("Error opening file %s for writing (%s)\n", path, strerror(errno));
        }
        return;
    }

    if (write(fd, data, strlen(data)) < 0) {
        printf("Error writing to file %s (%s)\n", path, strerror(errno));
    } else {
        printf("File written successfully: %s\n", path);
    }
    close(fd);
}

void read_file(const char *path) {
    struct stat st;
    
    // Check if the path exists and is a directory
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Error: %s is a directory, cannot read a directory.\n", path);
        return;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Error: File not found - %s\n", path);
        } else {
            printf("Error opening file %s for reading (%s)\n", path, strerror(errno));
        }
        return;
    }

    char buffer[128];
    int ret = read(fd, buffer, sizeof(buffer) - 1);
    if (ret > 0) {
        buffer[ret] = '\0';  // Null-terminate string
        printf("Read from %s: %s\n", path, buffer);
    } else {
        printf("Error reading file %s (%s)\n", path, strerror(errno));
    }
    close(fd);
}

void delete_file(const char *path) {
    struct stat st;
    
    // Check if the file exists before attempting to delete
    if (stat(path, &st) < 0) {
        if (errno == ENOENT) {
            printf("Error: File not found - %s\n", path);
        } else {
            printf("Error accessing file %s (%s)\n", path, strerror(errno));
        }
        return;
    }

    // If it's a directory, prevent accidental deletion
    if (S_ISDIR(st.st_mode)) {
        printf("Error: %s is a directory, use `rmdir` to remove directories.\n", path);
        return;
    }

    if (unlink(path) == 0) {
        printf("File deleted successfully: %s\n", path);
    } else {
        printf("Error deleting file %s (%s)\n", path, strerror(errno));
    }
}

void create_directory(const char *path) {
    if (mkdir(path, 0777) == 0) {
        printf("Directory created: %s\n", path);
    } else if (errno == EEXIST) {
        printf("Directory already exists: %s\n", path);
    } else {
        printf("Error creating directory %s (%s)\n", path, strerror(errno));
    }
}

void delete_directory(const char *path) {
    if (rmdir(path) == 0) {
        printf("Directory deleted: %s\n", path);
    } else {
        printf("Error deleting directory %s (%s)\n", path, strerror(errno));
    }
}

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        printf("Error opening directory %s (%s)\n", path, strerror(errno));
        return;
    }
    printf("\n Listing files in: %s\n", path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

int cmd_littlefs_test(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 0;
    }

    if (strcmp(argv[1], "write") == 0 && argc == 4) {
        write_file(argv[2], argv[3]);
    } else if (strcmp(argv[1], "read") == 0 && argc == 3) {
        read_file(argv[2]);
    } else if (strcmp(argv[1], "erase") == 0 && argc == 3) {
        delete_file(argv[2]);
    } else if (strcmp(argv[1], "list") == 0 && argc == 3) {
        list_files(argv[2]);
    } else if (strcmp(argv[1], "mkdir") == 0 && argc == 3) {
        create_directory(argv[2]);
    } else if (strcmp(argv[1], "rmdir") == 0 && argc == 3) {
        delete_directory(argv[2]);
    } else if (strcmp(argv[1], "help") == 0) {
        show_help();
    } else {
        printf("Invalid command. Type `littlefs_test help` for usage.\n");
    }
    return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	return cmd_littlefs_test(argc, argv);
}
