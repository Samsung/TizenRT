/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <tinyara/ascii.h>

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#include <tinyara/fs/fs.h>
#include <tinyara/fs/dirent.h>

#ifdef CONFIG_LIBC_TZDIR
#define TZDIR CONFIG_LIBC_TZDIR
#else
#define TZDIR "/usr/local/etc/zoneinfo"
#endif

#define MAX_FOLDER_SIZE      50
#define MAX_FILE_SIZE        50
#define MAX_FOLDER_NAME_LEN  10
#define MAX_FILE_NAEM_LEN    10
#define MAX_PATH_LEN         50

static char file_name[MAX_FILE_SIZE][MAX_FILE_NAEM_LEN];
static char folder_name[MAX_FOLDER_SIZE][MAX_FOLDER_NAME_LEN];

bool search_tz(char *dirpath)
{
	DIR *dirp;
	char c;
	struct dirent *entryp;
	int num = 0;
	int file_size = 0;
	int folder_size = 0;

	dirp = opendir(dirpath);
	if (!dirp) {
		printf("Failed to open directory %s\n", dirpath);
		return false;
	}
	while ((entryp = readdir(dirp)) != NULL) {
		if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
			strncpy(file_name[file_size++], entryp->d_name, MAX_FILE_NAEM_LEN);
		} else {
			strncpy(folder_name[folder_size++], entryp->d_name, MAX_FOLDER_NAME_LEN);
		}
	}

	printf("Folder : \n");
	for (int i = 1; i <= folder_size; i++) {
		printf("%d) %8s \t", i , folder_name[i - 1]);
		if ((i & 3) == 0) {
			printf("\n");
		}
	}

	printf("\nFile : \n");
	for (int i = 1; i <= file_size; i++) {
		printf("%d) %8s \t", folder_size + i, file_name[i - 1]);
		if ((i & 3) == 0) {
			printf("\n");
		}
	}

	printf("\nSelect : \n");
	while (1) {
		c = getchar();
		if ('0' <= c && c <= '9') {
			num *= 10;
			num += c - '0';
			putchar(c);
		}
		if (c == ASCII_CR) {
			break;
		}
	}

	strncat(dirpath, "/", MAX_PATH_LEN);
	if (1 <= num && num <= folder_size) {
		strncat(dirpath, folder_name[num - 1], MAX_PATH_LEN);
		search_tz(dirpath);
	} else if (folder_size < num && num <= folder_size + file_size) {
		strncat(dirpath, file_name[num - folder_size - 1], MAX_PATH_LEN);
	} else {
		printf("Out of index range.");
		return false;
	}

	closedir(dirp);
	return true;
}

int utils_tzselect(int argc, char **args)
{	
	char path[MAX_PATH_LEN];
	bool ret;
	strncpy(path, TZDIR, MAX_PATH_LEN);
	ret = search_tz(path);
	if (ret) {
		setenv("TZ", path, 0);
		printf("Set time zone : %s\n", path);
		tzset();
	} else {
		printf("The time zone is not set.\n");
	}
	return 0;
}
