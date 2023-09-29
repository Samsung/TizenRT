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
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <tinyara/clock.h>
#include <tinyara/os_api_test_drv.h>
#include <apps/system/dat_init.h>

#include <apps/shell/tash.h>

/****************************************************************************
 * Name:  show_usage
 *
 * Description:
 *   This function prints the usage of dat tash command
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void show_usage(void)
{
	printf("USAGE: \n");
	printf("Kindly provide 1 argument\n");
	printf("Example: \n");
#ifdef CONFIG_ARM_MPU
	printf("\'dat mpu\' to show MPU information\n");
#endif
	printf("\'dat tcb\' to show TCB information\n");
	printf("\'dat apps\' to show APPS information\n");
}

/****************************************************************************
 * Name:  dat_mpu_info
 *
 * Description:
 *   This function displays the MPU information
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_ARM_MPU
void dat_mpu_info(void)
{
	int ret = 0;
	int tc_fd = 0;

	/* Get number of binaries */

	tc_fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	if (tc_fd < 0) {
		printf("FAILED to open OS API test driver %s\n", OS_API_TEST_DRVPATH);
	}

	printf("MPU Information >>\n");

	ret = ioctl(tc_fd, TESTIOC_MPU_GET_ACTIVE_REGIONS, 0);
	if (ret	< 0) {
		printf("ERROR executing MPU ioctl!! ERROR code = %d\n", errno);
	}

	printf("Number of MPU regions: %d\n\n", ret);
	printf("End of MPU Information\n");

	close(tc_fd);
}
#endif

/****************************************************************************
 * Name:  dat_tcb_info
 *
 * Description:
 *   This function displays the TCB information of all the tasks
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/
void dat_tcb_info(void)
{
	int ret = 0;
	int tc_fd = 0;

	/* Get number of binaries */

	tc_fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	if (tc_fd < 0) {
		printf("FAILED to open OS API test driver %s\n", OS_API_TEST_DRVPATH);
	}

	printf("TCB Information >>\n");

	ret = ioctl(tc_fd, TESTIOC_GET_TCB_INFO, 0);
	if (ret	< 0) {
		printf("ERROR executing TCB ioctl!! ERROR code = %d\n", errno);
	}

	ret = ioctl(tc_fd, TESTIOC_GET_TCBLIST_INFO, 0);
	if (ret	< 0) {
		printf("ERROR executing TCB ioctl!! ERROR code = %d\n", errno);
	}

	ret = ioctl(tc_fd, TESTIOC_GET_TCBSTACK_INFO, 0);
	if (ret	< 0) {
		printf("ERROR executing TCB ioctl!! ERROR code = %d\n", errno);
	}

	printf("End of TCB Information\n");

	close(tc_fd);
}

/****************************************************************************
 * Name:  dat_app_info
 *
 * Description:
 *   This function displays the APPLICATION information
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_APP_BINARY_SEPARATION
void dat_app_info(void)
{
	int ret = 0;
	int tc_fd = 0;

	printf("Application Information >>\n");

	/* Get number of binaries */

	tc_fd = open(OS_API_TEST_DRVPATH, O_WRONLY);
	if (tc_fd < 0) {
		printf("FAILED to open OS API test driver %s\n", OS_API_TEST_DRVPATH);
		return;
	}

	ret = ioctl(tc_fd, TESTIOC_GET_APP_COUNT, 0);
	if (ret	< 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
		return;
	}

	printf("Number of binaries : %d\n\n", ret);

	/* Calculate app running time */
	printf("Application Runtime Information\n");

	clock_t ticktime;
#ifdef CONFIG_SYSTIME_TIME64
	uint64_t sec;
	uint64_t start_sec;
#else
	uint32_t sec;
	uint32_t start_sec;
#endif
	unsigned int csec;
	unsigned int start_csec;

	ticktime = clock();

	/* Convert the system up time to seconds + hundredths of second */
	sec = ticktime / CLOCKS_PER_SEC;
	csec = (100 * (ticktime % CLOCKS_PER_SEC) + (CLOCKS_PER_SEC / 2)) / CLOCKS_PER_SEC;

	/* Make sure that rounding did not force the hundredths of a second above 99 */
	if (csec > 99) {
		sec++;
		csec -= 100;
	}

	/* Retrieve app1 start timings */

	start_sec = ioctl(tc_fd, TESTIOC_GET_APP_START_TIME_SEC, 1);
	if (start_sec < 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
	}

	start_csec = ioctl(tc_fd, TESTIOC_GET_APP_START_TIME_CSEC, 1);
	if (start_csec < 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
	}

	printf("App[1] runtime :  %u.%u\n", sec - start_sec, csec - start_csec);

	/* Retrieve app2 start timings */

	start_sec = ioctl(tc_fd, TESTIOC_GET_APP_START_TIME_SEC, 2);
	if (start_sec < 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
	}

	start_csec = ioctl(tc_fd, TESTIOC_GET_APP_START_TIME_CSEC, 2);
	if (start_csec < 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
	}

	printf("App[2] runtime :  %u.%u\n", sec - start_sec, csec - start_csec);

#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	/* Display app memory address info */
	printf("\nApplication Memory Information\n");
	ret = ioctl(tc_fd, TESTIOC_GET_APP_MEMORY_ADDR, 0);
	if (ret	< 0) {
		printf("ERROR executing Application ioctl!! ERROR code = %d\n", errno);
	}
#endif
#endif
	printf("End of Application Information\n");
	close(tc_fd);
}
#endif

/****************************************************************************
 * dat_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int dat_main(int argc, char *argv[])
#endif
{
	if (argc < 2 || argc > 2) {
		show_usage();
	} else {

		if (strncmp(argv[1], "apps", 5) == 0) {
#ifdef CONFIG_APP_BINARY_SEPARATION
			/* apps : show application information (number, heap, memory) */
			dat_app_info();
#endif
		} else if (strncmp(argv[1], "mpu", 4) == 0) {
#ifdef CONFIG_ARM_MPU
			/* mpu : show number of MPU regions */
			dat_mpu_info();
#endif
		} else if (strncmp(argv[1], "tcb", 4) == 0) {
			/* tcb : show tcb information */
			dat_tcb_info();
		} else {
			printf("Invalid option!!\n");
			show_usage();
		}
	}
	return 0;
}

/****************************************************************************
 * Name:  dat_init
 *
 * Description:
 *   This function registers the dat command to TASH
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

void dat_init(void)
{
	return;
}
