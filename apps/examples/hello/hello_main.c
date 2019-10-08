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

char buf[1024] = {
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		00, 00, 00, 00, 00, 00, 00, 00,  00, 00, 00, 00, 00, 00, 00, 00,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		74, 65, 74, 65, 73, 74, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65,
		03, 00, 74, 65, 25, 01, 74, 65,  03, 00, 74, 65, 25, 01, 74, 65
};

static int make_file(char *filename)
{
	FILE *fp;
	int i;
	struct timeval erase_start, erase_end;

	fp = fopen(filename, "w+");
	if (fp == NULL) {
		printf("Cannot make file %s\n", filename);
		return -1;
	}

	gettimeofday(&erase_start, NULL);
	for (i = 0; i < 100; i++) {
		if (fwrite(&buf, sizeof(buf), 1, fp) < 0) {
			printf("Cannot write file\n");
			fclose(fp);
			return -1;
		}
	}
	gettimeofday(&erase_end, NULL);
	printf("Time taken to write 1024 * 100 bytes = %ld.%ld - %ld.%ld  \n",
				erase_end.tv_sec, erase_end.tv_usec, erase_start.tv_sec, erase_start.tv_usec);

	if (fclose(fp) != 0) {
		printf("Cannot close file %s\n", filename);
		return -1;
	}
	return OK;
}

/****************************************************************************
 * hello_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	int repeat = 5;
	int count;
	int ret;
	char path[100] = "/mnt/wr_test";

	printf("repeat = %d\n", repeat);

	printf("\n");

	for (count = 1; count != repeat; count++) {
		printf("\n%d", count);

		//write file
		if (make_file(path) < 0) {
			return -1;
		}
		//remove file
		ret = unlink(path);
		if (ret < 0) {
			printf("unlink fail [%d]\n", ret);
			return -1;
		}
	}

	printf("Hello, World!!\n");
	return 0;
}
