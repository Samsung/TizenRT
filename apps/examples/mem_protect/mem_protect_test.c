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
/// @file mem_protect_tc_main.c
/// @brief Main Function for MEM_PROTECT TestCase Example
#include <tinyara/config.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/os_api_test_drv.h>
#include <tinyara/mem_protect_test.h>

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mem_protect_tc_main(int argc, char *argv[])
#endif
{
	char ch;
	int tc_fd;
	struct mem_protecttest_arg_s obj;
	int ret;

	tc_fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	if (tc_fd < 0) {
		printf("\nFAILED to open OS API test driver %s\n", OS_API_TEST_DRVPATH);
		return ERROR;
	}

	printf("\nPress R - For Read Test\n");
	printf("Press W - For Write Test\n");

	ch = getchar();
	if (ch == 'R' || ch == 'r') {
		printf("\nPress A for app MEM_PROTECT test\n");
		printf("Press K for kernel MEM_PROTECT test\n");

		ch = getchar();

		if (ch == 'A' || ch == 'a') {
			obj.type = MEM_PROTECTTEST_APP_ADDR;
			ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
			if (ret < 0) {
				printf("ERROR: Failed to fetch test address from kernel\n");
				close(tc_fd);
				return ERROR;
			}

			uint32_t dest;

			printf("\n**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("* Test to verify protection of another app space *\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");

			printf("INFO: Read another app space: 0x%x\n", obj.addr);
			sleep(1);
			dest = *(obj.addr);

			printf("ERROR: This Task made invalid access to another app space\n");
		} else {

			printf("\nPress C - For Read Code Test\n");
			printf("Press D - For Read Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				obj.type = MEM_PROTECTTEST_KERNEL_CODE;
				ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
				if (ret < 0) {
					printf("ERROR: Failed to fetch test address from kernel\n");
					close(tc_fd);
					return ERROR;
				}

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel code space. MEM_PROTECT shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Code: 0x%x\n", obj.addr);

				sleep(1);
				dest = *(obj.addr);

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
				obj.type = MEM_PROTECTTEST_KERNEL_DATA;
				ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
				if (ret < 0) {
					printf("ERROR: Failed to fetch test address from kernel\n");
					close(tc_fd);
					return ERROR;
				}

				uint32_t dest;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to read     *\n");
				printf("* kernel data space. MEM_PROTECT shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Read Data: 0x%x\n", obj.addr);

				sleep(1);
				dest = *(obj.addr);


				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	} else if (ch == 'W' || ch == 'w') {
		printf("\nPress A for app MEM_PROTECT test\n");
		printf("Press K for kernel MEM_PROTECT test\n");

		ch = getchar();

		if (ch == 'A' || ch == 'a') {
			obj.type = MEM_PROTECTTEST_APP_ADDR;
			ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
			if (ret < 0) {
				printf("ERROR: Failed to fetch test address from kernel\n");
				close(tc_fd);
				return ERROR;
			}

			uint32_t dest = 0xdeadbeef;

			printf("\n**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("* Test to verify protection of another app space *\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("**************************************************\n");
			printf("INFO: Write another app space: 0x%x\n", obj.addr);

			sleep(1);
			*(obj.addr) = dest;

			printf("ERROR: This Task made invalid access to another app space\n");
		} else {

			printf("\nPress C - For Write Kernel Code Test\n");
			printf("Press D - For Write Kernel Data Test\n");

			ch = getchar();
			if (ch == 'C' || ch == 'c') {
				obj.type = MEM_PROTECTTEST_KERNEL_CODE;
				ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
				if (ret < 0) {
					printf("ERROR: Failed to fetch test address from kernel\n");
					close(tc_fd);
					return ERROR;
				}

				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel code     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel code space. MEM_PROTECT shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", obj.addr);

				sleep(1);
				*(obj.addr) = dest;

				printf("ERROR: User Task made invalid access to Kernel code space\n");
			} else if (ch == 'D' || ch == 'd') {
				obj.type = MEM_PROTECTTEST_KERNEL_DATA;
				ret = ioctl(tc_fd, TESTIOC_MEM_PROTECTTEST, (unsigned long)&obj);
				if (ret < 0) {
					printf("ERROR: Failed to fetch test address from kernel\n");
					close(tc_fd);
					return ERROR;
				}

				uint32_t dest = 0xdeadbeef;

				printf("\n************************************************\n");
				printf("* Test to verify protection of Kernel data     *\n");
				printf("* User Tasks should not be allowed to write    *\n");
				printf("* kernel data space. MEM_PROTECT shall raise exception *\n");
				printf("************************************************\n");
				printf("INFO: Write kernel space: 0x%x\n", obj.addr);

				sleep(1);
				*(obj.addr) = dest;

				printf("ERROR: User Task made invalid access to Kernel data space\n");
			}
		}
	}

	close(tc_fd);
	return OK;
}
