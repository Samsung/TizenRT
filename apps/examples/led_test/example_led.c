/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * examples/led_test/example_led.c
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define LED_ON       "1"
#define LED_OFF      "0"

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int led_test_main(int argc, char *argv[])
#endif
{
	printf("=== LED demo ===\n");
	char devpath[16];
	int fd = 0;

	snprintf(devpath, 16, "/dev/gpio%d", CONFIG_EXAMPLES_LED_PIN);

	fd = open(devpath, O_RDWR | O_SYNC, 0666);
	if (fd < 0) {
		printf("ERROR: Failed to open led gpio driver : %s error:%d\n", devpath, fd);
		return -1;
	}

	printf("Check LED operation on device. LED will blink 5 times with ON-OFF period of 1 sec.\n");
	for (int i = 0 ; i < 5; i++) {
		int ret = write(fd, LED_ON, 1);
		if (ret < 0) {
			printf("ERROR: failed to write to gpio driver, %d\n", ret);
			close(fd);
			return -1;
		}
		sleep(1);
		ret = write(fd, LED_OFF, 1);
		if (ret < 0) {
			printf("ERROR: failed to write to gpio driver, %d\n", ret);
			close(fd);
			return -1;
		}
		sleep(1);
	}
	
	close(fd);
	return 0;
}
